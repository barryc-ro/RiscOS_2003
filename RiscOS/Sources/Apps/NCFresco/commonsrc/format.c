/* -*-C-*- commonsrc/format.c
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Music used:
 *
 * Terrorvision: How to make friends and influence people.
 * The Offspring: Ixnay on the Hombre

 streams should end with <br clear=all> to ensure floating images
 properly finished.


  Percentage constraints have two effects.

  Firstly, they have an upward percolating effect to indicate minimum
  sizes if the particular contents of a cell are to be used. This is
  asking the question: if this cell has minimum size X and occupies Y
  percentage width of the table, what is the smallest size the whole
  table could be and meet both these criteria.

  Secondly, they
  indicate how to share space, as a downward percolating effect, when
  we have chosen a width for the table to be formatted within.

  Thus, the first use indicates whether we can satisfy any percentage
  constrains at all.

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



static void basic_size_stream(antweb_doc *doc,
			      rid_header *rh,
			      rid_text_stream *stream,
			      int depth);

static void basic_size_table(antweb_doc *doc,
			     rid_header *rh,
			     rid_table_item *table,
			     int depth);

static void allocate_widths(antweb_doc *doc,
			    rid_header *rh,
			    rid_text_stream *stream,
			    int fwidth);

static void recurse_format_stream(antweb_doc *doc,
				  rid_header *rh,
				  rid_text_stream *stream,
				  int depth);

static void allocate_widths_stream(antweb_doc *doc,
				   rid_header *rh,
				   rid_text_stream *stream,
				   int fwidth);



#if DEBUG
static const char * WIDTH_NAMES[N_COLSPAN_WIDTHS] = WIDTH_NAMES_ARRAY;
#endif

/*****************************************************************************/

static void format_one_off_assertions(void)
{
    ASSERT(rid_chf_ABSOLUTE == rid_rhf_ABSOLUTE);
    ASSERT(rid_chf_PERCENT  == rid_rhf_PERCENT );
    ASSERT(rid_chf_RELATIVE == rid_rhf_RELATIVE);

    ASSERT(PCT_MIN == ABS_MIN + 1);
    ASSERT(PCT_MAX == ABS_MAX + 1);
    ASSERT(FIRST_MIN == 0);
}


static void format_width_checking_assertions(rid_table_item *table, BOOL horiz)
{
    int *widths = HORIZCELLS(table,horiz);

    ASSERT(widths[RAW_MIN] <= widths[ABS_MIN]);
    ASSERT(widths[ABS_MIN] <= widths[PCT_MIN]);
    ASSERT(widths[PCT_MIN] <= widths[REL_MIN]);

    ASSERT(widths[ABS_MAX] <= widths[PCT_MAX]);
    ASSERT(widths[PCT_MAX] <= widths[REL_MAX]);
/*
    ASSERT(widths[RAW_MIN] <= widths[RAW_MAX]);
    ASSERT(widths[ABS_MIN] <= widths[ABS_MAX]);
    ASSERT(widths[PCT_MIN] <= widths[PCT_MAX]);
    ASSERT(widths[REL_MIN] <= widths[REL_MAX]);
*/
}

/*****************************************************************************/

static void basic_size_stream(antweb_doc *doc,
			      rid_header *rh,
			      rid_text_stream *stream,
			      int depth)
{
    rid_text_item *ti;

    FMTDBGN(("basic_size_stream(%p %p %p): basic stream sizing\n",
	     doc, rh, stream));

    for (ti = stream->text_list; ti != NULL; ti = ti->next)
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    basic_size_table(doc, rh, table, depth+1);
	}
	else
	{
	    (object_table[ti->tag].size)(ti, rh, doc);
	}
    }

    /*dump_header(rh);*/

    /* Now have all items in this stream sized except tables.  any
        nested tables have child sizes calculated. Format this stream
        to pick up size information. Set the stream width as wide as
        it will go so get the padding sums correct as well. URM: this
        means we always end up with the last item having a padding
        contribution when we don't really want it to. Improve someday!  */

    FMTDBGN(("basic_size_stream: now doing raw_minwidth\n"));

    stream->fwidth = 1000000000;
    format_stream(doc, rh, stream, MUST, depth);
    stream->width_info.minwidth = stream->widest;

    FMTDBGN(("basic_size_stream: now doing raw_maxwidth\n"));

    stream->fwidth = 1000000000;
    format_stream(doc, rh, stream, DONT, depth);
    stream->width_info.maxwidth = stream->widest;
}

/*****************************************************************************/

#if 0
static int pct_raw_recalc(rid_table_item *table, BOOL horiz)
{
    int width;

    /* Should be mergable - need colspan_all_and_neql_set() */
    colspan_all_and_eql_set(table, horiz, PCT_RAW,
			    colspan_flag_PERCENT_COL, 0, 0);

    /* Distribute percentages to cater for cells spanning multiple
       underlying grid entries. Find total percentage specified by
       user. Having done this, we operate only with the column header
       information. */

    /*colspan_trace_cells(table, horiz);*/

    /* Clear out unused entries */
    colspan_column_and_eql_set(table, horiz, PCT_RAW,
			       colspan_flag_PERCENT, 0, 0);
    width =  colspan_algorithm(table, PCT_RAW, horiz);

    FMTDBG(("After colspan\n"));
    /*colspan_trace_cells(table, horiz);*/

    colspan_column_init_from_leftmost(table, PCT_RAW, horiz);

    /*colspan_trace_cells(table, horiz);*/

    return width;
}
#endif

/*****************************************************************************

  This is the width below which there will be overlap of objects, even
  with all user constraints totally ignored. We can never display a
  table in less width than this.

*/

static void calc_raw_minwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL horiz)
{
    int *widths = (horiz ? table->hwidth : table->vwidth);
    int width;

    /* Set all unused header columns to zero */
    colspan_all_and_eql_set(table, horiz, RAW_MIN,
			    colspan_flag_USED, 0, 0);

    width = colspan_algorithm(table, RAW_MIN, horiz);

    widths [RAW_MIN] = width + TABLE_OUTSIDE_BIAS(table);

    FMTDBG(("calc_raw_minwidth: %d\n", widths [RAW_MIN]));
}

/*****************************************************************************

  If we totally ignore user constraints, we will only be adding
  whitespace above this size.

*/

