/* -*-C-*- commonsrc/tables.c - tables manipulation and support */
/*

Author: borris@ant.co.uk
Music used:
        Smash, Offspring.
        Formaldehyde, Terrorvision
        How to make friends and influence people, Terrorvision.
        Regular Urban Survirors, Terrorvision.

28-02-96 DAF    File created. Basic table structure building.
01-03-96 DAF    Added more support routines.
11-03-96 DAF    Polishing, borders, better formatting, etc
12-03-96 DAF    Better table construction - esp. better grouping
                into different phases, giving better fixed points
                where can be more definite about state of things.
26-03-96 DAF    Improved handling of table and cell borders.
26-03-96 DAF    Tweak for ALIGN=CHAR for me->align_char.
26-03-96 DAF    Fixed minor COLGROUP bug
27-03-96 DAF	Fixed height calculation bug
28-05-96 DAF    Converted to new parser. Much code has gone,
                which probably makes the code more reliable.
		Comments about implied elements no longer apply -
		the parser sorts out all the implied elements.
26--6-96 NvS	Moved code to locate the origin of a table stream
                into this file.
15-04-97 pdh    Two minor bugfixes tagged with 'pdh'

    Summary of 23-1-96 DRAFT Tables DTD section

    <!ELEMENT table - - (caption?, (col*|colgroup*), thead?, tfoot?, tbody+)>
    <!ELEMENT caption - - (%text;)+>
    <!ELEMENT thead - O (tr+)>
    <!ELEMENT tfoot - O (tr+)>
    <!ELEMENT tbody O O (tr+)>
    <!ELEMENT colgroup - O (col*)>
    <!ELEMENT col - O EMPTY>
    <!ELEMENT tr - O (th|td)+>
    <!ELEMENT (th|td) - O %body.content>

    In order that curstream of rid_header is never NULL, just in case we need
    to store some text somewhere when not expecting any, we take the attitude
    that any spurious text should not be thrown away if possible.  Ideally it
    should be put somewhere "sensible".  This implementation chooses the
    <CAPTION> section as good as any place to put such text.  This splits the
    CAPTION processing throughout the code a bit.  We can remove the caption
    if, when we get a </TABLE>, the caption contains no text and no
    properties. *** THIS COMMENT IS NOW OUT OF DATE, AS OF THE NEW PARSER ***

    If a column count is specified in the <TABLE> tag, then this is taken as
    the authoritive answer, and we can perform dynamic rendering.  If no
    column count is given, we add columns as they are refered to. We still
    attempt to perform dynamic rendering. An attempt to create a cell which
    cannot be valid (eg to the right of COLPSAN=0 cell) is deemed to have a
    missing <TR> element, which we insert. If this is not what the user
    intended, they should write correct HTML.

    mm_blah() are not NULL checked. Nicko says they shouldn't return with a NULL.
    Will need to update code if this changes.

    Column groups:

    Eventually, each column must belong to precisely one column
    group. This binding can happen at various different times.

    If no colgroup has been created by the end of the colgroup
    section, then a single group with infinite horizontal span is
    created.

    At /colgroup, if no cols have been entered, create to the span
    given in the colgroup. If cols have been given, the colgroup spans
    to the end of the last col and is then closed.

*/

/*****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

#include "myassert.h"
#include "memwatch.h"

#ifdef RISCOS
#include "wimp.h"
#endif

#include "consts.h"

#include "rid.h"
#include "charsets.h"
#include "util.h"
#include "webfonts.h"
#include "url.h"

#include "config.h"
#include "filetypes.h"
#include "parsers.h"
#include "images.h"

#include "htmlparser.h"
#include "tables.h"
#include "dump.h"
#include "gbf.h"

/*****************************************************************************/

static align_table halign_table[] =
{
    { HTML_TD_ALIGN_LEFT,	STYLE_ALIGN_LEFT },
    { HTML_TD_ALIGN_CENTER,	STYLE_ALIGN_CENTER },
    { HTML_TD_ALIGN_CENTRE,	STYLE_ALIGN_CENTER },
    { HTML_TD_ALIGN_RIGHT,	STYLE_ALIGN_RIGHT },
    { HTML_TD_ALIGN_CHAR,	STYLE_ALIGN_CHAR },
    { HTML_TD_ALIGN_JUSTIFY,	STYLE_ALIGN_JUSTIFY },
    { (unsigned int) -1,	0}
};

static align_table valign_table[] =
{
    { HTML_TD_VALIGN_TOP,	STYLE_VALIGN_TOP },
    { HTML_TD_VALIGN_MIDDLE,	STYLE_VALIGN_MIDDLE },
    { HTML_TD_VALIGN_BOTTOM,	STYLE_VALIGN_BOTTOM },
    { (unsigned int) -1,	0}
};

static VALUE no_value = { value_none, {0} };

/* Can be called with a cell or cell==NULL and x and y given */
/* Work out the borders for a given cell */

#ifdef STBWEB /*pdh*/
void table_cell_borders(rid_table_item *table, rid_table_cell *cell, int x, int y, border_str *bb)
{
    if (cell == NULL)
    {
	cell = *CELLFOR(table, x, y);
    }
    else
    {
	x = cell->cell.x;
	y = cell->cell.y;
    }

    bb->t = table->cellspacing + table->cellpadding;
    bb->b = table->cellpadding;
    bb->l = table->cellspacing + table->cellpadding;
    bb->r = table->cellpadding;
}
#endif

/* Return the offset from the top left of the cell to the origin of the enclosed stream */

void table_caption_stream_origin(rid_table_item *table, int *dx, int *dy)
{
    if (table->caption->calign == rid_ct_BOTTOM)
    {
	/* Caption at bottom of table */
	*dx = table->caption->off.x + table->cellspacing + table->cellpadding;
	*dy = table->caption->off.y - table->cellspacing - table->cellpadding - 2 * table->border;
    }
    else
    {
	/* Caption at top of table */
	*dx = table->caption->off.x + table->cellspacing + table->cellpadding;
	*dy = table->caption->off.y - table->cellspacing - table->cellpadding;
    }
}

void table_cell_stream_origin(rid_table_item *table, rid_table_cell *cell, int *dx, int *dy)
{
        rid_valign_tag rvt;
 	int ddy;
	const int f = table->cellspacing + table->cellpadding;

	TABDBGN(("table_cell_stream_origin(%p, %p, %p (%d), %p (%d))\n",
		table, cell, dx, *dx, dy, *dy));

	*dx = table->colhdrs[cell->cell.x]->offx + f;
	*dy = table->rowhdrs[cell->cell.y]->offy - f;

	/* Sort out VALIGN bits */
        rid_getprop(table, cell->cell.x, cell->cell.y, rid_PROP_VALIGN, &rvt);

  	ddy = cell->size.y - f - table->cellpadding + cell->stream.height;	/* Note height is -ve */

        TABDBGN(("table_cell_stream_origin: csy=%d csh=%d ddy=%d rvt=%d\n",
                 cell->size.y, cell->stream.height,
                 ddy, rvt ));

  	switch (rvt)
  	{
  	  	case STYLE_VALIGN_TOP:
  	  	case STYLE_VALIGN_BASELINE:
  	  	default:
  	  		break;
  	  	case STYLE_VALIGN_BOTTOM:
			*dy -= ddy;
  	  		break;
		case STYLE_VALIGN_MIDDLE:
			*dy -= (ddy >> 1);
  	  		break;
  	}


	TABDBGN(("table_cell_stream_origin() - done\n"));
}

/*****************************************************************************

  On entry *xp and *yp indicate where the last match was.  That cell can
  never be returned.  If NULL is returned, there are no further cells in
  the table and *xp and *yp values should not be used.  If a cell is
  detected, *xp and *yp are its coordinates on return.  For
  rid_next_root_cell, first call should have *xp=-1 and *yp=0.  For
  rid_prev_root_cell, first call should have *xp=table->cells.x,
  *yp=table->cells.y-1.

  */

extern rid_table_cell * rid_next_root_cell(rid_table_item *table, int *xp, int *yp)
{
    rid_table_cell *cell = NULL;
    int x = *xp, y = *yp;

    TASSERT(table != NULL);

    if (table->cells.x == 0 || table->cells.y == 0)
	return NULL;

    while (1)
    {
	x++;
	if (x >= table->cells.x)
	{
	    x = 0;
	    y++;
	}
	if (y >= table->cells.y)
	    return NULL;
	cell = *CELLFOR(table, x, y);
	if (cell == NULL)
	    continue;
	if (cell->cell.x == x && cell->cell.y == y)
	{
	    *xp = x;
	    *yp = y;
	    return cell;
	}
    }

    return NULL;
}

extern rid_table_cell *rid_prev_root_cell(rid_table_item *table, int *xp, int *yp)
{
    rid_table_cell *cell = NULL;
    int x = *xp, y = *yp;

    TASSERT(table != NULL);

    if (table->cells.x == 0 || table->cells.y == 0)
	return NULL;

    while (1)
    {
	x--;
	if (x < 0)
	{
	    x = table->cells.x - 1;
	    y--;
	}
	if (y < 0)
	    return NULL;
	cell = *CELLFOR(table, x, y);
	if (cell == NULL)
	    continue;
	if (cell->cell.x == x && cell->cell.y == y)
	{
	    *xp = x;
	    *yp = y;
	    return cell;
	}
    }

    return NULL;
}

/*****************************************************************************/

static void floating_table_format_alert_begin(SGMLCTX *context)
{
    rid_header *rh = htmlctxof(context)->rh;

    rh->full_format_nest++;
    /*rh->flags |= rid_hf_FULL_REFORMAT;*/
    FMTDBG(("floating_table_format_alert_begin() - now %d\n", rh->full_format_nest));
}

static void floating_table_format_alert_finish(SGMLCTX *context)
{
    rid_header *rh = htmlctxof(context)->rh;

    rh->full_format_nest--;

    if (rh->full_format_nest == 0)
	rh->flags &= ~rid_hf_FULL_REFORMAT;

    FMTDBG(("floating_table_format_alert_finish() - now %d\n", rh->full_format_nest));
}

/*****************************************************************************

  Extract properties information from the attributes given and generate
  a rid_table_props structure to describe them iff any attributes need
  recording, else just return NULL (ie this item will inherit all
  properties and save some memory whilst doing it).

  Note that the inheritence order requirements mean that we cannot just
  let style information stack as normal, as this doesn't spot vertically
  inherited alignment information form column groups, for example.

  Both the alignment attributes has an accompanying table that converts
  from the attributes specific enumeration through to generic align
  enumeration values. The table is terminated with (-1,-1).

  */

static rid_table_props *parse_table_props(VALUE *valign,
                                          VALUE *halign,
                                          VALUE *ch,
                                          VALUE *choff,
                                          VALUE *lang,
                                          VALUE *dir,
                                          VALUE *style,
					  VALUE *bgcolor,
					  align_table* valign_index,
					  align_table* halign_index
    )
{
    rid_table_props *p;
    int used = 0;

    p = mm_calloc(1, sizeof(*p));

    if (p == NULL)
	return NULL;

    if (valign->type == value_enum)
    {
	int spec = valign->u.i;
	align_table *ptr = valign_index;

	while (ptr->specific != (unsigned int) -1)
	{
	    if (ptr->specific == spec)
	    {
		used++;
		p->valign = ptr->generic;
		break;
	    }
	    ptr++;
	}
#if DEBUG
	if (ptr->specific == (unsigned int) -1)
	    fprintf(stderr, "parse_table_props(): unknown valign tag %d\n", spec);
#endif
    }

    if (halign->type == value_enum)
    {
	int spec = halign->u.i;
	align_table *ptr = halign_index;

	while (ptr->specific != (unsigned int) -1)
	{
	    if (ptr->specific == spec)
	    {
		used++;
		p->halign = ptr->generic;
		break;
	    }
	    ptr++;
	}
#if DEBUG
	if (ptr->specific == (unsigned int) -1)
	    fprintf(stderr, "parse_table_props(): unknown valign tag %d\n", spec);
#endif
    }

    if (dir->type == value_enum)
    {
	/* Either LTR or RTL - no other choices */
	p->dir = dir->u.i == HTML_TABLE_DIR_LTR ? rid_dt_LTR : rid_dt_RTL;
	used++;
    }

    if (ch->type == value_string)
    {
	used++;
	p->ch = ch->u.s.ptr[0];
    }

    if (choff->type == value_absunit)
    {
	used++;
	p->choff = *choff;
    }

    if (lang->type == value_string)
    {
	used++;
	p->lang = stringdup(lang->u.s);
    }

    if (style->type == value_string)
    {
	used++;
	p->style = stringdup(style->u.s);
    }

    if (bgcolor->type != value_none)
    {
	used++;
	p->flags |= rid_tpf_BGCOLOR;
	htmlriscos_colour( bgcolor, &p->bgcolor);
    }

    if (!used)
    {
	mm_free(p);
	return NULL;
    }

    return p;
}

/*****************************************************************************

  Generate a copy of a set of properties, including replicating any
  additional memory necessary.

 */

static rid_table_props *replicate_props(rid_table_props *props)
{
    rid_table_props *new;

    if (props == NULL)
	return NULL;

    new = mm_calloc(1, sizeof(*new));

    new->valign = props->valign;
    new->halign = props->halign;
    new->dir = props->dir;
    new->ch = props->ch;
    new->choff = props->choff;
    new->lang = strdup(props->lang);
    new->style = strdup(props->style);

    return new;
}

/*****************************************************************************/

/*needs to set group border for colgroups*/

