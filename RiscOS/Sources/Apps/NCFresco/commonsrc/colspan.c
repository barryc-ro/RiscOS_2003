/* -*-C-*- colspan.c
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Column sharing algorithm, designed by Patrick Campbell-Preston
 * (patrick@chaos.org.uk).
 *
 * Additional implementation by borris.
 *
 * Further applied hindsight by Patrick.
 *
 * NOTE: THE CODE HERE IS USED FOR BOTH HORIZONTAL AND VERTICAL
 *       PURPOSES. NAMES ARE ALWAYS CHOSEN ON THE BASIS THAT WE
 *       ARE WORKING HORIZONTALLY THOUGH. THINK VERY CAREFULLY.
 */

#include "rid.h"
/*#include "format.h"*/
#include "colspan.h"
#include "tables.h"
#include "util.h"
#include "gbf.h"

#ifdef PLOTCHECK
#include "rectplot.h"
#endif

#if DEBUG
static const char * WIDTH_NAMES[N_COLSPAN_WIDTHS] = WIDTH_NAMES_ARRAY;
#endif

/*****************************************************************************/

#ifndef FRESCO
static void generate_constraints_summary(rid_table_item *table,
					 BOOL horiz,
					 constraints_summary *ptr)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x;

    memset(ptr, 0, sizeof(*ptr));

    for (x = 0; x < max; x++)
    {
	const unsigned int flags = table->colspans[x].flags;

	if (flags & colspan_flag_USED)
	{
	    ptr->used += 1;

	    if (flags & colspan_flag_ABSOLUTE)
		ptr->with_absolute += 1;
	    else
		ptr->without_absolute += 1;

	    if (flags & colspan_flag_PERCENT)
		ptr->with_percent += 1;
	    else
		ptr->without_percent += 1;

	    if (flags & colspan_flag_RELATIVE)
		ptr->with_relative += 1;
	    else
		ptr->without_relative += 1;

	    if (flags & colspan_flag_FINISHED)
		ptr->finished += 1;
	    else
		ptr->unfinished += 1;
	}
	else
	    ptr->unused += 1;
    }
}
#endif

/*****************************************************************************

  Input:

  An array of integer values. An each <= 0 is considered 'notched out'

(int *array, int max, int slop

 */




/*****************************************************************************/

#if !defined(FRESCO) || DEBUG
static void print_colspan_flags(unsigned int flags)
{
    static struct { unsigned int f; char *n; }
    names[] =
    {
	{ colspan_flag_ABSOLUTE		, "colspan_flag_ABSOLUTE" } ,
	{ colspan_flag_PERCENT		, "colspan_flag_PERCENT" } ,
	{ colspan_flag_RELATIVE		, "colspan_flag_RELATIVE" } ,
	{ colspan_flag_ABSOLUTE_COL	, "colspan_flag_ABSOLUTE_COL" } ,
	{ colspan_flag_PERCENT_COL	, "colspan_flag_PERCENT_COL" } ,
	{ colspan_flag_RELATIVE_COL	, "colspan_flag_RELATIVE_COL" } ,
	{ colspan_flag_ABSOLUTE_GROUP	, "colspan_flag_ABSOLUTE_GROUP" } ,
	{ colspan_flag_PERCENT_GROUP	, "colspan_flag_PERCENT_GROUP" } ,
	{ colspan_flag_RELATIVE_GROUP	, "colspan_flag_RELATIVE_GROUP" } ,
	{ colspan_flag_FINISHED		, "colspan_flag_FINISHED" } ,
	{ colspan_flag_USED		, "colspan_flag_USED" } ,
	{ colspan_flag_STOMP_ABSOLUTE	, "colspan_flag_STOMP_ABSOLUTE" },
	{ colspan_flag_STOMP_PERCENT	, "colspan_flag_STOMP_PERCENT" },
	{ colspan_flag_STOMP_RELATIVE	, "colspan_flag_STOMP_RELATIVE" },
	{ 0, NULL }
    };

    int x;

    for (x = 0; x <= LAST_MAX; x++)
    {
	const int B = 1 << x;

	if (flags & B)
	    FMTDBG(("%s ", WIDTH_NAMES[x]));
    }

    for (x = 0; x < sizeof(names) / sizeof(names[1]); x++)
    {
	if ( names[x].f & flags )
	    FMTDBG(("%s ", names[x].n));
    }

    FMTDBG(("\n"));
}
#endif

/* trace the cells and their chains of groups */

extern void colspan_trace_cells (rid_table_item *table, BOOL horiz)
{
#if DEBUG
    if (horiz)
    {
    const int max = horiz ? table->cells.x : table->cells.y;
    int *width = HORIZCELLS(table,horiz);
    pcp_cell the_cells = table->colspans;
    int x, iw;

    FMTDBG(("_____________________________________________________________________________\n"
	    "\ncolspan_trace_cells(%p, %s): span %d, cells %p, depth %d, id %d\n",
	    table, HORIZVERT(horiz), max, the_cells, table->depth, table->idnum));

    /* Headings */
    for (iw = 0;  iw < N_COLSPAN_WIDTHS; iw++)
    {
	FMTDBG(("%s ", WIDTH_NAMES[iw]));
    }
    FMTDBG(("Extra\n\n"));

    /* Master summary */
    for (iw = 0;  iw < N_COLSPAN_WIDTHS; iw++)
    {
	FMTDBG(("  %4d  ", width[iw]));
    }
    FMTDBG(("MASTER\n\n"));

    /* The column headers */
    for (x = 0; x <= max; x++)	/* nb including dummy last column whose widths are all NOTINIT */
				/* could clarify this by tracing it seperately, I guess */
    {
	for (iw = 0; iw < N_COLSPAN_WIDTHS; iw++)
	    FMTDBG(("  %4d  ", the_cells[x].width[iw]));
	FMTDBG(("COL %2d LM %3d RM %3d\n", x, the_cells[x].leftmost, the_cells[x].rightmost));
    }
    FMTDBG(("\n"));

    /* The group cells */
    for (x = 0; x < max; x++)
    {
	pcp_group group;

	group = the_cells[x].first_start;

	while (group != NULL)
	{
	    for (iw = 0; iw < N_COLSPAN_WIDTHS; iw++)
		FMTDBG(("  %4d  ", group->width[iw]));
	    FMTDBG(("GRP %2d ST %3d EN %3d\n", x, group->istart, group->iend));
	    group = group->next_start;
	}
    }
    FMTDBG(("\n"));

    /* Column flags */
    for (x = 0; x < max; x++)
    {
	FMTDBG(("COL %2d ", x));
	print_colspan_flags(the_cells[x].flags);
    }

    /* Group flags */
    for (x = 0; x < max; x++)
    {
	pcp_group group;

	group = the_cells[x].first_start;

	while (group != NULL)
	{
	    FMTDBG(("GRP %2d ST %3d EN %3d ", x, group->istart, group->iend));
	    print_colspan_flags(group->flags);
	    group = group->next_start;
	}
    }

    FMTDBG(("\n"));
    }
#endif
}

/*****************************************************************************

NB colspan_algorithm() split up a bit by Borris and moved into a
separate file cs2.c because Norcroft C couldn't cope (pathetic);
Patrick deleted the #if'd out copy left here!

*****************************************************************************

  Used for adding border biases.

  Note that the rules for this are slightly more complicated than
  normal as these borders are, effectivly, an additional
  constraint. When can get circumstances where a cell starts or ends
  in a particular column from a group, but where that column has no
  direct specification of its own and where the contribution required
  for the group is less than the border width. Without forcing an
  additional constraint of the border width in here, we would
  otherwise end up without enough room for the border and choosing a
  negative fwidth value for the contents! As these are real
  constraints that must be obeyed, we need to flag these columns as
  having constraints as well. This should be done on RAW_MIN and
  RAW_MAX, and the results propogated to ensure that all other slots
  benefit from these extra constraints.

  When we make a previously unused column used, we need to set ALL
  relevant fields. Now hardcoded to be RAW_MIN and RAW_MAX
  specifically and to be called only once.

 */

