/* Copyright (c) 1996 by Oracle Corporation.  All Rights Reserved.
 *
 * NAME
 * mtz.c - Media Net Client Tools (mnls,mnping)
 *
 * DESCRIPTION:
 * This file serves as testbed to mn port to server and client platforms
 * 
 * REVISIONS
 * syen      06/01/96  Creation
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef MN_ORACLE
#include <mn.h>
#endif
#ifndef MZN_ORACLE
#include <mzn.h>
#endif
#ifndef SMN_ORACLE
#include <smn.h>
#endif
#ifndef YS_ORACLE
#include <ys.h>
#endif
#ifndef YSC_ORACLE
#include <ysc.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef SMNI_ORACLE
#include <smni.h>
#endif
#ifndef MTL_ORACLE
#include <mtl.h>
#endif

#ifndef MTZ_ORACLE
#include <smtz.h>
#endif

#include "smtzr.h"

/* global variable */

mtzctx  mtz;
syslctx osdctx;
char    addr[256];

/* these three globals should be moved into mtzctx */
boolean bRegMnAddr = FALSE;	     /* mnaddr initially set by registry val */
boolean bFirstMnConn = FALSE;		      /* first time of mn connection */
boolean bMnConn = FALSE;			   /* currently mn connected */

HWND      hWinMain;
HINSTANCE hInst;

/* function prototype */
externref void   doEcho(char *addr, boolean repeat, ub4 delay);

STATICF void mtzSyslPrint(CONST char *buf);
STATICF void mtzSyslError(CONST char *buf);

STATICF dvoid mtzMnPing(dvoid *addr);
STATICF boolean mtzMnLs(dvoid *osdCtx, const char *nm, sword argc, char **argv);
STATICF dvoid *mnlsAlloc(mnxStream *stream, size_t len);
STATICF sb4    mnlsPush(mnxStream *stream, dvoid *ptr);

/* 
 * WinMain() - open main dialog box for mntool 
 */
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				    LPSTR lpszCmdLine, int nCmdShow)
{
  MSG msg;

  hInst = hInstance;
  DialogBox (hInst, MAKEINTRESOURCE(IDD_MTZ_MAIN), NULL, mtzDlgMain);
  return msg.wParam;
}

/*
 * mtzDlgMain() - do all the message processing for the main dialog box
 */