static void set_column_flags(rid_table_item *table, int x)
{
#if 0
    if ( table->colhdrs[x].userwidth.type == rid_stdunit_PX )
    {
	table->colhdrs[x].flags |= rid_chf_ABSOLUTE;
    }
    else if ( table->colhdrs[x].userwidth.type == rid_stdunit_MULT )
    {
	table->colhdrs[x].flags |= rid_chf_RELATIVE;
    }
    else if ( table->colhdrs[x].userwidth.type == rid_stdunit_PCENT )
    {
	table->colhdrs[x].flags |= rid_chf_PERCENT;
    }
    else if (table->colhdrs[x].colgroup != NULL)
    {
	if ( table->colhdrs[x].colgroup->userwidth.type == rid_stdunit_PX ||
	     table->colhdrs[x].colgroup->userwidth.type == rid_stdunit_MULT ||
	     table->colhdrs[x].colgroup->userwidth.type == rid_stdunit_PCENT)
	{
	    /* If inheriting this from colgroup, copy it now for ease */
	    table->colhdrs[x].userwidth = table->colhdrs[x].colgroup->userwidth;
	    set_column_flags(table, x);
	    return;
	}
    }
#else
    if (table->colhdrs[x]->userwidth.type == value_none)
    {
	if (table->colhdrs[x]->colgroup != NULL)
	{
	    table->colhdrs[x]->userwidth = table->colhdrs[x]->colgroup->userwidth;
	}
    }
#endif
}

/*****************************************************************************

  Add a column before reached end of colgroup section. No rows exist.
  table->scaff.x is next column header to initialise, and not advanced
  here. There might or might not be an existing colgroup. If one does
  not exist, we don't create it here. Such a missing colgroup in the
  header is sorted out when the colgroupsection is closed. Once this
  point has been passed, we use add_retro_col(), which does its own
  thing.

  */

static rid_table_colhdr * add_new_colhdr(rid_table_item *table)
{
    const int x = table->cells.x++;

    TABDBGN(("add_new_colhdr(%p): from %d to %d columns, %p\n",
	    table, x, table->cells.x, table->colhdrs));

    ASSERT(table->cells.x == 1 || table->colhdrs != NULL);

    table->colhdrs = mm_realloc(table->colhdrs, (x + 1) * sizeof(rid_table_colhdr *));
    table->colhdrs[x] = mm_calloc(1, sizeof(rid_table_colhdr));

    return table->colhdrs[x];
}

/*****************************************************************************

  Column groups are only added during the colgroup section - there can be
  no rows in the table at this point.

  */

static rid_table_colgroup * add_new_colgroup(rid_table_item *table)
{
    const int x = table->num_groups.x++;

    TABDBG(("add_new_colgroup(%p): from %d to %d colgroups\n", table, x, table->num_groups.x));

    table->flags |= rid_tf_IN_COLGROUP;
    table->colgroups = mm_realloc(table->colgroups, (x+1) * sizeof(rid_table_colgroup *) );
    table->colgroups[x] = mm_calloc(1, sizeof(rid_table_colgroup) );

#if DEBUG && 0
    dump_cell_map(table, "Post add_new_colgroup");
#endif

    return table->colgroups[x];
}

#if 0
static void mark_row_as_end_rowgroup(HTMLCTX *me)
{
    rid_table_item *table = me->table;

    if (table->cells.y == 0)
	return;

    table->rowhdrs[table->cells.y-1]->flags |= rid_rhf_GROUP_BELOW;
}

static void mark_row_as_begin_rowgroup(HTMLCTX *me)
{
    rid_table_item *table = me->table;

    if (table->cells.y == 0)
	return;

    table->rowhdrs[table->cells.y-1]->flags |= rid_rhf_GROUP_ABOVE;
}
#endif

/*needs to set group border for rowgroup*/

static rid_table_rowgroup * add_new_rowgroup(rid_table_item *table)
{
    const int y = table->num_groups.y++;

    TABDBGN(("add_new_rowgroup(%p): from %d to %d rowgroups\n", table, y, table->num_groups.y));

    table->rowgroups = mm_realloc(table->rowgroups, (y+1) * sizeof(rid_table_rowgroup *) );
    table->rowgroups[y] = mm_calloc(1, sizeof(rid_table_rowgroup));

#if DEBUG && 0
    dump_cell_map(table, "Post add_new_rowgroup");
#endif

    return table->rowgroups[y];
}

static void copy_colhdr_props(rid_table_colhdr *from, rid_table_colhdr *to)
{
    /* Bulk copy */
    *to = *from;

    /* Replicate subsidary allocated memory */
    to->props         = replicate_props(to->props);
    to->id            = strdup(to->id);
    to->class         = strdup(to->class);
}


/*****************************************************************************

  Add a column to the table after the end of the colgroup section.  We
  perform the array reshaping through a memory copy - it's easy.  A
  new rid_table_colhdr item is also added.  If the number of columns
  has been fixed through <TABLE COLS=N> then we cannot add another
  column - should not be called under such circumstances.  We might
  have to replicate rid_table_colhdr attributes.  scaff.x is not
  updated.  colgroup->span is updated if we "are in a COLGROUP" -
  after the colgroup section, we set table->cur_colgroup if we don't
  have a fixed number of columns to indicate that the last column
  group should still get its span updated whenever a new column is
  created.  The number of columns covered by a rid_cf_INF_HORIZ item
  must be incremented so it has the actual span count by the time we
  come to format the table.

  16/8/96: A new colgroup will be created if there isn't one.

  */

static void add_retro_col(rid_table_item *table)
{
    rid_table_cell *cell, **cellp, **dstp, **srcp;
    rid_table_colhdr *colhdr;
    size_t dsts, srcs, cpys, size;
    int x, y;

    TABDBGN(("add_retro_col(%p) - from %d to %d columns\n",
	    table, table->cells.x, table->cells.x+1));

    if (table->state == tabstate_BAD)
	return;

    ASSERT( (table->flags & rid_tf_COLS_FIXED) == 0 );

    /* First add a column header */

    colhdr = add_new_colhdr(table);

    /* Maybe replicate previous colhdr attributes */

    if ( table->cells.x > 1 && (table->colhdrs[table->cells.x-2]->flags & rid_chf_REPLICATE) != 0 )
    {
	TABDBGN(("Replicating previous colhdr properties\n"));
	copy_colhdr_props(table->colhdrs[table->cells.x-2], colhdr);
    }

    set_column_flags(table, table->cells.x - 1);

    if ( table->cur_colgroup != NULL )
    {
	colhdr->colgroup = table->cur_colgroup;
	table->cur_colgroup->span++;
    }
    else
    {
	TABDBGN(("No colgroup to bump in add_retro_col() - creating one\n"));

	colhdr->colgroup = table->cur_colgroup = add_new_colgroup(table);
	colhdr->colgroup->span = 1;
	/* Not certain whether should forcibly clear this is not */
#if 0
	table->flags &= ~rid_tf_IN_COLGROUP;
#endif
    }

    if (table->cells.y == 0)
    {
	/* no rows yet - no actual size to cell array */
	TABDBGN(("No rows, so done add_retro_col()\n"));
	goto done;
    }

    /* Then add another column */

    size = (table->cells.x) * table->cells.y * sizeof(*cellp);
    cellp = mm_calloc( 1, size );

    TASSERT(cellp != NULL);

    dstp = cellp;
    srcp = table->array;
    dsts = table->cells.x;
    srcs = table->cells.x - 1;
    cpys = srcs * sizeof(*cellp);

    TABDBGN(("Reshape array: size %d, dstp %p, srcp %p, dsts %d, srcs %d, cpys %d\n",
	     (int) size, dstp, srcp, (int) dsts, (int) srcs, (int) cpys));

    /* Reshape the array by copying slices - no overlaps as new memory */

    for (y = 0; y < table->cells.y; y++)
    {
	memcpy(dstp, srcp, cpys);
	dstp += dsts;
	srcp += srcs;
    }

    mm_free(table->array);
    table->array = cellp;

    /* Then spread any cells wanting it */

    x = table->cells.x - 1;
    if (x < 1)
    {
	TABDBGN(("Not enough columns to have any replication\n"));
	goto done;
    }

    TABDBGN(("Replicating any necessary cells (%d,%d)\n", table->cells.x, table->cells.y));

    for (y = 0; y < table->cells.y; y++)
    {
	cell = * CELLFOR(table, x - 1, y);
	if (cell != NULL && (cell->flags & rid_cf_INF_HORIZ) != 0 )
	{
	    /*int  t;*/
	    * CELLFOR(table, x, y) = cell;
	    ASSERT(cell->span.x > 0);
#if NEWGROW
	    TABDBG(("Cell %d,%d in %d,%d table with span %d,%d has swant %d,%d\n",
		    x, y, table->cells.x, table->cells.y,
		    cell->span.x, cell->span.y,
		    cell->swant.x, cell->swant.y));

	    /* Must only do this once, even with ROWSPAN=N, N > 1 */
	    if (y == cell->cell.y)
	    {
		cell->span.x += 1;
		FMTDBG(("Cell rooted at %d,%d, bump span.x to %d\n", cell->cell.x, cell->cell.y, cell->span.x));
	    }

	    TABDBGN(("Cell %d,%d is replicated from %d,%d, span %d,%d\n",
		     x, y, cell->cell.x, cell->cell.y, cell->span.x, cell->span.y));
#else
	    TABDBG(("Cell %d,%d in %d,%d table with span %d,%d has sleft %d\n",
		    x, y, table->cells.x, table->cells.y,
		    cell->span.x, cell->span.y, cell->sleft));
	    t = cell->sleft / cell->span.x;

	    if (y == cell->cell.y)
	    {
		cell->span.x += 1;
		t *= cell->span.x;
		cell->sleft = t;
	    }

	    TABDBGN(("Cell %d,%d is replicated from %d,%d, new sleft %d, span %d,%d\n",
		     x, y, cell->cell.x, cell->cell.y, t, cell->span.x, cell->span.y));
#endif
	}
    }

done:

#if DEBUG && 0
    dump_cell_map(table, "Post add_retro_col");
#endif

    return;
}



/*****************************************************************************

  Add a row to the table and then spread any existing rid_cf_MULTIPLE
  cells.  No rowgroup is created here.  No header/footer count is
  incrememented here.  If there are no columns, we add one - the
  colgroup section is finished by now.  rowhdrs is extended.
  Completion marking is performed in a seperate phase so that eg a 2x2
  cell gets marked complete when all cells have been grown, rather
  than when the 1st cell reaches maximum extent.  The number of
  columns covered by a rid_cf_INF_VERT item must be incremented so it
  has the actual span count by the time we come to format the table.
  The span count of the current rowgroup is also incremented.

  Assert that we have a colgroup by the time we create a row - and
  it's current if we're not fixed width.

  */

static void add_new_row(rid_table_item *table)
{
    rid_table_cell *cell, **cellp;
    rid_table_rowhdr *rowhdr;
    size_t size, off;
    int x, y, did_repl;

    TABDBGN(("add_new_row(%p) - from %d to %d rows ptr is %p\n",
	     table, table->cells.y, table->cells.y+1, table->rowhdrs));

    if (table->state == tabstate_BAD)
	return;

    if (table->cells.x == 0)
    {
	TABDBGN(("No columns when adding a row - force one\n"));
	add_retro_col(table);
    }

    /* Add another rowhdr item */

    y = table->cells.y;

    table->rowhdrs = mm_realloc(table->rowhdrs, (y+1) * sizeof(rid_table_rowhdr *));
    table->rowhdrs[y] = mm_calloc(1, sizeof(rid_table_rowhdr));

    TABDBGN(("new hdrs %p, contents for %d is %p\n",
	    table->rowhdrs, y, table->rowhdrs[y]));

    rowhdr = table->rowhdrs[y];
    rowhdr->rowgroup = table->cur_rowgroup;
    rowhdr->rowgroup->span += 1;

    TABDBGN(("Bumping rowgroup span from %d to %d\n",
	     rowhdr->rowgroup->span-1, rowhdr->rowgroup->span));

    /* Note what sort of row it is */
    rowhdr->flags |= rowhdr->rowgroup->flags & (rid_rgf_THEAD | rid_rgf_TFOOT | rid_rgf_TBODY );
    if (rowhdr->rowgroup->span == 1 && y > 0)
    {
	TASSERT( (rowhdr->flags & rid_rhf_THEAD) == 0 );

	if ( (rowhdr->flags & rid_rhf_TFOOT) != 0 ||
	     (y > 1 && (table->rowhdrs[y-1]->flags & rid_rhf_TBODY) != 0) ||
	     table->rowgroups[0]->span > 0)
	{
	    TABDBGN(("add_new_row(): setting above/below flags (%d)\n", y));
	    rowhdr->flags |= rid_rhf_GROUP_ABOVE;
	    if ( (table->rowhdrs[y-1]->rowgroup->flags & rid_rgf_TFOOT) == 0)
		table->rowhdrs[y-1]->flags |= rid_rhf_GROUP_BELOW;
	}
	else
	{
	    TABDBGN(("add_new_row(): NOT setting above/below flags (%d)\n", y));
	}
    }
    else
    {
	TABDBGN(("add_new_row(): not considering doing group seperators\n"));
    }

    /* then add another row */

    size = table->cells.x * (table->cells.y + 1) * sizeof(*cellp);
    cellp = table->array == NULL ? mm_calloc(1, size) : mm_realloc(table->array, size);
    off = table->cells.x * table->cells.y * sizeof(*cellp);
    size = table->cells.x * sizeof(*cellp);
    memset(off + (char *) cellp, 0, size);
    table->cells.y++;
    table->array = cellp;

    y = table->cells.y - 1;
    if (y < 1)
    {
	TABDBGN(("Not enough rows to have anything to spread\n"));
	goto done;
    }

    TABDBGN(("Performing any spreading to row %d from row %d\n", y, y-1));

    for (did_repl = 0, x = table->cells.x - 1; x >= 0; x--)
    {
	cell = * CELLFOR(table, x, y - 1);
	if (cell == NULL || (cell->flags & rid_cf_COMPLETE) != 0 )
	    continue;

	if ( (cell->flags & rid_cf_INF_VERT) != 0 )
	{
	    TABDBGN(("Replicating INF_VERT %d,%d span %d,%d from %d,%d\n",
		     x, y, cell->span.x, cell->span.y, cell->cell.x, cell->cell.y));
	    * CELLFOR(table, x, y) = cell;
	    if ( x == cell->cell.x )
	    {
		cell->span.y += 1;
		TABDBGN(("Root cell - bumping span.y to %d\n", cell->span.y));
	    }
	}
#if NEWGROW
	else if (cell->swant.y != cell->span.y)
	{
	    did_repl = 1;
	    * CELLFOR(table, x, y) = cell;
	    if (x == cell->cell.x)
	    {
		cell->span.y += 1;
		TABDBG(("Cell %d,%d rooted so bumping span.y to %d\n", x,y,cell->span.y));
	    }
	    TABDBGN(("Replicating %d,%d from %d,%d, span %d,%d\n", x, y, cell->cell.x, cell->cell.y, cell->swant.x, cell->swant.y));
	}
#else
	else if ( cell->sleft > 0 )
	{
	    did_repl = 1;
	    TABDBGN(("Replicating %d,%d from %d,%d, sleft %d\n", x, y, cell->cell.x, cell->cell.y, cell->sleft));
	    * CELLFOR(table, x, y) = cell;
	    cell->sleft -= 1;
	}
#endif
    }

    if (! did_repl)
    {
	TABDBGN(("No completion to check for\n"));
	goto done;
    }

    TABDBGN(("Replicated non INF_VERT item - checking for completion\n"));

    /* Then spread completions of cells */

    for (x = 0; x < table->cells.x; x++)
    {
	cell = * CELLFOR(table, x, y - 1);
	if (cell != NULL && (cell->flags & rid_cf_COMPLETE) == 0
	    && (cell->flags & rid_cf_INF_VERT) == 0 )
	{
	    TABDBGN(("Considering cell %d,%d\n", cell->cell.x, cell->cell.y));
#if NEWGROW
	    if (cell->swant.y <= cell->span.y)
	    {
		TABDBGN(("Marking cell %d,%d as completed vertically, spans %d, %d\n",
			 cell->cell.x, cell->cell.y, cell->span.x, cell->span.y));
		cell->flags |= rid_cf_COMPLETE;
	    }
#else
	    if (cell->sleft <= 0)
	    {
		TABDBGN(("Marking cell %d,%d as completed vertically, spans %d, %d\n",
			 cell->cell.x, cell->cell.y, cell->span.x, cell->span.y));
		cell->flags |= rid_cf_COMPLETE;
	    }
#endif
	}
    }

    /* And update row group */

done:
#if DEBUG && 0
    dump_cell_map(table, "Post add_new_row");
#endif

    return;
}

