/* commonsrc/debug.h */

#ifndef __debug_h
#define __debug_h

#ifndef DEVELOPMENT
#error "You should #define DEVELOPMENT to the appropriate level"
#endif

/*****************************************************************************

  Debugging macros are now centralised. Each debugging control has two
  macros, permitting debugging messges to be "commented out" in a more
  controlled fashion. This is primarily controlled through the
  DEVELOPMENT define passed through from the makefile, and
  additionally controlled through environment variables of the same
  name as the debugging macro.

  DEVELOPMENT == 0: All debugging is compiled out
  unconditionally. This is how production code is made.

  DEVELOPMENT == 1: Standard debugging method. XXXDBG is compiled in
  and XXXDBGN is not compiled in. XXXDBG will actually be sent to
  stderr if XXXDBG is defined in the environment (value doesn't
  matter).

  DEVELOPMENT == 2: Verbose debugging method. Both XXXDBG and XXXDBGN
  are compiled in.  XXXDBG will actually be sent to stderr if XXXDBG
  is defined in the environment (value doesn't matter). Likewise
  XXXDBGN macros require the corresponding XXXDBGN environment
  variable to be set for output to be generated.

  DEVELOPMENT	Defined as 1 or 2 during development. Mutex PRODUCTION
  DEBUG		Defined during developent, not for production
  PRODUCTION	Defined as 1 for production. Mutex DEVELOPMENT
  TABDBG	Table debugging
  PRSDBG	Parser debugging
  RENDBG	Render debugging
  DICDBG	Debug Image Change
  CNFDBG	Configuration debugging
  FMTDBG	Formatting debugging
  IMGDBG	IMage debugging
  ACCDBG	Access debugging

  @@@@ We need some more debugging options that I have not yet put in.
  These include:

  ATHDBG	Authentication debugging
  LAYDBG	Frame layout debugging
  PRTDBG	Print debugging
  TRDDBG	Thread debugging
  LOCDBG	Locate debugging

  @@@@ Maybe we should change the use of environment variable so that
  we use FMTDBG is the FMTDBG variable is >=1 and use FMTDBGN is the
  variable is >= 2.

 */

/* DEBUG and PRODUCTION are derived from DEVELOPMENT only */
#ifdef DEBUG
#undef DEBUG
#endif

#ifdef PRODUCTION
#undef PRODUCTION
#endif

#define DBGPROTO(x) extern void x(const char *fmt, ...)

/* From usrtrc.c */

extern void init_usrtrc(void);

/* Set defines based upon DEVELOPMENT */
/* -------------------------------------------------------------------------------- */
#if DEVELOPMENT == 0

/* #undef  DEVELOPMENT */
#define PRODUCTION	1

#define DEBUG 0

#elif DEVELOPMENT == 1

#define DEBUG		1

#else

#define DEBUG		2

#endif
/* -------------------------------------------------------------------------------- */

#if DEBUG
extern void dbginit(void);
#endif

/*****************************************************************************/

/* Basic level debugging functions */

#if DEBUG < 1

#define TABDBG(x)
#define OBJDBG(x)
#define PRSDBG(x)
#define RENDBG(x)
#define DICDBG(x)
#define CNFDBG(x)
#define FMTDBG(x)
#define IMGDBG(x)
#define CKIDBG(x)
#define ACCDBG(x)
#define STBDBG(x)

#else	/* DEBUG < 1 */

#define TABDBG(x)	tabdbg x
#define OBJDBG(x)	objdbg x
#define PRSDBG(x)	prsdbg x
#define RENDBG(x)	rendbg x
#define DICDBG(x)	dicdbg x
#define CNFDBG(x)	cnfdbg x
#define FMTDBG(x)	fmtdbg x
#define IMGDBG(x)	imgdbg x
#define CKIDBG(x)	ckidbg x
#define ACCDBG(x)	accdbg x
#define STBDBG(x)	stbdbg x

/* -v1 and -v0 are NorCroft printf() checking indicators */
#pragma -v1
DBGPROTO(tabdbg);
DBGPROTO(objdbg);
DBGPROTO(prsdbg);
DBGPROTO(rendbg);
DBGPROTO(dicdbg);
DBGPROTO(cnfdbg);
DBGPROTO(fmtdbg);
DBGPROTO(imgdbg);
DBGPROTO(ckidbg);
DBGPROTO(accdbg);
DBGPROTO(stbdbg);
#pragma -v0

#endif	/* DEBUG < 1 */

/*****************************************************************************/

/* Excesive debuging functions */

#if DEBUG < 2

#define TABDBGN(x)
#define OBJDBGN(x)
#define PRSDBGN(x)
#define RENDBGN(x)
#define DICDBGN(x)
#define CNFDBGN(x)
#define FMTDBGN(x)
#define IMGDBGN(x)
#define CKIDBGN(x)
#define ACCDBGN(x)
#define STBDBGN(x)

#else	/* DEBUG < 2 */

#define TABDBGN(x)	tabdbgn x
#define OBJDBGN(x)	objdbgn x
#define PRSDBGN(x)	prsdbgn x
#define RENDBGN(x)	rendbgn x
#define DICDBGN(x)	dicdbgn x
#define CNFDBGN(x)	cnfdbgn x
#define FMTDBGN(x)	fmtdbgn x
#define IMGDBGN(x)	imgdbgn x
#define CKIDBGN(x)	ckidbgn x
#define ACCDBGN(x)	accdbgn x
#define STBDBGN(x)	stbdbgn x

#pragma -v1
DBGPROTO(tabdbgn);
DBGPROTO(objdbgn);
DBGPROTO(prsdbgn);
DBGPROTO(rendbgn);
DBGPROTO(dicdbgn);
DBGPROTO(cnfdbgn);
DBGPROTO(fmtdbgn);
DBGPROTO(imgdbgn);
DBGPROTO(ckidbgn);
DBGPROTO(accdbgn);
DBGPROTO(stbdbgn);
#pragma -v0

#endif	/* DEBUG < 2 */

/*****************************************************************************/

#endif /* __debug_h */

/* eof debug.h */
