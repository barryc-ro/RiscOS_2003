/* -*-c-*- */

/* oinput.c */

/*
 * 18/3/96: SJM: Added w and h paramteres to image_render
 * 20/3/96: SJM: Created oinput_update_box, used in input_key with update_item_trim
 * 13/5/96: SJM: display descaling in image click,
 *               sizes to image_render changed to -1,-1 in line with images.c change
 * 17/5/96: SJM: added ctrl-copy (as for ctrl K), made left/right not redraw at end of line
 * 04/6/96: SJM: new call to image_os_to_pixels for image coord translation
 * 11/6/96: SJM: do image scaling here rather than in image_info
 * 13/6/96: SJM: added _update_highlight method
 * 19/6/96: SJM: added ctrl click to IMAGE
 * 10/7/96: SJM: check xsize for being -1 in all places
 * 19/7/96: SJM: reworked loops for new form element lists
 * 08/8/96: SJM: Changed TEXT redraw clipping routines.
 * 28/8/96: SJM: A click anywhere in the TEXT icon gives the caret to the item.
 * 04/9/96: SJM: check for EXPIRE flag in image_find
 */

/* Methods for input objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memwatch.h"

#include "wimp.h"
#include "font.h"
#include "bbc.h"
#include "swis.h"
#include "akbd.h"
#include "colourtran.h"

#include "antweb.h"
#include "interface.h"
#include "rid.h"
#include "webfonts.h"
#include "consts.h"
#include "config.h"
#include "util.h"
#include "images.h"
#include "url.h"
#include "render.h"
#include "rcolours.h"
#include "makeerror.h"
#include "dfsupport.h"

#include "object.h"

#include "stream.h"
#include "oimage.h"
#include "gbf.h"

#if UNICODE
#include "Unicode/utf8.h"
#endif

/* ---------------------------------------------------------------------- */

#ifndef Wimp_TextOp
#define Wimp_TextOp 0x400F9
#endif

#define NUMBERS_SPACING_X	32
#define INPUT_TEXT_BORDER_X	12
#define INPUT_TEXT_BORDER_Y	8

#define DEFAULT_XSIZE		20

#define INPUT_BUTTON_BORDER_X	10

#ifdef STBWEB
#define INPUT_BUTTON_BORDER_Y	8
#else
#define INPUT_BUTTON_BORDER_Y	4
#endif

/* ---------------------------------------------------------------------- */

#define BUTTON_NAME_OPTION	4
#define BUTTON_NAME_RADIO	0
#define BUTTON_NAME_ON		2
#define BUTTON_NAME_OFF		0
#define BUTTON_NAME_HIGHLIGHT	1

static char *button_names[] =
{
    "radiooff", "radiooff1",
    "radioon", "radioon1",
    "optoff", "optoff1",
    "opton", "opton1"
};

#ifdef STBWEB
static int have_selected_sprites = -1;
#endif

/* ---------------------------------------------------------------------- */

/* extern void translate_escaped_text(char *src, char *dest, int len); */

/* ---------------------------------------------------------------------- */

static void oinput_update_box(rid_text_item *ti, wimp_box *box)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;

    memset(box, 0, sizeof(*box));
    switch (ii->tag)
    {
        case rid_it_TEXT:
        case rid_it_PASSWD:
	    if ((ii->flags & rid_if_NUMBERS) == 0)
	    {
		box->x0 =   2*5;
		box->x1 = - 2*5;
		box->y1 = - 2*5;
		box->y0 =   2*5;
	    }
            break;

        case rid_it_SUBMIT:
        case rid_it_RESET:
        case rid_it_IMAGE:
        default:
            break;
    }
}

static char *button_text(rid_input_item *ii)
{
    char *s = "";
    if (ii->value)
	s = ii->value;
    else switch (ii->tag)
    {
    case rid_it_RESET:
	s = "Reset";
	break;
    case rid_it_SUBMIT:
	s = "Submit";
	break;
    case rid_it_IMAGE:
	s = "Button";
	break;
    }
    return s;
}

/*
 * Returns the actual start plot point of a string taking into account the scrolling
 */

#ifndef BUILDERS
static int get_string_start(int whichfont, const char *str, int text_input_offset, int boxx, int boxw, int numbers)
{
    int x1, x2, slen, plotx;

    slen = strlen(str);
    x1 = webfont_font_width_n(whichfont, str, slen);			/* length of string */
    x2 = webfont_font_width_n(whichfont, str, text_input_offset);	/* length to caret */
    plotx = boxx;

    if (numbers)
    {
	x1 += (slen > 0 ? slen-1 : 0) * NUMBERS_SPACING_X;
	x2 += (text_input_offset > 0 ? text_input_offset : 0) * NUMBERS_SPACING_X;
    }

    if (x1 <= boxw)
    {
	/* The whole string fits in the box */
    }
    else
    {
	if (x2 <= (boxw >> 1) )
	{
	    /* At the left end */
	}
	else if (x2 >= (x1 - (boxw >> 1)))
	{
	    /* At the right end */
	    plotx -= x1;
	    plotx += boxw;
	}
	else
	{
	    /* Center the caret */
	    plotx -= x2;
	    plotx += (boxw >> 1);
	}
    }

    return plotx;
}
#endif

static int text_displayable_width(int xsize, antweb_doc *doc)
{
    int n = ((xsize != -1) ? xsize : DEFAULT_XSIZE) * doc->scale_value / 100;
    if (n == 0)
	n = 1;
    return n;
}

static int oinput_image_renderable(rid_input_item *ii, void *im, antweb_doc *doc)
{
    int fl;
    if (im == NULL)
	return FALSE;

    if (doc->flags & doc_flag_NO_PICS)
	return FALSE;

    image_info((image) im, 0, 0, 0, &fl, 0, 0);
    if (fl & image_flag_REALTHING)
	return TRUE;

    return FALSE;
}

static int insert_char(rid_input_item *ii, int pos, int c)
{
    char *str = ii->data.str;
    int len, clen, nchars;

    nchars = len = strlen(str);
    clen = 1;
#if UNICODE
    if (config_encoding_internal == 1)
    {
	nchars = UTF8_strlen(str);
	clen = UTF8_codelen(c);
    }
#endif

    if (nchars + 1 > ii->max_len)
	return 0;
    
    memmove(str + pos + clen, str + pos, len - pos + 1); /* +1 to move the NUL also */
#if UNICODE
    if (config_encoding_internal == 1)
    {
	UCS4_to_UTF8(str + pos, c);
    }
    else
#endif
    {
	str[pos] = c;
    }
    
    return clen;
}