BOOL CALLBACK mtzDlgMain (HWND hDlg, UINT msg, UINT wParam, LPARAM lParam)
{
  int     iThrdId = 0;
  char    buf[128];

  boolean bRet = FALSE;
  
  switch (msg)
  {
  case WM_INITDIALOG:
    hWinMain = (HWND) hDlg;
    //CenterWnd (hDlg, NULL, TRUE);
    
    /* standarized initialization */
    osdctx.syslcPrint = mtzSyslPrint;
    osdctx.syslcError = mtzSyslError;
    ssysiInit((dvoid *)&osdctx, "mtz");

    /* Set a resource which uniquely identifies us */
    DISCARD sprintf(buf, "%d", getpid());
    ysResSet("uniqueID", buf);

//    mtlInit( MtlLogTool, "mtz");

    /* get OMN_ADDR from registry and start mn */
    strcpy(mtz.mtzMnAddr, "");
    if( !(bRegMnAddr = smniRegGetVal( "SOFTWARE\\Oracle\\MediaNet", "UDP",
        	                      mtz.mtzMnAddr )) )
    {
      MessageBox(hDlg,
		 "Missing \\Oracle\\MediaNet\\UDP string in registry",
		 "MTZ Info", MB_OK | MB_ICONWARNING);
    }
    
    /* we use lParam to indicate initial mn connection */
    PostMessage(hWinMain, WM_PAINT, 0, 0);
    PostMessage(hWinMain, WM_COMMAND, IDC_MTZ_MNSVR, WM_INITDIALOG); 
    break;

  case WM_COMMAND:
    switch (wParam)
    {
    case IDC_MTZ_MNSVR:
      if (mtz.mtzHdlThrd)
      {
        MessageBox (hDlg, "Need to stop mnping first!\nTo allow mnTerm",
		    "MTZ: Warnning", MB_OK | MB_ICONWARNING);
        break;
      }
      
      bFirstMnConn = (lParam == WM_INITDIALOG);
      
      /* dont reset mnaddr if its already set from registry */
      if (!bFirstMnConn || !bRegMnAddr)
      {
        if(DialogBox(hInst, MAKEINTRESOURCE(IDD_MTZ_MNSVR), hDlg, mtzDlgMnSvr))
        {
	  sprintf(mtz.mtzMnAddr, "%s:%s:%s", mtz.mtzMnProt, mtz.mtzMnHost,
     	          mtz.mtzMnPort);
	}
      }

      DISCARD sprintf(buf, "Connecting to %s", mtz.mtzMnAddr);
      SetDlgItemText (hDlg, IDC_MTZ_MNADDR, buf);

      if (bFirstMnConn)
	bMnConn = ssmniInit(0, (mnLogger) 0, mtz.mtzMnAddr);
      else
      {
        if (bMnConn) ssmniTerm();
        bMnConn = ssmniInit((dvoid *) 0, (mnLogger) 0, mtz.mtzMnAddr);
      }
      if (!bMnConn)
        strcpy(mtz.mtzMnAddr, "");
      SetDlgItemText (hDlg, IDC_MTZ_MNADDR, mtz.mtzMnAddr);
      break;

    case IDC_MTZ_MNLS:
      if (mtz.mtzHdlThrd)
      {
        MessageBox (hDlg, "Must stop mnping first!", "MTZ: Warnning", 
                    MB_OK | MB_ICONWARNING);
        break;
      }
      if (mtz.mtzMnAddr[0])
	mtzMnLs((dvoid *)0, "mnls", 0, 0);
      else
      	MessageBox (hDlg, "Please set OMN_ADDR", "MTZ: Error", MB_OK);
      break;

    case IDC_MTZ_MNPING:
      if (mtz.mtzMnAddr[0]) 
      {
        GetDlgItemText(hWinMain, IDC_MTZ_CMDOPT, addr, sizeof(addr));
        if (!mtz.mtzHdlThrd)
        {
          mtz.mtzHdlThrd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)mtzMnPing, 
                               addr, 0, &iThrdId);
          if (mtz.mtzHdlThrd)
            SetDlgItemText(hDlg, IDC_MTZ_MNPING, "stop");
        }
        else
        {
          /* there is an mnping thread running, brutally murder it */
          if (TerminateThread(mtz.mtzHdlThrd, 0))
          {
            mtz.mtzHdlThrd = NULL;
            SetDlgItemText(hDlg, IDC_MTZ_MNPING, "mnping");
          }
        }
      }
      else
        MessageBox (hDlg, "Please set OMN_ADDR", "MTZ: Error", MB_OK);
      break;

    case IDC_MTZ_PINGRPT:
      /* toggle the checkbox */
      SendMessage((HWND)hDlg, BM_SETCHECK, (WPARAM)
                  !SendMessage((HWND)hDlg, BM_GETCHECK, 0, 0), 0);
      break;

    case WM_DESTROY:
    case IDC_MTZ_EXIT:
      if (mtz.mtzHdlThrd)
      {
        MessageBox (hDlg, "Must stop mnping first!", "MTZ: Warnning",
                    MB_OK | MB_ICONWARNING);
        break;
      }

      if (bMnConn) ssmniTerm();
      ssysiTerm((dvoid *) 0);
      EndDialog(hDlg, msg);
      bRet = TRUE;
      break;

    default:
      break;
    } /* end case WM_COMMAND */
    break;

  default:
    break;
  } /* end switch (msg) */

  return (bRet);
} /* end mtzDlgMain */

/*
 * mtzDlgMnSvr() - dlg to enter OMN_ADDR's protocol, hostname, port number
 */
BOOL CALLBACK mtzDlgMnSvr(HWND hDlg, UINT msg, UINT wParam, LPARAM lParam)
{
  BOOL bRet = FALSE;
  lParam = lParam;  /* avoid warning */

  switch (msg)
  {
  case WM_INITDIALOG:
    strcpy(mtz.mtzMnProt, "UDP");
    strcpy(mtz.mtzMnPort, "5000");
    SetDlgItemText (hDlg, IDC_MTZ_MNHOST, mtz.mtzMnHost);
    SetDlgItemText (hDlg, IDC_MTZ_MNPORT, mtz.mtzMnPort);
    SetDlgItemText (hDlg, IDC_MTZ_MNPROT, mtz.mtzMnProt);
    //CenterWnd (hDlg, hWinMain, TRUE);
    break;

  case WM_COMMAND:
    switch (wParam) 
    {
    case IDOK:
      GetDlgItemText (hDlg, IDC_MTZ_MNHOST, mtz.mtzMnHost, MAX_NAME_LEN);
      GetDlgItemText (hDlg, IDC_MTZ_MNPORT, mtz.mtzMnPort, MAX_NAME_LEN);
      GetDlgItemText (hDlg, IDC_MTZ_MNPROT, mtz.mtzMnProt, MAX_NAME_LEN);
      EndDialog (hDlg, TRUE);
      bRet = TRUE;
      break;
    
    case IDCANCEL:
      EndDialog (hDlg, FALSE);
      bRet = FALSE;
      break;
    
    default:
      break;
    }
  }        
  return(bRet);
} /* end mtzDlgMnSvr */

