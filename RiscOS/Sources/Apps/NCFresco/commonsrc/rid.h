/* -*-c-*- */

/* 23-02-96 DAF         Added some stuff related to tables. */
/* 19-03-96 DAF         Added rid_sf_RIGHTALIGN. Some tidying up. */
/* 02-04-96 SJM         Added meta_list, remove windowname */
/* 19/07/06 SJM		Change form element structures */
/* 26/02/97 DAF		FVPR changes */

/* rid.h */

#ifndef __rid_h
#define __rid_h

#ifndef __coretypes_h
#include "coretypes.h"
#endif

#ifndef __glue_h
#include "glue.h"
#endif

#ifndef __sgmlparser_h
#include "sgmlparser.h"
#endif

#ifndef MAX_TEXT_LINE
#define MAX_TEXT_LINE 1024
#endif

/* Wish to track propogation of this value. */
#define TABLE_NULL		((void*)1)

/*user count times two*/

#define DEFAULT_BORDER		2
#define DEFAULT_CELL_PADDING    4
#define DEFAULT_CELL_SPACING    2
#define DEFAULT_CAPTION_SPACING 4

#if 0
#define SHORTISH        int
#else
#define SHORTISH        short
#endif

/* This file defines the Riscos Internal Data structues to store the parsed data */

/**********************************************************************************

The data structure has to be able to store both the text itself and
all the relevant markup information.  It needs to store the data in a
form that makes it easy to map a position on the page to a section of
text for both ploting and use of links.  We need to be able to go from
an HREF 'name' attribute value to the start of the A block that has
that name value and we need to be able to go from the start of that
block to the position on the screen.  We also need to be able to
reformat the text with ease if the user wants to change the width.


The document is divided into five conceptual sections: header, text,
positioning, style and A-ref.

The header section contains document wide information.  This includes
the title, a base reference if there is one and pointers to locate the
other three parts.

The next part is the 'text' stream.  This is a doublely linked list of
items that will be displayed on the screen.  The items consist of a
header specifying the forward and reverse pointers and a tag
indicating what sort of object the item is, along with pointers to the
'positioning' list, to a style entry and to an A-ref entry.  Each
object also has values for its hight above and deapth below the text
base-line and its width on the display where relevent.  The object
might be a single word (in unformated text), a line (in pre-formated
text), a paragraph break, a horizontal rule or an image.  The exact
data context and size of the item depends on the tag value but the
header infomation at the from of the object is constant (and thus
determines the minimum size).

The 'positioning' list is a list of structures containing the maximum
rise and decent of the items on a line and the vertical position of
the top of the line within the displayed document.  It also contains a
pointer to the first object in the 'text' list that is to be displayed
on the given line.  To paint a strip of the screen the position is
located within the positioning list and then the objects are rendered
until the next object in the 'text' list is the first object of the
next line.

A style entry contains information used for the rendering of an object
from the 'text' list.  Since this infomation is usually the same from
one item to the next it makes sense to keep a pointer to the common
information rather than having copies for each object.  The style
entries are stored in a list so that they can be freed easily and so
that they can be changed should the user ask to change a font or tyep
size.

An A-ref entry is maintained for each 'A' block in the document.  It
holds all the attributes associated with the block and it contains a
pointer to the first item in the 'text' list in the within the block
so that when the section is found by name it can be located on the
page.  Pointing to the text item rather than directly to the
'positioning' list costs a little in terms of access but makes
reformating easier.  The A-ref items are stored in a list so that they
can be searched for a given name attribute.

**********************************************************************************/

/*****************************************************************************/

/* Forward declarations */

/*typedef struct rid_stdunits             *rid_stdunits;*/
#define rid_stdunits                    VALUE
typedef struct rid_table_props          rid_table_props;
typedef struct rid_width_info           rid_width_info;
typedef struct rid_table_caption        rid_table_caption;
typedef struct rid_table_cell           rid_table_cell;
typedef struct rid_table_colgroup       rid_table_colgroup;
typedef struct rid_table_rowgroup       rid_table_rowgroup;
typedef struct rid_table_item           rid_table_item;
typedef struct rid_text_item_table      rid_text_item_table;
typedef struct rid_text_stream          rid_text_stream;
typedef struct rid_table_rowhdr         rid_table_rowhdr;
typedef struct rid_table_colhdr         rid_table_colhdr;

typedef struct rid_frame                rid_frame;
typedef struct rid_frame_item           rid_frame_item;
typedef struct rid_frameset_item        rid_frameset_item;
typedef struct rid_frame_unit_totals    rid_frame_unit_totals;
typedef struct rid_area_item            rid_area_item;
typedef struct rid_map_item             rid_map_item;
typedef struct rid_fmt_info             rid_fmt_info;
typedef struct rid_fmt_state		rid_fmt_state;
typedef struct rid_meta_item            rid_meta_item;

typedef struct rid_object_param		rid_object_param;
typedef struct rid_object_item		rid_object_item;
typedef struct rid_text_item_object	rid_text_item_object;

/*****************************************************************************/

/* A memzone is used to allocate memory in a single contiguous block
 * that can grow as needed.  It uses a flex block that is grown in
 * chunks.  It can be made to keep objects word alligned or to use
 * character boundaries.  Items can not be freed, but the whole zone
 * can be disposed of. */

#define MEMZONE_WORDS   1       /* Items are word aligned */
#define MEMZONE_CHUNKS  2       /* Zone grows in chunks (and may need to be tidied at the end) */

typedef struct {
    char *data;
    int alloced;
    int used;
    int flags;
} memzone;


typedef struct
{
    char *src;
    void *im;
    int width, height;
} rid_tile;