static int delete_char(rid_input_item *ii, int pos)
{
    char *str = ii->data.str;
    int len = strlen(str);
    int clen;
#if UNICODE
    clen = UTF8_seqlen(str[pos]);
#else
    clen = 1;
#endif

    memmove(str + pos, str + (pos + clen), len - (pos + clen) + 1); /* +1 to move the NUL also */
    
    return clen;
}

static int move_point(rid_input_item *ii, int pos, int dir)
{
#if UNICODE
    char *s = ii->data.str + pos;
    if (config_encoding_internal == 1)
    {
	if (dir < 0)
	    s = UTF8_prev(s);
	else if (dir > 0)
	    s = UTF8_next(s);
    }
    else
	s += dir;
    
    return s - ii->data.str;
#else
    return pos + dir;
#endif
}

/* ---------------------------------------------------------------------- */

void oinput_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii;
    char *t;
    int height, width;
    image_flags fl;

    int whichfont;
    webfont *wf;

    ii = tii->input;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	if (ii->data.image.im == NULL)
	    ii->data.image.im = oimage_fetch_image(doc, ii->src, ii->ww.type == value_none || ii->hh.type == value_none);

	image_info((image) ii->data.image.im, &width, &height, 0, &fl, 0, 0);

	if (fl & image_flag_REALTHING)
	    ii->data.image.flags |= rid_image_flag_REAL;

	antweb_doc_ensure_font( doc, ALT_FONT );

	oimage_size_image(doc, button_text(ii), &ii->ww, &ii->hh, ii->data.image.flags, ti, doc->scale_value, fwidth, &width, &height);

	width += ii->bw*2*2;
	height += ii->bw*2*2;

	ti->width = width;
	ti->pad = 0;

	antweb_doc_ensure_font( doc, WEBFONT_BASE );

	if (ii->data.image.flags & rid_image_flag_ATOP)
	{
	    ti->max_up = webfonts[WEBFONT_BASE].max_up;
	}
	else if (ii->data.image.flags & rid_image_flag_ABOT)
	{
	    ti->max_up = height;
	}
	else
	{
	    ti->max_up = (height + webfonts[WEBFONT_BASE].max_up) >> 1;
	}
	ti->max_down = height - ti->max_up;
	break;

    case rid_it_TEXT:
    case rid_it_PASSWD:
	whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);

	if (ii->ww.type == value_absunit)
	    ti->width = (int)ii->ww.u.f;
	else
	{
	    int n = ii->flags & rid_if_NUMBERS ? (ii->xsize == -1 ? DEFAULT_XSIZE : ii->xsize) : text_displayable_width(ii->xsize, doc);
  	    ti->width = webfont_nominal_width(whichfont, n) + 2*INPUT_TEXT_BORDER_X + (ii->flags & rid_if_NUMBERS ? (n-1)*NUMBERS_SPACING_X : 0);
/*  	    ti->width = webfont_tty_width(n, 1) + 2*INPUT_TEXT_BORDER_X + (ii->flags & rid_if_NUMBERS ? (n-1)*NUMBERS_SPACING_X : 0); */
	}

	ti->max_up = webfonts[whichfont].max_up + INPUT_TEXT_BORDER_Y;
	ti->max_down = webfonts[whichfont].max_down + INPUT_TEXT_BORDER_Y;

	if (ii->hh.type == value_absunit)
	{
	    int extra = (int)ii->hh.u.f - (webfonts[whichfont].max_up + webfonts[whichfont].max_down + 2*INPUT_TEXT_BORDER_Y);
	    ti->max_up += extra/2;
	    ti->max_down += extra/2;
	}
	break;

    case rid_it_BUTTON:
    case rid_it_SUBMIT:
    case rid_it_RESET:

#ifndef BUILDERS
	whichfont = antweb_getwebfont(doc, ti, -1);
#else
	whichfont = ii->src ? ALT_FONT : WEBFONT_BUTTON;
        antweb_doc_ensure_font( doc, whichfont );
#endif
	wf = &webfonts[whichfont];

	t = button_text(ii);
	if (ii->src)
	{
	    if (ii->data.button.im == NULL)
		ii->data.button.im = oimage_fetch_image(doc, ii->src, ii->ww.type == value_none || ii->hh.type == value_none);

	    if (ii->src_sel && ii->data.button.im_sel == NULL)
		ii->data.button.im_sel = oimage_fetch_image(doc, ii->src_sel, FALSE);

	    image_info((image) ii->data.button.im, &width, &height, 0, &fl, 0, 0);

	    if (fl & image_flag_REALTHING)
		ii->data.button.flags |= rid_image_flag_REAL;

	    oimage_size_image(doc, t, &ii->ww, &ii->hh, ii->data.button.flags, ti, doc->scale_value, fwidth, &width, &height);

	    ti->width = width;
	    ti->max_up = (height - webfonts[WEBFONT_BUTTON].max_down + webfonts[WEBFONT_BUTTON].max_up)/2;
	    ti->max_down = height - ti->max_up;
	}
	else
	{
#ifndef BUILDERS
	    ti->width = ii->ww.type == value_absunit ? (int)ii->ww.u.f : webfont_font_width(whichfont, t) + INPUT_BUTTON_BORDER_X*2;
	    if (ii->hh.type != value_absunit)
	    {
		ti->max_up = wf->max_up + INPUT_BUTTON_BORDER_Y;
		ti->max_down = wf->max_down + INPUT_BUTTON_BORDER_Y;
	    }
	    else
	    {
		ti->max_up = ((int)ii->hh.u.f - wf->max_down + wf->max_up)/2;
		ti->max_down = (int)ii->hh.u.f - ti->max_up;
	    }
#else
	    ti->width = webfont_font_width(WEBFONT_BUTTON, t) + INPUT_BUTTON_BORDER_X*2;
	    ti->max_up = webfonts[WEBFONT_BUTTON].max_up + INPUT_BUTTON_BORDER_Y;
	    ti->max_down = webfonts[WEBFONT_BUTTON].max_down + INPUT_BUTTON_BORDER_Y;
#endif
	}
	break;

    case rid_it_CHECK:
    case rid_it_RADIO:
#ifdef STBWEB
	if (have_selected_sprites == -1)
	    have_selected_sprites = render_sprite_locate(button_names[ 0 + BUTTON_NAME_HIGHLIGHT ], NULL) != NULL;
