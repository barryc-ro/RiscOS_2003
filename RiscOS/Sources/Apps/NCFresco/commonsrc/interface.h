/* -*-c-*- */

/* interface.h */

#ifndef __interface_h
#define __interface_h

#ifndef __os_h
#include "os.h"
#endif

#ifndef __wimp_h
#include "wimp.h"
#endif

#ifndef __stdio_h
# include <stdio.h>
#endif

/* Describe the interface between the front and back of Fresco */

/***************************************************************************/
/* Abstractions of objects */

/* An fe_view is a handle onto a front end view on the screen */
typedef struct _frontend_view *fe_view;
/* A be_doc is a handle onto a back end document that may be onb the screen or may still be being fetched */
typedef struct _antweb_doc *be_doc;
/* A be_item is a handle onto an entity in a document that takes up space in a view */
typedef struct rid_text_item *be_item;

/***************************************************************************/
/* Error wraping functions */

/* If e is not NULL complain to the user.  Returns its parameter so that it can be used as a wrapper */
os_error *frontend_complain(os_error *e);

/* If e is not NULL report a fatal error and exit.  If NULL just return. */
void frontend_fatal_error(os_error *s);

/***************************************************************************/
/* A very few global variables. */

/* The number of OS units per pixel tend to be needed in redraw loops.
 * Having them as globals is a little faster than using a function to
 * read them. */
extern int frontend_dx, frontend_dy;

/***************************************************************************/
/* Functions for talking to the front end */

/* Open a URL and either put the data up in a new view or use an
 * existing one.  Sort out the referer name for the parent URL and
 * deal with the case where there is already a fetch in progress.  */

#define fe_open_url_NO_CACHE		(1<<0)	/* Don't get file from cache */
#define fe_open_url_NO_HISTORY		(1<<1)	/* Don't add file to history list */
#define fe_open_url_FROM_HISTORY	(1<<2)	/* url was pulled from history list */
#define fe_open_url_FROM_FRAME		(1<<3)	/* url was initiated from a frameset */
#define fe_open_url_NO_REFERER		(1<<4)	/* url is unrelated to current document */
#define fe_open_url_NO_ENCODING_OVERRIDE (1<<5)	/* don't let user override encoding */

os_error *frontend_open_url(char *url, fe_view parent, char *windowname, char *bfile, int flags);

/* Try to make sure that a vertical line from the height top down to
 * height bottom, at a horizontal position x, is visable in the view.
 * If top and bottom are equal then for the top position to the top of
 * the view where possible.  The function makes no allowances for
 * other windows on top. */
int frontend_view_ensure_visable(fe_view v, int x, int top, int bottom);
int frontend_view_ensure_visable_full(fe_view v, int left, int right, int top, int bottom, int flags);

/* Force the redraw for the view.  If bb is NULL redraw the whole lot.
 * The view is assumed to have the origin at the top left corner. */
int frontend_view_redraw(fe_view v, wimp_box *bb);

/* Force the update of a view.  Call the given function for each
 * rectangle with a pointer to redraw structure, the given handle and
 * a flag that is TRUE if the background has not been cleared */
typedef int (*fe_rectangle_fn)(wimp_redrawstr *r, void *h, int update);

#define fe_update_WONT_PLOT_ALL     (1<<0)
#define fe_update_IMAGE_RENDERING   (1<<1)

int frontend_view_update(fe_view v, wimp_box *bb, fe_rectangle_fn fn, void *h, int flags);

/* Move a rectangle of a view to somewhere else and force the redraw
 * of parts where the source was not visable. */
int frontend_view_block_move(fe_view v, wimp_box *bb, int newx, int newy);

/* Update the status box for a view.  The status type described what
 * sort of information is to be presented.  The info field may point
 * to a string to be inserted into a status window.
 * The varargs contain information dependant on the status_type.
 */
int frontend_view_status(fe_view v, int status_type, ...);

