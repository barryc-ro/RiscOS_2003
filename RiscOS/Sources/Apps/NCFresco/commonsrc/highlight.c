/* > highlight.c

 *

 * New routines for displaying a highlight on screen.
 
 */


/* doc->selection contains details of the currently selected item or
 * anchor.  This routine should scan to get the total covered area and
 * draw a highlight around all of it. Except where the boxes are
 * discontiguous in which case it should separate boxes.
 */

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

/* eof highlight.c */
