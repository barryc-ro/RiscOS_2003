/* history.c */

/* History list for Fresco
 * (C) ANT Limited 1997  All Rights Reserved
 *
 * Authors:
 *      Nicko van Someren   <nicko@ncipher.com>
 *      Peter Hartley       <peter@ant.co.uk>
 *
 * History:
 *      10-Mar-97 pdh Split off from frontend.c
 *
 */

#include <string.h>

#include "msgs.h"
#include "menu.h"

#include "guimenu.h"
#include "memwatch.h"
#include "flexwrap.h"
#include "util.h"
#include "makeerror.h"
#include "frontend.h"
#include "guifns.h"

#include "history.h"

#if 0 && DEBUG
#define fdebugf fprintf
#else
#define fdebugf 1?0:fprintf
#endif

historylist history_new( int nEntries )
{
    historylist res;

    if ( nEntries < 1 )
        nEntries = 1;

    res = mm_calloc( 1, sizeof(historyliststr)
                         + (nEntries-1)*sizeof(unsigned int));

    res->nSize = nEntries;

    if ( flex_alloc( &res->pEntries, nEntries*2 ) )
    {
        memset( res->pEntries, 0, nEntries*2 );
        res->nStringSize = nEntries*2;

        fdebugf( stderr, "hist%p: %d entries allocated OK\n", res, nEntries );
    }
    else
    {
        mm_free( res );
        res = NULL;

        fdebugf( stderr, "*** history_new failed to allocate memory\n" );
    }

    return res;
}

historylist history_clone( historylist hl )
{
    historylist res;
    int hlsize = sizeof(historyliststr) + (hl->nSize-1)*sizeof(unsigned int);
    int flexsize = hl->nStringSize;

    fdebugf( stderr, "hist%p: cloning\n", hl );

    res = mm_malloc( hlsize );
    memcpy( res, hl, hlsize );

    fdebugf( stderr, "hist%p: clone of %p\n", res, hl );

    res->pEntries = NULL;
    if ( flex_alloc( &res->pEntries, flexsize ) )
    {
        fdebugf( stderr, "hist%p: cloned %d bytes of entries\n", res, flexsize );
        memcpy( res->pEntries, hl->pEntries, flexsize );
    }
    else
    {
        fdebugf( stderr, "hist%p: flex_alloc FAILED\n", res );
        mm_free( res );
        res = NULL;
    }

    fdebugf( stderr, "hist%p: clone returning %p\n", hl, res );

    return res;
}

void history_destroy( historylist *phl )
{
    if ( *phl )
    {
        fdebugf( stderr, "hist%p: deleting\n", *phl );

        if ( (*phl)->pEntries )
            flex_free( &((*phl)->pEntries ) );
        mm_free( *phl );
    }
    *phl = NULL;
}

void history_add( historylist hl, const char *url, const char *title )
{
    int offset;
    int oldsize, newsize;
    BOOL ok = TRUE;
    char *ptr;
    int i;

    if ( !hl )
        return;

    ptr = hl->pEntries;

    for ( i=0; i < hl->nContents; i++ )
    {
        if ( !strcmp( ptr, url ) )
            return;
        ptr += strlen(ptr)+1;
        ptr += strlen(ptr)+1;
    }

    /* Wasn't in the history -- must add it */

    if ( !title )
        title = "";

    /* Discover length of final entry */

    ptr = hl->pEntries + hl->nStringSize;

    ptr--;  /* at final 0 */
    while ( ptr[-1] )
        ptr--;          /* start of last string */

    ptr--;
    while ( ptr[-1] )
        ptr--;          /* start of last-but-one string */

    offset = ptr - hl->pEntries;

    oldsize = hl->nStringSize - offset;

    newsize = strlen(url) + strlen(title) + 2;

    fdebugf( stderr, "hist%p: adding url «%s» title «%s» to history, oldsize=%d, newsize=%d\n",
            hl, url, title, oldsize, newsize );

    if ( oldsize != newsize )
        ok = flex_extend( &hl->pEntries, hl->nStringSize+newsize-oldsize );

    /* Not that there's a great deal we can do if it's not OK */
    if ( ok )
    {
        ptr = hl->pEntries;

        fdebugf( stderr, "hist%p: moving %d bytes up %d\n",
                 hl, hl->nStringSize - oldsize, newsize );

        memmove( ptr + newsize, ptr, hl->nStringSize - oldsize );

        strcpy( ptr, url );
        ptr += strlen(url) + 1;
        strcpy( ptr, title );

        fdebugf( stderr, "hist%p: moving %d bytes from %p to %p\n",
                 hl, (hl->nSize-1) * sizeof(unsigned int),
                 hl->hash, hl->hash+1 );

        memmove( &hl->hash[1], &hl->hash[0],
                 (hl->nSize-1) * sizeof(unsigned int) );

        hl->nAt = 0;

        if ( hl->nContents < hl->nSize )
            hl->nContents++;

        hl->hash[0] = string_hash( url );

        hl->nStringSize += newsize-oldsize;

        fdebugf( stderr, "hist%p: add successful, stringsize now %d\n",
                 hl, hl->nStringSize );
    }
    else
        fdebugf( stderr, "hist%p: add failed\n", hl );
}

