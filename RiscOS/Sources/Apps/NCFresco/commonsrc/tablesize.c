/* tablesize.c - Table sizing */

#include <math.h>
#include <float.h>
#include "rid.h"
#include "htmlparser.h"
#include "tables.h"
#include "util.h"
#include "webfonts.h"
#include "indent.h"


static void rid_table_accumulate_widths_l2r(int cellsx, int *sq, int *cum);
static void rid_table_accumulate_widths_r2l(int cellsx, int *sq, int *cum);
static void rid_close_fixed_widths(int cellsx, int *sqfixed, int overwrite);

#if DEBUG && 0
static void dump_sq(char *title, int *ptr, int x)
{
    int i,j;
    fprintf(stderr, "\n%s (%dx%d)\n", title, x, x);
    for (i = 0; i < x; i++)
    {
	fprintf(stderr, "%4d: ", i);
	for (j = 0; j < x; j++)
	{
	    fprintf(stderr, "%4d ", ptr[i * x + j]);
	}
	fprintf(stderr, "\n");
    }

    fprintf(stderr, "\n");
}
#else
#define dump_sq(title, ptr, x)
#endif

/*****************************************************************************/

#if 0
static int worst_max(int *sq, int cellsx)
{
    int i,j, w = cellsx - 1;

    for (i = cellsx - 1; i >= 0; i--)
    {
	for (j = 0; j < cellsx; j++)
	{
	    if ( sq[i + j * cellsx] != -1 )
	    {
		w = i;
		break;
	    }
	}
    }

    return w;
}
#endif


/*****************************************************************************

  New table sizing code, handling percentage and relative width based
  information.

  Originally based on an algorithm by nicko@ant.co.uk and
  peter@ant.co.uk, but mainly now due to borris@ant.co.uk, and later
  by nicko@ant.co.uk again!

  The {min,max}pct values describe where the left hand boundary goes.
  This means the first cell's {min,max}pct values are not used. This
  is easier than the right hand version, because cell->span.x does not
  affect the left hand boundary.

  Note that WIDTH=N* is not defined for the cells of tables. The HTML
  specification only requires width processing on columns, but advises
  that backward Netvirus compatibility requires absolute and percent
  widths for cells - Netvirus does not understand relative widths all
  at (as of 3.0b4), so backwards compatibility does not require
  relative width processing for individual cells. We might extend our
  code to handle this, but for now this is not permitted.

  */

static BOOL new_sizer(rid_table_item *table)
{
    const int cellsx = table->cells.x;
    const int cellsy = table->cells.y;

    BOOL rerun = FALSE;
    int *ap = mm_malloc(cellsx * sizeof(int));
    int *bp = mm_malloc(cellsx * sizeof(int));
    int x, y;
    double minc, maxc;

    TABDBG(("new_sizer(): (%d,%d) table\n", cellsx, cellsy));

    /* Choose initial absolute values */

    TABDBG(("(): min/max absolute list *before* accounting for non percent/absolute items\n"));

    PRINT_LIST(table->minabs, cellsx, "Min");
    PRINT_LIST(table->maxabs, cellsx, "Max");
    PRINT_LIST(table->cumminabs, cellsx, "CMin");
    PRINT_LIST(table->cummaxabs, cellsx, "CMax");

    for (x = 0; x < cellsx; x++)
    {
	table->minabs[x] = table->cumminabs[x+1] - table->cumminabs[x];
	table->maxabs[x] = table->cummaxabs[x+1] - table->cummaxabs[x];
    }

    TABDBG(("(): min/max absolute list after accounting for non percent/absolute items\n"));

    PRINT_LIST(table->minabs, cellsx, "Min");
    PRINT_LIST(table->maxabs, cellsx, "Max");
    PRINT_LIST(table->cumminabs, cellsx, "CMin");
    PRINT_LIST(table->cummaxabs, cellsx, "CMax");

    /* @@@@ Sort out scaling of %ages when there is more than 100% */

    for (x = 0; x < cellsx; x++)
    {
	const int a = table->minpct[x];
	const int b = table->maxpct[x];

	if (a > b)
	{
	    const int c = (a + b) / 2;
	    table->minpct[x] = c;
	    /* Don't lose 1/2 pixels */
	    table->maxpct[x] = a + b - c;
	    TABDBG(("new_sizer(): conflict resolved for column %d\n", x));
	}
    }

    TABDBG(("(): min/max percent list after resolving vertical conflicts\n"));

    PRINT_LIST(table->minpct, cellsx, "Min%");
    PRINT_LIST(table->maxpct, cellsx, "Max%");

    /* At this point we have determined the relative positions of the */
    /* columns. These same ratios must be used during formating, together */
    /* with the minimum width we are about to calculate, or else items */
    /* might not fit in their alloted space, and screen garbage will happen. */
    /* Today, we're choosing the table->minpct set. */

    if (table->caption)
    {
	minc = ((double) table->caption->stream.width_info.minwidth) / 100.0;
	maxc = ((double) table->caption->stream.width_info.maxwidth) / 100.0;
    }
    else
    {
	minc = maxc = 0;
    }

    for (y = 0; y < cellsy; y++)
    {
	double d;
	int pc_on_line = 0;
	int total_min_non_pc = 0, total_max_non_pc = 0;
	int had_empty = 0;

	for (x = 0; x < cellsx; x++)
	{
	    rid_table_cell *cell = *CELLFOR(table,x,y);
	    int q;

	    if (cell == NULL)
	    {
		TABDBGN(("Empty cell at %d,%d\n", x, y));
		had_empty = 1;
		continue;
	    }

	    if (x != cell->cell.x)
		continue;

	    if ((cell->flags & rid_cf_PERCENT) == 0)
	    {
		total_min_non_pc += table->cumminabs[x + cell->span.x] - table->cumminabs[x];
		total_max_non_pc += table->cummaxabs[x + cell->span.x] - table->cummaxabs[x];
	    }
	    else
	    {
		q = table->minpct[x + cell->span.x] - table->minpct[x];
		/* SJM: we have been getting 0 here */
		if (q <= 0)
		    q = 1;

		pc_on_line += q;

		d = table->cumminabs[x + cell->span.x] - table->cumminabs[x];
		d /= (double)q;
		if (d > minc)
		    minc = d;

		d = table->cummaxabs[x + cell->span.x] - table->cummaxabs[x];
		d /= (double)q;
		if (d > maxc)
		    maxc = d;
	    }
	}

	TABDBGN(("Line %d: %d%% on line, had_empty=%d\n", y, pc_on_line, had_empty));

	if (!had_empty && (pc_on_line < 100))
	{
	    double mpc = 100.0 - pc_on_line;

	    TABDBGN(("%d%% remaining on line, min %d, max %d outside percentages\n",
		     100 - pc_on_line, total_min_non_pc, total_max_non_pc));

	    d = (double) total_min_non_pc / mpc;
	    if (d > minc)
		minc = d;

	    d = (double) total_max_non_pc / mpc;
	    if (d > maxc)
		maxc = d;
	}
	else if ( (pc_on_line > 100) ||
		  (pc_on_line == 100 && (total_min_non_pc + total_max_non_pc) > 0 ) )
	{
	    TABDBG(("new_sizer(): decided must rerun sizing, with %%ages suppressed: line %d\n", y));
	    for (x = 0; x < cellsx; x++)
	    {
		rid_table_cell *cell = *CELLFOR(table,x,y);

		if (cell != NULL &&
		    x == cell->cell.x &&
		    (cell->flags & rid_cf_PERCENT) != 0 )
		{
		    /* Override %age specification */
		    TABDBG(("new_sizer(): squishing %%age for %d,%d\n", x, y));
		    cell->userwidth.type = value_void;
		}
	    }

	    rerun = TRUE;
	}
    }

    if (minc > maxc)
    {
	TABDBG(("new_sizer: conflict: increased maxc %f to minc %f\n", maxc, minc));
	maxc = minc;
    }

    TABDBG(("new_sizer(): chosen minc %f, maxc %f\n", minc, maxc));

    /* Now we can return the minimum and maximum width of the table */

    table->width_info.minwidth = (int) ((100.0 * minc) + 0.5);
    table->width_info.maxwidth = (int) ((100.0 * maxc) + 0.5);

    if (table->userwidth.type == value_absunit &&
	table->width_info.minwidth > (int) ceil(table->userwidth.u.f))
    {

	rid_table_cell *cell;
	/* @@@@ CRUDE: stomp anything specifying a horizontal width */
	/* on a per cell basis. Should really be retaining ratios   */
	/* between the column widths. 				    */

	TABDBG(("new_sizer(): abs width forcing restrictions on cells widths\n"));

	for (x = 0; x < table->num_groups.x; x++)
	{
	    if (table->colgroups[x]->userwidth.type != value_none)
	    {
		table->colgroups[x]->userwidth.type = value_none;
		rerun = TRUE;
	    }
	}

	for (x = 0; x < table->cells.x; x++)
	{
	    if (table->colhdrs[x]->userwidth.type != value_none)
	    {
		table->colhdrs[x]->userwidth.type = value_none;
		rerun = TRUE;
	    }
	}

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	{
	    if (cell->userwidth.type != value_none)
	    {
		cell->userwidth.type = value_none;
		rerun = TRUE;
	    }
	}
    }
    else
    {
	/* One cellspacing and two cellpaddings have already been allowed */
	/* for each cell. Add in the table frame and the very RHS cellspacing */
	table->width_info.minwidth += table->cellspacing + 2 * table->border;
	table->width_info.maxwidth += table->cellspacing + 2 * table->border;
    }

    mm_free(ap);
    mm_free(bp);

    TABDBG(("new_sizer(): Final table minwidth %d, maxwidth %d\n",
	    table->width_info.minwidth,
	    table->width_info.maxwidth));

    /* Can fail! */
    TASSERT(table->width_info.minwidth <= table->width_info.maxwidth);

    return rerun;
}

