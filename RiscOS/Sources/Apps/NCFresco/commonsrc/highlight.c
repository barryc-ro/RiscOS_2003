/* > highlight.c

 *

 * New routines for displaying a highlight on screen.
 
 */

#include <limits.h>

#include "bbc.h"
#include "wimp.h"

#include "antweb.h"
#include "interface.h"
#include "object.h"
#include "stream.h"
#include "util.h"
#include "gbf.h"
#include "config.h"

#include "rcolours.h"
#include "render.h"

/* ---------------------------------------------------------------------- */

/* bias values for text links */
#define TEXT_X0	12
#define TEXT_X1	12
#define TEXT_Y0	0
#define TEXT_Y1	6

#define IMAGE_SPACE	4

/* ---------------------------------------------------------------------- */

#if NEW_HL

typedef enum
{
    state_DOWN,
    state_RIGHT,
    state_UP,
    state_LEFT
} tracking_state;

/* ---------------------------------------------------------------------- */

static intxy point_array[256];
static int n_points = 0;

/* ---------------------------------------------------------------------- */

static void pointlist_add_point(antweb_selection_t *sel, int x, int y)
{
    point_array[n_points].x = x;
    point_array[n_points].y = y;
    n_points++;
}

static void pointlist_add_rectangle(antweb_selection_t *sel, wimp_box *box)
{
    pointlist_add_point(sel, box->x0, box->y0);
    pointlist_add_point(sel, box->x0, box->y1);
    pointlist_add_point(sel, box->x1, box->y0);
    pointlist_add_point(sel, box->x1, box->y1);
}

static int pointlist_find_next(const intxy *from, tracking_state state)
{
    const intxy *p;
    int best = -1;
    int best_prim_dist = INT_MAX;
    int best_sec_dist = INT_MAX;
    int i;
    
    for (i = 0, p = point_array; i < n_points; i++, p++)
    {
	int prim_dist = 0, sec_dist = 0;

	switch (state)
	{
	case state_DOWN:
	    prim_dist = from->y - p->y;
	    sec_dist = from->x - p->x;
	    break;

	case state_RIGHT:
	    prim_dist = p->x - from->x;
	    sec_dist = from->y - p->y;
	    break;

	case state_UP:
	    prim_dist = p->y - from->y;
	    sec_dist = p->x - from->x;
	    break;

	case state_LEFT:
	    prim_dist = from->x - p->x;
	    sec_dist = p->y - from->y;
	    break;
	}

	/* ensure that we are
	   a) in the correct quadrant
	   b) not the original point as passed in
	   c) have a lower primary distance that the original point, or

	   d) have the same primary distance and a *larger* secondary distance.
	   We go for larger as if we went to smaller we'd only go to larger the
	   next time round so we might as well skip that step. If this decision
	   changes then we need to change the start point for the finding the
	   first point below

	   Now use smaller
	*/
	if (prim_dist >= 0 && sec_dist >= 0 &&
	    !(prim_dist == 0 && sec_dist == 0) &&
	    (prim_dist < best_prim_dist || (prim_dist == best_prim_dist && sec_dist < best_sec_dist)))
	{
	    best_prim_dist = prim_dist;
	    best_sec_dist = sec_dist;
	    best = i;
	}
    }

    return best;
}

static int pointlist_find_next_index(int index, tracking_state state)
{
    return pointlist_find_next(&point_array[index], state);
}

static int pointlist_find_first(void)
{
    intxy start;

    /* to find the top left item we start from the bottom left and go
       right due to the way that the above search finds the largest
       secondary dostance */
    start.x = -0x4000;
    start.y =  0x4000;

    return pointlist_find_next(&start, state_RIGHT);
}

static void pointlist_clear(void)
{
    n_points = 0;
}

/* ---------------------------------------------------------------------- */


static void boundary_add_point(antweb_selection_t *sel, int plot, int x, int y)
{
    antweb_selection_boundary *item = mm_calloc(sizeof(*item), 1);

    if (sel->boundary_last)
	sel->boundary_last->next = item;
    else
	sel->boundary = item;
    
    item->plot = plot;
    item->x = x;
    item->y = y;
}

