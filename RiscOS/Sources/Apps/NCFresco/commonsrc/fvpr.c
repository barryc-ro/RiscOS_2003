/* -*-C-*-
 *
 * fvpr.c - Final Value Progressive Rendering
 *
 * (C) ANT Limited 1997. All rights reserved.
 */

#include "os.h"

#include "debug.h"
#include "rid.h"
#include "antweb.h"
#include "parsers.h"
#include "interface.h"
#include "backend.h"
#include "fvpr.h"
#include "images.h"
#include "myassert.h"
#include "htmlparser.h"
#include "gbf.h"
#include "dump.h"
#include "tables.h"

/* Forward reference */

static BOOL fvpr_progress_stream_2(rid_text_stream *stream, BOOL *pbComplete);

/*****************************************************************************

  Set FVPR flag on this item and any items it contains. There might not be
  a complete pos tree, so don't rely on it.

  */

static void set_fvpr_and_below(rid_text_item *item)
{
    item->flag |= rid_flag_FVPR;

    switch (item->tag)
    {
    case rid_tag_TABLE:
    {
	rid_table_item *table = ((rid_text_item_table *)item)->table;
	int x, y;
	rid_table_cell *cell;
	rid_text_item *ti;

	if (gbf_active(GBF_FVPR))
	{
	    /*ASSERT((table->flags & rid_tf_FINISHED) == 0);*/
	}

	if (table->caption != NULL)
	    for (ti = table->caption->stream.text_list; ti != NULL; ti = rid_scanf(ti))
		set_fvpr_and_below(ti);

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	    for (ti = cell->stream.text_list; ti != NULL; ti = rid_scanf(ti))
		set_fvpr_and_below(ti);
    }
    break;
    case rid_tag_OBJECT:
	break;
    }

}


/*****************************************************************************

  Clear FVPR flag on this item and any items it contains.

  */

static void forget_fvpr_and_below(rid_text_item *item)
{
    if ( gbf_active(GBF_FVPR) )
    {
        item->flag &= ~rid_flag_FVPR;

        switch (item->tag)
        {
        case rid_tag_TABLE:
        {
	    rid_table_item *table = ((rid_text_item_table *)item)->table;
            int x, y;
    	    rid_table_cell *cell;
    	    rid_text_item *ti;

	if (table->caption != NULL)
	    for (ti = table->caption->stream.text_list; ti != NULL; ti = rid_scanf(ti))
		forget_fvpr_and_below(ti);

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	    for (ti = cell->stream.text_list; ti != NULL; ti = rid_scanf(ti))
		forget_fvpr_and_below(ti);
        }
        break;
        case rid_tag_OBJECT:
	    break;
	}
    }
}

/*****************************************************************************

  Images flags that, when any of them is set, indicates we have final
  position and size values and can continue FVPR beyond the image in
  question.

 */

#define IMAGE_RENDERABLE_FLAGS ( \
	image_flag_FETCHED | \
	image_flag_RENDERABLE | \
	image_flag_ERROR | \
	image_flag_DEFERRED | \
	image_flag_TO_RELOAD | \
	image_flag_REALTHING )


/*--------------------------------------*
 * fvpr_item                            *
 * Stop here because of this text item? *
 *--------------------------------------*/