/*****************************************************************************

  Get ready for performing a sizing pass on the table. All values must be
  reset, as we may have already partially formatted this table, and these
  values might now be very wrong.

  Row and column headers indicate whether they contain ABSOLUTE, RELATIVE
  and PERCENT items to make eliminating work on whole rows and columns
  easier.

 */

static void ready_table_for_sizing(rid_table_item *table)
{
    int x, y;
    rid_table_cell *cell;
    BOOL have_rel = FALSE;

    TABDBG(("ready_table_for_sizing()\n"));

    nullfree((void**)&table->minpct);
    nullfree((void**)&table->maxpct);
    nullfree((void**)&table->minabs);
    nullfree((void**)&table->maxabs);

    nullfree((void**)&table->cumminabs);
    nullfree((void**)&table->cummaxabs);

    table->minpct    = mm_calloc((table->cells.x+1) , sizeof(int));
    table->maxpct    = mm_calloc((table->cells.x+1) , sizeof(int));

    table->minabs    = mm_malloc(table->cells.x * sizeof(int));
    table->maxabs    = mm_malloc(table->cells.x * sizeof(int));


    /* Extra item to hold overshoot */
    table->cumminabs = mm_calloc(table->cells.x + 1,  sizeof(int));
    table->cummaxabs = mm_calloc(table->cells.x + 1,  sizeof(int));

    for (x = 0; x < table->cells.x; x++)
	table->colhdrs[x]->flags &= ~ (rid_chf_PERCENT | rid_chf_ABSOLUTE | rid_chf_RELATIVE);

    for (y = 0; y < table->cells.y; y++)
	table->rowhdrs[y]->flags &= ~ (rid_rhf_PERCENT | rid_rhf_ABSOLUTE | rid_rhf_RELATIVE);

    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {
	rid_text_stream *stream = &cell->stream;
	VALUE width;

	cell->flags &= ~ (rid_cf_PERCENT | rid_cf_ABSOLUTE | rid_cf_RELATIVE);

	rid_getprop(table, x, y, rid_PROP_WIDTH, &width);

	switch (width.type)
	{
	case value_pcunit:
	    table->colhdrs[x]->flags |= rid_chf_PERCENT;
	    table->rowhdrs[y]->flags |= rid_rhf_PERCENT;
	    cell->flags |= rid_cf_PERCENT;
	    have_rel = TRUE;
	    break;
	case value_absunit:
	    table->colhdrs[x]->flags |= rid_chf_ABSOLUTE;
	    table->rowhdrs[y]->flags |= rid_rhf_ABSOLUTE;
	    cell->flags |= rid_cf_ABSOLUTE;
	    break;
	    /* Can only happen if column specifies it? */
	case value_relunit:
	    table->colhdrs[x]->flags |= rid_chf_RELATIVE;
	    table->rowhdrs[y]->flags |= rid_rhf_RELATIVE;
	    cell->flags |= rid_cf_RELATIVE;
	    have_rel = TRUE;
	    break;
	}

	memset( &stream->width_info, 0, sizeof(stream->width_info));
    }

}

static void done_table_sizing(rid_table_item *table)
{
    /* We might free some of the intermediate lists */
    /* of data at this point. For now, this all is */
    /* done in rid_free_table(), as some of it could */
    /* turn out to be useful during formatting itself */

    TABDBG(("\n\ndone_table_sizing(): DONE TABLE SIZING\n\n"));
}

/*****************************************************************************

  Get sizes for all child items.

 */

