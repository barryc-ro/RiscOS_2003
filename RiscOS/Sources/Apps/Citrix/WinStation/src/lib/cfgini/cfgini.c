
/*************************************************************************
*
*   CFGINI.C
*
*   UI Configuration INI library
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/12/95) [from Kurt Perry's CFG library]
*
*   cfgini.c,v
*   Revision 1.1  1998/01/12 11:37:14  smiddle
*   Newly added.#
*
*   Version 0.01. Not tagged
*
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

// local includes
#include "cfginip.h"
#include "inidef.h"

/*=============================================================================
==   Local Functions Used
=============================================================================*/

int CombineAndLoadSession( HANDLE, PCHAR, PCFGINIOVERRIDE,
                           PCHAR, PCHAR, PCHAR, PCHAR * );
int MergeAndLoadPd( HANDLE, PCHAR, PCHAR, PCHAR );
int MergeAndLoadWd( HANDLE, PCHAR, PCHAR ,PCHAR );
int MergeAndLoadVd( HANDLE, PCHAR, PCHAR, PCHAR, PCHAR );
int GetModuleName( PCHAR *, PCHAR );
PLIST BuildList( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR );
PCHAR GetFirstElement( PCHAR );
PCHAR GetNextElement( PCHAR );
VOID  DestroyList( PLIST );
int   SupportRequired( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, PCHAR );
void  GetString( PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, PCHAR, int );
int GetSection( PCHAR, PCHAR, PCHAR * );
int GetSectionLength( PCHAR );
PCHAR MergeSections( PCHAR, PCHAR, PCHAR );
PCHAR MergePrivateProfileEntries( PCHAR, PCHAR );
PCHAR CombinePrivateProfileEntries( PCHAR, PCHAR );
PCHAR BuildCfgIniOverridesSection( PCFGINIOVERRIDE );
char * basename( char *);
int GetPrivateProfileSectionDefault( PCHAR pSection, PCHAR pBuf, int cbLen, PCHAR pFile);
int GetPrivateProfileStringDefault( PCHAR pSection, PCHAR pKey, PCHAR pBuf, int cbLen, PCHAR pFile);

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

