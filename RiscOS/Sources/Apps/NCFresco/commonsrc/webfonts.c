/* -*-c-*- */

/* webfonts.c */

/* Font code for the ANTWeb WWW browser */

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "font.h"
#include "swis.h"
#include "msgs.h"
#include "os.h"

#include "webfonts.h"
#include "antweb.h"
#include "consts.h"
#include "util.h"
#include "config.h"

#ifndef Font_WideFormat
#define Font_WideFormat	0x400A9
#endif

#if UNICODE
#include "Unicode/utf8.h"
#endif

extern df_write_data(int fh, int pos, void *ptr, int size);

webfont webfonts[WEBFONT_COUNT];

static char *webfont_font_name(int n, char *buffer)
{
    if (n & WEBFONT_FLAG_SPECIAL)
    {
	switch (n & WEBFONT_SPECIAL_TYPE_MASK)
	{
	case WEBFONT_SPECIAL_TYPE_SYMBOL:
	    strcpy(buffer, config_font_names[12]);
	    break;
#ifdef STBWEB
	case WEBFONT_SPECIAL_TYPE_MENU:
	    strcpy(buffer, config_font_names[13]);
	    break;
#endif
	case WEBFONT_SPECIAL_TYPE_JAPANESE:
	    strcpy(buffer, config_font_names[14]);
	    break;
	case WEBFONT_SPECIAL_TYPE_CHINESE:
	    strcpy(buffer, config_font_names[15]);
	    break;
	case WEBFONT_SPECIAL_TYPE_KOREAN:
	    strcpy(buffer, config_font_names[16]);
	    break;
	case WEBFONT_SPECIAL_TYPE_RUSSIAN:
	    strcpy(buffer, config_font_names[17]);
	    break;
	case WEBFONT_SPECIAL_TYPE_GREEK:
	    strcpy(buffer, config_font_names[18]);
	    break;
	case WEBFONT_SPECIAL_TYPE_HEBREW:
	    strcpy(buffer, config_font_names[19]);
	    break;

	default:
	    return NULL;
	}
    }
    else
    {
	strcpy(buffer, config_font_names[(n & WEBFONT_FLAG_MASK) >> WEBFONT_FLAG_SHIFT]);
    }

    return buffer;
}

os_error *webfonts_init_font(int n)
{
    font_info fi;
    os_error *e;
    char buffer[256];
    webfont *item = webfonts + n;
    int size, xsize, ysize, extra_index;

    e = NULL;

    if (item->handle != -1)
    {
	e = font_lose(item->handle);
    }

    if (webfont_font_name(n, buffer) == NULL)
	return NULL;

    size = (n & WEBFONT_SIZE_MASK) >> WEBFONT_SIZE_SHIFT;
    /* size 7 is not used - the value in config_font_sizes is not filled in */
    if (size == 7)
	return NULL;

    size = config_font_sizes[size] * 16;

    /* don't scale the MENU font with the general scaling */
    if ((n & (WEBFONT_SPECIAL_TYPE_MASK|WEBFONT_FLAG_SPECIAL)) != (WEBFONT_SPECIAL_TYPE_MENU|WEBFONT_FLAG_SPECIAL))
	size = size * config_display_scale / 100;

    /* check for per font information */
    extra_index = -1;
    if (n & WEBFONT_FLAG_SPECIAL)
    {
	int nn = n & WEBFONT_SPECIAL_TYPE_MASK;
	if (nn >= WEBFONT_SPECIAL_TYPE_JAPANESE && nn <= WEBFONT_SPECIAL_TYPE_HEBREW)
	    extra_index = 3 + ((nn - WEBFONT_SPECIAL_TYPE_JAPANESE) >> WEBFONT_SPECIAL_TYPE_SHIFT);
    }
    else
    {
	int nn = (n & WEBFONT_FLAG_MASK) >> WEBFONT_FLAG_SHIFT;
	extra_index = nn >> 2;	/* get rid of bold and italic */
    }

    if (extra_index != -1)
    {
	xsize = size * config_font_scales[extra_index] / 100;
	ysize = xsize * config_font_aspects[extra_index] / 100;
    }
    else
	xsize = ysize = size;

    DBG(("Font index %d, extra %d scale %d%% aspect %d%% size %d, xsize %d, ysize %d, name %s\n",
	 n, extra_index,
	 extra_index != -1 ? config_font_scales[extra_index] : -1,
	 extra_index != -1 ? config_font_aspects[extra_index] : -1,
	 size, xsize, ysize, buffer));

    if (e == NULL)
    {
	e = font_find(buffer, xsize, ysize, 0, 0, &(item->handle));
    }

    if (e == NULL)
    {
        /* We want to make max_up and max_down depend *only* on the point
         * size, *not* on the bounding box of the font, otherwise we get
         * different answers for medium and bold fonts!
         */
	int fsizeos = (ysize * 180/16 + 71)/72;         /* points to OS units */

	if (config_display_leading_percent)
	    fsizeos += fsizeos * config_display_leading / 100;
	else
	    fsizeos += config_display_leading;
	
	item->max_down = fsizeos/4;
	item->max_up = fsizeos - item->max_down;

	e = font_charbbox(item->handle, 'i', font_OSCOORDS, &fi);
    }

    if (e == NULL)
    {
	item->space_width = fi.maxx - fi.minx;
    }

    return e;
}

