/* -*-c-*- */

/* stream.c */

/* Functions to operate on a stream of rid_text_items */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "memwatch.h"

#include "wimp.h"
#include "msgs.h"
#include "swis.h"
#include "bbc.h"
#include "visdelay.h"

#include "rid.h"
#include "antweb.h"
#include "webfonts.h"
#include "images.h"
#include "tables.h"
#include "indent.h"

#include "object.h"
#include "config.h"

#include "interface.h"
#include "render.h"

#include "drawfile.h"
#include "dfsupport.h"
#include "layout.h"

#include "myassert.h"

#include "stream.h"
#include "debug.h"

/* Setting to 1 independent of DEBUG is *bad* */
#if 0
#define LOCATEDBG(X)       DBG(X)
#else
#define LOCATEDBG(X)
#endif

#if 0 /*not called?*/
void stream_iterate(rid_text_stream *st, stream_callback_fn cbf, void *params)
{
    rid_text_item *ti;

    for (ti = st->text_list; ti; ti = rid_scanf(ti) )
    {
	cbf(st, ti, params);
    }
}
#endif

static void stream_table_locate_item(rid_table_item *table, int *xp, int *yp, be_item *ti)
{
    int x, y, tx = *xp, ty = *yp;
    be_item my_ti = NULL;
    /*        rid_table_cell *cell;*/

    LOCATEDBG((stderr, "Locating item in table %p, offset %d,%d\n", table, tx, ty));

    ty -= table->cellspacing + table->cellpadding;

    if (ty < 0)
    {
	LOCATEDBG((stderr, "Below the bottom border\n"));
	return;
    }

    y = table->cells.y - 1;
    while (y >= 0)
    {
	ty -= table->rowhdrs[y]->sizey;

	if (ty < 0)
	    break;
	y--;
    }


    if (ty < 0)
    {
	LOCATEDBG((stderr, "Matched vertically within row %d\n", y));

	tx -= table->cellspacing + table->cellpadding;

	if (tx < 0)
	{
	    LOCATEDBG((stderr, "In the left border\n"));
	    return;
	}

	x = 0;
	while (x < table->cells.x)
	{
	    tx -= table->colhdrs[x]->sizex;

	    if (tx < 0)
		break;
	    x++;
	}

	if (tx < 0 && y >= 0)
	{
	    rid_table_cell *cell = *CELLFOR(table, x, y);

	    LOCATEDBG((stderr, "Matched horizontally within cell %d,%d - %s\n", x, y, cell ? "Occupied" : "No cell"));
	    if (cell != NULL)
	    {
		/* Find the offset within the table */
		TABDBG(("stream_table_locate_item() 3\n"));
		table_cell_stream_origin(table, cell, &tx, &ty);
		tx = *xp - tx;
		/* Y offset is from top left, *yp is from bottom left */
		ty = (*yp - table->size.y) - ty;

		stream_find_item_at_location(&cell->stream, &tx, &ty, &my_ti);

		if (my_ti != NULL)
		{
		    LOCATEDBG((stderr, "Returning matched child item\n"));
		    *xp = tx;
		    *yp = ty;
		    *ti = my_ti;
		    return;
		}
	    }
	}

	LOCATEDBG((stderr, "Either beyond right border or not over an item\n"));
	return;
    }

    LOCATEDBG((stderr, "Trying to match within caption\n"));

    /* @@@@ this is probably wrong */
    if (table->caption != NULL)
    {
	ty = (*yp - table->size.y);
	stream_find_item_at_location(&table->caption->stream, &tx, &ty, &my_ti);

	if (my_ti != NULL)
	{
	    LOCATEDBG((stderr, "Matched in the caption\n"));
	    *xp = tx;
	    *yp = ty;
	    *ti = my_ti;
	    return;
	}
    }

    LOCATEDBG((stderr, "Failed to match any item - just returning the table\n"));

    return;
}