#define sb_status_WORLD		(-1)	/* Make the world turn */
                                        /* (void) */
#define sb_status_URL		0	/* Set the URL for the page we are viewing */
                                        /* (char *url) */
#define sb_status_IDLE		1	/* Nothing going on, clear the pointer info message */
                                        /* (void) */
#define sb_status_LINK		2	/* Display info about a link */
                                        /* (char *url, char *title) */
#define sb_status_MAP		3	/* Display info about an image map */
                                        /* (char *url, char *title, int x, int y) */
#define sb_status_FETCHED	4	/* Info about the amount of data fetched so far */
                                        /* (int so_far, int total_size) */
#define sb_status_PROGRESS	5	/* Progess information */
                                        /* (int status) */
#define sb_status_SENT		6	/* Info about the amount of data sent so far */
                                        /* (int so_far, int total_size) */
#define sb_status_IMAGE		7	/* Image transfer progress info*/
                                        /* (int fetched, fetching, waiting, error, deferred, sofar, size) */
#define sb_status_ABORTED	8	/* Indicate that a fetch was aborted */
                                        /* (void) */
#define sb_status_HELPER	9	/* Tell the user a helper appliaction was used */
                                        /* (void) */
#define sb_status_HELP		10	/* Provide raw help information for where the pointer is */
                                        /* (char *msg) */
#define sb_status_FINISHED	11	/* Got to the end of doc_complete */
                                        /* (void) */
#define sb_status_PLUGIN	12	/* A plugin has opened an instance or changed busy or play state */
                                        /* (plugin instance, int busy, int play_state, int opening, int closing) */
#define sb_status_LOADING	13	/* Like FETCHED but comes from local file */

#define sb_status_PROGRESS_ABORTED  (-1)
#define sb_status_PROGRESS_LOCAL    (-2)

/* Inform the frontend that the given URL has now arrived.  This has a
 * number of effects.  Firstly, a full window redraw is performed.
 * Secondly, the URL is inserted into the history in some suitable
 * way.  Thirdly, the status bar will be updated to reflect the new
 * URL.  There need not be an extra call to frontend_view_status().
 * The URL passed is the one to insert into a favourites list if the
 * user asks to remember the current page.  If the doc is NULL then
 * the front end takes this to mean the current fetch failed.
 */
int frontend_view_visit(fe_view v, be_doc doc, char *url, char *title);

typedef struct {
    int wa_width;		/* The width of the current work area */
    int user_width;		/* The width the user asked for */
    int doc_width;		/* The width of the widest item in the displaying document */
    int wa_height;		/* The height of the current work area */
    int min_height;		/* The minimum height the front end will allow */
    int doc_height;		/* The height of the formatted docuent being displayed */
    int layout_width;           /* width available for laying out frames */
    int layout_height;          /* height available for laying out frames */
} fe_view_dimensions;

int frontend_view_get_dimensions(fe_view v, fe_view_dimensions *fvd);

/* Get the bounding box, in work area coordinates, of what is visable. */
int frontend_view_bounds(fe_view v, wimp_box *box);
int frontend_view_margins(fe_view v, wimp_box *box);

/* Tell the front end the width of the widest item or unbreakable line
 * and the height of the document as a whole */
int frontend_view_set_dimensions(fe_view v, int width, int height);

/* Place the caret on a view.  Position in OS units relative to work
 * area origin (i.e. y is negative).  The height may be -ve to
 * indicate that the caret should not be visible.  If the on_screen
 * flag is set then the view should be scrolled to ensure the user can
 * see the caret. */

int frontend_view_caret(fe_view v, int x, int y, int height, int on_screen);

/* Return TRUE if the view has the caret at the moment */
int frontend_view_has_caret(fe_view v);

typedef struct _frontend_passwd_handle *fe_passwd;

typedef void (*backend_passwd_callback)(fe_passwd pw, void *handle, char *user, char *password);

