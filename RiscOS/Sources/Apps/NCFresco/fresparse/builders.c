/* builders.c - routines for building the basic structure */
/* (C) Copyright ANT Limited 1996. All rights reserved.   */

/* CHANGE LOG:
 * 02/07/96: SJM: fixed typo in add_strike. #define for default image align.
 * 04/07/96: SJM: added list types to push_bullet.
 * 10/07/96: SJM: fixed push_input to set xsize from integer and removed ysize
 * 11/07/96: SJM: fixed colour names (were wrong way around MSB-LSB)
 * 19/07/96: SJM: push_input,select,textarea all push a rid_form_element into one list
 * 23/07/96: SJM: added pcunit to image width
 * 24/07/96: SJM: made push_hr resistant to null VALUE's passed in.
 * 01/08/96: SJM: add 1 to MAXLENGTH for terminating null.
 * 02/08/96: SJM: changed bwidth to 0 if no anchor around it.
 * 11/09/96: SJM: new_area_item was checking for valid coords which obviously don't exist for SHAPE=DEFAULT
 * 27/02/97: SJM: Added new attributes to INPUT, TEXTAREA, SELECT for background colours and images
 * 29/04/97: SJM: Changed IMG ww and hh to being VALUEs.
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "myassert.h"
#include "memwatch.h"

#include "wimp.h"
#include "consts.h"

#include "rid.h"
#include "charsets.h"
#include "util.h"
#include "webfonts.h"
#include "url.h"
#include "indent.h"
#include "filetypes.h"
#include "parsers.h"
#include "images.h"
#include "gbf.h"
#include "tables.h"

#include "htmlparser.h"

#include "glue.h"
#include "sgmlexp.h"

/*****************************************************************************/

extern void select_pre_mode(HTMLCTX *me)
{
    PRSDBGN(("select_pre_mode(%p)\n", me));
    me->mode = HTMLMODE_PRE;
    me->sgmlctx->chopper = sgml_pre_word_chopper;
    sgml_reset_chopper_state(me->sgmlctx);
    string_free(&me->inhand_string);
}

extern void select_fmt_mode(HTMLCTX *me)
{
    PRSDBGN(("select_fmt_mode(%p)\n", me));
    me->mode = HTMLMODE_FMT;
    me->sgmlctx->chopper = sgml_fmt_word_chopper;
    sgml_reset_chopper_state(me->sgmlctx);
    string_free(&me->inhand_string);
}

/* When done htmlctx->inhand_string holds the string */

extern void select_str_mode(HTMLCTX *me)
{
    PRSDBGN(("select_str_mode(%p)\n", me));
    me->mode = HTMLMODE_STR;
    me->sgmlctx->chopper = sgml_str_word_chopper;
    sgml_reset_chopper_state(me->sgmlctx);
    string_free(&me->inhand_string);
}

/* Use for text documents */

extern void select_perm_mode(HTMLCTX *me)
{
/*    PRSDBG(("select_perm_mode(%p)\n", me));
    me->mode = HTMLMODE_PERM;
    me->sgmlctx->chopper = sgml_pre_word_chopper;
    sgml_reset_chopper_state(me->sgmlctx);
    string_free(&me->inhand_string);*/
}

extern void select_last_mode(HTMLCTX *me)
{
    switch (me->last_mode)
    {
    case HTMLMODE_FMT:
	select_fmt_mode(me);
	break;
    case HTMLMODE_STR:
	select_str_mode(me);
	break;
    case HTMLMODE_PRE:
	select_pre_mode(me);
	break;
    default:
    case HTMLMODE_BOGUS:
	PRSDBG(("select_last_mode(): HTMLMODE_BOGUS: can't nest HTMLCTX.last_mode\n"));
	select_fmt_mode(me);
	break;
    }

    me->last_mode = HTMLMODE_BOGUS;
}

/*****************************************************************************/

extern void bump_current_indent(SGMLCTX *context)
{
    BITS x = UNPACK( context->tos->effects_active, STYLE_INDENT);
    BITS rx = UNPACK( context->tos->effects_active, STYLE_RINDENT );

    if (x < (256 - INDENT_WIDTH - rx) )
    {
	x += INDENT_WIDTH;
	SET_EFFECTS(context->tos, STYLE_INDENT, x);
	/*PRSDBGN(("bump_current_indent(%p): now %d\n", context, x));*/
    }
    else
    {
	PRSDBG(("bump_current_indent(): at rightmost margin!\n"));
    }
}

extern void bump_current_rindent(SGMLCTX *context)
{
    BITS x = UNPACK( context->tos->effects_active, STYLE_INDENT);
    BITS rx = UNPACK( context->tos->effects_active, STYLE_RINDENT );

    if (rx < (256 - INDENT_WIDTH - x) )
    {
	rx += INDENT_WIDTH;
	SET_EFFECTS(context->tos, STYLE_RINDENT, rx);
	PRSDBGN(("bump_current_rindent(%p): now %d\n", context, rx));
    }
    else
    {
	PRSDBG(("bump_current_rindent(): at leftmost margin!\n"));
    }
}

/*****************************************************************************/

extern void add_fixed_to_font(SGMLCTX *context)
{
    if ((UNPACK(context->tos->effects_active, STYLE_WF_INDEX) & WEBFONT_FLAG_SPECIAL) == 0)
        set_font_type( context, WEBFONT_FLAG_FIXED );
    /*
	SET_EFFECTS_WF_FLAG(context->tos, WEBFONT_FLAG_FIXED);
	*/
    PRSDBGN(("add_fixed_to_font(%p: %08x)\n", context, context->tos->effects_active[0]));
}

