/* Copyright (c) 1994 by Oracle Corporation.  All Rights Reserved.
 *
 * smnudp.c - OMN UDP Network Interface for Solaris
 *
 * REVISIONS
 * syen      09/06/94  added #include <sys/file.h> for solaris port
 * jolkin    04/25/94  Fixed signal handler
 * jolkin    04/20/94  Creation of Version 2.0
 */

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>

#include <sx.h>
#ifndef YSID_ORACLE
#include <ysid.h>
#endif
#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef MN_ORACLE
#include <mn.h>
#endif
#ifndef MNNIO_ORACLE
#include <mnnio.h>
#endif
#ifndef SMN_ORACLE
#include <smn.h>
#endif
#ifndef SMNUDP_ORACLE
#include <smnudp.h>
#endif

/*
 * Routine Declarations
 */
static void smnudpClose(mnnio *nio);
static sb4  smnudpSend(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len);
static sb4  smnudpRecv(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len,
		       boolean poll, ub4 timeout);
static void smnudpSig(void);

/*
 * Driver Descriptor
 */
typedef struct smnudpdesc smnudpdesc;

struct smnudpdesc
{
  int fd;                                                     /* open socket */
  int slot;           /* slot in master file descriptor set (from ssysAddFd) */
};

/*
 * UDP Address Family
 * The layout of the UDP address family is as follows:  the first four
 * bytes is the IP address (in network order); the next two bytes is
 * the port number (in network order).
 */
#define FAMILY_NAME  "UDP"
#define smnudpAddr(buf)  *((ub4 *) (buf))
#define smnudpPort(buf)  *((ub2 *) (((ub1 *) (buf)) + 4))

/*
 * smnudpOpen - Opens a UDP NIO
 */
mnnio *smnudpOpen(CONST char *name, boolean intr)
{
  mnnio *nio;
  int    fd, len;
  smnudpdesc *desc;
  char   buf[128];
  struct sockaddr_in sa;
  struct sigaction act;
  char *ppp;
  int mysz;

  char *arg;
  
  /* initialize the NIO */
  if (!(nio = (mnnio *) ysmGlbAlloc(sizeof(mnnio), "mnnio")))
    return (mnnio *) 0;
  if (!(desc = (smnudpdesc *) ysmGlbAlloc(sizeof(smnudpdesc), "smnudpdesc")))
    return (mnnio *) 0;

  nio->usrp = (void *) desc;
  nio->pktmax = 4096;
  if((ppp = getenv ("OMX_UDP_PKTMAX")) && (mysz = atoi(ppp)) )
    nio->pktmax = mysz;

  nio->send = smnudpSend;
  nio->recv = smnudpRecv;
  nio->close = smnudpClose;

  nio->flags = MNNFLG_SEND | MNNFLG_RECV | MNNFLG_CKSM;

  /* open the socket */
  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    goto error_return;
  else
    desc->fd = fd;

  /* increase receive buffer size */
  len = 50000;
  setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char *) &len, sizeof(len));

  /* set for non-blocking */
  fcntl(desc->fd, F_SETFL, O_NONBLOCK);

  /* bind the address */
  if (name)
    {
      if (smnudpPa(&nio->pa, name))
	goto error_return;

      sa.sin_family = AF_INET;
      sa.sin_addr.s_addr = INADDR_ANY;
      sa.sin_port = smnudpPort(nio->pa.addr);
    }
  else
    {
      sa.sin_family = AF_INET;
      sa.sin_addr.s_addr = INADDR_ANY;
      sa.sin_port = 0;
    }

  if (bind(fd, (struct sockaddr *) &sa, sizeof(struct sockaddr_in)) == -1)
    goto error_return;

  /* get bound address */
  if (!name)
    {
      len = sizeof(struct sockaddr_in);
      getsockname(fd, (struct sockaddr *) &sa, &len);
      memcpy(buf, "UDP:", 4);

      arg = getenv("mzd_ip_addr");
      if (arg)
	strcpy(buf+4, arg);
      else
	gethostname(buf + 4, 64);
      sprintf(buf + strlen(buf), ":%d", smnNtoh2(sa.sin_port));
      if (smnudpPa(&nio->pa, buf))
	goto error_return;
    }

  /* enable interrupts if requested */
  if (intr)
    {
      fcntl(fd, F_SETFL, FASYNC);
      fcntl(fd, F_SETOWN, getpid());

      DISCARD memset(&act, 0, sizeof(act));
      act.sa_handler = smnudpSig;
      DISCARD sigaction(SIGIO, &act, (struct sigaction *) 0);

      nio->flags |= MNNFLG_INTR;
      nio->recv =
	(sb4 (*)(mnnio *, mnnpa *, ub1 *, size_t, boolean, ub4)) 0;
    }

  /* now register the file descriptor with the OSD layer poll mechanism */
  desc->slot = ssysAddFd(ysGetOsdPtr(), desc->fd, SSYS_FD_INPUT );

  return nio;

 error_return:
  if (fd != -1)
    close(fd);
  ysmGlbFree(nio);
  return (mnnio *) 0;
}

