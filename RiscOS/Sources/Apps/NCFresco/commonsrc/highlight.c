/* > highlight.c

 *

 * New routines for displaying a highlight on screen.
 
 */

#include "bbc.h"
#include "wimp.h"

#include "antweb.h"
#include "interface.h"
#include "object.h"
#include "stream.h"
#include "util.h"

#include "rcolours.h"
#include "render.h"

/* doc->selection contains details of the currently selected item or
 * anchor.  This routine should scan to get the total covered area and
 * draw a highlight around all of it. Except where the boxes are
 * discontiguous in which case it should separate boxes.
 */

#define BORDER_WIDTH	12
#define LINE_WIDTH	8

#if 0

#define HSPACE	4
#define VSPACE	4

void highlight_render_highlight(antweb_doc *doc, int ox, int oy, wimp_box *g)
{
    switch (doc->selection.tag)
    {
    case doc_selection_tag_TEXT:
    {
	int hpos, bline;

	if (stream_find_item_location(doc->selection.data.text.item, &hpos, &bline))
	{
	    render_set_colour(render_colour_HIGHLIGHT, doc);
	    render_item_outline(ti, hpos, bline);

	    bbc_rectangle(ox + hpos - HSPACE,
			  oy + bline - ti->max_down - VSPACE,
			  ti->width + 2*HSPACE - 1,
			  ti->max_down + ti->max_up + 2*VSPACE - 1);
	}

	break;
    }

    case doc_selection_tag_AREF:
	for (ti = item->aref->first; ti && ti->aref == item->aref; ti = rid_scanfr(ti))
	{
	    if (activate)
		ti->flag |= rid_flag_ACTIVATED;
	    else
		ti->flag &= ~rid_flag_ACTIVATED;
	    be_update_item_highlight(doc, ti);
	}
	break;

    case doc_selection_tag_AREA:
	break;
    }

}

typedef void (*highlight_line_fn)(int x, int y, int draw);

#define hl_MOVE_TO	0
#define hl_DRAW_TO	1

static void highlight_draw_line(int x, int y, int draw)
{
    switch (draw)
    {
    case hl_MOVE_TO:
	bbc_move(x, y);
	break;
    case hl_DRAW_TO:
	bbc_draw(x, y);
	break;
    }
}

static void highlight_redraw_line(int x, int y, int draw)
{
    switch (draw)
    {
    case hl_MOVE_TO:
	bbc_move(x, y);
	break;

    case hl_DRAW_TO:
    {
	wimp_box box;
	box.x0 = 
        frontend_view_update(doc->parent, &box, &backend_render_rectangle, doc, wont_plot_all);
	memset(&trim, 0, sizeof(trim));

	trim.x0 = ti->width - 4;
	antweb_update_item_trim(doc, ti, &trim, TRUE);
	break;
    }
    }
}

void highlight_generate_lines(antweb_doc *doc, int ox, int oy, wimp_box *g, highlight_line_fn process_line)
{
    switch (doc->selection.tag)
    {
    case doc_selection_tag_TEXT:
    {
	int hpos, bline;

	if (stream_find_item_location(doc->selection.data.text.item, &hpos, &bline))
	{
	    int x0, y0, x1, y1;

	    x0 = ox + hpos - HSPACE;
	    y0 = oy + bline - ti->max_down - VSPACE;
	    x1 = x0 + ti->width + 2*HSPACE - 1;
	    y1 = y0 + ti->max_down + ti->max_up + 2*VSPACE - 1;

	    process_line(x0, y0, hl_MOVE_TO);
	    process_line(x0, y1, hl_DRAW_TO);
	    process_line(x1, y1, hl_DRAW_TO);
	    process_line(x1, y0, hl_DRAW_TO);
	    process_line(x0, y0, hl_DRAW_TO);
	}
	break;
    }

    case doc_selection_tag_AREF:
	for (ti = item->aref->first; ti && ti->aref == item->aref; ti = rid_scanfr(ti))
	{
	    if (activate)
		ti->flag |= rid_flag_ACTIVATED;
	    else
		ti->flag &= ~rid_flag_ACTIVATED;
	    be_update_item_highlight(doc, ti);
	}
	break;

    case doc_selection_tag_AREA:
	break;
    }

}

#endif

