/* -*-C-*- csgraph.c
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Table constraints - graph-theoretic analysis code by
 * Patrick Campbell-Preston (patrick@chaos.org.uk).
 *
 * As usual borris will get his fingers into this the first moment I
 * let go of it, so I might as well leave in the following comment...
 *      Additional implementation by borris.
 *
 *
 * NOTE: THE CODE HERE IS USED FOR BOTH HORIZONTAL AND VERTICAL
 *       PURPOSES. NAMES ARE ALWAYS CHOSEN ON THE BASIS THAT WE
 *       ARE WORKING HORIZONTALLY THOUGH. THINK VERY CAREFULLY.  */

#include "rid.h"
#include "colspan.h"
#include "tables.h"
#include "util.h"

#ifdef PLOTCHECK
#include "rectplot.h"
#endif



/*****************************************************************************/

/* Internal functions */


/* called for each edge in the connectivity graph */

static void found_connection (pcp_cell the_cells, int start, int end)
{
    /* Method: Sedgewick p446 adjusted for indexing from 0 (so all dad entries are
       incremented by 1) - believe it or not this is effectively linear! */
    int i = start;
    int j = end;
    int t;
    /* find the roots of the connected components in the forest of parents */
    while (the_cells[i].dad > 0) i = the_cells[i].dad - 1;
    while (the_cells[j].dad > 0) j = the_cells[j].dad - 1;

    /* path compression making all ancestors point at the root */
    while (the_cells[start].dad > 0)
    {
	t = start;
	start = the_cells[start].dad - 1;
	the_cells[t].dad = i + 1;
    }
    while (the_cells[end].dad > 0)
    {
	t = end;
	end = the_cells[end].dad - 1;
	the_cells[t].dad = j + 1;
    }

    if (i != j)
    {
    /* merge the trees with weight balancing: dad of root is the number of
       descendants, negated, and we choose the larger of the two for the new root */
	if (the_cells[j].dad < the_cells[i].dad)
	{
	    the_cells[j].dad += (the_cells[i].dad - 1);
	    the_cells[i].dad = j + 1;
	}
	else
	{
	    the_cells[i].dad += (the_cells[j].dad - 1);
	    the_cells[j].dad = i + 1;
	}
    }

}


/*****************************************************************************/

/* Exported functions */

extern int csg_connected_components(pcp_cell the_cells, const int max)
{
    int x;
    int n_components = 0;

    /* run through all the cell records counting the non-positive dad fields */
    for (x = 0; x <= max; x++)
    {
	if (the_cells[x].dad <= 0) n_components++;
/*fprintf(stderr, "%4d ", the_cells[x].dad);*/
    }

/*fprintf(stderr, "\n");*/
    ASSERT (n_components > 0 && n_components <= max+1);

    return n_components;
}


/* Finds the connected components of the graph whose nodes are (horiz
   ? column : row) boundaries and whose edges are cells or groups in
   the usual colspan structure which actually exhibit a constraint of
   the kind represented by the supplied flags. The number of connected
   components in this graph is returned. The number of components is,
   by definition, in the range 1..N+1.

   970516: DAF: Except when there are no components marked by the
   flags whatsoever, when zero will be returned.  */

extern int find_connected_components (rid_table_item *table, unsigned int col_flags, unsigned int grp_flags, BOOL horiz)
{
    /* Method: see Sedgewick, Robert: 'Algorithms', second edition,
       Addison-Wesley 1988 - chapter 30 ('Connectivity'). */

    const int max = horiz ? table->cells.x : table->cells.y;
    pcp_cell the_cells = table->colspans;
    int n_components = 0;	/* function return value */
    int x;

    if (max == 0)
	return 0;

    ASSERT (max > 0);

    /* reinitialise the dad fields */
    for (x = 0; x <= max; x++)
	the_cells[x].dad = 0;

    /* loop through the cells looking for the required flag */
    for (x = 0; x < max; x++)
    {
	pcp_cell cell = the_cells+x;
	pcp_group group = cell->first_end;

	if (cell->flags & col_flags)
	{
	    found_connection (the_cells, x, x+1);
	}

	/* now do the same for the groups ending here. */
	while (group != NULL)
	{
	    if (group->flags & grp_flags)
	    {
		found_connection (the_cells, group->istart, x+1);
	    }
	    group = group->next_end;
	}
    }

    return csg_connected_components(the_cells, max);
}