/* Raise a dialogue box asking for a user ID and password.  The user
 * ID may be given.  Call back the cb function with the data handle
 * when the user has made a choice.  Call back with a NULL 'user'
 * variable if the user aborts */

fe_passwd frontend_passwd_raise(backend_passwd_callback cb, void *handle, char *user, char *realm, char *site);

/* Abort the password entry process before the user gives a reply. */
void frontend_passwd_dispose(fe_passwd pw);

/* Punt the job of dealing with a URL to some other task.  The
 * frontend needs to send the message and listen for the echo in case
 * noone picks it up. */

/* @@@@ */

void frontend_url_punt(fe_view v, char *url, char *bfile);

/* Given a file in the cache, cause the document to be opened.  This
 * will be called when a document of a type that can not be directly
 * opened is received from a server. */
void frontend_pass_doc(fe_view v, char *url, char *cfile, int ftype);

/* Tell the frontend what file name a document or image was saved
 * under so that the path can be suggested next time. */
extern void frontend_saver_last_name(char *fname);

typedef struct _frontend_menu_handle *fe_menu;
typedef struct {
    char flags;
    char language;
    char *name;
} fe_menu_item;

#define fe_menu_flag_CHECKED	(1 << 0)
#define fe_menu_flag_SHADED	(1 << 1)
#define fe_menu_flag_WIDE	(1 << 2) /* wide font necessary */

typedef void (*be_menu_callback)(fe_menu mh, void *handle, int item, int right);

fe_menu frontend_menu_create(fe_view parent, be_menu_callback cb, void *handle, int n, fe_menu_item *items, int size, int width);

/*
 * x and y are the wkarea coords of the top right of the item selected
 */
void frontend_menu_raise(fe_menu m, int x, int y);
void frontend_menu_dispose(fe_menu m);
void frontend_menu_update_item(fe_menu mh, int i);

#define fe_scrolling_AUTO   0
#define fe_scrolling_YES    1
#define fe_scrolling_NO     2

#define fe_divider_LEFT		0 /* must match with values in rid.h */
#define fe_divider_TOP		1
#define fe_divider_RIGHT	2
#define fe_divider_BOTTOM	3
#define fe_divider_BORDERLESS	0x00010000

typedef struct
{
    char *name;         /* this pointer can be copied */
    char *src;          /* this pointer must NOT be copied */
    char scrolling;     /* 0=AUTO, 1=YES, 2=NO */
    char noresize;      /* 0=allow user resizing, 1=don't */
    wimp_box box;
    wimp_box margin;
    int dividers[4];
} fe_frame_info;

void frontend_frame_layout(fe_view v, int nframes, fe_frame_info *info, int refresh_only, int dividers_max);
extern int frontend_view_get_dividers(fe_view v, int *dividers);

int frontend_test_history(char *url);

int frontend_can_handle_file_type(int ft);

/* Plugin related functions */

extern wimp_w frontend_get_window_handle(fe_view v);

extern wimp_t frontend_plugin_start_task(int filetype);
extern int frontend_plugin_handle_file_type(int filetype);
extern int frontend_message_send(wimp_msgstr *mp, wimp_t dest_t);

/* This is the same as used by win.h */
typedef int (*frontend_message_handler)(wimp_eventstr *e, void *handle);

extern int frontend_message_add_handler(frontend_message_handler fn, void *handle);
extern int frontend_message_remove_handler(frontend_message_handler fn, void *handle);

extern void frontend_fade_frame(fe_view v, wimp_paletteword colour);

#define fe_internal_url_ERROR		0 /* Display the No Data error */
#define fe_internal_url_REDIRECT	1 /* *new_url contains a URL to redirect to */
#define fe_internal_url_NO_ACTION	2 /* Do nothing, command completed successfully and quietly */
#define fe_internal_url_NEW		3 /* New file created in 'file' */
#define fe_internal_url_HELPER		4 /* Some kind of helper fired off */

extern int frontend_internal_url(const char *path, const char *query, const char *bfile, const char *referer, const char *file, char **new_url, int *flags);

