/****************************************************************************
*
*   CFGINI.C
*
*   UI Configuration INI library
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/12/95) [from Kurt Perry's CFG library]
*
*   $Log$
*  
*     Rev 1.77   28 May 1998 17:25:48   terryt
*  Fix win16 trap
*  
*     Rev 1.76   30 Apr 1998 16:04:16   terryt
*  remove inidef.h from DOS client
*  
*     Rev 1.75   26 Apr 1998 19:40:10   terryt
*  provide more descriptive out of memory error messages
*  
*     Rev 1.74   25 Apr 1998 17:52:52   terryt
*  Put in additional DOS out of memory checking
*  
*     Rev 1.73   17 Apr 1998 18:05:14   terryt
*  CDM CPM and COM disable
*  
*     Rev 1.72   02 Apr 1998 11:54:32   butchd
*  Use DEF_ENCRYPTIONDLL for encryption DLL check
*  
*     Rev 1.71   31 Mar 1998 09:47:32   butchd
*  use DEF_ defines for INI_CAM and INI_CM_UPDATESALLOWED checks
*  
*     Rev 1.70   14 Mar 1998 18:35:06   toma
*  CE Merge
*
*     Rev 1.69   11 Mar 1998 11:22:20   toma
*  update
*
*     Rev 1.67   01 Mar 1998 15:06:02   TOMA
*  ce merge
*
*     Rev 1.66   16 Feb 1998 09:43:34   kalyanv
*  fix for cpr 8442, added check for loading vdcm.dll
*
*     Rev 1.65   Feb 06 1998 17:54:40   briang
*  Fix Call to SupportRequired to use correct Section Name
*
*     Rev 1.64   Jan 28 1998 18:57:54   briang
*  Allow non-TCPIP ICA files connections if full client installed
*
*     Rev 1.63   Jan 07 1998 21:33:54   briang
*  Fix some memory leaking
*
*     Rev 1.62   06 Jan 1998 16:11:14   butchd
*  update
*
*     Rev 1.61   03 Dec 1997 11:33:00   terryt
*  vesa client
*
*     Rev 1.60   Oct 28 1997 23:38:46   briang
*  Fix for checking if support is required in INI files
*
*     Rev 1.59   28 Oct 1997 16:44:14   stephens
*  Added CLIENT_ERROR_DRIVER... for DOS Audio Driver
*
*     Rev 1.58   Oct 22 1997 11:56:38   briang
*  Allow Server section to overwrite WFClient sections
*
*     Rev 1.57   Oct 21 1997 17:49:08   briang
*  Allow for non-case matching INI entries
*
*     Rev 1.51   17 Oct 1997 14:38:44   terryt
*  fix audio disable
*
*     Rev 1.50   Oct 16 1997 10:36:02   briang
*  Add a generic section for serial conns
*
*     Rev 1.49   15 Oct 1997 14:38:56   tariqm
*  Audio disabling
*
*     Rev 1.48   Oct 14 1997 15:20:36   briang
*  add internetclient support
*
*     Rev 1.47   Oct 13 1997 18:11:54   briang
*  Fix Memory Leak in EatWhiteSpace
*
*     Rev 1.46   Oct 13 1997 10:06:08   briang
*  Get rid of my extra trace code
*
*     Rev 1.43   Oct 10 1997 17:05:28   briang
*  update
*
*     Rev 1.40   Oct 10 1997 13:38:58   briang
*  Update
*
*     Rev 1.37   29 Sep 1997 16:02:28   davidp
*  Put audio VD load hack into main VD load loop so it can be disabled
*
*     Rev 1.36   26 Sep 1997 19:18:20   davidp
*  Added temporary hack to load VDCAM in Win16 and Win32
*
*     Rev 1.35   26 Sep 1997 15:50:52   dmitryv
*  Changed GetSection() for WIN32 to work the same as WIN16 because of a WIN95 bug that won't open ReadOnly INI files
*
*     Rev 1.35   25 Sep 1997 16:41:46   dmitryv
*  Changed GetSection() for WIN32 to work the same as WIN16 because of a WIN95 bug that won't open ReadOnly INI files
*
*     Rev 1.34   04 Aug 1997 20:11:26   tariqm
*  Encryption support for web clients
*
*     Rev 1.33   22 Jul 1997 20:19:10   tariqm
*  Update
*
*     Rev 1.30   17 Jul 1997 14:55:12   tariqm
*  temp put
*
*     Rev 1.29   15 Apr 1997 18:48:56   TOMA
*  autoput for remove source 4/12/97
*
*     Rev 1.30   24 Mar 1997 08:59:34   richa
*  Update.
*
*     Rev 1.29   19 Mar 1997 14:02:22   richa
*  Send ClientName to the TD so that we'll be able to reconnect to disconnected session in a cluster.
*
*     Rev 1.28   07 Feb 1997 17:50:16   kurtp
*  update
*
*     Rev 1.27   18 Dec 1996 18:05:46   jeffm
*  Async working again after having TCP/IP the default transport
*
*     Rev 1.26   14 Aug 1996 15:22:44   richa
*  NULL out return value after freeing memory on error condition.
*
*     Rev 1.25   30 May 1996 16:53:36   jeffm
*  update
*
*     Rev 1.22   14 May 1996 11:28:40   jeffm
*  update
*
*     Rev 1.20   15 Dec 1995 10:51:52   butchd
*  update
*
*************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Get CLIB includes */
#include "../../inc/client.h"
#include "../../inc/iniapi.h"
#include "../../inc/logapi.h"
#include "../../inc/wdapi.h"
#include "../../inc/pdapi.h"
#include "../../inc/vdapi.h"
#include "../../inc/clib.h"
#include "../../inc/cfgini.h"

#ifdef WINCE
#include "../../inc/iniapi.h"
#endif


// local includes
#include "cfginip.h"
#ifndef DOS
#include "inidef.h"
#endif

/*=============================================================================
==   If we're building for DOS model, we'll get our memINI routines through
== the proctable method.  Otherwise (for Windows UI), we'll snarf in the guts
== of the memINI APIs and build them in static-like.  To make life easier for
== Windows UI OEMs wanting to add CFGINI support (or not).
=============================================================================*/
#ifdef DOS
#include "../ini/helpers.c"
#include "../../inc/miapi.h"
#else
#include "../memini/memguts.c"
#endif

/*=============================================================================
==   Local Functions Used
=============================================================================*/

int MakeProfileAndLoadSession( HANDLE, PCHAR, PCHAR, PCFGINIOVERRIDE,
                               PCHAR, PCHAR, PCHAR, BOOL);
int LoadPdDll( HANDLE, PCHAR, PCHAR );
int LoadWdDll( HANDLE, PCHAR, PCHAR );
int LoadVdDll( HANDLE, PCHAR, PCHAR, PCHAR );

PLIST BuildList( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR );
PLIST BuildMembersList( PCHAR, PCHAR, PCHAR );
VOID  DestroyList( PLIST );
VOID  DestroySList( PSECTIONLIST );
PCHAR GetFirstElement( PCHAR );
PCHAR GetNextElement( PCHAR );
PSECTIONLIST BuildAllSectionsList( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR );

char  * basename( char *);
void  GetString( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, int );
int   SupportRequired( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, BOOL );
int   GetSection( PCHAR, PCHAR, PCHAR *, PLIST );
#ifndef DOS
int   GetPrivateProfileStringDefault( PCHAR pSection, PCHAR pKey,
                                      PCHAR pBuf, int cbLen, PCHAR pFile);
int   GetPrivateProfileSectionDefault( PCHAR pSection, PCHAR pBuf,
                                       int cbLen, PCHAR pFile);
#endif
int   GetSectionString(PCHAR, PCHAR, PCHAR, PCHAR, int);
int   IniSize( PCHAR );
PCHAR BuildCfgIniOverridesSection( PCFGINIOVERRIDE );
PCHAR CombinePrivateProfileEntries( PCHAR, PCHAR );
PCHAR ConcatSections( PCHAR, PCHAR );
PCHAR AddHeaderSection(PCHAR , PCHAR );
PCHAR AddEntrySection(PCHAR , PCHAR, PCHAR );
void  EatWhiteSpace(PCHAR);

/*=============================================================================
==   Local Variables
=============================================================================*/

/*=============================================================================
==   External Functions Used
=============================================================================*/

/*=============================================================================
==   External Data
=============================================================================*/
extern CLIENTNAME   gszClientName;

/*=============================================================================
==   Global Data
=============================================================================*/

