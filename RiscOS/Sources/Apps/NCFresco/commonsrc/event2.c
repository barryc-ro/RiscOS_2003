/* event2.c */

/* Claiming and releasing of nulls and drag events
 * (C) ANT Limited 1997  All Rights Reserved
 *
 * Authors:
 *      Peter Hartley   <peter@ant.co.uk>
 *
 * History:
 *      03-Jun-97 pdh Started
 *
 */

#include "wimp.h"
#include "alarm.h"
#include "win.h"

#include "memwatch.h"

#include "event2.h"


        /*=================*
         *   Null events   *
         *=================*/


typedef struct e2_nullclaimer {
    event2_null_proc proc;
    void *handle;
    struct e2_nullclaimer *next;
} e2_nullclaimer;

static e2_nullclaimer *nulls = NULL;

static void event2_nullpoll( int called_at, void *handle )
{
    e2_nullclaimer *nc = nulls;

    while ( nc )
    {
        (nc->proc)(nc->handle);
        nc = nc->next;
    }

    /* if we're still active, reschedule */
    if ( nulls )
        alarm_set( 0, event2_nullpoll, NULL );
}

void event2_claim_nulls( event2_null_proc proc, void *handle )
{
    e2_nullclaimer *nc;

    if ( !nulls )
        alarm_set( 0, event2_nullpoll, NULL );

    nc = mm_malloc( sizeof(e2_nullclaimer) );
    nc->proc = proc;
    nc->handle = handle;
    nc->next = nulls;
    nulls = nc;
}

void event2_release_nulls( event2_null_proc proc, void *handle )
{
    e2_nullclaimer *nc, **pnc;

    pnc = &nulls;

    while ( (nc = *pnc) != NULL )
    {
        if ( nc->proc == proc && nc->handle == handle )
        {
            *pnc = nc->next;
            mm_free( nc );
            break;
        }
        pnc = &(nc->next);
    }

    /* if nulls goes to 0 then we won't be rescheduled on next alarm */
}


        /*=================*
         *   Drag events   *
         *=================*/


static event2_drag_proc dragproc = NULL;
static void            *draghandle;
BOOL timed_out = FALSE;     /* in here to help hide it */

static BOOL event2_dragpoll( wimp_eventstr *e, void *handle )
{
    if ( e->e == wimp_EUSERDRAG
         && dragproc )
    {
        (dragproc)( draghandle, &e->data.dragbox );
        event2_release_drag();
        return TRUE;
    }
    return FALSE;
}

void event2_claim_drag( event2_drag_proc proc, void *handle )
{
    if ( !dragproc )
    {
        win_add_unknown_event_processor( event2_dragpoll, NULL );
    }
    dragproc = proc;
    draghandle = handle;
}

void event2_release_drag( void )
{
    if ( dragproc )
    {
        win_remove_unknown_event_processor( event2_dragpoll, NULL );
        dragproc = NULL;
    }
}

        /*================*
         *   Icon stuff   *
         *================*/


void icon_setshade( wimp_w w, wimp_i i, BOOL shade )
{
    wimp_set_icon_state(w,i, shade ? wimp_INOSELECT : 0, wimp_INOSELECT );
}

void icon_setselect( wimp_w w, wimp_i i, BOOL select )
{
    wimp_set_icon_state(w,i, select ? wimp_ISELECTED : 0, wimp_ISELECTED );
}

BOOL icon_selected( wimp_w w, wimp_i i )
{
    wimp_icon wi;
    if ( wimp_get_icon_info( w, i, &wi ) )
        return FALSE;   /* window/icon doesn't exist */
    return (wi.flags & wimp_ISELECTED) != 0;
}

        /*===EOF===*/