static void boundary_add_point_index(antweb_selection_t *sel, int plot, int index)
{
    LNKDBG(("boundary_add_point_index: plot %d index %d\n", plot, index));

    boundary_add_point(sel, plot, point_array[index].x, point_array[index].y);
}

/* Add the given wimp box as a complete path to the boundary */

static void boundary_add_rectangle(antweb_selection_t *sel, const wimp_box *bb)
{
    LNKDBG(("boundary_add_rectangle: box %d,%d %d,%d\n", bb->x0, bb->y0, bb->x1, bb->y1));

    boundary_add_point(sel, selection_boundary_MOVE, bb->x0, bb->y1);
    boundary_add_point(sel, selection_boundary_DRAW, bb->x0, bb->y0);
    boundary_add_point(sel, selection_boundary_DRAW, bb->x1, bb->y0);
    boundary_add_point(sel, selection_boundary_DRAW, bb->x1, bb->y1);
    boundary_add_point(sel, selection_boundary_DRAW, bb->x0, bb->y1);
}

static void boundary_create_from_pointlist(antweb_selection_t *sel)
{
    int p, first;
    tracking_state state;

    LNKDBG(("boundary_create_from_pointlist:\n"));

    p = first = pointlist_find_first();

    boundary_add_point_index(sel, selection_boundary_MOVE, p);
    state = state_DOWN;

    LNKDBG(("boundary_create_from_pointlist: start %d\n", p));

    do
    {
	tracking_state orig_state = state;
	int next_point;

	do
	{
	    next_point = pointlist_find_next_index(p, state);

	    LNKDBG(("boundary_create_from_pointlist: state %d next %d\n", state, next_point));

	    if (next_point == -1)
		state = (tracking_state)((state + 1) % 4);
	}
	while (next_point == -1 && state != orig_state);
	    
	p = next_point;

	if (p != -1)
	    boundary_add_point_index(sel, selection_boundary_DRAW, p);
    }
    while (p != first && p != -1);

    boundary_add_point(sel, selection_boundary_END, 0, 0);

    pointlist_clear();
}

/* ---------------------------------------------------------------------- */

#if 0
typedef struct
{
    rid_aref_item *aref;
    wimp_box box;
    int is_disconnected;
} box_is_disconnected_str;

static BOOL box_is_disconnected_fn(be_doc doc, rid_text_item *ti, wimp_box *box, void *handle)
{
    box_is_disconnected_str *dis = handle;

    /* if different aref then stop */
    if (dis->aref != ti->aref)
	return TRUE;

    /* if there is an overlap then unset flag and stop */
    if (dis->box.x1 >= box->x0 &&
	box->x1 >= dis->box.x0)
    {
	dis->is_disconnected = FALSE;
	return TRUE;
    }	
	
    return FALSE;
}

static BOOL box_is_disconnected(be_doc doc, rid_aref_item *aref, wimp_box *box)
{
    box_is_disconnected_str dis;

    dis.aref = aref;
    dis.box = *box;
    dis.is_disconnected = 1;
	
    stream_iterate_box(doc, aref->first, box_is_disconnected_fn, &dis);

    return dis.is_disconnected;
}
#endif

/* ---------------------------------------------------------------------- */

static BOOL check_aref(be_doc doc, rid_text_item *ti, wimp_box *box, void *handle)
{
    antweb_selection_t *sel = &doc->selection;
    rid_aref_item *aref = handle;

    LNKDBG(("check_aref: ti%p ti->aref %p aref %p\n", ti, ti->aref, aref));

    /* if different aref then stop */
    if (aref != ti->aref)
	return TRUE;

#if 0
    if (box_is_disconnected(doc, sel->data.aref, box))
	boundary_add_rectangle(sel, box);
    else
#endif
	pointlist_add_rectangle(sel, box);

    return FALSE;
}

