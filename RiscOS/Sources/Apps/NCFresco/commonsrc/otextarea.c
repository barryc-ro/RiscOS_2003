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

#define TXTDBG(a) 

#if !NEW_TEXTAREA
#ifndef MAX_TEXT_LINE
#define MAX_TEXT_LINE 1024
#endif
#endif

#ifndef BUILDERS
static rid_header *ot_stored_rh;
static antweb_doc *ot_stored_doc;
static int ot_stored_hpos, ot_stored_bline;
#endif

#if NEW_TEXTAREA
static void insert_char(rid_textarea_item *tai, int point, int c)
{
    TXTDBG(("insert_char: tai%p point %d '%c'\n", tai, point, c));

    if (memzone_alloc(&tai->text, 1) != -1)
    {
	int n_to_move;
	    
	flexmem_noshift();

	/* for safety */
	if (point < 0)
	    point = 0;
	if (point > tai->text.used-1)
	    point = tai->text.used-1;

	n_to_move = tai->text.used - (point + 1);
	if (n_to_move > 0)
	    memmove(tai->text.data + point + 1, tai->text.data + point, n_to_move);
	tai->text.data[point] = c;

	flexmem_shift();
    }
}

static void delete_chars(rid_textarea_item *tai, int point, int n)
{
    int n_to_move;

    TXTDBG(("delete_chars: tai%p point %d n %d\n", tai, point, n));

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

/*
 * If no wrapping then next break is where the next newline is
 * Otherwise it is after the last white space that is within the
 * visible area of the textarea.

 * This won't work correctly unless offset is a break point
 */

static int find_next_break(rid_textarea_item *tai, int offset)
{
    char *s;
    int i, n, ws;
    
    TXTDBG(("find_next_break: tai%p offset %d data %p used %d\n", tai, offset, tai->text.data, tai->text.used));

    flexmem_noshift();

    s = tai->text.data + offset;
    n = tai->text.used - offset;
    ws = -1;
    
    for (i = 0; i < n; i++)
    {
	int c = *s++;

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
	    if (i == tai->useable_cols - 1)
	    {
		/* if has some whitespace reset to one past last whitespace */
		if (ws != -1)
		    i = ws + 1;
		else
		    i++;
		break;
	    }
	}
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

/* return length of displayable characters */
static int line_length(rid_textarea_item *tai, int line)
{
    int this_off = tai->lines[line];
    int next_off = tai->lines[line+1];
    int len = next_off - this_off;

    if (len > 0 && next_off > 0)
    {
	flexmem_noshift();

	if (tai->text.data[next_off-1] == '\n')
	    len--;

	flexmem_shift();
    }
    return len;
}

static void font_paint_n(const char *text, int n, int x, int y)
{
    TXTDBG(("paint: '%.*s'\n", n, text));

    _swix(Font_Paint, _INR(1,4) | _IN(7), text, (1<<7) | font_OSCOORDS, x, y, n);
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
	insert_char(tai, 0, '\n');
}
#endif

#if !NEW_TEXTAREA
static void otextarea_free_lines(rid_textarea_item *tai)
{
    rid_textarea_line *tal, *tal2;

    tal = tai->lines;

    while(tal)
    {
	tal2 = tal->next;

	if (tal->text)
	    mm_free(tal->text);
	mm_free(tal);

	tal = tal2;
    }

    tai->lines = tai->last_line = tai->caret_line = NULL;
}
#endif

#ifndef BUILDERS
static void otextarea_copy_defaults(rid_textarea_item *tai)
{
#if NEW_TEXTAREA
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
#else
    rid_textarea_line *def, *new_ta;

    if (tai->lines)
	otextarea_free_lines(tai);

    def = tai->default_lines;

    new_ta = mm_calloc(1, sizeof(*new_ta));
    new_ta->text = mm_malloc(MAX_TEXT_LINE);
    if (new_ta->text)
	new_ta->text[0] = 0;

    if (def)
    {
	if (def->text)
	    strncpy(new_ta->text, def->text, MAX_TEXT_LINE);
	def = def->next;
    }

    tai->lines = tai->last_line = new_ta;

    while (def)
    {
	new_ta = mm_calloc(1, sizeof(*new_ta));
	new_ta->text = mm_malloc(MAX_TEXT_LINE);

	if (def->text)
	    strncpy(new_ta->text, def->text, MAX_TEXT_LINE);
	else
	    new_ta->text[0] = 0;

	tai->last_line->next = new_ta;
	new_ta->prev = tai->last_line;
	tai->last_line = new_ta;

	def = def->next;
    }

    tai->caret_line = tai->lines;
#endif
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
    font_string fs;
    char *buffer;		/* SJM: changed from auto to malloc */

    if ( !GETFONTUSED( doc, WEBFONT_TTY ) )
    {
        webfont_find_font( WEBFONT_TTY );
        SETFONTUSED( doc, WEBFONT_TTY );
    }

    tai = ((rid_text_item_textarea *)ti)->area;

    otextarea_copy_defaults(tai);

    tai->useable_cols = useable_columns(tai, doc);
    
    buffer = mm_malloc(tai->useable_cols + 1);
    memset(buffer, ' ', tai->useable_cols);
    buffer[tai->useable_cols] = 0;
    
/*     for(i=0; i < tai->cols; i++) */
/* 	buffer[i] = ' '; */
/*     buffer[i] =0; */

    fs.s = buffer;
    fs.x = fs.y = (1 << 30);
    fs.split = -1;
    fs.term = tai->useable_cols;

    if ( font_setfont(webfonts[WEBFONT_TTY].handle) == NULL && font_strwidth(&fs) == NULL)
	ti->width = (fs.x / MILIPOINTS_PER_OSUNIT);
    else
	ti->width = tai->useable_cols * webfonts[WEBFONT_TTY].space_width;
    ti->width += 20;
    ti->max_up = webfonts[WEBFONT_TTY].max_up + 10;
    ti->max_down = tai->rows * (webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down) -
	webfonts[WEBFONT_TTY].max_up + 10;

    mm_free(buffer);

#if NEW_TEXTAREA
    ensure_one_newline(tai);

    build_line_list(tai);
#endif

#endif /* BUILDERS */
}

void otextarea_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_textarea_item *tai;
#if !NEW_TEXTAREA
    rid_textarea_line *tal;
#endif
    int i;
    int h;
    int line_gap = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
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
    
    tai = ((rid_text_item_textarea *)ti)->area;
    has_caret = be_item_has_caret(doc, ti);

    fg = tai->base.colours.back == -1 ?
	render_colour_RGB | config_colours[render_colour_PLAIN].word :
	render_text_link_colour(ti, doc);

    bg = tai->base.colours.back == -1 ? render_colour_WRITE :
	has_caret && tai->base.colours.select != -1 ?
	tai->base.colours.select | render_colour_RGB : tai->base.colours.back | render_colour_RGB;

    if (fs->lf != webfonts[WEBFONT_TTY].handle)
    {
	fs->lf = webfonts[WEBFONT_TTY].handle;
	font_setfont(fs->lf);
    }

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

#if !NEW_TEXTAREA
    for(i=tai->sy, tal = tai->lines; tal && i; i--, tal = tal->next)
	;
#endif
    
#if 0
    fprintf(stderr, "otextarea_redraw: gwind %d,%d,%d,%d\n", g->x0, g->y0, g->x1, g->y1);
    fprintf(stderr, "otextarea_redraw: box   %d,%d,%d,%d\n", hpos+8, bline-ti->max_down+8, hpos+ti->width-8, bline+ti->max_up-8);
#endif
    /* Check for whether the text needs redrawing if it does
     * use the intersection of graphics window and text box
     */
    ta_box.x0 = hpos+8;
    ta_box.y0 = bline-ti->max_down+8;
    ta_box.x1 = hpos+ti->width-8;
    ta_box.y1 = bline+ti->max_up-8;
    if (coords_intersection(&ta_box, g, &gwind_box))
    {
#if NEW_TEXTAREA
	int *this_line = &tai->lines[tai->sy];
	int end = tai->n_lines - tai->sy;
	if (end > tai->rows)
	    end = tai->rows;

	TXTDBG(("otexarea_redraw: rows %d n_lines %d sy %d end %d\n", tai->rows, tai->n_lines, tai->sy, end));
#endif
	
	bbc_gwindow(gwind_box.x0, gwind_box.y0, gwind_box.x1-dx, gwind_box.y1-dy);

#if NEW_TEXTAREA
	flexmem_noshift();
	for(i=0, h = bline; i < end; i++, h -= line_gap, this_line++)
	{
	    if (h - webfonts[WEBFONT_TTY].max_down < g->y1 && h + webfonts[WEBFONT_TTY].max_up > g->y0 )
	    {
		int len = this_line[1] - this_line[0] - tai->sx;
		if (len > 0)
		    font_paint_n(tai->text.data + this_line[0] + tai->sx, len, hpos + 10, h);
	    }
	}
	flexmem_shift();
#else
	for(i=tai->rows, h = bline; tal && i; i--, tal = tal->next, h -= line_gap)
	{
	    if (h - webfonts[WEBFONT_TTY].max_down < g->y1 && h + webfonts[WEBFONT_TTY].max_up > g->y0 )
		if (tal->text && strlen(tal->text) > tai->sx)
		    font_paint(tal->text + tai->sx, font_OSCOORDS, hpos + 10, h);
	}
#endif
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
    int line_gap = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;

    stream_find_item_location(ti, &ot_stored_hpos, &ot_stored_bline);

#if USE_MARGINS
    ot_stored_hpos += doc->margin.x0;
    ot_stored_bline += doc->margin.y1;
#endif

    ot_stored_rh = rh;
    ot_stored_doc = doc;

    box.x0 = ot_stored_hpos + 8;
    box.x1 = ot_stored_hpos + ti->width - 8;

    box.y1 = ot_stored_bline - (top    * line_gap) + webfonts[WEBFONT_TTY].max_up;
    box.y0 = ot_stored_bline - (bottom * line_gap) - webfonts[WEBFONT_TTY].max_down;

    frontend_view_update(doc->parent, &box, &otextarea_fast_redraw_fn, ti, FALSE);
}

static void otextarea_scroll_lines(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int top, int bottom, int new_bottom)
{
    rid_textarea_item *tai;
    int line_gap = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
    int hpos, vpos;
    wimp_box box;
    int new_y;

    tai = ((rid_text_item_textarea *)ti)->area;

    stream_find_item_location(ti, &hpos, &vpos);

#if USE_MARGINS
    hpos += doc->margin.x0;
    vpos += doc->margin.y1;
#endif

    box.x0 = hpos + 8;
    box.x1 = hpos + ti->width - 8;

    box.y1 = vpos - (top    * line_gap) + webfonts[WEBFONT_TTY].max_up;
    box.y0 = vpos - (bottom * line_gap) - webfonts[WEBFONT_TTY].max_down;

    new_y  = vpos - (new_bottom * line_gap) - webfonts[WEBFONT_TTY].max_down;

    frontend_view_block_move(doc->parent, &box, box.x0, new_y);
}
#endif /* BUILDERS */

void otextarea_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_textarea_item *tai;

    tai = ((rid_text_item_textarea *)ti)->area;
#if NEW_TEXTAREA
    memzone_destroy(&tai->text);
#else
    otextarea_free_lines(tai);
#endif
}

char *otextarea_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    int line_gap = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
    rid_textarea_item *tai;
#if !NEW_TEXTAREA
    rid_textarea_line *tal;
    int i;
#endif
    os_error *ep;
    os_regset r;

    tai = ((rid_text_item_textarea *)ti)->area;

    /* Remember that our Y coordinate is relative to the base line. */

    y -= 10;
    y = webfonts[WEBFONT_TTY].max_up - y;
    y /= line_gap;
    y += tai->sy;

#if !NEW_TEXTAREA
    for (i = y, tal = tai->lines; i && tal->next; i--, tal = tal->next)
	;

    tai->caret_line = tal;
#endif
    
#if NEW_TEXTAREA
    if (y >= tai->n_lines)
    {
	tai->cy = tai->n_lines-1;
	tai->cx = line_length(tai, tai->cy);
    }
#else
    if (i)
    {
	tai->cy = y - i;
	tai->cx = strlen(tal->text);
    }
#endif
    else
    {
	int len, start_offset;
	char *text;

	tai->cy = y;

#if NEW_TEXTAREA
	flexmem_noshift();

	len = line_length(tai, y);
	start_offset = tai->lines[y];
	text = tai->text.data + start_offset;
#else
	text = tal->text;
	len = strlen(text);
#endif
	if (tai->sx >= len)
	    tai->cx = len;
	else
	{
	    x -= 12;

	    r.r[0] = (int) (long) webfonts[WEBFONT_TTY].handle;
	    r.r[1] = (int) (long) text + tai->sx;
	    r.r[2] = (1 << 17) | (1 << 8) | (1 << 7);
	    r.r[3] = x * MILIPOINTS_PER_OSUNIT;
	    r.r[4] = r.r[5] = r.r[6] = 0;
	    r.r[7] = len - tai->sx;

	    ep = os_swix(Font_ScanString, &r);

	    if (ep == NULL)
	    {
		/* By taking the offset from the begining of the string the X scroll is taken into account */
		tai->cx = ((char*) (long) r.r[1]) - text;
	    }
	    else
	    {
		tai->cx = len;
	    }
	}
#if NEW_TEXTAREA
	flexmem_shift();
#endif
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
    os_error *ep;
    char *text;
    font_string fs;
#if !NEW_TEXTAREA
    rid_textarea_line *tal;
    int i;
#endif
    int h;

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

#if NEW_TEXTAREA
    if (tai->cy > tai->n_lines-1)
	tai->cy = tai->n_lines-1;
    text = tai->text.data + tai->lines[tai->cy];
#else
    for(tal = tai->lines, i = tai->cy; i && tal->next; i--, tal = tal->next)
	;
    if (i)
	tai->cy -= i;
    text = tal->text;
#endif

    stream_find_item_location(ti, &cx, &cy);

    fs.s = text + tai->sx;
    fs.x = fs.y = (1 << 30);
    fs.split = -1;
#if NEW_TEXTAREA
    fs.term = line_length(tai, tai->cy);
    if (fs.term > tai->cx)
	fs.term = tai->cx;
    fs.term -= tai->sx;
#else
    fs.term = tai->cx - tai->sx;
#endif
    ep = font_setfont(webfonts[WEBFONT_TTY].handle);
    if (ep)
	return FALSE;

    ep = font_strwidth(&fs);
    if (ep)
	return FALSE;
    cx += (fs.x / MILIPOINTS_PER_OSUNIT);
    cx += 10;

    cy -= webfonts[WEBFONT_TTY].max_down;
    cy -= (tai->cy - tai->sy) * (webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down);

#if USE_MARGINS
    cx += doc->margin.x0;
    cy += doc->margin.y1;
#endif
    h = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
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
#if NEW_TEXTAREA
    int dpoint, dchar;
    int changed_from;
#else
    rid_textarea_line *tal;
#endif
    int len;
    int osx, osy;
    int cols;

    tai = ((rid_text_item_textarea *)ti)->area;
#if NEW_TEXTAREA
    if (tai->cy < 0)				/* just for safety */
	tai->cy = 0;
    if (tai->cy > tai->n_lines - 1)
	tai->cy = tai->n_lines - 1;

    len = line_length(tai, tai->cy);		/* this is the printable line length */
    dchar = 0;					/* this is the number of chars added or removed (from dpoint) */
    dpoint = -1;				/* this is the point of insertion/deletion */
    changed_from = 0;
#else
    tal = tai->caret_line;
    len = strlen(tal->text);
#endif

    osx = tai->sx;
    osy = tai->sy;

    flags |= CHAR_USED;

    if (key >= 32 && key < 256 && key != 127)
    {
	if (tai->cx > len)
	{
	    tai->cx = len;
	    flags |= REPOS_CARET;
	}

#if NEW_TEXTAREA
	dchar = 1;
	dpoint = tai->lines[tai->cy] + tai->cx;
	insert_char(tai, dpoint, key);
/* 	tai->cx++; */

	flags |= (REDRAW_LINE | REPOS_CARET);
#else
	if (len + 1 < MAX_TEXT_LINE)
	{
	    memmove(tal->text + tai->cx + 1, tal->text + tai->cx, len + 1 - tai->cx );

	    tal->text[tai->cx] = key;
	    tai->cx++;

	    flags |= (REDRAW_LINE | REPOS_CARET);
	}
#endif
    }
    else
    {
	input_key_action action = lookup_key_action(key);

/* 	DBG(("otextarea_key: key %d action %d\n", key, action)); */
	
	switch (action)
	{
	case key_action_NEWLINE:
	case key_action_NEWLINE_SUBMIT_ALWAYS:
	case key_action_NEWLINE_SUBMIT_LAST:
#if NEW_TEXTAREA
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

	    dchar = 1;
	    dpoint = tai->lines[tai->cy] + tai->cx;	/* this is the caret point at the start */
	    insert_char(tai, dpoint, '\n');
/* 	    tai->cy++; */
/* 	    tai->cx = 0; */

	    flags |= (SPLIT_DOWN | REDRAW_LINE | REPOS_CARET);
#else
	    {
		rid_textarea_line *new_tal;

		if (tai->cx > len)
		{
		    tai->cx = len;
		    flags |= REPOS_CARET;
		}

		new_tal = mm_calloc(1, sizeof(*new_tal));
		new_tal->text = mm_malloc(MAX_TEXT_LINE);
		if (new_tal->text)
		    new_tal->text[0] = 0;

		new_tal->prev = tal;
		new_tal->next = tal->next;
		if (tal->next)
		    tal->next->prev = new_tal;
		else
		    tai->last_line = new_tal;
		tal->next = new_tal;

		if (tai->cx != len)
		{
		    strcpy(new_tal->text, tal->text + tai->cx);
		    tal->text[tai->cx] = 0;
		}

		tai->caret_line = new_tal;
		tai->cy++;
		tai->cx = 0;

		flags |= (SPLIT_DOWN | REDRAW_LINE | REPOS_CARET);
	    }
#endif
	    break;
	case key_action_DELETE_LEFT:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

#if NEW_TEXTAREA
	    if (tai->cx > 0)
	    {
		dchar = -1;
		dpoint = tai->lines[tai->cy] + tai->cx - 1;	/* this is the caret point at the start */
		delete_chars(tai, dpoint, 1);

/* 		tai->cx--; */
		flags |= (REDRAW_LINE | REPOS_CARET);
	    }
	    else if (tai->cy > 0)
	    {
/* 		tai->cy --; */
/* 		tai->cx = line_length(tai, tai->cy); */  /* do first to get point where lines join up */

		dchar = -1;
		dpoint = tai->lines[tai->cy] + tai->cx - 1;	/* this is the caret point at the start */
		delete_chars(tai, dpoint, 1);

		flags |= (REDRAW_LINE | REPOS_CARET | JOIN_LINES);
	    }
#else
	    if (tai->cx > 0)
	    {
		memmove(tal->text + tai->cx - 1, tal->text + tai->cx, len + 1 - tai->cx);
		tai->cx--;
		flags |= (REDRAW_LINE | REPOS_CARET);
	    }
	    else if (tal->prev)
	    {
		rid_textarea_line *tal2 = tal->prev;
		int ll;

		ll = strlen(tal2->text);

		strncpy(tal2->text + ll, tal->text, MAX_TEXT_LINE - ll - 1);
		tal2->text[MAX_TEXT_LINE -1] = 0;

		mm_free(tal->text);
		tal2->next = tal->next;
		if (tal->next)
		    tal->next->prev = tal2;
		else
		    tai->last_line = tal2;

		mm_free(tal);

		tai->cx = ll;
		tai->cy--;
		tai->caret_line = tal2;

		flags |= (REDRAW_LINE | REPOS_CARET | JOIN_LINES);
	    }
#endif
	    break;
	case key_action_DELETE_TO_END:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

#if NEW_TEXTAREA
	    if (tai->cx < len)
	    {
		dchar = - (len - tai->cx);
		dpoint = tai->lines[tai->cy] + tai->cx;	/* this is the caret point at the start */
		delete_chars(tai, dpoint, len - tai->cx);
		flags |= REDRAW_LINE;
		break;
	    }
#else
	    if (tal->text[tai->cx] != 0)
	    {
		tal->text[tai->cx] = 0;	/* Truncate the line */

		flags |= REDRAW_LINE;
		break;
	    }
#endif
	    /* Otherwise, fall throught and do a ctrl-D to the end of line */
	case key_action_DELETE_RIGHT:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

#if NEW_TEXTAREA
	    if (tai->cx < len || tai->cy < tai->n_lines-1)
	    {
		dchar = -1;
		dpoint = tai->lines[tai->cy] + tai->cx;	/* this is the caret point at the start */
		delete_chars(tai, dpoint, 1);
		if (tai->cx < len)
		    flags |= REDRAW_LINE;
		else
		    flags |= (REDRAW_LINE | REPOS_CARET | JOIN_LINES);
	    }
#else
	    if (tai->cx < len)
	    {
		memmove(tal->text + tai->cx, tal->text + tai->cx + 1, len - tai->cx);
		flags |= REDRAW_LINE;
	    }
	    else if (tal->next)
	    {
		rid_textarea_line *tal2 = tal->next;
		int ll;

		ll = strlen(tal2->text);

		strncpy(tal->text + tai->cx, tal2->text, MAX_TEXT_LINE - len - 1);
		tal->text[MAX_TEXT_LINE -1] = 0;

		mm_free(tal2->text);
		tal->next = tal2->next;
		if (tal2->next)
		    tal2->next->prev = tal;
		else
		    tai->last_line = tal;

		mm_free(tal2);

		flags |= (REDRAW_LINE | REPOS_CARET | JOIN_LINES);
	    }
#endif
	    break;
	case key_action_DELETE_ALL:
#if NEW_TEXTAREA
	    dpoint = tai->lines[tai->cy];
	    dchar = -len;
	    delete_chars(tai, dpoint, len);
#else
	    tal->text[0] = 0;
#endif
	    tai->cx = 0;
	    flags |= (REDRAW_LINE | REPOS_CARET);
	    break;
	case key_action_DELETE_ALL_AREA:
	{
#if NEW_TEXTAREA
	    dpoint = 0;
	    dchar = - tai->text.used;
	    delete_chars(tai, 0, tai->text.used);

	    ensure_one_newline(tai);
#else
	    rid_textarea_line *line = tai->lines;
	    while (line)
	    {
		line->text[0] = 0;
		line = line->next;
	    }

	    tai->caret_line = tai->lines;
#endif
	    tai->cx = tai->cy = 0;
	    flags |= (REDRAW_ALL | REPOS_CARET);
	    break;
	}
	case key_action_LEFT:
	case key_action_LEFT_OR_OFF:
#if NEW_TEXTAREA
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }
#endif
	    if (tai->cx > 0)
	    {
		tai->cx--;
	        flags |= REPOS_CARET;
	    }
            else if (action == key_action_LEFT_OR_OFF)
                flags &= ~CHAR_USED;
	    break;

	case key_action_RIGHT:
	case key_action_RIGHT_OR_OFF:
	    if (tai->cx < len /* MAX_TEXT_LINE */) /* SJM: 11/5/97: don't allow movement off the RH edge of text */
	    {
		tai->cx++;
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
#if !NEW_TEXTAREA
	    tai->caret_line = tai->lines;
#endif
	    flags |= REPOS_CARET;
	    break;

	case key_action_END_OF_AREA:
#if NEW_TEXTAREA
	    tai->cy = tai->n_lines - 1;
	    tai->cx = line_length(tai, tai->cy);
#else
	    while (tai->caret_line->next)
	    {
		tai->cy++;
		tai->caret_line = tai->caret_line->next;
	    }
	    tai->cx = strlen(tai->caret_line->text);
#endif
	    flags |= REPOS_CARET;
	    break;

	case key_action_UP:
#if NEW_TEXTAREA
	    if (tai->cy)
	    {
		tai->cy--;
		flags |= REPOS_CARET;
	    }
#else
	    if (tal->prev)
	    {
		tai->cy--;
		tai->caret_line = tal->prev;
		flags |= REPOS_CARET;
	    }
#endif
	    else
	    {
		flags &= ~CHAR_USED;
	    }
	    break;
	case key_action_DOWN:
#if NEW_TEXTAREA
	    if (tai->cy < tai->n_lines - 1)
	    {
		tai->cy++;
		flags |= REPOS_CARET;
	    }
#else
	    if (tal->next)
	    {
		tai->cy++;
		tai->caret_line = tal->next;
		flags |= REPOS_CARET;
	    }
#endif
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

#if NEW_TEXTAREA
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
#endif

    cols = tai->useable_cols;
    
    /* force the horizontal scroll position to be visible */
    if (tai->cx < tai->sx || tai->cx > (tai->sx + cols))
    {
	if (tai->cx < (cols / 2))
	    tai->sx = 0;
#if !NEW_TEXTAREA
	else if (tai->cx > MAX_TEXT_LINE - (cols / 2))
	    tai->sx = MAX_TEXT_LINE - cols;
#endif
	else
	    tai->sx = tai->cx - (cols / 2);

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
#if NEW_TEXTAREA
    else if (flags & REDRAW_DOWN)
    {
/* 	int cy = tai->cy < ocy ? tai->cy : ocy; */
	otextarea_update_lines(ti, rh, doc, changed_from - tai->sy, tai->rows-1);
    }
#endif
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
/*  rid_textarea_item *tai = ((rid_text_item_textarea *) ti)->area; */

    if (box)
	memset(box, 0, sizeof(*box));

    return TRUE;
}

#if NEW_TEXTAREA
void otextarea_append_to_buffer(rid_textarea_item *tai, char **buffer, int *blen)
{
    int i;

    flexmem_noshift();
    
    for (i = 0; i < tai->n_lines; i++)
    {
	int len = line_length(tai, i);
	BOOL terminated = (tai->lines[i+1] - tai->lines[i]) != len;

	be_ensure_buffer_space(buffer, blen, 3 * len + 2);

	url_escape_cat_n(*buffer, tai->text.data + tai->lines[i], *blen, len);

	if (i != tai->n_lines-1 &&
	    (tai->wrap == rid_ta_wrap_HARD || terminated))
	    strcat(*buffer, "%0D%0A");
    }

    flexmem_shift();
}

void otextarea_write_to_file(rid_textarea_item *tai, FILE *f, int url_encoding)
{
    int i;

    flexmem_noshift();
    
    for (i = 0; i < tai->n_lines; i++)
    {
	int len = line_length(tai, i);
	BOOL terminated = (tai->lines[i+1] - tai->lines[i]) != len;

	if (url_encoding)
	    url_escape_to_file_n(tai->text.data + tai->lines[i], f, len);
	else
	    fwrite(tai->text.data + tai->lines[i], len, 1, f);

	if ((i != tai->n_lines-1 || !url_encoding) &&
	    (tai->wrap == rid_ta_wrap_HARD || terminated))
	{
	    fputs(url_encoding ? "%0D%0A" : "\r\n", f);
	}
    }
    
    flexmem_shift();
}
#endif

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
