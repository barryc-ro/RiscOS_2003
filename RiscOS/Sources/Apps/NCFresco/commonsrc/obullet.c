/* -*-c-*- */

/* obullet.c */

/*
 * 21/3/96: SJM: obullet_size comes from indent.h, bullet character from BULLET_CHAR
 * 04/7/96: SJM: added list types
 */

/* Methods for bullet objects */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wimp.h"
#include "drawftypes.h"

#include "interface.h"
#include "antweb.h"
#include "rid.h"
#include "webfonts.h"
#include "consts.h"
#include "config.h"
#include "render.h"
#include "rcolours.h"

#include "dfsupport.h"
#include "object.h"

#include "indent.h"
#include "gbf.h"

#ifndef BULLET_CHAR
#define BULLET_CHAR '\x8F'
#endif

/* You don't want to set this to 1 */
#define DEBUG_DLCOMPACT 0

#ifndef BUILDERS
static void roman_one_five_ten(char *s, int n, char *oft)
{
    char *p = s + strlen(s);

    if ((n % 5) == 4)
    {
	*p++ = oft[0];
	n++;
    }

    if (n == 10)
    {
	*p++ = oft[2];
	n -= 10;
    }
    else if (n >= 5)
    {
	*p++ = oft[1];
	n -= 5;
    }

    while (n % 5)
    {
	*p++ = oft[0];
	n--;
    }

    *p = 0;
}
#endif

#ifndef BUILDERS
static char *roman_numeral(int n, int caps)
{
    /* Worst case 3888 = "MMMDCCCLXXXVIII" = 15 chars */
    static char buffer[16];
    char *letters = caps ? "IVXLCDM" : "ivxlcdm";

    if (n > 4000)
    {
	sprintf(buffer, "%d", n);
    }
    else
    {
	char *p = buffer;

	while (n >= 1000)
	{
	    *p++ = letters[6];
	    n -= 1000;
	}

	*p = 0;

	roman_one_five_ten(buffer, (n / 100) % 10, letters+4);
	roman_one_five_ten(buffer, (n /  10) % 10, letters+2);
	roman_one_five_ten(buffer, (n /   1) % 10, letters+0);
    }

    return buffer;
}
#endif

#ifndef BUILDERS
static char *obullet_string(rid_text_item_bullet *tib, BOOL symfont)
{
    static char buffer[24];		/* Big enough that it can be padded to a full word. */

    switch (tib->list_type)
    {
    case HTML_UL:
	buffer[0] = BULLET_CHAR;
	buffer[1] = (char)160;
	buffer[2] = 0;
	switch (tib->item_type)
	{
	case 0:     /* PLAIN */
	case HTML_UL_TYPE_PLAIN:
	    buffer[0] = 0;
	    break;

	    /* only override the standard bullet if we know the symbol font is available */
	case HTML_UL_TYPE_DISC:
	    if (symfont)
		buffer[0] = 108;
	    break;
	case HTML_UL_TYPE_SQUARE:
	    if (symfont)
		buffer[0] = 110;
	    break;
	case HTML_UL_TYPE_CIRCLE:
	    if (symfont)
		buffer[0] = 109;
	    break;
	}
	break;

    case HTML_OL:
	switch (tib->item_type)
	{
	default:
	case rid_bullet_ol_1:
	    sprintf(buffer, "%d", tib->list_no);
	    break;

	case rid_bullet_ol_a:
	    sprintf(buffer, "%c", 'a' + ((tib->list_no-1) % 26));
	    break;

	case rid_bullet_ol_A:
	    sprintf(buffer, "%c", 'A' + ((tib->list_no-1) % 26));
	    break;

	case rid_bullet_ol_I:
	case rid_bullet_ol_i:
	    sprintf(buffer, "%s", roman_numeral(tib->list_no,
						  (tib->item_type == rid_bullet_ol_i) ? 0 : 1 ));
	    break;
	}
	strcat(buffer, ")\240");
	break;

    case HTML_DL:       /* fake bullet for transferring info to formatter */
#if DEBUG_DLCOMPACT
        strcpy( buffer, "hoho" );
#else
        *buffer = 0;
#endif
        break;

    case HTML_MENU:
    case HTML_DIR:
    default:
	buffer[0] = BULLET_CHAR;
	buffer[1] = (char)160;
	buffer[2] = 0;
	break;
    }

    return buffer;
}
#endif

void obullet_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
    webfont *wf = &webfonts[ti->st.wf_index];
    rid_text_item_bullet *tib = (rid_text_item_bullet*) ti;

    /* Width comes from preformatted size */
    switch (tib->list_type)
    {
    case HTML_OL:
    case HTML_UL:
    case HTML_MENU:
    case HTML_DIR:
	ti->width = INDENT_WIDTH * INDENT_UNIT;
	break;

    case HTML_DL:
        ti->width = 0;
        break;

    default:
	ti->width = 2 * INDENT_UNIT;
	break;
    }
    ti->pad = 0;
    /* Height and deapth come from the real font */
    ti->max_up = wf->max_up;
    ti->max_down = wf->max_down;
}

