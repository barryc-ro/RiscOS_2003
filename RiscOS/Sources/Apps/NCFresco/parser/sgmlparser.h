/* sgmlparser.h - SGML parser - tailored for HTML */
/* (C) Copyright ANT Limited 1995. All rights reserved. */
/* Author: Borris */

#ifndef __sgmlparser_h
#define __sgmlparser_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#include <math.h>
#include "debug.h"

#ifndef __version_h
#include "version.h"
#endif

/* SGML client provides a few simple values. */
#include "glue.h"

#include "htmldefs.h"

#if UNICODE
#include "encoding.h"
#endif

/* Various configuration type values */

#define default_inhand_size		256
#define single_quote			'\''
#define double_quote			'\"'
#define MAXSTRING			4096
#define TRUE				1
#define FALSE				0
#define SGML_NO_ATTRIBUTE               -1
#define SGML_NO_ELEMENT                 -1

#ifndef min
#define min(a,b)			((a) < (b) ? (a) : (b))
#endif

/* If compiling for Fresco, don't override these */
#ifdef STDALONE
#define mm_calloc			calloc
#define mm_malloc			malloc
#define mm_realloc			realloc
#define mm_free				free
#define ASSERT(x)                       assert(x)
#else
#include "memwatch.h"
#include "myassert.h"
#endif


enum { mode_filter, mode_browser };

/* Attribute value parsing techniques */

enum
{
    PARSE_VOID,			/* No value expected */
    PARSE_ENUM,			/* Value must be one of supplied enumeration */
    PARSE_ENUM_VOID,		/* Value is enumerated or not supplied */
    PARSE_STRING,		/* Value is parsed as a string */
    PARSE_INTEGER,		/* Value is parsed as an integer */
    PARSE_STDUNIT,		/* Value is one of standard size units */
    PARSE_STRING_VOID,		/* Value is string or empty */
    PARSE_ENUM_TUPLE,		/* Value is enum or colour tuple */
    PARSE_INTEGER_VOID,
    PARSE_STDUNIT_VOID,
    PARSE_STDUNIT_LIST,		/* For coords */
    PARSE_ENUM_STRING,
    PARSE_BOOL,
    PARSE_COLOUR

#if 0
    PARSE_COLOUR_TUPLE,		/* Value is #abcdef colour tuple */
#endif

};

/* Types of a value associated with an attribute */

enum
{
    value_none,
    value_integer,
    value_tuple,
    value_string,
    value_enum,			/* ** REMEMBER: STARTING AT ONE ** */
    value_void,
    value_absunit,
    value_relunit,
    value_pcunit,
    value_stdunit_list,
    value_bool,			/* 0 or 1 */
    value_colour		/* always returns tuple, recognises X colours */
};

/* Modes of recognition the parser may be in. */

#define match_end_tag		0x01
#define match_unexpected	0x02
#define match_perm_text		0x03

#if 0
static char *match_names[] =
{
    "match_end_tag_only",
    "match_pre_tags_only",
    "match_all_tags",
    "match_markup_only",
    "match_no_block_elements",
    "match_restricted_markup"
};
#endif

/* Commands applied to an element number to describe relations */
/* Please keep debugging list in elements.c upto date as well. */
enum
{
    ONE_OF_ENCLOSING = 0,
    LAST_ONE_OF_ENCLOSING = 1,
    ENCLOSED_WITHIN = 2,
    PRECEEDED_BY = 3,
    CLOSE_ANY_OPEN = 4,
    POST_CLEAR_SEEN_FLAG = 5,
    SHOULD_CONTAIN = 6,
    QUIETLY_CLOSE_ANY_OPEN = 7,
    CONTAINER_ENCLOSED_WITHIN = 8,
    INVENT_ONLY_WITHIN = 9,
    REPLACE_WITH = 10,
    REPLACES_BAD = 11
};

/* Flags for an element */