extern void colspan_bias(rid_table_item *table, int bias, BOOL horiz)
{
    const int MAX = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int X;
    const width_array_e min = RAW_MIN;
    const width_array_e max = RAW_MAX;

    FMTDBGN(("colspan_bias(%p): bias %d onto RAW_MIN/RAW_MAX items\n",
	     table, bias));

    if (bias > 0)
    for (X = 0; X < MAX; X++)
    {
	pcp_group grp;
	pcp_cell cell = the_cells + X;

	if ( (cell->flags & colspan_flag_USED) != 0 )
	{
	    /* Column used directly - bias */
	    ASSERT(cell->width[min] != NOTINIT);
	    ASSERT(cell->width[max] != NOTINIT);
	    cell->width[min] += bias;
	    cell->width[max] += bias;
	}
	else if ( cell->first_start != NULL )
	{
	    /* Groups starting here - bias this column  */
	    ASSERT( cell->width[min] == NOTINIT );
	    ASSERT( cell->width[max] == NOTINIT );
	    cell->width[min] = bias;
	    cell->width[max] = bias;
	    cell->flags |= colspan_flag_USED;
	}
	else if ( cell->first_end != NULL )
	{
	    /* Groups ending here - bias this column  */
	    ASSERT( cell->width[min] == NOTINIT );
	    ASSERT( cell->width[max] == NOTINIT );
	    cell->width[min] = bias;
	    cell->width[max] = bias;
	    cell->flags |= colspan_flag_USED;
	}

	for (grp = cell->first_start; grp != NULL; grp = grp->next_start)
	{
	    /* Shouldn't make a group if its not used! */
	    ASSERT( grp->flags & colspan_flag_USED );

	    if ( (grp->flags & colspan_flag_USED) != 0 )
	    {
		ASSERT( grp->width[min] != NOTINIT );
		ASSERT( grp->width[max] != NOTINIT );
		grp->width[min] += bias;
		grp->width[max] += bias;
	    }
	}
    }
}

/*****************************************************************************

  Initialise the values in the column headers for the slot given from
  the leftmost values currently associated with the table.

 */

extern void colspan_column_init_from_leftmost(rid_table_item *table, width_array_e slot, BOOL horiz)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x, prev;

    FMTDBGN(("colspan_column_init_from_leftmost: id %d, slot %s, %s\n",
	    table->idnum, WIDTH_NAMES[slot], HORIZVERT(horiz)));
    /* leftmost and rightmost now refer to the left boundary of the
       cell, so we need to look at the leftmost of following cell to
       get the width of this one. */
    for (x = 0, prev = 0; x < max; x++)
    {
	pcp_cell cell = table->colspans + x;
	const int z = (cell+1)->leftmost;

	cell->width[slot] = z - prev;
	prev = z;
    }
}

/*****************************************************************************/

extern void colspan_column_and_eql_bitset(rid_table_item *table,
					BOOL horiz,
					width_array_e slot,
					unsigned int AND,
					unsigned int EQL,
					unsigned int SET)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_bitset: %s %s %x %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, SET));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].flags |= SET;
    }
}

extern void colspan_column_and_eql_set(rid_table_item *table,
				       BOOL horiz,
				       width_array_e slot,
				       unsigned int AND,
				       unsigned int EQL,
				       int SET)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_set: %s %s %x %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, SET));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] = SET;
    }
}

extern void colspan_column_and_eql_copy(rid_table_item *table,
					BOOL horiz,
					width_array_e slot,
					unsigned int AND,
					unsigned int EQL,
					width_array_e slot_from)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_copy: %s %s %x %x %s\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, WIDTH_NAMES[slot_from]));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] = the_cells[x].width[slot_from];
    }
}

extern void colspan_column_and_eql_lt_copy(rid_table_item *table,
					   BOOL horiz,
					   width_array_e slot,
					   unsigned int AND,
					   unsigned int EQL,
					   width_array_e slot_from)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_lt_copy: %s %s %x %x %s\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, WIDTH_NAMES[slot_from]));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	    if ( the_cells[x].width[slot] < the_cells[x].width[slot_from] )
		the_cells[x].width[slot] = the_cells[x].width[slot_from];
    }
}

extern void colspan_column_and_eql_upwards(rid_table_item *table,
					   BOOL horiz,
					   width_array_e slot,
					   unsigned int AND,
					   unsigned int EQL,
					   int to_share)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_upwards: %s %s %x %x %d\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, to_share));

    while (to_share > 0)
	for (x = 0; to_share > 0 && x < max; x++)
	{
	    if ( (the_cells[x].flags & AND) == EQL )
	    {
		the_cells[x].width[slot] += 1;
		to_share -= 1;
	    }
	}
}

extern void colspan_column_and_eql_downwards(rid_table_item *table,
					     BOOL horiz,
					     width_array_e slot,
					     unsigned int AND,
					     unsigned int EQL,
					     int to_share)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_downwards: %s %s %x %x %d\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, to_share));

    while (to_share > 0)
	for (x = 0; to_share > 0 && x < max; x++)
	{
	    if ( (the_cells[x].flags & AND) == EQL )
	    {
		the_cells[x].width[slot] -= 1;
		to_share -= 1;
	    }
	}
}

extern void colspan_column_and_eql_halve(rid_table_item *table,
					     BOOL horiz,
					     width_array_e slot,
					     unsigned int AND,
					     unsigned int EQL)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_halve: %s %s %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	{
	    the_cells[x].width[slot] /= 2;
	}
    }
}

extern void colspan_column_and_eql_double(rid_table_item *table,
					     BOOL horiz,
					     width_array_e slot,
					     unsigned int AND,
					     unsigned int EQL)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_double: %s %s %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	{
	    the_cells[x].width[slot] *= 2;
	}
    }
}

extern void colspan_column_and_eql_scale(rid_table_item *table,
					 BOOL horiz,
					 width_array_e slot,
					 unsigned int AND,
					 unsigned int EQL,
					 double scale)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_column_and_eql_scale: %s %s %x %x %g\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, scale));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & AND) == EQL )
	{
	    the_cells[x].width[slot] *= scale;
	}
    }
}

extern int colspan_sum_columns(rid_table_item *table,
				BOOL horiz,
				width_array_e slot)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;
    int sum = 0;


    for (x = 0; x < max; x++)
    {
	sum += the_cells[x].width[slot];
    }

    FMTDBGN(("colspan_sum_columns: %s %s: sum %d\n", HORIZVERT(horiz), WIDTH_NAMES[slot], sum));

    return sum;
}

/*****************************************************************************/


extern void colspan_all_and_eql_bitclr(rid_table_item *table,
					BOOL horiz,
					width_array_e slot,
					unsigned int AND,
					unsigned int EQL,
					unsigned int CLR)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_bitclr: %s %s %x %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, CLR));

    CLR = ~CLR;

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].flags &= CLR;

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		grp->flags &= CLR;
	}
    }
}

extern void colspan_all_and_eql_lt_copy(rid_table_item *table,
					BOOL horiz,
					width_array_e slot,
					unsigned int AND,
					unsigned int EQL,
					width_array_e slot_from)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_lt_copy: %s %s %x %x %s\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, WIDTH_NAMES[slot_from]));

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    if ( the_cells[x].width[slot] < the_cells[x].width[slot_from] )
		the_cells[x].width[slot] = the_cells[x].width[slot_from];

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		if ( grp->width[slot] < grp->width[slot_from] )
		    grp->width[slot] = grp->width[slot_from];
	}
    }
}

extern void colspan_all_and_eql_copy(rid_table_item *table,
				     BOOL horiz,
				     width_array_e slot,
				     unsigned int AND,
				     unsigned int EQL,
				     width_array_e slot_from)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_copy: %s %s %x %x %s\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, WIDTH_NAMES[slot_from]));

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] = the_cells[x].width[slot_from];

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		grp->width[slot] = grp->width[slot_from];
	}
    }
}

extern void colspan_all_and_eql_set(rid_table_item *table,
				    BOOL horiz,
				    width_array_e slot,
				    unsigned int AND,
				    unsigned int EQL,
				    int SET)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_set: %s %s %x %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, SET));

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] = SET;

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		grp->width[slot] = SET;
	}
    }
}


extern void colspan_all_and_eql_halve(rid_table_item *table,
				      BOOL horiz,
				      width_array_e slot,
				      unsigned int AND,
				      unsigned int EQL)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_halve: %s %s %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL));

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] /= 2 ;

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		grp->width[slot] /= 2;
	}
    }
}


extern void colspan_all_and_eql_double(rid_table_item *table,
				       BOOL horiz,
				       width_array_e slot,
				       unsigned int AND,
				       unsigned int EQL)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_double: %s %s %x %x\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL));

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] *= 2;

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		grp->width[slot] *= 2;
	}
    }
}


