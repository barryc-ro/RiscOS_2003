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
#include "util.h"

#include "object.h"

#include "stream.h"
#include "gbf.h"

#ifndef MAX_TEXT_LINE
#define MAX_TEXT_LINE 1024
#endif

#ifndef BUILDERS
static rid_header *ot_stored_rh;
static antweb_doc *ot_stored_doc;
static int ot_stored_hpos, ot_stored_bline;
#endif

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

#ifndef BUILDERS
static void otextarea_copy_defaults(rid_textarea_item *tai)
{
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

    tai->cx = tai->cy = tai->sx = tai->sy = 0;
}
#endif /* BUILDERS */

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
    int i;

    if ( !GETFONTUSED( doc, WEBFONT_TTY ) )
    {
        webfont_find_font( WEBFONT_TTY );
        SETFONTUSED( doc, WEBFONT_TTY );
    }

    tai = ((rid_text_item_textarea *)ti)->area;

    otextarea_copy_defaults(tai);

    buffer = mm_malloc(tai->cols + 1);
    memset(buffer, ' ', tai->cols);
    buffer[tai->cols] = 0;
    
/*     for(i=0; i < tai->cols; i++) */
/* 	buffer[i] = ' '; */
/*     buffer[i] =0; */

    fs.s = buffer;
    fs.x = fs.y = (1 << 30);
    fs.split = -1;
    fs.term = tai->cols;

    if ( font_setfont(webfonts[WEBFONT_TTY].handle) == NULL && font_strwidth(&fs) == NULL)
	ti->width = (fs.x / MILIPOINTS_PER_OSUNIT);
    else
	ti->width = tai->cols * webfonts[WEBFONT_TTY].space_width;
    ti->width += 20;
    ti->max_up = webfonts[WEBFONT_TTY].max_up + 10;
    ti->max_down = tai->rows * (webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down) -
	webfonts[WEBFONT_TTY].max_up + 10;

    mm_free(buffer);
#endif /* BUILDERS */
}

void otextarea_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_textarea_item *tai;
    rid_textarea_line *tal;
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
    render_plinth_full(bg,
		       has_caret ? plinth_col_HL_M : plinth_col_M,
		       has_caret ? plinth_col_HL_L : plinth_col_L,
		       has_caret ? plinth_col_HL_D : plinth_col_D,
		       render_plinth_RIM | render_plinth_DOUBLE_RIM,
		       hpos, bline - ti->max_down,
		       ti->width, (ti->max_up + ti->max_down), doc );
#else
    render_plinth(bg, render_plinth_RIM | render_plinth_IN,
		  hpos, bline - ti->max_down,
		  ti->width, (ti->max_up + ti->max_down), doc );
#endif

    for(i=tai->sy, tal = tai->lines; tal && i; i--, tal = tal->next)
	;

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
	bbc_gwindow(gwind_box.x0, gwind_box.y0, gwind_box.x1-dx, gwind_box.y1-dy);

	for(i=tai->rows, h = bline; tal && i; i--, tal = tal->next, h -= line_gap)
	{
	    if (h - webfonts[WEBFONT_TTY].max_down < g->y1 && h + webfonts[WEBFONT_TTY].max_up > g->y0 )
		if (tal->text && strlen(tal->text) > tai->sx)
		    font_paint(tal->text + tai->sx, font_OSCOORDS, hpos + 10, h);
	}

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

    otextarea_free_lines(tai);
}

