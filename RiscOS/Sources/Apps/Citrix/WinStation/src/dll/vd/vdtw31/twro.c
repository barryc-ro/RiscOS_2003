/* > twro.c

 * RISCOS Thinwire routines

 */

#include "windows.h"
#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "swis.h"

#include "../../../app/version.h"
#include "../../../app/utils.h"

#include "../../../inc/client.h"
#include "../../../inc/debug.h"
#include "../../../inc/mouapi.h"

#include "../../../inc/clib.h"

#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"

#include "wfglobal.h"

/* ---------------------------------------------------------------------------------------------------- */

#define OBJ_PEN             1
#define OBJ_BRUSH           2
#define OBJ_DC              3	// currently this is the screen, but it could mean window in future
//#define OBJ_METADC          4
#define OBJ_PAL             5
//#define OBJ_FONT            6
#define OBJ_BITMAP          7
#define OBJ_REGION          8
//#define OBJ_METAFILE        9
#define OBJ_MEMDC           10	// non-visible dc attached to a bitmap
//#define OBJ_EXTPEN          11
//#define OBJ_ENHMETADC       12
//#define OBJ_ENHMETAFILE     13
#define OBJ_CURSOR     14

#define NULL_COLOUR		0xFF000000
#define TRANSPARENT_COLOUR	(-1)
#define UNSET_COLOUR		(-2)

#define UNSET_COLTRANS		((void *)-1)

#define BRUSH_WIDTH		8
#define BRUSH_HEIGHT		8

#define RoundShort(a)		(((a) + 1) &~ 1)
#define RoundLong(a)		(((a) + 3) &~ 3)

/* ---------------------------------------------------------------------------------------------------- */

typedef struct /* Format of a sprite area control block */
{
  int size;
  int number;
  int sproff;
  int freeoff;
} sprite_area;

typedef struct /* Format of a sprite header */
{
  int  next;      /*  Offset to next sprite                */
  char name[12];  /*  Sprite name                          */
  int  width;     /*  Width in words-1      (0..639)       */
  int  height;    /*  Height in scanlines-1 (0..255/511)   */
  int  lbit;      /*  First bit used (left end of row)     */
  int  rbit;      /*  Last bit used (right end of row)     */
  int  image;     /*  Offset to sprite image               */
  int  mask;      /*  Offset to transparency mask          */
  int  mode;      /*  Mode sprite was defined in           */
                  /*  Palette data optionally follows here */
                  /*  in memory                            */
} sprite_header;

typedef struct
{
    sprite_area *area;

    int width, height, bpp;
} sprite_descr;

typedef union
{
    struct
    {
	char reserved;
	char red;
	char green;
	char blue;
    } b;
    int w;
} ropalette;

typedef struct
{
    char invert_P;
    char invert_D;
    char action;
} action_info;

/* ---------------------------------------------------------------------------------------------------- */

typedef struct region_rect region_rect;
typedef struct gdi_hdr gdi_hdr;

struct region_rect
{
    region_rect *next;
    RECT rect;
};

struct gdi_hdr
{
    int tag;
    void *next;
};

struct DC__
{
    gdi_hdr gdi;

    struct
    {
	HPEN pen;		// currently selected objects in this context

	HBRUSH brush;
	POINT brush_org;

	HBITMAP bitmap;		// bitmap that represents the drawing surface, MEMDC only
	HBITMAP bitmap_placeholder;

	POINT pen_pos;
	COLORREF bk_col;
	COLORREF text_col;
	int rop2;

	HRGN clip;
	HPALETTE palette;

	int syspal;		// are we using systemn reserved palette entries?


	int planes;		// always 1
	int bpp;
	int width;		// in pixels
	int height;

	int line_length;	// in bytes
	void *display_start;

	int mode;		// mode number or descriptor for this mode
	void *save_area;	// save area for switching output

	struct
	{
	    int pen_col;	// colour number corresponding to pen_col
	    int text_col;	// colour number corresponding to text_col
	    int bk_col;		// colour number corresponding to bk_col

	    int brush_solid;	// colour number corresponding to solid brush (if any)
	    
	    action_info action;	// action corresponding to ROP2
	} local;

	struct
	{
	    int fg_col;		// actual colours in use
	    int bg_col;
	    int action;		// actual action in use

	    ropalette *palette;	// 1 word per col palette in use
	    void *brush_coltrans;

	    RECT clip;
	} current;
    } data;
} DC;

struct RGN__
{
    gdi_hdr gdi;

    struct
    {
	region_rect *base;
    } data;
} region;

struct PALETTE__
{
    gdi_hdr gdi;
    struct
    {
	int n_entries;
	LPPALETTEENTRY colours;
    } data;
};

struct PEN__
{
    gdi_hdr gdi;
    struct
    {
	COLORREF colref;
	int style;
	int width;
    } data;
};

struct BRUSH__
{
    gdi_hdr gdi;

    struct
    {
	int solid;		// is it solid or patterned
	COLORREF colour;
	sprite_descr sprite;
	int color_table_type;
    } data;
};

struct BITMAP__
{
    gdi_hdr gdi;

    struct
    {
	sprite_descr sprite;
	int color_table_type;
    } data;
};

struct CURSOR__
{
    gdi_hdr gdi;

    struct
    {
	POINT hotspot;
	sprite_descr sprite;
    } data;
} cursor;

struct GDIOBJ__
{
    gdi_hdr gdi;
};

/* ---------------------------------------------------------------------------------------------------- */

/* convert from Windows coordinates (origin top left, positive downwards) to
 * RISC OS coordinates (origin bottom left, positive upwards, 2 units per pixel)

 * Note that in windows RECT's the bottom and right coordinates are exclusive

 */

#define cvtx(dc, x)	((x)*2)
#define cvty(dc, y)	(((dc)->data.height - 1 - (y))*2)

#ifdef DEBUG
#define is_screen(dc)	(((dc) == screen_dc) ? " (screen)" : "")
#endif

/* ---------------------------------------------------------------------------------------------------- */

#define plotaction_WRITE	0
#define plotaction_OR		1
#define plotaction_AND		2
#define plotaction_XOR		3
#define plotaction_INVERT	4
#define plotaction_NOP		5
#define plotaction_ANDNOT	6
#define plotaction_ORNOT	7

/* ---------------------------------------------------------------------------------------------------- */

/*

Bitmap format
-------------

Scanlines are top to bottom.
First pixel is in MSB of byte
Lines are short (1 byte) aligned
Left hand wastage is not allowed
Can be planar or scalar

May be assumptions that
  16 color is planar
  256 color is scalar
Specifically cache size calculations
  
DIB format
----------

Scanlines are bottom to top
First pixel is in MSB of byte
Lines are word (4 byte) aligned
Left hand wastage is allowed
Must be scalar
coord system is origin at bottom-left
1,4,8,24 bpp supported. 24bpp is triplets of bytes.

 */

/* ---------------------------------------------------------------------------------------------------- */

#define REVERSE_WORD(p)	((p << 24) | ((p << 8) & 0x00FF0000) | ((p >> 8) & 0x0000FF00) | (0 /*p >> 24*/))

//#define DISABLE_CURSOR "\23\1\0\0\0\0\0\0\0\0"
//#define DISABLE_CURSOR	"\23\0\10\x20\0\0\0\0\0\0"
#define DISABLE_CURSOR	"\5"

#define MOUSE_COLOURS "\x13\x1\x19\x00\x00\x00" "\x13\x2\x19\xff\xff\xff" "\x13\x3\x19\x00\x00\x00"

#define first_sprite(area)	((sprite_header *)((char *)(area) + (area)->sproff))
#define sprite_data(sprite)	((char *)(sprite) + (sprite)->image)
#define sprite_palette(sprite)	(ropalette *)((char *)(sprite) + sizeof(sprite_header))

struct os_mode_selector
{
    int flags;
    int xres;
    int yres;
    int log2_bpp;
    int frame_rate;
};

#define vduvar_XEig	4
#define vduvar_YEig	5
#define vduvar_LineLength	6
#define vduvar_Log2BPP	9
#define vduvar_XLimit	11
#define vduvar_YLimit	12

#define modevar_DisplayStart 149

/* ---------------------------------------------------------------------------------------------------- */

typedef struct
{
    int xSrc;
    int ySrc;
    int xDest;
    int yDest;
    int Width;
    int Height;
} bitblt_info;

typedef struct
{
    /* set by line_ptr() */
    unsigned *data;
    int bpp;

    /* set by line_init() */
    const char *coltrans;
    int ppw;			// pixels per word
    unsigned mask;
    unsigned solid_colour;

    /* set by line_init() and updated across line */
    unsigned *ptr;
    int phase;			// original bit offset of bit 0 of word/word_out
    unsigned word;	        // current word being read
    unsigned word_out;		// current word to write
} line_info;

typedef unsigned (*pp_function)(unsigned d, unsigned s, unsigned p);

/* rop3 defines */

#define EPS_OFF		3	/* 0000000000000011	Offset within parse string */
#define EPS_INDEX	0x1C	/* 0000000000011100	Parse string index */
#define LogPar		0x20	/* 0000000000100000	(1 indicates implied NOT as Logop6) */
/*#define LogOp1		   0000000011000000	Logical Operation #1 */
/*#define LogOp2		   0000001100000000	Logical Operation #2 */
/*#define LogOp3		   0000110000000000	Logical Operation #3 */
/*#define LogOp4		   0011000000000000	Logical Operation #4 */
/*#define LogOp5		   1100000000000000	Logical Operation #5 */
#define EPS_INDEX_SHIFT	2

#define LogOpShift	6
#define LogOpCount	5

#define OpSpec	0		  /* Special Operand as noted below */
#define OpSrc 	1		  /* Operand is source field */
#define OpDest	2		  /* Operand is destination field */
#define OpPat 	3		  /* Operand is pattern field */

#define OpRes	4
#define OpOut   5
#define OpDone	6		/* No more operands */

#define OpMask	3

#define LogNOT	0			/* NOT result */
#define LogXOR	1			/* XOR result with next operand */
#define LogOR	2			/* OR  result with next operand */
#define LogAND	3			/* AND result with next operand */

#define LogMask	3

#define LogBIC	4			/* not used in protocol but can be used in code */
#define LogMOV	5			/* not used in protocol but can be used in code */

/* register definitions */

#define rn_DEST		0
#define rn_SRC		1
#define rn_PAT		2
#define rn_RESULT	3
#define rn_SPECIAL	12
#define rn_LR		14
#define rn_PC		15

#define rn_OUTPUT	0

/* arm assembler defines */

#define code_ALWAYS	0xE0000000

#define code_AND	(0 << 21)
#define code_EOR	(1 << 21)
#define code_ORR	(0xC << 21)
#define code_MOV	(0xD << 21)
#define code_BIC	(0xE << 21)
#define code_MVN	(0xF << 21)

#define OP1(a)	((a) << 16)
#define DST(a)	((a) << 12)
#define OP2(a)	(a)

/* some prebuilt instructions */

#define code_MVN_output_result		(code_ALWAYS | code_MVN | DST(rn_OUTPUT) | OP2(rn_RESULT))
#define code_MOV_output_result		(code_ALWAYS | code_MOV | DST(rn_OUTPUT) | OP2(rn_RESULT))
#define code_MVN_result_result		(code_ALWAYS | code_MVN | DST(rn_RESULT) | OP2(rn_RESULT))
#define code_MOV_special_result		(code_ALWAYS | code_MOV | DST(rn_SPECIAL) | OP2(rn_RESULT))
#define code_MOV_pc_lr			(code_ALWAYS | code_MOV | DST(rn_PC) | OP2(rn_LR))

/* ---------------------------------------------------------------------------------------------------- */


static HGDIOBJ gdi_objects = NULL;
static HGDIOBJ stock_objects[STOCK_LAST] = { 0 };
static HDC current_dc = NULL;
static HDC screen_dc = NULL;
static HCURSOR screen_cursor = NULL;
static unsigned code_array[32];		// max operands is 15*3 + 2

/*
 * Decompose ROP2 actions into simpler form.
 * Invert Pattern can be done on colour set.
 * Invert destination can be done by a pre-pass
  
R2_BLACK            0		0	CPY
R2_NOTMERGEPEN      DPon	~P ~D	AND
R2_MASKNOTPEN       DPna	~P  D	AND
R2_NOTCOPYPEN       PN		~P	CPY
R2_MASKPENNOT       PDna	 P  ~D  AND
R2_NOT              Dn		    ~D  CPY
R2_XORPEN           DPx		 P   D  XOR
R2_NOTMASKPEN       DPan	~P  ~D  OR
R2_MASKPEN          DPa		 P   D  AND
R2_NOTXORPEN        DPxn	~P  ~D  XOR
R2_NOP              D		     D  CPY
R2_MERGENOTPEN      DPno	~P   D  OR
R2_COPYPEN          P		 P	CPY
R2_MERGEPENNOT      PDno	 P  ~D  OR
R2_MERGEPEN         DPo		 P   D  OR
R2_WHITE            1		1	CPY
 */

