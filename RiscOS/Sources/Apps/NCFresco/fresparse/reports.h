/* reports.h */

#ifndef __reports_h
#define __reports_h

extern STRING fprint(char *fmt, ...);
extern void my_sgml_reset_report(SGMLCTX * context);
#if 0
extern void print_bool (SGMLCTX *context, BOOL b, char *msg, char *pre, char *post);
extern void print_int (SGMLCTX *context, int i, char *msg, char *pre, char *post);
extern void print_row (SGMLCTX *context, int *row, int elems, char *msg, char *pre, char *post, BOOL close);
extern void print_list (SGMLCTX *context, STRING_LIST *the_list, char *msg, char *pre, char *post );
#endif
extern void print_report (SGMLCTX * context);
extern void my_sgml_note_good_open (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_good_close (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_implied_open (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_implied_close (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_missing_open (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_missing_close (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_close_without_open (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_had_unknown_attribute (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_had_bad_attribute (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_not_repeatable (SGMLCTX * context, ELEMENT * element);
extern void my_sgml_note_antiquated_feature (SGMLCTX * context);
extern void my_sgml_note_badly_formed_comment (SGMLCTX * context);
extern void my_sgml_note_stack_overflow (SGMLCTX * context);
extern void my_sgml_note_stack_underflow (SGMLCTX * context);
extern void my_sgml_note_unexpected_character (SGMLCTX * context);
extern void my_sgml_note_has_tables (SGMLCTX * context);
extern void my_sgml_note_has_forms (SGMLCTX * context);
extern void my_sgml_note_has_scripts (SGMLCTX * context);
extern void my_sgml_note_has_frames (SGMLCTX * context);
extern void my_sgml_note_has_address (SGMLCTX * context);
extern void my_sgml_note_unknown_element (SGMLCTX * context, char *fmt, va_list arglist); 
extern void my_sgml_note_unknown_attribute (SGMLCTX * context, char *fmt, va_list arglist); 
extern void my_sgml_note_bad_attribute (SGMLCTX * context, char *fmt, va_list arglist);
extern void my_sgml_note_message(SGMLCTX * context, char *fmt, va_list arglist );

#endif /* __reports_h */

/* eof */