extern void add_bold_to_font(SGMLCTX *context)
{
    if ((UNPACK(context->tos->effects_active, STYLE_WF_INDEX) & WEBFONT_FLAG_SPECIAL) == 0)
	SET_EFFECTS_WF_FLAG(context->tos, WEBFONT_FLAG_BOLD);
    PRSDBGN(("add_bold_to_font(%p)\n", context));
}

extern void add_italic_to_font(SGMLCTX *context)
{
    if ((UNPACK(context->tos->effects_active, STYLE_WF_INDEX) & WEBFONT_FLAG_SPECIAL) == 0)
	SET_EFFECTS_WF_FLAG(context->tos, WEBFONT_FLAG_ITALIC);
    PRSDBGN(("add_italic_to_font(%p)\n", context));
}

/*****************************************************************************/

extern void add_underline(SGMLCTX *context)
{
    SET_EFFECTS_FLAG(context->tos, STYLE_UNDERLINE);
    PRSDBGN(("add_underline(%p)\n", context));
}

extern void add_strike(SGMLCTX *context)
{
    SET_EFFECTS_FLAG(context->tos, STYLE_STRIKE);
    PRSDBGN(("add_strike(%p)\n", context));
}

/*****************************************************************************/

extern void set_font_size(SGMLCTX *context, int size)
{
    int x = UNPACK(context->tos->effects_active, STYLE_WF_INDEX);
    x &= ~WEBFONT_SIZE_MASK;
    x |= (size - 1) << WEBFONT_SIZE_SHIFT;
    SET_EFFECTS(context->tos, STYLE_WF_SIZE, x);
}

/*****************************************************************************/

/* This can set the SPECIAL bit, the HEADING bit, and the FIXED bit.
 */

extern void set_font_type(SGMLCTX *context, int type)
{
    int x = UNPACK(context->tos->effects_active, STYLE_WF_INDEX);

    if (type & WEBFONT_FLAG_SPECIAL)
	x &= WEBFONT_SIZE_MASK;
    else
	x &= WEBFONT_SIZE_MASK | WEBFONT_FLAG_BOLD | WEBFONT_FLAG_ITALIC;

    x |= type;

    SET_EFFECTS(context->tos, STYLE_WF_INDEX, x);
}

/*****************************************************************************/

/* Setting the LCR alignment is now potentially very common */

extern void std_lcr_align(SGMLCTX *context, VALUE *align)
{
    if (align->type == value_enum)
	switch (align->u.i)
	{
	case HTML_H1_ALIGN_LEFT:
	    SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_LEFT);
	    PRSDBGN(("std_lcr_align(%p,%p): LEFT alignment\n", context, align));
	    break;
	case HTML_H1_ALIGN_CENTER:
	case HTML_H1_ALIGN_CENTRE:
	    PRSDBGN(("std_lcr_align(%p,%p): CENTER alignment\n", context, align));
	    SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_CENTER);
	    break;
	case HTML_H1_ALIGN_RIGHT:
	    PRSDBGN(("std_lcr_align(%p,%p): RIGHT alignment\n", context, align));
	    SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_RIGHT);
	    break;
	case HTML_H1_ALIGN_JUSTIFY:
	    PRSDBGN(("std_lcr_align(%p,%p): JUSTIFY alignment\n", context, align));
	    SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_JUSTIFY);
	    break;
	}

}

/*****************************************************************************

  Colours are parsed into either an integer of the users value or an
  enumeration from the more recently defined colour set. This
  generates a single colour tuple for RISC OS to use.

*/

extern void htmlriscos_colour(VALUE *col, int *word)
{
    static int ct[16] =
    {
/* Aqua   */  0xFFFF0000,
/* Black  */  0x00000000,
/* Blue   */  0xFF000000,
/* Fuchsia*/  0xFF00FF00,
/* Gray   */  0x80808000,
/* Green  */  0x00800000,
/* Lime   */  0x00FF0000,
/* Maroon */  0x00008000,
/* Navy   */  0x80000000,
/* Olive  */  0x00808000,
/* Purple */  0x80008000,
/* Red    */  0x0000FF00,
/* Silver */  0xC0C0C000,
/* Teal   */  0x80800000,
/* White  */  0xFFFFFF00,
/* Yellow */  0x00FFFF00
    };

    switch (col->type)
    {
    case value_enum:
	/* -1 because enumerations are numbered from one */
	/* so that 0 can indicate not a valid enumeration item */
	ASSERT(col->u.i != 0);
	*word = ct[ col->u.i - 1 ];
	break;
    case value_tuple:
    {
	const int w1 = col->u.b;

	*word = ((w1 & 0xff0000) >> 8) |
	    ((w1 & 0xff00) << 8) |
	    ((w1 & 0xff) << 24);
    }
    break;

    default:
	*word = -1;		/* set to -1 if invalid colour specification */
	break;
    }
}

/*****************************************************************************

  Push the inhand string onto the item stream as a textual item. The string
  we operate on is htmlctx->inhand_string. Additional flags for the flags
  field may be supplied. Currently, text_item_push_word() does not set any
  flags unless word splitting for table alignment is happening - this needs
  thinking about. Zero or one spaces are automaticaly appended, according
  to the boolean space parameter. The string in htmlctx is freed after use.

  If the rid_flag_SOLO_SPACE option is provided, then an item with zero normal
  width but with a padding is entered.

  */