typedef SHORTISH rid_tag;
#define rid_tag_PBREAK          0       /* Paragraph break */
#define rid_tag_HLINE           1       /* Horizontal rule */
#define rid_tag_TEXT            2       /* Text item */
#define rid_tag_BULLET          3       /* Bullet item */
#define rid_tag_IMAGE           4       /* Inlined image */
#define rid_tag_INPUT           5       /* Any sort of INPUT item */
#define rid_tag_TEXTAREA        6       /* A writeable text area */
#define rid_tag_SELECT          7       /* A selection list (e.g. a menu) */
#define rid_tag_TABLE           8       /* A table */
#define rid_tag_OBJECT          9       /* An OBJECT */

#define rid_tag_LAST_TAG        10
#define rid_tag_MASK            0xf

typedef SHORTISH rid_flag;
#define rid_flag_LINE_BREAK     0x1     /* A line break MUST follow this item */
#define rid_flag_NO_BREAK       0x2     /* The break is just a style change and we are in an unbreakable object. */
#define rid_flag_SELECTED       0x4     /* This item is highlit */
#define rid_flag_LEFTWARDS      0x08    /* Floating or clearing leftwards */
#define rid_flag_RIGHTWARDS     0x10    /* Floating or clearing rightwards */
#define rid_flag_CLEARING	0x20	/* Clear floating items */
#define rid_flag_ACTIVATED	0x40	/* item is being clicked on */
#define rid_flag_EXPLICIT_BREAK	0x80	/* a BR element was used */

#define rid_flag_COLOUR_MASK   0xF00    /* Up to 16 colours per page */
#define rid_flag_COLOUR_SHIFT      8

#define rid_flag_FVPR		0x1000	/* Item reach FVPR style final values */
#define rid_flag_COALESCED      0x2000  /* Item has been coalesced with next */
#define rid_flag_RINDENT        0x4000  /* Right indent (or not! only one level) */

#define RID_COLOUR(rid) ( ( (rid)->st.flags >> STYLE_COLOURNO_SHIFT ) \
                         & STYLE_COLOURNO_MASK )

typedef struct rid_pos_item {
#if DEBUG
    MAGIC tag;
    int line_number;
#endif
    struct rid_pos_item *next, *prev;   /* Next line down */
    struct rid_text_stream *st; /* Parent */
    int top;                    /* The vertical starting position of the line */
    SHORTISH left_margin;	/* Space to the left of the items */
    SHORTISH leading;		/* Inter-word leading used in justified text */
    int max_up;			/* The height of the tallest riser - used to find the base line. */
    int max_down;		/* The depth of the lowest decender - I guess we could just use the next line. */
    struct rid_text_item *first; /* The first item on the line */
    struct rid_floats_link *floats; /* Non-null if either end of the line has any floats */
} rid_pos_item;

typedef struct rid_float_item {
    struct rid_text_item *ti;	/* The item that is floating */
    struct rid_pos_item *pi;	/* The FIRST line that it floats on */
    int height;			/* The height of the floating object */
    int left;			/* Left X for starting point */
    struct rid_float_item *next;
} rid_float_item;

typedef struct rid_floats_link {
    struct rid_float_item *left;
    struct rid_float_item *right;
} rid_floats_link;

/* This is just a hold-all used during formatting */
typedef struct {
    struct rid_text_item *ti;	/* Text item to float */
    struct rid_float_item *fi;	/* Float item when building list */
    int h;			/* Remaining height when formating */
    int used;			/* Flag indicating the item is used */
} rid_float_tmp_info;

typedef struct rid_text_item {
    struct rid_text_item *next; /* Single link now */
    struct rid_pos_item *line;  /* The start of the line */
    struct rid_aref_item *aref; /* The aref, or NULL if there is none */
    int max_up, max_down;       /* Riser and decender */
    SHORTISH width;             /* Display width, or -1 to extend all the way across the line */
    SHORTISH pad;               /* Pad width between this and the next item IF NOT AT THE END OF A LINE */
    rid_flag flag;              /* Flags */
    rid_tag tag;                /* Tag for the type of data */
    ROStyle st;                 /* Font and underline information */
    /* Extra information follows dependent on the tag value */
} rid_text_item;

typedef struct rid_style_item {
    struct rid_style_item *next; /* Linked list */
    /* Style information of some sort */
    struct webfont *wf;
    ROStyle ros;
} rid_style_item;

typedef int rid_aref_flags;

#define rid_aref_CHECKED_CACHE  0x01	/* we have checked the cache/history to see if this link is there */
#define rid_aref_IN_CACHE       0x02	/* this link is in the cache or history and should be coloured differently */
#define rid_aref_LABEL	        0x04	/* this link is not to a URL but to a form item */

typedef struct rid_aref_item {
    struct rid_aref_item *next, *prev;  /* Linked list */
    char *name;
    char *href;
    char *rel;
    char *title;
    rid_text_item *first;
    char *target;
    rid_aref_flags flags;
} rid_aref_item;


/* ----------------------------------------------------------------------------- */

typedef int rid_form_element_tag;
#define rid_form_element_INPUT		1
#define rid_form_element_SELECT		2
#define rid_form_element_TEXTAREA	3

typedef struct rid_form_element
{
    struct rid_form_element *next, *prev;
    struct rid_form_item *parent;
    struct rid_text_item *display;
    rid_form_element_tag tag;
    char *id;
    struct
    {
	int back;
	int select;
	int cursor;
    } colours;
    int tabindex;
} rid_form_element;


typedef enum {
    rid_it_TEXT,
    rid_it_PASSWD,
    rid_it_CHECK,
    rid_it_RADIO,
    rid_it_SUBMIT,
    rid_it_RESET,
    rid_it_IMAGE,
    rid_it_HIDDEN,
    rid_it_BUTTON
    } rid_input_tag;

