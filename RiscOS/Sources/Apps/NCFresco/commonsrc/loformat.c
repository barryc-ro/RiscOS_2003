/* -*-C-*- commonsrc/loformat.c
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Bottom end formatting routines.

*****************************************************************************/

#define FORMAT_PRIVATE_BITS

#define MAX(a,b)  ((a) > (b) ? (a) : (b))


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
#include "indent.h"
#include "webfonts.h"
#include "gbf.h"
#include "profile.h"
#include "images.h"
#include "oimage.h"

#ifdef PLOTCHECK
#include "rectplot.h"
#define NOTWRITTEN	plotcheck_boom()
#else
#define NOTWRITTEN	ASSERT(!"FACILITY NOT YET IMPLEMENTED")
#endif

#if DEBUG
static const char * WIDTH_NAMES[N_COLSPAN_WIDTHS] = WIDTH_NAMES_ARRAY;
#endif


static void close_down_current_line(RID_FMT_STATE *fmt);

/*****************************************************************************

  **** GBF_MINWIDTH_A ****

  Determine the effective current formatting width.

  Originally this would have been minwidth.

  But this got overridden to the supplied fwidth where possible so a
  single wide item did not cause everything to format beyond the
  supplied fwidth (window size) unnecessarily.

  But this has perceived "problems" with the adjaceny of some floating
  items. These arise due to the combination of having less pixels than
  the "expected" (ie much less than 640) coupled with an increased
  desire to minimise the amount of horizontal scrolling required. Thus
  GBF_MINWIDTH_A has been created in an attempt to meet such a
  contradictory set of requirements.

  When a line is created, a right margin is assigned to it. Two
  choices are now available - the minwidth value or the supplied
  fwidth value. The rules of formatting are updated as follows:

  IF minwidth < fwidth THEN fwidth is always used
  ELIF about to place right floaters THEN minwidth is used
  ELSE fwidth is used
  FI

  Minwidth is calculated in a different fashion: max(LM) + max(CW) + max(RM)

  If there is room within minwidth, floating items do not count when
  considering whether a non-floating item fits. Many implications of
  this!

*****************************************************************************/


/* Choose which format_width to use for a new line */
#define NEW_FWIDTH(fmt)			\
(					\
    (fmt)->pending_right_floaters 	\
    ? (fmt)->format_width2 		\
    : (fmt)->format_width		\
)					/**/

/* Pick up the format width previously chosen for the specified line */
#define GET_FWIDTH(fmt,pi)		\
(					\
	( (pi) == NULL || (pi)->floats == NULL ) \
	? ( (fmt)->format_width )	\
	:				\
	(				\
	    (pi)->floats->right == NULL \
	    ? (pi)->floats->right_margin 	\
	    : (pi)->floats->right->entry_margin\
        )				\
)					/**/

/*****************************************************************************/

static void center_and_right_align_adjustments(RID_FMT_STATE *fmt)
{
    rid_pos_item *new_pos = fmt->text_line;

    if (new_pos->first != NULL)
    {
	const int display_width = GET_FWIDTH(fmt, new_pos);
	const int width = fmt->x_text_pos;
	const int spare = (fmt->fmt_method == MAYBE) ?
	    (new_pos->floats->right_margin - fmt->x_text_pos) :
	    0;
	int flags = 0;
	rid_text_item *first_non_scaff;	/* SJM: 07Aug97: added this to catch HR after a SCAFF */

	FMTDBGN(("center_and_right_align_adjustments: display_width %d, text_pos.x %d, flags %x, spare %d\n",
		 display_width, width, 0 /* fmt->SOL->st.flags & rid_sf_ALIGN_MASK */, spare));

	if (new_pos->first->tag == rid_tag_SCAFF && new_pos->first->next != NULL)
	{
	    flags = new_pos->first->next->st.flags;
	    first_non_scaff = new_pos->first->next;
	}
	else
	{
	    flags = new_pos->first->st.flags;
	    first_non_scaff = new_pos->first;
	}
	
	switch (flags & rid_sf_ALIGN_MASK)
	{
	case rid_sf_ALIGN_JUSTIFY:
	    if (first_non_scaff->width != MAGIC_WIDTH_HR && spare > 0)
	    {
		rid_text_item *ti, *lti;
		int n;

		for(n=0, lti = NULL, ti = new_pos->first; ti; ti = ti->next)
		{
		    FMTDBG(("Item=%p, pos=%p, line=%p, flags=0x%02x\n",
			    ti, new_pos, ti->line, ti->flag));

		    if ( CLEARING_ITEM(ti) || ! FLOATING_ITEM(ti) )
		    {
			/* Not a floater */
			if (ti->line != new_pos)
			    break;
			n++;
			lti = ti;
		    }
		}

		FMTDBG(("Spare=%d, n=%d\n", spare, n));

		/* Have more than one word on the line and not the end of the paragraph */
		if ((n > 1) &&
#if NEW_BREAKS
		    ((lti->flag & (rid_flag_CLEARING | rid_flag_BREAK)) == 0) &&
#else
		    ((lti->flag & (rid_flag_CLEARING | rid_flag_LINE_BREAK)) == 0) &&
#endif
		    ti != NULL )
		{
		    new_pos->leading = spare / (n-1);

		    FMTDBG(("Leading=%d\n", new_pos->leading));
		}
	    }
	    break;
	case rid_sf_ALIGN_CENTER:
	    if (first_non_scaff->width != MAGIC_WIDTH_HR && spare > 0)
	    {
		/* Only center things that can fit on the display */

		FMTDBGN(("center, %d %d\n", width, display_width));

		if (width < display_width)
		{
		    new_pos->left_margin += (spare >> 1);
		    FMTDBG(("1 new_pos->left_margin %d\n", new_pos->left_margin));
		}
	    }
	    break;
	case rid_sf_ALIGN_RIGHT:
	    if (first_non_scaff->width != MAGIC_WIDTH_HR && spare > 0)
	    {
		FMTDBGN(("Right aligned\n"));

		new_pos->left_margin += spare;
		FMTDBG(("2 new_pos->left_margin %d\n", new_pos->left_margin));
	    }
	    break;
	}
    }
}

/*****************************************************************************/

static void dispose_pos_list(RID_FMT_STATE *fmt)
{
    rid_pos_item *pi = fmt->stream->pos_list;

    PINC_DISPOSE_POS_LIST;

    if (pi == NULL)
    {
	/*FMTDBGN(("dispose_pos_list: no pos list\n"));*/
	TASSERT(fmt->stream->pos_list == NULL);
    }
    else
    {
	/*FMTDBGN(("dispose_pos_list: freeing previous pos list\n"));*/

        /* pdh: this doesn't free float_links or float_items

	while (pi != fmt->stream->pos_last)
	{
	    rid_pos_item *pi2 = pi->next;
	    mm_free(pi);
	    pi = pi2;
	}
	mm_free( fmt->stream->pos_last );
	 */

	rid_free_pos( fmt->stream->pos_list );

	fmt->stream->pos_list = fmt->stream->pos_last = NULL;
    }
}

static rid_pos_item *new_pos_item(RID_FMT_STATE *fmt)
{
    rid_pos_item *new = mm_calloc(1, sizeof(*new));

    new->st = fmt->stream;
    new->floats = mm_calloc(1, sizeof(*new->floats));
    new->left_margin = NOTINIT;

    rid_pos_item_connect(fmt->stream, new);

    /*FMTDBGN(("pi%p: npi: floats %p\n", new, new->floats ));*/

    return new;
}


/*****************************************************************************/

/*

  IMPORTANT: ANY UBS MIGHT CONTAIN FLOATING ITEMS. THESE ARE NOT PART
  OF THE SEQUENCE AND MUST ALWAYS BE FACTORED OUT.

 */

/*****************************************************************************/

static void get_ubs_width(RID_FMT_STATE *fmt)
{
    rid_text_item *ti = fmt->unbreakable_start;
    int width = 0;
    int last_pad = 0;

    while (1)
    {
	TASSERT(ti != NULL);

	if (! FLOATING_ITEM(ti))
	{
	    if (ti->width == -1)
	    {
		/*ASSERT(width == 0 || width == -1);*/
		/* html/format/misc4.html shows this failing */
		if (width == 0 || width == -1)
		    width = -1;
		/* else pretend width==0 */
	    }
	    else
	    {
		ASSERT(width != -1);
		width += last_pad + ti->width;
	    }
	    last_pad = ti->pad;
	    if (ti == fmt->unbreakable_stop)
		break;
	}
	ti = rid_scanf(ti);
    }

    fmt->unbreakable_width = width;

    FMTDBGN(("get_ubs_width: sequence's width is %d\n", width));
}

/*****************************************************************************/

static void no_text_line(RID_FMT_STATE *fmt)
{
    /* Check can't loose partially done float lines here */
    fmt->last_last_unbreakable = NULL;
    fmt->text_line = NULL;
    fmt->x_text_pos = 0;
}