static action_info rop2_to_action[16] =
{
    { 0, 0, plotaction_WRITE },		// R2_BLACK,
    { 1, 1, plotaction_AND },		// R2_NOTMERGEPEN, 
    { 0, 0, plotaction_ANDNOT },	// R2_MASKNOTPEN,
    { 1, 0, plotaction_WRITE },		// R2_NOTCOPYPEN,
    { 0, 1, plotaction_AND },		// R2_MASKPENNOT,
    { 0, 0, plotaction_INVERT },	// R2_NOT,
    { 0, 0, plotaction_XOR },		// R2_XORPEN,
    { 1, 1, plotaction_OR },		// R2_NOTMASKPEN,
    { 0, 0, plotaction_AND },		// R2_MASKPEN,
    { 1, 1, plotaction_XOR },		// R2_NOTXORPEN,
    { 0, 0, plotaction_NOP },		// R2_NOP,
    { 0, 0, plotaction_ORNOT },		// R2_MERGENOTPEN,
    { 0, 0, plotaction_WRITE },		// R2_COPYPEN,
    { 0, 1, plotaction_OR },		// R2_MERGEPENNOT,
    { 0, 0, plotaction_OR },		// R2_MERGEPEN,
    { 0, 0, plotaction_WRITE }		// R2_WHITE
};

static int ParseStrings[] =
{
    0x07AAA,		/* src,pat,dest,dest,dest,dest,dest,dest */
    0x079E7,		/* src,pat,dest,src,pat,dest,src,pat */
    0x06DB6,		/* src,dest,pat,src,dest,pat,src,dest */
    0x0AAAA,		/* dest,dest,dest,dest,dest,dest,dest,dest */
    0x0AAAA,		/* dest,dest,dest,dest,dest,dest,dest,dest */
    0x04725,		/* src,spec,src,pat,spec,dest,src,src */
    0x04739,		/* src,spec,src,pat,spec,pat,dest,src */
    0x04639		/* src,spec,src,dest,spec,pat,dest,src */
};

#ifdef DEBUG

static char *operand_name[] =
{
    "SPE",
    "SRC",
    "DST",
    "PAT",
    "RES",
    "OUT",
    "OVR"
};

static char *logop_name[] =
{
    "NOT",
    "XOR",
    "OR",
    "AND",
    "BIC",
    "MOV"
};

static const char *objects[] =
{
    "<null>",
    "pen",
    "brush",
    "dc",
    "metadc",
    "palette",
    "font",
    "bitmap",
    "region",
    "metafile",
    "memdc",
    "extpen",
    "enhmetdc",
    "enmetafile",
    "cursor"
};

#endif

/* ---------------------------------------------------------------------------------------------------- */

static int vduval(int var)
{
    int val[3];

    val[0] = var;
    val[1] = -1;

    LOGERR(_swix(OS_ReadVduVariables, _INR(0,1), val, val + 2));

    return val[2];
}

/*
 * Get a sprite type corresponding to the current mode
 */

static int build_mode_number(int log2bpp)
{
    return 1 + (log2bpp+1)*0x08000000 +
	(180 >> vduval(vduvar_YEig))*0x00004000 +
	(180 >> vduval(vduvar_XEig))*0x00000002;
}

static int get_mode_number(int log2bpp)
{
    int mode;

    /* Else try and read screen mode - new way (3.5) */
    if (_swix(OS_ScreenMode, _IN(0) | _OUT(1), 1, &mode) != NULL)
    {
	/* Else try and read screen mode - old way (3.1) */
        LOGERR(_swix(OS_Byte, _IN(0) | _OUT(2), 135, &mode));
    }

    /* If new style mode must use new style sprites (3.5) */
    if ((unsigned)mode > 255)
    {
        struct os_mode_selector *m = (struct os_mode_selector *)(long)mode;
	mode = build_mode_number(m->log2_bpp);
    }

    return mode;
}

/* ---------------------------------------------------------------------------------------------------- */

#ifdef DEBUG

static void save_sprite(const sprite_descr *descr, const char *name)
{
    sprite_header *sprite;
    char filename[256];
    int index;

    DTRACE((TC_TW, TT_TW_res4, "save_sprite: in: descr %p name '%s'", descr, name));

    if (descr->area == NULL)
	return;
    
    sprite = first_sprite(descr->area);

    DTRACE((TC_TW, TT_TW_res4, "save_sprite: sprite %p", sprite));
    DTRACE((TC_TW, TT_TW_res4, "save_sprite: name '%.12s'", sprite->name));

    index = atoi(sprite->name);
    
    sprintf(filename, "<Wimp$ScrapDir>." APP_NAME ".%02d", index / 75);
    DTRACE((TC_TW, TT_TW_res4, "save_sprite: mkdir '%s'", filename));
    mkdir(filename);

    sprintf(filename, "<Wimp$ScrapDir>." APP_NAME ".%02d.%03d%s", index / 75, index, name);
    DTRACE((TC_TW, TT_TW_res4, "save_sprite: save %p to '%s'", descr->area, filename));
    _swix(OS_SpriteOp, _INR(0,2), 12 + 256, descr->area, filename);

    DTRACE((TC_TW, TT_TW_res4, "save_sprite: out"));
}
#else
#define save_sprite(a,b)
#endif

#if 0
static int getlog2bpp(int bpp)
{
    switch (bpp)
    {
    case 1:
	return 0;
    case 2:
	return 1;
    case 4:
	return 2;
    case 8:
	return 3;
    case 16:
	return 4;
    }
    return 5;
}
#endif

static int getspritemode(int bpp)
{
    switch (bpp)
    {
    case 1:
	return 25;
    case 2:
	return 26;
    case 4:
	return 27;
    case 8:
	return 28;
    }
    return 28;
}

static sprite_descr *get_sprite_descr(HGDIOBJ obj)
{
    sprite_descr *sprite = NULL;
    switch (obj->gdi.tag)
    {
    case OBJ_BITMAP:
	sprite = &((HBITMAP)obj)->data.sprite;
	break;
    case OBJ_CURSOR:
	sprite = &((HCURSOR)obj)->data.sprite;
	break;
    }
    return sprite;
}

static create_sprite_index = 1;

static int create_sprite(sprite_descr *descr, int w, int h, int bpp, BOOL palette)
{
    sprite_area *area;
    sprite_header *sprite;

    int bit_width = w * bpp;
    int word_width = (bit_width + 31)/32;
    int word_width_wastage = ((32 - bpp) + bit_width + 31)/32; // max left hand wastage + sprite + round up to a word (to allow for grabbing from screen)
    int pal_size = palette ? (8 << bpp) : 0;

    int size = sizeof(sprite_area) +
	sizeof(sprite_header) +
	pal_size +
	word_width_wastage * 4 * h;

    ASSERT(descr, 0);

    TRACE((TC_TW, TT_TW_res4, "create_sprite: %d @ %d x %d @ %d bpp palette %d size %d", create_sprite_index, w, h, bpp, palette, size));
    
    area = malloc(size);
    if (area)
    {
	char buf[12];
	area->size = size;		// area size is total size malloced including space for wastage

	size = sizeof(sprite_area) +	// recalculate size to use as the actual size of the sprite
	    sizeof(sprite_header) +
	    pal_size +
	    word_width * 4 * h;

	area->number = 1;
	area->sproff = sizeof(sprite_area);
	area->freeoff = size;

	sprite = first_sprite(area);
	sprite->next = size - sizeof(sprite_area);

	sprintf(buf, "%dbitmap", create_sprite_index++);
	strncpy(sprite->name, buf, sizeof(sprite->name));

	sprite->width = word_width - 1;
	sprite->height = h - 1;

	sprite->lbit = 0;
	sprite->rbit = (bit_width - 1) % 32;

	sprite->image = sizeof(sprite_header) + pal_size;
	sprite->mask = sprite->image;

	sprite->mode = getspritemode(bpp);

	DTRACEBUF((TC_TW, TT_TW_res4, area, sizeof(*area) + sizeof(*sprite)));
    }    

    descr->width = w;
    descr->height = h;
    descr->bpp = bpp;
    descr->area = area;

    return area != NULL;
}

static void copy_1bit(char *out, const char *in, int npixels)
{
    int pixel;
    for (pixel = 0; pixel < npixels; pixel += 8)
    {
	int p = *in++, q = 0;
	if (p & 1)
	    q |= 0x80;
	if (p & 2)
	    q |= 0x40;
	if (p & 4)
	    q |= 0x20;
	if (p & 8)
	    q |= 0x10;
	if (p & 0x10)
	    q |= 8;
	if (p & 0x20)
	    q |= 4;
	if (p & 0x40)
	    q |= 2;
	if (p & 0x80)
	    q |= 1;
	*out++ = q;
    }
}

static void copy_2bit(char *out, const char *in, int npixels)
{
    int pixel;
    for (pixel = 0; pixel < npixels; pixel += 2)
    {
	int p = *in++;
	*out++ = (p << 4) | (p >> 4);
    }
}

static void copy_line_to_sprite(char *out, const char *in, int bpp, int npixels)
{
    int pixel;
    switch (bpp)
    {
    case 1:
	copy_1bit(out, in, npixels);
	break;

    case 4:
	copy_2bit(out, in, npixels);
	break;

    case 8:
	memcpy(out, in, npixels);
	break;

    case 24:
	for (pixel = 0; pixel < npixels; pixel++)
	{
	    int r = *in++;
	    int g = *in++;
	    int b = *in++;
	    int w = (b << 16) | (g << 8) | r;

	    *(int *)out = w;
	    out += 4;
	}
	break;
    }
}

static void copy_line_from_sprite(char *out, const char *in, int bpp, int npixels)
{
    int pixel;
    switch (bpp)
    {
    case 1:
	copy_1bit(out, in, npixels);
	break;

    case 4:
	copy_2bit(out, in, npixels);
	break;

    case 8:
	memcpy(out, in, npixels);
	break;

    case 24:
	for (pixel = 0; pixel < npixels; pixel++)
	{
	    unsigned w = *(unsigned *)in;

	    *out++ = w;
	    *out++ = w >> 8;
	    *out++ = w >> 16;

	    in += 4;
	}
	break;
    }
}

static void fill_sprite(sprite_descr *descr, const void *bits, int in_line_length)
{
    sprite_header *sprite = first_sprite(descr->area);
    int out_line_length = (sprite->width + 1) * 4;
    char *out = sprite_data(sprite);
    const char *in = (const char *)bits;
    int line;

    for (line = 0;
	 line < descr->height;
	 line++, out += out_line_length, in += in_line_length)
    {
	copy_line_to_sprite(out, in, descr->bpp, descr->width);
    }

    ASSERT((descr->bpp == 1), descr->bpp);
}

#if 0
static void clear_sprite(sprite_descr *descr, int val)
{
    sprite_header *sprite = first_sprite(descr->area);
    memset((char *)sprite + sprite->image, val, sprite->next - sprite->image);
}

static void set_sprite_palette(sprite_descr *descr, const ropalette *palette)
{
    sprite_header *sprite = first_sprite(descr->area);
    int n_entries = (sprite->image - sizeof(sprite_header)) / 8;
    ropalette *pal = sprite_palette(sprite);
    int i;

    for (i = 0; i < n_entries; i++)
    {
	pal[1] = pal[0] = *palette++;
	pal += 2;
    }
}
#endif

/*
 * For this call the coordinates are already converted to OS coords, bottom left
 */

static void fill_sprite_from_screen(sprite_descr *descr, int x, int y)
{
    char buf[12];

    strcpy(buf, first_sprite(descr->area)->name);
    
    // remove any sprite definition in there already
    descr->area->number = 0;
    descr->area->freeoff = descr->area->sproff;

    // grab sprite from screen
    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
	  16 + 256,
	  descr->area, buf,
	  0,	// no palette
	  x, y,
	  x + descr->width*2 - 1, y + descr->height*2 - 1));

    DTRACE((TC_TW, TT_TW_res3, "fill_sprite_from_screen: descr %p from OS %dx%d to %dx%d",
	   descr, x, y,
	   x + descr->width*2 - 1,
	   y + descr->height*2 - 1));
//  save_sprite(descr, "grab");
}

/*
 * Copy betwen ropalette and PALETTEENTRY
 */

static void copy_palette_to_riscos(ropalette *to, LPPALETTEENTRY from, int n)
{
    int i;
    for (i = 0; i < n; i++, to++, from++)
	to->w = (*(unsigned *)from) << 8;
}

static void copy_palette_from_riscos(LPPALETTEENTRY to, const ropalette *from, int n)
{
    int i;
    for (i = 0; i < n; i++, to++, from++)
	*(unsigned *)to = from->w >> 8;
}

/* ---------------------------------------------------------------------------------------------------- */

static int dib_pal_size(const BITMAPINFOHEADER *info)
{
    return info->biBitCount > 8 ? 0 :
	info->biSize >= 40 && info->biClrUsed ? (int)info->biClrUsed :
	1 << (int)info->biBitCount;
}

/*
 * This routine can cope when the sprite is smaller than the DIB (as in the brush case).
 */

static void fill_sprite_from_dib(sprite_descr *descr, CONST BITMAPINFOHEADER *info, const void *bits, int color_table_type)
{
    sprite_header *sprite = first_sprite(descr->area);
    const char *in_data;
    char *out_data;
    int in_line_length, out_line_length;
    int line;

    in_data = (bits ? (const char *)bits :
	       (const char *)info + info->biSize +
	       dib_pal_size(info) * ( color_table_type == DIB_PAL_COLORS ? sizeof(WORD) : sizeof(RGBQUAD) )
	      );

    in_line_length = (((int)info->biWidth * info->biBitCount + 31) / 32) * 4;

    out_data = sprite_data(sprite);
    out_line_length = (sprite->width + 1) * 4;

    // start at last (ie top) line
    in_data += (info->biHeight - 1) * in_line_length;
    
    for (line = 0;
	 line < descr->height;
	 line++, in_data -= in_line_length, out_data += out_line_length)
    {
	copy_line_to_sprite(out_data, in_data, descr->bpp, descr->width);
    }
}

