
/******************************Module*Header*******************************\
*
*   TWSTYPE.H
*
*   Thin Wire Windows - General constants and structures.
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb) 15-Apr-1994 (tax day)
*
*   $Log$
*  
*     Rev 1.6   15 Apr 1997 18:16:32   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.5   08 May 1996 14:45:20   jeffm
*  update
*  
*     Rev 1.4   03 Jan 1996 14:02:30   kurtp
*  update
*  
\**************************************************************************/

#ifndef __TWTYPE_H__
#define __TWTYPE_H__

#if 0
typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned long ULONG, * PULONG;
typedef unsigned short USHORT;
typedef short SHORT;
typedef char CHAR;
typedef float               FLOAT;
typedef USHORT              WORD;
typedef ULONG           DWORD;
typedef ULONG FLONG;
#endif

#define TRUE  1
#define FALSE 0

typedef union _FLOAT_LONG
{
   FLOAT   e;
   LONG    l;
} FLOAT_LONG, *PFLOAT_LONG;

typedef struct _RECTI       /* rcl */
{
    int     left;
    int     top;
    int     right;
    int     bottom;
} RECTI, *PRECTI;

/*
 *  TWRealizePalette functions
 */

#define TWREALIZEPALETTE_INIT_FG    0
#define TWREALIZEPALETTE_INIT_BG    1
#define TWREALIZEPALETTE_FG         2
#define TWREALIZEPALETTE_BG         3
#define TWREALIZEPALETTE_FOCUS      4
#define TWREALIZEPALETTE_SET_FG     5
#define TWREALIZEPALETTE_SET_BG     6

#endif //__TWTYPE_H__
