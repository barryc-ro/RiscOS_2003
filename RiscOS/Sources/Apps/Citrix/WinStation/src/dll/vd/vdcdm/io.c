/*************************************************************************
*
* io.c
*
* copyright notice: Copyright 1994, Citrix Systems Inc.
*
* Author: John Richardson 01/20/94
*         Rich Andresen   06/12/95
*
* Log:
*
*************************************************************************/
#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../inc/client.h"

#include "../../../inc/clib.h"
#include "citrix/ica.h"
#include "citrix/ica30.h"
#include "citrix/ica-c2h.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/timapi.h"
#include "../../../inc/logapi.h"
#include "../../../inc/biniapi.h"
#include "../inc/vd.h"
#include "../../wd/inc/wd.h"
#include "citrix/cdmwire.h" // Wire protocol definitions

#include "vdcdm.h"

/*****************************************************************************
 *
 *  CdmDosError
 *
 *  This returns the DOS extended error, and error class after a DOS
 *  I/O call fails.
 *
 *
 * ENTRY:
 *   RetVal (input)
 *     Original return value from DOS I/O function. This is used in the
 *     case of DOS 2.x in order to map an error code and class when extended
 *     error information is not available.
 *
 *   pErrorClass (output)
 *     Pointer to variable to place the error class
 *
 *   pExtError (output)
 *     Pointer to variable for the extended DOS error
 *
 * EXIT:
 *
 ****************************************************************************/

void STATIC
CdmDosError( int     RetVal,
             PUSHORT pErrorClass,
             PUSHORT pExtError
           )
{
    _kernel_oserror *e;

    /*
     * If this is a 0, then there is no error
     */
     if (RetVal == 0)
     {
        TRACE(( TC_CDM, TT_API3, "CdmDosError: dosexterr 0, no error! Warning! No func Error! ret %d", RetVal));
        *pErrorClass = CDM_ERROR_NONE;
        *pExtError =   CDM_DOSERROR_NOERROR;
        return;
    }

    e = (_kernel_oserror *)RetVal;

    /*
     * If the extended error is 83, INT24 fail, see if the external
     * Int24HardErrorCode has been set. If so, map these codes and return
     * them as the DOS error code.
     */

    if( errbuf.exterror == CDM_DOSERROR_INT24FAIL ) {

#ifdef  DOS
        if( (c = _dos_mygetharderr()) != 0xFF ) {
            // Map these to the extended error codes as documented
            // in the DOS 3.x manual
            *pExtError = c + 19;
            *pErrorClass = CDM_ERROR_MEDIA;
            TRACE(( TC_CDM, TT_API3, "CdmDosError: INT24 Fail! HardErr %d, mapped newval %d", c, *pExtError));
            return;
        } else {
#endif
            TRACE(( TC_CDM, TT_API3, "CdmDosError: INT24 Fail! class %d exterr %d", errbuf.errclass, errbuf.exterror));
            // Not the result of a hard error
            *pErrorClass = errbuf.errclass;
            *pExtError = errbuf.exterror;
            return;
#ifdef  DOS
        }
#endif
    } else {
        // Not the result of a hard error
        *pErrorClass = errbuf.errclass;
        *pExtError = errbuf.exterror;
    }

    return;
}





/*****************************************************************************
 *
 *  _dos_rootftime
 *
 *   This returns the current date and time formatted into the
 *   same format that _dos_getftime() does since the root directory
 *   does not allow this.
 *
 * ENTRY:
 *   pFileDate (output)
 *     Pointer to variable to place the formated date
 *
 *   pFileTime (output)
 *     Pointer to variable to place the formated time
 *
 * EXIT:
 *    Returns the AX register value if the cflag is set, 0 if no error.
 *
 ****************************************************************************/

int STATIC _dos_rootftime( UINT * pFileDate,
                    UINT * pFileTime
                  )
{
    struct dosdate_t Date;
    struct dostime_t Time;
    unsigned int tmpdate, tmptime;

    _dos_getdate( &Date );

    tmpdate = 0;
    tmpdate = Date.day & 0x1F;  // (uchar) 1-31
    tmpdate |= ((unsigned int)Date.month << 5); // (uchar) 1-12
    tmpdate |= (Date.year << 9); // (uint)  1980-2099
    *pFileDate = tmpdate;

    _dos_gettime( &Time );

    tmptime = 0;
    tmptime = (Time.second/2) & 0x1F; // (uchar) 0-59
    tmptime |= ((unsigned int)Time.minute << 5); // (uchar) 0-59
    tmptime |= ((unsigned int)Time.hour << 11);   // (uchar) 0-23

    *pFileTime = tmptime;

    return(0);
}
