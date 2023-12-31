/*******************************************************************************
*
*   LOGAPI.C
*
*   Log and event logging functions
*
*
*   Copyright Citrix Systems Inc. 1990
*
*   Author:   Brad Pedersen
*
*   $Log$
*  
*     Rev 1.30   25 Feb 1998 16:46:28   TOMA
*  CE Merge
*  
*     Rev 1.35   28 Oct 1997 16:21:30   TOMA
*  moved wcecalls.h
*
*     Rev 1.34   26 Sep 1997 16:31:26   TOMA
*  update
*
*     Rev 1.32   Sep 17 1997 17:32:06   anthonyu
*  Added ce include.
*
*     Rev 1.31   19 Aug 1997 15:00:12   TOMA
*  update
*
*     Rev 1.30   12 Aug 1997 14:25:34   TOMA
*  udpate
*
*     Rev 1.29   15 Apr 1997 18:52:52   TOMA
*  autoput for remove source 4/12/97
*
*     Rev 1.28   27 Jan 1996 19:23:00   bradp
*  update
*
*     Rev 1.27   24 Jul 1995 17:55:12   richa
*
*     Rev 1.26   20 Jul 1995 15:24:02   kurtp
*  update
*
*     Rev 1.25   12 Jul 1995 09:59:18   bradp
*  update
*
*     Rev 1.24   30 Jun 1995 14:02:58   kurtp
*  update
*
*     Rev 1.23   22 Jun 1995 14:51:58   kurtp
*  update
*
*     Rev 1.22   10 Jun 1995 14:10:28   marcb
*  update
*
*     Rev 1.20   29 May 1995 09:28:22   richa
*
*     Rev 1.19   03 May 1995 17:27:24   kurtp
*  update
*
*     Rev 1.18   03 May 1995 16:15:56   kurtp
*  update
*
*     Rev 1.16   02 May 1995 14:06:12   butchd
*  update
*
******************************************************************************/

#include "windows.h"
#include "fileio.h"
#include "mem.h"

/*  Get the standard C includes */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*  Get CLIB includes */
#include "../../inc/client.h"
#include "../../inc/clib.h"
#include "../../inc/logapi.h"
#ifdef WINCE
#include <wcecalls.h>
#endif

#include "swis.h"

#if defined(REMOTE_DEBUG) && defined(DEBUG)
#include "debug/remote.h"
#endif

#include "../../app/version.h"

/*=============================================================================
==   Functions Defined
=============================================================================*/
void logWrite( char far * Format, ... );

#if defined(REMOTE_DEBUG) && defined(DEBUG)
static void rdebug_open(void);
static void LogMem(void);
#endif

/*=============================================================================
==   External Functions used
=============================================================================*/

/*=============================================================================
==   DATA
=============================================================================*/
int ghLogHandle=-1;
ULONG guLogClass=0;
ULONG guLogEnable=0;
ULONG guLogTWEnable=0;
ULONG guLogFlags=0;

PLIBPROCEDURE gpfnLogFunctions[]=
{
 (PLIBPROCEDURE)LogOpen,
 (PLIBPROCEDURE)LogClose,
 (PLIBPROCEDURE)LogPrintf,
 (PLIBPROCEDURE)LogBuffer,
 (PLIBPROCEDURE)LogAssert,
};

#if defined(REMOTE_DEBUG) && defined(DEBUG)
static debug_session *db_sess = NULL;
#endif

/*******************************************************************************
 *
 *  LogLoad
 *
 *    Initialize and return entrypoints.
 *
 * ENTRY:
 *    * pfnLogProcedures
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI LogLoad( PPLIBPROCEDURE pfnLogProcedures )
{

   ghLogHandle=-1;
   guLogClass=0;
   guLogEnable=0;
   guLogTWEnable=0;
   guLogFlags=0;

   // set up entrypoint array to return
   *pfnLogProcedures = (PLIBPROCEDURE)gpfnLogFunctions;

   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  LogOpen
 *
 *    Open log file
 *
 * ENTRY:
 *    pLogOpen - pointer to log options and file name
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *    CLIENT_ERROR_OPEN_FAILED
 *
 * NOTE:
 *    To change log options, call LogOpen again (LogClose not needed).
 *
 ******************************************************************************/

