
/*****************************************************************************\
*
*   TWDATA.C
*
*   Thin Wire Windows - global variables
*
*   Copyright (c) Citrix Systems Inc. 1992
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*   Revision 1.1  1998/01/19 19:12:46  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
*
*  
*     Rev 1.2   15 Apr 1997 18:16:14   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   21 Mar 1997 16:09:28   bradp
*  update
*  
*     Rev 1.1   03 Jan 1996 13:33:00   kurtp
*  update
*  
\*****************************************************************************/

#include "windows.h"

/*
 *  Get the standard C includes
 */
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../../../inc/client.h"
#include "twtype.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"
#include "twwin.h"
#include "twdata.h"


// Note: This file must be modified in sync with TWDATA.C

jmp_buf3  vjmpSuspend;
jmp_buf3  vjmpResume;
jmp_buf3  vjmpComplete;

THINWIRECAPS vThinWireMode;

#ifdef DEMO
char * vpszVersion="tek";
#else
char * vpszVersion="vdtw30";
#endif
