/* -*-C-*- dump.c - internal object debugging dumper */

/*
 * A "char *base" parameter has been added to most functions. This
 * is the base of the text from the memzone, and is to permit text
 * items to be printed. This is though sufficiently useful.
 */

#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

#include "memwatch.h"

#include "wimp.h"
#include "consts.h"
#include "sgmlparser.h"
#include "htmlparser.h"         /* Now includes bits that were local */

#include "rid.h"
#include "charsets.h"
#include "util.h"
#include "webfonts.h"
#include "url.h"

#include "filetypes.h"
#include "parsers.h"
#include "images.h"

#include "tables.h"
#include "unwind.h"



/*****************************************************************************/

/* DON'T want this going out in production code (again!) */

#if DEBUG || defined(BUILDERS)

#ifdef PRODUCTION
#error "CONTROL FLAGS SERIOUSLY CONFUSED"
#endif

static int indent = 0;
static int at_start = 0;

static void newline(void)
{
        fprintf(DBGOUT, "\n");
        at_start = 1;
}

static void enter(void)
{
        indent++;
        if (!at_start) newline ();
}

static void leave(void)
{
        indent--;
        if (!at_start) newline ();
}

static void my_print(char *format, ... )
{
	va_list(arg);
	va_start(arg, format);

        if (at_start)
        {
                int i;
                at_start = 0;
                for (i = 0; i < indent; i++)
                        fprintf(DBGOUT, "|  ");
        }
	vfprintf(DBGOUT, format, arg);
        va_end(arg);
        newline();
	fflush(DBGOUT);
}

static void nonlprint(char *format, ... )
{
	va_list(arg);
	va_start(arg, format);

        if (at_start)
        {
                int i;
                at_start = 0;
                for (i = 0; i < indent; i++)
                        fprintf(DBGOUT, "|  ");
        }
	vfprintf(DBGOUT, format, arg);
        va_end(arg);
	fflush(DBGOUT);
}

/*****************************************************************************/

static char *smartpos(rid_pos_item *pi)
{
    static char buf[32];

#if 0
    sprintf(buf, "%p", pi);
#else

    if (pi == NULL)
    {
	strcpy(buf, "No pos item");
    }
    else
    {
#if DEBUG
	sprintf(buf, "line %d", pi->linenum);
#else
	strcpy(buf, "No pos item");
#endif

#endif
    }

    return buf;
}

/*****************************************************************************/

#ifndef CHECKER
#define FIELD(ptr, field, fmt)  my_print( #field " " fmt, (ptr)->field )
#else
#define FIELD(ptr, field, fmt)  nonlprint( fmt " ", (ptr)->field )
#endif

#define X_OR_NULL(x)            (x ? x : "NULL")

/*****************************************************************************/




extern void dump_span(int *base, int span)
{
  	int x;
#ifndef NO_PTRS
 	my_print("Base %p, span %d", base, span);
#else
 	my_print("Span %d", base);
#endif

 	for (x = 0; x < span; x++)
 		nonlprint("%4d ", base[x]);

	my_print("");
}









extern void dump_style(ROStyle style)
{
    static char *align_names[8] = 
    {
	"", /*"unknown_align", but too common */
	"left",
	"center",
	"right",
	"char",
	"justify",
	"hspare6",
	"hspare7"
    };

    static char *valign_names[8] = 
    {
	"", /*"unknown_valign", but too common */
	"top",
	"middle",
	"bottom",
	"baseline",
	"texttop",
	"absbottom",
	"absmiddle"
    };

    ROStyle *ptr = &style;
    BITS *bp = (BITS *)ptr;
    int x;

    my_print("ROStyle %08x", UNPACK(bp, ROSTYLE) );

    enter();
        if ( UNPACK(bp, ROSTYLE) & 0xffff )
	{
	    nonlprint("Flags:");
	    if ( UNPACK(bp, STYLE_UNDERLINE) )
		nonlprint(" underlined");
	    nonlprint(" %s",  align_names[ UNPACK(bp, STYLE_ALIGN) ] );
	    nonlprint(" %s", valign_names[ UNPACK(bp, STYLE_ALIGN) ] );
	    if ( UNPACK(bp, STYLE_SUB) )
		nonlprint(" sub");
	    if ( UNPACK(bp, STYLE_SUP) )
		nonlprint(" sup");
	    if ( UNPACK(bp, STYLE_RESERVED) )
		nonlprint(" RESERVED");
	    if ( UNPACK(bp, STYLE_STRIKE) )
		nonlprint(" strike");
	    if ( UNPACK(bp, STYLE_COLOURNO) )
		nonlprint(" colour=%d", UNPACK(bp, STYLE_COLOURNO) );
	    my_print("");
	}

	x = UNPACK(bp, STYLE_WF_INDEX);
	if (x != 2)		/* 2 being the common case */
	{
	    nonlprint("Font %d:", x );
	    if (x & WEBFONT_FLAG_SPECIAL)
	    {
		int z = x & WEBFONT_SPECIAL_TYPE_MASK;
		nonlprint(" %d", z);
		if (z == WEBFONT_SPECIAL_TYPE_MENU)
		    nonlprint(" menu");
		else
		    nonlprint(" symbol");
	    }
	    else
	    {
		if (x & WEBFONT_FLAG_BOLD)
		    nonlprint(" bold");
		if (x & WEBFONT_FLAG_ITALIC)
		    nonlprint(" italic");
		if (x & WEBFONT_FLAG_FIXED)
		    nonlprint(" fixed");
		if (WEBFONT_SIZEOF(x) != 3)
		    nonlprint(" size=%d", WEBFONT_SIZEOF(x) );
	    }
	    my_print("");
	}

	if ( UNPACK(bp, STYLE_INDENT) )
	    my_print("Indent %d", UNPACK(bp, STYLE_INDENT) );
    leave();
}

