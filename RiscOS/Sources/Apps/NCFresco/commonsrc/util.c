/* -*-c-*- */

/*
 * 21/03/96: SJM: added wait_for_release
 * 26/03/96: SJM: added parse_http_header
 * 23/04/96: SJM: merged path_is_directory() and file_type() changes
 * 17/07/96: SJM: added write_text_in_box function
 * 02/08/96: SJM: write_text_in_box now centres text vertically.
 * 13/08/96: SJM: added mimemap lookup
 * 25/02/97: DAF: Moved lookup_key_action to portutil.c
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#include "memwatch.h"

#include "akbd.h"
#include "os.h"
#include "swis.h"
#include "filetypes.h"
#include "config.h"
#include "flexwrap.h"
#include "kernel.h"
#include "font.h"

#include "unwind.h"
#include "util.h"

#include "sgmlparser.h"

#ifndef UTIL_DEBUG
#define UTIL_DEBUG 0
#endif

#define MAPPING_FILE	"<Inet$MimeMappings>"

#define MAX_LINE	256

/* util.c */

static int suffix_or_mime_to_file_type(const char *suffix, const char *mime)
{
    FILE *mf;
    char line[MAX_LINE];
    int ft = -1;

    mf = fopen(MAPPING_FILE, "r");

    if (mf == NULL)
	return ft;

    while (!feof(mf))
    {
	char *p;
	char *mmaj, *mmin, *roft, *rofthex, *item;

	if (fgets(line, MAX_LINE, mf) == NULL)
	    break;

	p = line;
	while (isspace(*p))
	    p++;

	if (*p == '#')
	    continue;

	mmaj = strtok(p, "/");
	mmin = strtok(NULL, " \t");
	roft = strtok(NULL, " \t\n");
	rofthex = strtok(NULL, " \t\n");

	if (mmaj == NULL || mmin == NULL || roft == NULL || rofthex == NULL || roft[0] == '*')
	    continue;

	if (suffix)
	{
	    do
	    {
		item = strtok(NULL, " \t\n");
		if (item)
		{
		    if (item[0] == '.' || item[0] == '/')
			item++;

		    if (strcasecomp(suffix, item) == 0)
			break;	/* From the DO loop */
		}
	    } while (item);
	}
	else
	{
	    char *mimemaj, *mimemin;
	    char *mime1 = strdup(mime);

	    mimemaj = strtok(mime1, "/");
	    mimemin = strtok(NULL, " ");

	    if (strcasecomp(mmaj, mimemaj) == 0 &&
		(mmin[0] == '*' || strcasecomp(mmin, mimemin) == 0))
	    {
		item = mmaj;	/* value isn't important just non-null for success */
	    }
	    else
		item = NULL;

	    mm_free(mime1);
	}


	if (item)
	{
	    os_regset r;
	    os_error *ep;

	    r.r[0] = 31;
	    r.r[1] = (int) (long) roft;

	    ep = os_swix(OS_FSControl, &r);

	    if (ep)
	    {
		r.r[0] = 31;
		r.r[1] = (int) (long) rofthex;

		ep = os_swix(OS_FSControl, &r);
	    }

	    if (ep == NULL)
	    {
		ft = r.r[2];
		break;		/* From while(!feof()) */
	    }
	}
    }

    fclose(mf);

    return ft;
}

int suffix_to_file_type(const char *suffix)
{
    return suffix_or_mime_to_file_type(suffix, NULL);
}

int mime_to_file_type(const char *mime)
{
    return suffix_or_mime_to_file_type(NULL, mime);
}

int set_file_type(const char *fname, int ft)
{
    os_filestr osf;

    osf.action = 18;
    osf.name = (char *)fname;
    osf.loadaddr = ft;

    return (os_file(&osf) == NULL);
}

int file_and_object_type_real(const char *fname, int *obj_type)
{
    os_filestr ofs;
    os_error *ep;
    os_regset r;

    ofs.action = 17;		/* changed from 5 as we don't want any paths being looked at */
    ofs.name = (char *)fname;

    ep = os_file(&ofs);

    if (ep)
	return -1;

    if (obj_type)
	*obj_type = ofs.action;

    if (ofs.action == 2)
	return FILETYPE_DIRECTORY;

    if (ofs.action != 1 && ofs.action != 3)
	return -1;

    r.r[0] = 38;
    r.r[1] = (int) (long) ofs.name;
    r.r[2] = ofs.loadaddr;
    r.r[3] = ofs.execaddr;
    r.r[4] = ofs.start;
    r.r[5] = ofs.end;
    r.r[6] = 1;

    ep = os_swix(OS_FSControl, &r);
    if (ep)
	return -1;

    return r.r[2];
}


