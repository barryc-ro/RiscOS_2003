/* -*-c-*- */

/* otextarea.c */

/*
 * 21/3/96: SJM: Changed antweb_update_item to avoid refreshing borders
 * 17/5/96: SJM: added ctrl-copy to ctrl-k.
 * 11/6/96: SJM: added ch 10 to ch 13
 * 19/7/96: SJM: reworked loops to handle new list
 * 08/8/96: SJM: added gwindow to redraw
 */

/* Methods for textarea objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memwatch.h"

#include "wimp.h"
#include "bbc.h"
#include "swis.h"
#include "akbd.h"

#include "antweb.h"
#include "interface.h"
#include "rid.h"
#include "webfonts.h"
#include "consts.h"
#include "config.h"
#include "render.h"
#include "rcolours.h"
#include "url.h"
#include "util.h"

#include "object.h"

#include "stream.h"
#include "gbf.h"

#if UNICODE
#include "Unicode/utf8.h"
#else
#define UTF8_codelen(a) 1
#define UTF8_seqlen(a) 1
#endif

#define TXTDBG(a) DBG(a)
#define TXTDBGN(a)

#ifndef BUILDERS
static rid_header *ot_stored_rh;
static antweb_doc *ot_stored_doc;
static int ot_stored_hpos, ot_stored_bline;
#endif

static int insert_char(rid_textarea_item *tai, int point, int c)
{
    int nbytes = UTF8_codelen(c);

    TXTDBG(("insert_char: tai%p point %d '%c'\n", tai, point, c));

    if (memzone_alloc(&tai->text, nbytes) != -1)
    {
	int n_to_move;
	    
	flexmem_noshift();

	/* for safety */
	if (point < 0)
	    point = 0;
	if (point > tai->text.used-1)
	    point = tai->text.used-1;

	n_to_move = tai->text.used - (point + nbytes);
	if (n_to_move > 0)
	    memmove(tai->text.data + point + nbytes, tai->text.data + point, n_to_move);
#if UNICODE
	UCS4_to_UTF8(tai->text.data + point, c);
#else
	tai->text.data[point] = c;
#endif
	flexmem_shift();
    }
    return nbytes;
}

static void delete_bytes(rid_textarea_item *tai, int point, int n)
{
    int n_to_move;

    TXTDBG(("delete_bytes: tai%p point %d n %d\n", tai, point, n));

    flexmem_noshift();

    /* for safety */
    if (point < 0)
	point = 0;
    if (point > tai->text.used-1)
	point = tai->text.used-1;
    if (point + n > tai->text.used)
	n = tai->text.used - point;

    n_to_move = tai->text.used - (point + n);
    if (n_to_move > 0)
	memmove(tai->text.data + point, tai->text.data + point + n, n_to_move);
    tai->text.used -= n;

    flexmem_shift();
}

static int move_point(rid_textarea_item *tai, int dir)
{
    int moved;
#if UNICODE
    if (config_encoding_internal == 1)
    {
	char *s, *p;

	flexmem_noshift();

	p = s = tai->text.data + tai->lines[tai->cy] + tai->cx;
	if (dir < 0)
	    p = UTF8_prev(s);
	else if (dir > 0)
	    p = UTF8_next(s);
	moved = p - s;

	flexmem_shift();
    }
    else
	moved = dir;
#else
    moved = dir;
#endif
    return moved;
}

/*
 * If no wrapping then next break is where the next newline is
 * Otherwise it is after the last white space that is within the
 * visible area of the textarea.

 * This won't work correctly unless offset is a break point
 */