extern void frontend_pointer_set_position(fe_view v, int x, int y);

/* Called from memory handler. Frontend should free up any memory that
 * it doesn't really need and return 1 if it freed anything, 0 otherwise
 */

extern int frontend_memory_panic(void);


/*
 * Read the state of the underlying link
 */

#define fe_interface_UP		1	/* link is up, IP is flowing */
#define fe_interface_DOWN	0	/* link is down */
#define fe_interface_ERROR	(-1)	/* link is down, error occurred trying to bring it up */

extern int frontend_interface_state(void);

typedef struct
{
    BOOL verified;
    char *issuer;
    char *subject;
    char *cipher;
} fe_ssl_info;

typedef struct _frontend_ssl_handle *fe_ssl;

typedef void (*backend_ssl_callback)(void *handle, BOOL verified);

extern fe_ssl frontend_ssl_raise(backend_ssl_callback fn, const fe_ssl_info *info, void *handle);
extern void frontend_ssl_dispose(fe_ssl fe);

/***************************************************************************/
/* Functions for talking to the back end */

/* Tell the backend that the screen mode or palette may have changes */
#define be_change_MODE	0x01
#define be_change_PALETTE	0x02
os_error *backend_screen_changed(int flags);

/* A frontend view exists in some form, try to open the given URL,
 * maybe with a body sent and maybe non-cached.  If a connection can
 * be made then fill in the document handle at docp.  Return an error
 * if there is a problem but note that if the URL is punted this is
 * not a problem. */
os_error *backend_open_url(fe_view v, be_doc *docp,
			   char *url, char *bfile, char *referer,
			   int flags);
#define be_openurl_flag_NOCACHE		(1 << 0)
#define be_openurl_flag_DEFER_IMAGES	(1 << 1)
#define be_openurl_flag_ANTIALIAS	(1 << 2)
#define be_openurl_flag_BODY_COLOURS	(1 << 3)
#define be_openurl_flag_HISTORY		(1 << 4) /* url was pulled from history list */
#define be_openurl_flag_SOLID_HIGHLIGHT	(1 << 5)
#define be_openurl_flag_FAST_LOAD	(1 << 6)
#define be_openurl_flag_NO_ENCODING_OVERRIDE (1 << 7)	/* don't let user override encoding */

/* Jump the document to the fragment given */
extern os_error *backend_goto_fragment(be_doc doc, char *frag);

/* Alter the flags that were set when the document was opened */
extern os_error *backend_doc_set_flags(be_doc doc, int mask, int eor);

/* Dispose of a document which might still be in transit and may
 * include images that are still in transit.  Free all the associated
 * resources and generally tidy up. */
os_error *backend_dispose_doc(be_doc doc);

/* Abort the fetch of a document, leaving any existing part of the
 * document in a displayable state. */
os_error *backend_doc_abort(be_doc doc);

/* Reformat the document to the given width. */
os_error *backend_reset_width(be_doc doc, int width);

/* Inform the backend that the user has the pointer in the given
 * position.  If the button bits say that a button is pressed then
 * take appropriate action, otherwise just make a status update call.
 * */
os_error *backend_doc_click(be_doc doc, int x, int y, wimp_bbits bb);

/* Inform the backend that a key has been pressed.  The backend will
 * update the 'used' value if to indicate the truth of whether the key
 * press was used */

#define be_doc_key_NOT_USED	0
#define be_doc_key_USED		1
#define be_doc_key_SUBMIT	2 /* key was used and submitted a form */
#define be_doc_key_FILLED	3 /* key was used and filled a text entry field */

os_error *backend_doc_key(be_doc doc, int key, int *used);

/* Inform the backend that the user wants to move the caret. */
#define be_cursor_UP	(0 << 0) /* Move the cursor up */
#define be_cursor_DOWN	(1 << 0) /* Move the cursor down */
#define be_cursor_LIMIT	(1 << 1) /* Move the cursor to the limit within the document */
#define be_cursor_WRAP	(1 << 2) /* Wrap if the cursor moves off the end of the document */
os_error *backend_doc_cursor(be_doc doc, int motion, int *used);