#endif
	if (ii->src && ii->src_sel)
	{
	    antweb_doc_ensure_font( doc, WEBFONT_BUTTON );

	    if (ii->data.radio.im_off == NULL)
		ii->data.radio.im_off = oimage_fetch_image(doc, ii->src, ii->ww.type == value_none || ii->hh.type == value_none);

	    if (ii->data.radio.im_on == NULL)
		ii->data.radio.im_on = oimage_fetch_image(doc, ii->src_sel, FALSE);

	    image_info((image) ii->data.radio.im_off, &width, &height, 0, &fl, 0, 0);

	    if (fl & image_flag_REALTHING)
		ii->data.radio.flags |= rid_image_flag_REAL;

	    oimage_size_image(doc, "*", &ii->ww, &ii->hh, ii->data.radio.flags, ti, doc->scale_value, fwidth, &width, &height);

	    width += ii->bw*2*2;
	    height += ii->bw*2*2;

	    ti->width = width;
	    ti->max_up = (height - webfonts[WEBFONT_BUTTON].max_down + webfonts[WEBFONT_BUTTON].max_up)/2;
	    ti->max_down = height - ti->max_up;
	}
	else
	{
	    antweb_doc_ensure_font( doc, WEBFONT_BASE );

	    ti->width = CHAR_HEIGHT + 16;
	    ti->max_up = (CHAR_HEIGHT + 16 + webfonts[WEBFONT_BASE].max_up) / 2;
	    ti->max_down = (CHAR_HEIGHT + 16) - ti->max_up;
	}
	break;

    default:
        antweb_doc_ensure_font( doc, WEBFONT_BASE );
	ti->width = CHAR_HEIGHT + 16;
	ti->max_up = (CHAR_HEIGHT + 16 + webfonts[WEBFONT_BASE].max_up) / 2;
	ti->max_down = (CHAR_HEIGHT + 16) - ti->max_up;
	break;
    }
    ti->pad = 4;
}

void oinput_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
#ifndef BUILDERS
    oinput_size_allocate(ti, rh, doc, 0);
#else
    ti->width = 10;		/* yuch! */
    ti->max_up = 5;
    ti->max_down = 0;
#endif
}