static int find_next_break(rid_textarea_item *tai, int offset)
{
    char *s;
    int i, n, ws, column;
    
    TXTDBG(("find_next_break: tai%p offset %d data %p used %d\n", tai, offset, tai->text.data, tai->text.used));

    flexmem_noshift();

    s = tai->text.data + offset;
    n = tai->text.used - offset;
    ws = -1;
    i = 0;
    column = 0;
    
    while (i < n)
    {
	int c, nbytes;
	/* get next char */
#if UNICODE
	if (config_encoding_internal == 1)
	    nbytes = UTF8_to_UCS4(s, (UCS4 *)&c);
	else
	{
	    c = *s;
	    nbytes = 1;
	}
#else
	c = *s;
	nbytes = 1;
#endif
	
	if (c == '\n')
	{
	    /* increment past the newline */
	    i++;
	    break;
	}

	if (tai->wrap != rid_ta_wrap_NONE)
	{
	    if (c == ' ')
		ws = i;

	    /* if in last column */
	    if (column == tai->useable_cols - 1)
	    {
		/* if has some whitespace reset to one past last whitespace */
		if (ws != -1)
		    i = ws + 1;
		else
		    i += nbytes;
		break;
	    }
	}

	/* increment column number */
	column++;
	i += nbytes;
	s += nbytes;
    }

    flexmem_shift();

    TXTDBG(("find_next_break: returns %d\n", offset + i));

    return offset + i;
}

static int count_lines(rid_textarea_item *tai)
{
    int i, this_off;
    
    TXTDBG(("count_lines: tai%p\n", tai));

    this_off = 0;
    i = 0;
    while (this_off < tai->text.used)
    {
	this_off = find_next_break(tai, this_off);
	i++;
    }

    TXTDBG(("count_lines: returns %d\n", i));

    return i == 0 ? 1 : i;
}

static void fillin_line_list(rid_textarea_item *tai, int *list)
{
    int i;
    
    TXTDBG(("fillin_line_list: tai %p list %p\n", tai, list));

    list[0] = 0;
    for (i = 0; i < tai->n_lines; i++)
    {
	list[i+1] = find_next_break(tai, list[i]);
    }
}

static void build_line_list(rid_textarea_item *tai)
{
    TXTDBG(("build_line_list: tai %p\n", tai));

    tai->n_lines = count_lines(tai);

    mm_free(tai->lines);
    tai->lines = mm_malloc(sizeof(int) * (tai->n_lines + 1));

    fillin_line_list(tai, tai->lines);
}

static int check_line_list(rid_textarea_item *tai, int offset, int apply_after)
{
    int n_lines;
    int changed_from;
    int i;
    
    TXTDBG(("check_line_list: tai %p offset %d after %d\n", tai, offset, apply_after));

    /* calculate the new line count and allocate the structure */
    n_lines = count_lines(tai);
    
    tai->lines = mm_realloc(tai->lines, sizeof(int) * (n_lines + 1));

    tai->lines[0] = 0;
    changed_from = -1;

    /* for each new line see if it has changed from the old one */
    for (i = 0; i < n_lines; i++)
    {
	int start = find_next_break(tai, tai->lines[i]);

	/* if we haven't already recorded a change */
	if (changed_from == -1)
	{
	    /* if we are beyond the original number of lines then that
               is a change */
	    if (i >= tai->n_lines)
		changed_from = i;
	    else
	    {
		/* we may have to offset the original line start to
                   take account of characters added or removed */
		int cmp = tai->lines[i+1];

		if (cmp >= apply_after)
		    cmp += offset;

		if (cmp != start)
		    changed_from = i;
	    }
	}

	tai->lines[i+1] = start;
    }

    tai->n_lines = n_lines;

    return changed_from;
}

/*
 * convert from a character offset to a byte offset
 */

static int chars_to_bytes(rid_textarea_item *tai, int nchars)
{
    int cx;
#if UNICODE
    char *s;
    flexmem_noshift();

    s = tai->text.data + tai->lines[tai->cy];
    cx = UTF8_next_n(s, nchars) - s;

    flexmem_shift();
#else
    cx = nchars;
#endif
    return cx;
}

static int bytes_to_chars(rid_textarea_item *tai, int nbytes)
{
    int cx;
#if UNICODE
    char *s;
    flexmem_noshift();

    s = tai->text.data + tai->lines[tai->cy];
    cx = UTF8_strlen_n(s, nbytes);

    flexmem_shift();
#else
    cx = nbytes;
#endif
    return cx;
}

/* return length of displayable characters */
int otextarea_line_length(rid_textarea_item *tai, int line, BOOL *terminated)
{
    int this_off = tai->lines[line];
    int next_off = tai->lines[line+1];
    int len = next_off - this_off;
    BOOL term = FALSE;

    if (len > 0 && next_off > 0)
    {
	flexmem_noshift();

	if (tai->text.data[next_off-1] == '\n')
	    term = TRUE;

	flexmem_shift();
    }

    if (terminated)
	*terminated = term;
    
    if (term)
	len--;

    return len;
}

