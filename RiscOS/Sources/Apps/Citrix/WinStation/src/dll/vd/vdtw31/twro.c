/* > twro.c

 * RISCOS Thinwire routines

 */

#include "windows.h"

#include <stdlib.h>
#include <string.h>

#include "swis.h"

#include "../../../app/utils.h"
#include "../../../inc/client.h"
#include "../../../inc/debug.h"
#include "../../../inc/mouapi.h"

#include "citrix/ica.h"
#include "citrix/ica-c2h.h"
#include "citrix/twcommon.h"

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

#define RoundShort(a)		(((a) + 1) &~ 1)
#define RoundLong(a)		(((a) + 3) &~ 3)

#ifdef DEBUG
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

/* ---------------------------------------------------------------------------------------------------- */

typedef struct
{
    int tag;
    void *next;
} gdi_hdr;

typedef struct DC__
{
    gdi_hdr gdi;

    struct
    {
	HPEN pen;		// currently selected objects in this context

	HBRUSH brush;
	POINT brush_org;

	HBITMAP bitmap;		// bitmap that represents the drawing surface, MEMDC only

	POINT pen_pos;
	COLORREF bk_col;
	COLORREF text_col;
	int rop2;

	HRGN clip;
	HPALETTE palette;

	int syspal;		// are we using systemn reserved palette entries?


	int planes;		// always 1
	int log2bpp;
	int width;		// in pixels
	int height;

	int mode;		// mode number or descriptor for this mode
	void *save_area;	// save area for switching output

	struct
	{
	    int pen_col;	// colour number corresponding to pen_col
	    int text_col;	// colour number corresponding to text_col
	    int bk_col;		// colour number corresponding to bk_col
	    int action;		// action corresponding to ROP2
	} local;

	struct
	{
	    int fg_col;		// actual colours in use
	    int bg_col;
	    int action;		// actual action in use

	    ropalette *palette;	// 1 word per col palette in use
	    void *brush_coltrans;

	    void *bitmap_coltrans;
	    HBITMAP bitmap_bitmap;
	} current;
    } data;
} DC;

typedef struct region_rect region_rect;

struct region_rect
{
    region_rect *next;
    RECT rect;
};

typedef struct RGN__
{
    gdi_hdr gdi;

    struct
    {
	region_rect *base;
    } data;
} region;

typedef struct PALETTE__
{
    gdi_hdr gdi;
    struct
    {
	int n_entries;
	COLORREF *colours;
    } data;
} palette;

typedef struct PEN__
{
    gdi_hdr gdi;
    struct
    {
	COLORREF colref;
	int style;
	int width;
    } data;
} pen;

typedef struct BRUSH__
{
    gdi_hdr gdi;

    struct
    {
	sprite_descr sprite;
	int color_table_type;
    } data;
} brush;

typedef struct BITMAP__
{
    gdi_hdr gdi;

    struct
    {
	sprite_descr sprite;
	int color_table_type;
    } data;
} bitmap;

typedef struct CURSOR__
{
    gdi_hdr gdi;

    struct
    {
	POINT hotspot;
    } data;
} cursor;

typedef struct GDIOBJ__
{
    gdi_hdr gdi;
} gdiobj;

/* ---------------------------------------------------------------------------------------------------- */

#define cvtx(dc, x)	((x)*2)
#define cvty(dc, y)	((dc->data.height - 1 - (y))*2)

/* ---------------------------------------------------------------------------------------------------- */

static HGDIOBJ gdi_objects = NULL;
static HGDIOBJ stock_objects[STOCK_LAST] = { 0 };
static HDC current_dc = NULL;
static HDC screen_dc = NULL;

#define plotaction_WRITE	0
#define plotaction_OR		1
#define plotaction_AND		2
#define plotaction_XOR		3
#define plotaction_INVERT	4
#define plotaction_NOP		5
#define plotaction_ANDNOT	6
#define plotaction_ORNOT	7

static char rop2_to_action[16] =
{
    0xFF,		// R2_BLACK,
    0xFF,		// R2_NOTMERGEPEN,
    plotaction_ANDNOT,	// R2_MASKNOTPEN,
    0xFF,		// R2_NOTCOPYPEN,
    0xFF,		// R2_MASKPENNOT,
    plotaction_INVERT,	// R2_NOT,
    plotaction_XOR,	// R2_XORPEN,
    0xFF,		// R2_NOTMASKPEN,
    plotaction_AND,	// R2_MASKPEN,
    0xFF,		// R2_NOTXORPEN,
    plotaction_NOP,	// R2_NOP,
    plotaction_ORNOT,	// R2_MERGENOTPEN,
    plotaction_WRITE,	// R2_COPYPEN,
    0xFF,		// R2_MERGEPENNOT,
    plotaction_OR,	// R2_MERGEPEN,
    0xFF		// R2_WHITE
};

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