void oinput_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;
    int plotx;
    int slen;
    int fg, bg;
    char *t;
    int whichfont;
    struct webfont *wf;
    BOOL selected;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT)
    {
	if (oinput_update_highlight(ti, doc, 0, NULL))
	    highlight_render_outline(ti, doc, hpos, bline);
	return;
    }

    if (update == object_redraw_BACKGROUND)
	return;
    
    selected = backend_is_selected(doc, ti);

    switch (ii->tag)
    {
    case rid_it_IMAGE:
    {
	wimp_box bbox;
	int bw = ii->bw;

	bbox.x0 = hpos + 0 + bw*2;
	bbox.y0 = bline - ti->max_down + 0 + bw*2;
	bbox.x1 = bbox.x0 + ti->width - 0 - bw*2*2;
	bbox.y1 = bbox.y0 + ti->max_up + ti->max_down - 0 - bw*2*2;

	if (oinput_image_renderable(ii, ii->data.image.im, doc))
	{
	    int oox = ox, ooy = oy;
#if USE_MARGINS
	    oox -= doc->margin.x0;
	    ooy -= doc->margin.y1;
#endif /* USE_MARGINS */
	    image_render((image) ii->data.image.im, bbox.x0, bbox.y0,
			 (bbox.x1 - bbox.x0)/2,
			 (bbox.y1 - bbox.y0)/2,
			 doc->scale_value, antweb_render_background, doc,
			 oox, ooy);
	}
	else
	{
	    render_plinth(render_colour_BACK, render_plinth_NOFILL | render_plinth_DOUBLE,
			  bbox.x0, bbox.y0, bbox.x1 - bbox.x0, bbox.y1 - bbox.y0, doc);

	    oimage_render_text(ti, doc, fs, &bbox, button_text(ii));
	}
	break;
    }
    case rid_it_TEXT:
    case rid_it_PASSWD:
    {
	int has_caret;
	whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
	plotx = hpos + 10;

	slen = strlen(ii->data.str);
	has_caret = be_item_has_caret(doc, ti);

	fg = ii->base.colours.back == -1 && ii->base.colours.select == -1 ? render_colour_RGB | config_colours[render_colour_PLAIN].word :
	    render_text_link_colour(ti, doc);

	bg = has_caret && ii->base.colours.select != -1 ? ii->base.colours.select | render_colour_RGB :
	    ii->base.colours.back != -1 ? ii->base.colours.back | render_colour_RGB :
	    render_colour_WRITE;

	if (fs->lfc != fg || fs->lbc != bg)
	{
	    fs->lfc = fg;
	    fs->lbc = bg;
	    render_set_font_colours(fg, bg, doc);
	}

	if (has_caret)
	{
	    plotx = get_string_start(whichfont, ii->data.str, doc->selection.data.text.input_offset,
				     plotx, ii->base.display->width - 2*INPUT_TEXT_BORDER_X,
				     ii->flags & rid_if_NUMBERS);
	}

#ifdef STBWEB
	if (ii->flags & rid_if_NUMBERS)
	{
	    int i, char_width = webfont_nominal_width(whichfont, 1); /* webfont_tty_width(1, TRUE) */
	    int n = ii->xsize != -1 ? ii->xsize : DEFAULT_XSIZE;
	    int bg1 = ii->base.colours.back == -1 ? render_colour_WRITE : ii->base.colours.back | render_colour_RGB;

	    for (i = 0; i < n; i++)
	    {
		BOOL select = has_caret && i == doc->selection.data.text.input_offset;
		render_plinth_from_list(select ? bg : bg1,
					select && config_colour_list[render_colour_list_WRITE_HIGHLIGHT] ?
					  config_colour_list[render_colour_list_WRITE_HIGHLIGHT] :
					  config_colour_list[render_colour_list_WRITE],
					0,
					hpos + i * (NUMBERS_SPACING_X + char_width),
					bline - ti->max_down,
					char_width + 2*INPUT_TEXT_BORDER_X, (ti->max_up + ti->max_down),
					doc );
	    }
	}
	else
	{
	    render_plinth_from_list(bg,
				    has_caret && config_colour_list[render_colour_list_WRITE_HIGHLIGHT] ?
				      config_colour_list[render_colour_list_WRITE_HIGHLIGHT] :
				      config_colour_list[render_colour_list_WRITE],
				    0,
				    hpos, bline - ti->max_down,
				    ti->width, (ti->max_up + ti->max_down), doc );
	}
#else
	render_plinth(bg, render_plinth_RIM | render_plinth_IN,
		      hpos, bline - ti->max_down,
		      ti->width, (ti->max_up + ti->max_down), doc );
#endif

	/* Check for whether the text needs redrawing if it does
	 * use the intersection of graphics window and text box
	 */
	{
	    wimp_box ta_box, gwind_box;
	    int dx = frontend_dx, dy = frontend_dy;

	    ta_box.x0 = hpos + INPUT_TEXT_BORDER_X;
	    ta_box.y0 = bline-ti->max_down+8;
	    ta_box.x1 = hpos + ti->width - INPUT_TEXT_BORDER_X;
	    ta_box.y1 = bline+ti->max_up-8;
	    if (coords_intersection(&ta_box, g, &gwind_box))
	    {
		char buf[128];
		char *str;
		int coords[8];

		coords[0] = coords[1] = 0;
		coords[2] = NUMBERS_SPACING_X * MILIPOINTS_PER_OSUNIT;
		coords[3] = 0;

		bbc_gwindow(gwind_box.x0, gwind_box.y0, gwind_box.x1-dx, gwind_box.y1-dy);

		if (ii->tag == rid_it_PASSWD)
		{
		    if (slen >= sizeof(buf)) /* safe but inaccurate (if we have > 128 char passwords) */
			slen = sizeof(buf)-1;
		    memset(buf, config_display_char_password, slen);
		    buf[slen] = 0;
		    str = buf;
		}
		else
		    str = ii->data.str;

		render_text_full(doc, whichfont, str, plotx, bline, ii->flags & rid_if_NUMBERS ? coords : NULL, -1);
		bbc_gwindow(g->x0, g->y0, g->x1-dx, g->y1-dy);
	    }
	}
	break;
    }

    case rid_it_SUBMIT:
    case rid_it_RESET:
    case rid_it_BUTTON:
    {
	int split, width, t_len;

	fg = ii->base.colours.back == -1 && ii->base.colours.select == -1 && ii->data.button.im == NULL ? render_colour_INPUT_F :
	    render_text_link_colour(ti, doc);
	bg = 0;

	t = button_text(ii);
	t_len = strlen(t);

#ifdef STBWEB
	whichfont = antweb_getwebfont(doc, ti, -1);
#else
	whichfont = ii->src ? ALT_FONT : WEBFONT_BUTTON;
        antweb_doc_ensure_font( doc, whichfont );
#endif
	wf = &webfonts[whichfont];

	if (ii->data.button.im)
	{
	    int oox = ox, ooy = oy;
	    image im;
#if USE_MARGINS
	    oox -= doc->margin.x0;
	    ooy -= doc->margin.y1;
#endif
	    if (selected && ii->data.button.im_sel)
		im = (image)ii->data.button.im_sel;
	    else
		im = (image)ii->data.button.im;

	    if (oinput_image_renderable(ii, im, doc))
	    {
		image_render(im,
			 hpos, bline - ti->max_down,
			 ti->width/2, (ti->max_up + ti->max_down)/2,
			 doc->scale_value, antweb_render_background, doc, oox, ooy);
	    }
	    else
	    {
		render_plinth(render_colour_BACK, render_plinth_NOFILL | render_plinth_DOUBLE,
			      hpos, bline - ti->max_down,
			      ti->width, (ti->max_up + ti->max_down), doc);

	    }

	    plotx = (ti->width - webfont_font_width(whichfont, t))/2;
	    if (plotx < INPUT_BUTTON_BORDER_X)
		plotx = INPUT_BUTTON_BORDER_X;
	}
	else
	{
	    if (ii->data.button.tick)
	    {
		bg = render_colour_ACTION;
	    }
	    else if (selected)
	    {
		if (ii->base.colours.select == -1)
		    bg = render_colour_INPUT_S;
		else
		    bg = ii->base.colours.select | render_colour_RGB;
		fg = render_colour_INPUT_FS;
	    }
	    else
		bg = (ii->base.colours.back == -1 ? render_colour_INPUT_B : ii->base.colours.back | render_colour_RGB);

#ifdef STBWEB
	    render_plinth_from_list(bg,
				    selected && config_colour_list[render_colour_list_BUTTON_HIGHLIGHT] ?
					config_colour_list[render_colour_list_BUTTON_HIGHLIGHT] :
					config_colour_list[render_colour_list_BUTTON],
				    0,
				    hpos, bline - ti->max_down,
				    ti->width, (ti->max_up + ti->max_down), doc );
#else
	    render_plinth(bg,
			  ii->data.button.tick ? render_plinth_IN : 0,
			  hpos, bline - ti->max_down,
			  ti->width, (ti->max_up + ti->max_down), doc );
#endif
	    plotx = INPUT_BUTTON_BORDER_X;
	}

	if (fs->lfc != fg || fs->lbc != bg)
	{
	    fs->lfc = fg;
	    fs->lbc = bg;
	    render_set_font_colours(fg, bg, doc);
	}

	/* see if it will fit */
	split = t_len;
	width = 0;
	if (ii->data.button.im || ii->ww.type == value_absunit)
	{
	    split = webfont_split_point(whichfont, t, ti->width - INPUT_BUTTON_BORDER_X*2);

	    /* if not then make room for the ... */
	    if (split != t_len)
	    {
		split -= 3;
		width = webfont_font_width_n(whichfont, t, split);
	    }
	}

	RENDBG(("oinput_redraw: wf_index %x wf->handle %d split %d width %d\n", ti->st.wf_index, wf->handle, split, width));

	/* plot the main string */
	render_text_full(doc, whichfont, t, hpos + plotx, bline, NULL, split);

	/* write ... if didn't fit */
	if (split != t_len)
	{
	    render_text_full(doc, whichfont, "...", hpos + plotx + width, bline, NULL, split);
	}
	break;
    }

    case rid_it_RADIO:
    case rid_it_CHECK:
    {
	void *im = ii->data.radio.tick ? ii->data.radio.im_on : ii->data.radio.im_off;
	if (im)
	{
	    int oox = ox, ooy = oy;
#if USE_MARGINS
	    oox -= doc->margin.x0;
	    ooy -= doc->margin.y1;
#endif
	    image_render(im,
			 hpos + ii->bw*2, bline - ti->max_down + ii->bw*2,
			 ti->width/2 - ii->bw*2, (ti->max_up + ti->max_down)/2 - ii->bw*2,
			 doc->scale_value, antweb_render_background, doc, oox, ooy);
	}
	else
	{
	    /* try and draw a different sprite (suffix '1') for the highlighted version of the radio/check boxes */
	    int index = (ii->tag == rid_it_RADIO ? BUTTON_NAME_RADIO : BUTTON_NAME_OPTION) + (ii->data.radio.tick ? BUTTON_NAME_ON : BUTTON_NAME_OFF);

#ifdef STBWEB
	    if (selected && have_selected_sprites)
		render_plot_icon(button_names[index + BUTTON_NAME_HIGHLIGHT], hpos + 4, bline - ti->max_down + 4);
	    else
#endif
		render_plot_icon(button_names[index], hpos + ii->bw*2, bline - ti->max_down + ii->bw*2);
	}
	break;
    }
    default:
	break;
    }