static void size_child_items(rid_header *rh, rid_table_item *table, rid_fmt_info *parfmt)
{
    const int cellsx = table->cells.x;
    const int cb = table->cellspacing + 2 * table->cellpadding;
    int x, y;
    rid_table_cell *cell;
    rid_fmt_info fmt = *parfmt;
    int *sqpct, *sqmin, *sqmax, sq_index;

    TABDBG(("size_child_items(): entry min %d, max %d\n",
	    table->width_info.minwidth, table->width_info.maxwidth));

    x = sizeof(int) * cellsx * cellsx;

    sqpct = mm_malloc(x);
    sqmin = mm_malloc(x);
    sqmax = mm_malloc(x);

    if (x)
    {
	memset(sqpct, -1, x);
	memset(sqmin, -1, x);
	memset(sqmax, -1, x);
    }

    /* Ensure all children items have already been sized */

    if (table->caption != NULL)
    {
	/* Don't forget to ensure padding space for the caption as well */
	TABDBGN(("size_child_items(): sizing table's caption stream\n"));
	rid_size_stream(rh, &table->caption->stream, &fmt, 0, NULL);
	table->caption->stream.width_info.minwidth += cb;
	table->caption->stream.width_info.maxwidth += cb;
    }

    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {
	/* Size cell. Include any border type spacing */
	/* Might be prevented from wrapping by cell */
	/* Might be forced to minimum width by col or colgroup */
	int flags = 0, z;
	BOOL char_align;
	rid_width_info *wi = &cell->stream.width_info;
	rid_halign_tag halign;
	rid_stdunits width_units, choff_units;
	rid_table_colhdr *colhdr = table->colhdrs[x];

	TABDBGN(("\nsize_child_items(): starting to size cell (%d,%d)\n", x, y));

	rid_getprop(table, x, y, rid_PROP_HALIGN, &halign);
	rid_getprop(table, x, y, rid_PROP_WIDTH, &width_units);

	char_align = halign == STYLE_ALIGN_CHAR;

	if (char_align)
	{
	    TABDBG(("size_child_items(): cell has ALIGN=CHAR style alignment\n"));

	    rid_getprop(table, x, y, rid_PROP_CH, &fmt.align_char);
	    rid_getprop(table, x, y, rid_PROP_CHOFF, &choff_units);

	    if (choff_units.type == rid_stdunit_PX)
	    {
		wi->minleft = wi->maxleft = (int) ceil(choff_units.u.f);
		TABDBGN(("size_child_items(): cell given absolute width %d\n", wi->minleft));
	    }
	    flags |= rid_fmt_CHAR_ALIGN;
	}

	if ( (width_units.type != value_absunit) && ((cell->flags & rid_cf_NOWRAP) != 0) )
	{
	    flags |= rid_fmt_MAX_WIDTH;
	    TABDBGN(("size_child_items(): NOWRAP: forcing MAX_WIDTH\n"));
	}
	else if ( (cell->flags & (rid_cf_RELATIVE | rid_cf_PERCENT)) != 0 &&
	    width_units.u.f == 0.0 )
	{
	    flags |= rid_fmt_MIN_WIDTH;
	    TABDBGN(("size_child_items(): MIN_WIDTH due to 0%% or 0*\n"));
	}

	TABDBGN(("size_child_items(): doing the rid_size_stream()\n"));

	rid_size_stream(rh, &cell->stream, &fmt, flags, NULL);

#if DEBUG && 0
	TABDBG(("size_child_items(): rid_width_info for cell initialised to:\n"));
	dump_width_info(cell->stream.width_info);
#endif

	/* This is where we make tfoot cells during a table become */
	/* invisible. We set them to an effect cell contents width */
	/* of zero.  Spacing/padding allocation is always done, as */
	/* this can only ever stay the same or get larger.         */

	if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
	     (table->rowhdrs[y]->flags & rid_rhf_TFOOT) )
	{
	    TABDBG(("size_child_items(): cell %d,%d is currently invisible\n", x, y));
	    wi->minwidth = cb;
	    wi->maxwidth = cb;
	}
	else
	{
	    if (width_units.type != value_none)
	    {
		int width_px = 0;
		int set = 0;

		switch(width_units.type)
		{
		case value_absunit:
		    width_px = (int) ceil(width_units.u.f);
		    set = 1;
		    TABDBGN(("size_child_items(): absolute width requested %d\n", width_px));
		    break;
		case value_relunit:
		    break;
		case value_pcunit:
		    /* Save for later use */
		    /*cell->maxpct = cell->minpct = (int) ceil(width_units.u.f);*/
		    break;
		}

		if (set)
		{
		    if (wi->minwidth < width_px)
		    {
			TABDBGN(("size_child_items(): increasing minimum width from %d to %d\n",
				wi->minwidth, width_px));
			wi->minwidth = width_px;
		    }

		    TABDBGN(("size_child_items(): setting maximum width from %d to %d\n",
			    wi->maxwidth, wi->minwidth));
		    wi->maxwidth = wi->minwidth;
		}
	    }

	    TABDBGN(("size_child_items(): applying borders %d\n", cb));

	    wi->minwidth += cb;
	    wi->maxwidth += cb;

	    if (char_align)
	    {
		TABDBG(("size_child_items(): updating left/right alignment info\n"));

		if (wi->minleft > colhdr->width_info.minleft)
		{
		    TABDBG(("size_child_items(): minleft grows from %d to %d\n",
			    colhdr->width_info.minleft, wi->minleft));
		    colhdr->width_info.minleft = wi->minleft;
		}

		if (wi->maxleft > colhdr->width_info.maxleft)
		{
		    TABDBG(("size_child_items(): maxleft grows from %d to %d\n",
			    colhdr->width_info.maxleft, wi->maxleft));
		    colhdr->width_info.maxleft = wi->maxleft;
		}

		if (wi->minright > colhdr->width_info.minright)
		{
		    TABDBG(("size_child_items(): minright grows from %d to %d\n",
			    colhdr->width_info.minright, wi->minright));
		    colhdr->width_info.minright = wi->minright;
		}

		if (wi->maxright > colhdr->width_info.maxright)
		{
		    TABDBG(("size_child_items(): maxright grows from %d to %d\n",
			    colhdr->width_info.maxright, wi->maxright));
		    colhdr->width_info.maxright = wi->maxright;
		}

		/* This won't work, because a later cell might increase */
		/* wi->{min,max}{left,right} has hence invalidate the */
		/* work done here. FIXME: @@@@ XXXX */

		if ( (z = wi->minleft + wi->minright) > wi->minwidth )
		{
		    TABDBG(("size_child_items(): min left/right updating minwidth from %d to %d\n",
			    wi->minwidth, z));
		    wi->minwidth = z;
		    TABDBG(("size_child_items(): POSSIBLE ALIGN=CHAR GROWTH PROPOGATION PROBLEMS\n"));
		}

		if ( (z = wi->maxleft + wi->maxright) > wi->maxwidth )
		{
		    TABDBG(("size_child_items(): max left/right updating maxwidth from %d to %d\n",
			    wi->maxwidth, z));
		    wi->maxwidth = z;
		    TABDBG(("size_child_items(): POSSIBLE ALIGN=CHAR GROWTH PROPOGATION PROBLEMS\n"));
		}
	    }
	}

	sq_index = (x * cellsx) + cell->span.x - 1;

	if (wi->minwidth > sqmin[sq_index])
	    sqmin[sq_index] = wi->minwidth;
	if (wi->maxwidth > sqmax[sq_index])
	    sqmax[sq_index] = wi->maxwidth;

	if (width_units.type == value_pcunit)
	{
	    int pc = (int) ceil(width_units.u.f);
	    if (pc > sqpct[sq_index])
		sqpct[sq_index] = pc;
	}

	TABDBG(("size_child_items(): (%d, %d) sized as %d,%d,%d %d,%d,%d\n",
		x, y,
		wi->minleft, wi->minright, wi->minwidth,
		wi->maxleft, wi->maxright, wi->maxwidth));

	TASSERT(wi->minwidth <= wi->maxwidth);
    }


    dump_sq("Just sized min", sqmin, cellsx);
    dump_sq("Just sized max", sqmax, cellsx);
    dump_sq("Just sized pct", sqpct, cellsx);
    PRINT_LIST(table->cummaxabs, cellsx + 1, "x CMax:");


    for(x=0; x < cellsx; x++)
    {
	int worstmin = 0;
	int worstmax = 0;
	int minpc = 0;

	/* Let's just quickly force each column to have a minimum width of at least zero */
	if (sqmin[x * cellsx] == -1)
	{
	    sqmin[x *cellsx] = 0;
	}

	TABDBGN(("Finding end of col %d\n", x));

	for(y=1; y <= (x+1); y++)
	{
	    int index = ((x-(y-1))*cellsx) + (y-1);

	    TABDBGN(("Looking at cells that span %d (start col=%d), min=%d, max=%d, pc=%d\n",
		     y, x-(y-1), sqmin[index], sqmax[index], sqpct[index]));

	    if ((sqmin[index] != -1) &&
		(sqmin[index] + table->cumminabs[x-y+1] > worstmin))
		worstmin = sqmin[index] + table->cumminabs[x-y+1];

       	    if ((sqmax[index] != -1) &&
		(sqmax[index] + table->cummaxabs[x-y+1] > worstmax))
		worstmax = sqmax[index] + table->cummaxabs[x-y+1];

	    if ((sqpct[index] != -1) &&
		(sqpct[index] + table->minpct[x-y+1] > minpc))
		minpc = sqpct[index] + table->minpct[x-y+1];
	}

	if (worstmin < table->cumminabs[x])
	    worstmin = table->cumminabs[x];

	/*

	  Having this setting to -1 breaks the following example:

	  <table border=0 cellspacing=0 cellpadding=0 xwidth=100> <td
	  width=98%> <td width=10> <td width=10> </table>

	  Borris removed it 22/9/96. Can anyone say why it should go
	  back?

	  */

	/*if (worstmax == 0)
	    worstmax = -1;
	else*/ if (worstmax < table->cummaxabs[x])
	    worstmax = table->cummaxabs[x];

	if (minpc < table->minpct[x])
	    minpc = table->minpct[x];

	table->cumminabs[x+1] = worstmin;
	table->cummaxabs[x+1] = worstmax;
	table->minpct[x+1] = minpc;
    }

    TABDBG(("Point Z\n"));

    PRINT_LIST(table->cumminabs, cellsx + 1, "z CMin:");
    PRINT_LIST(table->cummaxabs, cellsx + 1, "z CMax:");
    PRINT_LIST(table->minpct, cellsx + 1, "z CPct:");

    table->maxpct[cellsx] = 100;

    for(x = cellsx-1; x >= 0; x--)
    {
	int bestpc = 100;

	TABDBGN(("Finding maxpc for start of col %d\n", x));

	for(y=1; y <= (cellsx-x); y++)
	{
	    int index = (x * cellsx) + (y-1);

	    TABDBGN(("Looking at cells that span %d (start col=%d), pc=%d\n",
		     y, x, sqpct[index]));

	    if ((sqpct[index] != -1) &&
		(table->maxpct[x+y] - sqpct[index] < bestpc))
		bestpc = table->maxpct[x+y] - sqpct[index];
	}

	if (bestpc > table->maxpct[x+1])
	    bestpc = table->maxpct[x+1];

	table->maxpct[x] = bestpc;
    }

    if (table->cummaxabs[cellsx] == -1)
	table->cummaxabs[cellsx] = table->cumminabs[cellsx];

    for(x = cellsx-1; x > 0; x--)
	if (table->cummaxabs[x] == -1)
	    table->cummaxabs[x] = table->cummaxabs[x+1];

    TABDBG(("size_child_items(): cummulative min/max abs lists are:\n"));
    PRINT_LIST(table->cumminabs, cellsx + 1, "Min:");
    PRINT_LIST(table->cummaxabs, cellsx + 1, "Max:");
    PRINT_LIST(table->minpct, cellsx + 1, "Min%:");
    PRINT_LIST(table->maxpct, cellsx + 1, "Max%:");

    mm_free(sqpct);
    mm_free(sqmin);
    mm_free(sqmax);
}