extern void colspan_all_and_eql_scale(rid_table_item *table,
				      BOOL horiz,
				      width_array_e slot,
				      unsigned int AND,
				      unsigned int EQL,
				      double scale)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int x;

    FMTDBGN(("colspan_all_and_eql_scale: %s %s %x %x %g\n", HORIZVERT(horiz), WIDTH_NAMES[slot], AND, EQL, scale));

    for (x = 0; x < max; x++)
    {
	pcp_group grp;

	if ( (the_cells[x].flags & AND) == EQL )
	    the_cells[x].width[slot] = ceil(0.5 + the_cells[x].width[slot] * scale);

	for (grp = the_cells[x].first_start; grp != NULL; grp = grp->next_start)
	{
	    if ( (grp->flags & AND) == EQL )
		grp->width[slot] = ceil(0.5 + grp->width[slot] * scale);
	}
    }
}



/*****************************************************************************

Free the colspan structure stored in table, if any.
*/

extern void colspan_free_structure(rid_table_item *table, BOOL horiz)
{
    pcp_cell the_cells = table->colspans;

    FMTDBG(("colspan_free_structure %s (table %p cells %p)\n",
	    HORIZVERT(horiz), table, the_cells));

    if (the_cells != NULL)
    {
	const int max = horiz ? table->cells.x : table->cells.y;
	int x;
	for (x = 0; x < max; x++)
	{
	    pcp_group grp = the_cells[x].first_start;
	    while (grp != NULL)
	    {
		pcp_group nxt = grp->next_start;
		mm_free (grp);
		grp = nxt;
	    }
	}
	mm_free (the_cells);
	table->colspans = NULL;
    }
}

/*****************************************************************************

  Create a new group record and chain it in (NB this means that
  multiple groups which start and end in the same places will lead to
  multiple records; this is deliberate to keep both space and time
  linear in (cells+groups)

  Returns a pointer to the widths array for further initialistion.

*/

static pcp_group new_group (pcp_cell_t * the_cells,
			    int istart,
			    int iend)
{
    int iw;
    pcp_group result = (pcp_group) mm_calloc(1, sizeof (struct pcp_group_s));
				/* panic here if null... */
    result->istart = istart;
    result->iend = iend;

    for (iw = 0; iw < N_COLSPAN_WIDTHS; iw++)
	result->width[iw] = NOTINIT;

    /* chain it in to the cell array in two places (one for each end) */
    result->next_start = the_cells[istart].first_start;
    the_cells[istart].first_start = result;
    result->next_end = the_cells[iend].first_end;
    the_cells[iend].first_end = result;
    /*result->flags = 0;*/

    return result;
}

/*****************************************************************************/

static void init_group(rid_table_item *table, BOOL horiz, rid_table_cell *cell, VALUE *vp, int scale_value)
{
    const int span = (horiz ? cell->span.x : cell->span.y);
    const int first = (horiz ? cell->cell.x : cell->cell.y);
    const int last = first + span - 1;
    pcp_group group;
    int *widths;
    unsigned int flags = 0;
    int z;

    group = new_group (table->colspans, first, last);
    group->table_cell = cell;
    widths = group->width;

    FMTDBGN(("init_group: id %d, %s, first %d, span %d\n",
	    table->id, HORIZVERT(horiz), first, span));

    /* New group, so always take these values */
    if (horiz)
    {
	widths[RAW_MIN] = cell->stream.width_info.minwidth;
	widths[RAW_MAX] = cell->stream.width_info.maxwidth;

#if DEBUG
	if (widths[RAW_MIN] > widths[RAW_MAX])
	{
	    FMTDBG(("init_group: min %d, max %d\n", widths[RAW_MIN] , widths[RAW_MAX]));
	}
#endif
	ASSERT(widths[RAW_MIN] <= widths[RAW_MAX]);
    }
    else
    {
	widths[RAW_MIN] = widths[RAW_MAX] = - cell->stream.height;
    }

    switch (vp->type)
    {
    case value_absunit:
	flags = colspan_flag_ABSOLUTE | colspan_flag_ABSOLUTE_GROUP;
	z = (scale_value * ceil(vp->u.f)) / 100;
	/* DAF: 970708: account for height this way now */
	if (widths[RAW_MIN] < z)
	    widths[RAW_MIN] = widths[RAW_MAX] = z;
	if (widths[ABS_MAX] < z)
	    widths[ABS_MAX] = widths[ABS_MIN] = z;
	break;

    case value_pcunit:
	flags = colspan_flag_PERCENT | colspan_flag_PERCENT_GROUP;
	/* Remember what %age the user asked for here. Later, we
	   will normalise this slot and use this and RAW_MIN
	   values to calculate actual pixel values for the PCT_MIN
	   slot. */
	widths[PCT_RAW] = ceil(vp->u.f);
	break;

    case value_relunit:
	flags = colspan_flag_RELATIVE | colspan_flag_RELATIVE_GROUP;
	/* need to scale all rel values so the smallest is an
	   integer, yuck need to do a pre-pass of some kind. */
	widths[REL_MIN] = ceil(vp->u.f); /* probably wrong for 0.5*!!! */
	break;
    }

    /* Might have already got some size information for a column
       header. */
    if (horiz)
    {
	if (widths[RAW_MIN] < cell->stream.width_info.minwidth)
	    widths[RAW_MIN] = cell->stream.width_info.minwidth;

	if (widths[RAW_MAX] < cell->stream.width_info.maxwidth)
	    widths[RAW_MAX] = cell->stream.width_info.maxwidth;

	FMTDBG(("prior to assert min %d, max %d - A\n", widths[RAW_MIN], widths[RAW_MAX]));
	ASSERT(widths[RAW_MIN] <= widths[RAW_MAX]);
    }
    else
    {
	if (widths[RAW_MIN] < - cell->stream.height )
	    widths[RAW_MIN] = widths[RAW_MAX] = - cell->stream.height;
    }

    flags |= colspan_flag_USED;
    group->flags |= flags;

    /* Flag all the columns covered */

    for (z = first; z <= last; z++)
    {
	/* Column head flags get the indication of what sort of
	   constraints contribute to them, but don;t get flagged as
	   being used just because they have received a constraint
	   contribution. */
	table->colspans[z].flags |= flags;
    }
}

/*****************************************************************************/

static void init_column(rid_table_item *table, BOOL horiz, rid_table_cell *cell, VALUE *vp, int scale_value)
{
    const int X = horiz ? cell->cell.x : cell->cell.y;
    unsigned int *flags = & table->colspans[X].flags;
    int *widths = table->colspans[X].width;
    int z;

    FMTDBGN(("init_column: id %d, %s, X %d\n",
	    table->idnum, HORIZVERT(horiz), X));

    switch (vp->type)
    {
    case value_absunit:
	*flags |= colspan_flag_ABSOLUTE | colspan_flag_ABSOLUTE_COL | colspan_flag_USED;
	z = (scale_value * ceil(vp->u.f)) / 100;
	/* DAF: 970708: account for height this way now */
	if (widths[RAW_MIN] < z)
	    widths[RAW_MIN] = widths[RAW_MAX] = z;
	if (widths[ABS_MAX] < z)
	    widths[ABS_MAX] = widths[ABS_MIN] = z;
	break;

    case value_pcunit:
	*flags |= colspan_flag_PERCENT | colspan_flag_PERCENT_COL | colspan_flag_USED;
	/* Remember what %age the user asked for here. Later, we
	   will normalise this slot and use this and RAW_MIN
	   values to calculate actual pixel values for the PCT_MIN
	   slot. */
	widths[PCT_RAW] = ceil(vp->u.f);
	break;

    case value_relunit:
	*flags |= colspan_flag_RELATIVE | colspan_flag_RELATIVE_COL | colspan_flag_USED;
	/* need to scale all rel values so the smallest is an
	   integer, yuck need to do a pre-pass of some kind. */
	widths[REL_MIN] = ceil(vp->u.f); /* probably wrong for 0.5*!!! */
	break;

    default:
	*flags |= colspan_flag_USED;
	break;
    }

    /* Might have already got some size information for a column
       header. */
    if (horiz)
    {
	if (widths[RAW_MIN] < cell->stream.width_info.minwidth)
	    widths[RAW_MIN] = cell->stream.width_info.minwidth;

	if (widths[RAW_MAX] < cell->stream.width_info.maxwidth)
	    widths[RAW_MAX] = cell->stream.width_info.maxwidth;

	FMTDBG(("prior to assert min %d, max %d - B\n", widths[RAW_MIN], widths[RAW_MAX]));

	if (widths[RAW_MIN] > widths[RAW_MAX])
	{
	    FMTDBG(("ASSERTION WOULD HAVE TRIGGERED\n"));
	}
#if 1
	ASSERT(widths[RAW_MIN] <= widths[RAW_MAX]);
#endif
	{ static int BORRIS_FIX_ME; }
    }
    else
    {
	if (widths[RAW_MIN] < - cell->stream.height )
	    widths[RAW_MIN] = widths[RAW_MAX] = - cell->stream.height;
    }
}

