/* mx/src/yd/ydbr.h */


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
# define YDBR_ORACLE

#ifndef NSI
# include <nsi.h>
#endif

#ifndef NL_ORACLE
# include <nl.h>
#endif

#ifdef WIN32COMMON
#ifndef SNS
# include <sns.h>
#endif
#endif 

#ifndef YS_ORACLE
# include <ys.h>
#endif

#ifndef YEEV_ORACLE
# include <yeev.h>
#endif

#ifndef SYSXCD_ORACLE
# include <sysxcd.h>
#endif

#ifndef SSYSI_ORACLE
# include <ssysi.h>
#endif

#ifndef YOBR_ORACLE
# include <yobr.h>
#endif

#ifndef YTEX_ORACLE
# include <ytex.h>
#endif

#ifndef YOCOA_ORACLE
# include <yocoa.h>
#endif

#ifndef YDBRI_IDL
#include <ydbri.h>
#endif

#ifdef NS_TRACE
#define YDBR_PFILENAME "sqlnet"
#define YDBR_PFILENAMEL sizeof(YDBR_PFILENAME) - 1
#define YDBR_TFILENAME "ydbr_ns_trace"
#define YDBR_TFILENAMEL sizeof(YDBR_TFILENAME) - 1
#define YDBR_TRCDIRNAME "ydbr.trace_directory"
#define YDBR_TRCLEVNAME "ydbr.trace_level"
#endif

enum { GIOP_MY_MAJOR = 1, GIOP_MY_MINOR = 0 };
 


typedef struct {ub1 major, minor; } ydbrGiopVer;
typedef struct {
     ub1            magic[4];    
     ydbrGiopVer    giop_version;
     ub1            byte_order;   
     ub1            message_type; 
     ub4            message_size;
} ydbrGiopMsgHdr;


#define GIOP_HDR_LEN   12


#define YDBRPORT (ub2) 7777


#define YDBR_DEF_MAX_CONNS  32

#define YDBR_IDLE_TIME 600000000 
#define YDBR_NO_DELAY 0
#define YDBRALL (ub2) 0
#define YDBRONE (ub2) 1
#define YDBR_MAX_REQUEST_ID  4294967295U
#define YDBR_TNS_STRING_SIZ  256
#define YDBR_HOST_STRING_SIZ 64 
#define YDBR_CLEANUP_INTERVAL 600000000   
#define YDBR_FAC "YDBR"

#define YDBR_BASE 10000
#define YDBR_DEBUG_BASE 10250
#define YDBR_TRACE_BASE 10300
#define YDBR_INFO_BASE 10500
#define YDBR_ERROR(n) YS_PRODUCT, YDBR_FAC, (YDBR_BASE + (ub4) n)
#define YDBR_DEBUG(n) YS_PRODUCT, YDBR_FAC, (YDBR_DEBUG_BASE + (ub4) n)
#define YDBR_INFO(n)  YS_PRODUCT, YDBR_FAC, (YDBR_INFO_BASE + (ub4) n)
#define YDBR_TRCINFO(n) YS_PRODUCT, YDBR_FAC, (YDBR_TRACE_BASE + (ub4) n)

#define YDBRLOW_EP_MARK  2
externref ysidDecl(YDBR_EX_NS_INIT_FAILED);
externref ysidDecl(YDBR_EX_NS_FAILED);
externref ysidDecl(YDBR_EX_INTERNAL_ERROR);
externref ysidDecl(YDBR_EX_CLOSE_CONNECTION);
externref ysidDecl(YDBR_EX_MESSAGE_ERROR);
externref ysidDecl(YDBR_EX_COMM_FAILED);


typedef struct ydbrmsg ydbrmsg;
typedef struct ydbrReqMap ydbrReqMap;
typedef struct ydbriep ydbriep;
typedef struct ydbrctx ydbrctx;



struct ydbriep 
{
    snshdl          fd;                                        
    ub2             flags;     
#define YDBRIEP_DATA  0x0001                              
#define YDBRIEP_CONN  0x0002                        
#define YDBRIEP_LSTN  0x0004                         
#define YDBRIEP_NOMSG 0x0008               
#define YDBRIEP_CXD_VALID 0x0010                      
#define YDBRIEP_MSG_PENDING  0x0020    
                                           
#define YDBRIEP_HEADER     0x0040     
#define YDBRIEP_PARTIAL_RECV 0x0080   
    nscxd           *cxd;                          
    sword           slot;                      
    ub2             port;   
    char            *host;                      
    ub2             state;                        
#define YDBRIEP_WAIT2READ    0x0001            
#define YDBRIEP_WAIT2WRITE   0x0002           
#define YDBRIEP_READY2READ   0x0004                        
#define YDBRIEP_READY2WRITE  0x0008                       
#define YDBRIEP_NONE         0x0000                          
    ydbrmsg           *sendq;            
    ydbrmsg           *recvq;          
    ub2             sendqlen;                
    ub2             recvqlen;                
    struct ydbriep  *next;                              
    struct ydbriep  *prev;                              
    
