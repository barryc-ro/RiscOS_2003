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

#include <limits.h>
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

#include "consts.h"
#include "memwatch.h"
#include "render.h"
#include "rcolours.h"
/*#include "fastfont.h"*/
#include "version.h"
#include "webfonts.h"
#include "util.h"

#if UNICODE
#include "utf8.h"
#include "Unicode/charsets.h"
#include "encoding.h"
#endif

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

void render_plinth_from_list(int bcol, wimp_paletteword *cols, int flags, int x, int y, int w, int h, antweb_doc *doc)
{
    /* total border width in pixels */
    int bw, xx, yy, ww, hh, col, i;

    if (cols == NULL)
	return;

    bw = cols[0].word/2;

    /* do background */
    if ((flags & render_plinth_NOFILL) == 0 && w > bw*2*2 && h > bw*2*2)
    {
	render_set_colour(bcol, doc);
	bbc_rectanglefill(x + bw*2, y + bw*2, w - bw*2*2, h - bw*2*2);
    }

    /* do top/left border */
    col = -1;
    ww = w - frontend_dx;
    hh = h - frontend_dy;
    xx = x;
    yy = y;
    for (i = 0; i < bw; i++, ww -= 4, hh -= 4, xx += 2, yy += 2)
    {
        bbc_move(xx, yy);

	if (col != cols[i+1].word)
	{
	    col = cols[i+1].word;
	    render_set_colour(col, doc);
	}

	if (hh > 0)
	    bbc_drawby(0, hh);
	if (ww > 0)
	    bbc_drawby(ww, 0);
    }

    /* do bottom/right border */
    col = -1;
    ww = w - frontend_dx;
    hh = h - frontend_dy;
    xx = x;
    yy = y;
    for (i = bw; i < bw*2; i++, ww -= 4, hh -= 4, xx += 2, yy += 2)
    {
        bbc_move(xx, yy);

	if (col != cols[i+1].word)
	{
	    col = cols[i+1].word;
	    render_set_colour(col, doc);
	}

	if (ww > 0)
	    bbc_drawby(ww, 0);
	if (hh > 0)
	    bbc_drawby(0, hh);
    }
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
#ifdef STBWEB
	/* make colour whilst loading black as it looks much better on TV's */
	if (doc == NULL || doc->rh == NULL)
	{
	    pw.word = 0x00000000;
	    return pw;
	}
	else
#endif
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

static int render__text_link_colour(rid_text_item *ti, antweb_doc *doc, BOOL allow_font_colour)
{
    int rcol;
    int no;    /* defined in rid.h */

    if (ti->flag & rid_flag_ACTIVATED)
    {
	rcol = render_colour_ACTIVATED;
    }
    /* font colour set and allowed */
    else if ( allow_font_colour && (no = RID_COLOUR(ti)) != 0 && doc && (doc->flags & doc_flag_DOC_COLOURS) )
    {
	rcol = render_colour_RGB | doc->rh->extracolourarray[no];
    }
    /* if not a link */
    else if (ti->aref == NULL || ti->aref->href == NULL || (ti->aref->flags & rid_aref_LABEL))
    {
	rcol = render_colour_PLAIN;
    }
    /* if it is a link */
    else
    {
	rid_aref_item *aref = ti->aref;

	rcol = render_colour_AREF;

        if ((aref->flags & rid_aref_CHECKED_CACHE) == 0)
        {
    	    char *linkval;

            linkval = url_join(BASE(doc), aref->href);

	    if (linkval && frontend_test_history(linkval))
                aref->flags |= (rid_aref_IN_CACHE | rid_aref_CHECKED_CACHE);
            else
                aref->flags |= rid_aref_CHECKED_CACHE;

	    if (linkval)
	        mm_free(linkval);
        }

        if (aref->flags & rid_aref_IN_CACHE)
	    rcol = render_colour_CREF;
    }
    return rcol;
}


/*
 * This is used for images (links and unlinks).
 */

int render_link_colour(rid_text_item *ti, antweb_doc *doc)
{
    int rcol;
    if (ti->flag & rid_flag_ACTIVATED)
	rcol = render_colour_ACTIVATED;
    else if (backend_is_selected(doc, ti))
	rcol = render_colour_HIGHLIGHT;
    else
        rcol = render__text_link_colour(ti, doc, FALSE);
    return rcol;
}

/*
 * This is used for text links. Note that (in the function above) rh is passed
 * as NULL (in this case aref should never be null though)
 * SJM: 210597 reordered as check activated, check font colout, check non-link, check link
 * previously was non-link (including font colour check), activated, link.
 * SJM: 110697 now passes FALSE to above, negating the whole point of the change as
 * it appears I was wrong about font colour and links. This may change again if I can
 * find the original bug that made me change it.
 * SJM: changed again because the above is rubbish and breaks font colour totally.
 */

int render_text_link_colour(rid_text_item *ti, antweb_doc *doc)
{
    return render__text_link_colour(ti, doc, TRUE);
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

        if ( st->bgcolour )
            return render_colour_RGB | ( st->bgcolour & ~1 );
        /* wonder whether cc realises that render_colour_RGB has its bottom
         * bit set?
         */
    }

    if ( doc && doc->rh && ( doc->rh->bgt & rid_bgt_COLOURS) )
	return render_colour_RGB | doc->rh->colours.back;

    return render_colour_BACK;
}