static os_error *one_of_each_please( void )
{
    int i;
    os_error *e = NULL;

    /* Ask for one of each type of font so "not found" errors happen now */
    for ( i=0; i < WEBFONT_FLAG_COUNT && !e; i++ )
    {
        int whichfont = WEBFONT_BASE + ( i << WEBFONT_FLAG_SHIFT );
        e = webfont_find_font( whichfont );
        if ( !e )
        {
            webfont_lose_font( whichfont );
        }
    }
    return e;
}

os_error *webfonts_initialise( void )
{
    int i;
    os_error *e;

    for ( i=0; i < WEBFONT_COUNT; i++ )
    {
        webfonts[i].handle = -1;
    }

    e = one_of_each_please();

    /* used everywhere for space calcs */
    if ( !e )
        e = webfont_find_font( WEBFONT_TTY );

#if 0
    /* nasty hack to ensure that japanese font is always open */
    if (!e && config_encoding_internal != 0)
    {
	for (i = 1; !e && i <= WEBFONT_SIZES; i++)
	    e = webfont_find_font( WEBFONT_JAPANESE + WEBFONT_SIZE(i) );
	e = NULL;
    }
#endif
    return e;
}

os_error *webfonts_reinitialise( void )
{
    int i;
    os_error *e=NULL, *e2;

    for ( i=0; i < WEBFONT_COUNT; i++ )
    {
        if ( webfonts[i].handle != -1 )
        {
            e2 = webfonts_init_font(i);
            if ( !e )
                e = e2;
        }
    }

    if ( !e )
        e = one_of_each_please();

    return e;
}

os_error *webfont_find_font( int which )
{
    if ( webfonts[which].usage_count++ == 0 )
        return webfonts_init_font( which );
    return NULL;
}

os_error *webfont_lose_font( int which )
{
    os_error *e = NULL;
    if ( --webfonts[which].usage_count == 0 )
    {
        e = font_lose( webfonts[which].handle );
        webfonts[which].handle = -1;
    }
    return e;
}

os_error *webfonts_tidyup(void)
{
    int i;
    os_error *e, *e2;

    e = e2 = NULL;

    /* was "i=WEBFONT_BASE"... el bogo, leaks font handles! */

    for(i = 0; i < WEBFONT_COUNT; i++)
    {
	if (webfonts[i].handle != -1)
	    e = font_lose(webfonts[i].handle);

	if (e)
	    e2 = e;
    }

    return e2;
}

typedef struct
{
    int fh;
    int flags;
    int width;
    int coords[5];
    int n;

    int w, h;			/* width/height in millipoints to the end of string/split point */
    int used;			/* count of chars passed in passes that didn't find a split character */
    int split_len;		/* n characters to the split point */
} scanstring_info;

static os_error *scanstring_fn(const char *text, BOOL last, void *handle)
{
    scanstring_info *si = handle;
    os_error *e;
    int w, h;

    /* DBG(("scanstring_fn: +\n")); */
    
    e = (os_error *)_swix(Font_ScanString, _INR(0,4) | _IN(7) | _OUTR(3,4),
			  si->fh, text, si->flags,
			  INT_MAX, INT_MAX,
			  si->n,
			  &w, &h);
    /* DBG(("scanstring_fn: -\n")); */
    if (!e)
    {
	si->w += w;
	si->h += h;
    }

    return e;
}