extern void dump_float_item(rid_float_item *ptr)
{
#ifndef NO_PTRS
    my_print("rid_float_item %p", ptr);
    enter();
    FIELD(ptr, ti, "%p");
    my_print("pi %s", smartpos(ptr->pi));
    FIELD(ptr, height, "%d");
    FIELD(ptr, height_left, "%d");
    FIELD(ptr, entry_margin, "%d");
    leave();
#else
    my_print("rid_float_item");
#endif
}

extern void dump_float_items(rid_float_item *ptr)
{
    while (ptr != NULL)
    {
	dump_float_item(ptr);
	ptr = ptr->next;
    }
}

extern void dump_floats_link(rid_floats_link *ptr)
{
#ifndef NO_PTRS
    my_print("rid_floats_link %p", ptr);
#else
    my_print("rid_floats_link");
#endif
    enter();
    FIELD(ptr, right_margin, "%d");
#ifndef NO_PTRS
    FIELD(ptr, left, "%p");
    FIELD(ptr, right, "%p");
#endif
    if (ptr->left)
    {
	my_print("left links");
	dump_float_items(ptr->left);
    }

    if (ptr->right)
    {
	my_print("right links");
	dump_float_items(ptr->right);
    }

    leave();
}

extern void dump_pos(rid_pos_item *pi)
{
#ifndef NO_PTRS
        my_print("rid_pos_item %s", smartpos(pi));
#else
        my_print("rid_pos_item");
#endif
        if (pi == NULL)
        	return;
        enter();
#ifndef NO_PTRS
		my_print("prev %s", smartpos(pi->prev));
		my_print("next %s", smartpos(pi->next));
                FIELD(pi, st, "%p");
#endif
                FIELD(pi, top, "%d");
                FIELD(pi, left_margin, "%d");
                FIELD(pi, max_up, "%d");
                FIELD(pi, max_down, "%d");

		switch (pi->leading)
		{
		case MAGIC_LEADING_PENDING:
		    my_print("Magic leading pending");
		    break;
		case -1:
		    my_print("Leading - generic MAGIC -1");
		    break;
		case 0:
		    break;
		default:
		    FIELD(pi, leading, "%d");
		    break;
		}

#ifndef NO_PTRS
                FIELD(pi, first, "%p");
		FIELD(pi, floats, "%p");
#endif
		if (pi->floats)
		    dump_floats_link(pi->floats);
        leave();
}

extern void dump_stdunits(rid_stdunits v)
{
    switch (v.type)
    {
    case value_absunit:
	my_print("stdunit %d", (int) ceil(v.u.f) / 2 );
	break;
    case value_relunit:
	my_print("stdunit %g*", v.u.f );
	break;
    case value_pcunit:
	my_print("stdunit %g%%", v.u.f );
	break;
    default:
	my_print("stdunit <%d>", v.type);
	break;
    }
}

char *item_names[] =
{
    "PBREAK",
    "HLINE",
    "TEXT",
    "BULLET",
    "IMAGE",
    "INPUT",
    "TEXTAREA",
    "SELECT",
    "TABLE",
    "OBJECT",
    "SCAFF"
};

extern void dump_item(rid_text_item *item, char *base)
{
    static char *flag_names[8] =
    {
	"rid_flag_LINE_BREAK     ",
	"rid_flag_NO_BREAK       ",
	"rid_flag_SELECTED       ",
	"rid_flag_LEFTWARDS      ",

	"rid_flag_RIGHTWARDS     ",
	"rid_flag_CLEARING	 ",
	"rid_flag_ACTIVATED	 ",
	"rid_flag_EXPLICIT_BREAK "
    };

    int i;
#ifndef NO_PTRS
        my_print("rid_text_item %p", item);
#else
        my_print("rid_text_item");
#endif
        if (item == NULL)
        	return;
        enter();
#ifndef NO_PTRS
	        if (item->next) FIELD(item, next, "%p");
                if (item->line) my_print("line %s", smartpos(item->line));
		if (item->aref) FIELD(item, aref, "%p");
#endif
                if (item->max_up) FIELD(item, max_up, "%d");
                if (item->max_down) FIELD(item, max_down, "%d");
                if (item->width) FIELD(item, width, "%d");
                if (item->pad) FIELD(item, pad, "%d");
                /*if (item->flag) FIELD(item, flag, "%x");*/
		enter();
		for (i = 0; i < 8; i++)
		    if (item->flag & (1<<i) )
			my_print("%s", flag_names[i]);
		leave();
                my_print("tag %s", item_names[item->tag]);
#if defined(CHECKER) || 1
                dump_style(item->st);
#endif
		switch (item->tag)
		{
		case rid_tag_TABLE:
		    dump_table( ((rid_text_item_table*)item)->table, base);
		    break;
		case rid_tag_TEXT:
		    if (base) my_print("\"%s\"", base + ((rid_text_item_text*)item)->data_off);
		    break;
		}

        leave();
}