static void calc_raw_maxwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL horiz)
{
    int *widths = (horiz ? table->hwidth : table->vwidth);
    int width;

    /* Set all unused header columns to zero */
    colspan_all_and_eql_set(table, horiz, RAW_MAX,
			    colspan_flag_USED, 0, 0);

    width = colspan_algorithm(table, RAW_MAX, horiz);

    widths [RAW_MAX] = width + TABLE_OUTSIDE_BIAS(table);

    FMTDBG(("calc_raw_maxwidth: %d\n", widths[RAW_MAX]));
}

/*****************************************************************************

  We have initialised this slot with any recorded absolute minimum
  pixel value from the user. Before actually resolving this slot, we
  need to update all absmin values to ensure none are smaller than
  their corresponding rawmin values. If they are already bigger than
  their corresponding rawmin value, then we are going to be
  considering the user's constraint and do not stomp with rawmin.

  */

static void calc_abs_minwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL horiz)
{
    int width;
    int *widths = (horiz ? table->hwidth : table->vwidth);

    /* Set unused entries to zero */
    colspan_all_and_eql_set(table, horiz, ABS_MIN,
			    colspan_flag_ABSOLUTE_COL | colspan_flag_ABSOLUTE_GROUP, 0, 0);

    /* Use RAW_MIN values for used locations that do not have an ABS
       contribution. */
    colspan_all_and_eql_copy(table, horiz, ABS_MIN,
			     colspan_flag_ABSOLUTE,
			     0,
			     RAW_MIN);

    /* For all used items with an ABS contribution, ensure it is at
       least as large as the RAW_MIN value. */
    colspan_column_and_eql_lt_copy(table, horiz, ABS_MIN,
				colspan_flag_ABSOLUTE_COL,
				colspan_flag_ABSOLUTE_COL,
				RAW_MIN);

    colspan_all_and_eql_lt_copy(table, horiz, ABS_MIN,
				colspan_flag_ABSOLUTE_GROUP,
				colspan_flag_ABSOLUTE_GROUP,
				RAW_MIN);

    /* and apply colspan algorithm */
    width = colspan_algorithm(table, ABS_MIN, horiz);

    /* Record widths for percentages to work from - use just the
       column headers! */
    colspan_column_init_from_leftmost(table, ABS_MIN, horiz);

    widths[ABS_MIN] = width + TABLE_OUTSIDE_BIAS(table);

    FMTDBG(("calc_abs_minwidth: %d\n", widths[ABS_MIN]));
}

/*****************************************************************************

  Taking into account absolute column width specifications, what is
  the widest the table can be before we are only adding
  whitespace. Note that this (ABS_MAX) can take an arbitary
  relationship to the raw maximum width (RAW_MAX) so care needs taking
  when deciding which slot to format within. Percentages do not figure
  here. If a column is constrained, use that value (unless minwidth
  overrides) else format to maxwidth and use that value (have
  previously done this with RAW_MAX).

  The definition of what we are calculating here: percentages affect
  the minimum width but not the maximum width and so don't figure in
  this. If a column has an absolute contribution, then that size is
  used, else the underlying stream is formatted to maxwidth and the
  relevant proportion of this used. We have a specific conditional
  slot copy operation to perform this, in case something else requires
  it.

  */

static void calc_abs_maxwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL horiz)
{
    int width;
    int *widths = (horiz ? table->hwidth : table->vwidth);

    /* Set unused column headers to zero */
    colspan_all_and_eql_set(table, horiz, ABS_MAX,
			    colspan_flag_ABSOLUTE_COL | colspan_flag_ABSOLUTE_GROUP, 0, 0);

    /* Use RAW_MAX values for used locations that do not have an ABS
       contribution. */
    colspan_all_and_eql_copy(table, horiz, ABS_MAX,
			     colspan_flag_ABSOLUTE,
			     0,
			     RAW_MAX);

    /* For all used items with an ABS contribution, ensure it is at
       least as large as the RAW_MIN value. */
    colspan_column_and_eql_lt_copy(table, horiz, ABS_MAX,
				   colspan_flag_ABSOLUTE_COL,
				   colspan_flag_ABSOLUTE_COL,
				   RAW_MIN);

    colspan_all_and_eql_lt_copy(table, horiz, ABS_MAX,
				colspan_flag_ABSOLUTE_GROUP,
				colspan_flag_ABSOLUTE_GROUP,
				RAW_MIN);

    width = colspan_algorithm(table, ABS_MAX, horiz);

    /* Record widths for percentages to work from */
    colspan_column_init_from_leftmost(table, ABS_MAX, horiz);

    widths [ABS_MAX] = width + TABLE_OUTSIDE_BIAS(table);

    FMTDBG(("calc_abs_maxwidth: %d\n", widths[ABS_MAX]));
}

/*****************************************************************************


  normalise_percentages() must have been called prior to this
  function. We expect to find a normalised (ie working) set of
  percentage constraints in PCT_RAW. We hence only expect to see 100%
  when all columns have a percentage contribution.

  The operational slot should have been initialised with a suitable
  ABS_??? set of values.

  Note the grubby use of 'slot-1'. This assumes ABS_MIN+1=PCT_MIN and
  ABS_MAX+1=PCT_MAX, which is the case at the time of writing. If we
  change our priorities, this might need tweaking. TASSERT development
  assertion used to validate during development - should be invariant
  once production entered.

  TABLE_OUTSIDE_BIAS is NOT included.

  */