#define FLAG_NONE                       0X00000000
#define FLAG_CONTAINER                  0X00000001
#define FLAG_CONTENT                    0X00000002
#define FLAG_ONLY_ONCE                  0X00000004
#define FLAG_NO_CLOSE                   0X00000008
#define FLAG_ARTIFICIAL                 0X00000010
#define FLAG_NOT_NESTABLE               0X00000020
#define FLAG_SILENT_FIX                 0X00000040
#define FLAG_ANTIQUATED                 0X00000080
#define FLAG_CLOSE_OPTIONAL             0X00000100
#define FLAG_STRIP_SPACE_PRE            0X00000200
#define FLAG_STRIP_SPACE_POST           0X00000400
#define FLAG_PRE_BREAK          	0X00000800
#define FLAG_POST_BREAK                 0X00001000
#define FLAG_TEXT               	0X00002000
#define FLAG_NO_TEXT               	0X00004000
#define FLAG_END_TAG               	0X00008000
#define FLAG_HEAD               	0X00010000
#define FLAG_HEADING_ITEM              	0X00020000
#define FLAG_BLOCK_LEVEL               	0X00040000
#define FLAG_OUT_OF_ORDER_CLOSE         0X00080000
#define FLAG_NO_HEADINGS_OR_ADDRESS     0X00100000
#define FLAG_NO_BLOCK_ELEMENTS          0X00200000
#define FLAG_CLOSE_INTERNAL		0X00400000
#define FLAG_AUTO_CLOSE			0X00800000
#define FLAG_FULL_UNWIND		0X01000000
#define FLAG_MANUAL_BREAK		0X02000000
#define FLAG_DONT_FORCE_CLOSE	0X04000000

#define FLAG_BLOCK			(FLAG_BLOCK_LEVEL)
#define FLAG_HEADING               	(FLAG_HEADING_ITEM | FLAG_BLOCK)
#define FLAG_STRIP_SPACE		(FLAG_STRIP_SPACE_PRE | FLAG_STRIP_SPACE_POST)
#define FLAG_BREAKS			(FLAG_PRE_BREAK | FLAG_POST_BREAK)

#if 0
#define    FLAG_NO_BLOCK_ELEMENTS_MODE         0X00000200
#define    FLAG_CAUSES_PRE_MODE                0X00000400
#define    FLAG_CAUSES_END_TAG_MODE            0X00000800
#define    FLAG_CAUSES_NO_TEXT_MODE            0X00001000
#define    FLAG_CAUSES_ALL_MARKUP_MODE         0X00002000
#define    FLAG_RESTRICTED_MARKUP_MODE         0X00004000
#define    FLAG_VERTICALLY_STACKING            0X00010000
/*#define    FLAG_STRIP_SPACE                    0X00020000*/
#endif

/* Forward declarations */

#ifndef BOOL
typedef int				BOOL;
#endif
typedef unsigned int			BITS;
typedef struct EBLOCK                   EBLOCK;
typedef struct MBLOCK                   MBLOCK;
typedef struct STRING                   STRING;
typedef struct ATTRIBUTE		ATTRIBUTE;
typedef struct ELEMENT			ELEMENT;
typedef struct SGMLCTX			SGMLCTX;
typedef struct STRING_LIST		STRING_LIST;
typedef struct PARSER_CONFIG		PARSER_CONFIG;
typedef struct REPORT			REPORT;
typedef struct ACT_ELEM			ACT_ELEM;
typedef struct BUFFER                   BUFFER;
typedef struct VALUE                    VALUE;
typedef struct VALUES                   VALUES;
typedef struct STACK_ITEM		STACK_ITEM;
typedef struct STDUNIT_LIST		STDUNIT_LIST;
typedef struct sgml_chopper_state	sgml_chopper_state;
typedef struct USTRING			USTRING;
typedef struct UBUFFER			UBUFFER;
#if UNICODE
typedef unsigned short			UCHARACTER;
#else
typedef char				UCHARACTER;
#endif

struct EBLOCK { void *ptr; int elts; }; /* Elements, eg int, struct */
struct MBLOCK { char *ptr; int size; }; /* Memory, bytes */
struct STRING { char *ptr; int nchars; }; /* eg strings */
struct STRING_LIST { STRING string; STRING_LIST *prev, *next; };
/*struct PARSER_CONFIG { BOOL print_unexpected_characters, print_sgml; };*/
struct BUFFER { char *data; int max, ix; };     /* ix is next to use */
struct ACT_ELEM { int action, element; };
struct STDUNIT_LIST { int num; VALUE *items; };
struct UBUFFER { UCHARACTER *data; int max, ix; };	/* This must be identical to BUFFER except for data type or else free_buffer won't work */
struct USTRING { UCHARACTER *ptr; int nchars; };		/* nchars is actually the number of elements, *NOT* the number of bytes */

/*****************************************************************************

  Enumeration values are based at one, so zero is available to
  indicate an item not present in an enumeration. Keeping the value
  the same eases coding. Negative values are avoided due to the
  complications associated with differing sized storage at different
  stages.

*/

struct VALUE
{
    int type;
    union
    {
	int i;          /* value_int, value_enum */
	BITS b;         /* value_tuple */
	STRING s;       /* value_string */
	double f;       /* value_absunit, value_relunit */
	STDUNIT_LIST l; /* value_stdunit_list */
    } u;
    int tag;                /* eg for order of arrival */
};

