/*************************************************************************
*
* WFSHELL.H
*
* WinFrame user shell header file
*
*  Copyright Citrix Systems Inc. 1995
*
*  Author: Marc Bloomfield    16 August 1995
*
* $Log$
*  
*     Rev 1.3   21 Apr 1997 18:00:32   TOMA
*  update
*  
*     Rev 1.2   19 Jan 1996 16:45:34   marcb
*  update
*  
*     Rev 1.1   06 Dec 1995 17:01:26   miked
*  update
*  
*     Rev 1.0   08 Nov 1995 09:35:26   marcb
*  update
*  
*************************************************************************/

#pragma pack(1)

/*******************************************************************************
 *
 * Virtual channel data flow upon connection/reconnection:
 *
 *
 *
 * <Host hooks host clipboard - updates to client disabled>
 *
 * Host (WFShell.Exe) --------- WFCLIP_INIT_REQUEST ---------> Client (VDClip.Dll)
 *
 * Host (WFShell.Exe) <-------- WFCLIP_INIT_REPLY ------------ Client (VDClip.Dll)
 *
 *                                                       <Client hooks client clipboard>
 *
 * Host (WFShell.Exe) <-------- WFCLIP_PLACE ----------------- Client (VDClip.Dll)
 *
 * <Host clipboard updates to client enabled>
 *                                  :
 *                                  :
 *                                  :
 *                                  :
 * <If client clipboard updated>
 * Host (WFShell.Exe) <-------- WFCLIP_PLACE ----------------- Client (VDClip.Dll)
 *                                  :
 *                                  :
 *                                  :
 *                                  :
 * <If host clipboard updated>
 * Host (WFShell.Exe) --------- WFCLIP_PLACE ----------------> Client (VDClip.Dll)
 *                                  :
 *                                  :
 *                                  :
 * <Upon disconnection, host clears delay-rendered client data from clipboard>
 * <Upon disconnection, client clears delay-rendered host data from clipboard>
 *
 ******************************************************************************/

#ifdef WIN32
#define HUGE_PTR *
#else
#define HUGE_PTR _huge *
#endif

/*
 * Data structure used for clipboard virtual channel
 * (Between host user shell WFSHELL.EXE and VDCLIP client virtual driver)
 */

/*
 * WFShell protocol version(s) supported
 */
#define WFSHELL_VERSION             0x0001

/*
 * Every packet sent by either the host or the client begins with a packet header in
 * the format of a WFCLIPHEADER structure. The WFCLIP_INIT_REQUEST packet is the only
 * packet which starts with just the Function element of the WFCLIPHEADER structure.
 *
 * Note: Protocol data elements must be size specific (USHORT or ULONG, not UINT or INT)
 */
typedef struct _WFCLIPHEADER {
    USHORT Function;        // INIT_REQUEST, PLACE, RENDER_REQUEST, RENDER_REPLY, etc.
    USHORT Flags;           // NOTIFICATION_ONLY, MULTIPLE_FORMATS, etc.
} WFCLIPHEADER, * PWFCLIPHEADER;

/*                                                    
 * This is the first packet sent to the client virtual clipboard driver from the host.
 * It is delivered as a WFCLIP_INIT_REQUEST function packet.
 */                                                 
typedef struct _WFCLIPINITREQUEST {                        
    USHORT Function;          //
    USHORT VersionLow;        // Lowest WFShell protocol level supported
    USHORT VersionHigh;       // Hightest WFShell protocol level supported 
} WFCLIPINITREQUEST, * PWFCLIPINITREQUEST;

/*                                                    
 * This is the first packet sent to the host from the client virtual clipboard driver.
 * It is sent in response to a WFCLIP_INIT_REQUEST packet and is tagged as a
 * WFCLIP_INIT_REPLY function packet.
 */                                                 
typedef struct _WFCLIPINITREPLY {                        
    WFCLIPHEADER header;
    USHORT       Version;     // WFShell protocol level
    ULONG        Threshold;   // Data packets bigger than this will use delayed rendering
                              //    0 - always delay render,  -1 - never delay render
    USHORT       CFNotify[1]; // 0-terminated array of clipboard formats to notify client about
                              //   0      - notify client of all clipboard formats
                              // negative - start/end of range
                              // other    - format to notify client about
//   Example clipboard format notification array at offset offCFNotify in this struct:
//
//  USHORT CFNotify[5];       // CFNotify[0] = CF_TEXT;          // Notify of CF_TEXT
//                            // CFNotify[1] = CF_BITMAP         // Notify of CF_BITMAP
//                            // CFNotify[2] = -CF_PRIVATEFIRST; // Notify CF_PRIVATEFIRST
//                            // CFNotify[3] = -CF_PRIVATELAST;  //   through CF_PRIVATELAST
//                            // CFNotify[4] = 0;                // That's all we want
//
//
} WFCLIPINITREPLY, * PWFCLIPINITREPLY;

/*
 * The WFCLIPFORMAT data block is sent as part of the WFCLIPDATA data block.
 * It is variable length depending upon the Flags specified as part of the packet
 * header.
 */
typedef struct _WFCLIPFORMAT {
    USHORT uFormat;         // CF_xxx, 0 indicates end of chain
    ULONG  cbData;          // size of Data
    BYTE   Data[1];         // exists unless WFCLIP_NOTIFICATION_ONLY 
//  If registered format, zero-terminated format name immediate follows cbData if
//  NOTIFICATION_ONLY, otherwise Data + cbData
//
//  If WFCLIP_MULTIPLE_FORMATS, next WFCLIPFORMAT structure immediately follows
//  After the last WFCLIPFORMAT block, a ULONG of 0 indicates the end of the chain
} WFCLIPFORMAT, * PWFCLIPFORMAT;

