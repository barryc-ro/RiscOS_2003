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
*  vio.h,v
*  Revision 1.1  1998/01/12 11:37:35  smiddle
*  Newly added.#
*
*  Version 0.01. Not tagged
*
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
extern void WriteCells(const char *String, int cb, int Row, int Column);
extern void WriteString(const char *String, int cb, int Row, int Column, int Attr);
extern void WriteCounted(int Char, int Attr, int cb, int Row, int Column);