/*****************************************************************************

  Have finished a table with a <TFOOT>. Need to swap the rows from the
  middle of the table to the end of the table - this is a shuffle
  operation. We take a memory copy of the portion of the cell array
  describing the TFOOT section, shuffle the TBODY section and write
  back the TFOOT section. In general, the TFOOT section is quite
  small. Our heap usage is a word per cell, which is acceptable.
  The y coordinate of each root needs consideration as well.

 */

static void do_tfoot_swapping(rid_table_item *table)
{
    const int cellsx = table->cells.x;

    rid_table_rowgroup *tg;
    int tfoot_group, other_group;
    int tfoot_line, other_line;
    int span;
    int x,y;
    int diff;
    rid_table_cell *cell;

    TABDBG(("do_tfoot_swapping(): table %d,%d, groups %d,%d\n",
	    table->cells.x,
	    table->cells.y,
	    table->num_groups.x,
	    table->num_groups.y));

    TASSERT(table->flags & rid_tf_HAVE_TFOOT);
    TASSERT(table->flags & rid_tf_TFOOT_INVISIBLE);

    table->flags &= ~rid_tf_TFOOT_INVISIBLE;

    for (tfoot_line = 0; tfoot_line < table->cells.y; tfoot_line++)
    {
	if (table->rowhdrs[tfoot_line]->rowgroup->flags & rid_rgf_TFOOT)
	    break;
    }

    ASSERT(tfoot_line < table->cells.y);

    for (tfoot_group = 0; tfoot_group < table->num_groups.y; tfoot_group++)
    {
	if (table->rowgroups[tfoot_group]->flags & rid_rgf_TFOOT)
	    break;
    }

    ASSERT(tfoot_group < table->num_groups.y);

    span = table->rowgroups[tfoot_group]->span;
    other_group = table->num_groups.y - 1;

    /* Row number of where 1st tfoot row will end up */
    other_line = table->cells.y - span;

    TABDBG(("do_tfoot_swapping(): tfoot_line %d, other_line %d\n", tfoot_line, other_line));
    TABDBG(("do_tfoot_swapping(): tfoot_group %d, other_group %d, span %d\n", tfoot_group, other_group, span));

    if (tfoot_line == other_line)
    {
	TABDBG(("do_tfoot_swapping(): <TFOOT> finishes table, so no swapping\n"));
	return;
    }

    ASSERT(other_line > tfoot_line);

    /* Swap row group, row header and cell information around */

    tg = table->rowgroups[tfoot_group];
    table->rowgroups[tfoot_group] = table->rowgroups[other_group];
    table->rowgroups[other_group] = tg;

    diff = other_line - tfoot_line;

    /* Update y position of root cells that are going to move */

    for (x = -1, y = 0; (cell = rid_next_root_cell(table, &x, &y)) != NULL; )
    {
	const rid_rowhdr_flags f = table->rowhdrs[cell->cell.y]->flags;

	if (f & rid_rhf_TBODY)
	{
	    cell->cell.y -= span;
	}
	else if (f & rid_rhf_TFOOT)
	{
	    cell->cell.y += diff;
	}
    }

    shuffle_array(table->array + cellsx * tfoot_line,
		  sizeof(rid_table_cell *),
		  cellsx,
		  span,
		  table->cells.y - tfoot_line - span);

    shuffle_array(table->rowhdrs + tfoot_line,
		  sizeof(rid_table_rowhdr *),
		  1,
		  span,
		  table->cells.y - tfoot_line - span);

    shuffle_array(table->rowgroups + tfoot_group,
		  sizeof(rid_table_rowgroup *),
		  1,
		  1,
		  table->num_groups.y - tfoot_group - 1);

    /* Edit group above/below flags */
    /* Careful on the order */

    table->rowhdrs[other_line - 1]->flags |= rid_rhf_GROUP_BELOW;
}


/*****************************************************************************

  Add a new column when we have a fixed number of columns. We must be
  in a column group.

  */


static void start_col_fixed(rid_table_item *table, int cols, int *firstp, int *countp)
{
    TABDBG(("start_col_fixed(%p,%d,%p=%d,%p=%d)\n",
	    table, cols, firstp, *firstp, countp, *countp));

    ASSERT( table->cur_colgroup != NULL );
    ASSERT( (table->flags & rid_tf_IN_COLGROUP) != 0 );

    if (cols == 0)
    {       /* Replicate to last column then no more columns */
	*firstp = table->scaff.x;
	*countp = table->cells.x - table->scaff.x;
	table->scaff.x = table->cells.x;
	table->flags |= rid_tf_NO_MORE_CELLS;
    }
    else
    {
	if ( table->scaff.x + cols > table->cells.x )
	{
	    cols = table->cells.x - table->scaff.x;
	}
	*firstp = table->scaff.x;
	*countp = cols;
	table->scaff.x += cols;
    }

    table->cur_colgroup->span += *countp;
}

static void start_col_growing(rid_table_item *table, int cols, int *firstp, int *countp)
{
    rid_table_colhdr *colhdr = table->cells.x == 0 ? NULL : table->colhdrs[table->cells.x -1];

    TABDBG(("start_col_growing(%p,%d,%p=%d,%p=%d)\n",
	    table, cols, firstp, *firstp, countp, *countp));

    ASSERT( table->cur_colgroup != NULL );
    ASSERT( (table->flags & rid_tf_IN_COLGROUP) != 0 );

    if ( table->cells.x > 0 && (colhdr->flags & rid_chf_REPLICATE) != 0 )
    {       /* Have to replicate previous column */
	*firstp = *countp = 0;  /* Current <COL> contributes only a span */
	if (cols == 0)
	    cols = 1;
	table->scaff.x += cols;
	table->cur_colgroup->span += cols;
	while (cols-- > 0)
	    copy_colhdr_props(colhdr, add_new_colhdr(table));
    }
    else if (cols == 0)
    {       /* Replicate to end of table */
	*firstp = table->cells.x;
	*countp = 1;
	table->cur_colgroup->span += 1;
	add_new_colhdr(table)->flags |= rid_chf_REPLICATE;
	/*table->cells.x += 1;*/ /* done by add_new_colhdr */
    }
    else
    {       /* Add as many columns as requested */
	*firstp = table->scaff.x;
	*countp = cols;
	table->scaff.x += cols;
	table->cur_colgroup->span += cols;
	while (cols-- > 0)
	    add_new_colhdr(table);
    }
}

/*****************************************************************************

  Reel back any cells spanning multiple rows that never actually saw
  as many rows as expected (ie N for ROWSPAN=N greater than number of
  <TR>s) needs reeling back in. Used after TFOOT section and the end
  of the table. Argument could be made either way for this being
  good/bad after THEAD and TBODY.

  */

static void restrain_rowspan_cells(HTMLCTX *me, rid_table_item *table)
{
    /*const int maxy = table->cells.y;*/
    int x, y;
    rid_table_cell *cell;

    TABDBGN(("restrain_rowspan_cells:\n"));
    /*dump_table(table, NULL)*/;

    for (x = -1, y = 0; (cell = rid_next_root_cell(table,&x,&y)) != NULL; )
    {
#if NEWGROW
	cell->flags |= rid_cf_COMPLETE;
	/* Might not have been able to grow as the user expected */
	if ( (cell->flags & rid_cf_INF_HORIZ) == 0)
	    cell->swant.x = cell->span.x;
	if ( (cell->flags & rid_cf_INF_VERT) == 0)
	    cell->swant.y = cell->span.y;
#else
	if (cell->cell.y + cell->span.y > maxy)
	{
	    cell->span.y = maxy - cell->cell.y;
	    TABDBG(("restrain_rowspan_cells: Restraining %d,%d to span %d\n", cell->cell.x, cell->cell.y, cell->span.y));
	}
#endif
    }
}

/*****************************************************************************

  The table is finished. The dimensions are now stable.

  */

static void tidy_table(HTMLCTX *me, rid_table_item *table)
{
}

/*****************************************************************************

  Inheritence of attributes:

  Each cell can have a number of attributes, either directly specified with
  the cell or inherited from its surroundings.  The order of inheritence is
  defined as follows:

  HALIGN, CHAR, CHAROFF
  cells > columns > column groups > rows > row groups > default
  xcCrRd

  VALIGN, LANG, DIR and STYLE
  cells > rows > row groups > columns > column groups > table > default
  xrRcCtd

  BGCOLOR
  cells > columns > column groups > rows > row groups > table > default
  xcCrRtd

  HEIGHT
  fudge on cells only at present.

  Within rid_getprop(), this is translated into a control string indicating
  where to look next for the attribute.  The magic characters are:

  x       cells
  r       rows
  R       row groups
  c       columns
  C       column groups
  t       table
  d       default

  Given how frequently this gets used, should perhaps be writing something
  that executes faster.

  colhdr gets userwidth copied automatically from its colgroup
  */

