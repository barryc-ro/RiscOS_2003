/* -*-c-*- */


/* otable.c */

/* Methods for table objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "drawftypes.h"
#include "wimp.h"
#include "font.h"
#include "bbc.h"

#include "interface.h"
#include "antweb.h"
#include "rid.h"
#include "webfonts.h"
#include "consts.h"
#include "config.h"
#include "util.h"
#include "render.h"
#include "rcolours.h"
#include "tables.h"

#include "object.h"
#include "version.h"
#include "dfsupport.h"

#include "stream.h"
#include "unwind.h"
#include "gbf.h"

#define XF              2       /* or frontend_dx */
#define YF              2       /* or frontend_dy */

/* This just propogates the size to items below. */
/* Can't rely upon pos list being valid */
/* Can't get real size for table until have some width information */
/* for our surroundings. */

void otable_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    if (gbf_active(GBF_NEW_FORMATTER))
    {
	TABDBG(("otable_size: new formatter so not doing anything\n"));
    }
    else
    {
        rid_table_item *table = ((rid_text_item_table *)ti)->table;
        rid_table_caption *capt = table->caption;
        rid_table_cell *cell;
        int x = -1, y = 0;

	TABDBG(("otable_size(%p, %p, %p)\n", ti, rh, doc));

        if ( capt != NULL )
        {
                for (ti = capt->stream.text_list; ti != NULL; ti = rid_scanf(ti) )
                {
                        if ( object_table[ti->tag].size != NULL )
                        	(object_table[ti->tag].size)(ti, doc->rh, doc);
                }
        }

        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL)
        {
	    if (cell->stream.text_list == NULL)
		continue;

	    for (ti = cell->stream.text_list; ti != NULL; ti = rid_scanf(ti) )
	    {
		if ( object_table[ti->tag].size != NULL )
		{
		    /*TABDBGN( ("Calling size for type %d at %p\n", ti->tag, ti) );*/
		    (object_table[ti->tag].size)(ti, doc->rh, doc);
		    /*TABDBGN( ("Size returned fine\n") );*/
		}
	    }
        }
        TABDBG(("otable_size done, caller = %s\n", caller(1) ) );
    }
}

/* This is the main function to render a stream of text, it is called
 * from the backend and from the code for rendering tables to render
 * the caption and each cell. */

#ifndef BUILDERS
static int stream_first_visable_cell(rid_table_item *table, int ox, int oy,
				     wimp_box *g, int *xx, int *yy)
{
	int x,y;
	rid_table_cell *cell;

	for (y=0; y < table->cells.y; y++)
	{
	    /* YF is to encompass the bottom cellspacing line, if any */
	    if ((oy + table->rowhdrs[y]->offy - table->rowhdrs[y]->sizey - YF) < g->y1)
		break;
	}

	if (y == table->cells.y)
		return FALSE;

        *yy = y;
	*xx = x = 0;

	for(x=0; x < table->cells.x; x++)
	{
		cell = *CELLFOR(table, x, y);
		if (cell && (cell->cell.y < *yy))
		{
			*yy = cell->cell.y;
			*xx = cell->cell.x;
		}
	}

	return TRUE;
}
#endif /* BUILDERS */

