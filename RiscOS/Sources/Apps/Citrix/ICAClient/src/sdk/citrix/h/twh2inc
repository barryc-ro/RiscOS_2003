/******************************Module*Header*******************************\
* Module Name: twh2inc.h
*
* This file contains ThinWire defines shared by both the host and the client
*
* Created: 17-Jul-1994
* Author: Marc Bloomfield
*
* Copyright (c) 1993-4 Citrix Systems, Inc.
*
*  $Log$
*  
*     Rev 1.12   21 Apr 1997 16:57:48   TOMA
*  update
*  
*     Rev 1.11   06 Nov 1996 09:43:08   kurtp
*  source sync
*  
*     Rev 1.10   10 May 1996 14:50:20   jeffm
*  update
*  
*     Rev 1.8   21 Sep 1995 10:39:04   kurtp
*  update
*  
*     Rev 1.7   08 May 1995 10:03:36   marcb
*  update
*  
*     Rev 1.4   23 Aug 1994 18:13:36   marcb
*  update
*  
*     Rev 1.2   05 Aug 1994 09:08:02   marcb
*  update
*  
*     Rev 1.1   25 Jul 1994 18:39:42   thanhl
*  update
*  
*     Rev 1.0   25 Jul 1994 17:34:34   thanhl
*  Initial revision.
*  
\**************************************************************************/


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//the following defines are used to compile beyond the demo level
//driver that we are going to build for techtronics.  This means
//that when we want to do the demo level build for techtronics we
//should remove these defines


//the host demo level build relies on the client returning dynamic capabilities
//that indicate no large or tiny cache and no bezier capability.

//in addition to these defines, the host and client must look for
//a specified name that is not thin00 in order to differentiate between
//the bleeding edge function driver and the demo level driver.

#ifndef DEMO
#define  BEYONDDEMO_COPYBITS  //jeff IF NOT DEFINED THEN BITMAP TO SCREEN COPY CONVERTED TO BITBLT TRICK
#define  BEYONDDEMO_BITBLT    //jeff IF NOT DEFINED THEN SOURCE BITBLT CONVERTED TO BITBLT TRICK
#define  BEYONDDEMO_TEXTOUT   //marc IF NOT DEFINED THEN ALL TEXT CONVERTED TO BITBLT TRICK
#define  BEYONDDEMO_LINES     //marc IF NOT DEFINED THEN ALL LINES CONVERTED TO BITBLT TRICK
#define  BEYONDDEMO_CACHING   //jeff IF NOT DEFINED THEN cache returned from client should be 0 and error trace if not 0
#define  BEYONDDEMO_SAVESCREEN   //thanh IF NOT DEFINED THEN NO SAVESCREENBITMAP
#define  BEYONDDEMO_SVGA      //thanh IF NOT DEFINED THEN HARDCODE TO 640 BY 480
#define  BEYONDDEMO_CMPLXCURVES //marc IF NOT DEFINED THEN error trace if capability turned on
#define  BEYONDDEMO_BRUSHNONSOLID  //jeff IF NOT DEFINED THEN ONLY SOLID COLOR BRUSHES
#define  BEYONDDEMO_NOTRACEPROTOCOL //jeff IF NOT DEFINED THEN TRACE ACTUAL THINWIRE DATASTREAM
#endif