int file_type_real(const char *fname)
{
    return file_and_object_type_real(fname, NULL);
}

/* return last modified time in unix style */

int file_last_modified(const char *fname)
{
    os_filestr ofs;

    ofs.action = 17;		/* changed from 5 as we don't want any paths being looked at */
    ofs.name = (char *)fname;

    if (os_file(&ofs) != NULL)
	return 0;

    if (((unsigned)ofs.loadaddr & 0xfff00000) == 0xfff00000)	/* date stamped file */
    {
        unsigned int t1, t2, tc;

        t1 = (unsigned int) ofs.execaddr;
        t2 = (unsigned int) ofs.loadaddr & 0xff;

        tc = 0x6e996a00U;
        if (t1 < tc)
	    t2--;
        t1 -= tc;
        t2 -= 0x33;		/* 00:00:00 Jan. 1 1970 = 0x336e996a00 */

        t1 = (t1 / 100) + (t2 * 42949673U);	/* 0x100000000 / 100 = 42949672.96 */
        t1 -= (t2 / 25);		/* compensate for .04 error */

        return t1;
    }

    return 0;
}

int file_type(const char *fname)
{
    return file_and_object_type(fname, NULL);
}

int file_and_object_type(const char *fname, int *obj_type)
{
    int ft, ft2;
    char *dot, *suffix;

    ft = file_and_object_type_real(fname, obj_type);

    if (ft != FILETYPE_TEXT && ft != FILETYPE_DATA && ft != FILETYPE_DOS && ft != FILETYPE_UNIXEX)
	return ft;

    suffix = strrchr(fname, '/');
    dot = strrchr(fname, '.');

    if (suffix == NULL || dot > suffix || suffix[1] == 0)
	return ft;

    suffix++;

    ft2 = suffix_to_file_type(suffix);

    if (ft2 != -1)
	ft = ft2;

    return ft;
}

int path_is_directory(const char *path)
{
    os_filestr ofs;
    os_error *ep;

    ofs.action = 5;
    ofs.name = (char *)path;

    ep = os_file(&ofs);

    /* A directory if no error, not a file and not not found */
    return ((ep == NULL) && (ofs.action != 1) && (ofs.action != 0));
}

char *reduce_file_name(char *fname, char *temp, char *pathname)
{
    char temp2[256];
    int len;

    if (_swix(OS_FSControl, _INR(0,6), 37, (int) (long) pathname, (int) (long) temp, 0, 0, 256) != NULL)
	return NULL;

    if (_swix(OS_FSControl, _INR(0,6), 37, (int) (long) fname, (int) (long) temp2, 0, 0, 256) != NULL)
	return NULL;

    len = strlen(temp);

    if (strncmp(temp, temp2, len) == 0)
    {
	strcpy(temp, pathname);
	strcat(temp, temp2+len+1);

	return temp;
    }

    return NULL;
}

/**********************************************************************/
void *rma_alloc(size_t n)
{
    os_regset r;

    r.r[0] = 6;
    r.r[3] = n;

    if (os_swix(OS_Module, &r) != NULL)
	return NULL;

    return (void*) (long) r.r[2];
}

void rma_free(void *p)
{
    os_regset r;

    r.r[0] = 7;
    r.r[2] = (int) (long) p;

    os_swix(OS_Module, &r);
}

/**********************************************************************/

#if MEMLIB

static int locks = 0;
static _kernel_ExtendProc *old_budge = NULL;

void flexmem_noshift( void )
{
    if ( !locks )
    {
        if ( !MemFlex_Dynamic() )
            old_budge = _kernel_register_slotextend( MemFlex_dont_budge );
    }
    locks++;
}

void flexmem_shift( void )
{
    locks--;
    if ( !locks )
    {
        if ( !MemFlex_Dynamic() )
            _kernel_register_slotextend( old_budge );
    }
}