static void no_unbreakable(RID_FMT_STATE *fmt)
{
    fmt->last_last_unbreakable = fmt->unbreakable_stop;
    fmt->unbreakable_start = NULL;
    fmt->unbreakable_stop = NULL;
    fmt->unbreakable_width = 0;
}

/*****************************************************************************

  Can we fit the next floating item within the current margins. Very
  similar type fitting operation to text words, except, if we place
  the floating item, it will end up outside the resulting margins.
  minwidth rules can force a break. maxwidth is what we are always
  striving for anyway though. UNCERTAIN ABOUT PAD VALUES HERE. The
  line we are attempting to fit the floater within should not have any
  words present, hence there is no need for per-pos x_text_item
  values, and they don't feature in this calculation. */

static BOOL floating_item_fits( RID_FMT_STATE *fmt, rid_pos_item *line,
                                rid_text_item *fltr )
{
    rid_floats_link *floater = line->floats;
    const int LM = line->left_margin;
    const int RM = floater->right_margin;
    const int FW = GET_FWIDTH(fmt, line);
    const int MW = fltr->width;
    const int have_text = TEXT_ITEMS_THIS_LINE(line);
    const int have_fltrs = FLOATERS_THIS_LINE(line);
    BOOL fits = (RM - LM - MW) >= 0;

    ASSERT(line != NULL);
    ASSERT(line->floats != NULL);
    ASSERT(fltr != NULL);
    ASSERT(line->left_margin != NOTINIT);

    FMTDBG(("floating_item_fits: %s: LM/RM %d/%d, FW %d, MW %d, txt %d, fltrs %d\n",
	     fits ? "FITS" : "DOESN'T FIT", LM, RM, FW, MW, have_text, have_fltrs));

    if (fits)
    {
	if (fmt->fmt_method == MUST && (have_text || have_fltrs) )
	{
	    FMTDBGN(("floating_item_fits: forcing fits=FALSE\n"));
	    fits = FALSE;
	}
    }
    else
    {
	/* Items should always fit when performing sizing
           passes. Therefore this tine should only be invoked during a
           real format operation. Restrictions can further be placed
           on when this can occur. Any floating table with a width
           determined other than by <TABLE WIDTH=NN> in absolute
           pixels can cause non-fitting. Any floating item with a
           percentage size can cause non-fitting. The detection of
           non-fitting need not occur with the item that caused
           non-fitted - it may have already been satisfactorily
           formatted starting on earlier lines. Such a non-fitting
           item must always be positionable entirely by itself (either
           minwidth/absolute sized and stream minwidth reflects this
           or %age based when should be restricted to at most 100% for
           any single item). */

	ASSERT(fmt->fmt_method == MAYBE);

	/* pdh: sometimes things just *don't* fit. We don't increase
	   fwidth in this case, 'cos that would make all the rest
	   format to this, too-large, width */
	if ( !have_text && !have_fltrs )
	    return TRUE;

	ASSERT(have_text || have_fltrs);
    }

    return fits;
}

/*****************************************************************************/

static void attach_float_at_end(rid_float_item **flp, rid_float_item *fi)
{
    if (*flp == NULL)
    {
	*flp = fi;
    }
    else
    {
	rid_float_item *tp = *flp;

	/* Not stunningly efficient, but only rarely get more than a
           few floating images per LHS/RHS at once in typical
           documents. */
	while (tp->next != NULL)
	    tp = tp->next;

	tp->next = fi;
    }
}

/*****************************************************************************

  We have a floating item and have decided that it will fit on the
  currently known float line. Append it to the L/R list and update
  information (margins especially) accordingly.

  The extra gluing rules for minwidth/widest and fwidth for floating
  images wanted by Si, and covered by GBF_MINWIDTH_A, are handled
  through getting the right margin set appropriately before we are
  called here. We still need to consider the possibility of not having
  enough space if something like <IMG ALIGN=LEFY WIDTH=70%> is
  repeated a few times.

 */

static void position_floating_item( RID_FMT_STATE *fmt, rid_pos_item *pi,
                                    rid_text_item *ti )
{
    const BOOL leftwards = (ti->flag & rid_flag_LEFTWARDS) != 0;
    rid_float_item *fi = mm_calloc(1, sizeof(*fi));
    rid_floats_link *fl = pi->floats;
    const int width = ti->width;
    const int format_width = GET_FWIDTH(fmt, pi);

    ASSERT(ti != NULL);
    ASSERT(pi != NULL);
    ASSERT(fl != NULL);
    ASSERT(pi->left_margin != NOTINIT);

    FMTDBG(("position_floating_item: %s, ti %p, pi %p, fi %p, fl %p, MW %d, LM %d, RM %d, FW %d, widest %d\n",
	    leftwards ? "LEFTWARDS" : "RIGHTWARDS",
	    ti, pi, fi, fl,
	    width, pi->left_margin, fl->right_margin, format_width, fmt->stream->widest ));

    ti->line = pi;
    fi->ti = ti;
    fi->pi = pi;
    fi->height = ti->max_up + ti->max_down; /* ? */
    fi->height_left = fi->height;

    if (leftwards)
    {
	fi->entry_margin = pi->left_margin;
	pi->left_margin += width;               /* hmm */

	FMTDBG(("3 pi->left_margin %d\n", pi->left_margin));

	fmt->x_text_pos = pi->left_margin;

	attach_float_at_end(&fl->left, fi);
    }
    else
    {
	if (width > format_width)
	{
	    FMTDBG(("position_floating_item: too wide special case\n"));
#if DEBUG && 0
	    ASSERT(!"This branched shouldn't be needed anymore!");
#endif
	    fi->entry_margin = width;
	    fl->right_margin = 0;
	    if (width > fmt->stream->widest)
		fmt->stream->widest = width;
	}
	else
	{
	    if (fl->right_margin > fmt->stream->widest &&
		fmt->fmt_method == MAYBE &&
		gbf_active(GBF_MINWIDTH_A))
	    {
		fmt->stream->widest = fl->right_margin;
		FMTDBG(("position_floating_item: MA: override widest to %d\n", fmt->stream->widest));
	    }
	    fi->entry_margin = fl->right_margin;
	    fl->right_margin -= width;
	}
	attach_float_at_end(&fl->right, fi);
    }

    FMTDBG(("ti%p: position_floating_item: em=%d w=%d now got LM %d, RM %d\n",
             ti, fi->entry_margin, width,
	     pi->left_margin, fl->right_margin ));

#if DEBUG
    /* pdh: crossing the margins is only allowed if there's only this floater
     * on the line
     */
    if ( ( fl->right_margin - pi->left_margin ) < 0 )
    {
        if ( leftwards )
        {
            ASSERT( fl->left == fi );
            ASSERT( fl->right == NULL );
        }
        else
        {
            ASSERT( fl->right == fi );
            ASSERT( fl->left == NULL );
        }
    }
#endif
}

/*****************************************************************************

  If we've got any pending floaters, try and attach them to the current
  line.  */

static void attach_new_floaters( RID_FMT_STATE *fmt, rid_pos_item *pi )
{
    rid_text_item *ti = fmt->first_pending_floater;

    ASSERT(pi->first == NULL);

    while ( ti )
    {
        FMTDBG(("ti%p: testing in a_n_f\n", ti));

        if ( floating_item_fits( fmt, pi, ti )
             || !FLOATERS_THIS_LINE(pi) )
        {
            position_floating_item( fmt, pi, ti );

            if ( ti->flag & rid_flag_LEFTWARDS )
                fmt->pending_left_floaters--;
            else
                fmt->pending_right_floaters--;

            if ( ti == fmt->last_pending_floater )
            {
                fmt->last_pending_floater = NULL;
                fmt->first_pending_floater = NULL;
                return;
            }
        }
        else
        {
            /* doesn't fit */
            fmt->first_pending_floater = ti;
            return;
        }

        /* just fitted one, but there's more: find 'em */
        do {
            ti = ti->next;
        } while ( ti && !FLOATING_ITEM(ti) );

        ASSERT( ti != NULL );
    }
}


/*****************************************************************************

  If have no floating lines, creating a line at the end of the pos
  list with no margin information, else advance into an existing pos
  item with existing margin information.  */

static void create_new_text_line(RID_FMT_STATE *fmt)
{
    ASSERT(fmt->text_line == NULL);
    ASSERT(fmt->unbreakable_start != NULL || fmt->first_pending_floater != NULL );

    {
	/* Create at end, no margin info */
	rid_pos_item *new = new_pos_item(fmt);
	fmt->text_line = new;
	FMTDBGN(("pi%p: create_new_text_line: Created a new line\n", new));
    }
}

/*****************************************************************************

  How much left margin have we got "trapped" behind left floaters...?  */

