/* frames.c - <FRAME> <FRAMESET> <NOFRAME> <NOFRAMES> <AREA> */
/* (C) ANT Limited 1996. All rights reserved. */
/* Original code by Simon Middleton. */
/* Converted to new parser by Borris. */
/* and then fixed by Si :-) */
/* CHANGELOG
 * 19/07/96: SJM: fixed the various implict cast warning.
 * 15/09/96: SJM: only trigger NOFRAMES if we've had a FRAMESET (mimic netscape behaviour)
 *		  and trigger it after last /FRAMESET anyway.
 */

#include "htmlparser.h"
#include "rid.h"
#include "memwatch.h"
#include "util.h"
#include "config.h"

#define USE_NOFRAMES	1

#define DEFAULT_BORDER_WIDTH	8		/* OS units */
#define DEFAULT_BORDER_3D	TRUE

/*****************************************************************************/

#if USE_NOFRAMES
static void noframes_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element);
#else
static void frameset_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element);
#endif

/*****************************************************************************/

/* SUPPORTING ROUTINES */

static int munge_frameset_list(VALUE *item, rid_stdunits **list_out, rid_frame_unit_totals *total)
{
    int i, n;
    rid_stdunits *up;

    /* count items */
    n = item->type == value_stdunit_list ? item->u.l.num : 1;

    /* there is no point in storing information about 1 row/col */
    if (n == 1)
        return 1;

    /* take long term copy */
    up = *list_out = mm_calloc(sizeof(rid_stdunits), n);
    memcpy(up, item->u.l.items, sizeof(rid_stdunits) * n);

    total->mult = total->pcent = total->px = 0.0f;
    for (i = 0; i < n; i++, up++)
    {
	switch (up->type)
	{
	case rid_stdunit_PX:
	    total->px += (int)up->u.f;
	    break;
	case rid_stdunit_MULT:
	    total->mult += (int)up->u.f;
	    break;
	case rid_stdunit_PCENT:
	    total->pcent += (int)up->u.f;
            break;
	}
    }

    return n;
}

/*****************************************************************************/

extern void startframe (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_frame *container;
    rid_frame_item *frame;
    VALUE *attr;

    generic_start (context, element, attributes);

    /* if no framesets on the stack then ignore this frame */
    if (me->frameset == NULL)
        return;

    container = mm_calloc(sizeof(*container), 1);
    container->tag = rid_frame_tag_FRAME;

    frame = &container->data.frame;

    if ( (attr = &attributes->value[HTML_FRAME_MARGINWIDTH])->type == value_integer)
	frame->marginwidth = attr->u.i*2;   /* convert to OS units */
    else
	frame->marginwidth = -1;

    if ( (attr = &attributes->value[HTML_FRAME_MARGINHEIGHT])->type == value_integer)
	frame->marginheight = attr->u.i*2;  /* convert to OS units */
    else
	frame->marginheight = -1;

    if ( (attr = &attributes->value[HTML_FRAME_NAME])->type == value_string)
	frame->name = stringdup(attr->u.s);

    if ( attributes->value[HTML_FRAME_NORESIZE].type != value_none)
	frame->noresize = TRUE;

    frame->scrolling = rid_scrolling_AUTO;
    if ( (attr = &attributes->value[HTML_FRAME_SCROLLING])->type == value_enum)
    {
	if (attr->u.i == HTML_FRAME_SCROLLING_YES)
            frame->scrolling = rid_scrolling_YES;
        else if (attr->u.i == HTML_FRAME_SCROLLING_NO)
            frame->scrolling = rid_scrolling_NO;
    }

    if ( (attr = &attributes->value[HTML_FRAME_SRC])->type == value_string)
	frame->src = stringdup(attr->u.s);

    /* Netscape 3 features */
    if ((attr = &attributes->value[HTML_FRAME_FRAMEBORDER])->type == value_bool)
        container->border = attr->u.i;
    else
	container->border = me->frameset->border;
    
    if ((attr = &attributes->value[HTML_FRAME_BORDERCOLOR])->type == value_none)
	attr = &attributes->value[HTML_FRAME_BORDERCOLOUR];
    if (attr->type != value_none)
	htmlriscos_colour( attr, &container->bordercolour);
    else
	container->bordercolour = me->frameset->bordercolour;

    /* link into the list of frames */
    rid_frame_connect(me->frameset, container);
}

#if 0
extern void finishframe (SGMLCTX * context, ELEMENT * element)
{
    generic_finish (context, element);
}
#endif

/*****************************************************************************/