static int largest_implied_table_width(rid_table_item *table,
				       width_array_e slot,
				       BOOL horiz)
{
    const int max = (horiz ? table->cells.x : table->cells.y);
    int *widths = (horiz ? table->hwidth : table->vwidth);
    const int pct_used = widths[PCT_RAW];
    /*int left;*/
    int x;
    int width_non_pct = 0;
    int widest = 0;
    /*int N;*/

    FMTDBGN(("largest_implied_table_width(%p %s %s): %d%% used\n",
	    table,
	    WIDTH_NAMES[slot],
	    HORIZVERT(horiz),
	    pct_used));

    ASSERT( widths[PCT_RAW] != NOTINIT );
    ASSERT(slot == PCT_MIN || slot == PCT_MAX);

    /*colspan_trace_cells(table, horiz);*/


    if (pct_used == 0)
    {
	FMTDBGN(("largest_implied_table_width: just using %s values\n",
		 WIDTH_NAMES[slot-1]));

	/* Widest is same as ABS_XXX. correct for bias */
	widest = widths[slot-1] - TABLE_OUTSIDE_BIAS(table);
    }
    else
    {
	/* Then we need to factor in abswidth values */
	for (x = 0; x < max; x++)
	{
	    const int q = table->colspans[x].width[slot];
	    const int a = table->colspans[x].width[PCT_RAW];

	    if ( a > 0 && (table->colspans[x].flags & colspan_flag_PERCENT) != 0 )
	    {
		const int z = (100 * q ) / a;
		if (z > widest)
		    widest = z;
		FMTDBGN(("q %d, a %d, z %d, widest %d\n", q,a,z,widest));
	    }
	    else
	    {
		width_non_pct += q;
	    }
	}

	/* Do a similar calculation for areas not covered by cells with
	   percentage specifictions. */

	if (pct_used != 100)
	{
	    x = (100 * width_non_pct) / (100 - pct_used);

	    if (x > widest)
		widest = x;
	}
	else if (width_non_pct > 0)
	{
	    /* DAF: 8/5/97: ? */
	    FMTDBG(("largest_implied_table_width: 100%% used by width_non_pct is %d\n", width_non_pct));
	    widest += width_non_pct;
	}

	FMTDBG(("largest_implied_table_width: widest %d, width_non_pct %d\n",
		widest, width_non_pct));
#if 0
	/* For each column with a percentage, we need to scale the
           minwidth value according to our newly calculated narrowest
           value. Be wary of small integer problems. 970424: Bitten!  */

	for (left = widest, N = x = 0; x < max; x++)
	{
	    if ( table->colspans[x].flags & colspan_flag_PERCENT )
	    {
		/* Attempt 50:50 rounding distribution. Not random,
                   but hopefully not bad.*/
		const int f = table->colspans[x].width[PCT_RAW];
		if (f > 0)
		{
		    const int z = (widest * f /*+ 50*/) / 100;
		    const int permit = z > left ? left : z;
#if 1
		    const int q = table->colspans[x].width[slot];

		    /* Probably guaranteed, but I'm not very good with
		       errors in integer arithmetic */
		    FMTDBG(("col %d, f %d, z %d, q %d, permit %d, left %d\n", x, f, z, q, permit, left));
		    /*ASSERT(permit >= q); pcent6.html */
#endif
		    if (permit > q)
		    {
			table->colspans[x].width[slot] = permit;
			left -= permit;
			N += 1;
		    }
		    else
		    {
			left -= q;
		    }
		}
	    }
	}

	if (left > 0)
	{
	    if (pct_used == 100)
	    {
		ASSERT(left < N);

		/* Round robin distribute single pixels until finished */
		for (x = 0; left > 0 && x < max; x++)
		    if ( table->colspans[x].flags & colspan_flag_PERCENT )
		    {
			table->colspans[x].width[slot] += 1;
			left -= 1;
		    }
		ASSERT(left == 0);
	    }
	    else
	    {
#if 0
		/* not elegant, but it's late */
		while (left > 0)
		    for (x = 0; left > 0 && x < max; x++)
			if ( (table->colspans[x].flags & colspan_flag_PERCENT) == 0 )
			{
			    table->colspans[x].width[slot] += 1;
			    left -= 1;
			}
#endif
	    }
	}
#endif

	/*widest += TABLE_OUTSIDE_BIAS(table);*/

	/* We now know the width the table table should be for the
	   current contents and percentage specifications. */
	FMTDBG(("Table has implied %s width %d\n", WIDTH_NAMES[slot], widest));

	/*widths[slot] = widest;*/
    }

    FMTDBG(("largest_implied_table_width: %d\n", widest));

    /*colspan_trace_cells(table, horiz);*/


    return widest;
}

/*****************************************************************************

  Force a total size change or +1 or -1 (the bias parameter) to the
  table, supplying the expected resulting size. This is a potentially
  expensive operation, as we might have to alter each group.

  We operate by running though each column with a group, L to R, and
  alter any PERCENT_THIS widths by the bias factor. Eventually, this
  should hit a point where a maximumal constraint gets lowered and the
  size changes. We also alter any such column header, just in case. We
  are careful about -1 (NOTINIT), 0 (INAVCTIVE) and 1 (DON'T GO LOWER
  THAN) and their meanings when we start altering them.

  @@@@ FIXME: There is probably a case that causes us to spin: A table
  where all width contributions come from percentages, where each such
  width is a group, each percentage is 1% and there are about target
  of them. Only another browser hacker is going to have a page like
  this, so delay fixing.

 */

#if 0
static int tiresome_pct_forced_change(rid_table_item *table, BOOL horiz, int bias, int target)
{
    const width_array_e max = HORIZMAX(table, horiz);
    const int bias2 = bias == 1 ? 0 : 1;
    int total = 0;
    int x;

    ASSERT(bias == -1 || bias == 1);

    FMTDBG(("tiresome_pct_forced_change(dp %d, id %d, %s) bias=%d, target=%d\n",
	    table->depth, table->idnum, HORIZVERT(horiz), bias, target));
    for (x = 0; x < max; x++)
    {
	pcp_cell cell = &table->colspans[x];
	pcp_group grp = cell->first_start;

	if (cell->flags & colspan_flag_PERCENT_THIS)
	    if (cell->width[PCT_RAW] > bias2)
		cell->width[PCT_RAW] += bias;

	if (grp != NULL)
	{
	    do
	    {
		if (grp->flags & colspan_flag_PERCENT_THIS)
		    if (grp->width[PCT_RAW] > bias2 )
			grp->width[PCT_RAW] += bias;
		grp = grp->next_start;
	    } while (grp != NULL);
	}

	total = pct_raw_recalc(table, horiz);

	if (total == target)
	    break;

	ASSERT(total == target - 1 || total == target + 1);
    }

    ASSERT(total == target);

    return total;
}
#endif
/*****************************************************************************

  Normalise the percentage specifications.

  We are working with smallish integers here, so care is
  required on the rounding.

  Obviously, 99% and 101% are easy things to achieve with an automatic
  tables generator and a lack of care over generating the integer
  specifications, so we ought to be perfectly happy to add or remove a
  single point from the (first encountered?) largest percentage
  specification.

  Larger contradictions can easily result from user carelessness.

  Ideally, we should be manipulating the entire cells and grids
  structure. For simplicity, we take the attitude that once we have
  done the initial distribution from groups to individual columns we
  work only with these seperate columns. This is not as good a
  solution as working with the groups (especially where integer
  rounding is concerned) but it is simpler. We can always refine the
  approach in the future if we need to.

  If there are columns that do not receive a percentage contribution
  we operate in a restricted alteration mode and only correct when the
  total percentage is >= 100% (note the equals case!)

  Otherwise:

  While the total is less than 50, we keep doubling. This avoids some
  problems with small integers.

  A scaling factor is calculated and applied. This will get close to
  100%, but small integers mean it could deviate by 5% very easily.

  If the total is not 100, we just adding/subtracting round robin
  until we reach 100%.

  Return the total %age used. If this is 100%, then there are only
  columns with %ages. If less than 100%, some columns don't have %ages
  and extra work is needed (greater than 100% is an error). This
  knowledge exists elsewhere: largest_implied_table_width for
  example. This knowledge is exported in the table flags as
  rid_tf_NON_PCT_COLS. The user might specify WIDTH=0% - we consider
  this to be silly and ignore such cases - thus a return value of zero
  means (effectively) no percentages.

  */

