/******************************************************************************
*
*  VIO.H
*
*  Private header file for Video APIs.
*
*  Copyright Citrix Systems Inc. 1994
*
*  Author: Kurt Perry (4/4/94)
*
*  $Log$
*  
*     Rev 1.2   22 Sep 1997 20:36:48   yis
*  update for DOS_V
*  
*     Rev 1.1   15 Apr 1997 18:51:24   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.0   22 Apr 1994 14:49:30   bradp
*  Initial revision.
*
*
*******************************************************************************/


/*=============================================================================
==   Typedefs and structures
=============================================================================*/

// video hook procedure structure
typedef struct _VIOHOOK {
   PLIBPROCEDURE pProcedure;
   struct _VIOHOOK * pNext;
} VIOHOOK, * PVIOHOOK;

/*=============================================================================
==   Functions
=============================================================================*/

extern void textwindow(int top, int left, int bottom, int right);
extern void resettextwindow(void);
extern void scrollwindow(int nlines, int dir);

extern void cursor_to(int x, int y);
extern void cursor_get(int *px, int *py);
extern void cursor_off(void);
extern void cursor_set_height(int yStart, int cEnd);

extern void WriteCells(const char *String, int cb, int Row, int Column);
extern void WriteString(const char *String, int cb, int Row, int Column, int Attr);
extern void WriteCounted(int Char, int Attr, int cb, int Row, int Column);

extern int read_char(void);
extern void write_char(char c);
extern void setup_char_defs(void);
extern void reset_char_defs(void);

extern void sound_beep(int freq, int duration);
extern void sound_off(void);

/*=============================================================================
==   Variables
=============================================================================*/

extern unsigned int usMaxRow;
extern unsigned int usMaxCol;
extern int fMONO;
extern char *VioScreen;
extern int cursor_x, cursor_y;
extern int current_attr;

#define SCROLL_LEFT	1
#define SCROLL_RIGHT	0
#define SCROLL_UP	3
#define SCROLL_DOWN	2