os_error *stream_find_item_at_location(rid_text_stream *st, int *x, int *y, be_item *ti)
{
    rid_pos_item *pi;
    rid_text_item *ti2 = NULL;	/* Keep dumb compiler quiet */
    int hpos;

    *ti = NULL;

    LOCATEDBG((stderr, "Locating item at %d,%d in stream %p\n", *x, *y, st));

    if (st == NULL || st->pos_list == NULL)
        return NULL;

    /* Above the top ? */
    if (st->pos_list->top <= *y)
       return NULL;

    for (pi = st->pos_list; pi->next && pi->next->top > *y; pi = pi->next)
       ;

    LOCATEDBG((stderr, "Found pos item %p, top at %d\n", pi, pi->top));

    /* A position way off to the left is taken as just wanting what is first on the line */
    if ( *x < -100)
    {
	*ti = pi->first;
    }
    else if (pi->next == NULL)
    {
	LOCATEDBG((stderr, "No items\n"));
    }
    else
    {
	rid_text_item *float_l, *float_r;
	rid_float_item *rfi;
	int found = FALSE;

	hpos = pi->left_margin;

	float_l = float_r = NULL;

	if (pi->floats && *x < pi->left_margin)
	{
	    rfi = pi->floats->left;

	    while ( rfi && !found )
	    {
	        if ( *x >= rfi->entry_margin
	             && *x < (rfi->entry_margin + rfi->ti->width) )
	        {
	            *x -= rfi->entry_margin;
	            ti2 = rfi->ti;
	            *y -= (ti2->line->top - ti2->max_up);
	            found = TRUE;
	        }
	        rfi = rfi->next;
	    }

	    if (pi->floats->left)
	    {
		float_l = pi->floats->left->ti;
		/* pdh: don't add this to left_margin as that includes floater
		 * width these days
		hpos += float_l->width;
		 */
	    }
	    if (pi->floats->right)
		float_r = pi->floats->right->ti;
	}
	else if ( pi->floats && *x > pi->floats->right_margin )
	{
	    rfi = pi->floats->right;

	    while ( rfi && !found )
	    {
	        if ( *x < rfi->entry_margin
	             && *x >= (rfi->entry_margin - rfi->ti->width ) )
	        {
	            *x -= (rfi->entry_margin - rfi->ti->width);
	            ti2 = rfi->ti;
	            *y -= (ti2->line->top - ti2->max_up);
	            found = TRUE;
	        }
	        rfi = rfi->next;
	    }
	}
	else
	{
	    if (*x < pi->left_margin)
	    {
		LOCATEDBG((stderr, "Pointer to the left of all items\n"));
	    }
	    else
	    {
		for( ti2 = pi->first; ti2; ti2 = ti2->next )
		{
		    int ohpos = hpos;

                    if ( FLOATING_ITEM(ti2) )
                        continue;

                    if ( ti2->line != pi )
                        break;

		    if ((ti2->flag & (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS)) == 0)
		    {
			hpos += ti2->width;
			hpos += ti2->pad;
			hpos += pi->leading;

			if (*x < hpos || ti2->width == MAGIC_WIDTH_HR)
			{
			    *y -= (pi->top - pi->max_up);         /* Relative to baseline */
			    *x -= ohpos ;
			    found = TRUE;
			    break;
			}
		    }
		}
	    }
	}

	if (found)
	{
	    LOCATEDBG((stderr, "Found item\n"));

	    *ti = ti2;

	    if (ti2->tag == rid_tag_TABLE)
	    {
		stream_table_locate_item( ((rid_text_item_table *)ti2)->table, x, y, ti);
	    }
	}
    }

    LOCATEDBG((stderr, "Locate pointer done: %p at %d, %d\n", *ti, *x, *y));

    return NULL;
}

/* Find the horizonlat offset (xx) and base line (yy) of a given item */

