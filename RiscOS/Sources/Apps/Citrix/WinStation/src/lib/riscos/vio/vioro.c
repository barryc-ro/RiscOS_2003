/* > vioro.c

 *

 */

#include "windows.h"

#include <ctype.h>
#include <string.h>

#include "../../../inc/client.h"

#include "swis.h"

#include "vio.h"

extern unsigned int usMaxRow;
extern unsigned int usMaxCol;
extern int fMONO;

static int current_attr = 0x0F;

/*****************************************************************************
 *
 ****************************************************************************/

void textwindow(int top, int left, int bottom, int right)
{
    char s[5];

    TRACE(( TC_WD, TT_API2, "textwindow: left %d bottom %d right %d top %d", left, bottom, right, top ));

    s[0] = 28;
    s[1] = left;
    s[2] = bottom;
    s[3] = right;
    s[4] = top;

    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void resettextwindow(void)
{
    char s[5];

    TRACE(( TC_WD, TT_API2, "resettextwindow: " ));

    s[0] = 28;
    s[1] = 0;
    s[2] = 0;
    s[3] = usMaxCol - 1;
    s[4] = usMaxRow - 1;

    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void scrollwindow(int nlines, int dir)
{
    char s[10];
    int i;

    TRACE(( TC_WD, TT_API2, "scrollwindow: nlines %d dir %d", nlines, dir ));

    memset(s, 0, sizeof(s));
    s[0] = 23;
    s[1] = 7;
    s[3] = dir;

    for (i = 0; i < nlines; i++)
	_swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void cursor_to(int x, int y)
{
    char s[3];

    TRACE(( TC_WD, TT_API2, "cursor_to: %d,%d", x, y ));

    s[0] = 31;
    s[1] = x;
    s[2] = y;
    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

static BOOL SetColours(int Attr)
{
    BOOL changed = FALSE;
    
    if (Attr != current_attr)
    {
	_swix(OS_SetColour, _INR(0,1),    0, Attr & 0x0f); // set fg
	_swix(OS_SetColour, _INR(0,1), 0x10, Attr >> 4);   // set bg
	current_attr = Attr;
	changed = TRUE;
    }

    return changed;
}

void WriteCells(const char *String, int cb, int Row, int Column)
{
    int i;
    
    TRACE(( TC_WD, TT_API2, "WriteCells: '%.*s' @ %d,%d", cb, String, Column, Row ));

    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);

    for (i = 0; i < cb; i++)
    {
	SetColours(String[i*2+1]);
	_swix(OS_WriteC, _IN(0), String[i*2]);
    }
}

void WriteString(const char *String, int cb, int Row, int Column, int Attr)
{
    TRACE(( TC_WD, TT_API2, "WriteString: '%.*s' @ %d,%d attr %d", cb, String, Column, Row, Attr ));

    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);
	
    if (Attr)
	SetColours(Attr);

    _swix(OS_WriteN, _INR(0,1), String, cb);
}

void WriteCounted(int Char, int Attr, int cb, int Row, int Column)
{
    int i;
    BOOL changed = FALSE;
    
    TRACE(( TC_WD, TT_API2, "WriteCounted: %c(%d)/%d x %d  @ %d,%d", isprint(Char) ? Char : '.', Char, Attr, cb, Column, Row ));

    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);

    if (Attr)
	changed = SetColours(Attr);

    if (Char)
    {
	for (i = 0; i < cb; i++)
	    _swix(OS_WriteC, _IN(0), Char);
    }
    else if (Attr && changed)
    {
	// changing Attr without changing characters.
	// read characters and write back.
	for (i = 0; i < cb; i++)
	{
	    int c;
	    _swix(OS_Byte, _IN(0) | _OUT(1), 135, &c);
	    _swix(OS_WriteC, _IN(0), c);
	}
    }
}

/* eof vioro.c */