STATICF void mtzSyslPrint(CONST char *obuf)
{
  char dbuf[DBUF_SIZE];   /* display buffer */
  char tmpbuf[DBUF_SIZE], tobuf[DBUF_SIZE];
  char *p;
  int  oblen;            /* obuf strlen */

  GetDlgItemText(hWinMain, IDC_MTZ_STDOUT, dbuf, DBUF_SIZE-(strlen(obuf)+1));
  
  sprintf(tobuf, "%s", obuf);
  oblen = strlen(tobuf);
  if (tobuf[oblen-1] == '\n')
  {
    /* convert \n\0 to \r\n\0 for obuf */
    p = tobuf + oblen - 1;                  /* p points to \n */
    *p++ = '\r'; *p++ = '\n'; *p = '\0';
  }
  sprintf(tmpbuf, "%s%s", tobuf, dbuf);
  SetDlgItemText( hWinMain, IDC_MTZ_STDOUT, tmpbuf);
}

STATICF void mtzSyslError(CONST char *obuf)
{
  if (strcmp(obuf, "\n"))
    MessageBox( (HWND)hWinMain, obuf, "mtzSyslError", MB_OK);
}

STATICF dvoid mtzMnPing(dvoid *addr)
{
  boolean bRepeat = FALSE;  
  char *mnping_addr = (char *) addr;
	   
  if (mnping_addr[0] == '\0')
    mnping_addr = NULL;

  bRepeat = IsDlgButtonChecked(hWinMain, IDC_MTZ_PINGRPT);
  doEcho( mnping_addr, bRepeat, 1000);
  
  mtz.mtzHdlThrd = NULL;
  SetDlgItemText(hWinMain, IDC_MTZ_MNPING, "mnping");

  ExitThread( 0);
}

/*
 * mtzMnLs() is cut-pasted from mnls.c. The main different is that it calls
 * ssmniTerm() instead mnTerm to terminate mn, in order to decrement the
 * ssmniRefCnt.
 */
boolean mtzMnLs(dvoid *osdCtx, const char *nm, sword argc, char **argv)
{
  char     *regexp = (char *)0;
//  sword     sts;
  sb4       cnt;
  mnxStream stream;
//  char	    *arg;
//  char       vbuf[80];
#if 0
  sts = ysArgParse( argc, argv, mnlsArgs );
  if (sts == YSARG_VERSION)
    yslError(ysVersion(vbuf, sizeof(vbuf)));
  if (sts != YSARG_NORMAL)
    return FALSE;

  if( arg = ysResGetLast( "mnls.regexp" ) )
    regexp = arg;
#endif
  if( !smniInit( osdCtx, (mnLogger)0 ) )
    return( FALSE );

  yslPrint("%-32.32s %-20s %-24s\n", "Name", "Address", "Program ID");

  stream.alloc = mnlsAlloc;
  stream.push = mnlsPush;
  stream.close = (void (*)(mnxStream *, sb4)) 0;

  cnt = mznQuery(regexp, MZN_ALL_NAMES, &stream);

  if (cnt < 0)
    yslError(mnError(cnt));
  else
    yslPrint("(%ld entr%s)\n", cnt, (cnt == 1 ? "y" : "ies"));

  ssmniTerm();

  return( TRUE );
}

dvoid *mnlsAlloc(mnxStream *stream, size_t len)
{
  return ysmGlbAlloc(len, "mnlsAlloc");
}

sb4 mnlsPush(mnxStream *stream, dvoid *ptr)
{
  mznent *entry;
  char    buf[256], out[MNAMAXLEN], *off;

  entry = (mznent *) ptr;

  mnAtos(&entry->addr, out, MNAMAXLEN);

  if (entry->id)
    sprintf(buf, "%08lx%08lx  %d.%d",
	    smnNtoh4(*((ub4 *) &entry->id->id[0])),
	    smnNtoh4(*((ub4 *) &entry->id->id[4])),
	    MNGETVERS(entry->id->vers), MNGETRELS(entry->id->vers));
  else if (off = strchr(entry->name, ':'))
    {
      out[0] = '\0';
      sprintf(buf, "-> ");
      strcat(buf, off + 1);
      *off = '\0';
    }
  else
    sprintf(buf, "(named port)");

  yslPrint("%-32.32s %-20s %-24s\n", entry->name, out, buf);

  mnxFree(mznxEntry, entry);
  ysmGlbFree(ptr);

  return 0;
}