typedef struct _WFCLIPDATA {
    WFCLIPHEADER header;
    WFCLIPFORMAT format;
} WFCLIPDATA, * PWFCLIPDATA;

/*
 * Functions ( client to host OR host to client ) [set in WFCLIPHEADER structure]
 */
#define WFCLIP_INIT_REQUEST   0x0001  // Initial packet sent from host to client
#define WFCLIP_INIT_REPLY     0x0002  // Client response to WFCLIP_INIT_REQUEST
#define WFCLIP_PLACE          0x0003  // Place this data into the clipboard
#define WFCLIP_RENDER_REQUEST 0x0004  // Send clipboard data across the wire
#define WFCLIP_RENDER_REPLY   0x0005  // This data was retrieved from the source clipboard
#define WFCLIP_CANCEL         0x0006  // Cancel request(s) in progress
#define WFCLIP_PROGRESS       0x0007  // Update progress indicator
#define WFCLIP_LAST           0x0007  // Last supported function

/*
 * Flags [set in WFCLIPHEADER structure]
 */
#define WFCLIP_NORMAL            0x0000
#define WFCLIP_NOTIFICATION_ONLY 0x0001 // Notification only - data not included
#define WFCLIP_MULTIPLE_FORMATS  0x0002 // Multiple formats included 
#define WFCLIP_REQUEST_FAILED    0x0004 // request failed 

#define WFCLIP_MAX_PACKET_SIZE (1024-4) // No single data packet can be greater than this size
#define WFCLIP_MIN_PACKET_SIZE (sizeof( WFCLIPDATA ) - 1)
#define WFCLIP_FORMAT_MIN  (sizeof( WFCLIPFORMAT ) - 1)
#define WFCLIP_DATA_MIN    (WFCLIP_FORMAT_MIN + sizeof( WFCLIPHEADER ))
#define WFCLIP_CB_RENDER_REQUEST (sizeof( WFCLIPHEADER ) + sizeof(ULONG))
// Example of some actual packet sizes:
//     Normal:            WFCLIP_DATA_MIN + cbData
//     NOTIFICATION_ONLY: WFCLIP_DATA_MIN
//     registered format, NOTIFICATION_ONLY: WFCLIP_DATA_MIN + strlen( pszName ) + 1
//     Multiple formats with two normal formats:
//             WFCLIP_DATA_MIN + cbData1 + WFCLIP_FORMAT_MIN + cbData2 + sizeof( ULONG )
//     RENDER_REQUEST:    WFCLIP_CB_RENDER_REQUEST  (header & uFormat only)
//


#ifdef WIN32
#define WFC_GLOBAL (GMEM_DDESHARE | GMEM_MOVEABLE)
#else
#define WFC_GLOBAL GHND
#endif

// Valid formats ( defined in winuser.h )
// CF_TEXT            1                     
// CF_BITMAP          2 
// CF_METAFILEPICT    3 
// CF_SYLK            4 
// CF_DIF             5 
// CF_TIFF            6 
// CF_OEMTEXT         7 
// CF_DIB             8 
// CF_PALETTE         9 
// CF_PENDATA         10
// CF_RIFF            11
// CF_WAVE            12
// CF_UNICODETEXT     13
// CF_ENHMETAFILE     14
                                                     
// CF_OWNERDISPLAY    0x0080  // This probably will never work
// CF_DSPTEXT         0x0081
// CF_DSPBITMAP       0x0082
// CF_DSPMETAFILEPICT 0x0083
// CF_DSPENHMETAFILE  0x008E
                                                    
// CF_PRIVATEFIRST    0x0200  // no guarantee these will work - only if hData is global data
// CF_PRIVATELAST     0x02FF                                                                          

#define WFCF_REGISTEREDFIRST 0xC000
#define WFCF_REGISTEREDLAST  0xFFFF

typedef BOOL (* PFNSENDPACKET)( WFCLIPDATA HUGE_PTR pData, ULONG cbData, BOOL fLast );
typedef struct _PACKETQUEUE {
    BOOL                fInProgress;
    HGLOBAL             hClipData;
    WFCLIPDATA HUGE_PTR pClipData;
    WFCLIPDATA HUGE_PTR pClipDataCurrent;
    ULONG               cbLeft;
} PACKETQUEUE, * PPACKETQUEUE;

#ifdef WIN16
#define MEMCPY hmemcpy
#else
#define MEMCPY memcpy
#endif

BOOL PlaceInClipboard( HWND hWnd, WFCLIPDATA HUGE_PTR pClipData, ULONG Length,
                       BOOL fRender, PPACKETQUEUE ppktQueue );
BOOL ProcessClipboardChange( HWND hWnd, ULONG cbThreshold, UINT uFormatRender,
                             PFNSENDPACKET pfnSendPacket );
BOOL RemoveClipboardData( HWND hWnd );
WFCLIPDATA HUGE_PTR InitQueue( WFCLIPDATA HUGE_PTR pClipData, ULONG Length,
                               PPACKETQUEUE ppktQueue );
BOOL wSendPacket( WFCLIPDATA HUGE_PTR pData, ULONG cbData, BOOL fLast );

#ifdef CITRIX // Host
#undef  ASSERT
#ifdef DEBUG
#define ASSERT(exp,rc) { \
   if (!(exp)) { \
      DbgTrace( "%s, %s, %d, %ld", #exp, __FILE__, __LINE__, (ULONG)rc ); \
      _asm int 3 \
   } \
}
#else
#define ASSERT(exp,rc) { }
#endif
#endif
#pragma pack()