static int normalise_percentages(rid_table_item *table, BOOL horiz)
{
    BOOL non_percent_column = FALSE;
    int total;
    int max = (horiz ? table->cells.x : table->cells.y), x;
    int *master = HORIZCELLS(table,horiz);
    int n_components;
    BOOL pct_con = FALSE;

    FMTDBGN(("normalise_percentages(%p %s): starting\n",
	    table, HORIZVERT(horiz)));

    n_components =  find_connected_components (table, colspan_flag_ABSOLUTE_COL, colspan_flag_ABSOLUTE_GROUP, horiz);
    FMTDBG(("%d absolute components\n", n_components));
    /*csg_examine(table->colspans, max);*/
    csg_find_abs_floaters(table, horiz);
    n_components =  find_connected_components (table, colspan_flag_PERCENT_COL, colspan_flag_PERCENT_GROUP, horiz);
    pct_con = (n_components < 2);
    FMTDBG (("normalise_percentages: n_components %d\n", n_components));


    /* Initialise column header locations without a percentage
       contribution */
    colspan_column_and_eql_set(table, horiz, PCT_RAW,
			       colspan_flag_PERCENT_COL, 0, 0);

    /* Generate a division of the groups into columns (L/R)*/
    colspan_algorithm(table, PCT_RAW, horiz);

    /* Gets widths based on leftmost data set */
    colspan_column_init_from_leftmost(table, PCT_RAW, horiz);


    /*colspan_trace_cells(table, horiz);*/

    /* Zero contributions from percent groups */
    /*colspan_all_and_eql_set(table, horiz, PCT_RAW,
			    colspan_flag_PERCENT_GROUP, colspan_flag_PERCENT_GROUP, 0);*/

    /*colspan_trace_cells(table, horiz);*/

    /* Mark all column headers with percentages as having a direct contribution */
    colspan_column_and_eql_bitset(table, horiz, PCT_RAW,
				  colspan_flag_PERCENT, colspan_flag_PERCENT, colspan_flag_PERCENT_COL);


    /*colspan_trace_cells(table, horiz);*/
    /* Clear the flags for percent groups */
    colspan_all_and_eql_bitclr(table, horiz, PCT_RAW,
			       colspan_flag_PERCENT_GROUP, colspan_flag_PERCENT_GROUP, colspan_flag_PERCENT_GROUP);


    /*colspan_trace_cells(table, horiz);*/
    /* Zero contributions that are not from percent columns */
    colspan_all_and_eql_set(table, horiz, PCT_RAW,
			    colspan_flag_PERCENT_COL, 0, 0);



    colspan_trace_cells(table, horiz);
    /* At this point, all the percent group values are zero and they
       have had their flags removed. An arbitary (leftmost in this
       case) crystallisation of the group has been chosen and
       reflected into the column headers covered. All column with
       percent contributions now flag themselves as being a percent
       constraint, even if initially they were only covered by a
       percent group. All entries that are not column percents have
       been zeroed. */

    total = colspan_sum_columns(table, horiz, PCT_RAW);

    if (total == 0)
    {
	FMTDBG(("Total percentage is zero - doing nothing more\n"));
	table->flags |= rid_tf_NON_PCT_COLS;
    }
    else
    {
	/*colspan_trace_cells(table, horiz);*/

	/* Are there any cells that do not have some percentage
	   contribution? */
	for (x = 0; x < max; x++)
	{
	    if ( (table->colspans[x].flags & colspan_flag_PERCENT) == 0 )
	    {
		table->flags |= rid_tf_NON_PCT_COLS;
		non_percent_column = TRUE;
		break;
	    }
	}

	/* We can now decide what our expectation of the total value
	   is and what sort of corrections we will consider
	   applying. */
	if (/*non_percent_column*/!pct_con)
	{
	    /* Expect there to be some percentage points left for the
	       entiries that exist and do not have percentage
	       specifications. */

	    if (total == 100)
	    {
		/* Be real crude - just halve so 50% not accounted for by
		   user percentages. If this isn't what they intended,
		   TOUGH. */
		FMTDBGN(("100%% specified but other columns to account for!\n"));
		colspan_column_and_eql_halve(table, horiz, PCT_RAW,
					     colspan_flag_PERCENT,
					     colspan_flag_PERCENT);
		total = colspan_sum_columns(table, horiz, PCT_RAW);
		FMTDBGN(("100%% reduced to %d%% through a halving operation\n", total));
		/* Should be 50%, but might get rounding artifacts? */
	    }
	    else if (total > 100)
	    {
		FMTDBGN(("%d%% specified but other columns to account for!\n", total));
		while (total >= 100)
		{
		    colspan_column_and_eql_halve(table, horiz, PCT_RAW,
					      colspan_flag_PERCENT,
					      colspan_flag_PERCENT);
		    total = colspan_sum_columns(table, horiz, PCT_RAW);
		    FMTDBGN(("Reduced to %d%%\n", total));
		}
	    }
	    /* else total < 100, which is just fine */

	    FMTDBGN(("normalise_percentages: non-%%age cells (now) account for %d%%\n", total));
	}
	else if (total != 100)
	{
	    double scale;		/* NB double so as to minimise errors */

	    /* All entries have percentage and not 100%, so need to scale
	       until we do have 100% in total. */

	    while (total <= 50)
	    {
		colspan_column_and_eql_double(table, horiz, PCT_RAW,
					   colspan_flag_PERCENT,
					   colspan_flag_PERCENT);
		total = colspan_sum_columns(table, horiz, PCT_RAW);
	    }

	    while (total > 100)
	    {
		colspan_column_and_eql_halve(table, horiz, PCT_RAW,
					  colspan_flag_PERCENT,
					  colspan_flag_PERCENT);
		total = colspan_sum_columns(table, horiz, PCT_RAW);
	    }

	    ASSERT(total != 0);
	    scale = 100.0 / total;

	    FMTDBGN(("Chosen scaling factor of %g for total specified %d\n", scale, total));

	    colspan_column_and_eql_scale(table, horiz, PCT_RAW,
				      colspan_flag_PERCENT,
				      colspan_flag_PERCENT,
				      scale);
	    total = colspan_sum_columns(table, horiz, PCT_RAW);

	    FMTDBGN(("Rescaled to a total of %d%%\n", total));

	    ASSERT(total != 0);

	    /* Simply tweaking one column header might not be enough
	       to decrease sufficient constraints to lower the table
	       size by one. Decreasing all groups will lower the
	       constraints by at least one, but it could lower it by
	       the number of groups as well! We attempt a quick
	       inc/dec and resort to a more tiresome iterative
	       approach if this fails.

	     */

	    if (total < 100)
	    {
		colspan_column_and_eql_upwards(table, horiz, PCT_RAW,
					       colspan_flag_PERCENT,
					       colspan_flag_PERCENT,
					       100 - total);
		total = colspan_sum_columns(table, horiz, PCT_RAW);
	    }
	    else if (total > 100)
	    {
		colspan_column_and_eql_downwards(table, horiz, PCT_RAW,
						 colspan_flag_PERCENT,
						 colspan_flag_PERCENT,
						 total - 100);
		total = colspan_sum_columns(table, horiz, PCT_RAW);
	    }

	    ASSERT(total == 100);

	    /* We have now normalised the percentages and they total
	       100. Phew! */

#if DEBUG
	    for (total = x = 0; x < max; x++)
	    {
		total += table->colspans[x].width[PCT_RAW];
	    }

	    ASSERT(total == 100);
#endif

	    total = 100;
	}

	colspan_trace_cells(table, horiz);
    }

    master[PCT_RAW] = total;

    /* We have now chosen some basic percentages for each column. We
       are not going to change our mind over these values
       again. However, we might need to increase minwidth. */

    FMTDBG(("normalise_percentages: finished with total %d%%\n", total));

    return total;
}