static int excess_left_margin_due_to_floaters( const rid_pos_item *pi )
{
    int excess = 0;
    int shouldbe = 0;
    rid_floats_link *fl = pi->floats;
    rid_float_item *fi;

    if ( !fl )
        return 0;
    fi = fl->left;

    while ( fi )
    {
        excess += ( fi->entry_margin - shouldbe );
        shouldbe = fi->entry_margin + fi->ti->width;
        fi = fi->next;
    }
    return excess;
}

/*****************************************************************************

  ... right floaters?  */

static int excess_right_margin_due_to_floaters( const RID_FMT_STATE *fmt,
                                                const rid_pos_item *pi )
{
    int excess = 0;
    int shouldbe = fmt->format_width;
    /* pdh: had been changed to GET_FWIDTH() but that made the function
     * always return zero; the whole point is that 'shouldbe' refers to
     * the galley width irrespective of floaters.
     * GET_FWIDTH() got it wrong in the <blockquote><img align=right>... case
     */
    rid_floats_link *fl = pi->floats;
    rid_float_item *fi;

    if ( !fl )
        return 0;
    fi = fl->right;

    while ( fi )
    {
        if ( shouldbe != fi->entry_margin )
            FMTDBG(("ermdf: shouldbe=%d entry_margin=%d\n", shouldbe,
                    fi->entry_margin));

        /* pdh: as right floaters can extend beyond fwidth, we should cater
         * for these being the wrong way round. This should only occur when
         * (a) this is the only right floater and (b) there are no left
         * floaters or any text items, in which case the actual answer to
         * the excess question is irrelevant.
         */
        if ( shouldbe < fi->entry_margin )
            return 0;

        excess += ( shouldbe - fi->entry_margin );
        shouldbe = fi->entry_margin - fi->ti->width;
        fi = fi->next;
    }
    return excess;
}

/*****************************************************************************

  Margin due to left floaters...   */

static int left_float_margin( const rid_pos_item *pi )
{
    int margin = 0;

    if ( pi->floats )
    {
        rid_float_item *fi = pi->floats->left;

        while ( fi )
        {
            margin = fi->entry_margin + fi->ti->width;
            fi = fi->next;
        }
    }
    return margin;
}

/*****************************************************************************

  ... right floaters   */

static int right_float_margin( const RID_FMT_STATE *fmt,
                               const rid_pos_item *pi )
{
    int margin = GET_FWIDTH(fmt, pi);

    if ( pi->floats )
    {
        rid_float_item *fi = pi->floats->right;

        while ( fi )
        {
            margin = fi->entry_margin - fi->ti->width;
            fi = fi->next;
        }
    }
    return margin;
}

/*****************************************************************************

  Set the margins on this line from the given text item (repeated
  often enough to warrant its own routine).

  */

static void set_margins_from_item( const RID_FMT_STATE *fmt, rid_pos_item *pi,
                                   const rid_text_item *ti, const int format_width )
{
    FMTDBGN(("set_margins_from_item: using fwidth of %d\n", format_width));

    while ( ti && (FLOATING_ITEM(ti) || ti->tag == rid_tag_SCAFF) )
        ti = ti->next;

    if ( ti )
    {

#if DEBUG
        if ( ti->tag == rid_tag_TEXT )
        {
            rid_text_item_text *tit = (rid_text_item_text*)ti;
            char *text = fmt->rh->texts.data + tit->data_off;
            FMTDBGN(("set_margins_from_item: text «%s»\n",text));
        }
	else
	    FMTDBGN(("set_margins_from_item: item type %d\n", ti->tag ));
#endif


        pi->left_margin = ti->st.indent * INDENT_UNIT;
	FMTDBG(("4 pi->left_margin %d\n", pi->left_margin));

        pi->floats->right_margin = format_width
                                    - ( (ti->flag & rid_flag_RINDENT)
                                         ? (INDENT_UNIT*INDENT_WIDTH) : 0 );
    }
    else
    {
        pi->left_margin = 0;
        pi->floats->right_margin = format_width;
    }
}


/*****************************************************************************

  A text line exists but has no margin information. Initialise this
  from the start of the unbreakable sequence. The caller now supplies
  what they believe we should be using for the fwidth value.  */

static void set_text_margin_info(RID_FMT_STATE *fmt, const int format_width)
{
    rid_pos_item *pi = fmt->text_line;
    int emdf;
    int floatmargin;
    int rindent;

    PINC_SET_TEXT_MARGIN_INFO;

    ASSERT(pi != NULL);

    /* Doesn't account for INDENT_SIZE */
    set_margins_from_item( fmt, pi, fmt->unbreakable_start, format_width );

    /* pdh: At this point, pi->left_margin is the amount of margin we want
     * for text_item reasons (lists, blockquote)
     */

    emdf = excess_left_margin_due_to_floaters(pi);
    floatmargin = left_float_margin(pi);

    FMTDBGN(("pi%p: LEFT: text_item margin=%d, emdf=%d, float margin=%d,", pi,
            pi->left_margin, emdf, floatmargin ));

    pi->left_margin -= emdf;
    if ( pi->left_margin < 0 )
        pi->left_margin = 0;

    pi->left_margin += floatmargin;

    FMTDBGN((" result %d\n", pi->left_margin));

    emdf = excess_right_margin_due_to_floaters(fmt, pi);
    floatmargin = right_float_margin(fmt, pi);
    rindent = format_width - pi->floats->right_margin;

    FMTDBGN(("pi%p: RIGHT: text_item indent=%d, emdf=%d, float margin=%d,", pi,
            rindent, emdf, floatmargin ));

    rindent -= emdf;
    if ( rindent < 0 )
        rindent = 0;

    pi->floats->right_margin = floatmargin - rindent;

    FMTDBGN((" result %d\n", pi->floats->right_margin));

    /* Should be before start adding to this line */
    fmt->x_text_pos = pi->left_margin;

    FMTDBG(("set_text_margin_info: LM %d, RM %d, XP %d, RI %d\n",
	     pi->left_margin,
	     pi->floats->right_margin,
	     fmt->x_text_pos,
	     rindent));
}

/*****************************************************************************/

static BOOL no_text_margin_info(RID_FMT_STATE *fmt)
{
    return fmt->text_line->left_margin == NOTINIT;
}

/*****************************************************************************/

static BOOL text_margin_indent_clash(RID_FMT_STATE *fmt)
{
    return FALSE;
}

/*****************************************************************************

  See if we can fit the current unbreakable sequence on the current
  line. Many factors influence this decision.

  Unbreakable sequences have already been factored out, with the
  resulting chain being stored in unbreakable_start to
  unbreakable_stop. This means places where we CANNOT have a break
  have been attended to. Places where we MUST break remain to be
  handled though.

  If the simple widths sum says there is no room, we always answer
  FALSE.

  This needs to incorporate the minwidth and maxwidth formatting logic.

  During pos items with floating items, we accept zero words per
  line. During left margin indented eg due to <LI>, we require one
  item minimum per line. When there are no floating items and no left
  margin indents we require one word minimum. Basically, a text word
  fits if there are no floating items on that line, whatever any other
  bits of logic say.

  Return FALSE for doesn't fit meaning 'must not go on this line', and
  TRUE for does fit meaning 'must go on this line'.

  If we are performing min/max formatting in a very wide stream, then
  the need for DONT is lost: we just won't break unless we need to. Of
  course, this is based upon the line not wanting to be longer than
  about 2^31, which is a perfectly reasonable limitation for a 32 bit
  implementation to my view.  */