static int line_length(rid_textarea_item *tai, int line)
{
    return otextarea_line_length(tai, line, NULL);
}

static void decode_offset(rid_textarea_item *tai, int cpos, SHORTISH *xpos, SHORTISH *ypos)
{
    int i;
    for (i = 0; i < tai->n_lines; i++)
    {
	if (cpos < tai->lines[i+1])
	{
	    *xpos = cpos - tai->lines[i];
	    *ypos = i;
	    return;
	}
    }
    *xpos = line_length(tai, tai->n_lines - 1);
    *ypos = tai->n_lines - 1;
}

static void ensure_one_newline(rid_textarea_item *tai)
{
    if (tai->text.used == 0)
    {
	TXTDBG(("ensure_one_newline: tai%p\n", tai));
	insert_char(tai, 0, '\n');
    }
}

#ifndef BUILDERS
static void otextarea_copy_defaults(rid_textarea_item *tai)
{
    int off = -2;

    TXTDBG(("otextarea_copy_defaults: tai%p default text %p used %d\n", tai, tai->default_text.data, tai->default_text.used));

    if ((tai->text.used >= tai->default_text.used) ||
	(off = memzone_alloc(&tai->text, tai->default_text.used - tai->text.used)) != -1)
    {
	flexmem_noshift();

	/* copy over the data and set the used value */
	memcpy(tai->text.data, tai->default_text.data, tai->default_text.used);
	tai->text.used = tai->default_text.used;

	TXTDBG(("default area: '%.*s'\n", tai->default_text.used, tai->default_text.data));
	
	flexmem_shift();

	/* tidy this block now */
	memzone_tidy(&tai->text);
    }
    TXTDBG(("otextarea_copy_defaults: off %d\n", off));
    tai->cx = tai->cy = tai->sx = tai->sy = 0;
}
#endif /* BUILDERS */

static int useable_columns(rid_textarea_item *tai, antweb_doc *doc)
{
    int cols = tai->cols;
    if (doc->scale_value != 100)
    {
	cols = cols * doc->scale_value / 100;
	if (cols == 0)
	    cols = 1;
    }
    return cols;
}    

void otextarea_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
#ifdef BUILDERS
    ti->width = 20;
    ti->pad = 1;
    ti->max_up = 16;
    ti->max_down = 1;
#else /* BUILDERS */

    rid_textarea_item *tai;
    int whichfont;

    whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);

    tai = ((rid_text_item_textarea *)ti)->area;

    otextarea_copy_defaults(tai);

    tai->useable_cols = useable_columns(tai, doc);
    
    ti->width = webfont_nominal_width(whichfont, tai->useable_cols);
    ti->width += 20;
    ti->max_up = webfonts[whichfont].max_up + 10;
    ti->max_down = tai->rows * (webfonts[whichfont].max_up + webfonts[whichfont].max_down) -
	webfonts[whichfont].max_up + 10;

    ensure_one_newline(tai);

    build_line_list(tai);
#endif /* BUILDERS */
}

void otextarea_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_textarea_item *tai;
    int i;
    int h;
    int whichfont;
    int line_gap;
    int dx = frontend_dx, dy = frontend_dy;
    wimp_box ta_box, gwind_box;
    int fg, bg;
    int has_caret;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT)
    {
	highlight_render_outline(ti, doc, hpos, bline);
	return;
    }

    if (update == object_redraw_BACKGROUND)
	return;

    whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
    line_gap = webfonts[whichfont].max_up + webfonts[whichfont].max_down;

    tai = ((rid_text_item_textarea *)ti)->area;
    has_caret = be_item_has_caret(doc, ti);

    fg = tai->base.colours.back == -1 ?
	render_colour_RGB | config_colours[render_colour_PLAIN].word :
	render_text_link_colour(ti, doc);

    bg = tai->base.colours.back == -1 ? render_colour_WRITE :
	has_caret && tai->base.colours.select != -1 ?
	tai->base.colours.select | render_colour_RGB : tai->base.colours.back | render_colour_RGB;

    if (fs->lfc != fg || fs->lbc != bg)
    {
	fs->lfc = fg;
	fs->lbc = bg;
	render_set_font_colours(fg, bg, doc);
    }

