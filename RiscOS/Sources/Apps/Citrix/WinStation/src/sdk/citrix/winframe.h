
/***************************************************************************
*
*  WINFRAME.H
*
*  This module contains Citrix WinFrame base APIs, structures, and defines,
*  including references to the base Multi-User NT (Hydrix) include file
*  winsta.h
*
*  Copyright Citrix Systems Inc. 1997
*
*  $Author$ Butch Davis
*
*  $Log$
*  
*     Rev 1.3   26 Aug 1997 21:32:30   scottn
*  Add hydra version defines.
*
*     Rev 1.2   20 Jun 1997 14:42:58   butchd
*  update
*
*     Rev 1.1   19 Jun 1997 17:00:32   butchd
*  update
*
*     Rev 1.0   19 Jun 1997 15:19:24   butchd
*  Initial revision.
*
****************************************************************************/

#ifndef _INC_WINSTCTX
#define _INC_WINSTCTX

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINAPI
#define WINAPI      __stdcall
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#include <citrix\winsta.h>
#include <citrix\ica.h>


/***********
 *  Defines
 ***********/

#define MAX_CLIENTDRIVES         26  // maximum drives for client drive mapping
#define CTX_APP_DELIMITER     (WCHAR)'#'

// these must be changed for each release of Picasso to match the
// hydra release (so if they upgrade hydra, picasso stops working)
#define EXPECTED_HYDRA_MAJOR	4
#define EXPECTED_HYDRA_MINOR	0

// BUGBUG: moved these to MS40 winsta.h - may move back
/*
 *  Registry keys and versions for Citrix Icons
 *  -- if "windows\setup\bom\citrix\lminfs\upgrade.inf" changes increment version
 */
//#define UPGRADE_KEY           TEXT("Software\\Citrix\\UPGRADE")
//#define UPGRADE_ICON_VERSION  TEXT("IconVersion")
//#define CURRENT_ICON_VERSION  2

//#define PSZ_ANONYMOUS         TEXT("Anonymous")

//#define WINFRAME_SOFTKEY_CLASS L"WinFrameSoftkey"
//#define WINFRAME_SOFTKEY_APPLICATION L"wfskey.exe"
// end BUGBUG -----------------------------------------------------------------

/************
 *  Typedefs
 ************/

/*------------------------------------------------*/

typedef struct _APPCONFIGW {

    ULONG fAnonymous : 1;           // Application is ANONYMOUS
    ULONG fInheritClientSize : 1;   // Client specifies window size
    ULONG fInheritClientColors : 1; // Client specifies window colors
    ULONG fHideTitleBar : 1;        // Application title bar is removed

    ULONG WindowScale;              // Percent of client desktop (1-100%)
    ULONG WindowWidth;              // Width of window (in pixels)
    ULONG WindowHeight;             // Height of window (in pixels)
    ULONG WindowColor;              // Window color:
                                    //   0x0001 = 16 color
                                    //   0x0002 = 256 color
                                    //   0x0004 = 64K color (future)
                                    //   0x0008 = 16M color (future)
} APPCONFIGW, * PAPPCONFIGW;

typedef struct _APPCONFIGA {

    ULONG fAnonymous : 1;           // Application is ANONYMOUS
    ULONG fInheritClientSize : 1;   // Client specifies window size
    ULONG fInheritClientColors : 1; // Client specifies window colors
    ULONG fHideTitleBar : 1;        // Application title bar is removed

    ULONG WindowScale;              // Percent of client desktop (1-100%)
    ULONG WindowWidth;              // Width of window (in pixels)
    ULONG WindowHeight;             // Height of window (in pixels)
    ULONG WindowColor;              // Window color:
                                    //   0x0001 = 16 color
                                    //   0x0002 = 256 color
                                    //   0x0004 = 64K color (future)
                                    //   0x0008 = 16M color (future)
} APPCONFIGA, * PAPPCONFIGA;