int WFCAPI LogOpen( PLOGOPEN pLogOpen )
{

   // close old log file
   if(ghLogHandle != -1) {
#ifdef WINCE
      _lclose(ghLogHandle);
#else
      close(ghLogHandle);
#endif
      ghLogHandle = -1;
   }

   // set up global data
   guLogClass  = pLogOpen->LogClass;
   guLogEnable = pLogOpen->LogEnable;
   guLogTWEnable = pLogOpen->LogTWEnable;
   guLogFlags  = pLogOpen->LogFlags;

//   if ( guLogClass == 0 || (guLogEnable == 0 && guLogTWEnable == 0))
//       return( CLIENT_STATUS_SUCCESS );

#if defined(REMOTE_DEBUG) && defined(DEBUG)
   rdebug_open();
#endif
   
   // check for nul device
   if ( (pLogOpen->LogFile[0] == '\0') || !stricmp( pLogOpen->LogFile, "nul" ) )
      return( CLIENT_STATUS_SUCCESS );

   // check for device type (ends in ':')
   if ( pLogOpen->LogFile[strlen(pLogOpen->LogFile)-1] == ':' ) {

      pLogOpen->LogFile[strlen(pLogOpen->LogFile)-1] = '\0';
#if defined(DOS) || defined(RISCOS)
      ghLogHandle = open( pLogOpen->LogFile, O_RDWR );
#else
#ifdef WINCE
      ghLogHandle = _lopen( pLogOpen->LogFile, OF_READWRITE );
#else
      ghLogHandle = open( pLogOpen->LogFile, O_RDWR, S_IREAD|S_IWRITE );
#endif
#endif

   } else if ( guLogFlags & LOG_APPEND ) {
#if defined(DOS) || defined(RISCOS)
      ghLogHandle = open( pLogOpen->LogFile, O_CREAT|O_RDWR|O_APPEND );
#else
#ifdef WINCE
      //BUGBUGCE Might have to change this to CreateFile
      //This may truncate the log file when we don't want to
      ghLogHandle = _lcreat( pLogOpen->LogFile, 0 );
#else
      ghLogHandle = open( pLogOpen->LogFile, O_CREAT|O_RDWR|O_APPEND, S_IREAD|S_IWRITE );
#endif
#endif
      if( ghLogHandle != -1 )
#ifdef WINCE
         _llseek(ghLogHandle, 0L, FILE_END);
#else
         lseek(ghLogHandle, 0L, SEEK_END);
#endif

   } else {
#if defined(DOS) || defined(RISCOS)
      ghLogHandle = open( pLogOpen->LogFile, O_CREAT|O_TRUNC|O_RDWR );
#else
#ifdef WINCE
      //BUGBUGCE Might have to change this to CreateFile see lcreat above
      ghLogHandle = _lcreat( pLogOpen->LogFile, 0 );
#else
      ghLogHandle = open( pLogOpen->LogFile, O_CREAT|O_TRUNC|O_RDWR, S_IREAD|S_IWRITE );
#endif
#endif
   }

   if( ghLogHandle == -1 )
      return( CLIENT_ERROR_OPEN_FAILED );

   return( CLIENT_STATUS_SUCCESS );
}


/*******************************************************************************
 *
 *  LogClose
 *
 *    Close log file
 *
 * ENTRY:
 *
 * EXIT:
 *    CLIENT_STATUS_SUCCESS - no error
 *
 ******************************************************************************/

int WFCAPI LogClose( void )
{
   int rc=CLIENT_STATUS_SUCCESS;

   // close old log file
   if(ghLogHandle != -1) {
#ifdef WINCE
      _lclose(ghLogHandle);
#else
      close(ghLogHandle);
#endif
      ghLogHandle = -1;
   }
   return(CLIENT_STATUS_SUCCESS);
}

/*******************************************************************************
 *
 *  LogPrintf
 *
 *  ENTRY:
 *     LogClass (input)
 *        log class mask
 *     LogEnable (input)
 *        log enable mask
 *     pFormat (input)
 *        format string
 *     ...  (input)
 *        enough arguments to satisfy format string
 *
 *  EXIT:
 *     number of bytes
 *
 ******************************************************************************/

void WFCAPI
LogPrintf( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, ... )
{
   va_list arg_marker;
   va_start( arg_marker, pFormat );

   LogVPrintf(LogClass, LogEnable, pFormat, arg_marker);

   va_end( arg_marker);
}