static void fill_dib_from_sprite(sprite_descr *descr, PBITMAPINFO info, void *bits, int start, int n_lines)
{
    sprite_header *sprite = first_sprite(descr->area);
    const char *in_data;
    char *out_data;
    int line_length;
    int line;

    in_data = sprite_data(sprite);
    line_length = (sprite->width + 1) * 4;

    out_data = bits;

    // offset to start line
    in_data += start * line_length;
//  out_data += (descr->height - 1 - start) * line_length;
    out_data += (n_lines - 1) * line_length;
    
    // copy data
    for (line = 0;
	 line < n_lines;
	 line++, in_data += line_length, out_data -= line_length)
    {
	copy_line_from_sprite(out_data, in_data, descr->bpp, descr->width);
    }
}

static void fill_palette_from_sprite(sprite_descr *descr, CONST BITMAPINFO *info, int color_table_type)
{
    sprite_header *sprite = first_sprite(descr->area);
    ropalette *in_pal;
    char *out_pal;
    int in_pal_size;
    int i;

    out_pal = ((char *)info + info->bmiHeader.biSize);

    in_pal = sprite_palette(sprite);
    in_pal_size = (sprite->image - sizeof(sprite_header)) / (2*sizeof(ropalette));

    DTRACE((TC_TW, TT_TW_res4, "fill_palette_from_sprite: in_pal_size %d out_pal_size %d", in_pal_size, out_pal_size));
    
    for (i = 0; i < in_pal_size; i++)
    {
	ropalette q = *in_pal;
	in_pal += 2;
	
	if (color_table_type == DIB_RGB_COLORS)
	{
	    *(unsigned *)out_pal = REVERSE_WORD(q.w);
	    out_pal += 4;
	}
	else
	{
	    *(unsigned short *)out_pal = q.w;
	    out_pal += 2;
	}
    }
}

static void fill_palette_from_dib(sprite_descr *descr, CONST BITMAPINFOHEADER *hdr, CONST BITMAPINFO *info, int color_table_type)
{
    sprite_header *sprite = first_sprite(descr->area);
    char *in_pal;
    ropalette *out_pal;
    int in_pal_size, out_pal_size;
    int i;

    in_pal = ((char *)info + info->bmiHeader.biSize);
    in_pal_size = dib_pal_size(hdr);

    out_pal = (ropalette *)((char *)sprite + sizeof(sprite_header));
    out_pal_size = (sprite->image - sizeof(sprite_header)) / (2*sizeof(ropalette));

    TRACE((TC_TW, TT_TW_res1, "fill_palette_from_dib: in_pal_size %d out_pal_size %d", in_pal_size, out_pal_size));
    
    for (i = 0; i < in_pal_size; i++)
    {
	unsigned q;
	if (color_table_type == DIB_RGB_COLORS)
	{
	    q = *(unsigned *)in_pal;
	    q = REVERSE_WORD(q);
	    in_pal += 4;
	}
	else
	{
	    q = *(unsigned short *)in_pal;
	    in_pal += 2;
	}

	out_pal[0].w = q;
	out_pal[1].w = q;
	out_pal += 2;
    }

    i = out_pal_size - in_pal_size;
    TRACE((TC_TW, TT_TW_res1, "fill_palette_from_dib: clear %d bytes", i));
    if (i)
	memset(out_pal, 0, 2*sizeof(unsigned)*i);
}

/* ---------------------------------------------------------------------------------------------------- */

static void free_region(HRGN rgn)
{
    region_rect *r = rgn->data.base;
    while (r)
    {
	region_rect *next = r->next;
	free(r);
	r = next;
    }
    rgn->data.base = NULL;
}

// link into chain
static void link_region(region_rect **head, region_rect **last, region_rect *n)
{
    if (*last)
	(*last)->next = n;
    else
	(*head) = n;
    (*last) = n;
}

/* ---------------------------------------------------------------------------------------------------- */

static void unlink_object(HGDIOBJ to_unlink)
{
    HGDIOBJ obj, prev;

    DTRACE((TC_TW, TT_TW_res4, "unlink_object: %p gdiobj %p", to_unlink, gdi_objects));

    for (obj = gdi_objects, prev = NULL; obj; prev = obj, obj = obj->gdi.next)
    {
	DTRACE((TC_TW, TT_TW_res4, "unlink_object: obj %p prev %p", obj, prev));

	if (obj == to_unlink)
	{
	    if (prev)
		prev->gdi.next = to_unlink->gdi.next;
	    else
		gdi_objects = to_unlink->gdi.next;
	    break;
	}
    }

    DTRACE((TC_TW, TT_TW_res4, "unlink_object: gdiobj %p", gdi_objects));
}

static void link_object(HGDIOBJ to_link)
{
    DTRACE((TC_TW, TT_TW_res4, "link_object: %p gdiobj %p", to_link, gdi_objects));

    to_link->gdi.next = gdi_objects;
    gdi_objects = to_link;
}

static HGDIOBJ create_object(int tag, int size)
{
    HGDIOBJ obj = calloc(size, 1);

    if (obj)
    {
	obj->gdi.tag = tag;
	
	link_object(obj);
    }
    
    TRACE((TC_TW, TT_TW_res4, "create_object: %p tag %d", obj, tag));

    return obj;
}

static void free_object(HGDIOBJ obj)
{
    unlink_object(obj);

    switch (obj->gdi.tag)
    {
    case OBJ_BITMAP:
    {
	HBITMAP bitmap = (HBITMAP)obj;

	free(bitmap->data.sprite.area);
	break;
    }

    case OBJ_BRUSH:
    {
	HBRUSH brush = (HBRUSH)obj;

	free(brush->data.sprite.area);
	break;
    }

    case OBJ_DC:
    case OBJ_MEMDC:
    {
	HDC dc = (HDC)obj;
	free(dc->data.current.palette);
	if (dc->data.current.brush_coltrans != UNSET_COLTRANS)
	    free(dc->data.current.brush_coltrans);
	free(dc->data.save_area);

	DeleteObject(dc->data.bitmap_placeholder);
	break;
    }

    case OBJ_PAL:
    {
	HPALETTE palette = (HPALETTE)obj;
	free(palette->data.colours);
	break;
    }

    case OBJ_REGION:
    {
	HRGN rgn = (HRGN)obj;
	free_region(rgn);
	break;
    }

    case OBJ_CURSOR:
    {
	HCURSOR cursor = (HCURSOR)obj;
	free(cursor->data.sprite.area);
	break;
    }
    }
   
    free(obj);
}

/* ---------------------------------------------------------------------------------------------------- */

static void switch__output(HDC dc)
{
    ASSERT(dc, 0);
    switch (dc->gdi.tag)
    {
    case OBJ_DC:		// switch to screen
	TRACE((TC_TW, TT_TW_res4, "switch__output: dc %p screen", dc));

	LOGERR(_swix(OS_SpriteOp, _INR(0,3), 60, NULL, 0, 1));
	break;

    case OBJ_MEMDC:		// switch to memory
    {
	sprite_descr *descr;
	sprite_area *area;

	TRACE((TC_TW, TT_TW_res4, "switch__output: dc %p", dc));
	ASSERT(dc->data.bitmap, 0);

	descr = get_sprite_descr((HGDIOBJ)dc->data.bitmap);
	area = descr->area;
	ASSERT(area, 0);

	// allocate a save area if there isn't one already
	if (dc->data.save_area == NULL)
	{
	    int size;
	    LOGERR(_swix(OS_SpriteOp, _INR(0,2) | _OUT(3), 62 + 512, area, first_sprite(area), &size));
	    dc->data.save_area = malloc(size);

	    ASSERT(dc->data.save_area, 0);
	    *(int *)dc->data.save_area = 0;
	}

	LOGERR(_swix(OS_SpriteOp, _INR(0,3), 60 + 512, area, first_sprite(area), dc->data.save_area));
	break;
    }

    default:
	ASSERT(0, dc->gdi.tag);
	break;
    }
}

static HDC switch_output(HDC dc)
{
    HDC old_dc = current_dc;
    if (dc != current_dc)
    {
	switch__output(dc);
	current_dc = dc;
    }
    return old_dc;
}

/*
 * Set the default values for a context
 */

static void set_default_dc(HDC dc)
{
    // select palette first as others might be realized
    SelectPalette(dc, (HPALETTE)GetStockObject(DEFAULT_PALETTE), TRUE);
//  RealizePalette(dc);

    SelectObject(dc, GetStockObject(BLACK_PEN));
    SelectObject(dc, GetStockObject(WHITE_BRUSH));

    SetBkColor(dc, PALETTERGB(0xFF, 0xFF, 0xFF));
    SetTextColor(dc, PALETTERGB(0, 0, 0));
    SetROP2(dc, R2_COPYPEN);
}

static void uncache_dc(HDC dc)
{
    // clear cached mode palette
    free(dc->data.current.palette);
    dc->data.current.palette = NULL;

    // cached colour lookup tables
    if (dc->data.current.brush_coltrans != UNSET_COLTRANS)
    {
	free(dc->data.current.brush_coltrans);
	dc->data.current.brush_coltrans = UNSET_COLTRANS;
    }

    // cached colour numbers
    dc->data.local.pen_col = UNSET_COLOUR;
    dc->data.local.text_col = UNSET_COLOUR;
    dc->data.local.bk_col = UNSET_COLOUR;
    dc->data.local.brush_solid = UNSET_COLOUR;
}

static void set_current_dc(HDC dc)
{
    int log2bpp = vduval(vduvar_Log2BPP);

    // set values
    dc->data.planes = 1;
    dc->data.bpp = 1 << log2bpp;
    dc->data.mode = get_mode_number(log2bpp);
    dc->data.width = vduval(vduvar_XLimit) + 1;
    dc->data.height = vduval(vduvar_YLimit) + 1;

    dc->data.line_length = vduval(vduvar_LineLength);
    dc->data.display_start = (void *)vduval(modevar_DisplayStart);
}

static void set_current_dc_from_bitmap(HDC dc)
{
    /* Note the bitmap selected into a MEMDC can in fact be a cursor */
    sprite_descr *sprite = get_sprite_descr((HGDIOBJ)dc->data.bitmap);

    dc->data.bpp = sprite->bpp;
    dc->data.width = sprite->width;
    dc->data.height = sprite->height;
}

/* ---------------------------------------------------------------------------------------------------- */

static int getcolournumber(HDC dc, COLORREF colref)
{
    int colour = -1;

    switch (colref >> 24)
    {
    case 1:			// choose palette entry
	colour = (int)colref & 0x000000ff;
	break;

    case 0:
    case 2:			// choose RGB
	ASSERT(dc->data.current.palette, 0);
	
	LOGERR(_swix(ColourTrans_ReturnColourNumberForMode, _INR(0,2) | _OUT(0),
	      (colref & 0x00ffffff) << 8,
	      dc->data.mode, dc->data.current.palette,
	      &colour));
	break;

    case (NULL_COLOUR >> 24):	// my extension for transparent
	colour = TRANSPARENT_COLOUR;
	break;
	
    default:
	ASSERT(0, (int)colref);
	break;
    }

    return colour;
}

#if 0
static int getrgbval(HDC dc, COLORREF colref)
{
    unsigned colour = (unsigned)-1;

    switch (colref >> 24)
    {
    case 1:			// choose palette entry
	ASSERT(dc->data.current.palette, 0);
	colour = dc->data.current.palette[colref & 0x000000ff].w;
	break;

    case 0:
    case 2:			// choose RGB
	colour = colref << 8;
	colour = REVERSE_WORD(colour);
	break;

    case 255:			// my extension for transparent
	colour = (unsigned)-1;
	break;
	
    default:
	ASSERT(0, (int)colref);
	break;
    }

    return colour;
}
#endif

