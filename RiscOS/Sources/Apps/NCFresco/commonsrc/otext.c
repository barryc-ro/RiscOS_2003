/* -*-c-*- */
/* CHANGE LOG

 *  8/5/96: SJM: added SUB and SUP.
 * 11/6/96: SJM: corrected underline position for SUB and SUP.
 * 13/6/96: SJM: added _update_highlight method.
 * 21/6/96: SJM: added boxing for selected text items.
 * 05/7/96: SJM: added STRIKE.

 */


/* otext.c */

/* Methods for text objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "drawftypes.h"
#include "wimp.h"
#include "font.h"
#include "bbc.h"
#include "swis.h"

#include "interface.h"
#include "antweb.h"
#include "rid.h"
#include "webfonts.h"
#include "consts.h"
#include "config.h"
#include "util.h"
#include "render.h"
#include "rcolours.h"
/*#include "fastfont.h"*/
#include "flexwrap.h"

#include "object.h"
#include "version.h"
#include "dfsupport.h"

/* Make this 1 to see item boundaries */
#define DEBUG_ITEMS 0

/* An empty text object does not have any padding either. */
/* This is so the object inserted by <BR> has no effective width */

void otext_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_text *tit = (rid_text_item_text *) ti;
    size_t str_len;
#ifdef BUILDERS
    str_len = strlen(rh->texts.data + tit->data_off);
    ti->width = str_len * 10;
    ti->pad = str_len == 0 ? 0 : 1;
    ti->max_up = 1;
    ti->max_down = 1;