void otable_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc,
		   int hpos, int bline, object_font_state *fs,
		   wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    int top, bot, left, right;
    rid_table_item *table = ((rid_text_item_table*)ti)->table;
    rid_table_caption *capt = table->caption;
    rid_table_cell *cell;
    int x = -1, y = 0;
    const int border = table->border;
    const capt_size = capt == NULL ? 0 : -capt->stream.height +
	2 * (table->border + table->cellspacing + table->cellpadding);
    int oy_top, oy_bot;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    ox = hpos;
    oy = bline + ti->max_up;
    oy_top = oy;
    oy_bot = oy - table->size.y;

    if (capt)
    {
	/* If caption not bottom align, only support it being top aligned */
	if (capt->calign == rid_ct_BOTTOM)
	    oy_bot += capt_size;
	else
	    oy_top -= capt_size;
    }

    top = g->y1 - oy;
    bot = g->y0 - oy;
    left = g->x0;
    right = g->x1;

  if (update != object_redraw_HIGHLIGHT)
  {
    
    RENDBGN(("Rendering table.  ox=%d, oy=%d. size = %dx%d\n", ox, oy, table->size.x, table->size.y));
    RENDBGN(("Top work area = %d, bottom = %d\n", top, bot));
    RENDBGN(("Left = %d, right = %d, capt_size = %d\n", left, right, capt_size));

    if (/*table->border > 0 &&*/ table->frame != HTML_TABLE_FRAME_VOID)
    {
	const fr = table->frame;
	const int span = -(oy_bot - oy_top);

	TABDBGN(("Some table border needs drawing\n"));

	render_set_colour(render_colour_PLAIN, doc);

	if (fr == HTML_TABLE_FRAME_ABOVE || fr == HTML_TABLE_FRAME_HSIDES || fr == HTML_TABLE_FRAME_BORDER)
	{
	    bbc_rectanglefill(ox, oy_top - border, table->size.x - XF, border - YF);
	}

	if (fr == HTML_TABLE_FRAME_BELOW || fr == HTML_TABLE_FRAME_HSIDES || fr == HTML_TABLE_FRAME_BORDER)
	{
	    bbc_rectanglefill(ox, oy_bot, table->size.x - XF, border - YF);
	}

	if (fr == HTML_TABLE_FRAME_LHS   || fr == HTML_TABLE_FRAME_VSIDES || fr == HTML_TABLE_FRAME_BORDER)
	{
	    bbc_rectanglefill(ox, oy_bot, border - XF, span - YF);
	}

	if (fr == HTML_TABLE_FRAME_RHS   || fr == HTML_TABLE_FRAME_VSIDES || fr == HTML_TABLE_FRAME_BORDER)
	{
	    bbc_rectanglefill(ox + table->size.x - border, oy_bot, border - XF, span - YF);
	}
    }

    /* Then any cell borders - all or nothing exercise */
    /* We draw a line around the inside of the cell border */
    /* This is the only place should need to decompose how */
    /* we arrived at the row/col border values. If we're drawing */
    /* a line, cellspacing will have room for it, 'cause we force this. */

    if (table->cellspacing > 0 && table->rules != HTML_TABLE_RULES_NONE)
    {
	if (stream_first_visable_cell(table, ox, oy, g, &x, &y))
	{
	    for (x--; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	    {
		if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
		     (table->rowhdrs[y]->flags & rid_rhf_TFOOT) )
		    continue;

		if (cell->stream.text_list)
		{
		    int dx, dy, width;

		    dy = table->rowhdrs[cell->cell.y]->offy;

		    if ((oy + dy) < g->y0)
		    {
			break;
		    }
		    else
		    {
                        RENDBG(("Cell size %dx%d\n", cell->size.x, cell->size.y));

			dx = table->colhdrs[cell->cell.x]->offx;
			width = cell->size.x;

			/* XF is to account for RHS cellspacing line */
			if (((ox + dx) < g->x1) &&	/* Stream.left to left of clip.right */
			    ((ox + dx + width + XF) > g->x0) )
			{
			    const int smxf = table->cellspacing - XF;

			    BOOL dt = TRUE, db = TRUE, dl = TRUE, dr = TRUE;
			    int ct = render_colour_PLAIN,
				cb = render_colour_PLAIN,
				cl = render_colour_PLAIN,
				cr = render_colour_PLAIN;

			    switch (table->rules)
			    {
			    case HTML_TABLE_RULES_ROWS:
				dl = dr = FALSE;
				break;

			    case HTML_TABLE_RULES_COLS:
				dt = db = FALSE;
				break;

			    case HTML_TABLE_RULES_ALL:
				/* @@@@ This test needs altering if table cells can have */
				/* background colours or images */
#if 0
				if ( (rh->bgt & (rid_bgt_COLOURS | rid_bgt_IMAGE | rid_bgt_FCOL)) == 0 )
				{
				    /* Do 3D shading iff doing all sides and no background image */
				    /* or colour to make this less visible */
				    ct = cl = render_colour_LINE_D;
				    cr = cb = render_colour_LINE_L;
				}
#endif
				break;

			    case HTML_TABLE_RULES_GROUPS:
				dl = (table->colhdrs[x]->flags & rid_chf_GROUP_LEFT ) != 0;
				dr = (table->colhdrs[x]->flags & rid_chf_GROUP_RIGHT) != 0;
				dt = (table->rowhdrs[y]->flags & rid_rhf_GROUP_ABOVE) != 0;
				db = (table->rowhdrs[y]->flags & rid_rhf_GROUP_BELOW) != 0;
				break;
			    }

			    if (dt)
			    {
				render_set_colour(ct, doc);
				/* top left */
				bbc_move(ox + table->colhdrs[x]->offx + smxf,
					 oy + table->rowhdrs[y]->offy - smxf - YF);
				/* rightwards */
				bbc_drawby(cell->size.x - smxf, 0);
			    }

			    if (db)
			    {
				render_set_colour(cb, doc);
				/* bottom left */
				bbc_move(ox + table->colhdrs[x]->offx + smxf,
					 oy + table->rowhdrs[y]->offy - cell->size.y - YF);
				/* rightwards */
				bbc_drawby(cell->size.x - smxf, 0);
			    }

			    if (dl)
			    {
				render_set_colour(cl, doc);
				/* bottom left */
				bbc_move(ox + table->colhdrs[x]->offx + smxf,
					 oy + table->rowhdrs[y]->offy - cell->size.y - YF);
				/* upwards */
				bbc_drawby(0, cell->size.y - smxf);
			    }

			    if (dr)
			    {
				render_set_colour(cr, doc);
				/* bottom right */
				bbc_move(ox + table->colhdrs[x]->offx + cell->size.x,
					 oy + table->rowhdrs[y]->offy - cell->size.y - YF);
				/* upwards */
				bbc_drawby(0, cell->size.y - smxf);
			    }
			}
		    }
	 	}
	    }
	}
    }

    /* Coloured backgrounds for cells, if necessary */

    if ((table->flags & rid_tf_BGCOLOR) && (doc->flags & doc_flag_DOC_COLOURS))
    {
	TABDBG(("Have table background colours\n"));

	if (stream_first_visable_cell(table, ox, oy, g, &x, &y))
	{
	    for (x--; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	    {
		if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
		     (table->rowhdrs[y]->flags & rid_rhf_TFOOT) )
		    continue;

		if (cell->flags & rid_cf_BACKGROUND)
		{
		    int dx, dy, width;

		    dy = table->rowhdrs[cell->cell.y]->offy;

		    if ((oy + dy) < g->y0)
		    {
			break;
		    }
		    else
		    {
			dx = table->colhdrs[cell->cell.x]->offx;
			width = cell->size.x;

			/* XF is to account for RHS cellspacing line */
			if (((ox + dx) < g->x1) &&	/* Stream.left to left of clip.right */
			    ((ox + dx + width + XF) > g->x0) )
			{
			    int i;

			    rid_getprop(table, x, y, rid_PROP_BGCOLOR, &i);
			    render_cell_background_colour(i);

			    TABDBGN(("Filling cell background to %x\n", i));

			    bbc_rectanglefill(ox + table->colhdrs[x]->offx + table->cellspacing,
					      oy + table->rowhdrs[y]->offy - cell->size.y,
					      cell->size.x - table->cellspacing - XF,
					      cell->size.y - table->cellspacing - XF);

			}
		    }
	 	}
	    }
	}
    }
  } /* update != 2 */

    /* Any caption */

    if ( capt != NULL )
    {
	int dx, dy;

	table_caption_stream_origin(table, &dx, &dy);

	stream_render(&capt->stream, doc,
		      ox + dx, oy + dy,
		      left, top - dy,
		      right, bot - dy,
		      fs,
		      g, update );
    }

    /* Fill in cell contents */

    if (stream_first_visable_cell(table, ox, oy, g, &x, &y))
    {
        for (x--; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
        {
	    int dx, dy;

	    if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
		 (table->rowhdrs[y]->flags & rid_rhf_TFOOT) )
		continue;

	    TABDBGN(("otable_redraw()\n"));
	    table_cell_stream_origin(table, cell, &dx, &dy);

	    /* Note that we must terminate the search only when the whole cell falls below the clip */
	    if ((oy + table->rowhdrs[cell->cell.y]->offy) < g->y0)
	    {
		break;
	    }
	    else if (((ox + dx) < g->x1) &&	/* Stream.left to left of clip.right */
		     ((ox + dx + cell->stream.fwidth) > g->x0) ) /* Stream.right to right of clip.left */
	    {
	        /* pdh: not sure I want to put my name to this horrible cheat
	         * to get background colours right. It works though.
	         */
	        int bgcol = cell->stream.bgcolour;
	        BOOL swapped = FALSE;
	        int oldbgt = 0;

	        if ( bgcol )
	        {
	            bgcol = doc->rh->colours.back;
	            doc->rh->colours.back = cell->stream.bgcolour & ~1;
	            oldbgt = doc->rh->bgt;
	            doc->rh->bgt |= rid_bgt_COLOURS;
	            swapped = TRUE;
	        }

		stream_render(&cell->stream, doc,
			      ox + dx, oy + dy,
			      left, top - dy,
			      right, bot - dy,
			      fs,
			      g, update );

                if ( swapped )
                {
                    doc->rh->colours.back = bgcol;
                    doc->rh->bgt = oldbgt;
                }
	    }
        }
    }