typedef int rid_input_flags;
#define rid_if_CHECKED  0x01		/* was it selected in the HTML? */
#define rid_if_DISABLED 0x02
#define rid_if_SCANNED	0x04		/* Used when checking elements at end of form */
#define rid_if_SELECTED	0x08		/* with OPTION, was it selected by the user */
#define rid_if_NOCURSOR	0x10		/* with IMAGE, don't need x,y coords */
#define rid_if_NOPOPUP	0x20		/* with SELECT, don't popup menu */
#define rid_if_NUMBERS	0x40		/* with TEXT, PASSWORD, only allow numbers */

typedef int rid_image_flags;
#define rid_image_flag_ISMAP    0x01
#define rid_image_flag_ATOP     0x02
#define rid_image_flag_ABOT     0x04
#define rid_image_flag_ABSALIGN 0x08
#define rid_image_flag_REAL     0x10 /* We are displaying the real image (not a dummy) */
#define rid_image_flag_PERCENT  0x20 /* size is a percent value not pixels */

typedef struct rid_input_item {
    struct rid_form_element base;
    rid_input_tag tag;
    rid_input_flags flags;
    char *name;
    char *value;
    int xsize;			/* physical size in characters */
    char *src;                  /* If we have an image */
    char *src_sel;		/* URL of image */
    int ww, hh;			/* specified size for IMAGE or BORDERIMAGE */
    int max_len;		/* max buffer size in characters */
    union {
        char *str;              /* Used for TEXT and PASSWORD */
/*         int tick;  */              /* Used for RADIO and CHECKBOX */
        struct
	{
            void *im;
            rid_image_flags flags;
            SHORTISH x,y;       /* Click position */
        } image;		/* Used for IMAGE */
	struct
	{
            void *im_off;       /* base image */
	    void *im_on;	/* selected base image */
            rid_image_flags flags;
	    int tick;
	} radio;
	struct
	{
            void *im;           /* base image */
	    void *im_sel;	/* selected base image */
            rid_image_flags flags;
	    int tick;
	} button;		/* Used for SUBMIT, RESET, BUTTON */
    } data;
} rid_input_item;

typedef struct rid_option_item {
    struct rid_option_item *next, *prev;
    char *text;                 /* The text displayed for this option */
    char *value;                /* The value given when the option is selected */
    rid_input_flags flags;      /* Checked and/or disabled */
} rid_option_item;

typedef int rid_select_flags;
#define rid_sf_MULTIPLE 0x01

typedef struct rid_select_item {
    struct rid_form_element base;
    char *name;
    SHORTISH count;             /* How many items in the list */
    SHORTISH size;              /* For the display */
    void *items;                /* An array of items for the front end to use. */
    void *menuh;                /* Handle to the menu back from the front end. */
    void *doc;                  /* An antweb_doc * to the document, setup when sized */
    rid_option_item *options, *last_option;
    rid_select_flags flags;
} rid_select_item;

typedef struct rid_textarea_line {
    struct rid_textarea_line *next, *prev;
    char *text;
} rid_textarea_line;

typedef struct rid_textarea_item {
    struct rid_form_element base;
    char *name;
    SHORTISH rows, cols;
    SHORTISH cx, cy;                    /* Caret position */
    SHORTISH sx, sy;                    /* Scroll possition */
    rid_textarea_line *default_lines, *def_last_line;
    rid_textarea_line *lines, *last_line;
    rid_textarea_line *caret_line;
} rid_textarea_item;

typedef enum {
    rid_fm_GET,
    rid_fm_POST
    } rid_form_method;

typedef struct rid_form_item {
    struct rid_form_item *next, *prev;
    struct rid_form_element *kids, *last_kid;
    struct rid_textarea_item *last_text;
    struct rid_select_item *last_select;
    rid_form_method method;
    char *action;
    char *target;
    char *id;
} rid_form_item;


/*****************************************************************************/

typedef struct { BITS specific, generic; } align_table;



/* Example: Used for <TABLE DIR=...> */

typedef unsigned char rid_dir_tag;

#define rid_dt_UNKNOWN          0
#define rid_dt_LTR              1
#define rid_dt_RTL              2

/*****************************************************************************/

/* Example: Used for <TD ALIGN=...> */

typedef unsigned char rid_halign_tag;

/*****************************************************************************/

/* Example: Used for <TABLE ALIGN=...> */

typedef unsigned char rid_align_tag;

/*****************************************************************************/

/* Example: Used for <TABLE VALIGN=...> */

typedef unsigned char rid_valign_tag;

/*****************************************************************************/

/* Example: Used for <CAPTION ALIGN=...> */

typedef unsigned char rid_calign_tag;

#define rid_ct_UNKNOWN          0
#define rid_ct_TOP              1
#define rid_ct_BOTTOM           2
#define rid_ct_LEFT             3
#define rid_ct_RIGHT            4

/*****************************************************************************/

/* <TABLE FRAME=...> */

typedef unsigned char rid_frame_tag;

/*****************************************************************************/

/* <TABLE RULES=...> */

typedef unsigned char rid_rules_tag;

/*****************************************************************************

The user can specify widths in a variety of units, both absolute and relative
to the table's overall size. Internally, we only operate with PX and MULT
units for absolute and relative respectively.

pixels => osunits requires vertical & horizontal double

*/

#define rid_stdunit_PX		value_absunit
#define rid_stdunit_MULT	value_relunit
#define rid_stdunit_PCENT	value_pcunit

#define rid_stdunit_DEFAULT     rid_stdunit_PX          /* Recommended */

/*****************************************************************************

rid_cf_INF_BLAH indicate a span of zero was given, meaning the cell extends to the
edge of the table.

rid_cf_MULTIPLE might be set for rid_cf_INF_VERT and rid_cf_INF_HORIZ cells when
only one cell is actually covered - if this inaccuracy becomes important,
a post scan of some form is required as we may know enough to be able to state
the actual covered size when the cell is created.

rid_cf_HEADER indicates a cell specified with <TH> rather than <TD>. My reading of 23-1-96 DRAFT
is that <TD> may appear in <THEAD> or <TFOOT> and that <TH> may appear in <TBODY>.
Using <TH> applies extra stylistic presentation to a cell.

*/

