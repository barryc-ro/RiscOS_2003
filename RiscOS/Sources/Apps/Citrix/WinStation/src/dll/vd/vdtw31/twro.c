/* > twro.c

 * RISCOS Thinwire routines

 */

#include "windows.h"

#include <stdlib.h>
#include <string.h>

#include "swis.h"

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

#define NULL_COLOUR		0xFF000000
#define TRANSPARENT_COLOUR	(-1)

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
	HPEN pen;
	HBRUSH brush;
	HBITMAP bitmap;		// bitmap that represents the drawing surface, MEMDC only
	POINT pen_pos;
	COLORREF bk_col;
	COLORREF text_col;
	POINT brush_org;
	HRGN clip;
	HPALETTE palette;
	int rop2;

	int syspal;


	int planes;
	int bpp;

	int mode;
	void *save_area;	// save area for switching output

	struct
	{
	    int pen_col;	// current mode versions of above colours
	    int text_col;
	    int bk_col;
	    int action;
	} local;

	struct
	{
	    int fg_col;		// actual current colours
	    int bg_col;
	    int action;

	    COLORREF *palette;	// 1 word per col palette
	} current;
    } data;
} DC;

typedef struct RGN__
{
    gdi_hdr gdi;

    struct
    {
	HRGN next;
	RECT rect;
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
    } data;
} brush;

typedef struct BITMAP__
{
    gdi_hdr gdi;

    struct
    {
	sprite_descr sprite;
    } data;
} bitmap;

typedef struct GDIOBJ__
{
    gdi_hdr gdi;
} gdiobj;

/* ---------------------------------------------------------------------------------------------------- */

static HGDIOBJ gdi_objects = NULL;
static HGDIOBJ stock_objects[STOCK_LAST] = { 0 };
static HDC current_dc = NULL;
static HDC screen_dc = NULL;

static char rop2_to_action[16] =
{
    0xFF,	// R2_BLACK,
    0xFF,	// R2_NOTMERGEPEN,
    6,		// R2_MASKNOTPEN,
    0xFF,	// R2_NOTCOPYPEN,
    0xFF,	// R2_MASKPENNOT,
    4,		// R2_NOT,
    3,		// R2_XORPEN,
    0xFF,	// R2_NOTMASKPEN,
    2,		// R2_MASKPEN,
    0xFF,	// R2_NOTXORPEN,
    5,		// R2_NOP,
    7,		// R2_MERGENOTPEN,
    0,		// R2_COPYPEN,
    0xFF,	// R2_MERGEPENNOT,
    1,		// R2_MERGEPEN,
    0xFF	// R2_WHITE
};

/* ---------------------------------------------------------------------------------------------------- */

#define first_sprite(area)	((sprite_header *)((char *)(area) + (area)->sproff))
#define sprite_data(sprite)	((char *)(sprite) + (sprite)->image)

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

    int size = sizeof(sprite_area) +
	sizeof(sprite_header) +
	(palette ? (8 << bpp) : 0) +
	((w * bpp + 31)/32) * 4 * h;

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

	sprite->width = ((w * bpp + 31)/32) * 4 - 1;
	sprite->height = h - 1;

	sprite->lbit = 0;
	sprite->rbit = (w * bpp - 1) % 32;

	sprite->image = size - sizeof(sprite_area) - sizeof(sprite_header);
	sprite->mask = sprite->image;

	sprite->mode = getspritemode(bpp);
    }    
    return area;
}

static void fill_sprite(sprite_descr *descr, void *bits)
{
    sprite_header *sprite = first_sprite(descr->area);
    int line_length = (sprite->width+1) * 4;

#if 0

    for (line = 0; line < nlines; line++)
	memcpy(spritedata, bits, linelength);

#endif
}

static void clear_sprite(sprite_descr *descr, int val)
{
    sprite_header *sprite = first_sprite(descr->area);
    memset((char *)sprite + sprite->image, val, sprite->next - sprite->image);
}

static void set_sprite_palette(sprite_descr *descr, COLORREF *palette)
{
    sprite_header *sprite = first_sprite(descr->area);
    int n_entries = (sprite->image - sizeof(sprite_header)) / 8;
    COLORREF *pal = (COLORREF *)((char *)sprite + sizeof(sprite_header));
    int i;

    for (i = 0; i < n_entries; i++)
    {
	pal[1] = pal[0] = *palette++;
	pal += 2;
    }
}

/* ---------------------------------------------------------------------------------------------------- */

static void unlink_object(HGDIOBJ to_unlink)
{
    HGDIOBJ obj, prev;
    for (obj = gdi_objects, prev = NULL; obj; prev = obj, obj = obj->gdi.next)
    {
	if (obj == to_unlink)
	{
	    if (prev)
		prev->gdi.next = to_unlink->gdi.next;
	    else
		gdi_objects = to_unlink->gdi.next;
	    break;
	}
    }
}