static BOOL unbreakable_sequence_fits(RID_FMT_STATE *fmt)
{
    rid_pos_item *line = fmt->text_line;
    rid_text_item *llu = fmt->last_last_unbreakable;
    const int prev_pad = llu == NULL ? 0 : llu->pad;
    const int LM = line->left_margin;
    const int RM = line->floats->right_margin;
    const int FW = GET_FWIDTH(fmt, line);
    const int TW = fmt->unbreakable_width + prev_pad;
    const BOOL have_floaters = FLOATERS_THIS_LINE(line);
    const BOOL have_text = TEXT_ITEMS_THIS_LINE(line);
    rid_text_item *ti = fmt->unbreakable_start;
    BOOL fits;

    ASSERT(fmt->text_line != NULL);
    ASSERT(fmt->text_line->left_margin != NOTINIT);

    /* pdh: reappearance of the infamous DL COMPACT bodge */
    if ( ti->tag == rid_tag_BULLET
         && ((rid_text_item_bullet*)ti)->list_type == HTML_DL )
    {
        int ipos = LM + INDENT_WIDTH*INDENT_UNIT;
        /*
        FMTDBG(("Found fake DL bullet %d (%d,%d,%d)\n",
                    ((rid_text_item_bullet*)ti)->list_type,
                     width, left_margin, INDENT_WIDTH*INDENT_UNIT));
         */
        if ( fmt->x_text_pos > ipos )
        {
            FMTDBG(("unbreakable_sequence_fits: a DL COMPACT forces break\n"));
            return FALSE;
        }
        else
        {
            FMTDBG(("unbreakable_sequence_fits: a DL COMPACT pad bodge\n"));
            if ( llu )
            {
                llu->pad = ( ipos - fmt->x_text_pos );
            }
        }
    }

    fits = (RM - fmt->x_text_pos - TW) >= 0;

    if (fits)
    {
	if ( fmt->fmt_method == MUST )
	{
	    if ( have_floaters || have_text )
	    {
		FMTDBGN(("unbreakable_sequence_fits: forcing fits=FALSE\n"));
		fits = FALSE;
	    }
	}
    }
    else
    {
	if (!have_text)
	{
	    if (!have_floaters)
	    {
		FMTDBGN(("unbreakable_sequence_fits: forcing fits=TRUE\n"));
		fits = TRUE;
	    }
	    else if ( (line->floats == NULL || (line->floats->right == NULL)) &&
		      fmt->fmt_method == MAYBE &&
		      gbf_active(GBF_MINWIDTH_A))
	    {
		const int diff = fmt->format_width2 - fmt->format_width;
		if ( diff > 0 && (RM - fmt->x_text_pos - TW + diff) >= 0 )
		{
		    FMTDBG(("unbreakable_sequence_fits: MA: override and force fit\n"));
		    fits = TRUE;
		}
	    }
	}
    }

    FMTDBGN(("unbreakable_sequence_fits: %s: pp %d, LM/RM %d/%d, FW %d, TW %d, XP %d, flt %d, txt %d\n",
	     fits ? "FITS" : "DOESN'T FIT",
	     prev_pad,  LM, RM, FW, TW, fmt->x_text_pos, have_floaters, have_text));

    return fits;
}

/*****************************************************************************

  Update the 'widest' field of the stream. This is complicated when
  there are right floating items. If we are doing a maxwidth format,
  we wish to be careful about getting a useful answer. A line is
  composed of left floaters (LLLL), text items (TTTT), empty space
  (....) and right floaters (RRRR). ZOM for all of them, except the
  line must hold at least a floater or a text item. Thus:

  LLLL TTTT TTTT .... RRRR

  The widest value we want to set is the sum of LLLL, TTTT and RRRR
  sections.

  */

static void find_widest_info(RID_FMT_STATE *fmt)
{
    const int FW = GET_FWIDTH(fmt, fmt->text_line);
    const int LM = fmt->text_line->left_margin;
    const int used = fmt->x_text_pos + (FW - fmt->text_line->floats->right_margin);

    if (LM != NOTINIT)
	if (used > fmt->stream->widest)
	    fmt->stream->widest = used;

    FMTDBGN(("find_widest_info: MB: LM %d, XP %d, FW %d, RM %d, used %d, widest now %d\n",
	    LM, fmt->x_text_pos, FW, fmt->text_line->floats->right_margin, used, fmt->stream->widest));

    if (fmt->fmt_method != MAYBE && gbf_active(GBF_MINWIDTH_A))
    {
	const int LM = fmt->text_line->left_margin;
	const int RM = FW - fmt->text_line->floats->right_margin;
	const int CW = fmt->x_text_pos - LM;

	if (LM != NOTINIT)
	{
	    if (LM > fmt->LM_widest)
		fmt->LM_widest = LM;

	    if (RM > fmt->RM_widest)
		fmt->RM_widest = RM;

	    if (CW > fmt->CW_widest)
		fmt->CW_widest = CW;
	}

	FMTDBGN(("find_widest_info: MA: LM %d, FW %d, CW %d, RM %d, LMW %d, CWW %d, RMW %d\n",
		LM, FW, CW, RM,
		fmt->LM_widest,
		fmt->CW_widest,
		fmt->RM_widest));
    }
}

/*****************************************************************************

  There may or may not be another line after text_line. Either way, a
  new line needs creating immediately after text_line, being inserted
  before subsequent lines if they exist. We set everything manually
  here as its such a special case. Margin information is inherited
  from the line we are fracturing. */

static void fracture_float_line(RID_FMT_STATE *fmt)
{
    rid_pos_item *text = fmt->text_line;
    rid_pos_item *later = text->next;
    rid_pos_item *new = mm_calloc(1, sizeof(*new));

    ASSERT(text != NULL);
    ASSERT(fmt->stream->pos_list != NULL);
    ASSERT(fmt->stream->pos_last != NULL);

    /* Link into chain */
    new->prev = text;
    text->next = new;

    if (later != NULL)
    {
	later->prev = new;
	new->next = later;
    }

    FMTDBGN(("ffl: %p -> added %p -> %p\n", text, new, later ));

    if (text == fmt->stream->pos_last)
    {
	ASSERT(later == NULL);
	fmt->stream->pos_last = new;
    }

    /* pdh: Margin values will be overwritten in fracture_copy_*, but only
     * on sides with floaters on. These margins are only used for adding
     * further floaters. The margins used for nonfloating items will be
     * determined when the first ubs is added to the line.
     */

    new->st = fmt->stream;
    new->floats = mm_calloc(1, sizeof(*new->floats));

/*     ti = fmt->unbreakable_start ? fmt->unbreakable_start */
/*                                 : fmt->next_item; */
/*  */
/*     FMTDBG(("fracture_float_line: ubs=%p (a %d), ni=%p (a %d)\n", */
/*             fmt->unbreakable_start, fmt->unbreakable_start ? fmt->unbreakable_start->tag : -1, */
/*             fmt->next_item, fmt->next_item ? fmt->next_item->tag : -1 )); */

    set_margins_from_item( fmt, new, fmt->unbreakable_start, GET_FWIDTH(fmt, text) );

    FMTDBGN(("pi%p: ffl: floats=%p\n", new, new->floats ));

    /* Linked into chain - still need to replicate surviving floating items. */
}


static void fracture_copy_left(RID_FMT_STATE *fmt)
{
    rid_pos_item *old = fmt->text_line;
    rid_pos_item *new = old->next;
    rid_float_item *fi = old->floats->left;
    rid_float_item *last = NULL;
    rid_float_item **finp = &new->floats->left;	/* Floating Item Next Pointer */
    int carried_over = 0;

    PINC_FRACTURE_COPY_LEFT;

    ASSERT(new != NULL);
    ASSERT(old->floats != NULL);
    ASSERT(fi != NULL);
    ASSERT(new->floats != NULL);

    while (fi != NULL)
    {
	if (fi->height_left > 0)
	{
	    /* Allocate new float_item to hold copy */
	    *finp = mm_calloc(1, sizeof(**finp));
	    /* Bulk copy the floating item */
	    **finp = *fi;
	    /* But update the linkage */
	    (*finp)->next = NULL;
	    last = *finp;
	    /* And advance our pointer */
	    finp = &((*finp)->next);
	    carried_over++;
	}
	fi = fi->next;
    }

    ASSERT(finp != &new->floats->left);

#if 0
    /* pdh: At this point, new->left_margin is the amount of margin we want
     * for text_item reasons (lists, blockquote)
     */

    FMTDBG(("pi%p: text_item margin=%d, emdf=%d, float margin=%d,", new,
            new->left_margin, excess_margin_due_to_floaters(new),
            last->entry_margin + last->ti->width ));

    new->left_margin -= excess_margin_due_to_floaters( new );
    if ( new->left_margin < 0 )
        new->left_margin = 0;
#endif

    new->left_margin = last->entry_margin + last->ti->width;

/*     FMTDBG((" result %d\n", new->left_margin)); */

    FMTDBGN(("fracture_copy_left: carried over %d items\n", carried_over));
}


static void fracture_copy_right(RID_FMT_STATE *fmt)
{
    rid_pos_item *old = fmt->text_line;
    rid_pos_item *new = old->next;
    rid_float_item *fi = old->floats->right;
    rid_float_item **finp = &new->floats->right;
    rid_float_item *last = NULL;
    int carried_over = 0;

    PINC_FRACTURE_COPY_RIGHT;

    ASSERT(new != NULL);
    ASSERT(old->floats != NULL);
    ASSERT(fi != NULL);
    ASSERT(new->floats != NULL);

    while (fi != NULL)
    {
	if (fi->height_left > 0)
	{
	    *finp = mm_calloc(1, sizeof(**finp));
	    last = *finp;
	    **finp = *fi;
	    (*finp)->next = NULL;
	    finp = &((*finp)->next);
	    carried_over++;
	}
	fi = fi->next;
    }

    ASSERT(finp != &new->floats->right);

    new->floats->right_margin = last->entry_margin - last->ti->width;

    FMTDBGN(("fracture_copy_right: carried over %d items\n", carried_over));
}

