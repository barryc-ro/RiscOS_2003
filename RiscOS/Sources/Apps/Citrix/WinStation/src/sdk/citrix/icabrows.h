/******************************************************************************
*
*  ICABROWS.H
*     This file contains definitions for the ICA Browser
*
*  Copyright Citrix Systems Inc. 1995
*
*  Author: Brad Pedersen (10/9/95)
*
*  $Log$
*  
*     Rev 1.41   21 Apr 1997 16:56:52   TOMA
*  update
*  
*     Rev 1.40   18 Apr 1997 14:35:34   thanhl
*  update
*  
*     Rev 1.39   18 Apr 1997 13:03:42   bradp
*  update
*  
*     Rev 1.38   Mar 25 1997 23:47:42   billm
*  Added extern C for C++
*  
*     Rev 1.37   21 Mar 1997 16:10:36   bradp
*  update
*  
*     Rev 1.36   14 Mar 1997 17:33:46   bradp
*  update
*  
*     Rev 1.35   11 Mar 1997 11:32:20   thanhl
*  Add UNICODE/DBCS support
*  
*     Rev 1.34   06 Mar 1997 23:02:34   chrisc
*  Add client election timer flag
*  
*     Rev 1.33   06 Mar 1997 16:05:28   bradp
*  update
*  
*     Rev 1.32   03 Mar 1997 18:01:30   bradp
*  update
*  
*     Rev 1.31   24 Feb 1997 11:25:28   bradp
*  update
*  
*     Rev 1.30   12 Feb 1997 17:39:02   butchd
*  added #define for invalid published app characters
*  
*     Rev 1.29   04 Feb 1997 18:58:38   bradp
*  update
*  
*     Rev 1.28   03 Feb 1997 15:55:32   bradp
*  update
*  
*     Rev 1.27   31 Jan 1997 17:56:28   bradp
*  update
*  
*     Rev 1.26   19 Oct 1996 10:43:56   bradp
*  update
*  
*     Rev 1.25   18 Oct 1996 15:51:22   jeffm
*  License pooling additions
*  
*     Rev 1.24   11 Oct 1996 16:34:30   bradp
*  update
*  
*     Rev 1.23   08 Aug 1996 15:00:26   bradp
*  update
*  
*     Rev 1.22   07 Aug 1996 09:41:34   bradp
*  update
*  
*
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

//#ragma pack(1)

/*=============================================================================
==   ICA browser commands
==
==   C-client, B-browser, M-master browser, G-gateway browser
=============================================================================*/

#define BR_ELECTION          0x01 // (B->B) request master browser election (broadcast)
#define BR_MASTER_DECLARE    0x02 // (M->B) declare browser is master (broadcast)

#define BR_BROWSER_UPDATE    0x10 // (B->M) master browser update from browser
#define BR_ACK               0x11 // (M->B) - browser ack
#define BR_UPDATE_NOW        0x12 // (C->B) force browsers to update master now (broadcast)
#define BR_DELETE_NOW        0x13 // (C->B) force browsers to delete database (broadcast)

#define BR_GATEWAY_ADD       0x20 // (B->B) add gateway address to master browser
#define BR_GATEWAY_DELETE    0x21 // (B->B) delete gateway address from master browser
#define BR_GATEWAY_UPDATE    0x22 // (G->M) master browser update from gateway

#define BR_REQUEST_MASTER    0x30 // (C->B) request master browser to identify (broadcast)
#define BR_MASTER            0x31 // (M->C) - master browser identify
#define BR_REQUEST_ENUM      0x32 // (C->M) request enumeration of browsers
#define BR_ENUM              0x33 // (M->C) - enumeration of browsers
#define BR_REQUEST_DATA      0x34 // (C->M) request browser data
#define BR_DATA              0x35 // (M->C) - browser data
#define BR_REQUEST_PING      0x36 // (C->B) request echo of received packet
#define BR_PING              0x37 // (B->C) - echo of received packet
#define BR_REQUEST_STATS     0x38 // (C->B) request browser statistics
#define BR_STATS             0x39 // (B->C) - browser statistics
#define BR_ERROR             0x3a // (B->C) error reponse
#define BR_REQUEST_LICENSE   0x3b // (C->M) request license
#define BR_LICENSE           0x3c // (M->C) - license reply


/*=============================================================================
==   Common defines
=============================================================================*/

