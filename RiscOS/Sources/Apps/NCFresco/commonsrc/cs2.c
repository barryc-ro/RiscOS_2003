/* -*-C-*- colspan.c
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Column sharing algorithm, designed by Patrick Campbell-Preston
 * (patrick@chaos.org.uk).
 *
 * Additional implementation by borris.
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

#ifdef PLOTCHECK
#include "rectplot.h"
#endif

#ifdef DEBUG
static const char * WIDTH_NAMES[N_COLSPAN_WIDTHS] = WIDTH_NAMES_ARRAY;
#endif

/*****************************************************************************

  This is the core of what Patrick designed. Borris made a couple of
  minor bug fixes.

  'slot' tells us which entry in the width array to use on this pass;
  return value is the calculated width for the whole table based on
  these particular numbers - this total *excludes* the
  TABLE_OUTSIDE_BIAS value.

 */

static void colspan_algorithmLR(rid_table_item *table, int slot, BOOL horiz)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x;
    pcp_cell the_cells = table->colspans;

    /*FMTDBG(("colspan_algorithm used %s (slot %s)\n", 
	    horiz ? "horizontally" : "vertically",
	    WIDTH_NAMES[slot]));*/

    /*colspan_trace_cells(table,horiz);*/

    if (max == 0)
	return;

    /* now do a left-to-right pass trying to set leftmosts */

    {
	int prev_leftmost = 0;

	for (x = 0; x < max; x++)
	{
	    pcp_cell cell = the_cells+x;
	    pcp_group group = cell->first_end;

	    /*ASSERT(cell->width[slot] != NOTINIT);*/ /* must be set by now */

	    /* initialise leftmost using min width of cell alone */
	    cell->leftmost = (prev_leftmost + cell->width[slot]);

	    /*FMTDBGN(("leftmosting cell %d, prev leftmost %d, own width[%s] = %d\n",
		    x, prev_leftmost, WIDTH_NAMES[slot], cell->width[slot]));*/

	    /* Run down groups ending here. */
	    while (group != NULL)
	    {
		/* update leftmost if the width of this group on
		   top of the leftmost of its start (as already
		   calculated) forces it to be bigger */

		/* (the_cells[group->istart].leftmost + group->width[slot]); */

		/*const*/ int effective_start;
		/*const*/ int my_leftmost;

		effective_start = group->istart == 0 ? 0 : the_cells[group->istart -1].leftmost;
		my_leftmost = effective_start + group->width[slot];

		/*FMTDBGN(("  considering group start %d, start leftmost %d, group minwidth %d\n",
			 group->istart, effective_start, group->width[slot]));*/

		/*ASSERT(group->width[slot] != NOTINIT);*/ /* must be set by now or already screwed! */

		if (my_leftmost > cell->leftmost)
		{
		    /*FMTDBGN(("  updating leftmost to %d\n", my_leftmost));*/
		    cell->leftmost = my_leftmost;
		}

		group = group->next_end;
	    }
	      
	    prev_leftmost = cell->leftmost;

	    /*FMTDBGN(("leftmosted cell %d, leftmost is %d\n",
		    x, cell->leftmost));*/
	}
    }

    return;
}



static void colspan_algorithmRL(rid_table_item *table, int slot, BOOL horiz)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x;
    pcp_cell the_cells = table->colspans;

    /*FMTDBG(("colspan_algorithm used %s (slot %s)\n", 
	    horiz ? "horizontally" : "vertically",
	    WIDTH_NAMES[slot]));*/

    /*colspan_trace_cells(table,horiz);*/

    if (max == 0)
	return;

    /*FMTDBGN(("after l-to-r pass for leftmosts...\n"));*/
    /*colspan_trace_cells (table, horiz);*/

    /* now do a right-to-left pass trying to set rightmosts */
    /* NB we count rightmosts downwards from 0 = rhs and then add
       the appropriate total at the end so that they go upwards
       left-to-right from the lhs */

    {
	int prev_rightmost = 0;

	for (x =  max - 1; x >= 0; x--)
	{
	    pcp_cell cell = the_cells+x;
	    pcp_group group = cell->first_start;

	    /* initialise rightmost using min width of cell alone */
	    cell->rightmost = (prev_rightmost - cell->width[slot]);

	    /*FMTDBGN(("rightmosting cell %d, prev rightmost %d, own width[%s] = %d\n",
		    x, prev_rightmost, WIDTH_NAMES[slot], cell->width[slot]));*/

	    /* Run down groups starting here */
	    while (group != NULL)
	    {
		/* update rightmost if the width of this group on
		   top of the rightmost of its end (as already
		   calculated) forces it to be more negative */

		/*const*/ int effective = group->iend == max - 1 ? 0 : the_cells[group->iend].rightmost;
		/*const*/ int my_rightmost = effective - group->width[slot];

		/*FMTDBGN(("  considering group end %d, end rightmost %d, group width %d\n",
			group->iend, effective, group->width[slot]));*/

		if (my_rightmost < cell->rightmost)
		{
		    /*FMTDBGN(("  updating rightmost to %d\n", my_rightmost));*/
		    cell->rightmost = my_rightmost;
		}

		group = group->next_start;
	    }
	      
	    prev_rightmost = cell->rightmost;

	    /*FMTDBGN(("rightmosted cell %d, rightmost is %d\n",
		    x, cell->rightmost));*/
	}
    }
}