/*******************************************************************************
 *
 *  CfgIniLoad
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pConnection (input)
 *          Name of connection entry in server file.
 *      pCfgIniOverrides (input)
 *          NULL if no overrides, or points to array of CFGINIOVERRIDE
 *          structures to combine into the pConnection entry's section.
 *      pServerFile (input)
 *          Server INI file name.   -- ICA or AppSrv file
 *      pProtocolFile (input)
 *          Protocol INI file name. -- Module.ini
 *      pModemFile (input)
 *          Modem INI file name.    -- Modem.ini (unused)
 *      pConfigFile (input)
 *          Config INI file name.   -- WFClient.ini
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int WFCAPI
CfgIniLoad( HANDLE   hClientHandle,
            LPSTR    pConnection,
            PCFGINIOVERRIDE pCfgIniOverrides,
            LPSTR    pServerFile,
            LPSTR    pProtocolFile,
            LPSTR    pModemFile,
            LPSTR    pConfigFile )
{
    int    rc;
    PCHAR  pWdType;
    PCHAR  pPdType;
    char   *pEncryption, *pTAPIModem;
    PLIST  pList = NULL;
    PLIST  pSave = NULL;
    char   OnOff[4]; // temp space for driver loading...
    char   pszBuffer[20];
    char   *pszDriver = INI_DRIVERNAME;

    DEVICENAME      pszDeviceName;
    BOOL            bTapi = FALSE;
    BOOL            bEncryption = FALSE;
    BOOL            bInternetConnection = FALSE;
    int i;
#ifndef DOS
    ENCRYPTIONLEVEL szEncryptionLevelSession;
#endif
#ifdef DOS
    ULONG           ReservedLowMemory;
    ULONG           LowMem;
    BOOL            fAudioLoaded = FALSE;
    BOOL            fVesaLoaded = FALSE;
#endif

    TRACE((TC_LIB, TT_API1, "CfgIniLoad: Connection %s, Server File %s, Protocol File %s", pConnection, pServerFile, pProtocolFile));

#ifdef DOS
    ReservedLowMemory = GetPrivateProfileLong( INI_ICA30,
                                               INI_LOWMEMRESERVE,
                                               DEF_LOWMEMRESERVE,
                                               pProtocolFile );
#endif

#ifdef RISCOS
    DefIniSect[0].pINISect = ModuleSect;
    DefIniSect[1].pINISect = WfclientSect;

    WfclientSect[0].pSectDefault = szWFThinWireSect;
    WfclientSect[1].pSectDefault = szWfclientSect;

    ModuleSect[0].pSectDefault = szTCPIPSect;
    ModuleSect[1].pSectDefault = szICA30Sect;
    ModuleSect[2].pSectDefault = szRFrameSect;
    ModuleSect[3].pSectDefault = szEncryptSect;
    ModuleSect[4].pSectDefault = szThinwire30Sect;
    ModuleSect[5].pSectDefault = szClientDriveSect;
    ModuleSect[6].pSectDefault = szClientPrinterSect;
    ModuleSect[7].pSectDefault = szClipboardSect;
#endif

    *pszDeviceName = '\0';
    pEncryption = (char*)malloc(15);
    memset(pEncryption, 0, 15);
    pTAPIModem = (char*)malloc(strlen(INI_TAPIMODEM) + 1);
    strcpy(pTAPIModem, INI_TAPIMODEM);

    /*
     *  allocate space for protocol and winstation name
     */
    if ( (pPdType = (PCHAR) malloc( MAX_INI_LINE )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    else if ( (pWdType = (PCHAR) malloc( MAX_INI_LINE )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    /*
     *  get protocol type from server file
     */
    if ( !GetPrivateProfileString( pConnection,
                                   INI_TRANSPORTDRIVER,
                                   INI_EMPTY,
                                   pPdType,
                                   MAX_INI_LINE,
                                   pServerFile ) ) {
#ifndef DOS
        /*
         * Windows ASYNC clients will have the TransportDriver specified in
         * the device, or will be implicit INI_SERIAL if TAPI is being used.
         * Check device or for TAPI before failing.
         */
        GetPrivateProfileString( pConnection,
                                 INI_DEVICE,
                                 INI_EMPTY,
                                 pszDeviceName,
                                 sizeof(pszDeviceName),
                                 pServerFile );
        if ( *pszDeviceName ) {

            /*
             * Check for TAPI first
             */
            GetPrivateProfileString( pConnection,
                                     INI_TAPIDEVICE,
                                     INI_OFF,
                                     pszBuffer,
                                     sizeof(pszBuffer),
                                     pServerFile );

            if ( !stricmp(pszBuffer, INI_ON) ) {

                bTapi = TRUE;
                strcpy(pPdType, INI_SERIAL);    // TAPI uses standard COM

            } else {

                GetPrivateProfileString( pszDeviceName,
                                         INI_TRANSPORTDRIVER,
                                         INI_SERIAL,
                                         pPdType,
                                         MAX_INI_LINE,
                                         pConfigFile );
            }
        }

        /*
         * if not an async connection, then default to TCP/IP for Internet
         * client.
         */
        if ( !(*pPdType) ) {
            strcpy(pPdType, INI_TCP);
        }
#else
         rc = CLIENT_ERROR_PD_ENTRY_NOT_FOUND;
         goto done;
#endif
    }
    TRACE((TC_LIB, TT_API1, "CfgIniLoad: Protocol Type '%s'", pPdType));

    /*
     *  get winstation type from server file
     */
    if ( !GetPrivateProfileString( pConnection,
                                   INI_TYPE,
                                   DEF_TYPE,
                                   pWdType,
                                   MAX_INI_LINE,
                                   pServerFile ) ) {
        rc = CLIENT_ERROR_WD_ENTRY_NOT_FOUND;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, "CfgIniLoad: WinStation Type %s", pWdType));


#ifndef DOS
    /*
     * find out if this connection was made by an Internet client
     * this will update the bInternetConnection boolean.
     */
    i = 0;
    bInternetConnection = FALSE;
    while ( pCfgIniOverrides[i].pszKey ) {
      if  ( stricmp(pCfgIniOverrides[i].pszKey, INI_CLIENTTYPE ) == 0) {
               if (stricmp(pCfgIniOverrides[i].pszValue, INTERNET_CLIENT) == 0)  {
                    bInternetConnection = TRUE;   // special message only from
                                                  //  Internet clients
               }
               break;
      }
      else
       i++;
    }
#endif

    /*
     * Make a buffered profile from all our files and CfgIniOverrides
     * and load the session.
     */
    if ( rc = MakeProfileAndLoadSession( hClientHandle, pConnection,
                                         pPdType, pCfgIniOverrides,
                                         pServerFile, pConfigFile,
                                         pProtocolFile, bInternetConnection ) ) {
        goto done;
    }

    /*
     * Fetch the device section if specified.  (NOTE: Windows ASYNC will
     * have already filled-in pszDeviceName at this point).
     */
    if ( !*pszDeviceName ) {
        GetString( pPdType, pProtocolFile, pConnection, pServerFile,
                   INI_DEVICE, pszDeviceName, sizeof(pszDeviceName) );
    }
    TRACE((TC_LIB, TT_API1, "CfgIniLoad: Device(%s)", pszDeviceName ));

    /*
     *  load transport Pd
     */
    if ( rc = LoadPdDll( hClientHandle, pPdType, pProtocolFile ) ) {
        goto done;
    }


#ifndef DOS
    //We need to find out if this is a non-encrypted internet client..
    // ....backward compatibility. we will not call this an internet client/connection :)
    if ( bInternetConnection ) {
      GetPrivateProfileString(pConnection, INI_ENCRYPTIONLEVELSESSION,
                              "Old",
                              szEncryptionLevelSession,
                              sizeof(szEncryptionLevelSession),
                              pServerFile);
      bInternetConnection = (strcmp(szEncryptionLevelSession, "Old") != 0);
    }
#endif

    /*
     *  Build PD list and load in order.
     */
    for ( pSave = pList = BuildList( pProtocolFile, pWdType,
                                     pProtocolFile, pPdType, PROTOCOL_SUPPORT );
          pList != NULL;
          pList = pList->pNext ) {

        /*
         *  validate current list element
         */
        if ( !pList->bValid )
            continue;

        /*
         *  check if current Pd is required
         */

            if ( !SupportRequired( pPdType, pConnection,
                                   pList->pElement, pszDeviceName,
                                   pProtocolFile, pConfigFile, pServerFile,
                                   bTapi ) )
            continue;

         //trap here  for the special processing for Encrypt Pd.
         if ( stricmp(pList->pElement, DEF_ENCRYPTIONDLL) == 0 ) {

             bEncryption = TRUE;   // this connection needs Encryption Pd

             // get the encryption Level from the CfgIniOverrrides

             i = 0;
             while ( pCfgIniOverrides[i].pszKey ) {
                 if ( stricmp(pCfgIniOverrides[i].pszKey, INI_ENCRYPTIONDLL ) == 0) {
                      strcpy(pEncryption, pCfgIniOverrides[i].pszValue);
                      free(pList->pElement);            // free old list element
                      pList->pElement = pEncryption;    // and assign new
                      break;
                 }
                 else
                     i++;
             }

         }

        // Special processing for TAPI modem
        if ( (stricmp(pList->pElement, "Modem") == 0) && bTapi ) {

            free(pList->pElement);          // free old list element
            pList->pElement = pTAPIModem;   // and assign new
        }

        /*
         *  load the current Pd
         */
         if ( bEncryption && bInternetConnection) {

             /*
              * for Internet connections we will use the section in
              * the ICAFile (ServerFile) to get the encryption *.dll
              */
            if ( rc = LoadPdDll( hClientHandle, pList->pElement, pServerFile ) ) {
                goto done;
            }
         }
         else {
            if ( rc = LoadPdDll( hClientHandle, pList->pElement, pProtocolFile ) ) {
                goto done;
            }

         }
    }

    /*
     *  free pd list memory
     */
    DestroyList( pSave );
    pSave = NULL;


    /*
     *  load winstation driver next
     */
    if ( rc = LoadWdDll( hClientHandle, pWdType, pProtocolFile ) ) {
        goto done;
    }


    /*
     *  Build VD list and load in order.
     */
    for ( pSave = pList = BuildList( pProtocolFile, pWdType,
                                     NULL, NULL, VIRTUAL_DRIVER );
          pList != NULL;
          pList = pList->pNext ) {

        TRACE(( TC_LIB, TT_API1, "CfgIniLoad: vdSection(%s)", pList->pElement ));


        /*
         *  check if Client Audio Mapping has been disabled.
         */

        if ( strcmp(pList->pElement, INI_CAM ) == 0) {
           GetPrivateProfileString(  pConnection,
                                     INI_CAM,
                                     (DEF_CAM ? "On" : "Off"),
                                     OnOff,
                                     sizeof(OnOff),
                                     pServerFile );

           if ( strcmp(OnOff,"Off") == 0)
           continue;
#ifdef DOS
           /*
            * We are trying to load audio.
            */
           fAudioLoaded = TRUE;
#endif
        }

        /*
         *  check if Client Updates has been disabled.
         */
        if ( strcmp(pList->pElement, INI_CM_VDSECTION ) == 0) {
           GetPrivateProfileString(  INI_WFCLIENT,
                                     INI_CM_UPDATESALLOWED,
                                     (DEF_CM_UPDATESALLOWED ? "On" : "Off"),
                                     OnOff,
                                     sizeof(OnOff),
                                     pServerFile );

           if ( stricmp(OnOff,"Off") == 0)
           continue;
        }

        /*
         *  check if Client COM port mapping has been disabled.
         */
        if ( strcmp(pList->pElement, INI_COMSECTION ) == 0) {
           GetPrivateProfileString(  INI_WFCLIENT,
                                     INI_COMALLOWED,
                                     (DEF_COMALLOWED ? "On" : "Off"),
                                     OnOff,
                                     sizeof(OnOff),
                                     pServerFile );

           if ( stricmp(OnOff,"Off") == 0)
           continue;
        }

        /*
         *  check if Client printer mapping has been disabled.
         */
        if ( strcmp(pList->pElement, INI_CPMSECTION ) == 0) {
           GetPrivateProfileString(  INI_WFCLIENT,
                                     INI_CPMALLOWED,
                                     (DEF_CPMALLOWED ? "On" : "Off"),
                                     OnOff,
                                     sizeof(OnOff),
                                     pServerFile );

           if ( stricmp(OnOff,"Off") == 0)
           continue;
        }

        /*
         *  check if Client disk mapping has been disabled.
         */
        if ( strcmp(pList->pElement, INI_CDMSECTION ) == 0) {
           GetPrivateProfileString(  INI_WFCLIENT,
                                     INI_CDMALLOWED,
                                     (DEF_CDMALLOWED ? "On" : "Off"),
                                     OnOff,
                                     sizeof(OnOff),
                                     pServerFile );

           if ( stricmp(OnOff,"Off") == 0)
           continue;
        }

        GetPrivateProfileString(  pList->pElement,
                                  INI_DRIVERNAMEALT,
                                  INI_DRIVERNAME,
                                  pszBuffer,
                                  sizeof(pszBuffer),
                                  pConfigFile );

        pszDriver = pszBuffer;

#ifdef DOS
        /*
         * We are trying to load VESA
         */
        if ( strcmp( pszDriver, INI_VDTWVESA ) == 0 )
            fVesaLoaded = TRUE;

        /*
         * Don't bother loading the Virtual driver if there isn't enough memory.
         */
        QueryHeap( NULL, &LowMem, NULL );

        if ( LowMem < ReservedLowMemory ) {
            TRACE(( TC_LIB, TT_API1, "Not enough memory before loading driver %s", pList->pElement ));
            rc = CLIENT_ERROR_NO_MEMORY;
            goto done;
        }
#endif

        /*
         *  load the current VD
         */
        if ( rc = LoadVdDll( hClientHandle, pList->pElement, pProtocolFile, pszDriver ) )  {
            switch ( rc ) {
                case CLIENT_ERROR_DRIVER_UNSUPPORTED:  // Non-fatal error
                case CLIENT_ERROR_DRIVER_BAD_CONFIG:
                   rc = CLIENT_STATUS_SUCCESS;
                   break;

                default:
                   goto done;
                   break;
            }
        }
    }


done:
    if (!bEncryption)
       free(pEncryption);
    if (!bTapi)
       free(pTAPIModem);

    if ( pSave )
        DestroyList( pSave );

    //if ( pList )
    //    DestroyList( pList );

    if ( pPdType )
        (void) free( pPdType );

    if ( pWdType )
        (void) free( pWdType );

#ifdef DOS
    /*
     * On success, make sure we have enough memory to continue.
     */
    if ( rc == CLIENT_STATUS_SUCCESS ) {

        QueryHeap( NULL, &LowMem, NULL );

        if ( LowMem < ReservedLowMemory ) {
            TRACE(( TC_LIB, TT_API1, "Not enough memory after modules loaded %lx", LowMem ));
            rc = CLIENT_ERROR_NO_MEMORY;
        }
    }
#endif
#if defined(DOS) || defined(RISCOS)
    /*
     * On success, flush INI cache to save memory prior to connection
     * SJM: and to close the files.
     */
    if ( rc == CLIENT_STATUS_SUCCESS )
        FlushPrivateProfileCache();

#endif
    /*
     *  On error unload drivers
     */
    if ( rc != CLIENT_STATUS_SUCCESS )
       WFEngUnloadDrivers( hClientHandle );

#ifdef DOS
    if ( rc == CLIENT_ERROR_NO_MEMORY ) {
        if ( fAudioLoaded && fVesaLoaded )
            rc = CLIENT_ERROR_NO_MEMORY_LOAD_AUDIO_VESA;
        else if ( fAudioLoaded )
            rc = CLIENT_ERROR_NO_MEMORY_LOAD_AUDIO;
        else if ( fVesaLoaded )
            rc = CLIENT_ERROR_NO_MEMORY_LOAD_VESA;
        else 
            rc = CLIENT_ERROR_NO_MEMORY_LOAD;
    }
#endif

    return( rc );
}


/*******************************************************************************
 *
 *  MakeProfileAndLoadSession
 *
 *    Will combine the cfgIniOverrides with all the sections found in the
 *    INI files to form one buffered INI profile. When a section is found
 *    in more than one file, the entries are combined with the following
 *    precedence: overrides, server file, config file, protocol file
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pConnection (input)
 *          Name of connection entry in server file.
 *      pTransportDriver
 *          name of this connection's transport driver
 *      pCfgIniOverrides (input)
 *          NULL for no overrides, or points to array of CFGINIOVERRIDE
 *          structures of key / value pairs to combine with [Server]
 *          section of the buffered INI profile.
 *      pServerFile (input)
 *          Server INI file name.
 *      pConfigFile (input)
 *          Config INI file name.
 *      pProtocolFile (input)
 *          Protocol INI file name.
 *      bInternet (input)
 *          Signifies if this is an inetrnet connection in
 *          which case we only work with the server file
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
MakeProfileAndLoadSession( HANDLE hClientHandle,
                           PCHAR pConnection,
                           PCHAR pTransportDriver,
                           PCFGINIOVERRIDE pCfgIniOverrides,
                           PCHAR pServerFile,
                           PCHAR pConfigFile,
                           PCHAR pProtocolFile,
                           BOOL  bInternet )
{
    int     rc,size,sections;
    PCHAR   pSection1     = NULL, pTempSec1     = NULL,
            pSection2     = NULL, pTempSec2     = NULL,
            pSection3     = NULL, pTempSec3     = NULL,
            pSerialSec    = NULL, pSerialTmp    = NULL,
            pTransportSec = NULL, pTransportTmp = NULL,
            pMergedSec    = NULL, pFinalSection = NULL;

    PSECTIONLIST   pSectionsList = NULL;     //list of all sections to retrieve
    PSECTIONLIST   pSectionsHead = NULL;
    PSECTIONLIST   pSectionsTemp = NULL;
    PSECTIONLIST   pNew = NULL;

    PLIST   pNewOverwrite, pList;
    PLIST   pOverwrites = NULL;
    PCHAR   pSerialPort = NULL;
    PCHAR   nullPointer = NULL;
    PCHAR   pTemp;
    int     len;

#ifndef DOS
    PDEFINISECT    pDefIniSect;
#endif
    WFELOAD        WFELoad;

    /*
     * If this is an async connection we need to know which serial port
     * will be in use. There will be a special SerialPort Section with
     * all the information regarding the port in use for this connection
     */
    pSerialPort = (PCHAR) malloc(MAX_INI_LINE);
    GetPrivateProfileString(pConnection, INI_DEVICE, INI_EMPTY,
                            pSerialPort, MAX_INI_LINE, pServerFile);

    /*
     * Build the [Server] Section which will act as the center of
     * INI information about the current connection session and the host.
     * This Section will be filled first with the cfgIniOverride
     *    entries which shall have top priority.
     * The section specific to the current connection is then added in.
     */
    pSection1 = BuildCfgIniOverridesSection( pCfgIniOverrides );
    if (pSection1 == NULL) {
       rc = CLIENT_ERROR_BAD_OVERRIDES;
       goto done;
    }
    if (rc = GetSection(pConnection, pServerFile, &pSection2, pOverwrites)) {
       if (rc == -1) {
          rc = CLIENT_ERROR_MISSING_CONNECTION_SECTION;
       }
       goto done;
    }
    if ( (pMergedSec =
          CombinePrivateProfileEntries(pSection1, pSection2)) == NULL) {
       rc = CLIENT_ERROR_BAD_COMBINE_ENTRIES;
       goto done;
    }

    //clean up the used sections for repeated use
    free(pSection1);
    free(pSection2);
    pSection1 = pSection2 = NULL;


    /*
     * Build a list of all entries in the profile so far so that they can be
     * overwritten into any subsequent sections that have the same key
     */
    pTemp = pMergedSec;
    while (len = strlen(pTemp)) {
       if (pTemp[0] != '[') {

          pNewOverwrite = (PLIST) malloc(sizeof(LIST));
          pNewOverwrite->pElement = strdup(pTemp);
          pNewOverwrite->pNext = NULL;
          if (pOverwrites == NULL) {
             pOverwrites = pList = pNewOverwrite;
          } else {
             pList->pNext = pNewOverwrite;
             pList = pList->pNext;
          }
       }
       pTemp += (len + 1);
    }

    /*
     * Get the WFClient section now because it is special and will be used
     * as overwrite information along with the Server section
     */
    if (rc = GetSection(INI_WFCLIENT, pServerFile, &pSection1, pOverwrites)) {
       if (rc == -1) {
          //don't fail if the ICAFile does not have a WFClient section
          goto skipcombine1;
       }
       goto done;
    }

    //Construct a ClientName=XXX entry and add to the buffered secction
    if ((pTempSec1 =
         AddEntrySection(pSection1, INI_CLIENTNAME, gszClientName)) == NULL) {
       rc = CLIENT_ERROR_BAD_ENTRY_INSERTION;
       goto done;
    }
    free(pSection1);
    pSection1 = pTempSec1;
    pTempSec1 = NULL;

    //add the section name as header to the sections we just retrieved
    if ((pTempSec1 =
         AddHeaderSection(pSection1, INI_WFCLIENT)) == NULL) {
       rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
       goto done;
    }
    free(pSection1);
    pSection1 = NULL;

#ifndef DOS
    if (bInternet) {
       //merge WFClient section into the profile
       if ((pSection1 =
            ConcatSections(pMergedSec, pTempSec1)) == NULL) {
          rc = CLIENT_ERROR_BAD_CONCAT_SECTIONS;
          goto done;
       }
       free(pTempSec1);
       free(pMergedSec);

       pMergedSec = pSection1;
       pTempSec1 = pSection1 = NULL;

       //skip the other two files since they don't exist in internet conn
       goto skipcombine1;
    }
#endif

    /*
     * Get the WFClient section from the other two files and merge
     */
    if (rc = GetSection(INI_WFCLIENT, pConfigFile, &pSection2, pOverwrites)) {
       if (rc == -1) {
          rc = CLIENT_ERROR_MISSING_WFCLIENT_SECTION;
       }
       goto done;
    }
    if (rc = GetSection(INI_WFCLIENT, pProtocolFile, &pSection3, pOverwrites)) {
       if (rc == -1) {
          rc = CLIENT_ERROR_MISSING_WFCLIENT_SECTION;
       }
       goto done;
    }

    if ((pTempSec2 =
         AddHeaderSection(pSection2, INI_WFCLIENT)) == NULL) {
       rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
       goto done;
    }
    if ((pTempSec3 =
         AddHeaderSection(pSection3, INI_WFCLIENT)) == NULL) {
       rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
       goto done;
    }
    free(pSection2);
    free(pSection3);
    pSection2 = pSection3 = NULL;

    /*
     * Combine the entries of the ConfigFile's WFClient with the
     *   ServerFile's WFClient sections making the ones from
     *   ServerFile the highest priority.
     */
    if ((pSection1 =
         CombinePrivateProfileEntries(pTempSec1, pTempSec2)) == NULL) {
       rc = CLIENT_ERROR_BAD_COMBINE_ENTRIES;
       goto done;
    }
    free(pTempSec1);
    free(pTempSec2);
    pTempSec1 = pTempSec2 = NULL;

    /*
     * Combine the entries of the ModuleFile's WFClient with the
     *   previously combined Server and Config files' sections
     *   making the previous ones the highest priority.
     */
    if ((pSection2 =
         CombinePrivateProfileEntries(pSection1, pTempSec3)) == NULL) {
       rc = CLIENT_ERROR_BAD_COMBINE_ENTRIES;
       goto done;
    }
    free(pSection1);
    free(pTempSec3);
    pSection1 = pTempSec3 = NULL;


    //add our new WFClient section onto the buffered profile
    if ((pTempSec1 =
         ConcatSections(pMergedSec, pSection2)) == NULL) {
       rc = CLIENT_ERROR_BAD_CONCAT_SECTIONS;
       goto done;
    }
    free(pSection2);
    free(pMergedSec);

    pMergedSec = pTempSec1;
    pSection2 = pTempSec1 = NULL;

skipcombine1:
    /*
     * Build a list of all entries in the profile so far so that they can be
     * overwritten into any subsequent sections that have the same key
     */
    DestroyList(pOverwrites);
    pOverwrites = NULL;

    pTemp = pMergedSec;
    while (len = strlen(pTemp)) {
       if (pTemp[0] != '[') {

          pNewOverwrite = (PLIST) malloc(sizeof(LIST));
          pNewOverwrite->pElement = strdup(pTemp);
          pNewOverwrite->pNext = NULL;
          if (pOverwrites == NULL) {
             pOverwrites = pList = pNewOverwrite;
          } else {
             pList->pNext = pNewOverwrite;
             pList = pList->pNext;
          }
       }
       pTemp += (len + 1);
    }

    /*
     * Now we shall get a list of all the sections we want to
     * add into the profile. This will consist of all sections in
     * the protocol file except those transport drivers that are not
     * currently active, all sections in the config file, and
     * all sections in the server file except those ApplicationServer
     * sections not currently connected to.
     */

#ifndef DOS
    if (bInternet) {
       /*
        * only build a list out of the server file since internet
        * connections only have a server file
        */
       pSectionsList = BuildAllSectionsList(pConnection, pTransportDriver,
                                    pServerFile, nullPointer, nullPointer);

       /*
        * Include the transport driver section if it is not TCP/IP and available.
        * This could happen if launching an IPX ICA file for example
        * with the full client installed
        */

       if (stricmp(pTransportDriver, INI_TCP)) {
          pNew = (PSECTIONLIST) malloc(sizeof(SECTIONLIST));
          pNew->pElement = strdup(pTransportDriver);
          pNew->pFile    = strdup(pProtocolFile);
          pNew->bValid   = 2;
          pNew->pNext    = pSectionsList;
          pSectionsList  = pNew;
       }

       /*
        * Also include the sections defined in the default inidef.h
        */
       pDefIniSect = ModuleSect;
       while(pDefIniSect->pSectName != NULL) {
          //add to list
          if (strcmp(INI_WFCLIENT, pDefIniSect->pSectName)) {
             pNew = (PSECTIONLIST) malloc(sizeof(SECTIONLIST));
             pNew->pElement = strdup(pDefIniSect->pSectName);
             pNew->pFile    = strdup(pProtocolFile);
             if (!strcmp(pTransportDriver, pDefIniSect->pSectName)) {
                pNew->bValid = 2;   //indicates special transport section
             } else {
                pNew->bValid = 1;
             }
             pNew->pNext = pSectionsList;
             pSectionsList = pNew;
          }
          pDefIniSect++;
       }
       pDefIniSect = WfclientSect;
       while(pDefIniSect->pSectName != NULL) {
          //add to list
          if (strcmp(INI_WFCLIENT, pDefIniSect->pSectName)) {
             pNew = (PSECTIONLIST) malloc(sizeof(SECTIONLIST));
             pNew->pElement = strdup(pDefIniSect->pSectName);
             pNew->pFile    = strdup(pConfigFile);
             pNew->bValid = 1;
             pNew->pNext = pSectionsList;
             pSectionsList = pNew;
          }
          pDefIniSect++;
       }

    } else {
#endif
       pSectionsList = BuildAllSectionsList(pConnection, pTransportDriver,
                                  pServerFile, pConfigFile, pProtocolFile);
#ifndef DOS
    }
#endif

    pSectionsHead = pSectionsList;
    pSectionsTemp = pSectionsList;
    while (pSectionsList != NULL) {

       TRACE((TC_LIB, TT_API1, "MakeProfileAndLoadSession: bValid %d Element '%s'", pSectionsList->bValid, pSectionsList->pElement));

       if (pSectionsList->bValid) {

          if (rc = GetSection(pSectionsList->pElement,
                   pSectionsList->pFile, &pSection1, pOverwrites)) {
             if (rc == -1) {
                rc = CLIENT_ERROR_MISSING_SECTION;
             }
             goto done;
          }

          /*
           * if this section is the current connection's serial port
           * then copy this into the generic SerialPortSection
           */
          if (!strcmp(pSerialPort, pSectionsList->pElement)) {

             //make a copy of the section
             len = IniSize(pSection1);
             pSerialSec = (PCHAR) malloc(len);
             memcpy(pSerialSec, pSection1, len);

             //add header to new copy
             pSerialTmp =
                AddHeaderSection(pSerialSec, INI_SERIAL_VER1);
             if (pSerialTmp == NULL) {
                rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
                goto done;
             }
             free(pSerialSec);
             pSerialSec = NULL;

             //add new serial section into buffered profile
             pSerialSec =
                ConcatSections(pMergedSec, pSerialTmp);
             if (pSerialSec == NULL) {
                rc = CLIENT_ERROR_BAD_CONCAT_SECTIONS;
                goto done;
             }
             free(pMergedSec);
             free(pSerialTmp);
             pMergedSec = pSerialSec;
             pSerialSec = pSerialTmp = NULL;
          }

          /*
           * We must make a general [Transport] section based on
           * which transport driver is to be loaded so that common
           * routines know where to search for info.
           * bValid == 2 if the magic number to signify that this is
           * the transport section
           */
          if (pSectionsList->bValid == 2) {

               //make a copy of section1 the transportdriver section
               len = IniSize(pSection1);
               pTransportSec = (PCHAR) malloc(len);
               memcpy(pTransportSec, pSection1, len);

               //add header to new copy
               pTransportTmp =
                  AddHeaderSection(pTransportSec, INI_TRANSPORTSECTION);
               if (pTransportTmp == NULL) {
                  rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
                  goto done;
               }
               free(pTransportSec);
               pTransportSec = NULL;

               //add new transport section into buffered profile
               pTransportSec =
                  ConcatSections(pMergedSec, pTransportTmp);
               if (pTransportSec == NULL) {
                  rc = CLIENT_ERROR_BAD_CONCAT_SECTIONS;
                  goto done;
               }
               free(pMergedSec);
               free(pTransportTmp);
               pMergedSec = pTransportSec;
               pTransportSec = pTransportTmp = NULL;
          }

          pTempSec1 =
             AddHeaderSection(pSection1, pSectionsList->pElement);
          if (pTempSec1 == NULL) {
             rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
             goto done;
          }
          free(pSection1);
          pSection1 = NULL;
          sections = 1;
          pSectionsList->bValid = 0;      //to ensure we dont repeat a section

          /*
           * Search through rest of list and see if the other 2 files
           * had sections with the same name
           */
          pSectionsTemp = pSectionsList;
          while (pSectionsTemp != NULL) {

             if (!strcmp(pSectionsList->pElement, pSectionsTemp->pElement) &&
                 (pSectionsTemp->bValid)) {

                sections++;
                pSectionsTemp->bValid = 0;

                if (sections == 2) {

                   if (rc = GetSection(pSectionsTemp->pElement,
                              pSectionsTemp->pFile, &pSection2, pOverwrites)) {
                      if (rc == -1) {
                         rc = CLIENT_ERROR_MISSING_SECTION;
                      }
                      goto done;
                   }
                   if ((pTempSec2 = AddHeaderSection(
                            pSection2, pSectionsTemp->pElement)) == NULL) {
                      rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
                      goto done;
                   }
                   free(pSection2);
                   pSection2 = NULL;

                   pSection1 =
                      CombinePrivateProfileEntries(pTempSec1, pTempSec2);
                   if (pSection1 == NULL) {
                      rc = CLIENT_ERROR_BAD_COMBINE_ENTRIES;
                      goto done;
                   }
                   free(pTempSec1);
                   free(pTempSec2);

                   //we need for consistency pTempSec1 to point to our working buffer
                   pTempSec1 = pSection1;
                   pSection1 = pTempSec2 = NULL;

                } else {
                   if (sections > 3) {
                      rc = CLIENT_ERROR_DUPLICATE_SECTIONS;
                      goto done;
                   }

                   if (rc = GetSection(pSectionsTemp->pElement,
                         pSectionsTemp->pFile, &pSection3, pOverwrites)) {
                      if (rc == -1) {
                         rc = CLIENT_ERROR_MISSING_SECTION;
                      }
                      goto done;
                   }
                   if ((pTempSec3 = AddHeaderSection(pSection3,
                                    pSectionsTemp->pElement)) == NULL) {
                      rc = CLIENT_ERROR_BAD_HEADER_INSERTION;
                      goto done;
                   }
                   free(pSection3);
                   pSection3 = NULL;

                   if (( pSection1 = CombinePrivateProfileEntries(
                           pTempSec1, pTempSec3)) == NULL) {
                      rc = CLIENT_ERROR_BAD_COMBINE_ENTRIES;
                      goto done;
                   }
                   free(pTempSec1);
                   free(pTempSec3);

                   //we need for loop consistency pTempSec1 to point to our working buffer
                   pTempSec1 = pSection1;
                   pSection1 = pTempSec3 = NULL;
                }
             }
             pSectionsTemp = pSectionsTemp->pNext;
          }

          pSection1 = ConcatSections(pMergedSec, pTempSec1);
          if (pSection1 == NULL) {
             rc = CLIENT_ERROR_BAD_CONCAT_SECTIONS;
             goto done;
          }
          free(pTempSec1);
          free(pMergedSec);

          //we need for loop consistency pMergedSection to point to our working buffer
          pMergedSec = pSection1;
          pSection1 = pTempSec1 = NULL;

       }
       pSectionsList = pSectionsList->pNext;
    }

    /*
     * We need to add a Magic Number to the front of our buffer to indicate
     * that it is of the new-styled buffered profile structure.
     * This will simply be a NULL at the head of the buffer.
     */
    size = IniSize(pMergedSec);
    pFinalSection = (PCHAR) malloc(size + 1);
    pFinalSection[0] = '\0';
    memcpy( &(pFinalSection[1]), pMergedSec, size);
    free(pMergedSec);
    pMergedSec = NULL;

    /*
     * Load the WFENGINE session.
     */

// BUGBUG: need a way to pass this stuff in:
#ifdef DOS
    WFELoad.pszModuleName = (LPSTR)"wfclient.exe\0uicwin.ddl\0uiiniw.ddl\0uihelp.ddl\0uierror.ddl\0\0";
#endif
#ifdef WIN16
    WFELoad.pszModuleName = (LPSTR)"wfica16.exe\0\0";
#endif
#ifdef WIN32
    WFELoad.pszModuleName = (LPSTR)"wfica32.exe\0\0";
#endif

    WFELoad.pIniSection = (LPVOID)pFinalSection;
    rc = WFEngLoadSession( hClientHandle, &WFELoad );

done:
    DestroySList( pSectionsHead );
    DestroyList( pOverwrites );

    if ( pSection1 ) {
       (void) free(pSection1);
    }
    if ( pSection2 ) {
       (void) free(pSection2);
    }
    if ( pSection3 ) {
       (void) free(pSection3);
    }
    if ( pMergedSec ) {
       (void) free(pMergedSec);
    }
    if ( pTempSec1 ) {
       (void) free(pTempSec1);
    }
    if ( pTempSec2 ) {
       (void) free(pTempSec2);
    }
    if ( pTempSec3 ) {
       (void) free(pTempSec3);
    }
    if ( pFinalSection ) {
       (void) free(pFinalSection);
    }
    if ( pTransportSec ) {
       (void) free(pTransportSec);
    }
    if ( pTransportTmp ) {
       (void) free(pTransportTmp);
    }
    if ( pSerialSec ) {
       (void) free(pSerialSec);
    }
    if ( pSerialTmp ) {
       (void) free(pSerialTmp);
    }
    if ( pSerialPort ) {
       (void) free(pSerialPort);
    }

    return( rc );
}

/*******************************************************************************
 *
 *  LoadPdDll
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pPdSection (input)
 *          Name of the VdSection to look in to find drivername.
 *      pFile (input)
 *          File to get the module name from
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
LoadPdDll( HANDLE hClientHandle, PCHAR pPdSection, PCHAR pFile )
{
    int     rc;
    PCHAR   pModuleName;
    PCHAR   pIniSection;
    WFELOAD WFELoad;

    /*
     * Set the Buffered Ini Section to empty since we are using the new
     * buffered profile structure for INI information. We must still pass
     * a buffered INI section to the engine for backward compatibility.
     * The double NULL is necessary as opposed to a NULL pointer since
     * procedures in the IPC required non-null buffer pointers.
     */
    pIniSection = (PCHAR) malloc(2);
    pIniSection[0] = 0;
    pIniSection[1] = 0;

    pModuleName = (PCHAR) malloc(MAX_INI_LINE);

    if ( !GetPrivateProfileString(pPdSection, INI_DRIVERNAME, INI_EMPTY,
                                  pModuleName, MAX_INI_LINE, pFile)) {
#ifndef DOS
       if (!GetPrivateProfileStringDefault(pPdSection, INI_DRIVERNAME,
            pModuleName, MAX_INI_LINE, pFile)) {
#endif
          rc = CLIENT_ERROR_PD_NAME_NOT_FOUND;
          goto done;
#ifndef DOS
       }
#endif
    }

    if ( !strcmp(pModuleName, INI_DRIVERUNSUPPORTED)) {
       rc = CLIENT_ERROR_DRIVER_UNSUPPORTED;
       goto done;
    }

    WFELoad.pszModuleName = (LPSTR)pModuleName;
    WFELoad.pIniSection = (LPVOID)pIniSection;
    rc = WFEngLoadPd( hClientHandle, &WFELoad );

done:
    if ( pModuleName )
        (void) free(pModuleName);
    free(pIniSection);

    return( rc );
}

