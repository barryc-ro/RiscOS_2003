/* sgmlloc.h - SGML-parser-local externals */

/*****************************************************************************/

/* elements.c */
/*extern void sgml_free_attributes(VALUE *values);*/
extern BOOL default_attributes(SGMLCTX *context, ELEMENT *element, VALUES *values);
/*extern ELEMENT *ensure_pre_requisites (SGMLCTX *context, ELEMENT *element);*/
/*extern void ensure_post_requisites (SGMLCTX *context, ELEMENT *element);*/
/*extern void special_container_open_actions(SGMLCTX *context, ELEMENT *element);*/
/*extern void special_container_close_actions(SGMLCTX *context, ELEMENT *element);*/
/*extern void perform_element_open(SGMLCTX *context, ELEMENT *element, VALUES *values);*/
extern void perform_element_close(SGMLCTX *context, ELEMENT *element);
/*extern void parse_then_perform_element_open(SGMLCTX *context);*/
extern void do_got_element(SGMLCTX *context);

/* entities.c */
extern void entity_recognition(SGMLCTX *context);

/* statemach.c */
extern void state_stuck_text(SGMLCTX *context, UCHARACTER input);
extern void state_end_found_element (SGMLCTX *context, UCHARACTER input);
extern void state_end_find_element_body (SGMLCTX *context, UCHARACTER input);
extern void state_end_find_element (SGMLCTX *context, UCHARACTER input);
extern void state_end_find_slash (SGMLCTX *context, UCHARACTER input);
extern void state_some_sgml_command (SGMLCTX *context, UCHARACTER input);
extern void state_comment_wait_close (SGMLCTX *context, UCHARACTER input);
extern void state_comment_wait_dash_2 (SGMLCTX *context, UCHARACTER input);
extern void state_comment_wait_dash_1 (SGMLCTX *context, UCHARACTER input);
extern void state_comment_wait_dash_2_0 (SGMLCTX *context, UCHARACTER input);
extern void state_comment_wait_dash_1_0 (SGMLCTX *context, UCHARACTER input);

extern void state_comment_pre_initial (SGMLCTX *context, UCHARACTER input);
extern void state_comment_maybe (SGMLCTX *context, UCHARACTER input);
extern void state_unquoted_value (SGMLCTX *context, UCHARACTER input);
extern void state_single_quoted_value (SGMLCTX *context, UCHARACTER input);
extern void state_double_quoted_value (SGMLCTX *context, UCHARACTER input);
extern void state_find_attribute_value (SGMLCTX *context, UCHARACTER input);
extern void state_find_attribute_equals (SGMLCTX *context, UCHARACTER input);
extern void state_in_attribute_name (SGMLCTX *context, UCHARACTER input);
extern void state_find_attribute_name (SGMLCTX *context, UCHARACTER input);
extern void state_in_element_name (SGMLCTX *context, UCHARACTER input);
extern void state_had_markup_open (SGMLCTX *context, UCHARACTER input);
extern void state_badly_formed_tag (SGMLCTX *context, UCHARACTER input);
extern void state_really_badly_formed_tag (SGMLCTX *context, UCHARACTER input);
extern void state_end_tag_only (SGMLCTX *context, UCHARACTER input);
extern void state_markup_only (SGMLCTX *context, UCHARACTER input);
extern void state_all_tags (SGMLCTX *context, UCHARACTER input);
extern state_fn get_state_proc(SGMLCTX *context);
#if DEBUG
extern char *get_state_name(state_fn fn);
#endif

/* support.c */