BOOL stream_find_item_location(be_item ti, int *xx, int *yy)
{
    rid_pos_item *pi = ti->line;
    rid_text_item *ti2;
    int hpos;
    int found = FALSE;

    /* Until we are formatted the item has no location */
    if (pi == NULL)
        return FALSE;

    hpos=pi->left_margin;

    if (pi->floats)
    {
	rid_float_item *fi;

	LOCATEDBG((stderr, "Have floats\n"));

        for ( fi = pi->floats->left; fi; fi = fi->next )
        {
            if ( ti == fi->ti )
            {
                hpos = fi->entry_margin;
                found = TRUE;
                break;
            }
        }

        if ( !found )
            for ( fi = pi->floats->right; fi; fi = fi->next )
            {
                if ( ti == fi->ti )
                {
                    hpos = fi->entry_margin - ti->width;
                    found = TRUE;
                }
            }

        if ( found )
                *yy = pi->top - ti->max_up;
    }

    if ( !found )
    {
        *yy = pi->top - pi->max_up;

	for (ti2 = pi->first;
	     ti2 && ti2 != pi->next->first;
	     ti2 = rid_scanf(ti2) )
	{
	    if (ti2 == ti)
	    {
		found=TRUE;
		break;
	    }

	    if ((ti2->flag & (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS)) == 0)
	    {
		hpos += ti2->width;
		hpos += ti2->pad;
		hpos += pi->leading;
	    }
	}
    }

    if (found)
    {
	*xx = hpos;
	/*if (ti->line->st->parent)*/
	if (ti->line->st->partype != rid_pt_HEADER)
	{
	    int px, py;
	    rid_text_item *par_table_ti;
	    rid_table_item *tabi;

	    if (ti->line->st->partype == rid_pt_CAPTION)
	    {
		rid_table_caption *capt = (rid_table_caption *) ti->line->st->parent;

		tabi = capt->table;

		/* Adjust offset to be within table */
		TABDBG(("stream_find_item_location() 2\n"));
		table_caption_stream_origin(tabi, &px, &py);
		*xx += px;
		*yy += py;
	    }
	    else
	    {
		rid_table_cell *cell = (rid_table_cell *) ti->line->st->parent;

		TASSERT(ti != NULL);
		TASSERT(ti->line != NULL);
		TASSERT(ti->line->st != NULL);
		TASSERT(ti->line->st->parent != NULL);
		TASSERT(ti->line->st->partype == rid_pt_CELL);
		tabi = cell->parent;

		/* Adjust offset to be within table */
		TABDBG(("stream_find_item_location() 1\n"));
		table_cell_stream_origin(tabi, cell, &px, &py);
		*xx += px;
		*yy += py;
	    }

	    par_table_ti = &(tabi->parent->base);

	    /* Adjust offset to be within work area */
	    stream_find_item_location(par_table_ti ,&px, &py);

	    *xx += px;
	    *yy += py;
	    *yy += tabi->size.y;
	}
	return TRUE;
    }

    return FALSE;
}

/* This is the main function to render a stream of text, it is called
 * from the backend and from the code for rendering tables to render
 * the caption and each cell. */

#if DEBUG
extern char *item_names[];
#endif

