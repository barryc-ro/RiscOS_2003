/* -*-c-*- */

/* select.c */

/* CHANGELOG
 * 19/07/96: SJM: Reworked for new form element lists.
 *		  Changed to use the current font rather than a fixed one.
 * 07/08/96: SJM: Removed checking for at least one OPTION selected (now done in fresparse/forms.c)
 */


/* Deal with the select list for forms */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "memwatch.h"

#include "wimp.h"
#include "font.h"
#include "bbc.h"

#include "antweb.h"
#include "interface.h"
#include "rid.h"
#include "webfonts.h"
#include "util.h"
#include "makeerror.h"
#include "consts.h"
#include "config.h"
#include "render.h"
#include "rcolours.h"

#include "object.h"
#include "gbf.h"

#if UNICODE
#include "Unicode/languages.h"
#endif

/* Option to use current font rather than fixed font */
#ifndef SELECT_CURRENT_FONT
#ifdef STBWEB
#define SELECT_CURRENT_FONT 1
#else
#define SELECT_CURRENT_FONT 0
#endif
#endif

#ifdef STBWEB
#define SELECT_BORDER_X		8 /* border size in OS units on each side */
#define SELECT_BORDER_Y		8
#else
#define SELECT_BORDER_X		4 /* border size in OS units on each side */
#define SELECT_BORDER_Y		4
#endif

#define SELECT_SPACE_X		4	/* space on either side of item */
#define SELECT_SPACE_Y		4

#define GRIGHT_SIZE		48

#if SELECT_CURRENT_FONT
#define BASE_FONT WEBFONT_TTY
#else
#define BASE_FONT (-1)
#endif


#ifndef BUILDERS
static void select_menu_callback(fe_menu mh, void *handle, int item, int right)
{
    rid_text_item_select *tis = (rid_text_item_select *) handle;
    int i;
    rid_option_item *oi;
    int tog, redraw;
    fe_menu_item *iis;

    tog = tis->select->flags & rid_sf_MULTIPLE;

    iis = (fe_menu_item*) tis->select->items;

    redraw = right;

    for(i = 0, oi = tis->select->options; oi; oi = oi->next, i++)
    {
	if (i == item)
	{
	    if (!tog)
	    {
		if ((oi->flags & rid_if_SELECTED) == 0)
		{
		    iis[i].flags |= fe_menu_flag_CHECKED;
		    oi->flags |= rid_if_SELECTED;
		    if (redraw)
		    {
			frontend_menu_update_item(mh, i);
		    }
		}
	    }
	    else
	    {
		iis[i].flags ^= fe_menu_flag_CHECKED;
		oi->flags ^= rid_if_SELECTED;
		if (redraw)
		{
		    frontend_menu_update_item(mh, i);
		}
	    }
	}
	else
	{
	    if (!tog)
	    {
		if ((oi->flags & rid_if_SELECTED))
		{
		    iis[i].flags ^= fe_menu_flag_CHECKED;
		    oi->flags ^= rid_if_SELECTED;
		    if (redraw)
		    {
			frontend_menu_update_item(mh, i);
		    }
		}
	    }
	}
    }

    sound_event(snd_MENU_SELECT);
    antweb_update_item( (antweb_doc*)tis->select->doc, &(tis->base));
}
#endif /* BUILDERS */


void oselect_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
#ifdef BUILDERS
    ti->width = 25;
    ti->pad = 1;
    ti->max_up = 16;
    ti->max_down = 1;