/*=============================================================================
==   If we're building for DOS model, we'll get our bINI routines through
== the proctable method.  Otherwise (for Windows UI), we'll snarf in the guts
== of the bINI APIs and build them in static-like.  To make like easier for
== Windows UI OEMs wanting to add CFGINI support (or not).
=============================================================================*/
#ifdef DOS
#include "../../inc/biniapi.h"
#else
#include "../bini/biniguts.c"
#endif

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
    PLIST  pList = NULL;
    PLIST  pSave = NULL;
    PCHAR  pModem = NULL;
    PCHAR  pDriverList = NULL;
    PCHAR  pConnSection = NULL;
    PCHAR  pPdSection = NULL;
    PCHAR  pPdSection2 = NULL;
    PCHAR  pVdSection = NULL;
    PCHAR  pVdSectionGlobal = NULL;
    PCHAR  pWdSection = NULL;
    PCHAR  pDeviceSection = NULL;
    char   *pEncryption;
    DEVICENAME pszDeviceName;
    PCHAR pTemp;
    BOOL bEncryption = FALSE;
    BOOL bInternetConnection = FALSE;
    char Buffer[sizeof(INI_CLIENTNAME) + sizeof(CLIENTNAME) + 4];
    int i;
    ENCRYPTIONLEVEL szEncryptionLevelSession; 

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
    pEncryption = (char*) malloc(15);      // storage for our encryption PD trap
    memset(pEncryption, 0, sizeof(pEncryption));
    TRACE((TC_LIB, TT_API1, "CfgIniLoad: Connection %s, Server File %s, Protocol File %s", pConnection, pServerFile, pProtocolFile));

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
         * the device.  Check there before failing.
         */
        GetPrivateProfileString( pConnection,
                                 INI_DEVICE,
                                 INI_EMPTY,
                                 pszDeviceName,
                                 sizeof(pszDeviceName),
                                 pServerFile );
        if ( *pszDeviceName ) {
            GetPrivateProfileString( pszDeviceName,
                                     INI_TRANSPORTDRIVER,
                                     INI_SERIAL,    // default to standard COM
                                     pPdType,
                                     MAX_INI_LINE,
                                     pConfigFile );
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
    TRACE((TC_LIB, TT_API1, "CfgIniLoad: Protocol Type %s", pPdType));

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

    /*
     *  allocate space for driver list
     */
    if ( (pDriverList = (PCHAR) malloc( MAX_INI_LINE )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    /*         
     * Form combined connection buffered INI section (with command-line
     * overrides) and load session.
     */
    if ( rc = CombineAndLoadSession( hClientHandle, pConnection, 
                                     pCfgIniOverrides,
                                     pServerFile, pConfigFile, 
                                     pProtocolFile, &pConnSection ) ) {
        goto done;
    }

    /*
     *  get the primary sections buffered for passing to MergeAndLoadXd calls
     */
    if ( (rc = GetSection( pPdType, pProtocolFile, &pPdSection )) 
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_PD_SECTION_NOT_FOUND;
        goto done;
    }
    else if ( (rc = GetSection( pWdType, pProtocolFile, &pWdSection ))
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_WD_SECTION_NOT_FOUND;
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

    if ( *pszDeviceName ) {
        if ( (rc = GetSection( pszDeviceName, pConfigFile, &pDeviceSection ))
                        != CLIENT_STATUS_SUCCESS ) {
            if ( rc == -1 )
                rc = CLIENT_ERROR_DEVICE_SECTION_NOT_FOUND;
            goto done;
        }
    }

    /*
     * Build the ClientName=xxx" entry and then Combine with pPdSection.
     */
    sprintf( Buffer, "%s=%s", INI_CLIENTNAME, gszClientName );
    if ( (pTemp = CombinePrivateProfileEntries( pPdSection, Buffer )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    free( pPdSection );
    pPdSection = pTemp;

    /*
     *  load transport Pd
     */
    if ( rc = MergeAndLoadPd( hClientHandle, pConnSection, 
                              pPdSection, pDeviceSection ) ) {
        goto done;
    }

    //find out if this connection was made by an Internet client
    //this will update the bInternetConnection boolean.

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

        
          
            if ( !SupportRequired( pProtocolFile, pPdType, 
                               pServerFile, pConnection,
                               pList->pElement, pDeviceSection ) )
            continue;

        /*
         *  get the secondary pd section for this pd
         */

        //trap here  for the special processing for Encrypt Pd.
        if ( stricmp(pList->pElement, "Encrypt") == 0 ) { // this is the encryption pd

            bEncryption = TRUE;   // this connection needs Encryption Pd

            // get the encryption Level from the CfgIniOverrrides
            
            i = 0;
            while ( pCfgIniOverrides[i].pszKey ) {
                if ( stricmp(pCfgIniOverrides[i].pszKey, INI_ENCRYPTIONDLL ) == 0) {
                     strcpy(pEncryption, pCfgIniOverrides[i].pszValue);   // make a copy
                     pList->pElement = pEncryption;
                     break;
                }
                else                         
                    i++;
            }

        }
                    
        if ( bEncryption && bInternetConnection) {   

            // for Internet connections we will use the section in
            // the ICAFile to get the encryption *.dll

          if ( (rc = GetSection( pList->pElement, 
                                 pServerFile, &pPdSection2 ))
                          != CLIENT_STATUS_SUCCESS ) {
              if ( rc == -1 )
                  rc = CLIENT_ERROR_PD_SECTION_NOT_FOUND;
              goto done;
          }
        }
        else {

          if ( (rc = GetSection( pList->pElement, 
                                 pProtocolFile, &pPdSection2 ))
                          != CLIENT_STATUS_SUCCESS ) {
              if ( rc == -1 )
                  rc = CLIENT_ERROR_PD_SECTION_NOT_FOUND;
              goto done;
          }

        }

        /*
         *  load the current Pd
         */
        if ( rc = MergeAndLoadPd( hClientHandle, pConnSection, 
                                  pPdSection2, pDeviceSection ) ) {
            goto done;
        }

        //  free sections
        free( pPdSection2 );
        pPdSection2 = NULL;
    }

    /*
     *  free pd list memory
     */
    DestroyList( pSave );
    pSave = NULL;

    /*
     *  load winstation driver next
     */
    if ( rc = MergeAndLoadWd( hClientHandle, pConnSection, 
                              pPdSection, pWdSection ) ) {
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
         *  get the vd section
         */
        if ( (rc = GetSection( pList->pElement, 
                               pProtocolFile, &pVdSection )) 
                        != CLIENT_STATUS_SUCCESS ) {
            if ( rc == -1 )
                rc = CLIENT_ERROR_VD_SECTION_NOT_FOUND;
            goto done;
        }

        /*
         *  get the global vd section (optional)
         */
        TRACE(( TC_LIB, TT_API1, "CfgIniLoad: get global vd section" ));
        GetSection( pList->pElement, pConfigFile, &pVdSectionGlobal );

        /*
         *  load the current VD
         */
        if ( rc = MergeAndLoadVd( hClientHandle, pConnSection, pWdSection, 
                                  pVdSection, pVdSectionGlobal ) )  {
            switch ( rc ) {
                case CLIENT_ERROR_DRIVER_UNSUPPORTED:  // Non-fatal error
                   rc = CLIENT_STATUS_SUCCESS; 
                   break;

                default:
                   free( pVdSection );
                   if ( pVdSectionGlobal )
                      free( pVdSectionGlobal );
                   goto done;
                   break;
            }
        }

        //  free section
        free( pVdSection );
        if ( pVdSectionGlobal )
           free( pVdSectionGlobal );
    }

    /*
     *  free vd list memory
     */
    DestroyList( pSave );
    pSave = NULL;
done:
    if ( pSave )
        DestroyList( pSave );

    if ( pDriverList )
        free( pDriverList );

    if ( pConnSection )
        free( pConnSection );

    if ( pPdSection )
        free( pPdSection );

    if ( pPdSection2 )
        free( pPdSection2 );

    if ( pWdSection )
        free( pWdSection );

    if ( pDeviceSection )
        free( pDeviceSection );

    if ( pPdType )
        (void) free( pPdType );

    if ( pWdType )
        (void) free( pWdType );

    /*
     *  On error unload drivers
     */
    if ( rc != CLIENT_STATUS_SUCCESS )
       WFEngUnloadDrivers( hClientHandle );

#if defined(DOS) || defined(RISCOS)
    /*
     * On success, flush INI cache to save memory prior to connection
     * SJM: and to close the files.
     */
    if ( rc == CLIENT_STATUS_SUCCESS )
        FlushPrivateProfileCache();
#endif

    return( rc );
}

/*******************************************************************************
 *
 *  CombineAndLoadSession
 *
 *      Combines optional CfgIni overrides with the [pConnection] 
 *      section of the Server file, the [INI_WFCLIENT] section of the Server 
 *      file and the [INI_WFCLIENT] section of the Configuration file, with 
 *      precidence of common entries in that order.
 *
 *      Also will combine additional support sections as defined in the 
 *      <BUGBUG define this> section of the <BUGBUG define this> .INI file.
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pConnection (input)
 *          Name of connection entry in server file.
 *      pCfgIniOverrides (input)
 *          NULL for no overrides, or points to array of CFGINIOVERRIDE 
 *          structures of key / value pairs to combine with [pConnection]
 *          section of the Server file.
 *      pServerFile (input)
 *          Server INI file name.
 *      pConfigFile (input)
 *          Config INI file name.
 *      pProtocolFile (input)
 *          Protocol INI file name.
 *      ppConnSection (output)
 *          Points to place to store final combined buffered INI section for 
 *          connection entry.
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 *      NOTE: the caller must free the buffer pointer to by *ppConnSection
 *              when done.
 *
 ******************************************************************************/

int
CombineAndLoadSession( HANDLE hClientHandle,
                       PCHAR pConnection,
                       PCFGINIOVERRIDE pCfgIniOverrides,
                       PCHAR pServerFile,
                       PCHAR pConfigFile,
                       PCHAR pProtocolFile,
                       PCHAR *ppConnSection )
{
    int    rc, cbCombinedSection;
    PCHAR   pSection1 = NULL,
            pSection2 = NULL,
            pCombinedSection = NULL;
    WFELOAD WFELoad;

    /*
     * Build a buffered INI section from the CfgIni overrides.  Note: this 
     * section may be 'empty'.
     */
    if ( (pSection1 = 
            BuildCfgIniOverridesSection( pCfgIniOverrides )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: CfgIniOverridesSection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection1, 
              (ULONG)bGetSectionLength(pSection1) ));

    /*
     * Get entry section.
     */
    if ( (rc = GetSection( pConnection, pServerFile, &pSection2 )) 
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_ENTRY_NOT_FOUND;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: EntrySection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection2, 
              (ULONG)bGetSectionLength(pSection2) ));

    /*
     * Combine CfgIniOverridesSection with EntryConfig section.
     */
    if ( (pCombinedSection = 
            CombinePrivateProfileEntries( pSection1, 
                                          pSection2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: CfgIniOverridesSection+EntrySection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)bGetSectionLength(pCombinedSection) ));

    /*
     * Cleanup and combine resulting section with ServerConfig section.
     */
    free( pSection1 );
    free( pSection2 );
    pSection1 = pCombinedSection;
    pCombinedSection = pSection2 = NULL;
    if ( (rc = GetSection( INI_WFCLIENT, pServerFile, &pSection2 )) 
                        != CLIENT_STATUS_SUCCESS ) {
        // don't fail if we don't have WFCLIENT in ICA file
        pCombinedSection = pSection1;
        pSection1 = NULL;
        goto skipcombine1;
    }
    TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: ServerConfigSection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection2, 
              (ULONG)bGetSectionLength(pSection2) ));

    if ( (pCombinedSection = 
            CombinePrivateProfileEntries( pSection1, 
                                          pSection2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: ...+ServerConfigSection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)bGetSectionLength(pCombinedSection) ));

skipcombine1:
    /*
     * Cleanup and combine resulting section with ConfigConfig section.
     */
    if(pSection1)
       free( pSection1 );

    if(pSection2)
       free( pSection2 );

    pSection1 = pCombinedSection;
    pCombinedSection = pSection2 = NULL;
    if ( (rc = GetSection( INI_WFCLIENT, pConfigFile, &pSection2 )) 
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_CONFIG_CONFIG_NOT_FOUND;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: ConfigConfigSection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection2, 
              (ULONG)bGetSectionLength(pSection2) ));

    if ( (pCombinedSection = 
            CombinePrivateProfileEntries( pSection1, 
                                          pSection2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    /*
     * The mandantory sections are combined now.  Save away the current 
     * section size of pCombinedSection now so we can realloc it back down 
     * to this size after WFEngLoadSession returns.
     */
    cbCombinedSection = bGetSectionLength(pCombinedSection);
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: ...+ConfigConfigSection" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)cbCombinedSection ));

    /*
     * Cleanup and combine mandantory section with optional section(s).
     */
    /*
     * HotkeyShiftStates
     */
    if(pSection1)
       free( pSection1 );

    if(pSection2)
       free( pSection2 );

    pSection1 = pCombinedSection;
    pCombinedSection = pSection2 = NULL;
    if ( (rc = GetSection( INI_HOTKEY_SHIFTSTATES, pProtocolFile, &pSection2 )) 
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_HOTKEY_SHIFTSTATES_NOT_FOUND;
        //goto done;
    }
    if(pSection2) {
       TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: HotkeyShiftStates" ));
       TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection2, 
                 (ULONG)bGetSectionLength(pSection2) ));
       }

    if ( (pCombinedSection = 
            CombinePrivateProfileEntries( pSection1, 
                                          pSection2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: ...+HotkeyShiftStates" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)bGetSectionLength(pCombinedSection) ));

    /*
     * HotkeyKeys
     */
    if(pSection1)
       free( pSection1 );
    if(pSection2)
       free( pSection2 );

    pSection1 = pCombinedSection;
    pCombinedSection = pSection2 = NULL;
    if ( (rc = GetSection( INI_HOTKEY_KEYS, pProtocolFile, &pSection2 )) 
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_HOTKEY_KEYS_NOT_FOUND;
        //goto done;
    }
    if(pSection2) {
       TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: HotkeyKeys" ));
       TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection2, 
                 (ULONG)bGetSectionLength(pSection2) ));
       }

    if ( (pCombinedSection = 
            CombinePrivateProfileEntries( pSection1, 
                                          pSection2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: ...+HotkeyKeys" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)bGetSectionLength(pCombinedSection) ));


    /*
     * Keyboard Layout
     */
    if(pSection1)
       free( pSection1 );
    if(pSection2)
       free( pSection2 );
    pSection1 = pCombinedSection;
    pCombinedSection = pSection2 = NULL;
    if ( (rc = GetSection( INI_KEYBOARDLAYOUT, pProtocolFile, &pSection2 )) 
                        != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_KEYBOARDLAYOUT_NOT_FOUND;
        //goto done;
    }
    if(pSection2) {
       TRACE((TC_LIB, TT_API1, "CombineAndLoadSession: KeyboardLayout" ));
       TRACEBUF((TC_LIB, TT_API1, (PCHAR)pSection2, 
                (ULONG)bGetSectionLength(pSection2) ));
       }

    if ( (pCombinedSection = 
            CombinePrivateProfileEntries( pSection1, 
                                          pSection2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: ...+KeyboardLayout" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)bGetSectionLength(pCombinedSection) ));

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
    WFELoad.pIniSection = pCombinedSection;
    rc = WFEngLoadSession( hClientHandle, &WFELoad );

    /*
     * Realloc pCombinedSection back down to mandantory stuff.
     */
    *(pCombinedSection+(cbCombinedSection-1)) = '\0';   // make double NULL again
    pCombinedSection = (PCHAR)realloc( pCombinedSection, cbCombinedSection );
    TRACE((TC_LIB, TT_API1, 
           "CombineAndLoadSession: FinalCombinedSection (mandantory stuff only)" ));
    TRACEBUF((TC_LIB, TT_API1, (PCHAR)pCombinedSection, 
              (ULONG)bGetSectionLength(pCombinedSection) ));

    /*
     * Pass back final combined section and return.
     */
    *ppConnSection = pCombinedSection;

done:
    if ( pSection1 )
        free( pSection1 );

    if ( pSection2 )
        free( pSection2 );

    return( rc );
}

/*******************************************************************************
 *
 *  MergeAndLoadPd
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pConnSection (input)
 *          Points to buffered INI section for connection entry.
 *      pPdSection (input)
 *          Points to buffered INI section for the Pd entry.
 *      pPdSectionOptional (input)
 *          Points to optional buffered INI section for the Pd device.
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
MergeAndLoadPd( HANDLE   hClientHandle,
                PCHAR    pConnSection,
                PCHAR    pPdSection,
                PCHAR    pPdSectionOptional )
{
    PCHAR  pModuleName = NULL;
    int    rc;
    PCHAR  pMergedSection = NULL, pS1, pS2;
    WFELOAD WFELoad;

    TRACE((TC_LIB, TT_API1, "MergeAndLoadPd: pConnSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pConnSection, 
               (ULONG)bGetSectionLength(pConnSection) ));
    TRACE((TC_LIB, TT_API1, "MergeAndLoadPd: pPdSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pPdSection, 
               (ULONG)bGetSectionLength(pPdSection) ));
    if ( pPdSectionOptional ) {
       TRACE((TC_LIB, TT_API1, "MergeAndLoadPd: pPdSectionOptional" ));
       TRACEBUF((TC_LIB, TT_API1, (char far *)pPdSectionOptional, 
                  (ULONG)bGetSectionLength(pPdSectionOptional) ));
        pS1 = pPdSectionOptional;
        pS2 = pPdSection;
    } else {
        pS1 = pPdSection;
        pS2 = NULL;
    }

    /*
     *  merge profile sections
     */
    if ( (pMergedSection = MergeSections( pConnSection, pS1, pS2 )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    TRACE((TC_LIB, TT_API1, "MergeAndLoadPd: pMergedSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pMergedSection, 
               (ULONG)bGetSectionLength(pMergedSection) ));

    /*
     * load the PD
     */
    if ( (rc = GetModuleName(&pModuleName, pPdSection)) != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_PD_NAME_NOT_FOUND;
        goto done;
    }
    WFELoad.pszModuleName = (LPSTR)pModuleName;
    WFELoad.pIniSection = (LPVOID)pMergedSection;
    rc = WFEngLoadPd( hClientHandle, &WFELoad );

done:
    if ( pModuleName )
        (void) free(pModuleName);

    if ( pMergedSection )
        (void) free( pMergedSection );

    return( rc );
}

/*******************************************************************************
 *
 *  MergeAndLoadWd
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pConnSection (input)
 *          Points to buffered INI section for connection entry.
 *      pPdSection (input)
 *          Points to buffered INI section for the Pd (transport) entry.
 *      pWdSection (input)
 *          Points to buffered INI section for the Wd entry.
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
MergeAndLoadWd( HANDLE   hClientHandle,
                PCHAR    pConnSection,
                PCHAR    pPdSection,
                PCHAR    pWdSection )
{
    PCHAR  pModuleName = NULL;
    int    rc;
    PCHAR  pMergedSection = NULL;
    WFELOAD WFELoad;

    TRACE((TC_LIB, TT_API1, "MergeAndLoadWd: pConnSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pConnSection, 
               (ULONG)bGetSectionLength(pConnSection) ));
    TRACE((TC_LIB, TT_API1, "MergeAndLoadWd: pPdSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pPdSection, 
               (ULONG)bGetSectionLength(pPdSection) ));
    TRACE((TC_LIB, TT_API1, "MergeAndLoadWd: pWdSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pWdSection, 
               (ULONG)bGetSectionLength(pWdSection) ));

    /*
     *  merge profile sections
     */
    if ( (pMergedSection = MergeSections( 
                                pConnSection, 
                                pPdSection, pWdSection )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    TRACE((TC_LIB, TT_API1, "MergeAndLoadWd: pMergedSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pMergedSection, 
               (ULONG)bGetSectionLength(pMergedSection) ));

    /*
     * Load the WD
     */
    if ( (rc = GetModuleName(&pModuleName, pWdSection)) != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_WD_NAME_NOT_FOUND;
        goto done;
    }
    WFELoad.pszModuleName = (LPSTR)pModuleName;
    WFELoad.pIniSection = (LPVOID)pMergedSection;
    rc = WFEngLoadWd( hClientHandle, &WFELoad );

done:
    if ( pModuleName )
        (void) free(pModuleName);

    if ( pMergedSection )
        (void) free( pMergedSection );

    return( rc );
}

/*******************************************************************************
 *
 *  MergeAndLoadVd
 *
 *  ENTRY:
 *      hClientHandle (input)
 *          HANDLE of the client to load (from WFEngOpen call).
 *      pConnSection (input)
 *          Points to buffered INI section for connection entry.
 *      pWdSection (input)
 *          Points to buffered INI section for the Wd entry.
 *      pVdSection (input)
 *          Points to buffered INI section for the Vd entry.
 *      pVdSectionGlobal (input)
 *          Points to buffered INI section for the Vd entry's global settings.
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
MergeAndLoadVd( HANDLE   hClientHandle,
                PCHAR    pConnSection,
                PCHAR    pWdSection,
                PCHAR    pVdSection,
                PCHAR    pVdSectionGlobal )
{
    PCHAR  pModuleName = NULL;
    int    rc;
    PCHAR  pMergedSection = NULL;
    PCHAR  pMergedVdSection = NULL;
    PCHAR  pVdSec;
    WFELOAD WFELoad;

    TRACE((TC_LIB, TT_API1, "MergeAndLoadVd: pConnSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pConnSection, 
               (ULONG)bGetSectionLength(pConnSection) ));
    TRACE((TC_LIB, TT_API1, "MergeAndLoadVd: pWdSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pWdSection, 
               (ULONG)bGetSectionLength(pWdSection) ));
    TRACE((TC_LIB, TT_API1, "MergeAndLoadVd: pVdSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pVdSection, 
               (ULONG)bGetSectionLength(pVdSection) ));
    TRACE((TC_LIB, TT_API1, "MergeAndLoadVd: pVdSectionGlobal" ));
    if (pVdSectionGlobal)
    {
	TRACEBUF((TC_LIB, TT_API1, (char far *)pVdSectionGlobal, 
		  (ULONG)bGetSectionLength(pVdSectionGlobal) ));
    }

    /*
     * Merge the VdSection with the global overrides
     */
    if ( pVdSectionGlobal ) {
       if ( (pMergedVdSection = MergeSections( 
                                    pVdSectionGlobal, 
                                    pVdSection, NULL )) == NULL ) {
           rc = CLIENT_ERROR_NO_MEMORY;
           goto done;
       }
       pVdSec = pMergedVdSection;
    } else {
       pVdSec = pVdSection;
    }

    TRACE((TC_LIB, TT_API1, "MergeAndLoadVd: pVdSec" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pVdSec, 
               (ULONG)bGetSectionLength(pVdSec) ));

    /*
     *  merge profile sections
     */
    if ( (pMergedSection = MergeSections( 
                                pConnSection, 
                                pWdSection, pVdSec )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    TRACE((TC_LIB, TT_API1, "MergeAndLoadVd: pMergedSection" ));
    TRACEBUF((TC_LIB, TT_API1, (char far *)pMergedSection, 
               (ULONG)bGetSectionLength(pMergedSection) ));

    /*
     * Load the Vd
     */
    if ( (rc = GetModuleName(&pModuleName, pVdSection)) != CLIENT_STATUS_SUCCESS ) {
        if ( rc == -1 )
            rc = CLIENT_ERROR_VD_NAME_NOT_FOUND;
        goto done;
    }
    WFELoad.pszModuleName = (LPSTR)pModuleName;
    WFELoad.pIniSection = (LPVOID)pMergedSection;
    rc = WFEngLoadVd( hClientHandle, &WFELoad );

done:
    if ( pModuleName )
        (void) free(pModuleName);

    if ( pMergedSection )
        (void) free( pMergedSection );

    if ( pMergedVdSection )
        (void) free( pMergedVdSection );

    return( rc );
}

/*******************************************************************************
 *
 *  GetModuleName
 *
 *      Retrieve the driver name from the specified bufered INI section.
 *
 *  ENTRY:
 *
 *  EXIT:
 *      (int) return code: CLIENT_STATUS_SUCCESS - if no error;
 *                         else CLIENT_ERROR_xxx error code
 *
 ******************************************************************************/

int
GetModuleName( PCHAR *ppModuleName, PCHAR pSection )
{
    int rc = CLIENT_STATUS_SUCCESS;

    /*
     *  allocate space for driver string
     */
    if ( (*ppModuleName = (PCHAR) malloc( MAXDRIVER )) == NULL ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }

    /*
     *  get dll name string from section buffer
     */
    if ( !bGetPrivateProfileString( pSection,
                                    INI_DRIVERNAME,
                                    INI_EMPTY,
                                    *ppModuleName,
                                    MAXDRIVER ) ) {
        free(*ppModuleName);
        *ppModuleName = NULL;
        rc = -1;   // error code resolved by caller
        goto done;
    }

    /*
     * If this driver is not supported by this emulator, fail this load
     * but don't necessarily fail the connection
     */
    if ( !stricmp( *ppModuleName, INI_DRIVERUNSUPPORTED ) ) {
        free( *ppModuleName );
        *ppModuleName = NULL;
        rc = CLIENT_ERROR_DRIVER_UNSUPPORTED;
    }

done:
    return(rc);
}

/*******************************************************************************
 *
 *  SupportRequired
 *
 *  ENTRY:
 *
 *  EXIT:
 *      CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int
SupportRequired( PCHAR pPFile, PCHAR pPSec, PCHAR pSFile, PCHAR pSSec,
                 PCHAR pEntry, PCHAR pDeviceSection )
{
    int   rc = FALSE;           // support not required
    CHAR  achAnswer[10];

    /*
     * If we're checking for Modem = On, we need to check the config file
     * for the associated device
     */
    TRACE((TC_LIB, TT_API1, "SupportRequired: pDevSec(%s)",
                  (pDeviceSection ? pDeviceSection : "<NULL>")));
    if ( pDeviceSection && !stricmp( pEntry, INI_MODEM ) ) {
       bGetPrivateProfileString( pDeviceSection, pEntry, INI_EMPTY,
                                 achAnswer, sizeof(achAnswer) );
    } else {

       /*
        *  get answer from ini files
        */
       GetString( pPSec, pPFile, pSSec, pSFile, pEntry, achAnswer, 10 );
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
        if( !GetPrivateProfileStringDefault( pPSec,
                                             pEntry,
                                             pEntryBuf,
                                             MAX_INI_LINE,
                                             pPFile) ) 
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
    if ( !GetPrivateProfileStringDefault( pSSec,
                                   pEntry,
                                   pEntryBuf,
                                   MAX_INI_LINE,
                                   pSFile ) ) 
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
GetString( PCHAR pPSec, PCHAR pPFile, PCHAR pSSec, PCHAR pSFile, PCHAR pEntry, PCHAR pString, int cbString )
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

           GetPrivateProfileStringDefault( pPSec, pEntry, pString, cbString, pPFile);
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


#if defined( DOS ) || defined( WIN32 ) || defined( RISCOS )
/*******************************************************************************
 *
 *  GetSection (DOS and WIN32)
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
            PCHAR *ppSectionBuffer )
{
    int rc = CLIENT_STATUS_SUCCESS;
    int   cb, cbSection = 0;
    PCHAR pSectionBuffer = NULL;


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
        if ( (pSectionBuffer = (PCHAR) malloc( cbSection )) == NULL ) {
            rc = CLIENT_ERROR_NO_MEMORY;
            goto ErrorReturn;
        }

        /*
         * try and read this section
         */
#if defined( DOS ) || defined( RISCOS )
        if ( !(cb = GetPrivateProfileString( pSectionName,
                                             NULL,
                                             INI_EMPTY,
                                             pSectionBuffer,
                                             cbSection,
                                             pFileName )) ) {
#else
        // win32 has a strange bug were it will create an empty INI file
        // if the requested INI file is not present.
        // to get around this, we will check for the INI file first.
        if ( (_access(pFileName,0)) || 
             !(cb = GetPrivateProfileSection( pSectionName,
                                              pSectionBuffer,
                                              cbSection,
                                              pFileName )) ) {
#endif
            cb = GetPrivateProfileSectionDefault( pSectionName,
                                                  pSectionBuffer,
                                                  cbSection,
                                                  pFileName);
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
            (void) free( pSectionBuffer );
        else
            break;  // got 'em all
    }

    /*
     * realloc to true size
     */
    pSectionBuffer = (PCHAR) realloc( pSectionBuffer, cb+1 );

done:
    *ppSectionBuffer = pSectionBuffer;
    return( rc );

//----------------
// error return
//----------------
ErrorReturn:
    if ( pSectionBuffer )
        free(pSectionBuffer);
    pSectionBuffer = NULL;
    goto done;

}  // end GetSection (DOS and WIN32)
#endif


#ifdef WIN16
/*******************************************************************************
 *
 *  GetSection (WIN16 - needs multiple steps to do what we want)
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
            PCHAR *ppSectionBuffer )
{
    int rc = CLIENT_STATUS_SUCCESS;
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
            // try using default section values before
            // giving up
            cb = GetPrivateProfileSectionDefault( pSectionName,
                                                  pEntriesBuffer,
                                                  cbEntries,
                                                  pFileName);
            if(cb==0) {                          
               rc = -1;    // 'not found'
               goto ErrorReturn;
               }
            else {
                /*
                 * realloc to true size
                 */
                pEntriesBuffer = (PCHAR) realloc( pEntriesBuffer, cb+1 );
                
                *ppSectionBuffer = pEntriesBuffer;
                return( rc );
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
#endif

/*******************************************************************************
 *
 *  MergeSections
 *
 *  ENTRY:
 *      pS0 (input)
 *          Primary section for merge.
 *      pS1 (input)
 *          Section to merge common pS0 entries into.
 *      pS2 (input)
 *          Optional section.  If not NULL, will first merge pS1 entries into
 *          this section.  Then, will merge the common pS0 entries into the
 *          intermediate merged section.
 *  EXIT:
 *      (PCHAR) pointer to final merged section
 *
 ******************************************************************************/

PCHAR
MergeSections( PCHAR pS0, PCHAR pS1, PCHAR pS2 )
{
    PCHAR pTemp;
    PCHAR pMergedSection = NULL;

    /*
     *  if optional section exists merge it first
     */
    if ( pS2 ) {

        TRACE((TC_LIB, TT_API1, "MergeSections: optional merge"));

        if ( (pTemp = MergePrivateProfileEntries( pS2, pS1 )) == NULL )
            return( NULL );
    }
    else {
        pTemp = pS1;
    }

    /*
     *  create final merged section
     */
    TRACE((TC_LIB, TT_API1, "MergeSections: final merge"));
    pMergedSection = MergePrivateProfileEntries( pTemp, pS0 );

    // if allocate buffer, free
    if ( pS2 )
        (void) free( pTemp );

    //  return merged string
    return( pMergedSection );
}

/*******************************************************************************
 *
 *  MergePrivateProfileEntries
 *
 *      This routine creates an output profile section consisting of all
 *      entry=value pairs in lpszPrimary, but with the values obtained from 
 *      lpszSecondary for entries which appear in both sections.
 *
 *  ENTRY:
 *      lpszPrimary (input)
 *          Primary profile entries.
 *      lpszSecondary (input)
 *          Secondary profile entries to merge into primary.
 *  EXIT:
 *      (PCHAR) pointer to merged profile entries; NULL if not enough
 *          memory to perform merge.
 *
 ******************************************************************************/

PCHAR
MergePrivateProfileEntries( PCHAR lpszPrimary, PCHAR lpszSecondary )
{
    int   len;
    int   cb;
    int   cbBufCur = 0;
    int   cbBufMax = SECTIONSIZE;
    PCHAR pSection = lpszPrimary;
    PCHAR pReturn  = NULL;
    PCHAR pEntry   = NULL;
    PCHAR pString  = NULL;
    PCHAR pLine    = NULL;
    PCHAR pBuf     = NULL;
    PCHAR p        = NULL;

    //  malloc for return entries
    if ( (pReturn = (PCHAR) malloc( cbBufMax )) == NULL )
        goto NoMemory;

    //  init return string and other junk
    memset( pReturn, 0, cbBufMax );

    //  malloc buffer for secondary profile entry
    if ( (pBuf = (PCHAR) malloc(MAX_INI_LINE)) == NULL )
        goto NoMemory;

    /*
     *  search while there are any entries left
     */
    while ( (len = strlen( pSection )) ) {

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

            //  now look for entry match in secondary string
            if ( bGetPrivateProfileString( lpszSecondary,
                                           pEntry,
                                           "",
                                           pBuf,
                                           MAX_INI_LINE ) ) {

                //  point to string
                TRACE((TC_LIB, TT_API1, "Merging entry: %s -> %s", pEntry, pBuf));
                pString = pBuf;
            }
            else {

                //  find begining of string
                pString = pString + strspn( pString, " \t" );

                //  find the end of the string and terminate
                p = pString + strlen( pString ) - 1;
                while ( *p == ' ' || *p == '\t' )
                    *(p--) = 0;

            }

            //  get length of pEntry, =, pString and NULL
            cb = strlen( pEntry ) + strlen( pString ) + 2;

            //  space left in buffer? no, then grow
            if ( (cbBufCur + cb) >= cbBufMax ) {
                cbBufMax += SECTIONSIZE;
                if ( (pReturn = (char *) realloc( pReturn, cbBufMax )) == NULL )
                    goto NoMemory;
            }

            //  get pointer to current location
            p = &(pReturn[cbBufCur]);

            //  copy into buffer
            sprintf( p, "%s=%s", pEntry, pString );

            //  index to next avail location in buffer
            cbBufCur += cb;

            //  double NULL terminate (last entry will remain this way).
            pReturn[cbBufCur] = 0;
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

    //  return merged entries
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

    cbBufCur = bGetSectionLength(lpszPrimary) - 1;  // don't include final NULL

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
            if ( !bGetPrivateProfileString( lpszPrimary,
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
    int rc = CLIENT_STATUS_SUCCESS;
    int i, cbKey, cbValue,
        cbBufCur = 0, cbBufMax = SECTIONSIZE;
    PCHAR pReturn = NULL;

    /*
     * Start with default section buffer.
     */
    if ( (pReturn = (PCHAR) malloc( cbBufMax )) == NULL )
        goto NoMemory;

    /*
     * Default to an 'empty' (double null) section buffer.  Will remain this
     * way if override loop encounters no overrides.
     */
    pReturn[0] = 0;
    pReturn[1] = 0;

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
