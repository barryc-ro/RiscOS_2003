/* lists.c - <OL> <UL> <DL> <LI> <DT> <DD> <MENU> <DIR> */

/* CHANGE LOG:
 * 02/07/96: SJM: added OL START
 * 04/07/96: SJM: changed DT for STBWEB, added bullet type code
 * 05/07/96: SJM: added TYPE=PLAIN
 */

#include "htmlparser.h"
#include "webfonts.h"
#include "indent.h"

/*****************************************************************************/

/* ULTYPE is defined as parse enum string
 *   when used with UL only enum's should be used
 *   when used with LI of UL then only enum's should be used
 *   when used with LI of OL then only string should be used
 * OLTYPE is parse string
 */

static char *ol_type_match = rid_bullet_ol_STRING;

/* returns 1-5, 0 means invalid */

static int decode_ol_type(VALUE *attr)
{
    if (attr->type == value_string)
    {
        STRING s = string_strip_space(attr->u.s);
        if (s.nchars == 1)
        {
            char *ss = memchr(ol_type_match, s.ptr[0], strlen(ol_type_match));
            if (ss)
                return ss - ol_type_match + 1;
        }
    }
    return 0;
}

/*****************************************************************************/

extern void startol (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    VALUE *attr;
    int type, start;

    generic_start (context, element, attributes);

    bump_current_indent(context);

    attr = &attributes->value[HTML_OL_START];
    if (attr->type == value_integer)
        start = attr->u.i - 1;
    else
        start = 0;

    type = decode_ol_type(&attributes->value[HTML_OL_TYPE]);
    if (type == 0)
        type = rid_bullet_ol_1;

    SET_EFFECTS(context->tos, LIST_ITEM_TYPE, type);
    SET_EFFECTS(context->tos, LIST_ITEM_NUM, start);
}

extern void startul (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    VALUE *attr;
    int type;

    generic_start (context, element, attributes);

    bump_current_indent(context);

    if (attributes->value[HTML_UL_PLAIN].type == value_void)
    {
        type = HTML_UL_TYPE_PLAIN;
    }
    else
    {
        attr = &attributes->value[HTML_UL_TYPE];
        if (attr->type == value_enum)
            type = attr->u.i;             /* from 1 */
        else
            type = HTML_UL_TYPE_DISC;
    }

    SET_EFFECTS(context->tos, LIST_ITEM_TYPE, type);
    SET_EFFECTS(context->tos, LIST_ITEM_NUM, 0);
}

extern void startmenu (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    bump_current_indent(context);

    SET_EFFECTS(context->tos, LIST_ITEM_NUM, 0);
}

extern void startdir (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    startmenu(context, element, attributes);
}

extern void startdl (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    bump_current_indent(context);

    if ( attributes->value[HTML_DL_COMPACT].type == value_void )
    {
        PRSDBG(("This DL is compact\n"));
        SET_EFFECTS( context->tos, LIST_ITEM_TYPE, TRUE );
    }
    else
    {
        PRSDBG(("This DL isn't compact\n" ));
        SET_EFFECTS( context->tos, LIST_ITEM_TYPE, FALSE );
    }
}

/*****************************************************************************

  Note that the list item number is stored one stack level outwards so that
  it is retained when an individual <LI> is closed. Otherwise, everything
  would see the same item number!.

  */

extern void startli (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    BITS x;
    int item_type;

    generic_start (context, element, attributes);

    item_type = UNPACK(context->tos->outer->effects_active, LIST_ITEM_TYPE);

    switch (context->tos->outer->element)
    {
    case HTML_OL:
	ASSERT(context->tos->outer != NULL);

        /* check for value from the element */
        if (attributes->value[HTML_LI_VALUE].type == value_integer)
            x = attributes->value[HTML_LI_VALUE].u.i;
        else
        {
	    x = 1 + UNPACK(context->tos->outer->effects_active, LIST_ITEM_NUM);
	    PRSDBG(("Unpacked current list item number as %d\n", x-1));
	}

	SET_EFFECTS(context->tos->outer, LIST_ITEM_NUM, x);

        /* check for type from the element */
        {
            int type = decode_ol_type(&attributes->value[HTML_LI_TYPE]);
            if (type != 0)
                item_type = type;
        }

	PRSDBG(("Packed item back as %d\n", x));
	break;

    case HTML_UL:
    {
        /* check for type from the element */
        VALUE *attr = &attributes->value[HTML_LI_TYPE];
        if (attr->type == value_enum)
            item_type = attr->u.i;
	break;
    }

    case HTML_MENU:
	break;

    case HTML_DIR:
	break;

    default:
	break;
    }

#if 0
    /*bump_current_indent(context);*/
    text_item_push_bullet(me, item_type);
#else
    {
	int oldindent = UNPACK(context->tos->effects_active, STYLE_INDENT);

	if (oldindent >= INDENT_WIDTH)
	{
	    SET_EFFECTS(context->tos, STYLE_INDENT, oldindent - INDENT_WIDTH);
	}
	text_item_push_bullet(me, item_type);
	SET_EFFECTS(context->tos, STYLE_INDENT, oldindent);
    }
#endif

}

extern void startdd (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    if (TRUE || context->tos->element == HTML_DL)
    {
/*	BITS x;
	x = UNPACK(context->tos->outer->effects_active, STYLE_INDENT);
	SET_EFFECTS(context->tos, STYLE_INDENT, x + INDENT_WIDTH);
	SET_EFFECTS(context->tos, STYLE_WF_INDEX, WEBFONT_DD);*/
#if 0 /* ndef STBWEB */
	bump_current_indent(context);
#endif
	PRSDBG(("startdD(): set indent to %d\n", UNPACK(context->tos->effects_active, STYLE_INDENT)));
    }

    /* pdh: This kinda assumes we're inside a DL, but fortunately if we
     * aren't, LIST_ITEM_TYPE is going to be zero
     */
    if ( UNPACK( context->tos->outer->effects_active, LIST_ITEM_TYPE ) )
    {
        HTMLCTX *me = htmlctxof( context );

        text_item_revoke_break( me );
        /* item_type here doesn't matter as any "bullet" in a DL must
         * be our magic one
         */
        PRSDBG(("Pushing fake DL bullet\n"));
        text_item_push_bullet( me, 0 );
    }
}

extern void startdt (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    if (TRUE || context->tos->element == HTML_DL)
    {
	BITS x;
	x = UNPACK(context->tos->outer->effects_active, STYLE_INDENT);
#if 1 /* def STBWEB */
	if (x >= INDENT_WIDTH)
	{
	    x -= INDENT_WIDTH;
	    SET_EFFECTS(context->tos, STYLE_INDENT, x);
	}
/*      add_bold_to_font(context); */
#else
	SET_EFFECTS(context->tos, STYLE_INDENT, x + 2);
	SET_EFFECTS(context->tos, STYLE_WF_INDEX, WEBFONT_DT);
#endif

	PRSDBG(("startdt(): set indent to %d\n", UNPACK(context->tos->effects_active, STYLE_INDENT)));
    }
}


/*****************************************************************************/


/* eof lists.c */