typedef SHORTISH rid_cell_flags;

#define rid_cf_NOWRAP           0x0001  /* Don't wrap words in cell */
#define rid_cf_HEADER           0x0002  /* Cell is a "header" cell */
#define rid_cf_MULTIPLE         0x0004  /* Cell covers more than one grid item */
#define rid_cf_COMPLETE         0x0008  /* </TD> or equiv seen for cell */
#define rid_cf_INF_VERT         0x0010  /* Cell extends to vert edge of table */
#define rid_cf_INF_HORIZ        0x0020  /* Cell extends to horiz edge of table */
#define rid_cf_PERCENT		0x0040  /* Cell has WIDTH=N% */
#define rid_cf_RELATIVE		0x0080  /* Cell has WIDTH=N* */
#define rid_cf_ABSOLUTE		0x0100  /* Cell has WIDTH=N  */
#define rid_cf_BACKGROUND       0x0200  /* Cell has background colour */

/*****************************************************************************

Flags for a column header. <COL SPAN=0> indicates the column header spans to
the horizontal extent. This requires colhdr replication during column addition
as well as cell spreading.

*/

typedef unsigned char rid_colhdr_flags;
typedef unsigned char rid_rowhdr_flags;
typedef unsigned char rid_rowgrp_flags;

#define rid_rhf_GROUP_ABOVE     0x0001
#define rid_rhf_GROUP_BELOW     0x0002
#define rid_rhf_PERCENT		0x0004
#define rid_rhf_RELATIVE	0x0008
#define rid_rhf_ABSOLUTE	0x0010
#define rid_rhf_THEAD		0x0020
#define rid_rhf_TBODY		0x0040
#define rid_rhf_TFOOT		0x0080

#define rid_chf_GROUP_LEFT      0x0001
#define rid_chf_GROUP_RIGHT     0x0002
#define rid_chf_REPLICATE       0x0004    /* Replicate this column header */
#define rid_chf_ABSOLUTE        0x0008    /* WIDTH=N */
#define rid_chf_PERCENT         0x0010    /* WIDTH=N% */
#define rid_chf_RELATIVE	0x0020    /* WIDTH=N* */

/* NB tie-up with above */
#define rid_rgf_THEAD		rid_rhf_THEAD
#define rid_rgf_TFOOT		rid_rhf_TFOOT
#define rid_rgf_TBODY		rid_rhf_TBODY

/*****************************************************************************/

typedef SHORTISH rid_table_flags;

#define rid_tf_COLS_FIXED       0x0001  /* Number of columns now fixed */
#define rid_tf_LINE_START       0x0002  /* Next <TH|TD> is 1st of line */
#define rid_tf_NO_MORE_CELLS    0x0004  /* for entire table */
#define rid_tf_DONE_COLGROUP    0x0008  /* Now into main section */
#define rid_tf_IN_COLGROUP      0x0010  /* Within <COLGROUP> */
#define rid_tf_FULL_BORDER      0x0020  /* Have borders on all 4 sides */
#define rid_tf_GROUP_SPAN       0x0040  /* No <COL> within <COLGROUP SPAN=N> yet */
#define rid_tf_IMPLIED_COLGROUP 0x0080  /* No <COLGROUP> was actually given */
#define rid_tf_3D_BORDERS       0x0100  /* Set=3d borders, Clear=boring lines */
#define rid_tf_COLGROUPSECTION	0x0200	/* */
#define rid_tf_HAVE_REL		0x0400  /* Use column ratios from sizing */
#define rid_tf_HAVE_TFOOT	0x0800  /* <TFOOT> processing required */
#define rid_tf_TFOOT_INVISIBLE	0x1000
#define rid_tf_BGCOLOR     	0x2000  /* One or more cells have BGCOLOR */
#define rid_tf_FINISHED		0x4000  /* Finished - ie seen </TABLE> */

/*****************************************************************************

    This describes the common properties that we frequently inherit or
    possess. In general, an item is more likely not to explicitly declare any
    of these, so we have a seperate structure to save on storage. A
    cell, row, column, row group, column group, caption and table may have an
    associated rid_table_props.

*/

#define rid_PROP_VALIGN         0
#define rid_PROP_HALIGN         1
#define rid_PROP_DIR            2
#define rid_PROP_CH             3
#define rid_PROP_CHOFF          4
#define rid_PROP_LANG           5
#define rid_PROP_STYLE          6
#define rid_PROP_WIDTH          7	/* Pseudo type */
#define rid_PROP_BGCOLOR	8	/* Extension to be this general */

#define NO_BGCOLOR		-1

typedef unsigned char rid_tprop_flags;

struct rid_table_props
{
        rid_valign_tag          valign;
        rid_halign_tag          halign;
        rid_dir_tag             dir;
        char                    ch;
        rid_tprop_flags         flags;
        rid_stdunits            choff;
#if 1
        void                    *lang;
        void                    *style;
#endif
        int			bgcolor;
};

#define rid_tpf_BGCOLOR		0x01

/*****************************************************************************/

struct rid_width_info
{
        int                     minleft;
        int                     minright;
        int                     minwidth;       /* at least minleft + minright */
        int                     maxleft;
        int                     maxright;
        int                     maxwidth;
};

/*****************************************************************************

    Row and column headers and groups may be defined - colgroups
    through use of the <COLGROUP>, rowgroups through <THEAD> <TFOOT>
    and <TBODY>, row headers with <TR> and column headers with <COL> -
    these structures desribes such groups and headers.

*/

/* Stop compiler warnings about C++ reserved word "class" */
#define class class__not