extern void text_item_push_word(HTMLCTX * me, rid_flag xf, BOOL space)
{
    rid_text_item_text *new;
    rid_text_item *nb;
/*    rid_text_item_text *new2;*/
    rid_text_item *nb2 = NULL;	/* new then maybe new2 */
/*    char ac;	*/		/* Align char */
/*    char *alignp = NULL;*/
/*    int i;*/
    char *ptr = me->inhand_string.ptr;
    int bytes = me->inhand_string.bytes;
    int rindent;

    ASSERT(me->magic == HTML_MAGIC);

    PRSDBGN(("text_item_push_word(%p,%x,%d:%s): '%.*s' (%d,%p)\n",
	    me, xf, space, space ? "WITH SPACE" : "WITHOUT SPACE", bytes, ptr ? ptr : "**NULL**",
	    bytes, ptr));

    if (me->no_break)
	xf |= rid_flag_NO_BREAK;

    if ( (xf & (rid_flag_LINE_BREAK | rid_flag_NO_BREAK)) == (rid_flag_LINE_BREAK | rid_flag_NO_BREAK) )
    {
	PRSDBG(("\ntext_item_push_word(): both LINE_BREAK and NO_BREAK - clearing NO_BREAK\n\n"));
	xf &= ~rid_flag_NO_BREAK;
    }

#if DEBUG
    flexmem_noshift();
    PRSDBG(("PUSH WORD '%.*s'\n", bytes, ptr));
    flexmem_shift();
#endif

    new = mm_calloc(1, sizeof(*new));
    nb = &(new->base);

#if 0
    /* FIXME: check this gets stacked all along - inheritence? */

    ac = UNPACK(me->sgmlctx->tos->effects_active, TABLE_ALIGN_CHAR);

    if ( ac != 0 )
    {
	PRSDBG(("text_item_push_word(%p,%x): looking for '%c', 0x%02X alignment character\n", me, flags, ac, ac));

	for (alignp = ptr, i = 0; i < bytes && alignp[i] != ac; i++)
	    ;

	if (i == bytes)
	{
	    alignp = NULL;
	}
	else
	{
	    PRSDBG(("text_item_push_word(): found at offset %d\n", alignp - ptr));
	}
    }

    /* This needs some thinking about, as well as the fiddly word building */
    /* What is the table alignment character has been set to a space? */
    /* then the treatment of flags also needs thinking about! */

    if (alignp != NULL)
    {
	/* Split into two words, so that align_char starts */
	/* the second word. The first word is always marked */
	/* as rid_flag_NO_BREAK to preserve the user's expected */
	/* formatting. The normal flags are applied to the */
	/* second word. */

	new2 = mm_calloc(1, sizeof(*new2));
	nb2 = &(new2->base);
	/* 123.567 */
	new->data_off = memzone_alloc(&me->rh->texts, alignp - ptr + 1);
	new2->data_off = memzone_alloc(&me->rh->texts, bytes - (alignp - ptr) + 1 + space);
	if (new->data_off == -1 || new2->data_off == -1 )
	{
	    usrtrc( "Memzone alloc failed for sting length %d ('%s')\n", bytes+1, ptr);
	}
	else
	{
	    int a, b;


	    flexmem_noshift();

	    memcpy(me->rh->texts.data + new->data_off, ptr, alignp - ptr);
	    (me->rh->texts.data + new->data_off)[alignp - ptr] = 0;

	    memcpy(me->rh->texts.data + new->data_off, alignp, bytes - (alignp - ptr) );
	    (me->rh->texts.data + new->data_off)[bytes] = " ";
	    (me->rh->texts.data + new->data_off)[bytes_space] = 0;

	    *alignp = 0;
	    strcpy(me->rh->texts.data + new->data_off, ptr);
	    *alignp = ac;
	    strcpy(me->rh->texts.data + new2->data_off, alignp);
	    flexmem_shift();
	}

	nb->flag |= rid_flag_NO_BREAK;
	nb2->flag |= flags;

	if (me->mode == HTMLMODE_PRE)
	    nb2->flag |= rid_flag_NO_BREAK;
    }
    else
#endif
    {
	new->data_off = memzone_alloc(&me->rh->texts, bytes+1+space);

	if (new->data_off == -1)
	{
	    usrtrc( "Memzone alloc failed for sting length %d ('%s')\n", bytes+1, ptr);
	}
	else
	{
	    flexmem_noshift();

	    memcpy(me->rh->texts.data + new->data_off, ptr, bytes);
	    (me->rh->texts.data + new->data_off)[bytes] = ' ';
	    (me->rh->texts.data + new->data_off)[bytes + space] = 0;

	    flexmem_shift();
	}

	nb->flag |= xf;
    }

    nb->tag = rid_tag_TEXT;
    nb->aref = me->aref;	/* Current anchor, or NULL */

    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;
    GET_ROSTYLE(nb->st);

    /* pdh: bodge warning: only one level of right indent supported */
    rindent = UNPACK(me->sgmlctx->tos->effects_active, STYLE_RINDENT);
    if ( rindent != 0 )
        nb->flag |= rid_flag_RINDENT;

    rid_text_item_connect(me->rh->curstream, nb);

    if (nb2 != NULL)
    {
	PRSDBG(("text_item_push_word(): adding 2nd half of split word\n"));
	nb2->tag = rid_tag_TEXT;
	nb2->aref = me->aref;	/* Current anchor, or NULL */

        /* pdh: bodge warning: only one level of right indent supported */
        rindent = UNPACK(me->sgmlctx->tos->effects_active, STYLE_RINDENT);
        if ( rindent != 0 )
            nb->flag |= rid_flag_RINDENT;

	GET_ROSTYLE(nb2->st);
	rid_text_item_connect(me->rh->curstream, nb2);
    }


    string_free(&me->inhand_string);

    return;
}

/*****************************************************************************/

extern void text_item_revoke_break( HTMLCTX *me )
{
    if ( me->rh && me->rh->curstream && me->rh->curstream->text_last )
        me->rh->curstream->text_last->flag &= ~rid_flag_LINE_BREAK;
}

/*****************************************************************************/

