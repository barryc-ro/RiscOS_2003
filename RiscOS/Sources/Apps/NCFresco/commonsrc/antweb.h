/* -*-c-*- */

#ifndef fresco_antweb_h
#define fresco_antweb_h

#ifndef __wimp_h
#include "wimp.h"
#endif

#ifndef __rid_h
#include "rid.h"
#endif

#ifndef __access_h
#include "access.h"
#endif

#define POLL_INTERVAL	20	/* centi-seconds */

#define ANTWEB_DOC_MAGIC	0x1bd739ef

#define SI_PRINT 1

typedef struct awp_page_str {
    struct _antweb_doc *doc;	/* So that we can get back to the document just from one of these */
    rid_pos_item *line, *last;	/* The first and last lines of a page */
    struct awp_page_str *next, *prev;
#if SI_PRINT
    int offset;
#else
    int pheight;		/* Formatted page height in OS units */
#endif
} awp_page_str;

#define doc_flag_UL_LINKS	(1<<0) /* Underline links when rendering. */
#define doc_flag_NO_PICS	(1<<1) /* Don't render pictures even if you have them. */
#define doc_flag_NO_FILL	(1<<2) /* Surpress filling of backgrounds (for printing) */
#define doc_flag_DEFER_IMAGES	(1<<3) /* Don't fetch images until asked */
#define doc_flag_ANTIALIAS	(1<<4) /* Use antialaised text */
#define doc_flag_DOC_COLOURS	(1<<5) /* Use colours from the document */
#define doc_flag_FROM_HISTORY	(1<<6) /* Document was loaded from the history list (don't check expiry) */
#define doc_flag_USE_DRAW_MOD	(1<<7) /* Draw lines with the draw module, rather than OS_Plot */
#define doc_flag_SOLID_HIGHLIGHT (1<<8) /* Draw highlight filled rather than boxed */
#define doc_flag_FAST_LOAD	(1<<9) /* load as quickly as possible */

#define doc_flag_DISPLAYING	(1<<16)	/* The document is being displayed, it's safe to try updating the view */
#define doc_flag_INCOMPLETE	(1<<17)	/* The document fetch was haltedc before it was done */
#define doc_flag_SECURE		(1<<18)	/* A secure method, such as SSL, was used. */
#define doc_flag_HAD_HEADERS	(1<<19)	/* Have we transferred HTTP headers into the META list */
#define doc_flag_WRECKED	(1<<20)	/* A dreadful memory problem has occurred */
#define doc_flag_DIRECTORY      (1<<21) /* FTP fetched us a directory */

#define doc_selection_tag_NONE	0
#define doc_selection_tag_TEXT	1
#define doc_selection_tag_AREF	2
#define doc_selection_tag_MAP	3

#define doc_selection_offset_UNKNOWN	(-1)
#define doc_selection_offset_NO_CARET	(-2)


typedef struct antweb_selection_boundary antweb_selection_boundary;
typedef struct antweb_selection_t antweb_selection_t;
typedef struct antweb_selection_descr antweb_selection_descr;
typedef struct antweb_selection_list_descr antweb_selection_list_descr;

#define selection_boundary_END	0
#define selection_boundary_MOVE	1
#define selection_boundary_DRAW	2

struct antweb_selection_boundary
{
    antweb_selection_boundary *next;

    char plot;
    char reserved[3];
    short x, y;
};

struct antweb_selection_t				/* the currently selected item. Could be an anchor or a text item or a map */
{
    int tag;
    union
    {
	struct
	{
	    rid_text_item *item;	/* a TEXT covers the other highlightable objects */
	    int input_offset;		/* caret offset */
	} text;

	rid_aref_item *aref;		/* an AREF covers an anchor linking several text items, or */
					/* a label anchor linking text and inputs  */
	struct
	{
	    rid_area_item *area;	/* an AREA covers an individual item in a client-side imagemap or SHAPED OBJECT */
	    rid_text_item *item;
	} map;
    } data;
    antweb_selection_boundary *boundary, *boundary_last;
};

#define LINK_SORT 0		/* future expansion... */

struct antweb_selection_descr
{
    wimp_box bbox;		/* bounding box of this item */
    antweb_selection_t item;	/* could be AREA or TEXT; AREFS are listed individually */

#if LINK_SORT
    int x, y;			/* hotspot to move off from */

    antweb_selection_descr *prev_x;		/* link to prev item horizontally */
    antweb_selection_descr *next_x;		/* link to next item horizontally */
    antweb_selection_descr *prev_y;		/* link to prev item vertically */
    antweb_selection_descr *next_y;		/* link to next item vertically */
#endif
};

struct antweb_selection_list_descr
{
    antweb_selection_descr *list;	/* pointer to base of link array */
    int count;			/* count of number of items in link array */

#if LINK_SORT
    antweb_selection_descr *left;		/* pointer to left most link item (counting from left edge) */
    antweb_selection_descr *right;		/* pointer to right most link item (counting from right edge) */
    antweb_selection_descr *top;		/* pointer to top most link item (counting from top edge) */
    antweb_selection_descr *bottom;		/* pointer to bottom most link item (counting from bottom edge) */
#endif
};