/*****************************************************************************/

extern void rid_size_table( rid_header *rh, rid_table_item *table, rid_fmt_info *parfmt )
{
    /* Iterate whilst we have %age total unacceptably large */
    /* The amount of work could be reduced, but this is at */
    /* least likely to work */

#if 0
    if (table->cells.x == 0 && table->cells.y == 0) /* try and track down heap corruption */
    {
	ready_table_for_sizing(table);
	table->minpct[0] = 0;
	table->maxpct[0] = 100;
	table->cumminabs[0] = 0;
	table->cummaxabs[0] = 0;
    }
    else do
    {
	TABDBG(("\n\nrid_size_table(%p, %p)\n", table, parfmt));

	ready_table_for_sizing(table);
	size_child_items(table, parfmt);
    } while ( new_sizer(table) );
#else
    do
    {
	TABDBG(("\n\nrid_size_table(%p, %p)\n", table, parfmt));

	ready_table_for_sizing(table);
	size_child_items(rh,table, parfmt);
    } while ( new_sizer(table) );
#endif

    TABDBG(("rid_size_table(): min %d, max %d\n",
	    table->width_info.minwidth, table->width_info.maxwidth));

    done_table_sizing(table);

    return;
}

/*****************************************************************************/

static void rid_table_set_stream_widths(rid_table_item *table)
{
    rid_table_cell *cell;
    int x, y;
    const int cellsx = table->cells.x;
    int twidth = table->cumminabs[cellsx];
    const int cb = table->cellspacing + 2 * table->cellpadding;

    TABDBG(("rid_table_set_stream_widths()\n"));

    table->size.x = twidth + 2 * table->border + table->cellspacing;

    if (table->caption)
    {
	table->caption->stream.fwidth = twidth - cb;
	/* DAF: 970315: hunting -ve fwidth */
	ASSERT(table->caption->stream.fwidth >= 0);
	table->caption->size.x = twidth;
    }

#if DEBUG && 0
    dump_table(table, NULL);
#endif

    for (x = 0; x < cellsx; x++)
    {
	table->colhdrs[x]->sizex = table->cumminabs[x+1] - table->cumminabs[x];
	TABDBG(("..._set_stream_width(): column %d has sizex %d\n", x, table->colhdrs[x]->sizex));
    }

    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {       /* Cells can have borders */
	cell->stream.fwidth = table->cumminabs[x + cell->span.x] - table->cumminabs[x];
#if DEBUG && 1
	fprintf(stderr, "%2d %2d = %3d, table->cumminabs[x=%2d + cell->span.x=%2d]=%3d - table->cumminabs[%2d]=%3d;\n",
	       x, y, cell->stream.fwidth, 
	       x, cell->span.x, 
	       table->cumminabs[x + cell->span.x], 
	       x, 
	       table->cumminabs[x] );
#endif
	/* DAF: 970315: hunting -ve fwidth */
	cell->stream.fwidth -= cb;
	if (cell->span.x == 0)
	{
#ifdef PLOTCHECK
	    plotcheck_boom();
#endif
	}
	else
	{
	    /* @@@@ */
	    /*ASSERT(cell->stream.fwidth >= 0);*/
	    /*cell->stream.fwidth = 0;*/
	}
    }
}

/* Table formatting */
/* fwidth of all text streams - possibly excluding borders */
/* sizex of all columns - borders are included here */
/* size.x of all cells/caption - borders are included here */





/*
  caption
  1 unit seperator line
  table top border
  table contents
  table bottom border
  */

static void borris_rid_table_share_spare_height(rid_table_item *table)
{
    const int cb = table->cellspacing + 2 * table->cellpadding;
    rid_table_cell *cell;
    int x, y, i, t;
    int *heights = NULL, nheights;
    int offy;

    TABDBG(("borris_rid_table_share_spare_height()\n"));

    offy = -table->border;

    /*
      add caption at top:

      */

    /* offy is -ve */

    if (table->caption && table->caption->calign != rid_ct_BOTTOM)
    {
	table->caption->off.x = table->border;
	offy -= table->cellspacing + table->cellpadding;
	table->caption->off.y = offy;
	offy += table->caption->stream.height;
	offy -= 2 * table->border + table->cellspacing + table->cellpadding;
    }

    nheights = table->cells.y;
    table->size.y = -offy;

    TABDBG(("Sharing spare height out over %d rows, cb is %d\n", nheights, cb));

    if (nheights > 0)
    {
	heights = mm_calloc(sizeof(int), nheights + 1);

	/* Ensure adequate heights */
	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	{       /* Even distribution is probably better than proportional */
	    /* distribution for the vertical spreading. */
	    int worst_span = nheights - cell->cell.y;
	    int hspan;

	    if (worst_span > cell->span.y)
		worst_span = cell->span.y;

	    /* Make hspan positive */

	    if ( (table->rowhdrs[y]->rowgroup->flags & rid_rgf_TFOOT) &&
		 (table->flags & rid_tf_TFOOT_INVISIBLE) )
		hspan = 0;
	    else
		hspan = (-cell->stream.height + cb);

	    TABDBG(("Cell %d,%d, span %d,%d, height %d:%d\n",
		    cell->cell.x,
		    cell->cell.y,
		    cell->span.x,
		    cell->span.y,
		    cell->stream.height,
		    hspan));

	    ensure_span_evenly(heights,
			       cell->cell.y,
			       worst_span,
			       hspan);
	}

	for (x = 0; x < nheights; x++)
	{
	    heights[x] = -heights[x];
	}

#if DEBUG > 2
	fprintf(stderr, "Heights after ensuring A\n");
	dump_span(heights, nheights);
#endif

	/* We build an accumulated width list, not we separate them out and change sign */
	for (x = 0; x < nheights; x++)
	{
	    /*heights[x] += heights[x+1];*/
	}

        /* SJM: heights are now all -ve */
#if DEBUG > 2
	fprintf(stderr, "Heights after ensuring B\n");
	dump_span(heights, nheights);
#endif
	/* Record for later easier examination */
	for (y = 0; y < table->cells.y; y++)
	{
	    rid_table_rowhdr *hdr = table->rowhdrs[y];
	    hdr->offy = offy;               /* hdr->offy is -ve */
	    hdr->sizey = -heights[y];       /* hdr->sizey is +ve */
	    offy += heights[y];
	}

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	{
	    for (t = 0, i = cell->cell.y ;
		 i < cell->cell.y + cell->span.y && i < nheights;
		 i++)
	    {
		t -= heights[i];    /* t is+ ve */
	    }
	    cell->size.y = t;
	}

	nullfree((void**)&heights);
    }

    /* add caption at bottom */
    if (table->caption && table->caption->calign == rid_ct_BOTTOM)
    {
	table->caption->off.x = table->border;
	offy -= 2 * table->border + table->cellspacing + table->cellpadding;
	table->caption->off.y = offy;
	offy += table->caption->stream.height;
	offy -= table->cellspacing + table->cellpadding;
    }

    offy -= table->cellspacing + table->border;

    table->size.y = -offy;
}

