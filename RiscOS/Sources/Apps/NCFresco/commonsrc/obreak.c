/* -*-c-*- */

/* obreak.c */

/* Methods for break (PBREAK and HLINE) objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wimp.h"
#include "bbc.h"

#include "antweb.h"
#include "interface.h"
#include "rid.h"
#include "consts.h"
#include "config.h"
#include "render.h"
#include "rcolours.h"
#include "dfsupport.h"

#include "object.h"
#include "version.h"
#include "gbf.h"

#ifdef STBWEB
#define RFLAGS (render_plinth_NOFILL | render_plinth_THIN | render_plinth_IN)
#else
#define RFLAGS (render_plinth_NOFILL | render_plinth_THIN)
#endif

#ifdef BUILDERS
extern
#else
static
#endif
void obreak_get_sizes(rid_text_item_hr *tih, int *left, int *right)
{
    int leftend = *left;
    int rightend = *right;
    int w = rightend - leftend;

    switch (tih->width.type)
    {
        case value_absunit:
            if (tih->width.u.f < w)
                w = (int)tih->width.u.f;
            break;

        case value_pcunit:
            if (tih->width.u.f <= 100)
                w = (int)(w*tih->width.u.f/100);
            break;
    }

    switch (tih->align)
    {
        case HTML_HR_ALIGN_LEFT:
            rightend = leftend + w;
            break;
        case HTML_HR_ALIGN_CENTER:
        case HTML_HR_ALIGN_CENTRE:
            leftend = (rightend + leftend - w)/2;
            rightend = leftend + w;
            break;
        case HTML_HR_ALIGN_RIGHT:
            leftend = rightend - w;
            break;
    }

    *left = leftend;
    *right = rightend;
}


void obreak_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_hr *tih = (rid_text_item_hr *)ti;

    ti->width = MAGIC_WIDTH_HR;
    ti->pad = 0;
    ti->max_up = tih->size/2 + PBREAK_SPACING/2;
    ti->max_down = ti->max_up;
}

void opbreak_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    ti->width = MAGIC_WIDTH_HR;
    ti->pad = 0;
    ti->max_up = PBREAK_HEIGHT;
    ti->max_down = 0;
}

void obreak_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_HIGHLIGHT || update == object_redraw_BACKGROUND)
	return;
    
    if (ti->tag == rid_tag_HLINE)
    {
        rid_text_item_hr *tih = (rid_text_item_hr *)ti;
	int leftend, rightend;

        antweb_get_edges(ti, &leftend, &rightend);

#if 1
	obreak_get_sizes(tih, &leftend, &rightend);

	if (tih->noshade)
	{
	    render_set_colour(render_colour_LINE_D, doc);
	    bbc_move(ox + leftend, bline - tih->size/2);
	    bbc_plot(bbc_RectangleFill + bbc_DrawRelFore, rightend - leftend, tih->size);
	}
	else
	{
	    render_plinth(0, RFLAGS, ox + leftend, bline - tih->size/2, rightend-leftend, tih->size, doc);
	}
#else
	render_set_colour(render_colour_LINE_L, doc);
	bbc_move(ox + leftend + 4, bline + (PBREAK_HEIGHT >> 1) - 2);
	bbc_drawby(0, frontend_dy);
	bbc_drawby((rightend - leftend) - 10, 0);

	render_set_colour(render_colour_LINE_D, doc);
	bbc_drawby(0, -frontend_dy);
	bbc_drawby(12 - (rightend-leftend), 0);
#endif
    }
#endif /* BUILDERS */
}

#if 0
void obreak_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
}

char *obreak_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, wimp_mousestr *m)
{
    return "";			/* Follow links, no fuss at all */
}
#endif

void obreak_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    if (ti->tag == rid_tag_HLINE)
	fputs("----------------------------------------------------------------------", f);
}

/* FIXME: SJM: haven't really finished the draw verion of new HR stuff */

void obreak_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
#ifndef BUILDERS
    rid_text_item_hr *tih = (rid_text_item_hr *)ti;
    wimp_paletteword trans, fill;
    wimp_box tb;
    int leftend, rightend;
    int toosmall;

    antweb_get_edges(ti, &leftend, &rightend);

    toosmall = (rightend - leftend) <= 24;

    if (ti->tag == rid_tag_HLINE)
        obreak_get_sizes(tih, &leftend, &rightend);

    trans.word = ~0;

    tb.x0 = x + leftend;
    if (!toosmall)
	tb.x0 += 8;
    tb.y0 = y + (PBREAK_HEIGHT >> 1) - 4;
    tb.x1 = x + rightend;
    if (!toosmall)
	tb.x1 -= 8;    
    tb.y1 = y + (PBREAK_HEIGHT >> 1) + 4;

    df_stretch_bb(bb, &tb);

    fill = (ti->tag == rid_tag_HLINE) ? render_get_colour(render_colour_PLAIN, doc) : trans;

    if (ti->tag == rid_tag_HLINE)
    {
	df_write_plinth(fh, &tb,
			render_get_colour(render_colour_LINE_L, doc),
			render_get_colour(render_colour_LINE_D, doc),
			fileoff);
    }
    else
    {
	df_write_filled_rect(fh, &tb, trans, trans, fileoff);
    }
#endif
}
