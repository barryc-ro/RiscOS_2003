/* -*-C-*- colspan.h
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * Column sharing algorithm, designed by Patrick Campbell-Preston
 * (patrick@chaos.org.uk).
 *
 * Additional implementation by borris.
 */

#ifndef __colspan_h
#define __colspan_h

/* Shout if these clash */
#define HORIZONTALLY		TRUE
#define VERTICALLY		FALSE

#define HORIZVERT(b)		( (b) ? "HORIZONTALLY" : "VERTICALLY" )
#define HORIZCELLS(t,h)		( (h) ? (t)->hwidth : (t)->vwidth )
#define HORIZMAX(t,h)		( (h) ? (t)->cells.x : (t)->cells.y )

/*****************************************************************************

  COLSPAN DATASTRUCTURES: here, a 'pcp_cell' represents a *base*
  column of the table (or a row, if we are applying the algorithm to
  heights rather than widths). A 'pcp_group' represents a table cell
  which spans more than one base column (or row).

  Each pcp_cell has a singly-linked chain of all the groups which
  start here, and another for all the groups which end here. Each
  group has an array of widths (N_COLSPAN_WIDTHS long) which stores
  several widths for the cell with slightly differing semantics.

  We could treat a single unspanning cell as a group of size one, but
  it's worth optimising this case by duplicating the widths array in
  the cell structure itself (where it will be used to remember the
  maximum value of each width over all such non-spanning cells in the
  column) instead simply to save allocations and total memory use in
  the simple case.

  Each pcp_cell also has two more fields, 'leftmost' and 'rightmost';
  these are used by the colspan algorithm itself to store the leftmost
  and rightmost positions of the right boundary of the column while
  considering one particular set of widths.

  We also store two summary N_COLSPAN_WIDTHS widths arrays in the
  table - one for vertical information and the other for
  horizontal. The main arrays associated with the cells are shared
  between horizontal and vertical passes. The values stored are
  inclusive of their respecitive border types. This means each cell
  include TABLE_INSIDE_BIAS and the table array includes
  TABLE_OUTSIDE_BIAS. Thus the sume of the cells might be less than
  the summary record in the table itself.

*****************************************************************************

  Constants for indexing into widths array


RAW_MAX	

width beyond which we are adding empty space 

RAW_MIN	

minimum rendering width (longest word, etc) 

ABS_MIN	

ditto unless made bigger by an abs width constraint

PCT_MIN	

actual percentage constraint in % (default 0), or (in a later pass)
min width if both abs and %age constraints are obeyed

REL_MIN	

relative constraint (default 1), or (in a later pass) min width if
abs, %age *and* relative constraints are all obeyed

ABS_MAX

Like RAW_MAX, except absolute sized columns are taken into
account. This figure can be smaller, the same size or bigger than
RAW_MAX - be careful!

PCT_RAW

This is our final decision as to how the percentages are to be
distributed. This takes into account any attempt by the user to
specify more than 100% (ie it has been scaled again).

ACTUAL

the actual width we eventually decide


N_COLSPAN_WIDTHS

Size of arrays indexed by the above

FIRST_MIN LAST_MIN

For looping through


Our preference for order of using the slots is (most preferable first):

	ABS_MAX
	RAW_MAX
	REL_MIN
	PCT_MIN
	ABS_MIN
	RAW_MIN

Note that ABS_MAX and RAW_MAX do not have obvious relative sizes - the
inclusion of user absolute size constraints could grow or shrink the
table size from the RAW_MAX value.

FIRST_MIN through LAST_MAX **must** be continuous and start at
zero. Do not re-order these without sorting everything else out.

*/

typedef enum
{
    FIRST_MIN = 0,
	
    RAW_MIN = FIRST_MIN,
    ABS_MIN,
    PCT_MIN,
    REL_MIN,

    LAST_MIN = REL_MIN,

    FIRST_MAX,

    RAW_MAX = FIRST_MAX,
    ABS_MAX,
    PCT_MAX,
    REL_MAX,

    LAST_MAX = REL_MAX,

    /* Entries after here have no particular order dependency */

    PCT_RAW,

    ACTUAL,

    N_COLSPAN_WIDTHS,

    /* This section are pseudo types and DO NOT exist in the arrays. */

    CS_STOP

} width_array_e;


#define WIDTH_NAMES_ARRAY { \
      "RAW_MIN", \
	  "ABS_MIN", \
	  "PCT_MIN", \
	  "REL_MIN", \
	  "RAW_MAX", \
	  "ABS_MAX", \
	  "PCT_MAX", \
	  "REL_MAX", \
	  "PCT_RAW", \
	  "ACTUAL ", \
	  } 

#define	  PCT_MIN_DEFAULT	0
#define   REL_MIN_DEFAULT	1

struct pcp_cell_s
{
    struct pcp_group_s * first_start; /* pointer to first group starting here */
    struct pcp_group_s * first_end; /* pointer to first group ending here */
    int leftmost;		/* lowest possible position of end of cell */
    int rightmost;		/* highest possible position of end of cell */
    unsigned int flags;		/* bitmap of constraints satisfied */
    int width[N_COLSPAN_WIDTHS]; /* widths of this column on its own */
};

typedef struct pcp_cell_s pcp_cell_t, *pcp_cell;