/* Return information about the document in hand. */
os_error *backend_doc_info(be_doc doc, int *flags, int *ftype, char **url, char **title);
#define be_doc_info_ISINDEX	(1 << 0) /* The document is marked as an index */
#define be_doc_info_HAS_BG	(1 << 1) /* The document has a background image */
#define be_doc_info_HTML_ERRS	(1 << 2) /* The document souce had HTML errors */
#define be_doc_info_IN_CACHE	(1 << 3) /* The document is in the cache */
#define be_doc_info_FETCHING	(1 << 4) /* The document is still being fetched and what is displayed is only partial */
#define be_doc_info_INCOMPLETE	(1 << 5) /* The document is incomplete due to being aborted during the fetch */
#define be_doc_info_SECURE	(1 << 6) /* The document was fetched over a secure transport */

/* Return information about the file a document came from */
os_error *backend_doc_file_info(be_doc doc, int *ft, int *load, int *exec, int *size);

/* Return information about images in the document */
os_error *backend_doc_images(be_doc doc, int *waiting, int *fetching, int *fetched, int *errors, int* in_trans, int *so_far);

/* Total number of images (i.e. whether save box says "with images") */
int backend_total_images( be_doc doc );

/* Return information about an image item. */
os_error *backend_image_info(be_doc doc, void *im, int *flags, int *ftype, char **url);
#define be_image_info_FETCHED		(1 << 0)
#define be_image_info_DEFERED		(1 << 1)
#define be_image_info_RENDERABLE	(1 << 2)
#define be_image_info_ERROR		(1 << 3)
#define be_image_info_BLACKLIST         (1 << 4)
#define be_image_info_INTERLACED	(1 << 16)
#define be_image_info_MASK		(1 << 17)
#define be_image_info_ANIMATION		(1 << 18)

/* Flush an image, or all images if the image handle is NULL. */
os_error *backend_doc_flush_image(be_doc doc, void *imh, int flags);

/* Defer all images. Doesn't abort fetches which have already started */
void backend_defer_images( be_doc doc );

/* Stop all GIF animations */
void backend_stop_animation( be_doc doc );

/* Given a position within a document, locate the item as the position
 * given in *x and *y.  The item at *ti is filled in and *x and *y are
 * updated to show the offset within the item.  If the position is not
 * with any item then *ti is set to NULL. */
os_error *backend_doc_locate_item(be_doc doc, int *x, int *y, be_item *ti);

/* Given an item, return the bouding box of the item on the work area */
os_error *backend_doc_item_bbox(be_doc doc, be_item ti, wimp_box *bb);

/* Return information flags about the item given.  Also returns the
 * destination of any anchor on the item.  This call can be used to
 * choose which menu items should be displayed, for choosing help
 * information or just for telling the user what an anchor links to.
 * If link is non-NULL the value it points to is filled in with a
 * pointer to a transient copy of the URL which must be copied before
 * another call (and must not be freed). */
os_error *backend_item_pos_info(be_doc doc, be_item ti, int *x, int *y, int *flags, char **link, void **im, char **title);
os_error *backend_item_info(be_doc doc, be_item ti, int *flags, char **link, void **im);
#define be_item_info_LINK	(1 << 0) /* The item has an anchor with an HREF */
#define be_item_info_ISMAP	(1 << 1) /* The item has the ISMAP flag set */
#define be_item_info_IMAGE	(1 << 2) /* There is an image file fetched for this item */
#define be_item_info_INPUT	(1 << 3) /* The item is a text item or texarea item */
#define be_item_info_MENU	(1 << 4) /* The item is a selection */
#define be_item_info_BUTTON	(1 << 5) /* The item is check, radio or reset button */
#define be_item_info_ACTION	(1 << 6) /* The item is submit button */
#define be_item_info_USEMAP	(1 << 7) /* The item is a clientside imagemap */
#define be_item_info_SECURE	(1 << 8) /* If ACTION then dest URL is https:... */
#define be_item_info_PLUGIN	(1 << 9) /* The item is a plugin, 'im' is the internal plugin handle */
#define be_item_info_LABEL	(1 << 10) /* The item is a LABEL for a form element */
#define be_item_info_NUMBERS	(1 << 11) /* INPUT TYPE=TEXT NUMBERS (NCFresco only) */

