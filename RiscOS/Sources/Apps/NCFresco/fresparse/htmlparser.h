/* htmlparser.h - HTML context structure to go with SGMLCTX */

#ifndef __htmlparser_h
#define __htmlparser_h

#ifndef __sgmlparser_h
#include "sgmlparser.h"
#endif

#if !defined( __rid_h ) && !defined(STDALONE)
#include "rid.h"
#endif

#ifndef __htmldefs_h
#include "htmldefs.h"
#endif

#ifndef __htmleprocs_h
#include "htmleprocs.h"
#endif

#ifndef __reports_h
#include "reports.h"
#endif

#define MAX_TITLE_LEN   128     /* The HTML spec says 80 is about right */

/* How we are handling spaces */
#define HTMLMODE_FMT        0  /* Space and EOL are just seperators */
#define HTMLMODE_PRE        1  /* Space and EOL count one for one */
#define HTMLMODE_STR        2  /* Accumulate into the inhand string */
#define HTMLMODE_BOGUS      3
#define NUM_HTMLMODES       3  /* see last_mode field of HTMLCTX */

#define WITH_SPACE    	TRUE
#define WITHOUT_SPACE   FALSE

/* This still exists to ease the joining between the layers */
/* It could easily disappear in the future */

typedef struct HTMLCTX HTMLCTX;
typedef struct HTMLCTXClass HTMLCTXClass;
typedef HTMLCTXClass HTStreamClass ;

struct HTMLCTXClass
{
    void (*_free) (HTMLCTX *me);
    void (*abort) (HTMLCTX *me /* , void *wibble */ );
    void (*put_character) (HTMLCTX *me, char ch);
    void (*put_string) (HTMLCTX *me, const char *str);
    void (*write) (HTMLCTX *me, const char *str, int len);
};

#define HTML_MAGIC		0xdeadf1d0

struct HTMLCTX
{
    int magic;

    const HTMLCTXClass *	isa;
    SGMLCTX *                   sgmlctx;

#if !defined(STDALONE)
    rid_header *                rh;
    rid_map_item *              map;
    rid_aref_item *		aref;		/* The current anchor, or NULL if we don't have one */
    rid_form_item *		form;		/* The current form the we are in, or NULL */
    rid_table_item *            table;          /* The current table we are in, or NULL */
    rid_frame *                 frameset_stack[20/*MAX_NESTING*/];  /* current frameset stack */
#endif

    unsigned int                mode:2,
	strip_space:1,
	inhand_reason:4,
	last_reason:4,
	last_mode:3,
	done_auto_body:1,
	discard_noembed:1,	/* Not stacked */
	no_break:1;		/* NOBR tag */
/* 	unstack_noembed:1 */

    /* change to buffer */

    STRING                      inhand_string;

    char *			basetarget;
    char *			auto_open_string;

    rid_frame *                 frameset;
    int                         noframe;

    void (*old_deliver) (SGMLCTX *, int, STRING, ELEMENT *); /* Not stacked */

    rid_object_item *		object;
    int				object_nesting;
};

#define htmlctxof(x) ((HTMLCTX*)(x)->clictx)

typedef struct { char *str; int tag; } strtag; /* only dump.c uses this now? */

/* Assumes VALUE *attributes */
#define HAVEATTR(attr, match)  (attributes->value[attr].type == (match))

/* x is notionally an HTStream type */
#define SGML_Get_Target(x)	((HTMLCTX *)(x)->target)

/*****************************************************************************/

/* FUNCTIONS WITHIN THE HTML SECTION OF FRESCO */

/* deliver.c */
extern void sgml_deliver(SGMLCTX *context, int reason, STRING item, ELEMENT *elem);

/* genproc.c */
extern void generic_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes);
extern void generic_finish (SGMLCTX * context, ELEMENT * element);
extern void report_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes);
extern void report_finish (SGMLCTX * context, ELEMENT * element);
extern void incomplete_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes);
extern void incomplete_finish (SGMLCTX * context, ELEMENT * element);

