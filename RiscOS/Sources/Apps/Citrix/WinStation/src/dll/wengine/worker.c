/*****************************************************************************
*
*   WORKER.C
*
*   Windows client engine - worker routines
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Marc Bloomfield (marcb) 27-Mar-1995
*
*   $Log$
*  
*     Rev 1.30   08 Aug 1997 21:18:54   tariqm
*  scripting support
*  
*     Rev 1.29   15 Jul 1997 15:58:12   davidp
*  Added include for hydra/picasso surgery
*  
*     Rev 1.28   15 Apr 1997 18:18:52   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.29   09 Apr 1997 13:38:48   terryt
*  fix connection status
*  
*     Rev 1.28   21 Mar 1997 16:10:06   bradp
*  update
*  
*     Rev 1.27   04 Mar 1997 17:41:52   terryt
*  client shift states
*  
*     Rev 1.26   20 Feb 1997 14:09:56   butchd
*  Added client data to connection status dialog
*  
*     Rev 1.25   22 Jan 1997 16:49:54   terryt
*  client data
*  
*     Rev 1.24   27 Apr 1996 15:50:28   andys
*  soft keyboard
*
*     Rev 1.23   14 Feb 1996 19:47:44   butchd
*  KurtP added some Keyboard LogPrintfs for Windows
*
*     Rev 1.22   12 Jan 1996 10:28:12   richa
*  Moved the setting of the CONNECTED status before the connect call to the TD so that it can be canceled when the connect call blocks in the TD.
*
*     Rev 1.21   11 Dec 1995 10:59:08   richa
*  ZDS changes.
*
*     Rev 1.20   06 Nov 1995 08:17:42   butchd
*  update
*
*     Rev 1.19   06 Oct 1995 10:53:38   kurtp
*  update
*
*     Rev 1.18   20 Jul 1995 09:54:28   marcb
*  update
*
*     Rev 1.17   14 Jul 1995 09:31:08   butchd
*  update
*
****************************************************************************/

#include "windows.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/client.h"
#include "../../inc/clib.h"
#include "../../inc/wdapi.h"
#include "../../inc/pdapi.h"
#include "../../inc/vdapi.h"
#include "../../inc/vioapi.h"
#include "../../inc/logapi.h"
#include "../../inc/cfgload.h"
#include "../../inc/biniapi.h"
#include "../../inc/wengapip.h"
#include "../../inc/timapi.h"
#include "../../inc/kbdapi.h"
/* #include "../../inc/xmsapi.h" */
#include "../../inc/encrypt.h"

#include "../../inc/sdapi.h"


#include "citrix/ctxver.h"
#include "citrix/ica.h"
#include "citrix/hydrix.h"

#include "../../inc/mouapi.h"

/*
 *  Script driver routines and variables
 */
extern INT    StopPolling( VOID );
extern HANDLE ghWFE;
DLLLINK       gSdLink;
BOOL          gSdError = FALSE;
PDLASTERROR   gSdLastError = {0};


/*******************************************************************************
 *
 *  Function: wdSetProductID
 *
 *  Purpose: Set the product ID
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdSetProductID( PWFEPRODUCTID pProductID, USHORT cbProductID )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR, "WENG: wSetProductID: Error wd not loaded" ));
       goto done;
   }

   /*
    * Set the Product ID
    */
   WdSetInfo.WdInformationClass  = WdSetProductID;
   WdSetInfo.pWdInformation      = pProductID;
   WdSetInfo.WdInformationLength = cbProductID;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR, "WENG: wdSetProductID: Error (%d) from WdSetProductID", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdSetProductID: Setting WdProductID rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdSetInfoPassthru
 *
 *  Purpose: Do a set info from engine to WD
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdSetInfoPassthru( USHORT wdInfoClass )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR, "WENG: wSetProductID: Error wd not loaded" ));
       goto done;
   }

   /*
    * Set the Product ID
    */
   WdSetInfo.WdInformationClass  = wdInfoClass;
   WdSetInfo.pWdInformation      = NULL;
   WdSetInfo.WdInformationLength = 0;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR, "WENG: wdSetInfoPassthru: Class %d Error (%d)", wdInfoClass, rc ));
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdSetInfoPassthru: Wd set info class %d", wdInfoClass, rc ));
   return( rc );
}
/*******************************************************************************
 *
 *  Function: wdSetDefaultMode
 *
 *  Purpose: Set the Default mode
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdSetDefaultMode( PWFEDEFAULTMODE pDefaultMode, USHORT cbDefaultMode )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR, "WENG: wSetDefaultMode: Error wd not loaded" ));
       goto done;
   }

   /*
    * Set the Product ID
    */
   WdSetInfo.WdInformationClass  = WdSetDefaultMode;
   WdSetInfo.pWdInformation      = pDefaultMode;
   WdSetInfo.WdInformationLength = cbDefaultMode;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR, "WENG: wdSetDefaultMode: Error (%d) from WdSetDefaultMode", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdSetDefaultMode: Setting WdDefaultMode rc(%d)", rc ));
   return( rc );
}



