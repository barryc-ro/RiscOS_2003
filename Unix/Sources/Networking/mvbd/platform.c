/*
 *
 *  Copyright (c) 2000 by Pace Micro Technology plc. All Rights Reserved.
 *
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Pace Micro Technology
 * plc.
 *
 *
 *                PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 */

#include "multicaster.h"

/* This file implements the platform-specific parts of the multicast TFTP server.
 * There are two section separating the RISC OS platform-specific code from what
 * might be considered generic UNIX code which should be more or less the same in
 * any UNIX port.
 */


/*********
 *
 * Section 1.  More highly platform-specific code.
 *
 *********/

static fd_set open_sockets;
static int log_mask = LOG_UPTO(LOG_INFO);

static void platform_init_msghdr(struct msghdr *msg, struct sockaddr_in *sin,
        struct iovec *iov, size_t len);

bmc_status platform_create_socket(multicast_socket *ms, u_char m_ttl)
{
        *ms = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (*ms >= 0) {
                int tos = IPTOS_LOWDELAY;
                int one = 1;

                platform_debug((LOG_DEBUG, "socket() -> %d\n", *ms));

                ioctl(*ms, FIONBIO, &one);

                /*setsockopt(*ms, IPPROTO_IP, IP_RECVDSTADDR, (void *)&one, sizeof(one));*/
                setsockopt(*ms, IPPROTO_IP, IP_TOS, (void *)&tos, sizeof(tos));
                if (m_ttl != 0) {
                        setsockopt(*ms, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&m_ttl, sizeof(m_ttl));
                }

                platform_fd_set(*ms, &open_sockets);
        }
        return *ms < 0 ? bmc_SYSCALL : bmc_OK;
}

bmc_status platform_create_icmp_socket(multicast_socket *ms)
{
        *ms = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (*ms >= 0) {
                int one = 1;

                platform_debug((LOG_DEBUG, "socket(<ICMP>) -> %d\n", *ms));

                ioctl(*ms, FIONBIO, &one);

                platform_fd_set(*ms, &open_sockets);
        }
        return *ms < 0 ? bmc_SYSCALL : bmc_OK;
}

static bmc_status platform_sendmsg(multicast_socket ms, struct sockaddr_in *dest,
        struct iovec *iov, size_t len)
{
        struct msghdr msg;
        platform_init_msghdr(&msg, dest, iov, len);

        platform_debug((LOG_DEBUG,
                "%c> %s:%d (sd %d) (vector %p:%d %p:%d (%d))\n",
                IN_MULTICAST(ntohl(dest->sin_addr.s_addr)) ? '=' : '-',
                inet_ntoa(dest->sin_addr), ntohs(dest->sin_port), ms,
                iov[0].iov_base, iov[0].iov_len, msg.msg_iovlen > 1 ? iov[1].iov_base : 0,
                msg.msg_iovlen > 1 ? iov[1].iov_len : 0, msg.msg_iovlen
                ));

        return sendmsg(ms, &msg, 0) < 0 ? bmc_SYSCALL : bmc_OK;
}

bmc_status platform_listen(multicast_socket ms, int queue_length)
{
        return listen(ms, queue_length) < 0 ? bmc_SYSCALL : bmc_OK;
}

bmc_status platform_set_socket_option(multicast_socket ms, int op, int cmd, const void *data, int sz)
{
        return setsockopt(ms, op, cmd, data, sz) < 0 ? bmc_SYSCALL : bmc_OK;
}

const char *platform_translate_errno(void)
{
        return strerror(errno);
}

bmc_status platform_report_error(const char *id, bmc_status status)
{
        if (status != bmc_OK) {
                if (status == bmc_SYSCALL) {
                        platform_log(LOG_ERR, "%s: %s (%s)",
                                id, i18n_translate_status(status),
                                platform_translate_errno());
                }
                else {
                        platform_log(LOG_ERR, "%s: %s", id, i18n_translate_status(status));
                }
        }

        return status;
}

/* This routine must return either bmc_OK or bmc_SYSCALL.
 */
