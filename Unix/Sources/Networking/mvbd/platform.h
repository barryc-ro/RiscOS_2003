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

#ifndef platform_multicaster_h_included
#define platform_multicaster_h_included

/* Any changes here need to be reflected in the implementations for every
 * different architecture implementation
 */

/* Initialisation */
extern void platform_init(void);
extern void platform_init_post_config(void);

/* Abstractions for the usual BSD sockets APIs */
extern bmc_status platform_create_socket(multicast_socket *, u_char /*multicast ttl*/);
extern bmc_status platform_create_icmp_socket(multicast_socket *);
extern bmc_status platform_close_socket(multicast_socket);
extern bmc_status platform_bind_socket(multicast_socket,
        u_long /*interface*/, u_short /*netorderport*/);
extern bmc_status platform_connect_socket(multicast_socket,
        u_long /*interface*/, u_short /*netorderport*/);
extern bmc_status platform_listen(multicast_socket, int /*queue length*/);
extern bmc_status platform_select(fd_set *, struct timeval * /*delay*/, int * /*ready*/);

/* Some more BSD socket API abstractions onto getsockname */
/* Read the port number assigned to a socket which has bound to port zero */
extern bmc_status platform_get_local_port(multicast_socket, u_short *);
/* Read the local IP address to which a socket has bound */
extern bmc_status platform_get_local_address(multicast_socket, struct in_addr *);

/* Try and determine the interface MTU to enable efficient (non-fragmenting) choice of
 * blksize.  Path MTU discovery could be done, but is probably overkill for this.
 * Returns -1 if unable or unwilling to determine the MTU.
 */
extern size_t platform_get_interface_mtu(struct in_addr);


/* Sending and receiving datagrams */
extern bmc_status platform_transmit(multicaster_object *);
extern bmc_status platform_transmit_to(multicaster_object *, struct sockaddr_in * /*dest*/);
extern bmc_status platform_receive_datagram(multicast_socket, void * /*buffer*/,
        int * /*in/out length*/, struct sockaddr_in * /*out sender*/,
        struct in_addr */*out local interface IP address datagram arrived on*/);
extern bmc_status platform_unicast(multicast_socket, struct sockaddr_in * /*dest*/,
        void * /*buffer*/, int /*length*/);
extern bmc_status platform_set_socket_option(multicast_socket, int, int, const void *, int);

/* Error reporting */
extern const char *platform_translate_errno(void);
extern bmc_status platform_report_error(const char * /*progname*/, bmc_status);

/* Called when system is idle */
extern void platform_idle(void);

/* Set up a struct sockaddr_in correctly inc. sin_len on systems which need it
 * Two versions: one for taking the 32-bit binary value, one for taking the struct in_addr.
 * Both return the first parameter cast to a struct sockaddr * for convenience.
 */
extern struct sockaddr *platform_init_sockaddr(struct sockaddr_in *, u_long, u_short);
extern struct sockaddr *platform_init_sockaddr_in(struct sockaddr_in *, struct in_addr,
        u_short);

/* Debugging, activity logging, warnings, error, critical errors etc.  syslog() i/f */
extern void platform_log(int, const char * /*fmt*/, ...);

/* abstract select() set manipulation */
extern void platform_fd_zero(fd_set *);
extern void platform_fd_clr(multicast_socket, fd_set *);
extern void platform_fd_set(multicast_socket, fd_set *);
extern int platform_fd_isset(multicast_socket, fd_set *);

/* Time management functions */

/* Absolute time in centiseconds */
extern platform_time platform_time_current_time(void);

/* Returns negative number if a earlier than b;
 * Returns zero if a == b;
 * Returns positive number if a later than b;
 * Function must handle wrapping correctly.
 */
extern int platform_time_cmp(platform_time, platform_time);

/* Returns the length of a file in a platform-dependent way */
extern bmc_status platform_file_length(int, size_t *);

/* Configuration file access */
extern FILE *platform_configuration_open(const char *);
extern void platform_configuration_close(FILE *);

extern void platform_reopen_log(void);
extern struct in_addr platform_find_interface_address(const char *);

#endif