/*****************************************************************************/

static void colspan_init_structure1(rid_table_item *table, BOOL horiz, int scale_value)
{
    int x, y, iw;
    rid_table_cell *cell;
    const int max = horiz ? table->cells.x : table->cells.y;
    const int prop = horiz ? rid_PROP_WIDTH : rid_PROP_HEIGHT;
    int *width = HORIZCELLS(table,horiz);

    /* allocate and init storage (NB now for max+1 records inc dummy last column PCP) */
    pcp_cell the_cells = (pcp_cell) mm_calloc(max+1, sizeof (struct pcp_cell_s));

    /* pdh: we may get called twice in a row if autofit has occurred. The
     * passing-in of HORIZ is *fragile* and depends on only being called
     * twice-in-a-row (no intervening free_structure or recurse_format_table)
     * on the same case.
     */
    if ( table->colspans )
        colspan_free_structure( table, horiz );

    table->colspans = the_cells;

    for (x = 0; x < N_COLSPAN_WIDTHS; x++)
	width[x] = NOTINIT;

    /* Set all values to some sensible default */
    for (x = 0; x <= max; x++)	/* including dummy last column */
    {
	/*the_cells[x].first_start = NULL;*/
	/*the_cells[x].first_end = NULL;*/
	for (iw = 0; iw < N_COLSPAN_WIDTHS; iw++)
	    the_cells[x].width[iw] = NOTINIT;
	the_cells[x].leftmost = NOTINIT;
	the_cells[x].rightmost = NOTINIT;
	/*the_cells[x].flags = 0;*/
	the_cells[x].dad = NOTINIT;/* PCP added for new constraints connectivity analysis 11/5/97 */
    }

    /* Then reflect what the cells say */
    for (x = -1, y = 0; (cell = rid_next_root_cell (table, &x, &y)) != NULL; )
    {
	const int span = (horiz ? cell->span.x : cell->span.y);
	VALUE v;

	if (cell->flags & rid_cf_NOCONS)
	{
	    FMTDBG(("Cell %d,%d flagged NOCONS\n", cell->cell.x, cell->cell.y));
	    v.type = value_none;
	}
	else
	    rid_getprop(table, x, y, prop, &v);

	if (span > 1)
	    init_group(table, horiz, cell, &v, scale_value);
	else
	    init_column(table, horiz, cell, &v, scale_value);
    }

    FMTDBG(("colspan_init_structure1: the cells:\n"));
    /*colspan_trace_cells(table, horiz);*/
}

/*****************************************************************************

  Working horizontally, so clear a vertical strip.

 */

static void set_nocons_horiz(rid_table_item *table, int x, rid_cell_flags f)
{
    int y;

    FMTDBG(("set_nocons_horiz: %d %d %x\n", table->idnum, x, f));

    for (y = 0; y < table->cells.y; y++)
    {
	rid_table_cell *cell = *CELLFOR(table, x, y);
	if (cell != NULL && cell->span.x == 1 && ((cell->flags & f) != 0) )
	{
	    FMTDBG(("Narrow cell %d,%d stomped\n", cell->cell.x, cell->cell.y));
	    cell->flags |= rid_cf_NOCONS;
	}
    }

    /* Clear column header as well */
    table->colhdrs[x]->userwidth.type = value_none;
    /* Ditto for the group */
    table->colhdrs[x]->colgroup->userwidth.type = value_none;
}

static void set_nocons_vert(rid_table_item *table, int y, rid_cell_flags f)
{
    int x;

    for (x = 0; x < table->cells.x; x++)
    {
	rid_table_cell *cell = *CELLFOR(table, x, y);
	if (cell != NULL && cell->span.y == 1 && ((cell->flags & f) != 0) )
	{
	    FMTDBG(("Narrow cell %d,%d stomped\n", cell->cell.x, cell->cell.y));
	    cell->flags |= rid_cf_NOCONS;
	}
    }
}

static void set_nocons(rid_table_item *table, BOOL horiz, int x, rid_cell_flags f)
{
    if (horiz)
	set_nocons_horiz(table, x, f);
    else
	set_nocons_vert(table, x, f);
}

extern void colspan_do_the_stomping(rid_table_item *table, BOOL horiz)
{
    const int max = HORIZMAX(table, horiz);
    int x;
    pcp_cell the_cells = table->colspans;

    FMTDBG(("colspan_do_the_stomping: table %d\n", table->idnum));

    colspan_trace_cells(table, horiz);

    /* Clear old contribution flags */
    for (x = 0; x < max; x++)
    {
	the_cells[x].flags &= ~ (colspan_flag_RELATIVE | colspan_flag_PERCENT | colspan_flag_ABSOLUTE);
    }
    
    /* Now recalculate what contributions we have to give each
       column. Some contributions *will* find themselves stomped on
       now. */

    for (x = 0; x < max; x++)
    {
	BOOL stomped = FALSE;
	pcp_cell cell = &the_cells[x];
	pcp_group group = cell->first_start;
	
	if ( (cell->flags & colspan_flag_STOMP_ABSOLUTE) != 0 )
	{
	    set_nocons(table, horiz, x, rid_cf_ABSOLUTE);
	    FMTDBG(("col %d, abs, stomped %d\n", x, stomped));
	}
	
	if ( (cell->flags & colspan_flag_STOMP_RELATIVE) != 0 )
	{
	    set_nocons(table, horiz, x, rid_cf_RELATIVE);
	    FMTDBG(("col %d, rel, stomped %d\n", x, stomped));
	}
	
	if ( (cell->flags & colspan_flag_STOMP_PERCENT) != 0 )
	{
	    set_nocons(table, horiz, x, rid_cf_PERCENT);
	    FMTDBG(("col %d, pct, stomped %d\n", x, stomped));
	}
	
	while (group != NULL)
	{
	    int z;
	    stomped = FALSE;
	    
	    if ( (group->flags & colspan_flag_ABSOLUTE_GROUP) != 0 )
	    {
		for (z = x; !stomped && z <= group->iend; z++)
		    stomped = (the_cells[z].flags & colspan_flag_STOMP_ABSOLUTE) != 0;
	    }
	    else if ( (group->flags & colspan_flag_PERCENT_GROUP) != 0 )
	    {
		for (z = x; !stomped && z <= group->iend; z++)
		    stomped = (the_cells[z].flags & colspan_flag_STOMP_PERCENT) != 0;
	    }
	    else if ( (group->flags & colspan_flag_RELATIVE_GROUP) != 0 )
	    {
		for (z = x; !stomped && z <= group->iend; z++)
		    stomped = (the_cells[z].flags & colspan_flag_STOMP_RELATIVE) != 0;
	    }
	    
	    if (stomped)
	    {
		FMTDBG(("Wide cell %d,%d stomped\n", group->table_cell->cell.x, group->table_cell->cell.y));
		group->table_cell->flags |= rid_cf_NOCONS;
	    }
	    
	    group = group->next_start;
	}
    }
    FMTDBG(("colspan_do_the_stomping: finished table %d\n", table->idnum));
}


