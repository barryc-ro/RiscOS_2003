/* -*-C-*- cs2.c
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

#ifdef PLOTCHECK
#include "rectplot.h"
#endif

#ifdef DEBUG
static const char * WIDTH_NAMES[N_COLSPAN_WIDTHS] = WIDTH_NAMES_ARRAY;
#endif

/*****************************************************************************

  This is the core of what Patrick designed. Borris made a couple of
  minor bug fixes. Then Patrick decided to rejig everything again.

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

    TASSERT (max > 0);

    /* do a left-to-right pass trying to set leftmosts */
    
    /* initialise the first leftmost which is the very start of the whole table */
    the_cells[0].leftmost = 0;

    /* loop through the rest of the cells */
    for (x = 0; x < max; x++)
    {
	pcp_cell cell = the_cells+x;
	pcp_cell next_cell = cell+1;
	pcp_group group = cell->first_end;
	
	ASSERT(cell->width[slot] != NOTINIT); /* must be set by now */
	
	/* initialise next cell's leftmost using min width of cell alone */
	next_cell->leftmost = (cell->leftmost + cell->width[slot]);
	
	/* Run down groups ending here. */
	while (group != NULL)
	{
	    /* update leftmost if the width of this group on top of the leftmost
	       of its start (as already calculated) forces it to be bigger */
	    
	    const int my_leftmost = the_cells[group->istart].leftmost + group->width[slot];

	    ASSERT(group->width[slot] != NOTINIT); /* must be set or already screwed! */

	    if (my_leftmost > next_cell->leftmost)
	    {
		/*FMTDBGN(("  updating leftmost to %d\n", my_leftmost));*/
		next_cell->leftmost = my_leftmost;
	    }
	    
	    group = group->next_end;
	}
	
    }

    /*FMTDBGN(("after l-to-r pass for leftmosts...\n"));*/
    /*colspan_trace_cells (table, horiz);*/

    return;
}



static void colspan_algorithmRL(rid_table_item *table, int slot, BOOL horiz)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x;
    pcp_cell the_cells = table->colspans;

    TASSERT (max > 0);

    /* now do a right-to-left pass trying to set rightmosts */
    /* NB we count rightmosts downwards from 0 = rhs and then add the
       appropriate total at the end so that they go upwards
       left-to-right from the lhs - NB each rightmost refers to the
       start of the corresponding cell */

    /* initialise the last rightmost which is the very end of the whole table */
    the_cells[max].rightmost = 0;

    /* loop through the rest of the cells backwards */
    for (x =  max - 1; x >= 0; x--)
    {
	pcp_cell cell = the_cells+x;
	pcp_cell next_cell = cell+1;
	pcp_group group = cell->first_start;
	
	/* initialise this cell's rightmost using min width of cell alone */
	cell->rightmost = (next_cell->rightmost - cell->width[slot]);

	/* Run down groups starting here */
	while (group != NULL)
	{
	    /* update rightmost if the width of this group on top of the rightmost
	       of its end (as already calculated) forces it to be more negative */
				/* NB add 1 since we measure width from end of end cell... */
	    const int my_rightmost = the_cells[group->iend+1].rightmost - group->width[slot];

	    if (my_rightmost < cell->rightmost)
	    {
		/*FMTDBGN(("  updating rightmost to %d\n", my_rightmost));*/
		cell->rightmost = my_rightmost;
	    }

	    group = group->next_start;
	}	
    }

    return;
}

/*****************************************************************************/

extern int colspan_algorithm(rid_table_item *table, int slot, BOOL horiz)
{
    const int max = horiz ? table->cells.x : table->cells.y;
    int x;
    pcp_cell the_cells = table->colspans;

    FMTDBG(("colspan_algorithm used %s (slot %s)\n", 
	    horiz ? "horizontally" : "vertically",
	    WIDTH_NAMES[slot]));

    /*colspan_trace_cells(table,horiz);*/

    if (max == 0)
	return 0;


    colspan_algorithmLR(table, slot, horiz);

    colspan_algorithmRL(table, slot, horiz);

    /* NB we count rightmosts downwards from 0 = rhs and then add
       the appropriate total at the end so that they go upwards
       left-to-right from the lhs */

#if 0
    /* old version (with fewer columns) left in temporarily for reference PCP 11/5/97 */
    for (x = 0; x < (max - 1); x++)
    {
	the_cells[x].rightmost = 
	    (the_cells[x+1].rightmost + 
	     the_cells[max - 1].leftmost);
    }
    the_cells[max - 1].rightmost = 
	the_cells[max - 1].leftmost;
#else
    /* can it really be this simple?(!) */
    for (x = max; x >= 0; x--)
    {
	the_cells[x].rightmost -= the_cells[0].rightmost;
    }
#endif

    /*FMTDBGN(("\nafter r-to-l pass for rightmosts (ie final values for %s)...\n", 
	    WIDTH_NAMES[slot]));*/
    /*colspan_trace_cells (table, horiz);*/

    /* PCP has left the building again */
    /*FMTDBG(("colspan_algorithm returning total %d for slot %s\n", 
	    the_cells[max - 1].leftmost, WIDTH_NAMES[slot]));*/

    return (the_cells[max].leftmost);
}


/*****************************************************************************/

/* eof cs2.c */