void highlight_boundary_build(be_doc doc)
{
    antweb_selection_t *sel = &doc->selection;
    wimp_box bb;

    LNKDBG(("boundary_build: doc%p\n", doc));
    
    highlight_boundary_clear(doc);

    switch (sel->tag)
    {
    case doc_selection_tag_NONE:
	break;

    case doc_selection_tag_TEXT:
    {
	wimp_box trim_box;
	be_item ti;
	    
	memset(&trim_box, 0, sizeof(trim_box));
	ti = sel->data.text.item;

	if (object_table[ti->tag].update_highlight == 0 ||
	    object_table[ti->tag].update_highlight(ti, doc, 0, &trim_box))
	{
	    backend_doc_item_bbox(doc, ti, &bb);

	    boundary_add_rectangle(sel, &bb);
	    boundary_add_point(sel, selection_boundary_END, 0, 0);
	}

	break;
    }
    
    case doc_selection_tag_AREF:
	stream_iterate_box(doc, sel->data.aref->first, check_aref, sel->data.aref);

	boundary_create_from_pointlist(sel);
	break;

    case doc_selection_tag_MAP:
	switch (sel->data.map.area->type)
	{
	case rid_area_RECT:
	    break;
	case rid_area_CIRCLE:
	    break;
	case rid_area_POLYGON:
	    break;
	}
	break;
    }
    LNKDBG(("boundary_build: out%p\n", doc));
}

void highlight_boundary_clear(be_doc doc)
{
    antweb_selection_t *sel = &doc->selection;
    antweb_selection_boundary *item = sel->boundary;

    LNKDBG(("boundary_clear: doc%p\n", doc));

    while (item)
    {
	antweb_selection_boundary *next = item->next;
	
	mm_free(item);

	item = next;
    }
    
    sel->boundary = NULL;
    sel->boundary_last = NULL;
}
#else

void highlight_boundary_build(be_doc doc)
{
}

void highlight_boundary_clear(be_doc doc)
{
}

#endif

/* ---------------------------------------------------------------------- */

#if NEW_HL

void highlight_render(wimp_redrawstr *rr, antweb_doc *doc)
{
    int ox, oy;
    antweb_selection_boundary *sb;

    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

#if USE_MARGINS
    ox += doc->margin.x0;
    oy += doc->margin.y1;
#endif

    for (sb = doc->selection.boundary; sb; sb = sb->next)
    {
	switch (sb->plot)
	{
	case selection_boundary_END:
	    break;	    
	case selection_boundary_MOVE:
	    bbc_move(ox + sb->x, oy + sb->y);
	    break;	    
	case selection_boundary_DRAW:
	    bbc_draw(ox + sb->x, oy + sb->y);
	    break;	    
	}
    }
}

void highlight_draw_text_box(rid_text_item *ti, antweb_doc *doc, int b, int hpos, BOOL has_text)
{
}

void highlight_render_outline(be_item ti, antweb_doc *doc, int hpos, int bline)
{
}

void highlight_update_border(antweb_doc *doc, wimp_box *box, BOOL draw)
{
}

void highlight_offset_border(wimp_box *box)
{
}


#else


static int get_colour_text(int i)
{
    wimp_paletteword *cols = config_colour_list[render_colour_list_HIGHLIGHT];
    return i > cols[0].word ? cols[1].word : cols[i+1].word;
}