#ifdef UNICODE
#define APPCONFIG APPCONFIGW
#define PAPPCONFIG PAPPCONFIGW
#else
#define APPCONFIG APPCONFIGA
#define PAPPCONFIG PAPPCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _SRVAPPCONFIGW {

    ULONG fServerNotResponding : 1; // Application server not responding
    ULONG fUpdatePending : 1;       // There is a update pending on this app
    ULONG fAccessDenied : 1;        // Can't write/update this server

    /* Application Server Name */
    WCHAR ServerName[ APPSERVERNAME_LENGTH + 3 ];

    /* Application executable & command line parameters */
    WCHAR InitialProgram[ INITIALPROGRAM_LENGTH + 1 ];

    /* Application working directory */
    WCHAR WorkDirectory[ DIRECTORY_LENGTH + 1 ];

} SRVAPPCONFIGW, * PSRVAPPCONFIGW;

typedef struct _SRVAPPCONFIGA {

    ULONG fServerNotResponding : 1; // Application server not responding
    ULONG fUpdatePending : 1;       // There is a update pending on this app
    ULONG fAccessDenied : 1;        // Can't write/update this server

    /* Application Server Name */
    CHAR ServerName[ APPSERVERNAME_LENGTH + 3 ];

    /* Application executable & command line parameters */
    CHAR InitialProgram[ INITIALPROGRAM_LENGTH + 1 ];

    /* Application working directory */
    CHAR WorkDirectory[ DIRECTORY_LENGTH + 1 ];

} SRVAPPCONFIGA, * PSRVAPPCONFIGA;

#ifdef UNICODE
#define SRVAPPCONFIG SRVAPPCONFIGW
#define PSRVAPPCONFIG PSRVAPPCONFIGW
#else
#define SRVAPPCONFIG SRVAPPCONFIGA
#define PSRVAPPCONFIG PSRVAPPCONFIGA
#endif /* UNICODE */

/*------------------------------------------------*/


/**********************
 *  License defines
 **********************/

#define REG_SERIALNUMBER_LENGTH              25
#define REG_LICENSENUMBER_LENGTH             35

typedef WCHAR REG_SERIALNUMBER[ REG_SERIALNUMBER_LENGTH + 1 ];
typedef WCHAR * PREG_SERIALNUMBER;

typedef WCHAR REG_LICENSENUMBER[ REG_LICENSENUMBER_LENGTH + 1 ];
typedef WCHAR * PREG_LICENSENUMBER;

// BUGBUG: moved this to MS40 winsta.h - may move back
//#define OEM_ID_LENGTH                        3

#define CLIENT_DESCRIPTION_LENGTH            32
#define FEATURE_DESCRIPTION_LENGTH           64


/**********************
 *  License Structures
 **********************/

typedef struct _LICENSE{
    REG_SERIALNUMBER    RegSerialNumber;
    REG_LICENSENUMBER   LicenseNumber;
    WCHAR               OemId[ OEM_ID_LENGTH + 1 ];
    ULONG               ProductSku;
    ULONG               EnablerFlags;
    ULONG               Flags;
    ULONG               LicenseCount;
    ULONG               PoolLicenseCount;
    ULONG               PoolType;
    ULONG               CodeLevel;      // for 1.5, was ClientProductId
    ULONG               BumpLevel;
    ULONG               ProductId;
    ULONG	        SerialNumberSeq;
    ULONG	        SerialNumberCrc;
    ULONG	        InstallationCode;
    ULONG	        LicenseNumberCrc;
    ULONG               InstallTime;
    ULONG		ActivationCode;
    ULONG		fRegistered : 1;
    ULONG               fConversion : 1; // set if converting entries
    WCHAR               ClientDescription[ CLIENT_DESCRIPTION_LENGTH + 1 ];
    WCHAR               FeatureDescription[ FEATURE_DESCRIPTION_LENGTH + 1 ];
} LICENSE, * PLICENSE;

