/* mx/src/yd/ydbr.c */


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





#ifndef YDBR_ORACLE
# include <ydbr.h>
#endif

#define ENTRY_POINT ydbrMain
#include <s0ysmain.c>


static ysmtagDecl(ydbrTagMsg) = "ydbrmsg";
static ysmtagDecl(ydbrTagCtx) = "ydbrctx";
static ysmtagDecl(ydbrTagCxd) = "nscxd";
static ysmtagDecl(ydbrTagInd) = "nsind";
static ysmtagDecl(ydbrTagIep) = "ydbriep";
static ysmtagDecl(ydbrTagReq) = "ydbrReq";
static ysmtagDecl(ydbrTagHdr) = "ydbrHdr";


externdef ysidDecl(YDBR_EX_NS_INIT_FAILED) = "ydbr::nsinit_failed";
externdef ysidDecl(YDBR_EX_NS_FAILED) = "ydbr::ns_subsystem_failed";
externdef ysidDecl(YDBR_EX_INTERNAL_ERROR) = "ydbr::internal_error";
externdef ysidDecl(YDBR_EX_CLOSE_CONNECTION) = "ydbr::NS_conn_closed";
externdef ysidDecl(YDBR_EX_MESSAGE_ERROR) = "ydbr::GIOP_MessageError";
externdef ysidDecl(YDBR_EX_COMM_FAILED) = "ydbr::communication_failed";
externdef ysidDecl(YDBR_EX_CONNMAX_REACHED) = "ydbr::max_connections_reached";
externdef ysidDecl(YDBR_EX_DEST_NOT_FOUND) = "ydbr::Destination not found";

 
struct ysargmap argmap[] = 
{
    {'n', "ydbr.max_connections", 1},
    {'c', "ydbr.close_on_error=true", 0},
    {'i', "ydbr.close_idle_conn=true", 0},
    {'g', "ydbr.grace_before_close", 1},
    {'p', "ydbr.port", 1},
    {'t', "ydbr.threads.package", 1},
    {0, (char *)0, 0}
};

ydbr_Stats ydbr_GetStatistics_i( ydbr or, yoenv* ev);

static const struct ydbr__tyimpl ydbr__impl =
{
  ydbr_GetStatistics_i
};


STATICF ydbrctx *ydbrInit(char *nm, sword argc, char **argv, dvoid *osdp); 
STATICF void ydbrTerm(ydbrctx *ctx);
STATICF void ydbrNsIdler(dvoid *usrp, CONST ysid *exid, 
                         dvoid *arg, size_t argsz);
STATICF void ydbrStartMnAsyncRd(ydbrctx *ctx) ;
STATICF void ydbrStartIiopAsyncRd(ydbrctx *ctx);
STATICF void ydbrMnMsgRcvHdlr(dvoid *usrp, CONST ysid *exid, 
                              dvoid *arg, size_t argsz);
STATICF void ydbrCleanUp(dvoid *usrp, CONST ysid *exid, dvoid *arg, 
                         size_t argsz);
STATICF ydbriep *ydbrMatchCxd(nscxd *cxd, ydbrctx *ctx);
STATICF void ydbrHandleMnMsg(ydbrmsg *msg);
STATICF void ydbrIcall(nscxd *cxd, ydbrctx *ctx);
STATICF void ydbrOcall(nscxd *cxd, ydbrctx *ctx);
STATICF void ydbrOsend(nscxd *cxd, ydbrctx *ctx);
STATICF void ydbrIsend(nscxd *cxd, ydbrctx *ctx);
STATICF void ydbrRecvError(ydbriep *ep, ydbrmsg *msg);
STATICF void ydbrHandleIiopMsg(ydbriep *ep, ydbrmsg *msg);
STATICF sword ydbrNsSend(ydbriep *ep);
STATICF nscnd *ydbrConvert2NVString(ydbrctx *ctx, CONST char *host, ub2 port);
STATICF void ydbrIntrHdlr(dvoid *usrp, CONST ysid *exid, 
                          dvoid *arg, size_t argsz);
STATICF sb4 ydbrRecvGiopHdr(nscxd *cxd, ydbrmsg *msg);
STATICF sb4 ydbrRecvGiopMsg(nscxd *cxd, ydbrmsg *msg);
STATICF void ydbrMsgSentHdlr(dvoid *usrp, CONST ysid *exid, 
                             dvoid *arg, size_t argsz);
STATICF void ydbrFreeMsg(ydbrmsg *msg, ub2 howmany);
STATICF void ydbrFreeEp(ydbrctx  *ctx, ydbriep *ep, ub2 howmany, 
                        CONST ysid  *exid);
STATICF ydbriep *ydbrAllocEp(ydbrctx *ctx, ub2 flag);
STATICF void ydbrSendMnMsg(ydbrmsg *msg);
STATICF void ydbrUpdateMcount(ydbriep *ep, ub1 type);
STATICF ydbrmsg *ydbrFindPartialMsg(ydbrmsg *msgq);
STATICF sword ydbrEqReq(dvoid *elm, dvoid *key, size_t keysz);
STATICF void ydbrBuildAbv(yobrMsg *ymsg, ysbv **abv, sword *anbv,
                  yogiIiopProf **ior, ydbrGiopMsgHdr **hdr);
STATICF yogiIiopProf *ydbrGetProf(yogiIOR *ior);
STATICF void ydbrFreeYdProf(dvoid *prof);
STATICF void ydbrFreeProf(yogiIiopProf *prof);
STATICF sword ydbrEqProf(dvoid *elm, dvoid *key, size_t keysz);
STATICF yogiIiopProf *ydbrDuplicateProf(yogiIiopProf *prof);
STATICF sword ydbrCmpProf(yogiIiopProf  *prof1, yogiIiopProf *prof2);
STATICF void ydbrScanf(char *buf,  char *host, ub2 *port);
STATICF char * ydbrFindString(char *buf, char *find);
STATICF ub4 yoProfHash(CONST dvoid *prof, size_t dummy, ub4 max);
STATICF void ydbrSendCloseConn(ydbriep *ep);



STATICF ydbrctx *
ydbrInit(char *nm, sword argc, char **argv, dvoid *osdp) 
{

   char        *tp, *str;
   ysevt       *evt = NULLP(ysevt);
   ydbrctx     *ctx = NULLP(ydbrctx);
   dvoid       *npdgbl = 0;
   nlstdatt    init;
   size_t      len = 0;
   dvoid       *nsgbl = NULLP(dvoid);
   sword       sts;
   text        errbuf[512];

   ysInit(osdp, nm);

   
   sts = ysArgParse(argc, argv, argmap);
   if (sts != YSARG_NORMAL) return(NULLP(ydbrctx));

   smniInit((dvoid *)0, (mnLogger)yslError);
   yoInit();

   yseTry 
      
      ctx = (ydbrctx *) ysmGlbAlloc(sizeof(ydbrctx), ydbrTagCtx);
      CLRSTRUCT(*ctx);
      ctx->osdp = osdp; 
      ctx->heap = ysmGlbHeap();
      
      
      ctx->hostname = ysGetHostName();
      ctx->pid = ysGetPid();
      ctx->flags = 0;
      if(ysResGetBool("ydbr.close_on_error"))
      {
          bis(ctx->flags, YDBRCLOSECONN_ONERROR);
      }

      if(ysResGetBool("ydbr.close_idle_conn"))
      {
          bis(ctx->flags, YDBRCLOSEIDLE_CONN);
      }

      tp = ysResGetLast("ydbr.threads.package");

      if(tp)
      {
         ysThrInit(tp);
         ctx->threaded = 1;
      } else
         ctx->threaded = 0;

      if(str = ysResGetLast("ydbr.max_connections"))
         ctx->maxfds = atoi(str);
      else
         ctx->maxfds = YDBR_DEF_MAX_CONNS;

      if(str = ysResGetLast("ydbr.grace_before_close"))
         sysb8ext(&ctx->idletime, atoi(str) * 1000000);
      else
         sysb8ext(&ctx->idletime, YDBR_IDLE_TIME);

      if(str = ysResGetLast("ydbr.port"))
         ctx->port = (ub2) atoi(str);
      else
         ctx->port = YDBRPORT;;

      ctx->ccount = ctx->ecount = 0;
 
      
      ctx->my_iiop_addr = (char *) ysmAlloc(ctx->heap, 
                                   4 + strlen(ctx->hostname) + 1 + 8, "addr");
      DISCARD sprintf(ctx->my_iiop_addr, "%s:%d", ctx->hostname, ctx->port);
      ctx->yobrctx = yobrInit("IIOP", ctx->my_iiop_addr);

      ctx->evtq = yoQueCreate("ydbrQue");
      
      DISCARD ysSetIntr(ydbrIntrHdlr, (dvoid *)ctx); 
      ctx->yev = yeevRemoteLog(NULL, ctx->evtq);
      

      sysb8ext(&ctx->cinterval, YDBR_CLEANUP_INTERVAL);
      evt = ysEvtCreate(ydbrCleanUp, (dvoid *)ctx, NULLP(ysque), FALSE);
      ysTimer(&ctx->cinterval, evt);

      
      ctx->reqhash = ysHshCreate((ub4)257, (yshash)0, ydbrEqReq, ysmFGlbFree);
      ctx->profhash = ysHshCreate((ub4)64, yoProfHash, ydbrEqProf, 
                                   ydbrFreeYdProf);

   yseCatchAll
      
      ysRecord(YDBR_ERROR(1), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), YSLEND);
      if(evt)
         ysEvtDestroy(evt);
      ydbrTerm(ctx);
      return(NULLP(ydbrctx));
   yseEnd;

   
   yseTry
      CLRSTRUCT(init);
      bis(init.mask_nlstdatt,  NLSTDATT_SYSPARMS | NLSTDATT_PF_ERRORS_OK 
                               | NLSTDATT_LOG | NLSTDATT_LG_ERRORS_OK 
	                       | NLSTDATT_TRACE );

#ifdef NS_TRACE
     init.syspdesc_nlstdatt.nlfnsname = (text *)YDBR_PFILENAME;
     init.syspdesc_nlstdatt.nlfnssize = YDBR_PFILENAMEL;

     init.trcdesc_nlstdatt.nlfnsname = (text *)YDBR_TFILENAME;
     init.trcdesc_nlstdatt.nlfnssize = YDBR_TFILENAMEL;
     init.trcparms_nlstdatt[NLSTDGO_TRACE_DIR] = (text *)YDBR_TRCDIRNAME;
     init.trcparms_nlstdatt[NLSTDGO_TRACE_LEVEL] = (text *)YDBR_TRCLEVNAME;