extern void dump_aref(rid_aref_item *ptr)
{
#ifndef NO_PTRS
        my_print("rid_aref_item %p", ptr);
#else
        my_print("rid_aref_item");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, prev, "%p");
                FIELD(ptr, next, "%p");
#endif
                my_print("name %s", X_OR_NULL(ptr->name));
                my_print("href %s", X_OR_NULL(ptr->href));
                my_print("rel %s", X_OR_NULL(ptr->rel));
#ifndef NO_PTRS
                FIELD(ptr, first, "%p");
#endif
        leave();
}





extern void dump_input(rid_input_item *ptr)
{
#ifndef NO_PTRS
        my_print("rid_input_item %p", ptr);
#else
        my_print("rid_input_item");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, base.prev, "%p");
                FIELD(ptr, base.next, "%p");
                FIELD(ptr, base.parent, "%p");
                FIELD(ptr, base.display, "%p");
#endif
                /*my_print("tag %s", str_from_tag(ptr->tag, strtag_input_tag));*/
                FIELD(ptr, flags, "%08x");
                my_print("name %s", X_OR_NULL(ptr->name));
                my_print("value %s", X_OR_NULL(ptr->value));
                FIELD(ptr, xsize, "%d");
/*              FIELD(ptr, ysize, "%d"); */
                my_print("src %s", X_OR_NULL(ptr->src));
                FIELD(ptr, max_len, "%d");
                my_print("data ... ");
        leave();
}



extern void dump_option(rid_option_item *ptr)
{
#ifndef NO_PTRS
        my_print("rid_option_item %p", ptr);
#else
        my_print("rid_option_item");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, prev, "%p");
                FIELD(ptr, next, "%p");
#endif
                my_print("text %s", X_OR_NULL(ptr->text));
                my_print("value %s", X_OR_NULL(ptr->value));
                FIELD(ptr, flags, "%08x");
        leave();
}



extern void dump_select( rid_select_item *ptr)
{
#ifndef NO_PTRS
        my_print("rid_select_item %p", ptr);
#else
        my_print("rid_select_item");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, base.prev, "%p");
                FIELD(ptr, base.next, "%p");
                FIELD(ptr, base.parent, "%p");
                FIELD(ptr, base.display, "%p");
#endif
                my_print("name %s", X_OR_NULL(ptr->name));
                FIELD(ptr, count, "%d");
                FIELD(ptr, size, "%d");
#ifndef NO_PTRS
                FIELD(ptr, items, "%p");
                FIELD(ptr, menuh, "%p");
                FIELD(ptr, doc, "%p");
                FIELD(ptr, options, "%p");
                FIELD(ptr, last_option, "%p");
#endif
                FIELD(ptr, flags, "%08x");
        leave();
}


extern void dump_textarea_line( rid_textarea_line *ptr)
{
#ifndef NO_PTRS
        my_print("rid_textarea_line %p", ptr);
#else
        my_print("rid_textarea_line");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, prev, "%p");
                FIELD(ptr, next, "%p");
#endif
                my_print("text %s", X_OR_NULL(ptr->text));
        leave();
}

extern void dump_textarea(rid_textarea_item *ptr)
{
#ifndef NO_PTRS
        my_print("rid_textarea_item %p", ptr);
#else
        my_print("rid_textarea_item");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, base.prev, "%p");
                FIELD(ptr, base.next, "%p");
                FIELD(ptr, base.parent, "%p");
                FIELD(ptr, base.display, "%p");
#endif
                my_print("name %s", X_OR_NULL(ptr->name));
                FIELD(ptr, rows, "%d");
                FIELD(ptr, cols, "%d");
                FIELD(ptr, cx, "%d");
                FIELD(ptr, cy, "%d");
                FIELD(ptr, sx, "%d");
                FIELD(ptr, sy, "%d");
#if !NEW_TEXTAREA
                FIELD(ptr, default_lines, "%d");
#ifndef NO_PTRS
                FIELD(ptr, def_last_line, "%p");
                FIELD(ptr, lines, "%p");
                FIELD(ptr, last_line, "%p");
                FIELD(ptr, caret_line, "%p");
#endif
#endif
        leave();
}



