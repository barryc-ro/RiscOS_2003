/* -*-c-*- */

#ifndef __wimp_h
#include "wimp.h"
#endif

#include "rid.h"
#include "access.h"

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

#define doc_flag_DISPLAYING	(1<<16)	/* The document is being displayed, it's safe to try updating the view */
#define doc_flag_INCOMPLETE	(1<<17)	/* The document fetch was haltedc before it was done */
#define doc_flag_SECURE		(1<<18)	/* A secure method, such as SSL, was used. */

typedef struct _antweb_doc {
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
    int im_in_transit;		/* The number of bytes in the images in transit */
    int im_so_far;		/* The bytes so far for those in transit */
    rid_text_item *input;
    int text_input_offset;
    awp_page_str *paginate, *last_page;
    struct _antweb_doc *fetching;
    struct layout_spacing_info *spacing_list;
    int object_handler_count;	/* Number of objects that are active, when 0 remove message handler */
#if USE_MARGINS
    wimp_box margin;
#endif
    int scale_value;
    int start_time;		/* clock() at which page started downloading */

    int threaded;
    int pending_delete;
} antweb_doc;

#define BASE(doc) ((doc->rh && doc->rh->base) ? doc->rh->base : doc->url)

extern rid_header *parse_html_file(char *fname, char *url);
extern rid_header *parse_text_file(char *fname, char *url);
extern rid_header *parse_gopher_file(char *fname, char *url);
extern rid_header *parse_gif_file(char *fname, char *url);

extern void antweb_doc_image_change(void *h, void *i, int status, wimp_box *box);
extern void antweb_update_item(antweb_doc *doc, rid_text_item *ti);
extern void antweb_submit_form(antweb_doc *doc, rid_form_item *form, int right);
extern void antweb_place_caret(antweb_doc *doc, rid_text_item *ti);
extern BOOL antweb_pointer_in_image(antweb_doc *doc, rid_text_item *ti, wimp_mousestr *m, int *xx, int *yy, BOOL in_pixels);
extern BOOL antweb_locate_item(rid_text_item *ti, int *xx, int *yy);
extern os_error *antweb_document_format(antweb_doc *doc, int user_width);
extern os_error *antweb_handle_url(antweb_doc *doc, rid_aref_item *aref, const char *query, const char *target);
extern void antweb_update_item_trim(antweb_doc *doc, rid_text_item *ti, wimp_box *box, BOOL wont_plot_all);

extern void be_update_link(antweb_doc *doc, rid_aref_item *aref, int selected);
extern int antweb_get_edges(const rid_text_item *ti, int *left, int *right);
extern int antweb_render_background(wimp_redrawstr *rr, void *h, int update);
extern int antweb_doc_abort_all(void);
extern os_error *antweb_document_sizeitems(antweb_doc *doc);

/* eof antweb.h */
