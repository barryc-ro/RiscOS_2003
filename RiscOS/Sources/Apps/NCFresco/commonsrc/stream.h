/* -*-c-*- */

/* stream.h */

/* Functions to operate on a stream of rid_text_items */

/* A Stream callback function gets called for each text item in a stream */

typedef void (*stream_callback_fn)(rid_text_stream *st, rid_text_item *ti, void *params);
void stream_iterate(rid_text_stream *st, stream_callback_fn cbf, void *params);

/* There are two questions to be asked about item locations within a
 * stream.  The first is 'What item is at location x,y?' and the
 * second is 'At what location is item ti?'
 */

os_error *stream_find_item_at_location(rid_text_stream *st, int *x, int *y, be_item *ti);

BOOL stream_find_item_location(be_item ti, int *x, int *y);

/* This is the main function to render a stream of text, it is called
 * from the backend and from the code for rendering tables to render
 * the caption and each cell. */

void stream_render(rid_text_stream *stream, antweb_doc *doc,
		   const int ox, const int oy, /* screen origin of text stream */
		   const int left, const int top, /* Area being redrawn in local coords */
		   const int right, const int bot,
		   object_font_state *fs, wimp_box *g, const int update );

/* We need to be able to render a stream as a drawfile to save the
 * whole document and also to save a cell from a table */

void stream_write_as_drawfile(be_doc doc, rid_text_stream *stream,
			     int fh, int *writepoint, int ox, int oy);

void stream_write_as_text(rid_header *rh, rid_text_stream *stream, FILE *f);
