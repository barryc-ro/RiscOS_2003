/* -*-C-*- commonsrc/loformat.c
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Bottom end formatting routines.

*****************************************************************************/

#define FORMAT_PRIVATE_BITS

#include <limits.h>
#include "rid.h"
#include "antweb.h"
#include "format.h"
#include "debug.h"
#include "colspan.h"
#include "object.h"
#include "tables.h"
#include "htmlparser.h"
#include "dump.h"
#include "util.h"

#ifdef PLOTCHECK
#include "rectplot.h"
#define NOTWRITTEN	plotcheck_boom()
#else
#define NOTWRITTEN	ASSERT(!"FACILITY NOT YET IMPLEMENTED")
#endif

#define pbreak_maybe	generic_maybe
#define pbreak_must	generic_must
#define pbreak_dont	generic_dont
#define hline_maybe	generic_maybe
#define hline_must	generic_must
#define hline_dont	generic_dont
#define text_maybe	generic_maybe
#define text_must	generic_must
#define text_dont	generic_dont
#define bullet_maybe	generic_maybe
#define bullet_must	generic_must
#define bullet_dont	generic_dont

#define image_maybe	generic_maybe
#define image_must	generic_must
#define image_dont	generic_dont

#define input_maybe	generic_maybe
#define input_must	generic_must
#define input_dont	generic_dont
#define textarea_maybe	generic_maybe
#define textarea_must	generic_must
#define textarea_dont	generic_dont
#define select_maybe	generic_maybe
#define select_must	generic_must
#define select_dont	generic_dont

#define object_maybe	generic_maybe
#define object_must	generic_must
#define object_dont	generic_dont

/*
pbreak_maybe pbreak_must pbreak_dont hline_maybe hline_must hline_dont
text_maybe text_must text_dont bullet_maybe bullet_must bullet_dont
image_maybe image_must image_dont input_maybe input_must input_dont
textarea_maybe textarea_must textarea_dont select_maybe select_must
select_dont table_maybe table_must table_dont object_maybe object_must
object_dont
*/




#ifdef DEBUG
static const char * WIDTH_NAMES[N_COLSPAN_WIDTHS] = WIDTH_NAMES_ARRAY;
#endif

/*****************************************************************************

  Get ready for a new horizontal strip of non-floating items. This
  will always result in fmt->WOP existing.

  */

static void prepare_for_text_line(RID_FMT_STATE *fmt)
{
    ASSERT(fmt->WOP == NULL);

    fmt->WOP = mm_calloc(1, sizeof(*fmt->WOP));

    /*FMTDBGN(("prepare_for_text_line: adding pos %p\n", fmt->WOP));*/

    rid_pos_item_connect(fmt->stream, fmt->WOP);
    fmt->WOP->st = fmt->stream;
    fmt->WOP->top = fmt->text_pos.y;
    fmt->SOL = NULL;
    fmt->text_pos.x = 0;
    fmt->max_pos.y = 0;
    fmt->brkstate = fmt->fmt_method;
}

/*****************************************************************************

  Does the current word (fmt->WIT) fit in the current margins? This decision
  is made independent of the ti->pad value. If there are no floating images,
  we can always fit one item. If floating images are present, then we might
  not be able to fit our widest word until we get back to the real margins.

  */

static BOOL current_word_fits(RID_FMT_STATE *fmt)
{
    BOOL b;

    ASSERT(fmt->WIT != NULL);

    b = (fmt->center_width - fmt->text_pos.x - fmt->WIT->width - fmt->previous_pad) >= 0;

    if (!b && fmt->FOP == NULL && fmt->SOL == NULL)
    {
	/* Not floating and starting line - override rule */
	b = TRUE;
    }

    return b;
}

/*****************************************************************************/

static void advance_WIT(RID_FMT_STATE *fmt)
{
    fmt->WIT = rid_scanf(fmt->WIT);
}

static void advance_WIT_and_set_brkstate(RID_FMT_STATE *fmt)
{
    int x;

    if (fmt->WIT == NULL)
	return;

    x = fmt->WIT->flag;

    if ( (x & (rid_flag_LINE_BREAK | rid_flag_EXPLICIT_BREAK)) != 0 )
    {
	fmt->brkstate = MUST;
    }
    else if ( (x & rid_flag_NO_BREAK) != 0 )
    {
	fmt->brkstate = DONT;
    }
    else
    {
	fmt->brkstate = fmt->fmt_method;
    }

    advance_WIT(fmt);
}