#define MAX_BR_ADDRESS              20  // maximum length of network address (including family)
#define MAX_BR_FORMATTED_ADDRESS    50  // maximum length of formatted network address (for BrFormatAddress API)
#define MAX_BR_BUFFER               512 // maximum length of browser request buffer
#define MAX_BR_LOADLEVEL          10000 // maximum load level

/*
 * Well known ICA Browser IPX socket
 */
#define CITRIX_BR_IPX_SOCKET 0x85BA  // old winview license sap

/*
 * Well known ICA Browser UDP port
 */
#define CITRIX_BR_UDP_PORT 1604  // Offical IANA assigned ICABROWSER port


/*
 * Well known ICA Browser NETBIOS group name and port
 */
#define ICABROWSER_NAME  "ICABROWSER"
#define ICABROWSER_PORT  60
#define NETBIOS_DEFAULT_NAME    0xffff

/*
 * List of invalid characters for published application names.
 */
#define PUBLISHEDAPP_INVALID_CHARACTERS "\\/:*?\"<>|#."

/*=============================================================================
==   Browser header and address structures
=============================================================================*/

/*
 *  Common address structure
 */
typedef struct _ICA_BR_ADDRESS {
    BYTE Address[MAX_BR_ADDRESS];   // bytes 0,1 family, 2-n address
} ICA_BR_ADDRESS, * PICA_BR_ADDRESS;


/*
 *  Common ICA browser header
 */
typedef struct _ICA_BR_HEADER {
    USHORT ByteCount;        // length of command (including this field)
    BYTE Version;            // command version
    BYTE Command;            // command type
    ULONG Signature;         // header signature (must be BR_SIGNATURE)
    ICA_BR_ADDRESS Address;  // source address
} ICA_BR_HEADER, * PICA_BR_HEADER;

#define BR_SIGNATURE 0xE3A8FD02



/*=============================================================================
==   ICA 3.0 Browser Election Data Structures
=============================================================================*/

/*
 *  BR_ELECTION
 *
 *  Request election of master browser
 *  -- browser -> browser (broadcast)
 *  -- client  -> browser (broadcast)
 */
typedef struct _ICA_BR_ELECTION {
    /* version 1 */
    ICA_BR_HEADER Header;
    USHORT oBrowserName;     // offset of null terminated browser name
    USHORT oCriteria;        // offset to ICA_BR_ELECTION_CRITERIA structure

    /* version 2 */
    USHORT ElectionCause;    // cause of election ELECTION_?

    /* version 3 */
                             // Browser name in UNICODE
} ICA_BR_ELECTION, * PICA_BR_ELECTION;

/*
 *  Cause of election request
 */
#define ELECTION_UNKNOWN           0
#define ELECTION_BROWSER_START     1
#define ELECTION_BROWSER_STOP      2
#define ELECTION_ACK_TIMEOUT       3
#define ELECTION_CLIENT_TIMEOUT    4
#define ELECTION_MULTIPLE_MASTERS  5


/*
 *  Master browser election criteria structure
 */
typedef struct _ICA_BR_ELECTION_CRITERIA {
    USHORT Version;                // version of this structure
    USHORT BrowserVersion;         // browser version number
    USHORT CriteriaFlags;          // election criteria flags (see CRITERIA_?)
    ULONG  BrowserUpTime;          // time browser has been running (minutes)
} ICA_BR_ELECTION_CRITERIA, * PICA_BR_ELECTION_CRITERIA;

/* 
 *  CriteriaFlags
 */
#define CRITERIA_MASTERBROWSER    0x0001 // statically configured as master browser
#define CRITERIA_DOMAINCONTROLLER 0x0002 // running on a NT domain controller


/*
 *  BR_MASTER_DECLARE
 *
 *  Master browser declare
 *  -- master browser -> browser (broadcast)
 */
typedef struct _ICA_BR_MASTER_DECLARE {
    ICA_BR_HEADER Header;
} ICA_BR_MASTER_DECLARE, * PICA_BR_MASTER_DECLARE;


/*=============================================================================
==   ICA 3.0 Browser Update Structures
=============================================================================*/

/*
 *  BR_BROWSER_UPDATE
 *  BR_GATEWAY_UPDATE
 *
 *  Master browser update
 *  -- browser -> master browser
 *  -- gateway -> master browser
 */
