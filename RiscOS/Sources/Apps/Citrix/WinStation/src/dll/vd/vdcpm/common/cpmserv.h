
/*************************************************************************
*
* cpmserv.h
*
* Header file for the Client Printer Mapping server program.
*
* This header file is used for building the server as a new CITRIX
* client Virtual Driver DOS DLL.
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author:  John Richardson 05/06/94
*
* $Log$
*  
*     Rev 1.3   15 Apr 1997 18:04:20   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.3   21 Mar 1997 16:09:18   bradp
*  update
*  
*     Rev 1.2   18 Oct 1995 18:00:54   JohnR
*  update
*
*     Rev 1.1   18 Oct 1995 17:52:34   JohnR
*  update
*
*     Rev 1.0   10 Jul 1995 11:33:46   JohnR
*  Initial revision.
*
*     Rev 1.9   27 Jun 1995 16:18:56   JohnR
*  update
*
*     Rev 1.8   27 Jun 1995 15:28:46   JohnR
*  update
*
*     Rev 1.7   23 Jun 1995 15:55:10   JohnR
*  update
*
*     Rev 1.6   03 May 1995 10:01:42   richa
*
*     Rev 1.5   28 Apr 1995 14:19:02   richa
*  Do Win16 & Win32.
*
*     Rev 1.4   17 Apr 1995 12:40:44   marcb
*  update
*
*************************************************************************/

#include "windows.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#define VDCPM  1  // lptapi.h needs this

#include "../../../../inc/client.h"

#ifdef DOS
#include "../../../../inc/dos.h"
#endif

#include "../../../../inc/clib.h"
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#include "../../../../inc/wdapi.h"
#include "../../../../inc/vdapi.h"
#include "../../../../inc/mouapi.h"
#include "../../../../inc/timapi.h"
#include "../../../../inc/logapi.h"
#include "../../inc/vd.h"
#include "../../../wd/inc/wd.h"

/*
 * Define abstract types used by cpmwire.h to describe the protocol
 */

#ifdef  OUTOUT
typedef unsigned long  ULONG;
typedef unsigned short USHORT;
typedef unsigned char  UCHAR;

typedef unsigned long  *PULONG;
typedef unsigned short *PUSHORT;
typedef unsigned char  *PUCHAR;
typedef void *PVOID;

typedef char  CHAR;
typedef char  *PCHAR;


#define TRUE  1
#define FALSE 0
#endif

typedef  int CPM_BOOLEAN;

// Now we can include our protocol header

#include "citrix/cpmwire.h"
#include "citrix/cpmold.h" // Wire protocol definitions