typedef struct _LICENSE_COUNTS {
    ULONG ServerPoolInstalled;
    ULONG ServerPoolInUse;
    ULONG ServerPoolAvailable;
    ULONG ServerLocalInstalled;
    ULONG ServerLocalInUse;
    ULONG ServerLocalAvailable;
    ULONG ServerTotalInstalled;
    ULONG ServerTotalInUse;
    ULONG ServerTotalAvailable;
    ULONG NetworkPoolInstalled;
    ULONG NetworkPoolInUse;
    ULONG NetworkPoolAvailable;
    ULONG NetworkLocalInstalled;
    ULONG NetworkLocalInUse;
    ULONG NetworkLocalAvailable;
    ULONG NetworkTotalInstalled;
    ULONG NetworkTotalInUse;
    ULONG NetworkTotalAvailable;
} LICENSE_COUNTS, * PLICENSE_COUNTS;


typedef BOOLEAN (WINAPI * PWINSTATIONQUERYLICENSE)( HANDLE,
                                                    PLICENSE_COUNTS,
                                                    ULONG );


/***************************************************
 *  Registry APIs for Application Configuration Data
 **************************************************/


typedef LONG (WINAPI * PFNREGAPPCONFIGSTATUSW)( LPWSTR );
typedef PFNREGAPPCONFIGSTATUSW * PPFNREGAPPCONFIGSTATUSW;

#ifdef UNICODE
#define PFNREGAPPCONFIGSTATUS  PFNREGAPPCONFIGSTATUSW
#define PPFNREGAPPCONFIGSTATUS PPFNREGAPPCONFIGSTATUSW
#else
#define PFNREGAPPCONFIGSTATUS
#define PPFNREGAPPCONFIGSTATUS
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegAppConfigSetW( PAPPLICATIONNAMEW       pAppName,
                  PAPPCONFIGW             pAppConfig,
                  ULONG                   AppConfigLength,
                  PSRVAPPCONFIGW          pSrvAppConfig,
                  ULONG                   SrvAppConfigLength,
                  ULONG                   SrvAppConfigCount,
                  LPWSTR                  pUserList,
                  LPWSTR                  pGroupList,
                  LPWSTR                  pLocalGroupList,
                  PPFNREGAPPCONFIGSTATUSW pfnRegAppConfigStatus );
#ifdef UNICODE
#define RegAppConfigSet RegAppConfigSetW
#else
#define RegAppConfigSet
#endif /* UNICODE */

/*------------------------------------------------*/

typedef LONG (WINAPI * PREGAPPCONFIGQUERYW)( PAPPLICATIONNAMEW, WCHAR *,
                                             PAPPCONFIGW, ULONG, PSRVAPPCONFIGW *,
                                             PULONG, PULONG, LPWSTR *, LPWSTR *,
                                             LPWSTR *, PPFNREGAPPCONFIGSTATUSW );
LONG WINAPI
RegAppConfigQueryW( PAPPLICATIONNAMEW       pAppName,
                    WCHAR *                 pServerName,
                    PAPPCONFIGW             pAppConfig,
                    ULONG                   AppConfigLength,
                    PSRVAPPCONFIGW *        ppSrvAppConfig,
                    PULONG                  pSrvAppConfigLength,
                    PULONG                  pSrvAppConfigCount,
                    LPWSTR *                ppUserList,
                    LPWSTR *                ppGroupList,
                    LPWSTR *                ppLocalGroupList,
                    PPFNREGAPPCONFIGSTATUSW pfnRegAppConfigStatus );

#ifdef UNICODE
#define RegAppConfigQuery RegAppConfigQueryW
#else
#define RegAppConfigQuery
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegAppConfigServerQueryW( PAPPLICATIONNAMEW       pAppName,
                          PAPPCONFIGW             pAppConfig,
                          ULONG                   AppConfigLength,
                          PSRVAPPCONFIGW          pSrvAppConfig,
                          ULONG                   SrvAppConfigLength,
                          LPWSTR *                ppUserList,
                          LPWSTR *                ppGroupList,
                          LPWSTR *                ppLocalGroupList );