void stream_render(rid_text_stream *stream, antweb_doc *doc,
		   const int ox, const int oy, /* screen origin of text stream */
		   const int left, const int top, /* Area being redrawn in local coords */
		   const int right, const int bot,
		   object_font_state *fs, wimp_box *g, const int update )
{
    rid_pos_item *pi, *firstpi;
    rid_text_item *ti;

    if (stream == NULL || stream->pos_list == NULL)
	return;

    RENDBG(("st%p: rendering rectangle.  ox=%d, oy=%d.\n", stream, ox, oy));
    RENDBG(("Top work area = %d, bottom = %d\n", top, bot));
    RENDBG(("Left = %d, right = %d\n", left, right));
    RENDBG(("Scanning down\n"));

    /* pdh: test that used to be here didn't DTRT for DL COMPACT, which ends
     * up using two pos items on the same line
     */
    for (pi = stream->pos_list; pi->next && (pi->top-pi->max_up-pi->max_down) > top; pi = pi->next)
    {
	RENDBGN(("Skipping line, top=%d\n", pi->top));
    }

    firstpi = pi;

    for( ; pi && pi->top > bot; pi = pi->next)
    {
	int hpos, bline;
	rid_float_item *rfi;

	RENDBGN(("Got line in range, top=%d\n", pi->top));

        hpos = ox;
	bline = pi->top - pi->max_up + oy;

	if (pi->floats)
	{
	    rfi = pi->floats->left;
	    while ( rfi )
	    {
		if ( pi == firstpi || rfi->pi == pi )
		{
		    (object_table[rfi->ti->tag].redraw)
			(rfi->ti, doc->rh, doc,
			 ox + rfi->entry_margin,
			 rfi->pi->top - rfi->ti->max_up + oy,
			 fs, g, ox, oy, update);
		}
		rfi = rfi->next;
	    }

            rfi = pi->floats->right;
            while ( rfi )
	    {
		if ( pi == firstpi || rfi->pi == pi )
		{
		    (object_table[rfi->ti->tag].redraw)
			(rfi->ti, doc->rh, doc,
			 ox + rfi->entry_margin - rfi->ti->width,
			 rfi->pi->top - rfi->ti->max_up + oy,
			 fs, g, ox, oy, update);
		}
		rfi = rfi->next;
	    }

            /* pdh: left_margin now includes float width
	    if (float_l)
		hpos += float_l->width;
             */
	}

	hpos = ox + pi->left_margin;

	RENDBGN(("Base line at %d, hpos starts at %d\n", bline, hpos));

        /* pdh: fiddled a bit to look like the New Loop Style (see
         * html/formatter.html)
         */

	for (ti = pi->first; ti; ti = rid_scanf(ti) )
	{
	    RENDBGN(("%s item at %p hpos=%d, width=%d\n", item_names[ti->tag], ti, hpos, ti->width));

            if ( FLOATING_ITEM(ti) )
                continue;

	    if ( hpos >= right || ti->line != pi )
		break;

	    if ((ti->flag & rid_flag_CLEARING) ||
		(ti->flag & (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS)) == 0)
	    {
		if ((ti->width == MAGIC_WIDTH_HR) || ((hpos + ti->width + ti->pad) >= left) )
		{
		    RENDBGN(("Rendering item at %d,%d\n", hpos, bline));

                    if ( ti->tag == rid_tag_TABLE )
                        RENDBGN(("st%p: rendering table %p at %d,%d pos=%p\n",
                                stream, ti, hpos, bline, pi ));

		    (object_table[ti->tag].redraw)(ti, doc->rh, doc, hpos, bline,
						   fs, g, ox, oy, update);
		}

		hpos += ti->width;
		hpos += ti->pad;
		hpos += pi->leading;
	    }
	}
    }
}

/* We need to be able to render a stream as a drawfile to save the
 * whole document and also to save a cell from a table */