static void *get_colour_lookup_table(HDC dc, const sprite_descr *descr, int color_table_type)
{
    sprite_area *area;
    sprite_header *spr;
    int i, size;
    void *table = NULL;

    ASSERT(descr, 0);
    ASSERT(descr->area, 0);

    area = descr->area;
    spr = first_sprite(area);

    TRACE((TC_TW, TT_TW_res4, "get_colour_lookup_table: dc %p pal %d type %d bpp %d", dc, spr->image != sizeof(sprite_header), color_table_type, descr->bpp));

    // if there is no palette in this bitmap
    if (spr->image == sizeof(sprite_header))
    {
	// if 1 bpp bitmap should use the text/bk colours
	// others will use no lookup - assume current mode
	if (descr->bpp == 1)
	{
	    size = 1 << descr->bpp;
	    if ((table = malloc(size)) == NULL)
	    {
		ASSERT(0, size);
		return NULL;
	    }

	    // realize text and bk colours if not already
	    if (dc->data.local.text_col == UNSET_COLOUR)
		dc->data.local.text_col = getcolournumber(dc, dc->data.text_col);
	
	    if (dc->data.local.bk_col == UNSET_COLOUR)
		dc->data.local.bk_col = getcolournumber(dc, dc->data.bk_col);
	
	    // fill in lookup table
	    ((char *)table)[0] = dc->data.local.text_col;
	    ((char *)table)[1] = dc->data.local.bk_col;
	}
    }
    else
    {
	// if there is an RGB palette, use colourtrans
	if (color_table_type == DIB_RGB_COLORS)
	{
	    ASSERT(dc->data.current.palette, 0);
	    
	    LOGERR(_swix(ColourTrans_GenerateTable, _INR(0,5) | _OUT(4),
			 area, spr,
			 dc->data.mode, dc->data.current.palette,
			 NULL, 1,
			 &size));
	   
	    if ((table = malloc(size)) == NULL)
	    {
		ASSERT(0, size);
		return NULL;
	    }
    
	    // init table with identity lookup
	    for (i = 0; i < size; i++)
		((char *)table)[i] = i;	    

	    LOGERR(_swix(ColourTrans_GenerateTable, _INR(0,5),
			 area, spr,
			 dc->data.mode, dc->data.current.palette,
			 table, 1));

	    // see if table differs from identity lookup
	    for (i = 0; i < size; i++)
		if (((char *)table)[i] == i)
		    break;

	    // if it does then free it
	    if (i == size)
	    {
		TRACE((TC_TW, TT_TW_res4, "get_colour_lookup_table: RGB identity - discarding"));
		free(table);
		table = NULL;
	    }
	}
	// if there is a palette palette then construct manually
	else
	{
	    const ropalette *in;
	    char *out;
	    int i;
	    BOOL discard;
	    
	    ASSERT(descr->bpp <= 8, descr->bpp);
	    
	    size = 1 << descr->bpp;
	    if ((table = malloc(size)) == NULL)
	    {
		ASSERT(0, size);
		return NULL;
	    }

	    in = sprite_palette(spr);
	    out = table;
	    discard = descr->bpp == dc->data.bpp; // can't get identity if bit depth is different
	    
	    for (i = 0; i < size; i++)
	    {
		if (in->w != i)
		    discard = FALSE;
		
		*out++ = in->w;
		in += 2;
	    }

	    // if end up with identity palette then discard it
	    if (discard)
	    {
		TRACE((TC_TW, TT_TW_res4, "get_colour_lookup_table: PAL identity - discarding"));
		free(table);
		table = NULL;
	    }
	}
    }

#ifdef DEBUG
    if (table)
	TRACEBUF((TC_TW, TT_TW_res4, table, size));
#endif
    
    return table;
}

static void realize_brush(HDC dc)
{
    HBRUSH brush = dc->data.brush;

    ASSERT(brush, 0);
    
    if (brush->data.solid)
    {
	if (dc->data.local.brush_solid == UNSET_COLOUR)
	    dc->data.local.brush_solid = getcolournumber(dc, brush->data.colour);
    }
    else
    {
	if (dc->data.current.brush_coltrans == UNSET_COLTRANS)
	    dc->data.current.brush_coltrans = get_colour_lookup_table(dc, &brush->data.sprite, brush->data.color_table_type);
    }
}

static void set_clip_rect(HDC dc, const RECT *r)
{
    if (r->top != dc->data.current.clip.top ||
	r->left != dc->data.current.clip.left ||
	r->bottom != dc->data.current.clip.bottom ||
	r->right != dc->data.current.clip.right)
    {
	char s[9];
    
	s[0] = 24;
	write_word(s+1, cvtx(dc, (int)r->left));
	write_word(s+3, cvty(dc, ((int)r->bottom - 1)));
	write_word(s+5, cvtx(dc, (int)r->right) - 1);
	write_word(s+7, cvty(dc, (int)r->top) + 1);
	
	_swix(OS_WriteN, _INR(0,1), s, sizeof(s));
   
	TRACE((TC_TW, TT_TW_res3, "set_clip_rect: OS %d,%d %d,%d",
	       cvtx(dc, (int)r->left),
	       cvty(dc, ((int)r->bottom - 1)),
	       cvtx(dc, (int)r->right) - 1,
	       cvty(dc, (int)r->top) + 1));

	dc->data.current.clip = *r;
    }
}

static region_rect *set_clip_full(HDC dc)
{
    static region_rect r;

    r.rect.left = 0;
    r.rect.top = 0;
    r.rect.right = dc->data.width;
    r.rect.bottom = dc->data.height;
    
    set_clip_rect(dc, &r.rect);

    return &r;
}

// assumed switched by now

static BOOL set__clip(HDC dc, HRGN rgn, region_rect **context)
{
    region_rect *r;

    TRACE((TC_TW, TT_TW_res4, "set__clip: dc %p%s rgn %p", dc, is_screen(dc), rgn));

    // if first call then initialise 'r'
    if ( (r = *context) == NULL )
	r = rgn->data.base;
    else
	r = r->next;

    // if no region left then return finish
    if (r == NULL)
	return FALSE;
    
    set_clip_rect(dc, &r->rect);

    // set context to this region
    *context = r;
    
    return TRUE;
}

static BOOL set_clip(HDC dc, region_rect **context)
{
    if (dc->data.clip)
	return set__clip(dc, dc->data.clip, context); 

    if (*context)
	return FALSE;

    *context = set_clip_full(dc);
	
    return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

BOOL MoveToEx(HDC dc, int x, int y, LPPOINT pPoint)
{
    TRACE((TC_TW, TT_TW_res3, "MoveToEx: dc %p%s %dx%d", dc, is_screen(dc), x, y));

    dc->data.pen_pos.x = x;
    dc->data.pen_pos.y = y;

    return 1;
}

BOOL LineTo(HDC dc, int x, int y)
{
    region_rect *context;
    int x0, y0, x1, y1;

    TRACE((TC_TW, TT_TW_res3, "LineTo: dc %p%s %dx%d", dc, is_screen(dc), x, y));

    switch_output(dc);

    // realize the current pen colour
    if (dc->data.local.pen_col == UNSET_COLOUR)
    {
	// get pen col to use bearing in mind overriding rop2 codes
	if (dc->data.rop2 == WHITENESS)
	    dc->data.local.pen_col = 0xff;
	else if (dc->data.rop2 == BLACKNESS)
	    dc->data.local.pen_col = 0x00;
	else
	    dc->data.local.pen_col = getcolournumber(dc, dc->data.pen->data.colref);

	// some tricky plot actions require inverting the foreground colour first
	if (dc->data.local.pen_col == TRANSPARENT_COLOUR)
	    ;
	else if (dc->data.local.action.invert_P)
	    dc->data.local.pen_col = (~dc->data.local.pen_col) & 0xff;
    }
    
    // don't need to set up bgcol as we aren't using any non solid pens
    // update the current colour and action
    if (dc->data.current.fg_col != dc->data.local.pen_col ||
	dc->data.current.action != dc->data.local.action.action)
    {
	dc->data.current.fg_col = dc->data.local.pen_col;
	dc->data.current.action = dc->data.local.action.action;
	
	LOGERR(_swix(OS_SetColour, _INR(0,1), dc->data.current.action, dc->data.current.fg_col));
    }

    x0 = cvtx(dc, dc->data.pen_pos.x);
    y0 = cvty(dc, dc->data.pen_pos.y);
    x1 = cvtx(dc, x);
    y1 = cvty(dc, y);

    context = 0;
    while (set_clip(dc, &context))
    {
	// some tricky plot actions require inverting the destination before plotting
	if (dc->data.local.action.invert_D)
	{
	    LOGERR(_swix(OS_Plot, _INR(0,2), 0+4, x0, y0));	// moveto
	    LOGERR(_swix(OS_Plot, _INR(0,2), 0+6, x1, y1));	// invertto
	}

	LOGERR(_swix(OS_Plot, _INR(0,2), 0+4, x0, y0)); // moveto
	LOGERR(_swix(OS_Plot, _INR(0,2), 0+5, x1, y1)); // drawto
    }

    dc->data.pen_pos.x = x;
    dc->data.pen_pos.y = y;

    return 1;
}

/* ---------------------------------------------------------------------------------------------------- */

/*
 * Return either the original bitmap or one grabbed from the screen
 */

static HBITMAP get_relevant_bitmap(HDC dc, bitblt_info *info, BOOL *delete_after)
{
    HBITMAP b = dc->data.bitmap;

    if (b == NULL)
    {
	TRACE((TC_TW, TT_TW_res3, "get_relevant_bitmap: dc %p%s from OS %dx%d", dc, is_screen(dc), 
				cvtx(dc, info->xSrc),
				cvty(dc, info->ySrc + info->Height - 1)
	    ));

	switch_output(dc);
	set_clip_full(dc);	// force it to be full screen
	
	// grab a sprite
	b = CreateCompatibleBitmap(dc, info->Width, info->Height);

	fill_sprite_from_screen(&b->data.sprite,
				cvtx(dc, info->xSrc),
				cvty(dc, info->ySrc + info->Height - 1));
	
	// mark to be deleted
	*delete_after = TRUE;

	// reset the offsets
	info->xSrc = info->ySrc = 0;
    }

    return b;
}

/*
 * Assume no scaling or colour mapping
 */

static void plotbitmap(HDC dc, HBITMAP bitmap, const bitblt_info *info, int action)
{
    void *coltrans;
    HRGN rgn;
    int x, y;
    region_rect *context;
    
    // calculate a clip window for the intersection of the current and Dest regions
    rgn = CreateRectRgn(info->xDest, info->yDest, info->xDest + info->Width, info->yDest + info->Height);
    CombineRgn(rgn, dc->data.clip, rgn, RGN_AND);

    switch_output(dc);

    TRACE((TC_TW, TT_TW_res3, "plotbitmap: dc %p bitmap %p action %d", dc, bitmap, action));

    x = cvtx(dc, info->xDest - info->xSrc);
    y = cvty(dc, info->yDest - info->ySrc + (bitmap->data.sprite.height - 1));
    
    TRACE((TC_TW, TT_TW_res3, "plotbitmap: sprite OS %d,%d %d,%d",
	    x, y,
	    x + bitmap->data.sprite.width*2 - 1,
	    y + bitmap->data.sprite.height*2 - 1
	));

    // get lookup table - can't cache because we are plotting from bitmap into a different DC
    coltrans = get_colour_lookup_table(dc, &bitmap->data.sprite, bitmap->data.color_table_type);

    context = NULL;
    while (set__clip(dc, rgn, &context))
    {
	LOGERR(_swix(OS_SpriteOp, _INR(0,7),
		     coltrans == NULL && bitmap->data.sprite.bpp == dc->data.bpp ? 34 + 512 : 52 + 512,
		     bitmap->data.sprite.area, first_sprite(bitmap->data.sprite.area),
		     x, y, action,
		     NULL,
		     coltrans));
    }

    free(coltrans);
    DeleteObject(rgn);

    //save_sprite(&bitmap->data.sprite, "tile");
}

static void tilebrush(HDC dc, const bitblt_info *info, int rop3)
{
    HBRUSH brush;
    HRGN rgn;
    region_rect *context;

    brush = dc->data.brush;
    ASSERT(brush, 0);

    // calculate a clip window for the intersection of the current and Dest regions
    rgn = CreateRectRgn(info->xDest, info->yDest, info->xDest + info->Width, info->yDest + info->Height);
    CombineRgn(rgn, dc->data.clip, rgn, RGN_AND);

    switch_output(dc);

    realize_brush(dc);

    if (brush->data.solid)
    {
	int rop2 = Rop3ToRop2[(rop3 >> 16) & 0xff];
	action_info new_action = rop2_to_action[rop2 - 1];
	int new_col;
	int x0, y0, x1, y1;

	if (rop2 == WHITENESS)
	    new_col = 0xff;
	else if (rop2 == BLACKNESS)
	    new_col = 0x00;
	else
	    new_col = dc->data.local.brush_solid;

	// some tricky plot actions require inverting the foreground colour first
	if (new_col == TRANSPARENT_COLOUR)
	    ;
	else if (new_action.invert_P)
	    new_col = (~new_col) & 0xff;

	// update currently set colour/action
	if (dc->data.current.fg_col != new_col ||
	    dc->data.current.action != new_action.action)
	{
	    dc->data.current.fg_col = new_col;
	    dc->data.current.action = new_action.action;
	    LOGERR(_swix(OS_SetColour, _INR(0,1), dc->data.current.action, dc->data.current.fg_col));
	}
	
	x0 = cvtx(dc, info->xDest);
	y0 = cvty(dc, info->yDest + info->Height - 1);
	x1 = cvtx(dc, info->xDest + info->Width);
	y1 = cvty(dc, info->yDest - 1);

	TRACE((TC_TW, TT_TW_res4, "tilebrush: dc %p brush %p colour %d (%x) rop2 %d action %d",
	       dc, brush, dc->data.current.fg_col, brush->data.colour, rop2, dc->data.current.action));
	TRACE((TC_TW, TT_TW_res3, "tilebrush: solid  OS %d,%d %d,%d", x0, y0, x1, y1));

	context = NULL;
	while (set__clip(dc, rgn, &context))
	{
	    // some tricky plot actions require inverting the destination before plotting
	    if (new_action.invert_D)
	    {
		LOGERR(_swix(OS_Plot, _INR(0,2), 0+4, x0, y0));	// moveto
		LOGERR(_swix(OS_Plot, _INR(0,2), 0+6, x1, y1));	// invertto
	    }

	    LOGERR(_swix(OS_Plot, _INR(0,2), 0+4, x0, y0));	// moveto
	    LOGERR(_swix(OS_Plot, _INR(0,2), 96+5, x1, y1));	// fillto
	}
    }
    else
    {
	//int x, y, xs, ys, xe, ye;
	int reason, action;

	action = plotaction_WRITE;
	if (rop3 == PATINVERT)
	    action = plotaction_XOR;

	TRACE((TC_TW, TT_TW_res4, "tilebrush: dc %p brush %p org %d,%d", dc, brush, dc->data.brush_org.x, dc->data.brush_org.y));

	//save_sprite(&brush->data.sprite, "tile");

	reason = dc->data.current.brush_coltrans == NULL && brush->data.sprite.bpp == dc->data.bpp ? 34 + 512 : 52 + 512;
	
	context = NULL;
	while (set__clip(dc, rgn, &context))
	{
	    int x, y, xs, ys, xe, ye;

	    xs = cvtx(dc, (context->rect.left &~ 7) - dc->data.brush_org.x);
	    xe = cvtx(dc, context->rect.right);

	    ys = cvty(dc, (context->rect.top &~ 7) - dc->data.brush_org.y + (BRUSH_HEIGHT-1));
	    ye = cvty(dc, context->rect.bottom + (BRUSH_HEIGHT-1));

	    TRACE((TC_TW, TT_TW_res3, "tilebrush: sprite OS %d,%d %d,%d reason %d", xs, ys, xe, ye, reason));

	    for (x = xs; x < xe; x += BRUSH_WIDTH*2)
	    {
		for (y = ys; y > ye; y -= BRUSH_HEIGHT*2)
		{
		    DTRACE((TC_TW, TT_TW_res3, "tilebrush: sprite plot @ OS %d,%d", x, y));
		
		    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
				 reason,
				 brush->data.sprite.area,
				 first_sprite(brush->data.sprite.area),
				 x, y, action,
				 NULL,
				 dc->data.current.brush_coltrans));
		}
	    }
	}
    }

    // delete region and reset clip
    DeleteObject(rgn);
}