#endif
}

void oinput_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	image_loose((image) ii->data.image.im, &antweb_doc_image_change, doc);
	break;

    case rid_it_SUBMIT:
    case rid_it_RESET:
    case rid_it_BUTTON:
	image_loose((image) ii->data.button.im, &antweb_doc_image_change, doc);
	image_loose((image) ii->data.button.im_sel, &antweb_doc_image_change, doc);
	break;

    case rid_it_RADIO:
    case rid_it_CHECK:
	image_loose((image) ii->data.radio.im_off, &antweb_doc_image_change, doc);
	image_loose((image) ii->data.radio.im_on, &antweb_doc_image_change, doc);
	break;
    }
    /* Don't free the input item itself, these are taken care of by the form */
}

char *oinput_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    rid_input_item *ii;
    rid_form_element *ife;
    int flags;
    int redraw;

    redraw = 0;

    ii = ((rid_text_item_input *) ti)->input;

    if (ii->base.parent == NULL)
    {
	frontend_complain(makeerror(ERR_BAD_FORM));
	return NULL;
    }

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	image_info((image) ii->data.image.im, NULL, NULL, NULL, &flags, NULL, NULL);

	if (flags & image_flag_DEFERRED)
	{
	    frontend_complain(image_flush((image) ii->data.image.im, 0));
	}
#if 0				/* SJM: 190297 this should never have been there really */
	else if (akbd_pollctl())
	{
            backend_select_item(doc, ti, -1);
            antweb_place_caret(doc);
	}
#endif
	else
	{
	    x = x - ii->bw*2;
	    y = (ti->max_up - ii->bw*2)-y;

            image_os_to_pixels((image) ii->data.image.im, &x, &y, doc->scale_value);

	    ii->data.image.x = x;
	    ii->data.image.y = y;

	    for (ife = ii->base.parent->kids; ife; ife = ife->next)
	    {
		if (ife->tag == rid_form_element_INPUT)
		{
		    rid_input_item *ii2 = (rid_input_item *)ife;

		    if (ii2->tag == rid_it_IMAGE && ii2 != ii)
		    {
			ii2->data.image.x = -1;
			ii2->data.image.y = -1;
		    }
		    if (ii2->tag == rid_it_SUBMIT)
		    {
			ii2->data.button.tick = 0;
		    }
		}
	    }

	    sound_event(snd_FORM_SUBMIT);
	    antweb_submit_form(doc, ii->base.parent, bb & wimp_BRIGHT);
	}

	break;
    case rid_it_TEXT:
    case rid_it_PASSWD:
	{
	    os_error *ep;
	    os_regset r;
	    int len = strlen(ii->data.str);
	    int coords[8];
	    int text_input_offset;
	    int whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
	    BOOL update = (ii->flags & rid_if_NUMBERS) != 0;

	    x-=INPUT_TEXT_BORDER_X;

	    /* take into account scrolled strings */
	    if (be_item_has_caret(doc, ti))
	    {
		int xoffset = get_string_start(whichfont, ii->data.str, doc->selection.data.text.input_offset,
				     0, ii->base.display->width - 2*INPUT_TEXT_BORDER_X,
				     ii->flags & rid_if_NUMBERS);
		x -= xoffset;
		update = TRUE;
	    }

	    coords[0] = coords[1] = 0;
	    coords[2] = NUMBERS_SPACING_X * MILIPOINTS_PER_OSUNIT;
	    coords[3] = 0;
	    coords[4] = -1;

	    text_input_offset = webfont_get_offset(whichfont, ii->data.str, x,
						   ii->flags & rid_if_NUMBERS ? coords : NULL, len);

	    LNKDBG(( "Caret set to item %p, offset %d\n", ti, text_input_offset));

	    antweb_place_caret(doc, ti, text_input_offset);

	    /* with numbers - if the focus is repositioned then it needs redrawing */
	    if (update)
		antweb_update_item(doc, ti);
	}
	break;
    case rid_it_CHECK:
	sound_event(snd_CHECKBOX_TOGGLE);
	ii->data.radio.tick = !ii->data.radio.tick;
	redraw = TRUE;
	break;
    case rid_it_RADIO:
	sound_event(snd_RADIO_TOGGLE);
	if (ii->data.radio.tick == FALSE)
	{
	    ii->data.radio.tick = TRUE;
	    for (ife = ii->base.parent->kids; ife; ife = ife->next)
	    {
		if (ife->tag == rid_form_element_INPUT)
		{
		    rid_input_item *iis = (rid_input_item *)ife;
		    if (iis->tag == rid_it_RADIO && ii != iis && strcasecomp(ii->name, iis->name) == 0 &&
			iis->data.radio.tick != FALSE) /* only update if needs to change */
		    {
			iis->data.radio.tick = FALSE;
			antweb_update_item(doc, iis->base.display);
		    }
		}
	    }

	    redraw = TRUE;
	}
	break;
    case rid_it_SUBMIT:
	sound_event(snd_FORM_SUBMIT);

	for (ife = ii->base.parent->kids; ife; ife = ife->next)
	{
	    if (ife->tag == rid_form_element_INPUT)
	    {
		rid_input_item *ii2 = (rid_input_item *)ife;

		if (ii2->tag == rid_it_IMAGE)
		{
		    ii2->data.image.x = -1;
		    ii2->data.image.y = -1;
		}
		if (ii2->tag == rid_it_SUBMIT && ii2 != ii)
		{
		    ii2->data.button.tick = 0;
		}
	    }
	}

	ii->data.button.tick = TRUE;
	antweb_update_item(doc, ti);

	antweb_submit_form(doc, ii->base.parent, bb & wimp_BRIGHT);

	ii->data.button.tick = FALSE;
	redraw = TRUE;
	break;

    case rid_it_RESET:
	sound_event(snd_FORM_RESET);

	ii->data.button.tick = TRUE;
	antweb_update_item(doc, ti);

	for (ife = ii->base.parent->kids; ife; ife = ife->next)
	{
	    switch (ife->tag)
	    {
	    case rid_form_element_INPUT:
	    {
		rid_input_item *iis = (rid_input_item *)ife;
		int touch = 0;

		switch(iis->tag)
		{
		case rid_it_TEXT:
		case rid_it_PASSWD:
		    if (iis->value)
		    {
/* 			translate_escaped_text(iis->value, iis->data.str, iis->max_len); */
			strcpy(iis->data.str, iis->value);
		    }
		    else
		    {
			iis->data.str[0] = 0;
		    }

		    /* SJM: removed 1/4/97: the caret is repositioned at the end anyway */
/* 		    if (be_item_has_caret(doc, iis->base.display)) */
/* 		    { */
/* 			int len = strlen(iis->data.str); */
/* 			if (doc->selection.data.text.input_offset > len) */
/* 			    doc->selection.data.text.input_offset = len; */
/* 		    } */
		    touch = 1;
		    break;
		case rid_it_CHECK:
		case rid_it_RADIO:
		    iis->data.radio.tick = iis->flags & rid_if_CHECKED;
		    touch = 1;
		    break;
		}
		if (touch)
		    antweb_update_item(doc, iis->base.display);
		break;
	    }

	    case rid_form_element_TEXTAREA:
	    {
		rid_textarea_item *tai = (rid_textarea_item *)ife;
		(object_table[tai->base.display->tag].size)(tai->base.display, rh, doc);
		antweb_update_item(doc, tai->base.display);
		break;
	    }

	    case rid_form_element_SELECT:
	    {
		rid_select_item *sis = (rid_select_item *)ife;
		rid_option_item *ois;
		for(ois = sis->options; ois; ois = ois->next)
		{
		    if (ois->flags & rid_if_CHECKED)
			ois->flags |=  rid_if_SELECTED;
		    else
			ois->flags &= ~rid_if_SELECTED;
		}
		antweb_update_item(doc, sis->base.display);
		break;
	    }
	    }
	}

	if (doc->selection.tag == doc_selection_tag_TEXT &&
	    object_table[doc->selection.data.text.item->tag].caret)
	{
	    doc->selection.data.text.input_offset = -1;
	    (object_table[doc->selection.data.text.item->tag].caret)(doc->selection.data.text.item, doc->rh, doc, TRUE);
	}

	ii->data.button.tick = FALSE;
	redraw = TRUE;
	break;
    }

    if (redraw)
    {
	antweb_update_item(doc, ti);
    }
