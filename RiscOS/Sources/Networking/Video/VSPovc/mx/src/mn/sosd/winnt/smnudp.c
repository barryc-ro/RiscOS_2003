#ifdef RCSID
static char *RCSid = 
"$Header: smnudp.c@@\main\oms_mx3.3_dev\oms_mx3.3_nt_dev\0 \
Checked out on Thu Mar 19 1997 15:8 by jchangav \
Copyright (c) 1997 by Oracle Corporation. All Rights Reserved. \
$";
#endif /* RCSID */

/* Copyright (c) 1994 by Oracle Corporation.  All Rights Reserved.
 *
 * smnudp.c - OMN UDP Network Interface for Windows NT
 *
 * DESCRIPTION
 * Both Winsock 1 and Winsock 2 are now supported.
 * Design changes for Winsock 2:
 *  On Winsock 2, we now have only a fixed number of threads (currently one
 *  but configurable using WS2_IO_THREADS parameter) to do asynchrnous receive
 *  for all the sessions using the overlapped I/O with completion routine
 *  mechanism on Winsock 2. The procedure for doing overlapped I/O is 
 *    1. initiate a read with overlapped I/O
 *    2. do other things
 *    3. call one of the alertable wait functions (which calls the completion
 *            routine, when the i/o gets done and returns)
 *  Two things should be noted here: steps 1 and 2 should be called from the
 *  same thread and the completion routine gets called only during an alertable
 *  wait. To do this in Media Net, smnudpRecv() queues up the receive operation
 *  and then sets an event to wake up the recv thread. The alertable wait in 
 *  the recv thread wakes up when one of the following occurs:
 *    1. timeout
 *    2. when there is something in the queue
 *         dequeue and initiate the receive operation.
 *    3. when i/o (recv) is complete.
 *         wake up sysiPause
 *
 *  In the Winsock 1 implementation, there was 1 event associated with each
 *  session because WaitForMultipleObjects can wait for a maximum of 64 events
 *  the number of concurrent sessions is limited to 64. But now we have only
 *  one event for all the sessions which is signalled when there is atleast
 *  one session where a receive is complete but not yet processed by Media Net
 *  and is reset (allow sysiPause to sleep) when there are no more completed
 *  receives to be processed.
 *
 *
 * REVISIONS
 * dbrower   12/ 2/96 - bug 423404 -- clear output completely in smnudpPa.
 * dbrower   09/ 3/96  make recv be threaded, using push.  This 
 *                     pile of complexity in sysiPause interaction.
 * syen      03/04/96  add a hook to check hServerStopEvent from win32 Service
 *                     Control Manager, allowing service to be stopped from SCM.
 * alind     07/21/95  began adapting to Windows NT from SunOS
 * jolkin    04/25/94  Fixed signal handler
 * jolkin    04/20/94  Creation of Version 2.0
 */

#define INCL_WINSOCK_API_TYPEDEFS 1
#include <winsock2.h>

#include <sx.h>
#ifndef YS_ORACLE
#include <ys.h>
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
#ifndef YSLST_ORACLE
#include <yslst.h>
#endif
#ifndef YSL_ORACLE
#include <ysl.h>
#endif
#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef MNI_ORACLE
#include <mni.h>
#endif
#ifndef SSYSI_ORACLE
#include <ssysi.h>
#endif

#include <stdio.h>

externref void stop_process(ub4 err);
externref HANDLE hServerStopEvent;

/* winsock2 routines */
static LPFN_WSASOCKET      ws2Socket;
static LPFN_WSARECVFROM    ws2Recvfrom;

/* winsock1 routines */
static LPFN_SOCKET         ws1Socket;
static LPFN_RECVFROM       ws1Recvfrom;

