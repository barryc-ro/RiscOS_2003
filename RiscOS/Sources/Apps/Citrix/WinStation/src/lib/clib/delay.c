

/*******************************************************************************
*
*   delay.C
*
*      delay n msecs  (1/1000 second)
*
*   Copyright Citrix Systems Inc. 1990
*
*   Author:   Brad Pedersen
*
*   $log:  $
******************************************************************************/

#include "windows.h"

/*  Get the standard C includes */
#include <stdio.h>
#include <string.h>

/*  Get CLIB includes */
#include "../../inc/client.h"
#include "../../inc/clib.h"

/*=============================================================================
==   External Functions Defined
=============================================================================*/

void Delay( int );


/*=============================================================================
==   External Functions Used
=============================================================================*/

/*******************************************************************************
 *
 *  Delay
 *
 *  ENTRY:
 *     delay in 1/1000 seconds
 *
 *  EXIT:
 *     nothing
 *
 ******************************************************************************/

/* maximum value Getmsec will return before wrapping */
#define MAXMSEC (((23L * 360000L) + (59L * 6000L) + (59L * 100L) + 99L) * 10L)

void
Delay( int time )
{
#ifdef WIN32
    Sleep( time );
#else
    ULONG EndTime;
    ULONG CurTime;
    ULONG PrevTime = 0;

    /*
     *  Get the end time
     */
    EndTime = Getmsec() + time;

    /*
     *  Poll for end of delay
     */
    for (;;) {

       /* get current time */
       CurTime = Getmsec();

       /* if clock wrapped adjust end time */
       if ( CurTime < PrevTime ) 
           EndTime -= MAXMSEC;

       /* check for end of delay */
       if ( CurTime >= EndTime )
          break;

       /* save previous time for wrap check */
       PrevTime = CurTime;

       /* yield cpu */
//     Yield();
    }
#endif
}

