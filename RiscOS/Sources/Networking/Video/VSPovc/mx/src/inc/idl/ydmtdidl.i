/* mx/src/inc/ydmtdidl.idl */


/*
ORACLE, Copyright (c) 1982, 1983, 1986, 1990 ORACLE Corporation
ORACLE Utilities, Copyright (c) 1981, 1982, 1983, 1986, 1990, 1991 ORACLE Corp

Restricted Rights
This program is an unpublished work under the Copyright Act of the
United States and is subject to the terms and conditions stated in
your  license  agreement  with  ORACORP  including  retrictions on
use, duplication, and disclosure.

Certain uncopyrighted ideas and concepts are also contained herein.
These are trade secrets of ORACORP and cannot be  used  except  in
accordance with the written permission of ORACLE Corporation.
*/





#ifndef YDMTDIDL_ORACLE
#define YDMTDIDL_ORACLE

#ifndef YOSTD_ORACLE
#include "yostd.idl"
#endif

#ifndef YOIDL_ORACLE
#include "yoidl.idl"
#endif

struct ydmtdStats
{
  sb4	    ndests_ydmtdStats;
  sb4	    nyorts_ydmtdStats;
  sb4	    npolls_ydmtdStats;
  sb4	    nsends_ydmtdStats;
};

struct ydmtdMetrics
{
  yort::procInfo    pinfo_ydmtdMetrics;
  yort::dispInfoList dlist_ydmtdMetrics;
  yort::implAllList metrics_ydmtdMetrics;
};

typedef sequence <ydmtdMetrics> ydmtdMetricsList;

interface ydmtd {

  readonly attribute ydmtd self;
  readonly attribute ydmtdStats stats;
  readonly attribute ydmtdMetricsList metrics;

  exception notFound {};

    attribute sb4		ttlMs;

    attribute sb4		pollIdleMs;

    attribute sb4		destCheckMs;
  
    attribute sb4		timeoutMs;

    attribute sb4		maxPoll;

    void getYortMetrics( in yort::proc yort, out ydmtdMetrics metrics )
    raises ( notFound );

    void getProcMetrics( in string host, in string pid, in string affinity,
		      out ydmtdMetrics metrics )
    raises ( notFound );

  oneway void shutdown();
};

#endif				