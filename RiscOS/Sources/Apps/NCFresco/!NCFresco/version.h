/* -*-c-*- */

#ifndef __version_h
#define __version_h

#ifndef __debug_h
#include "debug.h"
#endif

#if !defined(ANT_NCFRESCO) && defined( STBWEB )
#include "VersionNum"
#endif

/* version.h */

extern char *fresco_version;
extern char *program_name;
extern char *program_title;

#define PROGRAM_NAME		"NCFresco"
#define PROGRAM_TITLE		"NCBrowser"

#ifdef ANT_NCFRESCO

# define VERSION_NUMBER		"1.33A"     /* A for ANT... hmmm */
# define BASE_VERSION_NUMBER	"1.63"      /* Fresco version */
# ifdef PRODUCTION
#  define VERSION_QUALIFIER	"("BASE_VERSION_NUMBER")"
# else
#  define VERSION_QUALIFIER	"("BASE_VERSION_NUMBER" Development)"
# endif

#else

# define VERSION_NUMBER		Module_MajorVersion
# define BASE_VERSION_NUMBER	"1.63" /* Fresco version */
# ifdef PRODUCTION
#  define VERSION_QUALIFIER	"("BASE_VERSION_NUMBER")" Module_MinorVersion
# else
#  define VERSION_QUALIFIER	"("BASE_VERSION_NUMBER" Development)" Module_MinorVersion
# endif

#endif

#define ANTI_TWITTER		1	/* Define non-zero for thick lines etc. */
#define GROSS_ANTI_TWITTER	0	/* Define this to blur each rectangle */

#if 0
#define SPECIAL_SUFFIX	"-WCCE'95"
#else
#define SPECIAL_SUFFIX	""
#endif

/* If TIMEOUT is non-zero then the timeout date will be checked on startup */
#ifndef TIMEOUT
#define TIMEOUT	0
#endif

#define TIMEOUT_MUNGE 	0xEA		/* Make the top byte look like an instruction! */
#define TIMEOUT_DAY	1
#define TIMEOUT_MONTH	8
#define TIMEOUT_YEAR	1995

/* If TRUMPET is non-zero then the program will blow its own trumpet by sticking tha app. name everywhere */
#ifndef TRUMPET
#define TRUMPET 1
#endif

/* If PSYCHO is defined non-zero then licence violations will delete the !RunImage */
#ifndef PSYCHO
#define PSYCHO 0
#endif

/* Use this instead of fprintf(stderr,...) for production messages */

#pragma -v1
extern void usrtrc(const char *fmt, ...);
#pragma -v0

#endif /* __version_h */