static void line_ptr(line_info *line, sprite_descr *sprite, int y)
{
    if (sprite->area)
    {
	sprite_header *hdr = first_sprite(sprite->area);

	line->data = (unsigned *)sprite_data(hdr) + (hdr->width + 1) * y;
	line->bpp = sprite->bpp;
    }
    else
    {
	line->data = NULL;
	line->bpp = 0;
    }
}

static void dc_line_ptr(line_info *line, HDC dc, int y)
{
    if (dc->gdi.tag == OBJ_DC)
    {
	line->data = (unsigned *)((char *)dc->data.display_start + y * dc->data.line_length);
	line->bpp = dc->data.bpp;
    }	
    else if (dc->data.bitmap)
    {
	line_ptr(line, &dc->data.bitmap->data.sprite, y);
    }
    else
    {
	line->data = NULL;
	line->bpp = dc->data.bpp;
    }
}

static void line_init(line_info *line, const void *coltrans, int x)
{
    if (line->data)
    {
	line->ppw = 32 / line->bpp;
	line->mask = (1 << line->bpp) - 1;
	line->coltrans = (const char *)coltrans;

	line->phase = (x & (line->ppw - 1)) * line->bpp;
	line->ptr = line->data + (x * line->bpp / 32);

	line->word_out = line->word = *(line->ptr);
	line->word >>= line->phase;

	if (line->phase == 0)
	    line->word_out = 0;
	else
	    line->word_out &= ~(((unsigned) -1) << line->phase);

	DTRACE((TC_TW, TT_TW_res4, "line_init: line %p x %4d: ppw %d mask %2x line start %p current %p phase %d word base %08x in %08x out %08x",
	       line, x, line->ppw, line->mask, line->data, line->ptr, line->phase, *(line->ptr), line->word, line->word_out));
    }
    else
    {
	line->phase = 0;
	line->word = 0;
#ifdef DEBUG
	{
	    int i;
	    for (i = 0; i < 32; i += line->bpp)
		line->word |= line->solid_colour << i;
	}
#endif
    }
}

static int pixel(line_info *line)
{
    int pixel;

    if (line->data)
    {
	/* mask off bottom pixels */
	pixel = line->word & line->mask;

	/* increment phase and get new word of shift down existing word */
	if ((line->phase += line->bpp) == 32)
	{
	    line->word = *(++line->ptr);
	    line->phase = 0;
	}
	else
	{
	    line->word >>= line->bpp;
	}

	/* optionally lookup in colour table */
	if (line->coltrans)
	    pixel = line->coltrans[pixel];
    }
    else
    {
	pixel = line->solid_colour;
    }
    
    DTRACE((TC_TW, TT_TW_res4, "pixel: line %p pixel %2x phase %2d word %8x", line, pixel, line->phase, line->word));

    return pixel;

}

static void pixel_out(line_info *line, int pixel)
{
    line->word_out |= pixel << ((line->phase - line->bpp) & 31);

    DTRACE((TC_TW, TT_TW_res4, "p_out:               pixel %2x phase %2d word %8x", pixel, line->phase, line->word_out));

    if (line->phase == 0)
    {
	DTRACE((TC_TW, TT_TW_res4, "p_out: write %08x", line->word_out));

	line->ptr[-1] = line->word_out;	// subtract 1 because we've just loaded a new word at this point
	line->word_out = 0;
    }
}

static void pixel_flush(line_info *line)
{
    if (line->phase != 0)
    {
	line->word_out |= line->word << line->phase;
    
	DTRACE((TC_TW, TT_TW_res4, "p_fls: write %08x", line->word_out));
	
	line->ptr[0] = line->word_out;
    }
}


/*
 * dest and operand2 are always the result register
 *
 * LogXOR EOR result, op1, result
 * LogOR  ORR result, op1, result
 * LogAND AND result, op1, result
 * LogBIC BIC result, op1, result
 */

static unsigned construct_op(int logop, int op1, int op2)
{
    static unsigned logop_codes[] = { code_MVN, code_EOR, code_ORR, code_AND, code_BIC, code_MOV };
    static unsigned operand_codes[] = { rn_SPECIAL, rn_SRC, rn_DEST, rn_PAT, rn_RESULT, rn_OUTPUT };
    unsigned code;

    if (logop == LogMOV || logop == LogNOT)
	code = code_ALWAYS | logop_codes[logop] | DST(operand_codes[op1]) | OP2(operand_codes[op2]);
    else
	code = code_ALWAYS | logop_codes[logop] | DST(rn_RESULT) | OP1(operand_codes[op1]) | OP2(operand_codes[op2]);

    return code;
}

/* get next operand from the parse string and decrement the shift
 * factor.
 * Check for underflow.
 */

static int get_operand(int *parse_string)
{
    int operand = (*parse_string >>= 2) & OpMask;

    TRACE((TC_TW, TT_TW_res4, "get_operand: %s (%d)", operand_name[operand], operand));

    return operand;
}

static pp_function rop3_function(int rop3)
{
#ifdef DEBUG
    extern int ghLogHandle;
#endif
    int parse_string_index = (rop3 & EPS_INDEX) >> EPS_INDEX_SHIFT;
    int parse_string = ParseStrings[parse_string_index];
    BOOL invert = FALSE;
    BOOL pushed_special = FALSE;
    unsigned *code = code_array;

    int logop_shift;
    int current;
    int n_operands;
    int parse_shift;

    /* calculate number of operands that will be used.
     * It's 1 to start
     * plus 1 for each non-invert operator
     * plus an extra 2 if the parse_string has SPECIALS in it (strings 5-7)
     */
    n_operands = parse_string_index >= 5 ? 3 : 1;
    for (logop_shift = LogOpShift; logop_shift < 16; logop_shift += 2)
    {
	int logop = (rop3 >> logop_shift) & LogMask;
	if (logop != LogNOT)
	    n_operands++;
    }

    /* initialise the parse_string and current operand */
    parse_shift = (8 - (rop3 & EPS_OFF) - n_operands)*2;
    parse_string >>= parse_shift;

    current = parse_string & OpMask;
    TRACE((TC_TW, TT_TW_res4, "get_operand: %s (%d)", operand_name[current], current));

    for (logop_shift = LogOpShift; logop_shift < 16; logop_shift += 2)
    {
	int logop = (rop3 >> logop_shift) & LogMask;

	TRACE((TC_TW, TT_TW_res4, "logop: %s (%d)", logop_name[logop], logop));

	// if it is an invert then delay invert and just keep count for now
	if (logop == LogNOT)
	{
	    invert = !invert;
	}
	else
	{
	    // get next operand
	    int operand = get_operand(&parse_string);

	    // if it is a special operand then either push or use special register
	    if (operand == OpSpec)
	    {
		if (!pushed_special)
		{
		    *code++ = construct_op(LogMOV, OpSpec, current);

		    TRACE((TC_TW, TT_TW_res4, "PUSH SPECIAL"));

		    operand = get_operand(&parse_string);
		}
		else
		{
		    TRACE((TC_TW, TT_TW_res4, "PULL SPECIAL"));
		}
		    
		pushed_special = !pushed_special;
	    }

	    // attempt to use too many operands
	    if (operand == OpDone)
	    {
		TRACE((TC_TW, TT_TW_res4, "not enough operands"));
		ASSERT(0, 0);
		break;
	    }
	    
	    // if invert then do explicit invert or use BIC if we can
	    if (invert)
	    {
		TRACE((TC_TW, TT_TW_res4, "WRITE CODE INVERT"));
		
		if (logop == LogAND)
		    *code++ = construct_op(LogBIC, operand, current);
		else
		{
		    *code++ = construct_op(LogNOT, OpRes, current);
		    *code++ = construct_op(logop, operand, OpRes);
		}
		
		invert = FALSE;
	    }
	    else
	    {    
		TRACE((TC_TW, TT_TW_res4, "WRITE CODE"));
		*code++ = construct_op(logop, operand, current);
	    }

	    current = OpRes;
	}
    }

    // take into account parity bit
    if (rop3 & LogPar)
	invert = !invert;

    // copy to output with possible inversion
    *code++ = construct_op(invert ? LogNOT : LogMOV, OpOut, OpRes);

    *code = code_MOV_pc_lr;	// note, no increment

#ifdef DEBUG
    if ( ghLogHandle != -1 )
    {
	unsigned c, *cp = code_array;
	do
	{
	    char *s;
	    char buf[12];
	    int n;

	    c = *cp;

	    n = sprintf(buf, "%08x: ", c);
	    write( ghLogHandle, buf, n );

	    _swix(Debugger_Disassemble, _INR(0,1) | _OUTR(1,2), c, cp, &s, &n);
	    write( ghLogHandle, s, n );

	    write( ghLogHandle, "\n", 1 );

	    cp++;
	}
	while (c != code_MOV_pc_lr);
    }
#endif

    LOGERR(_swix(OS_SynchroniseCodeAreas, _INR(0,2), 1, code_array, code));
    
    return (pp_function)code_array;
}


static void complex_blt(HDC dcDest, HDC dcSrc, const bitblt_info *info, int rop3)
{
    HRGN rgn;
    void *bitmap_coltrans;
    region_rect *context;
    pp_function fn;

    // calculate a clip window for the intersection of the current and Dest regions
    rgn = CreateRectRgn(info->xDest, info->yDest, info->xDest + info->Width, info->yDest + info->Height);
    CombineRgn(rgn, dcDest->data.clip, rgn, RGN_AND);

    switch_output(dcDest);
    
    /* get colour lookup for source (if bitmap), none if from screen */
    if (dcSrc->data.bitmap)
	bitmap_coltrans = get_colour_lookup_table(dcDest, &dcSrc->data.bitmap->data.sprite, dcSrc->data.bitmap->data.color_table_type);
    else
	bitmap_coltrans = NULL;

    realize_brush(dcDest);

    /* get the pixel processor function */
    fn = rop3_function(rop3);

    TRACE((TC_TW, TT_TW_res4, "complex_blt: destDC %p (bitmap %p) srcDC %p (bitmap %p coltrans %p) brush %p (coltrans %p) solid %d (%x)",
	   dcDest, dcDest->data.bitmap, dcSrc, dcSrc->data.bitmap, bitmap_coltrans,
	   dcDest->data.brush, dcDest->data.current.brush_coltrans,
	   dcDest->data.brush ? dcDest->data.brush->data.solid : 0,
	   dcDest->data.brush ? dcDest->data.local.brush_solid : 0));

    /* enumerate the clip regions */
    context = NULL;
    while (set__clip(dcDest, rgn, &context))
    {
	int x, y, x0, y0, x1, y1; // these are all in dest dc space
	int brush_phase_x, brush_phase_y;
	int brush_phase_x0, brush_phase_y0;
	
	// use windows coords as they map to pixels
	// not pixel 0,0 is top left of screen (ie screen base + 0)
	x0 = context->rect.left;
	x1 = context->rect.right;
	y0 = context->rect.top;
	y1 = context->rect.bottom;

	// get starting phase of brush pixels
	brush_phase_x0 = (context->rect.left + dcDest->data.brush_org.x) & (BRUSH_WIDTH - 1);
	brush_phase_y0 = (context->rect.top + dcDest->data.brush_org.y) & (BRUSH_HEIGHT - 1);
	
	TRACE((TC_TW, TT_TW_res4, "complex_blt: %d,%d to %d,%d brush phase %d,%d", x0, y0, x1, y1, brush_phase_x0, brush_phase_y0));

	/* enumerate the lines */
	brush_phase_y = brush_phase_y0;
	for (y = y0; y < y1; y++)
	{
	    line_info s_line, p_line, d_line;

	    /* get line ptrs */
	    dc_line_ptr(&d_line, dcDest, y);
	    dc_line_ptr(&s_line, dcSrc, info->ySrc + (y - info->yDest));
	    line_ptr(&p_line, &dcDest->data.brush->data.sprite, brush_phase_y);

	    p_line.solid_colour = dcDest->data.local.brush_solid;
	    p_line.bpp = dcDest->data.bpp;
	    
	    line_init(&d_line, NULL, x0);
	    line_init(&s_line, bitmap_coltrans, info->xSrc + (x0 - info->xDest));
	    line_init(&p_line, dcDest->data.current.brush_coltrans, brush_phase_x0);

	    /* enumerate the pixels */
	    brush_phase_x = brush_phase_x0;
	    for (x = x0; x < x1; x++)
	    {
		int s, d, p, val;

		d = pixel(&d_line);
		s = pixel(&s_line);
		p = pixel(&p_line);

		val = fn(d, s, p);

		pixel_out(&d_line, val);
		
		if (++brush_phase_x == BRUSH_WIDTH)
		{
		    brush_phase_x = 0;
		    line_init(&p_line, dcDest->data.current.brush_coltrans, brush_phase_x);
		}
	    }

	    pixel_flush(&d_line);
	    
	    if (++brush_phase_y == BRUSH_HEIGHT)
		brush_phase_y = 0;
	}
    }

    free(bitmap_coltrans);
    DeleteObject(rgn);
}

