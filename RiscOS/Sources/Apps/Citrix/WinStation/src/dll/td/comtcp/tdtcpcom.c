/*************************************************************************
*
*  tdtcpcom.c
*
*  TCP Protocol driver external routines
*
*  Copyright Citrix Systems Inc. 1996
*
*  Author: Bill Guo 7/21/96
*

*
*************************************************************************/
#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"
#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/ibrowerr.h"

#ifdef DOS
#include "../../../inc/dos.h"
#endif
#include "../../../inc/clib.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/pdapi.h"
#include "../../../inc/nrapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/td.h"
//#include "../../../inc/loadstr.h"



/*=============================================================================
==   Functions Defined
=============================================================================*/

int WFCAPI GetTCPHostNamePort(char *szHostString, char *szHostName, USHORT *pPortNumber);

/*=============================================================================
==   External Functions used
=============================================================================*/


/*=============================================================================
==   Data
=============================================================================*/


/*=============================================================================
==   Global Data
=============================================================================*/


/*******************************************************************************
 *
 *  GetTCPHostNamePort
 *
 *    This functions parsing the Host entry string into two 
 *    parts: Host Name and TCP Port Number.
 *    
 *
 * ENTRY:
 *    szHostString (input) : Host entry string format: Hostname[:PortNumber]
 *    szHostName   (output): Host name after parsing
 *    pPortName    (Output): ICA TCP Port Number pointer
 *       
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/


int WFCAPI GetTCPHostNamePort(char *szHostString, char *szHostName, USHORT *pPortNumber)
{
    char * pDest;
	int iPosition;
    
	pDest=strrchr(szHostString, ':');
	if (pDest==NULL) {
		strcpy(szHostName, szHostString);
		szHostName[strlen(szHostString)] = '\0';
		*pPortNumber=0;
	}
	else {
		iPosition=(int) (pDest - szHostString) + 1;
		strncpy(szHostName,szHostString,iPosition-1 );
		szHostName[iPosition - 1]='\0';
		*pPortNumber=atoi(szHostString + iPosition);
	}
	AnsiUpper(szHostName);

    return( CLIENT_STATUS_SUCCESS );
}

