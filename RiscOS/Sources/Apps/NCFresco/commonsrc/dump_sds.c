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

FILE *DBGFP;

#define DUMPTAG			1

#define DBGSTR(s)		sds_debug_str(DBGFP, DUMPTAG, s)
#define DBGMSG(s)		sds_debug_msg(DBGFP, DUMPTAG, s)
#define DBGINT(i)		sds_debug_int(DBGFP, DUMPTAG, i)
#define DBGHEX(b)		sds_debug_bits(DBGFP, DUMPTAG, b)
#define DBGPTR(p)		sds_debug_ptr(DBGFP, DUMPTAG, p)
#define DBGCHR(c)		sds_debug_char(DBGFP, DUMPTAG, c)

#define DBGNL			sds_debug_newline(DBGFP, DUMPTAG)
#define BEGIN			sds_begin_nesting(DBGFP, DUMPTAG)
#define END			sds_end_nesting(DBGFP, DUMPTAG)
#define TERMINATOR		sds_terminator(DBGFP, DUMPTAG)

#define PINT(ptr, field)	DBGMSG(field # " "); DBGINT((ptr)->field); NL
#define PSTR(ptr, field)	DBGMSG(field # " "); DBGSTR((ptr)->field); NL
#define PPTR(ptr, field)	DBGMSG(field # " "); DBGPTR((ptr)->field); NL
#define PHEX(ptr, field)	DBGMSG(field # " "); DBGBITS((ptr)->field); NL

#define NL			DBGNL

/*****************************************************************************/

/* DON'T want this going out in production code (again!) */

#if DEBUG || defined(BUILDERS)

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

    DBGMSG("Base ");
    DBGPTR(base);
    DBGMSG(", span ");
    DBGINT(span);
    
    BEGIN;

    for (x = 0; x < span; x++)
    {
	char b[32];
	sprintf(b, "%4d ", base[x]);
	DBGSTR(b);
    }

    END;
}

/*****************************************************************************/

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

    DBGMSG("ROStyle ");
    DBGHEX(UNPACK(bp, ROSTYLE));
    
    BEGIN;

    if ( UNPACK(bp, ROSTYLE) & 0xffff )
    {
	DBGMSG("Flags:");
	if ( UNPACK(bp, STYLE_UNDERLINE) )
	    DBGMSG("underlined");
	DBGSTR(align_names[ UNPACK(bp, STYLE_ALIGN) ] );
	if ( UNPACK(bp, STYLE_SUB) )
	    DBGSTR("sub");
	if ( UNPACK(bp, STYLE_SUP) )
	    DBGSTR("sup");
	if ( UNPACK(bp, STYLE_RESERVED) )
	    DBGSTR("RESERVED");
	if ( UNPACK(bp, STYLE_STRIKE) )
	    DBGSTR("strike");
	if ( UNPACK(bp, STYLE_COLOURNO) )
	{
	    DBGMSG("colour");
	    DBGINT(UNPACK(bp, STYLE_COLOURNO) );
	}
	NL;
    }

    x = UNPACK(bp, STYLE_WF_INDEX);
    if (x != 2)		/* 2 being the common case */
    {
	DBGMSG("Font");
	DBGINT(x);

	if (x & WEBFONT_FLAG_SPECIAL)
	{
	    int z = x & WEBFONT_SPECIAL_TYPE_MASK;
	    DBGINT(z);
	    if (z == WEBFONT_SPECIAL_TYPE_MENU)
		DBGSTR("menu");
	    else
		DBGSTR("symbol");
	}
	else
	{
	    if (x & WEBFONT_FLAG_BOLD)
		DBGSTR(" bold");
	    if (x & WEBFONT_FLAG_ITALIC)
		DBGSTR(" italic");
	    if (x & WEBFONT_FLAG_FIXED)
		DBGSTR(" fixed");
	    if (WEBFONT_SIZEOF(x) != 3)
	    {
		DBGMSG("size");
		DBGINT(WEBFONT_SIZEOF(x) );
	    }
	}
	NL;
    }

    if ( UNPACK(bp, STYLE_INDENT) )
    {
	DBGMSG("indent");
	DBGINT(UNPACK(bp, STYLE_INDENT) );
    }

    END;
}

extern void dump_float_item(rid_float_item *ptr)
{
    DBGMSG("rid_float_item %p");
    DBGPTR(ptr);
    
    BEGIN();

    PPTR(ptr, ti);
    PPTR(ptr, pi);

    END;
}

extern void dump_floats_link(rid_floats_link *ptr)
{
    DBGMSG("rid_floats_link");
    DBGPTR(ptr);
    
    BEGIN;

    PPTR(ptr, left);
    PPTR(ptr, right);

    if (ptr->left)
	dump_float_item(ptr->left);
    if (ptr->right)
	dump_float_item(ptr->right);

    END;
}