static BOOL fvpr_item( rid_text_item *ti )
{
    BOOL stop = FALSE;

    switch (ti->tag)
    {
    /* Item arives FVPR atomically */
    default:
    case rid_tag_TEXT:
    break;

    case rid_tag_PBREAK:
	break;
    case rid_tag_HLINE:
	break;
    case rid_tag_BULLET:
	break;
    case rid_tag_IMAGE:
    {
	rid_text_item_image *im = (rid_text_item_image *) ti;
	image_flags flags;

	/* sjm: if image is fully specified then don't need to know any more*/
	if (im->ww.type != value_none && im->hh.type != value_none)
	{
	    RENDBG(("fvpr: image full specified\n"));
	    break;
	}

	if (im->im == NULL)
	{
	    RENDBG(("fvpr: no im field for image\n"));
	    stop = TRUE;
	    break;
	}

	if ( image_info( (image) im->im, NULL, NULL, NULL, &flags, NULL, NULL)
	     != NULL )
	{
	    RENDBG(("fvpr: error from image_info - delay until end of page\n"));
	    stop = TRUE;
	    break;
	}

        RENDBG(("fvpr: encountered image with flags=0x%x\n",flags));

        /* pdh: this && was an || but if the HTML gives the exact size
         * then fvpr may as well carry on
         */
	if ( ( (flags & IMAGE_RENDERABLE_FLAGS) == 0 )
	     /* && */
/* 	     (im->ww.type == value_none || im->hh.type == value_none)  */)
	{
	    RENDBG(("fvpr: don't know final size yet\n"));
	    stop = TRUE;
	    break;
	}

    }
    break;

    case rid_tag_INPUT:
    {
	rid_input_item *ii = ((rid_text_item_input *)ti)->input;
	switch (ii->tag)
	{
	case rid_it_IMAGE:
	    break;
	}
	break;
    }

    case rid_tag_TEXTAREA:
	break;

    case rid_tag_SELECT:
    {
	rid_select_item *sel = ((rid_text_item_select *)ti)->select;
	/* if we haven't received all the OPTION items then it may resize */
	if ((sel->flags & rid_sf_FINISHED) == 0)
	    stop = TRUE;
	break;
    }

    case rid_tag_TABLE:
    {
	rid_table_item *table = ((rid_text_item_table *)ti)->table;

	if ((table->flags & rid_tf_FINISHED) == 0)
	{
	    RENDBG(("fvpr: table not completed yet\n"));
	    stop = TRUE;
	}
	else
	{
            int x, y;
            rid_table_cell *cell;
            BOOL bComplete = TRUE;

	    RENDBG(("fvpr: table complete - scanning inside it\n"));

	    /* the following do not change 'changed' because we don't
	     * care whether cells have changed, only whether the whole
	     * table has
	     */

            if (table->caption != NULL)
                fvpr_progress_stream_2( &table->caption->stream,
                                        &bComplete );

            if ( !bComplete )
            {
                RENDBG(("fvpr: caption not finalised\n"));
                stop = TRUE;
                break;
            }

            for ( x = -1, y = 0;
                 (cell = rid_next_root_cell(table, &x, &y)) != NULL
                    && bComplete;
                 /* skip */
                 )
            {
                fvpr_progress_stream_2( &cell->stream, &bComplete );

                if ( !bComplete )
                    RENDBG(("fvpr: cell (%d,%d) not finalised\n",x,y));
            }

            if ( !bComplete )
            {
                stop = TRUE;
                break;
            }
	}
    }
    break;
    case rid_tag_OBJECT:
			/* need to fill stuff in here for rid_plugin_IMAGE */
	break;
    }

    return stop;
}


/*------------------------------------*
 * fvpr_floater                       *
 * Stop here because of this floater? *
 *------------------------------------*/

static BOOL fvpr_floater( rid_float_item *fi )
{
    while ( fi )
    {
        if ( fi->ti && fvpr_item( fi->ti ) )
            return TRUE;
        fi = fi->next;
    }
    return FALSE;
}


/*****************************************************************************

  Supplied with the TOP LEVEL stream.

  Attempts to advance pos_fvpr and text_fvpr, setting rid_flag_FVPR as
  it goes.

  Returns TRUE if the fvpr values changes.


  */