static void nicko_rid_table_share_spare_height(rid_table_item *table)
{
    const int cb = table->cellspacing + 2 * table->cellpadding;
    rid_table_cell *cell;
    int x, y, i, t;
    int *heights = NULL, nheights;
    int offy;

    TABDBG(("nicko_rid_table_share_spare_height()\n"));

    offy = -table->border;

    /*
      add caption at top:

      */

    /* offy is -ve */

    if (table->caption && table->caption->calign != rid_ct_BOTTOM)
    {
	table->caption->off.x = table->border;
	offy -= table->cellspacing + table->cellpadding;
	table->caption->off.y = offy;
	offy += table->caption->stream.height;
	offy -= 2 * table->border + table->cellspacing + table->cellpadding;
    }

    nheights = table->cells.y;
    table->size.y = -offy;

    TABDBG(("Sharing spare height out over %d rows\n", nheights));

    if (nheights > 0)
    {
	int *sqh;

	heights = mm_calloc(sizeof(int), nheights + 1);

	sqh = mm_malloc(sizeof(int) * nheights * nheights);
	if (nheights)
	    memset(sqh, -1, sizeof(int) * nheights * nheights);

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	{       /* Even distribution is probably better than proportional */
	    /* distribution for the vertical spreading. */
	    int worst_span = nheights - cell->cell.y;
	    int hspan;
	    int ydex;

	    if (worst_span > cell->span.y)
		worst_span = cell->span.y;

	    /* Make hspan positive */

	    if ( (table->rowhdrs[y]->rowgroup->flags & rid_rgf_TFOOT) &&
		 (table->flags & rid_tf_TFOOT_INVISIBLE) )
		hspan = 0;
	    else
		hspan = (-cell->stream.height + cb);

	    TABDBG(("Incorporate height %d, row%d, span %d\n",
		    hspan, y, worst_span));

	    ydex = (y * nheights) + (worst_span -1);

	    if (sqh[ydex] < hspan)
		sqh[ydex] = hspan;
	}

	rid_close_fixed_widths(nheights, sqh, 0);

	heights[0] = 0;
	rid_table_accumulate_widths_l2r(nheights, sqh, heights);

	nullfree((void**)&sqh);

	/* We build an accumulated width list, not we separate them out and change sign */
	for (x = 0; x < nheights; x++)
	{
	    heights[x] -= heights[x+1];
	}

        /* SJM: heights are now all -ve */
#if DEBUG == 3
	fprintf(stderr, "Heights after ensuring\n");
	dump_span(heights, nheights);
#endif
	/* Record for later easier examination */
	for (y = 0; y < table->cells.y; y++)
	{
	    rid_table_rowhdr *hdr = table->rowhdrs[y];
	    hdr->offy = offy;               /* hdr->offy is -ve */
	    hdr->sizey = -heights[y];       /* hdr->sizey is +ve */
	    offy += heights[y];
	}

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	{
	    for (t = 0, i = cell->cell.y ;
		 i < cell->cell.y + cell->span.y && i < nheights;
		 i++)
	    {
		t -= heights[i];    /* t is+ ve */
	    }
	    cell->size.y = t;
	}

	nullfree((void**)&heights);
    }

    /* add caption at bottom */
    if (table->caption && table->caption->calign == rid_ct_BOTTOM)
    {
	table->caption->off.x = table->border;
	offy -= 2 * table->border + table->cellspacing + table->cellpadding;
	table->caption->off.y = offy;
	offy += table->caption->stream.height;
	offy -= table->cellspacing + table->cellpadding;
    }

    offy -= table->cellspacing + table->border;

    table->size.y = -offy;
}


static void rid_close_fixed_widths(int cellsx, int *sqfixed, int overwrite)
{
    int x, i, j;
    int changed;

    /* Loop through setting widths until it settles */
    do
    {
	int jpos;
	int kpos, kspan;
	int idex, jdex, kdex;
	int wdiff;
	changed = 0;

	TABDBG(("rid_close_fixed_widths()\n"));
	dump_sq("rid_close_fixed_widths()", sqfixed, cellsx);

	/* For each column boundary */
	for (x = 0; x <= cellsx; x++)
	{
	    /* Check cells starting at the this boundary */
	    /* They can span anywhere fron 1 to cellsx - x but the last
	       span is only useful when the other of the pair goes the other way */

	    /*const int calc_worst_span = worst_max(sqfixed, cellsx);*/

	    TABDBG(("Start: %d\n", x));

	    for (i=1; i <= cellsx - x; i++)
	    {
		idex = (x * cellsx) + (i-1);

		TABDBGN(("  First span: %d (%d)\n", i, sqfixed[idex]));

		/* Only check where both have absolute values */
		if (sqfixed[idex] != -1)
		{
		    TABDBGN(("  On from cell %d...\n", x+i));

		    /* Check every pair the other starting at the end of the span */
		    for (j=1; j <= cellsx - x - i; j++)
		    {
			jpos = x+i;
			jdex = (jpos * cellsx) + (j-1);

			TABDBGN(("    Second span: %d (%d)\n", j, sqfixed[jdex]));

			if (sqfixed[jdex] != -1)
			{
			    /* Compare cell starting at x and spanning i with cell starting
			       at x+i and spanning j to right */
			    /* The span between them starts at (x) and spans (j+i) */

			    kpos = x;
			    kspan = j+i;
			    kdex = (kpos * cellsx) + (kspan-1);

			    if (overwrite || sqfixed[kdex] == -1)
			    {
				wdiff = sqfixed[jdex] + sqfixed[idex];

				if (sqfixed[kdex] < wdiff)
				{
				    TABDBGN(("    Starting at %d, span %d=%d, span %d=%d, cell %d span %d=%d (was %d)\n", x, i, sqfixed[idex], j, sqfixed[jdex], kpos, kspan, wdiff, sqfixed[kdex]));
				    changed = 1;
				    sqfixed[kdex] = wdiff;
				}
			    }
			}
		    }

		    TABDBGN(("  On from cell %d...\n", x));

		    /* Check every pair the other starting here */
		    for (j=i+1; j <= cellsx - x; j++)
		    {
			jpos = x;
			jdex = (jpos * cellsx) + (j-1);

			TABDBGN(("    Second span: %d (%d)\n", j, sqfixed[jdex]));

			if (sqfixed[jdex] != -1)
			{
			    /* Compare cells both starting at x and spanning i and j to right */
			    /* The span between them starts at (x+i) and spans (j-i) */

			    kpos = x+i;
			    kspan = j-i;
			    kdex = (kpos * cellsx) + (kspan-1);

			    if (overwrite || sqfixed[kdex] == -1)
			    {
				wdiff = sqfixed[jdex] - sqfixed[idex];

				if (sqfixed[kdex] < wdiff)
				{
				    TABDBGN(("    Starting at %d, span %d=%d, span %d=%d, cell %d span %d=%d (was %d)\n", x, i, sqfixed[idex], j, sqfixed[jdex], kpos, kspan, wdiff, sqfixed[kdex]));
				    changed = 1;
				    sqfixed[kdex] = wdiff;
				}
			    }
			}
		    }

		    TABDBGN(("  Back from cell %d...\n", x+i));

		    /* Check every pair the other ending at the end of the span */
		    for (j=1; j < i; j++)
		    {
			jpos = x+i-j;
			jdex = (jpos * cellsx) + (j-1);

			TABDBGN(("    Second span: %d (%d)\n", j, sqfixed[jdex]));

			if (sqfixed[jdex] != -1)
			{
			    /* Compare cell starting at x and spanning i with cell starting
			       at x+i-j and spanning j to right */
			    /* The span between them starts at (x) and spans (i-j) */

			    kpos = x;
			    kspan = i-j;
			    kdex = (kpos * cellsx) + (kspan-1);

			    if (overwrite || sqfixed[kdex] == -1)
			    {
				wdiff = sqfixed[idex] - sqfixed[jdex];

				if (sqfixed[kdex] < wdiff)
				{
				    TABDBGN(("    Starting at %d, span %d=%d, span %d=%d, cell %d span %d=%d (was %d)\n", x, i, sqfixed[idex], j, sqfixed[jdex], kpos, kspan, wdiff, sqfixed[kdex]));
				    changed = 1;
				    sqfixed[kdex] = wdiff;
				}
			    }
			}
		    }

		}
	    }
	}

    } while (changed);
}