extern void rid_getprop(rid_table_item *table, int x, int y, int prop, void *result)
{
#if DEBUG
    static char *prop_name[] =
    {
	"rid_PROP_VALIGN ",
	"rid_PROP_HALIGN ",
	"rid_PROP_DIR    ",
	"rid_PROP_CH     ",
	"rid_PROP_CHOFF  ",
	"rid_PROP_LANG   ",
	"rid_PROP_STYLE  ",
	"rid_PROP_WIDTH  ",
	"rid_PROP_BGCOLOR",
	"rid_PROP_HEIGHT "
    };
#endif

    rid_stdunits dsu = { rid_stdunit_MULT};

    char *control;
    rid_table_cell *cell = * CELLFOR(table, x, y);
    rid_table_props *props = NULL;
    int hdr = cell ? (cell->flags & rid_cf_HEADER) : 0;

    dsu.u.f = 1.0;

    TABDBGN(("rid_getprop(%p, %d, %d, %s, %p): cell props %p\n",
      table, x, y, prop_name[prop], result, cell->props));

    ASSERT(x < table->cells.x);
    ASSERT(y < table->cells.y);
    TASSERT(cell != NULL);

    switch (prop)
    {
    case rid_PROP_HALIGN:
    case rid_PROP_CH    :
    case rid_PROP_CHOFF :
	control = "xcCrRd";
	break;

    case rid_PROP_BGCOLOR:
	control = "xcCrRtd";
	break;

    case rid_PROP_VALIGN:
    case rid_PROP_DIR   :
    case rid_PROP_LANG  :
    case rid_PROP_STYLE :
	control = "xrRcCtd";
	break;

    case rid_PROP_WIDTH :
    {
	rid_table_colhdr *hdr = table->colhdrs[x];
	rid_table_colgroup *grp = hdr->colgroup;

	ASSERT(hdr != NULL);

	/*TABDBGN(("rid_getprop(): hdr %p, grp %p\n", hdr, grp));*/

	if (cell->userwidth.type != value_none)
	{
	    /*TABDBGN(("rid_getprop(): width from the cell itself\n"));*/
	    * ((VALUE *)result) = cell->userwidth;
	}
	else if (hdr->userwidth.type != value_none)
	{
	    /*TABDBGN(("rid_getprop(): width from column header\n"));*/
	    * ((VALUE *)result) = hdr->userwidth;
	}
	else if (grp != NULL && grp->userwidth.type != value_none)
	{
	    /*TABDBGN(("rid_getprop(): width from group header\n"));*/
	    * ((VALUE *)result) = grp->userwidth;
	}
	else
	{
	    /*TABDBGN(("rid_getprop(): width from default\n"));*/
#if 1
	    ((VALUE*)result)->type = value_none;
#else
	    * ((VALUE *)result) = dsu;
#endif
	}
	return;

    case rid_PROP_HEIGHT:
	* ((VALUE *)result) = cell->userheight;
	return;
    }
    break;

    default:
	usrtrc( "\n\n\n\nRID_GETPROP(): UNKNOWN PROPERTY\n\n\n\n\n");
	return;
	break;
    }

    while (1)
    {
	/*TABDBGN(("rid_getprop(): control string is '%s'\n", control));*/
	switch ( *control++ )
	{
	case 'x':       /* cells */
	    ASSERT(cell != NULL);
	    props = cell->props;
	    break;
	case 'r':       /* rows */
	    ASSERT(table->rowhdrs != NULL);
	    props = table->rowhdrs[y]->props;
	    break;
	case 'R':       /* row groups */
	    TASSERT(table->rowhdrs != NULL);
	    if (table->rowhdrs[y]->rowgroup != NULL)
		props = table->rowhdrs[y]->rowgroup->props;
	    break;
	case 'c':       /* columns */
	    ASSERT(table->colhdrs != NULL);
	    props = table->colhdrs[x]->props;
	    break;
	case 'C':       /* column groups */
	    if (table->colhdrs[x]->colgroup != NULL)
		props = table->colhdrs[x]->colgroup->props;
	    break;
	case 't':       /* table */
	    props = table->props;
	    break;
	default:
	case 'd':       /* default */
	    /*TABDBGN(("rid_getprop(): returning default value\n"));*/
	    switch (prop)
	    {
	    case rid_PROP_HALIGN:
		* ((rid_halign_tag *)result) = hdr ? STYLE_ALIGN_CENTER : STYLE_ALIGN_LEFT;
	    return;
	    case rid_PROP_CH    :
		* ((char *)result) = '.';       /* needs lang attention */
	    return;
	    case rid_PROP_VALIGN:
		* ((rid_valign_tag *)result) = STYLE_VALIGN_MIDDLE;
	    return;
	    case rid_PROP_DIR   :
		* ((rid_dir_tag *)result) = rid_dt_LTR;
	    return;
	    case rid_PROP_BGCOLOR:
		/* Should never be asked for */
/* 		TASSERT(0 == 1); */
		*((int *)result) = NO_BGCOLOR;
	    return;
	    case rid_PROP_LANG  :
	    case rid_PROP_STYLE :
	    case rid_PROP_CHOFF :
		/* not used yet - might need notable work */
		return;
	    default:
		usrtrc( "\n\n\n\n\n\n\nConfused about inheritence order\n\n\n\n\n\n\n");
		return;
	    }
	    break;
	}

	if (props == NULL)
	    continue;

	/*TABDBGN(("rid_getprop(): props is %p\n", props));*/

	switch (prop)
	{
	case rid_PROP_HALIGN:
	    if (props->halign != 0)
	    {
		* ((rid_halign_tag *)result) = props->halign;
		return;
	    }
	    break;
	case rid_PROP_CH    :
	    if (props->ch)
	    {
		* ((char *)result) = props->ch;
		return;
	    }
	    break;
	case rid_PROP_DIR   :
	    if (props->dir != 0)
	    {
		* ((rid_dir_tag *)result) = props->dir;
		return;
	    }
	    break;
	case rid_PROP_VALIGN:
	    if (props->valign != 0)
	    {
		* ((rid_valign_tag *)result) = props->valign;
		return;
	    }
	    break;
	case rid_PROP_BGCOLOR:
	    if (props->flags & rid_tpf_BGCOLOR)
	    {
		* ((int *)result) = props->bgcolor;
		return;
	    }
	    break;
	case rid_PROP_LANG  :
	case rid_PROP_CHOFF :
	case rid_PROP_STYLE :
	    return;
	}
    }
}

/*****************************************************************************

  Set the scaffolding for the next cell to use and return TRUE, or return
  FALSE if no empty cell can be found. Might insert <TR> elements as
  necessary.

  */

static BOOL find_empty_cell_this_line(rid_table_item *table)
{
    const int y = table->scaff.y;
    int x = table->scaff.x;
    rid_table_cell *cell;

    TABDBGN(("find_empty_cell_this_line(): looking from (%d,%d)\n",
	    table->scaff.x, table->scaff.y));

    for (; x < table->cells.x; x++)
    {
	if ( *CELLFOR(table,x,y) == NULL )
	{
	    table->scaff.x = x;
	    return TRUE;
	}
    }

    if ( (table->flags & rid_tf_COLS_FIXED) != 0 )
	return FALSE;

    x--;

    if (x >= 0)
    {
	cell = *CELLFOR(table, x, y);
	ASSERT(cell != NULL);
	if ( (cell->flags & rid_cf_INF_HORIZ) != 0 )
	{
	    TABDBGN(("This row has an INF_HORIZ cell ending it\n"));
	    return FALSE;
	}
    }

    add_retro_col(table);

    table->scaff.x = x+1;

    return TRUE;
}

static BOOL find_empty_cell(HTMLCTX *htmlctx, rid_table_item *table)
{
    /* Shouldn't get here if flagged as being full */
    ASSERT( (table->flags & rid_tf_NO_MORE_CELLS) == 0 );

    TABDBGN(("find_empty_cell(): start from (%d,%d)\n",
	    table->scaff.x, table->scaff.y));

    while (TRUE)
    {
	const int y = table->scaff.y;
	int x;
	BOOL inf = TRUE;

	if ( find_empty_cell_this_line(table) )
	{
	    TABDBGN(("Found an empty cell\n"));
	    return TRUE;
	}

	/* won't fit on this line - insert presumed implied <TR> */
	/* until a free cell is made available, unless all the */
	/* cells have infinite height, in which case leave the */
	/* current cell selected. */

	TABDBGN(("Cell won't fit - earlier COLSPAN=0 on this row\n"));

	for (x = 0; inf && x < table->cells.x; x++)
	{
	    rid_table_cell *cell = *CELLFOR(table, x, y);

	    if (cell == NULL || (cell->flags & rid_cf_INF_VERT) == 0 )
	    {
		inf = FALSE;
	    }
	}

	if (inf)
	{
	    TABDBGN(("Infinite cells occupy rest of table - no more room\n"));
	    return FALSE;
	}

	TABDBGN(("Growing table to create an empty cell\n"));

	/* sets scaff.x and scaff.y */
	pseudo_html(htmlctx, "<TR>");
    }
}

/*****************************************************************************

  Delivery veneer placed around tables.

  */



static void table_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element)
{
    /*HTMLCTX *htmlctx = htmlctxof(context);*/

    switch (reason)
    {
    case DELIVER_WORD:
	PRSDBGN(("table_deliver(): got DELIVER_WORD !! '%.*s'\n", item.nchars, item.ptr));
	string_free(&item);
	break;

    default:
    case DELIVER_SPACE:
    case DELIVER_EOL:
    case DELIVER_NOP :
	break;

    case DELIVER_UNEXPECTED:
    {
	BOOL empty = FALSE;
	STRING i = item;
	PRSDBG(("table_deliver(): unexpected characters '%.*s'\n", item.nchars, item.ptr));

	while (i.nchars > 0)
	{
	    if ( *i.ptr <= 32 )
	    {
		i.ptr++;
		i.nchars--;
	    }
	    else
		break;
	}

	empty = i.nchars == 0;
	if (! empty)
	{
	    sgml_remove_deliver(context, &table_deliver);
	    (*context->deliver) (context, reason, item, element);
	    /* We expect/require <TD> to be inserted */
	    ASSERT(context->tos->element == HTML_TD);
	}
	else
	{
	    /* Doing nothing, but must free string. */
	    string_free(&item);
	}
    }
        break;

    case DELIVER_SGML:
	PRSDBGN(("table_deliver(): passing on <%.*s>\n", item.nchars, item.ptr));
	(*context->dlist->this_fn) (context, reason, item, element);
	break;

    case DELIVER_PRE_OPEN_MARKUP:

	PRSDBGN(("table_deliver(): removing table_deliver for pre open <%s>\n", element->name.ptr));
	sgml_remove_deliver(context, &table_deliver);
#if 0				/* SJM: 19Mar97commented out as this kind of stuff should be handled by the parser */
	if ( context->applying_rules )
	{
	    PRSDBGN(("Applying rules - preventing <TD> consideration\n"));
	}

	if ( context->applying_rules == 0 )
	{
	    /* Consider inserting <TD> at this point */
	    BOOL include = (element->flags & FLAG_CONTENT) == 0;

	    if (!include)
	    {
		if (element->group != HTML_GROUP_TABLE && element->group != HTML_GROUP_COLGROUP)
		    include = TRUE;
	    }

	    if (include)
	    {
		/* Element is either <TABLE> or not a table element */
		PRSDBGN(("table_deliver(): <%s> implies <TD>\n", element->name.ptr));
		pseudo_html(htmlctxof(context), "<td>");
	    }
	}
#endif
	PRSDBGN(("table_deliver(): <%s> now having it's PRE_OPEN delivered\n", element->name.ptr));
	(*context->deliver) (context, reason, item, element);
	break;

    case DELIVER_POST_OPEN_MARKUP:
	PRSDBGN(("table_deliver(): passing POST OPEN <%s> onwards\n", element->name.ptr));
	(*context->dlist->this_fn) (context, reason, item, element);
	break;

    case DELIVER_PRE_CLOSE_MARKUP:

	PRSDBGN(("table_deliver(): removing table_deliver before closing <%s>\n", element->name.ptr));
	sgml_remove_deliver(context, &table_deliver);
	(*context->deliver) (context, reason, item, element);
	break;

    case DELIVER_POST_CLOSE_MARKUP:
	PRSDBGN(("table_deliver(): passing on post closure for <%s>\n", element->name.ptr));
	(*context->dlist->this_fn) (context, reason, item, element);
	break;

	/* This cannot be ignored and must be correctly delivered. */
    case DELIVER_EOS:
	PRSDBGN(("table_deliver(): EOS forcing restoration of original delivery handler\n"));
	sgml_unwind_deliver(context);
	PRSDBG(("table_deliver(): doing the real EOS delivery now\n"));
	(*context->deliver) (context, reason, item, element);
	break;
    }
}







/*****************************************************************************

  <TABLE>

  A new table is inserted as a single rid_text_item within the current
  rid_text_stream.

  Values are parsed and a rid_table_item initialised accordingly.

  A rid_text_stream is created in the caption position to capture any
  "floating" text outside <TD> items.  If we didn't do this, the creation
  would be performed by table_start_caption().

  curstream of the rid_header is updated to reference the caption item.

  <TABLE COLS=N> causes initialisation and fixing of the number of columns.
  COLGROUP and COL can still initialise them.

  */

extern void starttable(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    static align_table at[] =
    {
	{ HTML_TABLE_ALIGN_LEFT,	STYLE_ALIGN_LEFT    },
	{ HTML_TABLE_ALIGN_CENTER,	STYLE_ALIGN_CENTER  },
	{ HTML_TABLE_ALIGN_CENTRE,	STYLE_ALIGN_CENTER  },
	{ HTML_TABLE_ALIGN_RIGHT,	STYLE_ALIGN_RIGHT   }
    };

    HTMLCTX *me = htmlctxof(context);
    rid_table_item *tab;
    rid_text_item_table *rtit;
/*     rid_table_caption *rtc; */
    rid_text_item *nb;
    VALUE *attr;
    int x;

    generic_start(context, element, attributes);

    set_lang(context, &attributes->value[HTML_TABLE_LANG]);

    if ( !config_display_tables )
    {
        TABDBG(("starttable: ignoring\n"));
        return;
    }

    ASSERT(context->dlist == NULL);
    ASSERT(context->deliver != &table_deliver);

    /* New block level stuff should ensure this? */
    /*must_have_a_word_pushed(me);*/

    /* tab is the new table. rtit contains tab and rtc is contained by tab */
    /* Very important that everything in the table starts at zero! */

    tab = (rid_table_item *) mm_calloc(1, sizeof(*tab));
    rtit = (rid_text_item_table *) mm_calloc(1, sizeof(*rtit));
/*  rtc = (rid_table_caption *) mm_calloc(1, sizeof(*rtc)); SJM: This was never used!!*/

    /* Set by default, clear if get a <COLGROUP> */
    tab->flags = rid_tf_IMPLIED_COLGROUP;
    tab->state = tabstate_PRE;

    /* Link structures together */


    /* DAF: 970319: moving unexpected characters to the */
    /* last available stream might wish to alter the aref */
    /* list order that we are creating here. @@@@ */


    rtit->table = tab;
    tab->parent = rtit;
    nb = &rtit->base;
    nb->tag = rid_tag_TABLE;
    nb->aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;

    GET_ROSTYLE(nb->st);	/* Must be before set anything */
    /* and before center stuff below */

    tab->props = parse_table_props(&no_value,
				   &attributes->value[HTML_TABLE_ALIGN],
				   &attributes->value[HTML_TABLE_CHAR],
				   &attributes->value[HTML_TABLE_CHAROFF],
				   &attributes->value[HTML_TABLE_LANG],
				   &attributes->value[HTML_TABLE_DIR],
				   &attributes->value[HTML_TABLE_STYLE],
				   &attributes->value[HTML_TABLE_BGCOLOR],
				   NULL,
				   at);

    if ( (attr = &attributes->value[HTML_TABLE_BGCOLOR])->type != value_none)
	tab->flags |= rid_tf_BGCOLOR;
    if ( (attr = &attributes->value[HTML_TABLE_CLASS])->type == value_string)
	tab->class = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_TABLE_ID])->type == value_string)
	tab->id = stringdup(attr->u.s);
    tab->userwidth = attributes->value[HTML_TABLE_WIDTH];
    tab->userheight = attributes->value[HTML_TABLE_HEIGHT];

