/* remote.c */

/* "Remote control" of Fresco from a test harness
 * (C) ANT Limited 1997  All Rights Reserved
 *
 * Authors:
 *      Peter Hartley   <peter@ant.co.uk>
 *
 * History:
 *      03-Mar-97 pdh Started
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#if 0
#include "../!NCFresco/tcplibs.h"
#endif

#ifdef STBWEB
# include "alarm.h"
# include "wimp.h"
#else
# include "win.h"
# include "event.h"
#endif

#include "debug.h"
#include "version.h"
#include "remote.h"

#ifdef DEBUG

static int sock = -1;

#define BOGUS_WINDOW_HANDLE -6561
/* This used to be -42, but I discovered there was *already* a bodge in
 * RiscOS_Lib relying on window handles of -42 ... thus proving (a) all techies
 * have worryingly similar intertextual contexts, and (b) RiscOS_Lib is but
 * a thin gold thread of sensible code grotesquely bound about with hanging
 * gardens of bodges.
 */

#if 0
void RemoteControl__Poll( wimp_eventstr*, void* );

#ifdef STBWEB
static void RemoteControl__Poll1( int called_at, void *handle);
#endif
#endif

BOOL RemoteControl_Open( void )
{
#if 0
    int i, rc;
    time_t t = time(NULL);
    struct sockaddr_in sa;

    sock = socket( AF_INET, SOCK_STREAM, 0 );

    if ( sock < 0 )
    {
        usrtrc( "remotecontrol: can't create socket\n" );
        return FALSE;
    }

    i = 1;
    socketioctl( sock, FIONBIO, &i );
    i = 1;
    socketioctl( sock, FIOASYNC, &i );

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr( remotecontrol_HOST );
    sa.sin_port = htons( remotecontrol_PORT );
    memset( sa.sin_zero, 0, 8 );

    rc = -1;

    while ( difftime( time(NULL), t ) < 1.0
             && rc == 0 )
    {
        rc = connect( sock, (struct sockaddr *) &sa, sizeof(struct sockaddr));

        /* Legal return values are:
         *
         *  EINPROGRESS(36) The connection process has been started but no
         *                  result as yet.
         *  EINVAL(22)      The connection process failed, either due to a
         *                  timeout or a rejection.
         *  EALREADY(37)    The connection process is still going on and may yet
         *                  finish.
         *  EISCONN(56)     The connection process succeeded and we are now
         *                  connected.
         */

        if ( rc < 0 )
        {
            switch ( errno )
            {
            case EALREADY:
            case EISCONN:
                rc = 0;
                break;
            case EINPROGRESS:
                break;
            default:
                fprintf( stderr, "remotecontrol: initial connect failed (%d)\n",
                         errno );
                RemoteControl_Close();
                return FALSE;
            }
        }
    }

    if ( rc < 0 )
    {
        fprintf( stderr, "remotecontrol: connect timed out\n" );
        RemoteControl_Close();
        return FALSE;
    }

    RemoteControl_Send( "TESTME Fresco " VERSION_NUMBER "\n" );

    atexit( RemoteControl_Close );

    fprintf( stderr, "remote control enabled\n" );

#ifdef STBWEB
    alarm_set(0, RemoteControl__Poll1, NULL);
#else
    /* Look, right. I want idle events. But I don't have a window. And
     * RiscOS_Lib (as opposed to RiscOS) won't let me get idle events without
     * one. So therefore this. Blame & flame for this filth to be directed at
     * the authors of RiscOS_Lib, please, not at me.
     */

    win_register_event_handler( BOGUS_WINDOW_HANDLE, &RemoteControl__Poll, 0 );
    win_claim_idle_events( BOGUS_WINDOW_HANDLE );
    event_setmask( 0 );
#endif

#endif
    return TRUE;
}

BOOL RemoteControl_Active( void )
{
    return sock != -1;
}

#if 0
void RemoteControl_Send( const char *string )
{
    if ( sock != -1 )
        send( sock, (char*)string, strlen( string ), 0 );
}

void RemoteControl__Poll( wimp_eventstr *e, void *f )
{
    char buffer[1501];
    int rc;

    if ( sock != -1 )
    {
        rc = recv( sock, buffer, 1500, 0 );
        if ( rc>0 )
        {
            buffer[rc+1] = 0;
            fprintf( stderr, "remotecontrol: %s\n", buffer );
        }
        else if ( rc == 0 || errno != EWOULDBLOCK )
        {
            fprintf( stderr, "remotecontrol: connection closed\n" );
            RemoteControl_Close();
        }
    }
}

#ifdef STBWEB
static void RemoteControl__Poll1( int called_at, void *handle)
{
    RemoteControl__Poll(0, 0);

    alarm_set(0, RemoteControl__Poll1, handle);
    called_at = called_at;
}
#endif
#endif

void RemoteControl_Close( void )
{
#if 0
    if ( sock != -1 )
    {
        socketclose( sock );
        sock = -1;
    }
#endif
}

#endif