static BOOL have_contradictions(rid_table_item *table, BOOL horiz)
{
    int x;
    pcp_cell the_cells = table->colspans;
    const int max = horiz ? table->cells.x : table->cells.y;
    int worth_recalculating = 0;

    /* Now look for contradictory constraints of different types and
       remove them. Individual constrain override flags are used. */

    for (x = 0; x < max; x++)
    {
	int unsigned clash = 0;

	switch ( the_cells[x].flags & ( colspan_flag_RELATIVE | colspan_flag_PERCENT | colspan_flag_ABSOLUTE ) )
	{
	case 0:
	case colspan_flag_ABSOLUTE:
	case colspan_flag_PERCENT:
	case colspan_flag_RELATIVE:
	    /* Not more than one constraint - no clash */
	    break;

	case colspan_flag_PERCENT | colspan_flag_ABSOLUTE:
	    clash = colspan_flag_STOMP_PERCENT;
	    worth_recalculating++;
	    FMTDBG(("have_contradictions: col %d: pct:abs\n", x));
	    break;

	case colspan_flag_RELATIVE | colspan_flag_ABSOLUTE:
	case colspan_flag_RELATIVE | colspan_flag_PERCENT:
	    clash = colspan_flag_STOMP_RELATIVE;
	    worth_recalculating++;
	    FMTDBG(("have_contradictions: col %d: rel:abs or rel:pct\n", x));
	    break;

	case colspan_flag_RELATIVE | colspan_flag_PERCENT | colspan_flag_ABSOLUTE:
	    clash = colspan_flag_STOMP_PERCENT |  colspan_flag_STOMP_RELATIVE;
	    FMTDBG(("have_contradictions: col %d: rel:pct:abs\n", x));
	    worth_recalculating++;
	    break;
	}

	the_cells[x].flags |= clash;
    }

    if (worth_recalculating > 0)
    {
	FMTDBG(("colspan_init_structure: worth_recalculating is %d: MARKING\n", worth_recalculating));

	colspan_do_the_stomping(table, horiz);
    }

    FMTDBG(("have_contradictions: worth_recalculating is %d\n", worth_recalculating));

    return worth_recalculating > 0;
}

/*****************************************************************************/

/*****************************************************************************

  Initialise a colspan structure ready for manipulating. This is used
  for both horizontal and vertical operation. Initially, we do all the
  horizontal operations, deriving final stream format widths and hence
  an actual height for the contents. We then reuse most of the
  operations to share out these heights where ROWSPAN=N or HEIGHT=N
  has been used.

  NOTE: colspan_flag_USED should be used to determine whether a column
  header value is supplied from the user or inferred from surrounding
  information.

  This version takes what we consider to be a simplistic approach to
  constraints and overrides percentage constraints by absolute
  constraints and relative constraints by percentage constraints or
  absolute constraints. This extends over the entire group, if
  applicable. This means we end up with each column receiving ZOm user
  constraints, and no groups have holes in them.

*/

extern void colspan_init_structure(rid_table_item *table, BOOL horiz, int scale_value)
{
    colspan_init_structure1(table, horiz, scale_value);

    if ( have_contradictions(table, horiz) )
    {
	/* have_contradictions will have stomped constraints as
           necessary. */
	FMTDBG(("\nRebuilding colspan structure with some constraints overridden\n\n"));
	colspan_free_structure(table, horiz);
	colspan_init_structure1(table, horiz, scale_value);
colspan_trace_cells(table,horiz);
	/* Should have broken ALL clashes */
	ASSERT( ! have_contradictions(table, horiz) );
    }

    /* trace it */
    FMTDBG(("\nNewly initialised colspan structure:\n"));
    /*colspan_trace_cells (table, horiz);*/
}

/*****************************************************************************/

static BOOL notch(rid_table_item *table,
		  BOOL horiz,
		  width_array_e slot,
		  unsigned int AND,
		  unsigned int EQL)
{
    const int max = HORIZMAX(table, horiz);
    int x;
    pcp_cell cell = table->colspans;
    int matches = 0;

    for (x = 0; x < max; x++, cell++)
    {
	cell->width[slot] = abs(cell->width[slot]);

	if ( (cell->flags & AND) != EQL )
	    cell->width[slot] *= -1;
	else
	    matches++;
    }

    /* Say whether we having something active (ie to work on) */
    return matches > 0;
}

/*****************************************************************************

  We are going to be adding upto 'slop' pixels onto the 'data' slot
  until the values in the 'control' are reached. The ratio of data to
  control values is used to control the distribution of these
  pixels. The 'data' and 'control' columns can start with arbitary
  relationships and must be sensibly handled.

  Only items in the data slot with >0 value are considered.

  We might notch out other values as we progress.

  forced is used to force us to add when data has reached control. It
  causes us to add in the ratio of minwidth to maxwidth.

  Return how many slop pixels we did not use.

 */

#ifndef FRESCO
static int proportional_slop_share(rid_table_item *table,
				   BOOL horiz,
				   width_array_e min_slot,
				   width_array_e max_slot,
				   width_array_e fallback_slot,
				   int slop)
{
    const int max = HORIZMAX(table, horiz);
    pcp_cell cells = table->colspans;
    int *realmin = mm_calloc(sizeof(int*), max);
    int *master = HORIZCELLS(table,horiz);

    int FROM = 0;
    int TO = 0;

    int N;
    int W;
    int D;
    int x;

    FMTDBG(("proportional_slop_share(%p, %s, min %s, max %s, fall %s, slop %d)\n",
	    table,
	    HORIZVERT(horiz),
	    WIDTH_NAMES[min_slot],
	    WIDTH_NAMES[max_slot],
	    WIDTH_NAMES[fallback_slot],
	    slop));

    FMTDBG(("Looking for real growth potential in %s\n", WIDTH_NAMES[min_slot]));
    for (N = 0, x = 0; x < max; x++)
    {
	const int min = cells[x].width[min_slot];
	if ( min > 0 )
	{
	    const int to = cells[x].width[max_slot];
	    if (min < to)
	    {
		realmin[x] = min;
		N++;
	    }
	}
    }
    FMTDBG(("%d items have real growth potential\n", N));

    if (N == 0)
    {
	FMTDBG(("Looking for items with min/max width difference to grow in %s\n", WIDTH_NAMES[fallback_slot]));

	for (x = 0; x < max; x++)
	{
	    const int from = cells[x].width[min_slot];
	    if ( from > 0 )
	    {
		const int to = cells[x].width[max_slot];
		const int min = cells[x].width[fallback_slot];
		if (min < to)
		{
		    realmin[x] = min;
		    N++;
		}
	    }
	}
    }
    FMTDBG(("%d items have min/max difference\n", N));

    /* Might wish to prevent this for more precise control? */
    if (N == 0)
    {
	FMTDBG(("Restoring to pretending minwidth is 0 against maxwidth %s\n", WIDTH_NAMES[max_slot]));

	for (x = 0; x < max; x++)
	{
	    const int from = cells[x].width[min_slot];
	    if ( from > 0 )
	    {
		realmin[x] = 1;
		N++;
	    }
	}
    }

    if (N == 0)
    {
	FMTDBG(("Couldn't find any items whatsoever!\n"));
	mm_free(realmin);
	return slop;
    }

    FMTDBG(("Now getting scaling values\n"));

    for (N = 0, x = 0; x < max; x++)
    {
	const int from = cells[x].width[min_slot];

	if ( from > 0 )
	{
	    const int min = realmin[x];
	    const int to = cells[x].width[max_slot];

	    if (min >= to)
	    {
		/* Already indivudually met - notch out */
		cells[x].width[min_slot] = -from;
	    }
	    else
	    {
		/* Note some information */
		FROM += min;
		TO += to;
		N++;
	    }
	}
    }

    ASSERT(N != 0);

    if (N == 0)
	return slop;

    W = slop;
    D = TO - FROM;

    ASSERT(D > 0);
    ASSERT(W > 0);

    for (N = 0, x = 0; x < max; x++)
    {
	const int from = cells[x].width[min_slot];
	if ( from > 0 )
	{
	    const int min = realmin[x];
	    const int to = cells[x].width[max_slot];
	    const int d = to - min;
	    const int plus = (d * W) / D;
	    cells[x].width[min_slot] += plus;
	    N += plus;
	    FMTDBG(("%d slop to column %d, now %d\n", plus, x,cells[x].width[min_slot]));
	}
    }

    if (N < slop)
    {
	FMTDBG(("%d pixels in rounding errors being carried over\n", slop - N));
    }

    ASSERT(N <= slop);

    master [min_slot] += N;

    mm_free(realmin);

    return slop - N;
}

/*****************************************************************************

  Similar to proportional, except we have already met the maximum criteria and so use the

 */

static void padding_only_share(rid_table_item *table,
			       BOOL horiz,
			       int slop,
			       constraints_summary *csumm)
{
    while (slop > 0)		/* you don't want to do it like THAT! PCP 27/4/97 */
    {
	const int last_slop = slop;

	slop = proportional_slop_share(table, horiz, ACTUAL, LAST_MAX, RAW_MIN, slop);

	if (last_slop == slop)
	{
	    FMTDBG(("%d slop pixels won't share\n", slop));
	    break;
	}
    }

}

/*****************************************************************************

  Add space only to columns with an absolute width specification. May
  or may not satisfy all the absolute width constraints.

 */