#if 0
    switch (tab->userwidth.type)
    {
    case value_absunit:
	/* Per format, not sole forever */
	/*tab->flags |= rid_tf_HAVE_WIDTH;*/
	break;

    case value_pcunit:
    case value_relunit:
	TABDBG(("starttable: Forcing out %%/REL WIDTH constraint for now\n"));
	/* Fall through */

    default:
	tab->userwidth.type = value_none;
	break;
    }
#endif

    if (tab->props != NULL)
    {
	TABDBG(("Setting table alignment %d\n", tab->props->halign));

	switch ( tab->props->halign )
	{
#if 1
	case STYLE_ALIGN_LEFT:
	    nb->flag |= rid_flag_LEFTWARDS;
	    floating_table_format_alert_begin(context);
	    break;
	case STYLE_ALIGN_RIGHT:
	    nb->flag |= rid_flag_RIGHTWARDS;
	    floating_table_format_alert_begin(context);
	    break;
#endif
	case STYLE_ALIGN_CENTER:
	    nb->st.flags &= ~rid_sf_ALIGN_MASK;
	    nb->st.flags |= rid_sf_ALIGN_CENTER;
	    break;
	}
    }


    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	/* SJM: tables now have to manage their own breaks so ensure one if we are not floating */
	if ((nb->flag & (rid_flag_LEFTWARDS|rid_flag_RIGHTWARDS)) == 0)
	{
	    rid_text_item *ti;
	    if ( (ti = me->rh->curstream->text_last) != NULL )
#if NEW_BREAKS
		if (GET_BREAK(ti->flag) == rid_break_CAN)
		    SET_BREAK(ti->flag, rid_break_MUST);
#else
		ti->flag |= rid_flag_LINE_BREAK;
#endif    
	    /* DAF: 970319: don't put back with NEW_UNEXP_TABLE */
/* 	text_item_ensure_break(me); */
	}
    }

#if 0
    text_item_push_word(me, rid_flag_NO_BREAK, FALSE); /* empty item stuck to the front */
    nb->flag |= rid_flag_NO_BREAK;		       /* table sticks to empty item afterwards */
#endif

    /* Handle BORDER, CELLSPACING, CELLPADDING, FRAME, RULES */
    /* such that they are upwardly compatible with Netvirus. */
    /* The order of these is important */

    tab->usercellpadding = attributes->value[HTML_TABLE_CELLPADDING];
    if (tab->usercellpadding.type != value_integer ||
	(tab->cellpadding = tab->usercellpadding.u.i*2) < 0 )
    {
	tab->cellpadding = DEFAULT_CELL_PADDING;
    }

    tab->usercellspacing = attributes->value[HTML_TABLE_CELLSPACING];
    if (tab->usercellspacing.type != value_integer ||
	(tab->cellspacing = tab->usercellspacing.u.i*2) < 0 )
    {
	tab->cellspacing = DEFAULT_CELL_SPACING;
    }

    tab->border = DEFAULT_BORDER;
    tab->rules = HTML_TABLE_RULES_NONE;
    tab->frame = HTML_TABLE_FRAME_VOID;

    if ( (tab->userborder = attributes->value[HTML_TABLE_BORDER]).type == value_integer )
    {
	x = 2 * tab->userborder.u.i;
	if (x < 2)
	{
	    tab->border = 0;
	}
	else
	{
	    tab->border = x > 254 ? 254 : x;
	    tab->frame = HTML_TABLE_FRAME_BORDER;
	    tab->rules = HTML_TABLE_RULES_ALL;
	    /* Make room if necessary */
	    if (tab->cellspacing < 2)
		tab->cellspacing = 2;
	}
    }
    else if (tab->userborder.type == value_void)
    {
	tab->frame = HTML_TABLE_FRAME_BORDER;
	tab->rules = HTML_TABLE_RULES_ALL;
	/* Make room if necessary */
	if (tab->cellspacing < 2)
	    tab->cellspacing = 2;
    }

    if ( attributes->value[HTML_TABLE_FRAME].type == value_enum )
    {
	tab->frame = attributes->value[HTML_TABLE_FRAME].u.i;
	if (tab->frame == HTML_TABLE_FRAME_BOX)
	    tab->frame = HTML_TABLE_FRAME_BORDER;
	/* Make room if necessary */
	if (tab->frame != HTML_TABLE_FRAME_VOID)
	{
	    if (tab->cellspacing <= 2 || tab->border < 2)
	    {
		/* @@@@ In most shrunk case, this gives too large a table? */
		tab->border = 2;
		tab->cellspacing=2;
	    }
	}
    }

    if ( attributes->value[HTML_TABLE_RULES].type == value_enum )
    {
	tab->rules = attributes->value[HTML_TABLE_RULES].u.i;
	/* Make room if necessary */
	if (tab->rules != HTML_TABLE_RULES_NONE && tab->cellspacing < 2)
	    tab->cellspacing = 2;
    }

    /* Can we do on-the-fly rendering? Nowadays, more like is the number */
    /* of columns fixed or variable? */
#ifndef STBWEB
    if ( (attr = &attributes->value[HTML_TABLE_COLS])->type == value_integer)
    {
	/* We'll ignore if it looks bogus */
	int c = attr->u.i;
	if (c > 0)
	{
	    TABDBG(("starttable(): fixed at %d columns\n", c));
	    if (tab->userwidth.type == value_none)
	    {       /* Suggested by 23-1-96 DRAFT */
		tab->userwidth.type = value_pcunit;
		tab->userwidth.u.f = 100.0;
	    }
	    while (c--)
		add_new_colhdr(tab);
	    tab->flags |= rid_tf_COLS_FIXED;
	}
	/* else let table size itself */
    }
#endif

    tab->oldstream = me->rh->curstream;
    tab->oldtable = me->table;
    me->table = tab;

    me->rh->table_depth++;
    me->rh->idnum++;

    tab->depth = me->rh->table_depth;
    tab->idnum = me->rh->idnum;

    if (gbf_active(GBF_TABLES_UNEXPECTED))
    {
	PRSDBG(("Delaying connecting the table to the stream\n"));

	ASSERT(me->rh->curstream != NULL);
    }
    else
    {
	rid_text_item_connect(me->rh->curstream, &rtit->base);

	/* NULL is the most dangerous choice, but won't matter */
	/* if everything else is correct. */
	me->rh->curstream = TABLE_NULL;
    }

    TABDBGN(("Started table %p, current stream now %p\n", tab, me->rh->curstream));

    /* Patch in handler for <TABLE><H1> corrections */

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

/*****************************************************************************

  </TABLE>

  This routine is called when the </TABLE> tag is found.  This triggers
  some post processing.

  me->rh->curstream and me->table are updated to reflect the unwinding of
  nesting.

  Maybe should add a soft space after the table - this means, once the
  document has been fully parsed, text_last of rid_text_stream cannot refer
  to a table, which simplifies possible reformatting tests (might otherwise
  have last item as a table that was incomplete and is now complete but the
  text_last item is still the same).

  */

extern void finishtable(SGMLCTX *context, ELEMENT *element)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;

    generic_finish(context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finishtable: ignoring\n"));
        return;
    }

    restrain_rowspan_cells(me, table);

#if 0 && DEBUG
    dump_table(table, NULL);
#endif

    if (table->flags & rid_tf_HAVE_TFOOT)
	do_tfoot_swapping(table);

    tidy_table(me, table);

    me->rh->curstream = table->oldstream;
    me->table = table->oldtable;
    me->rh->table_depth--;

    if (gbf_active(GBF_TABLES_UNEXPECTED))
    {
	rid_text_item *nb = &table->parent->base;

	PRSDBG(("Delayed table text item connection now happening.\n"));

	ASSERT(me->rh->curstream != NULL);

	/* SJM: tables now have to manage their own breaks so ensure one if we are not floating */
	if ((nb->flag & (rid_flag_LEFTWARDS|rid_flag_RIGHTWARDS)) == 0)
	{
	    rid_text_item *ti;
	    if ( (ti = me->rh->curstream->text_last) != NULL )
#if NEW_BREAKS
		if (GET_BREAK(ti->flag) == rid_break_CAN)
		    SET_BREAK(ti->flag, rid_break_MUST);
#else
		ti->flag |= rid_flag_LINE_BREAK;
#endif    
	}
	rid_text_item_connect(me->rh->curstream, &table->parent->base);
    }

    /* So formatter can handle floating tables easier */
    table->flags |= rid_tf_FINISHED;

    if (table->props != NULL)
    {
	rid_text_item *nb = &table->parent->base;

	switch ( table->props->halign )
	{
	case STYLE_ALIGN_LEFT:
	    nb->flag |= rid_flag_LEFTWARDS;
	    floating_table_format_alert_finish(context);
	    break;
	case STYLE_ALIGN_RIGHT:
	    nb->flag |= rid_flag_RIGHTWARDS;
	    floating_table_format_alert_finish(context);
	    break;
	}
    }

    /* New block level stuff should ensure this? */
    /*must_have_a_word_pushed(me);*/

    /* SJM: tables now have to manage their own breaks so ensure one if we are not floating */
    /* SJM: push null item for convenience of scanning */
#if NEW_BREAKS
    rid_scaff_item_push(me->rh->curstream,
			(table->parent->base.flag & (rid_flag_LEFTWARDS|rid_flag_RIGHTWARDS)) == 0 ? rid_break_MUST : rid_break_CAN);
#else
    if ((table->parent->base.flag & (rid_flag_LEFTWARDS|rid_flag_RIGHTWARDS)) == 0)
	text_item_push_word(me, rid_flag_LINE_BREAK, FALSE);
    else
	text_item_push_word(me, 0, FALSE);
#endif
    
#if DEBUG == 3
    dump_cell_map(table, "</TABLE>");
#endif
}

/*****************************************************************************

  <CAPTION>

  The caption alignment is recorded directly, rather than being
  recorded in the style - it's enough of a one off.

  */

extern void startcaption(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_caption *cap;
    rid_table_item *tab = me->table;
    VALUE *attr;

    TABDBG(("Starting to do a caption\n"));

    generic_start(context, element, attributes);

#if IGNORE_CAPTION
    if ( !config_display_tables )
    {
        TABDBG(("caption: ignoring\n"));
        return;
    }

    cap = (rid_table_caption *) mm_calloc(1, sizeof(*cap));
    tab->caption = cap;
    cap->table = tab;
    cap->stream.parent = cap;
    cap->stream.partype = rid_pt_CAPTION;
    me->rh->curstream = &cap->stream;

    if ( (attr = &attributes->value[HTML_CAPTION_CLASS])->type == value_string )
	cap->class = stringdup(attr->u.s);

    if ( (attr = &attributes->value[HTML_CAPTION_ID])->type == value_string )
	cap->id = stringdup(attr->u.s);

    switch ( (attr = &attributes->value[HTML_CAPTION_ALIGN])->type == value_enum
	     ? attr->u.i : HTML_CAPTION_ALIGN_TOP )
    {
    case HTML_CAPTION_ALIGN_TOP:
	cap->calign = rid_ct_TOP;
	break;
    case HTML_CAPTION_ALIGN_BOTTOM:
	cap->calign = rid_ct_BOTTOM;
	break;
    case HTML_CAPTION_ALIGN_LEFT:
	cap->calign = rid_ct_LEFT;
	break;
    case HTML_CAPTION_ALIGN_RIGHT:
	cap->calign = rid_ct_RIGHT;
	break;
    }

    cap->props = parse_table_props(&no_value,
				   &no_value,
				   &no_value,
				   &no_value,
				   &attributes->value[HTML_CAPTION_LANG],
				   &attributes->value[HTML_CAPTION_DIR],
				   &attributes->value[HTML_CAPTION_STYLE],
				   &no_value,
				   NULL,
				   NULL);

    SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_CENTER);
#endif
    TABDBG(("Caption entered\n"));
}

/* If no words have been added, remove the caption now so that */
/* no space is given for any caption borders, etc. */

extern void finishcaption(SGMLCTX *context, ELEMENT *element)
{
    HTMLCTX *htmlctx = htmlctxof(context);
    rid_table_item *table = htmlctx->table;
    rid_table_caption *caption;

    generic_finish(context, element);

#if IGNORE_CAPTION
    if ( !config_display_tables )
    {
        TABDBG(("finishcaption: ignoring\n"));
        return;
    }

    /* DAF: 970701: AFTER rejecting case where caption does not exist! */
    caption = table->caption;

    if (caption->stream.text_list == NULL)
    {
	TABDBG(("finishcaption(): no text items so freeing caption\n"));
	rid_free_caption(caption);
	table->caption = NULL;
    }

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	PRSDBG(("finishcaption(): patching back to table_deliver\n"));
	htmlctx->rh->curstream = NULL;

	sgml_install_deliver(context, &table_deliver);
    }
    else
    {
	htmlctx->rh->curstream = table->oldstream;
    }
#endif
}

