
/******************************Module*Header*******************************\
*
*   TWTEXT.H
*
*   Thin Wire Windows - Text output constants and structures.
*
*   Copyright (c) Citrix Systems Inc. 1994-1996
*
*   Author: Marc Bloomfield (marcb) 15-Apr-1994 (tax day)
*
*   $Log$
*  
*     Rev 1.3   15 Apr 1997 18:16:30   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.2   08 May 1996 14:48:12   jeffm
*  update
*  
*     Rev 1.1   03 Jan 1996 13:35:20   kurtp
*  update
*  
\**************************************************************************/

#ifndef __TWTEXT_H__
#define __TWTEXT_H__

#define DR_SET   0x00
VOID far vSetStrips(ULONG, ULONG);
VOID far vClearStrips(ULONG);


#define TWTO_NOCLIP    0x0000
#define TWTO_RCLCLIP   0x0001
#define TWTO_CMPLXCLIP 0x0002

                                                        
// Decompress pt1Rcl & pt2Rcl
#define GETTEXTPTDATA( pt1Rcl, rcl, pBuf, dxLast, dyLast ) \
switch ( pt1Rcl.fType ) {                              \
   case TW_PT1RCL_SAMEYX1:                             \
      dxLast = (USHORT)(*((UCHAR *)pBuf));             \
      break;                                           \
   case TW_PT1RCL_SAMEYX2:                             \
      dxLast = *((USHORT *)pBuf);                      \
      break;                                           \
   case TW_PT1RCL_11X5Y:                               \
      dxLast = (USHORT)((PTWPT11X5Y)pBuf)->dx;         \
      dyLast = (USHORT)((PTWPT11X5Y)pBuf)->dy;         \
      break;                                           \
   case TW_PT1RCL_10X6Y:                               \
      dxLast = (USHORT)((PTWPT10X6Y)pBuf)->dx;         \
      dyLast = (USHORT)((PTWPT10X6Y)pBuf)->dy;         \
      break;                                           \
   case TW_PT1RCL_9X7Y:                                \
      dxLast = (USHORT)((PTWPT9X7Y)pBuf)->dx;          \
      dyLast = (USHORT)((PTWPT9X7Y)pBuf)->dy;          \
      break;                                           \
   case TW_PT1RCL_8X8Y:                                \
      dxLast = (USHORT)((PTWPT8X8Y)pBuf)->dx;          \
      dyLast = (USHORT)((PTWPT8X8Y)pBuf)->dy;          \
      break;                                           \
   case TW_PT1RCL_11X10Y:                              \
      dxLast = (USHORT)((PTWPT11X10Y)pBuf)->dx;        \
      dyLast = (USHORT)((PTWPT11X10Y)pBuf)->dy;        \
      break;                                           \
}                                                      \
rcl.left   = (USHORT)pt1Rcl.xLeft;                     \
rcl.top    = (USHORT)pt1Rcl.yTop;                      \
rcl.right  = rcl.left + dxLast;                        \
rcl.bottom = rcl.top  + dyLast;                  

/*****************************************************************************\
 * TWTOHCACHECACHE hCacheCache;
\*****************************************************************************/
typedef struct _TWTOHCACHECACHE {
   USHORT hCache          /*: 12*/; // value of the client cache handle
   USHORT ChunkType       /*: 2*/; // _2k, _512B, _128B, _32B
   USHORT res             /*: 2*/;
} TWTOHCACHECACHE, *PTWTOHCACHECACHE;

#endif //__TWTEXT_H__
