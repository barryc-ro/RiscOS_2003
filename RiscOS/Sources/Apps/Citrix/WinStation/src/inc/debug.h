/******************************************************************************
*
*  DEBUG.H
*
*     Header file for Logging and Tracing ASSERT and TRACE macros
*
*  Copyright Citrix Systems Inc. 1995
*
*  $Author: Butch Davis (from Brad Pedersen's logapi.h)
*
*   $Log$
*  
*     Rev 1.5   15 Apr 1997 18:45:02   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.4   06 May 1996 18:35:06   jeffm
*  update
*  
*     Rev 1.3   10 Jun 1995 14:17:16   marcb
*  update
*  
*     Rev 1.2   24 May 1995 09:22:48   marcb
*  update
*  
*     Rev 1.1   02 May 1995 13:39:12   butchd
*  update
*
*******************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifndef __LOGAPI_H
# include "logapi.h"
#endif

/*=============================================================================
 ==   ASSERT macro
 ============================================================================*/
#undef  ASSERT
#ifdef DEBUG
#define ASSERT(exp,rc) { \
   if (!(exp)) { \
      LogAssert( #exp, __FILE__, __LINE__, rc ); \
   } \
}
#else
#define ASSERT(exp,rc) { }
#endif


/*=============================================================================
 ==   TRACE macros
 ============================================================================*/

#ifdef DEBUG
#define TRACE(_arg) LogPrintf _arg
#define TRACEBUF(_arg) LogBuffer _arg
#define DEBUGBREAK() DebugBreak()
#else
#define TRACE(_arg) { }
#define TRACEBUF(_arg) { }
#define DEBUGBREAK() { }
#endif

#define DTRACE(_arg) { }
#define DTRACEBUF(_arg) { }
#define DASSERT(exp,rc) { }


#endif //__DEBUG_H__
