/* block.c - block level elements */

/* <H1> <H2> <H3> <H4> <H5> <H6> <H7>
   <P> <UL> <OL> <DL> <PRE> <DIV> <CENTER>
   <BLOCKQUOTE> <FORM> <<<ISINDEX>>> <HR> <TABLE>

   NOTE: <UL> <OL> <DL> are in lists.c
   NOTE: <FORM> is in forms.c
   NOTE: <TABLE> is in tables.c
   NOTE: <ISINDEX> is in head.c

*/

#include "htmlparser.h"
#include "webfonts.h"
#include "rid.h"
#include "indent.h"

extern void starthr (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    text_item_push_hr(htmlctxof(context),
        &attributes->value[HTML_HR_ALIGN],
        &attributes->value[HTML_HR_NOSHADE],
        &attributes->value[HTML_HR_SIZE],
        &attributes->value[HTML_HR_WIDTH]);
}


extern void startblockquote (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

#ifndef STBWEB
    SET_EFFECTS(context->tos, STYLE_WF_INDEX, WEBFONT_BLOCK);
#endif

#ifdef STBWEB
#if NEW_BREAKS
    rid_scaff_item_push(htmlctxof(context)->rh->curstream, rid_break_MUST);
#else
    rid_scaff_item_push(htmlctxof(context)->rh->curstream, rid_flag_LINE_BREAK);
#endif
/*  text_item_push_word(htmlctxof(context), rid_flag_LINE_BREAK, FALSE); */
#endif
    bump_current_indent(context);
    bump_current_rindent(context);
}

extern void finishblockquote (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}


/*****************************************************************************/

/* Purely so can do large scale alignment */

extern void startdiv (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    set_lang(context, &attributes->value[HTML_DIV_LANG]);

    std_lcr_align(context, &attributes->value[HTML_DIV_ALIGN]);
}

/*****************************************************************************/

extern void startcenter (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
    SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_CENTER);
}

extern void startcentre (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
    { startcenter(context, element, attributes); }

/*****************************************************************************/

/* THESE ALTER THE WORD CHOPPER BEHAVIOUR */

#define startxmp startpre

extern void startpre (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    set_lang(context, &attributes->value[HTML_PRE_LANG]);

#ifdef STBWEB
    add_fixed_to_font(context);
#else
    SET_EFFECTS(context->tos, STYLE_WF_INDEX, WEBFONT_PRE);
#endif

    /* These need active action to cancel */
    select_pre_mode( htmlctxof(context) );
}

#define finishxmp finishpre

extern void finishpre (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *htmlctx = (HTMLCTX *)context->clictx;

    generic_finish (context, element);

    /* Assume a stack is not necessary here ! */
    select_fmt_mode(htmlctx);
}

/*****************************************************************************/

/* HEADINGS */

static void starthN (SGMLCTX * context, ELEMENT * element, VALUES * attributes, BITS N)
{
    generic_start (context, element, attributes);

    SET_EFFECTS(context->tos, STYLE_WF_INDEX, N);
    std_lcr_align(context, &attributes->value[HTML_H1_ALIGN]);
}


extern void starth1 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H1);
}

extern void starth2 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H2);
}

extern void starth3 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H3);
}

extern void starth4 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H4);
}

extern void starth5 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H5);
}

extern void starth6 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H6);
}

extern void starth7 (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    starthN(context, element, attributes, WEBFONT_H7);
}

static void finishhN (SGMLCTX * context, ELEMENT * element)
{
    generic_finish (context, element);

}

extern void finishh1 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

extern void finishh2 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

extern void finishh3 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

extern void finishh4 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

extern void finishh5 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

extern void finishh6 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

extern void finishh7 (SGMLCTX * context, ELEMENT * element)
{
   finishhN(context, element);
}

/* eof block.c */