/*****************************************************************************/

extern int colspan_algorithm(rid_table_item *table, int slot, BOOL horiz)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x;
    pcp_cell the_cells = table->colspans;

    /*FMTDBG(("colspan_algorithm used %s (slot %s)\n", 
	    horiz ? "horizontally" : "vertically",
	    WIDTH_NAMES[slot]));*/

    /*colspan_trace_cells(table,horiz);*/

    if (max == 0)
	return 0;


    colspan_algorithmLR(table, slot, horiz);

    colspan_algorithmRL(table, slot, horiz);

    /* now do a left-to-right pass trying to set leftmosts */

#if 0
    {
	int prev_leftmost = 0;

	for (x = 0; x < max; x++)
	{
	    pcp_cell cell = the_cells+x;
	    pcp_group group = cell->first_end;

	    /*ASSERT(cell->width[slot] != NOTINIT);*/ /* must be set by now */

	    /* initialise leftmost using min width of cell alone */
	    cell->leftmost = (prev_leftmost + cell->width[slot]);

	    /*FMTDBGN(("leftmosting cell %d, prev leftmost %d, own width[%s] = %d\n",
		    x, prev_leftmost, WIDTH_NAMES[slot], cell->width[slot]));*/

	    /* Run down groups ending here. */
	    while (group != NULL)
	    {
		/* update leftmost if the width of this group on
		   top of the leftmost of its start (as already
		   calculated) forces it to be bigger */

		/* (the_cells[group->istart].leftmost + group->width[slot]); */

		/*const*/ int effective_start;
		/*const*/ int my_leftmost;

		effective_start = group->istart == 0 ? 0 : the_cells[group->istart -1].leftmost;
		my_leftmost = effective_start + group->width[slot];

		/*FMTDBGN(("  considering group start %d, start leftmost %d, group minwidth %d\n",
			 group->istart, effective_start, group->width[slot]));*/

		/*ASSERT(group->width[slot] != NOTINIT);*/ /* must be set by now or already screwed! */

		if (my_leftmost > cell->leftmost)
		{
		    /*FMTDBGN(("  updating leftmost to %d\n", my_leftmost));*/
		    cell->leftmost = my_leftmost;
		}

		group = group->next_end;
	    }
	      
	    prev_leftmost = cell->leftmost;

	    /*FMTDBGN(("leftmosted cell %d, leftmost is %d\n",
		    x, cell->leftmost));*/
	}
    }
#endif

    /*FMTDBGN(("after l-to-r pass for leftmosts...\n"));*/
    /*colspan_trace_cells (table, horiz);*/

    /* now do a right-to-left pass trying to set rightmosts */
    /* NB we count rightmosts downwards from 0 = rhs and then add
       the appropriate total at the end so that they go upwards
       left-to-right from the lhs */
#if 0
    {
	int prev_rightmost = 0;

	for (x =  max - 1; x >= 0; x--)
	{
	    pcp_cell cell = the_cells+x;
	    pcp_group group = cell->first_start;

	    /* initialise rightmost using min width of cell alone */
	    cell->rightmost = (prev_rightmost - cell->width[slot]);

	    /*FMTDBGN(("rightmosting cell %d, prev rightmost %d, own width[%s] = %d\n",
		    x, prev_rightmost, WIDTH_NAMES[slot], cell->width[slot]));*/

	    /* Run down groups starting here */
	    while (group != NULL)
	    {
		/* update rightmost if the width of this group on
		   top of the rightmost of its end (as already
		   calculated) forces it to be more negative */

		/*const*/ int effective = group->iend == max - 1 ? 0 : the_cells[group->iend].rightmost;
		/*const*/ int my_rightmost = effective - group->width[slot];

		/*FMTDBGN(("  considering group end %d, end rightmost %d, group width %d\n",
			group->iend, effective, group->width[slot]));*/

		if (my_rightmost < cell->rightmost)
		{
		    /*FMTDBGN(("  updating rightmost to %d\n", my_rightmost));*/
		    cell->rightmost = my_rightmost;
		}

		group = group->next_start;
	    }
	      
	    prev_rightmost = cell->rightmost;

	    /*FMTDBGN(("rightmosted cell %d, rightmost is %d\n",
		    x, cell->rightmost));*/
	}
    }
#endif
colspan_trace_cells(table, horiz);
    for (x = 0; x < (max - 1); x++)
    {
	the_cells[x].rightmost = (the_cells[x+1].rightmost + the_cells[max - 1].leftmost);
    }

    the_cells[max - 1].rightmost = the_cells[max - 1].leftmost;

    /*FMTDBGN(("\nafter r-to-l pass for rightmosts (ie final values for %s)...\n", 
	    WIDTH_NAMES[slot]));*/
    /*colspan_trace_cells (table, horiz);*/

    /* PCP has left the building */
    /*FMTDBG(("colspan_algorithm returning total %d for slot %s\n", 
	    the_cells[max - 1].leftmost, WIDTH_NAMES[slot]));*/

    return (the_cells[max - 1].leftmost);
}


/*****************************************************************************/

/* eof colspan.c */