static int attempt_meet_abs_min(rid_table_item *table, BOOL horiz, int slop, constraints_summary *csumm)
{
    if (csumm->with_absolute != 0)
    {
	if ( notch(table, horiz, ACTUAL, colspan_flag_ABSOLUTE, colspan_flag_ABSOLUTE) )
	    slop = proportional_slop_share(table, horiz, ACTUAL, ABS_MIN, RAW_MIN, slop);
    }

    return slop;
}

/*****************************************************************************

  Add space only to columns with a percetnage width specification. May
  or may not satisfy all these.

 */

static int attempt_meet_pct_min(rid_table_item *table, BOOL horiz, int slop, constraints_summary *csumm)
{
    if (csumm->with_percent != 0)
    {
	if ( notch(table, horiz, ACTUAL, colspan_flag_PERCENT, colspan_flag_PERCENT) )
	    slop = proportional_slop_share(table, horiz, ACTUAL, PCT_MIN, ABS_MIN, slop);
    }

    return slop;
}

/*****************************************************************************/

static int attempt_meet_rel_min(rid_table_item *table, BOOL horiz, int slop, constraints_summary *csumm)
{
    return slop;
}

/*****************************************************************************/

static int attempt_meet_abs_max(rid_table_item *table, BOOL horiz, int slop, constraints_summary *csumm)
{
    return slop;
}

/*****************************************************************************/

static int attempt_meet_pct_max(rid_table_item *table, BOOL horiz, int slop, constraints_summary *csumm)
{
    return slop;
}

/*****************************************************************************/

static int attempt_meet_rel_max(rid_table_item *table, BOOL horiz, int slop, constraints_summary *csumm)
{
    return slop;
}
#endif

/*****************************************************************************

  pdh: share out space according to width[A] - width[B], B=WIDTH_ZERO is a
  special value meaning just use width[A]       */

static int sharing_pass( pcp_cell the_cells, int max,
                         width_array_e A, width_array_e B, int slop )
{
    int total_excess = 0;
    int x;
    int left = slop;
    int my_excess, want, make_me_max;

/*    FMTDBG(("sharing_pass(%s-%s) slop=%d:\n", WIDTH_NAMES[A], WIDTH_NAMES[B],
            slop));
	    */
    for (x = 0; x < max; x++)
        if ( (the_cells[x].flags & (colspan_flag_ABSOLUTE | colspan_flag_PERCENT)) == 0 )
        {
            my_excess =   the_cells[x].width[A]
	                - ( (B==WIDTH_ZERO) ? 0 : the_cells[x].width[B] );
            if ( my_excess > 0 )
                total_excess += my_excess;
	}
    FMTDBG(("  total_excess=%d\n", total_excess));

    if ( total_excess == 0 )
        return slop;

    for (x = 0; left > 0 && x < max; x++)
	if ( (the_cells[x].flags & (colspan_flag_ABSOLUTE | colspan_flag_PERCENT)) == 0 )
	{
	    my_excess = the_cells[x].width[A]
	                - ( (B==WIDTH_ZERO) ? 0 : the_cells[x].width[B] );
	    want = (slop * my_excess) / total_excess;
	    make_me_max = the_cells[x].width[A]
	                            - the_cells[x].width[ACTUAL];

            FMTDBG(("  col %d: my_xs=%d, want=%d, makemax=%d: ", x, my_excess, want, make_me_max ));

            if ( make_me_max < 0 )
                make_me_max = 0;

            if ( B != WIDTH_ZERO && want > make_me_max )
                want = make_me_max;

            if ( my_excess > 0 )
            {
                the_cells[x].width[ACTUAL] += want;
    	        left -= want;
    	        FMTDBG(("adding %d, left now %d\n",want,left));
    	    }
    	    else
    	        FMTDBG(("leaving alone\n"));
	}

    return left;
}

/*****************************************************************************

  'slop' pixels MUST be shared out into the columns. We prefer to
  always do this in unconstrained columns. If there are no such
  columns, some other choice of where to put the space is
  required.

  pdh: three sharing passes now
    (1) share out according to RAW_MAX-RAW_MIN  (sorts out simple cases)
    (2) share out according to ABS_MAX-ABS_MIN  (sorts out colspan>1 cases)
    (3) share out according to ABS_MAX
  The final pass sorts out cases where there's quite simply too much space,
  eg. <table width=100%>[contents not very wide]

    */

static void share_raw_abs_pct(rid_table_item *table, BOOL horiz, int slop)
{
    const int max = HORIZMAX(table, horiz);
    int x;
    BOOL any_uncons = FALSE;
    pcp_cell the_cells = table->colspans;

    for (x = 0; ! any_uncons && x < max; x++)
	any_uncons = (the_cells[x].flags & (colspan_flag_ABSOLUTE | colspan_flag_PERCENT)) == 0;

    FMTDBG(("share_raw_abs_pct: %s, id %d, slop %d, any_uncons %d, max %d\n",
	    HORIZVERT(horiz), table->idnum, slop, any_uncons, max));

    /* What else can we do? */
    if (max == 0)
	return;

    if (any_uncons)
    {
	    slop = sharing_pass( the_cells, max, RAW_MAX, RAW_MIN, slop );
	ASSERT( slop >= 0 );
	if ( slop )
	    slop = sharing_pass( the_cells, max, ABS_MAX, ABS_MIN, slop );
        ASSERT( slop >= 0 );
        if ( slop )
            slop = sharing_pass( the_cells, max, ABS_MAX, WIDTH_ZERO, slop );
    }

    ASSERT( slop >= 0 );

    if ( slop > 0 )
        FMTDBG(("share_raw_abs_pct: sharing %d at last resort\n", slop));

    /* I know! */
    while (slop > 0)
	for (x = 0; slop > 0 && x < max; x++)
	    the_cells[x].width[ACTUAL]++, slop--;

    ASSERT(slop == 0);
}


/*****************************************************************************

  See how many more constraints we can satisfy to use up spare space.

  Then distribute the remaining slop in a fair fashion.

  fwidth still has borders

  */

#ifndef FRESCO
static void most_constraints_then_share_fairly(rid_table_item *table,
					       int best,
					       int fwidth,
					       BOOL horiz)
{
    int *width = HORIZCELLS(table,horiz);
    int slop = (fwidth - width[ACTUAL]);
    constraints_summary csumm;

    /* need to share out extra space and try to satisfy a few extra
       constraints (if there are any, and it's possible).  slop is
       remaining space still to share out */

    FMTDBG(("\nAdd %d slop pixels onto %s slot, fwidth %d, %s\n\n",
	    slop, WIDTH_NAMES[best], fwidth, HORIZVERT(horiz)));

    ASSERT(slop >= 0);

    generate_constraints_summary(table, horiz, &csumm);

    /* Either use REL_MIN or ABS_MAX as appropriate */
    ASSERT(best != RAW_MAX);

    /*slop -= TABLE_OUTSIDE_BIAS(table);*/

    FMTDBG(("Reduced slop to %d pixels, after account for border\n", slop));
    ASSERT(slop >= 0);

    /* Urm, this is a bit blunt! */

    switch (best)
    {
    case RAW_MIN:
	slop = attempt_meet_abs_min(table, horiz, slop, &csumm);
	if (slop == 0)
	{
	    FMTDBG(("Shared all space out without exceeding ABS_MIN criteria\n"));
	    break;
	}
	/* Deliberate fall though */
    case ABS_MIN:
	FMTDBG(("Met ABS_MIN criteria, %d slop pixels left\n", slop));
	/* We cannot grow the table to meet the percentage implied
	   widths with the fwidth value given. Grow roughly in
	   proportion to the percentage distribution, although we know
	   one or more column can never reach this fully. */
	slop = attempt_meet_pct_min(table, horiz, slop, &csumm);
	if (slop == 0)
	{
	    FMTDBG(("Shared all space out without exceeding PCT_MIN criteria\n"));
	    break;
	}
	/* Deliberate fall through */
    case PCT_MIN:
	FMTDBG(("Met PCT_MIN criteria, %d slop pixels left\n", slop));
	slop = attempt_meet_rel_min(table, horiz, slop, &csumm);
	if (slop == 0)
	{
	    FMTDBG(("Shared all space out without exceeding REL_MIN criteria\n"));
	    break;
	}
	/* Deliberate fall through */
    case REL_MIN:
	FMTDBG(("Met REL_MIN criteria, %d slop pixels left\n", slop));
	slop = attempt_meet_abs_max(table, horiz, slop, &csumm);
	if (slop == 0)
	{
	    FMTDBG(("Shared all space out without exceeding ABS_MAX criteria\n"));
	    break;
	}
	/* Deliberate fall through */
    case RAW_MAX:
	/* Deliberate fall through */
    case ABS_MAX:
	FMTDBG(("Met ABS_MAX criteria, %d slop pixels left\n", slop));
	slop = attempt_meet_pct_max(table, horiz, slop, &csumm);
	if (slop == 0)
	{
	    FMTDBG(("Shared all space out without exceeding PCT_MAX criteria\n"));
	    break;
	}
	/* Deliberate fall through */
    case PCT_MAX:
	FMTDBG(("Met PCT_MAX criteria, %d slop pixels left\n", slop));
	slop = attempt_meet_rel_max(table, horiz, slop, &csumm);
	if (slop == 0)
	{
	    FMTDBG(("Shared all space out without exceeding REL_MAX criteria\n"));
	    break;
	}
	/* Deliberate fall through */
    case REL_MAX:
	FMTDBG(("Met ALL criteria, %d slo pixels still left\n", slop));
	padding_only_share(table, horiz, slop, &csumm);
	break;
    }

    return;
}
#endif