/*****************************************************************************

  We have decided to add the current word to the current text line. Someone
  else has made the tests for correctness for this. 

  The addition of the padding is not correct when the next word forces
  us on to a new line. Might wish to alter this to retrospectively
  subtract the padding again?

  */

static void add_word_to_current_line(RID_FMT_STATE *fmt)
{

    if (fmt->WOP == NULL)
	prepare_for_text_line(fmt);

    fmt->text_pos.x += fmt->previous_pad;
    fmt->text_pos.x += fmt->WIT->width;
    fmt->previous_pad = 0;

    /*FMTDBGN(("add_word_to_current_line: now %d\n", fmt->text_pos.x));*/

    if ( fmt->center_width - fmt->text_pos.x - fmt->WIT->pad >= 0 )
    {
	/* Padding between words. Add in iff we stay on the same line
           and have another word. */
	fmt->previous_pad = fmt->WIT->pad;
    }

    if (fmt->WIT->max_up > fmt->WOP->max_up)
	fmt->WOP->max_up = fmt->WIT->max_up;

    if (fmt->WIT->max_down > fmt->WOP->max_down)
	fmt->WOP->max_down = fmt->WIT->max_down;

    if (fmt->SOL == NULL)
	fmt->SOL = fmt->WIT;

    fmt->WIT->line = fmt->WOP;
    if (fmt->WOP->first == NULL)
	fmt->WOP->first = fmt->WIT;

    advance_WIT_and_set_brkstate(fmt);
}

/*****************************************************************************/

static void center_and_right_align_adjustments(RID_FMT_STATE *fmt)
{
    rid_pos_item *new = fmt->WOP;
    const int display_width = fmt->center_width;
    const int width = fmt->text_pos.x;
    const int spare = display_width - width;

    FMTDBG(("center_and_right_align_adjustments: center_width %d, text_pos.x %d, flags %x, spare %d\n",
	    display_width, width, fmt->SOL->st.flags & rid_sf_ALIGN_MASK, spare));

    switch (new->first->st.flags & rid_sf_ALIGN_MASK)
    {
    case rid_sf_ALIGN_JUSTIFY:
	if (new->first->width != MAGIC_WIDTH_HR && spare > 0)
	{
	    rid_text_item *ti, *lti;
	    int n;

	    for(n=0, lti = NULL, ti = new->first; ti; ti = ti->next)
	    {
		FMTDBG(("Item=%p, pos=%p, line=%p, flags=0x%02x\n",
			ti, new, ti->line, ti->flag));

		if ( CLEARING_ITEM(ti) || ! FLOATING_ITEM(ti) )
		{
		    /* Not a floater */
		    if (ti->line != new)
			break;
		    n++;
		    lti = ti;
		}
	    }

	    FMTDBG(("Spare=%d, n=%d\n", spare, n));

	    /* Have more than one word on the line and not the end of the paragraph */
	    if ((n > 1) &&
		((lti->flag & (rid_flag_CLEARING | rid_flag_LINE_BREAK)) == 0) &&
		ti != NULL )
	    {
		new->leading = spare / (n-1);

		FMTDBG(("Leading=%d\n", new->leading));
	    }
	}
	break;
    case rid_sf_ALIGN_CENTER:
	if (new->first->width != MAGIC_WIDTH_HR && spare > 0)
	{
	    /* Only center things that can fit on the display */

	    FMTDBGN(("center, %d %d\n", width, display_width));

	    if (width < display_width)
		new->left_margin += (spare >> 1);
	}
	break;
    case rid_sf_ALIGN_RIGHT:
	if (new->first->width != MAGIC_WIDTH_HR && spare > 0)
	{
	    FMTDBGN(("Right aligned\n"));

	    new->left_margin += spare;
	}
	break;
    }

}

/*****************************************************************************

  For whatever reason, we have decided the current line is
  complete. fmt->WIT is hence the first item not to be part of this
  line.
  
 */

static void complete_current_line(RID_FMT_STATE *fmt)
{
    rid_text_item *ti;

    /*FMTDBGN(("complete_current_line:\n"));*/

    ASSERT(fmt->SOL != NULL);
    ASSERT(fmt->WOP != NULL);

    if (fmt->WOP->first == NULL)
	fmt->WOP->first = fmt->SOL;

    /* Pad from last word does not figure in the widths - otherwise it
       would start the next line, which is obviously wrong. */
    fmt->previous_pad = 0;

    for (ti = fmt->SOL; ti != fmt->WIT; ti = rid_scanf(ti) )
    {
	/* Only connect non-floating items */
	if ( ! FLOATING_ITEM(ti) )
	{
	    ti->line = fmt->WOP;
	}
    }

    fmt->text_pos.y -= fmt->WOP->max_up + fmt->WOP->max_down;

    if (fmt->text_pos.x > fmt->max_pos.x)
	fmt->max_pos.x = fmt->text_pos.x;

    center_and_right_align_adjustments(fmt);

    /* No output pos or start of line anymore */
    fmt->WOP = NULL;
    fmt->SOL = NULL;

    /*FMTDBGN(("complete_current_line: done\n\n"));*/
}

