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

#ifndef Wimp_TextOp
#define Wimp_TextOp 0x400F9
#endif

extern void translate_escaped_text(char *src, char *dest, int len);
extern void oimage_size_image(const char *alt, int req_ww, int req_hh, rid_image_flags flags, BOOL defer_images, int scale_value, int *iw, int *ih);
extern image oimage_fetch_image(antweb_doc *doc, const char *src);
extern void oimage_flush_image(image im);

#ifndef BUILDERS
static char *passwd_dummy =
    "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
#endif

static void oinput_update_box(rid_text_item *ti, wimp_box *box)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;

    memset(box, 0, sizeof(*box));
    switch (ii->tag)
    {
        case rid_it_TEXT:
        case rid_it_PASSWD:
            box->x0 =   2*5;
            box->x1 = - 2*5;
            box->y1 = - 2*5;
            box->y0 =   2*5;
            break;

        case rid_it_SUBMIT:
        case rid_it_RESET:
        case rid_it_IMAGE:
        default:
            break;
    }
}

void oinput_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii;
    char *t;
    int height, width;
    image_flags fl;

    ii = tii->input;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	if (ii->data.image.im == NULL)
	    ii->data.image.im = oimage_fetch_image(doc, ii->src);

	image_info((image) ii->data.image.im, &width, &height, 0, &fl, 0, 0);

	if (fl & image_flag_REALTHING)
	    ii->data.image.flags |= rid_image_flag_REAL;

	oimage_size_image("Submit", ii->ww, ii->hh, ii->data.image.flags, doc->flags & doc_flag_DEFER_IMAGES, doc->scale_value, &width, &height);

