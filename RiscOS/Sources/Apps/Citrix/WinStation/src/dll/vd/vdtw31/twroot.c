//#define DEEPDEBUG 1

/*******************************************************************************
*
*   TWROOT.C
*
*   Thin Wire Windows - root virtual page
*
*   Copyright (c) Citrix Systems Inc. 1992-1996
*
*   Author: Marc Bloomfield (marcb)
*
*   $Log$
*   Revision 1.1  1998/01/19 19:12:49  smiddle
*   Added loads of new files (the thinwire, modem, script and ne drivers).
*   Discovered I was working around the non-ansi bitfield packing in totally
*   the wrong way. When fixed suddenly the screen starts doing things. Time to
*   check in.
*
*   Version 0.02. Tagged as 'WinStation-0_02'
* 
*  
*     Rev 1.9   04 Aug 1997 19:14:12   kurtp
*  update
*  
*     Rev 1.7   25 Jul 1997 14:16:52   kurtp
*  Add Complex clip case to LVB
*  
*     Rev 1.6   14 Jul 1997 18:21:20   kurtp
*  Add LVB to transparent text ops
*  
*     Rev 1.5   15 Apr 1997 18:16:20   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   21 Mar 1997 16:09:30   bradp
*  update
*  
*     Rev 1.4   08 May 1996 14:50:40   jeffm
*  update
*  
*     Rev 1.3   03 Jan 1996 13:33:16   kurtp
*  update
*  
*******************************************************************************/

#include "windows.h"

/*
 *  Get the standard C includes
 */
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "kernel.h"

#include "../../../inc/client.h"

#include "../../../inc/clib.h"

#include "citrix/ica.h"

#include "../../../inc/wdapi.h"
#include "../../../inc/vdapi.h"
#include "../../../inc/mouapi.h"
#include "../../../inc/logapi.h"


#include "twtype.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"

#include "twwin.h"
#include "twdata.h"


#ifdef DEBUG
static char *buf_name(jmp_buf3 jb)
{
    if (jb == vjmpComplete)
	return "Complete";
    if (jb == vjmpResume)
	return "Resume";
    if (jb == vjmpSuspend)
	return "Suspend";
    return "unknown";
}
#endif

#if 1

#define setjmpNewStack2(jb) setjmp(jb)
#define setjmpSaveStack(jb) setjmp(jb)

#else