extern void new_form_item(HTMLCTX * me, VALUE *action, VALUE *method, VALUE *target, VALUE *id, VALUE *enctype)
{
    rid_form_item *new;

    PRSDBG(("new_form_item(%p,%p,%p,%p)\n", me, action, method, target));

    new = mm_calloc(1, sizeof(*new));

    if (action->type == value_string)
	new->action = stringdup(action->u.s);

    if (method->type == value_enum && method->u.i == HTML_FORM_METHOD_POST)
	new->method = rid_fm_POST;
    else
	new->method = rid_fm_GET; /* Default */

    if (target->type == value_string)
        new->target = stringdup(target->u.s);
    else
	new->target = strdup(me->basetarget);

    if (id->type == value_string)
	new->id = stringdup(id->u.s);

    if (enctype->type == value_string)
	new->enc_type = stringdup(enctype->u.s);

    rid_form_item_connect(me->rh, new);
    me->form = new;
}

/*****************************************************************************/

#define IMG_ALIGN_DEFAULT   HTML_IMG_ALIGN_BOTTOM /* HTML3.2/Netscape default */
/* #define IMG_ALIGN_DEFAULT   HTML_IMG_ALIGN_ABSMIDDLE old Fresco default */

extern void decode_img_align(int align, rid_image_flags *img_flags, rid_flag *item_flags)
{
    switch (align == -1 ? IMG_ALIGN_DEFAULT : align)
    {
    case HTML_IMG_ALIGN_TOP:
	*img_flags  |= rid_image_flag_ATOP;
	break;
    case HTML_IMG_ALIGN_TEXTTOP:
	*img_flags |= (rid_image_flag_ATOP | rid_image_flag_ABSALIGN);
	break;
    case HTML_IMG_ALIGN_BOTTOM:
    case HTML_IMG_ALIGN_BASELINE:
	*img_flags |= rid_image_flag_ABOT;
	break;
    case HTML_IMG_ALIGN_ABSBOTTOM:
	*img_flags |= (rid_image_flag_ABOT | rid_image_flag_ABSALIGN);
	break;
    case HTML_IMG_ALIGN_ABSMIDDLE:
	*img_flags |= rid_image_flag_ABSALIGN;
	break;

    case HTML_IMG_ALIGN_LEFT:
	/* Floating images align with the top of the text of their first line. */
	*img_flags  |= rid_image_flag_ATOP;
	*item_flags |= rid_flag_LEFTWARDS;
	break;
    case HTML_IMG_ALIGN_RIGHT:
	*img_flags  |= rid_image_flag_ATOP;
	*item_flags |= rid_flag_RIGHTWARDS;
	break;

    case HTML_IMG_ALIGN_MIDDLE:
    case HTML_IMG_ALIGN_CENTER:
    case HTML_IMG_ALIGN_CENTRE:
	break;
    }
}

extern void text_item_push_image(HTMLCTX * me,
				 int flags,
				 VALUE *src,
				 VALUE *alt,
				 VALUE *align,
				 VALUE *border,
				 VALUE *ww,
				 VALUE *hh,
				 VALUE *usemap,
				 VALUE *ismap,
				 VALUE *hspace,
				 VALUE *vspace)
{
    rid_text_item_image *new;
    rid_text_item *nb;

    PRSDBG(("text_item_push_image(%p,%x,etc): width=%p\n", me, flags, ww));

    new = mm_calloc(1, sizeof(*new));
    nb = &(new->base);

    if ( src->type == value_string )
	new->src = stringdup(src->u.s);
    if ( alt->type == value_string )
    {
	new->alt = stringdup(alt->u.s);
#if 0				/* SJM: removed 02/10/96 - this is handled before this point */
	if (new->alt)
	{
	    char *p;
	    for(p = new->alt; *p; p++)
		if (isspace(*p))
		    *p = ' ';
	}
#endif
    }
    if ( usemap->type == value_string )
	new->usemap = stringdup(usemap->u.s);
    if ( ismap->type == value_void )
	new->flags |= rid_image_flag_ISMAP;

#if 1
    new->ww = *ww;
    new->hh = *hh;
#else
    if ( ww->type == value_absunit )
	new->ww = (int)(ww->u.f/2);    /* convert back to pixels to avoid confusing users of this */
    else if ( ww->type == value_pcunit )
    {
        new->flags |= rid_image_flag_PERCENT;
	new->ww = (int)ww->u.f;
    }
    else
	new->ww = -1;

    if ( hh->type == value_integer )
	new->hh = hh->u.i;
    else
	new->hh = -1;
#endif
    if ( border->type == value_integer )
	new->bwidth = border->u.i;
    else
	new->bwidth = (me->aref && me->aref->href) || new->usemap ? 1 : 0;	/* Default is border if link else not */

    PRSDBGN(("Image %s, border=%d\n", new->src ? new->src : "", new->bwidth));

    if (hspace->type == value_integer)
        new->hspace = hspace->u.i;
    if (vspace->type == value_integer)
        new->vspace = vspace->u.i;

    decode_img_align(align->type == value_enum ? align->u.i : -1, &new->flags, &nb->flag);

    nb->tag = rid_tag_IMAGE;

    nb->flag |= rid_flag_LINE_BREAK & flags;
    if (me->mode == HTMLMODE_PRE || me->no_break)
	nb->flag |= rid_flag_NO_BREAK;
    nb->aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;

    GET_ROSTYLE(nb->st);

    rid_text_item_connect(me->rh->curstream, nb);

#if 0				/* FIXME: SJM: this new scheme doesn't work !!! */
    if (gbf_active(GBF_EARLYIMGFETCH))
    {
	rid_text_item_image *tii = (rid_text_item_image *) nb;

	IMGDBG(("text_item_push_image: src '%s' im %p bwidth %d, hspace %d, vspace %d\n",
		tii->src, tii->im, tii->bwidth, tii->hspace, tii->vspace));

#if 0 /*ndef BUILDERS*/
	if (tii->im == NULL)
	    tii->im = oimage_fetch_image(doc, tii->src, tii->ww.type == value_none || tii->hh.type == value_none);
#endif
    }
#endif
}

