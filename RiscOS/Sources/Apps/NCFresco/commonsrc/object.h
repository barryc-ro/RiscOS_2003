/* -*-c-*- */

/* Interface to the displayable objects (rid_text_items) in the ANTWeb system */

#ifndef fresco_object_h
#define fresco_object_h

#ifndef fresco_antweb_h
#include "antweb.h"
#endif

/* Size an object: use the object data to fill in the header.  This
 * may allocate resources. */

typedef void (*object_size)(rid_text_item *ti, rid_header *rh, antweb_doc *doc);

/* Redraw an object: given the intersection point of the base line and
 * the left hand edge of the object, and a pointer to the current
 * graphics clipping box */

typedef struct {
    int lf;			/* Last font handle */
    int lfc;			/* Last foreground colour */
    int lbc;			/* Last background colour */
} object_font_state;

#define object_redraw_REDRAW	0
#define object_redraw_UPDATE	1
#define object_redraw_HIGHLIGHT	2
#define object_redraw_BACKGROUND 3

typedef void (*object_redraw)(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);

/* Dispose of resources: Used after an item has been sized and is no
 * longer needed. */

typedef void (*object_dispose)(rid_text_item *ti, rid_header *rh, antweb_doc *doc);

/* Click: return a URL to join with the base or current aref.  Use ""
 * if there is nothing special about the object.  Return a NULL
 * pointer to surpess the link (i.e. select item in a form).  The
 * values x and y are offsets within the item clicked on. */

typedef char * (*object_click)(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);

/* The astext function tries to write the data of the object out to a
 * text file as best it can */

typedef void (*object_astext)(rid_text_item *ti, rid_header *rh, FILE *f);

/* Offer the caret to an object.  If the result is true the object
 * took the caret.  If FALSE (or if the function pointer was NULL)
 * then try giving the caret to someone else. If the repos flag is set
 * then the request is simply to redisplay the caret in the correct
 * position.  In this is not set then the caret may get placed at the
 * and of the writable region. */

#define object_caret_REDISPLAY	0 /* redisplay caret at current position */
#define object_caret_REPOSITION	1 /* reposition the caret */
#define object_caret_FOCUS	2 /* acquire the caret */
#define object_caret_BLUR	3 /* lose the caret */

typedef BOOL (*object_offer_caret)(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int repos);

/* Process a key press.  This will only be called for an object after
 * the offer_caret call has been made.  If the key press can be used
 * TRUE is returned.  If false is returned then the key press will be
 * passed on first to the core application and then may be punted with
 * a wimp_processkey() call.  */

typedef BOOL (*object_process_key)(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int key);

/* Perform actions on images within objects
 * HANDLE: Return the image handle for the image in the object or NULL if
 * there is no image.
 * ABORT: If the image is partially loaded then abort it and cleanup
 * BOX: Return a pointer to a box describing the offset of the actual image
 * within the rid_text_item. In OS units. in trim form.
 * OBJECT: return an image handle if this is an image or the plugin handle if
 * this is a plugin. Be careful not to pass this value to an image_ functions.
 * Only to be used for comparisons with similarly ambiguous values.
 */

#define object_image_HANDLE	0
#define object_image_ABORT	1
#define object_image_BOX	2
#define object_image_OBJECT	3

typedef void *(*object_image_handle)(rid_text_item *ti, antweb_doc *doc, int reason);

/* Write the object out to the output file as a draw object with the
 * left end of the base line at the specified location within the draw
 * work area.  The wimp_box bounding box is expanded to encompas the
 * object being drawn.  The file offset value indicates where in the
 * file to write to and should be updated to show how much was writen.
 * The file handle is for direct RISC OS file access. */

typedef void (*object_asdraw)(rid_text_item *di, antweb_doc *doc,
			      int fh, int x, int y, int *fileoff, wimp_box *bb);

/* Update the area of the screen that would be covered by the highlighting
 * use antweb_update_item_trim.
 * If 0 then the whole object will be updated using antweb_update_item.

 * reason BOUNDS:
 *  Return in box the bounds of this item
 *  Return 'int' that is TRUE if the item needs a box drawn.
 *  FALSE if it has its own highlighted state.
 */

#define object_highlight_BOUNDS		0

typedef int (*object_update_highlight)(rid_text_item *di, antweb_doc *doc, int reason, wimp_box *box);

/* The object_methods structure is used to hold pointers to all the
 * methods for a given type of object. */

typedef struct {
    object_size size;
    object_redraw redraw;
    object_dispose dispose;
    object_click click;
    object_astext astext;
    object_offer_caret caret;
    object_process_key key;
    object_image_handle imh;
    object_asdraw asdraw;
    object_update_highlight update_highlight;
} object_methods;

/* Now we have prototypes for the functions themselves */

void obreak_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void opbreak_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void obreak_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
#if 0
void obreak_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *obreak_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
#endif
void obreak_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void obreak_asdraw(rid_text_item *di, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);

void otext_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void otext_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
#if 0
void otext_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *otext_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
#endif
void otext_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void otext_asdraw(rid_text_item *di, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);
int otext_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box);

void obullet_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void obullet_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
#if 0
void obullet_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *obullet_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
#endif
void obullet_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void obullet_asdraw(rid_text_item *di, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);

void oimage_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void oimage_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void oimage_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *oimage_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void oimage_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void *oimage_image_handle(rid_text_item *ti, antweb_doc *doc, int reason);
void oimage_asdraw(rid_text_item *ti, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);
int oimage_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box);

void oinput_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void oinput_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void oinput_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *oinput_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void oinput_astext(rid_text_item *ti, rid_header *rh, FILE *f);
BOOL oinput_caret(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int repos);
BOOL oinput_key(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int key);
void *oinput_image_handle(rid_text_item *ti, antweb_doc *doc, int reason);
void oinput_asdraw(rid_text_item *di, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);
int oinput_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box);

void otextarea_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void otextarea_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void otextarea_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *otextarea_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void otextarea_astext(rid_text_item *ti, rid_header *rh, FILE *f);
BOOL otextarea_caret(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int repos);
BOOL otextarea_key(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int key);
void otextarea_asdraw(rid_text_item *di, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);
int otextarea_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box);
void otextarea_append_to_buffer(rid_textarea_item *tai, char **buffer, int *len);
void otextarea_write_to_file(rid_textarea_item *tai, FILE *f);
void otextarea_write_to_file_raw(rid_textarea_item *tai, FILE *f);


void oselect_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void oselect_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void oselect_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char *oselect_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void oselect_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void oselect_asdraw(rid_text_item *di, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);
int oselect_update_highlight(rid_text_item *ti, antweb_doc *doc, int reason, wimp_box *box);

void otable_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void otable_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void otable_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char * otable_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void otable_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void otable_asdraw(rid_text_item *ti, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);

void oobject_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void oobject_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void oobject_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char * oobject_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void oobject_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void oobject_asdraw(rid_text_item *ti, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);
void *oobject_image_handle(rid_text_item *ti, antweb_doc *doc, int reason);

void oscaff_size(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
void oscaff_redraw(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int hpos, int bline, object_font_state *fs, wimp_box *g, int ox, int oy, int update);
void oscaff_dispose(rid_text_item *ti, rid_header *rh, antweb_doc *doc);
char * oscaff_click(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int x, int y, wimp_bbits bb);
void oscaff_astext(rid_text_item *ti, rid_header *rh, FILE *f);
void oscaff_asdraw(rid_text_item *ti, antweb_doc *doc, int fh, int x, int y, int *fileoff, wimp_box *bb);

/* Finally, a jump table to dispatch from */
extern object_methods object_table[];

#endif