static void link_object(HGDIOBJ to_link)
{
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
	free(dc->data.palette);
	free(dc->data.save_area);
	break;
    }

    case OBJ_PAL:
    {
	HPALETTE palette = (HPALETTE)obj;
	free(palette->data.colours);
	break;
    }
    }

    free(obj);
}

/* ---------------------------------------------------------------------------------------------------- */

static void switch__output(HDC dc)
{
    switch (dc->gdi.tag)
    {
    case OBJ_DC:		// switch to screen
	_swix(OS_SpriteOp, _INR(0,3), 60, NULL, 0, 1);
	break;

    case OBJ_MEMDC:		// switch to memory
    {
	sprite_area *area = dc->data.bitmap->data.sprite.area;

	// allocate a save area if there isn't one already
	if (dc->data.save_area == NULL)
	{
	    int size;
	    _swix(OS_SpriteOp, _INR(0,2) | _OUT(3), 62, area, first_sprite(area), &size);
	    dc->data.save_area = malloc(size);
	    *(int *)dc->data.save_area = 0;
	}

	_swix(OS_SpriteOp, _INR(0,3), 60, area, first_sprite(area), dc->data.save_area);
	break;
    }
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

/* ---------------------------------------------------------------------------------------------------- */

static int getcolournumber(HDC dc, COLORREF colref)
{
    int colour = -1;

    switch (colref >> 24)
    {
    case 1:			// choose palette entry
	colour = colref & 0x000000ff;
	break;

    case 2:			// choose RGB
	_swix(ColourTrans_ReturnColourNumberForMode, _INR(0,2) | _OUT(0),
	      (colref & 0x00ffffff) << 8,
	      dc->data.mode, dc->data.current.palette,
	      &colour);
	break;

    case 255:			// my extension for transparent
	colour = TRANSPARENT_COLOUR;
	break;
	
    default:
	ERROR("getcolournumber: unknown COLORREF type 0x%x", colref);
	break;
    }

    return colour;
}

static COLORREF getrgbval(HDC dc, COLORREF colref)
{
    COLORREF colour = -1;

    switch (colref >> 24)
    {
    case 1:			// choose palette entry
	colour = dc->data.current.palette[colref & 0x000000ff];
	break;

    case 2:			// choose RGB
	colour = (colref & 0x00ffffff) << 8;
	break;

    case 255:			// my extension for transparent
	colour = -1;
	break;
	
    default:
	ERROR("getrgbval: unknown COLORREF type 0x%x", colref);
	break;
    }

    return colour;
}

/* ---------------------------------------------------------------------------------------------------- */

BOOL MoveToEx(HDC dc, int x, int y, LPPOINT pPoint)
{
    switch_output(dc);

    _swix(OS_Plot, _INR(0,2), 0+4, x, y); // moveto
    
    dc->data.pen_pos.x = x;
    dc->data.pen_pos.y = y;

    return 1;
}

BOOL LineTo(HDC dc, int x, int y)
{
    switch_output(dc);

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
	    _swix(OS_SetColour, _INR(0,1), dc->data.current.action, dc->data.current.fg_col);

	_swix(OS_Plot, _INR(0,2), 0+5, x, y); // drawto
    }

    dc->data.pen_pos.x = x;
    dc->data.pen_pos.y = y;

    return 1;
}

BOOL BitBlt(HDC dcDest, int xDest, int yDest, int Width, int Height, HDC dcSrc, int xSrc, int ySrc, DWORD rop)
{
    switch_output(dcDest);

    return 1;
}

int CombineRgn(HRGN dest, HRGN r1, HRGN r2, int type)
{
    switch (type)
    {
    case RGN_OR:
	break;

    default:
	ERROR("CombineRgn: unknown merge type %d", type);
	break;
    }

    return 1;
}

/* CreateBitmap is called with a stream of data that represents the
 * image data in a packed form?
 */

HBITMAP CreateBitmap(int w, int h, UINT planes, UINT bpp, CONST VOID *data)
{
    BITMAP b;
    memset(&b, 0, sizeof(b));

    b.bmWidth = w;
    b.bmHeight = h;
    b.bmWidthBytes = RoundShort(w*bpp/8);
    b.bmPlanes = planes;
    b.bmBitsPixel = bpp;
    b.bmBits = (LPVOID)data;

    return CreateBitmapIndirect(&b);
}

HBITMAP CreateBitmapIndirect(CONST BITMAP *b)
{
    HBITMAP bitmap = (HBITMAP)create_object(OBJ_BITMAP, sizeof(*bitmap));

    bitmap->data.sprite.area = create_sprite(b->bmWidth, b->bmHeight, b->bmBitsPixel, FALSE);
    bitmap->data.sprite.width = b->bmWidth;
    bitmap->data.sprite.height = b->bmHeight;
    bitmap->data.sprite.bpp = b->bmBitsPixel;

    if (bitmap->data.sprite.area == NULL)
    {
	free_object((HGDIOBJ)bitmap);
	bitmap = NULL;
    }
    else
	fill_sprite(&bitmap->data.sprite, b->bmBits);
    
    return bitmap;
}

HBITMAP CreateCompatibleBitmap(HDC dc, int w, int h)
{
    BITMAP b;
    memset(&b, 0, sizeof(b));

    b.bmWidth = w;
    b.bmHeight = h;
    b.bmWidthBytes = RoundShort(w * dc->data.bpp / 8);
    b.bmPlanes = dc->data.planes;
    b.bmBitsPixel = dc->data.bpp;

    return CreateBitmapIndirect(&b);
}

/*
 * Set the default values for a context
 */

static void set_default_dc(HDC dc)
{
    SelectObject(dc, GetStockObject(BLACK_PEN));
    SelectObject(dc, GetStockObject(WHITE_BRUSH));

    SetBkColor(dc, PALETTERGB(0xFF, 0xFF, 0xFF));
    SetTextColor(dc, PALETTERGB(0, 0, 0));
    SetROP2(dc, R2_COPYPEN);
    SelectPalette(dc, (HPALETTE)GetStockObject(DEFAULT_PALETTE), TRUE);
}

/*
 * Get a compatible context to the screen
 */

HDC CreateCompatibleDC(HDC dcBase)
{
    HDC dc = (HDC)create_object(OBJ_MEMDC, sizeof(*dc));

    set_default_dc(dc);
    
    return dc;
}

/*
 * Get the context for the screen
 */

HDC GetDC(void)
{
    HDC dc = screen_dc;
    if (dc == NULL)
    {
	dc = screen_dc = (HDC)create_object(OBJ_DC, sizeof(*dc));
	set_default_dc(dc);
    }    
    return dc;
}

HBITMAP CreateDIBitmap(HDC dc, CONST BITMAPINFOHEADER *dib, DWORD flags, CONST VOID *bits, CONST BITMAPINFO *dib1, UINT color_table_type)
{
    HBITMAP bitmap = (HBITMAP)create_object(OBJ_BITMAP, sizeof(*bitmap));

    return bitmap;
}

HBRUSH CreateDIBPatternBrush(HGLOBAL dib, UINT color_table_type)
{
    HBRUSH brush = (HBRUSH)create_object(OBJ_BRUSH, sizeof(*brush));

    brush->data.sprite.area = create_sprite(8, 8, dib_bpp(dib), FALSE);
    fill_sprite_with_dib(&brush->data.sprite, dib);

    return brush;
}

HPALETTE CreatePalette(CONST LOGPALETTE *pal)
{
    HPALETTE palette = (HPALETTE)create_object(OBJ_PAL, sizeof(*palette));

    palette->data.n_entries = pal->palNumEntries;
    palette->data.colours = malloc(pal->palNumEntries * sizeof(pal->palPalEntry[0]));
    memcpy(palette->data.colours, pal->palPalEntry, pal->palNumEntries * sizeof(pal->palPalEntry[0]));
    
    return palette;
}

HPEN CreatePen(int style, int width, COLORREF colref)
{
    HPEN pen = (HPEN)create_object(OBJ_PEN, sizeof(*pen));

    pen->data.colref = colref;
    pen->data.style = style;
    pen->data.width = width;

    if (style != PS_SOLID)
    {
	ERROR("CreatePen: unknown style used %d\n", style);
    }
    if (width != 1)
    {
	ERROR("CreatePen: unknown width used %d\n", width);
    }

    return pen;
}

HRGN CreateRectRgn(int left, int top, int right, int bottom) // ??????
{
    RECT r;

    r.top = top;
    r.bottom = bottom;
    r.left = left;
    r.right = right;

    return CreateRectRgnIndirect(&r);
}

HRGN CreateRectRgnIndirect(CONST RECT *prect)
{
    HRGN rgn = (HRGN)create_object(OBJ_REGION, sizeof(*rgn));

    rgn->data.rect = *prect;
    
    return rgn;
}

HBRUSH CreateSolidBrush(COLORREF color)
{
    HBRUSH brush = (HBRUSH)create_object(OBJ_BRUSH, sizeof(*brush));
#if 0
    brush->data.sprite.area = create_sprite(8, 8, 8, TRUE);

    if (brush->data.sprite.area)
    {
	COLORREF pal[2];

	clear_sprite(&brush->data.sprite, 0xff);

	pal[0] = 0;
	pal[1] = getrgb(color);
	set_sprite_palette(&brush->data.sprite, pal);
    }
#endif    
    return brush;
}

BOOL DeleteDC(HDC dc)
{
    return DeleteObject((HGDIOBJ)dc);
}

BOOL DeleteObject(LPVOID oobj)
{
    HGDIOBJ obj = oobj;
    
    free_object(obj);

    return 1;
}

/*
 * Read from 'bitmap' into the DIB addressed by 'info'.
 * Write the DIB parameters set into 'info'.
 * Always uses whole scan lines. start = 0 means the bottom line of the image.
 */

int GetDIBits(HDC dc, HBITMAP bitmap, UINT start, UINT num, LPVOID bits, LPBITMAPINFO info, UINT usage)
{
    switch_output(dc);

    return 1;
}

int GetRgnBox(HRGN rgn, LPRECT rect)
{
    return 1;
}

HGDIOBJ GetStockObject(int type)
{
    HGDIOBJ obj;

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
	ERROR("GetStockObject: unknown type %d", type);
	obj = NULL;
    }
    return obj;
}

