/* fonts.c - <TT> <I> <B> <U> <STRIKE> <BIG> <SMALL> <SUB> <SUP> */

/* CHANGE LOG:
 * 02/07/96: SJM: startu, starts now use add_ fns in builders.c
 * 04/07/96: SJM: added size-1 to SUB and SUP
 */

#include "htmlparser.h"
#include "webfonts.h"

/*****************************************************************************/

extern void startb (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    add_bold_to_font(context);
}

extern void finishb (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}

extern void startbasefont (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    VALUE *attr;

    generic_start (context, element, attributes);

    attr = &attributes->value[HTML_BASEFONT_SIZE];
    if (attr->type == value_integer)
    {
        me->rh->bgt |= rid_bgt_BASEFONT;
        me->rh->basefont = attr->u.i < 1 ? 1 : attr->u.i > 7 ? 7 : attr->u.i;
    }
}

#if 0
extern void finishbasefont (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

/*****************************************************************************/


extern void starttt (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    add_fixed_to_font(context);
}


extern void starti (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    add_italic_to_font(context);
}

extern void startu (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    add_underline(context);
}

extern void starts(SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    add_strike(context);
}

extern void startstrike(SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starts(context, element, attributes);
}

static void adjust_font(SGMLCTX * context, int by)
{
    BITS x;
    int size;

    x = UNPACK(context->tos->effects_active, STYLE_WF_INDEX);
    size = (x & WEBFONT_SIZE_MASK) >> WEBFONT_SIZE_SHIFT;
    size += by;
    if (size >= 0 && size <= 6)
    {
        x = (x &~ WEBFONT_SIZE_MASK) | (size << WEBFONT_SIZE_SHIFT);
	SET_EFFECTS(context->tos, STYLE_WF_SIZE, x);
    }
}

extern void startsub (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    adjust_font(context, -1);
    SET_EFFECTS_FLAG (context->tos, STYLE_SUB);
}

extern void startsup (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    adjust_font(context, -1);
    SET_EFFECTS_FLAG (context->tos, STYLE_SUP);
}

extern void startbig (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    adjust_font(context, +1);
}

extern void startsmall (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    adjust_font(context, -1);
}


/* eof fonts.c */

