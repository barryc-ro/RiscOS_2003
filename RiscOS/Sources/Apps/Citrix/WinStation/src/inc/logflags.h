/******************************************************************************
*
*  LOGFLAGS.H
*
*   Logging and Tracing flag definitions
*
*  Copyright Citrix Systems Inc. 1995
*
*  smiddle
*
*  logflags.h,v
*  Revision 1.1  1998/01/12 11:36:51  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
*  
*     Rev 1.6   15 Apr 1997 18:45:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   06 May 1996 18:39:24   jeffm
*  update
*  
*     Rev 1.4   08 Nov 1995 09:45:22   marcb
*  update
*  
*     Rev 1.3   20 Jul 1995 12:10:44   bradp
*  update
*  
*     Rev 1.2   22 Jun 1995 10:54:34   marcb
*  update
*  
*     Rev 1.1   10 Jun 1995 14:17:20   marcb
*  update
*  
*     Rev 1.0   26 May 1995 17:32:14   butchd
*  Initial revision.
*
*******************************************************************************/
#ifndef __LOGFLAGS_H__
#define __LOGFLAGS_H__

/*=============================================================================
 ==   LOG CLASS defines
 ============================================================================*/

// DEBUG: trace option classes
#define TC_UI       0x00000001
#define TC_WD       0x00000002
#define TC_RELI     0x00000004
#define TC_FRAME    0x00000008
#define TC_MODEM    0x00000010
#define TC_PD       0x00000020
#define TC_TD       0x00000040
#define TC_VD       0x00000080
#define TC_CDM      0x00000100
#define TC_CPM      0x00000200
#define TC_KEY      0x00000400
#define TC_MOU      0x00000800
#define TC_LIB      0x00001000
#define TC_CWIN     0x00002000
#define TC_CLIB     0x00004000
#define TC_TW       0x00008000
#define TC_VIO      0x00010000
#define TC_WENG     0x00020000
#define TC_DOSINI   0x00040000
#define TC_IPC      0x00080000
#define LOG_WLOG    0x00100000     // Send to WFDBG instead of to a file
#define TC_WFSHELL  0x00200000
#define LOG_ASSERT  0x40000000     // Always log this class 
#define LOG_CLASS   0x80000000     // UI: standard event logging class 
#define TC_ALL      0xffffffff



/*=============================================================================
 ==   LOG ENABLE defines
 ============================================================================*/

// DEBUG: generic trace option flags
#define TT_API1         0x00000001          // api level 1
#define TT_API2         0x00000002          // api level 2
#define TT_API3         0x00000004          // api level 3
#define TT_API4         0x00000008          // api level 4
#define TT_CONFIG       0x00000010          // configuration status
#define TT_CONNECT      0x00000020          // connect / disconnect status
#define TT_OFRAME       0x00000040          // outbound frame header
#define TT_IFRAME       0x00000080          // inbound frame header
#define TT_OFRAME2      0x00000100          // outbound frame header (level 2)
#define TT_IFRAME2      0x00000200          // inbound frame header (level 2)
#define TT_OCOOK        0x00000400          // outbound cooked dump
#define TT_ICOOK        0x00000800          // inbound cooked dump
#define TT_ORAW         0x00001000          // outbound raw data dump
#define TT_IRAW         0x00002000          // inbound raw data dump
#define TT_CONFIG2      0x00004000          // configuration status (level 2)
#define TT_MEM          0x00008000          // memory usage
#define TT_L1           0x00010000          // level 1
#define TT_L2           0x00020000          // level 2
#define TT_L3           0x00030000          // level 3
#define TT_L4           0x00040000          // level 4
#define TT_ERROR        0xffffffff          // error condition (Debug ERROR)

// UI: standard event logging flags
#define LOG_CONNECT     0x00000001
#define LOG_ERRORS      0x00000002
#define LOG_TRANSMIT    0x00000004
#define LOG_RECEIVE     0x00000008
#define LOG_KEYBOARD    0x00000010


/*=============================================================================
 ==   LOG FLAG defines
 ============================================================================*/

#define LOG_APPEND       0x00000001
#define LOG_FLUSH        0x00000002
#define LOG_PRINTF       0x00000004
#define LOG_REMOTE       0x00000008

#endif //__LOGFLAGS_H__