#endif

      if(nlstdgg(&npdgbl, &init, errbuf, sizeof(errbuf), &len))
      {
         ysRecord(YDBR_DEBUG(1), YSLSEV_DEBUG(8), (char *)0, 
                      YSLSTR("nlstdgg()"), YSLSTR(errbuf), YSLEND);
         yseThrow(YDBR_EX_NS_INIT_FAILED);
      }
      ctx->npdgbl = npdgbl;

      
      ctx->ind = (nsind *) ysmAlloc(ctx->heap, sizeof(nsind), ydbrTagInd);
      CLRSTRUCT(*ctx->ind); 
      ctx->ind->nsindmxa = ctx->maxfds;

      nsgblini(npdgbl, &nsgbl, ctx->ind);
      if(!nsgbl)
      {
         ysRecord(YDBR_DEBUG(2), YSLSEV_DEBUG(8), (char *)0, 
                      YSLSTR("nsgblini()"), YSLEND);
         yseThrow(YDBR_EX_NS_INIT_FAILED);
      }
      ctx->nsgbl = nsgbl;

      
      ctx->my_tns_addr = ydbrConvert2NVString(ctx, ctx->hostname, ctx->port);

      
      ctx->lfd = (ydbriep *) ysmAlloc(ctx->heap, sizeof(ydbriep), 
                                      ydbrTagIep);
      CLRSTRUCT(*ctx->lfd);

      ctx->nfds = 0;

      

      ctx->eplist = (ydbriep *)ysmAlloc(ctx->heap, sizeof(ydbriep),
                                        ydbrTagIep);
      CLRSTRUCT(*ctx->eplist);
      bis(ctx->eplist->flags, YDBRIEP_HEADER);

      ctx->request_id = 0;
      
      ysSetIdler("ydbrNsIdler()", ydbrNsIdler, (dvoid *)ctx);

  yseCatchAll
      ysRecord(YDBR_ERROR(1), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), YSLEND);
      ydbrTerm(ctx);
      return(NULLP(ydbrctx));
  yseEnd;

  return(ctx);  
}



STATICF void
ydbrTerm(ydbrctx *ctx)
{

   dvoid   *osdp;

   ysRecord(YDBR_ERROR(2), YSLSEV_INFO, (char *)0, YSLNONE); 

   
   ysSetIdler("ydbrNsIdler()",(ysHndlr)0,(dvoid *)0);
   DISCARD ysSetIntr((ysHndlr)0, (dvoid *)ctx); 

   
   if(ctx)
   {
      ysmCheck(ctx, ydbrTagCtx);
      osdp = ctx->osdp;

      if(ctx->my_tns_addr)
      {
          ysmFree(ctx->heap, ctx->my_tns_addr->nscndbuf);
          ysmFree(ctx->heap, ctx->my_tns_addr);
      }

      if(ctx->my_iiop_addr)
      {
          ysmFree(ctx->heap, ctx->my_iiop_addr);
      }
      if(ctx->ind)
         ysmFree(ctx->heap, ctx->ind);
      if(ctx->lfd)
         ydbrFreeEp(ctx, ctx->lfd, YDBRONE, (ysid *)0);

      if(ctx->eplist)
      {
          ydbrFreeEp(ctx, ctx->eplist, YDBRALL, YDBR_EX_INTERNAL_ERROR);
          ysmFree(ctx->heap, ctx->eplist);
      }

      if(ctx->reqhash) 
          ysHshDestroy(ctx->reqhash); 
      if(ctx->profhash) 
          ysHshDestroy(ctx->profhash); 
      if(ctx->yev)
         yeevTerm(ctx->yev);
      
      if(ctx->evtq)
          yoQueDestroy(ctx->evtq);

      if(ctx->yobrctx)
          yobrTerm(ctx->yobrctx);

      DISCARD nlstdstp(ctx->npdgbl);
      DISCARD nsgbltrm(ctx->nsgbl);
      yoTerm();
      mnTerm();
      ysmGlbFree(ctx); 
      ysTerm(osdp); 
   }
}


STATICF void
ydbrNsIdler(dvoid *usrp, CONST ysid *exid, dvoid *arg, size_t argsz)
{

   ydbrctx   *ctx = (ydbrctx *)usrp;
   sword     ret;
   eword     i;
   nscxd     **cxdl;
   eword     ncxd;
   boolean   *work = (boolean *)arg; 
   nsres     res;

   ysmCheck(ctx, ydbrTagCtx);
   if(ctx->nsidle)
      return;

   ctx->nsidle++;   
#ifdef YDBR_TRACE
   ysRecord(YDBR_TRCINFO(0), YSLSEV_ERR, (char *)0, YSLNONE);
#endif

   if(exid)
   {
       ysRecord(YDBR_ERROR(3), YSLSEV_ERR, (char *)0, 
                  YSLSTR("ydbrNsIdler()"), YSLSTR(exid), YSLEND);
       return;
   }

   
   
   if(bit(ctx->flags, YDBR_DONE_EVREG))
   {
       ret = nsevwait(ctx->nsgbl, &cxdl, &ncxd, TRUE, &res);
       if(ret < 0)
       {
          ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, YSLSTR("nsevwait()"),
                    YSLSB4(res.nsresns), YSLEND);
          ctx->nsidle--;
          return;
       }
   }
   else
   {
       ctx->nsidle--;
       return;
   }

   
   for(i = 0; i < ncxd; i++)
   {
      switch(cxdl[i]->nscxdsel)
      {
         case NSVICALL:
         case NSVOCALL:
         case NSVISEND:
         case NSVOSEND:
         case NSVISEND | NSVOSEND:
    
              if(bit(cxdl[i]->nscxdsel, NSVICALL))
              {
                  
                  ydbrIcall(cxdl[i], ctx);
              }

              if(bit(cxdl[i]->nscxdsel, NSVOCALL))
              {
                 
                 ydbrOcall(cxdl[i], ctx);
              }

              if(bit(cxdl[i]->nscxdsel, NSVOSEND))
              {
                 
                 ydbrOsend(cxdl[i], ctx);
              }

              if(bit(cxdl[i]->nscxdsel, NSVISEND))
              {
                  
                  ydbrIsend(cxdl[i], ctx);
                  *work = TRUE;
               }

               break;

           default:
               
               ysRecord(YDBR_ERROR(10), YSLSEV_ERR, (char *)0,
                        YSLSB4(cxdl[i]->nscxdsel), YSLPTR(cxdl[i]), YSLEND);
               break;
      } 
   }
   ctx->nsidle--;
}



STATICF boolean
ydbrMain(dvoid *osdp, char *nm, sword argc, char **argv)
{
  ydbrctx   *ctx; 
  boolean   ret_val = TRUE;
  
  ctx = ydbrInit(nm, argc, argv, osdp);

  if(!ctx)
     return(1);

  if(ctx->threaded)
  {
     
     ydbrTerm(ctx);
  } 
  else 
  {
     yseTry
      
      ydbrStartMnAsyncRd(ctx); 
  
      
      ydbrStartIiopAsyncRd(ctx);  
  
      
      yoSetImpl(ydbr__id, (char *)0, ydbr__stubs, (dvoid *)&ydbr__impl,
                           (yoload)0, TRUE, (dvoid *)ctx);
      yoImplReady(ydbr__id, (char *)0, ctx->evtq);

      yoService(ctx->evtq);

    yseCatchAll
      ysRecord(YDBR_ERROR(11), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), YSLEND);
      ret_val = FALSE;
    yseEnd;
    ydbrTerm(ctx);
    return(ret_val);
  }
}

STATICF void
ydbrStartMnAsyncRd(ydbrctx *ctx) 
{

  

   yseTry
      ctx->revt = ysEvtCreate(ydbrMnMsgRcvHdlr, (dvoid *)ctx, ctx->evtq, FALSE);
      yobrRecvMsg(ctx->yobrctx, ctx->revt);
   yseCatchAll
     if(ctx->revt)
     {
        ysEvtDestroy(ctx->revt);
        ctx->revt = (ysevt *)0;
     }
     ysRecord(YDBR_ERROR(12), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), 
              YSLSTR("ydbrStartMnAsyncRd()"), YSLEND);
     yseRethrow;
   yseEnd;
}

STATICF void
ydbrStartIiopAsyncRd(ydbrctx *ctx)
{

  sword    ret;
  ydbriep  *lfd = ctx->lfd;   
  ub2      opt = NSLDONTBLOCK;
  nsinf    info;

  

  CLRSTRUCT(info);
  bis(info.nsinflcl[1], NSLASYNC | NSLRAW | NSLNOPORTREQ);
  lfd->cxd = (nscxd *) ysmAlloc(ctx->heap, sizeof(nscxd), ydbrTagCxd);
  CLRSTRUCT(*lfd->cxd);
  bis(lfd->flags, YDBRIEP_CXD_VALID);
  ret = nslisten(ctx->nsgbl, lfd->cxd, ctx->my_tns_addr, 
                   &info, NULLP(nsres));
  if(ret < 0)
  {
     ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, YSLSTR("nslisten()"),
                  YSLSB4(lfd->cxd->nscxdres.nsresns), YSLEND);
     yseThrow(YDBR_EX_NS_FAILED);
  }
  nsexport(lfd->cxd, &lfd->fd); 
  lfd->slot = ssysAddFd(ctx->osdp, lfd->fd, (ub4) SSYS_FD_INPUT);
  bis(lfd->flags, YDBRIEP_LSTN);
  lfd->cxd->nscxdevt = NSVICALL;

  ret = nsevrgs(ctx->nsgbl, lfd->cxd);
  if(ret < 0)
  {
     ysRecord(YDBR_ERROR(22), YSLSEV_ERR, (char *)0, 
                  YSLSB4(lfd->cxd->nscxdres.nsresns), YSLSTR("ICALL"),
                  YSLSTR("ydbrStartIiopAsyncRd"), YSLEND);
     yseThrow(YDBR_EX_NS_FAILED);
  }
  bis(ctx->flags, YDBR_DONE_EVREG);
   

  ret = nsctl(lfd->cxd, NSCSETL, &opt);
  if(ret <0)
  {
     
     ysRecord(YDBR_ERROR(4), YSLSEV_INFO, (char *)0, YSLSTR("nsctl()"),
                  YSLSB4(lfd->cxd->nscxdres.nsresns), YSLEND);
  }
}