/*****************************************************************************

  COLGROUPSECTION is an artificial tag used to spot the correct point
  where corrective actions mya be required for the section of the
  table specifying column information and to serve as a structuring
  unit for element insertion with the parser..

  If no column group has been created at </COLGROUPSECTION> then we
  create a single default <COLGROUP>. This has to happen here as
  otherwise it would not span any columns. If the table has not been
  given a <TABLE COLS=> bound, then the colgroup extends as far to the
  right as is needed to keep everything in a single column group,
  including growing as more columns are added. If we are bounded, then
  the column group spans the relevant number of columns and is fixed
  width.

  Any <COL> processed with have a column group to reference., except
  in the case that <TABLES COLS=N> created the columns and then no
  <COL> was given. This will need patching.

  After </COLGROUPSECTION>, one of the following conditions must hold:

  1) rid_tf_COLS_FIXED set: One or more COLGROUPs exist, with every
  column referencing a column group. We cannot add any more columns
  and no column group can change size.

  2) rid_tf_COLS_FIXED clear, cur_colgroup == NULL: Every column that
  exists will be in a column group. If another column is implicitly
  created, then a colunm group must also be created. This will be the
  column group for all subsequent columns.

  3) rid_tf_COLS_FIXED clear, cur_colgroup != NULL: any new columns
  will be assigned to the current column group. This group might have
  a span that runs out. If so, it will be replaced with a group
  extending far right.

 */

extern void startcolgroupsection (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    if ( !config_display_tables )
    {
        TABDBG(("startcolgroupsection: ignoring\n"));
        return;
    }

    me->table->flags |= rid_tf_COLGROUPSECTION;

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

extern void finishcolgroupsection (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;

    generic_finish (context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finishcolgroupsection: ignoring\n"));
        return;
    }

    TABDBG(("Checking for columns not assigned to a column group yet\n"));

    if ( (table->flags & rid_tf_IMPLIED_COLGROUP) != 0 )
    {
	int x;

	TABDBG(("finishcolgroupsection(): creating a column group to span all columns\n"));

	ASSERT(table->num_groups.x == 0);
	ASSERT(table->colgroups == NULL);

	table->cur_colgroup = add_new_colgroup(table);

	ASSERT(table->cur_colgroup != NULL);

	for (x = 0; x < table->cells.x; x++)
	{
	    table->colhdrs[x]->colgroup = table->cur_colgroup;
	    table->cur_colgroup->span++;
	}
    }

    table->flags &= ~( rid_tf_COLGROUPSECTION |
		       rid_tf_NO_MORE_CELLS |
		       rid_tf_IMPLIED_COLGROUP        /* pdh: added this */
                         );

#if DEBUG == 3
    dump_table(table, NULL);
#endif

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

/*****************************************************************************

  <COLGROUP>

  Starts a new column group, unless columns already full.  If a span
  is supplied, start with this but get ready for <COL> overwriting it.
  Span can be zero to permit things like <COLGROUP SPAN=0 ID="wibble">

  If no more cells will fit, then we do not add a COLGROUP, even for
  <COLGROUP SPAN=0 ID="wibble"></COLGROUP>

  */


extern void startcolgroup(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;
    rid_table_colgroup *colgroup;
    int cols;
    VALUE *attr;

    generic_start(context, element, attributes);

    if ( !config_display_tables )
    {
        TABDBG(("startcolgroup: ignoring\n"));
        return;
    }

    if ( (table->flags & rid_tf_NO_MORE_CELLS) != 0 )
    {
	TABDBG(("startcolgroup(): rid_tf_NO_MORE_CELLS set - not doing anything\n"));
	return;
    }

    /* else must be enough room for at least one cell */

    table->flags &= ~rid_tf_IMPLIED_COLGROUP;
    colgroup = add_new_colgroup(table);
    colgroup->userwidth = attributes->value[HTML_COLGROUP_WIDTH];

    colgroup->props = parse_table_props(&attributes->value[HTML_COLGROUP_VALIGN],
					&attributes->value[HTML_COLGROUP_ALIGN],
					&attributes->value[HTML_COLGROUP_CHAR],
					&attributes->value[HTML_COLGROUP_CHAROFF],
					&attributes->value[HTML_COLGROUP_LANG],
					&attributes->value[HTML_COLGROUP_DIR],
					&attributes->value[HTML_COLGROUP_STYLE],
					&attributes->value[HTML_COLGROUP_BGCOLOR],
					valign_table,
					halign_table);

    if (attributes->value[HTML_COLGROUP_BGCOLOR].type != value_none)
	table->flags |= rid_tf_BGCOLOR;
    if ( (attr = &attributes->value[HTML_COLGROUP_ID])->type == value_string)
	colgroup->id = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_COLGROUP_CLASS])->type == value_string)
	colgroup->class = stringdup(attr->u.s);

    table->cur_colgroup = colgroup;

    attr = &attributes->value[HTML_COLGROUP_SPAN];

    if ( attr->type != value_integer )
    {
	cols = 1;
    }
    else
    {
	cols = attr->u.i;

	if (cols < 0)
	    cols = 0;
    }

    colgroup->span = cols;

    if (cols > 0)
    {       /* <COLGROUP> may span zero columns */
	table->flags |= rid_tf_GROUP_SPAN;
    }

    table->flags |= rid_tf_IN_COLGROUP;

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

/*****************************************************************************

  </COLGROUP>

  Close the current column group. If no <COL> items were found, then we
  can use any <COLGROUP SPAN=N> value.

  */

extern void finishcolgroup(SGMLCTX *context, ELEMENT *element)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;
    rid_table_colgroup *group;
    int x;

    generic_finish(context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finishcolgroup: ignoring\n"));
        return;
    }

    ASSERT( (table->flags & rid_tf_IN_COLGROUP) != 0 );
    ASSERT( table->cur_colgroup != NULL );
    ASSERT( table->num_groups.x > 0 );

    table->flags &= ~ rid_tf_IN_COLGROUP;
    table->cur_colgroup = NULL;

    if ( (table->flags & rid_tf_GROUP_SPAN) == 0 )
	return;

    TABDBG(("Create implied columns - no <COL> within <COLGROUP>\n"));

    table->flags &= ~rid_tf_GROUP_SPAN;
    group = table->colgroups[table->num_groups.x - 1];

    ASSERT(group->span != 0) ;

    if ( (table->flags & rid_tf_COLS_FIXED) != 0 )
    {
	TABDBG(("Constrained by <TABLE COLS=N>\n"));
	ASSERT( table->scaff.x < table->cells.x );
	if (table->scaff.x + group->span > table->cells.x)
	{
	    TABDBG(("Now accounted for all columns\n"));
	    table->flags |= rid_tf_NO_MORE_CELLS;
	    group->span = table->cells.x - table->scaff.x;
	}

	TABDBGN(("Setting colgroup of %d columns\n", group->span));

	for (x = table->scaff.x; x < table->scaff.x + group->span; x++)
	    table->colhdrs[x]->colgroup = group;
	table->scaff.x += group->span;
    }
    else
    {       /* Create columns as necessary */
	TABDBG(("finishcolgroup(): Creating columns due to <COLGROUP SPAN=%d>\n", group->span));
	for (x = 0; x < group->span; x++)
	    add_new_colhdr(table)->colgroup = group;
	table->scaff.x = table->cells.x;
    }

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

/*****************************************************************************

  <COL>

  Add another column header (rid_table_colhdr) to the table.  If the
  maximum number of columns have already been supplied, do nothing.
  Replicate attributes across multiple columns if <COL SPAN=N> with N > 1.
  A <COL SPAN=0> does not automatically prevent other <COL> items being
  processed - it merely overrides the user attributes with those of the
  colhdr being replicated.

  We must be in a COLGROUP: increment it's span.  xscaff on entry
  indicates the (first) column we refer to.  xscaff on exit indicates
  the next column to write to, or rid_tf_NO_MORE_CELLS is set to
  indicate we cannot accomodate any more colhdr items (either reached
  column count or last colhdr extends to extent of table).

  */


extern void startcol(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;
    rid_table_colhdr *hdr;
/*    rid_table_colgroup *group;*/
    int cols, first, count, x;
    VALUE *attr;

    generic_start(context, element, attributes);

    if ( !config_display_tables )
    {
        TABDBG(("startcol: ignoring\n"));
        return;
    }

    if ( (table->flags & rid_tf_NO_MORE_CELLS) != 0 )
    {
	TABDBG(("startcol(): rid_tf_NO_MORE_CELLS set - not doing anything\n"));
	return;
    }

    ASSERT( table->cur_colgroup != NULL );
    ASSERT( (table->flags & rid_tf_IN_COLGROUP) != 0 );

    if ( (table->flags & rid_tf_GROUP_SPAN) != 0 )
    {
	TABDBG(("startcol(): Cancelling <COLGROUP SPAN=%d> from earlier\n",
		table->cur_colgroup->span));
	table->flags &= ~rid_tf_GROUP_SPAN;
	table->cur_colgroup->span = 0;
    }

    attr = &attributes->value[HTML_COL_SPAN];

    if ( attr->type != value_integer )
    {
	cols = 1;
    }
    else if ( (cols = attr->u.i) < 0)
    {
	cols = 1;
    }

    if ( (table->flags & rid_tf_COLS_FIXED) != 0 )
	start_col_fixed(table, cols, &first, &count);
    else
	start_col_growing(table, cols, &first, &count);

    /* Columns will have been created. Range to initialise has */
    /* been determined. Any replication has already been handled. */

    TABDBG(("startcol(): <COL SPAN=%d>: properties init: first %d, count %d\n",
	    cols, first, count));

    for (x = first; count > 0; x++, count--)
    {
	hdr = table->colhdrs[x];
	hdr->props = parse_table_props(&attributes->value[HTML_COL_VALIGN],
				       &attributes->value[HTML_COL_ALIGN],
				       &attributes->value[HTML_COL_CHAR],
				       &attributes->value[HTML_COL_CHAROFF],
				       &attributes->value[HTML_COL_LANG],
				       &attributes->value[HTML_COL_DIR],
				       &attributes->value[HTML_COL_STYLE],
				       &attributes->value[HTML_COL_BGCOLOR],
				       valign_table,
				       halign_table);

	if (attributes->value[HTML_COL_BGCOLOR].type != value_none)
	    table->flags |= rid_tf_BGCOLOR;

	hdr->userwidth = attributes->value[HTML_COL_WIDTH];
	hdr->colgroup = table->cur_colgroup;

	if ( (attr = &attributes->value[HTML_COL_ID])->type == value_string)
	    hdr->id = stringdup(attr->u.s);
	if ( (attr = &attributes->value[HTML_COL_CLASS])->type == value_string)
	    hdr->class = stringdup(attr->u.s);
    }

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }

    return;
}

/*****************************************************************************

  <THEAD> <TFOOT> <TBODY>

  element_number is one of HTML_THEAD, HTML_TFOOT or HTML_TBODY.  Each
  introduces the start of a new row group, which must contain at least one
  <TR> item.  This creates a new entry in the rowgroups list, and advances
  the state.

  */


static void start_headfootbody(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;
    rid_table_rowgroup *rowgroup;
    VALUE *attr;

    generic_start(context, element, attributes);

    rowgroup = add_new_rowgroup(table);
    rowgroup->props = parse_table_props(&attributes->value[HTML_TBODY_VALIGN],
					&attributes->value[HTML_TBODY_ALIGN],
					&attributes->value[HTML_TBODY_CHAR],
					&attributes->value[HTML_TBODY_CHAROFF],
					&attributes->value[HTML_TBODY_LANG],
					&attributes->value[HTML_TBODY_DIR],
					&attributes->value[HTML_TBODY_STYLE],
					&attributes->value[HTML_TBODY_BGCOLOR],
					valign_table,
					halign_table);

    if (attributes->value[HTML_TBODY_BGCOLOR].type != value_none)
	table->flags |= rid_tf_BGCOLOR;
    if ( (attr = &attributes->value[HTML_TBODY_ID])->type == value_string)
	rowgroup->id = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_TBODY_CLASS])->type == value_string)
	rowgroup->class = stringdup(attr->u.s);

    table->cur_rowgroup = rowgroup;

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

extern void startthead(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    rid_table_item *table = htmlctxof(context)->table;

    if ( !config_display_tables )
    {
        TABDBG(("startthead: ignoring\n"));
        return;
    }

    start_headfootbody(context, element, attributes);

    table->rowgroups[table->num_groups.y-1]->flags |= rid_rgf_THEAD;
}

extern void starttfoot(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    rid_table_item *table = htmlctxof(context)->table;

    if ( !config_display_tables )
    {
        TABDBG(("starttfoot: ignoring\n"));
        return;
    }

    start_headfootbody(context, element, attributes);

    table->rowgroups[table->num_groups.y-1]->flags |= rid_rgf_TFOOT;
    table->flags |= rid_tf_HAVE_TFOOT | rid_tf_TFOOT_INVISIBLE;
}

extern void starttbody(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    rid_table_item *table = htmlctxof(context)->table;

    if ( !config_display_tables )
    {
        TABDBG(("starttbody: ignoring\n"));
        return;
    }

    start_headfootbody(context, element, attributes);

    table->rowgroups[table->num_groups.y-1]->flags |= rid_rgf_TBODY;
}