extern void dump_form( rid_form_item *ptr)
{
#ifndef NO_PTRS
        my_print("rid_form_item %p", ptr);
#else
        my_print("rid_form_item");
#endif
        if (ptr == NULL)
        	return;
        enter();
#ifndef NO_PTRS
                FIELD(ptr, prev, "%p");
                FIELD(ptr, next, "%p");
                FIELD(ptr, kids, "%p");
                FIELD(ptr, last_kid, "%p");
#if 0
                FIELD(ptr, texts, "%p");
                FIELD(ptr, last_text, "%p");
                FIELD(ptr, selects, "%p");
                FIELD(ptr, last_select, "%p");
#endif
                FIELD(ptr, method, "%p");
#endif
/*                my_print("method %s", str_from_tag(ptr->method, strtag_input_method));*/
		FIELD(ptr, method, "%d");
                my_print("action %s", X_OR_NULL(ptr->action));
        leave();
}



extern void dump_caption(rid_table_caption *ptr, char *base)
{
#ifndef NO_PTRS
 	my_print("rid_table_caption %p", ptr);
#else
 	my_print("rid_table_caption");
#endif
        if (ptr == NULL)
        	return;
 	enter();
		my_print("Size: %d,%d", ptr->size.x, ptr->size.y);
		my_print("Off: %d,%d", ptr->off.x, ptr->off.y);
#ifndef NO_PTRS
 		FIELD(ptr, table, "%p");
 		/*my_print("calign %s", str_from_tag(ptr->calign, strtag_calign));*/
        	/*FIELD(ptr, id, "%p");*/
        	/*FIELD(ptr, class, "%p");*/
        	/*FIELD(ptr, props, "%p");*/
#endif
 		dump_stream(&ptr->stream, base);
	leave();
}

extern void dump_cell_map(rid_table_item *ptr, char *msg)
{
    int x,y;
    rid_table_cell *cell;

    my_print("Cell occupancy: %s", msg);

    for (y = 0; y < ptr->cells.y; y++)
    {
	for (x = 0; x < ptr->cells.x; x++)
	{
	    cell = * CELLFOR(ptr, x, y);
	    if (cell == NULL)
	    {
		nonlprint(".");
	    }
	    else if (x == cell->cell.x && y == cell->cell.y)
	    {
		nonlprint("#");
	    }
	    else
	    {
		nonlprint("-");
	    }
	}
	my_print("");
    }
}

extern void dump_table(rid_table_item *ptr, char *base)
{
    typedef struct { char *name; BITS bit; } sb;

    static sb rid_tf[] =
    {
	{ "rid_tf_COLS_FIXED       ", rid_tf_COLS_FIXED },
	{ "rid_tf_LINE_START       ", rid_tf_LINE_START },
	{ "rid_tf_NO_MORE_CELLS    ", rid_tf_NO_MORE_CELLS },
	{ "rid_tf_DONE_COLGROUP    ", rid_tf_DONE_COLGROUP },
	{ "rid_tf_IN_COLGROUP      ", rid_tf_IN_COLGROUP },
	{ "rid_tf_FULL_BORDER      ", rid_tf_FULL_BORDER },
	{ "rid_tf_GROUP_SPAN       ", rid_tf_GROUP_SPAN },
	{ "rid_tf_IMPLIED_COLGROUP ", rid_tf_IMPLIED_COLGROUP },
	{ "rid_tf_3D_BORDERS       ", rid_tf_3D_BORDERS },
	{ "rid_tf_COLGROUPSECTION  ", rid_tf_COLGROUPSECTION },
	{ "rid_tf_HAVE_REL         ", rid_tf_HAVE_REL },
        { "rid_tf_HAVE_TFOOT       ", rid_tf_HAVE_TFOOT },
        { "rid_tf_TFOOT_INVISIBLE  ", rid_tf_TFOOT_INVISIBLE },
	{ NULL, 0 }
    };
    sb *tfp = rid_tf;

  	rid_table_cell *cell;
	int x,y;

#ifndef NO_PTRS
        my_print("rid_table_item id=%d %p", ptr->idnum, ptr);
#else
        my_print("rid_table_item id=%d", ptr->idnum);
#endif
        if (ptr == NULL)
        	return;
        enter();
#if 0
        	FIELD(ptr, parent, "%p");
        	FIELD(ptr, oldstream, "%p");
        	FIELD(ptr, flags, "%08x");
#endif

		enter();
		for (; tfp->name != NULL; tfp++)
		{
		    if ( (ptr->flags & tfp->bit) != 0 )
			my_print("%s", tfp->name);
		}
		leave();
#if 1
        	my_print("userborder");
        	dump_stdunits(ptr->userborder);
        	/*my_print("frame %s", str_from_tag(ptr->frame, strtag_frame));*/
		  /*my_print("rules %s", str_from_tag(ptr->rules, strtag_rules));*/
        	my_print("userwidth");
        	dump_stdunits(ptr->userwidth);
        	my_print("cellspacing");
        	dump_stdunits(ptr->usercellspacing);
        	my_print("cellpadding");
        	dump_stdunits(ptr->usercellpadding);
#endif
		FIELD(ptr, frame, "%d");
		FIELD(ptr, rules, "%d");
        	FIELD(ptr, border, "%d");
        	FIELD(ptr, cellspacing, "%d");
        	FIELD(ptr, cellpadding, "%d");
        	FIELD(ptr, header_rows, "%d");
        	FIELD(ptr, footer_rows, "%d");
        	my_print("num_groups %d %d", ptr->num_groups.x, ptr->num_groups.y);
#ifndef NO_PTRS
#if 1
        	FIELD(ptr, array, "%p");
        	FIELD(ptr, rowhdrs, "%p");
        	FIELD(ptr, colhdrs, "%p");
        	FIELD(ptr, rowgroups, "%p");
        	FIELD(ptr, colgroups, "%p");
#endif
#endif
        	my_print("cells %d %d", ptr->cells.x, ptr->cells.y);
        	my_print("size %d %d", ptr->size.x, ptr->size.y);
/*        	my_print("width_info");*/
/*        	dump_width_info(ptr->width_info);*/
/*        	FIELD(ptr, hwidths, "%p");*/
/*        	FIELD(ptr, vheights, "%p"); */
        	FIELD(ptr, state, "%d");
        	my_print("scaff %d %d", ptr->scaff.x, ptr->scaff.y);
                my_print("Column groups and headers");
                for (x = 0; x < ptr->num_groups.x; x++)
                        dump_colgroup(ptr->colgroups[x]);
                for (x = 0; x < ptr->cells.x; x++)
                        dump_colhdr(ptr->colhdrs[x]);

                my_print("Row groups and headers");
                for (y = 0; y < ptr->num_groups.y; y++)
                        dump_rowgroup(ptr->rowgroups[y]);
                for (y = 0; y < ptr->cells.y; y++)
                        dump_rowhdr(ptr->rowhdrs[y]);

                my_print("Text streams");
		dump_caption(ptr->caption, base);
		for (y = 0; y < ptr->cells.y; y++)
		{
			for (x = 0; x < ptr->cells.x; x++)
			{
			 	cell = * CELLFOR(ptr, x, y);
			 	if (cell == NULL)
			 	{
			 		my_print("Cell %d, %d: NULL", x, y);
			 	}
			 	else if (x == cell->cell.x && y == cell->cell.y)
			 	{
			 		my_print("Cell %d, %d: root item", x, y);
			 	 	dump_cell(cell, base);
			 	}
			 	else
			 	{
			 		my_print("Cell %d, %d: covered by cell %d,%d", x, y, cell->cell.x, cell->cell.y);
			 	}
			}
                        my_print("End of table row %d", y);
		}

		dump_cell_map(ptr, "");

        leave();
}




