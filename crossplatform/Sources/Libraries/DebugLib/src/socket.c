/**************************************************************************/
/* File:    socket.c                                                      */
/* Purpose: Routines for the Socket output method.                        */
/*                                                                        */
/* Copyright [1999-2001] Pace Micro Technology PLC.  All rights reserved. */
/*                                                                        */
/* The copyright in this material is owned by Pace Micro Technology PLC   */
/* ("Pace").  This material is regarded as a highly confidential trade    */
/* secret of Pace.  It may not be reproduced, used, sold or in any        */
/* other way exploited or transferred to any third party without the      */
/* prior written permission of Pace.                                      */
/**************************************************************************/

#include "include.h"

#ifdef ENABLE_SOCKET_SUPPORT

#include "globals.h"
#include "socket.h"
#include "misc.h"

#ifdef __riscos
#  include "socklib.h"
#  include "inetlib.h"
#  define write socketwrite
#  define close socketclose
#endif

#ifdef HAVE_NETINET_IN_H
#  include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#  include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#  include <sys/socket.h>
#endif

static int s = -1;

/************************************************************************/
/* debug_socket_init                                                    */
/*                                                                      */
/* Function to initialise a raw TCP connection to a remote host         */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    true or false.                                           */
/*                                                                      */
/************************************************************************/
bool debug_socket_init (void)
{
  if (s == -1) {
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s != -1) {
      char *host = debug_misc_getenv_task_specific("SocketHost");
      struct sockaddr_in sin;
      int t = -1;

      if (host) {
        char *port = debug_misc_getenv_task_specific("SocketPort");
        short portnum;
        portnum = port ? atoi(port) : 9; /* "discard" service */
        if (port) free(port);
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_port = htons(portnum);
        if (inet_aton(host, &sin.sin_addr)) {
          t = connect (s, (struct sockaddr *) &sin, sizeof(sin));
        }
        free(host);
      }

      if (t < 0) {
        close(s);
        s = -1;
      }
    }
  }

  return (s != -1);
}


/************************************************************************/
/* debug_socket_output                                                  */
/*                                                                      */
/* Function is the output routine for TCP socket.                       */
/*                                                                      */
/* Parameters: buffer   - text to output.                               */
/*                                                                      */
/* Returns:    void.                                                    */
/*                                                                      */
/************************************************************************/
void debug_socket_output (const char *buffer, size_t len)
{
  if (s != -1) {
    (void) write (s, buffer, len);
  }
}


/************************************************************************/
/* debug_socket_quit                                                    */
/*                                                                      */
/* Function to terminate a TCP socket connection                        */
/*                                                                      */
/*                                                                      */
/* Parameters: void.                                                    */
/*                                                                      */
/* Returns:    void         .                                           */
/*                                                                      */
/************************************************************************/
void debug_socket_quit (void)
{
  if (s != -1) {
    close (s);
    s = -1;
  }
}

#endif /* ENABLE_SOCKET_SUPPORT */
