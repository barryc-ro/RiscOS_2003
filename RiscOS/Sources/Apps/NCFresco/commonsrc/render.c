/* -*-c-*- */

/* render.c */

/*
 * 21/3/96: SJM: Took out RIM if anti-twittering, introduced render_colour_HIGHLIGHT for active link and cursor highlighting
 *               added render_item_outline
 * 2/4/96:  SJM: Changed render_link_colour to return HIGHLIGHT for all SELECTED items rather than just links.
 * 28/5/96: SJM: don't draw plinth if image isn't big enough
 * 24/6/96: SJM: added visited link colouring to render_link_colour, added render_text_link_colour.
 * 02/7/96: SJM: use of new flag ACTIVATED for when we click on a link
 * 10/9/96: SJM: render_set_(bg_)colour now returns the colour set (in it's 24 bit form).
 */

/* Bits of code to help with rendering */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wimp.h"
#include "font.h"
#include "bbc.h"
#include "colourtran.h"
#include "swis.h"
#include "resspr.h"

#include "interface.h"
#include "rid.h"
#include "config.h"
#include "antweb.h"
#include "access.h"
#include "url.h"

#include "memwatch.h"
#include "render.h"
#include "rcolours.h"
/*#include "fastfont.h"*/
#include "version.h"

static void draw_cornerBR(int x, int y, int w, int h, int thickness)
{
    int i;
    for (i = 0; i < thickness; i+=2)
    {
        bbc_move(x + i, y + i);
	if (w - frontend_dx - i*2 > 0)
	    bbc_drawby(w - frontend_dx - i*2, 0);
	if (h - frontend_dy - i*2 > 0)
	    bbc_drawby(0, h - frontend_dy - i*2);
    }
}

static void draw_cornerLT(int x, int y, int w, int h, int thickness)
{
    int i;
    for (i = 0; i < thickness; i+=2)
    {
        bbc_move(x + i, y + i);
	if (h - frontend_dy - i*2 > 0)
	    bbc_drawby(0, h - frontend_dy - i*2);
	if (w - frontend_dx - i*2 > 0)
	    bbc_drawby(w - frontend_dx - i*2, 0);
    }
}

void render_plinth(int bcol, int flags, int x, int y, int w, int h, antweb_doc *doc)
{
    render_plinth_full(bcol, render_colour_PLAIN,
		       flags & render_plinth_IN ? render_colour_LINE_D : render_colour_LINE_L,
		       flags & render_plinth_IN ? render_colour_LINE_L : render_colour_LINE_D,
		       flags, x, y, w, h, doc);
    
}

void render_plinth_full(int bcol, int rim_col, int top_col, int bottom_col, int flags, int x, int y, int w, int h, antweb_doc *doc)
{
    int off;

    /* If it is too small we have to make it thin */
    if ((h < 4) || (w < 4))
	flags |= render_plinth_THIN;

    off = flags & render_plinth_THIN ? 2 : 4;

    if ((flags & render_plinth_NOFILL) == 0 && w > off*2 && h > off*2)
    {
	render_set_colour(bcol, doc);
	bbc_rectanglefill(x + off, y + off, w - off*2, h - off*2);
    }

    /* If it is too small then we can't do a double */
    if ((h < off*2) || (w < off*2))
	flags &= ~render_plinth_DOUBLE;

    if ((flags & render_plinth_RIM) && w > 13 && h > 13)
    {
	render_set_colour(rim_col, doc);
	bbc_rectangle(x + 6, y + 6, w - (frontend_dx + 12), h - (frontend_dy + 12));

	if (flags & render_plinth_DOUBLE_RIM)
	    bbc_rectangle(x+off, y+off, w-2*off - frontend_dx, h-2*off - frontend_dy);
    }

    render_set_colour(bottom_col, doc);

    draw_cornerBR(x, y, w, h, off);
    if (flags & render_plinth_DOUBLE)
	draw_cornerLT(x+off, y+off, w-off*2, h-off*2, off);


    render_set_colour(top_col, doc);

    draw_cornerLT(x, y, w, h, off);
    if (flags & render_plinth_DOUBLE)
	draw_cornerBR(x+off, y+off, w-off*2, h-off*2, off);
}

