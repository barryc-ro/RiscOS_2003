/* > oimage.h
 *
 * Exported headers from oimage.c
 *
 */


/* ----------------------------------------------------------------------------- */

extern void oimage_render_text(rid_text_item *ti, antweb_doc *doc, object_font_state *fs, wimp_box *bbox, const char *text);
extern void oimage_render_border(rid_text_item *ti, antweb_doc *doc, wimp_box *bbox, int bw);
extern void oimage_size_image(const char *alt, const rid_stdunits *req_ww, const rid_stdunits *req_hh, rid_image_flags flags, BOOL defer_images, int scale_value, int fwidth, int *iw, int *ih);
extern image oimage_fetch_image(antweb_doc *doc, const char *src, BOOL need_size);
extern void oimage_flush_image(image im);
extern int oimage_decode_align(rid_image_flags flags, int height);
extern BOOL oimage_handle_usemap(rid_text_item *ti, antweb_doc *doc, int x, int y, wimp_bbits bb, rid_map_item *map);
extern void oimage_size_allocate(rid_text_item *ti, rid_header *rh, antweb_doc *doc, int fwidth);

/* ----------------------------------------------------------------------------- */

/* eof oimage.h */