/*******************************************************************************
 *
 *  LoadWdDll
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pWdSection (input)
 *          Name of the VdSection to look in to find drivername.
 *      pProtocolFile (input)
 *          File to get the module name from
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
LoadWdDll( HANDLE hClientHandle, PCHAR pWdSection, PCHAR pProtocolFile )
{
    int     rc;
    PCHAR   pModuleName;
    PCHAR   pIniSection;
    WFELOAD WFELoad;

    pIniSection = (PCHAR) malloc(2);
    pIniSection[0] = 0;
    pIniSection[1] = 0;

    pModuleName = (PCHAR) malloc(MAX_INI_LINE);

    if ( !GetPrivateProfileString(pWdSection, INI_DRIVERNAME, INI_EMPTY,
                                  pModuleName, MAX_INI_LINE, pProtocolFile)) {
#ifndef DOS
       if (!GetPrivateProfileStringDefault(pWdSection, INI_DRIVERNAME,
            pModuleName, MAX_INI_LINE, pProtocolFile)) {
#endif
          rc = CLIENT_ERROR_WD_NAME_NOT_FOUND;
          goto done;
#ifndef DOS
       }
#endif
    }

    if ( !strcmp(pModuleName, INI_DRIVERUNSUPPORTED)) {
       rc = CLIENT_ERROR_DRIVER_UNSUPPORTED;
       goto done;
    }

    WFELoad.pszModuleName = (LPSTR)pModuleName;
    WFELoad.pIniSection = (LPVOID)pIniSection;
    rc = WFEngLoadWd( hClientHandle, &WFELoad );

done:
    if ( pModuleName )
        (void) free(pModuleName);
    free(pIniSection);

    return( rc );
}

/*******************************************************************************
 *
 *  LoadVdDll
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pVdSection (input)
 *          Name of the VdSection to look in to find drivername.
 *      pProtocolFile (input)
 *          File to get the module name from
 *      pDriverName (input)
 *          Name of the driver
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
LoadVdDll( HANDLE hClientHandle, PCHAR pVdSection, PCHAR pProtocolFile, PCHAR pDriverName )
{
    int     rc;
    PCHAR   pModuleName;
    WFELOAD WFELoad;
    PCHAR   pIniSection;

    pIniSection = (PCHAR) malloc(2);
    pIniSection[0] = 0;
    pIniSection[1] = 0;

    pModuleName = (PCHAR) malloc(MAX_INI_LINE);

    if ( !GetPrivateProfileString(pVdSection, pDriverName, INI_EMPTY,
                                  pModuleName, MAX_INI_LINE, pProtocolFile)) {
#ifndef DOS
       if (!GetPrivateProfileStringDefault(pVdSection, pDriverName,
            pModuleName, MAX_INI_LINE, pProtocolFile)) {
#endif
          rc = CLIENT_ERROR_VD_NAME_NOT_FOUND;
          goto done;
#ifndef DOS
       }
#endif
    }

    if ( !strcmp(pModuleName, INI_DRIVERUNSUPPORTED)) {
       rc = CLIENT_ERROR_DRIVER_UNSUPPORTED;
       goto done;
    }

    WFELoad.pszModuleName = (LPSTR)pModuleName;
    WFELoad.pIniSection = (LPVOID)pIniSection;
    rc = WFEngLoadVd( hClientHandle, &WFELoad );

done:
    if ( pModuleName )
        (void) free(pModuleName);
     free(pIniSection);

    return( rc );
}

/*******************************************************************************
 *
 *  SupportRequired
 *       See if the current driver is supported as specified in the INI files
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
SupportRequired( PCHAR pPSec, PCHAR pSSec, PCHAR pEntry, PCHAR pDeviceName,
                 PCHAR pProtocolFile, PCHAR pConfigFile, PCHAR pServerFile,
                 BOOL bTapi )
{
    int   rc = FALSE;           // support not required
    CHAR  achAnswer[10];

    // if we are searching for modem=on then check in the device section
    // (always return TRUE if TAPI is specified)
    if ( !stricmp( pEntry, INI_MODEM ) ) {

       if ( bTapi )
          strcpy(achAnswer, INI_ON);
       else
          GetPrivateProfileString( pDeviceName, pEntry, INI_EMPTY,
                                   achAnswer, sizeof(achAnswer), pConfigFile );
    } else {
       GetString( pPSec, pProtocolFile, pSSec, pServerFile, pEntry, achAnswer, 10 );
    }

    /*
     *  note answer
     */
    if ( !strnicmp( achAnswer, INI_ON, 2 ) ) {
        rc = TRUE;
    }

    TRACE((TC_LIB, TT_API1, "SupportRequired: %s support needed? %s", pEntry, (rc ? "Yes" : "No")));
    return( rc );
}