static void rid_table_accumulate_widths_l2r(int cellsx, int *sq, int *cum)
{
    int x,y;

    for(x=0; x < cellsx; x++)
    {
	int worst = 0;

	TABDBGN(("Finding end of col %d\n", x));

	for(y=1; y <= (x+1); y++)
	{
	    int index = ((x-(y-1))*cellsx) + (y-1);

	    TABDBGN(("Looking at cells that span %d (start col=%d), min=%d\n",
		     y, x-(y-1), sq[index]));

	    if ((sq[index] != -1) &&
		(sq[index] + cum[x-y+1] > worst))
		worst = sq[index] + cum[x-y+1];
	}

	if (worst < cum[x])
	    worst = cum[x];

	cum[x+1] = worst;
    }
}

static void rid_table_accumulate_widths_r2l(int cellsx, int *sq, int *cum)
{
    int x,y;

    for(x=cellsx-1; x >= 0; x--)
    {
	int worst = cum[cellsx];

	TABDBGN(("Finding start of col %d\n", x));

	for(y=1; y <= (cellsx - x); y++)
	{
	    int index = (x*cellsx) + (y-1);

	    TABDBGN(("Looking at cells that span %d (start col=%d), min=%d\n",
		     y, x, sq[index]));

	    if ((sq[index] != -1) &&
		(cum[x+y] - sq[index] < worst))
		worst = cum[x+y] - sq[index];
	}

	if (worst > cum[x+1])
	    worst = cum[x+1];

	cum[x] = worst;
    }
}


static int rid_span_slack2(int cellsx, int start, int span, int *cum, int *sqmin, int *sqnew)
{
    int x,y;
    int sdex = (start * cellsx) + (span - 1);
    int slack;

    cum[start] = 0;

    for(x=start+1; x <= (start + span); x++)
    {
	int worst = 0;
	int max_y;

	TABDBGN(("Finding boundary %d\n", x));

	max_y = (x == start + span) ? (span - 1) : (x - start);

	for(y=1; y <= max_y; y++)
	{
	    int index = ((x-y)*cellsx) + (y-1);
	    int size;

	    size = sqnew[index];
	    if (size == -1)
		size = sqmin[index];

	    TABDBGN(("Looking at cells that span %d (start col=%d), min=%d\n",
		     y, x-y, size));

	    if ((size != -1) &&
		(size + cum[x-y] > worst))
		worst = size + cum[x-y];
	}

	if (worst < cum[x-1])
	    worst = cum[x-1];

	cum[x] = worst;
    }

    slack = (sqmin[sdex] - cum[start+span]);

    TABDBGN(("Last accumulation=%d, fixed span=%d, slack=%d\n",
	     cum[start+span], sqmin[sdex], slack));

    if (slack < 0)
	slack = 0;

    return slack;
}

static void rid_spread_slack(int cellsx, int start, int span, int *cum,
			     int *sqmin, int *sqnew, int *sqmax, int *sqfixed,
			     int slack)
{
    int i,j;

    TABDBG(("rid_spread_slack(): cellsx %d, start %d, span %d, slack %d\n",
	    cellsx, start, span, slack));

    PRINT_LIST(cum, cellsx, "Cum");

    if (slack <= 0)
	return;

    for(i=1; i < span; i++)
    {
	int share_over = 0;

	for (j=0; j <= span - i; j++)
	{
	    int idex = ((start + j) * cellsx) + (i-1);

	    if (sqnew[idex] == -1 && sqfixed[idex] == -1)
		share_over++;
	}

	TABDBG(("share_over %d\n", share_over));

	if (share_over)
	{
	    for (j=0; j <= span - i; j++)
	    {
		int idex = ((start + j) * cellsx) + (i-1);
		int grow;
#if 0
		/* Be VERY careful about rounding */
		grow = (((slack * (share_over+j))/span) -
			((slack * (j)  )/span) );
#else
		grow =  (slack * i) / span  ;
#endif

		TABDBG(("Offset %d, span %d, (idex %d) grow %d, fixed %d, min %d, max %d\n",
			 j, i, idex, grow, sqfixed[idex], sqmin[idex], sqmax[idex]));

		if (sqfixed[idex] == -1 && sqnew[idex] == -1)
		{
		    int newmin = sqmin[idex] + grow;

		    if (newmin > sqmax[idex])
			newmin = sqmax[idex];

		    sqnew[idex] = newmin;
		}
	    }
	}
    }

    dump_sq("Yield", sqnew, cellsx);
}