os_error *backend_image_size_info(be_doc doc, void *imh, int *width, int *height, int *bpp);
os_error *backend_image_file_info(be_doc doc, void *imh, int *load, int *exec, int *size);

/* Get the backend to render a part of a view.  The handle passed is
 * the document handle but is given as a void* so thatthe function can
 * be passed into the frontend_update_view call.  Note that since it
 * is pointless to try to return an error in a redraw loop the
 * fuinction is void. */
int backend_render_rectangle(wimp_redrawstr *r, void *h, int update);

/* A stack of functions to save out documents and images.  The handle
 * is a be_doc for the document savers and an image handle for the
 * images.  A void* is used so that that function can be passed to
 * saveas() or xfersend(). */
extern BOOL backend_doc_saver_text(char *fname, void *doc);
extern BOOL backend_doc_saver_draw(char *fname, void *doc);
extern BOOL backend_doc_saver_headers(char *fname, void *doc);

extern BOOL backend_image_saver_sprite(char *fname, void *imh);

/* Search a document for some string */
extern be_item backend_find(be_doc doc, be_item start, char *text, int flags);
#define be_find_BACKWARDS	(1 << 0) /* Search back from item (or from end) */
#define be_find_CASELESS	(1 << 1) /* Do caseless comparisons */

/* highlight next link from given link or position */
extern be_item backend_highlight_link(be_doc doc, be_item item, int flags);
extern be_item backend_highlight_link_xy(be_doc doc, be_item item, const wimp_box *box, int flags);
#define be_link_VERT		(1 << 0)	/* UP/down not left/right */
#define be_link_BACK		(1 << 1)	/* left/up not right/down */
#define be_link_VISIBLE	        (1 << 2)	/* only consider visible items */
#define be_link_INCLUDE_CURRENT	(1 << 3)	/* start on the current item */
#define be_link_DONT_WRAP	(1 << 4)	/* return null if no more in given direction */
#define be_link_DONT_HIGHLIGHT	(1 << 5)	/* return the new link but don't actually move highlight */
#define be_link_TEXT		(1 << 6)	/* only find INPUT TEXT and TEXTAREA items */
#define be_link_ONLY_CURRENT	(1 << 7)	/* only consider the current item */
#define be_link_CLEAR_REST	(1 << 8)	/* clear out all selected items */
#define be_link_XY		(1 << 9)	/* x,y coordinates given */
#define be_link_DONT_WRAP_H	(1 << 10)	/* don't wrap in h direction */
#define be_link_CARETISE	(1 << 11)	/* if a writeabnle ends up with the highlight then caretise it */
#define be_link_MOVE_POINTER	(1 << 12)	/* move the pointer with the highlight */
#define be_link_DONT_FORCE_ON	(1 << 13)	/* don't force onto the screem */

extern void backend_remove_highlight(be_doc doc);
extern be_item backend_read_highlight(be_doc doc, BOOL *had_caret);
extern void backend_set_caret(be_doc doc, be_item ti, int offset);
extern void backend_set_highlight(be_doc doc, be_item item);
extern int backend_is_selected(be_doc doc, be_item ti);

/* Activate a given link */
extern os_error *backend_activate_link(be_doc doc, be_item item, int flags);

/*
 * Place the caret in the given item, item=NULL to place on background somewhere
 * will probably end up calling frontend_view_caret
 * returns old position of caret
 */

