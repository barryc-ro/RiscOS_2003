
/***************************************************************************
*
*  VD.H
*
*  This module contains Virtual Driver (PD) defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Andy 4/6/94
*
*  $Log$
*  
*     Rev 1.6   15 Apr 1997 18:02:32   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   08 May 1996 13:59:42   jeffm
*  update
*  
*     Rev 1.4   01 Sep 1994 17:36:26   bradp
*  update
*  
*     Rev 1.3   24 Aug 1994 17:54:14   bradp
*  update
*  
*
****************************************************************************/

#ifndef __VD_H__
#define __VD_H__

/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _VD * PVD;


/*=============================================================================
==   defines
=============================================================================*/


/*=============================================================================
==   structures
=============================================================================*/

/*
 *  VD structure
 */
typedef struct _VD {

    ULONG ChannelMask;                  // bit mask of handled channels
    PDLLLINK pWdLink;                   // pointer to winstation driver
    int LastError;                      // Last Error code
    PVOID pPrivate;                     // pointer to VD uncommon data

    PPLIBPROCEDURE pDeviceProcedures;
    
} VD;

#endif //__VD_H__