extern void dump_props(rid_table_props *ptr)
{
#ifndef NO_PTRS
 	my_print("rid_table_props %p", ptr);
#else
 	my_print("rid_table_props");
#endif
        if (ptr == NULL)
        	return;
return;		/* @@@@ */
 	enter();
	/*my_print("valign %s", str_from_tag(ptr->valign, strtag_valign));*/
 		/*my_print("halign %s", str_from_tag(ptr->halign, strtag_halign));*/
	/*my_print("dir %s", str_from_tag(ptr->dir, strtag_dir));*/
 		/*FIELD(ptr, ch, "%02x");*/
 		/*my_print("choff");*/
 		dump_stdunits(ptr->choff);
		/*FIELD(ptr, lang, "%p");*/
		/*FIELD(ptr, style, "%p");*/
	leave();
}

/*****************************************************************************/

extern void dump_width_info(rid_width_info info)
{
  	rid_width_info *ptr = &info;
  	/*my_print("width_info");*/
  	enter();
/*		FIELD(ptr, minleft, "%d");
  		FIELD(ptr, minright, "%d");*/
  		FIELD(ptr, minwidth, "%d");
/*  		FIELD(ptr, maxleft, "%d");
  		FIELD(ptr, maxright, "%d");*/
  		FIELD(ptr, maxwidth, "%d");
	leave();
}


extern void dump_colgroup(rid_table_colgroup *ptr)
{
#ifndef NO_PTRS
  	my_print("rid_table_colgroup %p", ptr);
#else
  	my_print("rid_table_colgroup");
#endif
        if (ptr == NULL)
        	return;
  	enter();
#ifndef NO_PTRS
  		FIELD(ptr, props, "%p");
#endif
  		FIELD(ptr, first, "%d");
  		FIELD(ptr, span, "%d");
/* 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
		*/
 		my_print("width");
 		dump_stdunits(ptr->userwidth);
	leave();
}