extern void dump_pos(rid_pos_item *pi)
{
    DBGMSG("rid_pos_item %p");
    DBGPTR(pi);

    if (pi == NULL)
	return;

    ENTER;

    PPTR(pi, prev);
    PPTR(pi, next);
    PPTR(pi, st);

    PINT(pi, top);
    PINT(pi, left_margin);
    PINT(pi, max_up);
    PINT(pi, max_down);

    switch (pi->leading)
    {
    case MAGIC_LEADING_PENDING:
	DBGSTR("Magic leading pending");
	break;
    case -1:
	DBGSTR("Leading - generic MAGIC -1");
	break;
    case 0:
	break;
    default:
	PINT(pi, leading);
	break;
    }

    PPTR(pi, first);
    PPTR(pi, floats);

    if (pi->floats)
	dump_floats_link(pi->floats);

    END;
}

extern void dump_stdunits(rid_stdunits v)
{
    char b[64];

    DBGMSG("stdunit");

    switch (v.type)
    {
    case value_absunit:
	sprintf(b, "stdunit %d", (int) ceil(v.u.f) / 2 );
	break;
    case value_relunit:
	sprintf(b, "stdunit %g*", v.u.f );
	break;
    case value_pcunit:
	sprintf(b, "stdunit %g%%", v.u.f );
	break;
    default:
	sprintf(b, "stdunit <%d>", v.type);
	break;
    }

    DBGSTR(b);
    NL;
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
    "OBJECT"
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

    DBGMSG("rid_text_item");
    DBGPTR(item);
    NL;

    if (item == NULL)
	return;

    BEGIN;

    PPTR(item, next);
    PPTR(item, line);
    PPTR(item, aref);

    PINT(item, max_up);
    PINT(item, max_down);
    PINT(item, width);
    PINT(item, pad);

    PHEX(item, flag);

    ENTER;

    for (i = 0; i < 8; i++)
	if (item->flag & (1<<i) )
	{
	    PSTR(flag_names[i]);
	}

    END;

    DBGMSG("tag");
    PSTR(item_names[item->tag]);

    dump_style(item->st);

    switch (item->tag)
    {
    case rid_tag_TABLE:
	dump_table( ((rid_text_item_table*)item)->table, base);
	break;
    case rid_tag_TEXT:
	if (base)
	{
	    PSTR(base + ((rid_text_item_text*)item)->data_off);
	}
	break;
    }

    END;
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
    FIELD(ptr, default_lines, "%d");
#ifndef NO_PTRS
    FIELD(ptr, def_last_line, "%p");
    FIELD(ptr, lines, "%p");
    FIELD(ptr, last_line, "%p");
    FIELD(ptr, caret_line, "%p");
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
    DBGMSG("rid_table_caption");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PPTR(ptr, table);
    /*my_print("calign %s", str_from_tag(ptr->calign, strtag_calign));*/
    /*FIELD(ptr, id, "%p");*/
    /*FIELD(ptr, class, "%p");*/
    /*FIELD(ptr, props, "%p");*/
    dump_stream(&ptr->stream, base);

    END;
}