/*****************************************************************************/

static void calc_pct_minwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL horiz)

    /* First, calculate the total percentage the table thinks it has
       simply by adding up the numbers in the cells using bogstandard
       colspan.

       If all base columns have (or are covered by a spanning cell
       which has) a percentage specification, then either the answer
       is 100 or we simply scale all the numbers so that it becomes
       so. => time to write a generic 'colspan_scale_widths' function?!

       If, on the other hand, there are one or more base columns
       without a percentage constraint, then the total percentage from
       those columns which *do* have one set could be < 100% (and
       probably should be!). If it turns out to be >= 100% it is
       unclear what we should do - either scale somehow (in proportion
       to the raw minwidths? in proportion to the number of
       constrained vs unconstrained columns? or just give 'em 50% and
       let 'em hang?) or just ignore all the percentage constraints.

    * /        int width = colspan_algorithm(table, PCT_MIN, horiz); / *

       after we have sorted out the scaling of percentages, we need to
       do a simple pass through the whole table to find the maximum
       value over all cells with a set (nonzero) and normalised
       percentage of the width the whole table would have to have if
       that cell were rendered at its rawmin *and* it took exactly the
       given percentage - ie max of (100 * my_raw_width / my_percent)

       we store this number in table->width[PCT_RAW] for later, even
       though it's an absolute distance and for the moment the values
       in the pcp cells and groups are still percentages (normalised
       if neccessary).

       another niggle to think about: if not all columns have
       percentage constraints and thus the total percentage width is
       less than 100%, we really want to apply the same calculation to
       100 * (total width of those columns) / (total percentage left
       over) but this gets real difficult when there are spanning
       cells and it is actually going to take REAL ORIGINAL THOUGHT to
       work out how to adapt colspan techniques to doing this in the
       general case (it will probably require a variant of the
       share_out_space technique working on the raw percentages, plus
       a good dose of adhocery).  */

/* not yet - can do this only after table width has been multiplied through
   colspan_copy_if_smaller(table, ABS_MIN, PCT_MIN, horiz);
   table->width[PCT_MIN] = width + (2 * table->border) + table->cellspacing;
    */

{
    int width;
    int *widths = HORIZCELLS(table,horiz);

    FMTDBG(("calc_pct_minwidth(%p %p %p %s)\n",
	    doc, rh, table, HORIZVERT(horiz)));

    /* First, we want the %ages set to something sensible */
    (void) normalise_percentages(table, horiz);

    /* Pick up what absolute information we have. This should be a
       basic copy operation as we don't initialise PCT_MIN to
       anything. Remember, we are going to only be using individual
       column values from now on, as I can't wrap my head around doing
       all this with groups yet. */

    colspan_column_and_eql_copy(table, horiz, PCT_MIN,
				0, 0, ABS_MIN);

    width = largest_implied_table_width(table, PCT_MIN, horiz);

    width += TABLE_OUTSIDE_BIAS(table);

    if (width < widths[ABS_MIN])
    {
	FMTDBG(("Latching PCT_MIN (%d) so that it is not smaller than ABS_MIN (%d)\n",
		width, widths[ABS_MIN]));
	width = widths[ABS_MIN];
    }

    widths[PCT_MIN] = width;

    FMTDBG(("calc_pct_minwidth: %d\n", width));
}