/*****************************************************************************/

extern rid_aref_item *new_aref_item(HTMLCTX* me,
			  VALUE *href,
			  VALUE *name,
			  VALUE *rel,
			  VALUE *target,
			  VALUE *title)
{
    rid_aref_item *new;

    PRSDBGN(("new_aref_item(%p, etc) %d %d %d %d\n", me,
	    href->type, name->type, rel->type, target->type));

    new = mm_calloc(1, sizeof(*new));

    /* 16/8/96: Borris: added string_strip_space */
    if (href->type == value_string)
	new->href = stringdup(string_strip_space(href->u.s));
    if (name->type == value_string)
	new->name = stringdup(name->u.s);
    if (rel->type == value_string)
	new->rel = stringdup(rel->u.s);
    if (target->type == value_string)
	new->target = stringdup(target->u.s);
    else
	new->target = strdup(me->basetarget);
    if (title->type == value_string)
	new->title = stringdup(title->u.s);

    /* If we are trying to nest (illegally) and there is no body then mark the previous item */
    if (me->aref && me->aref->first == 0)
    {
	me->aref->first = me->rh->curstream->text_last;
    }

    rid_aref_item_connect(me->rh, new);

    me->aref = new;

    /* if there is no href field then create a scaffold element to
     * hold the anchor and don't set the 'aref' field.
     */
    if (new->href == NULL)
    {
	text_item_push_word(me, rid_flag_NO_BREAK, FALSE);
	me->aref = NULL;
    }

    return new;
}

/*****************************************************************************/

extern void new_option_item(HTMLCTX * me, VALUE *value, rid_input_flags flags)
{
    rid_option_item *new;

    PRSDBG(("new_option_item(%p, etc)\n", me));

    ASSERT( ! (me->form == NULL || me->form->kids == NULL) );

    new = mm_calloc(1, sizeof(*new));

    new->flags = flags;
    new->value = valuestringdup(value);

    rid_option_item_connect(me->form->last_select, new);
}


/*****************************************************************************/


extern void new_map_item(HTMLCTX *me, VALUE *name)
{
    rid_map_item *map;

    PRSDBG(("new_map_item(%p, etc)\n", me));

    map = mm_calloc(sizeof(*map), 1);

    if (name->type == value_string)
	map->name = stringdup(name->u.s);

    /* link into the list of maps */
    map->next = me->rh->map_list;
    me->rh->map_list = map;

    /* record current map */
    me->map = map;
}

/*****************************************************************************/

static void parse_coords_list(VALUE *item, int *vals, int n)
{
    rid_stdunits *unit;

    if (item->type != value_stdunit_list)
	return;

    if (n > item->u.l.num)
	n = item->u.l.num;

    /* Convert the OS unit coords into pixels */
    for (unit = item->u.l.items; n > 0; n--, vals++, unit++)
	*vals = (int) ceil(unit->u.f)/2;
}

extern void new_area_item(HTMLCTX *me,
			  rid_map_item *map,
			  VALUE *shape,
			  VALUE *coords,
			  VALUE *href,
			  VALUE *target,
			  VALUE *alt)
{
    rid_area_item *area;

    if (map == NULL)		/* can't check for no coords here as default will fail */
        return;

    area = mm_calloc(sizeof(*area), 1);

    switch ( shape->type == value_enum ? shape->u.i : HTML_AREA_SHAPE_RECT )
    {
    case HTML_AREA_SHAPE_RECT:
    case HTML_AREA_SHAPE_RECTANGLE:
        area->type = rid_area_RECT;
        parse_coords_list(coords, &area->coords.rect.x0, 4);
	break;
    case HTML_AREA_SHAPE_CIRC:
    case HTML_AREA_SHAPE_CIRCLE:
        area->type = rid_area_CIRCLE;
        parse_coords_list(coords, &area->coords.circle.x, 3);
	break;
    case HTML_AREA_SHAPE_POLY:
    case HTML_AREA_SHAPE_POLYGON:
        area->type = rid_area_POLYGON;
	if (coords->type == value_stdunit_list)
	{
	    intxy *first, *last;

	    area->coords.polygon.npoints = coords->u.l.num/2;
	    area->coords.polygon.point = mm_calloc(sizeof(intxy), area->coords.polygon.npoints+1);

	    parse_coords_list(coords, &area->coords.polygon.point->x, 2*area->coords.polygon.npoints);

	    /* check path is closed and close it if neceesary (hence +1 on malloc above */
            first = &area->coords.polygon.point[0];
            last = &area->coords.polygon.point[area->coords.polygon.npoints-1];
            if (first->x != last->x || first->y != last->y)
            {
                last[1] = *first;
                area->coords.polygon.npoints++;
            }
        }
	break;
    case HTML_AREA_SHAPE_POINT:
        area->type = rid_area_POINT;
        parse_coords_list(coords, &area->coords.point.x, 2);
	break;
    case HTML_AREA_SHAPE_DEF:
    case HTML_AREA_SHAPE_DEFAULT:
        area->type = rid_area_DEFAULT;
	break;
    }

    if ( href->type == value_string )
        area->href = stringdup(href->u.s);
    if ( alt->type == value_string )
        area->alt = stringdup(alt->u.s);
    if ( target->type == value_string )
        area->target = stringdup(target->u.s);
    else
        area->target = strdup(me->basetarget);

    /* link into the list of areas */
    rid_area_item_connect(map, area);
}

/*****************************************************************************/