#endif /* BUILDERS */
    return NULL;		/* Do not follow links */
}

void oinput_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    rid_input_item *ii;
    int i;

    ii = ((rid_text_item_input *) ti)->input;

    switch (ii->tag)
    {
    case rid_it_TEXT:
    {
        int xsize = ii->xsize != -1 ? ii->xsize : DEFAULT_XSIZE;
	fputc('[', f);
	for(i=0; ii->data.str[i] && i < xsize; i++)
	    fputc(ii->data.str[i], f);
	for( ; i < xsize; i++)
	    fputc(' ', f);
	fputc(']', f);
	break;
    }
    case rid_it_PASSWD:
    {
        int xsize = ii->xsize != -1 ? ii->xsize : DEFAULT_XSIZE;
	fputc('[', f);
	for(i=0; ii->data.str[i] && i < xsize; i++)
	    fputc('-', f);
	for( ; i < xsize; i++)
	    fputc(' ', f);
       	fputc('[', f);
	break;
    }
    case rid_it_CHECK:
    case rid_it_RADIO:
	fputc('[', f);
	fputc(ii->data.radio.tick ? 'x' : ' ', f);
	fputc(']', f);
	break;
    case rid_it_RESET:
    case rid_it_SUBMIT:
    case rid_it_BUTTON:
	fputc('[', f);
	fputs(button_text(ii), f);
	fputc(']', f);
	break;
    }
}

BOOL oinput_caret(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int repos)
{
#ifndef BUILDERS
    rid_input_item *ii;
    int take_it = FALSE;
    os_error *ep;
    int x1, x2;
    int boxw;
    int cx, cy;
    int h;
    int slen;
    int whichfont;

    LNKDBG(( "oinput_caret: repos=%d\n", repos));

    ii = ((rid_text_item_input *) ti)->input;

    switch (ii->tag)
    {
    case rid_it_TEXT:
    case rid_it_PASSWD:
	if (repos == object_caret_BLUR)
	{
#ifdef STBWEB
	    antweb_update_item(doc, ti);
#else
	    if (ii->base.colours.select != -1)
		antweb_update_item(doc, ti);
#endif
	    return FALSE;
	}

	whichfont = antweb_getwebfont(doc, ti, WEBFONT_TTY);
	slen = strlen(ii->data.str);

	if (doc->selection.data.text.input_offset < 0)
	{
	    doc->selection.data.text.input_offset = (ii->flags & rid_if_NUMBERS) ? 0 : slen;
	    if (repos == object_caret_REDISPLAY)
		repos = object_caret_REPOSITION;
	}
	if (doc->selection.data.text.input_offset >= ii->max_len && (ii->flags & rid_if_NUMBERS))
	{
	    doc->selection.data.text.input_offset = ii->max_len-1;
	    if (repos == object_caret_REDISPLAY)
		repos = object_caret_REPOSITION;
	}

	if (repos == object_caret_FOCUS)
	    antweb_update_item(doc, ti);

	stream_find_item_location(ti, &cx, &cy);
	x1 = webfont_font_width_n(whichfont, ii->data.str, doc->selection.data.text.input_offset);
	x2 = webfont_font_width_n(whichfont, ii->data.str, slen);

	if (ii->flags & rid_if_NUMBERS)
	{
	    x1 += (doc->selection.data.text.input_offset > 0 ? doc->selection.data.text.input_offset : 0) * NUMBERS_SPACING_X;
	    x2 += (slen > 0 ? slen-1 : 0) * NUMBERS_SPACING_X;
	}

	boxw = ii->base.display->width - 2*INPUT_TEXT_BORDER_X;
	cx += 10;

	if (x2 <= boxw)
	{
	    /* The whole string fits in the box */
	    cx += x1;
	}
	else
	{
	    if (x1 <= (boxw >> 1) )
	    {
		cx += x1;
	    }
	    else if (x1 >= (x2 - (boxw >> 1)))
	    {
		cx += (boxw - (x2 - x1));
	    }
	    else
	    {
		cx += (boxw >> 1);
	    }
	}

	cy -= webfonts[whichfont].max_down;

#if USE_MARGINS
	cx += doc->margin.x0;
	cy += doc->margin.y1;
#endif
	h = webfonts[whichfont].max_up + webfonts[whichfont].max_down;
	h |= render_caret_colour(doc, ii->base.colours.select != -1 ? ii->base.colours.select : ii->base.colours.back, ii->base.colours.cursor);

	frontend_view_caret(doc->parent, cx, cy, h, repos == object_caret_REPOSITION || repos == object_caret_FOCUS);

	take_it = TRUE;
	break;
    default:
	break;
    }

    return take_it;
#else
    return FALSE;
#endif /* BUILDERS */
}


