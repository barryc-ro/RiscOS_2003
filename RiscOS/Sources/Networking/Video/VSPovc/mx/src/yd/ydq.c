/* mx/src/yd/ydq.c */


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




#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YDQ_ORACLE
#include <ydq.h>
#endif
#ifndef YDIM_ORACLE
#include <ydim.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YDU_ORACLE
#include <ydu.h>
#endif
#ifndef YSR_ORACLE
#include <ysr.h>
#endif
#ifndef YDYOIDL_ORACLE
#include <ydyoidl.h>
#endif
#ifndef YOIDL_ORACLE
#include <yoidl.h>
#endif
#ifndef YDIDL_ORACLE
#include <ydidl.h>
#endif
#ifndef YSFMT_ORACLE
#include <ysfmt.h>
#endif



#define YDQ_BATCHSIZE	((sb4)100)






void ydqListAll(ydim_imr imr)
{
  ydim_infoIterator ii;
  ydim_infoList infoList;
  yoenv ev;

  yoEnvInit(&ev);
  ydqInfoHdr();
  infoList._length = 0;

  ydim_imr_listImpl(imr, &ev, YDQ_BATCHSIZE, (char*)0, (char*)0, (char*)0,
		    &infoList, &ii);
  do
  {
    ydqShowInfoList(&infoList);
    ydim_infoList__free( &infoList, yoFree );

  } while(ii && ydim_infoIterator_next_n(ii, &ev, YDQ_BATCHSIZE, &infoList));

  if(ii)
    ydim_infoIterator_destroy(ii, &ev);

  yoEnvFree(&ev);
}





void ydqShowInfoList(ydim_infoList *list)
{
  ub4 i;

  for(i = 0; i < list->_length ; i++)
    ydqShowInfo( &list->_buffer[i] );
}


void ydqShowInfo( ydim_implInfo *info )
{
  ub4	j;
  char	*refstr;
  boolean spawn, active;
  ydyo_activeInfo ainfo;
  yoenv env;

  spawn = ysResGetBool("yd.show-spawn");
  active = ysResGetBool("yd.show-active");

  if( active )
    yoEnvInit( &env );

  yslPrint("%-24.24s %-28.28s %-16.16s %6d\n",
	   yduIdStr(info->intf_ydim_implInfo),
	   yduIdStr(info->impl_ydim_implInfo),
	   yduStr(info->host_ydim_implInfo),
	   info->alist_ydim_implInfo._length);

  if(active)
  {
    yslPrint("   Active:\n");
    for(j = 0; j < info->alist_ydim_implInfo._length; j++)
    {
      ainfo = ydim_active__get_ainfo( info->alist_ydim_implInfo._buffer[j],
				     &env );
      refstr = yoRefToStr( (dvoid*)ainfo.yort_ydyo_activeInfo );
      yslPrint("%8s%s\n",
	       (ainfo.implFlags_ydyo_activeInfo & yort_suspended_implFlag) ?
	       "Susp-" : "",
	       refstr);
      ydyo_activeInfo__free( &ainfo, yoFree );
      yoFree((dvoid *)refstr);
    }
  }
  if(spawn)
  {
    yslPrint("%-3s Spawn\n", "");
    yslPrint("%-7s Path/Args: %s ",
	     "", yduStr(info->pathName_ydim_implInfo));
    for(j = 0; j < info->args_ydim_implInfo._length; j++)
      yslPrint("%s ", info->args_ydim_implInfo._buffer[j]);
    yslPrint("\n");
    yslPrint("%-7s minInstances:  %d  maxInstances: %d\n", "",
	     info->minInstances_ydim_implInfo,
	     info->maxInstances_ydim_implInfo);
  }

  if( active )
    yoEnvFree( &env );
}





void ydqInfoHdr(void)
{
  yslPrint("%-24s %-28s %-16s %-6s\n",
    "INTERFACE", "IMPLEMENTATION", "HOST", "ACTIVE");
  yslPrint("%-24s %-28s %-16s %-6s\n",
	   "------------------------",
	   "----------------------------",
	   "----------------",
	   "------"
	  );
}

void ydqActiveHdr(void)
{
  yslPrint("  %-10s %-24s %-24s\n",
	   "Active:", "Interface", "Implementation");
}

void ydqProcHdr(void)
{
  yslPrint("%-20s %-12s %-12s %-12s %-12s\n",
    "YORT OBJ", "NAME", "HOST", "PID", "AFFINITY");
  yslPrint("%-20s %-12s %-12s %-12s %-12s\n",
	   "--------------------",
	   "------------",
	   "------------",
	   "------------",
	   "------------"
	  );
}