/*****************************************************************************

  THIS IS MESSY. The text line being finished also has zOM floating
  items. We might need to fracture the current line to continue the
  floating item(s).

  For floating items that finish on our lowest pixel or above it, this
  text line represents the last of the processing required for them,
  and the point they cease contributing to the L/R margins and hence
  don't need replicating.

  For floating items where the bottom pixel of this text line is still
  above their bottom pixels, we need to fracture the current text into
  a text line with height dictated by the text contents of the text
  line and a second line to continue the height of the floating
  items. This second line might itself get fractured in the future,
  and so on until the height of the floating item is used up. Such a
  new fractured float line is past the point of having floating items
  added to it - we just need to flow the text correctly.

  Note that if we are called to close a line with floating items only
  (say IMG BR IMG), then the line will have no height. In that case,
  pretend the height of the line is the smallest value it could be
  whilst finishing at least one image. If this repeats multiple times,
  it will at least work. The alternative is to think harder about
  removing N images on one go. but this might easily end up with
  different semantics and be a bastard to track.  */

static void consider_fracturing_float_line(RID_FMT_STATE *fmt)
{
    rid_pos_item *pi = fmt->text_line;
    int this_height = pi->max_up + pi->max_down;
    rid_float_item *fi;
    BOOL l_fracture = FALSE;
    BOOL r_fracture = FALSE;

    FMTDBG(("consider_fracturing_float_line: this_height %d\n", this_height));

    if (this_height == 0)
    {
	this_height = INT_MAX;

	for (fi = pi->floats->left; fi != NULL; fi = fi->next)
	    if (fi->height_left < this_height && fi->height_left > 0)
		this_height = fi->height_left;

	for (fi = pi->floats->right; fi != NULL; fi = fi->next)
	    if (fi->height_left < this_height && fi->height_left > 0)
		this_height = fi->height_left;

	pi->max_up = this_height;

	FMTDBG(("consider_fracturing_float_line: this_height NOW %d\n", this_height));
    }

    /* Scan all floating items of this line and see if there are any
       that require us to fracture this line. Update their remaining
       height according to the line being completed. This can lead to
       negative height information - if this happens, it indicates how
       many pixels of padding at the bottom of the floater to the
       bottom of its last line. */
    for (fi = pi->floats->left; fi != NULL; fi = fi->next)
    {
	FMTDBG(("consider_fracturing_float_line: left height %d lowered by %d\n", fi->height_left, this_height));
	fi->height_left -= this_height;
	l_fracture |= (fi->height_left > 0);
    }

    for (fi = pi->floats->right; fi != NULL; fi = fi->next)
    {
	FMTDBG(("consider_fracturing_float_line: right height %d lowered by %d\n", fi->height_left, this_height));
	fi->height_left -= this_height;
	r_fracture |= (fi->height_left > 0);
    }

    if (l_fracture || r_fracture)
    {
	static char *msg[4] =
	{
	    "",
	    "left side",
	    "right side",
	    "both sides"
	};

	FMTDBG(("consider_fracturing_float_line: %s causing fracturing\n",
		msg[l_fracture + 2 * r_fracture]));

	/* Need to *insert* a new pos item and replicate the floating
           items that haven't been finished yet. */
	fracture_float_line(fmt);
	if (l_fracture)
	    fracture_copy_left(fmt);
	if (r_fracture)
	    fracture_copy_right(fmt);
    }
}


/*****************************************************************************

  The current text line is finished with. We can now determine its
  height and perform center/right adjustments. If there is a following
  float line that we can pickup on, we use that for text_line else
  NULL and a new item needs creating when a text item is next
  encounted.

  Given height information, we can determine whether any current
  floating items have their bottom pixel within this line or a line
  yet to come. If the latter, we need to fracture the current line
  into two lines. Remember that there might already be a floating line
  after us, so this means we can get multiple partially completed
  floating lines. Floating items are always added at the end of the
  stream, so only one float pointer (float_line) is needed.  */

static void close_down_current_line(RID_FMT_STATE *fmt)
{
    rid_pos_item *pi = fmt->text_line;
    rid_text_item *ti;
    int max_up = 0;
    int max_down = 0;
    BOOL goRoundAgain = FALSE;

    PINC_CDCL;

    ASSERT(fmt->text_line != NULL);

    /* Find height info for this line according to text items only */
    for (ti = pi->first; ti != NULL; ti = rid_scanf(ti))
    {
	if ( ! FLOATING_ITEM(ti) )
	{
	    /* Having determined it's not a floating item, this test
               can now be applied. */
	    if (ti->line != pi)
		break;

            /* If we've got an <img align=top> then we need *first* to set
             * max_up according to everything else, *then* go round again
             * placing them all at max_up in order to find out what max_down
             * is.
             */
	    if ( ti->tag == rid_tag_IMAGE
	         && ( ((rid_text_item_image*)ti)->flags & rid_image_flag_ATOP)
	       )
	        goRoundAgain = TRUE;

	    if (ti->max_up > max_up)
		max_up = ti->max_up;
	    if (ti->max_down > max_down)
		max_down = ti->max_down;
	}
    }

    FMTDBGN(("close_down_current_line: max up/down %d/%d\n", max_up, max_down));
#if DEBUG
    pi->linenum = ++fmt->linenum;

    if (debug_get("FMTDBGN"))
	dump_pos(pi);
#endif

    /* Can now set the height information for this strip */
    pi->top = fmt->y_text_pos;
    pi->max_up = max_up;
    pi->max_down = max_down;

    /* Go round again now; only <img align=top> will change */
    if ( goRoundAgain )
    {
        FMTDBGN(("close_down_current_line: recalculating max_up/down 'cos of images\n"));
        max_up = 0;
        max_down = 0;
        /* Very like the loop above */
        for (ti = pi->first; ti != NULL; ti = rid_scanf(ti))
        {
	    if ( ! FLOATING_ITEM(ti) )
	    {
	        if (ti->line != pi)
		    break;

	        /* Lots of filth for <IMG align=top> */
	        if ( ti->tag == rid_tag_IMAGE )
	        {
	            /* pdh: as si pointed out, just calling size DTWT
	             * with percentage images, so do this instead
	             */
	            int reduction = ti->st.indent * INDENT_UNIT
	                 + ( (ti->flag & rid_flag_RINDENT)
	                                ? (INDENT_UNIT*INDENT_WIDTH)
	                                : 0 );

                    oimage_size_allocate( ti, fmt->rh, fmt->doc,
                                          fmt->stream->fwidth - reduction );
/* 	            (object_table[ti->tag].size)( ti, fmt->rh, fmt->doc ); */
	        }

                /* max_up shouldn't actually be different this time round,
                 * but max_down may well be */
	        if (ti->max_up > max_up)
		    max_up = ti->max_up;
	        if (ti->max_down > max_down)
		    max_down = ti->max_down;
	    }
        }
        pi->max_up = max_up;
        pi->max_down = max_down;
        FMTDBGN(("close_down_current_line: max up/down %d/%d\n", max_up, max_down));
    }

    /* Might want to fracture a strip here */
    if ( FLOATERS_THIS_LINE(fmt->text_line)
         || fmt->first_pending_floater )
    {
        FMTDBG(("cdcl: has floaters, calling cffl\n"));
	consider_fracturing_float_line(fmt);
    }

    /* fracturing may change these */
    fmt->y_text_pos -= pi->max_up + pi->max_down;

    /* Close down line information */
    find_widest_info(fmt);
    center_and_right_align_adjustments(fmt);

    /* Auto advance text_line if have floating lines to advance in to */
    if (pi->next != NULL)
    {
	FMTDBGN(("close_down_current_line: got float line to use now\n"));
#if 0
	if (pi->next == fmt->float_line)
	{
	    FMTDBG(("close_down_current_line: have caught float line %p up - no float line now\n", fmt->float_line));
	    no_float_line(fmt);
	}
#endif
	no_text_line(fmt);
	fmt->text_line = pi->next;
	fmt->x_text_pos = fmt->text_line->left_margin;
    }
    else
	no_text_line(fmt);

    /* Note: this frees the right_margin value. This is needed in
       find_widest_info(), so this MUST happen afterwards, even
       although above looks like having a natural else part to attach
       this to. This also needs to happen after
       center_and_right_align_adjustments() has been called as that
       also needs the right margin value. Once
       center_and_right_align_adjustments() has been called, all text
       is positioned in distances from the left margin. */
    if ( ! FLOATERS_THIS_LINE(pi) )
    {
        FMTDBGN(("pi%p: freeing floats %p\n", pi, pi->floats ));
	mm_free(pi->floats);
	pi->floats = NULL;
    }

    FMTDBGN(("close_down_current_line: widest now %d, height %d/%d, line now %p, ypos %d, XP %d\n\n",
	     fmt->stream->widest,
	     pi->max_up,
	     pi->max_down,
	     fmt->text_line,
	     fmt->y_text_pos,
	     fmt->x_text_pos));
#if DEBUG
    if (debug_get("FMTDBGN"))
	dump_pos(pi);
#endif

    /* EXPERIMENT: Once a line has been finished with, free the
       pos_item to lower the transient memory overheads. */
#if 1
    if (fmt->fmt_method != MAYBE)
    {
	/* Only free once past any floating areas, which have more
           complex dependencies. */
	if (pi->floats == NULL)
	{
	    rid_free_pos_term(fmt->stream->pos_list, pi, fmt->stream);
	    FMTDBGN(("close_down_current_line: freeing pos list until %p\n", pi));
	}
    }
#endif
}