#elif !defined(STBWEB)

static int _no_budge_count = 0;
static _kernel_ExtendProc *old_budge = 0;

/* SJM */
static int dont_budge(int n, void **a)
{
   return 0;
}

/* Increment the count of users that don't want the flex blocks to shift */
void flexmem_noshift(void)
{
	if (_no_budge_count == 0)
	old_budge = _kernel_register_slotextend(dont_budge);

    _no_budge_count++;

#if UTIL_DEBUG
    fprintf(stderr, "flexmem_noshift called, count now %d ", _no_budge_count);
    fprintf(stderr, "by '%s' from '%s'\n", caller(1), ""/* caller(2) */);
#endif
}

/* Decrement the count of users that don't want the flex blocks to shift */
void flexmem_shift(void)
{
    if (_no_budge_count)
	_no_budge_count--;

    if (_no_budge_count == 0)
	_kernel_register_slotextend(old_budge);

#if UTIL_DEBUG
    fprintf(stderr, "flexmem_shift called, count now %d ", _no_budge_count);
    fprintf(stderr, "by '%s' from '%s'\n", caller(1), ""/* caller(2) */);
#endif
}
#endif /* 0 */



/**********************************************************************/

/*
 * wait for all mouse buttons to be released or 'max' centiseconds to elapse
 */

#define DRAG_DIST	8

int wait_for_release(int max)
{
    os_regset r;
    int end;
    int x, y;

    os_swix(OS_ReadMonotonicTime, &r);
    end = r.r[0] + max;

    os_swix(OS_Mouse, &r);
    x = r.r[1];
    y = r.r[2];

    for (;;)
    {
        os_swix(OS_Mouse, &r);

	/* check for button up */
        if (r.r[2] == 0)
            break;

	/* check for mouse moved */
	if (abs(r.r[1] - x) > DRAG_DIST || abs(r.r[2] - y) > DRAG_DIST)
	    return FALSE;

	/* check for timeout */
        os_swix(OS_ReadMonotonicTime, &r);
        if (r.r[0] >= end)
            break;
    }

    return TRUE;
}

/**********************************************************************/

/*
 * This message formatting code ripped off from Navigator
 */

#define LINEMAX 20

typedef struct
{
    const char *text;                   /* First char of this segment */
    int length;                         /* Length of this segment */
    int width;                          /* Width of this segment in millipoints */
} LineRec, *LinePtr;

static struct
{
    int xs, ys, xl, yl, splitchar;
} coordblock = {0, 0, 0, 0, 32};

static os_error *split_message(const char *str, int width, int handle, int *numlines, LineRec *lines)
{
    int segwidth;
    const char *seg = str, *splitpoint;
    os_error *e = NULL;

    while (!e && *numlines < LINEMAX && *seg)
    {
        os_regset r;
        r.r[0] = handle;
        r.r[1] = (int)seg;
        r.r[2] = (1<<5) | (1<<8) | (1<<9);
        r.r[3] = width;
        r.r[4] = 0;
        r.r[5] = (int) &coordblock;
	coordblock.splitchar = ' ';			/* split on space */
        e = os_swix(Font_ScanString, &r);
        if (!e)
        {
	    /* If the next word is too long, ie cannot be split in the space available,
	     * then segwidth will be 0 and splitpoint == seg. In this case we want to just
	     * skip to the next space.
	     */
	    if (r.r[3] == 0)
	    {
		r.r[3] = width;
		r.r[4] = 0;
		coordblock.splitchar = -1;		/* split on any character */
		e = os_swix(Font_ScanString, &r);
	    }

            splitpoint = (char *)r.r[1];
            segwidth = r.r[3];

#if UTIL_DEBUG
	    fprintf(stderr, "ScanString: '%s' w %d = split leaves '%s' w %d\n", seg, width, splitpoint, segwidth);
#endif

            /* splitpoint should now point to a space, or the end of the string */
            if (lines)
            {
                lines[*numlines].text = seg;
                lines[*numlines].length = splitpoint - seg;
                lines[*numlines].width = segwidth;
            }

	    seg = splitpoint;

	    /* skip to the next space */
 	    while (*seg && *seg != ' ')
		seg++;

	    /* skip any spaces in the text */
	    while (*seg == ' ')
		seg++;

            (*numlines)++;
        }
    }

    return e;
}