struct pcp_group_s
{
    struct pcp_group_s * next_start;  /* next group starting here */
    struct pcp_group_s * next_end;  /* next group ending here */
    int istart;			/* index of start cell */
    int iend;			/* index of end cell (>istart) */
    unsigned int flags;		/* bitmap of constraints satisfied */
    int width[N_COLSPAN_WIDTHS]; /* widths of this group */
};

typedef struct pcp_group_s pcp_group_t, *pcp_group;

/* Have contribtions of this type - maybe from a group */
#define colspan_flag_ABSOLUTE		0x80000000
#define colspan_flag_PERCENT		0x40000000
#define colspan_flag_RELATIVE		0x20000000

/* This item yields a contribution of this type */
#define colspan_flag_ABSOLUTE_THIS	0x10000000
#define colspan_flag_PERCENT_THIS	0x08000000
#define colspan_flag_RELATIVE_THIS	0x04000000


#define colspan_flag_FINISHED		0x02000000
#define colspan_flag_USED		0x01000000
#define colspan_flag_COLUMN_USED	0x00800000

/*

  For each width_array_e from FIRST_MIN...LAST_MAX, (1 << X) is a flag
  in the flags field that says whether we have satisfied that level of
  width. We also record whether the column/group has and width
  contributions from each of the three types of constraint.

*****************************************************************************/

typedef struct
{
    int with_absolute;
    int with_percent;
    int with_relative;
    int finished;
    int used;
    int without_absolute;
    int without_percent;
    int without_relative;
    int unfinished;
    int unused;
} constraints_summary;

typedef struct
{
    width_array_e	slot;
    int			width;
} slot_width;

/*****************************************************************************

  The functions provide from colspan.c

  */

extern void colspan_init_structure(rid_table_item *table, BOOL horiz);
extern void colspan_trace_cells (rid_table_item *table, BOOL horiz);
extern int colspan_algorithm(rid_table_item *table, int slot, BOOL horiz);
extern void colspan_bias(rid_table_item *table, int bias, BOOL horiz);
extern void colspan_share_extra_space (rid_table_item *table, int fwidth, BOOL horiz);
extern void colspan_free_structure(rid_table_item *table, BOOL horiz);

/*
extern void colspan_copy_if_smaller(rid_table_item *table, int oldslot, int newslot, BOOL horiz);

extern int colspan_double(rid_table_item *table, width_array_e slot, BOOL horiz);
extern int colspan_halve(rid_table_item *table, width_array_e slot, BOOL horiz);
extern int colspan_scale(rid_table_item *table, width_array_e slot, BOOL horiz, double scale);
extern void colspan_bump_upwards(rid_table_item *table, width_array_e slot, BOOL horiz, int spare);
extern void colspan_bump_downwards(rid_table_item *table, width_array_e slot, BOOL horiz, int spare);
extern void colspan_copy_only_if_uninit(rid_table_item *table, 
					width_array_e old,
					width_array_e new,
					BOOL horiz);
					*/

/*****************************************************************************/

extern void colspan_column_init_from_leftmost(rid_table_item *table, width_array_e slot, BOOL horiz);

extern void colspan_column_and_eql_set(rid_table_item *table, 
				       BOOL horiz, 
				       width_array_e slot,
				       unsigned int ANT,
				       unsigned int EQL,
				       int SET);

extern void colspan_column_and_eql_copy(rid_table_item *table, 
					BOOL horiz, 
					width_array_e slot,
					unsigned int ANT,
					unsigned int EQL,
					width_array_e slot_from);

extern void colspan_column_and_eql_lt_copy(rid_table_item *table, 
					   BOOL horiz, 
					   width_array_e slot,
					   unsigned int ANT,
					   unsigned int EQL,
					   width_array_e slot_from);

extern void colspan_column_and_eql_upwards(rid_table_item *table,
					   BOOL horiz, 
					   width_array_e slot,
					   unsigned int AND,
					   unsigned int EQL,
					   int to_share);

extern void colspan_column_and_eql_downwards(rid_table_item *table,
					     BOOL horiz, 
					     width_array_e slot,
					     unsigned int AND,
					     unsigned int EQL,
					     int to_share);

/*****************************************************************************/

extern void colspan_all_and_eql_lt_copy(rid_table_item *table, 
					BOOL horiz, 
					width_array_e slot,
					unsigned int ANT,
					unsigned int EQL,
					width_array_e slot_from);

extern void colspan_all_and_eql_copy(rid_table_item *table, 
				     BOOL horiz, 
				     width_array_e slot,
				     unsigned int ANT,
				     unsigned int EQL,
				     width_array_e slot_from);

extern void colspan_all_and_eql_set(rid_table_item *table,
				    BOOL horiz, 
				    width_array_e slot,
				    unsigned int AND,
				    unsigned int EQL,
				    int SET);

extern void colspan_all_and_eql_halve(rid_table_item *table,
				      BOOL horiz, 
				      width_array_e slot,
				      unsigned int AND,
				      unsigned int EQL);

extern void colspan_all_and_eql_double(rid_table_item *table,
				       BOOL horiz, 
				       width_array_e slot,
				       unsigned int AND,
				       unsigned int EQL);

extern void colspan_all_and_eql_scale(rid_table_item *table,
				      BOOL horiz, 
				      width_array_e slot,
				      unsigned int AND,
				      unsigned int EQL,
				      double scale);

/*****************************************************************************/




#endif /* __colspan_h */

/* eof colspan.h */