void highlight_render_outline(be_item ti, antweb_doc *doc, int hpos, int bline)
{
    LNKDBG(("highlight_render_outline: doc %p ti %p %d,%d sel %d\n", doc, ti, hpos, bline, ti->flag & rid_flag_SELECTED ? 1 : 0));

    if (ti->flag & (rid_flag_SELECTED|rid_flag_ACTIVATED))
    {
	int x, y, w, h, i;

	render_set_colour(ti->flag & rid_flag_ACTIVATED ? render_colour_ACTIVATED : render_colour_HIGHLIGHT, doc);

	x = hpos - BORDER_WIDTH;
	y = bline - ti->max_down - BORDER_WIDTH;
	w = ti->width + BORDER_WIDTH*2;
	h = ti->max_up + ti->max_down + BORDER_WIDTH*2;
	
	for (i = 0; i < LINE_WIDTH; i+=2)
	    bbc_rectangle(x+i, y+i, w - i*2 -1, h - i*2 - 1);
    }
}

#if 0
#define do_redraw frontend_view_redraw(doc->parent, &trim)
#else
#define do_redraw frontend_view_update(doc->parent, &trim, &backend_render_rectangle, doc, TRUE)
#endif


static int render_border(wimp_redrawstr *rr, void *h, int update)
{
    antweb_doc *doc = h;

    highlight_render(rr, doc);

    return 0;
    NOT_USED(update);
}

void highlight_update_border(antweb_doc *doc, wimp_box *box, BOOL draw)
{
    if ((doc->flags & doc_flag_DISPLAYING) == 0)
        return;

    LNKDBG(("highlight_update_border: draw %d box %d,%d %d,%d\n", draw, box->x0, box->y0, box->x1, box->y1));
    if (draw)
    {
        frontend_view_update(doc->parent, box, render_border, doc, 0);
    }
    else
    {
	wimp_box trim = *box;
    
	trim.x1 = trim.x0 + LINE_WIDTH;
	do_redraw;
	trim.x1 = box->x1;

	trim.y1 = trim.y0 + LINE_WIDTH;
	do_redraw;
	trim.y1 = box->y1;

	trim.x0 = trim.x1 - LINE_WIDTH;
	do_redraw;
	trim.x0 = box->x0;

	trim.y0 = trim.y1 - LINE_WIDTH;
	do_redraw;
    }
}

void highlight_offset_border(wimp_box *box)
{
    box->x0 -= BORDER_WIDTH;
    box->y0 -= BORDER_WIDTH;
    box->x1 += BORDER_WIDTH + (frontend_dx-1);
    box->y1 += BORDER_WIDTH + (frontend_dy-1);
}

void highlight_render(wimp_redrawstr *rr, antweb_doc *doc)
{
    int ox, oy;
    int hpos, bline;
    object_font_state fs;
    be_item ti;

    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

#if USE_MARGINS
    ox += doc->margin.x0;
    oy += doc->margin.y1;
#endif

    LNKDBG(("highlight_render: doc %p tag %d\n", doc, doc->selection.tag));

    switch (doc->selection.tag)
    {
    case doc_selection_tag_NONE:
	break;

    case doc_selection_tag_TEXT:
	ti = doc->selection.data.text.item;
	
	if (object_table[ti->tag].redraw &&
	    stream_find_item_location(ti, &hpos, &bline))
	    object_table[ti->tag].redraw(ti, doc->rh, doc, ox + hpos, oy + bline, &fs, &rr->g, ox, oy, object_redraw_HIGHLIGHT);
	break;

    case doc_selection_tag_MAP:
	ti = doc->selection.data.map.item;
	
	if (object_table[ti->tag].redraw &&
	    stream_find_item_location(ti, &hpos, &bline))
	    object_table[ti->tag].redraw(ti, doc->rh, doc, ox + hpos, oy + bline, &fs, &rr->g, ox, oy, object_redraw_HIGHLIGHT);
	break;

    case doc_selection_tag_AREF:
	/* this may or may not be quicker that finding each item individually - I'm not sure */
	stream_render(&doc->rh->stream, doc,
		      ox, oy,
		      rr->g.x0, rr->g.y1 - oy,
		      rr->g.x1, rr->g.y0 - oy,
		      &fs, &rr->g, object_redraw_HIGHLIGHT);
	break;
    }
}

/* eof highlight.c */
