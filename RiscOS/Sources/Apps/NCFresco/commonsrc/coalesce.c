/* coalesce.c */

/* Run Fresco rid text items together to save memory
 * (C) ANT Limited 1997  All Rights Reserved
 *
 * Authors:
 *      Peter Hartley   <peter@ant.co.uk>
 *
 * History:
 *      04-Mar-97 pdh Started
 *      18-Mar-97 pdh Change to cope with coalescing unformatted streams
 *
 */

#include <string.h>

#include "debug.h"
#include "rid.h"
#include "memwatch.h"
#include "object.h"
#include "coalesce.h"

#if DEBUG && 0
#define fdebugf fprintf
#else
#define fdebugf 1?0:fprintf
#endif

#define DEFEAT_COALESCE 1

/* Replaces all occurrences of one character with another */

static void strrepl( char *s, char from, char to )
{
    while ( *s )
    {
        if ( *s == from )
            *s = to;
        s++;
    }
}

/* Counts occurrences of the given character */

static int strcount( const char *s, char c )
{
    int n = 0;

    while ( *s )
    {
        if ( *s == c )
            n++;
        s++;
    }
    return n;
}

/*
 * When coalescing text items, we carefully ensure that the resulting item
 * has spaces (0x20's) in only where *we've* joined items up. Any other spaces,
 * which there probably won't be anyway, are converted to hard spaces (0xA0's)
 * so we can tell the difference when uncoalescing.
 *
 * Bearing in mind some of the spelling in these sources, calling a routine
 * and its C file "coalesce" could be asking for trouble, but we'll see how
 * it goes.
 */

void coalesce( rid_header *rh, rid_text_item *ti, rid_text_item *dont )
{
#ifndef BUILDERS
    rid_text_item_text *tit = (rid_text_item_text*)ti, *tit2;
    rid_pos_item *line = ti->line;      /* May be NULL */

#if DEBUG
    int tried = 0;
#endif

#if DEFEAT_COALESCE
    return;
#endif

    if ( rh == NULL
         || ti == NULL
         || dont == NULL )
        return;

    fdebugf( stderr, "coalesce: %p - %p\n", ti, dont );

    while ( tit && tit->base.line == line && (&tit->base) != dont
            && (tit->base.flag & rid_flag_LINE_BREAK) == 0 )
    {
        tit2 = (rid_text_item_text*) tit->base.next;

        if ( tit2
             && tit2->base.line == line
             && (&tit2->base) != dont )
        {
            if ( tit->base.tag == rid_tag_TEXT
                 && tit2->base.tag == rid_tag_TEXT
                 && (tit->base.flag & ~0x2003) == (tit2->base.flag & ~0x2003)
                 && *((int*)&tit->base.st) == *((int*)&tit2->base.st)
                 && tit->base.aref == tit2->base.aref
                 && (tit->base.flag & 0x10FF) == rid_flag_FVPR
                 && (tit->base.flag & 0xFF) == 0
                 && (tit2->base.flag & 3) != 2
                 && tit->base.pad
                 && tit2->base.pad)   /* if there's a space */
            {
                char *s = rh->texts.data + tit->data_off;
                char *s2 = rh->texts.data + tit2->data_off;
                int len = strlen(s);
                char *expect;

#if DEBUG
                tried++;
#endif

                fdebugf( stderr, "coalesce: flags 0x%X 0x%X %s\n",
                         tit->base.flag, tit2->base.flag, s );

                if ( tit->base.flag & rid_flag_COALESCED )
                {
                    expect = s + len + 1 + strcount( s, ' ' );
                }
                else
                {
                    strrepl( s, ' ', '\xA0' );
                    expect = s + len + 1;
                }

                /* Only do it for adjacent texts (i.e. not in the case
                 * of floating tables!)
                 */
                if ( s2 == expect )
                {
                    strrepl( s2, ' ', '\xA0' );

                    strcpy( s+len, s2 );
                    if ( len )
                        s[len-1] = ' ';

                    tit->base.width = tit->base.width + tit->base.pad
                                        + tit2->base.width;
                    tit->base.pad = tit2->base.pad;
                    tit->base.next = tit2->base.next;
                    /* maxup and maxdown will be the same as the font is the
                     * same
                     */
                    if ( tit2->base.flag & rid_flag_LINE_BREAK )
                        tit->base.flag |= rid_flag_LINE_BREAK;

                    tit->base.flag |= rid_flag_COALESCED;

/*                     if ( st->text_last == &tit2->base ) */
/*                         st->text_last = &tit->base; */
/*  */
/*                     if ( st->text_fvpr == &tit2->base ) */
/*                         st->text_fvpr = &tit->base; */

                    mm_free( tit2 );
                    tit2 = tit;
                }
            }
        }

        tit = tit2;
    }

#if DEBUG
    fdebugf( stderr,  "coalesce: tried %d words\n", tried );
#endif

#endif
}

