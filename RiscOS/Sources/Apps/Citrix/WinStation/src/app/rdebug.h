/* > rdebug.h
 *
 */

#ifndef __rdebug_h
# define __rdebug_h

/* --------------------------------------------------------------------------------------------- */

#ifdef DEBUG

extern void dbghdr(int priority, const char *file, int line);

extern void dbg(const char *fmt, ...);
extern void fdbg(void * /* FILE * */ f, const char *fmt, ...);
extern void vfdbg(void * /* FILE * */ f, const char *fmt, void * /* va_list */arglist);

#define DBG(x)		dbg x
#define FDBG(x)		fdbg x
#define VFDBG(x)	vfdbg x

#else

#define DBG(x)
#define FDBG(x)
#define VFDBG(x)

#endif

extern void rdebug_open(void);
extern void rdebug_poll(void);
extern void *rdebug_session(void);

/* --------------------------------------------------------------------------------------------- */


#endif

/* eof rdebug.h */