extern void startframeset (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_frame *container;
    rid_frameset_item *frameset;
    VALUE *attr;

    generic_start (context, element, attributes);

    if (!config_display_frames)
    {
        PRSDBG(("startframeset: Ignoring\n"));
        return;
    }

    /* we can only have one top level frameset */
    if (me->frameset == NULL && me->rh->frames != NULL)
    {
        PRSDBG(("startframeset: Ignoring > 1 top level frameset\n"));
        return;
    }

    container = mm_calloc(sizeof(*container), 1);
    container->tag = rid_frame_tag_FRAMESET;

    frameset = &container->data.frameset;

    frameset->ncols = munge_frameset_list(&attributes->value[HTML_FRAMESET_COLS],
					  &frameset->widths,
					  &frameset->width_totals);
    frameset->nrows = munge_frameset_list(&attributes->value[HTML_FRAMESET_ROWS],
					  &frameset->heights,
					  &frameset->height_totals);

    /* set some defaults based on parent frameset */
    if (me->frameset)
    {
        frameset->bwidth = me->frameset->data.frameset.bwidth;
	container->border = me->frameset->border;
	container->bordercolour = me->frameset->bordercolour;
    }
    else
    {
        frameset->bwidth = DEFAULT_BORDER_WIDTH;
	container->border = DEFAULT_BORDER_3D;
	container->bordercolour = -1;
    }

    /* Netscape 3/MSIE3 features */
    if ((attr = &attributes->value[HTML_FRAMESET_BORDER])->type == value_absunit)
        frameset->bwidth = (int)attr->u.f;
    else if ((attr = &attributes->value[HTML_FRAMESET_FRAMESPACING])->type == value_integer)
        frameset->bwidth = attr->u.i*2;

#ifdef STBWEB
    /* border must be 0 or >= DEFAULT */
    if (frameset->bwidth && frameset->bwidth < DEFAULT_BORDER_WIDTH)
	frameset->bwidth = DEFAULT_BORDER_WIDTH;
#endif

    if ((attr = &attributes->value[HTML_FRAMESET_FRAMEBORDER])->type == value_bool)
        container->border = attr->u.i;

    if ((attr = &attributes->value[HTML_FRAMESET_BORDERCOLOR])->type == value_none)
	attr = &attributes->value[HTML_FRAMESET_BORDERCOLOUR];
    if (attr->type != value_none)
    {
	htmlriscos_colour( attr, &container->bordercolour);
    }

    /* use the outermost frameset to set the background colour for the page */
#ifdef STBWEB
    if (me->frameset == NULL)
    {
	me->rh->bgt |= rid_bgt_COLOURS;
	me->rh->colours.back = 0; /* container->bordercolour; */
    }
#endif

    /* save last frameset for when we unstack */
    frameset->old_frameset = me->frameset;

    /* link into the list of framesets */
    if (me->frameset == NULL)
        me->rh->frames = container;
    else
        rid_frame_connect(me->frameset, container);

    /* update total number of frames */
    me->rh->nframes += frameset->ncols * frameset->nrows;

    /* record current frameset */
    me->frameset = container;

#if !USE_NOFRAMES
    /* Replace the delivery mechanism with the noframes variant */
    if (me->object_nesting++ == 0)
	sgml_install_deliver(context, frameset_deliver);
#endif
}

extern void finishframeset (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *me = htmlctxof(context);

    generic_finish (context, element);

    if (me->frameset)
    {
#if 0
	/* SJM: this is some code to move excess frames to somewhere
           sensible but it's not finished as need some reverse
           engineering first */
	rid_frame *frame, *frame_last;
	int i, n;

	frame = me->frameset;
	frame_last = NULL;
	n = frame->data.frameset->ncols * frame->data.frameset->nrows;

	/* count through the frames in this frameset */
	for (i = 0; i < n && frame; i++)
	{
	    frame_last = frame;
	    frame = frame->next;
	}

	/* if there are any frames left at this point then we need to move them */
	if (frame)
	{
	    /* unlink */
	    if (frame_last)
		frame_last->next = NULL;

	    /* link */
	    rid_frame_connect(me->frameset, frame);
	}
#endif	
	/* unstack frameset, check level as we may be skipping framesets */
	me->frameset = me->frameset->data.frameset.old_frameset;
	me->object_nesting--;

	/* If unstack to the end then discard everything else in the file */
	if (me->frameset == NULL)
	{
	    /* Replace the delivery mechanism with the noframes variant */
	    /* pseudo_html(me, "<NOFRAMES>"); */
	    me->object_nesting = 1;
	    sgml_install_deliver(context, &noframes_deliver);
	}
    }
}

