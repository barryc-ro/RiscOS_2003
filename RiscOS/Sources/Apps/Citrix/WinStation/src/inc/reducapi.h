///////////////////////////////////////////////////////////////////////////////
//
//  REDUCAPI.H
//
//  Header file for Reducer APIs
//
//  Copyright Citrix Systems Inc. 1998
//
//  Author: Simon Frost Apr 98
//
//  $Log : $
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __REDUCAPI_H__
#define __REDUCAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
==   Global Defines
=============================================================================*/
#define REDUCER_RING_BUF_POW2 11
#define DEFAULT_MAX_NEW_DATA  4096

#define MIN_H2C_POW2_SUPPORTED    10
#define MAX_H2C_POW2_SUPPORTED    15

#define MIN_C2H_POW2_SUPPORTED    10

#ifdef DOS
#define MAX_C2H_POW2_SUPPORTED    12
#else
#ifdef WIN16
#define MAX_C2H_POW2_SUPPORTED    12
#else
#define MAX_C2H_POW2_SUPPORTED    15
#endif  /*WIN16*/
#endif  /* DOS */

#define REDUCER_VERSION_LOW 1
#define REDUCER_VERSION_HIGH 1

/*=============================================================================
==   API entry points
=============================================================================*/

#define REDUCE__INITREDUCDQVAR         0
#define REDUCE__INITEXPANDDQVAR        1
#define REDUCE__REDUCDQMEMSIZE         2
#define REDUCE__EXPANDDQMEMSIZE        3
#define REDUCE__CONVDQTOBUFF           4
#define REDUCE__CONVBUFFTODQ           5 
#define REDUCE__EXTRACTDATAFROMQ       6

#define REDUCE__COUNT                  7 

#ifdef REDUCERLIB
// compiling the library itself

ULONG WFCAPI ReducerDataQueueMemSize( ULONG, ULONG );
ULONG WFCAPI ExpanderDataQueueMemSize( ULONG );
VOID  WFCAPI InitReducerDataQueueVariables( PDATA_QUEUE, ULONG, ULONG, LPUCHAR, ULONG );
VOID  WFCAPI InitExpanderDataQueueVariables( PDATA_QUEUE, ULONG, LPUCHAR, ULONG );
ULONG WFCAPI ConvertDataQueueIntoBuffer( PDATA_QUEUE, POUTBUF, ULONG, ULONG, ULONG );
VOID  WFCAPI ConvertBufferIntoDataQueue( LPUCHAR, ULONG, PDATA_QUEUE);
ULONG WFCAPI ExtractNewDataFromQueue( PDATA_QUEUE, PUCHAR FAR *, LPULONG );

#else

typedef ULONG (PWFCAPI PREDUCERDATAQUEUEMEMSIZE)( ULONG, ULONG );
typedef ULONG (PWFCAPI PEXPANDERDATAQUEUEMEMSIZE)( ULONG );
typedef VOID  (PWFCAPI PINITREDUCERDATAQUEUEVARIABLES)( PDATA_QUEUE, ULONG, ULONG, LPUCHAR, ULONG );
typedef VOID  (PWFCAPI PINITEXPANDERDATAQUEUEVARIABLES)( PDATA_QUEUE, ULONG, LPUCHAR, ULONG );
typedef ULONG (PWFCAPI PCONVERTDATAQUEUEINTOBUFFER)( PDATA_QUEUE, POUTBUF, ULONG, ULONG, ULONG );
typedef VOID  (PWFCAPI PCONVERTBUFFERINTODATAQUEUE)( LPUCHAR, ULONG, PDATA_QUEUE);
typedef ULONG (PWFCAPI PEXTRACTNEWDATAFROMQUEUE)( PDATA_QUEUE, PUCHAR FAR *, LPULONG );
//typedef VOID  (PWFCAPI PREDUCERMODINITPROC)( PREDUCERFNS, BOOLEAN );

// compiling someone else who uses the library

extern PPLIBPROCEDURE pReduceProcedures;

#define raInitReducerDataQueueVariables \
    ((PINITREDUCERDATAQUEUEVARIABLES)(pReduceProcedures[REDUCE__INITREDUCDQVAR]))
#define raInitExpanderDataQueueVariables \
    ((PINITEXPANDERDATAQUEUEVARIABLES)(pReduceProcedures[REDUCE__INITEXPANDDQVAR]))
#define raReducerDataQueueMemSize \
    ((PREDUCERDATAQUEUEMEMSIZE)(pReduceProcedures[REDUCE__REDUCDQMEMSIZE]))

#define raExpanderDataQueueMemSize \
    ((PEXPANDERDATAQUEUEMEMSIZE)(pReduceProcedures[REDUCE__EXPANDDQMEMSIZE]))

#define raConvertDataQueueIntoBuffer \
    ((PCONVERTDATAQUEUEINTOBUFFER)(pReduceProcedures[REDUCE__CONVDQTOBUFF]))

#define raConvertBufferIntoDataQueue \
    ((PCONVERTBUFFERINTODATAQUEUE)(pReduceProcedures[REDUCE__CONVBUFFTODQ]))

#define raExtractNewDataFromQueue \
    ((PEXTRACTNEWDATAFROMQUEUE)(pReduceProcedures[REDUCE__EXTRACTDATAFROMQ]))

#endif  // REDUCERLIB

#ifdef __cplusplus
}
#endif

#endif //__REDUCAPI_H__
