
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

#if 0
jmp_buf3  vjmpSuspend;
jmp_buf3  vjmpResume;
jmp_buf3  vjmpComplete;
#endif

THINWIRECAPS vThinWireMode;

#ifdef DEMO
char * vpszVersion="tek";
#else
char * vpszVersion="vdtw30";
#endif