typedef struct _ICA_BR_BROWSER_UPDATE {

    /* version 1 */
    ICA_BR_HEADER Header;
    USHORT oBrowserName;    // offset of null terminated browser name
    USHORT oFirstData;      // offset to first ICA_BR_DATA_HEADER structure

    /* version 2 */
    USHORT UpdateFlags;     // update flags (UPDATE_?)

    /* version 3 */
    USHORT Pad1; 
    ULONG  UpdateTime;      // update frequence (msecs)

    /* version 4 */
    ULONG  CountryCode;     // Country code of app server
    USHORT FormatFlags;     // Data format flags (FORMAT_?)
    USHORT CodePage;        // Code page
} ICA_BR_BROWSER_UPDATE, * PICA_BR_BROWSER_UPDATE;

/*
 *  Update flags  (UpdateFlags)
 */
#define UPDATE_DELETE     0x0001  // delete all data from database
#define UPDATE_GATEWAY    0x0002  // data was from gateway

/*
 *  Data format flags (FormatFlags)
 */
#define FORMAT_UNICODE    0x0001  // string data is unicode

/*
 *  Code page (CodePage)
 */
#define PRE_UNICODE_BROWSER_ACP 1252


/*
 *  BR_ACK
 *
 *  ACK for BR_BROWSER_UPDATE
 *  -- master browser -> browser
 */
typedef struct _ICA_BR_ACK {
    /* version 1 */
    ICA_BR_HEADER Header;

    /* version 2 */
                                  // packet sent by UNICODE browser
} ICA_BR_ACK, * PICA_BR_ACK;

/*
 *  BR_UPDATE_NOW
 *
 *  Force browsers to update master now
 *  -- client -> browser  (broadcast)
 */
typedef struct _ICA_BR_UPDATE_NOW {
    ICA_BR_HEADER Header;
} ICA_BR_UPDATE_NOW, * PICA_BR_UPDATE_NOW;

/*
 *  BR_DELETE_NOW
 *
 *  Force browsers to delete database
 *  -- client -> browser  (broadcast)
 */
typedef struct _ICA_BR_DELETE_NOW {
    ICA_BR_HEADER Header;
} ICA_BR_DELETE_NOW, * PICA_BR_DELETE_NOW;



/*=============================================================================
==   ICA 3.0 Browser Gateway Structures
=============================================================================*/

/*
 *  BR_GATEWAY_ADD
 *
 *  Add gateway address to master browser
 *  -- browser -> browser
 */
typedef struct _ICA_BR_GATEWAY_ADD {
    /* version 1 */
    ICA_BR_HEADER Header;
    ICA_BR_ADDRESS MasterAddress;  // master browser address

    /* version 2 */
                                  // packet sent by UNICODE browser
} ICA_BR_GATEWAY_ADD, * PICA_BR_GATEWAY_ADD;

/*
 *  BR_GATEWAY_DELETE
 *
 *  Delete gateway address from master browser
 *  -- browser -> browser
 */
typedef struct _ICA_BR_GATEWAY_DELETE {
    ICA_BR_HEADER Header;
    ICA_BR_ADDRESS MasterAddress;  // master browser address
} ICA_BR_GATEWAY_DELETE, * PICA_BR_GATEWAY_DELETE;


/*=============================================================================
==   ICA 3.0 Browser Data
=============================================================================*/

/*
 *  Browser data - header structure
 */
typedef struct _ICA_BR_DATA_HEADER {

    /* version 1 */
    USHORT Version;            // version of this structure
    USHORT DataType;           // type of browser data (DATATYPE_?)
    USHORT oData;              // offset of browser data (ICA_BR_DATA_?)
    USHORT DataLength;         // length of browser data in bytes
    USHORT oNextData;          // offset to next ICA_BR_DATA_HEADER structure

    /* version 2 */
    USHORT oName;              // offset of null terminated name of data
    USHORT DataFlags;          // data flags (DATAFLAG_?)

} ICA_BR_DATA_HEADER, * PICA_BR_DATA_HEADER;

/*
 *  DateType
 */
#define DATATYPE_NONE         0  // no data
#define DATATYPE_NAME         1  // browser name
#define DATATYPE_ADDRESS      2  // address data
#define DATATYPE_SERVER       3  // server data
#define DATATYPE_RESERVED     4  // reserved (don't use - old license data)
#define DATATYPE_SERIALNUMBER 5  // index of serial numbers
#define DATATYPE_APPLICATION  6  // application data  (application.browser)
#define DATATYPE_GATEWAY      7  // load balance data (browser.address)
#define DATATYPE_LOADDATA     8  // load balance data
#define DATATYPE_LICENSE      9  // license data (browser.serialnumber)
#define DATATYPE_DISCONNECT  10  // disconnect session data (clientname:appname)