extern void text_item_push_select(HTMLCTX * me, VALUE *name, VALUE *size, VALUE *multiple, VALUE *id, VALUE *bgcolor, VALUE *selcolor, VALUE *nopopup)
{
    rid_text_item_select *new;
    rid_text_item *nb;
    rid_select_item *sel;

    PRSDBG(("text_item_push_select(%p, etc)\n", me));

    if (me->form == NULL)
	return;

    new = mm_calloc(1, sizeof(*new));
    nb = &(new->base);
    sel = mm_calloc(1, sizeof(*sel));
    new->select = sel;

    sel->base.display = nb;
    sel->base.tag = rid_form_element_SELECT;

    sel->base.id = valuestringdup(id);
    htmlriscos_colour(bgcolor, &sel->base.colours.back);
    htmlriscos_colour(selcolor, &sel->base.colours.select);
    if (nopopup->type != value_none)
	sel->flags |= rid_if_NOPOPUP;

    if (name->type == value_string)
	sel->name = stringdup(name->u.s);
    if (size->type != value_integer)
	sel->size = -1;
    else
	sel->size = size->u.i;

    if (multiple->type == value_void)
	sel->flags |= rid_sf_MULTIPLE;

    rid_form_element_connect(me->form, &sel->base);
    me->form->last_select = sel;

    if (me->no_break)
	nb->flag |= rid_flag_NO_BREAK;

    nb->tag = rid_tag_SELECT;
    nb->aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;
    GET_ROSTYLE(nb->st);

    rid_text_item_connect(me->rh->curstream, nb);
}

/*****************************************************************************/

extern void text_item_push_textarea(HTMLCTX * me, VALUE *name, VALUE *rows, VALUE *cols, VALUE *id, VALUE *bgcolor, VALUE *selcolor, VALUE *cursor, VALUE *tabindex, VALUE *wrap)
{
    rid_text_item_textarea *new;
    rid_text_item *nb;
    rid_textarea_item *ta;

    PRSDBG(("text_item_push_textarea(%p, etc)\n", me));

    if (me->form == NULL)
	return;

    new = mm_calloc(1, sizeof(*new));
    nb = &(new->base);
    ta = mm_calloc(1, sizeof(*ta));
    new->area = ta;
    ta->base.display = nb;
    ta->base.tag = rid_form_element_TEXTAREA;

    ta->base.id = valuestringdup(id);
    htmlriscos_colour(bgcolor, &ta->base.colours.back);
    htmlriscos_colour(selcolor, &ta->base.colours.select);
    htmlriscos_colour(cursor, &ta->base.colours.cursor);

    if (name->type == value_string)
	ta->name = stringdup(name->u.s);

    if (rows->type == value_integer)
	ta->rows = rows->u.i;
    if (cols->type == value_integer)
	ta->cols = cols->u.i;

    if (tabindex->type == value_integer)
	ta->base.tabindex = tabindex->u.i;

    /*
     * HARD is the only attribute that causes NS and IE to do hard wrapping.
     * WRAP with any other attribute causes NS to soft wrap.
     * IE always softwraps with or without WRAP.
     */

    if (wrap->type == value_none)
	ta->wrap = rid_ta_wrap_NONE;
    else if (wrap->type == value_enum && wrap->u.i == HTML_TEXTAREA_WRAP_HARD)
	ta->wrap = rid_ta_wrap_HARD;
    else
	ta->wrap = rid_ta_wrap_SOFT;

    if (ta->rows == 0)
	ta->rows = 1;

    if (ta->cols == 0)
	ta->cols = 20;

    rid_form_element_connect(me->form, &ta->base);
    me->form->last_text = ta;

    if (me->no_break)
	nb->flag |= rid_flag_NO_BREAK;

    nb->tag = rid_tag_TEXTAREA;
    nb->aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;
    GET_ROSTYLE(nb->st);

#if NEW_TEXTAREA
    memzone_init(&ta->default_text, MEMZONE_CHUNKS);
    memzone_init(&ta->text, MEMZONE_CHUNKS);
#endif
    rid_text_item_connect(me->rh->curstream, nb);
}

/*****************************************************************************/