/*****************************************************************************

  We have determined that the current ubs will fit on the current
  line. Margin information was initialised ages ago. Each item in the
  ubs needs attaching to the current line - note that there may be
  floaters in the sequence. If the ubs indicates it must be followed
  by a break, we perform this NOW. Having a break overrides not having
  a break.

 */

static void append_unbreakable_sequence(RID_FMT_STATE *fmt)
{
    rid_text_item *ti = fmt->unbreakable_start;
    rid_pos_item *line = fmt->text_line;

    ASSERT(fmt->unbreakable_start != NULL);
    ASSERT(fmt->unbreakable_stop != NULL);
    ASSERT(fmt->text_line != NULL);

    /* Point line at start if currently empty */
    if (line->first == NULL)
	line->first = ti;

    /* Attach text items to their assigned line */
    while (1)
    {
	if (!FLOATING_ITEM(ti))
	{
	    ASSERT(ti->line == NULL);
	    ti->line = fmt->text_line;
	    if (ti == fmt->unbreakable_stop)
		break;
	}
	else
	{
	    /* Floaters should have been attached during the scan that
               yielded the last item in the ubs. */

            /* pdh: no longer true, floaters may stay pending for quite a
             * while
             */

/* 	    ASSERT(ti->line != NULL); */
	}
	ti = rid_scanf(ti);
    }

    /* Update X position */
    if (fmt->unbreakable_width != -1)
    {
	fmt->x_text_pos += fmt->unbreakable_width;
	FMTDBGN(("append_unbreakable_sequence: ubs contributes %d, giving %d\n",
		 fmt->unbreakable_width, fmt->x_text_pos));
    }

    if (fmt->last_last_unbreakable != NULL /* && fmt->x_text_pos + fmt->last_line_unbreakable->pad < fmt->stream->widest */)
    {
	fmt->x_text_pos += fmt->last_last_unbreakable->pad;
	FMTDBGN(("append_unbreakable_sequence: prev pad contributes %d, giving %d\n",
		 fmt->last_last_unbreakable->pad, fmt->x_text_pos));
    }

    FMTDBGN(("append_unbreakable_sequence: this line %d, widest %d from %d\n",
	     fmt->x_text_pos, fmt->stream->widest, fmt->unbreakable_width));

    /* If we must be followed by a break, ensure it now */
    if ( MUST_BREAK(fmt->unbreakable_stop) )
    {
	FMTDBG(("append_unbreakable_sequence: forcing BREAK\n"));
	no_unbreakable(fmt);
	close_down_current_line(fmt);
    }
    else
	no_unbreakable(fmt);
}

/*****************************************************************************/

static BOOL only_scaffolding(RID_FMT_STATE *fmt)
{
    while (fmt->unbreakable_start != fmt->unbreakable_stop &&
	   fmt->unbreakable_start->tag == rid_tag_SCAFF)
	fmt->unbreakable_start = fmt->unbreakable_start->next;

    return fmt->unbreakable_start->tag == rid_tag_SCAFF;
}

/*****************************************************************************

  We have a new unbreakable sequence of text items to place. We need
  to find a line which meets our margin criteria and into which we can
  fit. We might need to create lines or step past lines to achieve
  this.  */

static void deal_with_unbreakable_sequence(RID_FMT_STATE *fmt)
{
    BOOL usf = FALSE;

    if (fmt->unbreakable_start->tag == rid_tag_SCAFF)
    {
	fmt->previous_pad = 0;
    }

    FMTDBGN(("deal_with_unbreakable_sequence: a %d is come\n", fmt->unbreakable_start->tag));

    if ( /*! only_scaffolding(fmt)*/ 1 )
    {
	get_ubs_width(fmt);

	FMTDBGN(("deal_with_unbreakable_sequence: ubs width is %d\n", fmt->unbreakable_width));

	do
	{
	    while (1)
	    {
		FMTDBGN(("deal_with_unbreakable_sequence: looking for text line without margin clash\n"));

		if ( fmt->text_line == NULL )
		    create_new_text_line(fmt);

                if ( fmt->text_line->first == NULL
                     || no_text_margin_info( fmt ) )
                {
		    set_text_margin_info( fmt, NEW_FWIDTH(fmt) );
		    attach_new_floaters( fmt, fmt->text_line );
		}

		if ( text_margin_indent_clash(fmt) || ! (usf = unbreakable_sequence_fits(fmt)) )
                {
                    FMTDBGN(("deal_with_unbreakable_sequence: calling closedown\n"));
		    close_down_current_line(fmt);
		}
		else
		    break;
	    }
	    FMTDBGN(("deal_with_unbreakable_sequence: found: looking for line with enough room\n"));
	} while ( ! usf );

	FMTDBGN(("deal_with_unbreakable_sequence: found: appending ubs to line\n"));

	append_unbreakable_sequence(fmt);
    }
    FMTDBGN(("deal_with_unbreakable_sequence: out, ubs=%p\n",
            fmt->unbreakable_start));
}

/*****************************************************************************

  Handle the next non-floating item. Determine if we have finished the
  current unbreakable sequence and maybe proceed to placing

  */

static void next_nonfloating_item(RID_FMT_STATE *fmt)
{
    rid_text_item *ti = fmt->next_item;

    if (fmt->unbreakable_start == NULL)
    {
	FMTDBGN(("next_nonfloating_item: Starting a new unbreakable sequence\n"));
	fmt->unbreakable_start = ti;
    }

    /* Just track as each item goes by */
    fmt->unbreakable_stop = ti;

    /* Hence only spit out entire unbreakable sequences */
    if ( ! DONT_BREAK(ti) )
	deal_with_unbreakable_sequence(fmt);
}

/*****************************************************************************

  Handle the next floating item.

  The spec sez:

  align=left
            floats the image to the current left margin, temporarily
            changing this margin, so that subsequent text is flowed
            along the image's righthand side. The rendering depends on
            whether there is any left aligned text or images that
            appear earlier than the current image in the markup. Such
            text (but not images) generally forces left aligned images
            to wrap to a new line, with the subsequent text continuing
            on the former line.

  align=right
            floats the image to the current right margin, temporarily
            changing this margin, so that subsequent text is flowed
            along the image's lefthand side. The rendering depends on
            whether there is any right aligned text or images that
            appear earlier than the current image in the markup. Such
            text (but not images) generally forces right aligned
            images to wrap to a new line, with the subsequent text
            continuing on the former line.

  Both Netscape and SpyGlass take a lazy view of what floating to the
  margin means and simply choose a line with no text (consider the
  alternative on a partially filled line of left aligned text and
  trying to float to the right margin). We do the same lazy approach
  for the sake of compatibility (yuch!).


  */

static void next_floating_item(RID_FMT_STATE *fmt)
{
    rid_text_item *ti = fmt->next_item;

    FMTDBG(("ti%p: pending floater, width=%d\n", ti, ti->width ));

    /* der neue floater: don't even think about placing it until we sign
     * off the current line.
     */
    if ( !fmt->first_pending_floater )
        fmt->first_pending_floater = ti;
    fmt->last_pending_floater = ti;
    if ( ti->flag & rid_flag_LEFTWARDS )
        fmt->pending_left_floaters++;
    if ( ti->flag & rid_flag_RIGHTWARDS )
        fmt->pending_right_floaters++;
}

/*****************************************************************************

 */

static void perform_clearing(RID_FMT_STATE *fmt)
{
    BOOL left, right;

    left  = (fmt->next_item->flag & rid_flag_LEFTWARDS)  != 0;
    right = (fmt->next_item->flag & rid_flag_RIGHTWARDS) != 0;

/*     FMTDBGN(("perform_clearing( %s%s), tl=%p, fl=%p\n", */
/*              left ? "LEFT " : "", right ? "RIGHT " : "", */
/*              fmt->text_line, fmt->float_line )); */

    while ( fmt->text_line
             && (    (left &&  (fmt->text_line->floats->left
                                || fmt->pending_left_floaters) )
                  || (right && (fmt->text_line->floats->right
                                || fmt->pending_right_floaters) ) ) )
    {
        /* Get rid of any word we've got lying around -- this can only happen
         * the first time round the loop
         */
        if (fmt->unbreakable_start != NULL)
            deal_with_unbreakable_sequence(fmt);

	if ( fmt->text_line->first == NULL
             || no_text_margin_info(fmt) )
	{
	    set_text_margin_info(fmt, NEW_FWIDTH(fmt) );
	    attach_new_floaters(fmt, fmt->text_line);
	}

        FMTDBG(("perform_clearing: calling closedown\n"));

	close_down_current_line(fmt);

	if ( !fmt->text_line
	     && (    (left  && fmt->pending_left_floaters)
	          || (right && fmt->pending_right_floaters) ) )
	    create_new_text_line( fmt );
    }
}