void WFCAPI
LogVPrintf( ULONG LogClass, ULONG LogEnable, PCHAR pFormat, PVOID arg_marker)
{
#define LOCALBUFSIZE 1024
   char Buffer[LOCALBUFSIZE];     // BUGBUG - should make this bullet proof!!!
   char * pBuf = Buffer;          // (ie. Don't try to put more chars in Buffer than
                                  //      sizeof(Buffer) )

   if ( !(LogClass & LOG_ASSERT) )  // Always log asserts
   {
       if (!(LogClass & guLogClass) ||
	   !(LogEnable & (LogClass == TC_TW ? guLogTWEnable : guLogEnable)))
	   return;
   }

   {

#if 0
       int size_left;
       char t[5];

       t[0] = 3;
       _swix(OS_Word, _INR(0,1), 14, t);
       _swix(Territory_ConvertDateAndTime, _INR(0,4) | OUT(2),
	     -1, t, pBuf, LOCALBUFSIZE, "%24:%MI:%SE.%CS ",
	     &size_left);

       pBuf += LOCALBUFSIZE - size_left - 1;
#else
#define MS_PER_SECS   (CLOCKS_PER_SEC)        // 100L
#define MS_PER_MINS   (MS_PER_SECS*60L)       // 100L*60L
#define MS_PER_HOURS  (MS_PER_MINS*60L)       // 100L*60L*60L
#define MS_PER_DAYS   (MS_PER_HOURS*24L)      // 100L*60L*60L*24L

   DWORD    Tc;
   DWORD    Hours;
   DWORD    Mins;
   DWORD    Secs;
   DWORD    Cs;

   Tc = clock();
   //Days  = Tc / MS_PER_DAYS;
   Hours = (Tc %= MS_PER_DAYS) / MS_PER_HOURS ;
   Mins  = (Tc %= MS_PER_HOURS) / MS_PER_MINS ;
   Secs  = (Tc %= MS_PER_MINS) / MS_PER_SECS;
   Cs    = Tc % MS_PER_SECS;
   pBuf += sprintf( pBuf, "%02u:%02u:%02u ", (USHORT) Mins,
                                             (USHORT) Secs,
                                             (USHORT) Cs);
#endif
   }

   pBuf += vsprintf( pBuf, pFormat, arg_marker );

   pBuf += sprintf( pBuf, "\n" );
   *pBuf = '\0';

   if( guLogFlags & LOG_PRINTF )
      printf( "%s", Buffer );

   if ( ghLogHandle != -1 && guLogFlags & LOG_FILE ) {
#ifdef WINCE
      _lwrite( ghLogHandle, Buffer, strlen(Buffer) );
#else
      write( ghLogHandle, Buffer, strlen(Buffer) );
#endif
      if ( guLogFlags & LOG_FLUSH )
#if defined(DOS) || defined(RISCOS)
         flush( ghLogHandle );
#else
#ifdef WINCE
         FlushFileBuffers( (HANDLE)ghLogHandle );
#else
         _commit( ghLogHandle );
#endif
#endif
  }

#if defined(REMOTE_DEBUG) && defined(DEBUG)
   if( guLogFlags & LOG_REMOTE )
   {
       MEMCHECK_PUSH();
       
       debug_print_line(db_sess, Buffer + 9);	// skip over the timestamp

       MEMCHECK_POP();
   }
#endif
}

/*******************************************************************************
 *
 *  LogBuffer
 *
 *  This routine conditional writes a data buffer to the log file
 *
 *  ENTRY:
 *     LogClass (input)
 *        log class bit mask
 *     LogEnable (input)
 *        log type bit mask
 *     pBuffer (input)
 *        pointer to data buffer
 *     ByteCount (input)
 *        length of buffer
 *
 *  EXIT:
 *     nothing
 *
 ****************************************************************************/