#if 0				/* moved to forms.c */
extern void text_item_push_input(HTMLCTX * me, int flags,
				 VALUE *align,
				 VALUE *checked,
				 VALUE *disabled,
				 VALUE *maxlength,
				 VALUE *name,
				 VALUE *size,
				 VALUE *src,
				 VALUE *type,
				 VALUE *value,
				 VALUE *id,
				 VALUE *bgcolor,
				 VALUE *selcolor,
				 VALUE *cursor,
				 VALUE *nocursor,
				 VALUE *numbers,
				 VALUE *selimage)
{
    rid_text_item_input *new;
    rid_text_item *nb = NULL;
    rid_input_item *in;
    rid_input_tag tag = (rid_input_tag) -1;

    PRSDBG(("text_item_push_input(%p, %x, etc)\n", me, flags));

    switch (type->type == value_enum ? type->u.i : HTML_INPUT_TYPE_TEXT)
    {
    case HTML_INPUT_TYPE_TEXT:
	tag = rid_it_TEXT;
	break;
    case HTML_INPUT_TYPE_PASSWORD:
	tag = rid_it_PASSWD;
	break;
    case HTML_INPUT_TYPE_CHECKBOX:
	tag = rid_it_CHECK;
	break;
    case HTML_INPUT_TYPE_RADIO:
	tag = rid_it_RADIO;
	break;
    case HTML_INPUT_TYPE_IMAGE:
	tag = rid_it_IMAGE;
	break;
    case HTML_INPUT_TYPE_HIDDEN:
	tag = rid_it_HIDDEN;
	break;
    case HTML_INPUT_TYPE_SUBMIT:
	tag = rid_it_SUBMIT;
	break;
    case HTML_INPUT_TYPE_RESET:
	tag = rid_it_RESET;
	break;
    case HTML_INPUT_TYPE_BUTTON:
	tag = rid_it_BUTTON;
	break;
    }

    in = mm_calloc(1, sizeof(*in));

    if (tag != rid_it_HIDDEN)
    {
	new = mm_calloc(1, sizeof(*new));
	new->input = in;
	nb = &(new->base);
	in->base.display = nb;
    }
    else
	new = NULL;

    in->base.tag = rid_form_element_INPUT;
    in->tag = tag;

    in->base.id = valuestringdup(id);
    htmlriscos_colour(bgcolor, &in->base.colours.back);
    htmlriscos_colour(selcolor, &in->base.colours.select);
    htmlriscos_colour(cursor, &in->base.colours.cursor);

    if (nocursor->type != value_none)
	in->flags |= rid_if_NOCURSOR;
    if (numbers->type != value_none)
	in->flags |= rid_if_NUMBERS;

    if (checked->type != value_none)
	in->flags |= rid_if_CHECKED;
    if (disabled->type != value_none)
	in->flags |= rid_if_DISABLED;

    in->name = valuestringdup(name);
    in->value = valuestringdup(value);
    in->src = valuestringdup(src);
    in->src_sel = valuestringdup(selimage);


#if 0
    in->xsize = in->ysize = -1;
    /* NOTE: If SHORTISH is defined as short, rather than int, then */
    /* this scanf needs to be %hd if stray memory is not to be written! */

    if (size->type == value_string)
    {
	sscanf(size->u.s.ptr, "%hd,%hd", &in->xsize, &in->ysize);
    }
    if (in->xsize == -1)
	in->xsize = 20;
#else
    in->xsize = size->type == value_integer ? size->u.i : -1;
#endif

    if (maxlength->type == value_integer)
	in->max_len = maxlength->u.i;

    if (in->max_len == 0)
	in->max_len = 256;

    if (me->form)
	rid_form_element_connect(me->form, &in->base);

    switch (tag)
    {
    case rid_it_CHECK:
    case rid_it_RADIO:
	in->data.tick = ((in->flags & rid_if_CHECKED) != 0);
	break;
    case rid_it_TEXT:
    case rid_it_PASSWD:
	in->data.str = mm_malloc(in->max_len + 1); /* SJM: Add 1 for the terminating null, was added to max_len originally */
	if (in->value)
	{
	    translate_escaped_text(in->value, in->data.str, in->max_len + 1); /* add one here as len is len of output buffer */
	}
	else
	{
	    in->data.str[0] = 0;
	}
	break;
    case rid_it_IMAGE:
	decode_img_align(align->type == value_enum ? align->u.i : -1, &in->data.image.flags, &nb->flag);
	break;
    }

    if (nb)
    {
	nb->tag = rid_tag_INPUT;
	if (flags & rid_flag_LINE_BREAK)
	    nb->flag |= rid_flag_LINE_BREAK;
	if (me->mode == HTMLMODE_PRE || me->no_break) /* We need to be able to have both flags set */
	    nb->flag |= rid_flag_NO_BREAK;
	nb->aref = me->aref;	/* Current anchor, or NULL */
	if (me->aref && me->aref->first == NULL)
	    me->aref->first = nb;
	GET_ROSTYLE(nb->st);

	rid_text_item_connect(me->rh->curstream, nb);
    }
}
#endif

/*****************************************************************************

  Note that we must take a copy of any attributes before recursing back
  with pseudo_html, etc, as act of recursing will free our values to make
  room for newer values. In particular, there is no guarentee that the
  VALUEs are still present when the printf() type format is processed.

 */

extern void push_fake_search_form(HTMLCTX * me, VALUE *prompt)
{
    static char *maybe = "This document is an index.  Enter keywords here: ";
    rid_form_item *oldform;
    char *p = prompt->type == value_string ? stringdup(prompt->u.s) : maybe;

    oldform = me->form;
    pseudo_html(me, "<FORM METHOD=GET>%s\n<INPUT TYPE=TEXT>\n"
		"<INPUT TYPE=SUBMIT VALUE='Search'>\n</FORM>",
		p);

    if (p != maybe)
	mm_free(p);
#if 0
    new_form_item(me, &action, &method, &target);

    if (prompt->type == value_string)
	HTRISCOS_put_string(me, prompt);
    else
	HTRISCOS_put_string(me, "This document is an index.  Enter keywords here: ");

    text_item_push_word(me, 0);

    present[HTML_INPUT_TYPE] = TRUE;
    value[HTML_INPUT_TYPE] = "TEXT";

    text_item_push_input(me, 0, (const BOOL *) present, (const char **) value);

    value[HTML_INPUT_TYPE] = "SUBMIT";
    present[HTML_INPUT_VALUE] = 1;
    value[HTML_INPUT_VALUE] = "Search";

    text_item_push_input(me, 0, (const BOOL *) present, (const char **) value);
#endif
    me->form = oldform;
}

/*****************************************************************************

  Push a text item with zero width. It does not matter if the previous item
  has rid_flag_NO_BREAK set

  SJM: Currently this is only called from the <BR> code. We don't want it to
  push a break if the previous word has line break set.

  */

