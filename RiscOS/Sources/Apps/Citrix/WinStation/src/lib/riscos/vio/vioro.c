/* > vioro.c

 *

 */

#include "windows.h"

#include <string.h>

#include "../../../inc/client.h"

#include "swis.h"

#include "vio.h"

extern unsigned int usMaxRow;
extern unsigned int usMaxCol;
extern int fMONO;

/*****************************************************************************
 *
 ****************************************************************************/

void textwindow(int top, int left, int bottom, int right)
{
    char s[5];

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
    s[0] = 31;
    s[1] = x;
    s[2] = y;
    _swix(OS_WriteN, _INR(0,1), s, sizeof(s));
}

void WriteCells(const char *String, int cb, int Row, int Column)
{
    int i;
    
    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);

    for (i = 0; i < cb; i++)
    {
	_swix(OS_WriteC, _IN(0), String[i*2]);

	// ignore attributes for now
    }
}

void WriteString(const char *String, int cb, int Row, int Column, int Attr)
{
    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);
	
    _swix(OS_WriteN, _INR(0,1), String, cb);

    // ignore attributes for now
    if (Attr)
    {
    }
}

void WriteCounted(int Char, int Attr, int cb, int Row, int Column)
{
    int i;
    
    if (Row !=-1 && Column != -1)
	cursor_to(Column, Row);

    if (Char)
    {
	for (i = 0; i < cb; i++)
	    _swix(OS_WriteC, _IN(0), Char);
    }

    // ignore attributes for now
    if (Attr)
    {
    }
}

/* eof vioro.c */
