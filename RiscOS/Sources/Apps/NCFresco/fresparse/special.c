/* <HTML> <HEAD> <BODY> <A> <IMG> <APPLET> <FONT> <BR> <MAP> */

#include "util.h"
#include "memwatch.h"
#include "htmlparser.h"
#include "webfonts.h"

/*****************************************************************************

  As soon as <HTML> has been seen, we must prevent automatic insertion
  of <HTML> when unexpected characters are received, as this would be
  incorrect.

  */

extern void starthtml (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    htmlctxof(context)->done_auto_body = TRUE;

    TASSERT(htmlctxof(context)->aref == NULL);
}

extern void starthead (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);
}

extern void startbody (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    if (attributes->value[HTML_BODY_BACKGROUND].type == value_string)
    {
	me->rh->bgt |= rid_bgt_IMAGE;
	if (me->rh->tile.src)
	    mm_free(me->rh->tile.src);
	me->rh->tile.src = stringdup(attributes->value[HTML_BODY_BACKGROUND].u.s);
    }
    if (attributes->value[HTML_BODY_BGCOLOR].type != value_none)
    {
	me->rh->bgt |= rid_bgt_COLOURS;
	htmlriscos_colour( &attributes->value[HTML_BODY_BGCOLOR], &me->rh->colours.back);
    }
    if (attributes->value[HTML_BODY_TEXT].type != value_none)
    {
	me->rh->bgt |= rid_bgt_FCOL;
	htmlriscos_colour( &attributes->value[HTML_BODY_TEXT], &me->rh->colours.fore);
    }
    if (attributes->value[HTML_BODY_LINK].type != value_none)
    {
	me->rh->bgt |= rid_bgt_LCOL;
	htmlriscos_colour( &attributes->value[HTML_BODY_LINK], &me->rh->colours.link);
    }
    if (attributes->value[HTML_BODY_VLINK].type != value_none)
    {
	me->rh->bgt |= rid_bgt_VCOL;
	htmlriscos_colour( &attributes->value[HTML_BODY_VLINK], &me->rh->colours.vlink);
    }
    if (attributes->value[HTML_BODY_ALINK].type != value_none)
    {
	me->rh->bgt |= rid_bgt_ACOL;
	htmlriscos_colour( &attributes->value[HTML_BODY_ALINK], &me->rh->colours.alink);
    }

    if (attributes->value[HTML_BODY_LEFTMARGIN].type == value_integer)
	me->rh->margin.left = attributes->value[HTML_BODY_LEFTMARGIN].u.i;
    if (attributes->value[HTML_BODY_TOPMARGIN].type == value_integer)
	me->rh->margin.top = attributes->value[HTML_BODY_TOPMARGIN].u.i;

    select_fmt_mode(htmlctxof(context));
}

/*****************************************************************************

  If <A> is used within <OBJECT>, special behaviour is required

  */

extern void starta (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_start (context, element, attributes);

    TASSERT(htmlctx->magic == HTML_MAGIC);

    if (htmlctx->object_nesting == 0)
    {
	new_aref_item(htmlctx,
		      &attributes->value[HTML_A_HREF],
		      &attributes->value[HTML_A_NAME],
		      &attributes->value[HTML_A_REL],
		      &attributes->value[HTML_A_TARGET],
		      &attributes->value[HTML_A_TITLE]
	    );
    }
    else
    {
	PRSDBG(("starta(): object nesting active\n"));

	new_area_item(htmlctx, htmlctx->object->map,
		      &attributes->value[HTML_A_SHAPE],
		      &attributes->value[HTML_A_COORDS],
		      &attributes->value[HTML_A_HREF],
		      &attributes->value[HTML_A_TARGET],
		      &attributes->value[HTML_A_TITLE]);
    }
}

extern void startimg (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);
#if 0
    vp = &attributes->value[HTML_IMG_WIDTH];
    printf("HTML_IMG_WIDTH %p \n", vp);
#endif
#if 1
    PRSDBGN(("startimg(): %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n",
			 &attributes->value[HTML_IMG_SRC],
			 &attributes->value[HTML_IMG_ALT],
			 &attributes->value[HTML_IMG_ALIGN],
			 &attributes->value[HTML_IMG_BORDER],
			 &attributes->value[HTML_IMG_WIDTH],
			 &attributes->value[HTML_IMG_HEIGHT],
			 &attributes->value[HTML_IMG_USEMAP],
			 &attributes->value[HTML_IMG_ISMAP],
			 &attributes->value[HTML_IMG_HSPACE],
			 &attributes->value[HTML_IMG_VSPACE]));