void *render_sprite_locate(const char *sprite, void **sptr_out)
{
    sprite_id id;
    sprite_ptr sph = NULL;
    sprite_area *area;
    os_regset r;
    os_error *ep;

    id.tag = sprite_id_name;
    id.s.name = (char *)sprite;

    area = resspr_area();
    ep = sprite_select_rp(area, &id, &sph);

    if (ep)
    {
	ep = os_swix(Wimp_BaseOfSprites, &r);
	if (ep)
	    return NULL;

	area = (sprite_area *) (long) r.r[1];

	ep = sprite_select_rp(area, &id, &sph);

	if (ep)
	{
	    area = (sprite_area *) (long) r.r[0];

	    sprite_select_rp(area, &id, &sph);
	}
    }

    if (sptr_out)
	*sptr_out = sph;

    return sph ? area : NULL;
}

os_error *render_plot_icon(char *sprite, int x, int y)
{
    sprite_pixtrans pt[256];
    sprite_factors facs;
    sprite_area *area;
    sprite_id id;
    os_error *ep;

/*  DBG(("render_plot_icon: %s\n", sprite)); */
    
    id.tag = sprite_id_addr;
    area = render_sprite_locate(sprite, &id.s.addr);

/*  DBG(("render_plot_icon: area %p addr %p\n", area, id.s.addr)); */

    if (area == NULL)
	return NULL;
    
    /* read the scaling factors */
    if ((ep = wimp_readpixtrans(area, &id, &facs, pt)) != NULL)
	return ep;

    /* if 8bpp or more then read the colourtrans table separately */
    if (bbc_modevar(((sprite_header *)id.s.addr)->mode, bbc_Log2BPP) > 2)
    {
	if ((ep = (os_error *)_swix(ColourTrans_GenerateTable, _INR(0,5), area, id.s.addr, -1, -1, pt, 1)) != NULL)
	    return ep;
    }

    return sprite_put_scaled(area, &id, 0x8, x, y, &facs, pt);
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
	if (bbc_modevar(-1, bbc_Log2BPP) > 3)
	{
/*  	    h = (1<<26) | (1<<27) | (0xFF << 16); */
	}
	else
	{
	    wimp_paletteword fg_rgb, bg_rgb;
	    int fg, bg;

	    bg_rgb.word = back == -1 ? render_get_colour(render_colour_WRITE, doc).word : back;
	    colourtran_returnGCOL(bg_rgb, &bg);

	    fg_rgb.word = cursor == -1 ? 0x0000FF00 : cursor;
	    colourtran_returnGCOL(fg_rgb, &fg);

	    h = (1<<26) | (1<<27) | ((fg ^ bg) << 16);

	    RENDBGN(("bg rgb %08x col %d\nfg rgb %08x col %d\nheight %x\n", bg_rgb.word, bg, fg_rgb.word, fg, h));
	}
    }
    return h;
}

int render_text(be_doc doc, int font_index, const char *text, int x, int y)
{
    return render_text_full(doc, font_index, text, x, y, NULL, -1);
}

typedef struct
{
    int fh;
    int flags;
    int x, y;
    int *coords;
} paint_info;

static os_error *paint_fn(const char *text, BOOL last, void *handle)
{
    paint_info *pi = handle;
    os_error *e;

    /* DBG(("paint_fn: paint '%s' at %d,%d\n", text, pi->x, pi->y)); */

    e = (os_error *)_swix(Font_Paint, _INR(0,5),
			  pi->fh, text, pi->flags, pi->x, pi->y, pi->coords);

#if DEBUG
    if (e) DBG(("paint_fn: paint e %x '%s'\n", e->errnum, e->errmess));
#endif
    
    if (!e && !last)
    {
	int w, h;
	
	e = (os_error *)_swix(Font_ScanString, _INR(0,4) | _OUTR(3,4),
			      pi->fh, text, pi->flags, INT_MAX, INT_MAX, &w, &h);
	
	pi->x += w;
	/* pi->y += h; */

#if DEBUG
	if (e) DBG(("paint_fn: scan e %x '%s'\n", e->errnum, e->errmess));
#endif
    }

    return e;
}

int render_text_full(be_doc doc, int font_index, const char *text, int x, int y, int *coords, int n)
{
    int flags, fh;

    if (text == NULL || *text == 0)
	return FALSE;

#if 1
    if (config_display_blending)
	flags = 1<<11;
#else
    flags = 0;
    /* do we need font blending? */
    if (config_display_blending &&
	doc && (doc->flags & doc_flag_DOC_COLOURS))
    {
	rid_header *rh = doc->rh;
	if ((rh->bgt & rid_bgt_IMAGE) &&
	    rh->tile.im &&
	    (rh->tile.width != 0))
	{
	    flags |= 1<<11;
	}
    }
#endif
    
    if (coords)
	flags |= (1<<5);

    fh = 0;
    if (font_index != -1)
    {
	fh = webfonts[font_index].handle;
	flags |= 1<<8;
    }

#if UNICODE
    if (config_encoding_internal == 1 /* doc && doc->rh->encoding_write == csUTF8 */ && font_index != -1 && webfont_latin(font_index))
    {
	/* we have a utf8 stream which we want to display through an 8 bit font */
	paint_info pi;

	RENDBG(("render_text_full: font_index %d latin %d\n", font_index, webfont_latin(font_index)));
	
	pi.fh = fh;
	pi.flags = flags;
	pi.x = x * MILIPOINTS_PER_OSUNIT;
	pi.y = y * MILIPOINTS_PER_OSUNIT;
	pi.coords = coords;

	process_utf8_as_latin1(text, n, paint_fn, &pi);
    }
    else
#endif
    {
	if (n >= 0)
	    flags |= 1<<7;
    
	_swix(Font_Paint, _INR(0,5) | _IN(7), fh, text, flags, x * MILIPOINTS_PER_OSUNIT, y * MILIPOINTS_PER_OSUNIT, coords, n);
    }

    return TRUE;
}

/* eof render.c */
