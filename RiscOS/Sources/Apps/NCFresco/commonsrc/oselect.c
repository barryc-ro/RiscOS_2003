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
/*  int checked = FALSE; */
    int line_space;
    int i;
#ifdef SELECT_CURRENT_FONT
    struct webfont *wf;
#endif

    sel->doc = doc;

#ifdef SELECT_CURRENT_FONT
    wf = &webfonts[ti->st.wf_index];

    line_space = wf->max_up + wf->max_down;

    /* This isn't quite right but it will do for now */
    width = webfont_tty_width(6, 1); /* Length of '<none>' in chars */

    if (font_setfont(wf->handle) != 0)
	return;
#else
    line_space = webfonts[WEBFONT_TTY].max_up + webfonts[WEBFONT_TTY].max_down;
    width = webfont_tty_width(6, 1); /* Length of '<none>' in chars */

    if (font_setfont(webfonts[WEBFONT_TTY].handle) != 0)
	return;
#endif
    height = 0;

    for(oi = sel->options; oi; oi = oi->next)
    {
	/* @@@@ Borris asks if this will get freed correctly. 15/10/96 */

	if (oi->text == NULL)
	    oi->text = strdup(""); /* OK, so it uses loads of memory, but it should not happen in the first place */

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

    ti->width = width + 16;
    
    /* add on width for the the POPUP icon */
    if ((sel->flags & rid_if_NOPOPUP) == 0)
	ti->width += 48;

#ifndef SELECT_CURRENT_FONT
    ti->max_up = webfonts[WEBFONT_TTY].max_up + 6;
    ti->max_down = webfonts[WEBFONT_TTY].max_down + 8;
#else
    ti->max_up = wf->max_up + 6;
    ti->max_down = wf->max_down + 8;
#endif
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
    BOOL draw_selection_box = (ti->flag & rid_flag_SELECTED);
    int fg, bg;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (draw_selection_box)
    {
	if (sel->base.colours.select != -1)
	    bg = sel->base.colours.select | render_colour_RGB;
	else
	    bg = render_colour_INPUT_S;
	draw_selection_box = FALSE;
    }
    else
    {
	bg = sel->base.colours.back == -1 ? render_colour_INPUT_B : sel->base.colours.back | render_colour_RGB;
    }

    fg = sel->base.colours.back == -1 ? render_colour_INPUT_F : render_text_link_colour(ti, doc);

    
#ifdef STBWEB
    render_plinth_full(bg,
		       ti->flag & rid_flag_SELECTED ? plinth_col_HL_M : plinth_col_M, 
		       ti->flag & rid_flag_SELECTED ? plinth_col_HL_L : plinth_col_L, 
		       ti->flag & rid_flag_SELECTED ? plinth_col_HL_D : plinth_col_D,
		       render_plinth_RIM | render_plinth_DOUBLE_RIM,
		       hpos, bline - ti->max_down,
		       ti->width - (sel->flags & rid_if_NOPOPUP ? 4 : 52),
		       (ti->max_up + ti->max_down), doc );
#else
    render_plinth(bg, render_plinth_IN,
		  hpos, bline - ti->max_down,
		  ti->width - (sel->flags & rid_if_NOPOPUP ? 4 : 52),
		  (ti->max_up + ti->max_down), doc );
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

#ifdef SELECT_CURRENT_FONT
    if (fs->lf != webfonts[ti->st.wf_index].handle)
    {
	fs->lf = webfonts[ti->st.wf_index].handle;
	font_setfont(fs->lf);
    }
#else
    if (fs->lf != webfonts[WEBFONT_TTY].handle)
    {
	fs->lf = webfonts[WEBFONT_TTY].handle;
	font_setfont(fs->lf);
    }
#endif

    if (fs->lfc != fg)
    {
	fs->lfc = fg;
	render_set_font_colours(fs->lfc, bg, doc);
    }

    fstr.s = str;
    fstr.x = fstr.y = fstr.term = (1 << 30);
    fstr.split = -1;

    font_strwidth(&fstr);

    font_paint(str, font_OSCOORDS, hpos + ((ti->width - (sel->flags & rid_if_NOPOPUP ? 20 : 68) - (fstr.x / MILIPOINTS_PER_OSUNIT)) >> 1) + 10, bline);

    if ((sel->flags & rid_if_NOPOPUP) == 0)
	render_plot_icon("gright", hpos + ti->width - 48, bline + ((ti->max_up - ti->max_down) >> 1) - 22);

    /* SJM */
    if (draw_selection_box)
    {
	render_set_colour(render_colour_HIGHLIGHT, doc);
	render_item_outline(ti, hpos, bline);
    }
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
                                               sel->size, ti->width-48-16 );
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

/* eof oselect.c */