#else /* BUILDERS */

    rid_select_item *sel = ((rid_text_item_select *) ti)->select;
    rid_option_item *oi;
    int width, height;
    font_string fs;
    int line_space;
    int i;
    int whichfont;
    struct webfont *wf;

    sel->doc = doc;

    whichfont = antweb_getwebfont(doc, ti, BASE_FONT);
    wf = &webfonts[whichfont];

    line_space = wf->max_up + wf->max_down;

    /* This is better? */
    if (sel->flags & rid_sf_MULTIPLE)
	width = webfont_font_width(whichfont, "<None>");
    else
	width = 0;
    
    height = 0;

    for(oi = sel->options; oi; oi = oi->next)
    {
	/* oi->text pointers never get freed as they are copies of other pointers which are */
	if (oi->text == NULL)
	    oi->text = "";

	fs.x = webfont_font_width(whichfont, oi->text);
	if (width < fs.x)
	    width = fs.x;

	BENDBG(("oselect_size: ti %p font %d %s opt '%s' size %d (max %d)\n", ti, whichfont, ti->flag & rid_flag_WIDE_FONT ? "WIDE" : "NARROW", oi->text, fs.x, width));
	
	height ++;
    }

    if ((sel->flags & rid_if_NOPOPUP) == 0)
    {
        /* pdh: Repeat this if we resize with a different number of items,
         * otherwise the damn buffer ain't big enough...
         */

        if ( (height != sel->count) || !sel->items )
        {
            if ( sel->items )
                mm_free( sel->items );

            sel->items = mm_calloc(sizeof(fe_menu_item), height);
	    
            for (i=0, oi = sel->options; oi; i++, oi = oi->next)
            {
                fe_menu_item *ii = ((fe_menu_item*)sel->items) + i;
                ii->name = oi->text;
                if (oi->flags & rid_if_CHECKED)
                {
                    oi->flags |= rid_if_SELECTED;
                    ii->flags = fe_menu_flag_CHECKED;
                }
                else
                    oi->flags &= ~rid_if_SELECTED;
	    }

	    /* use the first menu item to pass the flags and language for the entire menu */
	    if (
		(ti->flag & rid_flag_WIDE_FONT)
#if UNICODE
		&& (ti->language > 1 || rh->language_num > 1)
#endif
		)
	    {
		fe_menu_item *first = &((fe_menu_item *)sel->items)[0];
		first->flags |= fe_menu_flag_WIDE;
		first->language = ti->language ? ti->language : rh->language_num;
		
		BENDBG(("oselect_size: set wide font flag\n"));
	    }
	}

        /* pdh: in view of the fact we may come through here twice on the same
         * select item (if it straddles a packet boundary) it's unwise to do
         * this here
         *
        sel->menuh = frontend_menu_create(doc->parent, select_menu_callback, ti, height, sel->items, sel->size, width);
         */

	sel->count = height;
    }
    else
    {
	for (oi = sel->options; oi; oi = oi->next)
	{
	    if (oi->flags & rid_if_CHECKED)
		oi->flags |= rid_if_SELECTED;
	    else
		oi->flags &= ~rid_if_SELECTED;
	}
    }

    ti->width = width + 2*SELECT_SPACE_X + 2*SELECT_BORDER_X;

    /* add on width for the the POPUP icon */
    if ((sel->flags & rid_if_NOPOPUP) == 0)
	ti->width += GRIGHT_SIZE;

    ti->max_up = wf->max_up + SELECT_SPACE_Y + SELECT_BORDER_Y - 2;
    ti->max_down = wf->max_down + SELECT_SPACE_Y + SELECT_BORDER_Y;
#endif /* BUILDERS */
}

void oselect_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_select_item *sel = ((rid_text_item_select *) ti)->select;
    rid_option_item *oi;
    int checked = 0;
    char *str = NULL;
    font_string fstr;
    int fg, bg;
    BOOL selected;
    int whichfont;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT)
    {
	if (oselect_update_highlight(ti, doc, 0, NULL))
	    highlight_render_outline(ti, doc, hpos, bline);
	return;
    }

    if (update == object_redraw_BACKGROUND)
	return;
    
    selected = backend_is_selected(doc, ti);

    fg = sel->base.colours.back == -1 ? render_colour_INPUT_F : render_text_link_colour(ti, doc);

    if (selected)
    {
	if (sel->base.colours.select != -1)
	    bg = sel->base.colours.select | render_colour_RGB;
	else
	    bg = render_colour_INPUT_S;
	fg = render_colour_INPUT_FS;
    }
    else
    {
	bg = sel->base.colours.back == -1 ? render_colour_INPUT_B : sel->base.colours.back | render_colour_RGB;
    }

#ifdef STBWEB
    render_plinth_from_list(bg,
			    selected && config_colour_list[render_colour_list_SELECT_HIGHLIGHT] ?
				config_colour_list[render_colour_list_SELECT_HIGHLIGHT] :
				config_colour_list[render_colour_list_SELECT],
			    0,
			    hpos + SELECT_SPACE_X, bline - ti->max_down + SELECT_SPACE_Y,
			    ti->width - (sel->flags & rid_if_NOPOPUP ? 0 : GRIGHT_SIZE) - SELECT_SPACE_X*2,
			    (ti->max_up + ti->max_down) - SELECT_SPACE_Y*2,
			    doc );