/*****************************************************************************/

/* Returns true if the given cell boundaries fall in the same component
   of the graph constructed by a previous call to find_connected_components() */

extern BOOL same_connected_component (pcp_cell the_cells, int i, int j)
{
    /* believe it or not these loops will on average go round about
       once, so it's not worth doing a pre-pass to set all the dad
       fields to a connected-component id. */
    while (the_cells[i].dad > 0) i = the_cells[i].dad - 1;
    while (the_cells[j].dad > 0) j = the_cells[j].dad - 1;
    return (i == j);
}

/*****************************************************************************

  #### BORRIS EXPERIMENTAL BIT. ####

  We are trying to join together the graph until a single component remains.

  If we have N components in the graph, then there are N-1 degrees of
  freedom and >= (N-1) cells to be "fixed". For an edge that can move,
  we want to determine if it is partially constrained or not. This can
  be determined by examining each */

extern int csg_find_abs_floaters(rid_table_item *table, BOOL horiz)
{
#if DEBUG  && 0
    int abs_con = find_connected_components(table, colspan_flag_ABSOLUTE_COL, colspan_flag_ABSOLUTE_GROUP, horiz);
    int cand_edge = 1;
    int flt_edges = 0;
    int x;
    const int max = HORIZMAX(table, horiz);
    pcp_cell the_cells = table->colspans;

    /* Chop out reduced cases */
    if (abs_con == 0 || max < 2 || !horiz)
	return 0;

    if (the_cells[0].flags & colspan_flag_ABSOLUTE)
	fprintf(stderr, "CELLS STARTING WITH EDGE 0 ARE PARTIALLY CONSTRAINED\n");

    /* This will not make more than a single pass over the rows/columns */
    while (abs_con > 1)
    {
	fprintf(stderr, "Finding next candidate absolute edge, starting with edge %d, %d independent components\n", cand_edge, abs_con);

	ASSERT(cand_edge <= max);

	if ( same_connected_component(the_cells, 0, cand_edge) )
	{
	    fprintf(stderr, "Edge %d is already fixed\n", cand_edge);
	}
	else
	{
	    BOOL cons = FALSE;
	    fprintf(stderr, "Edge %d can move\n",cand_edge);
	    /* Is this movement partially constrained though? */
	    if ( the_cells[cand_edge].flags & colspan_flag_ABSOLUTE )
	    {
		fprintf(stderr, "CELLS STARTING WITH EDGE %d ARE PARTIALLY CONSTRAINED\n", cand_edge);
		cons = TRUE;
	    }
	    if ( the_cells[cand_edge-1].flags & colspan_flag_ABSOLUTE )
	    {
		fprintf(stderr, "CELLS ENDING WITH EDGE %d ARE PARTIALLY CONSTRAINED\n", cand_edge);
		cons = TRUE;
	    }

	    if (cons)
		flt_edges++;

	    /* Now connect this edge into the graph */
	    found_connection(the_cells, 0, cand_edge);
	    abs_con = csg_connected_components(the_cells, max);
	}
	cand_edge += 1;
    }

    fprintf(stderr, "There were %d partially constrained floating edges\n\n", flt_edges);
    return flt_edges;
#else
    return 0;
#endif
}

#if 0
extern void csg_examine(pcp_cell the_cells, const int max)
{
#if DEBUG && 0
    int x;

    for (x = 1; x < max; x++)
    {
	if ( ! same_connected_component(the_cells, 0, x) )
	{
	    fprintf(stderr, "Edge %d is floating, dad %d\n", x, the_cells[x].dad);
	}
    }
#endif
}
#endif


/*****************************************************************************/

/* eof csgraph.c */