extern void finishthead(SGMLCTX *context, ELEMENT *element)
{
    generic_finish(context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finishthead: ignoring\n"));
        return;
    }

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

extern void finishtfoot(SGMLCTX *context, ELEMENT *element)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;

    generic_finish(context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finishtfoot: ignoring\n"));
        return;
    }

    /* Need to restrain now, as growing to the next row down counts */
    /* as a fracture on screen, jumping the cell back upwards. */
    /* We always get a TFOOT from the automatic insertion, but it */
    /* might not contain anything */

    if (table->rowgroups[table->num_groups.y-1]->span > 0)
    {
	restrain_rowspan_cells(me, me->table);
    }
    else
    {
	table->flags &= ~(rid_tf_TFOOT_INVISIBLE | rid_tf_HAVE_TFOOT);
    }

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

extern void finishtbody(SGMLCTX *context, ELEMENT *element)
{
    generic_finish(context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finishtbody: ignoring\n"));
        return;
    }

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

/*****************************************************************************

  <TR>

  Start a new table row.  We might increment the header_rows or footer_rows
  values.  A new row is added to the table, initially containing empty
  cells.

  Any previous cells that have not yet finished spreading are grown down by
  one row.  A new rowhdr value is added and filled in with the <TR>
  attributes.  We then determine the first free position to put the
  scaffold cursor at.  This can lead to rid_tf_NO_MORE_CELLS being set if
  all the cells of the row were occupied by the spreading.  If this flag is
  not set, the scaffolding refers to the cell to contain the text stream
  that the next <TH> or <TD> should create and attach.


  */

extern void starttr(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;
    rid_table_rowhdr *hdr;
    VALUE *attr;

    generic_start(context, element, attributes);

    set_lang(context, &attributes->value[HTML_TR_LANG]);

    if ( !config_display_tables )
    {
        TABDBG(("starttr: ignoring\n"));
        return;
    }

    if ( (table->flags & rid_tf_NO_MORE_CELLS) != 0 )
    {
	TABDBG(("starttr(): table full\n"));
	return;
    }

    TABDBG(("starttr(): scaff (%d,%d), cells (%d,%d)\n",
	    table->scaff.x, table->scaff.y, table->cells.x, table->cells.y));

    /* Add another row, and perform any necessary spreading */

    add_new_row(table);		/* bumps row span */

    /* fill in rowhdr values */

    ASSERT( table->cur_rowgroup->span != 0 );
    /*table->cur_rowgroup->span += 1;*/
    hdr = table->rowhdrs[table->cells.y - 1];
    hdr->props = parse_table_props(&attributes->value[HTML_TR_VALIGN],
				   &attributes->value[HTML_TR_ALIGN],
				   &attributes->value[HTML_TR_CHAR],
				   &attributes->value[HTML_TR_CHAROFF],
				   &attributes->value[HTML_TR_LANG],
				   &attributes->value[HTML_TR_DIR],
				   &attributes->value[HTML_TR_STYLE],
				   &attributes->value[HTML_TR_BGCOLOR],
				   valign_table,
				   halign_table);

    if (attributes->value[HTML_TR_BGCOLOR].type != value_none)
	table->flags |= rid_tf_BGCOLOR;
    if ( (attr = &attributes->value[HTML_TR_ID])->type == value_string)
	hdr->id = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_TR_CLASS])->type == value_string)
	hdr->class = stringdup(attr->u.s);

    /* Position the scaffolding for where we are to start looking for an empty cell */

    ASSERT(table->cells.y > 0);
    table->scaff.x = 0;
    table->scaff.y = table->cells.y - 1;

    if (!gbf_active(GBF_TABLES_UNEXPECTED))
    {
	sgml_install_deliver(context, &table_deliver);
    }
}

extern void finishtr(SGMLCTX *context, ELEMENT *element)
{
    generic_finish(context, element);

/*     sgml_install_deliver(context, &table_deliver); */
}

/*****************************************************************************

  Choose the position for the next cell. We must have had at least a
  <TR> to put us into a valid row. Set the scaffolding position to
  where the next cell goes, or set rid_tf_NO_MORE_CELLS (which now
  means per table, rather than per line).

 */

extern void pre_thtd_warning(HTMLCTX *me)
{
    rid_table_item *table = me->table;

    if (!config_display_tables)
    {
	TABDBG(("ignoring pre_thtd_warning\n"));
	return;
    }

    TABDBGN(("pre_thtd_warning(): looking for new cell position, scaff (%d,%d), cells (%d,%d)\n",
	    table->scaff.x, table->scaff.y, table->cells.x, table->cells.y));

    if ( (table->flags & rid_tf_NO_MORE_CELLS) != 0 )
    {
	TABDBG(("pre_thtd_warning(): table is full\n"));
	return;
    }

    ASSERT(table->scaff.y == 0 || table->scaff.y == table->cells.y - 1);

    if ( ! find_empty_cell(me, table) )
    {
	TABDBG(("pre_thtd_warning(): table has become full\n"));
	table->flags |= rid_tf_NO_MORE_CELLS;
	return;
    }
    else
    {
	TABDBGN(("pre_thtd_warning(): chosen (%d,%d) from (%d,%d) table\n",
		table->scaff.x, table->scaff.y, table->cells.x, table->cells.y));

	ASSERT( table->scaff.x < table->cells.x);
	ASSERT( table->scaff.y < table->cells.y);
    }
}

/*****************************************************************************

  <TH> <TD>

  Attempt to create a new text stream for writing into.  The scaffolding
  indicates where to start looking for an empty cell - it might not be
  empty itself.

  If we are operating with a known number of columns, we might not be able
  to find an empty cell. Even if we are operating with a flexible number
  of columns, we still might not be able to find an item. If a new cell
  cannot be located, the current cell (or caption) is retained.

  Adding a rid_cf_INF_HORIZ item will set rid_tf_NO_MORE_CELLS -
  new_retro_col() will ensure this gets grown as far as
  required. Otherwise, all the span of a cell will either be created
  or truncated (eg ROWSPAN=2 from above).

  DAF: 970315: Above logic fails for <table> <tr> <td colspan=3>0,0 -
  2,0 <tr> <td colspan=0>0,1 - 2,1 </table>

  When the current row cannot offer a location for the cell, we need
  to check whether we should insert <TR> elements or stop because an
  infinite number of <TR> elements will never find an empty cell. This
  test is reduced down to always stopping if all the cells in a row
  have the infinite vertical flag set. There are combinations of cells
  where we do not realise the infinite nature of the table until more
  rows have been added.

  */

static void start_tdth(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_item *table = me->table;
    rid_table_cell *cell, **cellp;
    unsigned char tag;
    int x;
    VALUE *attr;

    TABDBGN(("\n"));

    generic_start(context, element, attributes);

    set_lang(context, &attributes->value[HTML_TD_LANG]);

    if ( !config_display_tables )
    {
        TABDBG(("start_tdth: ignoring\n"));
        return;
    }

    /* Might be directed not to add more cells */
    if ( (table->flags & rid_tf_NO_MORE_CELLS) != 0 )
	return;

    /* Create a new cell and text stream at the specified scaffold position */
    cellp = CELLFOR(table, table->scaff.x, table->scaff.y);
    cell = mm_calloc(1, sizeof(*cell));

    cell->parent = table;
    cell->stream.parent = cell;
    cell->stream.partype = rid_pt_CELL;

    ASSERT(*cellp == NULL);
    *cellp = cell;

    cell->props = parse_table_props(&attributes->value[HTML_TD_VALIGN],
				    &attributes->value[HTML_TD_ALIGN],
				    &attributes->value[HTML_TD_CHAR],
				    &attributes->value[HTML_TD_CHAROFF],
				    &attributes->value[HTML_TD_LANG],
				    &attributes->value[HTML_TD_DIR],
				    &attributes->value[HTML_TD_STYLE],
				    &attributes->value[HTML_TD_BGCOLOR],
				    valign_table,
				    halign_table);

    /* Sketch out values */

    if (attributes->value[HTML_TD_BGCOLOR].type != value_none)
    {
	table->flags |= rid_tf_BGCOLOR;
    }

    if ( (attr = &attributes->value[HTML_TD_ID])->type == value_string)
	cell->id = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_TD_AXES])->type == value_string)
	cell->axes = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_TD_AXIS])->type == value_string)
	cell->axis = stringdup(attr->u.s);
    if ( (attr = &attributes->value[HTML_TD_CLASS])->type == value_string)
	cell->class = stringdup(attr->u.s);

    switch ( (attr = &attributes->value[HTML_TD_WIDTH])->type )
    {
    case value_absunit:
	TABDBG(("table cell size %g\n", attr->u.f));
	cell->flags |= rid_cf_ABSOLUTE;
	cell->userwidth = *attr;
	break;
#if 0
    case value_relunit:
	/* No specification requires this and no other implementation */
	/* does relative widths on cells and it's not easy, so don't */
	/* permit them - pretend they just never happened */
	cell->flags |= rid_cf_RELATIVE;
	cell->userwidth = *attr;
	break;
#endif
    case value_pcunit:
	if ( ceil(attr->u.f) > 99 )
	{
	    FMTDBG(("start_tdth: cell %%age is > 99, ignoring it entirely\n"));
	    cell->userwidth.type = value_none;
	}
	else if ( ceil(attr->u.f) > 0 )
	{
	    cell->flags |= rid_cf_PERCENT;
	    cell->userwidth = *attr;
	}
	break;

    default:
	if (value_none != 0)
	{
	    cell->userwidth.type = value_none;
	}
	break;
    }

    if ( (attr = &attributes->value[HTML_TD_HEIGHT])->type != value_none)
    {
	cell->userheight = *attr;
    }
    else if (value_none != 0)
    {
	cell->userheight.type = value_none;
    }

    cell->cell.x = table->scaff.x;
    cell->cell.y = table->scaff.y;

    /* Only take note of NOWRAP attribute when not overridden by an absolute width */
    {
	VALUE v;
	rid_getprop(table, cell->cell.x, cell->cell.y, rid_PROP_WIDTH, &v);
	if (v.type == value_none && attributes->value[HTML_TD_NOWRAP].type == value_void)
	    cell->flags |= rid_cf_NOWRAP;
    }

    if ( (attr = &attributes->value[HTML_TD_COLSPAN])->type == value_integer)
    {
	if ( (cell->span.x = attr->u.i) < 0 )
	    cell->span.x = 1;
	TABDBG(("start_tdth: user colspan %d\n", cell->span.x));
    }
    else
	cell->span.x = 1;

    if ( (attr = &attributes->value[HTML_TD_ROWSPAN])->type == value_integer)
    {
	if ( (cell->span.y = attr->u.i) < 0 )
	    cell->span.y = 1;
    }
    else
	cell->span.y = 1;

    /* Refine */
    if (cell->span.x == 0 && cell->span.y == 0)
    {
	TABDBG(("start_tdth: both spans are zero - ignoring\n"));
	cell->span.x = cell->span.y = 1;
    }

    if (element->id == HTML_TH)
	cell->flags |= rid_cf_HEADER;
#ifdef STBWEB
    /* SJM: need to ignore == 0 behaviour as no one else understands it */
    /* DAF: Bloody pathetic of the rest of the world! */
    if (cell->span.x <= 0)
	cell->span.x = 1;
    if (cell->span.y <= 0)
	cell->span.y = 1;
#else
    if (cell->span.x < 0)
	cell->span.x = 1;
    if (cell->span.y < 0)
	cell->span.y = 1;
#endif
    /* Note basic shaping information */
    if (cell->span.y == 0)
	cell->flags |= rid_cf_INF_VERT;
    if (cell->span.x == 0)
	cell->flags |= rid_cf_INF_HORIZ;
    if (cell->span.x != 1 || cell->span.y != 1)
	cell->flags |= rid_cf_MULTIPLE;
    if (cell->span.x == 1 && cell->span.y == 1)
    {       /* 1x1 cell - no more work */
	cell->flags |= rid_cf_COMPLETE;
    }

#if NEWGROW
    /* Record the size we want to be in swant and the size we actually
       are in span. This means we always present a valid span size,
       which the old approach did not. By having seperate x and y
       target sizes, the effects of truncatating growth in one
       direction on the other direction are greatly simplified. */
    cell->swant = cell->span;
    cell->span.x = 1;
    cell->span.y = 1;
#else
    /* SJM: added *span.x */
    /* DAF: If INF_HORIZ, *cell->span.x will always give zero! */
    cell->sleft = (cell->span.y - 1) * (cell->span.x == 0 ? 1 : cell->span.x);
#endif

    table->scaff.x += 1;		/* Starting looking after this */

    /* That's the new cell created at the right position */
    /* Do cell spreading: horizontal now, vertical row by row as table grows */
    /* Advance scaffolding and see if still have room. */

    /* Select new text stream */

    me->rh->curstream = &cell->stream;

    rid_getprop(table, cell->cell.x, cell->cell.y, rid_PROP_HALIGN, &tag);
    SET_EFFECTS(context->tos, STYLE_ALIGN, tag);
    /* SJM: added bold in headers */
    if (cell->flags & rid_cf_HEADER)
	add_bold_to_font(context);
    /* SJM: ensure no indent is carried into cell */
    SET_EFFECTS(context->tos, STYLE_INDENT, 0);
    SET_EFFECTS(context->tos, STYLE_RINDENT, 0);

    switch (tag)
    {
    case STYLE_ALIGN_LEFT:
	TABDBGN(("Choosing left aligned text for cell\n"));
	break;
    case STYLE_ALIGN_CENTER:
	TABDBGN(("Choosing centered text for cell\n"));
	break;
    case STYLE_ALIGN_RIGHT:
	TABDBGN(("Choosing right aligned text for cell\n"));
	break;
    case STYLE_ALIGN_CHAR:
	TABDBG(("Choosing character alignment for cell\n"));
	/*SET_EFFECTS(context->tos, STYLE_ALIGN, STYLE_ALIGN_LEFT);*/
	rid_getprop(table, cell->cell.x, cell->cell.y, rid_PROP_CH, &tag);
	SET_EFFECTS(context->tos, TABLE_ALIGN_CHAR, tag);
	break;
    default:
	TABDBG(("** CONFUSED HORIZONTAL CHARACTER ALIGNMENT %d **\n", tag));
	break;
    }

    rid_getprop(table, cell->cell.x, cell->cell.y, rid_PROP_VALIGN, &tag);
    SET_EFFECTS(context->tos, STYLE_VALIGN, tag);

    switch (tag)
    {
    case STYLE_VALIGN_TOP:
	TABDBG(("Choosing top aligned text for cell\n"));
	break;
    case STYLE_VALIGN_MIDDLE:
	TABDBG(("Choosing middle aligned text for cell\n"));
	break;
    case STYLE_VALIGN_BOTTOM:
	TABDBG(("Choosing bottom aligned text for cell\n"));
	break;
    case STYLE_VALIGN_BASELINE:
	TABDBG(("Choosing baseline aligned text for cell\n"));
	break;
    default:
	TABDBG(("** CONFUSED VERTICAL CHARACTER ALIGNMENT %d **\n", tag));
	break;
    }

    /* Common work done - can start returning now */

    if (cell->flags & rid_cf_COMPLETE)
	goto done;

    /* Need to ensure span.y of INF_VERT is greater than zero */

    if (cell->span.y == 0)
	cell->span.y = 1;

    if (cell->flags & rid_cf_COMPLETE)
	goto done;