os_error *write_text_in_box_height(const char *str, int width, int handle, int *height)
{
    int numlines;
    font_info info;
    os_error *e;
    int ww;

    numlines = 0;
    e = font_converttopoints(width, width, &ww, &ww);
    if (!e) e = split_message(str, ww, handle, &numlines, NULL);
    if (!e) e = font_readinfo(handle, &info);

    if (!e) *height = (info.maxy - info.miny) * numlines + info.maxy;

    return e;
}

/*
 * x0, x1, y0, y1 are the coordinates of the area into which we want to format
 * the text.  They are in OS Units.
 */

os_error *write_text_in_box(int handle, const char *str, void *bbox)
{
    os_error *e;
    int i, y, space_x, spacing;
    int x0, y0, x1, y1;
    font_info info;
    int numlines;
    LineRec lines[LINEMAX];
    wimp_box *box = (wimp_box*)bbox;
    int maxy, miny;

    e = font_converttopoints(box->x0, box->y0, &x0, &y0);
    if (!e) e = font_converttopoints(box->x1, box->y1, &x1, &y1);

    numlines = 0;
    if (!e) e = split_message (str, x1 - x0, handle, &numlines, lines);

    if (!e) e = font_readinfo(handle, &info);
    if (!e) e = font_converttopoints(info.maxx - info.minx, info.maxy, &space_x, &maxy);
    if (!e) e = font_converttopoints(info.maxx - info.minx, info.miny, &space_x, &miny);

    spacing = maxy - miny;

    /* Determine the Y coordinate of the top line */
#if 1
    y = y0 + (y1 - y0) / 2 + numlines * spacing / 2 - maxy;
    if (y > y1 - maxy)
        y = y1 - maxy;
#else
    y = y1 - maxy;
#endif

    /* Actually display the segments */
    for (i = 0; !e && i < numlines && y > y0 - miny; i++)
    {
        os_regset r;
        r.r[0] = handle;
        r.r[1] = (int)lines[i].text;
        r.r[2] = (1<<7) | (1<<8) | (1<<9);
        r.r[3] = x0 + (x1 - x0) / 2 - lines[i].width / 2;
        r.r[4] = y;
        r.r[7] = lines[i].length;
        e = os_swix(Font_Paint, &r);

        y -= spacing;
    }

    return e;
}

/**********************************************************************/

int kbd_pollalt(void)
{
    int x, y;

    x = 2 ^ 0xff;
    y = 0xff;

    os_byte(129, &x, &y);

    return x == 0xff;
}

/**********************************************************************/

os_error *file_lock(const char *file_name, int lock)
{
    os_filestr fs;
    os_error *e;

    fs.action = 17;
    fs.name = (char *)file_name;

    e = os_file(&fs);
    if (!e)
    {
	int new_attr = lock ? (fs.end | 8) : (fs.end &~ 8);
	if (new_attr != fs.end)
	{
	    fs.end = new_attr;
	    e = os_file(&fs);
	}
    }
    return e;
}

/**********************************************************************/

/* pdh: Try to reallocate the string in lower memory
 */
char *optimise_string( char *string )
{
    char *ptr;

    if ( !string )
        return NULL;

    ptr = (char*)mm_malloc( strlen(string)+1 );

    if ( ptr && ptr < string )
    {
        strcpy( ptr, string );
        mm_free( string );
        return ptr;
    }

    mm_free( ptr );
    return string;
}

#define GSTRANS_BUFSIZE	1024

extern char *strdup_gstrans(const char *input)
{
    os_regset r;
    os_error *e;
    char *output;

    output = mm_malloc(GSTRANS_BUFSIZE);

    r.r[0] = (int)input;
    r.r[1] = (int)output;
    r.r[2] = GSTRANS_BUFSIZE;
    e = os_swix(OS_GSTrans, &r);

    if (e)
    {
	mm_free(output);
	return strdup(input);
    }

    /* if overflow then just ensure terminated - should do better really */
    if (r.r[2] == GSTRANS_BUFSIZE)
	output[GSTRANS_BUFSIZE-1] = 0;

    return mm_realloc(output, r.r[2]);
}

