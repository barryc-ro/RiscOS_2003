/* glue.h - boundary between HTML and SGML style info */

#ifndef __glue_h
#define __glue_h

/*****************************************************************************

The underlying SGML parser stacks a simple array of
BITS, which it never tries to examine. This maps onto
a ROStyle structure via the  defines contained in this
file. Both HTML and SGML sections will include this file.
Subsequent words hold more recently added items of state.

 *****************************************************************************

Packing small integers into a large bitset. Each item has
a size (BLAH_MASK) and a starting bit position (BLAH_SHIFT).
Items cannot straddle a word boundary. The mask is always
positioned in the bottom of the word (eg 2nd byte has mask 0xff
and shift 8).

*/

#if 0
/* @@@@ This pack macro need to use the inverse of the mask on the second line */
#define PACK(ptr, item, value) \
        (ptr)[ ((item##_SHIFT) >> 5) ] = \
        ( (ptr)[ ((item##_SHIFT) >> 5) ] & ( item##_MASK << (item##_SHIFT & 0x1f) ) ) | \
        ( ( ((value)) & item##_MASK ) << (item##_SHIFT & 0x1f) )


#define UNPACK(ptr, item) \
        ( ( (ptr)[ ((item##_SHIFT) >> 5) ] >> (item##_SHIFT & 0x1f) ) & item##_MASK )

#else

#if 0
/* No longer use directly: */
#define PACK(ptr, item, value) \
    pack_fn(ptr, item##_SHIFT, item##_MASK, value)
#endif

#define UNPACK(ptr, item) \
    unpack_fn(ptr, item##_SHIFT, item##_MASK)

#define SET_EFFECTS_FLAG(ptr, item) \
    set_effects_fn (ptr, item##_SHIFT, item##_MASK, item)

#define SET_EFFECTS(ptr, item, value) \
    set_effects_fn (ptr, item##_SHIFT, item##_MASK, value)

#define SET_EFFECTS_WF_FLAG(ptr, item) \
    set_effects_wf_flag_fn (ptr, item)

#endif

/*****************************************************************************

First word - ROStyle overlay.

IF these are not kept in sync, then incorrect results *will* be obtained.

        IIII IIII FFFF FTEB CCCC XSSS VVVA AAbU

where U: underline, b: bold, A: align, V: valign, S: sub/sup, X: strike-through, C: colour
      B: webfont bold, E: webfont italic, T: webfont fixed, F: webfont size, I: indent

*/

/* 0 */

#define STYLE_UNDERLINE		0x0001

#define STYLE_UNDERLINE_MASK    0x0001
#define STYLE_UNDERLINE_SHIFT   0

#define rid_sf_UNDERLINE        ( STYLE_UNDERLINE << STYLE_UNDERLINE_SHIFT )

/*****************************************************************************/

/* 1 */

#define STYLE_BOLD		0x0001
#define STYLE_BOLD_MASK         0x0001
#define STYLE_BOLD_SHIFT        1

#define rid_sf_BOLD             ( STYLE_BOLD << STYLE_BOLD_SHIFT )

/*****************************************************************************/

/* 2,3,4 */

#define STYLE_ALIGN_UNKNOWN     0x0000
#define STYLE_ALIGN_LEFT        0x0001
#define STYLE_ALIGN_CENTER      0x0002
#define STYLE_ALIGN_RIGHT       0x0003
#define STYLE_ALIGN_CHAR        0x0004
#define STYLE_ALIGN_JUSTIFY     0x0005
#define STYLE_ALIGN_HSPARE6     0x0006
#define STYLE_ALIGN_HSPARE7     0x0007

#define STYLE_ALIGN_MASK        0x0007
#define STYLE_ALIGN_SHIFT       2

#define rid_sf_ALIGN_UNKNOWN    ( STYLE_ALIGN_UNKNOWN << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_LEFT       ( STYLE_ALIGN_LEFT << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_CENTER     ( STYLE_ALIGN_CENTER << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_RIGHT      ( STYLE_ALIGN_RIGHT << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_CHAR       ( STYLE_ALIGN_CHAR << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_JUSTIFY    ( STYLE_ALIGN_JUSTIFY << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_HSPARE6    ( STYLE_ALIGN_HSPARE6 << STYLE_ALIGN_SHIFT )
#define rid_sf_ALIGN_HSPARE7    ( STYLE_ALIGN_HSPARE7 << STYLE_ALIGN_SHIFT )

#define rid_sf_ALIGN_MASK       ( STYLE_ALIGN_MASK << STYLE_ALIGN_SHIFT )

/*****************************************************************************/

/* 5,6,7 */

#define STYLE_VALIGN_UNKNOWN    0x0000
#define STYLE_VALIGN_TOP        0x0001
#define STYLE_VALIGN_MIDDLE	0x0002
#define STYLE_VALIGN_BOTTOM	0x0003
#define STYLE_VALIGN_BASELINE	0x0004
#define STYLE_VALIGN_TEXTTOP	0x0005
#define STYLE_VALIGN_ABSBOTTOM	0x0006
#define STYLE_VALIGN_ABSMIDDLE	0x0007

#define STYLE_VALIGN_MASK       0x0007
#define STYLE_VALIGN_SHIFT      5

#define rid_sf_VALIGN_UNKNOWN   ( STYLE_VALIGN_UNKNOWN << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_TOP       ( STYLE_VALIGN_TOP << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_MIDDLE    ( STYLE_VALIGN_MIDDLE << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_BOTTOM    ( STYLE_VALIGN_BOTTOM << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_BASELINE  ( STYLE_VALIGN_BASELINE << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_TEXTTOP   ( STYLE_VALIGN_TEXTTOP << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_ABSBOTTOM ( STYLE_VALIGN_ABSBOTTOM << STYLE_VALIGN_SHIFT )
#define rid_sf_VALIGN_ABSMIDDLE ( STYLE_VALIGN_ABSMIDDLE << STYLE_VALIGN_SHIFT )

/*****************************************************************************/

/* 8,9 */

#define STYLE_SUB		0x0001
#define STYLE_SUB_MASK		0x0001
#define STYLE_SUB_SHIFT		8
#define rid_sf_SUB		( STYLE_SUB << STYLE_SUB_SHIFT )

#define STYLE_SUP		0x0001
#define STYLE_SUP_MASK		0x0001
#define STYLE_SUP_SHIFT		9
#define rid_sf_SUP		( STYLE_SUP << STYLE_SUP_SHIFT )

/* 10 RESERVED !! (Borris might have a use for it during progressive rendering) */
#define STYLE_RESERVED			0x0001
#define STYLE_RESERVED_MASK		0x0001
#define STYLE_RESERVED_SHIFT		10
/*#define rid_sf_ADJUSTED_SIZE		( RESERVED_SIZE << RESERVED_SIZE_SHIFT )*/
#define rid_sf_RESERVED			( RESERVED_SIZE << RESERVED_SIZE_SHIFT )

/* 11 strikethrough */

#define STYLE_STRIKE            0x0001
#define STYLE_STRIKE_MASK       0x0001
#define STYLE_STRIKE_SHIFT      11
#define rid_sf_STRIKE		( STYLE_STRIKE << STYLE_STRIKE_SHIFT )

/* 12.15 pdh: colour... zero means page's fg colour */

#define STYLE_COLOURNO_MASK     0x000F
#define STYLE_COLOURNO_SHIFT    12

/*****************************************************************************/

#if 0
/* Keep theses definitions in step with webfonts.h */

/* Just the style bits (bold/italic/fixed) */

#define STYLE_WF_STYLE_MASK     0x0007
#define STYLE_WF_STYLE_SHIFT    16

/* Just the font size */

#define STYLE_WF_SIZE_MASK      0x00F8
#define STYLE_WF_SIZE_SHIFT     (16+3)

#endif

/* All the font data */

#define STYLE_WF_INDEX_MASK     0x00ff
#define STYLE_WF_INDEX_SHIFT    16

#define STYLE_INDENT_MASK       0x00ff
#define STYLE_INDENT_SHIFT      24

/*****************************************************************************/

typedef struct {
    unsigned short flags;	/* Flags */
    unsigned char wf_index;     /* Index to the font to use */
    unsigned char indent;       /* Spaces to indent by */
} ROStyle;

/* Backwards compatibility #define */
#define rid_sf_CENTERED         rid_sf_ALIGN_CENTER

/* Get the whole lot at once */
#define ROSTYLE_MASK            (0xffffffffu)
#define ROSTYLE_SHIFT           0

#define GET_ROSTYLE(v)		*((BITS*)&(v)) = UNPACK(me->sgmlctx->tos->effects_active, ROSTYLE)

/*****************************************************************************

Second word - other items

*/

#define TABLE_ALIGN_CHAR_MASK   0x000000ff
#define TABLE_ALIGN_CHAR_SHIFT  32

/* 8..11 list types */

#define LIST_ITEM_TYPE_MASK     0x0000000f
#define LIST_ITEM_TYPE_SHIFT    40

/* 12..19 pdh Right-hand indent */

#define STYLE_RINDENT_MASK      0x000000ff
#define STYLE_RINDENT_SHIFT     44

/* 20..27 spare */

#define LANG_NUM_MASK		0x000000ff
#define LANG_NUM_SHIFT		52

/* 28..31 spare */

/* Third word - list item number */

#define LIST_ITEM_NUM_MASK      0xffffffff
#define LIST_ITEM_NUM_SHIFT     64

/*****************************************************************************/

/* Group ID values */

enum
{
    HTML_GROUP_NONE,
    HTML_GROUP_TABLE,
    HTML_GROUP_COLGROUP,
    HTML_GROUP_FORM,
    HTML_GROUP_HEAD,
    HTML_GROUP_LIST,
    HTML_GROUP_HTML,
    HTML_GROUP_SELECT,
    HTML_GROUP_BODY
};


/* The parser needs to know some information at compile time */

#define SGML_MAXIMUM_ATTRIBUTES		24
#define words_of_elements_bitpack	4
#define words_of_effects_bitpack	3

#endif /* __glue_h */

/* eof glue.h */