/*
 *  DataFlags
 */
#define DATAFLAG_DELETE   0x0001 // delete this data item

/*
 *  Browser data structure (DATATYPE_ADDRESS and DATATYPE_DISCONNECT)
 */
typedef struct _ICA_BR_DATA_ADDRESS {
    USHORT Version;                 // version of this structure
    USHORT AddrFlags;               // address flags (see ADDR_?)
    ICA_BR_ADDRESS Address;         // transport address
} ICA_BR_DATA_ADDRESS, * PICA_BR_DATA_ADDRESS;


/*
 *  Browser data structure (DATATYPE_ADDRESS)
 *  - optional data request parameters (oParams)
 *  - offsets are from the beginning of this data structure
 */
typedef struct _ICA_BR_ADDRESS_PARAMS {
    USHORT Version;      // version of this structure
    USHORT oClientName;  // offset of null terminated client name 
} ICA_BR_ADDRESS_PARAMS, * PICA_BR_ADDRESS_PARAMS;


/*
 *  AddrFlags
 */
#define ADDR_WINSTATION      0x0001 // winstation configured
#define ADDR_FREEWINSTATION  0x0002 // free winstation available
#define ADDR_FREEUSERLICENSE 0x0004 // free user license available
#define ADDR_APPLICATION     0x0008 // application name was specified on query
#define ADDR_DISABLE_ENUM    0x0010 // don't enumerate this server address
#define ADDR_ANONYMOUS       0x0020 // anonymous users configured
#define ADDR_LB_LICENSE      0x0040 // load-balance license enabled
#define ADDR_DOMAIN          0x0080 // server belongs to a domain

/*
 *  Browser data structure (DATATYPE_SERVER)
 */
typedef struct _ICA_BR_DATA_SERVER {
    USHORT Version;             // version of this structure

    /* version 1 */
    USHORT BrowserState;        // browser state (see STATE_?)
    USHORT TotalWinStations;    // total winstations on transport
    USHORT FreeWinStations;     // available winstations on transport
    USHORT TotalUserLicenses;   // total user licenses on server
    USHORT FreeUserLicenses;    // free user licenses on server
    USHORT oAppNames;           // offset of app name multi-string (version 1 only)
    USHORT LoadLevel;           // load on computer (0-10000) 0=idle
                                
    /* version 2 */
    USHORT TotalAnonymousUsers; // total anonymous users on server
    USHORT FreeAnonymousUsers;  // free anonymous users on server

    /* version 3 */
    USHORT CtxVersion;          // citrix server version (ie. 0200)
    USHORT CtxBuildNumber;      // citrix build number
    USHORT MsVersion;           // microsoft server version 
    USHORT MsBuildNumber;       // microsoft build number
} ICA_BR_DATA_SERVER, * PICA_BR_DATA_SERVER;

/*
 *  BrowserState
 */
#define STATE_GATEWAY           0x0001  // gateway browser
#define STATE_MASTER            0x0002  // master browser
#define STATE_ELECTION          0x0004  // election is in progress
#define STATE_CLIENT_ELECTION   0x0008  // client forced election is in progress

/*
 *  Browser data structure (DATATYPE_LICENSE)
 *  - browser.serialnumber
 */
typedef struct _ICA_BR_DATA_LICENSE {
    USHORT Version;              // version of this structure
    USHORT LocalInstalled;
    USHORT PoolInstalled;
    USHORT TotalInUse;
    ULONG  EnabledOptions;
    ULONG  LicenseType;
} ICA_BR_DATA_LICENSE, * PICA_BR_DATA_LICENSE;

/*
 *  Browser data structure (DATATYPE_APPLICATION)
 *  - application.browser
 */
typedef struct _ICA_BR_DATA_APPLICATION {
    USHORT Version;        // version of this structure
    USHORT LoadLevel;      // load on computer (0-10000) 0=idle
    USHORT AppFlags;       // application flags (see APPFLAG_?)
} ICA_BR_DATA_APPLICATION, * PICA_BR_DATA_APPLICATION;

/*
 *  AppFlags
 */