void stream_write_as_drawfile(be_doc doc, rid_text_stream *stream,
			     int fh, int *writepoint, int ox, int oy)
{
    int do_pc = (stream->parent == NULL);
    rid_pos_item *pi;
    rid_text_item *ti;
    rid_text_item *float_l, *float_r;
    int dwidth;
    wimp_paletteword trans;

    trans.word = (unsigned int)~0;

    dwidth = (stream->fwidth > stream->widest) ? stream->fwidth : stream->widest;

    float_l = float_r = NULL;

    /* For each line in the stream */
    for (pi = stream->pos_list; pi && pi->next; pi = pi->next)
    {
	int bline = oy + pi->top - pi->max_up;
	int hpos = ox + pi->left_margin;
	int line_base;
	char buffer[32];
	wimp_box bb, tb;

	if (do_pc)
	    visdelay_percent((pi->top * 100) / stream->height);

	if (pi->floats)
	{
	    if (pi->floats->left)
	    {
		if (pi->floats->left->ti != float_l)
		{
		    int at_y;

		    ti = float_l = pi->floats->left->ti;

		    at_y = pi->floats->left->pi->top - ti->max_up + oy;

		    if (object_table[ti->tag].asdraw)
			(object_table[ti->tag].asdraw)(ti, doc, fh, ox, at_y, writepoint, &bb);
		    else
		    {
			tb.x0 = ox;
			tb.y0 = at_y - ti->max_down;
			tb.x1 = ox   + ti->width;
			tb.y1 = at_y + ti->max_up;

			df_stretch_bb(&bb, &tb);

			df_write_filled_rect(fh, &tb, render_get_colour(render_colour_PLAIN, doc),
					     trans, writepoint);
		    }
		}
	    }
	    else
	    {
		float_l = NULL;
	    }

	    if (pi->floats->right)
	    {
		if (pi->floats->right->ti != float_r)
		{
		    int at_x, at_y;

		    ti = float_r = pi->floats->right->ti;

		    at_x = ox + pi->st->fwidth - ti->width;
		    at_y = pi->floats->right->pi->top - ti->max_up + oy;

		    if (object_table[ti->tag].asdraw)
			(object_table[ti->tag].asdraw)(ti, doc, fh, at_x, at_y, writepoint, &bb);
		    else
		    {
			tb.x0 = at_x;
			tb.y0 = at_y - ti->max_down;
			tb.x1 = at_x + ti->width;
			tb.y1 = at_y + ti->max_up;

			df_stretch_bb(&bb, &tb);

			df_write_filled_rect(fh, &tb, render_get_colour(render_colour_PLAIN, doc),
					     trans, writepoint);
		    }
		}
	    }
	    else
	    {
		float_r = NULL;
	    }

            /* pdh: left_margin include floater width now
	    if (float_l)
		hpos += float_l->width;
             */
	}

	bb.x0 = ox + dwidth;
	bb.y0 = bline + pi->max_up;
	bb.x1 = ox -1;
	bb.y1 = bline - pi->max_down;

	sprintf(buffer, "y=%d", bline);

        /* Start bounding object */
	line_base = df_write_box(fh, buffer, writepoint);

        /* For each item on the line */
	for (ti = pi->first; ti && ti != pi->next->first; ti = rid_scanf(ti) /*ti->next*/ )
	{
	    int w;

	    w = (ti->width < 0) ? stream->fwidth : ti->width;

	    if ((ti->flag & rid_flag_CLEARING) ||
		(ti->flag & (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS)) == 0)
	    {
		/* Output the item */
		if (object_table[ti->tag].asdraw)
		    (object_table[ti->tag].asdraw)(ti, doc, fh, hpos, bline, writepoint, &bb);
		else
		{
		    tb.x0 = hpos;
		    tb.y0 = bline - ti->max_down;
		    tb.x1 = hpos + w;
		    tb.y1 = bline + ti->max_up;

		    df_stretch_bb(&bb, &tb);

		    df_write_filled_rect(fh, &tb, render_get_colour(render_colour_PLAIN, doc), trans,
					 writepoint);
		}

		hpos += w + ti->pad + pi->leading;
	    }
	}

        /* Close off the line bounding object */
	df_write_box_fix(fh, line_base, &bb, writepoint);
    }

}

void stream_write_as_text(rid_header *rh, rid_text_stream *stream, FILE *f)
{
    rid_pos_item *pi;
    rid_text_item *ti;

    for(pi = stream->pos_list ; pi; pi = pi->next)
    {
	if (pi->first->st.indent)
	{
	    int i;
	    for (i=0; i < pi->first->st.indent; i++)
	    {
		fputc(' ', f);
	    }
	}

	for (ti = pi->first; ti && ti != pi->next->first; ti = rid_scanf(ti))
	{
	    (object_table[ti->tag].astext)(ti, rh, f);
	}
	fputc('\n', f);
    }
}