UINT GetSystemPaletteEntries(HDC dc, UINT what, UINT size, LPPALETTEENTRY pal)
{
    memcpy(pal, dc->data.current.palette, size * sizeof(PALETTEENTRY));
    return 1;
}

UINT GetSystemPaletteUse(HDC dc)
{
    return dc->data.syspal;
}

/* Use the brush set in 'dc' to paint the given rectangle of 'dc'
 * using the given ROP code (which is limited to those that don't use
 * a src HDC
 */

BOOL PatBlt(HDC dc, int x, int y, int w, int h, DWORD rop)
{
    switch_output(dc);
    
    return 1;
}

/* Take the last logical palette set by SelectPalette() and try to
 * grab the necessary colours.
 */

UINT RealizePalette(HDC dc)
{
    return 1;
}

int SelectClipRgn(HDC dc, HRGN rgn)
{
    SelectObject(dc, (HGDIOBJ)rgn);
    return 1;
}

LPVOID SelectObject(HDC dc, LPVOID oobj)
{
    HGDIOBJ obj = oobj;
    switch (obj->gdi.tag)
    {
    case OBJ_PEN:
	dc->data.pen = (HPEN)obj;

	dc->data.local.pen_col = getcolournumber(dc, dc->data.pen->data.colref);
	break;

    case OBJ_BRUSH:
	dc->data.brush = (HBRUSH)obj;
	break;

    case OBJ_BITMAP:
	dc->data.bitmap = (HBITMAP)obj;
	break;

    case OBJ_REGION:
	dc->data.clip = (HRGN)obj;
	break;

    default:
	ERROR("SelectObject: unknown object type %d", obj->gdi.tag);
	break;
    }

    return obj;
}