#else /* BUILDERS */
    font_string fs;
    struct webfont *wf;
    int len;

    str_len = strlen(rh->texts.data + tit->data_off);

    wf = &webfonts[ti->st.wf_index];

    font_setfont(wf->handle);

    fs.x = 1 << 30;
    fs.y = 1 << 30;
    fs.split = -1;

    flexmem_noshift();
    fs.term = len = str_len;
    fs.s = rh->texts.data + tit->data_off;
    font_strwidth(&fs);
    flexmem_shift();

    ti->pad = (fs.x + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT;

    fs.x = 1 << 30;
    fs.y = 1 << 30;
    fs.split = -1;

    flexmem_noshift();
    while(len && (rh->texts.data + tit->data_off)[len-1] == ' ')
	len--;
    fs.term = len;
    fs.s = rh->texts.data + tit->data_off;
    font_strwidth(&fs);
    flexmem_shift();

    ti->width = (fs.x + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT;

    if (str_len == 0)
    {
	ti->pad = 0;
    }
    else
    {
	ti->pad -= ti->width;
    }

#if 1
    if (str_len == 0)
    {
        /* FIXME: We shouldn't really give blank things the height of the
         * default font, as everything else on the line may be shorter than
         * that.
         * Sadly, setting these to 0 makes <br><br><br> etc *not* give lots of
         * blank space... trouble is, we can't tell at this point whether
         * we're the only thing on the line or not.

	 * 25/10/96: if we leave descenders off then this fixes the problem
	 * with <IMG><BR><IMG> giving spaces between images.
	 */
	ti->max_up = wf->max_up;
	ti->max_down = 0;
    }
    else
#endif
    {
        ti->max_up = wf->max_up;
        ti->max_down = wf->max_down;
    }

    switch (ti->st.flags & (rid_sf_SUB|rid_sf_SUP))
    {
        case rid_sf_SUB:
            ti->max_down += wf->max_up/4;
            ti->max_up -= wf->max_up/4;
            break;

        case rid_sf_SUP:
            ti->max_down -= wf->max_up/2;
            ti->max_up += wf->max_up/2;
            break;
    }
#endif /* BUILDERS */
}

#ifdef STBWEB
static void draw_partial_box(BOOL first, BOOL last, int x, int y, int w, int h)
{
    int dx = frontend_dx, dy = frontend_dy;
    int ww = last ? w : w - dx;

    if (first)
    {
        bbc_move(x, y);
        bbc_drawby(0, h-dy);
    }
    else
        bbc_move(x, y + h - dy);

    if (ww > 0)
	bbc_drawby(ww, 0);

    if (last)
        bbc_drawby(0, - (h-dy));
    else
        bbc_moveby(0, - (h-dy));

    if (ww > 0)
	bbc_drawby(-ww, 0);
}
#endif

#if 0
/* Draw a line using the draw module - gets round some printing problems - Oh no it doesn't !! */

static void draw_line(int x, int baseline, int w)
{
    int buf[7], *bp = buf;
    int cap[4];
    int trans[6];
    _kernel_oserror *e;

    *bp++ = 2;			/* move */
    *bp++ = x << 8;
    *bp++ = baseline << 8;

    *bp++ = 8;			/* line */
    *bp++ = (x + w) << 8;
    *bp++ = baseline << 8;

    *bp++ = 0;			/* end */

    cap[0] = 0;
    cap[1] = 0;
    cap[2] = 0;
    cap[3] = 0;

    trans[0] = 0x00010000;
    trans[1] = 0;
    trans[2] = 0;
    trans[3] = 0x00010000;

    trans[4] = 0;
    trans[5] = 0;

    e = _swix(Draw_Stroke, _INR(0,6), buf, 0x30, trans, 0, 4 << 8, cap, 0);
    if (e) usrtrc("Draw_Stroke: %x %s\n", e->errnum, e->errmess);
}
#endif

void otext_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_text_item_text *tit = (rid_text_item_text *) ti;
    int tfc, tbc;
    struct webfont *wf;
    int b;
    BOOL no_text, draw_highlight_box;

    if ((ti->flag & rid_flag_FVPR) == 0)
	return;

#ifdef STBWEB
    draw_highlight_box = ti->aref && (ti->aref->href || ti->aref->flags & rid_aref_LABEL) &&
	((ti->flag & (rid_flag_SELECTED|rid_flag_ACTIVATED)) == rid_flag_SELECTED);

    if (draw_highlight_box && (doc->flags & doc_flag_SOLID_HIGHLIGHT))
    {
	render_set_colour(render_link_colour(ti, doc), doc);
	bbc_rectanglefill(hpos, bline - ti->max_down, ti->width + ti->pad, ti->max_up + ti->max_down);
	draw_highlight_box = FALSE;
    }
#endif
    
    wf = &webfonts[ti->st.wf_index];

    if (fs->lf != wf->handle)
    {
	fs->lf = wf->handle;
	font_setfont(fs->lf);
    }

    tfc = render_text_link_colour(rh, ti, doc);
    tbc = render_background(rh, ti, doc);

    if ( fs->lfc != tfc || fs->lbc != tbc )
    {
	fs->lfc = tfc;
	fs->lbc = tbc;
	render_set_font_colours(fs->lfc, fs->lbc, doc);
    }

    /* adjust base line for subscipts and superscripts */
    b = bline;
    switch (ti->st.flags & (rid_sf_SUB|rid_sf_SUP))
    {
    case rid_sf_SUB:
	b -= wf->max_up/4;
	break;

    case rid_sf_SUP:
	b += wf->max_up/2;
	break;
    }

#if DEBUG_ITEMS
    bbc_move( hpos, b - ti->max_down );
    bbc_drawby( ti->width, ti->max_up + ti->max_down );
#endif

    flexmem_noshift();

    {
	char *s = rh->texts.data + tit->data_off;

	if ( s && *s )
	    font_paint(s,
	               font_OSCOORDS + (config_display_blending ? 0x800 : 0),
	               hpos, b/*,
	               ti->width + ti->pad*/ );

	no_text = *s == '\0' && ti->pad == 0;
    }

    flexmem_shift();

#ifdef STBWEB
    if (draw_highlight_box)
    {
	BOOL first = ti->aref->first == ti;
	BOOL last = ti->next == NULL || ti->next->aref == NULL || ti->next->aref != ti->aref;
	BOOL first_in_line = ti == ti->line->first;
	BOOL last_in_line = ti->next == ti->line->next->first;
	int width, height;

	if (!no_text || (first ^ last))
	{
	    first = first || first_in_line;
	    last = last || last_in_line || ti->next->tag != ti->tag; /* if type changes then coutn as end of line */

	    width = ti->width + (last ? 0 : ti->pad);
	    height = ti->max_up + ti->max_down - frontend_dy;

	    render_set_colour(render_link_colour(ti, doc), doc);

	    draw_partial_box(first, last, hpos, b - ti->max_down, width, height);

#if ANTI_TWITTER
	    if (first)
	    {
		hpos += 2;
		width -= 2;
	    }

	    if (last)
		width -= 2;

	    draw_partial_box(first, last, hpos, b - ti->max_down + 2, width, height - 4);
#endif
	}
    }
    else
#endif /* STBWEB */
	if ((ti->st.flags & rid_sf_UNDERLINE) || ((doc->flags & doc_flag_UL_LINKS) && ti->aref && ti->aref->href && (ti->aref->flags & rid_aref_LABEL) == 0) )
	{
	    if (!no_text)
	    {
		BOOL last = ti->next == ti->line->next->first;
		int w = last ? ti->width : ti->width + ti->pad;

		render_set_colour(fs->lfc, doc);

#if 0
		if (doc->flags & doc_flag_USE_DRAW_MOD)
		    draw_line(hpos, b - 5, w);
		else
#endif
		{
		    bbc_move(hpos, b - 4);
		    bbc_drawby(w, 0);
#if ANTI_TWITTER
		    bbc_move(hpos, b - 6);
		    bbc_drawby(w, 0);
#endif
		}
	    }
	}

    /* do STRIKE */
    if (ti->st.flags & rid_sf_STRIKE)
    {
	BOOL last = ti->next == ti->line->next->first;
	int w = last ? ti->width : ti->width + ti->pad;
	int bb = b + wf->max_up/4;

	render_set_colour(fs->lfc, doc);

#if 0
	if (doc->flags & doc_flag_USE_DRAW_MOD)
	    draw_line(hpos, bb, w);
	else
#endif
	{
	    bbc_move(hpos, bb);
	    bbc_drawby(w, 0);
#if ANTI_TWITTER
	    bbc_move(hpos, bb + 2);
	    bbc_drawby(w, 0);
#endif
	}
    }

#endif /* BUILDERS */
}