#define APPFLAG_ANONYMOUS        0x0001 // anonymous application


/*
 *  Browser data structure (DATATYPE_GATEWAY)
 *  - browser.address
 *  - exists if gateway addresses are configured in registry
 */
typedef struct _ICA_BR_DATA_GATEWAY {
    USHORT Version;         // version of this structure
} ICA_BR_DATA_GATEWAY, * PICA_BR_DATA_GATEWAY;

/*
 *  Browser data structure (DATATYPE_LOADDATA)
 */
typedef struct _ICA_BR_DATA_LOADDATA {
    USHORT Version;                // version of this structure
    ULONG LoadLevel;               // load on computer (0-10000) 0=idle
    ULONG LoadWinStations;         // winstation load (0-10000)
    ULONG RatioWinStations;
    ULONG LoadUserLicenses;        // user license load (0-10000)
    ULONG RatioUserLicenses;
    ULONG LoadPageFile;            // page file load (0-10000)
    ULONG RatioPageFile;
    ULONG LoadPageFaults;          // page fault load (0-10000)
    ULONG RatioPageFaults;
    ULONG LoadMemory;              // memory load (0-10000)
    ULONG RatioMemory;
    ULONG LoadProcessor;           // process load (0-10000)
    ULONG RatioProcessor;
} ICA_BR_DATA_LOADDATA, * PICA_BR_DATA_LOADDATA;



/*=============================================================================
==   ICA 3.0 Browser Client Request Data Structures
=============================================================================*/

/*
 *  BR_REQUEST_MASTER
 *
 *  Request ICA master browser to identify
 *  -- client -> master browser (broadcast)
 */
typedef struct _ICA_BR_REQUEST_MASTER {
    ICA_BR_HEADER Header;
    USHORT MasterReqFlags;
} ICA_BR_REQUEST_MASTER, * PICA_BR_REQUEST_MASTER;

/*
 *  MasterReqFlags
 */
#define MASTERREQ_BROADCAST 0x0001  // broadcast request


/*
 *  BR_MASTER
 *
 *  ICA browser identify
 *  -- master browser -> client
 */
typedef struct _ICA_BR_MASTER {
    ICA_BR_HEADER Header;
    ICA_BR_ADDRESS Address;
} ICA_BR_MASTER, * PICA_BR_MASTER;



/*
 *  BR_REQUEST_ENUM
 *
 *  Request ICA browser list
 *  -- client -> master browser
 */
typedef struct _ICA_BR_REQUEST_ENUM {
    /* version 1 */
    ICA_BR_HEADER Header;
    USHORT EnumReqFlags;  // enumeration request flags (see ENUMREQ_?)
    USHORT DataType;      // type of data request (DATATYPE_?)

    /* version 2 */
    USHORT FormatFlags;   // Data format flags (FORMAT_?)
    USHORT CodePage;      // Code page
} ICA_BR_REQUEST_ENUM, * PICA_BR_REQUEST_ENUM;

/* 
 *  EnumReqFlags
 */
#define ENUMREQ_WINSTATION   0x0001  // enum only configured winstations
#define ENUMREQ_FREE         0x0002  // enum only free user licenses and free winstation
#define ENUMREQ_APPLICATION  0x0004  // include application names
#define ENUMREQ_ANONYMOUS    0x0008  // enum only servers with anonymous users
#define ENUMREQ_LB_LICENSE   0x0010  // enum only servers with load-balance license
#define ENUMREQ_ONLY_APPS    0x0020  // enum only application names
#define ENUMREQ_DOMAIN       0x0040  // enum only servers that belong to a domain

/*
 *  BR_ENUM
 *
 *  ICA browser list
 *  -- master browser -> client
 */
typedef struct _ICA_BR_ENUM {
    /* version 1 */
    ICA_BR_HEADER Header;
    USHORT Sequence;      // packet sequence number (starts with 0)
    USHORT EnumFlags;     // enumeration flags (see ENUM_?)
    USHORT NameCount;     // number of names
    USHORT oFirstName;    // offset to first null terminated name

    /* version 2 */
    USHORT FormatFlags;   // Data format flags (FORMAT_?)
    USHORT CodePage;      // Code page
} ICA_BR_ENUM, * PICA_BR_ENUM;

/*
 *  EnumFlags defines
 */
#define ENUM_FINAL           0x0001  // final sequence number (final data packet)