/*******************************************************************************
 *
 *  BuildList
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

PLIST
BuildList( PCHAR pPFile, PCHAR pPSec, PCHAR pSFile, PCHAR pSSec, PCHAR pEntry )
{
    PLIST pRoot = NULL;
    PLIST pList = NULL;
    PLIST pTemp = NULL;
    PCHAR pEntryBuf = NULL;
    PCHAR pEntryElement;
    PCHAR p;
    CHAR  c;

    /*
     *  allocate space for entry string
     */
    if ( (pEntryBuf = (PCHAR) malloc( MAX_INI_LINE )) == NULL )
        goto done;

    /*
     *  get entry string from primary file and primary section
     */
    if ( !GetPrivateProfileString( pPSec,
                                   pEntry,
                                   INI_EMPTY,
                                   pEntryBuf,
                                   MAX_INI_LINE,
                                   pPFile ) ) {
#ifndef DOS
        if( !GetPrivateProfileStringDefault( pPSec,
                                             pEntry,
                                             pEntryBuf,
                                             MAX_INI_LINE,
                                             pPFile) )
#endif
        goto second;
    }

    /*
     *  walk entry string and build list
     */
    for ( pEntryElement=GetFirstElement(pEntryBuf);
          pEntryElement!=NULL;
          pEntryElement=GetNextElement(pEntryElement) ) {

        //  null terminate first driver in list, but first save
        if ( (p=strpbrk( pEntryElement, " ,\t\n" )) != NULL ) {
             c = *p;
            *p = 0;
        }

        //  create an empty list entry
        if ( (pTemp = (PLIST) malloc( sizeof(LIST) )) != NULL ) {

            //  fill in element
            pTemp->pElement = strdup( pEntryElement );
            pTemp->pNext    = NULL;
            pTemp->bValid   = FALSE;

            //  link in
            if ( pList == NULL ) {
                pRoot = pTemp;
                pList = pTemp;
            }
            else {
                pList->pNext = pTemp;
                pList = pTemp;
            }
        }

        // restore original string
        if ( p != NULL ) {
            *p = c;
        }
    }