void WFCAPI
LogBuffer( ULONG LogClass, ULONG LogEnable,
             LPVOID pBuffer, ULONG ByteCount )
{
   LPBYTE pData;
   ULONG i, j;
   int nl=0x0D|(0x0A<<8);

   if (!(LogClass & guLogClass) ||
       !(LogEnable & (LogClass == TC_TW ? guLogTWEnable : guLogEnable)))
       return;

    /*
     *  Output data
     */
   pData = (LPBYTE) pBuffer;
   for( i=0; i < ByteCount; ) {
      for( j=0; j < 16 && (i+j) < ByteCount; j++ )
         logWrite( "%02X ", pData[j] );
      for( ; j < 16; j++ )
         logWrite( "   " );
      logWrite( "  " );
      for( j=0; j < 16 && (i+j) < ByteCount; j++, pData++ ) {
         if ( *pData < 0x20 || *pData > 0x7f || *pData == '%' )
            logWrite( "." );
         else
            logWrite( "%c", *pData );
      }
      i += 16;

      if( guLogFlags & LOG_PRINTF )
         printf( "\n" );

      if ( ghLogHandle != -1  && guLogFlags & LOG_FILE) {
#ifdef WINCE
         _lwrite(ghLogHandle, "\n", 1 );
#else
         write(ghLogHandle, "\n", 1 );
#endif
         if(guLogFlags & LOG_FLUSH)
#if defined(DOS) || defined(RISCOS)
            flush( ghLogHandle );
#else
#ifdef WINCE
         FlushFileBuffers( (HANDLE)ghLogHandle );
#else
            _commit(ghLogHandle);
#endif
#endif
      }
   }
}
/*******************************************************************************
 *
 *  logWrite
 *
 *  writes a data buffer to the log file
 *
 *  ENTRY:
 *     Format (input)
 *        format string
 *     ...  (input)
 *        enough arguments to satisfy format string
 *
 *  EXIT:
 *     nothing
 *
 ******************************************************************************/

void
logWrite( char far * pFormat, ... )
{
   int bc;
   va_list  arg_marker;
#define LOGBUFFERSIZE 81
   char Buffer[LOGBUFFERSIZE];

   va_start( arg_marker, pFormat );
   bc = vsprintf( Buffer, pFormat, arg_marker );

   if ( ghLogHandle != -1 && guLogFlags & LOG_FILE)
#ifdef WINCE
      _lwrite(ghLogHandle, Buffer, bc);
#else
      write(ghLogHandle, Buffer, bc);
#endif

   if( guLogFlags & LOG_PRINTF )
      printf( "%s", Buffer );
}


/*******************************************************************************
 *
 *  LogAssert
 *
 *  ENTRY:
 *     pExpr (input)
 *        assert expression
 *     pFileName (input)
 *        pointer to file name
 *     LineNumber (input)
 *        Line number of assert in file
 *     rc (input)
 *        error code
 *
 *  EXIT:
 *     nothing
 *
 ******************************************************************************/

void WFCAPI
LogAssert( PCHAR pExpr, PCHAR pFileName, int LineNumber, int rc )
{
   char Buffer[256];

   sprintf( Buffer, "Assert: %s(%u), %s, %u", pFileName, LineNumber, pExpr, rc );

   LogPrintf( LOG_ASSERT, TT_ERROR, "%s", Buffer );
   fprintf(stderr,  "%s\n", Buffer );
}


/*******************************************************************************
 *
 *  WinLog
 *
 *  ENTRY:
 *      pBuffer (input)
 *          buffer to log to win client
 *
 *  EXIT:
 *     nothing
 *
 ******************************************************************************/

void *LogErr( void *err, PCHAR pFileName, int LineNumber )
{
    if (err)
    {
	char Buffer[256];
	_kernel_oserror *e = err;
	
	sprintf( Buffer, "OSERROR: %s(%u), %s, %u", pFileName, LineNumber, e->errmess, e->errnum );

	LogPrintf( LOG_ASSERT, TT_ERROR, "%s", Buffer );
	fprintf(stderr,  "%s\n", Buffer );
    }
    return err;
}

/* ----------------------------------------------------------------------------- */

#if defined(REMOTE_DEBUG) && defined(DEBUG)

#define HELP_INFO_1	\
			"help\n" \
			"dump (flex|heap)\n" \
			"message <message>\n" \
			"set log (class|enable|twenable) x\n" \
			"set log (file|remote) (1|0)\n" \
			"show (da|mem|gdi)\n"

static void cleanup(void)
{
    MEMCHECK_PUSH();

    remote_debug_close((debug_session *)db_sess);

    MEMCHECK_POP();

    db_sess = NULL;
}