void highlight_render_outline(be_item ti, antweb_doc *doc, int hpos, int bline)
{
    LNKDBGN(("highlight_render_outline: doc %p ti %p %d,%d sel %d\n", doc, ti, hpos, bline, ti->flag & rid_flag_SELECTED ? 1 : 0));

    if (ti->flag & (rid_flag_SELECTED|rid_flag_ACTIVATED))
    {
	int x, y, w, h;
	wimp_paletteword *cols = config_colour_list[ti->flag & rid_flag_ACTIVATED ? render_colour_list_ACTIVATED : render_colour_list_HIGHLIGHT];
	int border_width = cols[0].word/2*2 + IMAGE_SPACE;

	x = hpos - border_width;
	y = bline - ti->max_down - border_width;
	w = ti->width + border_width*2;
	h = ti->max_up + ti->max_down + border_width*2;
	
	render_plinth_from_list(0, cols, render_plinth_NOFILL, x, y, w, h, doc);
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

    LNKDBGN(("highlight_update_border: draw %d box %d,%d %d,%d\n", draw, box->x0, box->y0, box->x1, box->y1));
    if (draw && !gbf_active(GBF_ANTI_TWITTER))
    {
        frontend_view_update(doc->parent, box, render_border, doc, 0);
    }
    else
    {
	wimp_box trim = *box;
    
	trim.x1 = trim.x0 + (config_display_highlight_width*2);
	do_redraw;
	trim.x1 = box->x1;

	trim.y1 = trim.y0 + (config_display_highlight_width*2);
	do_redraw;
	trim.y1 = box->y1;

	trim.x0 = trim.x1 - (config_display_highlight_width*2) - 1;
	do_redraw;
	trim.x0 = box->x0;

	trim.y0 = trim.y1 - (config_display_highlight_width*2) - 1;
	do_redraw;
    }
}

void highlight_offset_border(wimp_box *box)
{
    int border_width = config_display_highlight_width*2 + 4;
    box->x0 -= border_width;
    box->y0 -= border_width;
    box->x1 += border_width + (frontend_dx-1);
    box->y1 += border_width + (frontend_dy-1);
}

static object_font_state hl_render_fs;
static rid_aref_item *hl_render_aref;

static int redraw_hl(be_doc doc, rid_text_item *ti, wimp_box *box, void *handle)
{
    wimp_redrawstr *rr = handle;
    int ox, oy;
    int hpos, bline;

    if (ti->aref != hl_render_aref)
	return TRUE;

    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

#if USE_MARGINS
    ox += doc->margin.x0;
    oy += doc->margin.y1;
#endif

    hpos = box->x0;
    bline = box->y0 + ti->max_down;

    object_table[ti->tag].redraw(ti, doc->rh, doc, ox + hpos, oy + bline, &hl_render_fs, &rr->g, ox, oy, object_redraw_HIGHLIGHT);

    return FALSE;
}

void highlight_render(wimp_redrawstr *rr, antweb_doc *doc)
{
    int ox, oy;
    int hpos, bline;
    be_item ti;

    ox = rr->box.x0 - rr->scx;
    oy = rr->box.y1 - rr->scy;

#if USE_MARGINS
    ox += doc->margin.x0;
    oy += doc->margin.y1;
#endif

    /* init font state */
    memset(&hl_render_fs, 0, sizeof(hl_render_fs));
    hl_render_fs.lf = hl_render_fs.lfc = -1;
    
    LNKDBGN(("highlight_render: doc %p tag %d\n", doc, doc->selection.tag));

    switch (doc->selection.tag)
    {
    case doc_selection_tag_NONE:
	break;

    case doc_selection_tag_TEXT:
	ti = doc->selection.data.text.item;
	
	if (object_table[ti->tag].redraw &&
	    stream_find_item_location(ti, &hpos, &bline))
	    object_table[ti->tag].redraw(ti, doc->rh, doc, ox + hpos, oy + bline, &hl_render_fs, &rr->g, ox, oy, object_redraw_HIGHLIGHT);
	break;

    case doc_selection_tag_MAP:
	ti = doc->selection.data.map.item;
	
	if (object_table[ti->tag].redraw &&
	    stream_find_item_location(ti, &hpos, &bline))
	    object_table[ti->tag].redraw(ti, doc->rh, doc, ox + hpos, oy + bline, &hl_render_fs, &rr->g, ox, oy, object_redraw_HIGHLIGHT);
	break;

    case doc_selection_tag_AREF:
	hl_render_aref = doc->selection.data.aref;
	stream_iterate_box(doc, doc->selection.data.aref->first, redraw_hl, rr);
	break;
    }
}


static void draw_partial_box(BOOL first, BOOL last, BOOL first_line, int x, int y, int w, int h)
{
    int dx = frontend_dx, dy = frontend_dy;
    int ww = last ? w : w - dx;

    if (first)
    {
        bbc_move(x, y);
        bbc_drawby(0, h-dy);
    }
    else
        bbc_move(x, y + h - dy);

    if (ww > 0)
    {
	if (first_line)
	    bbc_drawby(ww, 0);
	else
	    bbc_moveby(ww, 0);
    }

    if (last)
        bbc_drawby(0, - (h-dy));
    else
        bbc_moveby(0, - (h-dy));

    if (ww > 0)
	bbc_drawby(-ww, 0);
}

#if 0
/* Draw a line using the draw module - gets round some printing problems - Oh no it doesn't !! */

static void draw_line(int x, int baseline, int w)
{
    int buf[7], *bp = buf;
    int cap[4];
    int trans[6];
    _kernel_oserror *e;

    *bp++ = 2;			/* move */
    *bp++ = x << 8;
    *bp++ = baseline << 8;

    *bp++ = 8;			/* line */
    *bp++ = (x + w) << 8;
    *bp++ = baseline << 8;

    *bp++ = 0;			/* end */

    cap[0] = 0;
    cap[1] = 0;
    cap[2] = 0;
    cap[3] = 0;

    trans[0] = 0x00010000;
    trans[1] = 0;
    trans[2] = 0;
    trans[3] = 0x00010000;

    trans[4] = 0;
    trans[5] = 0;

    e = _swix(Draw_Stroke, _INR(0,6), buf, 0x30, trans, 0, 4 << 8, cap, 0);
    if (e) usrtrc("Draw_Stroke: %x %s\n", e->errnum, e->errmess);
}
#endif

static rid_pos_item *last_line_of_link(rid_aref_item *aref)
{
    rid_text_item *ti, *last_ti;

    for (last_ti = ti = aref->first;
	 ti && ti->aref == aref;
	 ti = rid_scanf(ti->next))
    {
	last_ti = ti;
    }

    return last_ti ? last_ti->line : NULL;
}

void highlight_draw_text_box(rid_text_item *ti, antweb_doc *doc, int b, int hpos, BOOL has_text)
{
    BOOL first = ti->aref->first == ti;
    BOOL last = ti->next == NULL || ti->next->aref == NULL || ti->next->aref != ti->aref;
    BOOL first_in_line = ti == ti->line->first;
    BOOL last_in_line = ti->next == ti->line->next->first;
    BOOL on_first_line = ti->line == ti->aref->first->line;
    BOOL on_last_line = last || last_line_of_link(ti->aref) == ti->line;
    int width, height;

    first = first || first_in_line;
    last = last || last_in_line || ti->next->tag != ti->tag; /* if type changes then count as end of line */

    if (has_text || (first ^ last))
    {
	int i, n, ypos;
	int last_col = -1;

	width = ti->width + (last ? 0 : ti->pad);
	height = ti->max_up + ti->max_down - frontend_dy;
	
	ypos =  b - ti->max_down;

	/* for now limit the text border width to 4 pixels */
	n = config_colour_list[render_colour_list_HIGHLIGHT][0].word/2;
	if (n > 4)
	    n = 4;

	/* raise first line by width + gap */
	if (on_first_line)
	    height += n*2 + IMAGE_SPACE;
	else
	    height += 2;	/* nasty value to fill hole - should be based on leading and stuff */

	if (on_last_line)
	{
	    ypos -= n*2/*  + IMAGE_SPACE */;
	    height += n*2/*  + IMAGE_SPACE */;
	}
	
	/* move first box left to avoid obscuring text */
	if (first)
	{
	    hpos -= n*2 + IMAGE_SPACE;
	    width += n*2 + IMAGE_SPACE;
	}

	/* move RH side right a bit to avoid obscuring text */
	if (last)
	{
	    width += n*2 + IMAGE_SPACE;
	}
	
	if (width < n*2)
	    width = n*2;
	
	for (i = 0; i < n; i++)
	{
	    int col = get_colour_text(i);

	    if (col != last_col)
		render_set_colour(last_col = col, doc);

	    draw_partial_box(first, last, TRUE, hpos, ypos, width, height);
	    if (first)
	    {
		hpos += 2;
		width -= 2;
	    }
	
	    if (last)
		width -= 2;
	
	    height -= 4;
	    ypos += 2;
	}
    }
}

#endif

/* eof highlight.c */