BOOL history_test( historylist hl, const char *url )
{
    char *ptr;
    unsigned int hash;
    int i;

    if ( !hl )
        return FALSE;

    hash = string_hash(url);	/* Hash the url for quick lookup */

    ptr = hl->pEntries;

    for ( i=0; i < hl->nContents; i++ )
    {
        if ( hash == hl->hash[i]
             && !strcmp( ptr, url ) )
            return TRUE;
        ptr += strlen(ptr)+1;
        ptr += strlen(ptr)+1;
    }

    return FALSE;
}

/* History menu */

menu history_menu = NULL;

static char *strflexdup( char **anchor, int offset )
{
    int len = strlen( (*anchor) + offset ) + 1;
    char *res = mm_malloc( len );
    memcpy( res, (*anchor)+offset, len );
    return res;
}

os_error *history_make_menu( historylist hl, fe_view v, be_item ti, int context)
{
    char *mtext;
    char *url, *title;
    int i;
    int offset = 0;

    history_kill_menu();

    if ( hl == NULL || hl->nContents == 0 )
	return makeerror(ERR_BAD_CONTEXT);

    url = hl->pEntries;

    title = url + strlen(url) + 1;

    offset = ( title + strlen(title) + 1 ) - hl->pEntries;

    mtext = strflexdup( &hl->pEntries, *title ? title-url : 0 );

    fdebugf( stderr, "hist%p: making new history menu\n", hl );

    history_menu = menu_new( msgs_lookup("hmenu"), fe_menu_padding(mtext) );

    fdebugf( stderr, "hist%p: first item is «%s»\n", hl, mtext);
    menu_make_text(history_menu, 1, mtext);

    mm_free( mtext );

    for ( i=1; i < hl->nContents; i++ )
    {
        url = hl->pEntries + offset;
        title = url + strlen(url) + 1;
        offset = ( title + strlen(title) + 1 ) - hl->pEntries;

	mtext = strflexdup( &hl->pEntries, *title ? title - hl->pEntries
	                                          : url - hl->pEntries );

	menu_extend(history_menu, fe_menu_padding(mtext));

	fdebugf( stderr, "hist%p: item %d is «%s»\n", hl, i+1, mtext);
	menu_make_text(history_menu, i+1, mtext);
        fdebugf( stderr, "hist%p: freeing mtext\n", hl );

	mm_free( mtext );
    }

    if ( hl->nAt > 0 && hl->nAt < hl->nSize )
    {
        fdebugf( stderr, "hist%p: ticking %d\n", hl, hl->nAt+1 );
	menu_setflags(history_menu, hl->nAt + 1, 1, 0);
    }

    fdebugf( stderr, "hist%p: raising menu\n", hl );

    /* Then raise it */
    guimenu_raise_submenu(context, history_menu, fe_gui_history_menu, v, ti);

    return NULL;
}

void history_kill_menu( void )
{
    if (history_menu)
	menu_dispose(&history_menu, 0);

    history_menu = NULL;
}


/* history_hit_menu
 *
 * Deal with menu hit -- NOTE that param counts from 1 whereas most everything
 * else, including hl->nAt, counts from 0
 */

os_error *history_hit_menu(historylist hl, fe_view v, int param)
{
    char *url;
    int i;
    os_error *e;

    history_kill_menu();

    if ( !hl || param < 1 || param > hl->nContents )
        return NULL;

    fdebugf( stderr, "hist%p: opening item %d\n", hl, param );

    url = hl->pEntries;

    for ( i=1; i < param; i++ )
    {
        url += strlen( url ) + 1;
        url += strlen( url ) + 1;
    }

    hl->nAt = param-1;

    url = strflexdup( &hl->pEntries, url - hl->pEntries );

    fdebugf( stderr, "hist%p: opening url «%s»\n", hl, url );

	/* SJM: added HISTORY flag */
	/* pdh: added referer flag */
    e = frontend_open_url( url, v, NULL, 0,
                           fe_open_url_FROM_HISTORY | fe_open_url_NO_REFERER );
    mm_free( url );
    return e;
}

/* Moving to and fro in history */

os_error *history_move(historylist hl, int param, fe_view v)
{
    int i;
    char *ptr;
    os_error *e;

    if (v == NULL)
	return makeerror(ERR_BAD_CONTEXT);

    i = -1;

    switch(param)
    {
    case history_FIRST: i = hl->nContents - 1; break;
    case history_PREV:  i = hl->nAt + 1;       break;
    case history_NEXT:  i = hl->nAt - 1;       break;
    case history_LAST:  i = 0;                 break;
    }

    if ( i == hl->nAt
         || i < 0
         || i >= hl->nContents )
	return makeerror(ERR_NO_HISTORY);

    hl->nAt = i;

    ptr = hl->pEntries;

    while ( i )
    {
        ptr += strlen(ptr)+1;
        ptr += strlen(ptr)+1;
        i--;
    }

    ptr = strflexdup(&hl->pEntries, ptr - hl->pEntries );

    e = frontend_open_url( ptr, v, NULL, 0,
                           fe_open_url_FROM_HISTORY | fe_open_url_NO_REFERER );

    mm_free( ptr );
    return e;
}