/*
 *  BR_REQUEST_DATA
 *
 *  Request ICA browser data
 *  -- client -> master browser
 */
typedef struct _ICA_BR_REQUEST_DATA {
    ICA_BR_HEADER Header;

    /* version 1 */
    USHORT oBrowserName;  // offset of null terminated name
    USHORT DataType;      // type of data request (DATATYPE_?)

    /* version 2 */
    USHORT oParams;       // offset of request parameters (optional)

    /* version 3 */
    USHORT FormatFlags;   // Data format flags (FORMAT_?)
    USHORT CodePage;      // Code page
} ICA_BR_REQUEST_DATA, * PICA_BR_REQUEST_DATA;

/*
 *  BR_DATA
 *
 *  ICA browser data
 *  -- master browser -> client
 */
typedef struct _ICA_BR_DATA {
    /* version 1 */
    ICA_BR_HEADER Header;
    USHORT DataType;       // type of data
    USHORT DataLength;     // length of browser data
    USHORT oData;          // offset to browser data

    /* version 2 */
    USHORT FormatFlags;    // Data format flags (FORMAT_?)
    USHORT CodePage;       // Code page
} ICA_BR_DATA, * PICA_BR_DATA;



/*
 *  BR_REQUEST_PING
 *
 *  Request echo packet
 *  -- client -> browser
 *
 *  BR_PING
 *
 *  Echo packet
 *  -- browser -> client
 */
typedef struct _ICA_BR_PING {
    ICA_BR_HEADER Header;
    USHORT DataLength;
    USHORT oData;
} ICA_BR_PING, * PICA_BR_PING;

/*
 *  Ping data used to verify remote browser address
 *  - pointed to by oData
 */
typedef struct _ICA_BR_PING_DATA {
    USHORT Version;           // version of this structure
    USHORT Command;           // ping command
    ULONG Handle;             // ping handle
} ICA_BR_PING_DATA, * PICA_BR_PING_DATA;

#define PING_ADDRESS 1  // ping command


/*
 *  BR_REQUEST_STATS
 *
 *  Request ICA browser statistics
 *  -- client -> browser
 */
typedef struct _ICA_BR_REQUEST_STATS {
    ICA_BR_HEADER Header;
    USHORT StatsFlags;      // statistic flags (see STATS_?)
} ICA_BR_REQUEST_STATS, * PICA_BR_REQUEST_STATS;

/*
 *  StatsFlags
 */
#define STATS_RESET 0x0001  // reset statistics

/*
 *  Statistics Data
 */
typedef struct _BR_STATISTICS {

    /* version 1 */
    ULONG ReceiveElection;
    ULONG ReceiveMasterDeclare;
    ULONG ReceiveBrowserUpdate;
    ULONG ReceiveAck;
    ULONG ReceiveUpdateNow;
    ULONG ReceiveDeleteNow;
    ULONG ReceiveGatewayAdd;
    ULONG ReceiveGatewayDelete;
    ULONG ReceiveGatewayUpdate;
    ULONG ReceiveRequestMaster;
    ULONG ReceiveRequestEnum;
    ULONG ReceiveRequestData;
    ULONG ReceiveRequestPing;
    ULONG ReceiveRequestStats;
    ULONG SendElection;
    ULONG SendMasterDeclare;
    ULONG SendBrowserUpdate;
    ULONG SendGatewayAdd;
    ULONG SendGatewayDelete;
    ULONG SendGatewayUpdate;
    ULONG SendMaster;
    ULONG SendAck;
    ULONG SendError;
    ULONG BytesRead;
    ULONG BytesWritten;
    ULONG BytesBroadcast;
    ULONG PacketsRead;
    ULONG PacketsWritten;
    ULONG PacketsBroadcast;
    ULONG AckTimeouts;
    ULONG StatisticsResets;
    ULONG ReceivePing;

    /* version 2 */
    ULONG ElectionUnknown;
    ULONG ElectionBrowserStart;
    ULONG ElectionBrowserStop;
    ULONG ElectionAckTimeout;
    ULONG ElectionClientTimeout;
    ULONG ElectionMultipleMasters;

} BR_STATISTICS, * PBR_STATISTICS;

/*
 *  BR_STATS
 *
 *  ICA browser statistics
 *  -- browser -> client
 */
typedef struct _ICA_BR_STATS {
    ICA_BR_HEADER Header;
    BR_STATISTICS Stats;
} ICA_BR_STATS, * PICA_BR_STATS;



