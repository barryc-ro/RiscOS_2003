/* Copyright (c) Oracle Corporation 1996.  All Rights Reserved. */

/*
  NAME
    syssvc.h
  DESCRIPTION
    header file for service functions
  PUBLIC FUNCTIONS
    <x>
  NOTES
    <x>
  MODIFIED   (MM/DD/YY)
    jchangav   12/09/96 -  created.
*/

#ifndef SYSSVCH_ORACLE
#define SYSSVCH_ORACLE

INT sysCreateServ(SC_HANDLE schSCManager, LPCSTR lpszExeName, 
		       LPCSTR lpszArg, LPCSTR lpszServName);
INT sysDestroyServ(SC_HANDLE schSCManager, LPCSTR lpszServName);
INT sysStartServ(SC_HANDLE schSCManager, LPCSTR lpszServName, 
	      INT argc, LPTSTR argv[]);
INT sysStopServ(SC_HANDLE schSCManager, LPCSTR lpszServName);

#endif