/*****************************************************************************/

static void reflect_into_table(rid_table_item *table, BOOL horiz)
{
    pcp_cell the_cells = table->colspans;
    int t, i, x, y;
    rid_table_cell *cell;
    const int cb = TABLE_INSIDE_BIAS(table);
    int *master = HORIZCELLS(table,horiz);

    FMTDBG(("reflect_into_table: id %d, %s\n", table->idnum, HORIZVERT(horiz)));
    /*dump_table(table, NULL);*/

    if (horiz)
    {
	y = table->border;
	if (table->caption)
	{
	    table->caption->off.x = y;
	    table->caption->size.x = table->caption->stream.fwidth; /* ?? */
	    FMTDBG(("reflect_into_table: pickup caption size.x %d\n", table->caption->size.x));
	}
	/* Set the horizontal offset for each column */
	for (x = 0; x < table->cells.x; x++)
	{
	    table->colhdrs[x]->offx = y;
	    table->colhdrs[x]->sizex = the_cells[x].width[ACTUAL];
	    y += table->colhdrs[x]->sizex;
	}
    }
    else
    {
	y = table->border;

	if (table->caption && table->caption->calign != rid_ct_BOTTOM)
	{
	    y += table->cellspacing + table->cellpadding;
	    table->caption->off.y = -y;
	    FMTDBG(("reflect_into_table: set off.y to %d\n", table->caption->off.y));
	    y += (table->caption->size.y = -table->caption->stream.height);
	    y += table->cellspacing + table->cellpadding;
	    FMTDBG(("reflect_into_table: top caption height %d\n", table->caption->stream.height));
	}

	/* Set the vertical offset for each column */
	for (x = 0; x < table->cells.y; x++)
	{
	    table->rowhdrs[x]->offy = -y;
	    table->rowhdrs[x]->sizey = the_cells[x].width[ACTUAL];
	    y += table->rowhdrs[x]->sizey;
	}

	if (table->caption && table->caption->calign == rid_ct_BOTTOM)
	{
	    y += 2 * table->cellspacing + 2 * table->cellpadding;
	    table->caption->off.y = -y;
	    table->caption->size.y = -table->caption->stream.height;
	    FMTDBG(("reflect_into_table: set off.y to %d\n", table->caption->off.y));
	    FMTDBG(("reflect_into_table: bottom caption height %d\n", table->caption->stream.height));
	}

    }


    /* Sum up cell sizes. Can probably do this better! */
    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
	if (horiz)
	{
	    for (t = 0, i = cell->cell.x; i < cell->cell.x + cell->span.x; i++)
		t += table->colhdrs[i]->sizex;
	    cell->size.x = t;
	    cell->stream.fwidth = t - cb;
	    if (cell->span.x == 0)
	    {
#ifdef PLOTCHECK
		plotcheck_boom();
#endif
	    }
	    else
	    {
		/* @@@@ */
		ASSERT(cell->stream.fwidth >= 0);
		/*cell->stream.fwidth = 0;*/
	    }
	}
	else
	{
	    for (t = 0, i = cell->cell.y; i < cell->cell.y + cell->span.y; i++)
		t += table->rowhdrs[i]->sizey;
	    cell->size.y = t;

	    if (cell->span.y == 0)
	    {
#ifdef PLOTCHECK
		plotcheck_boom();
#endif
	    }
	    else
	    {
		ASSERT(cell->stream.fwidth >= 0);
	    }
	}
    }

    if (horiz)
    {
	FMTDBG(("offx values : "));
	for (x = 0; x < table->cells.x; x++)
	{
	    FMTDBG(("%3d ", table->colhdrs[x]->offx));
	}
	FMTDBG(("\n"));

	FMTDBG(("sizex values: "));
	for (x = 0; x < table->cells.x; x++)
	{
	    FMTDBG(("%3d ", table->colhdrs[x]->sizex));
	}
	FMTDBG(("\n"));
    }
    else
    {
	FMTDBG(("offy values : "));
	for (x = 0; x < table->cells.y; x++)
	{
	    FMTDBG(("%3d ", table->rowhdrs[x]->offy));
	}
	FMTDBG(("\n"));

	FMTDBG(("sizey values: "));
	for (x = 0; x < table->cells.y; x++)
	{
	    FMTDBG(("%3d ", table->rowhdrs[x]->sizey));
	}
	FMTDBG(("\n"));
    }

    if (horiz)
	table->size.x = master[ACTUAL];
    else
	table->size.y = master[ACTUAL];
}

/*****************************************************************************

  Perform ALIGN and VALIGN positioning of cell streams within the
  eventual space allocated to the cells.

  */

#ifndef FRESCO
extern void format_position_table_cells(rid_table_item *table)
{
}
#endif

/*****************************************************************************/

#ifndef FRESCO
static width_array_e choose_best_slot(rid_table_item *table,
				      BOOL horiz,
				      int fwidth)
{
    width_array_e best = 0;
    int *master = HORIZCELLS(table,horiz);
    const rid_table_flags flag = horiz ? rid_tf_HAVE_WIDTH : rid_tf_HAVE_HEIGHT;

    if ( table->flags & flag )
	best = RAW_MIN;
    else  if (fwidth >= master[REL_MAX])
	best = REL_MAX;
    else if (fwidth >= master[PCT_MAX])
	best = PCT_MAX;
    else if (fwidth >= master[ABS_MAX])
	best = ABS_MAX;
/*  else if (fwidth >= master[RAW_MAX])
	best = RAW_MAX;*/
    else if (fwidth >= master[REL_MIN])
	best = REL_MIN;
    else if (fwidth >= master[PCT_MIN])
	best = PCT_MIN;
    else if (fwidth >= master[ABS_MIN])
	best = ABS_MIN;
    else
	best = RAW_MIN;

    return best;
}
#endif

/*****************************************************************************

  Given an fwidth, increase the size of the percentage columns to
  represent their share of this fwidth. We might have some spare
  pixels of error. We need to ensure that minwidth values are always
  met. Return the number of pixels left to share out: this is the
  total width minus all sizes allocated to percentages minus all
  absolute sizes minus min sizes for unconstrained columns.

 */