/*****************************************************************************

  Calculate the maximum width we can occupy without adding whitespace
  when both absolute and percentage specifications are
  considered. This must be at least ABS_MAX.

  For each column with a percentage constraint, determine with maximum
  width it would occupy without that constraint and use this value to
  calculate an implied table size. The PCT_MAX value is then the
  largest implied table size.

  calc_pct_minwidth must be called before calc_pct_maxwidth, as the
  former is where the working column percentage valuesare obtained
  from. We attempt to assert this.

  */

static void calc_pct_maxwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL horiz)
{
    int width;
    int *widths = HORIZCELLS(table,horiz);

    colspan_column_and_eql_copy(table, horiz, PCT_MAX,
				0, 0, ABS_MAX);

    width = largest_implied_table_width(table, PCT_MAX, horiz);

    width += TABLE_OUTSIDE_BIAS(table);

    if (width < widths[ABS_MAX])
    {
	FMTDBG(("Latching PCT_MAX (%d) so that it is not smaller than ABS_MAX (%d)\n",
		width, widths[ABS_MAX]));
	width = widths[ABS_MAX];
    }

    widths[PCT_MAX] = width;

    FMTDBG(("calc_pct_maxwidth: %d\n", width));
}

/*****************************************************************************/

static void calc_rel_minwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL  horiz)
{
    int *widths = (horiz ? table->hwidth : table->vwidth);

    /* Bulk copy PCT_MIN for now */
    colspan_all_and_eql_copy(table, horiz, REL_MIN,
			     0, 0, PCT_MIN);

    /* Something else obviously needs to go here! */
#if 0
    /* and apply colspan algorithm */
    width = colspan_algorithm(table, REL_MIN, horiz);

    widths [REL_MIN] = width + TABLE_OUTSIDE_BIAS(table);
#endif

    widths [REL_MIN] = widths[PCT_MIN];

    FMTDBG(("calc_rel_minwidth: %d\n", widths[REL_MIN]));
}

/*****************************************************************************/

static void calc_rel_maxwidth(antweb_doc *doc,
			      rid_header *rh,
			      rid_table_item *table,
			      BOOL  horiz)
{
    int *widths = (horiz ? table->hwidth : table->vwidth);

    /* Bulk copy PCT_MAX for now */
    colspan_all_and_eql_copy(table, horiz, REL_MAX,
			     0, 0, PCT_MAX);

    /* Something else obviously needs to go here! */
#if 0
    /* and apply colspan algorithm */
    width = colspan_algorithm(table, REL_MAX, horiz);

    widths [REL_MAX] = width + TABLE_OUTSIDE_BIAS(table);
#endif
    widths [REL_MAX] = widths[PCT_MAX];

    FMTDBG(("calc_rel_maxwidth: %d\n", widths[REL_MAX]));
}

/*****************************************************************************/

static void basic_size_table(antweb_doc *doc,
			     rid_header *rh,
			     rid_table_item *table,
			     int depth)
{
    int x, y;
    rid_table_cell *cell;
    int uwidth;

    FMTDBG(("basic_size_table(%p %p id %d): recurse down doing basic sizing\n", doc, rh, table->idnum));

    if (table->caption != NULL)
	basic_size_stream(doc, rh, &table->caption->stream, depth);

    for (x  = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	basic_size_stream(doc, rh, &cell->stream, depth);

    /* All descendent streams have been sized and raw_minwidth and
       raw_maxwidth calculated for them. Initialise the colspan data
       structure and then calculate some other pieces of
       information. */

    FMTDBG(("basic_size_table: id %d: now do colspan calculations\n", table->idnum));

    colspan_init_structure(table, HORIZONTALLY);

    for (x = 0; x < table->cells.x; x++)
	if (table->colspans[x].width[RAW_MIN] == NOTINIT)
	    table->colspans[x].width[RAW_MIN] = table->colspans[x].width[RAW_MAX] = 0;

    /* Cell borders */
    colspan_bias(table, TABLE_INSIDE_BIAS(table), HORIZONTALLY);

    calc_raw_minwidth(doc, rh, table, HORIZONTALLY);
    calc_raw_maxwidth(doc, rh, table, HORIZONTALLY);

    calc_abs_minwidth(doc, rh, table, HORIZONTALLY);
    calc_pct_minwidth(doc, rh, table, HORIZONTALLY);
    calc_rel_minwidth(doc, rh, table, HORIZONTALLY);

    calc_abs_maxwidth(doc, rh, table, HORIZONTALLY);
    calc_pct_maxwidth(doc, rh, table, HORIZONTALLY);
    calc_rel_maxwidth(doc, rh, table, HORIZONTALLY);

    FMTDBG(("basic_size_table: id %d: done\n", table->idnum));

    /* Attempt to override sizes with user width, if it exists */
#if 1
    switch (table->userwidth.type)
    {
    case value_absunit:
	uwidth = ceil(table->userwidth.u.f);
	if (uwidth >= table->hwidth[RAW_MIN])
	{
	    FMTDBG(("User width of %d being applied (RAW_MIN=%d)\n", uwidth, table->hwidth[RAW_MIN]));
	    /* Override ALL previously calculated values. */
#if 0
	    table->hwidth[RAW_MIN] =
		table->hwidth[ABS_MIN] =
		table->hwidth[PCT_MIN] =
		table->hwidth[REL_MIN] =
		table->hwidth[RAW_MAX] =
		table->hwidth[ABS_MAX] =
		table->hwidth[PCT_MAX] =
		table->hwidth[REL_MAX] = uwidth;
#endif
	    /* BLUNT! */
	    table->flags |= rid_tf_HAVE_WIDTH;
	}
	else
	{
	    FMTDBG(("User width %d smaller than RAW_MIN %d: ignoring\n", uwidth, table->hwidth[RAW_MIN]));
	}
	break;
    }
#endif
    colspan_trace_cells(table, HORIZONTALLY);

    format_width_checking_assertions(table, HORIZONTALLY);

    /* notice we don't free the colspan structure here, as we need it during
     * the "real" format recursion
     */
}

/*****************************************************************************

  The recursive width allocator.  table is the table we are recursing
  through, assigning widths to each cell within the stream. Once we
  have done this, we recurse through each nested stream (ie each cell
  and the caption) with their chosen fwidth value.

  The fwidth supplied is the space we have available to us - ie the
  width the stream WILL be formatted to. If we exceed this and we are
  not an outermost table, then we have got things wrong. The width is
  the margin distance and hence INCLUDES borders etc.


  */

extern void oimage_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);
extern void oinput_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);
extern void oobject_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);

