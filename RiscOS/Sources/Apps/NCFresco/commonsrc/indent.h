/* -*-c-*- */

/* indent.h */

#ifdef BUILDERS

#define INDENT_UNIT	1
#define INDENT_WIDTH	4

#else
#define INDENT_UNIT	(webfonts[WEBFONT_PRE].space_width)
#ifdef STBWEB
#define INDENT_WIDTH	4
#else
#define INDENT_WIDTH	6
#endif

#endif /* BUILDERS */