#if NEWGROW
    if (cell->swant.x == 1)
    {       /* Only vertical growth - done in add_row() */
	goto done;
    }
#else
    if (cell->span.x == 1)
    {       /* Only vertical growth - done in add_row() */
	goto done;
    }
#endif

    /* Something about the cell's shape requires extra work */

    if ( (table->flags & rid_tf_COLS_FIXED) != 0 )
    {       /* Constrained in number of columns */
	TABDBG(("More work needed, fixed columns\n"));
	if ( (cell->flags & rid_cf_INF_HORIZ) != 0)
	{
	    TABDBG(("Extend to end of row, then we're full %d...%d\n",
		    cell->cell.x, table->cells.x));
	    for (x = cell->cell.x + 1; x < table->cells.x; x++)
	    {       /* Stop early if hit cell coming down from above */
		cellp = CELLFOR(table, x, table->scaff.y);
		if (*cellp != NULL)
		{
		    TABDBG(("Cell in column %d means extended far enough\n", x));
		    break;
		}
		*cellp = cell;
	    }
	    /* DAF: 970321 */
	    cell->span.x = table->cells.x - cell->cell.x;
	    TABDBG(("colspan=0 case with cols=%d, set spanx to %d for %d,%d\n",
		    table->cells.x, cell->span.x, cell->cell.x, cell->cell.y));
	}
	else
	{
	    /* Try to fit */
#if NEWGROW
	    if (cell->cell.x + cell->swant.x > table->cells.x)
	    {       /* Filled up. Reduce span. */
		/*table->flags |= rid_tf_NO_MORE_CELLS;*/
		TABDBG(("cell %d,%d wanted span.x %d, but table is %d,%d, so limited to %d\n",
			cell->cell.x, cell->cell.y,
			cell->swant.x/* , cell->swant.y */,
			table->cells.x, table->cells.y,
			table->cells.x - cell->cell.x));
		cell->span.x = table->cells.x - cell->cell.x;
	    }
#else
	    if (cell->cell.x + cell->span.x > table->cells.x)
	    {       /* Filled up. Reduce span. */
		/*table->flags |= rid_tf_NO_MORE_CELLS;*/
		TABDBG(("cell %d,%d wanted span.x %d, but table is %d,%d, so limited to %d\n",
			cell->cell.x, cell->cell.y,
			cell->span.x, cell->span.y,
			table->cells.x, table->cells.y,
			table->cells.x - cell->cell.x));
		cell->span.x = table->cells.x - cell->cell.x;
	    }
#endif
/* 	    for (x = cell->cell.x + 1; x < table->cells.x + cell->span.x; x++) */
            /* pdh: would look better to me as this: */
	    /* DAF: good point! */
#if NEWGROW
	    for (x = cell->cell.x + 1; x < cell->cell.x + cell->swant.x; x++)
#else /* Sorry! */
	    for (x = cell->cell.x + 1; x < cell->cell.x + cell->span.x; x++)
#endif
	    {       /* Replicate cell pointer */
		TABDBG(("Repl cell %d,%d into %d,%d? ... ",
			cell->cell.x, cell->cell.y, x, table->scaff.y));
		cellp = CELLFOR(table, x, table->scaff.y);
		if (*cellp != NULL)
		{
		    TABDBG(("no, stopping at x=%d\n", x));
		    break;
		}
		TABDBG(("yes, doing x=%d\n", x));
		*cellp = cell;
	    }
	    cell->span.x = x - cell->cell.x;
	    TABDBG(("Eventually cell %d,%d has span.x %d\n", cell->cell.x, cell->cell.y, cell->span.x));
	    ASSERT(cell->span.x > 0);
	}

	goto done;
    }

    if ( (cell->span.x & rid_cf_INF_HORIZ) != 0)
    {
#if NEWGROW
	TABDBG(("Cell (%d,%d) (span %d,%d, swant %d,%d) extends right as far as possible\n",
		cell->cell.x, cell->cell.y, cell->span.x, cell->span.y, cell->swant.x, cell->swant.y));
#else
	TABDBG(("Cell (%d,%d) (span %d,%d, sleft %d) extends right as far as possible\n",
		cell->cell.x, cell->cell.y, cell->span.x, cell->span.y, cell->sleft));
#endif

	for (x = cell->cell.x + 1; x < table->cells.x; x++)
	{
	    cellp = CELLFOR(table,x,table->scaff.y);

	    if ( *cellp == NULL )
	    {
		*cellp = cell;
		TABDBG(("Bump span from %d\n", cell->span.x));
		cell->span.x++;	/* DAF: 970315 */
	    }
	    else
		break;
	}

#if NEWGROW
	TABDBG(("INF_HORIZ cell (%d,%d) now has span (%d,%d), swant %d,%d\n",
		cell->cell.x, cell->cell.y, cell->span.x, cell->span.y, cell->swant.x, cell->swant.y));
#else
	cell->span.x++;

	/* DAF: 970315: Can update this value a bit now */
	cell->sleft = (cell->span.y - 1) * cell->span.x;

	TABDBG(("INF_HORIZ cell (%d,%d) now has span (%d,%d), sleft %d\n",
		cell->cell.x, cell->cell.y, cell->span.x, cell->span.y, cell->sleft));
#endif
	goto done;
    }

    /* Grow col(s) to fit cell, if possible */
    /* Have cell->span.x > 1 and not fixed columns. */


#if NEWGROW
    TABDBGN(("Growing to meet span of %d\n", cell->swant.x));
    for (x = 1; x < cell->swant.x; x++)
#else
    TABDBGN(("Growing to meet span of %d\n", cell->span.x));
    for (x = 1; x < cell->span.x; x++)
#endif
    {       /* Add another column if we need it */
	if (cell->cell.x + x >= table->cells.x)
	{
	    add_retro_col(table);
	    if (table->state == tabstate_BAD)
		goto done;
	}
#if NEWGROW
	cell->span.x++;
#endif
	/* See if would overlap with a previous cell */
	cellp = CELLFOR(table, cell->cell.x + x, table->scaff.y);

	if (*cellp != NULL)
	{
#if NEWGROW
	    /* Would overlap - constrict current cell */
	    cell->span.x = x;
#else
	    if ( gbf_active(GBF_NETSCAPE_OVERLAPS) )
	    {
		/* SJM: netscape overlaps in the other order so try stomping over cell to see if it works */
		/* DAF: wasn't good enough - consider
		   <TABLE> <TR> <TD> <TD rowspan=2 colspan=2>
		   <TR> <TD colspan=2> </TABLE> */
		rid_table_cell *oldcell = *cellp;
		{ /* DAF: 970424: */ static int THIS_DOES_NOT_WORK_SO_DO_NOT_USE_UNLESS_PYSCHOTIC; }
		if ( (oldcell->flags & rid_cf_INF_VERT) != 0 )
		    oldcell->span.y--;
		else
		{
		    oldcell->span.y -= oldcell->sleft+1;
		    oldcell->sleft = 0;
		}
		oldcell->flags |= rid_cf_COMPLETE;

		/* DAF: Clear out ALL preceeding traces of the ealier
		   growth. To see why, work through this example:  */
		/* DAF: Mark ourselves for this cell */
		*cellp = cell;
	    }
	    else
	    {
		/* Would overlap - constrict current cell */
		cell->span.x = x;       /* ? */
	    }
#endif

	    goto done;
	}

	/* Grow the cell horizontally into the neighbouring cell */
	*cellp = cell;
    }

#if NEWGROW
    if (cell->swant.y == 1)
	cell->flags |= rid_cf_COMPLETE;
#else
    if (cell->span.y == 1)
	cell->flags |= rid_cf_COMPLETE;
#endif


done:
#if DEBUG && 0
    TABDBGN(("\n"));
    dump_cell_map(table, "Cell map after adding new cell");
#endif

    return;
}


extern void starttd(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    start_tdth(context, element, attributes);
}

extern void startth(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    start_tdth(context, element, attributes);
}

/*****************************************************************************

  We note whether a cell has any background colour property that it
  can see, so that rendering knows on a cell by cell basis whether to
  apply background colours and bother doing the rid_getprop() call at
  all.

  NEW_UNEXP_TABLE: Leave custream as is, so between cells we have a
  defined place to put unexpected text.

 */

static void finish_thtd (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *me = htmlctxof(context);
    rid_table_cell *cell;
    int i;

    generic_finish (context, element);

    if ( !config_display_tables )
    {
        TABDBG(("finish_tdth: ignoring\n"));
        return;
    }

    /* pdh: this is a bodge */ me->aref = NULL;

/*    TASSERT(me->partype == rid_pt_CELL);*/
    cell = (rid_table_cell *)(me->rh->curstream->parent);

    rid_getprop(cell->parent, cell->cell.x, cell->cell.y, rid_PROP_BGCOLOR, &i);

    if (i != NO_BGCOLOR)
    {
	cell->flags |= rid_cf_BACKGROUND;
	cell->stream.bgcolour = i | 1;
    }

/*      sgml_install_deliver(context, &table_deliver); */

    if (gbf_active(GBF_TABLES_UNEXPECTED))
    {
	me->rh->curstream = cell->parent->oldstream;
	ASSERT(me->rh->curstream != NULL);
    }
}

extern void finishth (SGMLCTX * context, ELEMENT * element)
{
    finish_thtd(context, element);
}

extern void finishtd (SGMLCTX * context, ELEMENT * element)
{
    finish_thtd(context, element);
}

/*****************************************************************************/

/* Sizing of tables - maybe no pos list */

/*****************************************************************************

  Called after the size method of each object has been invoked and before
  the outermost be_formater_loop().  This will recursively descend the text
  stream tree, calculating minimum and maximum width information.  All of
  this information must be collected prior to formatting.

  For the minimum width, we require a smallest value that will not cause
  overflow.  This hsa to tie up with the way formatting is actually done.

  */


#if 0
/*static*/ extern void rid_size_table( rid_header *rh, rid_table_item *table, rid_fmt_info *parfmt );

static void dummy_table_min(rid_header *rh, rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt)
{
    rid_table_item *table = ((rid_text_item_table *)item)->table;

    item->width = table->width_info.minwidth;
    return;
}

static void dummy_table_max(rid_header *rh, rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt)
{
    rid_table_item *table = ((rid_text_item_table *)item)->table;

    item->width = table->width_info.maxwidth;
    return;
}

extern void rid_size_stream(rid_header *rh, rid_text_stream *stream, rid_fmt_info *fmt, int flags, rid_text_item *ti)
{
    rid_width_info *info = &stream->width_info;
    void (*old_tab_proc)(rid_header *rh, rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt);

    if (ti == NULL)
	ti = stream->text_list;

    TABDBG(("rid_size_stream(%p, %p, %x, %p)\n", stream, fmt, flags, ti));

    /* Do this seperately or we do twice the work? */

    for (; ti != NULL; ti = rid_scanf(ti))
    {
	if (ti->tag == rid_tag_TABLE)
	{
	    rid_size_table( rh, ((rid_text_item_table *)ti)->table, fmt );
	}
    }

    if ( (flags & rid_fmt_MAX_WIDTH) == 0 )
    {
	fmt->left  = &info->minleft;
	fmt->right = &info->minright;
	fmt->width = &info->minwidth;
	old_tab_proc = fmt->table_proc;
	fmt->table_proc = dummy_table_min;
	be_formater_loop_core(rh, stream, stream->text_list, fmt, flags | rid_fmt_MIN_WIDTH);
	fmt->table_proc = old_tab_proc;
#if DEBUG && 0
	TABDBG(("rid_size_table(): formatted to MIN_WIDTH\n"));
	dump_width_info(*info);
#endif
    }

    if ( (flags & rid_fmt_MIN_WIDTH) == 0 )
    {
	fmt->left  = &info->maxleft;
	fmt->right = &info->maxright;
	fmt->width = &info->maxwidth;
	old_tab_proc = fmt->table_proc;
	fmt->table_proc = dummy_table_max;
	be_formater_loop_core(rh, stream, stream->text_list, fmt, flags | rid_fmt_MAX_WIDTH);
	fmt->table_proc = old_tab_proc;
#if DEBUG && 0
	TABDBG(("rid_size_table(): formatted to MAX_WIDTH\n"));
	dump_width_info(*info);
#endif
    }

    if ( (flags & rid_fmt_MIN_WIDTH) != 0 )
    {
	info->maxleft  = info->minleft;
	info->maxright = info->minright;
	info->maxwidth = info->minwidth;
#if DEBUG && 0
	TABDBG(("rid_size_table(): setting for MIN_WIDTH\n"));
	dump_width_info(*info);
#endif
    }

    if ( (flags & rid_fmt_MAX_WIDTH) != 0 )
    {
	info->minleft  = info->maxleft;
	info->minright = info->maxright;
	info->minwidth = info->maxwidth;
#if DEBUG && 0
	TABDBG(("rid_size_table(): setting for MAX_WIDTH\n"));
	dump_width_info(*info);
#endif
    }

#if DEBUG && 0
    TABDBG(("rid_size_stream(): done with\n"));
    dump_width_info(*info);
#endif
}

#endif

/* eof commonsrc/tables.c */