static void allocate_widths_table(antweb_doc *doc,
				  rid_header *rh,
				  rid_table_item *table,
				  int fwidth)
{
    int x, y;
    rid_table_cell *cell;

    FMTDBG(("allocate_widths_table(%p %p %p %d): depth %d, id %d\n",
	    doc, rh, table, fwidth, table->depth, table->idnum));

    colspan_share_extra_space (table, fwidth, HORIZONTALLY);

    /* now run down stream recursing to nested tables */

    if (table->caption != NULL)
    {
	allocate_widths_stream(doc,
			       rh,
			       &table->caption->stream,
			       fwidth - TABLE_OUTSIDE_BIAS(table));
    }

    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {
	rid_text_item *ti;

	for (ti = cell->stream.text_list; ti != NULL; ti = rid_scanf(ti))
	{
	    if (ti->tag == rid_tag_TABLE)
	    {
		allocate_widths_stream(doc,
				       rh,
				       &cell->stream,
				       cell->/*size.x*/stream.fwidth );
	    }
#if 1
	    /* SJM: set sizes for images where the fwidth is */
	    if (ti->tag == rid_tag_IMAGE)
	    {
		rid_text_item_image *tii = (rid_text_item_image *)ti;
		if (tii->ww.type == value_pcunit)
		{
		    oimage_size_allocate(ti, rh, doc, cell->stream.fwidth);
		}
	    }

	    if (ti->tag == rid_tag_INPUT)
	    {
		rid_input_item *ii = ((rid_text_item_input *)ti)->input;
		if (ii->tag == rid_it_IMAGE && ii->ww.type == value_pcunit)
		{
		    oinput_size_allocate(ti, rh, doc, cell->stream.fwidth);
		}
	    }

	    if (ti->tag == rid_tag_OBJECT)
	    {
		rid_object_item *obj = ((rid_text_item_object *)ti)->object;
		if (obj->userwidth.type == value_pcunit)
		{
		    oobject_size_allocate(ti, rh, doc, cell->stream.fwidth);
		}
	    }
#endif
	}
    }
}

/*****************************************************************************

  Top level routine. We have decide what width we are formatting
  within.  For the top level, a special rule applies. For all nested
  levels, the fwidth will not be less than the relevant minwidth. For
  the toplevel, we will have received the window width and might end
  up with something wider than this. We need to know the window width
  so that WIDTH/HEIGHT=50% on topmost table gets half of the right
  thing, etc. All nested tables requried final widths percolating
  downwards.

  This is also the time when we might be considering <IMG WIDTH=50%>
  type elements.

  The fwidth value is the full margin distance.

  */

extern void oimage_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);
extern void oinput_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);
extern void oobject_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);

static void allocate_widths_stream(antweb_doc *doc,
				   rid_header *rh,
				   rid_text_stream *stream,
				   int fwidth)
{
    rid_text_item *ti;

    FMTDBG(("allocate_widths_stream(%p %p %p %d): fwidth was %d\n",
	    doc, rh, stream, fwidth, stream->fwidth));

    stream->fwidth = fwidth;

    for (ti = stream->text_list; ti != NULL; ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;
	    allocate_widths_table(doc, rh, table, fwidth);
	}
#if 1
	/* SJM: set sizes for images where the fwidth is */
	if (ti->tag == rid_tag_IMAGE)
	{
	    rid_text_item_image *tii = (rid_text_item_image *)ti;
	    if (tii->ww.type == value_pcunit)
	    {
		oimage_size_allocate(ti, rh, doc, fwidth);
	    }
	}

	if (ti->tag == rid_tag_INPUT)
	{
	    rid_input_item *ii = ((rid_text_item_input *)ti)->input;
	    if (ii->tag == rid_it_IMAGE && ii->ww.type == value_pcunit)
	    {
		oinput_size_allocate(ti, rh, doc, fwidth);
	    }
	}

	if (ti->tag == rid_tag_OBJECT)
	{
	    rid_object_item *obj = ((rid_text_item_object *)ti)->object;
	    if (obj->userwidth.type == value_pcunit)
	    {
		oobject_size_allocate(ti, rh, doc, fwidth);
	    }
	}
#endif
    }
}

static void allocate_widths(antweb_doc *doc,
			    rid_header *rh,
			    rid_text_stream *stream,
			    int fwidth)
{
    FMTDBG(("\nallocate_widths(%p %p %p %d): choosing widths for table elements\n\n",
	    doc, rh, stream, fwidth));

    allocate_widths_stream(doc, rh, stream, fwidth);

    FMTDBG(("\nallocate_widths: finished\n\n"));
}

/*****************************************************************************

  Each stream object has a final fwidth value. We need to format to
  this size, including all streams below us, and pick up height
  information having formatted.

  We are expected to generated height information from this formatting
  so that the stream containing the table knows the height of each
  object, and hence where the next line begins. This means we have to
  pick up height information on the upwards pass of the recursion. Of
  course, once we have the heights of individual cells in a table, we
  use the colspan algorithm to distribute these.

  Note that we are stomping on the horizontal colspan data with the
  vertical colspan data. This is another reason to work on the upwards
  pass of the recursion!

  For now, we ignore percentages. This means we get an actual height
  from formatting the stream, optional absolute height for a cell and
  optional absolute height for the table. Unless there are
  contradictions, we always want to obey all the user
  constraints. When we take note of percentages, we can pick up
  minimum size information on the upwards pass, but will need another
  downwards pass to correctly relate this to the height that is being
  measured against - even although I'm not certain what this height
  actually is for some nested table examples.

  */