/*****************************************************************************/

static void formatting_start(RID_FMT_STATE *fmt)
{
    static char *fnames[3] = { "MAYBE", "MUST", "DONT" };

    rid_text_item *ti;

    /* Debugging fmt->format_width is fine as the is where we decide
       what fmt->format_width2 is going to be anyway. */
    FMTDBG(("formatting_start: format_width %d, stream->fwidth %d, stream->widest %d, minwidth %d, maxwidth %d\n",
	    fmt->format_width, fmt->stream->fwidth, fmt->stream->widest,
	    fmt->stream->width_info.minwidth, fmt->stream->width_info.maxwidth));

    /* Pick up the stream we are formatting */
    fmt->next_item = fmt->stream->text_list;

    /* Not expecting to partially reformat at present. */
    ASSERT(fmt->stream->pos_list == NULL);

    /* Nothing in hand to start with */
    no_unbreakable(fmt);
    no_text_line(fmt);
/*     no_float_line(fmt); */

    /* Initialise bits and pieces */
    fmt->format_width = fmt->stream->fwidth;
    fmt->format_width2 = fmt->stream->fwidth;
    fmt->y_text_pos = 0;
    fmt->linenum = 0;
    fmt->stream->widest = 0;
    fmt->stream->width = 0;

    /* If not using GBF_MINWIDTH_A, then both format widths are the
       same, so it makes no difference which value is used. This
       removes a lot of dynamic tests for which method is
       active. Avoid using the different behaviour during sizing
       passes. */
    if (gbf_active(GBF_MINWIDTH_A) &&
	fmt->fmt_method == MAYBE &&
	fmt->format_width < fmt->stream->width_info.minwidth)
    {
	FMTDBG(("formatting_start: applying fmt->stream->width_info.minwidth of %d\n", fmt->stream->width_info.minwidth));
	fmt->format_width2 = fmt->stream->width_info.minwidth;
    }

    /* Initialise these specifically, whether using GBF_MINWIDTH_A or
       not. This permits the gbf_active() test to be on the RHS of the
       &&, eliminating a lot of calls. */
    fmt->LM_widest = 0;
    fmt->CW_widest = 0;
    fmt->RM_widest = 0;

    if (fmt->next_item == NULL)
    {
	FMTDBG(("\n\nformatting_start: WARNING: empty text stream\n\n"));
	return;
    }

    /* Set width of all tables to actual/min/max according to reason
       being formatted. */
    for (ti = fmt->stream->text_list;
	 ti != NULL;
	 ti = rid_scanf(ti))
    {
	/* Might be reformatting - used for assertions elsewhere. */
	ti->line = NULL;

	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table*)ti)->table;
	    int cand_width = 0;
	    int x;

	    switch (fmt->fmt_method)
	    {
	    case MAYBE:
		/* This width should take into account any caption
                   requirements by now. */
		cand_width = table->hwidth[ACTUAL];
#if DEBUG
		if (table->caption != NULL)
		{
		    x = table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table);
		    FMTDBG(("formatting_start: x %d, cand_width %d\n", x, cand_width));
		    if (x > cand_width)
		    {
			TASSERT(x > cand_width);
		    }
		}
#endif
		break;
	    case DONT:
		if ( (table->flags & rid_tf_HAVE_WIDTH) != 0 &&
		     table->userwidth.type == value_absunit)
		{
		    cand_width = (fmt->doc->scale_value * ceil(table->userwidth.u.f)) / 100;
		    FMTDBG(("formatting_start: pickup user width of %d\n", ti->width));
		    if (cand_width < table->hwidth[LAST_MIN])
		    {
			cand_width = table->hwidth[LAST_MIN];
			FMTDBG(("formatting_start: override user width with %d\n", cand_width));
		    }

		    /* Table has a size, so don't let caption's
                       maxwidth grow us. However, we must still meet
                       its minwidth. */
		    if (table->caption != NULL &&
			table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table) > cand_width)
		    {
			FMTDBG(("formatting_start: caption minwidth %d overrides sized table width %d\n",
				table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table), cand_width));
			cand_width = table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table);
		    }
		}
		else
		{
		    cand_width = table->hwidth[LAST_MAX];

		    /* If the caption can go wider than the table, make
		       the table maxwidth appear bigger than it actually
		       is. */
		    if (table->caption != NULL &&
			table->caption->stream.width_info.maxwidth + CAPTION_TOTAL_BIAS(table) > cand_width)
		    {
			FMTDBG(("formatting_start: caption maxwidth %d overrides table maxwidth %d\n",
				table->caption->stream.width_info.maxwidth + CAPTION_TOTAL_BIAS(table), cand_width));
			cand_width = table->caption->stream.width_info.maxwidth + CAPTION_TOTAL_BIAS(table);
		    }
		}

		break;
	    case MUST:
		if ( (table->flags & rid_tf_HAVE_WIDTH) != 0 &&
		     table->userwidth.type == value_absunit)
		{
		    cand_width = (fmt->doc->scale_value * ceil(table->userwidth.u.f)) / 100;
		    FMTDBG(("formatting_start: pickup user width of %d\n", cand_width));
		    if (cand_width < table->hwidth[LAST_MIN])
		    {
			cand_width = table->hwidth[LAST_MIN];
			FMTDBG(("formatting_start: override user width with %d\n", cand_width));
		    }
		}
		else
		    cand_width = table->hwidth[LAST_MIN];

		/* If the minwidth of the caption is greater than the
                   minwidth of the table, we must present the larger
                   minwidth. This applies whether the table is sized
                   or not. */
		if (table->caption != NULL &&
		    table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table) > cand_width)
		{
		    FMTDBG(("formatting_start: caption minwidth %d overrides table minwidth %d\n",
			    table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table), cand_width));
		    cand_width = table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table);
		}

		break;
	    }

	    /* DAF: 970628: ti->width is SHORTISH which is currently a
	       plain short, so we only have 15 bits available for
	       horizontal widths. Perform some clipping! */
	    if (cand_width > 0x7fff)
	    {
		FMTDBG(("\n\nformatting_start: clipped width from %d to 0x7fff\n\n\n", cand_width));
		cand_width = 0x7fff;
	    }

	    ti->width = cand_width;

	    FMTDBG(("formatting_start: set table's id=%d width to %d (%s)\n",
		    ((rid_text_item_table *)ti)->table->idnum, cand_width, fnames[fmt->fmt_method]));
	}

	/* SJM: do the stuff for other scaleable items */
    	if ((ti->tag == rid_tag_IMAGE &&
	     ((rid_text_item_image *)ti)->ww.type == value_pcunit) ||
	    (ti->tag == rid_tag_INPUT &&
	     (((rid_text_item_input *)ti)->input)->tag == rid_it_IMAGE && (((rid_text_item_input *)ti)->input)->ww.type == value_pcunit) ||
	    (ti->tag == rid_tag_OBJECT &&
	     (((rid_text_item_object *)ti)->object)->userwidth.type == value_pcunit)
	    )
	{
	    switch (fmt->fmt_method)
	    {
	    case MAYBE:
		break;
	    case DONT:
	    case MUST:
		ti->width = 1;
		break;
	    }

	    FMTDBG(("formatting_start: set tag %d's width to %d (%s)\n", ti->tag, ti->width,
		    fnames[fmt->fmt_method]));
	}
    }

    FMTDBG(("\nformatting_start: initialised formatting state, width %d (A %d), mode %s\n",
	    fmt->format_width, fmt->format_width2, fnames[fmt->fmt_method]));
}

/*****************************************************************************

  The stream is finished. Flush until we get a pos item with nothing
  on it, thus ensuring all lines with something on them get properly
  completed. We could use pseudo_html(, "<BR CLEAR=ALL>"), but this
  either messes up progressive rendering or is of no use until the end
  of the stream. */

