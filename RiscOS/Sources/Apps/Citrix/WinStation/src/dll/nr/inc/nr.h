
/***************************************************************************
*
*  NR.H
*
*  This module contains internal Name Resolver (NR) defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (11/3/94)
*
*  nr.h,v
*  Revision 1.1  1998/01/12 11:35:26  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.3   15 Apr 1997 16:19:38   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   08 May 1996 16:27:10   jeffm
*  update
*  
*     Rev 1.1   16 Feb 1995 17:33:24   terryt
*  Spruce up name resolvers
*  
*     Rev 1.0   07 Nov 1994 19:19:50   bradp
*  Initial revision.
*  
*
****************************************************************************/

#ifndef __NR_H__
#define __NR_H__

/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _NR * PNR;


/*=============================================================================
==   structures
=============================================================================*/

/*
 *  NR structure
 */
typedef struct _NR {

//   USHORT NotUsed;
    PCHAR pProtocolName;
    PPLIBPROCEDURE pDeviceProcedures;
} NR;

/*
 *  PD error message structure
 */
typedef struct _PDERRORMESSAGE {
    int Error;
    char * pMessage;
} PDERRORMESSAGE, * PPDERRORMESSAGE;

#define ERROR_HOST_NOT_FOUND 0xFFFF

#endif //__NR_H__