HPALETTE SelectPalette(HDC dc, HPALETTE pal, BOOL foreground)
{
    HPALETTE old_pal = NULL;

    dc->data.palette = pal;
    
    return old_pal;
}

COLORREF SetBkColor(HDC dc, COLORREF col)
{
    COLORREF old = dc->data.bk_col;
    dc->data.bk_col = col;

    dc->data.local.bk_col = getcolournumber(dc, col);
    return old;
}

int SetDIBitsToDevice(HDC dc,
		      int dest_x, int dest_y,	// top left of destination rectangle
		      DWORD w, DWORD h,		// image size to transfer
		      int src_x, int src_y,	// start position in source DIB
		      UINT stuff1, UINT stuff2,	// ????
		      CONST VOID *bits, CONST BITMAPINFO *info, UINT dib_plot)
{
    switch_output(dc);

    return 1;
}
			
int SetROP2(HDC dc, int rop2)
{
    int old = dc->data.rop2;
    dc->data.rop2 = rop2;

    dc->data.local.action = rop2_to_action[rop2 & 0x0F];

    if (dc->data.local.action == 0xFF)
    {
	ERROR("SetROP2: unknown rop2 code %d\n", rop2);
    }

    return old;
}

BOOL SetBrushOrgEx(HDC dc, int x, int y, LPPOINT pt)
{
    return 1;
}

UINT SetSystemPaletteUse(HDC dc, UINT syspal)
{
    int old = dc->data.syspal;
    dc->data.syspal = syspal;
    return old;
}

COLORREF SetTextColor(HDC dc, COLORREF col)
{
    COLORREF old = dc->data.text_col;
    dc->data.text_col = col;

    dc->data.local.text_col = getcolournumber(dc, col);
    return old;
}

BOOL UnrealizeObject(HGDIOBJ obj)
{
    switch (obj->gdi.tag)
    {
    case OBJ_PAL:
	// remap logical palette to system next time it is needed?
	break;

    case OBJ_BRUSH:
	// reset origin on next use
	break;

    default:
	ERROR("UnrealizeObject: unknown object type %d", obj->gdi.tag);
	break;
    }
    return 1;
}

void RestoreScreen(void)
{
    switch_output(screen_dc);
}

/* eof twro.c */