int webfont_font_width_n(int f, const char *s, int n)
{
    scanstring_info si;

    memset(&si, 0, sizeof(si));
    if (f != -1)
    {
	si.fh = webfonts[f].handle;
	si.flags = (1<<8);
    }
    
#if UNICODE
    if (config_encoding_internal == 1 && f != -1 && webfont_latin(f))
    {
	/* we have a utf8 stream which we want to display through an 8 bit font */
	process_utf8_as_latin1(s, n, scanstring_fn, &si);
    }
    else
#endif
    {
	if (n != -1)
	{
	    si.n = n;
	    si.flags |= (1<<7);
	}
	
	scanstring_fn(s, TRUE, &si);
    }
	
    return (si.w + MILIPOINTS_PER_OSUNIT/2) / MILIPOINTS_PER_OSUNIT;	/* return length in OS units */
}

int webfont_font_width(int f, const char *s)
{
    return webfont_font_width_n(f, s, -1);
}

/*
 * Find the nearest split point before width OS units are passed
 * and return its index. If the end of the string is reached
 * then return length of string.

 * Note splitting needs to be done in 'bytes' and so the same routine is used
 * whether it is UTF8 or Latin1 (I hope).
 */

int webfont_split_point_char(int f, const char *s, int width, int c, int *segwidth)
{
    int coords[9];
    const char *split;

    memset(coords, 0, sizeof(coords));
    coords[4] = c;

#if 0
    DBG(("split_point: "));
    DBG(("splitchar %d f=%d s %p='%s' flags=%x w=%d h=%d coords=%p split=%p segwidth=%p",
	 c,
	 f == -1 ? 0 : webfonts[f].handle,
	 s, s,
	 (f == -1 ? 0 : (1<<8)) | (1<<5),	/* pass handle, use coord block */
	 width * MILIPOINTS_PER_OSUNIT, 400 * MILIPOINTS_PER_OSUNIT,
	 coords,
	 &split,
	 segwidth));
#endif
    
    _swix(Font_ScanString, _INR(0,5) | _OUT(1) | (segwidth ? _OUT(3) : 0),
	  f == -1 ? 0 : webfonts[f].handle,
	  s,
	  (f == -1 ? 0 : (1<<8)) | (1<<5),	/* pass handle, use coord block */
	  width * MILIPOINTS_PER_OSUNIT, 0,
	  coords,
	  &split,
	  segwidth);

    /* DBG((" - split = %p\n", split)); */

    return split ? split - s : -1;
}

int webfont_split_point(int f, const char *s, int width)
{
    return webfont_split_point_char(f, s, width, -1, NULL);
}

/*
 * Given an x offset from the start of a string find the byte offset into it.
 */

static os_error *scanstring_offset_fn(const char *text, BOOL last, void *handle)
{
    scanstring_info *si = handle;
    os_error *e;
    const char *ptr;
    int w, h;

    if (si->split_len)
	return NULL;
    
    e = (os_error *)_swix(Font_ScanString, _INR(0,5) | _IN(7) | _OUT(1) | _OUTR(3,4),
			  si->fh, text, si->flags,
			  si->width - si->w, 0,
			  si->coords,
			  si->n, /* only used when called directly */
			  &ptr,
			  &w, &h);
    if (!e)
    {
	int len = strlen(text);
	int offset = ptr - text;

	si->w += w;
	si->h += h;

	/* DBG(("scanstring_offset_fn: text '%s' split_len %d used %d in %p out %p size %dx%d total %dx%d\n", text, si->split_len, si->used, text, ptr, w, h, si->w, si->h)); */

	if (ptr && (offset < len || last))
	    si->split_len = si->used + offset;
	else
	    si->used += si->flags & (1<<7) ? si->n : len;
    }

    return e;
}