static BOOL fvpr_progress_stream_2(rid_text_stream *stream, BOOL *pbComplete)
{
    BOOL changed = FALSE;
    rid_text_item *ti;
    rid_pos_item *lastpi = NULL;

    if (gbf_active(GBF_FVPR))
    {
	RENDBG(("fvpr_progress_stream(%p)\n", stream));

        if ( pbComplete )
            *pbComplete = FALSE;

	if (stream == NULL || stream->text_list == NULL || stream->pos_list == NULL)
	{
	    RENDBG(("SOMETHING STILL NULL\n"));

            if ( pbComplete )
                *pbComplete = TRUE; /* pdh: not sure about this */

	    return FALSE;
	}

	if (stream->text_fvpr == NULL)
	    stream->text_fvpr = stream->text_list;

	ti = stream->text_fvpr;

	RENDBG(("fvpr: start tracing from item %p\n", ti));

	while (1)
	{
	    rid_pos_item *pi = ti->line;

	    if (pi == NULL || pi->next == NULL)
	    {
		RENDBG(("fvpr: stoppping for pos_item reasons\n"));
		break;
	    }

	    if ( pi != lastpi && pi->floats )
	    {
	        /* Try and only check each pi once (efficiency). We'll still
	         * be checking each floater several times though; perhaps
	         * rid_float_item really wants an fvpr flag?
	         */

	        if ( fvpr_floater( pi->floats->left ) )
                {
                    RENDBG(("fvpr: stopping for left-floater reasons\n"));
                    break;
                }
	        if ( fvpr_floater( pi->floats->right ) )
                {
                    RENDBG(("fvpr: stopping for right-floater reasons\n"));
                    break;
                }
	    }
            lastpi = pi;

            /* Stop for text_item reasons? */
	    if ( fvpr_item(ti) )
		break;

	    ti = ti->next;

	    if (ti == NULL || ti->line == NULL)
	    {
		RENDBG(("fvpr: hit end of text_item stream\n"));
		break;
	    }
	}

	if (ti != stream->text_fvpr)
	{
	    rid_text_item *mark = stream->text_fvpr;

	    changed = TRUE;

	    RENDBG(("fvpr: FVPR marking from %p to just before %p\n", mark, ti));

	    if (ti == NULL)
	    {
		ti = stream->text_last;

		/* pdh: make sure last item marked! */
		ti->flag |= rid_flag_FVPR;

		if ( pbComplete )
		    *pbComplete = TRUE;
	    }

	    for (; mark != ti; mark = rid_scanf(mark))
	    {
		set_fvpr_and_below(mark);
	    }

	    stream->text_fvpr = ti;
	}

	/* Should always be stopping AT the end */
	ASSERT(stream->text_fvpr != NULL);
    }

    return changed;
}

extern BOOL fvpr_progress_stream(rid_text_stream *stream)
{
    return fvpr_progress_stream_2( stream, NULL );
}

/*****************************************************************************/

extern BOOL fvpr_progress_stream_flush(rid_text_stream *stream)
{
    BOOL changed = FALSE;

    if (gbf_active(GBF_FVPR))
    {
	if (stream->text_fvpr == NULL)
	    stream->text_fvpr = stream->text_list;

	RENDBG(("fvpr: flushing from %p to %p\n", stream->text_fvpr, stream->text_last));

	if (stream->text_fvpr != NULL)
	{
	    if (stream->text_fvpr != stream->text_last)
	    {
		rid_text_item *ti;

		changed = TRUE;

		for (ti = stream->text_fvpr; ti != stream->text_last; ti = rid_scanf(ti))
		{
		    set_fvpr_and_below(ti);
		}

		/* pdh: last one too! */
		set_fvpr_and_below(stream->text_last);
	    }
	}

    }
    return changed;
}

/*****************************************************************************/

extern void fvpr_forget( rid_text_stream *stream )
{
    if ( gbf_active( GBF_FVPR ) )
    {
        rid_text_item *ti;

        for ( ti = stream->text_list; ti; ti = rid_scanf(ti) )
            forget_fvpr_and_below(ti);

        stream->text_fvpr = NULL;
    }
}

/* eof fvpr.c */