extern void dump_cell_map(rid_table_item *ptr, char *msg)
{
    int x,y;
    rid_table_cell *cell;

    DBGMSG("Cell occupancy");
    DBGMSG(msg);

    for (y = 0; y < ptr->cells.y; y++)
    {
	for (x = 0; x < ptr->cells.x; x++)
	{
	    cell = * CELLFOR(ptr, x, y);
	    if (cell == NULL)
	    {
		DBGCHR('.');
	    }
	    else if (x == cell->cell.x && y == cell->cell.y)
	    {
		DBGCHR('#');
	    }
	    else
	    {
		DBGCHR('-');
	    }
	}
	NL;
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

    DBGMSG("rid_table_item");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PPTR(ptr, parent);
    PPTR(ptr, oldstream);
    PHEX(ptr, flags);

    BEGIN;

    for (; tfp->name != NULL; tfp++)
    {
	if ( (ptr->flags & tfp->bit) != 0 )
	{
	    PSTR(tfp->name);
	}
    }

    END;

    DBGMSG("userborder");

    dump_stdunits(ptr->userborder);
    /*my_print("frame %s", str_from_tag(ptr->frame, strtag_frame));*/
    /*my_print("rules %s", str_from_tag(ptr->rules, strtag_rules));*/

    DBGMSG("userwidth");
    dump_stdunits(ptr->userwidth);

    DBGMSG("cellspacing");
    dump_stdunits(ptr->usercellspacing);

    DBGMSG("cellpadding");
    dump_stdunits(ptr->usercellpadding);

    PINT(ptr, frame);
    PINT(ptr, rules);
    PINT(ptr, border);
    PINT(ptr, cellspacing);
    PINT(ptr, cellpadding);
    PINT(ptr, header_rows);
    PINT(ptr, footer_rows);

    DBGMSG("num_groups");
    DBGINT(ptr->num_groups.x);
    DBGINT(ptr->num_groups.y);
    NL;

    PPTR(ptr, array);
    PPTR(ptr, rowhdrs);
    PPTR(ptr, colhdrs);
    PPTR(ptr, rowgroups);
    PPTR(ptr, colgroups);

    DBGMSG("cells");
    DBGINT(ptr->cells.x);
    DBGINT(ptr->cells.y);
    NL;

    DBGMSG_("size");
    DBGINT(ptr->size.x);
    DBGINT(ptr->size.y);
    NL;

    DBGMSG("width_info");

/*        	FIELD(ptr, hwidths, "%p");*/
/*        	FIELD(ptr, vheights, "%p"); */

    dump_width_info(ptr->width_info);

    PINT(ptr, state);

    DBGMSG("scaff");
    DBGINT(ptr->scaff.x);
    DBGINT(ptr->scaff.y);
    NL;

    DBGINT("Column groups and headers");
    NL;

    for (x = 0; x < ptr->num_groups.x; x++)
	dump_colgroup(ptr->colgroups[x]);
    for (x = 0; x < ptr->cells.x; x++)
	dump_colhdr(ptr->colhdrs[x]);

    DBGMSG("Row groups and headers");
    NL;

    for (y = 0; y < ptr->num_groups.y; y++)
	dump_rowgroup(ptr->rowgroups[y]);
    for (y = 0; y < ptr->cells.y; y++)
	dump_rowhdr(ptr->rowhdrs[y]);

    DBGMSG("Text streams");
    NL;

    dump_caption(ptr->caption, base);
    for (y = 0; y < ptr->cells.y; y++)
    {
	for (x = 0; x < ptr->cells.x; x++)
	{
	    cell = * CELLFOR(ptr, x, y);
	    if (cell == NULL)
	    {
		DBGMSG("Cell");
		DBGINT(x);
		DBGINT(y);
		NL;
	    }
	    else if (x == cell->cell.x && y == cell->cell.y)
	    {
		DBGMSG_("Cell");
		DBGINT(x);
		DBGINT(y);
		DBGMSG("root item");
		NL;

		dump_cell(cell, base);
	    }
	    else
	    {
		DBGMSG_("Cell");
		DBGINT(x);
		DBGINT(y);
		DBGMSG("covered item");
		DBGINT(cell->cell.x);
		DBGINT(cell->cell.y);
		NL;
	    }
	}
	DBGMSG("End of table row");
	DBGINT(y);
	NL;
    }

    dump_cell_map(ptr, "");

    END;
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

    BEGIN;

    PINT(ptr, minleft);
    PINT(ptr, minright);
    PINT(ptr, minwidth);
    PINT(ptr, maxleft);
    PINT(ptr, maxright);
    PINT(ptr, maxwidth);

    END;
}


extern void dump_colgroup(rid_table_colgroup *ptr)
{
    DBGMSG("rid_table_colgroup");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PPTR(ptr, props);

    PINT(ptr, first);
    PINT(ptr, span);
/* 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/
    DBGMSG("width");
    dump_stdunits(ptr->userwidth);

    END;
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

    DBGMSG("rid_table_rowgroup");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PPTR(ptr, props);

    PINT(ptr, span);
/* 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/

    for (i = 0; ; i++)
    {
	if ( flags[i].name == NULL )
	    break;
	if ( flags[i].flag & ptr->flags )
	{
	    PSTR(flags[i].name);
	}
    }

    END;
}

extern void dump_colhdr(rid_table_colhdr *ptr)
{
    DBGMSG("rid_table_colhdr");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PPTR(ptr, colgroup);

/*		FIELD(ptr, props, "%p");
		my_print("width");
		dump_stdunits(ptr->width);
 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/
    DBGMSG("width_info");

    dump_width_info(ptr->width_info);

    PHEX(ptr, flags);
    PINT(ptr, offx);
    PINT(ptr, sizex);

    ENDL
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

    DBGMSG("rid_table_rowhdr");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    ENTER;

    PPTR(ptr, rowgroup);

/* 		FIELD(ptr, props, "%p");
 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("class %s", X_OR_NULL(ptr->class));
*/

    PINT(ptr, offy);
    PINT(ptr, sizey);

    for (i = 0; ; i++)
    {
	if ( flags[i].name == NULL )
	    break;
	if ( flags[i].flag & ptr->flags )
	{
	    PSTR(flags[i].name);
	}
    }

    END;
}




extern void dump_cell(rid_table_cell *ptr, char *base)
{
    DBGMSG("rid_table_cell");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    ENTER;

    PPTR(ptr, parent);

/*  		FIELD(ptr, props, "%p");
 		my_print("id %s", X_OR_NULL(ptr->id));
 		my_print("axes %s", X_OR_NULL(ptr->axes));
 		my_print("axis %s", X_OR_NULL(ptr->axis));
 		my_print("class %s", X_OR_NULL(ptr->class));
		FIELD(ptr, flags, "%08x");
*/		my_print("cell %d %d", ptr->cell.x, ptr->cell.y);

    DBGMSG("span");
    DBGINT(ptr->span.x);
    DBGINT(ptr->span.y);
    NL;

    DBGMSG("size");
    DBGINT(ptr->size.x);
    DBGINT(ptr->size.y);
    NL;

    DBGMSG("sleft");
    DBGINT(ptr->sleft);
    NL;

    dump_props(ptr->props);
    dump_stream(&ptr->stream, base);

    END;
}

extern void dump_stream(rid_text_stream *ptr, char *base)
{
    rid_text_item *ti;
    rid_pos_item *pi;

    DBGMSG("rid_text_stream");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PINT(ptr, pos_list);
    PPTR(ptr, pos_last);
    PPTR(ptr, text_list);
    PPTR(ptr, text_last);
    PPTR(ptr, text_fvpr);
    PPTR(ptr, parent);
    PINT(ptr, partype);
    PINT(ptr, fwidth);
    PINT(ptr, width);
    PINT(ptr, widest);
    PINT(ptr, height);

    DBGMSG("width_info");

    dump_width_info(ptr->width_info);

    for (ti = ptr->text_list; ti != NULL; ti = ti->next)
	dump_item(ti, base);

    PPTR(ptr, pos_list);

    for (pi = ptr->pos_list; pi != NULL; pi = pi->next)
	dump_pos(pi);

    END;
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
    my_print("default lines %p, %p", ptr->default_lines, ptr->def_last_line);
    my_print("lines %p, %p", ptr->lines, ptr->last_line);
    my_print("caret line %p", ptr->caret_line);
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

    DBGMSG("rid_header");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PPTR(ptr, curstream);
    PPTR(ptr, aref_list);
    PPTR(ptr, aref_last);
    PPTR(ptr, form_list);
    PPTR(ptr, form_last);
    PHEX(ptr, flags);

    DBGMSG("title");
    DBGSTR(X_OR_NULL(ptr->title));
    NL;

    DBGMSG("base");
    DBGSTR(X_OR_NULL(ptr->base));
    NL;

    dump_stream(&ptr->stream, ptr->texts.data);
    for (x = 0, fi = ptr->form_list; fi != NULL; x++, fi = fi->next)
    {
	DBGMSG("FORM");
	DBGINT(x);
	dump_form_item(fi);
    }

    END;
}

extern void dump_buffer(BUFFER *ptr)
{
    my_print("BUFFER [:%d] = '%.*s', max %d",
	     ptr->ix, ptr->ix, ptr->data, ptr->max);
}

extern void dump_sgmlctx(SGMLCTX *ptr)
{
    DBGMSG("SGMLCTX");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PHEX(ptr, magic);
    PPTR(ptr, elements);
    PINT(ptr, mode);
    PPTR(ptr, clictx);
    PINT(ptr, apply_heuristics);
    PPTR(ptr, tos);
    PINT(ptr, line);
    dump_buffer(&ptr->inhand);
    dump_buffer(&ptr->prechop);
    PPTR(ptr, state);
    PPTR(ptr, chopper);
    PPTR(ptr, deliver);

    END;
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

    DBGMSG("HTMLCTX ");
    DBGPTR(ptr);
    NL;

    if (ptr == NULL)
	return;

    BEGIN;

    PINT(ptr, magic);
    PPTR(ptr, isa);
    PPTR(ptr, sgmlctx);
    PPTR(ptr, rh);
    PPTR(ptr, map);
    PPTR(ptr, aref);
    PPTR(ptr, form);
    PPTR(ptr, form);

    BEGIN;
    for (i = 0; i < 20; i++)
    {
	DBGPTR(ptr->frameset_stack[i]);
	DBGMSG(" ");
    }
    END;

    PSTR(ptr, basetarget);
    PSTR(ptr, auto_open_string);
    PPTR(ptr, frameset);
    PINT(ptr, noframe);
    PPTR(ptr, old_deliver);
    PPTR(ptr, object);
    PINT(ptr, object_nesting);

    END;
}

#endif /* DEBUG */

/* eof dump.c */
