/* > vioro.c

 *

 */

#include "windows.h"

#include <ctype.h>
#include <string.h>

#include "../../../inc/client.h"

#include "swis.h"

#include "vio.h"

int current_attr = 0x0F;
int cursor_x = 0, cursor_y = 0;

static int text_top, text_left, text_bottom, text_right;

/*****************************************************************************
 *
 ****************************************************************************/

static void scroll_vertically(int up, int cbLines)
{
    int cb2, lw, lo, i, j, k;

    /*
     * Calculate the bytes to xfer.
     */
    cb2 = (text_right - text_left + 1) * 2;
    lw  = usMaxCol * 2;
    lo  = text_left * 2;

    /*
     *  Scroll down
     */
    j = (text_bottom - text_top + 1) - cbLines;
    k = (up ? text_top : text_bottom) * lw + lo;

    if (!up)
	lw = lw;

    for ( i=0; i<j; i++ ) {
	memcpy( VioScreen+k + (i*lw), VioScreen+k  + ((i+cbLines)*lw), cb2 );
    }
}

static void scroll_horizontally(int right, int cbCol)
{
    int cb, cb3, lw, lo, i, j, k, l;
    const char *in;
    char *out;

    /*
     * Calculate the bytes to xfer.
     */
    cb  = text_right - text_left + 1;
    cb3 = cbCol << 1;
    lw  = usMaxCol << 1;
    lo  = text_left << 1;

    /*
     *  Scroll h
     */
    if ( cbCol < cb ) {

	j = text_bottom - text_top + 1;
	k = text_top * lw + lo;
	l = (cb - cbCol) << 1;

	in = out = VioScreen + k;
	if (right)
	    out += cb3;
	else
	    in += cb3;

	for ( i=0; i<j; i++ ) {
            memcpy( out + (i*lw), in + (i*lw), l );
	}
    }
}

void textwindow(int top, int left, int bottom, int right)
{
    char s[5];

    TRACE(( TC_WD, TT_API2, "textwindow: left %d bottom %d right %d top %d", left, bottom, right, top ));

    s[0] = 28;
    s[1] = text_left = left;
    s[2] = text_bottom = bottom;
    s[3] = text_right = right;
    s[4] = text_top = top;

    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void resettextwindow(void)
{
    char s[5];

    TRACE(( TC_WD, TT_API2, "resettextwindow: " ));

    s[0] = 28;
    s[1] = text_left = 0;
    s[2] = text_bottom = 0;
    s[3] = text_right = usMaxCol - 1;
    s[4] = text_top = usMaxRow - 1;

    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void scrollwindow(int n, int dir)
{
    char s[10];
    int i;

    TRACE(( TC_WD, TT_API2, "scrollwindow: nlines %d dir %d", n, dir ));

    memset(s, 0, sizeof(s));
    s[0] = 23;
    s[1] = 7;
    s[3] = dir;

    for (i = 0; i < n; i++)
    {
	_swix(OS_WriteN, _INR(0,1), s, sizeof(s));
    }

    switch (dir)
    {
    case SCROLL_UP:
	scroll_vertically(TRUE, n);
	break;
    case SCROLL_DOWN:
	scroll_vertically(FALSE, n);
	break;
    case SCROLL_LEFT:
	scroll_horizontally(FALSE, n);
	break;
    case SCROLL_RIGHT:
	scroll_horizontally(TRUE, n);
	break;
    }
}

void cursor_to(int x, int y)
{
    char s[3];

    TRACE(( TC_WD, TT_API2, "cursor_to: %d,%d (was %d,%d)", x, y , cursor_x, cursor_y));

    if (cursor_x != x || cursor_y != y)
    {
	s[0] = 31;
	s[1] = x;
	s[2] = y;
	_swix(OS_WriteN, _INR(0,1), s, sizeof(s));

	cursor_x = x;
	cursor_y = y;
    }
}

void cursor_get(int *px, int *py)
{
    *py = cursor_y;
    *px = cursor_x;
}

void cursor_off(void)
{
    char s[10];
    memset(s, 0, sizeof(s));

    s[0] = 23;
    s[2] = 10;
    s[3] = 1<<5;
    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void cursor_set_height(int yStart, int cEnd)
{
    char s[10];
    memset(s, 0, sizeof(s));

    s[0] = 23;

    s[2] = 10;
    s[3] = yStart;
    LOGERR(_swix(OS_WriteN, _INR(0,1), s, sizeof(s)));

    s[2] = 11;
    s[3] = cEnd;
    LOGERR(_swix(OS_WriteN, _INR(0,1), s, sizeof(s)));
}

static BOOL SetColours(int Attr)
{
    BOOL changed = FALSE;
    
    if (Attr != current_attr)
    {
	char s[4];
	TRACE(( TC_WD, TT_API4, "SetColours: fg %d bg %d", Attr & 0x0f, Attr >> 4));
	
//	_swix(OS_SetColour, _INR(0,1),    0, Attr & 0x0f); // set fg
//	_swix(OS_SetColour, _INR(0,1), 0x10, Attr >> 4);   // set bg
	s[0] = 17;
	s[1] = Attr & 0x0f;
	s[2] = 17;
	s[3] = 128 + (Attr >> 4);
	_swix(OS_WriteN, _INR(0,1), s, sizeof(s));
	
	current_attr = Attr;
	changed = TRUE;
    }

    return changed;
}

void WriteCells(const char *String, int cb, int Row, int Column)
{
    int oldColumn, oldRow;
    int i;
    
    TRACE(( TC_WD, TT_API2, "WriteCells: '%.*s' @ %d,%d", cb, String, Column, Row ));

    cursor_get(&oldColumn, &oldRow);

    if (Row != -1 && Column != -1)
	cursor_to(Column, Row);

    for (i = 0; i < cb; i++)
    {
	SetColours(String[i*2+1]);
	write_char(String[i*2]);
    }

    cursor_to(oldColumn, oldRow);
}

void WriteString(const char *String, int cb, int Row, int Column, int Attr)
{
    int oldColumn, oldRow;
    int i;
    
    TRACE(( TC_WD, TT_API2, "WriteString: '%.*s' @ %d,%d attr %02x", cb, String, Column, Row, Attr ));

    cursor_get(&oldColumn, &oldRow);

    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);
	
    if (Attr)
	SetColours(Attr);

    for (i = 0; i < cb; i++)
	write_char(String[i]);

    cursor_to(oldColumn, oldRow);
}

void WriteCounted(int Char, int Attr, int cb, int Row, int Column)
{
    int i;
    BOOL changed = FALSE;
    int oldColumn, oldRow;
    
    TRACE(( TC_WD, TT_API2, "WriteCounted: %c(%d)/%02x x %d  @ %d,%d", isprint(Char) ? Char : '.', Char, Attr, cb, Column, Row ));

    cursor_get(&oldColumn, &oldRow);
    
    if (Row != -1 && Column != -1)
	cursor_to(Column, Row);

    if (Attr)
	changed = SetColours(Attr);

    if (Char)
    {
	for (i = 0; i < cb; i++)
	    write_char(Char);
    }
    else if (Attr && changed)
    {
	// changing Attr without changing characters.
	// read characters and write back.
	for (i = 0; i < cb; i++)
	{
	    write_char(read_char());
	}
    }

    cursor_to(oldColumn, oldRow);
}

/*
 * freq is in Hz ?
 * duration can be -1 for minimal, otherwise as passed to Delay() function.
 */

void sound_beep(int freq, int duration)
{
    LOGERR(_swix(OS_WriteI + 7, 0));
}

void sound_off(void)
{
}

/* eof vioro.c */