static void rid_table_place_cols(rid_text_stream *stream,
				 rid_table_item *table,
				 rid_fmt_info *parfmt, int width)
{
    const int cellsx = table->cells.x;
    int *sqfixed, *sqmin, *sqmax, *sqnew;
    int sq_index;
    rid_table_cell *cell;
    int x,y;
    int i;
    int changed;
    int strongly_constrained;
    BOOL too_small = FALSE, overridden = FALSE;

    x = sizeof(int) * cellsx * cellsx;

    sqfixed = mm_malloc(x);
    sqmin = mm_malloc(x);
    sqmax = mm_malloc(x);
    sqnew = mm_malloc(x);

    TABDBG(("rid_table_place_cols(): min/max absolute list before accounting for non percent/absolute items\n"));

    PRINT_LIST(table->minabs, cellsx, "Min");
    PRINT_LIST(table->maxabs, cellsx, "Max");

    if (table->width_info.maxwidth < width)
    {
#if 0
	for (x = 0, i = 0; i < cellsx; i++)
	{
	    x += table->maxabs[i];
	}

	if (x < width)
	{
	    TABDBG(("rid_table_place_cols(): sum maxabs %d < width %d\n", x, width));
	    too_small = TRUE;
	}
#else
	too_small = TRUE;
#endif
    }

    for (strongly_constrained = 0; strongly_constrained < 2; strongly_constrained++)
    {
	x = sizeof(int) * cellsx * cellsx;
	if (x)
	{
	    memset(sqfixed, -1, x);
	    memset(sqmin, -1, x);
	    memset(sqmax, -1, x);
	}

	for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
	{
	    /* Size cell. Include any border type spacing */
	    /* Might be prevented from wrapping by cell */
	    /* Might be forced to minimum width by col or colgroup */

	    rid_width_info *wi = &cell->stream.width_info;
	    rid_halign_tag halign;
	    rid_stdunits width_units;
	    int hard;
	    int width_px = 0;

	    if ( (table->rowhdrs[y]->rowgroup->flags & rid_rgf_TFOOT) &&
		 (table->flags & rid_tf_TFOOT_INVISIBLE) )
		continue;

	    rid_getprop(table, x, y, rid_PROP_HALIGN, &halign);
	    rid_getprop(table, x, y, rid_PROP_WIDTH, &width_units);

	    width_px = wi->minwidth;
	    hard = 0;

	    if (width_units.type != value_none)
	    {
		switch(width_units.type)
		{
		case value_absunit:
		    hard = 1;
		    break;
		case value_pcunit:
		    width_px = (int) ceil(((double) width) * width_units.u.f / 100.0);
		    hard = 1;
		    break;
		}
	    }

	    sq_index = (x * cellsx) + cell->span.x - 1;

	    if (hard && !strongly_constrained)
	    {
		if (width_px > sqfixed[sq_index])
		    sqfixed[sq_index] = width_px;
	    }
	    else
	    {
		if (too_small)
		{
		    overridden = TRUE;
		    TASSERT(wi->maxwidth < width);
		    /* @@@@ */
		    wi->maxwidth = width;
		}

		if (width_px > sqmin[sq_index])
		    sqmin[sq_index] = width_px;
		if (wi->maxwidth > sqmax[sq_index])
		    sqmax[sq_index] = wi->maxwidth;
	    }
	}

	if (!too_small || overridden)
	    break;

	TABDBG(("Over-constrained table.\n"));
    }

    /* We can add one more to this: the span starting at the left edge and
       ending at the right edge must be the width the caller asked for
       SJM: 101096: don't do it if cellsx == 0... */

    if (cellsx)
	sqfixed[cellsx-1] = width;

    /* Let's just quickly force each column to have a minimum width of at least zero */
    for(x = 0; x < cellsx; x++)
    {
	if (sqmin[x * cellsx] == -1)
	{
	    sqmin[x *cellsx] = 0;
	}
    }

    /* Now we need to compute the transitive closure of the width 'graph' */

    rid_close_fixed_widths(cellsx, sqfixed, 1);

    /* Where we have known values copy them over to the min table */
    for(x = 0; x < cellsx; x++)
    {
	for(i = 1; i <= cellsx-x; i++)
	{
	    int idex = (x * cellsx) + (i-1);

	    TABDBGN(("Start %d, span %d: fixed=%d, min=%d\n", x, i, sqfixed[idex], sqmin[idex]));

	    if (sqfixed[idex] != -1)
	    {
		int p,q;

		sqmax[idex] = sqmin[idex] = sqfixed[idex];

		/* If we have a fixed span over more than one col then enclosed cols are bounded */
		if (i > 1)
		{
		    for(p=1; p < i; p++)
		    {
			for (q=x; q <= (x+i-p); q++)
			{
			    int pdex = (q * cellsx) + (p-1);

			    if (sqmax[pdex] != -1 && sqmax[pdex] > sqfixed[idex])
				sqmax[pdex] = sqfixed[idex];
			}
		    }
		}
	    }
	}
    }

    do
    {
	if (cellsx)
	    memset(sqnew, -1, sizeof(int) * cellsx * cellsx);

	changed = 0;

	for(i=2; i <= cellsx; i++)
	{
	    for(x=0; x <= cellsx - i; x++)
	    {
		int idex = (x * cellsx) + (i-1);

		if (sqfixed[idex] != -1)
		{
		    int slack;

		    TABDBGN(("idex %d (%d,%d): fixed %d, min %d, new %d\n",
			    idex, i, x, sqfixed[idex], sqmin[idex], sqmax[idex]));

		    slack = rid_span_slack2(cellsx, x, i, table->cumminabs, sqmin, sqnew);

		    rid_spread_slack(cellsx, x, i, table->cumminabs,
				     sqmin, sqnew, sqmax, sqfixed, slack);
		}
	    }
	}

	for(i=1; i <= cellsx; i++)
	{
	    for(x=0; x <= cellsx - i; x++)
	    {
		int idex = (x * cellsx) + (i-1);

		if ((sqnew[idex] != -1) && (sqnew[idex] != sqmin[idex]))
		{
		    TABDBG(("Cell %d span %d stretched from %d to %d\n",
			     x, i, sqmin[idex], sqnew[idex]));
		    sqmin[idex] = sqnew[idex];
		    changed = 1;
		}
	    }
	}

#if DEBUG
	dump_sq("After munging, min is ", sqmin, cellsx);
#endif

	/* Two passes:
	   Build cumminabs with a L->R of sqmin
	   Build cummaxabs with a R->L of sqmin
	   */

	/* L->R pass of sqmin for min values */
	table->cumminabs[0] = 0;
	rid_table_accumulate_widths_l2r(cellsx, sqmin, table->cumminabs);

	PRINT_LIST(table->cumminabs, cellsx + 1, "Cum:");

	/* R->L pass of sqmin for max values */
	table->cummaxabs[cellsx] = table->cumminabs[cellsx];
	rid_table_accumulate_widths_r2l(cellsx, sqmin, table->cummaxabs);

	TABDBG(("rid_table_place_cols(): cummulative min/max abs lists are:\n"));
	PRINT_LIST(table->cumminabs, cellsx + 1, "Min:");
	PRINT_LIST(table->cummaxabs, cellsx + 1, "Max:");

#if 0
	for(leftfix = 0; leftfix < cellsx; /**/ )
	{
	    int spread_cols, spread_width, spare_width;

	    while ((leftfix < cellsx) &&
		   (table->cumminabs[leftfix+1] == table->cummaxabs[leftfix+1]))
		leftfix++;

	    if (leftfix == cellsx)
		break;

	    rightfix = leftfix + 1;

	    while ((rightfix < cellsx) &&
		   (table->cumminabs[rightfix] != table->cummaxabs[rightfix]))
		rightfix++;

	    /* We know that that leftfix and rightfix point to fixed boundaries
	       and that the boundaries in between are flexable */

	    spread_cols = rightfix - leftfix;
	    /* @@@@ Wrong!!!! */
	    spread_width = table->cumminabs[rightfix] - table->cumminabs[rightfix-1];

	    TABDBGN(("Left %d, right %d, spread_width %d\n", leftfix, rightfix, spread_width));

	    for(i=1; i <= spread_cols; i++)
	    {
		for (j=0; j <= spread_cols - i; j++)
		{
		    int idex = ((leftfix + j) * cellsx) + (i-1);
		    int grow;

		    /* Be VERY careful about rounding */
		    grow = (((spread_width * (i+j))/spread_cols) -
			    ((spread_width * (j)  )/spread_cols) );

		    TABDBGN(("Offset %d, span %d, (idex %d) grow %d, fixed %d, min %d, max %d\n",
			     j, i, idex, grow, sqfixed[idex], sqmin[idex], sqmax[idex]));

		    if (sqfixed[idex] == -1)
		    {
			int newmin = sqmin[idex] + grow;

			if (newmin > sqmax[idex])
			    newmin = sqmax[idex];

			if (sqmin[idex] != newmin)
			{
			    changed = 1;
			    sqmin[idex] = newmin;
			}
		    }
		}
	    }
	    leftfix = rightfix;
	}

	TABDBG(("rid_table_place_cols(): cummulative min/max abs lists are:\n"));
	PRINT_LIST(table->cumminabs, cellsx + 1, "Min:");
	PRINT_LIST(table->cummaxabs, cellsx + 1, "Max:");
#endif
    } while (changed);

    /* Choose initial absolute values */

    for (x = 0; x < cellsx; x++)
    {
	table->colhdrs[x]->width_info.minwidth =
	    table->minabs[x] = table->cumminabs[x+1] - table->cumminabs[x];
	table->colhdrs[x]->width_info.maxwidth =
	    table->maxabs[x] = table->cummaxabs[x+1] - table->cummaxabs[x];
    }

    TABDBG(("(): min/max absolute list after accounting for non percent/absolute items\n"));

    PRINT_LIST(table->minabs, cellsx, "Min");
    PRINT_LIST(table->maxabs, cellsx, "Max");

    /* Don't forget to free these before we go... */
    mm_free(sqfixed);
    mm_free(sqmin);
    mm_free(sqmax);
    mm_free(sqnew);
}