#if 0
static int colour_distance(wimp_paletteword c1, wimp_paletteword c2)
{
    int r,g,b;
    r = (c1.bytes.red - c2.bytes.red);
    g = (c1.bytes.green - c2.bytes.green);
    b = (c1.bytes.blue - c2.bytes.blue);
    return (r*r + g*g + b*b)/3;
}
#endif

wimp_paletteword render_get_colour(int colour, be_doc doc)
{
    wimp_paletteword pw;

    switch (colour)
    {
    case render_colour_BACK:
	if (doc &&
	    (doc->flags & doc_flag_DOC_COLOURS) &&
	    doc->rh &&
	    (doc->rh->bgt & rid_bgt_COLOURS) )
	{
	    pw.word = doc->rh->colours.back;
	    return pw;
	}
	break;
    case render_colour_AREF:
	if (doc &&
	    (doc->flags & doc_flag_DOC_COLOURS) &&
	    doc->rh &&
	    (doc->rh->bgt & rid_bgt_LCOL) )
	{
	    pw.word = doc->rh->colours.link;
	    return pw;
	}
	break;
    case render_colour_CREF:
	if (doc &&
	    (doc->flags & doc_flag_DOC_COLOURS) &&
	    doc->rh &&
	    (doc->rh->bgt & rid_bgt_VCOL) )
	{
	    pw.word = doc->rh->colours.vlink;
	    return pw;
	}
	break;
    case render_colour_PLAIN:
	if (doc &&
	    (doc->flags & doc_flag_DOC_COLOURS) &&
	    doc->rh &&
	    (doc->rh->bgt & rid_bgt_FCOL) )
	{
	    pw.word = doc->rh->colours.fore;
	    return pw;
	}
	break;
#if 0
    case render_colour_LINE_L:
    case render_colour_LINE_D:
        if (doc &&
            (doc->flags & doc_flag_DOC_COLOURS) &&
            doc->rh &&
	    (doc->rh->bgt & (rid_bgt_COLOURS|rid_bgt_IMAGE)) == rid_bgt_COLOURS)
	{
            wimp_paletteword col;
	    col.word = doc->rh->colours.back;
	    pw.word = config_colours[colour].word;
	    if (colour_distance(col, pw) < 16*16)
		pw.word = config_colours[colour == render_colour_LINE_D ? render_colour_LINE_L : render_colour_LINE_D].word;
	    return pw;
        }
        break;
#endif

#if 0
	/* old way was to use the ALINK colour for the highlight - new is to just use whatever we configured */
    case render_colour_HIGHLIGHT:
	if (doc &&
	    (doc->flags & doc_flag_DOC_COLOURS) &&
	    doc->rh &&
	    (doc->rh->bgt & rid_bgt_ACOL) )
	{
            wimp_paletteword col;

	    pw.word = doc->rh->colours.alink;

            col = render_get_colour(render_colour_AREF, doc);
            if (colour_distance(pw, col) < 64*64)
                pw.word = col.word ^ 0xffffff00;

            col = render_get_colour(render_colour_BACK, doc);
            if (colour_distance(pw, col) < 64*64)
                pw.word = col.word ^ 0xffffff00;

	    return pw;
	}
#endif

    case render_colour_ACTIVATED:
	if (doc &&
	    (doc->flags & doc_flag_DOC_COLOURS) &&
	    doc->rh &&
	    (doc->rh->bgt & rid_bgt_ACOL) )
	{
	    pw.word = doc->rh->colours.alink;
	    return pw;
	}
	break;
    }

    if ( (unsigned int)colour >= (unsigned int)render_colour_RGB )
    {
        /* Too late now to check DOC_COLOURS as the information as to *which*
         * colour this is has been lost. DOC_COLOURS must be checked before
         * a colour_RGB colour is chosen in the first place
         */
	pw.word = colour & 0xFFFFFF00;
	return pw;
    }

    return config_colours[colour];
}

int render_set_colour(int colour, antweb_doc *doc)
{
    int junk;
    wimp_paletteword pal = render_get_colour(colour, doc);

    colourtran_setGCOL(pal, 0, 0, &junk);

    return pal.word;
}

