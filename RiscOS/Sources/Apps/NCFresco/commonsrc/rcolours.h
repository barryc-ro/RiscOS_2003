/* -*-c-*- */

#ifndef __rcolours_h
# define __rcolours_h

/* rcolours.h */

#define render_colour_PLAIN	0	/* text fg, HR fill colour, TEXT, PASSWORD, TEXTAREA fg */
#define render_colour_AREF	1	/* colour for text links and image links borders (link) */
#define render_colour_CREF	2	/* colour for visited links (vlink) */
#define render_colour_BACK	3	/* page background colour */
#define render_colour_ACTION	4	/* pressed bg colour for SUBMIT, RESET, BUTTON items */
#define render_colour_WRITE	5	/* bg colour for TEXT, PASSWORD, TEXTAREA */
#define render_colour_LINE_L	6	/* colour for 3D plinthing */
#define render_colour_LINE_D	7	/* colour for 3D plinthing */
#define render_colour_INPUT_F	8	/* fg colour for SELECT & SUBMIT, RESET, BUTTON items */
#define render_colour_INPUT_B	9	/* unselected bg colour for SELECT & SUBMIT, RESET, BUTTON items */
#define render_colour_INPUT_S	10	/* selected bg colour for SELECT & SUBMIT, RESET, BUTTON items */
#define render_colour_HIGHLIGHT 11	/* col for highlight box */
#define render_colour_ACTIVATED 12	/* col for activating links (alink) */
#define render_colour_RGB       13

/* pdh: render_colour_RGB in the bottom byte means that the top three bytes
 * are an RGB colour
 */

#define render_colour_COUNT	14

/* ---------------------------------------------------------------------- */

/*#ifdef STBWEB*/

/* unhighlighted plinth colours */
#define plinth_col_L	(0x55555500 | render_colour_RGB)
#define plinth_col_M	(0x42424200 | render_colour_RGB)
#define plinth_col_D	(0x21212100 | render_colour_RGB)

/* highlighted plinth colours */
#define plinth_col_HL_L	(0x87878700 | render_colour_RGB)
#define plinth_col_HL_M	(0x75757500 | render_colour_RGB)
#define plinth_col_HL_D	(0x66666600 | render_colour_RGB)

/* blue plinth colours */
#define plinth_col_B_L	(0xcc444400 | render_colour_RGB)
#define plinth_col_B_M	(0xff773300 | render_colour_RGB)
#define plinth_col_B_D	(0x88444400 | render_colour_RGB)

/* plinth fill hilighted colour */
#define plinth_col_FILL	(0x88000000 | render_colour_RGB)

/*#endif*/

/* ---------------------------------------------------------------------- */

#endif