STATICF void
ydbrMnMsgRcvHdlr(dvoid *usrp, CONST ysid *exid, dvoid *arg, size_t argsz)
{

  ydbrctx              *ctx = (ydbrctx *)usrp;
  ydbrmsg              *msg;
  yobrMsg              *ymsg = (yobrMsg *)arg;

  ysmCheck(ctx, ydbrTagCtx);

  
  ctx->revt = (ysevt *)0;

  
  if(exid)
  {
    ysRecord(YDBR_ERROR(3), YSLSEV_ERR, (char *)0, 
              YSLSTR("ysbrMnMsgRcvHdlr()"), YSLSTR(exid), YSLEND);
    if(!ysidEq(exid, YT_EX_DISCONN)) 
        goto initiate_next;
    else
        return;
  }
  msg = ysmAlloc(ctx->heap, sizeof(ydbrmsg), ydbrTagMsg);
  CLRSTRUCT(*msg);
  msg->ctx = ctx;
  CPSTRUCT(msg->yobrmsg, *ymsg);
  msg->from_addr = yoDuplicate(ymsg->from);
  bis(msg->flags, YDBRMSG_YOBRALLOC);

  

  yseTry
    ydbrBuildAbv(ymsg, &msg->abv, &msg->anbv, &msg->prof, &msg->hdr);
    msg->org_prof = msg->prof;
    ydbrHandleMnMsg(msg);

  yseCatchAll
    
    ysRecord(YDBR_ERROR(12), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), 
             YSLSTR("ydbrMnMsgRcvHdlr()"), YSLEND);
    ysRecord(YDBR_DEBUG(4), YSLSEV_DEBUG(8), (char *)0, YSLNONE);
  yseEnd;

  
initiate_next:

  ydbrStartMnAsyncRd(ctx); 
}

STATICF ydbriep *
ydbrMatchCxd(cxd, ctx)
nscxd    *cxd;
ydbrctx  *ctx;
{

    ydbriep    *list;

    YDBRGET_MUTEX(ctx->eplist, ctx);
    list = ctx->eplist;
    while(list)
    {
        if(list->cxd == cxd)
             break;
        else
           list = list->next;
    }
    YDBRRELEASE_MUTEX(ctx->eplist, ctx);
    return(list);
}



STATICF void
ydbrHandleMnMsg(msg)
ydbrmsg               *msg;
{

    ydbriep       *ep;
    ydbrctx       *ctx = msg->ctx;
    ydbrReqMap    *req;
    ub4           request_id;
    ydbrProf      *ydprof;

    ysmCheck(msg, ydbrTagMsg);
    ysmCheck(msg->ctx, ydbrTagCtx);

    request_id = yobrGetReqId(msg->abv, msg->anbv);

    switch(msg->hdr->message_type)
    {
       case ydbr_Request:

          
          req = (ydbrReqMap *) ysmGlbAlloc(sizeof(ydbrReqMap), ydbrTagReq);
          CLRSTRUCT(*req);
          req->mn_addr = msg->from_addr;
          req->orig_reqid = request_id;
          req->ctx = ctx;
          while(++ctx->request_id)
          {
             
             if(!ysHshFind(ctx->reqhash, (dvoid *)&ctx->request_id, 
                   sizeof(ub4)))
             {
                  break;
             }
             
             if(ctx->request_id == YDBR_MAX_REQUEST_ID)
                 ctx->request_id = 0;
          }
          req->mapped_reqid = ctx->request_id;
          req->stage = 1;
          req->ep = NULLP(ydbriep);
          msg->req = req;
          ysHshIns(ctx->reqhash, &req->mapped_reqid, sizeof(ub4), (dvoid *)req);
 
          
          yobrPutReqId(msg->abv, msg->anbv, ctx->request_id);
          

           ydprof = (ydbrProf *) ysHshFind(ctx->profhash, (dvoid *)msg->prof, 
               sizeof(yogiIiopProf));
           if(ydprof)
           {
               msg->prof = ydprof->fwd_prof;
           }
#ifdef YDBR_TRACE
           ysRecord(YDBR_TRCINFO(1), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
           ydbrSendMnMsg(msg);
        
          break;

       case ydbr_Reply:

#ifdef YDBR_TRACE
        ysRecord(YDBR_TRCINFO(2), YSLSEV_ERR, (char *)0, YSLNONE);
#endif

        
        req = (ydbrReqMap *)ysHshFind(msg->ctx->reqhash, &request_id, 
                                     sizeof(ub4));
        if(!req)
        {
           
           ysRecord(YDBR_ERROR(13), YSLSEV_ERR, (char *)0, 
                  YSLSB4(msg->hdr->message_type), YSLSB4(request_id), 
                  YSLSTR("MN"), YSLEND);
           msg->ctx->ecount++;
           ydbrFreeMsg(msg, YDBRONE);
           yseThrow(YDBR_EX_INTERNAL_ERROR);
        }
        
        req->stage = 2;
        msg->req = req;
        
        yobrPutReqId(msg->abv, msg->anbv, req->orig_reqid);

        

#ifdef YDBR_TRACE
        ysRecord(YDBR_TRCINFO(3), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
        ydbrSendMnMsg(msg);
        break;

       case ydbr_LocateRequest:
       case ydbr_LocateReply:
       case ydbr_MessageError:
       case ydbr_CloseConnection:
       case ydbr_CancelRequest:
       default:

          
          ysRecord(YDBR_ERROR(14), YSLSEV_ERR, (char *)0, 
                   YSLSB4(msg->hdr->message_type), 
                   YSLSTR("ydbrHandleMnMsg()"), YSLEND);
          ydbrFreeMsg(msg, YDBRONE);
          break;
    }

}
            


STATICF void
ydbrSendMnMsg(ydbrmsg *msg)
{
     ydbrctx      *ctx = msg->ctx;
     ydbrReqMap   *req = msg->req;
     ydbrReqMap   *req1;
     ydbriep      *ep = req->ep;
     sword        ret;

     if(!req->ep)
     {
 
        
        YDBRGET_MUTEX(ctx->eplist, ctx);
        ep = ctx->eplist->next;
        while(ep)
        {
            if((ep->port == msg->prof->port) && 
               !strcmp((char *)ep->host, (char *)msg->prof->host))
            {
                 break;
            }
            ep = ep->next;
        }
    
        if(!ep)
        {
           ydbriep      *newep;
           nsinf         info;
           nscnd         *cnda;

           
           yseTry
             newep = ydbrAllocEp(ctx, (ub2) YDBRIEP_DATA);

           yseCatchAll
             ysRecord(YDBR_ERROR(7), YSLSEV_ERR, (char *)0, 
                           YSLSTR(yseExid), YSLEND);
             yobrSendException(ctx->yobrctx, req->mn_addr, req->orig_reqid, 
                                YDBR_EX_INTERNAL_ERROR);
             req1 = (ydbrReqMap *) ysHshRem(ctx->reqhash, &req->mapped_reqid, 
                                            sizeof(ub4));
             if(req1 != req)
                 yseThrow(YDBR_EX_INTERNAL_ERROR);
             yoRelease(req->mn_addr);
             ysmGlbFree(req);
             ydbrFreeMsg(msg, YDBRONE); 
             yseRethrow;
           yseEnd;

           newep->host = (char *) ysmGlbAlloc(strlen(msg->prof->host) + 1,
                                         "HOST");
           DISCARD strcpy(newep->host, msg->prof->host);
           newep->port = msg->prof->port;

           CLRSTRUCT(info);
           bis(info.nsinflcl[1], NSLASYNC | NSLRAW | NSLNOPORTREQ);
           cnda = ydbrConvert2NVString(ctx, (char *) msg->prof->host, 
                                       msg->prof->port);

           ret = nscall(ctx->nsgbl, newep->cxd, cnda, NULLP(nscnd), 
                     NULLP(nscnd), &info, NULLP(nsres));
           ysmFree(ctx->heap, cnda->nscndbuf);
           ysmFree(ctx->heap, cnda);
           if(ret < 0)
           {
               
               ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, 
                        YSLSTR("nscall()"), 
                        YSLSB4(newep->cxd->nscxdres.nsresns), YSLEND);
               
               ctx->ecount++;
               ydbrFreeEp(ctx, ep, YDBRONE, YDBR_EX_INTERNAL_ERROR);
               req1 = (ydbrReqMap *) ysHshRem(ctx->reqhash, &req->mapped_reqid, 
                                            sizeof(ub4));
               if(req1 != req)
                 yseThrow(YDBR_EX_INTERNAL_ERROR);
               yoRelease(req->mn_addr);
               ysmGlbFree(req);
               ydbrFreeMsg(msg, YDBRONE); 
               yseThrow(YDBR_EX_COMM_FAILED);
           }

           
           
           newep->cxd->nscxdevt = NSVOCALL;
           ret = nsevrgs(ctx->nsgbl, newep->cxd);
           if(ret < 0)
           {
               ysRecord(YDBR_ERROR(22), YSLSEV_ERR, (char *)0, 
                             YSLSB4(newep->cxd->nscxdres.nsresns), 
                             YSLSTR("OCALL"), YSLSTR("ydbrSendMnMsg"), YSLEND);
               
               ctx->ecount++;
               ydbrFreeEp(ctx, newep, YDBRONE, YDBR_EX_INTERNAL_ERROR);
               req1 = (ydbrReqMap *) ysHshRem(ctx->reqhash, &req->mapped_reqid, 
                                            sizeof(ub4));
               if(req1 != req)
                 yseThrow(YDBR_EX_INTERNAL_ERROR);
               yoRelease(req->mn_addr);
               ysmGlbFree(req);
               ydbrFreeMsg(msg, YDBRONE); 
               yseThrow(YDBR_EX_NS_FAILED);
           }
        
           nsexport(newep->cxd, &newep->fd);
           newep->slot = ssysAddFd(ctx->osdp, newep->fd, (ub4) SSYS_FD_INPUT);
           req->ep = newep;

           ydbrUpdateMcount(newep, msg->hdr->message_type);
           YDBRINSERT_MSG(newep->sendq, msg);
           newep->sendqlen++;
           bis(newep->flags, YDBRIEP_MSG_PENDING);

           YDBRGET_MUTEX(ctx->eplist, ctx);
           YDBRINSERT_EP(ctx->eplist, newep);
           ctx->nfds++;
           YDBRRELEASE_MUTEX(ctx->eplist, ctx);
           newep->LastMsg = msg->hdr->message_type;
           return;
        }
        else
           req->ep = ep;
     }
      

     ydbrUpdateMcount(ep, msg->hdr->message_type);
     YDBRGET_MUTEX(ep, ctx);
     YDBRINSERT_MSG(ep->sendq, msg);
     ep->sendqlen++;
     YDBRRELEASE_MUTEX(ep, ctx);
     bis(ep->flags, YDBRIEP_MSG_PENDING);
     ep->LastMsg = msg->hdr->message_type;

     if(bit(ep->state, YDBRIEP_READY2WRITE))
     {
#ifdef YDBR_TRACE
         ysRecord(YDBR_TRCINFO(4), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
         ret = ydbrNsSend(ep); 
         if(ret == -1)
         {
             
             ysRecord(YDBR_ERROR(15), YSLSEV_ERR, (char *)0, 
                      YSLSB4(ep->cxd->nscxdres.nsresns), YSLEND);
             ctx->ecount++;
             if(bit(ctx->flags, YDBRCLOSECONN_ONERROR))
             {
                 YDBRGET_MUTEX(ctx->eplist, ctx);
                 YDBRDEQUE_EP(ep);
                 ctx->nfds--;
                 YDBRRELEASE_MUTEX(ctx->eplist, ctx);

                 
                 ydbrFreeEp(ctx, ep, YDBRONE, YDBR_EX_INTERNAL_ERROR);
              }
              else
                 ep->ErrCount++;
              yseThrow(YDBR_EX_COMM_FAILED);
         }
         
         bic(ep->state, YDBRIEP_READY2WRITE);

         if(ep->sendqlen)
             bis(ep->flags, YDBRIEP_MSG_PENDING);
         else
             bic(ep->flags, YDBRIEP_MSG_PENDING);
     }
#ifdef YDBR_TRACE
     else
     {
         ysRecord(YDBR_TRCINFO(5), YSLSEV_ERR, (char *)0, YSLNONE);
     }
#endif
     
           
     DISCARD nsevmute(ep->cxd, NSVOSEND, FALSE);
     bis(ep->state, YDBRIEP_WAIT2WRITE);
     ysClock(&ep->LastUsed);
}



STATICF sword
ydbrNsSend(ep)
ydbriep  *ep;
{

     sword     ret = 0;
     sword     i, rlen;
     ydbrmsg   *msg = ep->sendq->next;

     ysmCheck(ep, ydbrTagIep);
     if(!msg) return(ret);

     ysRecord(YDBR_DEBUG(6), YSLSEV_DEBUG(8), (char *)0, YSLNONE);
     for(i = 0; i < msg->anbv; i++)
     {
         rlen = msg->abv[i].len;
         ret = nssend(ep->cxd, NSWDATA, msg->abv[i].buf, &rlen, 0);
         if(ret < 0)
         {
             if(ep->cxd->nscxdres.nsresns == NSEWOULDBLOCK)
             {
                
                if(rlen == 0)
                {
                   
                   ysRecord(YDBR_ERROR(16), YSLSEV_INFO, (char *)0, 
                      YSLSB4(msg->abv[i].len), YSLSB4(rlen), YSLEND);
                   return((sword)0);
                }
                if(rlen < msg->abv[i].len)
                {
                   
                   ysRecord(YDBR_ERROR(16), YSLSEV_INFO, (char *)0, 
                      YSLSB4(msg->abv[i].len), YSLSB4(rlen), YSLEND);
                   msg->abv[i].buf+= rlen;
                   msg->abv[i].len-= rlen;
                   msg->anbv-= i;
                   return((sword)0);
                 }
             }    
             else
             {
                
                ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, 
                         YSLSTR("nssend()"), YSLSB4(ep->cxd->nscxdres.nsresns),
                         YSLEND);
                ep->ErrCount++;
                msg->ctx->ecount++;
                return((sword)-1);
             }
         }  
     } 

     
     ret = nsflush(ep->cxd);
     if(ret < 0)
     {
         ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, 
                  YSLSTR("nsflush()"), YSLSB4(ep->cxd->nscxdres.nsresns),
                  YSLEND);
     }
     ysRecord(YDBR_DEBUG(5), YSLSEV_DEBUG(8), (char *)0, YSLNONE);
     if(msg->req->stage == 2)
     {
          ydbrReqMap    *req1;

          req1 = (ydbrReqMap *) ysHshRem(msg->ctx->reqhash, 
                                         &msg->req->mapped_reqid, 
                                         sizeof(ub4));
          if(req1 != msg->req)
             yseThrow(YDBR_EX_INTERNAL_ERROR);
          if(req1->mn_addr)
             yoRelease(req1->mn_addr);
          ysmGlbFree(req1);
     }

     YDBRGET_MUTEX(ep, ctx);
     ep->sendqlen--;
     YDBRDEQUE_MSG(msg);
     YDBRRELEASE_MUTEX(ep, ctx);
     

     if(msg->hdr->message_type == ydbr_Request)
     {
        msg->req->msg = msg;
     }
     else
     {
         ydbrFreeMsg(msg, YDBRONE);
     }
     return((sword)0);
}


