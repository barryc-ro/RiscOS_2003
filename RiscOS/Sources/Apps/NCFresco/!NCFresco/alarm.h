/****************************************************************************
 * This source file was written by Acorn Computers Limited. It is part of   *
 * the RISCOS library for writing applications in C for RISC OS. It may be  *
 * used freely in the creation of programs for Archimedes. It should be     *
 * used with Acorn's C Compiler Release 3 or later.                         *
 *                                                                          *
 ***************************************************************************/
/*
 * Title:    alarm.h
 * Purpose:  alarm facilities for wimp programs, using non-busy waiting
 *
 */

#ifndef __alarm_h
#define __alarm_h

# ifndef BOOL
# define BOOL int
# define TRUE 1
# define FALSE 0
# endif

#define alarm_timenow() ncalarm_timenow()
#define alarm_set(a,p,h) ncalarm_set(a,p,h)
#define alarm_removeall(h) ncalarm_removeall(h)
#define alarm_anypending(h) ncalarm_anypending(h)
#define alarm_next(r) ncalarm_next(r)
#define alarm_callnext() ncalarm_callnext()

typedef void (*alarm_handler)(int called_at, void *handle);

extern int ncalarm_timenow(void);
extern void ncalarm_set(int at, alarm_handler proc, void *handle);
extern void ncalarm_removeall(void *handle);
extern BOOL ncalarm_anypending(void *handle);
extern BOOL ncalarm_next(int *result);
extern void ncalarm_callnext(void);

#endif

/* end of alarm.h */
