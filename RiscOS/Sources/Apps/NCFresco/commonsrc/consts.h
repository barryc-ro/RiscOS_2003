/* -*-c-*- */

/* Not all of these are considered contstant! Shame! */
/* Most should be obtained on a per-document basis.  */

/**********************************************************************/

#define MILIPOINTS_PER_OSUNIT	400

/* Chars are 16 OS units wide and 32 tall */
#define CHAR_WIDTH	16
#define CHAR_HEIGHT	32
/* Width in OS units */
#define DEFAULT_WIDTH	(80*CHAR_WIDTH)
#define MAXIMUM_WIDTH	(160*CHAR_WIDTH)

#ifndef LEFT_OFFSET
#define LEFT_OFFSET	6
#endif

#define MIN_WIN_HEIGHT	-1024

#ifdef PLOTCHECK
#	define PBREAK_SPACING	2
#	define PBREAK_HEIGHT	(PBREAK_SPACING+2)
#else
#	define PBREAK_SPACING	12
#	define PBREAK_HEIGHT	(PBREAK_SPACING+4)
#endif

#define BIGINT          0x70000000      /* Used by tables - a wide formatting width */


/*#define rid_CELL_XBORDER        4*/
/*#define rid_CELL_YBORDER        4*/