/*
 * This can be from screen to mem or mem to screen.
 */

BOOL BitBlt(HDC dcDest, int xDest, int yDest, int Width, int Height, HDC dcSrc, int xSrc, int ySrc, DWORD rop3)
{
    bitblt_info info;
    HBITMAP bitmap;
    int delbitmap;

    TRACE((TC_TW, TT_TW_res4, "BitBlt: %p%s %dx%d %dx%d from %p%s %dx%d brush %p rop3=0x%x",
	   dcDest, is_screen(dcDest),
	   xDest, yDest, Width, Height,
	   dcSrc, is_screen(dcSrc),
	   xSrc, ySrc,
	   dcSrc ? dcSrc->data.brush : NULL,
	   rop3));

    ASSERT(Width > 0, Width);
    ASSERT(Height > 0, Height);
    
    info.xSrc = xSrc;
    info.ySrc = ySrc;
    info.xDest = xDest;
    info.yDest = yDest;
    info.Width = Width;
    info.Height = Height;
    
    bitmap = NULL;
    delbitmap = FALSE;

    switch (rop3)
    {
    case PATINVERT:
    case PATCOPY:
	tilebrush(dcDest, &info, rop3);
	break;
	
    default:
    {
	TRACE((TC_TW, TT_TW_res4, "BitBlt: complicated rop3 %x", rop3));
	ASSERT(0,rop3);

	complex_blt(dcDest, dcSrc, &info, rop3);
	break;
    }

#if 1
    case SRCCOPY:
	bitmap = get_relevant_bitmap(dcSrc, &info, &delbitmap);
	plotbitmap(dcDest, bitmap, &info, plotaction_WRITE);
	break;

    case SRCPAINT:
	bitmap = get_relevant_bitmap(dcSrc, &info, &delbitmap);
	plotbitmap(dcDest, bitmap, &info, plotaction_OR);
	break;

    case SRCAND:
	bitmap = get_relevant_bitmap(dcSrc, &info, &delbitmap);
	plotbitmap(dcDest, bitmap, &info, plotaction_AND);
	break;

    case SRCINVERT:
	bitmap = get_relevant_bitmap(dcSrc, &info, &delbitmap);
	plotbitmap(dcDest, bitmap, &info, plotaction_XOR);
	break;

    case MERGEPAINT:
	bitmap = get_relevant_bitmap(dcSrc, &info, &delbitmap);
	plotbitmap(dcDest, bitmap, &info, plotaction_ORNOT);
	break;


    case DSTINVERT:
	bitmap = get_relevant_bitmap(dcSrc, &info, &delbitmap);
	plotbitmap(dcDest, bitmap, &info, plotaction_INVERT);
	break;

    case BLACKNESS:
    case WHITENESS:
    {
	HBRUSH old_brush = SelectObject(dcDest, GetStockObject(rop3 == BLACKNESS ? BLACK_BRUSH : WHITE_BRUSH));
	tilebrush(dcDest, &info, plotaction_WRITE);
	SelectObject(dcSrc, old_brush);
	break;
    }
#endif
    }    
    
    if (delbitmap)
	DeleteObject(bitmap);

    TRACE((TC_TW, TT_TW_res4, "BitBlt: out"));

    return 1;
}

/*
 * Note 'dest' and 'rgn1' or 'dest' and 'rgn2' can be the same.
 */

int CombineRgn(HRGN rgn_dest, HRGN rgn1, HRGN rgn2, int type)
{
    region_rect *regd, *regd_last; // chain for output regions
    
    TRACE((TC_TW, TT_TW_res4, "CombineRgn: type %d regions %p %p", type, rgn1, rgn2));

    ASSERT(type == RGN_OR || type == RGN_AND, type);
    ASSERT(rgn_dest, 0);
    ASSERT(rgn2, 0);
    ASSERT(rgn2->data.base->next == NULL, 0);
    
    regd = regd_last = NULL;

    // if rgn2 is NULL then assume it means whole screen
    if (rgn1 == NULL)
    {
	switch (type)
	{
	case RGN_AND:
	{
	    region_rect *n = calloc(sizeof(*n), 1);
	    n->rect = rgn2->data.base->rect;
	    link_region(&regd, &regd_last, n);
	    break;
	}

	case RGN_OR:
	    // everything OR anything is everything
	    break;
	}
    }
    else
    {
	region_rect *reg1;

	for (reg1 = rgn1->data.base; reg1; reg1 = reg1->next)
	{
	    region_rect *n = NULL;
	    switch (type)
	    {
	    case RGN_AND:
	    {
		RECT rect;
		if (IntersectRect(&rect, &reg1->rect, &rgn2->data.base->rect))
		{
		    n = calloc(sizeof(*n), 1);
		    n->rect = rect;
		}
		break;
	    }
	
	    case RGN_OR:
		// copy data to add to chain
		n = calloc(sizeof(*n), 1);
		n->rect = reg1->rect;
		break;
	    }

	    // link into chain
	    if (n)
		link_region(&regd, &regd_last, n);
	}

	if (type == RGN_OR)
	{
	    // add on most recent one
	    region_rect *n = calloc(sizeof(*n), 1);
	    n->rect = rgn2->data.base->rect;

	    // link into chain
	    link_region(&regd, &regd_last, n);
	}
    }

    // free the original dest chain
    free_region(rgn_dest);

    // write in the newly allocated one
    rgn_dest->data.base = regd;

    return 1;
}

/* CreateBitmap can be called with a stream of data that represents the
 * image data in a packed form.
 */

HBITMAP CreateBitmap(int w, int h, UINT planes, UINT bpp, CONST VOID *data)
{
    BITMAP b;

    TRACE((TC_TW, TT_TW_res4, "CreateBitmap:"));

    memset(&b, 0, sizeof(b));

    b.bmWidth = w;
    b.bmHeight = h;
    b.bmWidthBytes = RoundShort((DWORD)w * bpp / 8);
    b.bmPlanes = planes;
    b.bmBitsPixel = bpp;
    b.bmBits = (LPVOID)data;

    return CreateBitmapIndirect(&b);
}

HBITMAP CreateBitmapIndirect(CONST BITMAP *b)
{
    HBITMAP bitmap;

    TRACE((TC_TW, TT_TW_res4, "CreateBitmapIndirect: %dx%d bpp %d data %p", b->bmWidth, b->bmHeight, b->bmBitsPixel, b->bmBits));
    DTRACEBUF((TC_TW, TT_TW_res4, b->bmBits, b->bmWidthBytes * b->bmHeight));

    bitmap = (HBITMAP)create_object(OBJ_BITMAP, sizeof(*bitmap));

    if (bitmap == NULL)
	return NULL;
    
    create_sprite(&bitmap->data.sprite, (int)b->bmWidth, (int)b->bmHeight, b->bmBitsPixel, FALSE);
    bitmap->data.color_table_type = DIB_RGB_COLORS; // actually none at all

    if (bitmap->data.sprite.area)
    {
	if (b->bmBits)
	{
	    fill_sprite(&bitmap->data.sprite, b->bmBits, (int)b->bmWidthBytes);
//	    save_sprite(&bitmap->data.sprite, "bitmap");
	}
    }
    else
    {
	free_object((HGDIOBJ)bitmap);
	bitmap = NULL;
    }
    
    return bitmap;
}

HBITMAP CreateCompatibleBitmap(HDC dc, int w, int h)
{
    BITMAP b;

    TRACE((TC_TW, TT_TW_res4, "CreateCompatibleBitmap: dc %p%s %dx%d", dc, is_screen(dc), w, h));

    memset(&b, 0, sizeof(b));

    b.bmWidth = w;
    b.bmHeight = h;
    b.bmWidthBytes = RoundShort((w *dc->data.bpp) / 8);
    b.bmPlanes = dc->data.planes;
    b.bmBitsPixel = dc->data.bpp;

    return CreateBitmapIndirect(&b);
}

/*
 * Get a compatible context to the screen
 */

HDC CreateCompatibleDC(HDC dcBase)
{
    HDC dc;

    TRACE((TC_TW, TT_TW_res4, "CreateCompatibleDC: dc %p%s", dcBase, is_screen(dcBase)));

    dc = (HDC)create_object(OBJ_MEMDC, sizeof(*dc));
    if (dc == NULL)
	return NULL;
    
    set_default_dc(dc);

    dc->data.planes = dcBase->data.planes;
    dc->data.bpp = dcBase->data.bpp;
    dc->data.mode = dcBase->data.mode;
    dc->data.bitmap_placeholder = dc->data.bitmap = CreateBitmap(1, 1, 1, dc->data.bpp, NULL);
    
    return dc;
}

int FillRect(HDC dc, CONST RECT *lprc, HBRUSH hbr)
{
    HBRUSH old;

    TRACE((TC_TW, TT_TW_res3, "FillRect: dc %p%s brush %p", dc, is_screen(dc), hbr));

    old = SelectObject(dc, hbr);

    PatBlt(dc, lprc->left, lprc->top,
	   (int)lprc->right - (int)lprc->left, 
	   (int)lprc->bottom - (int)lprc->top,
	   PATCOPY);

    SelectObject(dc, old);
    return 1;
}

/*
 * Get the context for the screen
 */

HDC GetDC(HWND hwnd)
{
    HDC dc = screen_dc;

    TRACE((TC_TW, TT_TW_res4, "GetDC:"));

    if (dc == NULL)
    {
	dc = (HDC)create_object(OBJ_DC, sizeof(*dc));

	if (dc == NULL)
	    return NULL;

	set_default_dc(dc);
	set_current_dc(dc);

	current_dc = screen_dc = dc;
    }    
    return dc;
}

/*
 * Create a Bitmap from a DIB.
 *
 * The info structure is in 'info'
 * The bitstream is in 'bits'
 * The palette is in 'dib'
 * 'color_table_type' says what kind of palette it is.
 * flags is whether to fill the bitmap with the bitstream given
 */


HBITMAP CreateDIBitmap(HDC dc, CONST BITMAPINFOHEADER *info, DWORD flags, CONST VOID *bits, CONST BITMAPINFO *dib, UINT color_table_type)
{
    HBITMAP bitmap;

    ASSERT(flags == CBM_INIT, flags);
    
    TRACE((TC_TW, TT_TW_res4, "CreateDIBitmap: dc %p%s table %d pal entries %d(%d) used %d(%d) hdr size %d (%d)",
	   dc, is_screen(dc), color_table_type,
	   1 << info->biBitCount, 1 << dib->bmiHeader.biBitCount, 
	   info->biClrUsed, dib->bmiHeader.biClrUsed,
	   info->biSize, dib->bmiHeader.biSize
	));

    bitmap = (HBITMAP)create_object(OBJ_BITMAP, sizeof(*bitmap));

    if (bitmap == NULL)
	return NULL;
    
    create_sprite(&bitmap->data.sprite, (int)info->biWidth, (int)info->biHeight, info->biBitCount, TRUE);
    bitmap->data.color_table_type = color_table_type;
    
    if (bitmap->data.sprite.area == NULL)
    {
	free_object((HGDIOBJ)bitmap);
	return NULL;
    }

    fill_sprite_from_dib(&bitmap->data.sprite, info, bits, color_table_type);
    fill_palette_from_dib(&bitmap->data.sprite, info, dib, color_table_type);

    //save_sprite(&bitmap->data.sprite, "dib");
    
    return bitmap;
}

HBRUSH CreateDIBPatternBrush(HGLOBAL gdib, UINT color_table_type)
{
    HBRUSH brush;
    const BITMAPINFO *info = (const BITMAPINFO *)gdib;

    TRACE((TC_TW, TT_TW_res1, "CreateDIBPatternBrush: dib %p table %d", gdib, color_table_type));

    brush = (HBRUSH)create_object(OBJ_BRUSH, sizeof(*brush));
    if (brush == NULL)
	return NULL;
    
    ASSERT(info->bmiHeader.biWidth >= 8, (int)info->bmiHeader.biWidth);
    ASSERT(info->bmiHeader.biHeight >= 8, (int)info->bmiHeader.biHeight);
    
    create_sprite(&brush->data.sprite, 8, 8, info->bmiHeader.biBitCount, TRUE);
    brush->data.color_table_type = color_table_type;
    brush->data.solid = FALSE;

    if (brush->data.sprite.area == NULL)
    {
	free_object((HGDIOBJ)brush);
	return NULL;
    }
    
    fill_sprite_from_dib(&brush->data.sprite, &info->bmiHeader, NULL, color_table_type);
    fill_palette_from_dib(&brush->data.sprite, &info->bmiHeader, info, color_table_type);

//    save_sprite(&brush->data.sprite, "brush");

    return brush;
}