STATICF void
ydbrHandleIiopMsg(ydbriep *ep, ydbrmsg *msg)
{

    ysevt          *evt = NULLP(ysevt);
    ydbrReqMap     *req = NULLP(ydbrReqMap);
    ub4            request_id;
    ydbrctx        *ctx = msg->ctx;
    ysbv           *reply_bv;
    sword          reply_nbv;
    ydbrmsg        *reply_msg;
    yogiLocRep     reply;
    yogiIiopProf   *prof;
    ub2            yobrsend = 0;
    ydbrProf       *ydprof = NULLP(ydbrProf);
    yogiMsg        gm;

    ysmCheck(msg, ydbrTagMsg);
    ysmCheck(ctx, ydbrTagCtx);

    ep->LastMsg = msg->hdr->message_type;
    ydbrUpdateMcount(ep, msg->hdr->message_type);
    request_id = yobrGetReqId(msg->ydbrbv, msg->ydbrnbv);

    switch(msg->hdr->message_type)
    {

      case ydbr_Request:
      case ydbr_LocateRequest:

      YDBRDEQUE_MSG(msg); 
      ep->recvqlen--;

      yseTry
      {
        req = (ydbrReqMap *) ysmGlbAlloc(sizeof(ydbrReqMap), ydbrTagReq);
        CLRSTRUCT(*req);
        req->ep = ep;
        req->ctx = ctx;
        req->orig_reqid = request_id;
        while(++ctx->request_id)
        {
             
             if(!ysHshFind(ctx->reqhash, &ctx->request_id, sizeof(ub4)))
                 break;
             
             if(ctx->request_id == YDBR_MAX_REQUEST_ID)
                ctx->request_id = 0;
        }
        req->mapped_reqid = ctx->request_id;
        if(msg->hdr->message_type == ydbr_Request)
        {
            req->stage = 1;
            msg->objref = yobrGetObjRef(msg->ydbrbv, msg->ydbrnbv);
            req->mn_addr = yoDuplicate(msg->objref);
        }
        else
        {
            req->stage = 2;
        }
        ysHshIns(ctx->reqhash, &req->mapped_reqid, sizeof(ub4), (dvoid *)req);

        if(msg->hdr->message_type == ydbr_Request)
        {
           
           yobrPutReqId(msg->ydbrbv, msg->ydbrnbv, ctx->request_id);
           evt = ysEvtCreate(ydbrMsgSentHdlr, (dvoid *)req, ctx->evtq, FALSE);
           bis(req->flags, YDBRREQ_HDLR_ACTIVE);
#ifdef YDBR_TRACE
           ysRecord(YDBR_TRCINFO(6), YSLSEV_ERR, (char *)0, YSLNONE);
#endif 
           yobrsend = 1;
           req->current = msg;
           yobrSendMsg(ctx->yobrctx, req->mn_addr, msg->ydbrbv, 
                       msg->ydbrnbv, evt);
        }
        else
        {
#ifdef YDBR_TRACE
           ysRecord(YDBR_TRCINFO(7), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
           
           reply.reqid = request_id;
           reply.status = YOGILRS_HERE;
           yobrEncGiopLocRep(&reply, NULLP(yogiIOR), &reply_bv, &reply_nbv);
           
           reply_msg = (ydbrmsg *) ysmAlloc(ctx->heap, sizeof(ydbrmsg),
                                  ydbrTagMsg);
           CLRSTRUCT(*reply_msg);
           bis(reply_msg->flags, YDBRMSG_ABV);
           reply_msg->ctx = ctx;
           reply_msg->abv = reply_bv;
           reply_msg->anbv = reply_nbv;
           reply_msg->req = req;
           reply_msg->hdr = (ydbrGiopMsgHdr *) ysmAlloc(ctx->heap, 
                             sizeof(ydbrGiopMsgHdr), ydbrTagHdr);
           reply_msg->hdr->message_type = ydbr_LocateReply;

           
#ifdef YDBR_TRACE
           ysRecord(YDBR_TRCINFO(8), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
           ydbrSendMnMsg(reply_msg);
        }
      }
      yseCatchAll
      {
          if(evt)
          {
             ysEvtDestroy(evt);
          }
          
          ysRecord(YDBR_ERROR(12), YSLSEV_ERR, (char *)0, 
                  YSLSTR(yseExid), YSLSTR("ydbrHandleIiopMsg()"), YSLEND);
          ysRecord(YDBR_DEBUG(7), YSLSEV_DEBUG(8), (char *)0, YSLNONE);
          yobrsend = 0;
          if(req)
          {
             ydbrReqMap    *req1;
             req1 = (ydbrReqMap *) ysHshRem(ctx->reqhash, &req->mapped_reqid, 
                                            sizeof(ub4));
             if(req1 != req)
                 yseThrow(YDBR_EX_INTERNAL_ERROR);
             yoRelease(req->mn_addr);
             ysmGlbFree(req);
          }
      }
      yseEnd; 

      
      if(!yobrsend)
        ydbrFreeMsg(msg, YDBRONE);
      break;

      case ydbr_Reply:

      YDBRDEQUE_MSG(msg); 
      ep->recvqlen--;
      yseTry
      {

        
        req = (ydbrReqMap *) ysHshFind(ctx->reqhash, &request_id, sizeof(ub4));
        if(!req)
        {
           
           ysRecord(YDBR_ERROR(13), YSLSEV_ERR, (char *)0, 
                  YSLSB4(msg->hdr->message_type), YSLSB4(request_id), 
                  YSLSTR("NS"), YSLEND);
           yseThrow(YDBR_EX_INTERNAL_ERROR);
        }
        

        CLRSTRUCT(gm);
        yobrDecGiopMsg(msg->ydbrbv, msg->ydbrnbv, &gm);
        if(gm.b.rep.hdr.status == YOGIRS_LOCFWD)
        {
            prof = ydbrGetProf(&gm.b.rep.ior); 
            if(!prof)
            {
                
                ysRecord(YDBR_ERROR(23), YSLSEV_ERR, (char *)0, 
                         YSLSTR("ydbrBuildAbv()"), YSLEND); 
                yobrSendException(ctx->yobrctx, req->mn_addr, 
                                    req->orig_reqid, YDBR_EX_DEST_NOT_FOUND);
                yseThrow(YDBR_EX_DEST_NOT_FOUND);
            }
            
            ydprof = (ydbrProf *) ysHshFind(ctx->profhash, 
                                            (dvoid *)req->msg->org_prof, 
                                            sizeof(yogiIiopProf));
            if(!ydprof)   
            {
               ydprof = (ydbrProf *) ysmGlbAlloc(sizeof(ydbrProf), "ydprof");
               ydprof->flags = 0;
               ydprof->orig_prof = ydbrDuplicateProf(req->msg->org_prof);
            }
            else
            {
               if(ydbrCmpProf(prof, ydprof->orig_prof) ||
                  ydbrCmpProf(prof, ydprof->fwd_prof))
               {
                  
                  ysRecord(YDBR_ERROR(24), YSLSEV_ERR, (char *)0, YSLNONE);
                  yobrSendException(ctx->yobrctx, req->mn_addr, 
                                    req->orig_reqid, YDBR_EX_DEST_NOT_FOUND);
                  yseThrow(YDBR_EX_DEST_NOT_FOUND);
               }
               ydbrFreeProf(ydprof->fwd_prof);
            }
 
            ydprof->fwd_prof = ydbrDuplicateProf(prof);
            ysHshIns(ctx->profhash, (dvoid *)ydprof->orig_prof,
                      sizeof(yogiIiopProf), (dvoid *)ydprof);
            req->ep = 0;
            req->msg->prof = prof;
            ydbrSendMnMsg(req->msg);
        }
        else
        {
            ydbrmsg     *omsg = req->msg;
            req->stage = 2;
            
            yobrPutReqId(msg->ydbrbv, msg->ydbrnbv, req->orig_reqid);
            
           
            evt = ysEvtCreate(ydbrMsgSentHdlr, (dvoid *)req, ctx->evtq, FALSE);
            bis(req->flags, YDBRREQ_HDLR_ACTIVE);
#ifdef YDBR_TRACE
           ysRecord(YDBR_TRCINFO(9), YSLSEV_ERR, (char *)0, YSLNONE);
#endif 
            yobrsend = 1;
            req->current = msg;
            yobrSendMsg(ctx->yobrctx, req->mn_addr, msg->ydbrbv, 
                        msg->ydbrnbv, evt);
            
            if(omsg)
               ydbrFreeMsg(omsg, YDBRONE);
        }
      }
      yseCatchAll
      {
          if(evt)
          {
             ysEvtDestroy(evt);
          }
          
          ysRecord(YDBR_ERROR(12), YSLSEV_ERR, (char *)0, 
                  YSLSTR(yseExid), YSLSTR("ydbrHandleIiopMsg()"), YSLEND);
          ysRecord(YDBR_DEBUG(7), YSLSEV_DEBUG(8), (char *)0, YSLNONE);
          
          yobrsend = 0;
          if(req)
          {
             ydbrReqMap    *req1;
             req1 = (ydbrReqMap *) ysHshRem(ctx->reqhash, &req->mapped_reqid, 
                                            sizeof(ub4));
             if(req1 != req)
                 yseThrow(YDBR_EX_INTERNAL_ERROR);
             yoRelease(req->mn_addr);
             ydbrFreeMsg(req->msg, YDBRONE);
             ysmGlbFree(req);
             req = 0;
          }
      }
      yseEnd; 

      if(!yobrsend)
      {
          ydbrFreeMsg(msg, YDBRONE);
      }
      yobrFreeGiopMsg(&gm);

      break;

     case ydbr_LocateReply:
          
          ysRecord(YDBR_ERROR(14), YSLSEV_ERR, (char *)0, 
                   YSLSB4(msg->hdr->message_type), 
                   YSLSTR("ydbrHandleIiopMsg()"), YSLEND);
          YDBRDEQUE_MSG(msg); 
          ydbrFreeMsg(msg, YDBRONE);
          ep->recvqlen--;

          break;

     case ydbr_CancelRequest:
          
          ysRecord(YDBR_ERROR(21), YSLSEV_INFO, (char *)0, YSLNONE); 
          YDBRDEQUE_MSG(msg); 
          ydbrFreeMsg(msg, YDBRONE);
          ep->recvqlen--;
          break;

     case ydbr_CloseConnection:
     case ydbr_MessageError:
     default:
          
       
          YDBRDEQUE_EP(ep);
          ctx->nfds--;
          
          if(msg->hdr->message_type == ydbr_CloseConnection)
             ydbrFreeEp(ctx, ep, YDBRONE, YDBR_EX_CLOSE_CONNECTION);
          else
             ydbrFreeEp(ctx, ep, YDBRONE, YDBR_EX_MESSAGE_ERROR);

          break;
    }
}



STATICF nscnd *
ydbrConvert2NVString(ydbrctx *ctx, CONST char *host, ub2 port)
{
    nscnd      *cnda;

    cnda = (nscnd *)ysmAlloc(ctx->heap, sizeof(nscnd), "ydbrCnd");
    cnda->nscndbuf = (ub1 *) ysmAlloc(ctx->heap, YDBR_TNS_STRING_SIZ, "tns");
    cnda->nscndblen = YDBR_TNS_STRING_SIZ;

    DISCARD sprintf((char *)cnda->nscndbuf, 
               "(ADDRESS=(PROTOCOL=tcp)(port=%.4d)(host=%s))", port, host);
    cnda->nscndlen = strlen((char *)cnda->nscndbuf);
    return(cnda);
}



STATICF void
ydbrIntrHdlr(dvoid *usrp, CONST ysid *exid, dvoid *arg, size_t argsz)
{
   ydbrctx    *ctx = (ydbrctx *)usrp;

   ysmCheck(ctx, ydbrTagCtx);
   ysRecord(YDBR_ERROR(17), YSLSEV_INFO, (char *)0, YSLSTR(exid), YSLEND);

   yoShutdown(ctx->evtq);
}



STATICF sb4
ydbrRecvGiopHdr(nscxd *cxd, ydbrmsg *msg)
{
   sword    ret;
   ub1      what;
   size_t   alen, rlen;
   ub1      *buf, *tmp;
   ub4      size;
   ub1      *tmp1 = (ub1 *)&size;

    if(!msg->hdr)
    {
        msg->hdr = (ydbrGiopMsgHdr *) ysmAlloc(msg->ctx->heap, 
                             sizeof(ydbrGiopMsgHdr), ydbrTagHdr);
        buf = (ub1 *)msg->hdr;
        alen = rlen = GIOP_HDR_LEN;
    }
    else
    {
        buf = (ub1 *)msg->hdr + msg->rlen;
        alen = rlen = GIOP_HDR_LEN - msg->rlen;
        
    }

    
    ret = nsrecv(cxd, &what, buf, &rlen, 0);
    
    if(ret < 0)
    {
       
       return((sb4) ret);
    }
    else if (rlen < alen)
    {
         return((sb4)rlen);
    }

    
    
    buf = (ub1 *) msg->hdr;
    if (!(buf[0] == 'G' && buf[1] == 'I'
            && buf[2] == 'O' && buf[3] == 'P'))
    {
      ysRecord(YDBR_ERROR(18), YSLSEV_ERR, (char *)0, YSLNONE);
      ysmFree(msg->ctx->heap, (dvoid *)msg->hdr);
      msg->hdr = 0;
      return(-1);
    }

    
    if(buf[6] != YDBR_MY_BYTE_SEX)
    {
        tmp = &buf[8];
        tmp1 [3] = *tmp++;
        tmp1 [2] = *tmp++;
        tmp1 [1] = *tmp++;
        tmp1 [0] = *tmp++;
    }
    else
        size = *((ub4 *)&buf[8]);

    
    yoAllocMsgBufs(msg->ctx->heap, size + GIOP_HDR_LEN, &msg->ydbrbv, 
                   &msg->ydbrnbv);
    msg->tmpbuf = (ub1 *) ysmAlloc(msg->ctx->heap, size + GIOP_HDR_LEN, "msg");
    DISCARD memcpy(msg->tmpbuf, (ub1 *) msg->hdr, GIOP_HDR_LEN);
    
    msg->hdr->message_size = size;
    return((sb4) rlen);
}

 

STATICF sb4
ydbrRecvGiopMsg(nscxd *cxd, ydbrmsg *msg)
{
   sword    ret;
   ub1      what;
   size_t   alen, rlen;
   ub1      *buf1;
   

    alen = rlen = msg->alen - msg->rlen; 
    ret = nsrecv(cxd, &what, &msg->tmpbuf[msg->rlen], &rlen, 0);
    
    if(ret < 0)
    {
       
       return((sb4) ret);
    }
    else if (rlen < alen)
    {
       return((sb4)rlen);
    }
    
    DISCARD ysBvScatter(msg->tmpbuf, msg->alen, msg->ydbrbv, msg->ydbrnbv);
    ysmFree(msg->ctx->heap, msg->tmpbuf);
    msg->tmpbuf = 0;
    return((sb4)rlen);
}



STATICF void
ydbrMsgSentHdlr(dvoid *usrp, CONST ysid *exid, dvoid *arg, size_t argsz)
{

    ydbrReqMap  *req = (ydbrReqMap *)usrp;

#ifdef YDBR_TRACE
    ysRecord(YDBR_TRCINFO(10), YSLSEV_ERR, (char *)0, YSLNONE);
#endif

    ysmCheck(req, ydbrTagReq);
    if(exid)
    {
        ysRecord(YDBR_ERROR(3), YSLSEV_ERR, (char *)0, 
                 YSLSTR("ydbrMsgSentHdlr()"), YSLSTR(exid), YSLEND);
        req->stage = 2; 
    }
    ydbrFreeMsg(req->current, YDBRONE);
    if(req->stage == 2 || bit(req->flags, YDBRREQ_FREE_REQ))
    {
        
        ydbrReqMap      *req1;
        ysRecord(YDBR_ERROR(19), YSLSEV_DEBUG(8), (char *)0, YSLNONE);
        req1 = (ydbrReqMap *) ysHshRem(req->ctx->reqhash, &req->mapped_reqid, 
                                            sizeof(ub4));
        
        if(req->mn_addr)
           yoRelease(req->mn_addr);
        ysmGlbFree(req);
    }
    else
        bic(req->flags, YDBRREQ_HDLR_ACTIVE);
}






STATICF void
ydbrFreeMsg(ydbrmsg *msg, ub2 howmany)
{

  ydbrctx   *ctx = msg->ctx;
  ydbrmsg   *tmsg, *otmsg;

  
  if(howmany == YDBRALL)
     tmsg = msg->next;
  else
     tmsg = msg;
   
  while(tmsg)
  {
      ysmCheck(tmsg, ydbrTagMsg);

      otmsg = tmsg;

      if(bit(tmsg->flags, YDBRMSG_YOBRALLOC))
      {
          yobrFreeMsg(&tmsg->yobrmsg);
      }
      else
      {
         if(tmsg->ydbrbv)
            ysBvFree(ctx->heap, tmsg->ydbrbv, tmsg->ydbrnbv);
      }

      if(bit(tmsg->flags, YDBRMSG_ABV))
      {
         ysBvFree(ctx->heap, tmsg->abv, tmsg->anbv);
      }
      else
      {
         if(tmsg->abv)
            ysmGlbFree((dvoid *)tmsg->abv);
      }
      if(tmsg->hdr)
         ysmGlbFree((dvoid *)tmsg->hdr);
      if(tmsg->objref)
         yoRelease(tmsg->objref);

      if(tmsg->tmpbuf)
         ysmFree(ctx->heap, (dvoid *)tmsg->tmpbuf);

      if(howmany == YDBRALL)
      {
          YDBRDEQUE_MSG(tmsg);
          tmsg = tmsg->next;
      }
      else
          tmsg = NULLP(ydbrmsg);

      ysmFree(ctx->heap, (dvoid *)otmsg);
  }
}


STATICF void
ydbrFreeEp(ydbrctx  *ctx, ydbriep *ep, ub2 howmany, CONST ysid  *exid)
{
    ydbriep    *tep, *otep;
    ydbrReqMap *req;
    yshshpos   hpos;
    dvoid      *elem;

    if(howmany == YDBRALL)
       tep = ep->next;
    else
       tep = ep;

    while(tep)
    {
       ysmCheck(tep, ydbrTagIep);
       otep = tep;

       if(!bit(tep->flags, YDBRIEP_LSTN))
       {
          ydbrFreeMsg(tep->recvq, YDBRALL);
          ydbrFreeMsg(tep->sendq, YDBRALL);
          ysmFree(ctx->heap, (dvoid *)tep->recvq);
          ysmFree(ctx->heap, (dvoid *)tep->sendq);
       }
     
       if(bit(tep->flags, YDBRIEP_CXD_VALID))
       {
          DISCARD nsdisc(tep->cxd, 0);
          ysmFree(ctx->heap, (dvoid *)tep->cxd); 
       }
       if(tep->slot)
          ssysRemFd(ctx->osdp, tep->slot, (ub4) SSYS_FD_INPUT);
       if(tep->host)
         ysmGlbFree((dvoid *)tep->host);
 
       

       for (elem = ysHshFirst(ctx->reqhash, &hpos); elem; 
            elem = ysHshNext(ctx->reqhash, &hpos))
       {
           req = (ydbrReqMap *)elem;
           if(req->ep == ep)
           {
              if(exid)
              {
                  yobrSendException(ctx->yobrctx, req->mn_addr, 
                                    req->orig_reqid, exid);
              }
              if(bit(req->flags, YDBRREQ_HDLR_ACTIVE))
              {
                 bis(req->flags, YDBRREQ_FREE_REQ);
              }
              else
              {
                 ysHshRemByPos(ctx->reqhash, &hpos);
                 yoRelease(req->mn_addr);
                 ysmGlbFree(req);
              }
           }
       }

       if(howmany == YDBRALL)
       {
          YDBRDEQUE_EP(tep);
          tep = tep->next;
       }
       else
          tep = NULLP(ydbriep); 

       ysmFree(ctx->heap, (dvoid *)otep);
    }
}



STATICF ydbriep *
ydbrAllocEp(ydbrctx *ctx, ub2 flag)
{

    ydbriep  *ep = NULLP(ydbriep);
    ysmhp    *heap = ctx->heap;

  yseTry

    if(ctx->nfds == ctx->maxfds)
       yseThrow(YDBR_EX_CONNMAX_REACHED);

    ep = ysmAlloc(heap, sizeof(ydbriep), ydbrTagIep);
    CLRSTRUCT(*ep);
    bis(ep->flags, flag);

    ep->cxd = ysmAlloc(heap, sizeof(nscxd), ydbrTagCxd);
    CLRSTRUCT(*ep->cxd);
    bis(ep->flags, YDBRIEP_CXD_VALID);

    ep->recvq = (ydbrmsg *) ysmAlloc(heap, sizeof(ydbrmsg), ydbrTagMsg);
    CLRSTRUCT(*ep->recvq);
    bis(ep->recvq->flags, YDBRMSG_HEADER); 
    ep->recvq->ctx = ctx;

    ep->sendq = (ydbrmsg *) ysmAlloc(heap, sizeof(ydbrmsg), ydbrTagMsg);
    CLRSTRUCT(*ep->sendq);
    bis(ep->sendq->flags, YDBRMSG_HEADER); 
    ep->sendq->ctx = ctx;
    ysClock(&ep->LastUsed);
    ctx->ccount++;

  yseCatchAll

    ysRecord(YDBR_ERROR(12), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), 
             YSLSTR("ydbrAllocEp()"), YSLEND);
    if(ep)
    {
       if(ep->cxd)
          ysmFree(heap, ep->cxd);
       if(ep->sendq)
          ysmFree(heap, ep->sendq);
       if(ep->recvq)
          ysmFree(heap, ep->recvq);

       ysmFree(heap, ep);
       ep = NULLP(ydbriep);
    }

    yseRethrow;

  yseEnd;

  return(ep);
}



STATICF ydbrmsg *
ydbrFindPartialMsg(ydbrmsg *msgq)
{
   ydbrmsg  *pmsg = msgq->next;

   while(pmsg)
   {
     if(bit(pmsg->flags, YDBRMSG_PARTIAL_HDR) ||
        bit(pmsg->flags, YDBRMSG_PARTIAL_MSG)) 
     {
         break;
     }
     pmsg = pmsg->next;
   }
   return(pmsg);
}



STATICF void
ydbrCleanUp(dvoid *usrp, CONST ysid *exid, dvoid *arg, size_t argsz)
{
   ysevt    *evt = NULLP(ysevt);
   ydbrctx  *ctx = (ydbrctx *)usrp;
   ydbriep  *ep;
   sysb8    time_now, time_diff;
   ydbriep  *oep;


   
   if(!ysidEq(exid, YS_EX_TIMEOUT))
      return;
   ysmCheck(ctx, ydbrTagCtx);
   ep = ctx->eplist->next;

   
   ysClock(&time_now); 
   while(ep)
   {
     oep = ep;
     ep = ep->next;
     sysb8sub(&time_diff, &time_now, &oep->LastUsed);
     if(sysb8cmp(&time_diff, >, &ctx->idletime))
     {
        ysRecord(YDBR_ERROR(20), YSLSEV_INFO, (char *)0, YSLPTR(oep->cxd), 
                 YSLEND);

        if((!YDBR_REQUESTS_PENDING(oep) || bit(ctx->flags, YDBRCLOSEIDLE_CONN))
          && (ctx->maxfds - ctx->nfds <= YDBRLOW_EP_MARK))
        {
           
           if(bit(oep->flags, YDBRIEP_CONN))
              ydbrSendCloseConn(oep);
           
           ysRecord(YDBR_ERROR(9), YSLSEV_INFO, (char *)0, YSLPTR(oep->cxd), 
                    YSLEND);
           YDBRDEQUE_EP(oep);
           ctx->nfds--;
           ydbrFreeEp(ctx, oep, YDBRONE, YDBR_EX_CLOSE_CONNECTION);  
        }
     }
   }

   

   ysRecord(YDBR_INFO(0), YSLSEV_INFO, (char *)0, YSLSB4(ctx->ccount), 
            YSLSB4(ctx->ecount), YSLSB4(ctx->nfds), YSLEND);
   
   yseTry
      evt = ysEvtCreate(ydbrCleanUp, (dvoid *)ctx, NULLP(ysque), FALSE);
      ysTimer(&ctx->cinterval, evt);
   yseCatchAll
      ysRecord(YDBR_ERROR(12), YSLSEV_ERR, (char *)0, YSLSTR(yseExid), 
               YSLSTR("ydbrCleanUp()"), YSLEND);
      if(evt)
         ysEvtDestroy(evt);
      yoShutdown(ctx->evtq);
   yseEnd;
}

STATICF void 
ydbrUpdateMcount(ydbriep *ep, ub1 type)
{

   switch(type)
   {
       case ydbr_Request:
            ep->mcount.Request++;
            break;
       case ydbr_Reply:
            ep->mcount.Reply++;
            break;
       case ydbr_CancelRequest:
            ep->mcount.CancelRequest++;
            break;
       case ydbr_LocateRequest:
            ep->mcount.LocateRequest++;
            break;
       case ydbr_LocateReply:
            ep->mcount.LocateReply++;
            break;
       case ydbr_CloseConnection:
            ep->mcount.CloseConnection++;
            break;
       case ydbr_MessageError:
            ep->mcount.MessageError++;
            break;
       default:
            
            break;
   }
}




void
ydbrIcall(nscxd *cxd, ydbrctx *ctx)
{

       ydbriep      *ep;
       sword        ret;
       ydbriep      *lfd = ctx->lfd;
       size_t       buflen = YDBR_TNS_STRING_SIZ;
       nsinf        info;
       char         buf[YDBR_TNS_STRING_SIZ];

#ifdef YDBR_TRACE
       ysRecord(YDBR_TRCINFO(11), YSLSEV_ERR, (char *)0, YSLNONE);
#endif

       if(cxd != lfd->cxd)
       {
            
           ysRecord(YDBR_ERROR(5), YSLSEV_INFO, (char *)0, YSLNONE);
           return;
       }
       yseTry
          ep = ydbrAllocEp(ctx, (ub2) YDBRIEP_CONN); 
       yseCatchAll
          ysRecord(YDBR_ERROR(7), YSLSEV_ERR, (char *)0, 
                    YSLSTR(yseExid), YSLEND);
          return;
       yseEnd;
   
       CLRSTRUCT(info);
       
       bis(info.nsinflcl[1], NSLRAW | NSLNOPORTREQ);
       ret = nsanswer(ctx->nsgbl, ep->cxd, NULLP(nscnd), &info, 
                       NULLP(nsres), lfd->cxd);
       if(ret < 0)
       {
          
          ysRecord(YDBR_ERROR(6), YSLSEV_INFO, (char *)0, YSLNONE);
          ctx->ecount++;
          ydbrFreeEp(ctx, ep, YDBRONE, (ysid *) 0);
          return;
       }

       
       YDBRGET_MUTEX(ctx->eplist, ctx);
       YDBRINSERT_EP(ctx->eplist, ep);
       ctx->nfds++;
       YDBRRELEASE_MUTEX(ctx->eplist, ctx); 

       nsexport(ep->cxd, &ep->fd);
       ep->slot = ssysAddFd(ctx->osdp, ep->fd, (ub4) SSYS_FD_INPUT);
       
       ep->host = (char *) ysmGlbAlloc(YDBR_HOST_STRING_SIZ,
                                         "HOST");
       ret = nsgetaddr(ep->cxd, (eword) 0, buf, 
                           &buflen, 0);
       if(ret < 0)
       {
           
           ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, 
                    YSLSTR("nsgetaddr()"), 
                    YSLSB4(ep->cxd->nscxdres.nsresns), YSLEND);
           ysmGlbFree(ep->host);
           ep->host = 0;
       }
       else
       {
          
          ydbrScanf(buf, ep->host, &ep->port);
       }
       
       ep->cxd->nscxdevt = NSVISEND | NSVOSEND;
       bis(ep->state, YDBRIEP_WAIT2READ | YDBRIEP_WAIT2WRITE); 
       ret = nsevrgs(ctx->nsgbl, ep->cxd);
       if(ret < 0)
       {
          ysRecord(YDBR_ERROR(22), YSLSEV_ERR, (char *)0, 
                    YSLSB4(ep->cxd->nscxdres.nsresns), YSLSTR("ISEND|OSEND"), 
                    YSLSTR("ydbrIcall"), YSLEND);
          yseThrow(YDBR_EX_NS_FAILED);
       }
       
       DISCARD nsevmute(ep->cxd, NSVOSEND, TRUE);
       return;
}