BOOL oinput_key(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int key)
{
    rid_input_item *ii;
    int used = FALSE;
    int len;
    int redraw = FALSE;
    int i;

    ii = ((rid_text_item_input *) ti)->input;

    LNKDBG(( "oinput_key(): key %d tag %d text_input_offset %d\n", key, ii->tag, doc->selection.data.text.input_offset));

    i = doc->selection.data.text.input_offset;

    switch (ii->tag)
    {
    case rid_it_TEXT:
    case rid_it_PASSWD:
	len = strlen(ii->data.str);

	if (key >= 32 && key != 127
#if !UNICODE
	    && key < 256
#endif
	    )
	{
	    if (ii->flags & rid_if_NUMBERS)
	    {
		/* since numbers are all ascii don't do special code for now */
		if ( isdigit(key) ||
		    ((ii->flags & rid_if_PBX) && (key == '#' || key == ',' || key == '*')) )
		{
		    if (i >= ii->max_len)
			i = ii->max_len-1;

		    ii->data.str[i] = key;
		    if (i == len)
			ii->data.str[i+1] = 0;

		    /* if fall off the end then counts as not used */
		    if (doc->selection.data.text.input_offset < ii->max_len-1)
		    {
			doc->selection.data.text.input_offset++;
			used = TRUE;
		    }
		    else
			used = be_doc_key_FILLED;

		    redraw = TRUE;
		}
		else
		{
		    used = TRUE;
		    sound_event(snd_WARN_BAD_KEY);
		}
	    }
	    else
	    {
#if 1
		int offset = insert_char(ii, i, key);
		if (offset > 0)
		{
		    doc->selection.data.text.input_offset += offset;
		    redraw = TRUE;
		}
#else
		if (len+1 <= ii->max_len)	/* 15/8/96: DAF: < to <= */
		{
 		    memmove(ii->data.str + i + 1, ii->data.str + i, (len + 1 - i));
 		    ii->data.str[i] = key;
 		    doc->selection.data.text.input_offset++;
		    redraw = TRUE;
		}
#endif
		else
		{
		    sound_event(snd_WARN_BAD_KEY);
		}
		used = TRUE;
	    }
	}
	else
	{
	    int action;
#if UNICODE
	    if (key < 0)
		key = -key;
#endif
	    action = lookup_key_action(key);
	    switch (action)
	    {
	    case key_action_NEWLINE:
	    case key_action_NEWLINE_SUBMIT_ALWAYS:
	    case key_action_NEWLINE_SUBMIT_LAST:
		/* ii->base.parent->last_text== NULL : SJM this said
		don't do a possible submit if a TEXTAREA was in the
		form. I don't understand what that has to do with
		anything so removed it */

		if (ii->base.parent) 
		{
		    rid_input_item *first, *last;
		    rid_form_element *ife;

		    first = last = NULL;

		    /* scan through all the input items, clear the IMAGE settings and tick the first SUBMIT button */
		    for (ife = ii->base.parent->kids; ife; ife = ife->next)
		    {
			if (ife->tag == rid_form_element_INPUT)
			{
			    rid_input_item *ins = (rid_input_item *)ife;

			    if (ins->tag == rid_it_IMAGE)
			    {
				ins->data.image.x = -1;
				ins->data.image.y = -1;
			    }
			    if (ins->tag == rid_it_SUBMIT)
			    {
				if (first == NULL)
				{
				    ins->data.button.tick = 1;
				    first = ins;
				}
				else
				    ins->data.button.tick = 0;
			    }

			    /* standard behaviour, if a passwd or there is more than one TEXT then don't submit */
			    if (action == key_action_NEWLINE)
			    {
				if (ins->tag == rid_it_PASSWD || (ins->tag == rid_it_TEXT && ins != ii))
				    break;
			    }
			    /* new behaviour 1 if in the last TEXT or PASSWD of however many then submit (unless a numbers field)*/
			    else if (action == key_action_NEWLINE_SUBMIT_LAST)
			    {
				if ((ins->tag == rid_it_PASSWD || ins->tag == rid_it_TEXT) && (ins->flags & rid_if_NUMBERS) == 0)
				    last = ins;
			    }
			    /* else, new behaviour 2 always submit */
			}
		    }

		    LNKDBG(("oinput_key: ife %p action %d ii %p last %p\n", ife, action, ii, last));
		    
		    /* if we got to the end and either we were on the last text item or we didn't care then submit */
		    if (ife == NULL && (action != key_action_NEWLINE_SUBMIT_LAST || ii == last))
		    {
			/* No passwords, the only text item is this one and only the first submit button 'ticked' */
			sound_event(snd_FORM_SUBMIT);
			antweb_submit_form(doc, ii->base.parent, 0);	/* No such thing as a right click here */
			if (first)
			    first->data.button.tick = 0;

			used = be_doc_key_SUBMIT;
			redraw = TRUE;
		    }
		}
		break;
	    case key_action_DELETE_LEFT:
		if (i > 0)
		{
		    doc->selection.data.text.input_offset = move_point(ii, i, -1);
		    delete_char(ii, doc->selection.data.text.input_offset);
/* 		    memmove(ii->data.str + i - 1, ii->data.str + i, (len + 1 - i)); */
/* 		    doc->selection.data.text.input_offset--; */
		    used = redraw = TRUE;
		}
		break;
	    case key_action_DELETE_ALL:
	    case key_action_DELETE_ALL_AREA:
		ii->data.str[0] = 0;
		doc->selection.data.text.input_offset = 0;
		used = redraw = TRUE;
		break;

	    case key_action_DELETE_TO_END:
		ii->data.str[i] = 0;
		used = redraw = TRUE;
		break;

	    case key_action_DELETE_RIGHT:
		if (i < len)
		{
		    delete_char(ii, i);
/* 		    memmove(ii->data.str + i, ii->data.str + i + 1, (len - i)); */
		    used = redraw = TRUE;
		}
		break;

	    case key_action_LEFT:
		used = TRUE;
		/* deliberate fall-through */
	    case key_action_LEFT_OR_OFF:
		if (i > 0)
		{
		    doc->selection.data.text.input_offset = move_point(ii, i, -1);
/* 		    doc->selection.data.text.input_offset--; */
		    used = redraw = TRUE;
		}
		break;

	    case key_action_RIGHT:
		used = TRUE;
		/* deliberate fuall-through */
	    case key_action_RIGHT_OR_OFF:
		if (i < (ii->flags & rid_if_NUMBERS ? len-1 : len))
		{
		    doc->selection.data.text.input_offset = move_point(ii, i, +1);
/* 		    doc->selection.data.text.input_offset++; */
		    used = redraw = TRUE;
		}
		break;

	    case key_action_START_OF_LINE:
	    case key_action_START_OF_AREA:
		doc->selection.data.text.input_offset = 0;
		used = redraw = TRUE;
		break;

	    case key_action_END_OF_LINE:
	    case key_action_END_OF_AREA:
		doc->selection.data.text.input_offset = ii->flags & rid_if_NUMBERS ? len-1 : len;
		used = redraw = TRUE;
		break;

	    default:
		/* Ignore spurious control chars */
		break;
	    }
	}
	break;

    default:
	break;
    }

    if (redraw)
    {
        wimp_box box;
	oinput_caret(ti, rh, doc, object_caret_REPOSITION);

        oinput_update_box(ti, &box);
	antweb_update_item_trim(doc, ti, &box, FALSE);

/* 	used = TRUE; */
    }

    return used;
}