HPALETTE CreatePalette(CONST LOGPALETTE *pal)
{
    HPALETTE palette;

    TRACE((TC_TW, TT_TW_res4, "CreatePalette:"));

    palette = (HPALETTE)create_object(OBJ_PAL, sizeof(*palette));
    if (palette == NULL)
	return NULL;
    
    palette->data.n_entries = pal->palNumEntries;
    palette->data.colours = malloc(pal->palNumEntries * sizeof(pal->palPalEntry[0]));
    memcpy(palette->data.colours, pal->palPalEntry, pal->palNumEntries * sizeof(pal->palPalEntry[0]));

    return palette;
}

HPEN CreatePen(int style, int width, COLORREF colref)
{
    HPEN pen;

    TRACE((TC_TW, TT_TW_res4, "CreatePen: color 0x%x", colref));

    pen = (HPEN)create_object(OBJ_PEN, sizeof(*pen));
    if (pen == NULL)
	return NULL;
    
    pen->data.colref = colref;
    pen->data.style = style;
    pen->data.width = width;

    ASSERT((style == PS_SOLID), style);
    ASSERT((width == 1), width);

    return pen;
}

HRGN CreateRectRgn(int left, int top, int right, int bottom)
{
    RECT r;

    TRACE((TC_TW, TT_TW_res4, "CreateRectRgn:"));

    r.left = left;
    r.top = top;
    r.right = right;
    r.bottom = bottom;

    return CreateRectRgnIndirect(&r);
}

HRGN CreateRectRgnIndirect(CONST RECT *prect)
{
    HRGN rgn;
    region_rect *rr;

    TRACE((TC_TW, TT_TW_res4, "CreateRectRgnIndirect: %d,%d to %d,%d", prect->left, prect->top, prect->right, prect->bottom));

    rgn = (HRGN)create_object(OBJ_REGION, sizeof(*rgn));
    if (rgn == NULL)
	return NULL;

    rr = calloc(sizeof(*rr), 1);

    if (rr == NULL)
    {
	free_object((HGDIOBJ)rgn);
	return NULL;
    }
    
    rr->rect = *prect;
    rgn->data.base = rr;
    
    return rgn;
}

HBRUSH CreateSolidBrush(COLORREF colref)
{
    HBRUSH brush;
    ropalette pal[2];

    TRACE((TC_TW, TT_TW_res4, "CreateSolidBrush: color 0x%x", colref));

    brush = (HBRUSH)create_object(OBJ_BRUSH, sizeof(*brush));
    if (brush == NULL)
	return NULL;
    
#if 1
    brush->data.solid = TRUE;
    brush->data.colour = colref;
#else
    create_sprite(&brush->data.sprite, 8, 8, 1, TRUE);

    pal[0].w = 0;
    switch (colref >> 24)
    {
    case 1:			// choose palette entry
	pal[1].w = colref & 0xFF;
	brush->data.color_table_type = DIB_PAL_COLORS;
	break;

    case 2:			// choose RGB
    {
	unsigned c = colref << 8;
	pal[1].w = REVERSE_WORD(c);
	brush->data.color_table_type = DIB_RGB_COLORS;
	break;
    }

    default:
	ASSERT(0, 0);
	break;
    }

    if (brush->data.sprite.area == NULL)
    {
	free_object((HGDIOBJ)brush);
	return NULL;
    }

    clear_sprite(&brush->data.sprite, 0xff);
    set_sprite_palette(&brush->data.sprite, pal);
#endif

    return brush;
}

BOOL DeleteDC(HDC dc)
{
    TRACE((TC_TW, TT_TW_res4, "DeleteDC: dc %p%s", dc, is_screen(dc)));
    return DeleteObject((HGDIOBJ)dc);
}

BOOL DeleteObject(LPVOID oobj)
{
    HGDIOBJ obj = oobj;
    
    TRACE((TC_TW, TT_TW_res4, "DeleteObject: %p %s (%d)",
	   obj,
	   objects[obj ? obj->gdi.tag : 0],
	   obj ? obj->gdi.tag : 0));

    free_object(obj);

    return 1;
}

/*
 * Read from 'bitmap' into the DIB addressed by 'info'.
 * Write the DIB parameters set into 'info'.
 * Always uses whole scan lines. start = 0 means the bottom line of the image.
 * return the number of scanlines read (should be 'num' for success
 */

int GetDIBits(HDC dc, HBITMAP bitmap, UINT start, UINT num, LPVOID bits, LPBITMAPINFO info, UINT usage)
{
    sprite_descr *descr;
    
    TRACE((TC_TW, TT_TW_res4, "GetDIBits: dc %p%s bitmap %p lines %d(%d) usage %d", dc, is_screen(dc), bitmap, start, num, usage));
    
    // fill in information
    descr = &bitmap->data.sprite;
    info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info->bmiHeader.biWidth = descr->width;
    info->bmiHeader.biHeight = descr->height;
    info->bmiHeader.biPlanes = 1;
    info->bmiHeader.biBitCount = descr->bpp == 32 ? 24 : descr->bpp;
    info->bmiHeader.biCompression = 0;
    info->bmiHeader.biSizeImage = 0;
    info->bmiHeader.biXPelsPerMeter = 0;
    info->bmiHeader.biYPelsPerMeter = 0;
    info->bmiHeader.biClrUsed = 0;
    info->bmiHeader.biClrImportant = 0;

    // get data
    fill_dib_from_sprite(&bitmap->data.sprite, info, bits, start, num);
    fill_palette_from_sprite(&bitmap->data.sprite, info, usage);

    return num;
}

int GetRgnBox(HRGN rgn, LPRECT rect)
{
    TRACE((TC_TW, TT_TW_res4, "GetRgnBox: rgn %p", rgn));
    return 1;
}

HGDIOBJ GetStockObject(int type)
{
    HGDIOBJ obj;

    TRACE((TC_TW, TT_TW_res4, "GetStockObject: %d", type));

    if (type <= NULL_PEN && stock_objects[type])
    {
	obj = stock_objects[type];
    }
    else if (type <= NULL_BRUSH)
    {
	static COLORREF brush_colors[] =
	{
	    PALETTERGB(0xFF, 0xFF, 0xFF),
	    PALETTERGB(0xCC, 0xCC, 0xCC),
	    PALETTERGB(0x88, 0x88, 0x88),
	    PALETTERGB(0x44, 0x44, 0x44),
	    PALETTERGB(0x00, 0x00, 0x00),
	    NULL_COLOUR
	};
	stock_objects[type] = obj = (HGDIOBJ)CreateSolidBrush( brush_colors[type] );
    }
    else if (type <= NULL_PEN)
    {
	static COLORREF pen_colors[] =
	{
	    PALETTERGB(0xFF, 0xFF, 0xFF),
	    PALETTERGB(0x00, 0x00, 0x00),
	    NULL_COLOUR
	};
	stock_objects[type] = obj = (HGDIOBJ)CreatePen(PS_SOLID, 1, pen_colors[type - WHITE_PEN] );
    }
    else if (type == DEFAULT_PALETTE)
    {
	static PALETTEENTRY low[16] =
	{
	    {0,   0,    0,    0  },
	    {0x80, 0,   0,    0  },
	    {0,   0x80, 0,    0  },
	    {0x80, 0x80, 0,   0 },
	    {0,   0,    0x80, 0  },
	    {0x80, 0,   0x80, 0  },
	    {0,   0x80, 0x80, 0  },
	    {0xC0, 0xC0, 0xC0, 0 },

	    { 192, 220, 192, 0 },
	    { 166, 202, 240, 0 }
	};

	static PALETTEENTRY high[16] =
	{
	    { 255, 251, 240, 0 },
	    { 160, 160, 164, 0 },

	    {0x80, 0x80, 0x80, 0 },
	    {0xFF, 0,   0,    0  },
	    {0,   0xFF, 0,    0  },
	    {0xFF, 0xFF, 0,   0 },
	    {0,   0,    0xFF, 0  },
	    {0xFF, 0,   0xFF, 0  },
	    {0,   0xFF, 0xFF, 0  },
	    {0xFF, 0xFF, 0xFF, 0 }
	};
	
	PLOGPALETTE lpal;
	PPALETTEENTRY pe;
	int r,g,b,i;

	lpal = malloc(sizeof(*lpal) + 256*sizeof(PALETTEENTRY));

	if (lpal == NULL)
	    return NULL;

	lpal->palVersion = 0;
	lpal->palNumEntries = 256;

	pe = lpal->palPalEntry;
	r = 80;
	g = 8;
	b = 0;

	for (i = 0; i < 10; i++)
	    pe[i] = low[i];

	for (; i < 246; i++)
	{
	    if ( ! ( (r += 8) & 0x3F ) )
		if ( ! ( (g += 8) & 0x3F ) )
		    b += 16;

	    pe[i].peRed = r;
	    pe[i].peGreen = g;
	    pe[i].peBlue = b;
	    pe[i].peFlags = 0;
	}

	for (; i < 256; i++)
	    pe[i] = high[i - 246];

	stock_objects[type] = obj = (HGDIOBJ)CreatePalette(lpal);

	free(lpal);
    }
    else
    {
	ASSERT(0, type);
	obj = NULL;
    }
    return obj;
}

UINT GetSystemPaletteEntries(HDC dc, UINT what, UINT size, LPPALETTEENTRY pal)
{
    TRACE((TC_TW, TT_TW_res4, "GetSystemPaletteEntries: dc %p%s what %d size %d to %p", dc, is_screen(dc), what, size, pal));

    ASSERT(dc->data.current.palette, 0);

    copy_palette_from_riscos(pal, dc->data.current.palette, size);

    return 1;
}

UINT GetSystemPaletteUse(HDC dc)
{
    TRACE((TC_TW, TT_TW_res4, "GetSystemPaletteUse: dc %p%s", dc, is_screen(dc)));

    return dc->data.syspal;
}

BOOL IntersectRect(LPRECT lprcDst, CONST RECT *lprcSrc1, CONST RECT *lprcSrc2)
{
    BOOL overlap;
    
    lprcDst->left = MAX(lprcSrc1->left, lprcSrc2->left);
    lprcDst->right = MIN(lprcSrc1->right, lprcSrc2->right);
    lprcDst->top = MAX(lprcSrc1->top, lprcSrc2->top);
    lprcDst->bottom = MIN(lprcSrc1->bottom, lprcSrc2->bottom);
			
    overlap = lprcDst->left < lprcDst->right && lprcDst->bottom > lprcDst->top;

    TRACE((TC_TW, TT_TW_res4, "IntersectRect: %d,%d to %d,%d = overlap %d", lprcDst->left, lprcDst->top, lprcDst->right, lprcDst->bottom, overlap));

    return overlap;
}

/* Use the brush set in 'dc' to paint the given rectangle of 'dc'
 * using the given ROP code (which is limited to those that don't use
 * a src HDC
 */

BOOL PatBlt(HDC dc, int x, int y, int w, int h, DWORD rop3)
{
    TRACE((TC_TW, TT_TW_res3, "PatBlt: dc %p%s %dx%d %dx%d rop3=0x%x",
	   dc, is_screen(dc), x, y, w, h, rop3));

    BitBlt(dc, x, y, w, h, dc, 0, 0, rop3);
    
    return 1;
}

/* Take the last logical palette set by SelectPalette() and try to
 * grab the necessary colours.
 * Return the number of colours successfully realized.
 */

UINT RealizePalette(HDC dc)
{
    TRACE((TC_TW, TT_TW_res4, "RealizePalette: dc %p%s", dc, is_screen(dc)));

    ASSERT(dc->data.palette, 0);
    ASSERT(dc->data.palette->data.colours, 0);
    
    TRACE((TC_TW, TT_TW_res4, "RealizePalette: pal %p ncols %d ", dc->data.palette, dc->data.palette->data.n_entries));

    // allocate a 'dc' palette cache if not there already
    if (dc->data.current.palette == NULL)
    {
	TRACE((TC_TW, TT_TW_res4, "RealizePalette: add palette to dc"));

	dc->data.current.palette = malloc(sizeof(ropalette) << dc->data.bpp);

	if (dc->data.current.palette == NULL)
	    return 0;
    }

    // copy palette to the 'dc' palette cache
    ASSERT((1 << dc->data.bpp) == dc->data.palette->data.n_entries, dc->data.bpp);
    copy_palette_to_riscos(dc->data.current.palette, dc->data.palette->data.colours, dc->data.palette->data.n_entries);
    
    TRACEBUF((TC_TW, TT_TW_res4, dc->data.current.palette, sizeof(ropalette) << dc->data.bpp));

    // set palette to screen or sprite
    if (dc->gdi.tag == OBJ_DC)
    {
	LOGERR(_swix(ColourTrans_WritePalette, _INR(0,4),
		     -1, -1,
		     dc->data.current.palette,
		     0, 0));
    }
    else
    {
	sprite_area *area;
	sprite_header *sprite;
	HBITMAP bitmap;

	bitmap = dc->data.bitmap;
	ASSERT(bitmap, 0);
	
	area = bitmap->data.sprite.area;
	ASSERT(area, 0);

	sprite = first_sprite(area);
	ASSERT(sprite, 0);

	TRACE((TC_TW, TT_TW_res4, "RealizePalette: bitmap %p bpp %d", bitmap, bitmap->data.sprite.bpp));
	
	// if bitmap doesn't have a palette then add one
	if (sprite->image == sizeof(sprite_header))
	{
	    int pal_size = 8 << bitmap->data.sprite.bpp;

	    TRACE((TC_TW, TT_TW_res4, "RealizePalette: adding palette to bitmap %p", bitmap));
	    
	    area = realloc(area, area->size + pal_size);
	    if (!area)
	    {
		ASSERT(area != NULL, 0);
		return 0;
	    }

	    area->size += pal_size;
	    area->freeoff += pal_size;

	    bitmap->data.sprite.area = area;

	    sprite = first_sprite(area);
	    memmove((char *)sprite_data(sprite) + pal_size, sprite_data(sprite), sprite->next - sprite->image);

	    sprite->next += pal_size;
	    sprite->image += pal_size;
	    sprite->mask += pal_size;
	}

	ASSERT((1 << bitmap->data.sprite.bpp) == dc->data.palette->data.n_entries, bitmap->data.sprite.bpp);
	
	LOGERR(_swix(ColourTrans_WritePalette, _INR(0,4),
		     area, sprite,
		     dc->data.current.palette,
		     0, 1));	// R1 is pointer to sprite
    }

    return dc->data.palette->data.n_entries;
}