#ifndef BUILDERS
/*         width = width*doc->scale_value/100 + 4; */
/*         height = height*doc->scale_value/100 + 4; */
#endif
	ti->width = width;
	ti->pad = 0;

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
    {
	int n = ((ii->xsize != -1) ? ii->xsize : 20) * doc->scale_value / 100;
	if (n == 0)
	    n = 1;
	ti->width = webfont_tty_width(n, 1) + 24;
	ti->max_up = webfonts[WEBFONT_TTY].max_up + 8;
	ti->max_down = webfonts[WEBFONT_TTY].max_down + 8;
	break;
    }
    case rid_it_BUTTON:
    case rid_it_SUBMIT:
    case rid_it_RESET:
	t = ii->value ? ii->value : (ii->tag == rid_it_RESET ? "Reset" : "Submit");
	if (ii->src)
	{
	    if (ii->data.button.im == NULL)
		ii->data.button.im = oimage_fetch_image(doc, ii->src);

	    if (ii->src_sel && ii->data.button.im_sel == NULL)
		ii->data.button.im_sel = oimage_fetch_image(doc, ii->src_sel);
	
	    image_info((image) ii->data.button.im, &width, &height, 0, &fl, 0, 0);

	    if (fl & image_flag_REALTHING)
		ii->data.button.flags |= rid_image_flag_REAL;

	    oimage_size_image(t, ii->ww, ii->hh, ii->data.button.flags, doc->flags & doc_flag_DEFER_IMAGES, doc->scale_value, &width, &height);
	    ti->width = width;
	    ti->max_up = (height - webfonts[WEBFONT_BUTTON].max_down + webfonts[WEBFONT_BUTTON].max_up)/2;
	    ti->max_down = height - ti->max_up;
	}
	else
	{
#ifdef STBWEB
	    ti->width = webfont_font_width(WEBFONT_BUTTON, t) + 20;
#else
	    ti->width = webfont_tty_width(strlen(t), 1) + 20;
#endif
	    ti->max_up = webfonts[WEBFONT_BUTTON].max_up + 4;
	    ti->max_down = webfonts[WEBFONT_BUTTON].max_down + 4;
	}
	break;
    default:
	ti->width = CHAR_HEIGHT + 16;
	ti->max_up = (CHAR_HEIGHT + 16 + webfonts[WEBFONT_BASE].max_up) / 2;
	ti->max_down = (CHAR_HEIGHT + 16) - ti->max_up;
	break;
    }
    ti->pad = 4;
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
    BOOL draw_selection_box = (ti->flag & rid_flag_SELECTED) != 0;

    if ((ti->flag & rid_flag_FVPR) == 0)
	return;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
	if (ii->data.image.im && ((doc->flags & doc_flag_NO_PICS) == 0))
	{
	    int oox = ox, ooy = oy;
#if USE_MARGINS
	    oox -= doc->margin.x0;
	    ooy -= doc->margin.y1;
#endif
	    image_render((image) ii->data.image.im, hpos+2, bline + 2 - ti->max_down,
			 ti->width/2, (ti->max_up + ti->max_down)/2, doc->scale_value, antweb_render_background, doc, oox, ooy);
	}
	else
	{
	    render_set_colour(render_colour_PLAIN, doc);
	    bbc_move(hpos, bline - ti->max_down);
	    bbc_drawby(ti->width - frontend_dx, 0);
	    bbc_drawby(0, ti->max_down + ti->max_up - frontend_dy);
	    bbc_drawby(- (ti->width - frontend_dx), 0);
	    bbc_drawby(0, -(ti->max_down + ti->max_up - frontend_dy) );
	}
	break;
    case rid_it_TEXT:
    case rid_it_PASSWD:
	plotx = hpos + 10;

	slen = strlen(ii->data.str);

	fg = ii->base.colours.back == -1 ? render_colour_INPUT_F : render_text_link_colour(rh, ti, doc);
	bg = ii->base.colours.back == -1 ? render_colour_WRITE :
	    (ti->flag & rid_flag_SELECTED) && ii->base.colours.select != -1 ?
	    ii->base.colours.select : ii->base.colours.back;
    
	if (fs->lf != webfonts[WEBFONT_TTY].handle)
	{
	    fs->lf = webfonts[WEBFONT_TTY].handle;
	    font_setfont(fs->lf);
	}

	if (fs->lfc != fg)
	{
	    fs->lfc = fg;
	    render_set_font_colours(fs->lfc, bg, doc);
	}

	if (ti == doc->input)
	{
	    font_string fs;
	    os_error *ep;
	    int x1, x2;
	    int boxw = ii->base.display->width - 24;

	    if (doc->text_input_offset < 0)
		doc->text_input_offset = strlen(ii->data.str);

	    fs.s = ii->data.str;
	    fs.x = fs.y = (1 << 30);
	    fs.split = -1;
	    fs.term = doc->text_input_offset;

	    ep = font_strwidth(&fs);

	    x1 = (fs.x / MILIPOINTS_PER_OSUNIT);

	    fs.s = ii->data.str;
	    fs.x = fs.y = (1 << 30);
	    fs.split = -1;
	    fs.term = slen;

	    ep = font_strwidth(&fs);

	    x2 = (fs.x / MILIPOINTS_PER_OSUNIT);

	    if (x2 <= boxw)
	    {
		/* The whole string fits in the box */
	    }
	    else
	    {
		if (x1 <= (boxw >> 1) )
		{
		    /* At the left end */
		}
		else if (x1 >= (x2 - (boxw >> 1)))
		{
		    /* At the right end */
		    plotx -= x2;
		    plotx += boxw;
		}
		else
		{
		    /* Center the caret */
		    plotx -= x1;
		    plotx += (boxw >> 1);
		}
	    }
	}

	render_plinth(bg, render_plinth_RIM | render_plinth_IN,
		      hpos, bline - ti->max_down,
		      ti->width, (ti->max_up + ti->max_down), doc );

	/* Check for whether the text needs redrawing if it does 
	 * use the intersection of graphics window and text box
	 */
	{
	    wimp_box ta_box, gwind_box;
	    int dx = frontend_dx, dy = frontend_dy;

	    ta_box.x0 = hpos+8;
	    ta_box.y0 = bline-ti->max_down+8;
	    ta_box.x1 = hpos+ti->width-8;
	    ta_box.y1 = bline+ti->max_up-8;
	    if (coords_intersection(&ta_box, g, &gwind_box))
	    {
		bbc_gwindow(gwind_box.x0, gwind_box.y0, gwind_box.x1-dx, gwind_box.y1-dy);

		if (ii->tag == rid_it_TEXT)
		{
		    font_paint(ii->data.str, font_OSCOORDS + (config_display_blending ? 0x800 : 0),
			       plotx, bline);
		}
		else
		{
		    font_paint(passwd_dummy + 255 - slen,
			       font_OSCOORDS + (config_display_blending ? 0x800 : 0),
			       plotx, bline);
		}

		bbc_gwindow(g->x0, g->y0, g->x1-dx, g->y1-dy);
	    }
	}
	break;

    case rid_it_SUBMIT:
    case rid_it_RESET:
    case rid_it_BUTTON:
	fg = ii->base.colours.back == -1 && ii->data.button.im == NULL ? render_colour_INPUT_F : render_text_link_colour(rh, ti, doc);
	bg = ii->data.button.tick ? render_colour_ACTION : ii->base.colours.back == -1 ? render_colour_INPUT_B : ii->base.colours.back;
	t = ii->value ? ii->value : ii->tag == rid_it_SUBMIT ? "Submit" : "Reset";

	if (ii->data.button.im)
	{
	    int oox = ox, ooy = oy;
	    image im;
#if USE_MARGINS
	    oox -= doc->margin.x0;
	    ooy -= doc->margin.y1;
#endif
	    if (draw_selection_box && ii->data.button.im_sel)
	    {
		im = (image)ii->data.button.im_sel;
		draw_selection_box = FALSE;
	    }
	    else
		im = (image)ii->data.button.im;
	    
	    image_render(im,
			 hpos, bline - ti->max_down,
			 ti->width/2, (ti->max_up + ti->max_down)/2,
			 doc->scale_value, antweb_render_background, doc, oox, ooy);
	    
	    plotx = (ti->width - webfont_font_width(WEBFONT_BUTTON, t))/2;
	    draw_selection_box = FALSE;
	}
	else
	{
	    render_plinth(bg,
		      ii->data.button.tick ? render_plinth_IN : 0,
		      hpos, bline - ti->max_down,
		      ti->width, (ti->max_up + ti->max_down), doc );
	    plotx = 10;
	}
	
	if (fs->lf != webfonts[WEBFONT_BUTTON].handle)
	{
	    fs->lf = webfonts[WEBFONT_BUTTON].handle;
	    font_setfont(fs->lf);
	}

	if (fs->lfc != fg)
	{
	    fs->lfc = fg;
	    render_set_font_colours(fs->lfc, bg, doc);
	}

	font_paint(t, font_OSCOORDS + (config_display_blending ? 0x800 : 0),
		   hpos + plotx, bline);
	break;
    case rid_it_RADIO:
    case rid_it_CHECK:
        {
	    char *sname;

	    sname = ((ii->tag == rid_it_RADIO) ?
		     (ii->data.tick ? "radioon" : "radiooff") :
		     (ii->data.tick ? "opton" : "optoff") );
	    render_plot_icon(sname, hpos + 4, bline - ti->max_down + 4);
	}
	break;
    default:
	break;
    }

    if (draw_selection_box)
    {
	render_set_colour(render_colour_HIGHLIGHT, doc);
	render_item_outline(ti, hpos, bline);       /* SJM */
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
	    x = x-2;
	    y = (ti->max_up-2)-y;

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

	    antweb_submit_form(doc, ii->base.parent, bb & wimp_BRIGHT);
	}

	break;
    case rid_it_TEXT:
    case rid_it_PASSWD:
	{
	    os_error *ep;
	    os_regset r;
	    int len = strlen(ii->data.str);

	    /* FIXME: This doesn't take into account scrolled strings */

	    x-=12;

	    r.r[0] = (int) (long) webfonts[WEBFONT_TTY].handle;
	    r.r[1] = (int) (long) ii->data.str;
	    r.r[2] = (1 << 17) | (1 << 8) | (1 << 7);
	    r.r[3] = x * MILIPOINTS_PER_OSUNIT;
	    r.r[4] = 0;
	    r.r[7] = len;

	    ep = os_swix(Font_ScanString, &r);

	    /* This now always sets the caret */
	    doc->text_input_offset = ep ? strlen(ii->data.str) : ((char *) (long) r.r[1]) - ii->data.str;
	    doc->input = ti;
#if 0
	    fprintf(stderr, "Caret set to item %p, offset %d\n", doc->input, doc->text_input_offset);
#endif
	    antweb_place_caret(doc);
	}
	break;
    case rid_it_CHECK:
	ii->data.tick = !ii->data.tick;
	redraw = TRUE;
	break;
    case rid_it_RADIO:
	if (ii->data.tick == FALSE)
	{
	    ii->data.tick = TRUE;
	    for (ife = ii->base.parent->kids; ife; ife = ife->next)
	    {
		if (ife->tag == rid_form_element_INPUT)
		{
		    rid_input_item *iis = (rid_input_item *)ife;
		    if (iis->tag == rid_it_RADIO && ii != iis && strcasecomp(ii->name, iis->name) == 0)
		    {
			iis->data.tick = FALSE;
			antweb_update_item(doc, iis->base.display);
		    }
		}
	    }

	    redraw = TRUE;
	}
	break;
    case rid_it_SUBMIT:
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
			translate_escaped_text(iis->value, iis->data.str, iis->max_len);
		    }
		    else
		    {
			iis->data.str[0] = 0;
		    }

		    if (iis->base.display == doc->input)
		    {
			int len = strlen(iis->data.str);
			if (doc->text_input_offset > len)
			    doc->text_input_offset = len;
		    }
		    touch = 1;
		    break;
		case rid_it_CHECK:
		case rid_it_RADIO:
		    iis->data.tick = iis->flags & rid_if_CHECKED;
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

	if (doc->input)
	{
	    doc->text_input_offset = -1;
	    (object_table[doc->input->tag].caret)(doc->input, doc->rh, doc, TRUE);
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
        int xsize = ii->xsize != -1 ? ii->xsize : 20;
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
        int xsize = ii->xsize != -1 ? ii->xsize : 20;
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
	fputc(ii->data.tick ? 'x' : ' ', f);
	fputc(']', f);
	break;
    case rid_it_RESET:
    case rid_it_SUBMIT:
    case rid_it_BUTTON:
	fputc('[', f);
	fputs(ii->value ? ii->value : ( ii->tag == rid_it_SUBMIT ? "Submit" : "Reset" ), f);
	fputc(']', f);
	break;
    }
}

