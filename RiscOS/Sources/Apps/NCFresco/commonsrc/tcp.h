/* Very smuch simplified version of the headers for the HTML code */

#ifndef TCP_H
#define TCP_H

#define PARSER_ONLY

#undef GOT_SYSTEM
#define DEBUG                   /* Can't put it on the CC command line  */
#define NO_UNIX_IO              /* getuid() missing                     */
#define NO_GETPID               /* getpid() does not exist              */
#define NO_GETWD                /* getwd() does not exist               */

extern int errno;
#include <time.h>

#if 0
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "sys/errno.h"
#include "pwd.h"
#endif

#define TCP_INCLUDES_DONE
#define INCLUDES_DONE

#include <stdio.h>
#define STDIO_H

#include <string.h>             /* For bzero etc - not  VM */

/*

   On non-VMS machines, the GLOBALDEF and GLOBALREF storage types default to normal C
   storage types.

 */
#ifndef GLOBALREF
#define GLOBALDEF
#define GLOBALREF extern
#endif

/*  MACROS FOR CONVERTING CHARACTERS */

#ifndef TOASCII
#define TOASCII(c) (c)
#define FROMASCII(c) (c)
#endif

/* WHITE CHARACTERS
   Is character c white space? For speed, include all control characters
 */

#define WHITE(c) (((unsigned char)(TOASCII(c))) <= 32)

#endif /* TCP_H */

/* end of system-specific file */