void ydqShowActiveInfo( ydyo_activeInfo* ainfo ) 
{
  yslPrint("  %-10s %-24.24s %-24.24s\n",
	   "",
 	   yduIdStr(ainfo->intf_ydyo_activeInfo),
	   yduIdStr(ainfo->impl_ydyo_activeInfo));
}

void ydqShowProc( yort_procInfo *pinfo )
{
  char *refstr;
  refstr = yoRefToStr( (dvoid*)pinfo->self_yort_procInfo );
  yslPrint("%-20s %-12s %-12s %-12s %-12s\n",
	   refstr,
	   yduStr(pinfo->name_yort_procInfo),
	   yduStr(pinfo->host_yort_procInfo),
	   yduStr(pinfo->pid_yort_procInfo),
	   yduStr(pinfo->affinity_yort_procInfo));
  yoFree( (dvoid*)refstr );
}


void ydqShowSProcHdr(void)
{
  yslPrint("%10s %10s %10s %10s %8s %12s\n",
	   "HOST", "PID", "AFFINITY", "NAME", "PARENT", "STATE" );

  yslPrint("%10s %10s %10s %10s %8s %12s\n",
	   "----------",
	   "----------",
	   "----------",
	   "----------",
	   "--------",
	   "------------" );
}

void ydqShowSProc( ydsp_procInfo *spinfo )
{
  CONST char *state;

  switch( spinfo->state_ydsp_procInfo )
  {
  case ydsp_start_ok:    state = "start_ok"; break;
  case ydsp_start_fail:  state = "start_fail"; break;
  case ydsp_exit_ok:    state = "exit_ok"; break;
  case ydsp_exit_bad:    state = "exit_bad"; break;
  case ydsp_exit_unknown:    state = "exit_unknown"; break;
  default:	    state = "UNKNOWN"; break;
  }

  yslPrint("%10.10s %10.10s %10.10s %10.10s %8s %12.12s\n",
	   yduStr(spinfo->host_ydsp_procInfo),
	   yduStr(spinfo->pid_ydsp_procInfo),
	   yduStr(spinfo->affinity_ydsp_procInfo),
	   yduStr(spinfo->name_ydsp_procInfo),
	   spinfo->parent_ydsp_procInfo ? "spawned" : "orphan",
	   state ); 
}



void ydqShowMtdList( ydmtdMetricsList *mlist )
{
  ub4	i;
  ydmtdMetrics	*m;

  for( i = 0 ; i < mlist->_length ; i++ )
  {
    m = &mlist->_buffer[i];
    yslPrint("Proc %s:%s:%s %s %dk %dCs y:%x\n",
	     m->pinfo_ydmtdMetrics.host_yort_procInfo,
	     m->pinfo_ydmtdMetrics.pid_yort_procInfo,
	     yduStr(m->pinfo_ydmtdMetrics.affinity_yort_procInfo),
	     m->pinfo_ydmtdMetrics.name_yort_procInfo,
	     m->pinfo_ydmtdMetrics.memKb_yort_procInfo,
	     m->pinfo_ydmtdMetrics.cpuCs_yort_procInfo,
	     m->pinfo_ydmtdMetrics.self_yort_procInfo );
    ydqShowDispInfoList( &m->dlist_ydmtdMetrics );
    ydqShowImplAllList( &m->metrics_ydmtdMetrics, FALSE );
    yslPrint("\n");
  }
}

void ydqDispInfoHdr( ub4 n )
{
  yslPrint("  %d Dispatchers active:\n", n ); 
  yslPrint("   %-49.49s %-8.8s %7s %4s %4s\n",
	   "Dispatcher", "QueName", "numDisp", "opSt", "adSt" );
}

void ydqDispInfo( yort_dispInfo *dinfo, char *queueName )
{
  char *rs;

  rs = yoRefToStr( (dvoid*)dinfo->self_yort_dispInfo );
  yslPrint("   %-49.49s %-8.8s %7d %4s %4s\n",
	   rs, queueName, 
	   dinfo->numDispatches_yort_dispInfo,
	   ydqSvcState( dinfo->operState_yort_dispInfo ),
	   ydqSvcState( dinfo->adminState_yort_dispInfo ));
  yoFree( (dvoid*)rs );
}

void ydqShowDispInfoList( yort_dispInfoList *dlist )
{
  ub4 noreg i;
  yort_queueInfo qi;
  yoenv ev;

  NOREG(i);

  yoEnvInit(&ev);
  ydqDispInfoHdr( dlist->_length ); 
  for( i = 0 ; i < dlist->_length ; i++ )
  {
    yseTry
    {
      qi = yort_queue__get_info( dlist->_buffer[i].queue_yort_dispInfo, &ev );
      ydqDispInfo( &dlist->_buffer[i], qi.name_yort_queueInfo );
      yort_queueInfo__free( &qi, yoFree );
    }
    yseCatchAll
    {
      yslPrint("[Caught exception %s\n]", yseExid );
    }
    yseEnd;
  }
  yoEnvFree(&ev);
}

