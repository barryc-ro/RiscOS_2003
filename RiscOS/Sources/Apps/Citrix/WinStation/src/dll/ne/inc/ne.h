
/***************************************************************************
*
*  NE.H
*
*  This module contains internal Name Enumerator (NE) defines and structures
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Brad Pedersen  (11/11/94)
*
*  $Log$
*  
*     Rev 1.2   11 Jun 1997 07:57:06   butchd
*  Define SV_TYPE_APPSERVER bit if needed
*  
*     Rev 1.1   15 Apr 1997 16:18:48   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   11 Nov 1994 18:39:22   bradp
*  Initial revision.
*  
*
****************************************************************************/


/*=============================================================================
==   Citrix AppServer mask (ms networks NetServerEnum type field)
==   This is (will be) in the standard lmserver.h file as the new 'multi-user
==   WindowsNT bit", but may not be for a while yet, so we define it here if
==   needed.
=============================================================================*/

#ifndef SV_TYPE_APPSERVER
#define SV_TYPE_APPSERVER   0x10000000
#endif

/*=============================================================================
==   typedefs
=============================================================================*/

typedef struct _NE * PNE;


/*=============================================================================
==   structures
=============================================================================*/

/*
 *  NE structure
 */
typedef struct _NE {

    USHORT NotUsed;

} NE;