void un_coalesce( rid_header *rh, rid_text_item *ti )
{
#ifndef BUILDERS
    rid_text_item_text *tit = (rid_text_item_text*) ti;
    rid_pos_item *line;
    int count;
    char *s;
    int len;
    int i;
    rid_text_item_text *list = NULL, *ptr;
    BOOL bFinalSpace;
    char *from;

    fdebugf( stderr, "uncoalesce: %p\n", ti );

    if ( rh == NULL
         || ti == NULL
         || ti->tag != rid_tag_TEXT
       )
        return;

    s = rh->texts.data + tit->data_off;
    len = strlen(s);

    if ( (ti->flag & rid_flag_COALESCED) == 0 )
        return;

    ti->flag &= ~rid_flag_COALESCED;

    count = strcount( s, ' ' );

    bFinalSpace = s[len-1] == ' ';

    if ( bFinalSpace )
        count--;

    fdebugf( stderr, "unco: %d spaces\n", count );

    if ( !count )
        return;

    fdebugf( stderr, "unco: was «%s»\n", s );

    for ( i=0; i<count; i++ )
    {
        ptr = mm_malloc( sizeof(rid_text_item_text) );   /* the turtle moves */
        *ptr = *tit;
        ptr->base.flag &= ~rid_flag_LINE_BREAK;
        ptr->base.next = (rid_text_item*)list;
        list = ptr;
    }

    ptr = list;     /* keep compiler happy */

    while ( ptr->base.next )
        ptr = (rid_text_item_text*)ptr->base.next;

    /* ptr now points at end of list */

    ptr->base.next = (rid_text_item*)ti->next;
    ti->next = (rid_text_item*)list;

    if ( tit->base.flag & rid_flag_LINE_BREAK )
        ptr->base.flag |= rid_flag_LINE_BREAK;

    tit->base.flag &= ~rid_flag_LINE_BREAK;

    /* Recalculate this as texts.data is a flex anchor and could've moved */

    s = rh->texts.data + tit->data_off;

    from = s + len - 1;

    if ( bFinalSpace )
        from--;

    for ( i=count; i>0; i-- )
    {
        while ( *from != ' '
                && from > s )
            from--;

        if ( from != s )
            from++;     /* character after space */

        fdebugf( stderr, "copying «%s»\n", from );

        memmove( from+i, from, strlen(from)+1 );

        *from = 0;  /* character after space is 0 */

        from -= 2;  /* points before space */
    }

    ptr = (rid_text_item_text*)ti->next;

    fdebugf( stderr, "      now «%s»", s );

    for ( i=0; i < count; i++ )
    {
        s += strlen(s)+1;
        strrepl( s, '\xA0', ' ' );

        fdebugf( stderr, " «%s»", s );
        ptr->data_off = s - rh->texts.data;

        /* @@@@ double bodge
         * otext_size  (a) doesn't shift the flex area
         *         and (b) doesn't use its third argument.
         * If either of those facts changes, this will go wrong
         */
        otext_size( (rid_text_item*)ptr, rh, NULL );

        ptr = (rid_text_item_text*)ptr->base.next;
    }

    otext_size( ti, rh, NULL );

    fdebugf( stderr, "\nunco: out\n" );

    /* Phew... */
#endif
}