struct rid_table_colgroup
{
        rid_table_props         *props;         /* Associated properties */
        int                     first;          /* write code to init */
        int                     span;           /* How many cols covered */
        char                    *id;
        void                    *class;
        rid_stdunits            userwidth;
};

struct rid_table_colhdr
{
        rid_table_colgroup      *colgroup;      /* write code to init */
        rid_table_props         *props;
        rid_stdunits            userwidth;
        rid_colhdr_flags        flags;
        char                    *id;
        void                    *class;
        rid_width_info          width_info;
        int                     align_pos;      /* for ALIGN=CHAR */
        int                     offx;           /* From left of table */
        int                     sizex;          /* horizontal span */
};

struct rid_table_rowgroup
{
        rid_table_props         *props;         /* Associated properties */
        int                     span;           /* How many rows covered */
        rid_rowgrp_flags	flags;
#if 0
        int			first;		/* write code to init */
#endif
        char                    *id;
        void                    *class;
#if 0
	int			top_offy;	/* From top of table (incl) */
	int			bot_offy;	/* Ditto but exclusive */
#endif
};

struct rid_table_rowhdr
{
        rid_table_rowgroup      *rowgroup;      /* write code to init */
        rid_table_props         *props;
        rid_rowhdr_flags        flags;
        char                    *id;
        void                    *class;
        int                     offy;           /* From top of table */
        int                     sizey;          /* vertical span */
};

/*****************************************************************************

    This structure describes a table. To the outside, a table appears
    much as an image or word, having a rectangular size, resize and
    format methods, etc. Inside it is composed of an optional caption
    and a grid of cells. The size recorded by the rid_text_item
    includes any borders and any caption. The caption is currently
    forced above the table, as if <CAPTION ALIGN=TOP> was also
    forced. The size of the table does not include the caption
    (wbsize, etc). The caption's size, if any, must be investigated
    when required.

    Some items are constant as soon as just the <TABLE> element itself
    has been seen. Some become constant after the <COLGROUP> section
    has finished. Others are valid when the table is created but can
    change as more of the table is parsed.

*/

struct rid_table_item
{
        /* Internal connections */
        rid_text_item_table     *parent;        /* Containing item */
        struct rid_text_item    *prev;		/* for rid_scanb when floating */
        rid_text_stream         *oldstream;
        rid_table_item		*oldtable;
        rid_table_flags         flags;          /* */
        rid_table_caption       *caption;       /* Caption for the table, if any */
        int                     state;          /* tabstate_BLAH value */

        /* Constant after opening <TABLE> has been seen */
        rid_table_props         *props;         /* Alignment of table relative to container */
        char                    *id;
        void                    *class;
        rid_frame_tag           frame;          /* Which part of table frame to include */
        rid_rules_tag           rules;          /* Which rules to draw between cells */
        rid_stdunits            userborder;     /* Width of border framing table. */
        rid_stdunits            userwidth;      /* Author's width of table, relative to container */
        rid_stdunits            usercellspacing;/* Back-compat. */
        rid_stdunits            usercellpadding;/* Back-compat. */

        int                     cellpadding;    /* how much padding around contents */
        int                     cellspacing;    /* how much padding outside cell border */
	int			border;		/* width of table frame */

        /* Change during table arrival. Always at least initialised */
        int                     header_rows;    /* Number of rows in the header */
        int                     footer_rows;    /* Number of rows in the footer */
        intxy                   num_groups;     /* Number of row/col groups */
        rid_table_cell          **array;        /* Ptrs to cell(s) if any in table (hdr, ftr, bodies) */
        rid_table_rowhdr        **rowhdrs;      /* List of properties, one per row */
        rid_table_colhdr        **colhdrs;      /* List of properties, one per col */
        rid_table_rowgroup      **rowgroups;    /* List of properties, one per row group */
        rid_table_colgroup      **colgroups;    /* List of properties, one per col group */
        intxy                   cells;          /* Size of table in cells */
        intxy                   size;           /* Including all borders, etc */

        /* Used during sizing and formatting */
        rid_width_info          width_info;     /* Got by examining all child text streams */
	int			offy;		/* so can move up/down easier */

        /* Scaffolding for use during construction */
        intxy                   scaff;
        rid_table_colgroup      *cur_colgroup;
        rid_table_rowgroup      *cur_rowgroup;
        rid_table_colhdr        *cur_colhdr;
        rid_table_rowhdr        *cur_rowhdr;

	int			colpct_spare;
	int *minabs, *maxabs, *minpct, *maxpct, *cumminabs, *cummaxabs;
    	int			min_cand;
    	int			max_cand;
};


#define CELLFOR(table, xx, yy)    ( & (table)->array[ (xx) + (yy) * (table)->cells.x ] )

/*****************************************************************************/

typedef enum
{
    rid_fmt_idle/*,
		 */
} rid_fmt_enum;

#define rid_MAX_ALIGNS		20

struct rid_fmt_state
{
    struct rid_text_item *	ti;
    rid_fmt_enum	state;
    intxy	        left_nest[rid_MAX_ALIGNS];
    intxy	        right_nest[rid_MAX_ALIGNS];
    int			left_sp;
    int			right_sp;
};


/* Parent type */

#define rid_pt_HEADER           0       /* rid_header */
#define rid_pt_CAPTION          1       /* rid_table_caption */
#define rid_pt_CELL             2       /* rid_table_cell */

