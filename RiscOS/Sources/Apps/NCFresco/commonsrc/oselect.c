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

static int getwebfont(antweb_doc *doc, rid_text_item *ti)
{
    int whichfont;

#ifdef SELECT_CURRENT_FONT
    whichfont = ti->st.wf_index;
#else
    whichfont = WEBFONT_TTY;
#endif

    /* pdh: autofit bodge */
    if ( gbf_active( GBF_AUTOFIT ) && gbf_active( GBF_AUTOFIT_ALL_TEXT ) )
    {
        if ( doc->scale_value < 100
             && ( (whichfont & WEBFONT_SIZE_MASK) > 0 ) )
        {
           /* make it one size smaller */
           TASSERT( WEBFONT_SIZE_SHIFT == 0 );
           whichfont -= 1;
        }
    }

    antweb_doc_ensure_font( doc, whichfont );

    return whichfont;
}

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

    whichfont = getwebfont(doc, ti);
    wf = &webfonts[whichfont];

    line_space = wf->max_up + wf->max_down;

#if 1
    /* This is better? */
    if (sel->flags & rid_sf_MULTIPLE)
	width = webfont_font_width(whichfont, "<None>");
    else
	width = 0;
#else
    /* This isn't quite right but it will do for now */
    width = webfont_tty_width(6, 1); /* Length of '<none>' in chars */
#endif
    
    if (font_setfont(wf->handle) != 0)
	return;

    height = 0;

    for(oi = sel->options; oi; oi = oi->next)
    {
	/* @@@@ Borris asks if this will get freed correctly. 15/10/96 */

        /* #*$@ Peter discovers that it doesn't. 9/6/97 */

	if (oi->text == NULL)
#if 0
	    oi->text = strdup("");
#else
            oi->text = "";
#endif

	/* Start at the end; while not before the start and on a space; work backwards */
	for(i = strlen(oi->text)-1; i>=0 && isspace(oi->text[i]); i--)
	    oi->text[i] = 0;

	fs.s = oi->text;
	fs.x = fs.y = fs.term = (1 << 30);
	fs.split = -1;

	if (font_strwidth(&fs) == NULL)
	{
	    if (width < (fs.x / MILIPOINTS_PER_OSUNIT))
		width = (fs.x / MILIPOINTS_PER_OSUNIT);
	}

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

            for(i=0, oi = sel->options; oi; i++, oi = oi->next)
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

    whichfont = getwebfont(doc, ti);
    
    if (fs->lf != webfonts[whichfont].handle)
    {
	fs->lf = webfonts[whichfont].handle;
	font_setfont(fs->lf);
    }

    if (fs->lfc != fg || fs->lbc != bg)
    {
	fs->lfc = fg;
	fs->lbc = bg;
	render_set_font_colours(fg, bg, doc);
    }

    fstr.s = str;
    fstr.x = fstr.y = fstr.term = (1 << 30);
    fstr.split = -1;

    font_strwidth(&fstr);

#if 1
    render_text(doc, str, hpos + ((ti->width - (sel->flags & rid_if_NOPOPUP ? 0 : GRIGHT_SIZE) - (fstr.x / MILIPOINTS_PER_OSUNIT)) >> 1), bline);
#else
    font_paint(str, font_OSCOORDS,
	       hpos + ((ti->width - (sel->flags & rid_if_NOPOPUP ? 0 : GRIGHT_SIZE) - (fstr.x / MILIPOINTS_PER_OSUNIT)) >> 1),
	       bline);
#endif
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