/* common routines */
static LPFN_WSASTARTUP     wsStartup;
static LPFN_WSACLEANUP     wsCleanup;
static LPFN_IOCTLSOCKET    wsIoctlsocket;
static LPFN_SETSOCKOPT     wsSetsockopt;
static LPFN_BIND           wsBind;
static LPFN_GETSOCKNAME    wsGetsockname;
static LPFN_GETHOSTNAME    wsGethostname;
static LPFN_WSAGETLASTERROR    wsGetlasterror;
static LPFN_GETHOSTBYNAME  wsGethostbyname;
static LPFN_INET_ADDR      wsInet_addr;
static LPFN_CLOSESOCKET    wsClosesocket;
static LPFN_SENDTO         wsSendto;
static LPFN_SELECT         wsSelect;

externdef LPFN_HTONS          wsHtons;
externdef LPFN_HTONL          wsHtonl;
externdef LPFN_NTOHS          wsNtohs;
externdef LPFN_NTOHL          wsNtohl;

/*
 * Routine Declarations
 */
static void smnudpClose(mnnio *nio);
static sb4  smnudpSend(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len);

static SOCKET smnudpWSOpenSocket(void);

static const char *smnudpWSDecode( DWORD err );

static sb4 smnudpRecv(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len,
		       boolean poll, ub4 timeout);

static sb4 smnudpSel( mnnio *nio, boolean poll, ub4 timeout );

static void smnudpSelectThread( mnnio *nio );

/* the following three routines are for Winsock 2 */
static void CALLBACK CompletionRoutine(DWORD dwError, DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags);
static int QueueRecv( mnnio *nio, int op );
static VOID RecvThread();


/*
 * UDP Address Family
 * The layout of the UDP address family is as follows:  the first four
 * bytes is the IP address (in network order); the next two bytes is
 * the port number (in network order).
 */
#define FAMILY_NAME  "UDP"
#define smnudpAddr(buf)  *((ub4 *) (buf))
#define smnudpPort(buf)  *((ub2 *) (((ub1 *) (buf)) + 4))

typedef struct
{
  SOCKET    s;			/* the socket */
  CRITICAL_SECTION crit;
  HANDLE    selevt;		/* main thread needs to wake up sel. */
  HANDLE    pauseevt;		/* sel thread to wake up sysiPause */
  HANDLE    selthread;		/* the receive thread */
  boolean   rdy;
  WSAOVERLAPPED wsaoverlapped;  /* for Winsock2 */
  WSABUF    wsabuf;             /* for Winsock2 */
  int       dwError;            /* for Winsock2 */
  SOCKADDR_IN from;             /* for Winsock2 */
  int       fromLen;            /* for Winsock2 */
} smnudpcx;

static int winsock2;

#define WS2_OP_THREADS	1

typedef struct QRecvElem {
  struct QRecvElem *next;
  int   op;			/* see below */
  mnnio *nio;
} QRecvElem;

#define QOP_RECV	1
#define QOP_CLOSE	2

struct {
  QRecvElem *first;               /* first element in the list */
  QRecvElem *last;                /* last element in the list */
  QRecvElem *free;                /* the free list */
  CRITICAL_SECTION crit;
  HANDLE pauseevt;
  HANDLE recvevt;
  int nrecv;                      /* no. of receives pending to be processed */
} QRecvHeader;
/*
 * smnudpOpen - Opens a UDP NIO
 */