bmc_status platform_select(fd_set *fd, struct timeval *delay, int *ready)
{
        int max_sd = FD_SETSIZE;

        /* Find the topmost set bit.  If no bits are set, just implement
         * a delay loop and say that the select() worked and that there
         * are no sockets with data pending (because there aren't!)
         */
        while (--max_sd >= 0) {
                if (platform_fd_isset(max_sd, fd)) {
                        break;
                }
        }
        if (max_sd < 0) {
                /* We didn't have a socket - enforce the delay "manually" */
                const clock_t wait_until = clock() +
                        (clock_t) (delay->tv_sec * CLOCKS_PER_SEC +
                        delay->tv_usec / (1000000UL / CLOCKS_PER_SEC));
                while (clock() < wait_until) {
                        platform_idle();
                }
                *ready = 0;
                return bmc_OK;
        }
        else {
                struct timeval tv;
                if (delay->tv_sec == INT_MAX) {
                        delay = NULL;
                }
                else {
                        tv = *delay;
                        delay = &tv;
                }
                for (;;) {
                        const fd_set copy = *fd;
                        *ready = select(max_sd + 1, fd, NULL, NULL, delay);
                        if (*ready >= 0) {
                                return bmc_OK;
                        }
                        else {
                                if (errno == EINTR) {
                                        *fd = copy;
                                        continue;
                                }
                                else {
                                        return bmc_SYSCALL;
                                }
                        }
                }
        }
}

static bmc_status platform_get_local_name(multicast_socket ms, struct sockaddr_in *sin)
{
        int size = sizeof(*sin);
        return getsockname(ms, (struct sockaddr *) sin, &size) < 0 ? bmc_SYSCALL : bmc_OK;
}

void platform_idle(void)
{
        /* Idle */
}

static void platform_exit(void)
{
        int i;

        for (i=0; i<FD_SETSIZE; ++i) {
                if (platform_fd_isset(i, &open_sockets)) {
                        platform_debug((LOG_DEBUG, "Discarding unclosed socket (%d)\n", i));
                        socket_close(i);
                }
        }
}

void platform_reopen_log(void)
{
        closelog();
        openlog(main_get_application_id(), main_get_log_options(), 0);
}

void platform_init(void)
{
        platform_reopen_log();
        atexit(closelog);
        atexit(platform_exit);
        FD_ZERO(&open_sockets);
}

void platform_init_post_config(void)
{
        if (strcmp(configure_read_root_directory(), "/") != 0) {
                if (chdir(configure_read_root_directory()) < 0) {
                        platform_log(LOG_CRIT, "Unable to chdir to %s: %s\n",
                                configure_read_root_directory(),
                                platform_translate_errno());
                        exit(EXIT_FAILURE);
                }
        }
}

struct sockaddr *platform_init_sockaddr(struct sockaddr_in *sin, u_long address, u_short port)
{
        struct sockaddr *const sa_result = memset(sin, '\0', sizeof(*sin));

        sin->sin_family = AF_INET;
        /* BSD 4.4 only: sin->sin_len = sizeof(*sin); */
        sin->sin_port = port;
        sin->sin_addr.s_addr = address;

        return sa_result;
}

void platform_log(int level, const char *fmt, ...)
{
        if (LOG_MASK(level & LOG_PRIMASK) & log_mask) {
                va_list ap;
                va_start(ap, fmt);
                vsyslog(level, fmt, ap);
                va_end(ap);
#ifdef DEBUGLIB
                if (!(main_get_log_options() & LOG_PERROR)) {
                        va_start(ap, fmt);
                        vfprintf(stderr, fmt, ap);
                        va_end(ap);
                }
#endif
        }
}


size_t platform_get_interface_mtu(struct in_addr if_addr)
{
        /* Not worth the effort?  Perhaps a configuration file would state
         * this.  We *could* read it, but that would involve SIOCGIFCONF,
         * a search of the interface list and then a SIOCGIFMTU to read the
         * MTU.
         */
        size_t mtu = ETHERMTU;
        int ms;
        struct ifreq *ifr, *end;
        struct ifconf ifc;
        struct sockaddr_in *sin;
        char ifcbuf[1500];

        ifc.ifc_len = sizeof(ifcbuf);
        ifc.ifc_buf = ifcbuf;

        if (platform_create_socket(&ms, 0) == bmc_OK) {
                if (ioctl(ms, SIOCGIFCONF, (char *) &ifc) >= 0) {
                        end = (struct ifreq *) (ifc.ifc_buf + ifc.ifc_len);
                        ifr = ifc.ifc_req;
                        while (ifr < end) {
                                if (ifr->ifr_addr.sa_family == AF_INET) {
                                        sin = (struct sockaddr_in *) (&ifr->ifr_addr);
                                        if (sin->sin_addr.s_addr == if_addr.s_addr) {
                                                /* Found it! */
                                                if (ioctl(ms, SIOCGIFMTU, (char *) ifr) < 0) {
                                                        /* Unable to read it - but give up anyway */
                                                        mtu = ETHERMTU;
                                                }
                                                else {
                                                        platform_debug((LOG_DEBUG,
                                                                "Interface %s (%s) MTU = %d\n",
                                                                ifr->ifr_name,
                                                                inet_ntoa(if_addr),
                                                                ifr->ifr_metric));
                                                        mtu = ifr->ifr_metric;
                                                }
                                                break;
                                        }
                                }
#if 0
                                if (ifr->ifr_addr.sa_len) {
                                        /* Dodgy hack for dual Internet 4/Internet 5 compat */
                                        ifr = (struct ifreq *) ((caddr_t) ifr +
                                                ifr->ifr_addr.sa_len - sizeof(struct sockaddr));
                                }
#endif
                                ++ifr;
                        }
                }
                platform_close_socket(ms);
        }

        return mtu - sizeof(struct ip) - sizeof(struct udphdr);
}

