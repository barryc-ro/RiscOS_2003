/* -*-c-*- */

/* imagemap.h */

extern os_error *imagemap_check_all_images(antweb_doc *doc);
extern void imagemap_draw_area(antweb_doc *doc, void *im, rid_area_item *area, int xb, int yb);

extern rid_map_item *imagemap_find_map(rid_header * rh, const char *name);
extern rid_area_item *imagemap_find_area(rid_map_item * map, int x, int y);

extern rid_area_item *imagemap_find_area_from_name(rid_header * rh, const char *name, int x, int y);

/* eof imagemap.h */