second:

    /*
     *  If current list is empty or secondary file not specified then done
     */
    if ( pRoot == NULL || pSFile == NULL )
        goto done;

    /*
     *  get entry string from secondary file and secondary section
     */
    if ( !GetPrivateProfileString( pSSec,
                                   pEntry,
                                   INI_EMPTY,
                                   pEntryBuf,
                                   MAX_INI_LINE,
                                   pSFile ) ) {
#ifndef DOS
    if ( !GetPrivateProfileStringDefault( pSSec,
                                   pEntry,
                                   pEntryBuf,
                                   MAX_INI_LINE,
                                   pSFile ) )
#endif
        goto done;
    }

    /*
     *  walk entry string and build list
     */
    for ( pEntryElement=GetFirstElement(pEntryBuf);
          pEntryElement!=NULL;
          pEntryElement=GetNextElement(pEntryElement) ) {

        /*
         *  null terminate first driver in list, but first save
         */
        if ( (p=strpbrk( pEntryElement, " ,\t\n" )) != NULL ) {
             c = *p;
            *p = 0;
        }

        /*
         *  search current list for element
         */
        for ( pTemp=pRoot; pTemp!=NULL; pTemp=pTemp->pNext ) {

            // match
            if ( !stricmp( pTemp->pElement, pEntryElement ) ) {
                pTemp->bValid = TRUE;
                break;
            }
        }

        // restore original string
        if ( p != NULL )
            *p = c;
    }


done:
    // free list buffer
    if ( pEntryBuf )
        (void) free( pEntryBuf );

    return( pRoot );
}

/*******************************************************************************
 *
 *  DestroyList
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

VOID
DestroyList( PLIST pList )
{
    PLIST pTemp = NULL;
    PLIST pNext;

    /*
     *  walk list freeing elements as we go
     */
    for ( pTemp=pList; pTemp!=NULL; pTemp=pNext ) {

        /*
         *  save address to next node
         */
        pNext = pTemp->pNext;

        /*
         *  free current node string
         */
        if ( pTemp->pElement )
            (void) free( pTemp->pElement );

        /*
         *  free current node
         */
        (void) free( pTemp );
    }
}

/*******************************************************************************
 *
 *  DestroySList
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

VOID
DestroySList( PSECTIONLIST pList )
{
    PSECTIONLIST pTemp = NULL;
    PSECTIONLIST pNext;

    /*
     *  walk list freeing elements as we go
     */
    for ( pTemp=pList; pTemp!=NULL; pTemp=pNext ) {

        /*
         *  save address to next node
         */
        pNext = pTemp->pNext;

        /*
         *  free current node string
         */
        if ( pTemp->pElement )
            (void) free( pTemp->pElement );
        if ( pTemp->pFile )
            (void) free( pTemp->pFile );

        /*
         *  free current node
         */
        (void) free( pTemp );
    }
}

/*******************************************************************************
 *
 *  GetFirstElement
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

PCHAR
GetFirstElement( PCHAR pPd )
{
    unsigned short p;

    /*
     *  Scan forward to first Pd in list of Pds
     */
    p = strspn( pPd, " \t\n" );

    /*
     *  Empty string?
     */
    if ( p == strlen( pPd ) )
        return( NULL );

    /*
     *  Return location of start
     */
    return( (pPd+p) );
}

/*******************************************************************************
 *
 *  GetNextElement
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

PCHAR
GetNextElement( PCHAR pPd )
{
    PCHAR p;

    /*
     *  Seek to comma
     */
    if ( (p = strchr( pPd, ',' )) == NULL )
       return( NULL );

    /*
     *  Find Pd and return
     */
    return( GetFirstElement( ++p ) );
}

/*******************************************************************************
 *
 *  GetString
 *
 *  ENTRY:
 *      pPSec   (input) -   protocol file section to look under
 *      pPFile  (input) -   protocol file to look in
 *      pSSec   (input) -   server file section to look under
 *      pSFile  (input) -   server file to look in
 *      pEntry  (input) -   entry to look for
 *      pString (output) -  return value
 *      cbString (intput) - length of pString
 *
 *  EXIT:
 *
 ******************************************************************************/

void
GetString( PCHAR pPSec, PCHAR pPFile, PCHAR pSSec, PCHAR pSFile,
           PCHAR pEntry, PCHAR pString, int cbString )
{
    PCHAR pBuffer;

    /*
     *  null input string
     */
    memset( pString, 0, cbString );

    /*
     *  allocate space for answer
     */
    if ( (pBuffer = (PCHAR) malloc( cbString )) == NULL )
        return;

    /*
     *  get primary file requirement
     */
    if ( pPFile )
        if( ! GetPrivateProfileString( pPSec, pEntry, INI_EMPTY,
                                        pString, cbString, pPFile )) {

#ifndef DOS
           GetPrivateProfileStringDefault( pPSec, pEntry, pString, cbString, pPFile);
#endif
           }

    /*
     *  check secondary file
     */
    if ( pSFile )
        if ( GetPrivateProfileString( pSSec, pEntry, INI_EMPTY,
                                      pBuffer, cbString, pSFile ) )
            strncpy( pString, pBuffer, cbString );

    /*
     *  free temp buf
     */
    (void) free( pBuffer );
}

#if defined(DOS) || defined(WINCE) || defined(RISCOS)
/*******************************************************************************
 *
 *  GetSection (DOS)
 *
 *  ENTRY:
 *      pSectionName (input)
 *          Name of section to get.
 *      pFileName (input)
 *          Name of INI file to look in.
 *      ppSection (output)
 *          Points to pointer for section buffer.
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS if no error
 *      CLIENT_ERROR_NO_MEMORY if not enough memory for section buffer
 *      -1 if section not found (caller reports specific error code).
 *
 *      NOTE: The caller must free the section buffer after using.
 *
 ******************************************************************************/