/*
 * smnudpPa - converts a text string to a physical address
 */
sb4 smnudpPa(mnnpa *pa, CONST char *name)
{
  char ip[64];
  int  prtno;
  long host;
  struct hostent *he;

  /* parse text string */
  if (sscanf(name, "UDP:%[^:]:%d", ip, &prtno) < 2)
    return -1;

  if (isalpha(ip[0]))
    {
      /* search for address by name */
      he = gethostbyname(ip);
      if (!he)
	return -1;
      host = *((long *) he->h_addr_list[0]);
    }
  else
    {
      /* parse IP address */
      host = inet_addr(ip);
      if (host == -1)
	return -1;
    }

  /* build physical address */
  memcpy(pa->family, FAMILY_NAME, 4);
  smnudpAddr(pa->addr) = host;
  smnudpPort(pa->addr) = smnHton2(prtno);

  return 0;
}

/*
 * smnudpClose - Closes a UDP NIO
 */
static void smnudpClose(mnnio *nio)
{
  smnudpdesc *desc;

  desc = (smnudpdesc *) nio->usrp;
  ssysRemFd(ysGetOsdPtr(), desc->slot, SSYS_FD_INPUT );
  close(desc->fd);
  ysmGlbFree((dvoid *) desc);
  ysmGlbFree((dvoid *) nio);
}

/*
 * smnudpSend - Sends data over UDP
 */
static sb4 smnudpSend(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len)
{
  smnudpdesc *desc;
  int cnt;
  struct sockaddr_in sa;

  desc = (smnudpdesc *) nio->usrp;

  /* create the address */
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = smnudpAddr(pa->addr);
  sa.sin_port = smnudpPort(pa->addr);

  cnt = sendto(desc->fd, (char *) buf, len, 0,
	       (struct sockaddr *) &sa, sizeof(struct sockaddr_in));

  if (cnt == -1)
    return (errno == ENOBUFS ? MNERR_OVERFLOW : MNERR_FAILURE);
  else
    return cnt;
}

/*
 * smnudpRecv - Receives data from UDP
 */
static sb4 smnudpRecv(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len,
		      boolean poll, ub4 timeout)
{
  smnudpdesc *desc;
  int cnt, fromlen;
  struct sockaddr_in from;

  desc = (smnudpdesc *) nio->usrp;
  if (!poll)
    ysePanic(YS_EX_BADPARAM);

  fromlen = sizeof(struct sockaddr_in);
  cnt = recvfrom(desc->fd, (char *) buf, len, 0,
		 (struct sockaddr *) &from, &fromlen);
  if (cnt != -1)
    {
      /* fill in address and return */
      memcpy(pa->family, FAMILY_NAME, 4);
      smnudpAddr(pa->addr) = from.sin_addr.s_addr;
      smnudpPort(pa->addr) = from.sin_port;
      return (sb4) cnt;
    }
  else if (errno == EWOULDBLOCK)
    return MNERR_WOULDBLOCK;
  else
    return MNERR_FAILURE;
}

/*
 * smnudpSig - Received I/O signal handler
 */
static void smnudpSig(void)
{
  /* do nothing; the polling will receive all the data */
  return;
}
