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
} webfont;

#define WEBFONT_FLAG_SHIFT	3
#define WEBFONT_FLAG_BOLD	(1 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_ITALIC	(2 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_FIXED	(4 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_MASK	(7 << WEBFONT_FLAG_SHIFT)
#define WEBFONT_FLAG_COUNT	8

#define WEBFONT_SIZE_SHIFT	0
#define WEBFONT_SIZE_MASK	(7 << 0)
#define WEBFONT_SIZE(x)		(((x)-1) << WEBFONT_SIZE_SHIFT)
#define WEBFONT_SIZES		7
#define WEBFONT_SIZEOF(f)	((((f) & WEBFONT_SIZE_MASK) >> WEBFONT_SIZE_SHIFT) + 1)

/* If bit 6 is set then we are using a symbol font
 * The FLAG bits determine which font we are using as
 * symbol fonts have no variants
 */

#define WEBFONT_FLAG_SPECIAL		(1 << 6)
#define WEBFONT_SPECIAL_TYPE_SHIFT	WEBFONT_FLAG_SHIFT
#define WEBFONT_SPECIAL_COUNT		2
#define WEBFONT_SPECIAL_TYPE_MASK	(7 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_SYMBOL	(0 << WEBFONT_SPECIAL_TYPE_SHIFT)
#define WEBFONT_SPECIAL_TYPE_MENU	(1 << WEBFONT_SPECIAL_TYPE_SHIFT)

#define WEBFONT_SYMBOL(n)	(WEBFONT_SIZE(n) + WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_SYMBOL)

/* this knows the ordering of the flag bits */
#define WEBFONT_COUNT		(WEBFONT_FLAG_SPECIAL + (WEBFONT_SPECIAL_COUNT << WEBFONT_SPECIAL_TYPE_SHIFT))

/* Some standard font definitions */

#define WEBFONT_BASE	(WEBFONT_SIZE(3))

#define WEBFONT_PRE	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED)
#define WEBFONT_CITE	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED)
#define WEBFONT_DT	(WEBFONT_SIZE(4) + WEBFONT_FLAG_BOLD)
#define WEBFONT_DD	(WEBFONT_SIZE(3))
#define WEBFONT_BLOCK	(WEBFONT_SIZE(3) + WEBFONT_FLAG_ITALIC)
#define WEBFONT_TTY	(WEBFONT_SIZE(3) + WEBFONT_FLAG_FIXED)

#ifdef STBWEB

#define WEBFONT_H1	(WEBFONT_SIZE(6) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H2	(WEBFONT_SIZE(5) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H3	(WEBFONT_SIZE(4) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H4	(WEBFONT_SIZE(3) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H5	(WEBFONT_SIZE(2) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H6	(WEBFONT_SIZE(1) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H7	(WEBFONT_SIZE(1) + WEBFONT_FLAG_BOLD)

#define WEBFONT_BUTTON  (WEBFONT_SIZE(3))

#else

#define WEBFONT_H1	(WEBFONT_SIZE(7) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H2	(WEBFONT_SIZE(6) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H3	(WEBFONT_SIZE(5) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H4	(WEBFONT_SIZE(5) + WEBFONT_FLAG_ITALIC)
#define WEBFONT_H5	(WEBFONT_SIZE(4) + WEBFONT_FLAG_BOLD)
#define WEBFONT_H6	(WEBFONT_SIZE(4) + WEBFONT_FLAG_ITALIC)
#define WEBFONT_H7	(WEBFONT_SIZE(3) + WEBFONT_FLAG_BOLD)

#define WEBFONT_BUTTON  WEBFONT_TTY

#endif

extern webfont webfonts[WEBFONT_COUNT];

extern os_error *webfonts_init_font(int n);
extern os_error *webfonts_init(void);
extern os_error *webfonts_tidyup(void);
extern int webfont_tty_width(int w, int in_chars);
extern os_error *webfont_declare_printer_fonts(void);
extern os_error *webfont_drawfile_fontlist(int fh, int *writeptr);
extern int webfont_font_width(int f, const char *s);
extern int webfont_lookup(const char *font_name);