BOOL oinput_caret(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int repos)
{
#ifndef BUILDERS
    rid_input_item *ii;
    int take_it = FALSE;
    font_string fs;
    os_error *ep;
    int x1, x2;
    int boxw;
    int cx, cy;
    int h;

    ii = ((rid_text_item_input *) ti)->input;

    switch (ii->tag)
    {
    case rid_it_TEXT:
    case rid_it_PASSWD:
	stream_find_item_location(ti, &cx, &cy);

	if (doc->text_input_offset < 0)
	{
	    doc->text_input_offset = strlen(ii->data.str);
	    repos = 1;
	}

	fs.s = ii->data.str;
	fs.x = fs.y = (1 << 30);
	fs.split = -1;
	fs.term = doc->text_input_offset;

	ep = font_setfont(webfonts[WEBFONT_TTY].handle);
	if (ep)
	    break;

	ep = font_strwidth(&fs);
	if (ep)
	    break;

	x1 = (fs.x / MILIPOINTS_PER_OSUNIT);

	fs.s = ii->data.str;
	fs.x = fs.y = (1 << 30);
	fs.split = -1;
	fs.term = strlen(ii->data.str);

	ep = font_strwidth(&fs);
	if (ep)
	    break;

	boxw = ii->base.display->width - 24;
	x2 = (fs.x / MILIPOINTS_PER_OSUNIT);
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

	cy -= webfonts[WEBFONT_TTY].max_down;

#if USE_MARGINS
	cx += doc->margin.x0;
	cy += doc->margin.y1;
#endif
	h = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
	h |= render_caret_colour(doc, ii->base.colours.back, ii->base.colours.cursor);

	frontend_view_caret(doc->parent, cx, cy, h, repos);

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

    i = doc->text_input_offset;

    ii = ((rid_text_item_input *) ti)->input;

    if (doc->text_input_offset < 0)
	doc->text_input_offset = strlen(ii->data.str);

    switch (ii->tag)
    {
    case rid_it_TEXT:
    case rid_it_PASSWD:
	len = strlen(ii->data.str);

	if (key >= 32 && key < 256 && key != 127)
	{
	    if (ii->flags & rid_if_NUMBERS)
	    {
		if (isdigit(key))
		{
		    if (i >= ii->max_len)
			i--;
		    if (i <= ii->max_len)
			doc->text_input_offset++;
		    
		    ii->data.str[i] = key;
		    redraw = TRUE;
		    used = TRUE;
		}
	    }
	    else 
	    {
		if (len+1 <= ii->max_len)	/* 15/8/96: DAF: < to <= */
		{
		    memmove(ii->data.str + i + 1, ii->data.str + i, (len + 1 - i));

		    ii->data.str[i] = key;
		    doc->text_input_offset++;
		    redraw = TRUE;
		}
		used = TRUE;
	    }
	}
	else
	{
	    switch (lookup_key_action(key))
	    {
	    case key_action_NEWLINE:
		if (ii->base.parent && ii->base.parent->last_text == NULL)
		{
		    rid_input_item *first;
		    rid_form_element *ife;
		    first = NULL;

		    for(ife = ii->base.parent->kids; ife; ife = ife->next)
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
#ifndef STBWEB
			    if (ins->tag == rid_it_PASSWD || (ins->tag == rid_it_TEXT && ins != ii))
				break;
#endif
			}
		    }

		    if (ife == NULL)
		    {
			/* No passwords, the only text item is this one and only the first submit button 'ticked' */
			antweb_submit_form(doc, ii->base.parent, 0);	/* No such thing as a right click here */
			if (first)
			    first->data.button.tick = 0;
			redraw = TRUE;
		    }
		}
		break;
	    case key_action_DELETE_LEFT:
		if (i > 0)
		{
		    memmove(ii->data.str + i - 1, ii->data.str + i, (len + 1 - i));
		    doc->text_input_offset--;
		    redraw = TRUE;
		}
		break;
	    case key_action_DELETE_ALL:
		ii->data.str[0] = 0;
		doc->text_input_offset = 0;
		redraw = TRUE;
		break;

	    case key_action_DELETE_TO_END:
		ii->data.str[i] = 0;
		redraw = TRUE;
		break;

	    case key_action_DELETE_RIGHT:
		if (i < len)
		{
		    memmove(ii->data.str + i, ii->data.str + i + 1, (len - i));
		    redraw = TRUE;
		}
		break;

	    case key_action_LEFT:
#ifdef STBWEB
		if (i > 0)
		{
		    doc->text_input_offset--;
		    redraw = TRUE;
		}
#else
		if (i > 0)
		    doc->text_input_offset--;
		redraw = TRUE;
#endif
		break;

	    case key_action_RIGHT:
#ifdef STBWEB
		if (i < len)
		{
		    doc->text_input_offset++;
		    redraw = TRUE;
		}
#else
		if (i < len)
		    doc->text_input_offset++;
		redraw = TRUE;
#endif
		break;

	    case key_action_START_OF_LINE:
		doc->text_input_offset = 0;
		redraw = TRUE;
		break;

	    case key_action_END_OF_LINE:
		doc->text_input_offset = len;
		redraw = TRUE;
		break;

	    default:
#if DEBUG
                fprintf(stderr, "Key %d\n", key);
#endif
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
	oinput_caret(ti, rh, doc, TRUE);

        oinput_update_box(ti, &box);
	antweb_update_item_trim(doc, ti, &box, FALSE);

	used = TRUE;
    }

    return used;
}

void *oinput_image_handle(rid_text_item *ti, antweb_doc *doc, int reason)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;

    if (ii->tag == rid_it_IMAGE || ii->tag == rid_it_SUBMIT || ii->tag == rid_it_RESET || ii->tag == rid_it_BUTTON) switch (reason)
    {
    case object_image_HANDLE:
	return ii->tag == rid_it_IMAGE ? ii->data.image.im : ii->data.button.im;

    case object_image_ABORT:
	if (ii->tag == rid_it_IMAGE)
	    oimage_flush_image(ii->data.image.im);
	else
	{
	    oimage_flush_image(ii->data.button.im);
	    oimage_flush_image(ii->data.button.im_sel);
	}
	break;

    case object_image_BOX:
    {
	static wimp_box box;
	memset(&box, 0, sizeof(box));
	return &box;
    }
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
		     (ii->data.tick ? "radioon" : "radiooff") :
		     (ii->data.tick ? "opton" : "optoff") );
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
 * Also currently used for textarea and select
 */

void oinput_update_highlight(rid_text_item *ti, antweb_doc *doc)
{
    rid_text_item_input *tii = (rid_text_item_input *) ti;
    rid_input_item *ii = tii->input;
    wimp_box trim;

    memset(&trim, 0, sizeof(trim));

    if ((ii->tag == rid_it_SUBMIT || ii->tag ==  rid_it_RESET || ii->tag == rid_it_BUTTON) &&
	ii->data.button.im_sel)
    {
	antweb_update_item_trim(doc, ti, &trim, FALSE);
    }
    else
    {
	trim.x0 = ti->width - 4;
	antweb_update_item_trim(doc, ti, &trim, TRUE);
	trim.x0 = 0;

	trim.y0 = ti->max_up + ti->max_down - 4;
	antweb_update_item_trim(doc, ti, &trim, TRUE);
	trim.y0 = 0;

	trim.x1 = - (ti->width - 4);
	antweb_update_item_trim(doc, ti, &trim, TRUE);
	trim.x1 = 0;

	trim.y1 = - (ti->max_up + ti->max_down - 4);
	antweb_update_item_trim(doc, ti, &trim, TRUE);
    }
}

/* eof oinput.c */