#define REVERSE_WORD(p)	((p << 24) | ((p << 8) & 0x00FF0000) | ((p >> 8) & 0x0000FF00) | (0 /*p >> 24*/))

/* ---------------------------------------------------------------------------------------------------- */

struct os_mode_selector
   {  int flags;
      int xres;
      int yres;
      int log2_bpp;
      int frame_rate;
   };

#define vduvar_XEig	4
#define vduvar_YEig	5
#define vduvar_Log2BPP	9
#define vduvar_XLimit	11
#define vduvar_YLimit	12

static int vduval(int var)
{
    int val;
    LOGERR(_swix(OS_ReadModeVariable, _INR(0,1) | _OUT(2), -1, var, &val));
    return val;
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

#define first_sprite(area)	((sprite_header *)((char *)(area) + (area)->sproff))
#define sprite_data(sprite)	((char *)(sprite) + (sprite)->image)
#define sprite_palette(sprite)	(ropalette *)((char *)(sprite) + sizeof(sprite_header))

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

static sprite_area *create_sprite(int w, int h, int bpp, BOOL palette)
{
    sprite_area *area;
    sprite_header *sprite;

    int bit_width = w * bpp;
    int word_width = ((32 - bpp) + bit_width + 31)/32; // max left hand wastage + sprite + round up to a word (to allow for grabbing from screen)
    int pal_size = palette ? (8 << bpp) : 0;

    int size = sizeof(sprite_area) +
	sizeof(sprite_header) +
	pal_size +
	word_width * 4 * h;

    TRACE((TC_TW, TT_API4, "create_sprite: %d x %d @ %d bpp palette %d size %d", w, h, bpp, palette, size));
    
    area = malloc(size);
    if (area)
    {    
	area->size = size;
	area->number = 1;
	area->sproff = sizeof(sprite_area);
	area->freeoff = size;

	sprite = first_sprite(area);
	sprite->next = size - sizeof(sprite_area);
	strncpy(sprite->name, "bitmap", sizeof(sprite->name));

	sprite->width = word_width - 1;
	sprite->height = h - 1;

	sprite->lbit = 0;
	sprite->rbit = (bit_width - 1) % 32;

	sprite->image = sizeof(sprite_header) + pal_size;
	sprite->mask = sprite->image;

	sprite->mode = getspritemode(bpp);

	DTRACEBUF((TC_TW, TT_API4, area, sizeof(*area) + sizeof(*sprite)));
    }    
    return area;
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
	memcpy(out, in, in_line_length);	// in_line_length is always less than out_line_length
    }

    ASSERT((descr->bpp == 1), descr->bpp);
}

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

static void fill_sprite_from_screen(sprite_descr *descr, int x, int y)
{
    // remove anhy sprite definition in there already
    descr->area->number = 0;
    descr->area->freeoff = descr->area->sproff;

    // grab sprite from screen
    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
	  16 + 512,
	  descr->area, first_sprite(descr->area),
	  0,	// no palette
	  x*2, y*2,
	  (x + descr->width)*2 - 1, (y + descr->height)*2 - 1));
}

static void copy_palette(void *to, const void *from, int n)
{
    int *t = to;
    const int *f = (const int *)from;
    int i;
    for (i = 0; i < n; i++)
    {
	int w = *f++;
	*t++ = REVERSE_WORD(w);
    }
}

/* ---------------------------------------------------------------------------------------------------- */