#ifdef STBWEB
    render_plinth_from_list(bg,
			    has_caret && config_colour_list[render_colour_list_WRITE_HIGHLIGHT] ?
			      config_colour_list[render_colour_list_WRITE_HIGHLIGHT] :
			      config_colour_list[render_colour_list_WRITE],
			    0,
			    hpos, bline - ti->max_down,
			    ti->width, (ti->max_up + ti->max_down), doc );
#else
    render_plinth(bg, render_plinth_RIM | render_plinth_IN,
		  hpos, bline - ti->max_down,
		  ti->width, (ti->max_up + ti->max_down), doc );
#endif

    TXTDBGN(("otextarea_redraw: gwind %d,%d,%d,%d\n", g->x0, g->y0, g->x1, g->y1));
    TXTDBGN(("otextarea_redraw: box   %d,%d,%d,%d\n", hpos+8, bline-ti->max_down+8, hpos+ti->width-8, bline+ti->max_up-8));

    /* Check for whether the text needs redrawing if it does
     * use the intersection of graphics window and text box
     */
    ta_box.x0 = hpos+8;
    ta_box.y0 = bline-ti->max_down+8;
    ta_box.x1 = hpos+ti->width-8;
    ta_box.y1 = bline+ti->max_up-8;
    if (coords_intersection(&ta_box, g, &gwind_box))
    {
	int *this_line = &tai->lines[tai->sy];
	int end = tai->n_lines - tai->sy;
	if (end > tai->rows)
	    end = tai->rows;

	TXTDBG(("otextarea_redraw: rows %d n_lines %d sy %d end %d\n", tai->rows, tai->n_lines, tai->sy, end));
	
	bbc_gwindow(gwind_box.x0, gwind_box.y0, gwind_box.x1-dx, gwind_box.y1-dy);

	flexmem_noshift();
	for(i=0, h = bline; i < end; i++, h -= line_gap, this_line++)
	{
	    if (h - webfonts[whichfont].max_down < g->y1 && h + webfonts[whichfont].max_up > g->y0 )
	    {
		int len = this_line[1] - this_line[0] - tai->sx;
		if (len > 0)
		{
		    TXTDBG(("otextarea_redraw: this[1] %d this[0] %d sx %d = len %d\n", this_line[1], this_line[0], tai->sx, len));
		    TXTDBG(("otextarea_redraw: '%.*s'\n", len, tai->text.data + this_line[0] + tai->sx));
		    render_text_full(doc, whichfont, tai->text.data + this_line[0] + tai->sx, hpos + 10, h, NULL, len);
		}
	    }
	}
	flexmem_shift();

	bbc_gwindow(g->x0, g->y0, g->x1-dx, g->y1-dy);
    }
#endif /* BUILDERS */
}

#ifndef BUILDERS
static int otextarea_fast_redraw_fn(wimp_redrawstr *r, void *h, int update)
{
    rid_text_item *ti = (rid_text_item *) h;
    int ox, oy;
    object_font_state fs;

    fs.lf = fs.lfc = -1;

    ox = r->box.x0 - r->scx;
    oy = r->box.y1 - r->scy;

    otextarea_redraw(ti, ot_stored_rh, ot_stored_doc, ot_stored_hpos + ox, ot_stored_bline + oy, &fs, &r->g, ox, oy, update);

    return 0;
}

static void otextarea_update_lines(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int top, int bottom)
{
    wimp_box box;
    int line_gap;
    int whichfont;

    whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
    line_gap = webfonts[whichfont].max_up + webfonts[whichfont].max_down;

    stream_find_item_location(ti, &ot_stored_hpos, &ot_stored_bline);

#if USE_MARGINS
    ot_stored_hpos += doc->margin.x0;
    ot_stored_bline += doc->margin.y1;
#endif

    ot_stored_rh = rh;
    ot_stored_doc = doc;

    box.x0 = ot_stored_hpos + 8;
    box.x1 = ot_stored_hpos + ti->width - 8;

    box.y1 = ot_stored_bline - (top    * line_gap) + webfonts[whichfont].max_up;
    box.y0 = ot_stored_bline - (bottom * line_gap) - webfonts[whichfont].max_down;

    frontend_view_update(doc->parent, &box, &otextarea_fast_redraw_fn, ti, FALSE);
}