void
ydbrOcall(nscxd *cxd, ydbrctx *ctx)
{

       ydbriep      *ep;
       sword        ret;

#ifdef YDBR_TRACE
       ysRecord(YDBR_TRCINFO(12), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
       ep = ydbrMatchCxd(cxd, ctx);
       if(!ep)
       {
          
          ysRecord(YDBR_ERROR(8), YSLSEV_ERR, (char *)0, 
                   YSLSTR("NSVOCALL"), YSLEND);
          ctx->ecount++; 
          return;
       }

       

       bis(ep->state, YDBRIEP_WAIT2WRITE | YDBRIEP_WAIT2READ);
       ep->cxd->nscxdevt = NSVOSEND | NSVISEND;
       ret = nsevrgs(ctx->nsgbl, ep->cxd);
       if(ret)
       {
            ysRecord(YDBR_ERROR(22), YSLSEV_ERR, (char *)0, 
                    YSLSB4(ep->cxd->nscxdres.nsresns), YSLSTR("OSEND|ISEND"),
                    YSLSTR("ydbrOcall"), YSLEND);
            ctx->ecount++;
            ep->ErrCount++;
            return;
       }
}




void
ydbrOsend(nscxd *cxd, ydbrctx *ctx)
{

       ydbriep     *ep;
       sword       ret;

       ep = ydbrMatchCxd(cxd, ctx);
       if(!ep)
       {
          
          ysRecord(YDBR_ERROR(8), YSLSEV_ERR, (char *)0, 
                   YSLSTR("NSVOSEND"), YSLEND);
          ctx->ecount++;
          return;
       }
       
       bic(ep->state, YDBRIEP_WAIT2WRITE);
       bis(ep->state, YDBRIEP_READY2WRITE);
       if(bit(ep->flags, YDBRIEP_MSG_PENDING))
       {
#ifdef YDBR_TRACE
       ysRecord(YDBR_TRCINFO(13), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
              ysClock(&ep->LastUsed);
              ret = ydbrNsSend(ep);
              if(ret == -1)
              {
                  
                  ctx->ecount++; 
                  if(bit(ctx->flags, YDBRCLOSECONN_ONERROR))
                  {
                      ysRecord(YDBR_ERROR(9), YSLSEV_INFO, 
                               (char *)0, YSLPTR(ep->cxd), YSLNONE);
                      YDBRGET_MUTEX(ctx->eplist, ctx);
                      YDBRDEQUE_EP(ep);
                      ctx->nfds--;
                      YDBRRELEASE_MUTEX(ctx->eplist, ctx);

                      
                      ydbrFreeEp(ctx, ep, YDBRONE, 
                                 YDBR_EX_INTERNAL_ERROR);
                   }
                   else
                      ep->ErrCount++;
                   return;
              }
              
              bic(ep->state, YDBRIEP_READY2WRITE);
              bis(ep->state, YDBRIEP_WAIT2WRITE);
              if(!ep->sendqlen)
              {
                  bic(ep->flags, YDBRIEP_MSG_PENDING);
                  
                  DISCARD nsevmute(ep->cxd, NSVOSEND, TRUE);
              }
       }
       else
       {
#ifdef YDBR_TRACE
       ysRecord(YDBR_TRCINFO(14), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
             
             DISCARD nsevmute(ep->cxd, NSVOSEND, TRUE);
       }
}



void
ydbrIsend(nscxd *cxd, ydbrctx *ctx)
{
       ydbriep    *ep;
       ydbrmsg    *msg;
       ub4        msglen, hdr_len;
       sb4        ret;

#ifdef YDBR_TRACE
       ysRecord(YDBR_TRCINFO(15), YSLSEV_ERR, (char *)0, YSLNONE);
#endif
       ep = ydbrMatchCxd(cxd, ctx);
       if(!ep)
       {
          
          ysRecord(YDBR_ERROR(8), YSLSEV_ERR, (char *)0, 
                  YSLSTR("NSVISEND"), YSLEND);
          ctx->ecount++;
          return;
       }
       ysClock(&ep->LastUsed);

                        
       if(bit(ep->flags, YDBRIEP_PARTIAL_RECV))
       {
          msg = ydbrFindPartialMsg(ep->recvq);
          if(bit(msg->flags, YDBRMSG_PARTIAL_HDR)) 
          {
            
            hdr_len = msg->alen - msg->rlen;
            goto read_hdr;
          }
          else
          {
             msglen = msg->alen - msg->rlen;
             goto read_msg;
          }
       }

       
       msg = (ydbrmsg *) ysmAlloc(ctx->heap, sizeof(ydbrmsg),
                                  ydbrTagMsg);
       CLRSTRUCT(*msg);
       msg->ctx = ctx;
       YDBRGET_MUTEX(ep->recvq, ctx);
       YDBRINSERT_MSG(ep->recvq, msg);
       ep->recvqlen++;
       YDBRRELEASE_MUTEX(ep->recvq, ctx);

       
       hdr_len = GIOP_HDR_LEN;
read_hdr:
       ret = ydbrRecvGiopHdr(ep->cxd, msg);

       if(ret < 0)
       {
          ydbrRecvError(ep, msg);
          return;
       }
       if(ret != hdr_len)
       {
          bis(msg->flags, YDBRMSG_PARTIAL_HDR);
          bis(ep->flags, YDBRIEP_PARTIAL_RECV);
          msg->alen = GIOP_HDR_LEN;
          msg->rlen = ret;
          return; 
       }

       
       msg->alen = GIOP_HDR_LEN + msg->hdr->message_size;
       msg->rlen = GIOP_HDR_LEN;
       msglen = msg->hdr->message_size;
       bic(msg->flags, YDBRMSG_PARTIAL_HDR);
read_msg:
       
       ret = ydbrRecvGiopMsg(ep->cxd, msg);
       if(ret < 0)
       {
          ydbrRecvError(ep, msg);
          return;
       }
      
       if( ret != msglen)
       {
          bit(msg->flags, YDBRMSG_PARTIAL_MSG);
          bis(ep->flags, YDBRIEP_PARTIAL_RECV);
          msg->alen = GIOP_HDR_LEN + msg->hdr->message_size;
          msg->rlen = GIOP_HDR_LEN + ret;
          return; 
       }
       bic(msg->flags, YDBRMSG_PARTIAL_MSG);
       bic(ep->flags, YDBRIEP_PARTIAL_RECV);
       
       ydbrHandleIiopMsg(ep, msg);
       DISCARD nsevmute(ep->cxd, NSVOSEND, FALSE);

}



void
ydbrRecvError(ydbriep *ep, ydbrmsg *msg)
{ 
      msg->ctx->ecount++; 
      ysRecord(YDBR_ERROR(4), YSLSEV_ERR, (char *)0, YSLSTR("nsrecv()"), 
                YSLSB4(ep->cxd->nscxdres.nsresns), YSLEND); 
      if(ep->cxd->nscxdres.nsresns == NSEENDOFFILE || 
                         bit(msg->ctx->flags, YDBRCLOSECONN_ONERROR)) 
      { 
         ysRecord(YDBR_ERROR(9), YSLSEV_INFO, (char *)0, YSLPTR(ep->cxd), 
                  YSLEND); 
         YDBRGET_MUTEX(msg->ctx->eplist, msg->ctx); 
         YDBRDEQUE_EP(ep); 
         msg->ctx->nfds--; 
         YDBRRELEASE_MUTEX(msg->ctx->eplist, msg->ctx); 
         ydbrFreeEp(msg->ctx, ep, YDBRONE, YDBR_EX_INTERNAL_ERROR); 
      } 
      else 
      { 
        ysRecord(YDBR_DEBUG(3), YSLSEV_DEBUG(8), (char *)0, YSLNONE); 
        YDBRDEQUE_MSG(msg); 
        ydbrFreeMsg(msg, YDBRONE); 
        ep->recvqlen--; 
        ep->ErrCount++; 
      } 
      return;
}

sword 
ydbrEqReq(dvoid *elm, dvoid *key, size_t keysz)
{
    ydbrReqMap     *req = (ydbrReqMap  *)elm;

    if(req->mapped_reqid == *(ub4 *)key)
       return((sword) 0);
    else
       return((sword) 1);
}


void
ydbrBuildAbv(ymsg, abv, anbv, ior, hdr)
yobrMsg         *ymsg;
ysbv            **abv;
sword           *anbv;
yogiIiopProf    **ior; 
ydbrGiopMsgHdr  **hdr;
{
    ysbv            *bv;
    sword           i, idx;
    yogiIiopProf    *iprof = NULLP(yogiIiopProf);

    
    *anbv = ymsg->nbv - ymsg->idx;
    *abv = bv = ysmGlbAlloc( sizeof(ysbv) * (*anbv), "abv"); 
    idx = ymsg->idx;
    bv[0].buf = ymsg->bv[idx].buf + ymsg->off;
    bv[0].len = ymsg->bv[idx].len - ymsg->off;
    idx++;

    for( i = 1; i < *anbv; i++, idx++)
    {
       bv[i].buf = ymsg->bv[idx].buf;
       bv[i].len = ymsg->bv[idx].len;
    }

    iprof = ydbrGetProf(&ymsg->ior);
    if(!iprof && ymsg->ior.type_id && ymsg->ior.profs )
    {
       
       ysRecord(YDBR_ERROR(23), YSLSEV_ERR, (char *)0, 
                YSLSTR("ydbrBuildAbv()"), YSLEND); 
       ysmGlbFree(bv);
       yseThrow(YDBR_EX_INTERNAL_ERROR);
    }       
    *ior = iprof;

    
    *hdr = (ydbrGiopMsgHdr *)ysmGlbAlloc(sizeof(ydbrGiopMsgHdr), "hdr");
    (*hdr)->message_type = ymsg->mtype;
}


yogiIiopProf *
ydbrGetProf(yogiIOR *ior)
{
    ysle            *le;
    yogiTagProf     *prof;
    yogiIiopProf    *iprof = NULLP(yogiIiopProf);

    if(ior->type_id && ior->profs)
    {
       
       for(le = ysLstHead(ior->profs); le; le = ysLstNext(le)) 
       { 
          prof = (yogiTagProf *) ysLstVal(le); 
          if(prof->tag == YOGIIOR_TAG_INTERNET) 
          {
             iprof = &(prof->pdata.iiop);
             break; 
          }
       }
    }
    return(iprof);
}


void 
ydbrFreeYdProf(dvoid *prof)
{
     ydbrProf  *p = (ydbrProf *)prof;
     if(p)
     {
         ydbrFreeProf(p->orig_prof);
         ydbrFreeProf(p->fwd_prof);
         ysmGlbFree(p);
     }
}


void 
ydbrFreeProf(yogiIiopProf *prof)
{
   if(prof)
   {
      ysmGlbFree(prof->host);
      if(prof->objkey)
         ysmGlbFree(prof->objkey);
      ysmGlbFree(prof);
   }
}


sword 
ydbrEqProf(dvoid *elm, dvoid *key, size_t keysz)
{
     ydbrProf     *ydprof = (ydbrProf *)elm;
     yogiIiopProf *prof = (yogiIiopProf *)key;

     if(ydbrCmpProf(ydprof->orig_prof, prof))
       return((sword)0);
     else
       return((sword)1);
}


yogiIiopProf *
ydbrDuplicateProf(yogiIiopProf *prof)
{
     yogiIiopProf    *nprof;

     if(!prof)
        return(NULLP(yogiIiopProf));

     nprof = (yogiIiopProf *) ysmGlbAlloc(sizeof(yogiIiopProf), "prof");
     CLRSTRUCT(*nprof);
     nprof->ver_major = prof->ver_major;
     nprof->ver_minor = prof->ver_minor;
     nprof->port = prof->port;
     nprof->host = (char *) ysmGlbAlloc(strlen(prof->host) + 1, "host");
     DISCARD strcpy(nprof->host, prof->host);
     if(prof->objkey)
     {
         nprof->objkey = (yogiOctSeq *) ysmGlbAlloc(sizeof(yogiOctSeq) +
                                    prof->objkey->len - 1, "objkey");
         nprof->objkey->len = prof->objkey->len;
         DISCARD memcpy(nprof->objkey->data, prof->objkey->data, 
                        prof->objkey->len);
     }
     else
       nprof->objkey = 0;

    return(nprof);
}


sword 
ydbrCmpProf(yogiIiopProf  *prof1, yogiIiopProf *prof2)
{
     if(prof1->ver_major == prof2->ver_major &&
        prof1->ver_minor == prof2->ver_minor &&
        prof1->port == prof2->port &&
        !strcmp(prof1->host, prof2->host))
     {
        if(!prof1->objkey && !prof2->objkey)
              return((sword) 1);

        if(prof1->objkey)
        {  
            if(prof2->objkey && 
                              !memcmp(prof1->objkey->data, 
                               prof2->objkey->data, prof1->objkey->len))  
            {
               return((sword) 1);  
            }
        }
     }
     return((sword) 0);
}

void
ydbrScanf(char *buf,  char *host, ub2 *port)
{
    char    *buf1;
    char    *ptr;

    buf1 = (char *) ysmGlbAlloc(strlen(buf) + 1, "buf");
    DISCARD strcpy(buf1, buf);

    ptr = ydbrFindString(buf1, "HOST=");
    if(ptr)
        DISCARD strcpy(host, ptr);

    ptr = ydbrFindString(buf, "PORT=");
    if(ptr)
        *port = atoi(ptr); 

    ysmGlbFree(buf1);
}

char *
ydbrFindString(char *buf, char *find)
{
 
  char   *ptr;

  ptr = strtok(buf, "(");
  while(ptr)
  {
    if(!strncmp(ptr, find, strlen(find)))
    {
         char    *ptr1;
         ptr+= strlen(find);
 
         ptr1 = strchr(ptr, ')');
         ptr[ptr1 - ptr] = '\0';
         break;
    }
    else
       ptr = strtok(NULL, "(");
  }
  return(ptr);
}

ub4 
yoProfHash(CONST dvoid *prof, size_t dummy, ub4 max)
{
  ub4              hash = 0;
  yogiIiopProf     *p = (yogiIiopProf *)prof;
  ub1              *cp;
  ub4              i;

  if(!prof)
    return(hash);

  hash += p->ver_major;
  hash += p->ver_minor;

  for( cp = (ub1 *)p->host; *cp; cp++)
      hash += *cp;

  hash += p->port;

  hash += p->objkey->len;
  for (cp = p->objkey->data, i = 0; i < p->objkey->len; cp++, i++)
    hash += *cp;

  return(hash % max);
}


void
ydbrSendCloseConn(ydbriep *ep)
{

   char          msg[GIOP_HDR_LEN];
   sword         ret, rlen = GIOP_HDR_LEN;
   

   msg[0] = 'G';
   msg[1] = 'I';
   msg[2] = 'O';
   msg[3] = 'P';
   msg[4] = 1;
   msg[5] = 0;
   msg[6] = YDBR_MY_BYTE_SEX; 
   msg[7] = ydbr_CloseConnection;
   msg[8] = msg[9] = msg[10] = msg[11] = 0;

   ret = nssend(ep->cxd, NSWDATA, msg, &rlen, 0);
   if(ret < 0)
   {
       
       ysRecord(YDBR_ERROR(25), YSLSEV_ERR, (char *)0, 
                YSLSTR("ydbrSendCloseConn()"), 
                YSLSTR("nssend()"), 
                YSLSB4(ep->cxd->nscxdres.nsresns), YSLEND);
   }
}



ydbr_Stats 
ydbr_GetStatistics_i( ydbr or, yoenv* ev)
{
     ydbrctx         *ctx;
     ydbr_ConnInfo   *cinfo;
     ub4             i;
     ydbriep         *ep;
     ydbr_Stats      stats;

     ctx = (ydbrctx *) yoGetImplState(or);
     ysmCheck(ctx, ydbrTagCtx);

     stats.TotalConn = ctx->ccount; 
     stats.TotalErrs = ctx->ecount;
     stats.CurrConn = ctx->nfds;

     stats.ConnStats._maximum = stats.ConnStats._length = ctx->nfds;

     stats.ConnStats._buffer = 
             (ydbr_ConnInfo *) ysmGlbAlloc(sizeof(ydbr_ConnInfo) * ctx->nfds,
                                           "Connection info");
     
        
     ep = ctx->eplist->next;
     for( i = 0; i < ctx->nfds, ep; i++)
     {
           cinfo = &(stats.ConnStats._buffer[i]);

           cinfo->LastMsg = ep->LastMsg;
           cinfo->LastUsed = ep->LastUsed;
           cinfo->mcount = ep->mcount; 
           cinfo->ErrCount = ep->ErrCount;
           cinfo->host = (char *) ysmGlbAlloc(strlen(ep->host) + 1, "host");
           strcpy(cinfo->host, ep->host);
  
           ep = ep->next;
     }

     return(stats); 
}