/*********
 *
 * Section 2.  Platform-specific code - but more generic and hopefully
 *             suitable for just about any UNIX-related system.
 *
 *********/

static void platform_init_msghdr(struct msghdr *msg, struct sockaddr_in *sin,
        struct iovec *iov, size_t len)
{
        memset(msg, '\0', sizeof(*msg));
        msg->msg_name = (caddr_t) sin;
        msg->msg_namelen = sizeof(*sin);
        msg->msg_iov = iov;
        msg->msg_iovlen = len;
#ifdef HAVE_BSD44_CMSGS
        msg->msg_control = NULL;
        msg->msg_controllen = 0;
        msg->msg_flags = 0;
#else
        msg->msg_accrights = NULL;
        msg->msg_accrightslen = 0;
#endif
}

bmc_status platform_unicast(multicast_socket ms, struct sockaddr_in *dest,
        void *buffer, int length)
{
        struct iovec data[1];

        data[0].iov_len = length;
        data[0].iov_base = buffer;

        return platform_sendmsg(ms, dest, data, sizeof(data)/sizeof(*data));
}

bmc_status platform_transmit_to(multicaster_object *mo, struct sockaddr_in *dest)
{
        struct iovec iov[MSG_MAXIOVLEN];

        return platform_sendmsg(multicaster_get_socket(mo), dest, iov,
                multicaster_set_iovec(mo, iov));
}

bmc_status platform_transmit(multicaster_object *mo)
{
        return platform_transmit_to(mo, multicaster_get_target(mo));
}

bmc_status platform_bind_socket(multicast_socket ms, u_long interface, u_short netorderport)
{
        struct sockaddr_in sin;

        platform_init_sockaddr(&sin, interface, netorderport);
        return bind(ms, (struct sockaddr *)&sin, sizeof(sin)) < 0 ? bmc_SYSCALL : bmc_OK;
}

bmc_status platform_connect_socket(multicast_socket ms, u_long dest, u_short netorderport)
{
        struct sockaddr_in sin;

        platform_init_sockaddr(&sin, dest, netorderport);
        return connect(ms, (struct sockaddr *)&sin, sizeof(sin)) < 0 ? bmc_SYSCALL : bmc_OK;
}

static void platform_init_cmsg(struct msghdr *hdr, void *cmsgbuf, size_t bufsize)
{
#ifdef HAVE_BSD44_CMSGS
        hdr->msg_control = cmsgbuf;
        hdr->msg_controllen = bufsize;
#else
        hdr->msg_accrights = cmsgbuf;
        hdr->msg_accrightslen = bufsize;
#endif
}

static void platform_find_source_dest(struct sockaddr_in *sin, struct in_addr *arrival_if)
{
        static int s = multicaster_INVALID_SOCKET;
        bmc_status status;

        if (s == multicaster_INVALID_SOCKET) {
                if (platform_create_socket(&s, 0) != bmc_OK) {
                        s = multicaster_INVALID_SOCKET;
                }
                else {
                        /*platform_bind_socket(s, htonl(INADDR_ANY), 0); XXX: do we need this? */
                        shutdown(s, 2);
                }
        }
        if (s != multicaster_INVALID_SOCKET) {
                status = platform_connect_socket(s, sin->sin_addr.s_addr, sin->sin_port);
                if (status == bmc_OK) {
                        status = platform_get_local_address(s, arrival_if);
                        if (status != bmc_OK) {
                                arrival_if->s_addr = INADDR_ANY;
                        }
                }
        }
}

#ifdef HAVE_BSD44_CMSGS
static void platform_find_cmsg_dest(char *cmsgc, size_t length, struct in_addr *arrival_if)
{
        while (length >= sizeof(struct cmsghdr)) {
                struct cmsghdr *cmsg = (struct cmsghdr *) cmsgc;
                if (cmsg->cmsg_len > length) {
                        break;
                }
                if (cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVDSTADDR) {
                        memcpy(arrival_if, (cmsg + 1), sizeof(*arrival_if));
                        break;
                }
                else {
                        const int gap = (cmsg->cmsg_len + 3) & ~3;
                        length -= gap;
                        cmsgc += gap;
                }
        }
}
#endif

