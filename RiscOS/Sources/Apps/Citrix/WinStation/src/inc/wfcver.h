
/***************************************************************************
*
*  WFCVER.H
*
*  This module defines the client build version client product information
*   for file versioning purposes
*
*  The standard Windows CLIENT versioning sets up the .rc file in the
*  following way:
*
*  #ifdef WIN16
*  #include <ver.h>
*  #else
*  #include <winver.h>
*  #endif
*
*  #include "wfcver.h"      // this file
*
*  [file-specific #defines, such as...]
*
*  #define VER_FILETYPE                VFT_APP
*  #define VER_FILESUBTYPE             VFT2_UNKNOWN
*  #define VER_FILEDESCRIPTION_STR     "WinFrame Client for Windows"
*  #define VER_INTERNALNAME_STR        "WFWin"
*  #define VER_ORIGINALFILENAME_STR    "WFWIN.EXE"
*
*  [file version should also be defined here if defaults not desired]
*
*  #include "commonp.ver"   // also in this directory
*
*  Copyright Citrix Systems Inc. 1995-1996
*
*  Author: Mike Discavage
*
*  $Log$
*  
*     Rev 1.206   25 Aug 1997 11:53:48   wf20r
*  Update build number
*  
*     Rev 1.205   23 Aug 1997 17:16:42   wf20r
*  Update build number
*  
*     Rev 1.204   22 Aug 1997 20:10:18   wf20r
*  Update build number
*  
*     Rev 1.203   23 Aug 1997 00:01:44   tariqm
*  RSA Copyright
*  
*     Rev 1.196   13 Aug 1997 13:57:56   wf20r
*  Update build number
*  
*     Rev 1.195   11 Aug 1997 12:20:08   wf20r
*  Update build number
*  
*     Rev 1.194   29 Aug 1997 16:30:44   kalyanv
*  updated the client numbers
*  
*     Rev 1.193   07 Aug 1997 17:53:38   wf20r
*  Update build number
*  
*     Rev 1.192   07 Aug 1997 17:53:06   wf20r
*  Update build number
*  
*     Rev 1.191   05 Aug 1997 17:34:04   wf20r
*  Update build number
*  
*     Rev 1.190   04 Aug 1997 18:38:00   wf20r
*  Update build number
*  
*     Rev 1.189   31 Jul 1997 18:55:04   wf20r
*  Update build number
*  
*     Rev 1.188   31 Jul 1997 13:46:54   wf20r
*  Update build number
*  
*     Rev 1.187   30 Jul 1997 16:52:02   wf20r
*  Update build number
*  
*     Rev 1.186   24 Jul 1997 14:16:44   wf20r
*  Update build number
*  
*     Rev 1.185   14 Jul 1997 14:26:52   wf20r
*  Update build number
*  
*     Rev 1.184   14 Jul 1997 10:42:02   wf20r
*  Update build number
*  
*     Rev 1.183   02 Jul 1997 09:01:54   wf20r
*  Update build number
*  
*     Rev 1.182   28 Jun 1997 08:43:08   wf20r
*  Update build number
*  
*     Rev 1.181   26 Jun 1997 08:53:16   wf20r
*  Update build number
*  
*     Rev 1.180   19 Jun 1997 17:58:08   WF20R
*  Update build number
*  
*     Rev 1.179   10 Jun 1997 11:02:14   WF20R
*  Update build number
*  
*     Rev 1.178   06 May 1997 20:37:58   WF20R
*  Update build number
*  
*     Rev 1.177   05 May 1997 18:31:02   WF20R
*  Update build number
*  
*     Rev 1.176   05 May 1997 18:30:24   WF20R
*  Update build number
*  
*     Rev 1.175   29 Apr 1997 22:31:26   WF20R
*  Update build number
*  
*     Rev 1.174   26 Apr 1997 20:49:46   WF20R
*  Update build number
*  
*     Rev 1.173   23 Apr 1997 20:37:32   WF20R
*  Update build number
*  
*     Rev 1.172   23 Apr 1997 20:37:10   WF20R
*  no update
*  
*     Rev 1.171   23 Apr 1997 20:26:24   WF20R
*  no change
*  
*     Rev 1.170   19 Apr 1997 17:55:24   WF20R
*  Update build number
*  
*     Rev 1.169   16 Apr 1997 18:56:24   WF20R
*  Update build number
*  
*     Rev 1.168   15 Apr 1997 18:46:20   TOMA
*  autoput for remove source 4/12/97
*  
*     Rev 1.174   11 Apr 1997 17:53:04   WF20R
*  Update build number
*  
*     Rev 1.173   08 Apr 1997 18:35:28   WF20R
*  Update build number
*  
*     Rev 1.172   03 Apr 1997 19:01:52   WF20R
*  Update build number
*  
*     Rev 1.171   01 Apr 1997 18:20:26   WF20R
*  Update build number
*  
*     Rev 1.170   26 Mar 1997 18:15:54   WF20R
*  Update build number
*  
*     Rev 1.169   21 Mar 1997 17:21:22   WF20R
*  Update build number
*  
*     Rev 1.168   18 Mar 1997 18:35:40   WF20R
*  Update build number
*  
*     Rev 1.167   14 Mar 1997 18:07:46   WF20R
*  Update build number
*  
*     Rev 1.166   11 Mar 1997 18:22:00   WF20R
*  Update build number
*  
*     Rev 1.165   06 Mar 1997 18:36:48   WF20R
*  Update build number
*  
*     Rev 1.164   03 Mar 1997 18:15:36   WF20R
*  Update build number
*  
*     Rev 1.163   26 Feb 1997 18:37:46   WF20R
*  Update build number
*  
*     Rev 1.162   15 Feb 1997 17:48:24   WF20R
*  Update build number
*  
*     Rev 1.161   13 Feb 1997 20:12:52   WF20R
*  Update build number
*  
*     Rev 1.160   10 Feb 1997 18:45:08   WF20R
*  Update build number
*  
*     Rev 1.159   09 Feb 1997 19:07:48   WF20R
*  Update build number
*  
*     Rev 1.158   06 Feb 1997 18:57:10   WF20R
*  Update build number
*  
*     Rev 1.157   04 Feb 1997 19:35:20   WF20R
*  Update build number
*  
*     Rev 1.156   03 Feb 1997 10:23:44   butchd
*  Bumped copyright date to 1997
*  
*     Rev 1.155   30 Jan 1997 14:20:50   WF20R
*  Update build number
*  
*     Rev 1.154   27 Jan 1997 17:55:26   WF20R
*  Update build number
*  
*     Rev 1.153   20 Jan 1997 16:53:24   WF20R
*  Update build number
*  
*     Rev 1.152   14 Jan 1997 18:31:50   WF20R
*  Update build number
*  
*     Rev 1.151   19 Dec 1996 11:27:34   WF20R
*  Update build number
*  
*     Rev 1.150   18 Dec 1996 21:43:38   WF20R
*  Update build number
*  
*     Rev 1.149   18 Dec 1996 18:13:44   WF20R
*  Update build number
*  
*     Rev 1.148   17 Dec 1996 11:48:34   WF20R
*  Update build number
*  
*     Rev 1.147   12 Dec 1996 11:43:48   WF20R
*  Update build number
*  
*     Rev 1.146   09 Dec 1996 00:17:02   WF20R
*  Update build number
*  
*     Rev 1.145   04 Dec 1996 19:02:44   WF20R
*  Update build number
*  
*     Rev 1.144   02 Dec 1996 16:40:24   WF20R
*  Update build number
*  
*     Rev 1.143   02 Dec 1996 16:39:44   WF20R
*  Update build number
*  
*     Rev 1.142   22 Nov 1996 17:32:30   WF20R
*  Update build number
*  
*     Rev 1.141   20 Nov 1996 17:27:04   WF20R
*  Update build number
*  
*     Rev 1.140   13 Nov 1996 18:20:16   richa
*  Changed VER_PRODUCTBUILF_STR to VER_CTX...
*  
*     Rev 1.139   13 Nov 1996 10:25:26   WF20R
*  Update build number
*  
*     Rev 1.138   13 Nov 1996 10:19:08   richa
*  No change.
*  
*     Rev 1.137   13 Nov 1996 10:11:26   richa
*  1st 2.0 Build.
*  
*  
****************************************************************************/