typedef struct _antweb_doc
{
    struct _antweb_doc *next;
    int magic;			/* Magic number */
    char *url;
    char *frag;			/* Fragment string */
    char *cfile;		/* RISC OS cache file */
    int flags;
    int file_load_addr;
    int file_exec_addr;
    int file_size;
    access_handle ah;
    int lstatus, lbytes;	/* Used to stop dbox flicker */
    rid_header *rh;
    void *ph;			/* Parser handle */
    void *pd;			/* Pointer to the parser_details to use */
#if 0
    int user_width;		/* The last 'width' we we fed for formatting */
    int height;			/* This is the y0 value from the extent, ie minus the height */
    int widest;			/* Width of the widest item in the page */
#endif
    struct _frontend_view *parent;	/* The view that this document will come out in */

    int im_fetched;		/* The number of images fetched so far */
    int im_fetching;		/* The number of images being fetched */
    int im_unfetched;		/* The number of images yet to be fetched */
    int im_error;		/* The number of images that failed to get fetched */
    int im_deferred;		/* The number of images that are deferred */
    int im_in_transit;		/* The number of bytes in the images in transit */
    int im_so_far;		/* The bytes so far for those in transit */

/*     rid_text_item *input; */
/*     int text_input_offset; */

    antweb_selection_t selection; /* holds currently selected object */
    antweb_selection_list_descr selection_list;	/* array to help move selection around */

    awp_page_str *paginate, *last_page;
    struct _antweb_doc *fetching; /* currently fetching imagemap */
    struct layout_spacing_info *spacing_list;
    int object_handler_count;	/* Number of objects that are active, when 0 remove message handler */
#if USE_MARGINS
    wimp_box margin;
#endif
    int scale_value;
    int start_time;		/* clock() at which page started downloading */

    int threaded;
    int pending_delete;

    int encoding_user;		/* user set encoding (from config) */
    int encoding_user_override;	/* user really wants encoding */
    
    unsigned int fontusage[8];  /* 8x32=256-bit array of font usage */

    BOOL bHighlightPersistent;  /* highlight is actually a selection */
} antweb_doc;

#define SETFONTUSED(doc,n) doc->fontusage[n>>5] |= (1 << (n&31))
#define GETFONTUSED(doc,n) ( (doc->fontusage[n>>5] & (1 << (n&31)) ) != 0 )

#define BASE(doc) ((doc->rh && doc->rh->base) ? doc->rh->base : doc->url)

extern rid_header *parse_html_file(char *fname, char *url);
extern rid_header *parse_text_file(char *fname, char *url);
extern rid_header *parse_gopher_file(char *fname, char *url);
extern rid_header *parse_gif_file(char *fname, char *url);

extern void antweb_doc_image_change(void *h, void *i, int status, wimp_box *box);
extern void antweb_update_item(antweb_doc *doc, rid_text_item *ti);
extern void antweb_submit_form(antweb_doc *doc, rid_form_item *form, int right);

#define antweb_place_caret(doc, ti, offset) backend_set_caret(doc, ti, offset)

extern void antweb_default_caret(antweb_doc *doc, BOOL take_caret);
extern BOOL antweb_pointer_in_image(antweb_doc *doc, rid_text_item *ti, wimp_mousestr *m, int *xx, int *yy, BOOL in_pixels);
extern BOOL antweb_locate_item(rid_text_item *ti, int *xx, int *yy);
extern os_error *antweb_document_format(antweb_doc *doc, int user_width);
extern os_error *antweb_handle_url(antweb_doc *doc, rid_aref_item *aref, const char *query, const char *target);
extern void antweb_update_item_trim(antweb_doc *doc, rid_text_item *ti, wimp_box *box, BOOL wont_plot_all);

extern int antweb_get_edges(const rid_text_item *ti, int *left, int *right);
extern int antweb_render_background(wimp_redrawstr *rr, void *h, int update);
extern int antweb_doc_abort_all(int level);
extern os_error *antweb_trigger_fetching(antweb_doc *doc);
extern void antweb_uncache_image_info(antweb_doc *doc);
extern int antweb_getwebfont(antweb_doc *doc, rid_text_item *ti, int base);

/* backend.c */

extern os_error *antweb_doc_ensure_font( antweb_doc *doc, int n );
extern void be_ensure_buffer_space(char **buffer, int *len, int more);

/* from keyhl.c */

extern void antweb_build_selection_list(antweb_doc *doc);
extern rid_text_item *be_doc_read_caret(antweb_doc *doc);
extern BOOL be_item_has_caret(antweb_doc *doc, rid_text_item *ti);

/* From highlight.c */

/* extern void highlight_render(int ox, int oy, const wimp_box *g, antweb_doc *doc); */
extern void highlight_render_outline(rid_text_item *ti, antweb_doc *doc, int hpos, int bline);
extern void highlight_update_border(antweb_doc *doc, wimp_box *box, BOOL draw);
extern void highlight_offset_border(wimp_box *box);
extern void highlight_render(wimp_redrawstr *rr, antweb_doc *doc);
extern void highlight_draw_text_box(rid_text_item *ti, antweb_doc *doc, int b, int hpos, BOOL has_text);

extern void highlight_boundary_build(antweb_doc *doc);
extern void highlight_boundary_clear(antweb_doc *doc);



#endif

/* eof antweb.h */