static int debug_cmd_handler(int argc, char *argv[], void *handle)
{
    int i, handled = -1;

    if (stricmp(argv[0], "help") == 0)
    {
	debug_print_line(db_sess, HELP_INFO_1);
	handled = 1;
    }
    else if (stricmp(argv[0], "message") == 0)
    {
	handled = 0;
	if (argc > 1)
	{
	    char s[1024];

	    strcpy(s, "*** MESSAGE *** ");
	    for (i = 1; i < argc; i++)
		strcat(s, argv[i]);

	    debug_print_line(db_sess, s);
	    handled = 1;
	}
    }
    else if (stricmp(argv[0], "dump") == 0)
    {
	if (stricmp(argv[1], "flex") == 0)
	{
	    MemFlex_Dump(NULL);
	    handled = 1;
	}
	else if (stricmp(argv[1], "heap") == 0)
	{
	    malloc_stats();
	    handled = 1;
	}
    }
    else if (stricmp(argv[0], "show") == 0)
    {
	if (stricmp(argv[1], "mem") == 0)
	{
	    LogMem();
	    handled = 1;
	}
	else if (stricmp(argv[1], "da") == 0)
	{
	    int area = -1;
	    do
	    {
		_swix(OS_DynamicArea, _INR(0,1) | _OUT(1), 3, area, &area);
		if (area != -1)
		{
		    int size;
		    char *name;
		    _swix(OS_DynamicArea, _INR(0,1) | _OUT(2) | _OUT(8), 2, area, &size, &name);
		    LogPrintf(TC_ALL, TT_ERROR, "da: size %10d name '%s'", size, name);
		}
	    }
	    while (area != -1);
	    handled = 1;
	}
	else if (stricmp(argv[1], "gdi") == 0)
	{
	    dump_objects();
	    handled = 1;
	}
    }
    else if (stricmp(argv[0], "set") == 0)
    {
	handled = 0;
	if (argc > 1)
	{
	    if (stricmp(argv[1], "log") == 0)
	    {
		if (argc > 3)	// type and value
		{
		    handled = 1;

		    if (stricmp(argv[2], "class") == 0)
			guLogClass = strtoul(argv[3], NULL, 16);
		    else if (stricmp(argv[2], "enable") == 0)
			guLogEnable = strtoul(argv[3], NULL, 16);
		    else if (stricmp(argv[2], "twenable") == 0)
			guLogTWEnable = strtoul(argv[3], NULL, 16);

		    else if (stricmp(argv[2], "remote") == 0)
		    {
			if (atoi(argv[3]))
			    guLogFlags |= LOG_REMOTE;
			else
			    guLogFlags &= ~LOG_REMOTE;
		    }
		    else if (stricmp(argv[2], "file") == 0)
		    {
			if (atoi(argv[3]))
			    guLogFlags |= LOG_FILE;
			else
			    guLogFlags &= ~LOG_FILE;
		    }

		    else
			handled = 0;
		}
	    }
	}
    }
    
    return handled;
    handle = handle;
}

static void rdebug_open(void)
{
    if (!db_sess)
    {
	MEMCHECK_PUSH();
	
	remote_debug_open(APP_NAME, &db_sess);
	if (db_sess)
	    remote_debug_register_cmd_handler(db_sess, debug_cmd_handler, NULL);

	MEMCHECK_POP();
	
	atexit(cleanup);
    }
}

void LogPoll(void)
{
    if (db_sess)
	debug_poll(db_sess);
}

static void LogMem(void)
{
    extern char *flexptr__base;
    extern char *flexptr__free;
    extern char *flexptr__slot;
    extern int malloc_size, malloc_da, flex__da;

    int area = -1;
    int us, next, free;

    _swix(Wimp_SlotSize, _INR(0,1) | _OUTR(0,2), -1, -1, &us, &next, &free);
    LogPrintf(TC_ALL, TT_ERROR,
	      "slot: size %dK next %dK free %dK",
	      us/1024, next/1024, free/1024);

    LogPrintf(TC_ALL, TT_ERROR,
	      "flex: area %dK size %dK top %dK",
	      _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), flex__da)/1024,
	      (flexptr__slot - flexptr__base)/1024, (flexptr__free - flexptr__base)/1024);

    LogPrintf(TC_ALL, TT_ERROR,
	      "mall: area %dK size %dK",
	      _swi(OS_ReadDynamicArea, _IN(0) | _RETURN(1), malloc_da)/1024, malloc_size/1024);

    twreportmem();
}

#endif