/*******************************************************************************
 *
 *  Function: wdSetFocus
 *
 *  Purpose: Give focus to wd
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdSetFocus( VOID  )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdSetFocus: Error wd not loaded" ));
       goto done;
   }

   /*
    * If it already has the focus, no need to set it again
    */
   if ( gState & WFES_FOCUS ) {
       TRACE(( TC_WENG, TT_L1,
          "WENG: wdSetFocus: WARNING - already has focus" ));
       goto done;
   }

   /*
    * Set the Wd focus
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass = WdSetFocus;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdSetFocus: Error (%d) from WdSetInfo-WdSetFocus", rc ));
       ASSERT( 0, 0 );
       goto done;
   }
   gState |= WFES_FOCUS;

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdSetFocus: Setting WdFocus rc(%d)", rc ));
   return( rc );
}


/*******************************************************************************
 *
 *  Function: wdKillFocus
 *
 *  Purpose: Take focus from wd
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdKillFocus( VOID )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdKillFocus: Error wd not loaded" ));
       goto done;
   }

   /*
    * If it already has the focus, no need to set it again
    */
   if ( !(gState & WFES_FOCUS) ) {
       TRACE(( TC_WENG, TT_L1,
          "WENG: wdKillFocus: WARNING - doesn't have focus" ));
       goto done;
   }

   /*
    * Kill the Wd focus
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass = WdKillFocus;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdKillFocus: Error (%d) from WdSetInfo-WdKillFocus", rc ));
       ASSERT( 0, 0 );
       goto done;
   }
   gState &= ~WFES_FOCUS;

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdKillFocus: Killing WdFocus rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdWindowSwitch
 *
 *  Purpose: Inform WD of a window switch (so it can update the keyboard)
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdWindowSwitch( VOID  )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdWindowSwitch: Error wd not loaded" ));
       goto done;
   }

   /*
    * Inform the WD
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass = WdWindowSwitch;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdWindowSwitch: Error (%d) from WdSetInfo-WdWindowSwitch", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdWindowSwitch rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdVdAddress
 *
 *  Purpose: Tell wd about vds
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdVdAddress( VOID )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdVdAddress: Error wd not loaded" ));
       goto done;
   }

   /*
    * If already done, no need to do it again
    */
   if ( gbVdAddrToWd ) {
       goto done;
   }

   /*
    * Tell the wd
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdVdAddress;
   WdSetInfo.pWdInformation      = gpaVdLink;
   WdSetInfo.WdInformationLength = sizeof(PDLLLINK)
                                 * gMaxVirtualChannels;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdVdAddress: Error (%d) from WdSetInfo-WdVdAddress", rc ));
       ASSERT( 0, 0 );
       goto done;
   }
   gbVdAddrToWd = TRUE;

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdVdAddress: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdConnect
 *
 *  Purpose: Make a connection
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdConnect( HWND hWnd  )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdConnect: ERROR - wd not loaded" ));
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       goto done;
   }

   /*
    * If already connected, abort
    */
   if ( gState & WFES_CONNECTED ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdConnect: ERROR - already connected" ));
       rc = CLIENT_ERROR_ALREADY_CONNECTED;
       goto done;
   }

   /*
    * Initialize the window
    */
   if ( rc = wdInitWindow( hWnd ) ) {
       goto done;
   }

   /*
    * Tell the wd
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdConnect;
   WdSetInfo.WdInformationLength = sizeof(PDLLLINK)
                                 * gMaxVirtualChannels;
   gState |= WFES_CONNECTED;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdConnect: Error (%d) from WdSetInfo-WdConnect", rc ));
       ASSERT( 0, 0 );
       gState &= ~WFES_CONNECTED;
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdConnect: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdDisconnect
 *
 *  Purpose: Disconnect
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdDisconnect( VOID )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdDisconnect: ERROR - wd not loaded" ));
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       goto done;
   }

   /*
    * If not connected, abort
    */
   if ( !(gState & WFES_CONNECTED) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdDisconnect: ERROR - not connected" ));
       rc = CLIENT_ERROR_NOT_CONNECTED;
       goto done;
   }

   /*
    * Make sure we don't have the focus
    */
   wdKillFocus();

   /*
    * Tell the wd
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdDisconnect;
   WdSetInfo.WdInformationLength = sizeof(PDLLLINK)
                                 * gMaxVirtualChannels;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdDisconnect: Error (%d) from WdSetInfo-WdDisconnect", rc ));
       ASSERT( 0, 0 );
       goto done;
   }
   gState &= ~WFES_CONNECTED;

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdDisconnect: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdCharCode
 *
 *  Purpose: Send character code
 *
 *  Entry:
 *     pCharCodes (input)
 *        character codes
 *     cCharCodes (input)
 *        character count
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdCharCode( PUCHAR pCharCodes, USHORT cCharCodes )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;
   CHARCODE cc;
#ifndef DOS
    USHORT i;

    for ( i=0; i<cCharCodes; i++ ) {
        LogPrintf( LOG_CLASS, LOG_KEYBOARD,
                   "KEYBOARD: char (%02X)(%c)",
                   *(pCharCodes+i), *(pCharCodes+i) );
    }
#endif

   TRACE(( TC_WENG, TT_L2,
           "WENG: wdCharCode: char(%02X)(%c) count(%d)",
           *pCharCodes, *pCharCodes, cCharCodes ));

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       goto done;
   }

   /*
    * If not connected, abort
    */
   if ( !(gState & WFES_CONNECTED) ) {
       goto done;
   }

   /*
    * Send keyboard event
    */
   cc.pCharCodes = (LPVOID) pCharCodes;
   cc.cCharCodes = cCharCodes;
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdCharCode;
   WdSetInfo.pWdInformation      = (LPVOID)&cc;
   WdSetInfo.WdInformationLength = sizeof( cc );
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdCharCode: Error (%d) from WdSetInfo-WdCharCode", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdCharCode: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdScanCode
 *
 *  Purpose: Send scan code
 *
 *  Entry:
 *     pScanCodes (input)
 *        pointer to scan code array
 *     cScanCodes (input)
 *        number of scan codes in array
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdScanCode( PUCHAR pScanCodes, USHORT cScanCodes )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;
   SCANCODE sc;