static void formatting_stop(RID_FMT_STATE *fmt)
{
    rid_text_item fudge_ti;
    rid_text_stream *stream = fmt->stream;

    ASSERT(fmt->next_item == NULL);

    /* Flush any word */
    if (fmt->unbreakable_start != NULL)
    {
	FMTDBGN(("formatting_stop: flushing inhand unbreakable sequence\n"));
	deal_with_unbreakable_sequence(fmt);
    }

    if (fmt->text_line != NULL)
    {
	FMTDBG(("formatting_stop: closing down current line\n"));
	close_down_current_line(fmt);
    }

    while ( fmt->text_line != NULL
             || fmt->pending_left_floaters || fmt->pending_right_floaters )
    {
	FMTDBGN(("formatting_stop: close line to get past floaters\n"));

	if ( !fmt->text_line )
	    create_new_text_line( fmt );

        if ( fmt->text_line->first == NULL
             || no_text_margin_info( fmt ) )
        {
	    set_text_margin_info(fmt, NEW_FWIDTH(fmt) );
	    attach_new_floaters( fmt, fmt->text_line );
	}

	attach_new_floaters( fmt, fmt->text_line );

	close_down_current_line(fmt);
    }

#if 0
    if ( fmt->float_line )
    {
        FMTDBG(("** fmt->float_line = %p in formatting_stop (?)\n", fmt->float_line ));
    }
#endif

    /* Then force a last pos item to give the stream a final height
       and so that we can do certain grubby list following things that
       now relay upon there being a final, empty pos item. fudge_ti is
       used purely for create_new_text_line()'s
       assertions. no_unbreakable() will clear the inhand unbreakable
       anyway. */

    fmt->unbreakable_start = fmt->unbreakable_stop = &fudge_ti;
    create_new_text_line(fmt);
    no_unbreakable(fmt);
    close_down_current_line(fmt);

    if (fmt->fmt_method != MAYBE && gbf_active(GBF_MINWIDTH_A))
    {
#if 1
	/* Attempts to not get carried away with the width to allocate */
	const int a = fmt->LM_widest+ fmt->RM_widest;
	const int b = fmt->CW_widest;
	stream->widest = MAX(stream->widest, a);
	stream->widest = MAX(stream->widest, b);

	/*stream->widest = MAX(a+b, stream->widest);*/

	/*stream->widest = MAX(a,b);*/
#else
	/* Should work, but can lead to very wide widths. */
	stream->widest = fmt->LM_widest + fmt->CW_widest + fmt->RM_widest;
#endif
	FMTDBG(("formatting_stop: GBF_MINWIDTH_A: %d = %d+%d+%d\n",
		stream->widest , fmt->LM_widest , fmt->CW_widest , fmt->RM_widest));
    }

    if (stream->widest > stream->fwidth)
	stream->width = stream->widest;
    else
	stream->width = stream->fwidth;

    stream->height = fmt->y_text_pos;

#if DEBUG
    {
        rid_pos_item *pi;

        FMTDBG(("pos items:"));
        for ( pi = stream->pos_list; pi; pi = pi->next )
        {
            FMTDBG((" %p%c", pi, pi->floats ? '+' : ' '));
        }
        FMTDBG(("\n"));
    }
#endif

    FMTDBG(("formatting_stop: width %d, widest %d, fwidth %d, height %d\n",
	    stream->width,
	    stream->widest,
	    stream->fwidth,
	    stream->height));
}

/*****************************************************************************/

static void formatting_loop(RID_FMT_STATE *fmt)
{
    formatting_start(fmt);

    while (fmt->next_item != NULL/*  && !gbf_active(GBF_VERY_LOW_MEMORY)  */)
    {
	PINC_FORMATTING_LOOP_ITERS;

	/*dump_item(fmt->next_item, fmt->rh->texts.data);*/

	if ( FLOATING_ITEM(fmt->next_item) )
	    next_floating_item(fmt);
	else
	{
	    if ( CLEARING_ITEM(fmt->next_item) )
		perform_clearing(fmt);
	    next_nonfloating_item(fmt);
	}
	fmt->next_item = rid_scanf(fmt->next_item);
    }

    formatting_stop(fmt);
}

/*****************************************************************************

  Format the stream given. No recursion for nested tables. fmt_method
  dictates what line breaking state value to use when we might
  otherwise go for MAYBE. This permits MUST and DONT to be supplied,
  giving minwidth and maxwidth respectively.

  On entry, stream->fwidth indicates the width we are formatting
  within. If fmt_method is DONT or MUST, then this should hold ?.

  On exit, stream->widest is the lenght of the widest line. Displaying
  this stream in less width will cause some of its contents to be
  obscured in some fashion. stream->height is the height of the
  formatted stream (find out if +ve or -ve height!). fmt->width is the
  size we formatted in, and may be the <, == or > widest.

  */

extern void format_stream(antweb_doc *doc,
			  rid_header *rh,
			  rid_text_stream *stream,
			  int fmt_method,
			  int depth)
{
    static char *fmt_names[3] = { "MAYBE", "MUST ", "DONT " };

    RID_FMT_STATE tfmt, *fmt = &tfmt;

    FMTDBG(("\nformat_stream(%p %p stream %p %s): depth %d, fmt_state %p, fwidth %d\n",
	     doc, rh, stream, fmt_names[fmt_method],
	     depth, fmt, stream->fwidth));

    memset(fmt, 0, sizeof(*fmt));
    fmt->doc = doc;
    format_attach_header(fmt, rh);
    format_attach_stream(fmt, stream);
    fmt->depth = depth;
    dispose_pos_list(fmt);
    fmt->fmt_method = fmt_method;

    formatting_loop(fmt);

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

  */

extern void format_attach_stream(RID_FMT_STATE *fmt, rid_text_stream *stream)
{
    FMTDBGN(("attach_stream(%p, %p): attaching stream to format state\n",
	     fmt, stream));

    fmt->stream = stream;
    fmt->format_width = stream->fwidth;
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
	/*ti->flag &= ~ (rid_flag_LEFTWARDS | rid_flag_RIGHTWARDS | rid_flag_CLEARING);*/

	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    stomp_table(table);
	}
    }
}

extern void stomp_captions_until_working(rid_header *header)
{
    if (! gbf_active(GBF_CAPTIONS))
	stomp_stream(&header->stream);
}

/*****************************************************************************

  <IMG> SPACE <IMG> should not introduce a text item and the implied
  text line movement. Crudely, we prevent this by looking for
  sequences that give us gyp and convert the space sequences into
  scaffolding objects.

 */

static void entry_fixes_stream(rid_text_stream *stream, rid_header *rh);

static void entry_fixes_table(rid_table_item *table, rid_header *rh)
{
    int x, y;
    rid_table_cell *cell;

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
	entry_fixes_stream(&cell->stream, rh);
    }
}

static BOOL only_whitespace(rid_text_item *ti, rid_header *rh)
{
    rid_text_item_text *tit = (rid_text_item_text *)ti;
    char *s;
    BOOL only = FALSE;

    flexmem_noshift();		/* no shift whilst accessing text array */

    s = rh->texts.data + tit->data_off;
    if ( ( s[0] == ' ' && s[1] == '\0' )
         || s[0] == '\0' )
	only = TRUE;

    flexmem_shift();		/* no shift whilst accessing text array */

    return only;
}

static void scaffold_check(rid_text_item *ti, rid_header *rh)
{
    FMTDBGN(("scaffold_check: ti is %p\n"));

    if ( ti->next != NULL &&
	 ti->next->tag == rid_tag_TEXT &&
	 ! FLOATING_ITEM(ti->next) &&
	 only_whitespace(ti->next, rh) )
    {
	rid_text_item *other = ti;

	FMTDBG(("scaffold_check: worth scanning\n"));

	while (1)
	{
	    other = other->next;
	    FMTDBG(("scanning: other now %p\n", other));
	    if (other->next == NULL)
		break;
	    if (FLOATING_ITEM(other->next))
		break;
	    if (other->next->tag != rid_tag_TEXT)
		break;
	    if (! only_whitespace(other, rh) )
		break;
	    FMTDBG(("scaffold_check: continue looping\n"));
	}

	FMTDBG(("scaffold_check: other %p, other->next %p\n", other, other->next));

	if ( other->next != NULL && FLOATING_ITEM(other->next) )
	{
	    FMTDBG(("scaffold_check: converting intervening whitespace to scaffold\n"));
	    do
	    {
		ti = ti->next;
		FMTDBG(("scaffold_check: convert %p to rid_tag_SCAFF\n", ti));
		ti->tag = rid_tag_SCAFF;
#if NEW_BREAKS
		ti->flag = rid_break_MUST_NOT;
#else
		ti->flag = rid_flag_NO_BREAK;
#endif
		ti->max_up = ti->max_down = 0;
		ti->pad = ti->width = 0;
	    } while (ti != other);
	}
    }
}

static void entry_fixes_stream(rid_text_stream *stream, rid_header *rh)
{
    rid_text_item *ti;

    for (ti = stream->text_list; ti != NULL; ti = rid_scanf(ti))
    {
	if ( FLOATING_ITEM(ti) )
	    scaffold_check(ti, rh);

	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    entry_fixes_table(table, rh);
	}
    }
}

extern void format_precondition(rid_header *header)
{
    entry_fixes_stream(&header->stream, header);
}

/*****************************************************************************/

static void exit_fixes_stream(rid_text_stream *stream);

static void exit_fixes_table(rid_table_item *table)
{
    int x, y;
    rid_table_cell *cell;

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
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

extern void format_postcondition(rid_header *header)
{
    exit_fixes_stream(&header->stream);
}

/*****************************************************************************/

/* eof loformat.c */
