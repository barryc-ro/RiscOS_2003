/* -*-c-*- */

/* webfonts.c */

/* Font code for the ANTWeb WWW browser */

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

extern df_write_data(int fh, int pos, void *ptr, int size);

webfont webfonts[WEBFONT_COUNT];

static char *webfont_font_name(int n, char *buffer)
{
    if (n & WEBFONT_FLAG_SPECIAL)
    {
	switch (n & WEBFONT_SPECIAL_TYPE_MASK)
	{
	case WEBFONT_SPECIAL_TYPE_SYMBOL:
	    strcpy(buffer, "Selwyn");
	    break;
#ifdef STBWEB
	case WEBFONT_SPECIAL_TYPE_MENU:
	    strcpy(buffer, "Lucida.Sans");
	    break;
#endif
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
    int size;

    e = NULL;

    if (item->handle != -1)
    {
	e = font_lose(item->handle);
    }

    if (webfont_font_name(n, buffer) == NULL)
	return NULL;

    size = (n & WEBFONT_SIZE_MASK) >> WEBFONT_SIZE_SHIFT;
    size = config_font_sizes[size];
#ifdef STBWEB
    if ((n & (WEBFONT_SPECIAL_TYPE_MASK|WEBFONT_FLAG_SPECIAL)) != (WEBFONT_SPECIAL_TYPE_MENU|WEBFONT_FLAG_SPECIAL))
#endif
	size = size*config_display_scale/100;

#if 0
    fprintf(stderr, "Font index %d, size %d, name %s\n", n, size, buffer);
#endif

    if (e == NULL)
    {
	e = font_find(buffer, size * 16, size * 16, 0, 0, &(item->handle));
    }

    if (e == NULL)
    {
	e = font_readinfo(item->handle, &fi);
    }

    if (e == NULL)
    {
	int inc_up, inc_dn;
	inc_up = config_display_leading/2;
	inc_dn = config_display_leading - config_display_leading/2;

	if (config_display_leading_percent)
	{
	    item->max_up = fi.maxy * (100 + inc_up) / 100;
	    item->max_down = (-fi.miny) * (100 + inc_dn) / 100;
	}
	else
	{
	    item->max_up = fi.maxy + inc_up;
	    item->max_down = (-fi.miny) + inc_dn;
	}

	e = font_charbbox(item->handle, 'i', font_OSCOORDS, &fi);
    }

    if (e == NULL)
    {
	item->space_width = fi.maxx - fi.minx;
    }

    return e;
}

os_error *webfonts_init(void)
{
    int i;
    os_error *e;

    for(e= NULL, i = 0; (e == NULL) && (i < WEBFONT_COUNT); i++)
    {
	e = webfonts_init_font(i);
	if (e == NULL)
	{
	    /* */
	}
	else if (i & WEBFONT_FLAG_SPECIAL)
	{			/* if no symbol font then we will work around */
	    e = NULL;
	}
    }

    return e;
}

os_error *webfonts_tidyup(void)
{
    int i;
    os_error *e, *e2;

    e = e2 = NULL;

    for(i = WEBFONT_BASE; i < WEBFONT_COUNT; i++)
    {
	if (webfonts[i].handle != -1)
	    e = font_lose(webfonts[i].handle);

	if (e)
	    e2 = e;
    }

    return e2;
}

int webfont_font_width(int f, const char *s)
{
    webfont *wf = &webfonts[f];
    font_string fs;
    int result;

    fs.s = (char *)s;
    fs.x = fs.y = (1 << 30);
    fs.split = -1;
    fs.term = strlen(s);

    if (font_setfont(wf->handle) == NULL && font_strwidth(&fs) == NULL)
        result = (fs.x / MILIPOINTS_PER_OSUNIT);
    else
        result = fs.term * wf->space_width;

    return result;
}

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

os_error *webfont_declare_printer_fonts(void)
{
    os_regset r;
    os_error *ep = NULL;
    webfont *item;
    int i;

    r.r[1] = 0;
    r.r[2] = 0;

    /* declare standard fonts */
    for(i = 0 ; ep == NULL && i < WEBFONT_FLAG_COUNT; i++)
    {
	item = webfonts + (i << WEBFONT_FLAG_SHIFT);

	if (item->handle)
	{
	    r.r[0] = item->handle;
	    ep = os_swix(PDriver_DeclareFont, &r);
	}
    }

    /* declare special fonts */
    for (i = 0; i < WEBFONT_SPECIAL_COUNT && ep == NULL; i++)
    {
	item = webfonts + WEBFONT_FLAG_SPECIAL + (i << WEBFONT_SPECIAL_TYPE_SHIFT);

	if (item->handle > 0)
	{
	    r.r[0] = item->handle;
	    ep = os_swix(PDriver_DeclareFont, &r);
	}
    }

    /* terminate */
    if (ep == NULL)
    {
	r.r[0] = 0;
	ep = os_swix(PDriver_DeclareFont, &r);
    }

    return ep;
}

os_error *webfont_drawfile_fontlist(int fh, int *writeptr)
{
    int i;
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

    for (i=0; i < WEBFONT_FLAG_COUNT; i++)
    {
	int len;

	buffer[0] = (char) i+1;
	webfont_font_name(i << WEBFONT_FLAG_SHIFT, buffer+1);
	len = strlen(buffer+1) + 2;
	df_write_data(fh, *writeptr, buffer, len);
	*writeptr += len;
	size -= len;
    }
    
    for (i=0; i < WEBFONT_SPECIAL_COUNT; i++)
    {
	int len;

	buffer[0] = (char) i+1;
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

    return 0;
}

/* eof webfonts.c */