#if 0
void otext_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
}

char *otext_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, wimp_mousestr *m)
{
    return "";			/* Follow links, no fuss at all */
}
#endif

void otext_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    rid_text_item_text *tit = (rid_text_item_text *) ti;

#if 0
    fprintf(stderr, "rh = 0x%p, texts = 0x%p, data = 0x%p\n", rh, &(rh->texts), rh->texts.data);
#endif
    flexmem_noshift();
    fputs(rh->texts.data + tit->data_off, f);
    flexmem_shift();
}

void otext_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
#ifndef BUILDERS
    rid_text_item_text *tit = (rid_text_item_text *) ti;
    rid_header *rh = doc->rh;
    draw_textstrhdr txt;
    int len, l2;
    draw_textstyle dts = {0};
    int size;
    char zero[4] = {0,0,0,0};
    char *cp;
    wimp_box tb;

    flexmem_noshift();
    cp = rh->texts.data + tit->data_off;
    len = strlen(cp);
    while(len && cp[len-1] == ' ')
	len--;
    flexmem_shift();

    l2 = (len + 4) & (~3);

    size = WEBFONT_SIZEOF(ti->st.wf_index);
    size = config_font_sizes[size];
    size *= 640;

    txt.tag = draw_OBJTEXT;
    txt.size = sizeof(txt) + l2;
    txt.bbox.x0 = x << 8;
    txt.bbox.y0 = (y - ti->max_down) << 8;
    txt.bbox.x1 = (x + ti->width) << 8;
    txt.bbox.y1 = (y + ti->max_up) << 8;
    txt.textcolour = (int) render_get_colour(render_link_colour(ti, doc), doc).word;
    txt.background = (int) render_get_colour(render_colour_BACK, doc).word;
    dts.fontref = ((ti->st.wf_index & WEBFONT_FLAG_MASK) >> WEBFONT_FLAG_SHIFT) + 1;
    txt.textstyle = dts;
    txt.fsizex = size;
    txt.fsizey = size;
    txt.coord.x = x << 8;
    txt.coord.y = y << 8;

    tb.x0 = txt.bbox.x0 >> 8;
    tb.y0 = txt.bbox.y0 >> 8;
    tb.x1 = txt.bbox.x1 >> 8;
    tb.y1 = txt.bbox.y1 >> 8;

    df_stretch_bb(bb, &tb);

    df_write_data(fh, *fileoff, &txt, sizeof(txt));
    *fileoff += sizeof(txt);
    flexmem_noshift();
    df_write_data(fh, *fileoff, rh->texts.data + tit->data_off, len);
    flexmem_shift();
    *fileoff += len;
    df_write_data(fh, *fileoff, zero, l2 - len);
    *fileoff += l2-len;
#endif /* BUILDERS */
}

void otext_update_highlight(rid_text_item *ti, antweb_doc *doc)
{
    wimp_box trim;
    memset(&trim, 0, sizeof(trim));

    trim.x1 = ti->pad + 2;
    antweb_update_item_trim(doc, ti, &trim, TRUE);
}

/* eof otext.c */