/* pos_fvpr and text_fvpr only for top level stream */
struct rid_text_stream {
    rid_pos_item *pos_list,	/* First in list */
	*pos_last;		/* Last in list */
#if 0
	*pos_fvpr;		/* FVPR ptr */
#endif
    rid_text_item *text_list,	/* First in list */
	*text_last,		/* Last in list */
	*text_fvpr;		/* FVPR ptr */
    void *parent;               /* The object in which this text flows, if any */
    int partype;                /* Type of parent we are in */
    int fwidth;                 /* Width to format to */
    int width;                  /* Width of formatted text */
    int widest;                 /* The widest single item in the stream */
    int height;                 /* The height of the formatted text */
    rid_width_info width_info;  /* Describes stream; used when doing tables*/
    rid_fmt_state *fmt_state;	/* Freed after use */
};

typedef int rid_header_flags;
#define rid_hf_ISINDEX          0x01   /* The document is an index         */
#define rid_hf_HTML_ERRS        0x02   /* The document has errors in the source */
#define rid_hf_FULL_REFORMAT	0x04   /* No progressive formatting */

typedef int rid_bgt;
#define rid_bgt_COLOURS 0x01    /* We have a background colour */
#define rid_bgt_IMAGE   0x02    /* We have a background tiled image */
#define rid_bgt_FCOL    0x04    /* We have a foreground colour */
#define rid_bgt_LCOL    0x08    /* We have a link colour */
#define rid_bgt_VCOL    0x10    /* We have a visited colour */
#define rid_bgt_ACOL    0x20    /* We have a active colour */
#define rid_bgt_BASEFONT        0x40    /* The base font size has been set */

#define rid_EXTRACOLOURS    16  /* Number of font colours per page supported */

typedef struct rid_header {
    rid_text_stream stream;
    rid_text_stream *curstream;         /* For table nesting */
    rid_aref_item *aref_list, *aref_last;
    rid_form_item *form_list, *form_last;
    memzone texts;
    rid_header_flags flags;
    char *title;
    char *base;
    rid_bgt bgt;
    struct {
        int back;
        int fore;
        int link;
        int vlink;
        int alink;
    } colours;
    rid_tile tile;
    int basefont;               /* Base font size (1-7) */
    int refreshtime;
    char *refreshurl;

    rid_frame *frames;
    int nframes;

    rid_map_item *map_list;

    rid_meta_item *meta_list;

    struct
    {
	int left;
	int top;
    } margin;

    int full_format_nest;

    int extracolours;
    int extracolourarray[rid_EXTRACOLOURS];

#ifdef BUILDERS
    int cwidth;			/* Width of monospaced text font */
#endif

} rid_header;

typedef struct {
    rid_text_item base;
    int data_off;
} rid_text_item_text;


/*****************************************************************************/

struct rid_table_caption
{
        rid_text_stream         stream;         /* Text stream of caption */
        rid_table_item          *table;         /* WRITE CODE TO INITIALISE */
        rid_calign_tag          calign;         /* rid_ct_TOP is what's done though */
        char                    *id;
        void                    *class;
        rid_table_props         *props;         /* Not all apply, but close enough */
        intxy                   off;            /* Offset from top,left of table */
        intxy                   size;           /* */
};

/*****************************************************************************

    One of these per cell within a table - one of these can span multiple
    items in the regular grid overlayed on the table.  To the contents, we
    wish to appear as a containing object with a specific width.  The
    rid_text_stream does not contain any rid_text_item pointers (either
    forwards or backwards) that point out (or up) of this cell to either neighbouring
    cells or the enclosing table or beyond. This leads to numerous chains
    that need to be actively linked together in order to scan all
    rid_text_items for a document.

*/

struct rid_table_cell
{
        rid_text_stream         stream;         /* The contents of the cell */
        rid_table_item          *parent;        /* Table this cell is in */
        rid_table_props         *props;         /* Any properties recorded with this cell */
        char                    *id;
        char                    *axes;
        char                    *axis;
        void                    *class;
        rid_cell_flags          flags;          /* eg NOWRAP flag */
        intxy                   cell;           /* The root cell */
        intxy                   span;           /* Span in cells */
        int                     sleft;          /* Span left to grow - stop when zero */
        intxy                   size;           /* Cell's size in pixels - incl borders */
        VALUE                   userwidth;      /* Support for Netvirus */
        VALUE                   userheight;     /* Support for Netvirus */
};

#define rid_bullet_ol_1 1
#define rid_bullet_ol_A 2
#define rid_bullet_ol_I 3
#define rid_bullet_ol_a 4
#define rid_bullet_ol_i 5
#define rid_bullet_ol_STRING    "1AIai"

typedef struct {
    rid_text_item base;
    int list_type;                  /* HTML_OL, UL, MENU or DIR */
    int item_type;                  /* depends on list type */
    int list_no;
} rid_text_item_bullet;

typedef struct {
    rid_text_item base;
    struct rid_text_item *prev; /* for rid_scanb when floating */
    char *src;                  /* Location of image */
    char *alt;                  /* Text to use in place */
    rid_image_flags flags;
    int bwidth;                 /* border width */
    int ww, hh;                 /* Width and height if given by the author */
    void *im;
    char *usemap;               /* client side image map */
    int hspace, vspace;         /* gutters */
    struct
    {
        struct
        {
            rid_area_item *selection;
        } usemap;
    } data;
} rid_text_item_image;

typedef struct {
    rid_text_item base;
    rid_input_item *input;
} rid_text_item_input;

typedef struct {
    rid_text_item base;
    rid_select_item *select;
} rid_text_item_select;

typedef struct {
    rid_text_item base;
    rid_textarea_item *area;
} rid_text_item_textarea;

struct rid_text_item_table {
   rid_text_item base;
   rid_table_item *table;
} ;

typedef struct {
   rid_text_item base;
} rid_text_item_pad;

typedef struct {
   rid_text_item base;
   SHORTISH size;
   char noshade;
   char align;
   VALUE width;
} rid_text_item_hr;

struct rid_text_item_object
{
    rid_text_item base;
    rid_object_item *object;
};

/* ------------------------------------------------ */

typedef int rid_object_flags;

