/* fresco/commonsrc/backend.h */

#ifndef __backend_h
#define __backend_h

extern void rid_zero_widest_height_from_item(rid_text_item *item);
extern rid_pos_item *be_formater_loop_core( rid_header *rh, rid_text_stream *st, rid_text_item *this_item, rid_fmt_info *fmt, int flags);
extern int  dummy_tidy_proc(rid_header *rh, rid_pos_item *new, int width, int display_width);
extern void dummy_table_proc(rid_header *rh, rid_text_stream *stream, rid_text_item *item, rid_fmt_info *parfmt);
extern pparse_details *be_lookup_parser(int ft);
extern os_error *backend_doc_set_flags(be_doc doc, int mask, int eor);
#ifdef BUILDERS
extern os_error *antweb_trigger_fetching(antweb_doc *doc);
#endif

#endif /* __backend_h */

/* eof */