static int dib_pal_size(const BITMAPINFOHEADER *info)
{
    return info->biBitCount > 8 ? 0 :
	info->biSize >= 40 && info->biClrUsed ? (int)info->biClrUsed :
	1 << (int)info->biBitCount;
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

/*
 * This routine can cope when the sprite is smaller than the DIB (as in the brush case).
 */

static void fill_sprite_from_dib(sprite_descr *descr, CONST BITMAPINFOHEADER *info, const void *bits)
{
    sprite_header *sprite = first_sprite(descr->area);
    const char *in_data;
    char *out_data;
    int in_line_length, out_line_length;
    int line;

    in_data = (bits ? (const char *)bits : (const char *)info + info->biSize + dib_pal_size(info) * sizeof(RGBQUAD));
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
    out_data += (descr->height - 1 - start) * line_length;
    
    // copy data
    for (line = 0;
	 line < n_lines;
	 line++, in_data += line_length, out_data -= line_length)
    {
	copy_line_from_sprite(out_data, in_data, descr->bpp, descr->width);
    }

    // fill in information
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

    // fill in palette
}

static void fill_palette_from_dib(sprite_descr *descr, CONST BITMAPINFOHEADER *info, int color_table_type)
{
    sprite_header *sprite = first_sprite(descr->area);
    unsigned *in_pal, *out_pal;
    int in_pal_size;
    int i;

    in_pal = (unsigned *)((char *)info + info->biSize);
    in_pal_size = dib_pal_size(info) / 4;

    out_pal = (unsigned *)((char *)sprite + sizeof(sprite_header));

    for (i = 0; i < in_pal_size; i++)
    {
	unsigned p = *in_pal++;
	unsigned q = color_table_type == DIB_PAL_COLORS ? REVERSE_WORD(p) : p;
	*out_pal++ = q;
	*out_pal++ = q;
    }
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

/* ---------------------------------------------------------------------------------------------------- */

static void unlink_object(HGDIOBJ to_unlink)
{
    HGDIOBJ obj, prev;

    DTRACE((TC_TW, TT_API4, "unlink_object: %p gdiobj %p", to_unlink, gdi_objects));

    for (obj = gdi_objects, prev = NULL; obj; prev = obj, obj = obj->gdi.next)
    {
	DTRACE((TC_TW, TT_API4, "unlink_object: obj %p prev %p", obj, prev));

	if (obj == to_unlink)
	{
	    if (prev)
		prev->gdi.next = to_unlink->gdi.next;
	    else
		gdi_objects = to_unlink->gdi.next;
	    break;
	}
    }

    DTRACE((TC_TW, TT_API4, "unlink_object: gdiobj %p", gdi_objects));
}

static void link_object(HGDIOBJ to_link)
{
    DTRACE((TC_TW, TT_API4, "link_object: %p gdiobj %p", to_link, gdi_objects));

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
	free(dc->data.current.brush_coltrans);
	free(dc->data.current.bitmap_coltrans);
	free(dc->data.save_area);
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
	break;
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
	TRACE((TC_TW, TT_API4, "switch__output: dc %p screen", dc));

	LOGERR(_swix(OS_SpriteOp, _INR(0,3), 60, NULL, 0, 1));
	break;

    case OBJ_MEMDC:		// switch to memory
    {
	sprite_area *area;

	TRACE((TC_TW, TT_API4, "switch__output: dc %p", dc));
	ASSERT(dc->data.bitmap, 0);

	area = dc->data.bitmap->data.sprite.area;
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
    RealizePalette(dc);

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
    free(dc->data.current.brush_coltrans);
    dc->data.current.brush_coltrans = NULL;

    free(dc->data.current.bitmap_coltrans);
    dc->data.current.bitmap_coltrans = NULL;
    dc->data.current.bitmap_bitmap = NULL;

    // cached colour numbers
    dc->data.local.pen_col = UNSET_COLOUR;
    dc->data.local.text_col = UNSET_COLOUR;
    dc->data.local.bk_col = UNSET_COLOUR;
}

static void set_current_dc(HDC dc)
{
    // set values
    dc->data.planes = 1;
    dc->data.log2bpp = vduval(vduvar_Log2BPP);
    dc->data.mode = get_mode_number(dc->data.log2bpp);
    dc->data.width = vduval(vduvar_XLimit) + 1;
    dc->data.height = vduval(vduvar_YLimit) + 1;
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

    case 2:			// choose RGB
	ASSERT(dc->data.current.palette, 0);
	
	LOGERR(_swix(ColourTrans_ReturnColourNumberForMode, _INR(0,2) | _OUT(0),
	      (colref & 0x00ffffff) << 8,
	      dc->data.mode, dc->data.current.palette,
	      &colour));
	break;

    case 255:			// my extension for transparent
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
    int size;
    void *table = NULL;

    ASSERT(descr, 0);
    ASSERT(dc->data.current.palette, 0);

    TRACE((TC_TW, TT_API4, "get_colour_lookup_table: dc %p type %d", dc, color_table_type));

    area = descr->area;
    spr = first_sprite(area);

    // if there is no palette in this bitmap
    if (spr->image == sizeof(sprite_header))
    {
	ASSERT(descr->bpp == 1, descr->bpp);

	// a 1 bpp bitmap should use the text/bk colours
	if ((table = malloc(1 << descr->bpp)) == NULL)
	    return NULL;

	// realize text and bk colours if not already
	if (dc->data.local.text_col == UNSET_COLOUR)
	    dc->data.local.text_col = getcolournumber(dc, dc->data.text_col);
	
	if (dc->data.local.bk_col == UNSET_COLOUR)
	    dc->data.local.bk_col = getcolournumber(dc, dc->data.bk_col);
	
	// fill in lookup table
	((char *)table)[0] = dc->data.local.bk_col;
	((char *)table)[1] = dc->data.local.text_col;
    }
    // if there is a palette, use colourtrans
    else
    {
	if (color_table_type == DIB_RGB_COLORS)
	{
	    LOGERR(_swix(ColourTrans_GenerateTable, _INR(0,5) | _OUT(4),
			 area, spr,
			 dc->data.mode, dc->data.current.palette,
			 NULL, 1,
			 &size));
	   
	    if ((table = malloc(size)) == NULL)
		return NULL;
    
	    LOGERR(_swix(ColourTrans_GenerateTable, _INR(0,5),
			 area, spr,
			 dc->data.mode, dc->data.current.palette,
			 table, 1));
	}
	else
	{
	    const ropalette *in;
	    char *out;
	    int i;
	    
	    ASSERT(descr->bpp <= 3, descr->bpp);
	    
	    size = 1 << descr->bpp;
	    if ((table = malloc(size)) == NULL)
		return NULL;

	    in = sprite_palette(spr);
	    out = table;
	    
	    for (i = 0; i < size; i++)
	    {
		*out++ = in->w;
		in += 2;
	    }
	}
    }
    return table;
}

/* ---------------------------------------------------------------------------------------------------- */

BOOL MoveToEx(HDC dc, int x, int y, LPPOINT pPoint)
{
    TRACE((TC_TW, TT_API4, "MoveToEx: dc %p %dx%d", dc, x, y));

    switch_output(dc);

    LOGERR(_swix(OS_Plot, _INR(0,2), 0+4, cvtx(dc, x), cvty(dc, y))); // moveto
    
    dc->data.pen_pos.x = x;
    dc->data.pen_pos.y = y;

    return 1;
}

BOOL LineTo(HDC dc, int x, int y)
{
    TRACE((TC_TW, TT_API4, "LineTo: dc %p %dx%d", dc, x, y));

    switch_output(dc);

    if (dc->data.local.pen_col == UNSET_COLOUR)
	dc->data.local.pen_col = getcolournumber(dc, dc->data.pen->data.colref);
    
    // don't need to set up bgcol as we aren't using any non solid pens
    if (dc->data.local.pen_col != TRANSPARENT_COLOUR)
    {
	BOOL set = FALSE;

	if (dc->data.current.fg_col != dc->data.local.pen_col)
	{
	    dc->data.current.fg_col = dc->data.local.pen_col;
	    set = TRUE;
	}

	if (dc->data.current.action != dc->data.local.action)
	{
	    dc->data.current.action = dc->data.local.action;
	    set = TRUE;
	}

	if (set)
	    LOGERR(_swix(OS_SetColour, _INR(0,1), dc->data.current.action, dc->data.current.fg_col));

	LOGERR(_swix(OS_Plot, _INR(0,2), 0+5, cvtx(dc, x), cvty(dc, y))); // drawto
    }

    dc->data.pen_pos.x = x;
    dc->data.pen_pos.y = y;

    return 1;
}

typedef struct
{
    int xSrc;
    int ySrc;
    int xDest;
    int yDest;
    int Width;
    int Height;
} bitblt_info;

/*
 * Return either the original bitmap or one grabbed from the screen
 */

static HBITMAP get_relevant_bitmap(HDC dc, bitblt_info *info, BOOL *delete_after)
{
    HBITMAP b = dc->data.bitmap;

    if (b == NULL)
    {
	switch_output(dc);
	
	// grab a sprite
	b = CreateCompatibleBitmap(dc, info->Width, info->Height);

	fill_sprite_from_screen(&b->data.sprite, info->xSrc, info->ySrc);
	
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
    switch_output(dc);

    // set a clip window to intersection of the current, Dest and Src
    
#if 1

    if (dc->data.current.bitmap_bitmap != bitmap || dc->data.current.bitmap_coltrans == NULL)
    {
	dc->data.current.bitmap_bitmap = bitmap;
	dc->data.current.bitmap_coltrans = get_colour_lookup_table(dc, &bitmap->data.sprite, bitmap->data.color_table_type);
    }

    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
		 52 + 512,
		 bitmap->data.sprite.area, first_sprite(bitmap->data.sprite.area),
		 cvtx(dc, info->xDest), cvty(dc, info->yDest),
		 action,
		 NULL,
		 dc->data.current.bitmap_coltrans));
#else
    LOGERR(_swix(OS_SpriteOp, _INR(0,5),
		 28 + 512,
		 bitmap->data.sprite.area, first_sprite(bitmap->data.sprite.area),
		 info->xDest*2, info->yDest*2,
		 action));
#endif
}

// need to add support for brush rotation in here
// this will be a different colour depth probably...

static void tilebrush(HDC dc, const bitblt_info *info, int action)
{
    HBRUSH brush;
    int x, y;

    switch_output(dc);

    brush = dc->data.brush;

    // get and cache the colourtrans table
    if (dc->data.current.brush_coltrans == NULL)
	dc->data.current.brush_coltrans = get_colour_lookup_table(dc, &brush->data.sprite, brush->data.color_table_type);
    
    for (x = 0; x < info->Width; x += 8)
	for (y = 0; y < info->Height; y += 8)
	{
#if 1
	    LOGERR(_swix(OS_SpriteOp, _INR(0,7),
			 52 + 512,
			 brush->data.sprite.area, first_sprite(brush->data.sprite.area),
			 cvtx(dc, info->xDest + x), cvty(dc, info->yDest + y),
			 action,
			 NULL,
			 dc->data.current.brush_coltrans));
#else
	    LOGERR(_swix(OS_SpriteOp, _INR(0,5),
		  28 + 512,
		  brush->data.sprite.area, first_sprite(brush->data.sprite.area),
		  (info->xDest + x)*2, (info->yDest + y)*2,
		  action));
#endif
	}   
}

/*
 * This can be from screen to mem or mem to screen.
 */

BOOL BitBlt(HDC dcDest, int xDest, int yDest, int Width, int Height, HDC dcSrc, int xSrc, int ySrc, DWORD rop3)
{
    bitblt_info info;
    HBITMAP bitmap;
    int delbitmap;

    TRACE((TC_TW, TT_API4, "BitBlt: %p%s %dx%d %dx%d from %p%s %dx%d brush %p rop3=0x%x",
	   dcDest, dcDest == screen_dc ? " (screen)" : "",
	   xDest, yDest, Width, Height,
	   dcSrc, dcSrc == screen_dc ? " (screen)" : "",
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
	tilebrush(dcDest, &info, plotaction_XOR);
	break;

    case PATCOPY:
	tilebrush(dcDest, &info, plotaction_WRITE);
	break;

	
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
    default:
	TRACE((TC_TW, TT_API4, "BitBlt: unsupported rop3", rop3));
	ASSERT(0,0);
	break;
    }    
    
    if (delbitmap)
	DeleteObject(bitmap);

    TRACE((TC_TW, TT_API4, "BitBlt: out"));

    return 1;
}

/*
 * Note 'dest' and 'rgn1' can be the same.
 */

int CombineRgn(HRGN rgn_dest, HRGN rgn1, HRGN rgn2, int type)
{
    TRACE((TC_TW, TT_API4, "CombineRgn:"));
    ASSERT(type == RGN_OR, type);

    switch (type)
    {
    case RGN_OR:
    {
	region_rect *reg1, *n;
	region_rect *regd, *regd_last;
	
	ASSERT(rgn1, (int)rgn1);
	ASSERT(rgn2, (int)rgn2);
	ASSERT(rgn_dest, (int)rgn_dest);
	ASSERT(rgn2->data.base->next == NULL, (int)rgn2->data.base->next);

	// copy existing rectangles
	regd = regd_last = NULL;
	for (reg1 = rgn1->data.base; reg1; reg1 = reg1->next)
	{
	    n = calloc(sizeof(*n), 1);

	    // fill in data
	    n->rect = regd->rect;

	    // link into chain
	    if (regd_last)
		regd_last->next = n;
	    else
		regd = regd_last = n;
	    regd_last = n;
	}

	// add on most recent one
	n = calloc(sizeof(*n), 1);
	n->rect = rgn2->data.base->rect;
	regd_last->next = n;

	// free the original dest chain
	free_region(rgn_dest);

	// write in the newly allocated one
	rgn_dest->data.base = regd;
	
	break;
    }
    }

    return 1;
}

/* CreateBitmap can be called with a stream of data that represents the
 * image data in a packed form.
 */

HBITMAP CreateBitmap(int w, int h, UINT planes, UINT bpp, CONST VOID *data)
{
    BITMAP b;

    TRACE((TC_TW, TT_API4, "CreateBitmap:"));

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

    TRACE((TC_TW, TT_API4, "CreateBitmapIndirect:"));

    bitmap = (HBITMAP)create_object(OBJ_BITMAP, sizeof(*bitmap));

    if (bitmap == NULL)
	return NULL;
    
    bitmap->data.sprite.area = create_sprite((int)b->bmWidth, (int)b->bmHeight, b->bmBitsPixel, FALSE);
    bitmap->data.sprite.width = (int)b->bmWidth;
    bitmap->data.sprite.height = (int)b->bmHeight;
    bitmap->data.sprite.bpp = b->bmBitsPixel;
    bitmap->data.color_table_type = DIB_RGB_COLORS; // actually none at all

    if (bitmap->data.sprite.area)
    {
	if (b->bmBits)
	    fill_sprite(&bitmap->data.sprite, b->bmBits, (int)b->bmWidthBytes);
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

    TRACE((TC_TW, TT_API4, "CreateCompatibleBitmap: dc %p %dx%d", dc, w, h));

    memset(&b, 0, sizeof(b));

    b.bmWidth = w;
    b.bmHeight = h;
    b.bmWidthBytes = RoundShort((w << dc->data.log2bpp) / 8);
    b.bmPlanes = dc->data.planes;
    b.bmBitsPixel = 1 << dc->data.log2bpp;

    return CreateBitmapIndirect(&b);
}

/*
 * Get a compatible context to the screen
 */

HDC CreateCompatibleDC(HDC dcBase)
{
    HDC dc;

    TRACE((TC_TW, TT_API4, "CreateCompatibleDC: dc %p", dcBase));

    dc = (HDC)create_object(OBJ_MEMDC, sizeof(*dc));
    if (dc == NULL)
	return NULL;
    
    set_default_dc(dc);

    dc->data.planes = dcBase->data.planes;
    dc->data.log2bpp = dcBase->data.log2bpp;
    dc->data.mode = dcBase->data.mode;
    
    return dc;
}

int FillRect(HDC hdc, CONST RECT *lprc, HBRUSH hbr)
{
    HBRUSH old;

    TRACE((TC_TW, TT_API4, "FillRect:"));

    old = SelectObject(hdc, hbr);

    PatBlt(hdc, lprc->left, lprc->top,
	   lprc->right - lprc->left, 
	   lprc->bottom - lprc->top,
	   PATCOPY);

    SelectObject(hdc, old);
    return 1;
}

/*
 * Get the context for the screen
 */

HDC GetDC(HWND hwnd)
{
    HDC dc = screen_dc;

    TRACE((TC_TW, TT_API4, "GetDC:"));

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
 * The bistream is in 'bits'
 * The palette is in 'dib'
 * 'color_table_type' says what kind of palette it is.
 * flags has whether something shouldbe initialised or not.
 */


HBITMAP CreateDIBitmap(HDC dc, CONST BITMAPINFOHEADER *info, DWORD flags, CONST VOID *bits, CONST BITMAPINFO *dib, UINT color_table_type)
{
    HBITMAP bitmap;

    bitmap = (HBITMAP)create_object(OBJ_BITMAP, sizeof(*bitmap));

    TRACE((TC_TW, TT_API4, "CreateDIBitmap: %p", bitmap));

    if (bitmap == NULL)
	return NULL;
    
    bitmap->data.sprite.width = (int)info->biWidth;
    bitmap->data.sprite.height = (int)info->biHeight;
    bitmap->data.sprite.bpp = info->biBitCount;
    bitmap->data.sprite.area = create_sprite((int)info->biWidth, (int)info->biHeight, info->biBitCount, TRUE);
    bitmap->data.color_table_type = color_table_type;
    
    if (bitmap->data.sprite.area == NULL)
    {
	free_object((HGDIOBJ)bitmap);
	return NULL;
    }

    fill_sprite_from_dib(&bitmap->data.sprite, info, bits);
    fill_palette_from_dib(&bitmap->data.sprite, info, color_table_type);

    return bitmap;
}

HBRUSH CreateDIBPatternBrush(HGLOBAL gdib, UINT color_table_type)
{
    HBRUSH brush;
    const BITMAPINFOHEADER *info = (const BITMAPINFOHEADER *)gdib;

    TRACE((TC_TW, TT_API4, "CreateDIBPatternBrush:"));

    brush = (HBRUSH)create_object(OBJ_BRUSH, sizeof(*brush));
    if (brush == NULL)
	return NULL;
    
    ASSERT(info->biWidth >= 8, (int)info->biWidth);
    ASSERT(info->biHeight >= 8, (int)info->biHeight);
    
    brush->data.sprite.width = 8;
    brush->data.sprite.height = 8;
    brush->data.sprite.bpp = info->biBitCount;
    brush->data.sprite.area = create_sprite(8, 8, info->biBitCount, TRUE);
    brush->data.color_table_type = color_table_type;

    if (brush->data.sprite.area == NULL)
    {
	free_object((HGDIOBJ)brush);
	return NULL;
    }
    
    fill_sprite_from_dib(&brush->data.sprite, info, NULL);
    fill_palette_from_dib(&brush->data.sprite, info, color_table_type);

    return brush;
}

HPALETTE CreatePalette(CONST LOGPALETTE *pal)
{
    HPALETTE palette;

    TRACE((TC_TW, TT_API4, "CreatePalette:"));

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

    TRACE((TC_TW, TT_API4, "CreatePen: color 0x%x", colref));

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

    TRACE((TC_TW, TT_API4, "CreateRectRgn:"));

    r.top = top;
    r.bottom = bottom;
    r.left = left;
    r.right = right;

    return CreateRectRgnIndirect(&r);
}

HRGN CreateRectRgnIndirect(CONST RECT *prect)
{
    HRGN rgn;
    region_rect *rr;

    TRACE((TC_TW, TT_API4, "CreateRectRgnIndirect:"));

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

    TRACE((TC_TW, TT_API4, "CreateSolidBrush: color 0x%x", colref));

    brush = (HBRUSH)create_object(OBJ_BRUSH, sizeof(*brush));
    if (brush == NULL)
	return NULL;
    
    brush->data.sprite.area = create_sprite(8, 8, 1, TRUE);

    pal[0].w = 0;
    switch (colref >> 24)
    {
    case 1:			// choose palette entry
	pal[1].w = (int)colref & 0x000000ff;
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

    return brush;
}

BOOL DeleteDC(HDC dc)
{
    TRACE((TC_TW, TT_API4, "DeleteDC: dc %p", dc));
    return DeleteObject((HGDIOBJ)dc);
}

BOOL DeleteObject(LPVOID oobj)
{
    HGDIOBJ obj = oobj;
    
    TRACE((TC_TW, TT_API4, "DeleteObject: %p %s (%d)",
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
    TRACE((TC_TW, TT_API4, "GetDIBits: cd %p bitmap %p lines %d(%d) usage %d", dc, bitmap, start, num, usage));
    
    fill_dib_from_sprite(&bitmap->data.sprite, info, bits, start, num);

    return num;
}

int GetRgnBox(HRGN rgn, LPRECT rect)
{
    TRACE((TC_TW, TT_API4, "GetRgnBox: rgn %p", rgn));
    return 1;
}

HGDIOBJ GetStockObject(int type)
{
    HGDIOBJ obj;

    TRACE((TC_TW, TT_API4, "GetStockObject: %d", type));

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
    TRACE((TC_TW, TT_API4, "GetSystemPaletteEntries: dc %p what %d size %d to %p", dc, what, size, pal));

    ASSERT(dc->data.current.palette, 0);

    copy_palette(pal, dc->data.current.palette, size);

    return 1;
}

UINT GetSystemPaletteUse(HDC dc)
{
    TRACE((TC_TW, TT_API4, "GetSystemPaletteUse: dc %p", dc));

    return dc->data.syspal;
}

BOOL IntersectRect(LPRECT lprcDst, CONST RECT *lprcSrc1, CONST RECT *lprcSrc2)
{
    TRACE((TC_TW, TT_API4, "IntersectRect:"));

    lprcDst->left = MAX(lprcSrc1->left, lprcSrc2->left);
    lprcDst->right = MIN(lprcSrc1->right, lprcSrc2->right);
    lprcDst->bottom = MAX(lprcSrc1->bottom, lprcSrc2->bottom);
    lprcDst->top = MIN(lprcSrc1->top, lprcSrc2->top);
			
    return lprcDst->left < lprcDst->right && lprcDst->bottom < lprcDst->top;
}

/* Use the brush set in 'dc' to paint the given rectangle of 'dc'
 * using the given ROP code (which is limited to those that don't use
 * a src HDC
 */

BOOL PatBlt(HDC dc, int x, int y, int w, int h, DWORD rop3)
{
    TRACE((TC_TW, TT_API4, "PatBlt: %p%s %dx%d %dx%d rop3=0x%x",
	   dc, dc == screen_dc ? " (screen)" : "", x, y, w, h, rop3));

    BitBlt(dc, x, y, w, h, dc, 0, 0, rop3);
    
    return 1;
}

/* Take the last logical palette set by SelectPalette() and try to
 * grab the necessary colours.
 * Return the number of colours successfully realized.
 */

UINT RealizePalette(HDC dc)
{
    TRACE((TC_TW, TT_API4, "RealizePalette: dc %p%s", dc, dc == screen_dc ? " (screen)" : ""));

    ASSERT(dc->data.palette, 0);
    ASSERT(dc->data.palette->data.colours, 0);
    
    TRACE((TC_TW, TT_API4, "RealizePalette: pal %p ncols %d ", dc->data.palette, dc->data.palette->data.n_entries));

    if (dc->data.current.palette == NULL)
    {
	dc->data.current.palette = malloc(sizeof(ropalette) << dc->data.log2bpp);

	if (dc->data.current.palette == NULL)
	    return 0;
    }

    copy_palette(dc->data.current.palette, dc->data.palette->data.colours, dc->data.palette->data.n_entries);
    
    if (dc == screen_dc)
    {
	TRACEBUF((TC_TW, TT_API4, dc->data.current.palette, sizeof(ropalette) << dc->data.log2bpp));
	LOGERR(_swix(ColourTrans_WritePalette, _INR(0,4),
		     -1, -1,
		     dc->data.current.palette,
		     0, 0));
    }
    
    return dc->data.palette->data.n_entries;
}

int ReleaseDC(HWND hwnd, HDC dc)
{
    TRACE((TC_TW, TT_API4, "ReleaseDC:"));
    return 1;
}

int SelectClipRgn(HDC dc, HRGN rgn)
{
    TRACE((TC_TW, TT_API4, "SelectClipRgn: dc %p rgn %p", dc, rgn));

    if (rgn == NULL)
	dc->data.clip = NULL;
    else
	SelectObject(dc, (HGDIOBJ)rgn);
    
    return 1;
}

LPVOID SelectObject(HDC dc, LPVOID oobj)
{
    HGDIOBJ obj = oobj;

    TRACE((TC_TW, TT_API4, "SelectObject: dc %p obj %p %s %d",
	   dc, obj,
	   objects[obj ? obj->gdi.tag : 0],
	   obj ? obj->gdi.tag : 0));
    ASSERT(obj, 0);
	
    switch (obj->gdi.tag)
    {
    case OBJ_PEN:
	dc->data.pen = (HPEN)obj;

	dc->data.local.pen_col = UNSET_COLOUR;
	break;

    case OBJ_BRUSH:
	dc->data.brush = (HBRUSH)obj;

	free(dc->data.current.brush_coltrans);
	dc->data.current.brush_coltrans = NULL;
	break;

    case OBJ_BITMAP:
	ASSERT(dc->gdi.tag == OBJ_MEMDC, dc->gdi.tag);

	if (dc->gdi.tag == OBJ_MEMDC)
	{
	    dc->data.bitmap = (HBITMAP)obj;

	    uncache_dc(dc);
	    // transfer palette?
	}
	break;

    case OBJ_REGION:
	dc->data.clip = (HRGN)obj;
	break;

    default:
	ASSERT(0, obj->gdi.tag);
	break;
    }

    return obj;
}

HPALETTE SelectPalette(HDC dc, HPALETTE pal, BOOL foreground)
{
    HPALETTE old_pal = NULL;

    TRACE((TC_TW, TT_API4, "SelectPalette: dc %p pal %p", dc, pal));

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

    TRACE((TC_TW, TT_API4, "SetBkColor: color 0x%x", col));

    dc->data.bk_col = col;
    dc->data.local.bk_col = UNSET_COLOUR;

    return old;
}

int SetDIBitsToDevice(HDC dc,
		      int dest_x, int dest_y,	// top left of destination rectangle
		      DWORD w, DWORD h,		// image size to transfer
		      int src_x, int src_y,	// start position in source DIB
		      UINT stuff1, UINT stuff2,	// ????
		      CONST VOID *bits, CONST BITMAPINFO *info, UINT dib_plot)
{
    TRACE((TC_TW, TT_API4, "SetDIBitsToDevice:"));

    switch_output(dc);

    return 1;
}
			
int SetROP2(HDC dc, int rop2)
{
    int old = dc->data.rop2;

    TRACE((TC_TW, TT_API4, "SetROP2: dc %p rop2 %d", dc, rop2));

    dc->data.rop2 = rop2;
    dc->data.local.action = rop2_to_action[(rop2 - 1) & 0x0F];

    ASSERT((dc->data.local.action != 0xFF), rop2);

    return old;
}

BOOL SetBrushOrgEx(HDC dc, int x, int y, LPPOINT pt)
{
    TRACE((TC_TW, TT_API4, "SetBrushOrgEx:"));

    dc->data.brush_org.x = x;
    dc->data.brush_org.y = y;

    ASSERT((pt == NULL), (int)pt);
    
    return 1;
}

UINT SetSystemPaletteUse(HDC dc, UINT syspal)
{
    int old = dc->data.syspal;

    TRACE((TC_TW, TT_API4, "SetSystemPaletteUse: dc %p %d", dc, syspal));

    dc->data.syspal = syspal;
    return old;
}

COLORREF SetTextColor(HDC dc, COLORREF col)
{
    COLORREF old = dc->data.text_col;

    TRACE((TC_TW, TT_API4, "SetTextColor: color 0x%x", col));

    dc->data.text_col = col;
    dc->data.local.text_col = UNSET_COLOUR;

    return old;
}

BOOL UnrealizeObject(HGDIOBJ obj)
{
    TRACE((TC_TW, TT_API4, "UnrealizeObject: %p", obj));

    switch (obj->gdi.tag)
    {
    case OBJ_PAL:
	// remap logical palette to system next time it is needed?
	break;

    case OBJ_BRUSH:
	// reset origin on next use
	break;

    default:
	ASSERT(0, obj->gdi.tag);
	break;
    }
    return 1;
}

void RestoreScreen(void)
{
    TRACE((TC_TW, TT_API4, "RestoreScreen:"));

    switch_output(screen_dc);
}

/* ---------------------------------------------------------------------------------------------------- */

/* realize the cursor on the screen */

HCURSOR SetCursor(HCURSOR hCursor)
{
    TRACE((TC_TW, TT_API4, "SetCursor:"));
    return hCursor;
}

#if 0
BOOL GetCursorPos(LPPOINT lpPoint)
{
    return 1;
}
#endif

HCURSOR CreateCursor(HINSTANCE hInst, int xHotSpot, int yHotSpot, int nWidth, int nHeight, CONST VOID *pvANDPlane, CONST VOID *pvXORPlane)
{
    HCURSOR cursor;
    TRACE((TC_TW, TT_API4, "CreateCursor:"));
    cursor = (HCURSOR)create_object(OBJ_CURSOR, sizeof(*cursor));
    return cursor;
}

BOOL DestroyCursor(HCURSOR hCursor)
{
    TRACE((TC_TW, TT_API4, "DestroyCursor: %p", hCursor));
    DeleteObject(hCursor);
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

    TRACE((TC_TW, TT_API4, "SetMode: %d x %d @ %d bpp", xres, yres, 1 << m[3]));

    LOGERR(_swix(OS_ScreenMode, _INR(0,1), 0, m));

    // update the screen dc values
    set_current_dc(screen_dc);
    uncache_dc(screen_dc);

    (void) MouseShowPointer( TRUE );
}

/* ---------------------------------------------------------------------------------------------------- */

/* eof twro.c */