extern void text_item_push_break(HTMLCTX * me)
{
    rid_text_item *nb;
    BOOL add_break;

    /* SJM: added the line break check */
    add_break = TRUE;
    if ( (nb = me->rh->curstream->text_last) != NULL) /* SJM: 19Jun97 stream to curstream */
    {
	/* if previous has a line break and isn't an explicitly pushed break
	   then don't push break item but mark that we had it */
	if ((nb->flag & (rid_flag_LINE_BREAK | rid_flag_EXPLICIT_BREAK)) == rid_flag_LINE_BREAK)
	{
	    nb->flag |= rid_flag_EXPLICIT_BREAK;
	    add_break = FALSE;
	}
    }

    if (add_break)
    {
	rid_text_item_text *ti = mm_calloc(1, sizeof(*ti));

	PRSDBG(("line break\n"));

	nb = (rid_text_item *) ti;

	nb->tag = rid_tag_TEXT;
	nb->flag |= rid_flag_LINE_BREAK | rid_flag_EXPLICIT_BREAK;
	nb->aref = me->aref;	/* Current anchor, or NULL */
	if (me->aref && me->aref->first == NULL)
	    me->aref->first = nb;
	GET_ROSTYLE(nb->st);

	/* Zero sized text item */
	ti->data_off = memzone_alloc(&me->rh->texts, 1);
	flexmem_noshift();
	me->rh->texts.data[ti->data_off] = 0;
	flexmem_shift();

	rid_text_item_connect(me->rh->curstream, nb);

    }
    else
    {
	PRSDBG(("line break ignored\n"));
    }
}

/*****************************************************************************

  This is used for block level elements, which never generate more than one
  PBREAK item. There is no variant available to fill in a line, <HR> style.

  */

extern void text_item_ensure_break(HTMLCTX * me)
{
    rid_text_item *nb;

    PRSDBG(("Ensure Paragraph break.\n"));

    if ( (nb = me->rh->curstream->text_last) != NULL &&	/* SJM: 19Jun07 stream to curstream. Fixed problem with delayed tables */
	 nb->tag != rid_tag_PBREAK &&
	 nb->tag != rid_tag_BULLET) /* SJM: added bullet check */
    {
        nb = mm_calloc(1, sizeof(*nb));

	nb->tag = rid_tag_PBREAK;
	nb->flag |= rid_flag_LINE_BREAK;
	nb->aref = me->aref;	/* Current anchor, or NULL */
	if (me->aref && me->aref->first == NULL)
	    me->aref->first = nb;
	GET_ROSTYLE(nb->st);

	rid_text_item_connect(me->rh->curstream, nb);
    }
    else
    {
	PRSDBG(("No need\n"));
    }
}

extern void text_item_push_hr(HTMLCTX *me, VALUE *align, VALUE *noshade, VALUE *size, VALUE *width)
{
    rid_text_item_hr *item;
    rid_text_item *ti;

    /* This implies LINE_BREAK overrides NO_BREAK. Don't forget this
       when altering the formatter! */
    if ( (ti = me->rh->curstream->text_last) != NULL )
	ti->flag |= rid_flag_LINE_BREAK;

    item = mm_calloc(1, sizeof(*item));
    item->base.tag = rid_tag_HLINE;
    item->base.flag |= rid_flag_LINE_BREAK;
    item->base.aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
        me->aref->first = &item->base;

    item->align = align->type == value_enum ? align->u.i : HTML_HR_ALIGN_CENTRE;

    if (noshade->type != value_none)
        item->noshade = 1;

    item->size = size->type == value_integer && size->u.i > 1 ? size->u.i*2 : 4;

    if (width->type == value_absunit || width->type == value_pcunit)
    {
        item->width = *width;
    }
    else
    {
        item->width.type = value_pcunit;
        item->width.u.f = 100;
    }

    GET_ROSTYLE(item->base.st);

    rid_text_item_connect(me->rh->curstream, &item->base);
}

/*****************************************************************************/

extern void text_item_push_bullet(HTMLCTX * me, int item_type)
{
    rid_text_item_bullet *nbb;
    rid_text_item *nb;

    PRSDBGN(("text_item_push_bullet(%p) %d/%d/%d\n", me,
        me->sgmlctx->tos->outer->element,
        item_type,
        UNPACK(me->sgmlctx->tos->outer->effects_active, LIST_ITEM_NUM)));

    nbb = mm_calloc(1, sizeof(*nbb));
    nb = &(nbb->base);

    nb->tag = rid_tag_BULLET;
    nb->aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;
    GET_ROSTYLE(nb->st);
#if 0
    PRSDBG(("list item number before push %x\n",
	    UNPACK(me->sgmlctx->tos->outer->effects_active, LIST_ITEM_NUM)));
#endif

    nbb->list_no = UNPACK(me->sgmlctx->tos->outer->effects_active, LIST_ITEM_NUM);
    nbb->list_type = me->sgmlctx->tos->outer->element;
    nbb->item_type = item_type;

#if 0
    SET_EFFECTS(me->sgmlctx->tos->outer, LIST_ITEM_NUM, nbb->list_no + 1);

    PRSDBG(("list item number before push %x\n",
	    UNPACK(me->sgmlctx->tos->outer->effects_active, LIST_ITEM_NUM)));
#endif
    rid_text_item_connect(me->rh->curstream, nb);
}

/*****************************************************************************

  All recursion is encouraged to come via this point, so that only a single
  function needs to know how to support recursion.

 */

extern void pseudo_html(HTMLCTX *ctx, const char *fmt, ...)
{
    SGMLCTX *context = ctx->sgmlctx;
    char *buffer;
    va_list arglist;

    PRSDBG(("pseudo_html(%s)\n", fmt));

    buffer = mm_malloc(MAXSTRING + 32);	/* SJM: changed to malloc from auto */

    va_start(arglist, fmt);
    vsprintf(buffer, fmt, arglist);

    PRSDBG(("pseudo_html(): %s\n", buffer));

    ASSERT(strlen(buffer) < (MAXSTRING+32));

    sgml_recursion_warning_pre(context);

    context->state = get_state_proc(context);
    clear_inhand(context);

    sgml_feed_characters(context, buffer, strlen(buffer));

    sgml_recursion_warning_post(context);
    va_end(arglist);

    mm_free(buffer);

    PRSDBG(("pseudo_html(): recursion finished\n"));
}

/*****************************************************************************/

/* eof builders.c */