static void recurse_format_table(antweb_doc *doc,
				 rid_header *rh,
				 rid_table_item *table,
				 int depth)
{
    rid_text_item *orig_item;
    int x,y;
    rid_table_cell *cell;
    int height;

    FMTDBG(("recurse_format_table(%p %p id=%d): depth %d, entered\n", doc, rh, table->idnum, depth));

    /* Format all the descendents below us */

    if (table->caption != NULL)
	recurse_format_stream(doc, rh, &table->caption->stream, depth);

    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {
	VALUE v;
	int z;

	recurse_format_stream(doc, rh, &cell->stream, depth);

	rid_getprop(table, x, y, rid_PROP_HEIGHT, &v);

	switch (v.type)
	{
	case value_absunit:
	    z = - ceil(v.u.f);
	    if (z < cell->stream.height)
		cell->stream.height = z;
	    break;
	}
    }

    /*  All descendent cells/streams now have heights. These heights
        can only be grown, and then for one of three reasons: 1)
        Because other cells in the same row are taller. 2) Meet user
        absolute and percentage height requests. 3) Because of
        ROWSPAN=N implications. The colspan algorithm will resolve all
        of these readily.

	Having chosen final height values, we can handle
	top/middle/bottom alignment position.

	*/

    FMTDBG(("\nrecurse_format_table: depth %d, now getting vertical information\n\n", depth));

    /* This is wasteful - the shape remains the same! */
    colspan_free_structure(table, HORIZONTALLY);
    colspan_init_structure(table, VERTICALLY);

    for (x = 0; x < table->cells.y; x++)
	if (table->colspans[x].width[RAW_MIN] == NOTINIT)
	    table->colspans[x].width[RAW_MIN] = table->colspans[x].width[RAW_MAX] = 0;

    /* Borders for each cell */
    colspan_bias(table, TABLE_INSIDE_BIAS(table), VERTICALLY);

    calc_raw_minwidth(doc, rh, table, VERTICALLY);
    calc_raw_maxwidth(doc, rh, table, VERTICALLY);

    calc_abs_minwidth(doc, rh, table, VERTICALLY);
    calc_pct_minwidth(doc, rh, table, VERTICALLY);
    calc_rel_minwidth(doc, rh, table, VERTICALLY);

    calc_abs_maxwidth(doc, rh, table, VERTICALLY);
    calc_pct_maxwidth(doc, rh, table, VERTICALLY);
    calc_rel_maxwidth(doc, rh, table, VERTICALLY);

    FMTDBG(("Have worked out height information for table id=%d, depth %d\n",
	    table->idnum, depth));

    /*colspan_trace_cells(table, VERTICALLY);*/

    for (x = 0; x < N_COLSPAN_WIDTHS; x++)
    {
	FMTDBG(("recurse_format_table: %s = %d\n", WIDTH_NAMES[x], table->vwidth[x]));
    }

    format_width_checking_assertions(table, VERTICALLY);

    /* Format so we have just enough room */
    height = table->vwidth[REL_MIN];
    colspan_share_extra_space (table, height, VERTICALLY);

    orig_item = &table->parent->base;
    orig_item->max_up = table->size.y;
    orig_item->max_down = 0;
    orig_item->width = table->size.x;
    orig_item->pad = 0;

    /* Perform vertical and horizontal positioning of cells within table */
    /*format_position_table_cells(table);*/

    FMTDBG(("recurse_format_table: Chosen size %d,%d for table id=%d at depth %d\n", 
	    table->size.x, table->size.y, table->idnum, depth));

    /* pdh: here? */
    colspan_free_structure( table, VERTICALLY );
}

/*****************************************************************************

  Format a text stream to the fwidth given in it. This is the final format
  and must always yield rid_pos_item chains, whatever the other format
  operations do. If there are any nested streams, they are recursively
  formatted as well.

 */

static void recurse_format_stream(antweb_doc *doc,
				  rid_header *rh,
				  rid_text_stream *stream,
				  int depth)
{
    rid_text_item *ti;

    FMTDBGN(("recurse_format_stream: depth %d, recursing on any descendent streams first\n", depth));

    /* FIRST any descendent levels */
    for (ti = stream->text_list; ti != NULL; ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_table_item *table = ((rid_text_item_table *)ti)->table;

	    recurse_format_table(doc, rh, table, depth+1);
	}
    }

    FMTDBGN(("recurse_format_stream: finished recursing on any descendent streams\n"));

    /* Then we can consider formatting this level */
    FMTDBGN(("recurse_format_stream: now formatting depth %d stream\n", depth));

    /* This level */
    format_stream(doc, rh, stream, MAYBE, depth);
}

/*****************************************************************************

  This version of the formatter does not take progressive hints. We throw
  all previous partial formats away.



 */

extern void rid_toplevel_format(antweb_doc *doc,
				rid_header *rh,
				rid_text_item *start_from,
				int fwidth,
				int fheight)
{
    FMTDBG(("\n\n\n\n\n\nrid_toplevel_format(%p) entered\n\n", rh));

    /*dump_header(rh);*/

    format_one_off_assertions();

    ASSERT(rh != NULL);
    ASSERT(doc != NULL);

    /* And floating images. */

    stomp_captions_until_working(rh);

    format_precondition(rh);

    if (rh->stream.text_list == NULL)
    {
	FMTDBG(("Document empty\n"));
	rh->stream.fwidth = fwidth;
	rh->stream.width = 0;
	rh->stream.widest = 0;
	rh->stream.height = 0;
    }
    else
    {
	FMTDBG(("Sizing root stream\n"));
	basic_size_stream(doc, rh, &rh->stream, 0);
	FMTDBG(("\nDone sizing root stream:\n"));
	/*dump_header(rh);*/

	FMTDBG(("Allocating widths to root stream\n"));
	allocate_widths(doc, rh, &rh->stream, fwidth);
	FMTDBG(("\nDone allocating widths to root stream: fwidth %d, width %d, widest %d\n",
		rh->stream.fwidth, rh->stream.width, rh->stream.widest));
	/*dump_header(rh);*/

	if (rh->stream.widest > fwidth)
	{
	    FMTDBG(("\n\nCASE WHERE AUTOFIT MIGHT DO SOMETHING BETTER\n\n\n"));
/* 	    rh->stream.fwidth = rh->stream.widest; */
 	    rh->stream.fwidth = rh->stream.width_info.minwidth;
	}

	FMTDBG(("Formatting root stream with fwidth %d\n", rh->stream.fwidth));
	recurse_format_stream(doc, rh, &rh->stream, 0);
	FMTDBG(("\nDone formatting root stream with fwidth %d:\n", rh->stream.fwidth));

#if 0
	dump_header(rh);
#endif
    }

    FMTDBG(("rid_toplevel_format: finished\n"));

    format_postcondition(rh);

    return;
}

/* eof format.c */