/* builders.c */
/* Out of date: No more than MAX_STRING character should result from formatting. */
/* Use %.*s formatting if uncertain */
extern void new_option_item(HTMLCTX * me, VALUE *value, rid_input_flags flags);
extern void pseudo_html(HTMLCTX *context, const char *fmt, ... );
extern void select_pre_mode(HTMLCTX *context);
extern void select_fmt_mode(HTMLCTX *context);
extern void select_str_mode(HTMLCTX *context);
extern void select_perm_mode(HTMLCTX *context);
extern void select_last_mode(HTMLCTX *context);
extern void text_item_push_bullet(HTMLCTX * me, int item_type);
extern void push_fake_search_form(HTMLCTX * me, VALUE *prompt);
extern void text_item_push_break(HTMLCTX * me);
extern void text_item_ensure_break(HTMLCTX * me);
extern void text_item_revoke_break(HTMLCTX * me);
extern void text_item_push_hr(HTMLCTX * me, VALUE *align, VALUE *noshade, VALUE *size, VALUE *width);
extern void new_form_item(HTMLCTX * me, VALUE *action, VALUE *method, VALUE *target, VALUE *id, VALUE *enctype);
extern void bump_current_indent(SGMLCTX *context);
extern void bump_current_rindent(SGMLCTX *context); /* Right-hand indent */
extern void add_fixed_to_font(SGMLCTX *context);
extern void add_bold_to_font(SGMLCTX *context);
extern void add_italic_to_font(SGMLCTX *context);
extern void add_underline(SGMLCTX *context);
extern void add_strike(SGMLCTX *context);
extern void set_font_size(SGMLCTX *context, int size);
extern void set_font_type(SGMLCTX *context, int type);
extern void std_lcr_align(SGMLCTX *context, VALUE *align);
extern void htmlriscos_colour(VALUE *col, int *word);
/*extern void must_have_a_word_pushed(HTMLCTX *htmlctx);*/
extern void text_item_push_word(HTMLCTX * me, rid_flag xf, BOOL space);
extern void decode_img_align(int align, rid_image_flags *img_flags, rid_flag *item_flags);
extern void text_item_push_image(HTMLCTX * me,
				 int flags,
				 VALUE *src,
				 VALUE *alt,
				 VALUE *align,
				 VALUE *border,
				 VALUE *ww,
				 VALUE *hh,
				 VALUE *usemap,
				 VALUE *ismap,
				 VALUE *hspace,
				 VALUE *vspace);
extern rid_aref_item * new_aref_item(HTMLCTX* me,
			  VALUE *href,
			  VALUE *name,
			  VALUE *rel,
			  VALUE *target,
			  VALUE *title);
extern void new_map_item(HTMLCTX *me, VALUE *name);
extern void new_area_item(HTMLCTX *me,
			  rid_map_item *map,
			  VALUE *shape,
			  VALUE *coords,
			  VALUE *href,
			  VALUE *target,
			  VALUE *alt);
extern void text_item_push_select(HTMLCTX * me, VALUE *name, VALUE *size, VALUE *multiple, VALUE *id, VALUE *bgcolor, VALUE *selcolor, VALUE *nopopup);
extern void text_item_push_textarea(HTMLCTX * me, VALUE *name, VALUE *rows, VALUE *cols, VALUE *id, VALUE *bgcolor, VALUE *selcolor, VALUE *cursor, VALUE *tabindex, VALUE *wrap);
extern void text_item_push_input(HTMLCTX * me, int flags,
				 VALUE *align,
				 VALUE *checked,
				 VALUE *disabled,
				 VALUE *maxlength,
				 VALUE *name,
				 VALUE *size,
				 VALUE *src,
				 VALUE *type,
				 VALUE *value,
				 VALUE *id,
				 VALUE *bgcolor,
				 VALUE *selcolor,
				 VALUE *cursor,
				 VALUE *nocursor,
				 VALUE *numbers,
				 VALUE *selimage);
extern void push_fake_search_form(HTMLCTX * me, VALUE *prompt);

/* hparse.c */

extern void parse_frames(int yes);
extern void my_sgml_pre_open(SGMLCTX *context, ELEMENT *element);

/*****************************************************************************/

#endif /* __htmlparser_h */

/* eof htmlparser.h */