#endif /* BUILDERS */
}

void otable_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
        rid_table_item *table = ((rid_text_item_table *)ti)->table;
        rid_table_caption *capt = table->caption;
        rid_table_cell *cell;
        int x = -1, y = 0;

        if ( capt != NULL )
        {
                for (ti = capt->stream.text_list; ti != NULL; ti = rid_scanf(ti) )
                {
                        if ( object_table[ti->tag].dispose != NULL )
                        	(object_table[ti->tag].dispose)(ti, doc->rh, doc);
                }
        }

        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL)
        {
                if (cell->stream.text_list == NULL)
                        continue;
                for (ti = cell->stream.text_list; ti != NULL; ti = rid_scanf(ti) )
                {
                        if ( object_table[ti->tag].dispose != NULL )
                             	(object_table[ti->tag].dispose)(ti, doc->rh, doc);
                }
        }
}

/* There is no need for an otable_click() function */




/* this is a simple rendering of tables - horizontal adjacency is lost */

void otable_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
        rid_table_item *table = ((rid_text_item_table *)ti)->table;
        rid_table_caption *capt = table->caption;
        rid_table_cell *cell;
        int x = -1, y = 0;

        if ( capt != NULL )
        {
	stream_write_as_text(rh, &(capt->stream), f);
        }

        while ( (cell = rid_next_root_cell(table, &x, &y)) != NULL)
        {
	    /* Don't print incomplete footer cells in the wrong place */
	    if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
		 (table->rowhdrs[y]->flags & rid_rhf_TFOOT) )
		continue;

	    stream_write_as_text(rh, &(cell->stream), f);
        }
}