#ifdef UNICODE
#define RegAppConfigServerQuery RegAppConfigServerQueryW
#else
#define RegAppConfigServerQuery
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegAppConfigDeleteW( PAPPLICATIONNAMEW       pAppName,
                     WCHAR *                 pServerName,
                     PPFNREGAPPCONFIGSTATUSW pfnRegAppConfigStatus );

#ifdef UNICODE
#define RegAppConfigDelete RegAppConfigDeleteW
#else
#define RegAppConfigDelete
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegAppConfigEnumerateW( WCHAR *  pDomainName,
                        WCHAR *  pServerName,
                        BOOLEAN bIgnoreDomainAndServer,
                        LPWSTR * ppBuffer );

#ifdef UNICODE
#define RegAppConfigEnumerate RegAppConfigEnumerateW
#else
#define RegAppConfigEnumerate
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegAppConfigServerEnumerateW( WCHAR *  pDomainName,
                              ULONG    EnumFlags,
                              LPWSTR * ppBuffer );

/* EnumFlags */
#define REGSERVER_ANONYMOUS   0x00000001
#define REGSERVER_DOMAINSONLY 0x00000002

#ifdef UNICODE
#define RegAppConfigServerEnumerate RegAppConfigServerEnumerateW
#else
#define RegAppConfigServerEnumerate
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegAppConfigPendingW( LPWSTR                  pServerName,
                      PPFNREGAPPCONFIGSTATUSW pfnRegAppConfigStatus );

#ifdef UNICODE
#define RegAppConfigPending RegAppConfigPendingW
#else
#define RegAppConfigPending
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegFreeBufferW( LPWSTR pBuffer );

#ifdef UNICODE
#define RegFreeBuffer RegFreeBufferW
#else
#define RegFreeBuffer
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegICABrowserResetW( LPWSTR pServerName );

#ifdef UNICODE
#define RegICABrowserReset RegICABrowserResetW
#else
#define RegICABrowserReset
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegICABrowserGatewayUpdateW( LPWSTR pServerName,
                             int AddressFamily,
                             LPWSTR pUpdateItem,
                             BOOLEAN fDelete );

#ifdef UNICODE
#define RegICABrowserGatewayUpdate RegICABrowserGatewayUpdateW
#else
#define RegICABrowserGatewayUpdate
#endif /* UNICODE */

/*------------------------------------------------*/

typedef struct _LOADBALANCING {

    ULONG dwBalanceWinStations;         // WinStation weighting factor
    ULONG dwBalanceUserLicenses;        // User license weighting factor
    ULONG dwBalanceMaxUserLicenses;     // Maximum user license count
    ULONG dwBalancePageFile;            // Page file weighting factor
    ULONG dwBalanceMinPageFile;         // Minimum page file size
    ULONG dwBalancePageFaults;          // Page faults weighting factor
    ULONG dwBalanceMaxPageFaults;       // Maximum page fault count
    ULONG dwBalanceMemoryLoad;          // Memory load weighting factor
    ULONG dwBalanceProcessorBusy;       // Processor busy weighting factor
    ULONG dwBalanceBias;                // Overall weighting bias

} LOADBALANCING, * PLOADBALANCING;

#define LOADBALANCING_BIAS_MIN              -100
#define LOADBALANCING_BIAS_MAX              100
#define LOADBALANCING_WEIGHTING_MIN         0
#define LOADBALANCING_WEIGHTING_MAX         1000
#define LOADBALANCING_MAXUSERLICENSES_MIN   0
#define LOADBALANCING_MAXUSERLICENSES_MAX   0xffffff
#define LOADBALANCING_MINPAGEFILE_MIN       0
#define LOADBALANCING_MINPAGEFILE_MAX       0xffffff
#define LOADBALANCING_MAXPAGEFAULTS_MIN     0
#define LOADBALANCING_MAXPAGEFAULTS_MAX     0xffffff