struct VALUES { VALUE value[SGML_MAXIMUM_ATTRIBUTES]; };

struct REPORT
{
    BOOL has_tables, has_forms, has_frames, has_scripts, has_address;

    int
    antiquated_features,            /* Such as <XMP> */
        badly_formed_comments,          /* Such as <!-- > */
        unexpected_characters,          /* Such as Wibble<HTML> */
        stack_overflows,                /* Might not be appropriate */
        stack_underflows;               /* in the long run? */

    int
    good_opens[NUMBER_SGML_ELEMENTS],
        good_closes[NUMBER_SGML_ELEMENTS],
        implied_opens[NUMBER_SGML_ELEMENTS],                  /* Such as <TD> */
        implied_closes[NUMBER_SGML_ELEMENTS],                 /* Such as <LI> */
        missing_opens[NUMBER_SGML_ELEMENTS],                  /* Such as <CAPTION> */
        missing_closes[NUMBER_SGML_ELEMENTS],                 /* Such as <CAPTION> */
        close_without_opens[NUMBER_SGML_ELEMENTS],            /* Such as <ZL> </OL> */
        had_unknown_attributes[NUMBER_SGML_ELEMENTS],         /* Such as <A WIBBLE> */
        had_bad_attributes[NUMBER_SGML_ELEMENTS],             /* Such as <CELL ALIGN=WIBBLE> */
        not_repeatables[NUMBER_SGML_ELEMENTS];                /* Such as <BODY> <BODY> */

    STRING_LIST
    * unknown_elements,             /* Such as <ZL> */
        * unknown_attributes,           /* Such as <A WIBBLE> */
        * bad_attributes,               /* Such as <CELL ALIGN=WIBBLE> */
        * messages;

    FILE
        *output;
};

struct STACK_ITEM
{
    int element, matching_mode;
    BITS elements_open[words_of_elements_bitpack],
	 elements_seen[words_of_elements_bitpack];
    BITS effects_active[words_of_effects_bitpack];
    BITS effects_applied[words_of_effects_bitpack];
    STACK_ITEM *inner, *outer;
};


struct ATTRIBUTE
{
    STRING name;
    STRING *templates;
    int parse;
};



struct ELEMENT
{
    STRING name;
    int id, group;
    BITS flags;

    ATTRIBUTE **attributes;	/* Empty name terminates list */
    ACT_ELEM *requirements;

    /* When element_open is called, tos references the stacked */
    /* element with the id of the element being opened. The style */
    /* information has been copied from the context - if no changes */
    /* are made to this, then the element has no effect on the style */
    void (*element_open) (SGMLCTX *ctx, ELEMENT *elem, VALUES *values);
    void (*element_close) (SGMLCTX *ctx, ELEMENT *elem);
};

typedef void (*state_fn)(SGMLCTX *, UCHARACTER) ;

typedef void (*sgml_deliver_fn) (SGMLCTX *, int, STRING, ELEMENT *);

typedef struct sgml_deliver_list { sgml_deliver_fn this_fn; struct sgml_deliver_list *previous; } sgml_deliver_list;

/*****************************************************************************/

typedef struct
{
    void (* note_good_open) (SGMLCTX * context, ELEMENT * element);
    void (* note_good_close) (SGMLCTX * context, ELEMENT * element);
    void (* note_implied_open) (SGMLCTX * context, ELEMENT * element);
    void (* note_implied_close) (SGMLCTX * context, ELEMENT * element);
    void (* note_missing_open) (SGMLCTX * context, ELEMENT * element);
    void (* note_missing_close) (SGMLCTX * context, ELEMENT * element);
    void (* note_close_without_open) (SGMLCTX * context, ELEMENT * element);
    void (* note_had_unknown_attribute) (SGMLCTX * context, ELEMENT * element);
    void (* note_had_bad_attribute) (SGMLCTX * context, ELEMENT * element);
    void (* note_not_repeatable) (SGMLCTX * context, ELEMENT * element);
    void (* note_antiquated_feature) (SGMLCTX * context);
    void (* note_badly_formed_comment) (SGMLCTX * context);
    void (* note_stack_overflow) (SGMLCTX * context);
    void (* note_stack_underflow) (SGMLCTX * context);
    void (* note_unexpected_character) (SGMLCTX * context);
    void (* note_has_tables) (SGMLCTX * context);
    void (* note_has_forms) (SGMLCTX * context);
    void (* note_has_scripts) (SGMLCTX * context);
    void (* note_has_frames) (SGMLCTX * context);
    void (* note_has_address) (SGMLCTX * context);
    void (* note_unknown_element) (SGMLCTX * context, const char *fmt, va_list list) ;
    void (* note_unknown_attribute) (SGMLCTX * context, const char *fmt, va_list list) ;
    void (* note_bad_attribute) (SGMLCTX * context, const char *fmt, va_list list);
    void (* note_message) (SGMLCTX * context, const char *fmt, va_list list );
    void (* reset_report) (SGMLCTX * context);
    void (* pre_open) (SGMLCTX *context, ELEMENT *element);
} SGMLBACK;