extern BOOL gstrans_not_null(const char *input)
{
    char *file_name;
    BOOL is_null;

    file_name = strdup_gstrans(input);

    is_null = file_name == NULL || file_name[0] == 0;

    mm_free(file_name);

    return !is_null;
}

#if defined(STBWEB) && !defined(CBPROJECT)
extern os_error *ensure_modem_line(void)
{
    os_error *e;
    e = os_cli("*EnsureLine_EnsureLine");
    if (e && e->errnum == 214)	/* file not found */
	e = NULL;

    return e;
}
#endif

#ifdef STBWEB
int cmos_op(int bit_start, int n_bits, int new_val, BOOL write)
{
    int mask, byte, offset, r;

    mask = (1<<n_bits) - 1;
    byte = bit_start/8;
    offset = bit_start%8;

    /* read current value */
    r = _kernel_osbyte(0xA1, byte, 0);
    if (r == _kernel_ERROR)
	return -1;

    r = (r >> 8) & 0xff;

    if (!write)
	return (r & mask) >> offset;

    r &= ~(mask << offset);
    r |= new_val << offset;
    _kernel_osbyte(0xA2, byte, r);

    return 0;
}

#define NVRAM_Read	0x4EE00
#define NVRAM_Write	0x4EE01

int nvram_read(const char *tag, int *val)
{
    char buf[4];
    int err = -1;
    _swix(NVRAM_Read, _INR(0,2) | _OUT(0), tag, buf, 0, &err);
#if DEBUG
    fprintf(stderr, "nvram_read: '%s' = %d err %d\n", tag, *(int *)buf, err);
#endif
    if (err >= 0)
	*val = *(int *)buf;
    return err >= 0;
}

int nvram_write(const char *tag, int new_val)
{
    char buf[4];
    int err = -1;
    *(int *)buf = new_val;
    _swix(NVRAM_Write, _INR(0,2) | _OUT(0), tag, buf, 0, &err);
#if DEBUG
    fprintf(stderr, "nvram_write: '%s' = %d err %d\n", tag, new_val, err);
#endif
    return err == 0;
}

/* trigger an event */

#define SoundFX_Play 0x4ef41

void sound_event(sound_event_t event_num)
{
    if (config_sound_fx && event_num != snd_NONE)
	_swix(SoundFX_Play, _INR(0,1), 0, event_num);
}

#endif

/*
 * Set the mouse pointer to a given screen coordinate
 */

#define osword_Mouse        0x15
#define Mouse_SetPosition   3   /* signed 16bit - X, Y - mouse position */

void pointer_set_position(int x, int y)
{
    char block[5];
    block[0] = Mouse_SetPosition;
    block[1] = x & 0xff;
    block[2] = (x >> 8) & 0xff;
    block[3] = y & 0xff;
    block[4] = (y >> 8) & 0xff;
    _kernel_osword(osword_Mouse, (int *)block);
}

static char tmpnam_buf[L_tmpnam] = "";
static char count = 0;

char *rs_tmpnam(char *s)
{
    FILE *f;
    BOOL present = FALSE;
    do
    {
	int sig = (time(NULL) << 8) | count++;

	if (!s)
	    s = tmpnam_buf;

	sprintf(s, "<Wimp$ScrapDir>.%08x", sig);
	f = fopen(s, "w");
	if (f)
	{
	    fclose(f);
	    present = TRUE;
	}
    }
    while (!present);
    return s;
}

/*****************************************************************************/

/* Change a window's flags: only in Nested Wimp (3.90 and later)
 * No-op (but returns error) if used on earlier wimps.
 */

os_error *wimp_set_wind_flags( wimp_w w, wimp_wflags bic, wimp_wflags eor )
{
    wimp_wstate ws;
    os_error *e;
    _kernel_swi_regs r;

    e = wimp_get_wind_state( w, &ws );
    if ( e )
        return e;
    ws.flags = ( ws.flags & ~bic ) ^ eor;

    r.r[1] = (int)&ws;
    r.r[2] = *((int*)"TASK");
    r.r[3] = -1;
    r.r[4] = 1;

    return (os_error*)_kernel_swi( 0x600C5, &r, &r );   /* XWimp_OpenWindow */
}

/*****************************************************************************/

/* eof util.c */