#ifndef __WFCVER_H__
#define __WFCVER_H__

/*
 * Include ctxver.h to get host build number defines
 */
#include "citrix/ctxver.h"

#ifdef VER_HOSTBUILD
#undef VER_HOSTBUILD
#endif
#define VER_HOSTBUILD       VER_CTXPRODUCTBUILD

#ifdef VER_HOSTBUILD_STR
#undef VER_HOSTBUILD_STR
#endif
#define VER_HOSTBUILD_STR   VER_CTXPRODUCTBUILD_STR

#ifdef VER_CLIENTBUILD
#undef VER_CLIENTBUILD
#endif
#define VER_CLIENTBUILD            323

#ifdef VER_CLIENTBUILD_STR
#undef VER_CLIENTBUILD_STR
#endif
#define VER_CLIENTBUILD_STR        "323"

/*--------------------------------------------------------------*/
/* the following section defines values used in the version     */
/* data structure for all Windows CLIENT files, and which do    */
/* not change.                                                  */
/*--------------------------------------------------------------*/

/* default is nodebug */
#if DEBUG
#define VER_DEBUG                       VS_FF_DEBUG
#else
#define VER_DEBUG                       0
#endif

/* default is prerelease */
#if BETA
#define VER_PRERELEASE                  VS_FF_PRERELEASE
#else
#define VER_PRERELEASE                  0
#endif

#define VER_FILEFLAGSMASK               VS_FFI_FILEFLAGSMASK
#define VER_FILEFLAGS                   (VER_PRERELEASE|VER_DEBUG)

#define VER_LEGALCOPYRIGHT_STR          "Copyright \251 1990-1997 Citrix Systems, Inc."
#define ENC_VER_DOS_LEGALCOPYRIGHT_STR  "Copyright (c) 1990-1997 Citrix Systems, Inc.\nCopyright (c) 1986-1997 RSA Data Security Inc."
#define VER_DOS_LEGALCOPYRIGHT_STR      "Copyright (c) 1990-1997 Citrix Systems, Inc."

#define VER_COMPANYNAME_STR             "Citrix Systems, Inc."
#define VER_PRODUCTNAME_STR             "Citrix WinFrame Client"
#define VER_PRODUCTVERSION_STR          "3.00"
#define VER_CLIENTVERSION               3,00,VER_CLIENTBUILD
#define VER_CLIENTVERSION_STR           VER_PRODUCTVERSION_STR "." VER_CLIENTBUILD_STR 

#endif //__WFCVER_H__