#define setjmpNewStack2(jb) \
	    (TRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack2: jb %s", buf_name(jb))), \
	    setjmp(jb))

#define setjmpSaveStack(jb) \
	    (TRACE((TC_TW, TT_TW_PACKET, "setjmpSaveStack: jb %s", buf_name(jb))), \
	    setjmp(jb))
#endif
		
/*=============================================================================
==   Data
=============================================================================*/

#ifdef DEBUG
USHORT cbDebugByteCount=0;
char   cDebugCmd=0;
USHORT cbDebugCmdCount=0;
#endif

typedef void (* NTCMD)( HWND hWnd, HDC hdc );
NTCMD pfnNTCallTable[] = {
                         TWCmdStrokePath,                 // 0x80
                         TWCmdStrokePathStraight,         // 0x81
                         TWCmdTextOutNoClip,              // 0x82
                         TWCmdInit,                       // 0x83
                         TWCmdTextOutRclClip,             // 0x84
                         TWCmdTextOutCmplxClip,           // 0x85
                         TWCmdTextOutRclExtra,            // 0x86
                         TWCmdPalette,                    // 0x87
                         };

NTCMD pfnCallTable[]   = {
                         TWCmdInitializeThinwireClient,   // 0x01
                         TWCmdBitBltSourceROP3NoClip,     // 0x02
                         TWCmdBitBltNoSrcROP3NoClip,      // 0x03
                         TWCmdBitBltScrToScrROP3,         // 0x04
                         TWCmdPointerSetShape,            // 0x05
                         TWCmdBitBltNoSrcROP3CmplxClip,   // 0x06
                         TWCmdSSBSaveBitmap,              // 0x07
                         TWCmdSSBRestoreBitmap,           // 0x08
                         TWCmdBitBltSourceROP3SimpleClip, // 0x09
                         TWCmdBitBltSourceROP3CmplxClip,  // 0x0A
                         };


HDC  vhdc  = NULL;
HWND vhWnd = NULL;



/*=============================================================================
==   External Functions and Data
=============================================================================*/

extern int cbTWPackLen;
extern LPBYTE pTWPackBuf;

// Stuff in tw.c
extern BOOL  fWindowsSynced;
extern BYTE bCmdInProg;

#ifndef DOS
// Stuff in twtext.c
extern LPBYTE vpLVB;
void   MaskLVBToScreen( HDC, BOOL );
#endif

/*=============================================================================
==   Functions Defined
=============================================================================*/
int WFCAPI TWDisplayPacket(PVD pVd, USHORT uChan,
                                  LPBYTE pBuffer, USHORT Length);
static BOOL far NewNTCommand(CHAR Cmd);
static BOOL far ResumeNTCommand(CHAR DataByte);
static void create_stack(void);

static int  far _cdecl setjmpNewStack(jmp_buf3, NTCMD fn);
//static int  far _cdecl setjmpNewStack2(jmp_buf3);
//static int  far _cdecl setjmpSaveStack(jmp_buf3);

/**************************************************************************
*
*  TWDisplayPacket
*
*     Handle a virtual write packet.
*
*  ENTRY:
*     pWin (input)
*        pointer to ICA protocol window
*     pIca (input)
*        pointer to ICA data structure
*     Length (input)
*        length of data
*
*  EXIT:
*     CLIENT_STATUS_SUCCESS - successful
*
*  NOTES:
*     Display packets are complicated in that they contain pieces of
*     commands and/or multiple windows commands.  This function
*     represents a complex interaction with the thinwire code to
*     handle commands which cross packet boundaries.
*
***************************************************************************/
int WFCAPI TWDisplayPacket(PVD pVd, USHORT uChan,
                                  LPBYTE pBuffer, USHORT Length)
{
   UCHAR ch;
   BOOL (far *pfnNewWindowsCommand)(CHAR Cmd);
   BOOL (far *pfnResumeWindowsCommand)(CHAR DataByte);

   TRACE((TC_VD, TT_API2, "win: TWDisplayPacket enter sync=%u len=%u",fWindowsSynced,Length));

   ASSERT((cbTWPackLen == 0), cbTWPackLen);

   /*
    *  Save pointer to work buffer and number of data bytes
    */
   pTWPackBuf  = pBuffer;
   cbTWPackLen = Length;

   /* Check that the stack chunk exists */
   create_stack();
   
   /*
    *  service all commands in packet
    */
   while(cbTWPackLen) {

      // get a data byte if there is one (and there better be, i'll tell you)
      // this kicks off the beginning of a new packet
      // the packet could be the resumption of a previous windows
      // command, or it could start a new command
      // all subsequent data bytes in the packet are retrieved via
      // ctx_get_display_data_byte() until the packet is empty

      ch = *(pTWPackBuf)++;
      cbTWPackLen--;

      TRACE((TC_VD, TT_API4, "win: TWDisplayPacket - fetch first databyte %02x, left %u",
                 ch, cbTWPackLen ));

      // if a command is already in progress send the data
      // on to resume the command
      if(bCmdInProg) {

         TRACE((TC_VD, TT_API4, "win: resuming %x - count %u - data = %x",
                  bCmdInProg, cbDebugByteCount, ch));
	 pfnResumeWindowsCommand = ResumeNTCommand;

	 // Put the data byte back in the packet
	 *(--pTWPackBuf) = ch;
	 cbTWPackLen++;

         // command in progress is waiting for data, go back
         // and finish it up maybe
         if((*pfnResumeWindowsCommand)(ch)) {
            // the resumed command was finished in this packet
            // there can be more windows commands within this packet
            TRACE((TC_VD, TT_API4, "win: End (resumed) command %x - count %u", cDebugCmd, cbDebugByteCount));
            bCmdInProg = 0;
            TRACE(( TC_TW, TT_TW_PACKET, "TWDisplyPacket: ResumeWindowsCommand complete" ));
         }
         else {
            // the resumed command needs more data but ran out of
            // data in the packet - circle back around for another
            // display packet to complete this command.  there
            // could be more data in the comm buffer but the next
            // set of display data should come with a header
            ASSERT((cbTWPackLen == 0), cbTWPackLen);
            TRACE((TC_VD, TT_API4, "win: Out of Data (resumed command %x - count %u)", cDebugCmd, cbDebugByteCount));
            TRACE(( TC_TW, TT_TW_PACKET, "TWDisplyPacket: ResumeWindowsCommand requires more data" ));
         }
      }
      else {
         // set new command in progress
         bCmdInProg = ch;
	 pfnNewWindowsCommand = NewNTCommand;

	 // save the command ID and use the id as a flag
         // no windows command is 0 so this works and is hot for debugging
         ASSERT((bCmdInProg != 0), bCmdInProg);
         if(bCmdInProg == 0) {
            TRACE(( TC_TW, TT_TW_PACKET, "TWDisplayPacket: Jumping to jmperror" ));
            return(CLIENT_ERROR_THINWIRE_OUTOFSYNC);
         }

#ifdef DEBUG
         cbDebugByteCount = 1;
         cbDebugCmdCount++;
         cDebugCmd = bCmdInProg;
         TRACE((TC_VD, TT_API4, "win: New command %x - command number %u", bCmdInProg, cbDebugCmdCount));
#endif

         if((*pfnNewWindowsCommand)(bCmdInProg)) {
            // the new command was finished in this packet
            // there can be more windows commands within this packet
            TRACE((TC_VD, TT_API4, "win: End (new) command %x - count %u", cDebugCmd, cbDebugByteCount));
            TRACE(( TC_TW, TT_TW_PACKET, "TWDisplyPacket: NewWindowsCommand complete" ));
            bCmdInProg = 0;
         }
         else {
            // the new command needs more data but ran out of
            // data in the packet - circle back around for another
            // display packet to complete this command.  there
            // could be more data in the comm buffer but the next
            // set of display data should come with a header
            ASSERT((cbTWPackLen == 0), cbTWPackLen);
            TRACE((TC_VD, TT_API4, "win: Out of Data (new command %x - count %u)", cDebugCmd, cbDebugByteCount));
            TRACE(( TC_TW, TT_TW_PACKET, "TWDisplyPacket: NewWindowsCommand requires more data" ));
         }
      }
   }
   TRACE((TC_VD, TT_API2, "win: TWDisplayPacket exit "));
   return(CLIENT_STATUS_SUCCESS);
}

/****************************************************************************\
 *  NewNTCommand
 *
 *  This is the primary entry point for new NT thinwire packets which use
 *  a C front end instead of an ASM front end.
 *
 *  PARAMETERS:
 *     CHAR Cmd (INPUT) - Thinwire packet command ( >= NTCMD_NT )
 *
 *  RETURN:
 *     TRUE  - packet request is complete.
 *     FALSE - more data from another packet is required to complete the
 *             request.
 *
 *  EFFECTS:
 *     A Thinwire packet service routine is  called to process the specified
 *     command.  Service routines use the following prototype:
 *
 *     PARAMETERS:
 *        none
 *
 *     RETURN:   ( via TWCmdReturn() - a longjmp in disguise )
 *        TRUE  - success
 *        FALSE - error occurred
 *
 *     Note: Processing by the service routine may be suspended (via longjmp)
 *           if the service routine calls GetNextTWCmdBytes when there is
 *           not enough data available in the current thinwire command
 *           packet.  If this occurs, seamless resumption will continue
 *           via another longjmp by the ResumeNTCommand when a packet
 *           becomes available.
 *
 *           The service routine must exit via a call to TWCmdReturn()
 *           instead of via the standard return().  TWCmdReturn() will
 *           longjmp to either NewNTCommand or ResumeNTCommand when the
 *           service routine has finished processing all of the thinwire
 *           command packet data it needs.
 *
\****************************************************************************/
static BOOL far NewNTCommand(CHAR Cmd)
{
    BOOL fComplete = -1;
    static int  cmd;
    int rc; 

    cmd = (int)Cmd & 0x00FF;

// If we get a bogus command on retail version, we're toast whether or not
// we return.
    if (  ((cmd < TWCMD_NT) || (cmd > TWCMD_NT_LAST))
          && ( !cmd || (cmd > TWCMD_LAST))
       ) {
        ASSERT( 0, cmd );
        TRACE(( TC_ALL, 0xFFFF, "NewNTCommand: invalid cmd(%02X)", cmd ));
        TRACEBUF(( TC_TW, TT_TW_PACKET, (char far *)pTWPackBuf, 300 ));
        return( TRUE );
    }


    TRACE(( TC_TW, TT_TW_PACKET, "NewNTCommand: cmd(%02X) cbTWPackLen(%d) pTWPackBuf(%08lX)",
              cmd, cbTWPackLen, pTWPackBuf ));
    TRACEBUF(( TC_TW, TT_TW_PACKET, (char far *)pTWPackBuf, (ULONG)cbTWPackLen ));


    //=======================================================================
    // Set up a "Complete" jmp buffer
    //
    // Since the service routine may return to either this routine or
    // to ResumeNTCommand, we need to set up a complete jump point for
    // both routines.
    //=======================================================================
    rc = TWSetJmpSaveStack( vjmpComplete );

    if (rc)
    {
       fComplete = TRUE;
       TRACE(( TC_TW, TT_TW_PACKET, "NewNTCommand: returning from SaveStack" ));
       return fComplete;
    }

    if (!rc)
    {
       //====================================================================
       // Set up a "Suspend" jmp buffer
       //
       // If the service routine requires more data than is currently
       // available, the GetNextTWCmdBytes routine will jump to this
       // "Suspend" mark
       //
       // ResumeNTCommand will then jump to the GetNextTWCmdBytes routine
       // when more thinwire packet command data is available.
       //====================================================================

	TWSetJmpNewStack( vjmpSuspend, 
			  cmd <= TWCMD_LAST ? pfnCallTable[cmd-1] : pfnNTCallTable[cmd-TWCMD_NT]);

	fComplete = FALSE;
	TRACE(( TC_TW, TT_TW_PACKET, "NewNTCommand: returning from NewStack" ));
    }
    TRACE(( TC_TW, TT_TW_PACKET, "NewNTCommand: returning(%d)", fComplete ));

    return( fComplete );
}

/****************************************************************************\
 *  ResumeNTCommand
 *
 *  This routine is called to supply more thinwire command packet data to
 *  a C thiwire command service routine which had previously been suspended
 *  because it needed more data.
 *
 *  PARAMETERS:
 *     CHAR DataByte (INPUT) - Next byte of data in the command packet
 *
 *  RETURN:
 *     TRUE  - packet request is complete.
 *     FALSE - more data from another packet is required to complete the
 *             request.
 *
 *  EFFECTS:
 *     This routine will return to the GetNextTWCmdBytes function (via longjmp).
 *     The suspended service routine will resume without even being
 *     aware of the interruption.  This routine will exit due to another
 *     request by the GetNextTWCmdBytes function or by thee service routine
 *     completing its processing of the command packet data.
 *
\****************************************************************************/
static BOOL far ResumeNTCommand(CHAR DataByte)
{
    BOOL fComplete = TRUE;

    TRACE(( TC_TW, TT_TW_PACKET, "ResumeNTCommand: DataByte(%02X) cbTWPackLen(%d) pTWPackBuf(%08lX)",
              DataByte, cbTWPackLen, pTWPackBuf ));
    TRACEBUF(( TC_TW, TT_TW_PACKET, (char far *)pTWPackBuf, (ULONG)cbTWPackLen ));

    //=======================================================================
    // Set up a "Complete" jmp buffer
    //
    // When the service routine has finished doing its business, it will
    // jump here, indicating that it does not need any more data
    //=======================================================================
    if ( TWSetJmpSaveStack( vjmpComplete ) ) {
       fComplete = TRUE;
    } else {
       //====================================================================
       // Set up a "Suspend" jmp buffer
       //
       // If the service routine requires more data than is currently
       // available, the GetNextTWCmdBytes routine will jump to this
       // "Suspend" mark
       //
       // ResumeNTCommand will then jump to the
       //====================================================================
       if ( TWSetJmpNewStack2( vjmpSuspend ) ) {
          fComplete = FALSE;
       } else {
          //=================================================================
          // Jump to resume processing in GetNextTWCmdBytes function
          //=================================================================
          TWLongJmpChangeStack( vjmpResume, TRUE );
       }
    }

    TRACE(( TC_TW, TT_TW_PACKET, "ResumeNTCommand: returning(%d)", fComplete ));

    return( fComplete );
}
/****************************************************************************\
 *  GetNextTWCmdBytes
 *
 *  This routine is called from a thinwire command service routine to
 *  retrieve the next 1-n bytes from the current and/or next thinwire
 *  command packet(s).
 *
 *  PARAMETERS:
 *     void *  pData  (OUTPUT) - place to copy the next specified command
 *                               packet bytes
 *     int     cbData (INPUT)  - total number of bytes to be copied
 *
 *  RETURN:
 *     TRUE  - The requested data was retrieved successfully
 *     FALSE - An error occurred retrieving the requested data
 *
\****************************************************************************/
BOOL far GetNextTWCmdBytes( void * pData, int cbData )
{
    int  cbCopy;
#ifdef DEBUG
    LPBYTE pBuf = pData;
    int    cBuf = cbData;
#endif


    TRACE(( TC_TW, TT_TW_PACKET, "GetNextTWCmdBytes: Requested%08lX:(%d) Avail(%d) pTWPackBuf(%08lX) pData(%08lX)",
              (ULONG)&cbData, cbData, cbTWPackLen, pTWPackBuf, pData ));

    while ( cbData ) {
       if ( cbCopy = MIN( cbData, cbTWPackLen ) ) {
          TRACE(( TC_TW, TT_TW_PACKET, "GetNextTWCmdBytes: Copying (%d) bytes", cbCopy ));

          cbData      -= cbCopy;
          cbTWPackLen -= cbCopy;
          while ( cbCopy-- )
	  {
	      char *pDataC = pData;
	      *pDataC++ = *pTWPackBuf++;
	      pData = pDataC;
	  }
       }

       if ( cbData ) {
          //=======================================================================
          // Set up a "Resume" jmp buffer to wait for the next packet
          //
          // The ResumeNTCommand will jump here when more thinwire command
          // packet data is available
          //=======================================================================
          TRACE(( TC_TW, TT_TW_PACKET, "GetNextTWCmdBytes: suspending for more %08lX:data(%d)",
                  (ULONG)&cbData, cbData ));
          if ( !TWSetJmpSaveStack( vjmpResume ) ) {
             //====================================================================
             // Return to NewNTCommand or ResumeNTCommand to wait for
             // more thinwire command packet data to become available
             //====================================================================
             TWLongJmpChangeStack( vjmpSuspend, TRUE );
          }
          TRACE(( TC_TW, TT_TW_PACKET, "GetNextTWCmdBytes: resuming to finish%08lX:(%d)",
                  (ULONG)&cbData, cbData ));
       }
    }

#ifdef DEBUG
    TRACE(( TC_TW, TT_TW_PACKET, "GetNextTWCmdBytes: Return to user pData(%08lX) %d Bytes:",
               (LONG)pBuf, cBuf ));
    TRACEBUF(( TC_TW, TT_TW_PACKET, (char far *)pBuf, cBuf ));
#endif

    TRACE(( TC_TW, TT_TW_PACKET, "GetNextTWCmdBytes: Exiting cbTWPackLen(%d) pTWPackBuf(%08lX)",
               cbTWPackLen, pTWPackBuf ));
    return( TRUE );
}

/* ---------------------------------------------------------------------------------------------------- */

/* On RISCOS, we store the _kernel_stack_chunk at the lowest address part
 * of the redzone. We set the stack limit to point to the top of the
 * redzone. 
 */

#define REDZONESIZE	560
#define BASE_SIZE	(4 * 1024)

typedef struct
{
    void *base;			// base of stack (ie top)
    int size;			// size of useable stack
} stack_t;

#define jbreg_sp	7
#define jbreg_sl	8
#define jbreg_lr	9

// the stack base that will be used for all thinwire processing

static stack_t stack_tw = NULL;

static void create_stack(void)
{
    _kernel_stack_chunk *chunk, *curchunk;
    if (!stack_tw.base)
    {
	int size = BASE_SIZE + REDZONESIZE;

	stack_tw.base = malloc(size);

	stack_tw.base = (char *)stack_tw.base + size;
	stack_tw.size = size - REDZONESIZE;
    
	/* Stack allocated successfully. Copy the stack magic from the
	 * current stack chunk. At the moment, stack_tw.base is the TOP
	 * of the stack, stack_tw.size is the size of the stack, but it
	 * does not include the redzone.
	 */
	curchunk = _kernel_current_stack_chunk();
	chunk = (_kernel_stack_chunk *)((char *)stack_tw.base - 
					(stack_tw.size + REDZONESIZE));

	/* Copy current chunks data and then reset selected fields */
	memcpy((void *)chunk, (void *)curchunk, REDZONESIZE);
	chunk->sc_next = chunk->sc_prev = NULL;
	chunk->sc_size = stack_tw.size + REDZONESIZE/* - sizeof(sys_thread_t)*/;

	/* We never want the root chunk to be deallocated */
	chunk->sc_deallocate = NULL;
    }
}

static NTCMD starter_fn = NULL;

static void starter(void)
{
    starter_fn(vhWnd, vhdc);

    DTRACE((TC_TW, TT_TW_PACKET, "starter: %p **** returned ARRRRGGGG", starter_fn));
}

#define JB_SIZE	sizeof(jmp_buf)

/*
 * Save current position in 'jb' and Switch to alternate stack.
 *
 * Set up a 'Suspend' buffer in NewNTCommand
 */

static int setjmpNewStack(jmp_buf3 jb, NTCMD fn)
{
    int rc;

    DTRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack: jb %s fn %p", buf_name(jb), fn));

    if ((rc = setjmp(jb)) == 0)
    {
	jmp_buf3 newjb;

	DTRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack: jb %s rc %d", buf_name(jb), rc));
	DTRACEBUF((TC_TW, TT_TW_PACKET, jb, JB_SIZE));
	
	memset(newjb, 0, JB_SIZE);
	newjb[jbreg_lr] = (int)starter;
	newjb[jbreg_sp] = (int)stack_tw.base;
	newjb[jbreg_sl] = (int)stack_tw.base - stack_tw.size;

	starter_fn = fn;
	longjmp(newjb, 1);
	TRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack: jb %s **** returned ARRRRGGGG", buf_name(jb)));
    }

    DTRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack: jb %s rc %d", buf_name(jb), rc));
    DTRACEBUF((TC_TW, TT_TW_PACKET, jb, JB_SIZE));

    return rc;
}

#if 0
/*
 * Save current position in 'jb' and Switch to alternate stack.
 *
 * Set up a 'Suspend' buffer in ResumeNTCommand
 */

static int setjmpNewStack2(jmp_buf3 jb)
{
    int rc;
    DTRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack2: jb %s", buf_name(jb)));
    rc = setjmp(jb);
    DTRACE((TC_TW, TT_TW_PACKET, "setjmpNewStack2: jb %s rc %d", buf_name(jb), rc));
    DTRACEBUF((TC_TW, TT_TW_PACKET, jb, JB_SIZE));
    return rc;
}


/*
 *
 * Set up a 'Complete' buffer in NewNTCommand or ResumeNTCommand
 * Set up a 'Resume' buffer GetNextTWCmdBytes
 */
static int setjmpSaveStack(jmp_buf3 jb)
{
    int rc;
    DTRACE((TC_TW, TT_TW_PACKET, "setjmpSaveStack: jb %s", buf_name(jb)));
    rc = setjmp(jb);
    DTRACE((TC_TW, TT_TW_PACKET, "setjmpSaveStack: jb %s rc %d", buf_name(jb), rc));
    DTRACEBUF((TC_TW, TT_TW_PACKET, jb, JB_SIZE));
    return rc;
}
#endif

/*
 * This will jump back to whereever the jump_buf was saved, returning
 * the given return code.
 *
 * TWCommandReturn calls this with 'jmpbufComplete' and a return code
 * of TRUE of FALSE.
 *
 * It is also called internally to
 *  a) jump to suspend buffer from GetNextTWCmdBytes
 *  b) jump to resume buffer from ResumeNTCommand
 *
 */

void longjmpChangeStack(jmp_buf3 jb, int rc)
{
    DTRACE((TC_TW, TT_TW_PACKET, "setjmpChangeStack: jb %s rc %d", buf_name(jb), rc));
    DTRACEBUF((TC_TW, TT_TW_PACKET, jb, JB_SIZE));
    longjmp(jb, rc);
    DTRACE((TC_TW, TT_TW_PACKET, "setjmpChangeStack: jb %s **** returned ARRRRGGGG", buf_name(jb)));
}


/*
  
TWDisplayPacket
	|
	|---------------------------------------|
	|					|
NewNTCommand				ResumeNTCommand

	SaveStack   -> Complete			SaveStack   -> Complete
	NewStack    -> Suspend			NewStack2   -> Suspend
						ChangeStack -> Resume
	|
	|				
process function
	|
	|---------------------------------------|
	|					|
GetNextTWCmdBytes			   ChangeStack -> Complete
	SaveStack   -> Resume
	ChangeStack -> Suspend
	|
	|
	
 */