#else
    render_plinth(bg, render_plinth_IN,
		  hpos + SELECT_SPACE_X, bline - ti->max_down + SELECT_SPACE_Y,
		  ti->width - (sel->flags & rid_if_NOPOPUP ? 0 : GRIGHT_SIZE) - SELECT_SPACE_X*2,
		  (ti->max_up + ti->max_down) - SELECT_SPACE_Y*2,
		  doc );
#endif

    for(oi = sel->options; oi; oi = oi->next)
    {
	if (oi->flags & rid_if_SELECTED)
	{
	    str = oi->text;
	    checked ++;
	}
    }

    if (checked == 0)
	str = "<None>";
    if (checked > 1)
	str = "<Many>";

    whichfont = antweb_getwebfont(doc, ti, BASE_FONT);
    
    if (fs->lfc != fg || fs->lbc != bg)
    {
	fs->lfc = fg;
	fs->lbc = bg;
	render_set_font_colours(fg, bg, doc);
    }

    fstr.x = webfont_font_width(whichfont, str);

    render_text(doc, whichfont, str, hpos + ((ti->width - (sel->flags & rid_if_NOPOPUP ? 0 : GRIGHT_SIZE) - fstr.x) >> 1), bline);

    if ((sel->flags & rid_if_NOPOPUP) == 0)
	render_plot_icon("gright",
			 hpos + ti->width - GRIGHT_SIZE - SELECT_SPACE_X,
			 bline + ((ti->max_up - ti->max_down) >> 1) - GRIGHT_SIZE/2 + 2);

#endif /* BUILDERS */
}

void oselect_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
#ifndef BUILDERS
    rid_select_item *sel = ((rid_text_item_select *) ti)->select;

    if (sel->menuh)
	frontend_menu_dispose( sel->menuh );

    if (sel->items)
    {
	mm_free(sel->items);
	sel->items = NULL;
    }
#endif /* BUILDERS */
}

char *oselect_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb)
{
#ifndef BUILDERS
    rid_select_item *sel = ((rid_text_item_select *) ti)->select;
    if (sel->flags & rid_if_NOPOPUP)
    {
	rid_option_item *oi;

	/* find the first selected */
	for (oi = sel->options; oi; oi = oi->next)
	    if (oi->flags & rid_if_SELECTED)
		break;

	/* deselect it and select the next one, wrapping around */
	if (oi == NULL)
	{
	    if (sel->options)
		sel->options->flags |= rid_if_SELECTED;
	}
	else
	{
	    oi->flags &= ~rid_if_SELECTED;
	    if (oi->next)
		oi->next->flags |= rid_if_SELECTED;
	    else
		sel->options->flags |= rid_if_SELECTED;
	}

	sound_event(snd_MENU_SELECT);
	antweb_update_item(doc, ti);
    }
    else
    {
	wimp_box box;
	backend_doc_item_bbox(doc, ti, &box);

        /* pdh: moved menu brewing into here */

        if ( !sel->menuh )
        {
            sel->menuh = frontend_menu_create( doc->parent,
                                               select_menu_callback, ti,
                                               sel->count, sel->items,
                                               sel->size, ti->width - GRIGHT_SIZE - 2*SELECT_SPACE_Y - 2*SELECT_BORDER_Y );
        }

	frontend_menu_raise(((rid_text_item_select *)ti)->select->menuh, box.x1, box.y1);
    }
#endif /* BUILDERS */
    return NULL;		/* Links should not be followed */
}

void oselect_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    fputs("[Selection menu]", f);
}

int oselect_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box)
{
    rid_select_item *sel = ((rid_text_item_select *) ti)->select;
    BOOL draw_box;

    if (box)
	memset(box, 0, sizeof(*box));

    draw_box = sel->base.colours.select == -1 &&
	config_colours[render_colour_INPUT_S].word == config_colours[render_colour_INPUT_B].word &&
	config_colour_list[render_colour_list_SELECT_HIGHLIGHT] == NULL;
    
    return draw_box;
}

/* eof oselect.c */
