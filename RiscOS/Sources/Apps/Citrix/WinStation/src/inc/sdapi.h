
/***************************************************************************
*
*   SDAPI.H  - Script Driver
*
*   This module contains external script driver API defines and structures
*
*   Copyright Citrix Systems Inc. 1990-1996
*
*   Author: Kurt Perry (kurtp)
*
*   $Log$
*
****************************************************************************/

/*
 *  Index into SD procedure array
 */
//      DLL$LOAD                 0
//      DLL$UNLOAD               1
//      DLL$OPEN                 2
//      DLL$CLOSE                3
//      DLL$INFO                 4
//      DLL$POLL                 5
#define SD__ERRORLOOKUP          6
#define SD__COUNT              	 7   /* number of external sd procedures */

/*
 *  SdOpen structure
 */
typedef struct _SDOPEN {

    LPBYTE         pScriptFile;
    PDLLLINK       pWdLink;
    PPLIBPROCEDURE pClibProcedures;
    PPLIBPROCEDURE pLogProcedures;
    PPLIBPROCEDURE pModuleProcedures;

} SDOPEN, * PSDOPEN;


#define SCRIPT_STATUS_SUCCESS   0
#define SCRIPT_STATUS_COMPLETE  1