mnnio *smnudpOpen(CONST char *name, boolean intr)
{
  mnnio *nio = NULL;
  smnudpcx  *cx = NULL;
  SOCKET s = INVALID_SOCKET;
  int len;
  char buf[128];
  SOCKADDR_IN sa;
  boolean ws_regd = FALSE;
  DWORD thrid;
  unsigned long nonblock = 1;
  DWORD err;
  static int first = TRUE;
  char *ppp;
  int mysz;

  /* allocate the NIO */
  if (!(nio = (mnnio *)malloc(sizeof(mnnio))))
  {
    /* unable to allocate an NIO */
    return NULL;
  }

  if (!(cx = (smnudpcx *)malloc(sizeof(*cx))))
  {
    /* unable to allocate private context */
    free( (dvoid*)nio );
    return NULL;
  }

  /* initialize the NIO */
  nio->pktmax = 4096;
  if((ppp = getenv ("OMX_UDP_PKTMAX")) && (mysz = atoi(ppp)) )
    nio->pktmax = mysz;
  nio->send = smnudpSend;
  nio->recv = smnudpRecv;	/* buffered receive */
  nio->close = smnudpClose;
  nio->usrp = (dvoid*)cx;

  /* we "interrupt" for recv */
  nio->flags = MNNFLG_SEND | MNNFLG_RECV | MNNFLG_CKSM;
	
  InitializeCriticalSection( &cx->crit );
  if (!winsock2)
  {
  EnterCriticalSection( &cx->crit );
  cx->selevt = CreateEvent( 0, TRUE, FALSE, 0 );
  cx->pauseevt = CreateEvent( 0, TRUE, FALSE, 0 );
  LeaveCriticalSection( &cx->crit );
  }

  if (!winsock2)
  DISCARD ssysAddWaitObject( ysGetOsdPtr(), cx->pauseevt );
  else 
  {
    EnterCriticalSection( &QRecvHeader.crit );
    if (first)
    {
      DWORD idRecvThr;
      int i;
      for (i = 0; i < WS2_OP_THREADS; i++)
      {
        if ((CreateThread( NULL, 8096, (LPTHREAD_START_ROUTINE)RecvThread, 
                          NULL, 0, &idRecvThr )) == NULL)
        {
          LeaveCriticalSection( &QRecvHeader.crit );
    goto error_return;
  }
  }
      DISCARD ssysAddWaitObject( ysGetOsdPtr(), QRecvHeader.pauseevt );
      first = FALSE;
    }
    LeaveCriticalSection( &QRecvHeader.crit );
  }

  /* open the recv socket */

  if (winsock2)
    s = (*ws2Socket)(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
  else
    s = (*ws1Socket)(AF_INET, SOCK_DGRAM, 0);

  if (s == INVALID_SOCKET)
  {
    /* we weren't able to open the socket */
    goto error_return;
  }

  /* save a reference to the socket */
  cx->s = s;

  if (!winsock2)
  {
  /* set socket to non-blocking mode */
    if( (*wsIoctlsocket)( s, FIONBIO, (u_long FAR*)&nonblock ) )
    {
      err = WSAGetLastError();
      yslError("smnudpOpen: ioctlsocket failed: %d %s\n",
	       err, smnudpWSDecode(err)); 
      goto error_return;
    }

  }

  /* increase the receive buffer size */
  len = 50000;
  (*wsSetsockopt)(s, SOL_SOCKET, SO_RCVBUF, (char *)&len, sizeof(len));	

  /* bind the address */
  if (name)
  {
    if (smnudpPa(&nio->pa, name))
    {
      /* we weren't able to get a physical address */
      goto error_return;
    }

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

  if ((*wsBind)(s, (struct sockaddr *)&sa, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
  {
    /* we weren't able to bind the socket */
    goto error_return;
  }

  /* get bound address */
  if (!name)
  {
    len = sizeof(SOCKADDR_IN);
    (*wsGetsockname)(s, (struct sockaddr *)&sa, &len);
    memcpy(buf, "UDP:", 4);
    (*wsGethostname)(buf + 4, 64);
    sprintf(buf + strlen(buf), ":%d", smnNtoh2(sa.sin_port));
    if (smnudpPa(&nio->pa, buf))
    {
      /* we weren't able to get a physical address */
      goto error_return;
    }
  }

  if (intr)
  {
  }

  if (!winsock2)
  {
  /* only a small stack needed for rcv thread */
  cx->selthread = CreateThread(0, 4096,
				(LPTHREAD_START_ROUTINE)smnudpSelectThread,
				(LPVOID)nio, 0, &thrid );
  if( cx->selthread )
    return nio;
  }
  else
  {
    if (!(cx->wsabuf.buf = (char *)malloc(nio->pktmax)))
    {
      free(cx);
      free(nio);
      return NULL;
    }
    memset((char *)&cx->wsaoverlapped, '\0', sizeof(WSAOVERLAPPED));
    cx->dwError = 0;
    cx->rdy = FALSE;

    /* initiate a read */
    
    if (QueueRecv( nio, QOP_RECV ) == FALSE)
      goto error_return;

    return nio;
  }
  /* else thread create failed, fall into... */

error_return:

  err = (*wsGetlasterror)(); 
  yslError("smnudpOpen: error: %d %s\n", err, smnudpWSDecode(err)); 

  if (s != INVALID_SOCKET)
    (*wsClosesocket)(s);
  free(nio);
  return NULL;
}

/*
 * smnudpPa - converts a text string to a physical address
 */
sb4 smnudpPa(mnnpa *pa, CONST char *name)
{
  char ip[64];
  int prtno;
  long host;
  struct hostent *he;

  CLRSTRUCT(*pa);
  
  /* parse text string */
  if (sscanf(name, "UDP:%[^:]:%d", ip, &prtno) < 2)
    return -1;

  if (isalpha(ip[0]))
  {
    /* search for address by name */
    he = (*wsGethostbyname)(ip);
    if (!he)
    {
      yslError("smnudpPa: gethostbyname for %s failed", ip );
      return -1;
    }
    host = *((long *) he->h_addr_list[0]);
  }
  else
  {
    /* parse IP address */
    host = (*wsInet_addr)(ip);
    if (host == -1)
      return -1;
  }

  /* build physical address */
  memcpy(pa->family, FAMILY_NAME, 4);
  smnudpAddr(pa->addr) = host;
  smnudpPort(pa->addr) = smnHton2((ub2)prtno);

  return 0;
}

/*
 * smnudpClose - Closes a UDP NIO
 */
static void smnudpClose(mnnio *nio)
{
  smnudpcx *cx = (smnudpcx*)nio->usrp;
  SOCKET s = cx->s;

  if (!winsock2)
  {
  /* Shutdown recv thread, wait for it to exit. */ 
  TerminateThread( cx->selthread, 0 ); 

  /* Clean up context and nio */
  CloseHandle( cx->selthread );
  ssysRemWaitObject( ysGetOsdPtr(), cx->pauseevt );
 
  CloseHandle( cx->pauseevt );
    CloseHandle( cx->selevt );

    /* Clean up context and nio */
  DeleteCriticalSection( &cx->crit );

  free( nio->usrp );

  /* close the socket */
    (*wsClosesocket)(s);

  /* deallocate the NIO */
  free(nio);
}
  else
  {
    /* queue this connection for close */
    QueueRecv(nio, QOP_CLOSE);
  }
}

/*
 * smnudpSend - Sends data over UDP
 */
static sb4 smnudpSend(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len)
{
  smnudpcx *cx = nio->usrp;
  SOCKADDR_IN sa;
  sb4 cnt;
  DWORD err;

  /* create the address of our intended receiver */
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = smnudpAddr(pa->addr);
  sa.sin_port = smnudpPort(pa->addr);

  /* send the data */
  cnt = (*wsSendto)(cx->s, buf, len, 0, (struct sockaddr*)&sa, sizeof(SOCKADDR_IN));
  if (cnt == SOCKET_ERROR)
  {
    err = WSAGetLastError(); 
    yslError("smnudpSend: sendto error: %d %s\n", err, smnudpWSDecode(err)); 
    /* we've got problems... */
    cnt = (err == WSAENOBUFS) ? MNERR_OVERFLOW : MNERR_FAILURE;
  }
  return cnt;
}

/*
 * smnudpRecv - Receives data after select thread sez it is OK.
 */
static sb4 smnudpRecv(mnnio *nio, mnnpa *pa, ub1 *buf, size_t len,
		      boolean poll, ub4 timeout)
{
  smnudpcx *cx = (smnudpcx*)nio->usrp;
  int cnt, fromlen;
  sb4 err = MNSTS_NORMAL;
  DWORD serr;
  SOCKADDR_IN from;
  
  EnterCriticalSection( &cx->crit );
  if( !cx->rdy )
    {
      LeaveCriticalSection( &cx->crit );
      err = MNERR_WOULDBLOCK; 
    }
  else
    {
      cx->rdy = FALSE;
      LeaveCriticalSection( &cx->crit );

      if (!winsock2)
      {
      fromlen = sizeof(SOCKADDR_IN);
        cnt = (*ws1Recvfrom)(cx->s, buf, len, 0, (struct sockaddr *)&from, &fromlen);
      if (cnt == SOCKET_ERROR)
	{
          serr = (*wsGetlasterror)();
	  if( serr == WSAEWOULDBLOCK )
	    {
	      err =  MNERR_WOULDBLOCK;
	    }
	  else
	    {
	      err = MNERR_FAILURE; 
	      yslError("smnudpRecv: SOCKET_ERROR %d %s\n",
		       serr, smnudpWSDecode( serr ));
	    }
	}
      else
	{
	  /* fill in address */
	  memcpy(pa->family, FAMILY_NAME, 4);
	  smnudpAddr(pa->addr) = from.sin_addr.s_addr;
	  smnudpPort(pa->addr) = from.sin_port;
	  err = cnt;
	}
      /* allow sysiPause to sleep now */
      ResetEvent( cx->pauseevt );
      /* wake up select thread (and maybe sysiPause) */
      SetEvent( cx->selevt );
    }
      else /* winsock2 */
      {
        if (cx->dwError)
        {
	  err = MNERR_FAILURE; 
	  yslError("smnudpRecv: SOCKET_ERROR %d %s\n",
		         serr, smnudpWSDecode( serr ));
        }
        else
        {
          DWORD recvlen=0;
          int ret;

          memcpy(pa->family, FAMILY_NAME, 4);
          smnudpAddr(pa->addr) = cx->from.sin_addr.s_addr;
          smnudpPort(pa->addr) = cx->from.sin_port;
          err = cnt = (cx->wsabuf.len < len) ? cx->wsabuf.len : len;
          memcpy(buf, cx->wsabuf.buf, cnt);

          /* initiate the next read */
          if (QueueRecv(nio, QOP_RECV) == FALSE)
            err = MNERR_FAILURE;
        }
        /* allow sysiPause to sleep now if there are no pending receives */
        EnterCriticalSection(&QRecvHeader.crit);
        if (--QRecvHeader.nrecv == 0)
          ResetEvent( QRecvHeader.pauseevt );
        LeaveCriticalSection(&QRecvHeader.crit);
      }
    }
  return err;
}


/*
 * smnudpWSOpenSocket - uses WinSock to open a socket
 */
static SOCKET smnudpWSOpenSocket(void)
{
  SOCKET s;


  return s;
}

static sb4 smnudpSel( mnnio *nio, boolean poll, ub4 timeout )
{
  smnudpcx *cx = (smnudpcx *)nio->usrp;
  SOCKET s = cx->s;
  fd_set fdset;
  sb4 cnt;
  struct timeval to;
  struct timeval *top;

  FD_ZERO(&fdset);
  FD_SET(s, &fdset);
  if( poll )
  {
    to.tv_sec = 0;
    to.tv_usec = 0;
  }
  else if (timeout)
  {
    to.tv_sec = timeout / 1000;
    to.tv_usec = (timeout % 1000) * 1000;
  }

  /* wait for timeout or poll */
  top = (poll || timeout) ? &to : NULL;
  cnt = (*wsSelect)(0, &fdset, NULL, NULL, top);
  return cnt;
}



/* -------------------------- snmudpSelectThread --------------------------- */
/*
  NAME
    smnudpSelectThread
  DESCRIPTION
    Thread function to do repeated selects on the UDP port.  When it is
    ready, set the "rdy" flag in the context.
    
  PARAMETERS
    usrp    -- the nio as a dvoid *.
  RETURNS
    none
*/

static void smnudpSelectThread( mnnio *nio )
{
  smnudpcx *cx = nio->usrp;
  SOCKET s = (SOCKET)cx->s;
  int cnt;
  boolean rdy = FALSE;

  /* while shutdown mutex held, continue */

  while( TRUE )
  {
    if( rdy )			/* if set last time through, wait for reader */
    {
      WaitForSingleObject( cx->selevt, INFINITE );
      ResetEvent( cx->selevt );
    }

    rdy = FALSE;
    /* 1 minute max select wait */
    cnt = smnudpSel( nio, FALSE, 60000 );
    if (cnt > 0)
    {
      EnterCriticalSection( &cx->crit );
      rdy = cx->rdy = TRUE;
      /* maybe wakeup sysiPause */
      SetEvent( cx->pauseevt );
      LeaveCriticalSection( &cx->crit );
    }
  }
  return;			/* return exits thread */
}


static const char *smnudpWSDecode( DWORD err )
{
  const char *s;
  switch( err )
  {
  case WSAEFAULT:   s = "WSAEFAULT"; break;
  case WSAEINTR:    s = "WSAEINTR"; break;
  case WSAEINPROGRESS:	s = "WSAEINPROGRESS"; break;
  case WSAEINVAL:   s = "WSAEINVAL"; break;
  case WSAENOTCONN: s = "WSAENOTCONN"; break;
  case WSAENOTSOCK: s = "WSAENOTSOCK"; break;
  case WSAEOPNOTSUPP:	s = "WSAEOPNOTSUPP"; break;
  case WSAESHUTDOWN:	s = "WSAESHUTDOWN"; break;
  case WSAEWOULDBLOCK:	s = "WSAEWOULDBLOCK"; break;
  case WSAEMSGSIZE: s = "WSAEMSGSIZE"; break;
  case WSAECONNABORTED:	s = "WSAECONNABORTED"; break;
  case WSAECONNRESET:	s = "WSAECONNRESET"; break;
  default:
    s = "unknown";
  }
  return s;
}

static void CALLBACK CompletionRoutine(DWORD dwError, DWORD cbTransferred,
                                LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
  mnnio *nio = (mnnio *)lpOverlapped->hEvent;
  smnudpcx *cx;

//  if (dwError == WSA_OPERATION_ABORTED || dwError == WSAEINVAL)
  if (dwError)
  {
    /* the socket has been closed */
    return;
  }

  cx = (smnudpcx *)nio->usrp;

  EnterCriticalSection( &cx->crit );
  cx->rdy = TRUE;
  if (dwError)
  {
    cx->dwError = dwError;
  }
  else
    cx->wsabuf.len = cbTransferred;

  LeaveCriticalSection( &cx->crit );

  EnterCriticalSection( &QRecvHeader.crit );
  QRecvHeader.nrecv++;
  LeaveCriticalSection( &QRecvHeader.crit );
}

/* Queue the Receive call */
static int QueueRecv( mnnio *nio, int op)
{
  QRecvElem *qelem;

  EnterCriticalSection( &QRecvHeader.crit );
  if (qelem = QRecvHeader.free)
  {
    QRecvHeader.free = qelem->next;
    LeaveCriticalSection( &QRecvHeader.crit );
  }
  else
  {
    LeaveCriticalSection( &QRecvHeader.crit );
    if (!(qelem = (QRecvElem *)malloc(sizeof(QRecvElem))))
      return FALSE;
  }

  qelem->nio = nio;
  qelem->op = op;
  qelem->next = NULL;

  EnterCriticalSection( &QRecvHeader.crit );
  if (QRecvHeader.last)
    QRecvHeader.last->next = qelem;
  else
    QRecvHeader.first = qelem;
  QRecvHeader.last = qelem;
  LeaveCriticalSection( &QRecvHeader.crit );

  /* wake up the other thread to initiate the operation */
  SetEvent( QRecvHeader.recvevt );

  return TRUE;
}


/* A single receive thread for Winsock2 */
static VOID RecvThread()
{
  QRecvElem *qelem;
  DWORD recvlen = 0, err;
  int ret;
  DWORD flags = 0;
  smnudpcx *cx;
  mnnio *nio;
  int op;

  while (TRUE)
  {
//    SleepEx( 10, TRUE );
//    if (WaitForMultipleObjectsEx( 1, ahEvt, FALSE, 1, TRUE ) == WAIT_OBJECT_0)
    if (WaitForSingleObjectEx( QRecvHeader.recvevt, 1000, TRUE ) == WAIT_OBJECT_0)
      ResetEvent (QRecvHeader.recvevt );

    /* Dequeue and initiate receive */
    EnterCriticalSection( &QRecvHeader.crit );
    if (QRecvHeader.nrecv)
      SetEvent( QRecvHeader.pauseevt);

    qelem = QRecvHeader.first;
    while (qelem)
    {
      nio = qelem->nio;
      op = qelem->op;

      /* remove the element from the queue */
      QRecvHeader.first = qelem->next;
      if (qelem->next == NULL)
        QRecvHeader.last = NULL;

      /* and put it on the free list */
      qelem->next = QRecvHeader.free;
      QRecvHeader.free = qelem;
      LeaveCriticalSection( &QRecvHeader.crit );

      cx = (smnudpcx *)nio->usrp;

      if (op == QOP_CLOSE)
      {
        /* this connection is queued for closing */
        if( cx->rdy )
        {
          /* allow sysiPause to sleep now if there are no pending receives */
          EnterCriticalSection(&QRecvHeader.crit);
          if (--QRecvHeader.nrecv == 0)
            ResetEvent( QRecvHeader.pauseevt );
          LeaveCriticalSection(&QRecvHeader.crit);
        }

        /* Clean up context and nio */
        DeleteCriticalSection( &cx->crit );
      
        /* close the socket */
        (*wsClosesocket)(cx->s);

        /* deallocate stuff allocated by this nio */
        free( cx->wsabuf.buf );
        free( nio->usrp );
        free( nio );
      }
      else
      {
        /* initiate an overlapped i/o */
        cx->wsabuf.len = nio->pktmax;
        cx->dwError = 0;
    
        cx->wsaoverlapped.hEvent = (WSAEVENT) nio;
        cx->fromLen = sizeof(cx->from);
        ret = (*ws2Recvfrom)(cx->s, &cx->wsabuf, 1, &recvlen, &flags, 
                       (struct sockaddr *)&cx->from, (LPINT)&cx->fromLen, 
                       &cx->wsaoverlapped,
                       (LPWSAOVERLAPPED_COMPLETION_ROUTINE)CompletionRoutine);
        if (!ret)
        {
          /* there is already some data */
          EnterCriticalSection( &QRecvHeader.crit );
          if (QRecvHeader.nrecv)
            SetEvent( QRecvHeader.pauseevt);
          LeaveCriticalSection( &QRecvHeader.crit );
        }
        else if ((err = (*wsGetlasterror)()) != WSA_IO_PENDING)
        {
          EnterCriticalSection( &cx->crit );
          cx->rdy = TRUE;
          cx->dwError = err;
          LeaveCriticalSection( &cx->crit );
          /* maybe wakeup sysiPause */
          SetEvent( QRecvHeader.pauseevt );
        }
      }
      EnterCriticalSection( &QRecvHeader.crit );
      qelem = QRecvHeader.first;
    }
    LeaveCriticalSection( &QRecvHeader.crit );
  }
}

BOOL WINAPI smnDllInit(HANDLE hDll, DWORD reason, LPVOID reserved)
{
  /* do Winsock init/deinit here */
  switch (reason)
  {
    case DLL_PROCESS_ATTACH:
    {
      OSVERSIONINFO osverinfo;
      HINSTANCE hLib;
      int major_ver, minor_ver;
      DWORD idRecvThr;
      WORD wVersionRequested;
      WSADATA wsaData;

      osverinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (!GetVersionEx(&osverinfo))
        return FALSE;
      if (osverinfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
          osverinfo.dwMajorVersion >= 4)
      {
        winsock2 = TRUE;
        hLib = LoadLibrary( "ws2_32.dll" );
        major_ver = 2;
        minor_ver = 0;
        
        ws2Socket = (LPFN_WSASOCKET)GetProcAddress( hLib, "WSASocketA" );
        ws2Recvfrom = (LPFN_WSARECVFROM)GetProcAddress( hLib, "WSARecvFrom" );
        if (!ws2Socket || !ws2Recvfrom)
          return FALSE;
      }
      else
      {
        hLib = LoadLibrary( "wsock32.dll" );
        major_ver = 1;
        minor_ver = 1;

        ws1Socket = (LPFN_SOCKET)GetProcAddress( hLib, "socket" );
        ws1Recvfrom = (LPFN_RECVFROM)GetProcAddress( hLib, "recvfrom" );
        if (!ws1Socket || !ws1Recvfrom)
          return FALSE;
      }
      wsStartup = (LPFN_WSASTARTUP)GetProcAddress( hLib, "WSAStartup" );
      wsCleanup = (LPFN_WSACLEANUP)GetProcAddress( hLib, "WSACleanup" );
      wsIoctlsocket = (LPFN_IOCTLSOCKET)GetProcAddress( hLib, "ioctlsocket" );
      wsSetsockopt = (LPFN_SETSOCKOPT)GetProcAddress( hLib, "setsockopt" );
      wsBind = (LPFN_BIND)GetProcAddress( hLib, "bind" );
      wsGetsockname = (LPFN_GETSOCKNAME)GetProcAddress( hLib, "getsockname" );
      wsGethostname = (LPFN_GETHOSTNAME)GetProcAddress( hLib, "gethostname" );
      wsGetlasterror = (LPFN_WSAGETLASTERROR)GetProcAddress( hLib, "WSAGetLastError" );
      wsGethostbyname = (LPFN_GETHOSTBYNAME)GetProcAddress( hLib, "gethostbyname" );
      wsInet_addr = (LPFN_INET_ADDR)GetProcAddress( hLib, "inet_addr" );
      wsClosesocket = (LPFN_CLOSESOCKET)GetProcAddress( hLib, "closesocket" );
      wsSendto = (LPFN_SENDTO)GetProcAddress( hLib, "sendto" );
      wsSelect = (LPFN_SELECT)GetProcAddress( hLib, "select" );

      if (!wsStartup || !wsCleanup || !wsIoctlsocket || !wsSetsockopt ||
          !wsBind || !wsGetsockname || !wsGethostname || !wsGetlasterror ||
          !wsGethostbyname || !wsInet_addr || !wsClosesocket ||
          !wsSendto || !wsSelect)
        return FALSE;

      wsHtons = (LPFN_HTONS)GetProcAddress( hLib, "htons" );
      wsHtonl = (LPFN_HTONL)GetProcAddress( hLib, "htonl" );
      wsNtohs = (LPFN_NTOHS)GetProcAddress( hLib, "ntohs" );
      wsNtohl = (LPFN_NTOHL)GetProcAddress( hLib, "ntohl" );

      if (!wsHtons || !wsHtonl || !wsNtohs || !wsNtohl)
        return FALSE;

      wVersionRequested = MAKEWORD(major_ver, minor_ver);
      if ((*wsStartup)(wVersionRequested, &wsaData ))
        return FALSE;

      if ((LOBYTE(wsaData.wVersion) != major_ver) ||
          (HIBYTE(wsaData.wVersion) != minor_ver))
        return FALSE;

      if (winsock2)
      {
        /* create a single thread for recv */
        QRecvHeader.first = NULL;
        QRecvHeader.last = NULL;
        QRecvHeader.free = NULL;
        QRecvHeader.nrecv = 0;
        QRecvHeader.pauseevt = CreateEvent( 0, TRUE, FALSE, 0 );
        QRecvHeader.recvevt = CreateEvent( 0, TRUE, FALSE, 0 );
        InitializeCriticalSection(&QRecvHeader.crit);
#if 0
        if (winsock2)
        { 
          int i;
          /* create the required number of threads for recv operations */
          for (i = 0; i < WS2_OP_THREADS; i++)
            if ((CreateThread( NULL, 8096, (LPTHREAD_START_ROUTINE)RecvThread, 
                            NULL, 0, &idRecvThr )) == NULL)
              return FALSE;
        }
#endif
      }

      break;
    }
    case DLL_THREAD_ATTACH:
      break;
     
    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      if (winsock2)
      {
        DeleteCriticalSection( &QRecvHeader.crit );
        CloseHandle( QRecvHeader.pauseevt );
      }
      (*wsCleanup)();
      break;
  }
  return TRUE;
}
