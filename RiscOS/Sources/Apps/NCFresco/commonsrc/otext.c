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

#include <limits.h>
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
#include "gbf.h"

#if UNICODE
#include "Unicode/encoding.h"
#endif

/* Make this 1 to see item boundaries */
#define DEBUG_ITEMS 0

#if DEBUG
static void dump_data(const char *s, int len)
{
    int i;
    fprintf(stderr, "data: (%2d)", len);

    for (i = 0; i < len; i++)
	fprintf(stderr, " %02x", s[i]);

    fprintf(stderr, "\n          ");
    for (i = 0; i < len; i++)
	fprintf(stderr, "  %c", s[i] < ' ' ? '.' : s[i]);

    fputc('\n', stderr);
}
#endif

/* An empty text object does not have any padding either. */
/* This is so the object inserted by <BR> has no effective width */

void otext_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    rid_text_item_text *tit = (rid_text_item_text *) ti;
    size_t str_len;

#ifdef BUILDERS
    str_len = strlen(rh->texts.data + tit->data_off);
    ti->width = str_len * rh->cwidth;
    ti->pad = str_len == 0 ? 0 : rh->cwidth;
    ti->max_up = 1;
    ti->max_down = rh->cwidth == 1 ? 0 : 1;
#else /* BUILDERS */
    int whichfont;
    struct webfont *wf;
    char *s;

    flexmem_noshift();		/* no shift whilst accessing text array */

    s = rh->texts.data + tit->data_off;
    str_len = strlen(s);

    /* get font descriptor */
    whichfont = antweb_getwebfont(doc, ti, -1);
    wf = &webfonts[whichfont];

#if UNICODE
    RENDBGN(("otext: enc %d lang '%s' f %d f1 %d\n", doc->rh->encoding, encoding_default_language(doc->rh->encoding), whichfont, ti->st.wf_index));
#endif

    if (str_len == 0)
    {
	ti->width = 0;
#if NEW_BREAKS
	ti->pad = ti->flag & rid_flag_SPACE ? wf->space_width : 0;
#else
	ti->pad = 0;
#endif
    }
    else
    {
#if NEW_BREAKS
	int width = webfont_font_width(whichfont, s);

	ti->width = width;
	ti->pad = ti->flag & rid_flag_SPACE ? wf->space_width : 0;

	RENDBGN(("otext: scanstring '%s' str_len %d width %d pad %d font %d enc %d\n", s, str_len, ti->width, ti->pad, whichfont, doc->rh->encoding));
#else
	int flags; 
	int width1, width2;
	int len;

	/* set font and read width */
/* 	font_setfont(wf->handle); */
	flags |= (1<<8)/*  | (1<<7) */;

	_swix(Font_ScanString, _INR(0,7) | _OUT(3),
	      wf->handle, s, flags,
	      INT_MAX, INT_MAX,
	      NULL, NULL, str_len,
	      &width1);

	if (flags & (1<<12))
	{
	    width2 = width1;
	}
	else
	{
	    flags |= 1<<7;

	    len = str_len;
	    while (len && s[len-1] == ' ')
		len--;

	    /* read width again without added spaces */
	    _swix(Font_ScanString, _INR(0,7) | _OUT(3),
		  wf->handle, s, flags,
		  INT_MAX, INT_MAX,
		  NULL, NULL, len,
		  &width2);
	}
	
	ti->width = (width2 + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT;
	ti->pad = (width1 + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT - ti->width;
#endif
    }

    flexmem_shift();		/* shiftable */

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

void otext_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    rid_text_item_text *tit = (rid_text_item_text *) ti;
    int tfc, tbc;
    int whichfont;
    struct webfont *wf;
    int b;
    BOOL no_text, draw_highlight_box;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    if (update == object_redraw_BACKGROUND)
	return;
    
#ifdef STBWEB
    draw_highlight_box = ti->aref && ti->aref->href &&
	((ti->aref->flags & rid_aref_LABEL) == 0 || ti->aref->href) && /* SJM: 11/05/97: made LABELs not cause text to highlight */
	((ti->flag & (rid_flag_SELECTED|rid_flag_ACTIVATED)) == rid_flag_SELECTED);
#endif

    whichfont = antweb_getwebfont(doc, ti, -1);
    wf = &webfonts[whichfont];

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

    if (update == object_redraw_HIGHLIGHT)
    {
	if (draw_highlight_box && (doc->flags & doc_flag_SOLID_HIGHLIGHT) == 0)
	    highlight_draw_text_box(ti, doc, b, hpos, TRUE);
	return;
    }

    /* quick exit if there is no text to display */
    /* removed because it upsets the highligt box drawing */

    if (rh->texts.data[tit->data_off] == 0)
	return;

    tfc = render_text_link_colour(ti, doc);
    tbc = render_background(ti, doc);

#ifdef STBWEB
    if (draw_highlight_box && (doc->flags & doc_flag_SOLID_HIGHLIGHT))
    {
	tbc = render_link_colour(ti, doc);

	render_set_colour(tbc, doc);

	bbc_rectanglefill(hpos, bline - ti->max_down, ti->width + ti->pad, ti->max_up + ti->max_down);
	draw_highlight_box = FALSE;
    }
#endif

    if ( fs->lfc != tfc || fs->lbc != tbc )
    {
	fs->lfc = tfc;
	fs->lbc = tbc;
	render_set_font_colours(tfc, tbc, doc);
    }

#if DEBUG_ITEMS
    render_set_colour(0x0000ff00 | render_colour_RGB, doc);
    bbc_rectangle( hpos, b - ti->max_down, ti->width, ti->max_up + ti->max_down);
#endif

    flexmem_noshift();

    RENDBGN(("otext_redraw: font %d fg %08x bg %08x text '%s' w %d vlink=%08x - %08x\n", whichfont, tfc, tbc, rh->texts.data + tit->data_off, ti->width, doc->rh->colours.vlink, render_get_colour(render_colour_CREF, doc).word));

    /* dump_data(rh->texts.data + tit->data_off, strlen(rh->texts.data + tit->data_off)); */

    no_text = !render_text(doc, whichfont, rh->texts.data + tit->data_off, hpos, b);

    if (ti->pad)
	no_text = FALSE;
    flexmem_shift();

#ifdef STBWEB
    if (draw_highlight_box)
    {
/* 	highlight_draw_text_box(ti, doc, b, hpos, !no_text); */
    }
    else
#endif /* STBWEB */
	if ((ti->st.flags & rid_sf_UNDERLINE) || ((doc->flags & doc_flag_UL_LINKS) && ti->aref && ti->aref->href))
/* 						  ((ti->aref->flags & rid_aref_LABEL) == 0) || ti->aref->href)) */
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

/*  fprintf(stderr, "rh = 0x%p, texts = 0x%p, data = 0x%p\n", rh, &(rh->texts), rh->texts.data); */

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
    size = config_font_sizes[size-1];
    size *= 640 * config_display_scale/100;

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

int otext_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box)
{
    if (box)
    {
	int d = config_display_highlight_width*2 + 4;
       
	/* these values must tie up with what's in highlight_draw_text_box */
	box->x0 = ti == ti->aref->first || ti == ti->line->first ? -d : 0;
	box->y1 = ti->line == ti->aref->first->line ? d : 2;
	box->x1 = ti->pad + 2 + d;
	if (box->x1 < d)
	    box->x1 = d;
	box->y0 = -d;
    }

    return FALSE;
}

/* eof otext.c */