/* Don't know if I can recurse like this */
/* No table border frame yet */
void otable_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
#ifndef BUILDERS
    rid_table_item *table = ((rid_text_item_table *)ti)->table;
    rid_table_caption *capt = table->caption;
    rid_table_cell *cell;
    wimp_box tb;
    int xx = -1, yy = 0;
    int dx, dy;
    int sbase;
    char buffer[12];

    y += table->size.y;

    if ( capt != NULL && capt->calign != rid_ct_BOTTOM )
    {
	table_caption_stream_origin(table, &dx, &dy);

	sbase = df_write_box(fh, "Caption", fileoff);

	stream_write_as_drawfile(doc, &(capt->stream), fh, fileoff, x + dx, y + dy);

	tb.x0 = x+dx;
	tb.y1 = y+dy;

	tb.x1 = tb.x0 + capt->stream.fwidth;
	tb.y0 = tb.y1 + capt->stream.height;

	df_write_box_fix(fh, sbase, &tb, fileoff);

	df_stretch_bb(bb, &tb);
    }

    while ( (cell = rid_next_root_cell(table, &xx, &yy)) != NULL)
    {
	/* Don't render to draw incomplete footer cells */
	if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
	     (table->rowhdrs[yy]->flags & rid_rhf_TFOOT) )
	    continue;

	if (cell->stream.text_list == NULL)
	    continue;

	table_cell_stream_origin(table, cell, &dx, &dy);

	sprintf(buffer, "%d,%d", xx, yy);
	sbase = df_write_box(fh, buffer, fileoff);

	/* If we ever support cell backgrounds we should output it here */

	stream_write_as_drawfile(doc, &(cell->stream), fh, fileoff, x + dx, y + dy);

	tb.x0 = x+dx;
	tb.y1 = y+dy;

	tb.x1 = tb.x0 + cell->stream.fwidth;
	tb.y0 = tb.y1 + cell->stream.height;

	df_write_box_fix(fh, sbase, &tb, fileoff);

	df_stretch_bb(bb, &tb);
    }

    if ( capt != NULL && capt->calign == rid_ct_BOTTOM )
    {
	table_caption_stream_origin(table, &dx, &dy);

	sbase = df_write_box(fh, "Caption", fileoff);

	stream_write_as_drawfile(doc, &(capt->stream), fh, fileoff, x + dx, y + dy);

	tb.x0 = x+dx;
	tb.y1 = y+dy;

	tb.x1 = tb.x0 + capt->stream.fwidth;
	tb.y0 = tb.y1 + capt->stream.height;

	df_write_box_fix(fh, sbase, &tb, fileoff);

	df_stretch_bb(bb, &tb);
    }

#endif /* BUILDERS */
}