int render_set_bg_colour(int colour, antweb_doc *doc)
{
    int junk;
    wimp_paletteword pal = render_get_colour(colour, doc);

    colourtran_setGCOL(pal, (1<<8) | (1<<7), 0, &junk);

    return pal.word;
}

int render_cell_background_colour(int i)
{
    wimp_paletteword pal;
    int junk;

    /* @@@@ Hacky! */
    pal.word = i;

    colourtran_setGCOL(pal, 0, 0, &junk);

    return pal.word;
}

void render_set_font_colours(int f, int b, antweb_doc *doc)
{
    int maxcols = (doc->flags & doc_flag_ANTIALIAS) ? 14 : 0;
    font fh;
    wimp_paletteword ff, bb;

    fh = 0;
    ff = render_get_colour(f, doc);
    bb = render_get_colour(b, doc);

    colourtran_setfontcolours( &fh, &bb, &ff, &maxcols );
}

/*
 * This is used for images (links and unlinks).
 */

int render_link_colour(rid_text_item *ti, antweb_doc *doc)
{
    int rcol;
    if (ti->flag & rid_flag_SELECTED)
	rcol = render_colour_HIGHLIGHT;
    else if (ti->flag & rid_flag_ACTIVATED)
	rcol = render_colour_ACTIVATED;
    else
        rcol = render_text_link_colour(ti, doc);
    return rcol;
}

/*
 * This is used for text links. Note that (in the function above) rh is passed
 * as NULL (in this case aref should never be null though)
 */

int render_text_link_colour(rid_text_item *ti, antweb_doc *doc)
{
    int rcol;

    if (ti->aref == NULL || ti->aref->href == NULL || (ti->aref->flags & rid_aref_LABEL))
    {
        /* pdh: It's not a link -- but what colour is it? */
        int no = RID_COLOUR(ti);    /* defined in rid.h */

        if ( no && doc && (doc->flags & doc_flag_DOC_COLOURS) )
            rcol = render_colour_RGB | doc->rh->extracolourarray[no];
        else
            rcol = render_colour_PLAIN;
    }
    else if (ti->flag & rid_flag_ACTIVATED)
	rcol = render_colour_ACTIVATED;
    else
    {
#if 0
	rcol = render_colour_AREF;
#else
	rid_aref_item *aref = ti->aref;

	rcol = render_colour_AREF;

        if ((aref->flags & rid_aref_CHECKED_CACHE) == 0)
        {
    	    char *linkval;

            linkval = url_join(BASE(doc), aref->href);
/*
            if (linkval && (frag = strrchr(linkval, '#')) != NULL)
                *frag = 0;
 */
	    if (linkval && frontend_test_history(linkval))
                aref->flags |= (rid_aref_IN_CACHE | rid_aref_CHECKED_CACHE);
            else
                aref->flags |= rid_aref_CHECKED_CACHE;

	    if (linkval)
	        mm_free(linkval);
        }

        if (aref->flags & rid_aref_IN_CACHE)
	    rcol = render_colour_CREF;
#endif
    }
    return rcol;
}

/*
 * Get hold of the right background colour, which, now tables can have
 * background colours, is not as easy as it used to be
 */

int render_background(rid_text_item *ti, antweb_doc *doc )
{
    if ( doc && !( doc->flags & doc_flag_DOC_COLOURS ) )
        return render_colour_BACK;

    if ( ti && ti->line )
    {
        struct rid_text_stream *st = ti->line->st;
#if 1
        if ( st->bgcolour )
            return render_colour_RGB | ( st->bgcolour & ~1 );
        /* wonder whether cc realises that render_colour_RGB has its bottom
         * bit set?
         */
#else
        if ( st && st->parent )
        {
            /* The shin-bone's connected to the ... ankle-bone */
            void *parent = st->parent;
            rid_table_props *props = NULL;

            switch ( st->partype )
            {
            case rid_pt_CAPTION:
                {
                    rid_table_caption *caption = (rid_table_caption*)parent;

                    if ( caption )
                        props = caption->props;
                }
                break;
            case rid_pt_CELL:
                {
                    rid_table_cell *cell = (rid_table_cell*)parent;
                    if ( cell )
                        props = cell->props;
                }
                break;
            }

            /* YCGLIYT I'm recursing all the way up to find a colour */
            if ( props
                 && ( props->flags & rid_tpf_BGCOLOR ) )
                    return render_colour_RGB | props->bgcolor;
        }
#endif
    }

    if ( doc && doc->rh && ( doc->rh->bgt & rid_bgt_COLOURS) )
	return render_colour_RGB | doc->rh->colours.back;

    return render_colour_BACK;
}