/*****************************************************************************

 */

static void generic_maybe(RID_FMT_STATE *fmt)
{
    FMTDBGN(("generic_maybe:\n"));

    if ( current_word_fits(fmt) )
    {
	add_word_to_current_line(fmt);
    }
    else
    {
	complete_current_line(fmt);
	prepare_for_text_line(fmt);
	/*FMTDBGN(("generic_maybe: expecting this item to be tried again\n"));*/
    }
}

static void generic_must(RID_FMT_STATE *fmt)
{
    FMTDBGN(("generic_must:\n"));

    complete_current_line(fmt);
    prepare_for_text_line(fmt);
    add_word_to_current_line(fmt);
}

static void generic_dont(RID_FMT_STATE *fmt)
{
    FMTDBGN(("generic_dont:\n"));

    add_word_to_current_line(fmt);
}

/*****************************************************************************

 */

static void table_maybe(RID_FMT_STATE *fmt)
{
    rid_table_item *table = ((rid_text_item_table*)fmt->WIT)->table;

    FMTDBGN(("table_maybe: %d\n", table->hwidth[ACTUAL]));

    generic_maybe(fmt);
}

static void table_must(RID_FMT_STATE *fmt)
{
    rid_table_item *table = ((rid_text_item_table*)fmt->WIT)->table;

    FMTDBGN(("table_must: %d\n", table->hwidth[RAW_MIN]));

    generic_must(fmt);
}

static void table_dont(RID_FMT_STATE *fmt)
{
    rid_table_item *table = ((rid_text_item_table*)fmt->WIT)->table;

    FMTDBGN(("table_dont: %d\n", table->hwidth[RAW_MAX]));

    generic_dont(fmt);
}

/*****************************************************************************

  Do initialisations for when not yet started formatting a stream.

  */

static void stream_first_use_init(RID_FMT_STATE *fmt)
{
    rid_text_item *ti;

    for (ti = fmt->WIT = fmt->stream->text_list;
	 ti != NULL;
	 ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table*)ti)->table;

	    switch (fmt->fmt_method)
	    {
	    case MAYBE:
		ti->width = table->hwidth[ACTUAL];
		break;
	    case DONT:
		ti->width = table->hwidth[RAW_MAX];
		break;
	    case MUST:
		ti->width = table->hwidth[RAW_MIN];
		break;
	    }

	    FMTDBG(("stream_first_use_init: set table's width to %d (%d)\n", ti->width, fmt->fmt_method));
	}
    }
}

/*****************************************************************************

  There might be a small number of items whose line is incomplete and hence
  cannot be properly finished with yet. Either further HTML arrives at some
  stage in the future or a specific flush action is required at the end of
  the stream.

  We indicate items are pending by creating an extra rid_pos_item with
  a magic leading value of MAGIC_LEADING_PENDING. At present, there is
  a strong desire to not add any new fields to this structure as the
  memory overheads can be quite significant. All items from fmt->WIT
  onwards are pended, independent of whether they are considered
  floating or not.

  */

#if 0
static void pend_inhand_items(RID_FMT_STATE *fmt)
{
    rid_pos_item *pi = fmt->WOP;

    FMTDBG(("pend_inhand_items:\n"));

    if (pi == NULL)
    {
	FMTDBG(("No output pos item, no text, nothing to do\n"));

	ASSERT(fmt->stream->pos_list == NULL);
	ASSERT(fmt->stream->pos_last == NULL);
	ASSERT(fmt->stream->text_list == NULL);
	ASSERT(fmt->stream->text_last == NULL);

	return;
    }

    ASSERT(fmt->stream->text_list != NULL);
    ASSERT(fmt->stream->text_last != NULL);
    ASSERT(pi->leading != MAGIC_LEADING_PENDING);
    ASSERT(fmt->SOL != NULL);
    ASSERT(pi->first == fmt->SOL);

    pi->leading = MAGIC_LEADING_PENDING;
}  
#endif