extern int strnicmp(const char *a, const char *b, int n);
#if UNICODE
extern int strcmpu(const UCHARACTER *a, const char *b);
extern int strnicmpu(const UCHARACTER *a, const char *b, int n);
extern STRING mkstringu(void *encoding, UCHARACTER *ptr, int n);
#else
#define strcmpu(a,b) strcmp(a,b)
#define strnicmpu(a,b,c) strnicmp(a,b,c)
#define mkstringu(a,b,c) mkstring(b,c)
#endif
extern char *stringdup(STRING s);
extern char *strip_stringdup(STRING s);
extern char *valuestringdup(const VALUE *v);
extern STRING mkstring(char *ptr, int n);
extern STRING mktempstring(char *ptr);
extern STRING stringcat(STRING a, STRING b);
extern void string_free(STRING *ptr);
extern void string_list_free(STRING_LIST *ptr);
extern STRING string_strip_start(STRING s);
extern STRING string_strip_end(STRING s);
extern STRING string_strip_space(STRING s);
extern USTRING ustring_strip_start(USTRING s);
extern USTRING ustring_strip_end(USTRING s);
extern USTRING ustring_strip_space(USTRING in);
extern int string_count_elements(STRING s, int c);
extern STRING stringtok(STRING *s1, const char *p);
extern STRING string_skip_to_chars(STRING s, const char *p);
extern STRING string_skip_chars(STRING s, const char *p);
extern int string_count_tokens(STRING s, char *breaks);
extern STRING get_tab_expanded_string(STRING item, STRING inhand);
extern void sgml_support_initialise(void);
extern BOOL is_whitespace(UCHARACTER c);
extern BOOL is_element_start_character(UCHARACTER c);
extern BOOL is_element_body_character(UCHARACTER c);
extern BOOL is_attribute_start_character(UCHARACTER c);
extern BOOL is_attribute_body_character(UCHARACTER c);
extern BOOL is_value_start_character(UCHARACTER c);
extern BOOL is_value_body_character(UCHARACTER c);
extern BOOL is_entity_character(UCHARACTER c);
extern BOOL element_bit_clear(BITS *elems, int tag);
extern BOOL element_bit_set(BITS *elems, int tag);
extern void element_clear_bit(BITS *elems, int tag);
extern void element_set_bit(BITS *elems, int tag);
extern void element_bitset_or(BITS *elem_inout, BITS *elem_in, const size_t n_elems);
extern int find_element(SGMLCTX *context, USTRING s);
extern int find_attribute(SGMLCTX *context, ELEMENT *element, USTRING s, BOOL *guessed);
extern void clear_stack_item(STACK_ITEM *stack);
extern void clear_stack(SGMLCTX *context);
extern void reset_lexer_state(SGMLCTX *context);
extern char *elements_name(SGMLCTX *context, int ix);
extern void push_stack(SGMLCTX *context, ELEMENT *element);
extern void pop_stack(SGMLCTX *context);
extern STACK_ITEM *find_element_in_stack (SGMLCTX *context, ELEMENT *element);
extern void pull_stack_item_to_top (SGMLCTX *context, STACK_ITEM *item);
extern void pull_stack_item_to_top_correcting_effects (SGMLCTX *context, STACK_ITEM *item);
extern void clear_inhand(SGMLCTX *context);
extern void reset_tokeniser_state(SGMLCTX *context);
extern void add_to_buffer(BUFFER *buffer, const char *input, int len);
extern void add_to_prechop_buffer(SGMLCTX *context, UCHARACTER input);
extern void add_char_to_inhand(SGMLCTX *context, UCHARACTER input);
extern void push_inhand(SGMLCTX *context);
extern void push_bar_last_inhand(SGMLCTX *context);
#if UNICODE
extern char *usafe(USTRING s);
extern BOOL ustrnearly( const UCHARACTER *input, size_t ilen,
                const char *pattern, size_t plen, size_t hownear );
#else
#define usafe(a) ((a).ptr)
#define ustrnearly(a,b,c,d,e) strnearly(a,b,c,d,e)
#endif

extern long ustrtol(UCHARACTER *u, UCHARACTER **end, int base);

/* colours.c */
extern VALUE colour_lookup(STRING name);

/* eof sgmlloc.h */