#if 0
#define backend_place_caret_READ	((be_item)-1)

extern be_item backend_place_caret(be_doc doc, be_item item);
extern be_item backend_read_highlight(be_doc doc);
#endif

/* Get a name for a file in the cache */

extern char *backend_temp_file_name(void);

/* Register a file with the cache manager
 * Only use with filenames returned by above function
 * If not callng this function then remove the file
 */

extern void backend_temp_file_register(char *url, char *file_name);

/*
 * This searches for the piece of META information with the given name
 * and returns a pointer to it. This pointer has the same life time as the doc.
 */

extern const char *backend_check_meta(be_doc doc, const char *name);

#if 0
/* Clear selection from all items */
extern void backend_clear_selected(be_doc doc);

/* Select an item, whatever its type */
extern void backend_select_item(be_doc doc, be_item ti, int select);

/* return the first item with SELETED bit set */
extern be_item backend_find_selected(be_doc doc);

/* this takes 1,0,-1 to change the SELECTED flag status
 * It returns item or if item is a link then the first item in that anchor sequence
 */
extern be_item backend_update_link(be_doc doc, be_item item, int selected);
#endif

/* this takes 0 or 1 to change the ACTIVATED flag status */
extern void backend_update_link_activate(be_doc doc, be_item item, int activated);

#define be_resize_NONE      0
#define be_resize_WIDTH     1
#define be_resize_HEIGHT    2

extern int backend_frame_resize_bounds(be_doc doc, int x, int y, wimp_box *box, int *handle);
extern void backend_frame_resize(be_doc doc, int x, int y, int handle);

#if 0
extern void backend_set_margin(be_doc doc, wimp_box *margin);
#endif

#if 0
extern void backend_image_expire(be_doc doc, void *imh);
#endif

extern void backend_doc_reformat(be_doc doc);
extern void backend_doc_set_scaling(be_doc doc, int scale_value);

extern BOOL backend_submit_form(be_doc doc, const char *id, int right);

/*
 * This function sends one of three different messages to the plugin.
 * CLOSE: sends a close message - tells the plugin to remove its window as if the
 *	page were being left.
 * ABORT: sends an ABORT message - tells the plugin to stop what its doing but to
 *	leave the window open so the user can see what happened.
 * action: an ACTION message (STOP, PLAY etc.) the window stays open.
 *
 * The message can be directed at a specific embedded object, all helpers, or all
 * helpers and embedded objects.
 */

#define be_plugin_action_ABORT		(-1)
#define be_plugin_action_CLOSE		(-2)
#define be_plugin_action_item_HELPERS	((be_item)-1)
#define be_plugin_action_item_ALL	((be_item)0)
extern void backend_plugin_action(be_doc doc, be_item item, int action);

#define be_plugin_BUSY		0x0001
#define be_plugin_MUTED		0x0002
#define be_plugin_CAN_FOCUS	0x0100
#define be_plugin_CAN_ACTION	0x0200
#define be_plugin_HELPER	0x0400

extern void backend_plugin_info(be_doc doc, void *pp, int *flags, int *state);

extern void backend_mark_page_visited( const char *url );

extern be_item backend_locate_id(be_doc doc, const char *id);

#define be_encoding_READ	(-1)
extern int backend_doc_encoding(be_doc doc, int encoding);

/* Return the language code for this item or the document if item is
 * NULL or has no language of its own.
 */
extern int backend_doc_item_language(be_doc doc, be_item item);

/* Functions in layout.c */
/* Write out frame layout as a table, return the number of frames */

typedef void (*be_layout_write_table_fn)(FILE *f, const char *frame_specifier, int w, int h);

extern void backend_layout_write_table(FILE *f, be_doc doc, be_layout_write_table_fn fn, const char *prefix, int w, int h);

extern int backend_getwebfont(be_doc doc, BOOL wide, int language, int font1, int base);

#endif /* __interface_h */

