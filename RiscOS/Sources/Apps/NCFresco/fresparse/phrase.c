/* phrase.c - <EM> <STRONG> <DFN> <CODE> <SAMP> <KBD> <VAR> <CITE> */

/* CHANGE LOG:
 * 02/07/96: SJM: restored EM to being italic rather than bold
 * 19/07/96: SJM: used wrong #define in cite
 */

#include "htmlparser.h"
#include "webfonts.h"

extern void startem (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    add_italic_to_font(context);
}


extern void startstrong (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    startb(context, element, attributes);
}

/* Dunno? */
extern void startdfn (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    add_bold_to_font(context);
}

extern void startcode (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    add_fixed_to_font(context);
}

extern void startsamp (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    startcode(context, element, attributes);
}


extern void startkbd (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    startcode(context, element, attributes);
}

extern void startvar (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    add_italic_to_font(context);
}

extern void startcite (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

#if 1 /* pdh: was ifdef STBWEB */
    add_italic_to_font(context);
#else
    SET_EFFECTS(context->tos, STYLE_WF_INDEX, WEBFONT_CITE);
#endif
}

/* eof phrase.c */