// end of special defines !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// General stuff...
////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------
-- TRACE STUFF
--------------------------------------------------------------------------*/
// For use with TC_TW class
// Client and host common defines go here
#define TT_TW_ERROR             0x0001  // a.k.a. CTXDBG_ERROR                      
#define TT_TW_ENTRY_EXIT        0x0002  // a.k.a. CTXDBG_ENTRYEXIT                 
#define TT_TW_BLT               0x0004  // a.k.a. CTXDBG_BLT                       
#define TT_TW_STROKE            0x0008  // a.k.a. CTXDBG_STROKE                    
#define TT_TW_CACHE             0x0010  // a.k.a. CTXDBG_CACHE                     
#define TT_TW_TEXT              0x0020  // a.k.a. CTXDBG_TEXT                      
#define TT_TW_PROTOCOLDATA      0x0040  // a.k.a. CTXDBG_PROTOCOLDATA, TT_TW_PACKET
#define TT_TW_SSB               0x0080  // a.k.a. CTXDBG_SSB                       
#define TT_TW_PTRMOVE           0x0100  // a.k.a. CTXDBG_PTRMOVE    
#define TT_TW_PALETTE           0x0200  // a.k.a. CTXDBG_PALETTE          
#define TT_TW_DIM               0x0400  // a.k.a. CTXDBG_DIM
#define TT_TW_res5              0x0800 
#define TT_TW_res4              0x1000 
#define TT_TW_res3              0x2000 
#define TT_TW_res2              0x4000 
#define TT_TW_res1              0x8000 
// Host-only stuff goes here (if added to client, move to upper section)
#define TT_TW_CONNECT       0x00010000  // a.k.a. CTXDBG_CONNECT
#define TT_TW_LOWCACHE      0x00020000  // a.k.a. CTXDBG_LOWCACHE
#define TT_TW_CLIPCMPLX     0x00040000  // a.k.a. CTXDBG_CLIPCMPLX
#define TT_TW_RLESTATS      0x00080000  // a.k.a. CTXDBG_RLESTATS
#define TT_TW_res9          0x00100000  
#define TT_TW_res10         0x00200000  
#define TT_TW_res11         0x00400000  
#define TT_TW_PTRSHAPE      0x00800000  // a.k.a. CTXDBG_PTRSHAPE
#define TT_TW_COPY          0x01000000  // a.k.a. CTXDBG_COPY
#define TT_TW_PAINT         0x02000000  // a.k.a. CTXDBG_PAINT 
#define TT_TW_FILL          0x04000000  // a.k.a. CTXDBG_FILL 
#define TT_TW_DITHER        0x08000000  // a.k.a. CTXDBG_DITHER 
#define TT_TW_BRUSH         0x10000000  // a.k.a. CTXDBG_BRUSH 
#define TT_TW_XLATEOBJ      0x20000000  // a.k.a. CTXDBG_XLATEOBJ 
#define TT_TW_BLTTRICK      0x40000000  // a.k.a. CTXDBG_BLTTRICK 
#define TT_TW_res8          0x80000000 

//These are for client compatibility - otherwise they are obsolete
#define TT_TW_PACKET        TT_TW_PROTOCOLDATA


//these are the thinwire function byte defines
#define  INITIALIZE_THINWIRE_CLIENT          0x01
#define  BITBLT_SOURCE_ROP3_NOCLIP           0x02
#define  BITBLT_NOSRC_ROP3_NOCLIP            0x03
#define  BITBLT_SCRTOSCR_ROP3                0x04
#define  POINTER_SETSHAPE                    0x05
#define  BITBLT_NOSRC_ROP3_CMPLXCLIP         0x06
#define  SSB_SAVE_BITMAP                     0x07
#define  SSB_RESTORE_BITMAP                  0x08
#define  BITBLT_SOURCE_ROP3_SIMPLECLIP       0x09
#define  BITBLT_SOURCE_ROP3_CMPLXCLIP        0x0A
#define  TWCMD_LAST                          0x0A

#define TWCMD_NT                 0x80
#define TWCMD_STROKEPATH         TWCMD_NT
#define TWCMD_STROKEPATHSTRAIGHT TWCMD_NT+1
#define TWCMD_TEXTOUT_NOCLIP     TWCMD_NT+2
#define TWCMD_INIT               TWCMD_NT+3
#define TWCMD_TEXTOUT_RCLCLIP    TWCMD_NT+4
#define TWCMD_TEXTOUT_CMPLXCLIP  TWCMD_NT+5
#define TWCMD_TEXTOUT_RCLEXTRA   TWCMD_NT+6
#define TWCMD_PALETTE            TWCMD_NT+7
#define TWCMD_NT_LAST            TWCMD_NT+7

//used to make bitmap and copybits more general but hardcoded into
//the cache manager
#define  LARGE_CACHE_CHUNK_SIZE  2048

#undef _TW_jmp_buf
#undef TW_jmp_buf

typedef struct _TW_jmp_buf {        // keep in sync with tw.asm
#ifdef WIN32
    long twjb_EBP;
    long twjb_EDI;
    long twjb_ESI;
    long twjb_EBX;
    long twjb_ESP;
    long twjb_EIP;
    long twjb_STKHQQ; // End of stack (lowest address)
#else
    int twjb_BP;
    int twjb_DI;
    int twjb_SI;
    int twjb_SP;
    int twjb_IP;
    int twjb_CS;
    int twjb_DS;
    int twjb_SS;
    int twjb_STKHQQ; // End of stack (lowest address)
#endif
} TW_jmp_buf;
#define TW_SETJMP_BUFFER_SIZE sizeof(struct _TW_jmp_buf)

#undef jmp_buf3
typedef  int  jmp_buf3[TW_SETJMP_BUFFER_SIZE];
