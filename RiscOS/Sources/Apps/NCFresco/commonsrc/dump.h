extern void dump_span(int *base, int span);
extern void dump_style(ROStyle style);
extern void dump_pos(rid_pos_item *pi);
extern void dump_stdunits(rid_stdunits u);
extern void dump_item(rid_text_item *item, char *base);
extern void dump_aref(rid_aref_item *ptr);
extern void dump_input(rid_input_item *ptr);
extern void dump_option(rid_option_item *ptr);
extern void dump_select( rid_select_item *ptr);
extern void dump_textarea_line( rid_textarea_line *ptr);
extern void dump_textarea(rid_textarea_item *ptr);
extern void dump_form( rid_form_item *ptr);
extern void dump_caption(rid_table_caption *ptr, char *base);
extern void dump_table(rid_table_item *ptr, char *base);
extern void dump_props(rid_table_props *ptr);
extern void dump_width_info(rid_width_info info);
extern void dump_colgroup(rid_table_colgroup *ptr);
extern void dump_rowgroup(rid_table_rowgroup *ptr);
extern void dump_colhdr(rid_table_colhdr *ptr);
extern void dump_rowhdr(rid_table_rowhdr *ptr);
extern void dump_cell(rid_table_cell *ptr, char *base);
extern void dump_stream(rid_text_stream *ptr, char *base);
extern void dump_header(rid_header *ptr);
extern void dump_buffer(BUFFER *ptr);
extern void dump_sgmlctx(SGMLCTX *ptr);
extern void dump_table_width_details(rid_table_item *table);
extern void dump_htmlctx(HTMLCTX *ptr);
extern void dump_float_item(rid_float_item *ptr);
extern void dump_floats_link(rid_floats_link *ptr);
extern void dump_cell_map(rid_table_item *ptr, char *msg);
extern void dump_form_element(rid_form_element *ptr);
extern void dump_textarea_item(rid_textarea_item *ptr);
extern void dump_select_item(rid_select_item *ptr);
extern void dump_form_item(rid_form_item *ptr);



extern char *item_names[];