void *oinput_image_handle(rid_text_item *ti, antweb_doc *doc, int reason)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;
    static wimp_box box, *bp = NULL;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	switch (reason)
	{
	case object_image_HANDLE:
	case object_image_OBJECT:
	    return ii->data.image.im;

	case object_image_ABORT:
	    oimage_flush_image(ii->data.image.im);
	    break;

	case object_image_BOX:
	    memset(&box, 0, sizeof(box));
	    return &box;
	}
	break;

    case rid_it_SUBMIT:
    case rid_it_RESET:
    case rid_it_BUTTON:
	switch (reason)
	{
	case object_image_HANDLE:
	case object_image_OBJECT:
	    return backend_is_selected(doc, ti) && ii->data.button.im_sel ? ii->data.button.im_sel : ii->data.button.im;

	case object_image_ABORT:
	    oimage_flush_image(ii->data.button.im);
	    oimage_flush_image(ii->data.button.im_sel);
	    break;

	case object_image_BOX:
	    memset(&box, 0, sizeof(box));
	    return &box;
	}
	break;

    case rid_it_RADIO:
    case rid_it_CHECK:
	switch (reason)
	{
	case object_image_HANDLE:
	case object_image_OBJECT:
	    return ii->data.radio.tick ? ii->data.radio.im_off : ii->data.radio.im_on;

	case object_image_ABORT:
	    oimage_flush_image(ii->data.radio.im_off);
	    oimage_flush_image(ii->data.radio.im_on);
	    break;

	case object_image_BOX:
	    memset(&box, 0, sizeof(box));
	    return &box;
	}
	break;
    }

    return NULL;
}

void oinput_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
#ifndef BUILDERS
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;
    wimp_box tb;
    wimp_paletteword trans;

    trans.word = ~0;

    tb.x0 = x;
    tb.y0 = y - ti->max_down;
    tb.x1 = x + ti->width;
    tb.y1 = y + ti->max_up;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	if (ii->data.image.im && ((doc->flags & doc_flag_NO_PICS) == 0))
	{
	    tb.x0 += 2;
	    tb.y0 += 2;
	    tb.x1 -= 2;
	    tb.y1 -= 2;

	    image_save_as_draw((image) ii->data.image.im, fh, &tb, fileoff);
	}
	else
	{
	    df_write_filled_rect(fh, &tb, render_get_colour(render_colour_PLAIN, doc), trans,
				 fileoff);
	}
	break;
    case rid_it_RADIO:
    case rid_it_CHECK:
        {
	    char *sname;

	    sname = ((ii->tag == rid_it_RADIO) ?
		     (ii->data.radio.tick ? "radioon" : "radiooff") :
		     (ii->data.radio.tick ? "opton" : "optoff") );
	    df_write_sprite(fh, sname, x + 4, y - ti->max_down + 4, fileoff);
	}
	break;
    case rid_it_TEXT:
    case rid_it_PASSWD:
	tb.x0 += 8;
	tb.y0 += 8;
	tb.x1 -= 9;
	tb.y1 -= 9;

	df_write_filled_rect(fh, &tb, render_get_colour(render_colour_PLAIN, doc),
			     render_get_colour(render_colour_WRITE, doc),
			     fileoff);
	tb.x0 -= 9;
	tb.y0 -= 9;
	tb.x1 += 8;
	tb.y1 += 8;
	df_write_plinth(fh, &tb,
			render_get_colour(render_colour_LINE_D, doc),
			render_get_colour(render_colour_LINE_L, doc),
			fileoff);
	break;
    case rid_it_SUBMIT:
    case rid_it_RESET:
    case rid_it_BUTTON:
	df_write_plinth(fh, &tb,
			render_get_colour(render_colour_LINE_L, doc),
			render_get_colour(render_colour_LINE_D, doc),
			fileoff);
	break;
    default:
	tb.x0 = x;
	tb.y0 = y - ti->max_down;
	tb.x1 = x + ti->width;
	tb.y1 = y + ti->max_up;

	df_write_filled_rect(fh, &tb, render_get_colour(render_colour_PLAIN, doc), trans, fileoff);

	break;
    }

    df_stretch_bb(bb, &tb);
#endif /* BUILDERS */
}

/*
 */

int oinput_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box)
{
    rid_input_item *ii = ((rid_text_item_input *) ti)->input;
    BOOL own_hl = FALSE;

    if (box)
	memset(box, 0, sizeof(*box));

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	break;

    case rid_it_TEXT:
    case rid_it_PASSWD:
	break;

    case rid_it_SUBMIT:
    case rid_it_BUTTON:
    case rid_it_RESET:
	own_hl = (ii->data.button.im && ii->data.button.im_sel) ||
	    ii->base.colours.select != -1 ||
	    config_colours[render_colour_INPUT_S].word != config_colours[render_colour_INPUT_B].word ||
	    config_colour_list[render_colour_list_BUTTON_HIGHLIGHT] != NULL;
	break;

    case rid_it_CHECK:
    case rid_it_RADIO:
    {
#ifdef STBWEB
	own_hl = have_selected_sprites;
#endif
	break;
    }

    case rid_it_HIDDEN:
	own_hl = TRUE;
	break;
    }
    return !own_hl;
}

/* eof oinput.c */
