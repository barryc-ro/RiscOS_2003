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

static struct webfont *getwebfont(antweb_doc *doc, rid_text_item *ti)
{
    struct webfont *wf;
    if (doc->encoding != be_encoding_LATIN1 && (ti->flag & rid_flag_WIDE_FONT))
	wf = &webfonts[(ti->st.wf_index & WEBFONT_SIZE_MASK) | WEBFONT_JAPANESE];
    else
	wf = &webfonts[ti->st.wf_index];
    return wf;
}

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
    struct webfont *wf;
    const char *s;

    flexmem_noshift();		/* no shift whilst accessing text array */

    s = rh->texts.data + tit->data_off;
    str_len = strlen(s);

    /* check to see if there are any wide characters in there */
    if (doc->encoding != be_encoding_LATIN1)
    {
	int i;
	for (i = 0; i < str_len; i++)
	    if (s[i] & 0x80)
	    {
		ti->flag |= rid_flag_WIDE_FONT;
		break;
	    }
    }

    /* get font descriptor */
    wf = getwebfont(doc, ti);

    if (str_len == 0)
    {
	ti->width = 0;
	ti->pad = 0;
    }
    else
    {
	int flags = doc->encoding != be_encoding_LATIN1 && (ti->flag & rid_flag_WIDE_FONT) ? 1<<12 : 0;
	int width1, width2;
	int len;

	/* set font and read width */
/* 	font_setfont(wf->handle); */
	flags |= (1<<8) | (1<<7);

	_swix(Font_ScanString, _INR(0,7) | _OUT(3),
	      wf->handle, s, flags,	      
	      INT_MAX, INT_MAX,
	      NULL, NULL, str_len,
	      &width1);

	len = str_len;
	while (len && s[len-1] == ' ')
	    len--;

	/* read width again without added spaces */
	_swix(Font_ScanString, _INR(0,7) | _OUT(3),
	      wf->handle, s, flags,
	      INT_MAX, INT_MAX,
	      NULL, NULL, len,
	      &width2);

	ti->width = (width2 + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT;
	ti->pad = (width1 + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT - ti->width;

#if 0
	fprintf(stderr, "otext: scanstring '%s' str_len %d width1 %d width2 %d\n", s, str_len, width1, width2);
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

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

    /* quick exit if there is no text to display */
    if (rh->texts.data[tit->data_off] == 0)
	return;
    
    tfc = render_text_link_colour(ti, doc);
    tbc = render_background(ti, doc);

#ifdef STBWEB
    draw_highlight_box = ti->aref && (ti->aref->href || ti->aref->flags & rid_aref_LABEL) &&
	((ti->flag & (rid_flag_SELECTED|rid_flag_ACTIVATED)) == rid_flag_SELECTED);

    if (draw_highlight_box && (doc->flags & doc_flag_SOLID_HIGHLIGHT))
    {
	tbc = render_link_colour(ti, doc);

	render_set_colour(tbc, doc);

	bbc_rectanglefill(hpos, bline - ti->max_down, ti->width + ti->pad, ti->max_up + ti->max_down);
	draw_highlight_box = FALSE;
    }
#endif
    wf = getwebfont(doc, ti);

    if (fs->lf != wf->handle)
    {
	fs->lf = wf->handle;
	font_setfont(fs->lf);
    }

    if ( fs->lfc != tfc || fs->lbc != tbc )
    {
	fs->lfc = tfc;
	fs->lbc = tbc;
	render_set_font_colours(tfc, tbc, doc);
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

#if 0
    dump_data(rh->texts.data + tit->data_off, strlen(rh->texts.data + tit->data_off));
#endif
    
    no_text = !render_text(doc, rh->texts.data + tit->data_off, hpos, b);
    if (ti->pad)
	no_text = FALSE;

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