static void otextarea_scroll_lines(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int top, int bottom, int new_bottom)
{
    rid_textarea_item *tai;
    int line_gap;
    int hpos, vpos;
    wimp_box box;
    int new_y;
    int whichfont;

    whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
    line_gap = webfonts[whichfont].max_up + webfonts[whichfont].max_down;
    tai = ((rid_text_item_textarea *)ti)->area;

    stream_find_item_location(ti, &hpos, &vpos);

#if USE_MARGINS
    hpos += doc->margin.x0;
    vpos += doc->margin.y1;
#endif

    box.x0 = hpos + 8;
    box.x1 = hpos + ti->width - 8;

    box.y1 = vpos - (top    * line_gap) + webfonts[whichfont].max_up;
    box.y0 = vpos - (bottom * line_gap) - webfonts[whichfont].max_down;

    new_y  = vpos - (new_bottom * line_gap) - webfonts[whichfont].max_down;

    frontend_view_block_move(doc->parent, &box, box.x0, new_y);
}
#endif /* BUILDERS */

void otextarea_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_textarea_item *tai;

    tai = ((rid_text_item_textarea *)ti)->area;
    memzone_destroy(&tai->text);
}

char *otextarea_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    int line_gap, whichfont;
    rid_textarea_item *tai;

    whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
    line_gap = webfonts[whichfont].max_up + webfonts[whichfont].max_down;
    tai = ((rid_text_item_textarea *)ti)->area;

    /* Remember that our Y coordinate is relative to the base line. */

    y -= 10;
    y = webfonts[whichfont].max_up - y;
    y /= line_gap;
    y += tai->sy;

    if (y >= tai->n_lines)
    {
	tai->cy = tai->n_lines-1;
	tai->cx = line_length(tai, tai->cy);
    }
    else
    {
	int len, start_offset;
	char *text;

	tai->cy = y;

	flexmem_noshift();

	len = line_length(tai, y);
	start_offset = tai->lines[y];
	text = tai->text.data + start_offset;

	if (tai->sx >= len)
	    tai->cx = len;
	else
	{
	    x -= 12;

	    tai->cx = webfont_get_offset(whichfont, text + tai->sx, x, NULL, len - tai->sx);

	    TXTDBG(("otextarea_click: x %d len %d out %d\n", x, len - tai->sx, tai->cx));
	}

	flexmem_shift();
    }

    antweb_place_caret(doc, ti, doc_selection_offset_UNKNOWN);
#endif /* BUILDERS */
    return NULL;		/* Do not follow links */
}

void otextarea_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    fputs("[Text input]", f);
}

BOOL otextarea_caret(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int repos)
{
#ifndef BUILDERS
    int cx, cy;
    rid_textarea_item *tai;
    char *text;
    int h;
    int term;
    int whichfont;

    tai = ((rid_text_item_textarea *)ti)->area;

#ifdef STBWEB
    if (repos == object_caret_BLUR)
    {
	antweb_update_item(doc, ti);
	return FALSE;
    }

    if (repos == object_caret_FOCUS)
	antweb_update_item(doc, ti);
#else
    if (repos == object_caret_BLUR && tai->base.colours.select != -1)
    {
	antweb_update_item(doc, ti);
	return FALSE;
    }

    if (repos == object_caret_FOCUS && tai->base.colours.select != -1)
	antweb_update_item(doc, ti);
#endif

    if (doc->selection.data.text.input_offset < 0)
    {
	doc->selection.data.text.input_offset = 0;
	repos = object_caret_REPOSITION;
    }

    if (tai->cy > tai->n_lines-1)
	tai->cy = tai->n_lines-1;
    text = tai->text.data + tai->lines[tai->cy];

    stream_find_item_location(ti, &cx, &cy);

    term = line_length(tai, tai->cy);
    if (term > tai->cx)
	term = tai->cx;
    term -= tai->sx;

    whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
	    
    cx += webfont_font_width_n(whichfont, text + tai->sx, term);
    cx += 10;

    cy -= webfonts[whichfont].max_down;
    cy -= (tai->cy - tai->sy) * (webfonts[whichfont].max_up + webfonts[whichfont].max_down);

#if USE_MARGINS
    cx += doc->margin.x0;
    cy += doc->margin.y1;
#endif
    h = webfonts[whichfont].max_up + webfonts[whichfont].max_down;
    h |= render_caret_colour(doc, tai->base.colours.back, tai->base.colours.cursor);

    frontend_view_caret(doc->parent, cx, cy, h, repos == object_caret_REPOSITION || repos == object_caret_FOCUS);
#endif /* BUILDERS */
    return TRUE;
}