/*
 *  BR_ERROR
 *
 *  Error response
 *  -- browser -> client
 */
typedef struct _ICA_BR_ERROR {
    ICA_BR_HEADER Header;
    ULONG Error;
} ICA_BR_ERROR, * PICA_BR_ERROR;

/*
 *  BR_REQUEST_LICENSE
 *
 *  Request license
 *  -- client -> master
 */
typedef struct _ICA_BR_REQUEST_LICENSE {
    ICA_BR_HEADER Header;
    ULONG LicenseType;
    ULONG LicenseFlags;
} ICA_BR_REQUEST_LICENSE, * PICA_BR_REQUEST_LICENSE;

/*
 *  License count data structure
 */
typedef struct _ICA_BR_LICENSE_COUNTS {
    USHORT Version;              // version of this structure
    ULONG PoolInstalled;
    ULONG PoolInUse;
    ULONG LocalInstalled;
    ULONG LocalInUse;
    ULONG TotalInstalled;
    ULONG TotalInUse;
} ICA_BR_LICENSE_COUNTS, * PICA_BR_LICENSE_COUNTS;

/*
 *  BR_LICENSE
 *
 *  license reply
 *  -- master -> client
 */
typedef struct _ICA_BR_LICENSE {
    /* version 1 */
    ICA_BR_HEADER Header;
    USHORT fLicense;

    /* version 2 */
    ICA_BR_LICENSE_COUNTS Counts;
} ICA_BR_LICENSE, * PICA_BR_LICENSE;





/*=============================================================================
==   IPX SAP Data Structures
=============================================================================*/

#define SAP_SOCKET 0x0452

/*
 *  SAP Request structure  (find nearest server)
 */
typedef struct _SAP_REQUEST {
    USHORT QueryType;
    USHORT ServerType;
} SAP_REQUEST, * PSAP_REQUEST;

/*
 *  SAP Response structure
 */
typedef struct _SAP_RESPONSE {
    USHORT ResponseType;
    USHORT ServerType;
    BYTE   ServerName[48];
    BYTE   Network[4];
    BYTE   Node[6];
    USHORT Socket;
    USHORT IntermediateNetworks;
} SAP_RESPONSE, * PSAP_RESPONSE;


/*=============================================================================
==   ICA Browser APIs
=============================================================================*/

typedef LONG (WINAPI * PBRNAMEENUMERATEA)(PVOID,ULONG,ULONG,ULONG,PULONG,LPSTR *);

LONG WINAPI BrNameEnumerateA( PVOID, ULONG, ULONG, ULONG, PULONG, LPSTR * );
LONG WINAPI BrQueryNameDataA( PVOID, ULONG, CHAR *, ULONG, PVOID *, PULONG );
LONG WINAPI BrQueryServerDataA( PVOID, ULONG, CHAR *, ULONG, PVOID *, PULONG );
LONG WINAPI BrQueryMasterBrowserAddressA( ULONG, PICA_BR_ADDRESS );
LONG WINAPI BrNameToAddressA( PVOID, ULONG, CHAR *, CHAR *, PICA_BR_ADDRESS );
LONG WINAPI BrForceElectionA( PVOID, ULONG );
LONG WINAPI BrForceUpdateA( PVOID, ULONG, CHAR * );
LONG WINAPI BrDeleteAllDataA( PVOID, ULONG, CHAR * );
LONG WINAPI BrPingServerA( PVOID, ULONG, CHAR *, ULONG, PULONG );
LONG WINAPI BrStatisticsA( PVOID, ULONG, CHAR *, BOOLEAN, PVOID, ULONG, PULONG );

LONG WINAPI BrSetDefaultAddressA( ULONG, CHAR * );
LONG WINAPI BrSetLanaNumberA( ULONG );
LONG WINAPI BrQueryLanaNumberA( PULONG );
LONG WINAPI BrLogInitA( ULONG );
LONG WINAPI BrFormatAddressA( PICA_BR_ADDRESS, PUCHAR, ULONG );
VOID WINAPI BrMemoryFreeA( PVOID );
LONG WINAPI BrRequestLicenseA( PVOID, ULONG, ULONG, ULONG, PBOOLEAN,
                               PICA_BR_LICENSE_COUNTS );

//#pragma pack()

#ifdef __cplusplus
}
#endif