    ydbr_GiopMsgType LastMsg;       
    sysb8           LastUsed;                      
    ydbr_MsgCount   mcount;             
    ub4             ErrCount;       
};


struct ydbrReqMap
{
    ub2                 flags;
#define      YDBRREQ_HDLR_ACTIVE     0x0001   
#define      YDBRREQ_FREE_REQ        0x0002    
    ydbriep             *ep; 
    dvoid               *mn_addr;
    ub4                 orig_reqid;
    ub4                 mapped_reqid;
    ub2                 stage;
    ydbrctx             *ctx;
    ydbrmsg             *msg;   
    ydbrmsg             *current;  
};

struct ydbrctx 
{
   CONST char *hostname;
   CONST char *pid;
   ub2      flags;                                       
#define YDBRCLOSECONN_ONERROR    0x0001   
#define YDBR_DONE_EVREG 0x0002           
#define YDBRCLOSEIDLE_CONN  0x0004            
   ub2      nsidle;    
   ysque    *evtq;       
   ub2      port;                               
   char     *my_iiop_addr;       
   dvoid    *osdp;                   
   ysevt    *revt;                                   
   ysmhp    *heap; 
   ub2      threaded;                          
   dvoid    *npdgbl;                                        
   dvoid    *nsgbl;                                          
   nscnd    *my_tns_addr;                    
   nsind    *ind;                         
   ydbriep  *lfd;                          
   ub2      maxfds;                            
   ub2      nfds;                                      
   ydbriep  *eplist;                               
   yeev     yev;                                
   ub4      request_id;                           
   sysb8      idletime;               
   sysb8      cinterval;              
   yobrctx    *yobrctx;              
   yshsh      *reqhash;                     
   yshsh      *profhash;                   
   
   ub4      ccount;              
   ub4      ecount;                   
};


struct ydbrmsg 
{
   ub2                 flags;          
#define YDBRMSG_PARTIAL_HDR   0x0001           
#define YDBRMSG_PARTIAL_MSG   0x0002          
#define YDBRMSG_HEADER        0x0004                
#define YDBRMSG_YOBRALLOC     0x0008        
#define YDBRMSG_ABV           0x0010
   sb4                 alen;                     
   sb4                 rlen;               
   yobrMsg             yobrmsg;                           
#define ydbrbv    yobrmsg.bv
#define ydbrnbv   yobrmsg.nbv
#define from_addr  yobrmsg.from
   ydbrctx             *ctx;                            
   ysbv                *abv;                   
   sword               anbv;                                
   yogiIiopProf        *prof;           
   yogiIiopProf        *org_prof;  
   ydbrGiopMsgHdr      *hdr;              
   dvoid               *objref;  
   ub1                 *tmpbuf;
   ydbrReqMap          *req;   
                                                 
   struct ydbrmsg      *next;                    
   struct ydbrmsg      *prev;                    
}; 

typedef struct
{
     ub2             flags;
     yogiIiopProf    *orig_prof;
     yogiIiopProf    *fwd_prof;
} ydbrProf;

#define YDBR_REQUESTS_PENDING(ep) \
  (ep->mcount.Request - ep->mcount.Reply - ep->mcount.onewayRequests)



#define YDBRGET_MUTEX(ptr, ctx)



#define YDBRRELEASE_MUTEX(ptr, ctx) 



#define YDBRINSERT_MBR(mbrlist, newmbr) \
 { \
   if(mbrlist->next) \
   { \
      newmbr->next = mbrlist->next; \
      mbrlist->next->prev = newmbr; \
   } \
   else \
   { \
      newmbr->next = 0; \
   } \
   newmbr->prev = mbrlist; \
   mbrlist->next = newmbr; \
 }

#define YDBRINSERT_EP(eplist, newep) YDBRINSERT_MBR(eplist, newep) 

#define YDBRINSERT_MSG(msgq, newmsg) \
  { \
     ydbrmsg   *list = msgq; \
     while(list->next) \
        list = list->next; \
     list->next = newmsg; \
     newmsg->next = NULLP(ydbrmsg); \
     newmsg->prev = list; \
  }



#define YDBRDEQUE_MBR(mbr) \
 { \
    mbr->prev->next = mbr->next; \
    if(mbr->next) \
      mbr->next->prev = mbr->prev; \
 }

#define YDBRDEQUE_EP(ep) YDBRDEQUE_MBR(ep) 
#define YDBRDEQUE_MSG(msg) YDBRDEQUE_MBR(msg)



#define YDBR_MY_BYTE_SEX SYSX_BYTE_ORDER   

#endif   
