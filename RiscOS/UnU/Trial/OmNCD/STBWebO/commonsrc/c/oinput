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

void translate_escaped_text(char *src, char *dest, int len);

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

    ii = tii->input;

    switch (ii->tag)
    {
    case rid_it_IMAGE:
#if 0
	if (ii->src)
#endif
	{
	    image_flags fl;
	    int new_im = 0;

	    if (ii->data.image.im == NULL)
	    {
		char *url;
		int ffl;

		new_im = 1;

		ffl = (doc->flags & doc_flag_DEFER_IMAGES) ? image_find_flag_DEFER : 0;
		if ((doc->flags & doc_flag_FROM_HISTORY) == 0)
		    ffl |= image_find_flag_CHECK_EXPIRE;

		url = url_join(BASE(doc), ii->src);

		image_find(url, BASE(doc), ffl,
			   &antweb_doc_image_change, doc, render_get_colour(render_colour_BACK, doc),
			   (image*) &(ii->data.image.im));

		mm_free(url);
	    }

	    image_info((image) ii->data.image.im, &width, &height, 0, &fl, 0, 0);

	    /* copied this over from oimage.c in case it is important */
	    if (new_im && (fl & image_flag_REALTHING))
		ii->data.image.flags |= rid_image_flag_REAL;
	}
#if 0
	else
	{
	    width = 68;
	    height = 68;
	}
#endif
#ifndef BUILDERS
        width = width*config_display_scale_image/100 + 4;
        height = height*config_display_scale_image/100 + 4;
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
	ti->width = webfont_tty_width(((ii->xsize != -1) ? ii->xsize : 20), 1) + 24;
	ti->max_up = webfonts[WEBFONT_TTY].max_up + 8;
	ti->max_down = webfonts[WEBFONT_TTY].max_down + 8;
	break;
    case rid_it_SUBMIT:
    case rid_it_RESET:
	t = ii->value ? ii->value : (ii->tag == rid_it_RESET ? "Reset" : "Submit");
#ifdef STBWEB
	ti->width = webfont_font_width(WEBFONT_BUTTON, t) + 20;
#else
	ti->width = webfont_tty_width(strlen(t), 1) + 20;
#endif
	ti->max_up = webfonts[WEBFONT_BUTTON].max_up + 4;
	ti->max_down = webfonts[WEBFONT_BUTTON].max_down + 4;
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
			 -1, -1, config_display_scale_image, antweb_render_background, doc, oox, ooy);
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

	if (fs->lf != webfonts[WEBFONT_TTY].handle)
	{
	    fs->lf = webfonts[WEBFONT_TTY].handle;
	    font_setfont(fs->lf);
	}

	if (fs->lfc != render_colour_INPUT_F )
	{
	    fs->lfc = render_colour_INPUT_F;
	    render_set_font_colours(fs->lfc, render_colour_INPUT_B, doc);
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

	render_plinth(render_colour_WRITE, render_plinth_RIM | render_plinth_IN,
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
	render_plinth(ii->data.tick ? render_colour_ACTION : render_colour_INPUT_B,
		      ii->data.tick ? render_plinth_IN : 0,
		      hpos, bline - ti->max_down,
		      ti->width, (ti->max_up + ti->max_down), doc );

	if (fs->lf != webfonts[WEBFONT_BUTTON].handle)
	{
	    fs->lf = webfonts[WEBFONT_BUTTON].handle;
	    font_setfont(fs->lf);
	}

	if (fs->lfc != render_colour_INPUT_F )
	{
	    fs->lfc = render_colour_INPUT_F;
	    render_set_font_colours(fs->lfc, render_colour_INPUT_B, doc);
	}

	font_paint(ii->value ? ii->value : ( ii->tag == rid_it_SUBMIT ? "Submit" : "Reset" ),
		   font_OSCOORDS + (config_display_blending ? 0x800 : 0), hpos + 10, bline);
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

    if (ti->flag & rid_flag_SELECTED)
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

    if (ii->tag == rid_it_IMAGE)
	image_loose((image) ii->data.image.im, &antweb_doc_image_change, doc);
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
	else if (akbd_pollctl())
	{
            backend_select_item(doc, ti, -1);
            antweb_place_caret(doc);
	}
	else
	{
	    x = x-2;
	    y = (ti->max_up-2)-y;

            image_os_to_pixels((image) ii->data.image.im, &x, &y, config_display_scale_image);

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
			ii2->data.tick = 0;
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
		    ii2->data.tick = 0;
		}
	    }
	}

	ii->data.tick = TRUE;
	antweb_update_item(doc, ti);

	antweb_submit_form(doc, ii->base.parent, bb & wimp_BRIGHT);

	ii->data.tick = FALSE;
	redraw = TRUE;
	break;

    case rid_it_RESET:
	ii->data.tick = TRUE;
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
	    }
	}

	if (doc->input)
	{
	    doc->text_input_offset = -1;
	    (object_table[doc->input->tag].caret)(doc->input, doc->rh, doc, TRUE);
	}

	ii->data.tick = FALSE;
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
	frontend_view_caret(doc->parent, cx, cy, webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down, repos);

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
	    if (len+1 <= ii->max_len)	/* 15/8/96: DAF: < to <= */
	    {
		memmove(ii->data.str + i + 1, ii->data.str + i, (len + 1 - i));

		ii->data.str[i] = key;
		doc->text_input_offset++;
		redraw = TRUE;
	    }
	    used = TRUE;
	}
	else
	{
	    switch (key)
	    {
	    case 13:
	    case 10:
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
				    ins->data.tick = 1;
				    first = ins;
				}
				else
				    ins->data.tick = 0;
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
			    first->data.tick = 0;
			redraw = TRUE;
		    }
		}
		break;