#endif

    text_item_push_image(me, 0,
			 &attributes->value[HTML_IMG_SRC],
			 &attributes->value[HTML_IMG_ALT],
			 &attributes->value[HTML_IMG_ALIGN],
			 &attributes->value[HTML_IMG_BORDER],
			 &attributes->value[HTML_IMG_WIDTH],
			 &attributes->value[HTML_IMG_HEIGHT],
			 &attributes->value[HTML_IMG_USEMAP],
			 &attributes->value[HTML_IMG_ISMAP],
			 &attributes->value[HTML_IMG_HSPACE],
			 &attributes->value[HTML_IMG_VSPACE]);
}

#if 0
/* Moved to objparse.c */
extern void startapplet (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
/*    HTMLCTX *me = htmlctxof(context); */

    generic_start (context, element, attributes);

}

extern void finishapplet (SGMLCTX * context, ELEMENT * element)
{
    generic_finish(context, element);
}
#endif

/* rh_find_colour
 *
 * Find a place in the rid_header to store the colour
 */

static int rh_find_colour( rid_header *rh, int colour )
{
    int i;

    if ( rh->extracolours >= rid_EXTRACOLOURS )
        return rid_EXTRACOLOURS-1;      /* abject failure */

    /* not 0 as that means "use the normal colour" */
    for ( i=1; i < rh->extracolours; i++ )
        if ( rh->extracolourarray[i] == colour )
            return i;

    if ( !rh->extracolours )
        rh->extracolours = 1;

    rh->extracolourarray[ rh->extracolours ] = colour;
    rh->extracolours++;

    return rh->extracolours-1;
}

extern void startfont (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    if (HAVEATTR(HTML_FONT_SIZE, value_string))
    {
	int size = (me->rh->bgt & rid_bgt_BASEFONT) ? me->rh->basefont : 3; /* Default font size is 3 */
	unsigned int x;

	char *cp = attributes->value[HTML_FONT_SIZE].u.s.ptr;
	if (cp[0] == '+')
	{
	    size += atoi(cp+1);
	}
	else if (cp[0] == '-')
	{
	    size -= atoi(cp+1);
	}
	else
	{
	    size = atoi(cp);
	}

	if (size < 1)
	    size = 1;
	if (size > 7)
	    size = 7;

	PRSDBG(("Setting font size to %d\n", size));

	x = UNPACK(context->tos->effects_active, STYLE_WF_INDEX);
	x &= ~WEBFONT_SIZE_MASK;
	x |= (size - 1) << WEBFONT_SIZE_SHIFT;
	PACK(context->tos->effects_active, STYLE_WF_INDEX, x);
    }

    /* pdh: font colour */
    if (attributes->value[HTML_FONT_COLOR].type != value_none)
    {
        int thecolour;
        unsigned int no;

	htmlriscos_colour( &attributes->value[HTML_FONT_COLOR], &thecolour );

	no = rh_find_colour( me->rh, thecolour );

	PACK( context->tos->effects_active, STYLE_COLOURNO, no );
    }
}

extern void startbr (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_text_item *last;

    generic_start (context, element, attributes);

    text_item_push_break(me);

    /* Borris asks how this can ever be NULL */
    last = (me && me->rh && me->rh->curstream) ? me->rh->curstream->text_last : NULL;

    if (last &&
	((attributes->value[HTML_BR_CLEAR].type == value_enum) ||
	 (attributes->value[HTML_BR_CLEAR].type == value_void) ) )
    {
	switch ((attributes->value[HTML_BR_CLEAR].type == value_enum) ?
		attributes->value[HTML_BR_CLEAR].u.i :
		HTML_BR_CLEAR_ALL )
	{
	case HTML_BR_CLEAR_LEFT:
	    last->flag |= (rid_flag_CLEARING | rid_flag_LEFTWARDS);
	    break;
	case HTML_BR_CLEAR_RIGHT:
	    last->flag |= (rid_flag_CLEARING | rid_flag_RIGHTWARDS);
	    break;
	case HTML_BR_CLEAR_ALL:
	    last->flag |= (rid_flag_CLEARING | rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS);
	    break;
	}
    }
}

extern void startmap (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    new_map_item(me, &attributes->value[HTML_MAP_NAME]);
}

/*****************************************************************************/

/*
 * These elements are for line break control.
 * NOBR /NOBR inhibits line breaks
 * WBR inserts a line break within a NOBR block
 */

extern void startnobr (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_start (context, element, attributes);

    htmlctx->no_break = 1;
}

extern void finishnobr (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_finish (context, element);

    htmlctx->no_break = 0;
}

extern void startwbr (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    text_item_push_break(htmlctxof(context));
}

/*****************************************************************************/

extern void finishbody (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}






extern void finisha (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *me = htmlctxof(context);

    generic_finish (context, element);

    if (me->aref && me->aref->first == NULL)
    {
	if (me->rh->curstream->text_last)
	    me->aref->first = me->rh->curstream->text_last;
	else if (me->rh->curstream != &me->rh->stream)			/* only if not top level stream */
	    text_item_push_word(me, rid_flag_NO_BREAK, FALSE);
    }

    me->aref = NULL;
}



/* eof special.c */