int ReleaseDC(HWND hwnd, HDC dc)
{
    TRACE((TC_TW, TT_TW_res4, "ReleaseDC: dc %p%s", dc, is_screen(dc)));
    return 1;
}

int SelectClipRgn(HDC dc, HRGN rgn)
{
    int r = NULLREGION;
    
    TRACE((TC_TW, TT_TW_res4, "SelectClipRgn: dc %p%s rgn %p clip %p", dc, is_screen(dc), rgn, dc->data.clip));

    if (rgn == NULL)
	dc->data.clip = NULL;
    else
	r = (int)SelectObject(dc, (HGDIOBJ)rgn);
    
    return r;
}

LPVOID SelectObject(HDC dc, LPVOID oobj)
{
    HGDIOBJ obj = oobj;
    LPVOID old_obj = NULL;

    ASSERT(obj, 0);

    TRACE((TC_TW, TT_TW_res4, "SelectObject: dc %p%s obj %p %s %d",
	   dc, is_screen(dc), obj,
	   objects[obj && (unsigned)obj->gdi.tag < sizeof(objects)/sizeof(objects[0]) ? obj->gdi.tag : 0],
	   obj ? obj->gdi.tag : 0));

    if (!obj)
	return NULL;
    
    switch (obj->gdi.tag)
    {
    case OBJ_PEN:
	old_obj = dc->data.pen;
	dc->data.pen = (HPEN)obj;

	dc->data.local.pen_col = UNSET_COLOUR;
	break;

    case OBJ_BRUSH:
	old_obj = dc->data.brush;
	dc->data.brush = (HBRUSH)obj;

	if (dc->data.current.brush_coltrans != UNSET_COLTRANS)
	{
	    free(dc->data.current.brush_coltrans);
	    dc->data.current.brush_coltrans = UNSET_COLTRANS;
	}

	dc->data.local.brush_solid = UNSET_COLOUR;
	break;

    case OBJ_BITMAP:
    case OBJ_CURSOR:
	ASSERT(dc->gdi.tag == OBJ_MEMDC, dc->gdi.tag);

	if (dc->gdi.tag == OBJ_MEMDC)
	{
	    old_obj = dc->data.bitmap;
	    dc->data.bitmap = (HBITMAP)obj;

	    uncache_dc(dc);
	    set_current_dc_from_bitmap(dc);
	    // transfer palette?
	}
	break;

    case OBJ_REGION:
	dc->data.clip = (HRGN)obj;
	old_obj = (LPVOID)SIMPLEREGION;
	break;

    default:
	ASSERT(0, obj->gdi.tag);
	break;
    }

    DTRACE((TC_TW, TT_TW_res4, "SelectObject: old %p type %d", old_obj, old_obj && obj->gdi.tag != OBJ_REGION ? ((HGDIOBJ)old_obj)->gdi.tag : 0));

    return old_obj;
}

HPALETTE SelectPalette(HDC dc, HPALETTE pal, BOOL foreground)
{
    HPALETTE old_pal = NULL;

    TRACE((TC_TW, TT_TW_res4, "SelectPalette: dc %p%s pal %p", dc, is_screen(dc), pal));

    if (dc)
    {
	old_pal = dc->data.palette;
	dc->data.palette = pal;
    }
    
    return old_pal;
}

COLORREF SetBkColor(HDC dc, COLORREF col)
{
    COLORREF old = dc->data.bk_col;

    TRACE((TC_TW, TT_TW_res4, "SetBkColor: dc %p%s color 0x%x", dc, is_screen(dc), col));

    dc->data.bk_col = col;
    dc->data.local.bk_col = UNSET_COLOUR;

    return old;
}

int SetDIBitsToDevice(HDC dc,
		      int xDest, int yDest,	// top left of destination rectangle
		      DWORD Width, DWORD Height,// image size to transfer
		      int xSrc, int ySrc,	// start position (bottom left) in source DIB
		      UINT start, UINT num,	// 'band' of scan lines that are contained in the 'bits' array
		      CONST VOID *bits, CONST BITMAPINFO *info, UINT color_table_type)
{
    HBITMAP bitmap;
    HDC dcCompat;
    
    TRACE((TC_TW, TT_TW_res3, "SetDIBitsToDevice: dc %p%s dest %dx%d size %dx%d src %dx%d scanlines %d(%d) bits %p info %p paltype %d",
	   dc, is_screen(dc), xDest, yDest, Width, Height, xSrc, ySrc, start, num, bits, info, color_table_type));

    ASSERT(start == 0, start);		// I don't think they use any other form
    ASSERT(num == Height, num);
    
    bitmap = CreateDIBitmap(dc, &info->bmiHeader, CBM_INIT, bits, info, color_table_type);

    dcCompat = CreateCompatibleDC(dc);
    SelectObject(dcCompat, bitmap);
    
    BitBlt(dc, xDest, yDest, Width, Height, dcCompat, xSrc, ySrc, SRCCOPY);

    DeleteObject(dcCompat);
    DeleteObject(bitmap);
    
    return 1;
}
			
int SetROP2(HDC dc, int rop2)
{
    int old = dc->data.rop2;

    TRACE((TC_TW, TT_TW_res4, "SetROP2: dc %p%s rop2 %d", dc, is_screen(dc), rop2));

    dc->data.rop2 = rop2;
    dc->data.local.action = rop2_to_action[(rop2 - 1) & 0x0F];
    dc->data.local.pen_col = UNSET_COLOUR;

    return old;
}

/*
 * Default brush origin is the window top left.
 * This offsets the origin up and to the left
 */

BOOL SetBrushOrgEx(HDC dc, int x, int y, LPPOINT pt)
{
    TRACE((TC_TW, TT_TW_res4, "SetBrushOrgEx: dc %p%s %dx%d", dc, is_screen(dc), x, y));

    if (pt)
	*pt = dc->data.brush_org;
    
    dc->data.brush_org.x = x;
    dc->data.brush_org.y = y;

    ASSERT((pt == NULL), (int)pt);
    
    return 1;
}

UINT SetSystemPaletteUse(HDC dc, UINT syspal)
{
    int old = dc->data.syspal;

    TRACE((TC_TW, TT_TW_res4, "SetSystemPaletteUse: dc %p%s %d", dc, is_screen(dc), syspal));

    dc->data.syspal = syspal;
    return old;
}

COLORREF SetTextColor(HDC dc, COLORREF col)
{
    COLORREF old = dc->data.text_col;

    TRACE((TC_TW, TT_TW_res4, "SetTextColor: dc %p%s color 0x%x", dc, is_screen(dc), col));

    dc->data.text_col = col;
    dc->data.local.text_col = UNSET_COLOUR;

    return old;
}

BOOL UnrealizeObject(HGDIOBJ obj)
{
    TRACE((TC_TW, TT_TW_res4, "UnrealizeObject: %p", obj));

    switch (obj->gdi.tag)
    {
    case OBJ_PAL:
	// remap logical palette to system next time it is needed?
	ASSERT(!UnrealizeObject, obj->gdi.tag);
	break;

    case OBJ_BRUSH:
	// reset origin on next use
	ASSERT(!UnrealizeObject, obj->gdi.tag);
	break;

    default:
	ASSERT(!UnrealizeObject, obj->gdi.tag);
	break;
    }
    return 1;
}

void RestoreScreen(void)
{
    TRACE((TC_TW, TT_TW_res4, "RestoreScreen:"));

    switch_output(screen_dc);
}

/* ---------------------------------------------------------------------------------------------------- */

/* realize the cursor on the screen */

HCURSOR SetCursor(HCURSOR cursor)
{
    HCURSOR old = screen_cursor;
    char trans[4];

    TRACE((TC_TW, TT_TW_res4, "SetCursor: %p", cursor));

    screen_cursor = cursor;

    if (cursor)
    {
	trans[0] = 0;
	trans[1] = 1;
	trans[2] = 2;
	trans[3] = 3;

	LOGERR(_swix(OS_SpriteOp, _INR(0,7), 36 + 512,
		     cursor->data.sprite.area, first_sprite(cursor->data.sprite.area),
		     2 | (1<<5), // shape 2, no palette
		     cursor->data.hotspot.x, cursor->data.hotspot.y,
		     NULL,
		     trans));
    }
    else
    {
	MouseShowPointer(0);
    }
    
    return old;
}

/*
 * XORPlane has the information bits.
 *  0 maps to colour 1
 *  1 maps to colour 3
 *
 * ANDPlane has the mask bits.
 *  0 leaves bit unchanged
 *  1 sets bit to 0 (ie transparent)
 */

HCURSOR CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, CONST VOID *pvANDPlane, CONST VOID *pvXORPlane)
{
    HDC dc, old_dc;
    HCURSOR cursor;
    sprite_descr and_sprite, xor_sprite;
    char coltrans[2];

    TRACE((TC_TW, TT_TW_res4, "CreateCursor: HotSpot %d,%d Size %d,%d", xHotSpot, yHotSpot, nWidth, nHeight));

    cursor = (HCURSOR)create_object(OBJ_CURSOR, sizeof(*cursor));
    if (cursor == NULL)
	return NULL;

    /* FIll in easy values */
    cursor->data.hotspot.x = xHotSpot;
    cursor->data.hotspot.y = yHotSpot;
    
    /* allocate objects */
    and_sprite.area = xor_sprite.area = NULL;
    if (!create_sprite(&cursor->data.sprite, nWidth, nHeight, 2, FALSE) ||
	!create_sprite(&and_sprite, nWidth, nHeight, 1, FALSE) ||
	!create_sprite(&xor_sprite, nWidth, nHeight, 1, FALSE))
    {
	free(and_sprite.area);
	free_object((HGDIOBJ)cursor);
	return NULL;
    }

    fill_sprite(&and_sprite, pvANDPlane, (nWidth+7)/8);
    fill_sprite(&xor_sprite, pvXORPlane, (nWidth+7)/8);

    /* create a DC to plot into and select cursor into it */
    dc = CreateCompatibleDC(screen_dc);
    SelectObject(dc, cursor);

    old_dc = switch_output(dc);

    /* build up composite cursor */
    coltrans[0] = 3;
    coltrans[1] = 0;
    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
		 52 + 512,
		 and_sprite.area, first_sprite(and_sprite.area),
		 0, 0, plotaction_WRITE, NULL, coltrans));

    coltrans[0] = 0;
    coltrans[1] = 1;
    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
		 52 + 512,
		 xor_sprite.area, first_sprite(xor_sprite.area),
		 0, 0, plotaction_XOR, NULL, coltrans));

    /* switch back to last dc and free temporary objects */
    switch_output(old_dc);

    DeleteDC(dc);
    
    free(and_sprite.area);
    free(xor_sprite.area);

    //save_sprite(&and_sprite, "AND");
    //save_sprite(&xor_sprite, "XOR");
    //save_sprite(&cursor->data.sprite, "cursor");

    return cursor;
}

BOOL DestroyCursor(HCURSOR cursor)
{
    TRACE((TC_TW, TT_TW_res4, "DestroyCursor: %p", cursor));

    DeleteObject(cursor);

    return 1;
}

/* ---------------------------------------------------------------------------------------------------- */

void SetMode(int colorcaps, int xres, int yres)
{
    int m[10];

    m[0] = 1;
    m[1] = xres;
    m[2] = yres;
    m[3] = colorcaps == Color_Cap_16 ? 2 : 3;
    m[4] = -1;			// highest frame rate

    if (m[3] == 3)
    {
	m[5] = 0;		// programmable palette
	m[6] = 0x80;

	m[7] = 3;
	m[8] = 255;

	m[9] = -1;		// special parameter end
    }
    else
	m[5] = -1;		// special parameter end

    TRACE((TC_TW, TT_TW_res4, "SetMode: %d x %d @ %d bpp", xres, yres, 1 << m[3]));

    LOGERR(_swix(OS_ScreenMode, _INR(0,1), 0, m));

    // update the screen dc values
    set_current_dc(screen_dc);
    uncache_dc(screen_dc);

    RealizePalette(screen_dc);
    
    LOGERR(_swix(Hourglass_Smash, 0));
    (void) MouseShowPointer( TRUE );

    LOGERR(_swix(OS_WriteN, _INR(0,1), MOUSE_COLOURS, sizeof(MOUSE_COLOURS)-1));

    LOGERR(_swix(OS_RemoveCursors, 0));
}

void GetModeSpec(int *width, int *height)
{
    if (width)
	*width = vduval(vduvar_XLimit) + 1;
    if (height)
	*height = vduval(vduvar_YLimit) + 1;
}

/* ---------------------------------------------------------------------------------------------------- */

/* eof twro.c */
