
/*************************************************************************
*
*   CFGINIP.H
*
*   Configuration INI library private include file
*
*   Copyright Citrix Systems Inc. 1995
*
*   Author: Butch Davis (5/12/95) [from Kurt Perry's CFGPAPI.H]
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:48:58   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   04 May 1995 20:03:06   butchd
*  update
*
*
*************************************************************************/

/*=============================================================================
==   Structures
=============================================================================*/

//  list
typedef struct _LIST {
    int   bValid;
    PCHAR pElement;
    struct _LIST * pNext;
} LIST, * PLIST;

// GetSection (WIN16) list
typedef struct _ENTRYLIST {
    PCHAR pEntryName;
    PCHAR pEntryString;
    struct _ENTRYLIST * pNext;
} ENTRYLIST, * PENTRYLIST;


/*=============================================================================
==   Macros
=============================================================================*/

#define SECTIONSIZE 1024            // start value for a section

//  Profile Entries
#define VIRTUAL_DRIVER      "VirtualDriver"
#define PROTOCOL_SUPPORT    "ProtocolSupport"
#define MAXDRIVER     128           // maximum driver (DLL) name length

