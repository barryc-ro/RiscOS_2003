/* sgmlimp.h - functions the SGML parser imports */

/* These are now in sgmlparser.c, which uses the SGMLBACK */
/* structure in SGMLCTX to call the user. Makes life easier */
/* when only a stupid linker is present. */

#ifndef SGML_REPORTING
#define SGML_REPORTING 0
#endif

#if SGML_REPORTING

#define sgml_note_good_open(a,b) a->callback.note_good_open(a,b)
#define sgml_note_good_close(a,b) a->callback.note_good_close(a,b)
#define sgml_note_implied_open(a,b) a->callback.note_implied_open(a,b)
#define sgml_note_implied_close(a,b) a->callback.note_implied_close(a,b)
#define sgml_note_missing_open(a,b) a->callback.note_missing_open(a,b)
#define sgml_note_missing_close(a,b) a->callback.note_missing_close(a,b)
#define sgml_note_close_without_open(a,b) a->callback.note_close_without_open(a,b)
#define sgml_note_had_unknown_attribute(a,b) a->callback.note_had_unknown_attribute(a,b)
#define sgml_note_had_bad_attribute(a,b) a->callback.note_had_bad_attribute(a,b)
#define sgml_note_not_repeatable(a,b) a->callback.note_not_repeatable(a,b)
#define sgml_note_antiquated_feature(a) a->callback.note_antiquated_feature(a)
#define sgml_note_badly_formed_comment(a) a->callback.note_badly_formed_comment(a)
#define sgml_note_stack_overflow(a) a->callback.note_stack_overflow(a)
#define sgml_note_stack_underflow(a) a->callback.note_stack_underflow(a)
#define sgml_note_unexpected_character(a) a->callback.note_unexpected_character(a)
#define sgml_note_has_tables(a) a->callback.note_has_tables(a)
#define sgml_note_has_forms(a) a->callback.note_has_forms(a)
#define sgml_note_has_scripts(a) a->callback.note_has_scripts(a)
#define sgml_note_has_frames(a) a->callback.note_has_frames(a)
#define sgml_note_has_address(a) a->callback.note_has_address(a)

extern void sgml_note_unknown_element (SGMLCTX * context, const char *fmt, ...) ;
extern void sgml_note_unknown_attribute (SGMLCTX * context, const char *fmt, ...) ;
extern void sgml_note_bad_attribute (SGMLCTX * context, const char *fmt, ...);
extern void sgml_note_message(SGMLCTX * context, const char *fmt, ... );

#else /* SGML_REPORTING */

#define sgml_note_good_open(a,b)
#define sgml_note_good_close(a,b)
#define sgml_note_implied_open(a,b)
#define sgml_note_implied_close(a,b)
#define sgml_note_missing_open(a,b)
#define sgml_note_missing_close(a,b)
#define sgml_note_close_without_open(a,b)
#define sgml_note_had_unknown_attribute(a,b)
#define sgml_note_had_bad_attribute(a,b)
#define sgml_note_not_repeatable(a,b)
#define sgml_note_antiquated_feature(a)
#define sgml_note_badly_formed_comment(a)
#define sgml_note_stack_overflow(a)
#define sgml_note_stack_underflow(a)
#define sgml_note_unexpected_character(a)
#define sgml_note_has_tables(a)
#define sgml_note_has_forms(a)
#define sgml_note_has_scripts(a)
#define sgml_note_has_frames(a)
#define sgml_note_has_address(a)

#endif /* SGML_REPORTING */

/* eof sgmlimp.h */
