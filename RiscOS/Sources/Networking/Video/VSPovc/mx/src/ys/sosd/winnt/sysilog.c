/* Copyright (c) Oracle Corporation 1996.  All Rights Reserved. */

/*
  NAME
    sysilog.c
  DESCRIPTION
    main program for sysimsg file.

    This file contains the "messages" that translate the events we send
    in ssyslSysLog to the NT event log service.  It needs to be installed
    where the SSYSI_EVENT_LOGKEY\\EventMessageFile says it should be,
    usually the value of SSYSI_DEF_MSGFILE.

    It needs to be linked with the output of the sysmsg.mc message file,
    which becomes a .rc file and a .bin file:

	mc sysmsg.mc # -> sysmsg.rc MSG0001.bin
	rc sysmsg.rc # -> sysmsg.h SYSMSG.RES, etc.

  PUBLIC FUNCTIONS
    <x>
  PRIVATE FUNCTIONS
    <x>
  NOTES
    <x>
  MODIFIED   (MM/DD/YY)
    dbrower   10/18/96 -  created.
*/

#ifndef SSYSI_ORACLE
#include "ssysi.h"
#endif

/* decl */
boolean sysilogMain(dvoid *osdp, char *nm, sword argc, char **argv);

#define ENTRY_POINT sysilogMain

#include <s0ysmain.c>

/* PRIVATE TYPES AND CONSTANTS */

/*
 * Command-line Arguments
 */
static struct ysargmap sysilogmap[] =
{
  { 0, (char *) 0, 0 }
};

/* PRIVATE FUNCTION DECLARATIONS */

static const char *sysilogTypeDecode( DWORD type );

/* PUBLIC FUNCTIONS */

/* ---------------------------- sysilogMain ---------------------------- */
/*
  NAME
    sysilogMain
  DESCRIPTION
    Main function for the message file.
  PARAMETERS
    osdp    -- osd pointer
    nm	    -- program name string
    argc    -- arg count
    argv    -- argument vector.
  RETURNS
    TRUE on success, FALSE on error exit.
*/

boolean sysilogMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  sword     sts;
  boolean   ok;
  char      vbuf[80];
  HKEY	    hk = (HKEY)0;
  char	    subkey[SYSFP_MAX_PATHLEN];
  char	    buf[SYSFP_MAX_PATHLEN];
  int	    argLen, type;
  DWORD	    err;

  /* initialization */
  ysInit(osdp, nm);
  sts = ysArgParse(argc, argv, sysilogmap);
  if (sts == YSARG_VERSION)
    {
      yslError("Oracle Media Exchange ssyslSysLog Message File");
      vbuf[0] = 0;
      yslError(ysVersion(vbuf, sizeof(vbuf)));
    }
  ok = (sts == YSARG_NORMAL);

  if( !ok )
    return( FALSE );

  yslPrint("%s should be installed where the registry entry\n", nm );
  yslPrint("%s\nEventMessageFile points.\nThis is usually %s\n",
	   SSYSI_EVENT_LOGKEY, SSYSI_EVENT_MSGFILE );

  do
  {
    strcpy(subkey, SSYSI_EVENT_LOGKEY);
    if (RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &hk) != ERROR_SUCCESS)
    {
      yslPrint("Registry Key\n  %s\ndoes not exist.\n", subkey );
      break;
    }
    yslPrint("%s exists\n", subkey);
    if (RegQueryValueEx(hk, "EventMessageFile", NULL, &type, NULL, &argLen)
	!= ERROR_SUCCESS)
    {
      yslPrint("EventMessageFile does not exist\n");
      break;
    }
    yslPrint("EventMessageFile exists, currently set to ");
    argLen = sizeof(buf); 
    err = RegQueryValueEx(hk, "EventMessageFile", NULL, &type, buf, &argLen);
    if ( err == ERROR_SUCCESS )
    {
      yslPrint("%s\n", buf );
    }
    else
    {
      yslPrint("EventMessageFile query returned err %d\n", err );
    }
  } while(FALSE);
  if(hk)
    RegCloseKey(hk);
  return( ok );
}


static const char *sysilogTypeDecode( DWORD type )
{
  const char *rv;
  switch( type )
  {
  case REG_BINARY:	rv = "REG_BINARY"; break;
#ifdef NEVER
  /* this is really REG_DWORD_LITTLE_ENDIAN or REG_DWORD_BIG_ENDIAN*/
  case REG_DWORD	rv = "REG_DWORD "; break;
#endif
  case REG_DWORD_LITTLE_ENDIAN:	rv = "REG_DWORD_LITTLE_ENDIAN "; break;
  case REG_DWORD_BIG_ENDIAN:	rv = "REG_DWORD_BIG_ENDIAN "; break;
  case REG_EXPAND_SZ:	rv = "REG_EXPAND_SZ "; break;
  case REG_LINK:	rv = "REG_LINK "; break;
  case REG_MULTI_SZ:	rv = "REG_MULTI_SZ "; break;
  case REG_NONE:	rv = "REG_NONE "; break;
  case REG_RESOURCE_LIST:  rv = "REG_RESOURCE_LIST "; break;
  case REG_SZ:		rv = "REG_SZ "; break;

  default: rv = "<unknown type>"; break;
  }
  return rv;
}

