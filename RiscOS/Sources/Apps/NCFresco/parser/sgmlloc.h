/* sgmlloc.h - SGML-parser-local externals */

/*****************************************************************************/

/* elements.c */
/*extern void sgml_free_attributes(VALUE *values);*/
extern BOOL default_attributes(SGMLCTX *context, ELEMENT *element, VALUES *values);
extern void ensure_pre_requisites (SGMLCTX *context, ELEMENT *element);
extern void ensure_post_requisites (SGMLCTX *context, ELEMENT *element);
/*extern void special_container_open_actions(SGMLCTX *context, ELEMENT *element);*/
/*extern void special_container_close_actions(SGMLCTX *context, ELEMENT *element);*/
extern void perform_element_open(SGMLCTX *context, ELEMENT *element, VALUES *values);
extern void perform_element_close(SGMLCTX *context, ELEMENT *element);
extern void parse_then_perform_element_open(SGMLCTX *context);
extern void do_got_element(SGMLCTX *context);

/* entities.c */
extern void entity_recognition(SGMLCTX *context);

/* statemach.c */
extern void state_stuck_text(SGMLCTX *context, char input);
extern void state_end_found_element (SGMLCTX *context, char input);
extern void state_end_find_element_body (SGMLCTX *context, char input);
extern void state_end_find_element (SGMLCTX *context, char input);
extern void state_end_find_slash (SGMLCTX *context, char input);
extern void state_some_sgml_command (SGMLCTX *context, char input);

#if OLD_COMMENTS
extern void state_comment_pre_weak_strong (SGMLCTX *context, char input);
extern void state_comment_strong (SGMLCTX *context, char input);
extern void state_comment_pre_strong_1(SGMLCTX *context, char input);
extern void state_comment_pre_strong (SGMLCTX *context, char input);
extern void state_comment_weak (SGMLCTX *context, char input);
extern void state_comment_pre_weak_1 (SGMLCTX *context, char input);
extern void state_comment_pre_weak_initial (SGMLCTX *context, char input);
extern void state_comment_pre_initial_1 (SGMLCTX *context, char input);
extern void state_comment_initial (SGMLCTX *context, char input);
#else
extern void state_comment_wait_close (SGMLCTX *context, char input);
extern void state_comment_wait_dash_2 (SGMLCTX *context, char input);
extern void state_comment_wait_dash_1 (SGMLCTX *context, char input);
#endif

extern void state_comment_pre_initial (SGMLCTX *context, char input);
extern void state_comment_maybe (SGMLCTX *context, char input);
extern void state_unquoted_value (SGMLCTX *context, char input);
extern void state_single_quoted_value (SGMLCTX *context, char input);
extern void state_double_quoted_value (SGMLCTX *context, char input);
extern void state_find_attribute_value (SGMLCTX *context, char input);
extern void state_find_attribute_equals (SGMLCTX *context, char input);
extern void state_in_attribute_name (SGMLCTX *context, char input);
extern void state_find_attribute_name (SGMLCTX *context, char input);
extern void state_in_element_name (SGMLCTX *context, char input);
extern void state_had_markup_open (SGMLCTX *context, char input);
extern void state_badly_formed_tag (SGMLCTX *context, char input);
extern void state_really_badly_formed_tag (SGMLCTX *context, char input);
extern void state_end_tag_only (SGMLCTX *context, char input);
extern void state_markup_only (SGMLCTX *context, char input);
extern void state_all_tags (SGMLCTX *context, char input);
extern state_fn get_state_proc(SGMLCTX *context);
#if DEBUG
extern char *get_state_name(state_fn fn);
#endif

/* support.c */

extern int strnicmp(char *a, char *b, int n);
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
extern int string_count_elements(STRING s, int c);
extern STRING stringtok(STRING *s1, const char *p);
extern STRING string_skip_to_chars(STRING s, const char *p);
extern STRING string_skip_chars(STRING s, const char *p);
extern int string_count_tokens(STRING s, char *breaks);
extern void sgml_support_initialise(void);
extern BOOL is_whitespace(char c);
extern BOOL is_element_start_character(char c);
extern BOOL is_element_body_character(char c);
extern BOOL is_attribute_start_character(char c);
extern BOOL is_attribute_body_character(char c);
extern BOOL is_value_start_character(char c);
extern BOOL is_value_body_character(char c);
extern BOOL is_entity_character(char c);
extern BOOL element_bit_clear(BITS *elems, int tag);
extern BOOL element_bit_set(BITS *elems, int tag);
extern void element_clear_bit(BITS *elems, int tag);
extern void element_set_bit(BITS *elems, int tag);
extern int find_element(SGMLCTX *context, STRING s);
extern int find_attribute(SGMLCTX *context, ELEMENT *element, STRING s);
extern void clear_stack_item(STACK_ITEM *stack);
extern void clear_stack(SGMLCTX *context);
extern void reset_lexer_state(SGMLCTX *context);
extern char *elements_name(SGMLCTX *context, int ix);
extern void push_stack(SGMLCTX *context, ELEMENT *element);
extern void pop_stack(SGMLCTX *context);
extern void clear_inhand(SGMLCTX *context);
extern void reset_tokeniser_state(SGMLCTX *context);
extern void add_to_buffer(BUFFER *buffer, char input);
extern void add_char_to_inhand(SGMLCTX *context, char input);
extern void push_inhand(SGMLCTX *context);
extern void push_bar_last_inhand(SGMLCTX *context);

/* eof sgmlloc.h */