#define rid_object_flag_DECLARE	0x01
#define rid_object_flag_SHAPES	0x02

/* Special valuetype values for pseudo params */
#define rid_object_param_OBJECT_DATA	(-1)
#define rid_object_param_OBJECT_URL	(-2)

struct rid_object_param
{
    rid_object_param *next;
    char *name;
    char *value;
    int valuetype;
    char *type;
};

typedef enum
{
    rid_object_type_IMAGE,	/* OBJECT can be handled by IMG code */
    rid_object_type_INTERNAL,	/* OBJECT is handled internally not by IMG code */
    rid_object_type_FRAME,	/* OBJECT will bae handled by frame code */
    rid_object_type_PLUGIN,	/* OBJECT wil be handled by a plugin */
    rid_object_type_UNKNOWN	/* OBJECT cannot be handled */
} rid_object_type;

struct rid_object_item
{
    int element;		/* Original element tag (EMBED,APPLET,OBJECT) */
    rid_object_type type;

    char *classid;
    char *classid_mime_type;
    int classid_ftype;

    char *data;
    char *data_mime_type;
    int data_ftype;

    char *codebase;
    char *usemap;

    char *standby;              /* Text to use in place */
    char *name;

    char *id;

    /* Sizes passed in from HTML */
    rid_stdunits userborder;			/* border width */
    rid_stdunits userwidth, userheight;         /* Width and height if given by the author */
    rid_stdunits userhspace, uservspace;        /* gutters */

    /* Calculated sizes */
    int bwidth, ww, hh, hspace, vspace;		/* in OS units */

    rid_object_flags oflags;
    rid_image_flags iflags;			/* for image alignment */

    rid_object_param *params;			/* list of extra parameters */

    rid_map_item *map;				/* If we have shapes then we create a private map here */

    union
    {
	struct
	{
	    void *pp;
	    int box[4];
	} plugin;
	struct
	{
	    void *im;
	} image;
    } state;

    rid_text_item *text_item;
};

/* ------------------------------------------------ */

typedef char rid_frame_scrolling;

#define rid_scrolling_AUTO      0
#define rid_scrolling_YES       1
#define rid_scrolling_NO        2

/*
 * A frame. If frameset != NULL then it points to a nested frameset and the rest of
 * the frame information is invalid, only the width and height are used.
 */

#define rid_frame_tag_FRAMESET  0
#define rid_frame_tag_FRAME     1

struct rid_frame_unit_totals
{
        float                   mult, pcent, px;
};

struct rid_frame_item
{
        char                    *src;           /* url of frame contents */
        char                    *name;		/* name for TARGET's */
        int                     marginwidth;    /* pixels */
        int                     marginheight;   /* pixels */
        rid_frame_scrolling     scrolling;	/* char, show scroll bars? */
        char                    noresize;       /* boolean, can we resize */
};

struct rid_frameset_item
{
        int                     ncols, nrows;
        rid_stdunits            *widths, *heights;  /* only px,pc,mult allowed */
        rid_frame_unit_totals   width_totals, height_totals;
        rid_frame               *frame_list, *frame_last;
        int                     bwidth;             /* pixel width */
	rid_frame               *old_frameset;
};

struct rid_frame
{
        rid_frame               *next;
        int                     tag;

        char                    border;             /* boolean, display border on frame or frames within frameset */
        int                     bordercolour;       /* standard colour for border region */

        union
        {
            rid_frame_item      frame;
            rid_frameset_item   frameset;
        } data;
};

/* ------------------------------------------------ */

typedef char rid_area_type;

#define rid_area_RECT       0
#define rid_area_CIRCLE     1
#define rid_area_POLYGON    2
#define rid_area_DEFAULT    3
#define rid_area_POINT      4

struct rid_area_item
{
        rid_area_item           *next;
        char                    *alt;
        char                    *href;
        char                    *target;
        rid_area_type           type;
        union
        {
            struct
            {
                int x0, y0, x1, y1;
            } rect;
            struct
            {
                int x, y, r;
            } circle;
            struct
            {
                intxy *point;
                int npoints;
            } polygon;
            intxy point;
            /* default type has no coords */
        } coords;
};

typedef int rid_map_flags;

#define rid_map_ERROR   0x01    /* tried to download map but it gave an error of somekind */

struct rid_map_item
{
        rid_map_item            *next;
        rid_map_flags           flags;

        rid_area_item           *area_list, *area_last;
        char                    *name;
};

/*****************************************************************************/

struct rid_meta_item
{
        rid_meta_item           *next;
        char                    *name;
        char                    *content;
        char                    *httpequiv;
};
/*****************************************************************************/

#define rid_fmt_MIN_WIDTH       0x00001
#define rid_fmt_MAX_WIDTH       0x00002
#define rid_fmt_BUILD_POS       0x00004
#define rid_fmt_CHAR_ALIGN      0x00008
/*#define rid_fmt_STOMP_WIDTHS	0x00010*/
#define rid_fmt_HAD_ALIGN       0x10000

struct rid_fmt_info
{
        int  (*margin_proc) (rid_text_stream *stream, rid_text_item *item);
        int  (*tidy_proc)   (rid_header *rh, rid_pos_item *new_pos, int width, int display_width);
        void (*table_proc)  (rid_header *rh, rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt);
        char align_char;
        int *left, *right, *width;
        char *text_data;
	int scale_value;	/* percentage scale value from autofit routine */
};

/*****************************************************************************/

#ifdef BUILDERS

typedef struct
{
    intxy		size;
    char *		text;

} textplot;

#endif