/*****************************************************************************

  The end of the stream has been reached. Any trailing pos item marked
  as MAGIC_LEADING_PENDING can now be finalised.
 */

#if 0
static void flush_format_stream(RID_FMT_STATE *fmt)
{
    rid_pos_item *pi;

    FMTDBG(("flush_format_stream(%p:%p)\n", fmt, fmt->stream));

    if (fmt->stream == NULL)
	return;

    pi = fmt->stream->pos_last;

    if (pi == NULL || pi->leading != MAGIC_LEADING_PENDING)
    {
	FMTDBG(("No MAGIC_LEADING_PENDING pos item so no pending\n"));
	return;
    }

    ASSERT(pi->leading == MAGIC_LEADING_PENDING);
    ASSERT(pi == fmt->stream->pos_last);
    ASSERT(pi->first == fmt->SOL);

    /* Clear this before might get used for justification */
    pi->leading = 0;
    
    complete_current_line(fmt);

    FMTDBG(("\nWidest line is %d pixels, total height is %d pixels\n",
	    fmt->max_pos.x, fmt->text_pos.y));

    fmt->stream->widest = fmt->max_pos.x;
    fmt->stream->height = fmt->text_pos.y;
}
#endif

/*****************************************************************************/

static void dispose_pos_list(RID_FMT_STATE *fmt)
{
    rid_pos_item *pi = fmt->stream->pos_list;

    if (pi == NULL)
    {
	/*FMTDBGN(("dispose_pos_list: no pos list\n"));*/
	TASSERT(fmt->stream->pos_list == NULL);
    }
    else
    {
	/*FMTDBGN(("dispose_pos_list: freeing previous pos list\n"));*/

	while (pi != fmt->stream->pos_last)
	{
	    rid_pos_item *pi2 = pi->next;
	    mm_free(pi);
	    pi = pi2;
	}

	fmt->stream->pos_list = fmt->stream->pos_last = NULL;
    }
}

/*****************************************************************************

  Format the stream given. No recursion for nested tables. fmt_method
  dictates what line breaking state value to use when we might
  otherwise go for MAYBE. This permits MUST and DONT to be supplied,
  giving minwidth and maxwidth respectively.

  On entry, stream->fwidth indicates the width we are formatting
  within. If fmt_method is DONT or MUST, then this should hold zero.

  On exit, stream->widest is the lenght of the widest line. Displaying
  this stream in less width will cause some of its contents to be
  obscured in some fashion. stream->height is the height of the
  formatted stream (find out if +ve or -ve height!). fmt->width is the
  size we formatted in, and may be the<. == or > widest.

  */

typedef void (*brkstate_fn) (RID_FMT_STATE *);

static brkstate_fn fn_tuple_table [3 * rid_tag_LAST_TAG] =
{
    pbreak_maybe,	pbreak_must,	pbreak_dont ,
    hline_maybe,	hline_must,	hline_dont ,
    text_maybe,		text_must,	text_dont ,
    bullet_maybe,	bullet_must,	bullet_dont ,
    image_maybe,	image_must,	image_dont ,
    input_maybe,	input_must,	input_dont ,
    textarea_maybe,	textarea_must,	textarea_dont ,
    select_maybe,	select_must,	select_dont ,
    table_maybe,	table_must,	table_dont ,
    object_maybe,	object_must,	object_dont 
};

extern void format_stream(antweb_doc *doc,
			  rid_header *rh,
			  rid_text_stream *stream,
			  int fmt_method)
{
    static char *fmt_names[3] = { "MAYBE", "MUST ", "DONT " };

    RID_FMT_STATE tfmt, *fmt = &tfmt;

    FMTDBGN(("\nformat_stream(%p %p %p %s): fmt_state %p, fwidth %d\n",
	    doc, rh, stream, fmt_names[fmt_method],
	    fmt, stream->fwidth));

    memset(fmt, 0, sizeof(*fmt));
    format_attach_header(fmt, rh);
    format_attach_stream(fmt, stream);
    dispose_pos_list(fmt);
    fmt->brkstate = fmt->fmt_method = fmt_method;

    if (fmt->stream != NULL && fmt->stream->text_list != NULL)
    {
	/* Pick up the text item thread if not already got it */
	if (fmt->WIT == NULL)
	    stream_first_use_init(fmt);

	ASSERT(fmt->WIT != NULL);

	if ( ! FLOATING_ITEM(fmt->WIT) )
	{
	    add_word_to_current_line(fmt);
	}

	/* The action routine advances fmt->WIT if appropriate */
	while (fmt->WIT != NULL)
	{
	    rid_text_item *ti = fmt->WIT;

	    if ( FLOATING_ITEM(ti) )
	    {
		/* Floating item handling, without traditional break stuff */

	    }
	    else
	    {
		/* Standard formatting with the break state machine */
		fn_tuple_table[ ti->tag * 3 + fmt->brkstate ] (fmt);
	    }
	}

	if (fmt->SOL != NULL)
	    complete_current_line(fmt);
	prepare_for_text_line(fmt);

	FMTDBG(("Widest line is %d pixels, total height is %d pixels\n",
		fmt->max_pos.x, fmt->text_pos.y));

	fmt->stream->widest = fmt->max_pos.x;
	fmt->stream->width = /*fmt->max_pos.x;*/ fmt->stream->fwidth;
	fmt->stream->height = fmt->text_pos.y;
    }
    else
    {
	FMTDBGN(("format_stream: nothing to format at this level\n"));
    }

    FMTDBGN(("format_stream(%p): Format complete: width %d, height %d, widest %d.\n", 
	     fmt, fmt->stream->width, fmt->stream->height, fmt->stream->widest));
}