int
GetSection( PCHAR pSectionName,
            PCHAR pFileName,
            PCHAR *ppSectionBuffer,
            PLIST pOverwrites )
{
    int   rc = CLIENT_STATUS_SUCCESS;
    int   cb, cbSection = 0;
    int   len1, len2, lenTemp;
    PCHAR pSectionBuffer = NULL;
    PCHAR pTempBuffer = NULL;
    PCHAR pTemp = NULL;
    PCHAR pEqual1 = NULL;
    PCHAR pEqual2 = NULL;
    BOOL  KeyFound;
    PLIST pList;
    CHAR  OverwriteEntry[MAX_INI_LINE];
    PCHAR pEntry;

    TRACE((TC_LIB, TT_API1, "GetSection: S '%s' F '%s' Overwrites %p", pSectionName ? pSectionName : "", pFileName ? pFileName : "", pOverwrites));

    pEntry = (PCHAR) malloc(MAX_INI_LINE);

    /*
     * continue till all is read
     */
    for ( ;; ) {

        /*
         * bump section size up
         */
        cbSection += SECTIONSIZE;

        /*
         * allocate section buffer
         */
        if ( (pTempBuffer = (PCHAR) malloc( cbSection )) == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto ErrorReturn;
        }

        /*
         * try and read this section
         */
#if defined( DOS ) || defined(WINCE) || defined( RISCOS )
        if ( !(cb = GetPrivateProfileString( pSectionName,
                                             NULL,
                                             INI_EMPTY,
                                             pTempBuffer,
                                             cbSection,
                                             pFileName )) ) {
#else /* DmitryV 9/25/97
         This code does not get executed anymore because we moved the WIN32 stuff to the
         GetSection Function below.  Read the comments to the WIN16 GetSection().
      */
        // win32 has a strange bug were it will create an empty INI file
        // if the requested INI file is not present.
        // to get around this, we will check for the INI file first.
        if ( (_access(pFileName,0)) ||
             !(cb = GetPrivateProfileSection( pSectionName,
                                              pTempBuffer,
                                              cbSection,
                                              pFileName )) ) {
#endif
#ifndef DOS
            cb = GetPrivateProfileSectionDefault( pSectionName,
                                                  pTempBuffer,
                                                  cbSection,
                                                  pFileName);
#endif
            if(cb==0) {
               rc = -1;    // 'not found'
               goto ErrorReturn;
               }
        }

        /*
         * if buffer is too small, free current buffer and try for
         * a bigger one.
         */
        if ( cb == cbSection-2 )
            (void) free( pTempBuffer );
        else
            break;  // got 'em all
        }

    /*
     * realloc to true size
     */
    pTempBuffer = (PCHAR) realloc( pTempBuffer, cb+1 );

    TRACE((TC_LIB, TT_API2, "GetSection: got buffer at %p cb %d last three chars %d %d %d",
	   pTempBuffer, cb, pTempBuffer[cb-2], pTempBuffer[cb-1], pTempBuffer[cb]));

    /* 
     * Second pass on the buffer to allow for overwrites and to eliminate 
     * all whitespace
     */
    pSectionBuffer = (PCHAR) malloc(cb+1+SECTIONSIZE);
    cb = 0;
    pTemp = pTempBuffer;
    while (lenTemp = strlen(pTemp)) {
       
       TRACE((TC_LIB, TT_API2, "GetSection: entry '%s' len %d", pTemp, lenTemp));

       memcpy(pEntry, pTemp, lenTemp);
       pEntry[lenTemp] = 0;
       pEqual1 = strchr(pEntry, '=');
       if (pEqual1 != NULL)
          *pEqual1++ = 0;

       EatWhiteSpace(pEntry);

       KeyFound = FALSE;
       pList = pOverwrites;
       
       while ((pList != NULL) && !KeyFound) {  
	  //see if this entry should be overwritten

	  TRACE((TC_LIB, TT_API2, "GetSection: overwrite element '%s'", pList->pElement));

          strcpy(OverwriteEntry, pList->pElement);

          pEqual2  = strchr(OverwriteEntry, '=');
          if (pEqual2 != NULL)
             *pEqual2++ = 0;

          if (!stricmp(OverwriteEntry, pEntry)) {
             KeyFound = TRUE;
          } else {
             pList = pList->pNext;
          }
       }
       len1= strlen(pEntry);
       memcpy( &(pSectionBuffer[cb]), pEntry, len1);
       cb += len1;
       pSectionBuffer[cb++] = '=';

       if (KeyFound) {  //use the value in the overwrites
          if (pEqual2 != NULL) {
             len2 = strlen(pEqual2);
             memcpy( &(pSectionBuffer[cb]), (pEqual2), len2);
          }
       } else {
          len2=0;
          if (pEqual1 != NULL) {
             EatWhiteSpace(pEqual1);
             len2 = strlen(pEqual1);
             memcpy( &(pSectionBuffer[cb]), pEqual1, len2);
          }
       }

       cb += len2;
       pSectionBuffer[cb++] = 0;
       pTemp += (lenTemp + 1);

       TRACE((TC_LIB, TT_API2, "GetSection: entry end cb=%d", cb));
    }

#if 0
    /* add this to stop buffer overruns on empty sections */
    if (cb == 0)
	pSectionBuffer[cb++] = 0;
#endif
    pSectionBuffer[cb++] = 0;
    pSectionBuffer = (PCHAR) realloc(pSectionBuffer, cb);

done: 
    TRACE((TC_LIB, TT_API1, "GetSection: "));

    free(pTempBuffer);
    free(pEntry);
    *ppSectionBuffer = pSectionBuffer;

    TRACE((TC_LIB, TT_API1, "GetSection: rc %d", rc));

    return( rc );

//----------------
// error return
//----------------
ErrorReturn:
    if ( pSectionBuffer )
        free(pSectionBuffer);
    pSectionBuffer = NULL;
    goto done;

}  // end GetSection (DOS)

#else
/*******************************************************************************
 *
 *  GetSection (DmitryV 9/25/97 WIN16 doesn't have the GetPrivateProfileSection(),
 *                              and WIN32 fails on WINDOWS95 ReadOnly INI files,
 *                              so we do have to use multiple steps)
 *
 *  For WIN16, there is no api to get the whole section, so we need to do multiple
 *  steps to do what we want.
 *  For WIN32, there is an api -- GetPrivateProfileSection -- but on WIN95 it fails
 *  if the *.INI file is marked "ReadOnly" because it tries to open it as "rw", not
 *  just "r".  And our customers don't want their people to be able to modify the
 *  INI files that MIS so painstakingly put together.
 *
 *  So, we could do two things:
 *
 *     1. Dynamically query to find out if we are on NT or 95, and then choose which
 *        path to take based on the result, or
 *     2. Do what WIN16 does, and parse the sections ourselves
 *
 *  For now, we will just do step 2, and let WIN32 and WIN16 execute this function,
 *  especially since we know it worked for at least WIN16.  Maybe in the future
 *  WIN95 will call LockFile rather than fopen(..., "rw"), and we can be more
 *  efficient.  If that happens, just move the || defined(WIN32) to other GetSection().
 *
 *  ENTRY:
 *      pSectionName (input)
 *          Name of section to get.
 *      pFileName (input)
 *          Name of INI file to look in.
 *      ppSection (output)
 *          Points to pointer for section buffer.
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS if no error
 *      CLIENT_ERROR_NO_MEMORY if not enough memory for section buffer
 *      -1 if section not found (caller reports specific error code).
 *
 *      NOTE: The caller must free the section buffer after using.
 *
 ******************************************************************************/

int
GetSection( PCHAR pSectionName,
            PCHAR pFileName,
            PCHAR *ppSectionBuffer,
            PLIST pOverwrites )
{
    int  rc = CLIENT_STATUS_SUCCESS;
    int  cb, cbTemp,
         cbSection = 0,
         cbEntries = 0;
    PENTRYLIST pEntryListRoot = NULL,
               pEntryList = NULL,
               pEntryListNext;
    PCHAR pSectionBuffer = NULL,
          pEntriesBuffer = NULL,
          pEntry;
    CHAR  StringBuffer[MAX_INI_LINE];

    PLIST pList;
    CHAR  EntryCopy[MAX_INI_LINE];
    CHAR  EntryCopy2[MAX_INI_LINE];
    BOOL  KeyFound;
    PCHAR pTemp;
    PCHAR pEqual;
    PCHAR pEqual2;
    int   len;

    /*
     * First, get all entries in the section.
     */
    for ( ;; ) {

        /*
         * bump entries size up
         */
        cbEntries += SECTIONSIZE;

        /*
         * allocate entries buffer
         */
        if ( (pEntriesBuffer = (PCHAR) malloc( cbEntries )) == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto ErrorReturn;
        }

        /*
         * read all entry names
         */
        if ( !(cb = GetPrivateProfileString( pSectionName,
                                             NULL,
                                             INI_EMPTY,
                                             pEntriesBuffer,
                                             cbEntries,
                                             pFileName )) ) {

#ifndef DOS
            // try using default section values before
            // giving up
            cb = GetPrivateProfileSectionDefault( pSectionName,pEntriesBuffer,
                                                  cbEntries,
                                                  pFileName);
#endif
            if(cb==0) {
               rc = -1;    // 'not found'
               goto ErrorReturn;

            } else {

               /*
                * Add bytes for the potential overrides
                * This is a bit wasteful but will soon be freed anyway
                * Possibly fix this in future
                */
               pList = pOverwrites;
               while (pList != NULL) {
                  cb += strlen(pList->pElement);
                  pList = pList->pNext;
               }
               /*
                * we must stop here and overwrite the values
                * if necessary as specified from the overwrites list
                */
               if ( (pSectionBuffer = (PCHAR) malloc(cb)) == NULL ) {
                   rc = CLIENT_ERROR_NO_MEMORY;
                   goto ErrorReturn;
               }

               pEntry = pSectionBuffer;    //buffer to write into
               pTemp  = pEntriesBuffer;    //buffer reading from

               while (len = strlen(pTemp)) {
                  KeyFound = FALSE;
                  pList = pOverwrites;

                  while ((pList != NULL) && !KeyFound) {
                     //see if this entry should be overwritten
                     strcpy(EntryCopy,  pTemp);
                     strcpy(EntryCopy2, pList->pElement);

                     pEqual = strchr(EntryCopy, '=');
                     *pEqual++ = 0;
                     pEqual2 = strchr(EntryCopy2, '=');
                     *pEqual2++ = 0;

                     if (!stricmp(EntryCopy, EntryCopy2)) {
                        KeyFound = TRUE;
                     } else {
                        pList = pList->pNext;
                     }
                  }

                  if (KeyFound) {

                     cbTemp = strlen(pList->pElement);
                     memcpy( pEntry, pList->pElement, cbTemp );
                     pEntry += cbTemp;
                     *pEntry++ = '\0';

                  } else {

                     memcpy(pEntry, EntryCopy, (cbTemp = strlen(EntryCopy)) );
                     pEntry += cbTemp;
                     *pEntry++ = '=';
                     if ( *pEqual ) {
                         memcpy( pEntry, pEqual, (cbTemp = strlen(pEqual)) );
                         pEntry += cbTemp;
                     }
                     *pEntry++ = '\0'; //NULL terminate the entry/string pair
                  }
                  pTemp += (len + 1);  //move to next buffer entry
               }
               *pEntry = '\0'; //final NULL terminator.
               goto done; //buffer is complete
            }
        }

        /*
         * if buffer is too small, free current buffer and try for
         * a bigger one.
         */
        if ( cb == cbEntries-2 )
            (void) free( pEntriesBuffer );
        else
            break;  // got 'em all
    }

    /*
     * Section found in file... do the extra formatting now
     * realloc entries buffer to true size
     */
    pEntriesBuffer = (PCHAR)realloc( pEntriesBuffer, cb+1 );

    /*
     * Next, loop through the entry names and get the associated strings,
     * adding the entry, string pairs to a list.  Also, keep track of the
     * total number of bytes needed to store the completed section.
     */
    for ( pEntry = pEntriesBuffer; *pEntry; ) {

        /*
         * Allocate and prepare this list element.
         */
        if ( (pEntryList = (PENTRYLIST)malloc(sizeof(ENTRYLIST))) == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto ErrorReturn;
        }

        pEntryList->pEntryName = pEntry;
        pEntryList->pEntryString = NULL;
        pEntryList->pNext = NULL;
        cbSection += (cbTemp = strlen(pEntry));
        pEntry += (cbTemp + 1);

        /*
         * Link element into list
         */
        if ( !pEntryListRoot )
            pEntryListRoot = pEntryList;
        else
            pEntryListNext->pNext = pEntryList;
        pEntryListNext = pEntryList;

        /*
         * read the string associated with this entry
         */
        GetPrivateProfileString( pSectionName,
                                 pEntryList->pEntryName,
                                 INI_EMPTY,
                                 StringBuffer,
                                 MAX_INI_LINE,
                                 pFileName );

        /*
         * if a string was returned (not null string), duplicate
         * it and set into entry.
         */
        if ( StringBuffer[0] ) {
            if ( (pEntryList->pEntryString = strdup(StringBuffer)) == NULL ) {
                rc = CLIENT_ERROR_NO_MEMORY;
                goto ErrorReturn;
            }
            cbSection += strlen(StringBuffer);
        }

        /*
         * Add one byte for '=' and one for terminating NULL.
         */
        cbSection += 2;
    }

    /*
     * Add one byte for final terminating NULL.
     */
    cbSection++;

    /*
     * Add bytes for the potential overrides
     * This is a bit wasteful but will soon be freed anyway
     * Possibly fix this in future
     */
    pList = pOverwrites;
    while (pList != NULL) {
       cbSection += strlen(pList->pElement);
       pList = pList->pNext;
    }

    /*
     * allocate the section buffer
     */
    if ( (pSectionBuffer = (PCHAR)malloc(cbSection)) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto ErrorReturn;
    }
#ifdef DEBUG
    memset(pSectionBuffer, 0xAA, cbSection);
#endif

    /*
     * Finally, traverse the entry list and copy entry, '=', and string
     * (if there) to the section buffer, terminating each entry/string pair
     * with a NULL and the final pair with double NULL (just like NT's
     * GetPrivateProfileSection).  We'll also free the list element as
     * we go so we don't have to traverse the list again when done.
     */
    for ( pEntryListNext = pEntryListRoot, pEntry = pSectionBuffer;
          pEntryListNext; ) {

        pEntryList = pEntryListNext;
        pEntryListNext = pEntryList->pNext;

        KeyFound = FALSE;
        pList = pOverwrites;

        while ((pList != NULL) && !KeyFound) {
           //see if this entry should be overwritten
           strcpy(EntryCopy, pList->pElement);

           pEqual = strchr(EntryCopy, '=');
           *pEqual = 0;

           if (!stricmp(EntryCopy, pEntryList->pEntryName)) {
              KeyFound = TRUE;
           } else {
              pList = pList->pNext;
           }
        }

        if (KeyFound) {

           cbTemp = strlen(pList->pElement);
           memcpy( pEntry, pList->pElement, cbTemp );
           pEntry += cbTemp;
           *pEntry++ = '\0';

        } else {
           memcpy( pEntry, pEntryList->pEntryName,
                   (cbTemp = strlen(pEntryList->pEntryName)) );
           pEntry += cbTemp;
           *pEntry++ = '=';
           if ( pEntryList->pEntryString ) {
               memcpy( pEntry, pEntryList->pEntryString,
                       (cbTemp = strlen(pEntryList->pEntryString)) );
               pEntry += cbTemp;
               free(pEntryList->pEntryString);
           }
           *pEntry++ = '\0'; // NULL terminate the entry/string pair.
        }
        free(pEntryList);
    }
    *pEntry = '\0'; // final NULL terminator.

done:
    /*
     * free entries buffer
     */
    if ( pEntriesBuffer )
        free(pEntriesBuffer);

    *ppSectionBuffer = pSectionBuffer;
    return( rc );

//----------------
// error return
//----------------
ErrorReturn:
    /*
     * free entry list
     */
    for ( pEntryListNext = pEntryListRoot;
         pEntryListNext; ) {

        pEntryList = pEntryListNext;
        pEntryListNext = pEntryList->pNext;

        if ( pEntryList->pEntryString )
            free(pEntryList->pEntryString);

        free(pEntryList);
    }

    if ( pSectionBuffer )
        free(pSectionBuffer);
    pSectionBuffer = NULL;

    goto done;

}  // end GetSection (WIN16)
#endif //end ifdef DOS else


/*******************************************************************************
 *
 *  CombinePrivateProfileEntries
 *
 *      This routine creates an output profile section consisting of all
 *      entry=value pairs in lpszPrimary combined with all entry=value pairs
 *      in lpszSecondary that are not already in the new section (ie: not
 *      originally in lpszPrimary).
 *
 *  ENTRY:
 *      lpszPrimary (input)
 *          Primary profile entries.
 *      lpszSecondary (input)
 *          Secondary profile entries to combine with primary.
 *  EXIT:
 *      (PCHAR) pointer to combined profile entries; NULL if not enough
 *          memory to perform combine.
 *
 ******************************************************************************/

PCHAR
CombinePrivateProfileEntries( PCHAR lpszPrimary, PCHAR lpszSecondary )
{
    int   len;
    int   cb;
    int   cbBufCur = 0;
    int   cbBufMax = SECTIONSIZE;
    PCHAR pSection = lpszSecondary;
    PCHAR pReturn  = NULL;
    PCHAR pEntry   = NULL;
    PCHAR pString  = NULL;
    PCHAR pLine    = NULL;
    PCHAR pBuf     = NULL;
    PCHAR p        = NULL;

    cbBufCur = IniSize(lpszPrimary) - 1;  // don't include final NULL

    /*
     * malloc for combined entries
     */
    cbBufMax += cbBufCur;
    if ( (pReturn = (PCHAR) malloc( cbBufMax )) == NULL )
        goto NoMemory;

    /*
     * Initialize return buffer and copy primary section there.
     */
    memset( pReturn, 0, cbBufMax );
    memcpy( pReturn, lpszPrimary, cbBufCur );

    /*
     * malloc buffer for secondary profile entry
     */
    if ( (pBuf = (PCHAR) malloc(MAX_INI_LINE)) == NULL )
        goto NoMemory;

    /*
     * search secondary buffer while there are any entries left
     */
    while ( (pSection!=NULL) && (len = strlen( pSection )) ) {

        //  make copy so we may mess with it
        if ( (pLine = strdup( pSection )) == NULL )
            goto NoMemory;

        //  find = sign, and use as temporary terminator
        if ( (pString=strchr( pLine, '=' )) != NULL ) {

            //  find begining of entry
            pEntry = pLine + strspn( pLine, " \t" );

            //  get past the = and terminate the pEntry
            *(pString++) = 0;

            //  find end of entry and terminate
            if ( (p = strpbrk( pEntry, " \t" )) != NULL )
               *p = 0;

            //  now look for entry match in primary buffer.
            if ( !GetSectionString( lpszPrimary,
                                    pEntry,
                                    "",
                                    pBuf,
                                    MAX_INI_LINE ) ) {

                /*
                 * Entry not in primary section; copy it there.
                 */
                cb = strlen( pSection ) + 1;

                //  space left in buffer? no, then grow
                if ( (cbBufCur + cb) >= cbBufMax ) {
                    cbBufMax += SECTIONSIZE;
                    if ( (pReturn = (char *) realloc( pReturn, cbBufMax )) == NULL )
                        goto NoMemory;
                }

                // copy into buffer
                memcpy( &(pReturn[cbBufCur]), pSection, cb );

                //  index to next avail location in buffer
                cbBufCur += cb;

                //  double NULL terminate (last entry will remain this way).
                pReturn[cbBufCur] = 0;
            }
        }

        //  free duped line
        (void) free( pLine );

        //  next string
        pSection += (len + 1);
    }

Done:
    //  free temp buffer
    if ( pBuf )
        (void) free( pBuf );

    //  return combined entries
    return( pReturn );

//----------------
// error return
//----------------
NoMemory:
    if ( pLine )
        free(pLine);
    if ( pReturn )
        free(pReturn);
    pReturn = NULL;
    goto Done;
}

/*******************************************************************************
 *
 *  AddHeaderSection
 *
 *    will pad the header name to the front of the provided section
 *
 ******************************************************************************/

PCHAR AddHeaderSection( PCHAR pSection, PCHAR pName )
{
   PCHAR pReturn;
   int   curr;
   int   len;
   int   size;

   len = strlen(pName);
   size = IniSize(pSection);

   pReturn = (PCHAR) malloc(size + (len + 4));    //we shall add four extra characters

   curr = 0;
   pReturn[curr++] = '[';
   memcpy ( &(pReturn[curr]), pName, len);
   curr += len;
   pReturn[curr++] = ']';
   pReturn[curr++] = '0';
   pReturn[curr++] = 0;
   memcpy ( &(pReturn[curr]), pSection, size);

   return(pReturn);
}

/*******************************************************************************
 *
 *  BuildCfgIniOverridesSection
 *
 *  ENTRY:
 *      pCfgIniOverrides (input)
 *          NULL if no overrides, or points to array of CFGINIOVERRIDE
 *          structures.
 *  EXIT:
 *      (PCHAR) pointer to created section; NULL if not enough memory to
 *      build the section.
 *
 ******************************************************************************/

PCHAR
BuildCfgIniOverridesSection( PCFGINIOVERRIDE pCfgIniOverrides )
{
    int   rc = CLIENT_STATUS_SUCCESS;
    int   i, cbKey, cbValue, cbBufCur;
    int   cbBufMax = SECTIONSIZE;

    PCHAR pReturn  = NULL;

    /*
     * Start with default section buffer.
     */
    if ( (pReturn = (PCHAR) malloc( cbBufMax )) == NULL )
        goto NoMemory;

    /*
     * Default to an 'empty' (double null) section buffer with
     * [Server] Section Name and an 0 offset value. ie :: [Server]00000\0\0
     * Will remain this way if override loop encounters no overrides.
     */
    pReturn[0] = '[';
    cbBufCur = 1;
    cbKey = strlen( INI_SERVERSECTION );
    memcpy( &(pReturn[cbBufCur]), INI_SERVERSECTION, cbKey );
    cbBufCur += cbKey;
    pReturn[cbBufCur++] = ']';
    pReturn[cbBufCur++] = '0';
    pReturn[cbBufCur++] = 0;
    pReturn[cbBufCur] = 0;

    /*
     * Loop through the overrides.
     */
    for ( i = 0; pCfgIniOverrides && pCfgIniOverrides[i].pszKey; i++ ) {

        /*
         * If this entry's Value is not NULL or empty, add this
         * key=value pair to the section buffer.
         */
        if ( pCfgIniOverrides[i].pszValue &&
             *(pCfgIniOverrides[i].pszValue) ) {

            cbKey = strlen( pCfgIniOverrides[i].pszKey );
            cbValue = strlen( pCfgIniOverrides[i].pszValue );

            /*
             * Grow the buffer if no space left (key length + value length +
             * 2 for '=' and terminating null).
             */
            if ( (cbBufCur + cbKey + cbValue + 2) >= cbBufMax ) {
                cbBufMax += SECTIONSIZE;
                if ( (pReturn = (char *) realloc( pReturn, cbBufMax )) == NULL )
                    goto NoMemory;
            }

            /*
             * Copy key, '=', value, and null into buffer.
             */

            memcpy( &(pReturn[cbBufCur]), pCfgIniOverrides[i].pszKey, cbKey );
            cbBufCur += cbKey;
            pReturn[cbBufCur++] = '=';
            memcpy( &(pReturn[cbBufCur]), pCfgIniOverrides[i].pszValue, cbValue );
            cbBufCur += cbValue;
            pReturn[cbBufCur++] = 0;

            /*
             * Double null terminate (last entry will remain this way).
             */
            pReturn[cbBufCur] = 0;
        }
    }

Done:
    return(pReturn);

//----------------
// error return
//----------------
NoMemory:
    if ( pReturn )
        free(pReturn);
    pReturn = NULL;
    goto Done;
}

#ifndef DOS
/*
 * GetPrivateProfileSectionDefault
 *
 */
int GetPrivateProfileSectionDefault( PCHAR pSection, PCHAR pBuf, int cbLen, PCHAR pFile)
{
  PDEFINIFILE pDefIniFile;
  PDEFINISECT pDefIniSect;
  char *pBasename;
  char *pSectionDefault;
  int cbSectDef;
  char *pTmp;

  // find file
  pDefIniFile = DefIniSect;
  pBasename = basename(pFile);
  while(pDefIniFile->pFileName!=NULL) {
     if(stricmp(pDefIniFile->pFileName,pBasename)==0)
        break;
     pDefIniFile++;
     }

  // if file not found, return nothing
  if(pDefIniFile->pFileName == NULL)
     return(0);

  // find section
  pDefIniSect = pDefIniFile->pINISect;
  while(pDefIniSect->pSectName != NULL) {
     if(stricmp(pDefIniSect->pSectName, pSection)==0)
        break;
     pDefIniSect++;
     }

  // if section not found, return nothing
  if(pDefIniSect->pSectName == NULL)
     return(0);

  pSectionDefault = pDefIniSect->pSectDefault;

  // calculate the section length
  cbSectDef = 0;
  pTmp = pSectionDefault;
  while(*pTmp != '\0') {
     cbSectDef += strlen(pTmp)+1;
     pTmp += strlen(pTmp) + 1;
     }
  cbSectDef += 2;

  if(cbSectDef < cbLen) {
     memcpy( pBuf, pSectionDefault, cbSectDef);
     return(cbSectDef-2);
     }
  else {
     memcpy( pBuf, pSectionDefault, cbLen-2);
     *(pBuf+(cbLen-2)) = '\0';
     *(pBuf+(cbLen-1)) = '\0';
     return(cbLen-2);
     }
}


/*
 * GetPrivateProfileStringDefault
 *
 */
int GetPrivateProfileStringDefault( PCHAR pSection, PCHAR pKey, PCHAR pBuf, int cbLen, PCHAR pFile)
{
  PDEFINIFILE pDefIniFile;
  PDEFINISECT pDefIniSect;
  char *pBasename;
  char *pSectionDefault;
  char *pKeyDefault;
  char *pTmp;
  char *pKeyValDef;
  char *pKeyNameDef;
  char szKeyBuf[MAX_INI_LINE];
  BOOL bKeyFound = FALSE;

  // find file
  pDefIniFile = DefIniSect;
  pBasename = basename(pFile);
  while(pDefIniFile->pFileName!=NULL) {
     if(stricmp(pDefIniFile->pFileName,pBasename)==0)
        break;
     pDefIniFile++;
     }

  // if file not found, return nothing
  if(pDefIniFile->pFileName == NULL)
     return(0);

  // find section
  pDefIniSect = pDefIniFile->pINISect;
  while(pDefIniSect->pSectName != NULL) {
     if(stricmp(pDefIniSect->pSectName, pSection)==0)
        break;
     pDefIniSect++;
     }

  // if section not found, return nothing
  if(pDefIniSect->pSectName == NULL)
     return(0);

  pSectionDefault = pDefIniSect->pSectDefault;

  // find the VALUE for KEY
  pKeyDefault = pSectionDefault;
  while(*pKeyDefault != '\0') {

     // build a copy of KEY and VALUE
     strcpy(szKeyBuf, pKeyDefault);
     pKeyNameDef = szKeyBuf;
     pTmp = strchr(pKeyNameDef, '=');
     if(pTmp) {
        *pTmp = '\0';
        pKeyValDef = pTmp+1;
        }
     else
        pKeyValDef = '\0';

     if(stricmp(pKeyNameDef, pKey) == 0) {
        bKeyFound = TRUE;
        break;
        }

     pKeyDefault += strlen(pKeyDefault) + 1;
     }

  // return KEY if found
  if(bKeyFound == TRUE) {
     if( (int)(strlen(pKeyValDef)+1) < cbLen) {
        strcpy( pBuf, pKeyValDef);
        return(strlen(pKeyValDef));
        }
     else {
        memcpy( pBuf, pKeyValDef, cbLen-1);
        *(pBuf+(cbLen-1)) = '\0';
        return(cbLen-1);
        }
     }
  else
     return(0);
}

/*******************************************************************************
 *
 *  basename
 *
 *    return pointer to base name in path
 *
 * ENTRY:
 *    pName (input)
 *       pointer to name of dll
 *
 * EXIT:
 *    pointer to basename
 *
 ******************************************************************************/

char * basename( char * pName )
{
    char * pBasename;
    int i;

    for ( i=0; i < (int)strlen(pName); i++ ) {
        if ( pName[i] == '/' )
            pName[i] = '\\';
    }

    if ( pBasename = strrchr(pName,'\\') )
        pBasename++;
    else
        pBasename = pName;

    return( pBasename );
}
#endif

/*******************************************************************************
 *
 *  BuildMembersList
 *
 *    return pointer to a list of all section members
 *
 *    ValidSection (input)
 *       name of the section we wish to make valid
 *    ListSection
 *       name of the Section where the list of available sections resides
 *
 ******************************************************************************/

PLIST BuildMembersList( PCHAR ValidSection, PCHAR ListSection,
                        PCHAR lpszFileName) {

   PLIST pList = NULL;
   PLIST pRoot = NULL;
   PLIST pTemp = NULL;
   PCHAR pIniSection = NULL;
   PCHAR pSave;
   PCHAR p;
   int   len;
   PLIST pOverwrites = NULL;

   TRACE((TC_LIB, TT_API1, "BuildMembersList: Valid '%s' List '%s' File '%s'", ValidSection, ListSection, lpszFileName));
   
   GetSection(ListSection, lpszFileName, &pIniSection, pOverwrites);

   pSave = pIniSection; //save head of buffered section so we may free it all
   while (len = strlen(pSave)) {

      pTemp = (PLIST) malloc(sizeof(LIST));
      p = strchr(pSave, '=');
      *p = 0;
      pTemp->pElement = strdup( pSave );

      if (!strcmp(ValidSection, pSave)) {
         pTemp->bValid = 1;
      } else {
         pTemp->bValid = 0; }

      pTemp->pNext = NULL;

      if (pRoot == NULL) {
         //first component
         pList = pTemp;
         pRoot = pList;
      } else {
         pList->pNext = pTemp;
         pList = pList->pNext;
      }
      pSave += (len + 1);

      TRACE((TC_LIB, TT_API1, "BuildMembersList: Element '%s' Valid %d", pTemp->pElement, pTemp->bValid));
   }

   free(pIniSection);
   return(pRoot);
}

/*******************************************************************************
 *
 *  BuildAllSectionsList
 *
 *    return pointer to a list of all valid sections in the specified files
 *
 ******************************************************************************/

PSECTIONLIST BuildAllSectionsList( PCHAR pConnection,
                                   PCHAR pTransport,
                                   PCHAR pServerFile,
                                   PCHAR pConfigFile,
                                   PCHAR pProtocolFile )
{
#ifdef WINCE
   HANDLE         infile = NULL;
   CHAR           *CurChar;
   DWORD          NumRead=0, CurPos=0;
#else
   FILE           *infile = NULL;
#endif
   BOOL           InvalidSection;
   BOOL           Transport;

   PSECTIONLIST   pRoot = NULL;
   PSECTIONLIST   pList = NULL;
   PSECTIONLIST   pNew  = NULL;
   PSECTIONLIST   pTemp = NULL;

   PLIST pAppServerList = NULL;
   PLIST pTransportList = NULL;
   PLIST pTemp2;

   PCHAR CurrSectionName = NULL;
   PCHAR p;
   char  ReadBuf[MAX_INI_LINE];          // temp line buffer

   /*
    * Create our sections list starting with the valid sections in server file.
    * Make a list of all ApplicationServer sections in the server file
    */
   pAppServerList = BuildMembersList(pConnection, INI_APPSERVERLIST, pServerFile);

   //validate parameters
   if ( pServerFile == NULL) {
      goto done;
   }

   //open the server file
#ifdef WINCE
   if ( (infile = CreateFile(pServerFile,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL)) == INVALID_HANDLE_VALUE ) {
#else
   if ( (infile = fopen( pServerFile, "r" )) == NULL ) {
#endif
      goto done;
   }

#ifdef WINCE
   CurPos = SetFilePointer((HANDLE)infile, 0L, NULL, FILE_CURRENT);

   while( (ReadFile(infile,
                    ReadBuf,
                    MAX_INI_LINE,
                    &NumRead,
                    NULL) != 0) && (NumRead !=0) ) {

          CurChar = strstr(ReadBuf, "\r");
          if (CurChar != NULL) {
             *CurChar = 0x00;
             if (*(CurChar+1) == 0xA)
                *(CurChar+1) = 0x00;
          }
          CurPos = CurPos + strlen(ReadBuf)+2;
          SetFilePointer((HANDLE)infile, CurPos , NULL, FILE_BEGIN);

#else
   while( fgets(ReadBuf, MAX_INI_LINE, infile) != NULL ) {
#endif

      // section header?
      if( (ReadBuf[0] == '[') && ((p = strchr( ReadBuf, ']' )) != NULL) ) {

         //replace right bracket with null and extract section name
         *p = 0;
         CurrSectionName = &ReadBuf[1];

         //search the appserverlist to see if this is a appserver section
         InvalidSection = FALSE;
         pTemp2 = pAppServerList;
         while (pTemp2 != NULL) {
            if (!strcmp(pTemp2->pElement, CurrSectionName)) {
               InvalidSection = TRUE;
               break;
            }
            pTemp2 = pTemp2->pNext;
         }

         //add to list if valid and not the WFClient section since we handle that special
         if (!InvalidSection && (strcmp(INI_WFCLIENT, CurrSectionName))) {
            pNew = (PSECTIONLIST) malloc(sizeof(SECTIONLIST));
            pNew->pElement = strdup(CurrSectionName);
            pNew->pFile    = strdup(pServerFile);
            pNew->bValid = 1;
            pNew->pNext = NULL;
            if (pRoot == NULL) {
               pList = pNew;
               pRoot = pList;
            } else {
               pList->pNext = pNew;
               pList = pList->pNext;
            }
         }
      }
   }

#ifdef WINCE
   if (CloseHandle( infile )) {
#else
   if (fclose( infile )) {
#endif
      goto done;
   }

   /*
    * Add to our sections list the sections in the config file
    */
   if ( pConfigFile == NULL) {
      goto skipconfig;
   }

   //open the config file
#ifdef WINCE
   if ( (infile = CreateFile(pConfigFile,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL)) == INVALID_HANDLE_VALUE ) {
#else
   if ( (infile = fopen( pConfigFile, "r" )) == NULL ) {
#endif
      goto done;
   }

#ifdef WINCE
   CurPos = SetFilePointer((HANDLE)infile, 0L, NULL, FILE_CURRENT);

   while( (ReadFile(infile,
                    ReadBuf,
                    MAX_INI_LINE,
                    &NumRead,
                    NULL) != 0) && (NumRead !=0) ) {

          CurChar = strstr(ReadBuf, "\r");
          if (CurChar != NULL) {
             *CurChar = 0x00;
             if (*(CurChar+1) == 0xA)
                *(CurChar+1) = 0x00;
          }
          CurPos = CurPos + strlen(ReadBuf)+2;
          SetFilePointer((HANDLE)infile, CurPos , NULL, FILE_BEGIN);

#else
   while( fgets(ReadBuf, MAX_INI_LINE, infile) != NULL ) {
#endif

      if( (ReadBuf[0] == '[') && ((p = strchr( ReadBuf, ']' )) != NULL) ) {

         *p = 0;
         CurrSectionName = &ReadBuf[1];

         //add to list
         if (strcmp(INI_WFCLIENT, CurrSectionName)) {
            pNew = (PSECTIONLIST) malloc(sizeof(SECTIONLIST));
            pNew->pElement = strdup(CurrSectionName);
            pNew->pFile    = strdup(pConfigFile);
            pNew->bValid = 1;
            pNew->pNext = NULL;
            if (pRoot == NULL) {
               pList = pNew;
               pRoot = pList;
            } else {
               pList->pNext = pNew;
               pList = pList->pNext;
            }
         }
      }
   }

#ifdef WINCE
   if (CloseHandle( infile )) {
#else
   if (fclose( infile )) {
#endif
      goto done;
   }

skipconfig:
   if ( pProtocolFile == NULL) {
      goto done;
   }

   /*
    * Add to our sections list the valid sections in the protocol file.
    * make a list of all TransportDriver sections in the protocol file and
    * only make the one we are currently using valid
    */
   pTransportList = BuildMembersList(pTransport, INI_TRANSPORTDRIVER, pProtocolFile);

   /*
    * Add to our sections list the valid sections in the protocol file
    */

   //validate parameters
   if ( pProtocolFile == NULL) {
      goto done;
   }

   //open the server file
#ifdef WINCE
   if ( (infile = CreateFile(pProtocolFile,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL)) == INVALID_HANDLE_VALUE ) {
#else
   if ( (infile = fopen( pProtocolFile, "r" )) == NULL ) {
#endif
      goto done;
   }

#ifdef WINCE
   CurPos = SetFilePointer((HANDLE)infile, 0L, NULL, FILE_CURRENT);

   while( (ReadFile(infile,
                    ReadBuf,
                    MAX_INI_LINE,
                    &NumRead,
                    NULL) != 0) && (NumRead !=0) ) {

          CurChar = strstr(ReadBuf, "\r");
          if (CurChar != NULL) {
             *CurChar = 0x00;
             if (*(CurChar+1) == 0xA)
                *(CurChar+1) = 0x00;
          }
          CurPos = CurPos + strlen(ReadBuf)+2;
          SetFilePointer((HANDLE)infile, CurPos , NULL, FILE_BEGIN);

#else
   while( fgets(ReadBuf, MAX_INI_LINE, infile) != NULL ) {
#endif

      if( (ReadBuf[0] == '[') && ((p = strchr( ReadBuf, ']' )) != NULL) ) {

         *p = 0;
         CurrSectionName = &ReadBuf[1];

         //search the transportlist to see if this is a transport section
         InvalidSection = FALSE;
         Transport = FALSE;
         pTemp2 = pTransportList;
         while (pTemp2 != NULL) {
            if (!strcmp(pTemp2->pElement, CurrSectionName)) {

               //make sure we dont invalidate our actual transport driver section
               if (pTemp2->bValid) {
                  Transport = TRUE; //special case to indicate we need extra sec
               } else {
                  InvalidSection = TRUE;
               }
               break;
            }
            pTemp2 = pTemp2->pNext;
         }

	 TRACE((TC_LIB, TT_API1, "BuildAllSectionsList: CurrSectionName '%s' InvalidSection %d Transport %d",
		CurrSectionName, InvalidSection, Transport));

         //add to list if valid
         if (!InvalidSection && (strcmp(INI_WFCLIENT, CurrSectionName))) {
            pNew = (PSECTIONLIST) malloc(sizeof(SECTIONLIST));
            pNew->pElement = strdup(CurrSectionName);
            pNew->pFile    = strdup(pProtocolFile);
            if (Transport) {
               pNew->bValid = 2;
            } else {
               pNew->bValid = 1;
            }
            pNew->pNext = NULL;
            if (pRoot == NULL) {
               pList = pNew;
               pRoot = pList;
            } else {
               pList->pNext = pNew;
               pList = pList->pNext;
            }
         }
      }
   }

#ifdef WINCE
   if (CloseHandle( infile )) {
#else
   if (fclose( infile )) {
#endif
      goto done;
   }

done:
   DestroyList(pAppServerList);
   DestroyList(pTransportList);

   return(pRoot);
}

/*******************************************************************************
 *
 *  ConcatSections
 *
 *    take 2 sections concatenate the 2nd onto the front of the first, then
 *    calculate the offset and put that into the first section
 *
 *******************************************************************************/

PCHAR ConcatSections( PCHAR pMain, PCHAR pNew )
{
   int    len,
          offset,
          mainSize,
          finalSize;
   PCHAR  pReturn   = NULL;
   PCHAR  offstring = NULL;
   PCHAR  pTemp     = NULL;
   PCHAR  pNewBody  = NULL;
   PCHAR  pHeader   = NULL;

   pHeader = (PCHAR) malloc(MAX_INI_LINE);

   /*
    * Find the length of the new section minus its section name and current offset
    * Subtract 1 from the length to remove the extra NULL at the end of the section
    */
   pNewBody = pNew + strlen(pNew);
   offset = IniSize(pNewBody) - 1;

   /*
    * Make a new header for the new section with its section name
    * and the new calculated offset
    */

   offstring = (PCHAR) malloc(MAX_INI_LINE);
   sprintf( offstring, "%d", offset );

   memcpy(pHeader, pNew, strlen(pNew));
   pTemp = strchr(pHeader, ']');
   len = strlen(offstring);
   memcpy( ++pTemp, offstring, len);  //place the new offset just behind the section name
   pTemp += len;
   *pTemp = 0;
   len = strlen(pHeader);

   /*
    * Determine the size needed for our concatenated buffer and do it
    */
   mainSize = IniSize(pMain);
   finalSize = offset + mainSize + len;
   pReturn = (PCHAR) malloc(finalSize);
   memcpy(pReturn, pHeader, len);

   pTemp = pReturn + len;
   memcpy(pTemp, pNewBody, offset);

   pTemp += offset;
   memcpy(pTemp, pMain, mainSize);

   free(offstring);
   free(pHeader);

   return(pReturn);
}


/*******************************************************************************
 *
 *  IniSize
 *
 *    Determine how many bytes of data that the specified INI file section consumes
 *
 * ENTRY:
 *    pIniSection (input)
 *       pointer to INI file section data
 *
 * EXIT:
 *    number of bytes consumed by pIniSection
 *
 ******************************************************************************/
INT IniSize( PCHAR pIniSection )
{
    INT cb = 0;

    for ( cb = 0; ; cb++ ) {
       if ( !*pIniSection++ && !*pIniSection ) {
           cb += 2;
           break;
       }
    }

    return( cb );
}


/*******************************************************************************
 *
 *  AddEntrySection
 *
 *    This will add the key=value into section or overwrite the value already there
 *
 ******************************************************************************/

PCHAR AddEntrySection( PCHAR pSection, PCHAR pEntry, PCHAR pValue )
{
   int      len, half1, half2;
   PCHAR    pTemp      = NULL;
   PCHAR    pEqual     = NULL;
   PCHAR    pNewEntry  = NULL;
   PCHAR    pReturn    = NULL;
   BOOL     EntryFound = FALSE;

   TRACE((TC_LIB, TT_API1, "AddEntrySection: section %p add %s=%s", pSection, pEntry, pValue));

   pNewEntry = (PCHAR) malloc(MAX_INI_LINE);

   pTemp = pSection;
   while ( (len = strlen(pTemp)) && !EntryFound) {

      strcpy(pNewEntry, pTemp);
      pEqual = strchr(pNewEntry, '=');

      //remove the equal sign and leave just the entry naame
      if (pEqual != NULL) {
         *pEqual = 0;
      }

      /*
       * If the entry already exists in the section then mark the location
       * of the entry with a NULL to cut off the first half of the buffer
       * from the second half for a re-merge later
       */
      if (!strcmp(pNewEntry, pEntry)) {
         EntryFound = TRUE;
         *pTemp = 0;
      }
      pTemp += (len + 1);
   }

   if (EntryFound) {

      len = strlen(pNewEntry);
      pNewEntry[len++] = '=';
      memcpy( &(pNewEntry[len]), pValue, strlen(pValue));
      len += strlen(pValue);
      pNewEntry[len] = 0;

      half1 = IniSize(pSection) - 1;     //minus 1 to remove the extra NULL
      half2 = IniSize(pTemp);            //pTemp points 1 entry behind found entry

      pReturn = (PCHAR) malloc(half1 + half2 + len + 1);
      memcpy( pReturn, pSection, half1);
      memcpy( &(pReturn[half1]), pNewEntry, len);
      half1 += len;
      pReturn[half1++] = 0;
      memcpy( &(pReturn[half1]), pTemp, half2);

      TRACE((TC_LIB, TT_API1, "AddEntrySection: found length %d output %p", half1 + half2, pReturn));
      
   } else {

      len = strlen(pEntry);
      memcpy( pNewEntry, pEntry, len);
      pNewEntry[len++] = '=';
      memcpy( &(pNewEntry[len]), pValue, strlen(pValue));
      len += strlen(pValue);
      pNewEntry[len] = 0;

      half1 = IniSize(pSection) - 1;
      pReturn = (PCHAR) calloc (half1 + len + 2, 1);
      memcpy( pReturn, pSection, half1);
      memcpy( &(pReturn[half1]), pNewEntry, len);
      half1 += len;

      DTRACE((TC_LIB, TT_API1, "AddEntrySection: write0 at %d", half1));
      pReturn[half1++] = 0;

      DTRACE((TC_LIB, TT_API1, "AddEntrySection: write0 at %d", half1));
      pReturn[half1++] = 0;

      DTRACE((TC_LIB, TT_API1, "AddEntrySection: add new length %d output %p %d/%d", half1, pReturn, pReturn[half1-2], pReturn[half1-1]));
      DTRACE((TC_LIB, TT_API1, "AddEntrySection: add new length %d output %p", half1, pReturn));
      DTRACEBUF((TC_LIB, TT_API1, pReturn, half1));
   }

if (pNewEntry) {
   free(pNewEntry);
}

   return(pReturn);
}

/*******************************************************************************
 *
 *  GetSectionString
 *
 *  ENTRY:
 *      lpszSection (input)
 *          Memory buffer to search.
 *      lpszEntry (input)
 *          Key name to search for associated entry.
 *      lpszDefault (input)
 *          String to return if lpszEntry not found.
 *      lpszDefault (input)
 *          String to return if lpszEntry not found.
 *      cbReturnBuffer (input)
 *          Maximum number of characters that can be written to lpszReturnBuffer.
 *
 *  EXIT:
 *      (int) number of characters written to lpszReturnBuffer (not including
 *          the terminating NULL).
 *
 ******************************************************************************/

int GetSectionString( PCHAR lpszSection,
                      PCHAR lpszEntry,
                      PCHAR lpszDefault,
                      PCHAR lpszReturnBuffer,
                      int   cbReturnBuffer )
{
    int   cb;

    /*
     * Validate parameters.
     */
    ASSERT(lpszSection != NULL, 0);
    ASSERT(lpszEntry != NULL, 0);
    ASSERT(lpszDefault != NULL, 0);
    ASSERT(lpszReturnBuffer != NULL, 0);

    /*
     * Search for string in buffer
     */
    if ( !(cb = _w_gpps( lpszSection, lpszEntry, -1,
                         lpszReturnBuffer, cbReturnBuffer, -1 )) ) {

        /*
         * Not found - use default.
         */
        strncpy(lpszReturnBuffer, lpszDefault, cbReturnBuffer);
        lpszReturnBuffer[cbReturnBuffer-1] = 0;
        cb = strlen(lpszReturnBuffer);
    }
    return(cb);
}

VOID EatWhiteSpace( PCHAR pString )
{
    PCHAR pTemp = NULL;
    PCHAR pTempHead = NULL;
    UINT  p;

    pTemp = (PCHAR) malloc(MAX_INI_LINE);

    strcpy(pTemp, pString);
    pTempHead = pTemp;        //Save head of temp so we may free full string

    //trim left: spaces and tabs
    p = 0;
    while ( (pTemp[p] == ' ') || (pTemp[p] == '\t') ) { p++; }
    pTemp = &pTemp[p];

    //trim right : newline, return, tab, spaces
    p = strlen(pTemp);
    while ( p > 0 && (
	      (pTemp[p-1] == '\n') || (pTemp[p-1] == '\r') 
           || (pTemp[p-1] == ' ' ) || (pTemp[p-1] == '\t') )
	)
       { pTemp[--p] = 0; }

    strcpy(pString, pTemp);
    free(pTempHead);
}