static int reflect_percentages(rid_table_item *table, BOOL horiz, int fwidth)
{
    const int max = HORIZMAX(table, horiz);
    int used = 0;
    pcp_cell the_cells = table->colspans;
    int *master = HORIZCELLS(table, horiz);
    int x;

    fwidth -= TABLE_OUTSIDE_BIAS(table);

    FMTDBG(("reflect_percentages: table %d, %s, inner fwidth %d\n",
	    table->idnum, HORIZVERT(horiz), fwidth));

    ASSERT(fwidth >= 0);

    if (master[PCT_RAW] == 100)
    {
	/* Fudge for things we ended up treating as WIDTH=0%  */
	for (x = 0; x < max; x++)
	{
	    if (the_cells[x].width[PCT_RAW] == 0)
		fwidth -= the_cells[x].width[ACTUAL];
	}
    }

    FMTDBG(("reflect_percentages: fwidth reduction leaves fwidth as %d\n", fwidth));

    for (x = 0; x < max; x++)
    {
	if ( (the_cells[x].flags & colspan_flag_PERCENT) != 0 )
	{
	    if ( the_cells[x].width[PCT_RAW] != 0 )
	    {
		const int want = (fwidth * the_cells[x].width[PCT_RAW]) / 100;
		if (want <= the_cells[x].width[ACTUAL])
		{
		    used += the_cells[x].width[ACTUAL];
		    FMTDBG(("reflect_percentages: want %d smaller actual %d, using actual\n",
			    want, the_cells[x].width[ACTUAL]));
		}
		else
		{
		    the_cells[x].width[ACTUAL] = want;
		    used += want;
		    FMTDBG(("reflect_percentages: using want %d\n", want));
		}
	    }
	    /* else done in 1st loop - UNLESS PCT_RAW < 100 */
	    else if (master[PCT_RAW] < 100)
	    {
		FMTDBG(("reflect_percentages: zero percent - width %d\n", the_cells[x].width[ACTUAL]));
		used += the_cells[x].width[ACTUAL];
	    }
	}
	else
	{
	    FMTDBG(("reflect_percentages: non percent - width %d\n", the_cells[x].width[ACTUAL]));
	    used += the_cells[x].width[ACTUAL];
	}
    }

    /* Columns with percentage contributions now have their FINAL
       sizes. The only reason we might add to a percentage column is
       if we have NOT been able to meet all percentage criteria, and
       hence have not called this function. URM: except for rounding
       errors! */
    FMTDBG(("reflect_percentages: reflected fwidth %d into ACTUAL with %d slop\n", fwidth, fwidth - used));

    ASSERT(used <= fwidth);

    colspan_trace_cells(table, horiz);

    return fwidth - used;
}

/*****************************************************************************

  shares out extra space and sets ACTUAL fields in base column records
  to actual position of right cell boundary.

  Remember, the value in ACTUAL fields and the fwidth given include
  TABLE_OUTSIDE_BIAS.

  FIRST_MIN: FIRST_MIN > fwidth ? FIRST_MIN : fwidth;

  fwidth

  LAST_MAX: sized ? sized size : LAST_MAX
*/

extern void colspan_share_extra_space (rid_table_item *table,
				       int fwidth,
				       BOOL horiz,
				       int scale_value)
{
    int *master = HORIZCELLS(table, horiz);
    int user_width = 0;
    int slop = 0;

    FMTDBG(("colspan_share_extra_space: fwidth %d, %s on table %d\n", fwidth, HORIZVERT(horiz), table->idnum));

    if (horiz)
    {
	switch (table->userwidth.type)
	{
	case value_absunit:
	    FMTDBG(("colspan_share_extra_space: abs userwidth is present\n"));
	    if (table->flags & rid_tf_HAVE_WIDTH)
	    {
		user_width = (scale_value * ceil(table->userwidth.u.f)) / 100;
		if (user_width < table->hwidth[REL_MIN])
		    user_width = table->hwidth[REL_MIN];

#if 0
		/* pdh: clip this if necessary DAF: You can't just do
		   this - no check against real minwidth to start
		   with. Plus it's not the behaviour we want, plus
		   there should be a nesting level check.  */
		if ( gbf_active( GBF_AUTOFIT )
		     && user_width > fwidth )
		{
		    FMTDBG(("tab%p: user_width %d > fwidth %d, shrinking\n",
		            table, user_width, fwidth ));
		    user_width = fwidth;
		}
#endif
		fwidth = user_width;

		FMTDBG(("colspan_share_extra_space: still active - new fwidth %d\n", fwidth));
	    }
	    else
	    {
		FMTDBG(("colspan_share_extra_space: been stomped - ignoring\n"));
		user_width = fwidth = master[RAW_MIN];
	    }
 	    break;

	case value_pcunit:
	    FMTDBG(("colspan_share_extra_space: pct userwidth is present\n"));
	    user_width = ceil( (table->userwidth.u.f * fwidth ) / 100.0 );
	    if (user_width >= master[RAW_MIN]) /* Should be... */
	    {
		FMTDBG(("colspan_share_extra_space: user_width >= master[RAW_MIN]\n"));
		fwidth = user_width;
	    }
	    else
	    {
		FMTDBG(("colspan_share_extra_space: can't use it - STRANGE\n"));
		user_width = fwidth = master[RAW_MIN];
	    }
	    break;
	}
    }
    else
    {
	switch (table->userheight.type)
	{
	case value_absunit:
	    user_width = (scale_value * ceil(table->userheight.u.f)) / 100;
	    break;

	case value_pcunit:
	    user_width = ceil( (table->userheight.u.f * fwidth ) / 100.0 );
	    break;
	}
    }

    /* Not correct interpretation for heights? */
    if (horiz)
    {
    }
    else
    {
	if (user_width > master[RAW_MIN])
	    fwidth = user_width;
	else
	    fwidth = master[RAW_MIN];
    }

    /*colspan_trace_cells(table, horiz);*/
    FMTDBG(("colspan_share_extra_space: table %d, user_width %d, fwidth %d, slop %d\n",
	    table->idnum, user_width, fwidth, slop));

    if (table->flags & rid_tf_HAVE_WIDTH)
	FMTDBG(("colspan_share_extra_space: have width\n"));
    if (table->flags & rid_tf_HAVE_HEIGHT)
	FMTDBG(("colspan_share_extra_space: have height\n"));

    /* Experiment by Dave */
	colspan_algorithm(table, RAW_MIN, horiz);
	colspan_column_init_from_leftmost(table, RAW_MIN, horiz);
	colspan_column_and_eql_copy(table, horiz, ACTUAL, 0,0, RAW_MIN);

#if 0
    if (fwidth < master[PCT_MIN])
    {
	FMTDBG(("colspan_share_extra_space: forcing PCT_MIN of %d\n", master[PCT_MIN]));
	colspan_algorithm(table, PCT_MIN, horiz);
	colspan_column_init_from_leftmost(table, PCT_MIN, horiz);
	colspan_column_and_eql_copy(table, horiz, ACTUAL, 0,0, PCT_MIN);
	slop = 0;
	fwidth = master[PCT_MIN];
    }
    else
    {
	if (fwidth > master[LAST_MAX] && user_width == 0)
	    fwidth = master[LAST_MAX];
	colspan_column_and_eql_copy(table, horiz, ACTUAL, 0,0, ABS_MIN);
	slop = reflect_percentages(table, horiz, fwidth);
	FMTDBG(("colspan_share_extra_space: can do PCT_MIN: slop %d\n", slop));
    }
#else
    colspan_column_and_eql_copy(table, horiz, ACTUAL, 0,0, ABS_MIN);
	
    if (fwidth < master[PCT_MIN])
    {
	FMTDBG(("colspan_share_extra_space: forcing PCT_MIN of %d\n", master[PCT_MIN]));
	fwidth = master[PCT_MIN];
    }
    else
    {
	if (fwidth > master[LAST_MAX] && user_width == 0)
	    fwidth = master[LAST_MAX];
	FMTDBG(("colspan_share_extra_space: can do PCT_MIN: slop %d\n", slop));
    }

    if (horiz && table->caption != NULL)
    {
	const int x = table->caption->stream.width_info.minwidth + CAPTION_TOTAL_BIAS(table);
	if (x > fwidth)
	{
	    FMTDBG(("Caption minwidth %d overrides fwidth %d\n",
		    x, fwidth));
	    fwidth = x;
	}
	table->caption->stream.fwidth = fwidth - CAPTION_TOTAL_BIAS(table);
	FMTDBG(("colspan_share_extra_space: caption given fwidth %d\n",
		table->caption->stream.fwidth));
    }

    slop = reflect_percentages(table, horiz, fwidth);
#endif
    
    ASSERT(slop >= 0);
    
    if (slop > 0)
    {
	/* Share in order raw, abs then pct */
	share_raw_abs_pct(table, horiz, slop);
    }


    {
	const int max = HORIZMAX(table, horiz);
	int x, tot;

	for (x = 0, tot = 0; x < max; x++)
	    tot += table->colspans[x].width[ACTUAL];

	tot += TABLE_OUTSIDE_BIAS(table);
	tot = fwidth - tot;

	if (tot != 0)
	{
	    FMTDBG(("colspan_share_extra_space: SEEM TO HAVE %d PIXELS NOT ACCOUNTED FOR\n", tot));

	    share_raw_abs_pct(table, horiz, tot);
	}
    }


    master[ACTUAL] = fwidth;

    /* Reflect the values calculated in the existing colhdr structure. */
    reflect_into_table(table, horiz);

    FMTDBG(("colspan_share_extra_space: Table %d, final fwidth %d, done\n", table->idnum, fwidth));

    colspan_trace_cells(table, horiz);


}


/*****************************************************************************/

/* eof colspan.c */