/*****************************************************************************

    Scanning the list of items forming a document is complicated by the
    nesting introduced for tables.  Rather than direct examination, all
    traversal must now go through rid_scan() or antweb_apply().  Some
    optimising macros wrap rid_scan() for the typical cases.

    rid_scan() will, by default, return the next item, neither entering
    and not leaving tables.  The action pack permits different behaviour.
    SCAN_BACK causes the scan to go backwards (ie using prev pointers rather
    than next pointers).  SCAN_UP causes the scan to continue in any
    enclosing table once the end of the current stream has been met.
    SCAN_DOWN causes tables to be entered.  A table item is never returned if
    the SCAN_DOWN flag is included.  If SCAN_FILTER is included, then a
    rid_tag should also be included.  Only items with a matching type are
    then returned.  SCAN_EXCLUDE causes negation on the SCAN_FILTER criteria.
    Filtering does not override the SCAN_DOWN exclusion of tables.  NULL is
    returned when another suitable item cannot be found (eg end of chain).

    SCAN_RECURSE replaces SCAN_DOWN and SCAN_UP - you might be able to go up
    and down independently, but not at present.

    antweb_apply() performs the same traversal as rid_scan(), except it
    then takes the supplied function and invokes it on each item that
    rid_scan() would have successively yielded.  Scanning is terminated if
    the user function returns non-NULL.  antweb_apply() returns NULL to
    indicate it reached the end of the list, or a non-NULL value returned by
    the callers function.  Note that the use of antweb_apply() is drastically
    restricted because we cannot have nested function definitions with
    context shared with their parent.  Sigh.

    Four macros perform the frequent operations.  rid_scanf() scans
    forward without recursion, rid_scanb() scans backwards without
    recursion, rid_scanfr() scans forward with recursion and
    rid_scanbr() scans backwards with recursion.  The forward scanning
    macros perform some optimisation and attempt to avoid the overhead of a
    function call.

*/

#define SCAN_FWD        0x0000          /* Cosmetic - the default */
#define SCAN_BACK       0x0100          /* Check rid_scan() before */
#define SCAN_DOWN       0x0200          /* changing these. */
#define SCAN_UP         0x0400
#define SCAN_EXCLUDE    0x0800
#define SCAN_FILTER     0x1000
#define SCAN_THIS	0x2000		/* Consider the item passed in first */
#define SCAN_RECURSE    (SCAN_DOWN | SCAN_UP)

/*typedef void * (*b_apply_fn) (be_item item, be_doc doc);*/

#define rid_scanb(ti)        rid_scan((ti), SCAN_BACK)
#define rid_scanbr(ti)       rid_scan((ti), SCAN_BACK | SCAN_RECURSE)
#define rid_scanf(ti)        ( (ti) ? (ti)->next : NULL )
#define rid_scanfr(ti)       (                                                          \
                                        (                                               \
                                                (ti) &&                                 \
                                                (ti)->tag != rid_tag_TABLE &&           \
                                                (ti)->next                              \
                                        )                                               \
                                        ? (ti)->next                                    \
                                        : rid_scan( (ti), SCAN_FWD | SCAN_RECURSE )     \
                                )                                                       /**/

/* Function prototypes for rid.c */

/*extern void nullfree(void **vpp);*/
extern void rid_free_object(rid_object_item *obj);
extern void rid_free_pos(rid_pos_item *p);
extern void rid_free_pos_tree(rid_pos_item *p);
extern void rid_free_text(rid_text_item *p);
extern void rid_free_aref(rid_aref_item *p);
extern void rid_free_input(rid_input_item *p);
extern void rid_free_textarea_lines(rid_textarea_line *p);
extern void rid_free_textarea(rid_textarea_item *p);
extern void rid_free_select_options(rid_option_item *p);
extern void rid_free_selects(rid_select_item *p);
extern void rid_free_form(rid_form_item *p);
extern void rid_free_props(rid_table_props *props);
extern void rid_free_colgroup(rid_table_colgroup *colgroup);
extern void rid_free_colhdr(rid_table_colhdr *colhdr);
extern void rid_free_rowgroup(rid_table_rowgroup *rowgroup);
extern void rid_free_rowhdr(rid_table_rowhdr *rowhdr);
extern void rid_free_caption(rid_table_caption *caption);
extern void rid_free_table(rid_table_item *table);
extern void rid_free(rid_header *rh);
extern void rid_frame_connect(rid_frame *f_top, rid_frame *f);
extern void rid_area_item_connect(rid_map_item *m, rid_area_item *a);
extern void rid_text_item_connect(rid_text_stream *st, rid_text_item *t);
extern void rid_pos_item_connect(rid_text_stream *st, rid_pos_item *p);
extern void rid_aref_item_connect(rid_header *rh, rid_aref_item *a);
extern void rid_form_item_connect(rid_header *rh, rid_form_item *f);
extern void rid_form_element_connect(rid_form_item *f, rid_form_element *i);
extern void rid_option_item_connect(rid_select_item *s, rid_option_item *o);
extern void rid_meta_connect(rid_header *rh, rid_meta_item *m);
extern int memzone_init(memzone *mz, int flags);
extern int memzone_alloc(memzone *mz, int size);
extern int memzone_tidy(memzone *mz);
extern void memzone_destroy(memzone *mz);
extern rid_text_item *rid_outermost_item(rid_text_item *item);
extern rid_text_item * rid_scan(rid_text_item * item, int action);
extern void rid_zero_widest_height_from_item(rid_text_item *item);
extern void rid_zero_widest_height(rid_text_stream *stream);
extern rid_pos_item *rid_clone_pos_list(rid_pos_item *pos);

/* Some useful form scanning functions */

typedef void (*form_element_enumerate_fn)(rid_form_element *fe, void *handle);

#define form_element_enumerate_ALL	((const char *)-1)

extern char *form_get_element_name(rid_form_element *fe);
extern void form_element_enumerate(rid_form_item *form, int element_type, int tag, const char *name, form_element_enumerate_fn fn, void *handle);

extern void rid_free_form_elements(rid_form_element *f);

#endif /* __rid_h */