#define CHAR_USED	0x01	/* Don't let the character be passed back */
#define REDRAW_LINE	0x02	/* Redraw from one before the caret to the end of the line */
#define SPLIT_DOWN	0x04	/* Shift the lines from one below the caret down one, redraw caret line and line above */
#define REPOS_CARET	0x08	/* Reposition the caret */
#define REDRAW_ALL	0x10	/* Repaint the whole lot */
#define JOIN_LINES	0x20	/* Shift all lines from two below up one */
#if NEW_TEXTAREA
#define REDRAW_DOWN	0x40	/* Repaint from the caret line downwards */
#endif

BOOL otextarea_key(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int key)
{
#ifndef BUILDERS
    int flags = 0;
    rid_textarea_item *tai;
    int dpoint, dchar;
    int changed_from;
    int len;
    int osx, osy;
    int cols;
    int cxpos, sxpos;

    tai = ((rid_text_item_textarea *)ti)->area;

    if (tai->cy < 0)				/* just for safety */
	tai->cy = 0;
    if (tai->cy > tai->n_lines - 1)
	tai->cy = tai->n_lines - 1;

    len = line_length(tai, tai->cy);		/* this is the printable line length (in bytes) */
    dchar = 0;					/* this is the number of bytes added or removed (from dpoint) */
    dpoint = -1;				/* this is the point of insertion/deletion */
    changed_from = 0;

    osx = tai->sx;
    osy = tai->sy;

    flags |= CHAR_USED;

    if (key >= 32 && key != 127
#if !UNICODE
	&& key < 256
#endif
	)
    {
	if (tai->cx > len)
	{
	    tai->cx = len;
	    flags |= REPOS_CARET;
	}

/* 	dchar = 1; */
	dpoint = tai->lines[tai->cy] + tai->cx;
	dchar = insert_char(tai, dpoint, key);

	flags |= (REDRAW_LINE | REPOS_CARET);
    }
    else
    {
	input_key_action action;
#if UNICODE
	if (key < 0)
	    key = -key;
#endif
	action = lookup_key_action(key);

/* 	DBG(("otextarea_key: key %d action %d\n", key, action)); */
	
	switch (action)
	{
	case key_action_NEWLINE:
	case key_action_NEWLINE_SUBMIT_ALWAYS:
	case key_action_NEWLINE_SUBMIT_LAST:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

/* 	    dchar = 1; */
	    dpoint = tai->lines[tai->cy] + tai->cx;	/* this is the caret point at the start */
	    dchar = insert_char(tai, dpoint, '\n');

	    flags |= (SPLIT_DOWN | REDRAW_LINE | REPOS_CARET);
	    break;

	case key_action_DELETE_LEFT:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

	    if (tai->cx > 0)
	    {
/* 		dchar = -1; */
		dpoint = tai->lines[tai->cy] + tai->cx /* + dchar */;	/* this is the caret point at the start */
		dchar = move_point(tai, -1);
		dpoint += dchar;
		delete_bytes(tai, dpoint, -dchar);

		flags |= (REDRAW_LINE | REPOS_CARET);
	    }
	    else if (tai->cy > 0)
	    {
/* 		dchar = -1; */
		dpoint = tai->lines[tai->cy] + tai->cx /* + dchar */;	/* this is the caret point at the start */
		dchar = move_point(tai, -1);
		dpoint += dchar;
		delete_bytes(tai, dpoint, -dchar);

		flags |= (REDRAW_LINE | REPOS_CARET | JOIN_LINES);
	    }
	    break;

	case key_action_DELETE_TO_END:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

	    if (tai->cx < len)
	    {
		dchar = - (len - tai->cx);
		dpoint = tai->lines[tai->cy] + tai->cx;	/* this is the caret point at the start */
		delete_bytes(tai, dpoint, -dchar);
		flags |= REDRAW_LINE;
		break;
	    }
	    /* Otherwise, fall throught and do a ctrl-D to the end of line */
	case key_action_DELETE_RIGHT:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

	    if (tai->cx < len || tai->cy < tai->n_lines-1)
	    {
		dpoint = tai->lines[tai->cy] + tai->cx;	/* this is the caret point at the start */
		dchar = - move_point(tai, +1);
		delete_bytes(tai, dpoint, - dchar);
		if (tai->cx < len)
		    flags |= REDRAW_LINE;
		else
		    flags |= (REDRAW_LINE | REPOS_CARET | JOIN_LINES);
	    }
	    break;
	case key_action_DELETE_ALL:
	    dpoint = tai->lines[tai->cy];
	    dchar = -len;
	    delete_bytes(tai, dpoint, len);
	    tai->cx = 0;
	    flags |= (REDRAW_LINE | REPOS_CARET);
	    break;
	case key_action_DELETE_ALL_AREA:
	{
	    dpoint = 0;
	    dchar = - tai->text.used;
	    delete_bytes(tai, 0, tai->text.used);

	    ensure_one_newline(tai);
	    tai->cx = tai->cy = 0;
	    flags |= (REDRAW_ALL | REPOS_CARET);
	    break;
	}
	case key_action_LEFT:
	case key_action_LEFT_OR_OFF:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

	    if (tai->cx > 0)
	    {
/* 		tai->cx--; */
		tai->cx += move_point(tai, -1);
	        flags |= REPOS_CARET;
	    }
            else if (action == key_action_LEFT_OR_OFF)
                flags &= ~CHAR_USED;
	    break;

	case key_action_RIGHT:
	case key_action_RIGHT_OR_OFF:
	    if (tai->cx < len /* MAX_TEXT_LINE */) /* SJM: 11/5/97: don't allow movement off the RH edge of text */
	    {
		tai->cx += move_point(tai, +1);
/* 		tai->cx++; */
	        flags |= REPOS_CARET;
	    }
            else if (action == key_action_RIGHT_OR_OFF)
                flags &= ~CHAR_USED;
	    break;

	case key_action_START_OF_LINE:
	    tai->cx = 0;
	    flags |= REPOS_CARET;
	    break;

	case key_action_END_OF_LINE:
	    tai->cx = len;
	    flags |= REPOS_CARET;
	    break;

	case key_action_START_OF_AREA:
	    tai->cx = tai->cy = 0;
	    flags |= REPOS_CARET;
	    break;

	case key_action_END_OF_AREA:
	    tai->cy = tai->n_lines - 1;
	    tai->cx = line_length(tai, tai->cy);
	    flags |= REPOS_CARET;
	    break;

	case key_action_UP:
	    if (tai->cy)
	    {
		tai->cy--;
		tai->cx = chars_to_bytes(tai, bytes_to_chars(tai, tai->cx));
		tai->sx = chars_to_bytes(tai, bytes_to_chars(tai, tai->sx));
		flags |= REPOS_CARET;
	    }
	    else
	    {
		flags &= ~CHAR_USED;
	    }
	    break;
	case key_action_DOWN:
	    if (tai->cy < tai->n_lines - 1)
	    {
		tai->cy++;
		tai->cx = chars_to_bytes(tai, bytes_to_chars(tai, tai->cx));
		tai->sx = chars_to_bytes(tai, bytes_to_chars(tai, tai->sx));
		flags |= REPOS_CARET;
	    }
	    else
	    {
		flags &= ~CHAR_USED;
	    }
	    break;
	default:
	    flags &= ~CHAR_USED;

	    DBG(("Key %d\n", key));
	    /* Ignore spurious control chars */
	    break;
	}
    }

    /* see if any characters have been added */
    if (dchar != 0)
    {
	int cpos;

	/* rebuild the line start list */
	changed_from = check_line_list(tai, dchar, dpoint);

	/* recalculate the caret position */
	cpos = dpoint;
	if (dchar > 0)
	    cpos += dchar;

	decode_offset(tai, cpos, &tai->cx, &tai->cy);
	
	/* if wrapping is on then redraw all below here
	 * this is a bit crude but may do
	 */
	if (tai->wrap != rid_ta_wrap_NONE && changed_from != -1)
	    flags |= REDRAW_DOWN;
    }

    cols = tai->useable_cols;
    cxpos = bytes_to_chars(tai, tai->cx);
    sxpos = bytes_to_chars(tai, tai->sx);

    /* force the horizontal scroll position to be visible */
    if (cxpos < sxpos || cxpos > (sxpos + cols))
    {
	if (cxpos < (cols / 2))
	    tai->sx = 0;
	else
	    tai->sx = chars_to_bytes(tai, cxpos - (cols / 2));

	flags |= REDRAW_ALL;
    }

    /* force the vertical scroll position to be visible */
    if (tai->cy < tai->sy || tai->cy >= (tai->sy + tai->rows))
    {
	if (tai->cy < (tai->rows / 2))
	    tai->sy = 0;
	else
	    tai->sy = tai->cy - (tai->rows / 2);
    }

    if ((flags & REDRAW_ALL) || (tai->sy != osy))
    {
        wimp_box box;
        box.x0 =   2*5;
        box.x1 = - 2*5;
        box.y1 = - 2*5;
        box.y0 =   2*5;
	antweb_update_item_trim(doc, ti, &box, FALSE);
    }
    else if (flags & REDRAW_DOWN)
    {
	otextarea_update_lines(ti, rh, doc, changed_from - tai->sy, tai->rows-1);
    }
    else if (flags & JOIN_LINES)
    {
	/* We want to move the caret out of the area that gets block-copied before the copy */
	if (flags & REPOS_CARET)
	{
	    otextarea_caret(ti, rh, doc, object_caret_REPOSITION);
	    flags &= ~REPOS_CARET;
	}

	if ((tai->cy - tai->sy + 2) < tai->rows)
	    otextarea_scroll_lines(ti, rh, doc, tai->cy - tai->sy + 2, tai->rows -1, tai->rows - 2);
	otextarea_update_lines(ti, rh, doc, tai->cy - tai->sy, tai->cy - tai->sy);
	if (tai->cy - tai->sy != tai->rows -1)
	    otextarea_update_lines(ti, rh, doc, tai->rows - 1, tai->rows - 1);
    }
    else if (flags & SPLIT_DOWN)
    {
	if (tai->cy - tai->sy != tai->rows - 1)
	    otextarea_scroll_lines(ti, rh, doc, tai->cy - tai->sy, tai->rows -2, tai->rows - 1);
	/* Since this is only called when there has been no scroll... */
	otextarea_update_lines(ti, rh, doc, tai->cy - tai->sy - 1, tai->cy - tai->sy);
    }
    else if (flags & REDRAW_LINE)
    {
	otextarea_update_lines(ti, rh, doc, tai->cy - tai->sy, tai->cy - tai->sy);
    }

    if (flags & REPOS_CARET)
    {
	otextarea_caret(ti, rh, doc, object_caret_REPOSITION);
    }

    return ((flags & CHAR_USED) != 0);
#else
    return FALSE;
#endif /* BUILDERS */

}

int otextarea_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box)
{
    if (box)
	memset(box, 0, sizeof(*box));

    return TRUE;
}



/*

Using a flex area for text data rather than malloced lines

The area contains the full text as entered. New lines specifically
entered are stored as a single newline (character 10).

The first 'n' words are offsets from the start of the text area to the
start of each line. There is one word for each line. The number of
lines is stored in the textarea header.

Editing is done by inserting or delating a character as appropriate
and then rescanning the text for line start changes. This compares
line start and end before and after taking into account the delta of
chracters added or removed.

A line start is always immediately after a newline and if wrapping is
on, then when the words won't fit on the line.

*/


/* eof otextarea.c */