os_error *render_plot_icon(char *sprite, int x, int y)
{
    sprite_pixtrans pt[16], *ptp;
    sprite_factors facs;
    sprite_header *sph;
    sprite_area *area;
    sprite_id id;
    os_regset r;
    os_error *ep;

    id.tag = sprite_id_name;
    id.s.name = sprite;

    area = resspr_area();
    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

    if (ep)
    {
	ep = os_swix(Wimp_BaseOfSprites, &r);

	if (ep)
	    return ep;

	area = (sprite_area *) (long) r.r[1];

	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

	if (ep)
	{
	    area = (sprite_area *) (long) r.r[0];
	}
    }

    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

    if (ep)
	return ep;

    id.tag = sprite_id_addr;
    id.s.addr = sph;

    if ((ep = wimp_readpixtrans(area, &id, &facs, pt)) != NULL)
	return ep;

    if (bbc_modevar(sph->mode, bbc_Log2BPP) > 2)
	ptp = NULL;
    else
	ptp = pt;

    return sprite_put_scaled(area, &id, 0x8, x, y, &facs, ptp);
}


/* SJM */
void render_item_outline(rid_text_item *ti, int hpos, int bline)
{
    int dx, dy;

    dx = frontend_dx;
    dy = frontend_dy;

    bbc_move(hpos, bline - ti->max_down);
    bbc_drawby(ti->width - dx, 0);
    bbc_drawby(0, ti->max_down + ti->max_up - dy);
    bbc_drawby(- (ti->width - dx), 0);
    bbc_drawby(0, -(ti->max_down + ti->max_up - dy) );
#if ANTI_TWITTER
    bbc_move(hpos + 2, bline - ti->max_down + 2);
    bbc_drawby(ti->width - dx - 4, 0);
    bbc_drawby(0, ti->max_down + ti->max_up - dy - 4);
    bbc_drawby(- (ti->width - dx - 4), 0);
    bbc_drawby(0, -(ti->max_down + ti->max_up - dy - 4) );
#endif
}

int render_caret_colour(be_doc doc, int back, int cursor)
{
    int h = 0;
    if (cursor != -1 || back != -1)
    {
	wimp_paletteword fg_rgb, bg_rgb;
	int fg, bg;

	bg_rgb.word = back == -1 ? render_get_colour(render_colour_WRITE, doc).word : back;
	colourtran_returnGCOL(bg_rgb, &bg);

	fg_rgb.word = cursor == -1 ? 0x0000FF00 : cursor;
	colourtran_returnGCOL(fg_rgb, &fg);

	h = (1<<26) | (1<<27) | ((fg ^ bg) << 16);

#if 0
	fprintf(stderr, "bg rgb %08x col %d\nfg rgb %08x col %d\nheight %x\n", bg_rgb.word, bg, fg_rgb.word, fg, h);
#endif
    }
    return h;
}

int render_text(be_doc doc, const char *text, int x, int y)
{
    int flags = font_OSCOORDS;
    rid_header *rh = doc->rh;

    if (text == NULL || *text == 0)
	return FALSE;

    /* do we need font blending? */
    if (config_display_blending &&
	(doc->flags & doc_flag_DOC_COLOURS) &&
	(rh->bgt & rid_bgt_IMAGE) &&
	rh->tile.im &&
	(rh->tile.width != 0))
    {
	flags |= 1<<11;
    }

    /* do we need different character set? */
#if 1
    if (doc->encoding != be_encoding_LATIN1)
    {
	RENDBG(("render_text: set bit 12\n"));
	flags |= 1<<12;
    }
#endif

    font_paint((char *)text, flags, x, y);

    return TRUE;
}

/* eof render.c */