extern void dump_rowgroup(rid_table_rowgroup *ptr)
{
    static struct { char *name; rid_rowhdr_flags flag; }
    flags [] =
    {
	{ "rid_rgf_THEAD", rid_rgf_THEAD },
	{ "rid_rgf_TFOOT", rid_rgf_TFOOT },
	{ "rid_rgf_TBODY", rid_rgf_TBODY },
	{ NULL, 0 }
    };
    int i;

#ifndef NO_PTRS
  	my_print("rid_table_rowgroup %p", ptr);
#else
  	my_print("rid_table_rowgroup");
#endif
        if (ptr == NULL)
        	return;
  	enter();
#ifndef NO_PTRS
  		FIELD(ptr, props, "%p");
#endif
  		FIELD(ptr, span, "%d");
/* 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/

		for (i = 0; ; i++)
		{
		    if ( flags[i].name == NULL )
			break;
		    if ( flags[i].flag & ptr->flags )
		    {
			my_print("%s ", flags[i].name);
		    }
		}


	leave();
}

extern void dump_colhdr(rid_table_colhdr *ptr)
{
#ifndef NO_PTRS
	my_print("rid_table_colhdr %p", ptr);
#else
	my_print("rid_table_colhdr");
#endif
        if (ptr == NULL)
        	return;
	enter();
#ifndef NO_PTRS
		FIELD(ptr, colgroup, "%p");
#endif
/*		FIELD(ptr, props, "%p");
		my_print("width");
		dump_stdunits(ptr->width);
 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/ 		my_print("width_info");
 		dump_width_info(ptr->width_info);
		FIELD(ptr, flags, "%08x");
		FIELD(ptr, offx, "%d");
		FIELD(ptr, sizex, "%d");
	leave();
}


extern void dump_rowhdr(rid_table_rowhdr *ptr)
{
    static struct { char *name; rid_rowhdr_flags flag; }
    flags [] =
    {
	{ "rid_rhf_GROUP_ABOVE", rid_rhf_GROUP_ABOVE },
	{ "rid_rhf_GROUP_BELOW", rid_rhf_GROUP_BELOW },
	{ "rid_rhf_PERCENT", rid_rhf_PERCENT },
	{ "rid_rhf_RELATIVE", rid_rhf_RELATIVE },
	{ "rid_rhf_ABSOLUTE", rid_rhf_ABSOLUTE },
	{ "rid_rhf_THEAD", rid_rhf_THEAD },
	{ "rid_rhf_TBODY", rid_rhf_TBODY },
	{ "rid_rhf_TFOOT", rid_rhf_TFOOT },
	{ NULL, 0 }
    };
    int i;

#ifndef NO_PTRS
 	my_print("rid_table_rowhdr %p", ptr);
#else
 	my_print("rid_table_rowhdr");
#endif
        if (ptr == NULL)
        	return;
 	enter();
#ifndef NO_PTRS
 		FIELD(ptr, rowgroup, "%p");
#endif
/* 		FIELD(ptr, props, "%p");
 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/		FIELD(ptr, offy, "%d");
		FIELD(ptr, sizey, "%d");

		for (i = 0; ; i++)
		{
		    if ( flags[i].name == NULL )
			break;
		    if ( flags[i].flag & ptr->flags )
		    {
			my_print("%s ", flags[i].name);
		    }
		}

  	leave();
}




extern void dump_cell(rid_table_cell *ptr, char *base)
{
#ifndef NO_PTRS
  	my_print("rid_table_cell %p", ptr);
#else
  	my_print("rid_table_cell");
#endif
        if (ptr == NULL)
        	return;
  	enter();
#ifndef NO_PTRS
  		FIELD(ptr, parent, "%p");
#endif
/*  		FIELD(ptr, props, "%p");
 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("axes %s", X_OR_NULL(ptr->axes));
 		my_print("axis %s", X_OR_NULL(ptr->axis));
 		my_print("class %s", X_OR_NULL(ptr->class));
		FIELD(ptr, flags, "%08x");
*/		my_print("cell %d %d", ptr->cell.x, ptr->cell.y);
		my_print("span %d %d", ptr->span.x, ptr->span.y);
		my_print("size %d %d", ptr->size.x, ptr->size.y);
#if NEWGROW
		my_print("want %d %d", ptr->swant.x, ptr->swant.y);
#else
		my_print("sleft %d", ptr->sleft);
#endif
		dump_props(ptr->props);
  		dump_stream(&ptr->stream, base);
	leave();
}

extern void dump_stream(rid_text_stream *ptr, char *base)
{
  	rid_text_item *ti;
  	rid_pos_item *pi;
#ifndef NO_PTRS
 	my_print("rid_text_stream %p", ptr);
#else
 	my_print("rid_text_stream");
#endif
        if (ptr == NULL)
        	return;
 	enter();
#ifndef NO_PTRS
		/* Don't number these */
  		FIELD(ptr, pos_list, "%p");
  		FIELD(ptr, pos_last, "%p");
  		FIELD(ptr, text_list, "%p");
  		FIELD(ptr, text_last, "%p");
		FIELD(ptr, text_fvpr, "%p");
  		/*FIELD(ptr, parent, "%p");*/
  		/*FIELD(ptr, partype, "%d");*/
#endif
  		FIELD(ptr, fwidth, "%d");
  		FIELD(ptr, width, "%d");
  		FIELD(ptr, widest, "%d");
  		FIELD(ptr, height, "%d");
  		my_print("width_info");
  		dump_width_info(ptr->width_info);
		for (ti = ptr->text_list; ti != NULL; ti = ti->next)
			dump_item(ti, base);
#ifndef NO_PTRS
  		FIELD(ptr, pos_list, "%p");
#endif
		for (pi = ptr->pos_list; pi != NULL; pi = pi->next)
			dump_pos(pi);
  	leave();

}

/*****************************************************************************

  Dump out the textual words with line breaks where the formatter has
  decided they should be. Depending upon font sizing, this might not
  look initially sensible in monospace.

  */

