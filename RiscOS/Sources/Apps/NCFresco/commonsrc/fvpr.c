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


/*****************************************************************************

  Supplied with the TOP LEVEL stream.

  Attempts to advance pos_fvpr and text_fvpr, setting rid_flag_FVPR as
  it goes.

  Returns TRUE if the fvpr values changes.


  */

extern BOOL fvpr_progress_stream(rid_text_stream *stream)
{
    BOOL changed = FALSE;
    rid_text_item *ti;

    if (gbf_active(GBF_FVPR))
    {
	RENDBG(("fvpr_progress_stream(%p)\n", stream));
	
	if (stream == NULL || stream->text_list == NULL || stream->pos_list == NULL)
	{
	    RENDBG(("SOMETHING STILL NULL\n"));
	    return FALSE;
	}
	
	if (stream->text_fvpr == NULL)
	    stream->text_fvpr = stream->text_list;
	
	ti = stream->text_fvpr;
	
	RENDBG(("fvpr: start tracing from item %p\n", ti));
	
	while (1)
	{
	    BOOL stop = FALSE;
	    rid_pos_item *pi = ti->line;
	    
	    if (pi == NULL || pi->next == NULL || pi->floats != NULL)
	    {
		RENDBG(("fvpr: stoppping for pos_item reasons\n"));
		break;
	    }
	    
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
		
		if (im->im == NULL)
		{
		    RENDBG(("fvpr: no im field for image\n"));
		    stop = TRUE;
		    break;
		}
		
		if ( image_info( (image) im->im, NULL, NULL, NULL, &flags, NULL, NULL) != NULL )
		{
		    RENDBG(("fvpr: error from image_info - delay until end of page\n"));
		    stop = TRUE;
		    break;
		}
		
		if ( (flags & IMAGE_RENDERABLE_FLAGS) == 0 )
		{
		    RENDBG(("fvpr: not got realthing flag yet\n"));
		    stop = TRUE;
		    break;
		}
		
		if (im->ww == -1 || im->hh == -1)
		{
		    RENDBG(("fvpr: ww or hh still -1\n"));
		    stop = TRUE;
		    break;
		}
		
	    }
	    break;
	    case rid_tag_INPUT:
		break;
	    case rid_tag_TEXTAREA:
	    break;
	    case rid_tag_SELECT:
		break;
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
		    RENDBG(("fvpr: table completed - scanning beyond it\n"));
		}
	    }
	    break;
	    case rid_tag_OBJECT:
		break;
	    }
	    
	    if (stop)
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

/*****************************************************************************/

extern BOOL fvpr_progress_stream_flush(rid_text_stream *stream)
{
    BOOL changed = FALSE;

    if (gbf_active(GBF_FVPR))
    {
	if (stream->text_fvpr == NULL)
	    stream->text_fvpr = stream->text_list;
	
	RENDBG(("fvpr: flushing from %p to %p", stream->text_fvpr, stream->text_last));
	
	if (stream->text_fvpr != NULL)
	{
	    if (stream->text_fvpr != stream->text_last)
	    {
		rid_text_item *ti;
		
		changed = TRUE;
		
		for (ti = stream->text_fvpr; ti != stream->text_last; ti = rid_scanf(ti))
		{
		    TASSERT(ti->line != NULL);
		    set_fvpr_and_below(ti);
		}
		
		/* pdh: last one too! */
		set_fvpr_and_below(stream->text_last);
	    }
	}
	
    }
    return changed;
}


/* eof fvpr.c */