int webfont_get_offset(int f, const char *s, int x, const int *coords, int len)
{
    scanstring_info si;

    memset(&si, 0, sizeof(si));

    si.flags = (1<<17);		/* return offset thingy */

    if (coords)
    {
	memcpy(si.coords, coords, sizeof(si.coords));
	si.flags |= (1<<5);
    }

    if (f != -1)
    {
	si.fh = webfonts[f].handle;
	si.flags = (1<<8);
    }

    si.width = x * MILIPOINTS_PER_OSUNIT;
    
#if UNICODE
    if (config_encoding_internal == 1 && f != -1 && webfont_latin(f))
    {
	/* DBG(("webfont_get_offset: s '%s' len %d width %d\n", s, len, si.width)); */

	/* calculate the split offset in latin1 characters*/
	process_utf8_as_latin1(s, len, scanstring_offset_fn, &si);

	/* convert back to utf8 offset */
	si.split_len = (const char *)UTF8_next_n(s, si.split_len) - s;
    }
    else
#endif
    {
	if (len != -1)
	{
	    si.n = len;
	    si.flags |= (1<<7);
	}
	
	scanstring_offset_fn(s, TRUE, &si);
    }
	
    return si.split_len;
}

int webfont_nominal_width(int font_index, int n_chars)
{
    char s[256];
    memset(s, ' ', sizeof(s));
    return webfont_font_width_n( font_index, s, MIN(n_chars, 256) );

}

#if 0
/* Take a width either in OS units or in chars and return the value in the other for a string of TTY chars */

int webfont_tty_width(int w, int in_chars)
{
    int i;
    char buffer[256];
    int result;

    for(i=0; i < 255; i++)
	buffer[i] = ' ';
    buffer[i] = 0;

    if (in_chars)
    {
	font_string fs;

	fs.s = buffer;
	fs.x = fs.y = (1 << 30);
	fs.split = -1;
	fs.term = w;

	if ( w < 256 && font_setfont(webfonts[WEBFONT_TTY].handle) == NULL && font_strwidth(&fs) == NULL)
	    result = (fs.x / MILIPOINTS_PER_OSUNIT);
	else
	    result = w * webfonts[WEBFONT_TTY].space_width;
    }
    else
    {
	os_regset r;

	r.r[0] = (int) (long) webfonts[WEBFONT_TTY].handle;
	r.r[1] = (int) (long) buffer;
	r.r[2] = (1 << 17) | (1 << 8) | (1 << 7);
	r.r[3] = w * MILIPOINTS_PER_OSUNIT;
	r.r[4] = r.r[5] = r.r[6] = 0;
	r.r[7] = 255;

	if (os_swix(Font_ScanString, &r) == NULL)
	{
	    result = ((char*) (long) r.r[1]) - buffer;
	}
	else
	{
	    result = w / webfonts[WEBFONT_TTY].space_width;
	}
    }

    return result;
}
#endif

static os_error *declare_one_of_sizes(const webfont *item)
{
    int size;

    for (size = 0;
	 size < WEBFONT_SIZES;
	 size++, item += (1 << WEBFONT_SIZE_SHIFT))
    {
	if (item->handle > 0)
	    return (os_error *)_swix(PDriver_DeclareFont, _INR(0,2), item->handle, 0, 0);
    }

    return NULL;
}

os_error *webfont_declare_printer_fonts(void)
{
    os_error *ep = NULL;
    int i;

    /* declare standard fonts */
    for (i = 0; ep == NULL && i < WEBFONT_FLAG_COUNT; i++)
    {
	ep = declare_one_of_sizes(webfonts + (i << WEBFONT_FLAG_SHIFT));
    }

    /* declare special fonts */
    if (!ep) for (i = 0; i < WEBFONT_SPECIAL_COUNT && ep == NULL; i++)
    {
	ep = declare_one_of_sizes(webfonts + WEBFONT_FLAG_SPECIAL + (i << WEBFONT_SPECIAL_TYPE_SHIFT));
    }

    /* terminate */
    if (ep == NULL)
    {
	ep = (os_error *)_swix(PDriver_DeclareFont, _INR(0,2), 0, 0, 0);
    }

    return ep;
}

