/* -*-c-*- */

/* webfonts.c */

/* Font code for the ANTWeb WWW browser */

#ifndef __font_h
#include "font.h"
#endif

typedef struct webfont {
    font handle;		/* Font handle, or -1 before we have one */
    int max_up;			/* In OS units */
    int max_down;		/* In OS units */
    int space_width;		/* In OS units */
    int usage_count;
} webfont;

#define WEBFONT_FLAG_SHIFT	3
#define WEBFONT_FLAG_BOLD	(1 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_ITALIC	(2 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_FIXED	(4 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_HEADING	(8 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_MASK	(15 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_COUNT	12

#define WEBFONT_SIZE_SHIFT	0
#define WEBFONT_SIZE_MASK	(7 << 0)
#define WEBFONT_SIZE(x)		(((x)-1) << WEBFONT_SIZE_SHIFT)
#define WEBFONT_SIZES		7
#define WEBFONT_SIZEOF(f)	((((f) & WEBFONT_SIZE_MASK) >> WEBFONT_SIZE_SHIFT) + 1)

/* If bit 6 is set then we are using a symbol font
 * The FLAG bits determine which font we are using as
 * symbol fonts have no variants
 */

#define WEBFONT_FLAG_SPECIAL		(1 << 7)
#define WEBFONT_SPECIAL_TYPE_SHIFT	WEBFONT_FLAG_SHIFT
#define WEBFONT_SPECIAL_TYPE_MASK	(7 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_SYMBOL	(0 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_MENU	(1 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_JAPANESE	(2 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_CHINESE	(3 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_KOREAN	(4 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_RUSSIAN	(5 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_GREEK	(6 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_HEBREW	(7 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_COUNT		7

#define WEBFONT_SYMBOL(n)	(WEBFONT_SIZE(n) + WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_SYMBOL)
#define WEBFONT_JAPANESE	(WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_JAPANESE)
#define WEBFONT_CHINESE		(WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_CHINESE)
#define WEBFONT_KOREAN		(WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_KOREAN)
#define WEBFONT_RUSSIAN		(WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_RUSSIAN)
#define WEBFONT_GREEK		(WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_GREEK)
#define WEBFONT_HEBREW		(WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_HEBREW)

/* this knows the ordering of the flag bits */
#define WEBFONT_COUNT		(WEBFONT_FLAG_SPECIAL + (WEBFONT_SPECIAL_COUNT << WEBFONT_SPECIAL_TYPE_SHIFT))

/* Some standard font definitions */

#define WEBFONT_BASE	(WEBFONT_SIZE(3))

#define WEBFONT_PRE	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED)
#define WEBFONT_CITE	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED)
#define WEBFONT_DT	(WEBFONT_SIZE(4) + WEBFONT_FLAG_BOLD)
#define WEBFONT_DD	(WEBFONT_SIZE(3))
#define WEBFONT_BLOCK	(WEBFONT_SIZE(3) + WEBFONT_FLAG_ITALIC)

#ifdef STBWEB

#define WEBFONT_H1	(WEBFONT_SIZE(6) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H2	(WEBFONT_SIZE(5) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H3	(WEBFONT_SIZE(4) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H4	(WEBFONT_SIZE(3) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H5	(WEBFONT_SIZE(2) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H6	(WEBFONT_SIZE(1) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H7	(WEBFONT_SIZE(1) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)

#define WEBFONT_TTY	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED + WEBFONT_FLAG_BOLD)

#define WEBFONT_BUTTON  (WEBFONT_SIZE(3))

#else

#define WEBFONT_H1	(WEBFONT_SIZE(7) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H2	(WEBFONT_SIZE(6) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H3	(WEBFONT_SIZE(5) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H4	(WEBFONT_SIZE(5) + WEBFONT_FLAG_ITALIC + WEBFONT_FLAG_HEADING)
#define WEBFONT_H5	(WEBFONT_SIZE(4) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)
#define WEBFONT_H6	(WEBFONT_SIZE(4) + WEBFONT_FLAG_ITALIC + WEBFONT_FLAG_HEADING)
#define WEBFONT_H7	(WEBFONT_SIZE(3) + WEBFONT_FLAG_BOLD + WEBFONT_FLAG_HEADING)

#define WEBFONT_TTY	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED)

#define WEBFONT_BUTTON  (WEBFONT_SIZE(3))

#endif

/* DL: The following extra masks are for use with SET_EFFECTS
       to achieve finer control over which effects are recorded
       as 'applied'. The naming is to allow macro pasting.
 */

#define STYLE_WF_SIZE_SHIFT STYLE_WF_INDEX_SHIFT
#define STYLE_WF_SIZE_MASK  WEBFONT_SIZE_MASK

extern webfont webfonts[WEBFONT_COUNT];

extern os_error *webfonts_initialise(void);

extern os_error *webfonts_reinitialise(void);

extern os_error *webfonts_tidyup(void);
/* extern int webfont_tty_width(int w, int in_chars); */
extern os_error *webfont_declare_printer_fonts(void);
extern os_error *webfont_drawfile_fontlist(int fh, int *writeptr);
extern int webfont_font_width(int f, const char *s);
extern int webfont_lookup(const char *font_name);

extern os_error *webfont_find_font( int n );
extern os_error *webfont_lose_font( int n );

extern int webfont_font_width_n(int f, const char *s, int n);
extern int webfont_split_point(int f, const char *s, int width);
extern int webfont_need_wide_font(const char *s, int n_bytes);
extern void webfont_set_wide_format(int fh);
extern int webfont_get_offset(int f, const char *s, int x, const int *coords, int len);
extern int webfont_split_point_char(int f, const char *s, int width, int c, int *segwidth);
extern int webfont_latin(int index);
extern int webfont_nominal_width(int font_index, int n_chars);