/*****************************************************************************

  Attach a rid_header to a format state. This only occurs once per entire
  format operation, including any recursion that tables may trigger.

  */

extern void format_attach_header(RID_FMT_STATE *fmt, rid_header *rh)
{
    FMTDBGN(("attach_header(%p, %p): attaching header to format state\n",
	     fmt, rh));

    fmt->rh = rh;
}

/*****************************************************************************

  Attach a stream to the current format state. Initialise stream specific
  information (eg the format width as the initial working margins). This
  operation is expected to be performed multiple times to a format state if
  there are tables.

  WHAT HAPPENS TO REMEMBER THE OLD STREAM? - now have seperate format
  state structures.

  */

extern void format_attach_stream(RID_FMT_STATE *fmt, rid_text_stream *stream)
{
    FMTDBGN(("attach_stream(%p, %p): attaching stream to format state\n",
	     fmt, stream));

    fmt->stream = stream;
    fmt->center_width = stream->fwidth;
}

/*****************************************************************************/

static void stomp_stream(rid_text_stream *stream);

static void stomp_table(rid_table_item *table)
{
    int x, y;
    rid_table_cell *cell;

    if (table->caption)
    {
	FMTDBG(("Stomping caption!\n"));
	rid_free_caption(table->caption);
	table->caption = NULL;
    }

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
	stomp_stream(&cell->stream);
    }
}

static void stomp_stream(rid_text_stream *stream)
{
    rid_text_item *ti;

    for (ti = stream->text_list; ti != NULL; ti = rid_scanf(ti))
    {
	/* Stmop floating items for now */
	ti->flag &= ~ (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS | rid_flag_CLEARING);

	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    stomp_table(table);
	}
    }
}

extern void stomp_captions_until_working(rid_header *header)
{
    stomp_stream(&header->stream);
}

/*****************************************************************************

  During table building, the span y field can reflect the number of
  rows we want, not the number actually allocated yet. However, sleft
  records the number yet to acquire. So, quick pre/post format fudge
  to account for this/.

 */

static void entry_fixes_stream(rid_text_stream *stream);

static void entry_fixes_table(rid_table_item *table)
{
    int x, y;
    rid_table_cell *cell;

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
/*	if (cell->sleft > 0)
	    cell->span.y -= cell->sleft;*/
	entry_fixes_stream(&cell->stream);
    }
}

static void entry_fixes_stream(rid_text_stream *stream)
{
    rid_text_item *ti;

    for (ti = stream->text_list; ti != NULL; ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    entry_fixes_table(table);
	}
    }
}

extern void format_entry_fixes(rid_header *header)
{
    entry_fixes_stream(&header->stream);
}

/*****************************************************************************/

static void exit_fixes_stream(rid_text_stream *stream);

static void exit_fixes_table(rid_table_item *table)
{
    int x, y;
    rid_table_cell *cell;

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
/*	if (cell->sleft > 0)
	    cell->span.y += cell->sleft;*/
	exit_fixes_stream(&cell->stream);
    }
}

static void exit_fixes_stream(rid_text_stream *stream)
{
    rid_text_item *ti;

    for (ti = stream->text_list; ti != NULL; ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    exit_fixes_table(table);
	}
    }
}

extern void format_exit_fixes(rid_header *header)
{
    exit_fixes_stream(&header->stream);
}

/*****************************************************************************/

/* eof loformat.c */