static void dump_textual_formatting(rid_header *ptr)
{
    rid_text_item *ti;
    char *base = ptr->texts.data;

    if (ptr == NULL)
	return;

    my_print("<<<<");

    for (ti = ptr->stream.text_list; ti != NULL; ti = ti->next)
    {
	switch (ti->tag)
	{
	case rid_tag_TEXT:
	    if (ti == NULL || (ti->line && ti == ti->line->first))
		nonlprint("\n");
	    nonlprint("%s", base + ((rid_text_item_text*)ti)->data_off);
	    
	    break;

	case rid_tag_PBREAK:
	    nonlprint("\n");
	    break;

	default:
	    break;
	}
    }

    my_print(">>>>");
}

extern void dump_form_element(rid_form_element *ptr)
{
    my_print("rid_form_element %p", ptr);
    if (ptr == NULL)
	return;

    enter();
        my_print("prev %p, next %p, parent %p, display %p",
		 ptr->prev, ptr->next, ptr->parent, ptr->display);
	switch (ptr->tag)
	{
	case rid_form_element_INPUT:
	    my_print("Tag: INPUT");
	    break;
	case rid_form_element_SELECT:
	    my_print("Tag: SELECT");
	    break;
	case rid_form_element_TEXTAREA:
	    my_print("Tag: TEXTAREA");
	    break;
	default:
	    my_print("Tag: **** UNKNOWN: %d ****", ptr->tag);
	    break;
	}
	my_print("Colours back %d, select %d, cursor %d",
		 ptr->colours.back, ptr->colours.select, ptr->colours.cursor);
	my_print("Tabindex %d", ptr->tabindex);
    leave();
}

extern void dump_textarea_item(rid_textarea_item *ptr)
{
    my_print("rid_textarea_item %p", ptr);
    if (ptr == NULL)
	return;
    enter();
        dump_form_element(&ptr->base);
	my_print("name %s", ptr->name);
	my_print("size %d,%d, caret %d,%d, scroll %d,%d",
		 ptr->rows, ptr->cols, ptr->cx, ptr->cy, ptr->sx, ptr->sy);
#if !NEW_TEXTAREA
	my_print("default lines %p, %p", ptr->default_lines, ptr->def_last_line);
	my_print("lines %p, %p", ptr->lines, ptr->last_line);
	my_print("caret line %p", ptr->caret_line);
#endif
    leave();
}

extern void dump_select_item(rid_select_item *ptr)
{
    my_print("rid_select_item %p", ptr);
    if (ptr == NULL)
	return;
    enter();
        dump_form_element(&ptr->base);
        my_print("name %s", ptr->name);
	my_print("count %d, size %d", ptr->count, ptr->size);
	my_print("items %p, menuh %p, doc %p", ptr->items, ptr->menuh, ptr->doc);
	my_print("options %p, %p", ptr->options, ptr->last_option);
	my_print("flags 0x%x", ptr->flags);
    leave();
}

extern void dump_form_item(rid_form_item *ptr)
{
    rid_form_element *elem;

    my_print("rid_form_item %p", ptr);
    if (ptr == NULL)
	return;
    enter();
        my_print("next %p, prev %p", ptr->next, ptr->prev);
	my_print("kids %p, last_kid %p", ptr->kids, ptr->last_kid);
	for (elem = ptr->kids; elem != NULL; elem = elem->next)
	    dump_form_element(elem);
	my_print("last text %p", ptr->last_text);
	dump_textarea_item(ptr->last_text);
	my_print("last_select %p", ptr->last_select);
	dump_select_item(ptr->last_select);
	switch (ptr->method)
	{
	case rid_fm_GET:
	    my_print("Method: GET");
	    break;
	case rid_fm_POST:
	    my_print("Method: POST");
	    break;
	default:
	    my_print("Method: ***** UNKNOWN: %d ****", ptr->method);
	    break;
	}
	my_print("Action: %s", ptr->action);
	my_print("Target: %s", ptr->target);
	my_print("Id: %s", ptr->id);
    leave();
}

extern void dump_header(rid_header *ptr)
{
    int x;
    rid_form_item *fi;

#if 0
#ifndef NO_PTRS
  	my_print("rid_header %p (%s, %s, %s)", ptr, caller(1), caller(2), caller(3));
#else
  	my_print("rid_header (%s, %s, %s)", caller(1), caller(2), caller(3));
#endif
#else
	my_print("rid_header");
#endif

        if (ptr == NULL)
        	return;
  	enter();
#ifndef NO_PTRS
  		FIELD(ptr, curstream, "%p");
  		FIELD(ptr, aref_list, "%p");
  		FIELD(ptr, aref_last, "%p");
  		FIELD(ptr, form_list, "%p");
  		FIELD(ptr, form_last, "%p");
#endif
  		FIELD(ptr, flags, "%08x");
  		my_print("title %s", X_OR_NULL(ptr->title));
  		my_print("base %s", X_OR_NULL(ptr->base));
  		dump_stream(&ptr->stream, ptr->texts.data);
		for (x = 0, fi = ptr->form_list; fi != NULL; x++, fi = fi->next)
		{
		    my_print("FORM %d", x);
		    dump_form_item(fi);
		}
  	leave();

	/*dump_textual_formatting(ptr);*/
}