os_error *webfont_drawfile_fontlist(int fh, int *writeptr)
{
    int i, handle;
    int size;
    char buffer[256];
    int word;

    size = 0;
    for (i = 0; i < WEBFONT_FLAG_COUNT; i++)
    {
	webfont_font_name(i << WEBFONT_FLAG_SHIFT, buffer);
	size += strlen(buffer)+2;
    }

    for (i = 0; i < WEBFONT_SPECIAL_COUNT; i++)
    {
	webfont_font_name(WEBFONT_FLAG_SPECIAL + (i << WEBFONT_SPECIAL_TYPE_SHIFT), buffer);
	size += strlen(buffer)+2;
    }

    size = ROUND4(size);

    size += 2 * sizeof(int);	/* Two word header */

    word = 0;			/* Font table, hard-wired. */
    df_write_data(fh, *writeptr, &word, sizeof(word));
    *writeptr += sizeof(word);
    df_write_data(fh, *writeptr, &size, sizeof(size));
    *writeptr += sizeof(size);

    size -= 2 * sizeof(int);	/* Two word header */

    handle = 1;

    for (i=0; i < WEBFONT_FLAG_COUNT; i++, handle++)
    {
	int len;

	buffer[0] = (char) handle;
	webfont_font_name(i << WEBFONT_FLAG_SHIFT, buffer+1);
	len = strlen(buffer+1) + 2;
	df_write_data(fh, *writeptr, buffer, len);
	*writeptr += len;
	size -= len;
    }

    for (i=0; i < WEBFONT_SPECIAL_COUNT; i++, handle++)
    {
	int len;

	buffer[0] = (char) handle;
	webfont_font_name(WEBFONT_FLAG_SPECIAL + (i << WEBFONT_SPECIAL_TYPE_SHIFT), buffer+1);
	len = strlen(buffer+1) + 2;
	df_write_data(fh, *writeptr, buffer, len);
	*writeptr += len;
	size -= len;
    }

    df_write_data(fh, *writeptr, "\0\0\0\0", size);
    *writeptr += size;

    return NULL;
}

int webfont_lookup(const char *font_name)
{
#ifdef STBWEB
    if (strcasecomp(font_name, "ncoffline") == 0)
	return WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_MENU;
#endif

    if (strcasecomp(font_name, "dingbats") == 0)
	return WEBFONT_FLAG_SPECIAL + WEBFONT_SPECIAL_TYPE_SYMBOL;

    if (strcasecomp(font_name, "courier") == 0)
	return WEBFONT_FLAG_FIXED;

    return -1;
}

int webfont_need_wide_font(const char *s, int n_bytes)
{
#if UNICODE
    /* check to see if there are any wide characters in there */ 
    switch (config_encoding_internal)
    {
    case 0:			/* latin1 */
	break;

    case 1:			/* utf8 */
    case 3:			/* sjis */
    case 4:			/* euc */
    {
	int i, c;
	for (i = 0; i < n_bytes; i++)
	{
#if 1
	    int c = *s++;
	    if ((c & 0xC0) == 0xC0 &&		/* if isn't ascii and is initial byte of sequence*/
		c != 0xC2 && c != 0xC3)		/* and isn't Latin1 character */
	    {
		return TRUE;			/* probably need a wide font */
	    }
#else
	    if (s[i] & 0x80)
		return TRUE;
#endif
	}
	break;
    }

    case 2:			/* utf8 plot as unicode */
	return TRUE;
    }
#endif
    
    return FALSE;
}

void webfont_set_wide_format(int index)
{
    static char format[] =
    {
	0,			/* latin1 */
	12,			/* utf8 */
	0,			/* unicode */
	1,			/* sjis */
	3,			/* euc */
	2,			/* jis */
	4,			/* korean */
    };

    /*RENDBG(("webfont_set_wide_format: index %d\n", index));*/
    
    _swix(Font_WideFormat, _INR(0,1), webfonts[index].handle, format[config_encoding_internal]);
}

int webfont_latin(int index)
{
    if ((index & WEBFONT_FLAG_SPECIAL) == 0)
	return TRUE;
    
    switch (index & WEBFONT_SPECIAL_TYPE_MASK)
    {
    case WEBFONT_SPECIAL_TYPE_SYMBOL:
    case WEBFONT_SPECIAL_TYPE_JAPANESE:
    case WEBFONT_SPECIAL_TYPE_CHINESE:
    case WEBFONT_SPECIAL_TYPE_KOREAN:
	    return FALSE;

    case WEBFONT_SPECIAL_TYPE_MENU:
	    return TRUE;
    }

    return FALSE;
}

/* eof webfonts.c */