void obullet_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update)
{
#ifndef BUILDERS
    char *buffer;
    int tfc, tbc;
    font_string fstr;
    font fh;
    rid_text_item_bullet *tib = (rid_text_item_bullet*) ti;
    BOOL symfont;

    if (gbf_active(GBF_FVPR) && (ti->flag & rid_flag_FVPR) == 0)
	return;

#if !DEBUG_DLCOMPACT
    if ( tib->list_type == HTML_DL )
        return;
#endif

    symfont = FALSE;
    fh = webfonts[ti->st.wf_index].handle;

    /* check and see if thew symbol font is available */
    if (tib->list_type == HTML_UL && tib->item_type != HTML_UL_TYPE_DISC)
    {
	font fhsym = webfonts[WEBFONT_SYMBOL(WEBFONT_SIZEOF(ti->st.wf_index))].handle;
	if (fhsym > 0)
	{
	    fh = fhsym;
	    symfont = TRUE;
	}
    }

    if (fs->lf != fh)
    {
	fs->lf = fh;
	font_setfont(fs->lf);
    }

    buffer = obullet_string(tib, symfont);

    fstr.x = 1 << 30;
    fstr.y = 1 << 30;
    fstr.split = -1;

    fstr.term = strlen(buffer);
    fstr.s = buffer;
    font_strwidth(&fstr);

    tfc = render_text_link_colour(rh, ti, doc);
    tbc = render_background(rh, ti, doc);

    if ( fs->lfc != tfc || fs->lbc != tbc )
    {
	fs->lfc = tfc;
	fs->lbc = tbc;
	render_set_font_colours(fs->lfc, fs->lbc, doc);
    }

    font_paint(buffer, font_OSCOORDS + (config_display_blending ? 0x800 : 0),
	       hpos + ti->width - (fstr.x / MILIPOINTS_PER_OSUNIT), bline);
#endif /* BUILDERS */
}

#if 0
void obullet_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc)
{
}

char *obullet_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, wimp_mousestr *m)
{
    return "";			/* Follow links, no fuss at all */
}
#endif

void obullet_astext(rid_text_item *ti, rid_header *rh, FILE *f)
{
    rid_text_item_bullet *tib = (rid_text_item_bullet*) ti;

    if ( tib->list_type == HTML_DL )
        return;

    if (tib->list_no == 0)
	fputs("      * ", f);
    else
    {
	char buffer[16];
	sprintf(buffer, "%6d) ", tib->list_no);
	buffer[4] = 0;
	fputs(buffer, f);
    }
}

void obullet_asdraw(rid_text_item *ti, antweb_doc *doc, int fh,
		  int x, int y, int *fileoff, wimp_box *bb)
{
#ifndef BUILDERS
    char *buffer;
    rid_text_item_bullet *tib = (rid_text_item_bullet*) ti;
    draw_textstrhdr txt;
    int len;
    draw_textstyle dts = {0};
    int size;
    wimp_box tb;
    font_string fstr;
    int hp;

    if ( tib->list_type == HTML_DL )
        return;

    buffer = obullet_string(tib, FALSE);

    for (len = strlen(buffer)+1; (len % 4) != 0 ; len++)
	buffer[len] = 0;

    font_setfont(webfonts[ti->st.wf_index].handle);

    fstr.x = 1 << 30;
    fstr.y = 1 << 30;
    fstr.split = -1;

    fstr.term = strlen(buffer);
    fstr.s = buffer;
    font_strwidth(&fstr);

    size = WEBFONT_SIZEOF(ti->st.wf_index);
    size = config_font_sizes[size];
    size *= 640;

    hp = x + ti->width - (fstr.x / MILIPOINTS_PER_OSUNIT);

    txt.tag = draw_OBJTEXT;
    txt.size = sizeof(txt) + len;
    txt.bbox.x0 = hp << 8;
    txt.bbox.y0 = (y - ti->max_down) << 8;
    txt.bbox.x1 = (x + ti->width) << 8;
    txt.bbox.y1 = (y + ti->max_up) << 8;
    txt.textcolour = (int) render_get_colour(render_link_colour(ti, doc), doc).word;
    txt.background = (int) render_get_colour(render_colour_BACK, doc).word;
    dts.fontref = ((ti->st.wf_index & WEBFONT_FLAG_MASK) >> WEBFONT_FLAG_SHIFT) + 1;
    txt.textstyle = dts;
    txt.fsizex = size;
    txt.fsizey = size;
    txt.coord.x = hp << 8;
    txt.coord.y = y << 8;

    tb.x0 = txt.bbox.x0 >> 8;
    tb.y0 = txt.bbox.y0 >> 8;
    tb.x1 = txt.bbox.x1 >> 8;
    tb.y1 = txt.bbox.y1 >> 8;

    df_stretch_bb(bb, &tb);

    df_write_data(fh, *fileoff, &txt, sizeof(txt));
    *fileoff += sizeof(txt);
    df_write_data(fh, *fileoff, buffer, len);
    *fileoff += len;
#endif /* BUILDERS */
}