bmc_status platform_receive_datagram(multicast_socket ms, void *buffer,
        int *length, struct sockaddr_in *sender, struct in_addr *arrival_if)
{
        struct msghdr msg;
        char cmsgbuf[sizeof(struct cmsghdr) + sizeof(struct in_addr)];
        struct iovec data[1];

        platform_init_msghdr(&msg, sender, data, 1);
        platform_init_cmsg(&msg, &cmsgbuf, sizeof(cmsgbuf));

        data[0].iov_len = *length;
        data[0].iov_base = buffer;

        *length = recvmsg(ms, &msg, 0);
        if (*length <= 0) {
                return bmc_SYSCALL;
        }

#ifdef HAVE_BSD44_CMSGS
        if (msg.msg_flags & MSG_TRUNC) {
                return bmc_DATAGRAM_TRUNCATED;
        }
#endif

        if (arrival_if) {
                /* Find out to which interface this packet was addresses.  If the system is
                 * only BSD 4.3 style, we don't have the capability to receive this data,
                 * so we have to try and work it out by connecting a datagram socket to the
                 * source of the message and reading the local name of it.
                 */
                arrival_if->s_addr = INADDR_ANY;

#ifdef HAVE_BSD44_CMSGS
                platform_find_cmsg_dest(msg.msg_control, msg.msg_controllen, arrival_if);
                /* Suppress compiler warnings */
                (void) platform_find_source_dest;
#else
                platform_find_source_dest((struct sockaddr_in *) msg.msg_name, arrival_if);
#endif
        }

        return bmc_OK;
}

bmc_status platform_get_local_address(multicast_socket ms, struct in_addr *address)
{
        struct sockaddr_in sin;
        bmc_status status = platform_get_local_name(ms, &sin);

        if (status == bmc_OK) {
                *address = sin.sin_addr;
        }

        return status;
}

bmc_status platform_get_local_port(multicast_socket ms, unsigned short *pport)
{
        struct sockaddr_in sin;
        bmc_status status = platform_get_local_name(ms, &sin);

        if (status == bmc_OK) {
                *pport = sin.sin_port;
        }

        return status;
}

struct sockaddr *platform_init_sockaddr_in(struct sockaddr_in *sin, struct in_addr addr, u_short port)
{
        return platform_init_sockaddr(sin, addr.s_addr, port);
}

bmc_status platform_close_socket(multicast_socket ms)
{
        if (ms != multicaster_INVALID_SOCKET) {
                platform_debug((LOG_DEBUG, "Closed socket %d\n", ms));
                platform_fd_clr(ms, &open_sockets);
                (void) socket_close(ms);
        }
        return bmc_OK;
}

/* abstract select() set manipulation */
void platform_fd_zero(fd_set *fd)
{
        FD_ZERO(fd);
}

void platform_fd_clr(multicast_socket ms, fd_set *fd)
{
        FD_CLR(ms, fd);
}

void platform_fd_set(multicast_socket ms, fd_set *fd)
{
        FD_SET(ms, fd);
}

int platform_fd_isset(multicast_socket ms, fd_set *fd)
{
        return (FD_ISSET(ms, fd)) != 0;
}

/* Time management functions */
platform_time platform_time_current_time(void)
{
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 100 + tv.tv_usec / 10000;
}

int platform_time_cmp(platform_time a, platform_time b)
{
        platform_time_diff c = a - b;

        if (c < 0) {
                return -1;
        }
        else if (c > 0) {
                return +1;
        }
        else {
                return 0;
        }
}

/* Returns the length of a file in a platform-dependent way - if necessary */
bmc_status platform_file_length(int f, size_t *length)
{
        off_t result;
        result = lseek(f, 0, SEEK_END);
        if (result == -1) {
                return bmc_SYSCALL;
        }
        *length = (size_t) result;
        return bmc_OK;
}

/* Configuration file access */
FILE *platform_configuration_open(const char *name)
{
        FILE *f;

        if (name == NULL) name = CONFIG_FILE;
        f = fopen(name, "r");

        if (f == NULL) {
                platform_log(LOG_ERR, "Unable to open configuration file %s\n", name);
        }

        return f;
}

void platform_configuration_close(FILE *f)
{
        (void) fclose(f);
}

struct in_addr platform_find_interface_address(const char *ifname)
{
        static struct ifreq ifr;
        struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;
        multicast_socket ms;

        sin->sin_addr.s_addr = INADDR_ANY;

        if (platform_create_socket(&ms, 0) == bmc_OK) {
                strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
                if (ioctl(ms, SIOCGIFADDR, &ifr) < 0) {
                        sin->sin_addr.s_addr = INADDR_ANY;
                }
                platform_close_socket(ms);
        }

        return sin->sin_addr;
}
