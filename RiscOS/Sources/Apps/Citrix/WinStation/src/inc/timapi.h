/*************************************************************************
*
* timapi.h
*
* Timer library header
*
* Copyright 1994, Citrix Systems Inc.
*
* Author: Andy (3/15/94)
*
* $Log$
*  
*     Rev 1.7   15 Apr 1997 18:45:56   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.6   06 May 1996 18:42:38   jeffm
*  update
*  
*     Rev 1.5   02 May 1995 13:40:40   butchd
*  update
*
*************************************************************************/
#ifndef __TIMAPI_H__
#define __TIMAPI_H__

#define TIM__ADDHOOK     0
#define TIM__REMOVEHOOK  1
#define TIM__COUNT       2


/*=============================================================================
==   Internal Routines
=============================================================================*/

int WFCAPI TimerLoad( PPLIBPROCEDURE pfnTimProcedures );
int WFCAPI TimerUnload( VOID );


/*=============================================================================
==   External Routines
=============================================================================*/

#ifndef TIMERLIB

/*
 * Note: These function typedefs must be maintained in sync with the
 *       below function prototypes
 */


typedef int (PWFCAPI PTIMERADDHOOK)( LPVOID );
typedef int (PWFCAPI PTIMERREMOVEHOOK)( LPVOID );

extern PPLIBPROCEDURE pTimerProcedures;

#define TimerAddHook     ((PTIMERADDHOOK)(pTimerProcedures[TIM__ADDHOOK]))
#define TimerRemoveHook  ((PTIMERREMOVEHOOK)(pTimerProcedures[TIM__REMOVEHOOK]))

#else

/*
 * Note: These function prototypes must be maintained in sync with the
 *       function typedefs above
 */

int WFCAPI TimerAddHook( LPVOID fnProc );
int WFCAPI TimerRemoveHook( LPVOID fnProc );

#endif

#endif //__TIMAPI_H__