#if !USE_NOFRAMES
static void frameset_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    switch (reason)
    {
    case DELIVER_WORD:
    case DELIVER_UNEXPECTED:
    case DELIVER_SGML:
	string_free(&item);
	/* deliberate fall through */

	/* This lot we are ignoring */
    case DELIVER_NOP :
    case DELIVER_SPACE:
    case DELIVER_EOL:
    case DELIVER_POST_OPEN_MARKUP:
    case DELIVER_POST_CLOSE_MARKUP:
	PRSDBG(("frameset_deliver(): ignoring reason %d\n", reason));
	break;

    case DELIVER_PRE_CLOSE_MARKUP:
	/* If the current level then remove the deliver handler and call finish() */
	switch (element->id)
	{
	case HTML_FRAMESET:
	case HTML_FRAME:
	    PRSDBG(("frameset_deliver(): force deliver </%s>\n", element->name.ptr));
	    (*context->dlist->this_fn) (context, reason, item, element);
	    context->force_deliver = TRUE;
	    break;

	default:
	    PRSDBG(("frameset_deliver(): ignoring preclose </%s>\n", element->name.ptr));
	    break;
	}
	break;

    case DELIVER_PRE_OPEN_MARKUP:
	switch (element->id)
	{
	case HTML_FRAMESET:
	case HTML_FRAME:
	    PRSDBG(("frameset_deliver(): force deliver <%s>\n", element->name.ptr));
	    (*context->dlist->this_fn) (context, reason, item, element);
	    context->force_deliver = TRUE;
	    break;

	default:
	    PRSDBG(("frameset_deliver(): ignoring preopen reason %d\n", reason));
	    break;
	}
	break;

	/* This cannot be ignored and must be correctly delivered. */
    case DELIVER_EOS:
	PRSDBG(("frameset_deliver(): EOS forcing restoration of original delivery handler\n"));
	sgml_unwind_deliver(context);

	PRSDBG(("frameset_deliver(): doing the real EOS delivery now\n"));
	(*context->deliver) (context, reason, item, element);
	break;
    }
}
#endif

/*****************************************************************************/

extern void startarea (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    new_area_item(me, me->map, &attributes->value[HTML_AREA_SHAPE],
		  &attributes->value[HTML_AREA_COORDS],
		  &attributes->value[HTML_AREA_HREF],
		  &attributes->value[HTML_AREA_TARGET],
		  &attributes->value[HTML_AREA_ALT]);
    
}

#if 0
extern void finisharea (SGMLCTX * context, ELEMENT * element)
{
    generic_finish (context, element);
}
#endif

/*****************************************************************************

  If frames are being parsed then anything between the NOFRAMES
  /NOFRAMES tag pair should be discarded. Just about anything could be
  between them from a complete document including HTML ...  /HTML to
  just a few paragraphs. Also there is no standardisation on whether
  the NOFRAMES tags should be inside a FRAMESET or outside.
 
  */
#if USE_NOFRAMES

static void noframes_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    switch (reason)
    {
    case DELIVER_WORD:
    case DELIVER_UNEXPECTED:
    case DELIVER_SGML:
	string_free(&item);
	/* deliberate fall through */

	/* This lot we are ignoring */
    case DELIVER_NOP :
    case DELIVER_SPACE:
    case DELIVER_POST_OPEN_MARKUP:
    case DELIVER_POST_CLOSE_MARKUP:
    case DELIVER_EOL:
	PRSDBG(("noframes_deliver(): ignoring reason %d\n", reason));
	break;

    case DELIVER_PRE_CLOSE_MARKUP:
	/* If the current level then remove the deliver handler and call finish() */
	if ((element->id == HTML_NOFRAMES || element->id == HTML_NOFRAME) &&
	    --htmlctx->object_nesting == 0)
	{
	    PRSDBG(("noframes_deliver(): passing reason %d onwards\n", reason));
	    sgml_remove_deliver(context, noframes_deliver);
	    (*context->deliver) (context, reason, item, element);
	    context->force_deliver = TRUE;
	}
	else
	{
	    PRSDBG(("noframes_deliver(): ignoring preclose </%s>\n", element->name.ptr));
	}
	break;

    case DELIVER_PRE_OPEN_MARKUP:
	if (element->id == HTML_NOFRAMES || element->id == HTML_NOFRAME)
	{
	    htmlctx->object_nesting++;
	    PRSDBG(("noframes_deliver(): nested NOFRAMES, count=%d\n", htmlctx->object_nesting));
	}
	else
	{
	    PRSDBG(("noframes_deliver(): ignoring reason %d\n", reason));
	}
	break;

/* This cannot be ignored and must be correctly delivered. */
    case DELIVER_EOS:
	PRSDBG(("noframes_deliver(): EOS forcing restoration of original delivery handler\n"));
	sgml_unwind_deliver(context);
	PRSDBG(("noframes_deliver(): doing the real EOS delivery now\n"));
	(*context->deliver) (context, reason, item, element);
	break;
    }
}
#endif

extern void startnoframes (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_start (context, element, attributes);

#if USE_NOFRAMES
    if (config_display_frames && htmlctx->rh->frames) /* Ignore noframes if we haven't had a frameset */
    {
	/* Replace the delivery mechanism with the noframes variant */
	htmlctx->object_nesting = 1;
	
	sgml_install_deliver(context, &noframes_deliver);
    }
    /* else just ignore it */
#endif
}

extern void finishnoframes (SGMLCTX * context, ELEMENT * element)
{
    generic_finish (context, element);	
}

/* Alternative spellings */

extern void startnoframe (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{  
    startnoframes(context, element, attributes);
}

extern void finishnoframe (SGMLCTX * context, ELEMENT * element)
{
    finishnoframes(context, element);
}

/*****************************************************************************/

/* eof frames.c */