#ifndef STBWEB
	    case 127:
#endif
	    case 8:
		if (i > 0)
		{
		    memmove(ii->data.str + i - 1, ii->data.str + i, (len + 1 - i));
		    doc->text_input_offset--;
		    redraw = TRUE;
		}
		break;
	    case 21:		/* ctrl-U */
		ii->data.str[0] = 0;
		doc->text_input_offset = 0;
		redraw = TRUE;
		break;

	    case akbd_Ctl + akbd_CopyK:
#ifndef STBWEB
	    case 11:		/* ctrl-K */
#endif
		ii->data.str[i] = 0;
		redraw = TRUE;
		break;

#ifdef STBWEB
	    case 127:           /* STB: Delete right */
#else
	    case akbd_CopyK:    /* riscos: Delete right */
#endif
#ifndef STBWEB
	    case 4:		/* ctrl-D */
#endif
		if (i < len)
		{
		    memmove(ii->data.str + i, ii->data.str + i + 1, (len - i));
		    redraw = TRUE;
		}
		break;

	    case akbd_LeftK:
#ifndef STBWEB
	    case 2:		/* ctrl-B */
#endif
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

	    case akbd_RightK:
#ifndef STBWEB
	    case 6:		/* ctrl-F */
#endif
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

	    case akbd_LeftK + akbd_Ctl:
#ifndef STBWEB
	    case 1:		/* ctrl-A */
#endif
#ifdef STBWEB
            case 0x1E:          /* STB: home */
#endif
		doc->text_input_offset = 0;
		redraw = TRUE;
		break;

	    case akbd_RightK + akbd_Ctl:
#ifndef STBWEB
	    case 5:		/* ctrl-E */
#endif
#ifdef STBWEB
            case akbd_CopyK:        /* STB: end */
#endif
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

    if (ii->tag == rid_it_IMAGE) switch (reason)
    {
    case object_image_HANDLE:
	return ii->data.image.im;

    case object_image_ABORT:
    {
	int f;
	if (ii->data.image.im &&
	    image_info(ii->data.image.im, NULL, NULL, NULL, &f, NULL, NULL) == NULL &&
	    (f & image_flag_FETCHED) == 0)
	{
#if 0
	    wimp_box box;
	    backend_doc_item_bbox(doc, ti, &box);

	    image_loose(ii->data.image.im, antweb_doc_image_change, doc);
	    ii->data.image.im = NULL;

	    frontend_view_redraw(doc->parent, &box);
#else
	    image_mark_to_flush(ii->data.image.im, image_find_flag_DEFER);
#endif
	}
	break;
    }

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
    wimp_box trim;
    memset(&trim, 0, sizeof(trim));

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

/* eof oinput.c */