/*****************************************************************************

  We don't have height information until after we have formatted, but
  formatting sets top fields. So, we format each child text stream as if
  at the top of the table, and then move individual streams down after we've
  shared out height information. This ends up with all items in the table
  correctly positioned if the text table starts at zero.

  */

/* Called by be_formater_loop */
/* Formats child text streams. */
/* Chooses widths and then heights */
/* Sets xoff and yoff for items */

extern void rid_table_share_width(rid_header *rh, rid_text_stream *stream, rid_text_item *orig_item, rid_fmt_info *parfmt)
{
    rid_table_item *table = ((rid_text_item_table *)orig_item)->table;
    rid_table_cell *cell;
    int x, y, t, i;
    int given_width = stream->fwidth - (orig_item->st.indent * INDENT_UNIT);
    const int old_fwidth = stream->fwidth;
    rid_fmt_info fmt;

    fmt = *parfmt;

    TABDBG(("\n\n\nrid_table_share_width() entered: fwidth %d, working given %d\n",
	    old_fwidth, given_width));

    if (table->userwidth.type == rid_stdunit_PX)
    {
	TABDBG(("Applying absolute table width\n"));
	given_width = 2 * table->border + (int) ceil(table->userwidth.u.f);
	if (given_width < table->width_info.minwidth)
	    given_width = table->width_info.minwidth;
    }
    else
    {
	if (table->userwidth.type == rid_stdunit_MULT)
	{       /* Try to apply a relative width */
	    TABDBG(("Applying relative table width\n"));
	    given_width = (int) ceil(table->userwidth.u.f * (double) given_width);
	    if (given_width < table->width_info.minwidth)
		given_width = table->width_info.minwidth;
	}
	else if (table->userwidth.type == rid_stdunit_PCENT)
	{       /* Try to apply a relative width */
	    TABDBG(("Applying table width percentage\n"));
	    if (table->userwidth.u.f <= 100.0)
	    {
		given_width = (int) ceil(table->userwidth.u.f * (double) given_width / 100.0);
		if (given_width < table->width_info.minwidth)
		    given_width = table->width_info.minwidth;
	    }
	}
	/* Items have min|max sizes */
	else if (given_width < table->width_info.minwidth)
	{       /* Use minimum widths all over */
	    TABDBG(("Setting table to minimum width %d, instead of offered width %d\n",
		     table->width_info.minwidth, given_width));
	    given_width = table->width_info.minwidth;
	}
	else if (given_width > table->width_info.maxwidth)
	{       /* Use maximum widths all over */
	    TABDBG(("Setting table to maximum width %d, instead of offered width %d\n",
		    table->width_info.maxwidth, given_width));
	    given_width = table->width_info.maxwidth;
	}
	else
	{       /* Minimum widths + share of what's left */
	    TABDBG(("Setting table width to min=%d < given=%d < max=%d\n",
		    table->width_info.minwidth, given_width, table->width_info.maxwidth));
	}
    }

    TABDBG(("rid_table_share_width(): overall given_width of %d chosen\n", given_width));

    given_width -= 2 * table->border + table->cellspacing;

    TABDBG(("rid_table_share_width(): final given_width of %d chosen\n", given_width));

    stream->fwidth = given_width;
    if (stream->fwidth == 0)
    {
	/* @@@@ */
	/*ASSERT(stream->fwidth >= 0);*/
	/*stream->fwidth = 0;*/
    }

    rid_table_place_cols(stream, table, parfmt, given_width);
    rid_table_set_stream_widths(table);

    stream->fwidth = old_fwidth;
    ASSERT(stream->fwidth >= 0);

    TABDBGN(("Width all shared out.\n"));

    /* Set the horizontal offset for each column */
    for (x = 0, y = table->border; x < table->cells.x; x++)
    {
	table->colhdrs[x]->offx = y;
	y += table->colhdrs[x]->sizex;
    }

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
	for (t = 0, i = cell->cell.x; i < cell->cell.x + cell->span.x; i++)
	    t += table->colhdrs[i]->sizex;
	cell->size.x = t;
    }

    /* Each stream now has an fwidth to format with - it is guarenteed */
    /* to be big enough to hold the single largest item, or I've coded it wrong. */
    /* Each column and the caption also have their size field - this includes */
    /* any borders that the fwidth may have excluded. */
    /* Recursively format all components of the table first */

    if (table->caption)
    {
	TABDBGN(("Formatting the caption.\n"));

	be_formater_loop_core( rh, &table->caption->stream,
			      table->caption->stream.text_list,
			      &fmt, rid_fmt_BUILD_POS);
    }

    for ( x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {
	TABDBGN(("Formatting cell %d,%d\n", x, y));

	be_formater_loop_core( rh, &cell->stream, cell->stream.text_list, &fmt, rid_fmt_BUILD_POS);

	/* Handle <TD HEIGHT=Npx> */

	if (cell->userheight.type == value_absunit)
	{
	    /* No *2 scaling??? */
	    const int mh = (int) ceil(cell->userheight.u.f);
            /* SJM: added - signs on height */
	    if (mh > -cell->stream.height)
	    {
		TABDBG(("Vertically growing cell due to <TD HEIGHT=%d>\n", mh));
		cell->stream.height = -mh;
	    }
	}

	/* We have to let the formatting happen so that we */
	/* get the rid_pos chain built, or we'll end up having */
	/* to put this check all over the place - eg searching? */
	if ( (table->flags & rid_tf_TFOOT_INVISIBLE) &&
	     (table->rowhdrs[y]->flags & rid_rhf_TFOOT))
	    cell->stream.height = 0;
    }

    /* Now, eventually, we can get some height information */

    if (table->cells.y > 30)
	borris_rid_table_share_spare_height(table);
    else
	nicko_rid_table_share_spare_height(table);

    /* Now each stream has real width and height. */
    /* Calculate the sizes */
    /* for the rid_text_item holding the table */

    orig_item->max_up = table->size.y;
    orig_item->max_down = 0;
    orig_item->width = table->size.x;
    orig_item->pad = 0;

    /* The rid_text_item containing the table has correct width */
    /* and height information.  */

    TABDBG(("Finished sharing widths\n"));
}

/*****************************************************************************/


/* eof tablesize.c */