/*****************************************************************************

In order that the SGML parser may be called recursively, care is
needed to ensure that characters are still processed in the desired
order. Markup is recognised on the trailling > character, so further
character's won't have been fed and buffered. The only other real
concern is for items buffered somewhere along the line. The chopper
state is flushed by the SGML parser before calling the deliver routine
for any markup. It SGML client must ensure than any state recorded via
clictx is also flushed. For HTML and Fresco, this means ensuring
inhand_string has been flushed via text_item_push_word() where
necessary.

FIXME: someone has to free the attributes array data if recursion is
to happen.

*/

#define SGML_MAGIC		(0xf1d0deadu)

struct sgml_chopper_state
{
    char s1,			/* Chopper state machine */
	s2,			/* EOL state machine */
	s3,			/* spare */
	s4;			/* spare */
};


struct SGMLCTX
{
    int magic;

    unsigned int apply_heuristics:1,
	force_deliver:1,	/* if (->dlist != NULL) */
	applying_rules:1,
	threaded:1,		/* inside the 'data' function */
	pending_close:1,	/* close called whilst threaded */
	strip_initial_nl:1,	/* strip newline immediately after start tag */
	strip_initial_cr:1,	/* strip cr immediately after start tag */
	strip_final_nl:1,	/* strip newline immediately before end tag  */
	strip_final_cr:1,	/* strip cr immediately before end tag  */
	:0;			/* placeholder */
    
    /* Context operating within */

    ELEMENT *elements;
    int mode;
    void *clictx;           /* Client context, eg HTMLCTX */

    /* Lexer Context */

    STACK_ITEM *tos;

    /* Tokeniser context */

    int line;
    UBUFFER inhand;
    UBUFFER prechop;
    int comment_anchor; /* DL for bad comment recovery */
    state_fn state;

    /* Client callback routines */

    sgml_chopper_state chopper_state;

    void (*chopper) (SGMLCTX *, USTRING);

    sgml_deliver_list *dlist;
    sgml_deliver_fn deliver;
/*     void (*deliver) (SGMLCTX *, int, STRING, ELEMENT *); */

    STRING unexpected_string;

    SGMLBACK callback;
    REPORT report;

    BITS dont_stack_elements_open[words_of_elements_bitpack];

#if UNICODE
    int enc_num;
    Encoding *encoding;

    int enc_num_write;
    Encoding *encoding_write;

    int pending_enc_num;

    int encoding_threaded;

    struct
    {
	int enc_num;
	int state;
	BUFFER inhand;
    } autodetect;
#endif
};



/* See also sgmlexp.h and chopper.c */
#define DELIVER_NOP             	0
#define DELIVER_WORD            	1       /* A textual word */
#define DELIVER_SPACE           	2       /* Non end-of-line whitespace */
#define DELIVER_PRE_OPEN_MARKUP		3       /* Some markup parsed - before *fn */
#define DELIVER_POST_OPEN_MARKUP     	4       /* Some markup parsed - after  *fn */
#define DELIVER_PRE_CLOSE_MARKUP	5       /* Some markup parsed - before *fn */
#define DELIVER_POST_CLOSE_MARKUP	6       /* Some markup parsed - after  *fn */
#define DELIVER_UNEXPECTED      	7       /* Unexpected characters */
#define DELIVER_SGML            	8       /* General <SGML> */
#define DELIVER_EOL             	9       /* End of line */
#define DELIVER_EOS             	10      /* End of stream */
#define DELIVER_WORD_NOBREAK           	11      /* A textual word, non breaking with previous */

#define NUM_DELIVER_METHODS     	12

#include "sgmlexp.h"    /* Parser exports */
#include "sgmlimp.h"    /* Parser imports */
#include "sgmlloc.h"    /* Parser local externals */
#include "htmleprocs.h"   /* From SGML client application via attrgen.py */


#endif /* __sgmlparser_h */

/* eof sgmlparser.h */