/*------------------------------------------------*/

LONG WINAPI
RegICABrowserLoadBalancingSetW( LPWSTR pServerName,
                                PLOADBALANCING pLoadBalancing,
                                ULONG LoadBalancingLength );

#ifdef UNICODE
#define RegICABrowserLoadBalancingSet RegICABrowserLoadBalancingSetW
#else
#define RegICABrowserLoadBalancingSet
#endif /* UNICODE */

/*------------------------------------------------*/

LONG WINAPI
RegICABrowserLoadBalancingQueryW( LPWSTR pServerName,
                                  PLOADBALANCING pLoadBalancing,
                                  ULONG LoadBalancingLength );

#ifdef UNICODE
#define RegICABrowserLoadBalancingQuery RegICABrowserLoadBalancingQueryW
#else
#define RegICABrowserLoadBalancingQuery
#endif /* UNICODE */

/*------------------------------------------------*/

VOID WINAPI
RegAppCreateAnonymousUserList( VOID );

/*------------------------------------------------*/

typedef VOID (WINAPI * PREGAPPQUERYANONYMOUSUSERCOUNTS)( PULONG, PULONG );

VOID WINAPI
RegAppQueryAnonymousUserCounts( PULONG pAnonymousUsersAvailable,
                                PULONG pAnonymousUsersTotal );

/*------------------------------------------------*/

typedef BOOLEAN (WINAPI * PREGQUERYENABLER)(PULONG);

BOOLEAN WINAPI
RegQueryEnabler(
    PULONG Mask
    );

/*------------------------------------------------*/

BOOLEAN WINAPI RegGetVersionEx( PVOID /* LPOSVERSIONINFO */ );


/*------------------------------------------------*/

/*
 * License Enabler Bits
 *
 * The License Enabler Bits are used to enable and disable
 * functionality.  Hence, each product (Access, Enterprise, etc.)
 * has its own unique set of enabler bits which establishes
 * each product's capabilities.
 *
 * Special Note and History Lesson
 * WinFrame revision 1.5 used bit 3 (0x8) as a Modem bit, and 1.6
 * left it set, but undefined, and an unreleased product redefined bit 3 to
 * enable load balancing.  In order to remove all the confusion and
 * to reduce the complication of the licensing code, bit 3 is undefined
 * and cannot be used if the older license scheme are to be supported
 * without additional code to adjust the enabler flags based on
 * license scheme format.
 */
#define LICENSE_ENABLER_NET_SHARE        0x00000001
#define LICENSE_ENABLER_RAS_SERVER       0x00000002
#define LICENSE_ENABLER_SFM              0x00000004
// #define LICENSE_ENABLER_not_used      0x00000008   // see explanation above
#define LICENSE_ENABLER_DOMAIN           0x00000010
#define LICENSE_ENABLER_CLIENTLICENSE    0x00000020
#define LICENSE_ENABLER_NO_LAN_ICA       0x00000040
#define LICENSE_ENABLER_NFR              0x00000080   // Not for Resale
#define LICENSE_ENABLER_MAC_SERVER       0x00000100
#define LICENSE_ENABLER_PREACTIVATED     0x00000200   // preactivated license
#define LICENSE_ENABLER_NO_POOL          0x00000400   // not a pool contributor
#define LICENSE_ENABLER_PILOT            0x00000800
#define LICENSE_ENABLER_INTERNET_SERVER  0x00001000
#define LICENSE_ENABLER_LOADBALANCE      0x00002000
#define LICENSE_ENABLER_NOCLILICENSE     0x00004000   // turn off CLIENTLICENSE

/*------------------------------------------------*/



#ifdef __cplusplus
}
#endif

#endif  /* !_INC_WINSTCTX */