#ifndef DOS
    USHORT i;

    for ( i=0; i<cScanCodes; i++ ) {
        LogPrintf( LOG_CLASS, LOG_KEYBOARD,
                   "KEYBOARD: scan (%02X)",
                   *(pScanCodes+i) );
    }
#endif

   TRACE(( TC_WENG, TT_L2,
           "WENG: wdScanCode: scan(%02X)(%c) count(%d)",
           *pScanCodes, *pScanCodes, cScanCodes ));

   /*
    * Make sure there is a WD out there
    */
   if ( !(gState & WFES_LOADEDWD ) ) {
       goto done;
   }

   /*
    * If not connected, abort
    */
   if ( !(gState & WFES_CONNECTED) ) {
       goto done;
   }

   /*
    * Send keyboard event
    */
   sc.pScanCodes = (LPVOID) pScanCodes;
   sc.cScanCodes = cScanCodes;
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdScanCode;
   WdSetInfo.pWdInformation      = (LPVOID)&sc;
   WdSetInfo.WdInformationLength = sizeof( sc );
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdScanCode: Error (%d) from WdSetInfo-WdScanCode", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdScanCode: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdMouseData
 *
 *  Purpose: Send mouse data
 *
 *  Entry:
 *     pMouseData (input)
 *        mouse data
 *     cMouseData (input)
 *        mouse data count
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdMouseData( LPVOID pMouseData, USHORT cMouseData )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;
   MOUSEINFO mi;

   DTRACE(( TC_WENG, TT_L2, "WENG: wdMouseData: count(%d)", cMouseData ));

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       goto done;
   }

   /*
    * If not connected, abort
    */
   if ( !( gState & WFES_CONNECTED ) ) {
       goto done;
   }

   /*
    * Send mouse events
    */
   mi.pMouseData = pMouseData;
   mi.cMouseData = cMouseData;
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdMouseInfo;
   WdSetInfo.pWdInformation      = (LPVOID)&mi;
   WdSetInfo.WdInformationLength = sizeof( mi );
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       DTRACE(( TC_WENG, TT_ERROR,
          "WENG: wdMouseData: Error (%d) from WdSetInfo->WdMouseInfo", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   DTRACE(( TC_WENG, TT_L1, "WENG: wdMouseInfo: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdAddReadHook
 *
 *  Purpose: Add modem read hook
 *
 *  Entry:
 *     ModemReadHook (input)
 *        pointer to modem read hook function
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdAddReadHook( PVOID ModemReadHook )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdAddReadHook: Error wd not loaded" ));
       goto done;
   }

   /*
    * Add read hook
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdAddReadHook;
   WdSetInfo.pWdInformation      = (LPVOID)ModemReadHook;
   WdSetInfo.WdInformationLength = sizeof( ModemReadHook );
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdAddReadHook: Error (%d) from WdSetInfo-WdAddReadHook", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdAddReadHook: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdRemoveReadHook
 *
 *  Purpose: Remove modem read hook
 *
 *  Entry:
 *     VOID
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdRemoveReadHook( VOID )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdRemoveReadHook: Error wd not loaded" ));
       goto done;
   }

   /*
    * Remove read hook
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdRemoveReadHook;
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdRemoveReadHook: Error (%d) from WdSetInfo-WdRemoveReadHook", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdRemoveReadHook: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdLastError
 *
 *  Purpose: Get last wd error
 *
 *  Entry:
 *     pLastError (output)
 *        pointer to place to return error
 *     cb (input)
 *        size of place to return error
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdLastError( PWFELASTERROR pLastError, UINT cb )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDQUERYINFORMATION WdQueryInfo;
   PDLASTERROR PdLastError;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdLastError: Error wd not loaded" ));
       goto done;
   }
   if ( gSdError == TRUE ) {
       memcpy( &PdLastError, &gSdLastError, sizeof(gSdLastError) );
       gSdError = FALSE;
   }
   else {
   /*
    * Get last error
    */
       memset( &WdQueryInfo, 0, sizeof(WdQueryInfo) );
       WdQueryInfo.pWdInformation      = &PdLastError;
       WdQueryInfo.WdInformationLength = sizeof(PdLastError);
       WdQueryInfo.WdInformationClass  = WdLastError;
       if ( rc = ModuleCall( &gWdLink, WD__QUERYINFORMATION, &WdQueryInfo ) ) {
           TRACE(( TC_WENG, TT_ERROR,
              "WENG: wdLastError: Error (%d) from WdQueryInfo-WdLastError", rc ));
           ASSERT( 0, 0 );
           goto done;
       }
    }
   /*
    * Copy PDLASTERROR fields to WFELASTERROR fields.
    */
   strcpy(pLastError->DriverName, PdLastError.ProtocolName);
   pLastError->Error = PdLastError.Error;
   strcpy(pLastError->Message, PdLastError.Message);

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdLastError: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdLoadBalance
 *
 *  Purpose: Load balance
 *
 *  Entry:
 *     pLoadBalance (output)
 *        pointer to place to return load balance info
 *     cb (input)
 *        size of place to return load balance info
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdLoadBalance( PLOADBALANCE pLoadBalance, UINT cb )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDQUERYINFORMATION WdQueryInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdLoadBalance: Error wd not loaded" ));
       goto done;
   }

   /*
    * Get last error
    */
   memset( &WdQueryInfo, 0, sizeof(WdQueryInfo) );
   WdQueryInfo.pWdInformation      = pLoadBalance;
   WdQueryInfo.WdInformationLength = cb;
   WdQueryInfo.WdInformationClass  = WdLoadBalance;
   if ( rc = ModuleCall( &gWdLink, WD__QUERYINFORMATION, &WdQueryInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdLoadBalance: Error (%d) from WdQueryInfo-WdLoadBalance", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdLoadBalance: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: vdThinwireCache
 *
 *  Purpose: Load balance
 *
 *  Entry:
 *     pCache (output)
 *        pointer to place to return cache info
 *     cb (input)
 *        size of place to return cache info
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT vdThinwireCache( PVDTWCACHE pCache, UINT cb )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   VDQUERYINFORMATION VdQueryInfo;

   /*
    * Make sure there is a VD out there
    */
   if ( !( gState & WFES_LOADEDVD ) ) {
       rc = CLIENT_ERROR_VD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: vdThinwireCache: Error vd not loaded" ));
       goto done;
   }

   /*
    * Get last error
    */
   memset( &VdQueryInfo, 0, sizeof(VdQueryInfo) );
   VdQueryInfo.pVdInformation      = pCache;
   VdQueryInfo.VdInformationLength = cb;
   VdQueryInfo.VdInformationClass  = VdThinWireCache;
   if ( rc = ModuleCall( gpaVdLink[Virtual_ThinWire], VD__QUERYINFORMATION, &VdQueryInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: vdThinwireCache: Error (%d) from VdQueryInfo-VdThinWireCache", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: vdThinwireCache: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdInitWindow
 *
 *  Purpose: Initialize the display window
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdInitWindow( HWND hWnd )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure we didn't already do this
    */
   if ( gState & WFES_WINDOW ) {
       rc = CLIENT_ERROR_ALREADY_CONNECTED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdInitWindow: Error already initialized" ));
       goto done;
   }

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdInitWindow: Error wd not loaded" ));
       goto done;
   }

   /*
    * Initialize the window
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdInitWindow;
   WdSetInfo.pWdInformation      = (LPVOID)hWnd;
   WdSetInfo.WdInformationLength = sizeof(hWnd);
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdInitWindow: Error (%d) from WdSetInfo-WdInitWindow", rc ));
       ASSERT( 0, 0 );
       goto done;
   }
   gState |= WFES_WINDOW;

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdInitWindow: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdDestroyWindow
 *
 *  Purpose: Terminate the display window
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdDestroyWindow( HWND hWnd )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure the window was initialized
    */
   if ( !( gState & WFES_WINDOW ) ) {
       rc = CLIENT_ERROR_NOT_CONNECTED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdDestroyWindow: Error window not initialized" ));
       goto done;
   }

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdDestroyWindow: Error wd not loaded" ));
       goto done;
   }

   /*
    * Initialize the window
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdDestroyWindow;
   WdSetInfo.pWdInformation      =(LPVOID)hWnd;
   WdSetInfo.WdInformationLength = sizeof(hWnd);
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdDestroyWindow: Error (%d) from WdSetInfo-WdDestroyWindow", rc ));
       ASSERT( 0, 0 );
       goto done;
   }
   gState &= ~WFES_WINDOW;

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdDestroyWindow: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdPaint
 *
 *  Purpose: Redraw the display
 *
 *  Entry:
 *     hWnd (input)
 *        window handle
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdPaint( HWND hWnd )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdPaint: Error wd not loaded" ));
       goto done;
   }

   /*
    * Update the window
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdPaint;
   WdSetInfo.pWdInformation      = (LPVOID)hWnd;
   WdSetInfo.WdInformationLength = sizeof(hWnd);
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdPaint: Error (%d) from WdSetInfo-WdPaint", rc ));
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdPaint: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdThinwireStack
 *
 *  Purpose: Tell wd to give the thinwire stack to the thinwire vd
 *
 *  Entry:
 *     pTWStack (input)
 *        pointer to thinwire stack description
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdThinwireStack( PVDTWSTACK pTWStack  )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDSETINFORMATION WdSetInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdVdAddress: Error wd not loaded" ));
       goto done;
   }

   /*
    * Tell the wd
    */
   memset( &WdSetInfo, 0, sizeof( WdSetInfo ) );
   WdSetInfo.WdInformationClass  = WdThinwireStack;
   WdSetInfo.pWdInformation      = pTWStack;
   WdSetInfo.WdInformationLength = sizeof(pTWStack);
   if ( rc = ModuleCall( &gWdLink, WD__SETINFORMATION, &WdSetInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdThinwireStack: Error (%d) from WdSetInfo-WdThinwireStack", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdThinwireStack: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: wdEncryptionInit
 *
 *  Purpose: Retrieve encryption initialization information
 *
 *  Entry:
 *     pInitLevel (output)
 *        pointer to place to return
 *     cb (input)
 *        size of place to return
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT wdEncryptionInit( PENCRYPTIONINIT pEncryptionInit, UINT cb )
{
   INT rc = CLIENT_STATUS_SUCCESS;
   WDQUERYINFORMATION WdQueryInfo;

   /*
    * Make sure there is a WD out there
    */
   if ( !( gState & WFES_LOADEDWD ) ) {
       rc = CLIENT_ERROR_WD_NOT_LOADED;
       TRACE(( TC_WENG, TT_ERROR,
               "WENG: wdEncryptionInit: Error wd not loaded" ));
       goto done;
   }

   /*
    * Get Encryption Information
    */
   memset( &WdQueryInfo, 0, sizeof(WdQueryInfo) );
   WdQueryInfo.pWdInformation      = pEncryptionInit;
   WdQueryInfo.WdInformationLength = cb;
   WdQueryInfo.WdInformationClass  = WdEncryptionInit;
   if ( rc = ModuleCall( &gWdLink, WD__QUERYINFORMATION, &WdQueryInfo ) ) {
       TRACE(( TC_WENG, TT_ERROR,
          "WENG: wdEncryptionInit: Error (%d) from WdQueryInfo-WdEncryptionInit", rc ));
       ASSERT( 0, 0 );
       goto done;
   }

done:
   TRACE(( TC_WENG, TT_L1, "WENG: wdEncryptionInit: rc(%d)", rc ));
   return( rc );
}

/*******************************************************************************
 *
 *  Function: vdInfo
 *
 *  Purpose: Set/query virtual driver information
 *
 *  Entry:
 *     pVdInfo (input or output)
 *        pointer to vd info
 *     fSet (input)
 *        TRUE - set info, FALSE - query info
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/
INT vdInfo( PWFEVDINFO pVdInfo, BOOL fSet )
{
    INT              rc;
    VDSETINFORMATION Info;
    PDLLPROCEDURE    pProcedure;
    USHORT           ProcIndex;
    PDLLLINK         pLink;

    Info.VdInformationClass  = (VDINFOCLASS)pVdInfo->type;
    Info.pVdInformation      = pVdInfo->pData;
    Info.VdInformationLength = pVdInfo->cbData;

    pLink     = gpaVdLink[(USHORT)(int)pVdInfo->hVd];
    ProcIndex = fSet ? VD__SETINFORMATION : VD__QUERYINFORMATION;

    if ( !pLink ) {
        TRACE(( TC_WENG, TT_ERROR, "WFEngx.Exe  vdInfo: VD channel (%u) not found",
                pVdInfo->hVd ));
        rc = CLIENT_ERROR_VD_NOT_FOUND;
        goto done;
    }

    if ( ProcIndex >= pLink->ProcCount ) {
        TRACE(( TC_WENG, TT_ERROR,
                "WFEngx.Exe  vdInfo: Procedure not found in VD (channel %u)",
                pVdInfo->hVd ));
        rc = CLIENT_ERROR_BAD_PROCINDEX;
        goto done;
    }

    pProcedure = ((PDLLPROCEDURE *) pLink->pProcedures)[ ProcIndex ];
    if ( !pProcedure ) {
        TRACE(( TC_WENG, TT_ERROR,
                "WFEngx.Exe  vdInfo: Procedure not found in VD (channel %u)",
                pVdInfo->hVd ));
        rc = CLIENT_ERROR_VD_NOT_FOUND;
        goto done;
    }

    rc = (*pProcedure)( pLink->pData, &Info );

done:
    return( rc );
}

/*
 * WdGetInfoForId - local worker routine
 *
 * CLIENT_ERROR_NO_MEMORY        - can't allocate memory
 * CLIENT_ERROR_BUFFER_TOO_SMALL - Buffer length too small
 * CLIENT_ERROR                  - Id not found
 * CLIENT_STATUS_SUCCESS         - Id found 
 *
 */
INT WdGetInfoForId(LPBYTE Id, LPSTR pData, USHORT Length, USHORT *pReturnLength)
{
    int rc=CLIENT_STATUS_SUCCESS;
    WDQUERYINFORMATION WdQueryInfo;
    BYTE *pBuffer = NULL;

    memset( &WdQueryInfo, 0, sizeof(WdQueryInfo) );
    pBuffer = malloc( sizeof(INFODATA) + Length );
    if ( !pBuffer ) {
        rc = CLIENT_ERROR_NO_MEMORY;
        goto done;
    }
    memset( pData, 0, Length );
    memset( pBuffer, 0, sizeof(INFODATA) + Length );
    memcpy( ((PINFODATA)pBuffer)->Id, Id, INFODATA_ID_SIZE );
    WdQueryInfo.pWdInformation      = pBuffer;
    WdQueryInfo.WdInformationLength = sizeof(INFODATA) + Length;
    WdQueryInfo.WdInformationClass  = WdGetInfoData;
    if ( rc = ModuleCall( &gWdLink, WD__QUERYINFORMATION, &WdQueryInfo ) ) {
        TRACE(( TC_WENG, TT_ERROR,
            "WENG: GetInfoData: Error (%d) from WdQueryInfo-WdGetInfoData", rc ));
        *pReturnLength = WdQueryInfo.WdReturnLength - sizeof(INFODATA);
        goto done;
    }

    *pReturnLength = WdQueryInfo.WdReturnLength - sizeof(INFODATA);

    if ( WdQueryInfo.WdReturnLength > sizeof(INFODATA) ) {
        memcpy( pData, pBuffer+sizeof(INFODATA), *pReturnLength );
    }

done:

    if ( pBuffer )
        free(pBuffer);

    return(rc);
}

/*******************************************************************************
 *
 *  Function: wdGetClientDataServer
 *
 *  Purpose: Get client data Server element
 *
 *  Entry:
 *     pData (output)
 *        pointer to buffer that will receive data
 *     Length (input)
 *        length of buffer
 *
 *  Exit:
 *     0 if success, or error code
 *      On error, buffer will be nul terminated.
 *
 ******************************************************************************/
INT wdGetClientDataServer(LPSTR pData, USHORT Length)
{
    USHORT ReturnLength;
    INT rc;
    
    rc = WdGetInfoForId(CLIENTDATA_SERVER, pData, Length, &ReturnLength);

    if ( rc )
        pData[0] = 0;

    return(rc);
}

/*******************************************************************************
 *
 *  Function: wdGetClientDataDomain
 *
 *  Purpose: Get client data Domain element
 *
 *  Entry:
 *     pData (output)
 *        pointer to buffer that will receive data
 *     Length (input)
 *        length of buffer
 *
 *  Exit:
 *     0 if success, or error code
 *      On error, buffer will be nul terminated.
 *
 ******************************************************************************/
INT wdGetClientDataDomain(LPSTR pData, USHORT Length)
{
    USHORT ReturnLength;
    INT rc;
    
    rc = WdGetInfoForId(CLIENTDATA_DOMAIN, pData, Length, &ReturnLength);

    if ( rc )
        pData[0] = 0;

    return(rc);
}

/*******************************************************************************
 *
 *  Function: wdGetClientDataUsername
 *
 *  Purpose: Get client data Username element
 *
 *  Entry:
 *     pData (output)
 *        pointer to buffer that will receive data
 *     Length (input)
 *        length of buffer
 *
 *  Exit:
 *     0 if success, or error code
 *      On error, buffer will be nul terminated.
 *
 ******************************************************************************/
INT wdGetClientDataUsername(LPSTR pData, USHORT Length)
{
    USHORT ReturnLength;
    INT rc;
    
    rc = WdGetInfoForId(CLIENTDATA_USERNAME, pData, Length, &ReturnLength);

    if ( rc )
        pData[0] = 0;

    return(rc);
}

/*******************************************************************************
 *
 *  Function: sdLoad
 *
 *  Purpose: 
 *
 *  Entry:
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

BOOL 
sdLoad( LPBYTE pScriptFile, LPBYTE pScriptDriver )
{
    int rc;
    SDOPEN SdOpen;
           
    TRACE((TC_WENG, TT_API2, "sdLoad: pScriptFile %s, pScriptDriver %s", 
           pScriptFile, pScriptDriver));
    
   
    /*
     *  Load dll into memory
     */
    rc = ModuleLoad( pScriptDriver, &gSdLink );

    TRACE((TC_WENG, TT_API2, "sdLoad: ModuleLoad %s, rc=%u", pScriptDriver, rc));
    if ( rc != CLIENT_STATUS_SUCCESS ) {
        return( FALSE );
    }

    /*
     *  Open script driver
     */
    memset( &SdOpen, 0, sizeof(SDOPEN) );
    SdOpen.pScriptFile       = pScriptFile;
#ifndef RISCOS
    SdOpen.pClibProcedures   = pClibProcedures;
    SdOpen.pLogProcedures    = pLogProcedures;
    SdOpen.pModuleProcedures = pModuleProcedures;
#endif
    SdOpen.pWdLink           = &gWdLink;
    rc = ModuleCall( &gSdLink, DLL__OPEN, &SdOpen );
    TRACE((TC_WENG, TT_API2, "sdLoad: ModuleCall DLL__OPEN, rc=%u", rc));
    if ( rc != CLIENT_STATUS_SUCCESS ) {
        rc = ModuleUnload( &gSdLink );
        return( FALSE );
    }

    return( TRUE );
}


/*******************************************************************************
 *
 *  Function: sdPoll
 *
 *  Purpose: 
 *
 *  Entry:
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

BOOL 
sdPoll()
{
    int rc;

    /*
     *  Poll the script driver
     */
    rc = ModuleCall( &gSdLink, DLL__POLL, NULL );

    /*
     *  If errors
     */
    if ( rc > SCRIPT_STATUS_COMPLETE ) {

        INT rc2;
     
        /*
         * Get last error
         */
        memset( &gSdLastError, 0, sizeof(gSdLastError) );
        gSdLastError.Error = rc;
        if ( rc2 = ModuleCall( &gSdLink, SD__ERRORLOOKUP, &gSdLastError ) ) {
            TRACE(( TC_WENG, TT_ERROR,
               "WENG: sdPoll: Error (%d) from SdLastError", rc2 ));
            ASSERT( 0, 0 );
            goto done;
        }
     
        /*
         *  Mark global error
         */
        gSdError = TRUE;

        /*
         *  Tell ui
         */
done:
        StatusMsgProc( ghWFE, CLIENT_ERROR_PD_ERROR );
        StopPolling();
        TRACE(( TC_WENG, TT_L1, "WENG: sdPoll: SdLastError: rc(%d)", rc ));
    }

    return( (rc == SCRIPT_STATUS_SUCCESS) );
}


/*******************************************************************************
 *
 *  Function: sdUnload
 *
 *  Purpose: 
 *
 *  Entry:
 *
 *  Exit:
 *     0 if success, or error code
 *
 ******************************************************************************/

VOID 
sdUnload()
{
    int rc;

    /*
     *  Close the script driver
     */
    rc = ModuleCall( &gSdLink, DLL__CLOSE, NULL );
    TRACE((TC_WENG, TT_API2, "sdUnload: ModuleCall DLL__CLOSE, rc=%u", rc));

    /*
     *  Unload the script driver
     */
    rc = ModuleUnload( &gSdLink );
    TRACE((TC_WENG, TT_API2, "sdUnload: ModuleUnload, rc=%u", rc));

    return;
}