extern void dump_buffer(UBUFFER *ptr)
{
    my_print("UBUFFER [:%d] = '%.*s', max %d",
	      ptr->ix, ptr->ix, ptr->data, ptr->max);
}

extern void dump_sgmlctx(SGMLCTX *ptr)
{
#ifndef NO_PTRS
    my_print("SGMLCTX %p", ptr);
#else
    my_print("SGMLCTX");
#endif
    if (ptr == NULL)
	return;
    enter();
	FIELD(ptr, magic, "%x");
#ifndef NO_PTRS
	FIELD(ptr, elements, "%p");
#endif
	FIELD(ptr, mode, "%d");
#ifndef NO_PTRS
	FIELD(ptr, clictx, "%p");
#endif
	FIELD(ptr, apply_heuristics, "%d");
#ifndef NO_PTRS
	FIELD(ptr, tos, "%p");
#endif
	FIELD(ptr, line, "%d");
	dump_buffer(&ptr->inhand);
	dump_buffer(&ptr->prechop);
#ifndef NO_PTRS
	FIELD(ptr, state, "%p");
	FIELD(ptr, chopper, "%p");
	FIELD(ptr, deliver, "%p");
#endif
    leave();
}



extern void dump_table_width_details(rid_table_item *table)
{
#if 0
#define NO_PCT 255

    int x, y;

    TABDBG(("Min col (>=): "));
    for (x = 0; x < table->cells.x; x++)
    {
	rid_table_colhdr *colhdr = &table->colhdrs[x];
	if (colhdr->minpct == NO_PCT)
	    TABDBG(("---- "));
	else
	    TABDBG(("%3d%% ", colhdr->minpct));
    }
    TABDBG(("\nMax col (<=): "));
    for (x = 0; x < table->cells.x; x++)
    {
	rid_table_colhdr *colhdr = &table->colhdrs[x];
	if (colhdr->maxpct == NO_PCT)
	    TABDBG(("---- "));
	else
	    TABDBG(("%3d%% ", colhdr->maxpct));
    }
    TABDBG(("\nFinal width : "));
    for (x = 0; x < table->cells.x; x++)
    {
	rid_table_colhdr *colhdr = &table->colhdrs[x];
	if (colhdr->finpct == NO_PCT)
	    TABDBG(("---- "));
	else
	    TABDBG(("%3d%% ", colhdr->finpct));
    }
    TABDBG(("\nSum offset  : "));
    for (x = 0; x < table->cells.x; x++)
    {
	rid_table_colhdr *colhdr = &table->colhdrs[x];
	if (colhdr->sumpct == NO_PCT)
	    TABDBG(("---- "));
	else
	    TABDBG(("%3d%% ", colhdr->sumpct));
    }
    TABDBG(("\n\n"));

    for (y = 0; y < table->cells.y; y++)
    {
	TABDBG(("Min %3d (>=): ", y));
	for (x = 0; x < table->cells.x; x++)
	{
	    rid_table_cell *cell = *CELLFOR(table, x, y);
	    if (cell == NULL || 
		(cell->flags & rid_cf_PERCENT) == 0 ||
		cell->minpct == NO_PCT)
		TABDBG(("---- "));
	    else
		TABDBG(("%3d%% ", cell->minpct));
	}
	TABDBG(("\nMax %3d (<=): ", y));
	for (x = 0; x < table->cells.x; x++)
	{
	    rid_table_cell *cell = *CELLFOR(table, x, y);
	    if (cell == NULL || 
		(cell->flags & rid_cf_PERCENT) == 0 ||
		cell->maxpct == NO_PCT)
		TABDBG(("---- "));
	    else
		TABDBG(("%3d%% ", cell->maxpct));
	}
	TABDBG(("\n"));
    }
#endif
}

extern void dump_htmlctx(HTMLCTX *ptr)
{
    int i;
#ifndef NO_PTRS
    my_print("HTMLCTX %p", ptr);
#else
    my_print("HTMLCTX");
#endif
    if (ptr == NULL)
	return;
    enter();
	FIELD(ptr, magic, "%x");
#ifndef NO_PTRS
	FIELD(ptr, isa, "%p");
	FIELD(ptr, sgmlctx, "%p");
	FIELD(ptr, rh, "%p");
	FIELD(ptr, map, "%p");
	FIELD(ptr, aref, "%p");
	FIELD(ptr, form, "%p");
	FIELD(ptr, table, "%p");

	for (i = 0; i < 20; i++)
	    fprintf(DBGOUT, "%p ", ptr->frameset_stack[i]);
	fprintf(stderr, "\n");
#endif

	FIELD(ptr, basetarget, "%s");
	FIELD(ptr, auto_open_string, "%s");
#ifndef NO_PTRS
	FIELD(ptr, frameset, "%p");
#endif
	FIELD(ptr, noframe, "%d");
#ifndef NO_PTRS
	FIELD(ptr, old_deliver, "%p");
	FIELD(ptr, object, "%p");
#endif
	FIELD(ptr, object_nesting, "%d");
    leave();
}

#endif /* DEBUG */

/* eof dump.c */