char *otextarea_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    int line_gap = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
    rid_textarea_item *tai;
    rid_textarea_line *tal;
    int i;
    os_error *ep;
    os_regset r;

    tai = ((rid_text_item_textarea *)ti)->area;

    /* Remember that our Y coordinate is relative to the base line. */

    y -= 10;
    y = webfonts[WEBFONT_TTY].max_up - y;
    y /= line_gap;
    y += tai->sy;

    for (i = y, tal = tai->lines; i && tal->next; i--, tal = tal->next)
	;

    tai->caret_line = tal;

    if (i)
    {
	tai->cy = y - i;
	tai->cx = strlen(tal->text);
    }
    else
    {
	int len;

	tai->cy = y;

	len = strlen(tal->text);

	if (tai->sx >= len)
	    tai->cx = len;
	else
	{
	    x -= 12;

	    r.r[0] = (int) (long) webfonts[WEBFONT_TTY].handle;
	    r.r[1] = (int) (long) tal->text + tai->sx;
	    r.r[2] = (1 << 17) | (1 << 8) | (1 << 7);
	    r.r[3] = x * MILIPOINTS_PER_OSUNIT;
	    r.r[4] = r.r[5] = r.r[6] = 0;
	    r.r[7] = len - tai->sx;

	    ep = os_swix(Font_ScanString, &r);

	    if (ep == NULL)
	    {
		/* By taking the offset from the begining of the string the X scroll is taken into account */
		tai->cx = ((char*) (long) r.r[1]) - tal->text;
	    }
	    else
	    {
		tai->cx = len;
	    }

	}
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
    font_string fs;
    os_error *ep;
    rid_textarea_item *tai;
    rid_textarea_line *tal;
    int i, h;

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

    for(tal = tai->lines, i = tai->cy; i && tal->next; i--, tal = tal->next)
	;

    if (i)
	tai->cy -= i;

    stream_find_item_location(ti, &cx, &cy);

    fs.s = tal->text + tai->sx;
    fs.x = fs.y = (1 << 30);
    fs.split = -1;
    fs.term = tai->cx - tai->sx;

    ep = font_setfont(webfonts[WEBFONT_TTY].handle);
    if (ep)
	return FALSE;

    ep = font_strwidth(&fs);
    if (ep)
	return FALSE;

    cx += 10;
    cx += (fs.x / MILIPOINTS_PER_OSUNIT);

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

BOOL otextarea_key(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int key)
{
#ifndef BUILDERS
    int flags = 0;
    rid_textarea_item *tai;
    rid_textarea_line *tal;
    int len;
    int osx, osy;

    tai = ((rid_text_item_textarea *)ti)->area;
    tal = tai->caret_line;

    osx = tai->sx;
    osy = tai->sy;

    len = strlen(tal->text);

    flags |= CHAR_USED;

    if (key >= 32 && key < 256 && key != 127)
    {
	if (tai->cx > len)
	{
	    tai->cx = len;
	    flags |= REPOS_CARET;
	}

	if (len + 1 < MAX_TEXT_LINE)
	{
	    memmove(tal->text + tai->cx + 1, tal->text + tai->cx, len + 1 - tai->cx );

	    tal->text[tai->cx] = key;
	    tai->cx++;

	    flags |= (REDRAW_LINE | REPOS_CARET);
	}
    }
    else
    {
	input_key_action action = lookup_key_action(key);
	switch (action)
	{
	case key_action_NEWLINE:
	case key_action_NEWLINE_SUBMIT_ALWAYS:
	case key_action_NEWLINE_SUBMIT_LAST:
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
	    break;
	case key_action_DELETE_LEFT:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

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
	    break;
	case key_action_DELETE_TO_END:
	    if (tai->cx > len)
	    {
		tai->cx = len;
		flags |= REPOS_CARET;
	    }

	    if (tal->text[tai->cx] != 0)
	    {
		tal->text[tai->cx] = 0;	/* Truncate the line */

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
	    break;
	case key_action_DELETE_ALL:
	    tal->text[0] = 0;
	    tai->cx = 0;
	    flags |= (REDRAW_LINE | REPOS_CARET);
	    break;
	case key_action_DELETE_ALL_AREA:
	{
	    rid_textarea_line *line = tai->lines;
	    while (line)
	    {
		line->text[0] = 0;
		line = line->next;
	    }

	    tai->caret_line = tai->lines;
	    tai->cx = tai->cy = 0;
	    flags |= (REDRAW_ALL | REPOS_CARET);
	    break;
	}
	case key_action_LEFT:
	case key_action_LEFT_OR_OFF:
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
	    tai->caret_line = tai->lines;
	    flags |= REPOS_CARET;
	    break;

	case key_action_END_OF_AREA:
	    while (tai->caret_line->next)
	    {
		tai->cy++;
		tai->caret_line = tai->caret_line->next;
	    }
	    tai->cx = strlen(tai->caret_line->text);
	    flags |= REPOS_CARET;
	    break;

	case key_action_UP:
	    if (tal->prev)
	    {
		tai->cy--;
		tai->caret_line = tal->prev;
		flags |= REPOS_CARET;
	    }
	    else
	    {
		flags &= ~CHAR_USED;
	    }
	    break;
	case key_action_DOWN:
	    if (tal->next)
	    {
		tai->cy++;
		tai->caret_line = tal->next;
		flags |= REPOS_CARET;
	    }
	    else
	    {
		flags &= ~CHAR_USED;
	    }
	    break;
	default:
	    flags &= ~CHAR_USED;
#if DEBUG
	    fprintf(stderr, "Key %d\n", key);
#endif
	    /* Ignore spurious control chars */
	    break;
	}
    }

    if (tai->cx < tai->sx || tai->cx > (tai->sx + tai->cols))
    {
	if (tai->cx < (tai->cols / 2))
	    tai->sx = 0;
	else if (tai->cx > MAX_TEXT_LINE - (tai->cols / 2))
	    tai->sx = MAX_TEXT_LINE - tai->cols;
	else
	    tai->sx = tai->cx - (tai->cols / 2);

	flags |= REDRAW_ALL;
    }

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
    rid_textarea_item *tai = ((rid_text_item_textarea *) ti)->area;

    if (box)
	memset(box, 0, sizeof(*box));

    return TRUE;
}

/* eof otextarea.c */