void ydqShowImplAllList( yort_implAllList *ialist, boolean terse )
{
  ub4	    j, k;

  yort_implAll	    *am; 
  yort_queueInfo    *qi;
  yort_methodInfo   *mi;
  char	    ibuf[ 512 ];

  if( terse )
    yslPrint("   %-48.48s %8s %4s %4s %4s\n",
	     "Intf/Impl", "totIn", "qLen", "opSt", "adSt");

  for( j = 0 ; j < ialist->_length ; j++ )
  {
    am = &ialist->_buffer[j];
    qi = &am->queue_yort_implAll;
    
    ysFmtStr( ibuf, "%s/%s", 
	      am->info_yort_implAll.intf_yort_implInfo,
	      yduStr(am->info_yort_implAll.impl_yort_implInfo) );

    yslPrint( terse ? "   %-48.48s %8d %4d %4s %4s\n" :
	     "\n   Intf %s -- totIn: %d  qlen: %d  opSt: %s  adSt: %s\n",
	     ibuf,
	     am->info_yort_implAll.totalInboundAssocs_yort_implInfo,
	     qi->curLen_yort_queueInfo,
	     ydqSvcState( am->info_yort_implAll.operStatus_yort_implInfo),
	     ydqSvcState( am->info_yort_implAll.adminStatus_yort_implInfo));

    if( !terse )
    {
      ydqQInfoHdr();
      ydqQInfo( qi );
      yslPrint("   %-2d %-33s %8s %6s %6s %6s\n",
	       am->methods_yort_implAll._length,
	       "Methods", "Count", "lastMs", "maxMs", "avgMs" );
      for( k = 0; k < am->methods_yort_implAll._length; k++ )
      {
	mi = &am->methods_yort_implAll._buffer[k];
	yslPrint("   %-36.36s %8d %6d %6d %6d\n",
		 yduStr(mi->name_yort_methodInfo),
		 mi->cnt_yort_methodInfo,
		 mi->lastTimeMs_yort_methodInfo,
		 mi->maxTimeMs_yort_methodInfo,
		 mi->avgTimeMs_yort_methodInfo);
      }
    }
  }
}


void ydqQInfoHdr(void)
{
  yslPrint("   %-16s %8s %6s %6s %6s %8s %8s %8s\n",
	   "Queue Name", "totEnq", "curLen", "maxLen", "avgLen",
	   "curMs", "maxMs", "avgMs" );
}

void ydqQInfo( yort_queueInfo *qi )
{
  yslPrint("   %-16.16s %8d %6d %6d %6d %8d %8d %8d\n",
	   yduStr(qi->name_yort_queueInfo),
	   qi->totEnq_yort_queueInfo,
	   qi->curLen_yort_queueInfo,
	   qi->maxLen_yort_queueInfo,
	   qi->avgLen_yort_queueInfo,
	   qi->curDelayMs_yort_queueInfo,
	   qi->maxDelayMs_yort_queueInfo,
	   qi->avgDelayMs_yort_queueInfo );
}


void ydqShowYortQueueList( yort_queueList *qlist )
{
  ub4 i;
  yoenv ev;
  yort_queueInfo qinfo;
  
  yoEnvInit( &ev );
  ydqQInfoHdr();

  for( i = 0 ; i < qlist->_length ; i++ )
  {
    qinfo = yort_queue__get_info( qlist->_buffer[i], &ev );
    ydqQInfo( &qinfo );
    yort_queueInfo__free( &qinfo, yoFree );
  }
  yoEnvFree( &ev );
}

CONST char *ydqSvcState( yort_svcState state )
{
  CONST char *rv;
  switch( state )
  {
  case yort_SvcStDown:	    rv = "down"; break;
  case yort_SvcStRun:	    rv = "run"; break;
  case yort_SvcStHalted:    rv = "halt"; break;
  case yort_SvcStCongested: rv = "congested"; break;
  case yort_SvcStRestart:   rv = "restarg"; break;
  case yort_SvcStQuiesce:   rv = "quiesce"; break;
  default:		    rv = "unknown"; break; 
  }
  return( rv );
}

CONST char *ydqProcState( yort_procState state )
{
  CONST char *rv;
  switch( state )
  {
  case yort_ProcStRunning:	rv = "Running"; break;
  case yort_ProcStRunnable:	rv = "halt"; break;
  case yort_ProcStNotRunnable:	rv = "NotRunnable"; break;
  case yort_ProcStInvalid:	rv = "Invalid"; break;
  default:			rv = "unknown"; break; 
  }
  return( rv );
}
