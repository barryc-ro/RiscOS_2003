/* GENERATED FILE
 * mtut - public declarations
 * from mtut.idl
 */

#ifndef MTUT_IDL
#define MTUT_IDL

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

EXTC_START

/**********  SEQUENCE DECLARATIONS *********/
#ifndef YCIDL_sequence_CORBA_Object_DEFINED
#define YCIDL_sequence_CORBA_Object_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  CORBA_Object* _buffer;
} YCIDL_sequence_CORBA_Object;
#ifndef YCIDL_sequence_CORBA_Object_SUPP_FUNCS
#define YCIDL_sequence_CORBA_Object_SUPP_FUNCS
#endif /* YCIDL_sequence_CORBA_Object_SUPP_FUNCS */

#endif /* YCIDL_sequence_CORBA_Object_DEFINED */


/* interface mtut_grab declarations */
#ifndef mtut_grab_DECLARED
#define mtut_grab_DECLARED
typedef struct YCmtut_grab* mtut_grab;
yotk* mtut_grab__getTC(void);
#ifndef YCTC_mtut_grab
#define YCTC_mtut_grab   (mtut_grab__getTC())
#endif
#endif /* mtut_grab_DECLARED */

#ifndef mtut_grab_SUPP_FUNCS
#define mtut_grab_SUPP_FUNCS
void mtut_grab__free( mtut_grab* val, ysmff ffunc);
void mtut_grab__copy( mtut_grab* dest, mtut_grab* src, ysmaf afunc);
#endif /* mtut_grab_SUPP_FUNCS */

#ifndef YCIDL_sequence_mtut_grab_DEFINED
#define YCIDL_sequence_mtut_grab_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mtut_grab* _buffer;
} YCIDL_sequence_mtut_grab;
#ifndef YCIDL_sequence_mtut_grab_SUPP_FUNCS
#define YCIDL_sequence_mtut_grab_SUPP_FUNCS
#endif /* YCIDL_sequence_mtut_grab_SUPP_FUNCS */

#endif /* YCIDL_sequence_mtut_grab_DEFINED */

/******* NON-SEQUENCE DECLARATIONS *******/

/* interface yoevt declarations */
#ifndef yoevt_DECLARED
#define yoevt_DECLARED
typedef struct YCyoevt* yoevt;
yotk* yoevt__getTC(void);
#ifndef YCTC_yoevt
#define YCTC_yoevt   (yoevt__getTC())
#endif
#endif /* yoevt_DECLARED */

#ifndef yoevt_SUPP_FUNCS
#define yoevt_SUPP_FUNCS
void yoevt__free( yoevt* val, ysmff ffunc);
void yoevt__copy( yoevt* dest, yoevt* src, ysmaf afunc);
#endif /* yoevt_SUPP_FUNCS */

#ifndef yostd_stringList_DECLARED
#define yostd_stringList_DECLARED
typedef YCIDL_sequence_string yostd_stringList;
yotk* yostd_stringList__getTC(void);
#ifndef YCTC_yostd_stringList
#define YCTC_yostd_stringList   (yostd_stringList__getTC())
#endif
#endif /* yostd_stringList_DECLARED */

#ifndef yostd_stringList_SUPP_FUNCS
#define yostd_stringList_SUPP_FUNCS
void yostd_stringList__free( yostd_stringList* val, ysmff ffunc);
void yostd_stringList__copy( yostd_stringList* dest, yostd_stringList* src, 
  ysmaf afunc);
#endif /* yostd_stringList_SUPP_FUNCS */

#ifndef yostd_objList_DECLARED
#define yostd_objList_DECLARED
typedef YCIDL_sequence_CORBA_Object yostd_objList;
yotk* yostd_objList__getTC(void);
#ifndef YCTC_yostd_objList
#define YCTC_yostd_objList   (yostd_objList__getTC())
#endif
#endif /* yostd_objList_DECLARED */

#ifndef yostd_objList_SUPP_FUNCS
#define yostd_objList_SUPP_FUNCS
void yostd_objList__free( yostd_objList* val, ysmff ffunc);
void yostd_objList__copy( yostd_objList* dest, yostd_objList* src, ysmaf 
  afunc);
#endif /* yostd_objList_SUPP_FUNCS */

#ifndef yostd_octetSeq_DECLARED
#define yostd_octetSeq_DECLARED
typedef YCIDL_sequence_ub1 yostd_octetSeq;
yotk* yostd_octetSeq__getTC(void);
#ifndef YCTC_yostd_octetSeq
#define YCTC_yostd_octetSeq   (yostd_octetSeq__getTC())
#endif
#endif /* yostd_octetSeq_DECLARED */

#ifndef yostd_octetSeq_SUPP_FUNCS
#define yostd_octetSeq_SUPP_FUNCS
void yostd_octetSeq__free( yostd_octetSeq* val, ysmff ffunc);
void yostd_octetSeq__copy( yostd_octetSeq* dest, yostd_octetSeq* src, ysmaf 
  afunc);
#endif /* yostd_octetSeq_SUPP_FUNCS */

#ifndef mtut_slaveExc_DECLARED
#define mtut_slaveExc_DECLARED
typedef ub4 mtut_slaveExc;
yotk* mtut_slaveExc__getTC(void);
#ifndef YCTC_mtut_slaveExc
#define YCTC_mtut_slaveExc   (mtut_slaveExc__getTC())
#endif
#endif /* mtut_slaveExc_DECLARED */

#ifndef mtut_slaveExc_DEFINED
#define mtut_slaveExc_DEFINED
#define mtut_slaveOutMem ((mtut_slaveExc) 0)
#define mtut_slaveBadParam ((mtut_slaveExc) 1)
#define mtut_slaveChillOut ((mtut_slaveExc) 2)
#define mtut_slaveJustDontLikeYou ((mtut_slaveExc) 3)
#endif /* mtut_slaveExc_DEFINED */

#ifndef mtut_slaveExc_SUPP_FUNCS
#define mtut_slaveExc_SUPP_FUNCS
void mtut_slaveExc__free( mtut_slaveExc* val, ysmff ffunc);
void mtut_slaveExc__copy( mtut_slaveExc* dest, mtut_slaveExc* src, ysmaf 
  afunc);
#endif /* mtut_slaveExc_SUPP_FUNCS */

#ifndef MTUT_EX_SLAVE_DECLARED
#define MTUT_EX_SLAVE_DECLARED
CONST ysid* mtut_slave__getId(void);
#ifndef MTUT_EX_SLAVE
#define MTUT_EX_SLAVE   (mtut_slave__getId())
#endif
#endif /* MTUT_EX_SLAVE_DECLARED */

#ifndef mtut_slave_DECLARED
#define mtut_slave_DECLARED
typedef struct mtut_slave mtut_slave;
#endif /* mtut_slave_DECLARED */

#ifndef mtut_slave_DEFINED
#define mtut_slave_DEFINED
struct mtut_slave
{
  mtut_slaveExc failType;
};
yotk* mtut_slave__getTC(void);
#ifndef YCTC_mtut_slave
#define YCTC_mtut_slave   (mtut_slave__getTC())
#endif
#endif /* mtut_slave_DEFINED */

#ifndef mtut_slave_SUPP_FUNCS
#define mtut_slave_SUPP_FUNCS
void mtut_slave__free( mtut_slave* val, ysmff ffunc);
void mtut_slave__copy( mtut_slave* dest, mtut_slave* src, ysmaf afunc);
#endif /* mtut_slave_SUPP_FUNCS */

#ifndef mtut_fmt_DECLARED
#define mtut_fmt_DECLARED
typedef ub4 mtut_fmt;
yotk* mtut_fmt__getTC(void);
#ifndef YCTC_mtut_fmt
#define YCTC_mtut_fmt   (mtut_fmt__getTC())
#endif
#endif /* mtut_fmt_DECLARED */

#ifndef mtut_fmt_DEFINED
#define mtut_fmt_DEFINED
#define mtut_fmtError ((mtut_fmt) 0)
#define mtut_fmtMpeg1SS ((mtut_fmt) 1)
#define mtut_fmtMpeg2T ((mtut_fmt) 2)
#define mtut_fmtAVI ((mtut_fmt) 3)
#endif /* mtut_fmt_DEFINED */

#ifndef mtut_fmt_SUPP_FUNCS
#define mtut_fmt_SUPP_FUNCS
void mtut_fmt__free( mtut_fmt* val, ysmff ffunc);
void mtut_fmt__copy( mtut_fmt* dest, mtut_fmt* src, ysmaf afunc);
#endif /* mtut_fmt_SUPP_FUNCS */

#ifndef mtut_m1ssGuts_DECLARED
#define mtut_m1ssGuts_DECLARED
typedef struct mtut_m1ssGuts mtut_m1ssGuts;
#endif /* mtut_m1ssGuts_DECLARED */

#ifndef mtut_m1ssGuts_DEFINED
#define mtut_m1ssGuts_DEFINED
struct mtut_m1ssGuts
{
  ub1 nothing;
};
yotk* mtut_m1ssGuts__getTC(void);
#ifndef YCTC_mtut_m1ssGuts
#define YCTC_mtut_m1ssGuts   (mtut_m1ssGuts__getTC())
#endif
#endif /* mtut_m1ssGuts_DEFINED */

#ifndef mtut_m1ssGuts_SUPP_FUNCS
#define mtut_m1ssGuts_SUPP_FUNCS
void mtut_m1ssGuts__free( mtut_m1ssGuts* val, ysmff ffunc);
void mtut_m1ssGuts__copy( mtut_m1ssGuts* dest, mtut_m1ssGuts* src, ysmaf 
  afunc);
#endif /* mtut_m1ssGuts_SUPP_FUNCS */

#ifndef mtut_m2tGuts_DECLARED
#define mtut_m2tGuts_DECLARED
typedef struct mtut_m2tGuts mtut_m2tGuts;
#endif /* mtut_m2tGuts_DECLARED */

#ifndef mtut_m2tGuts_DEFINED
#define mtut_m2tGuts_DEFINED
struct mtut_m2tGuts
{
  ub1 nothing;
};
yotk* mtut_m2tGuts__getTC(void);
#ifndef YCTC_mtut_m2tGuts
#define YCTC_mtut_m2tGuts   (mtut_m2tGuts__getTC())
#endif
#endif /* mtut_m2tGuts_DEFINED */

#ifndef mtut_m2tGuts_SUPP_FUNCS
#define mtut_m2tGuts_SUPP_FUNCS
void mtut_m2tGuts__free( mtut_m2tGuts* val, ysmff ffunc);
void mtut_m2tGuts__copy( mtut_m2tGuts* dest, mtut_m2tGuts* src, ysmaf afunc)
  ;
#endif /* mtut_m2tGuts_SUPP_FUNCS */

#ifndef mtut_aviGuts_DECLARED
#define mtut_aviGuts_DECLARED
typedef struct mtut_aviGuts mtut_aviGuts;
#endif /* mtut_aviGuts_DECLARED */

#ifndef mtut_aviGuts_DEFINED
#define mtut_aviGuts_DEFINED
struct mtut_aviGuts
{
  ub1 nothing;
};
yotk* mtut_aviGuts__getTC(void);
#ifndef YCTC_mtut_aviGuts
#define YCTC_mtut_aviGuts   (mtut_aviGuts__getTC())
#endif
#endif /* mtut_aviGuts_DEFINED */

#ifndef mtut_aviGuts_SUPP_FUNCS
#define mtut_aviGuts_SUPP_FUNCS
void mtut_aviGuts__free( mtut_aviGuts* val, ysmff ffunc);
void mtut_aviGuts__copy( mtut_aviGuts* dest, mtut_aviGuts* src, ysmaf afunc)
  ;
#endif /* mtut_aviGuts_SUPP_FUNCS */

#ifndef mtut_specifics_DECLARED
#define mtut_specifics_DECLARED
typedef struct mtut_specifics mtut_specifics;
#endif /* mtut_specifics_DECLARED */

#ifndef mtut_specifics_DEFINED
#define mtut_specifics_DEFINED
struct mtut_specifics
{
  mtut_fmt _d;
  union
  {
    mtut_m1ssGuts m1ss;
    mtut_m2tGuts m2t;
    mtut_aviGuts avi;
  } _u;
};
yotk* mtut_specifics__getTC(void);
#ifndef YCTC_mtut_specifics
#define YCTC_mtut_specifics   (mtut_specifics__getTC())
#endif
#endif /* mtut_specifics_DEFINED */

#ifndef mtut_specifics_SUPP_FUNCS
#define mtut_specifics_SUPP_FUNCS
void mtut_specifics__free( mtut_specifics* val, ysmff ffunc);
void mtut_specifics__copy( mtut_specifics* dest, mtut_specifics* src, ysmaf 
  afunc);
#endif /* mtut_specifics_SUPP_FUNCS */

#ifndef mtut_header_DECLARED
#define mtut_header_DECLARED
typedef struct mtut_header mtut_header;
#endif /* mtut_header_DECLARED */

#ifndef mtut_header_DEFINED
#define mtut_header_DEFINED
struct mtut_header
{
  ub4 bitsPerSec;
  ub2 heightInPixels;
  ub2 widthInPixels;
  sb4 pelAspectRatio;
  ub4 framesPerKiloSec;
  sysb8 lastTimeSeen;
  mtut_specifics also;
  YCIDL_sequence_ub1 cmpData;
};
yotk* mtut_header__getTC(void);
#ifndef YCTC_mtut_header
#define YCTC_mtut_header   (mtut_header__getTC())
#endif
#endif /* mtut_header_DEFINED */

#ifndef mtut_header_SUPP_FUNCS
#define mtut_header_SUPP_FUNCS
void mtut_header__free( mtut_header* val, ysmff ffunc);
void mtut_header__copy( mtut_header* dest, mtut_header* src, ysmaf afunc);
#endif /* mtut_header_SUPP_FUNCS */

#ifndef mtut_tagList_DECLARED
#define mtut_tagList_DECLARED
typedef YCIDL_sequence_ub1 mtut_tagList;
yotk* mtut_tagList__getTC(void);
#ifndef YCTC_mtut_tagList
#define YCTC_mtut_tagList   (mtut_tagList__getTC())
#endif
#endif /* mtut_tagList_DECLARED */

#ifndef mtut_tagList_SUPP_FUNCS
#define mtut_tagList_SUPP_FUNCS
void mtut_tagList__free( mtut_tagList* val, ysmff ffunc);
void mtut_tagList__copy( mtut_tagList* dest, mtut_tagList* src, ysmaf afunc)
  ;
#endif /* mtut_tagList_SUPP_FUNCS */

#ifndef mtut_state_DECLARED
#define mtut_state_DECLARED
typedef ub4 mtut_state;
yotk* mtut_state__getTC(void);
#ifndef YCTC_mtut_state
#define YCTC_mtut_state   (mtut_state__getTC())
#endif
#endif /* mtut_state_DECLARED */

#ifndef mtut_state_DEFINED
#define mtut_state_DEFINED
#define mtut_stateUnknown ((mtut_state) 0)
#define mtut_stateIdle ((mtut_state) 1)
#define mtut_stateElHosed ((mtut_state) 2)
#define mtut_stateWorking ((mtut_state) 3)
#endif /* mtut_state_DEFINED */

#ifndef mtut_state_SUPP_FUNCS
#define mtut_state_SUPP_FUNCS
void mtut_state__free( mtut_state* val, ysmff ffunc);
void mtut_state__copy( mtut_state* dest, mtut_state* src, ysmaf afunc);
#endif /* mtut_state_SUPP_FUNCS */


/* interface mtut_grab declarations */
#ifndef mtut_grab_DECLARED
#define mtut_grab_DECLARED
typedef struct YCmtut_grab* mtut_grab;
yotk* mtut_grab__getTC(void);
#ifndef YCTC_mtut_grab
#define YCTC_mtut_grab   (mtut_grab__getTC())
#endif
#endif /* mtut_grab_DECLARED */

#ifndef mtut_grab_SUPP_FUNCS
#define mtut_grab_SUPP_FUNCS
void mtut_grab__free( mtut_grab* val, ysmff ffunc);
void mtut_grab__copy( mtut_grab* dest, mtut_grab* src, ysmaf afunc);
#endif /* mtut_grab_SUPP_FUNCS */

#ifndef mtut_grab_DEFINED
#define mtut_grab_DEFINED
struct yostub* mtut_grab__getStubs(void);
#define mtut_grab__stubs (mtut_grab__getStubs())
#endif /* mtut_grab_DEFINED */

CONST ysid* mtut_grab__getId(void);
#ifndef mtut_grab__id
#define mtut_grab__id   (mtut_grab__getId())
#endif
void mtut_grab_porQua( mtut_grab or, yoenv* ev, mtut_state* status);
void mtut_grab_porQua_nw( mtut_grab or, yoenv* ev, mtut_state* status, 
  ysevt* _sem);
yopar* mtut_grab_porQua__getPars(void);
#ifndef mtut_grab_porQua_pars
#define mtut_grab_porQua_pars (mtut_grab_porQua__getPars())
#endif

void mtut_grab_tags( mtut_grab or, yoenv* ev, mtut_header* hdr, 
  mtut_tagList* tags);
void mtut_grab_tags_nw( mtut_grab or, yoenv* ev, mtut_header* hdr, 
  mtut_tagList* tags, ysevt* _sem);
yopar* mtut_grab_tags__getPars(void);
#ifndef mtut_grab_tags_pars
#define mtut_grab_tags_pars (mtut_grab_tags__getPars())
#endif

#ifndef mtut_grab__tyimpl_DEFINED
#define mtut_grab__tyimpl_DEFINED
struct mtut_grab__tyimpl
{
  void (*porQua)( mtut_grab, yoenv*, mtut_state*);
  void (*tags)( mtut_grab, yoenv*, mtut_header*, mtut_tagList*);
};
#endif /* mtut_grab__tyimpl_DEFINED */

#ifndef mtut_grabList_DECLARED
#define mtut_grabList_DECLARED
typedef YCIDL_sequence_mtut_grab mtut_grabList;
yotk* mtut_grabList__getTC(void);
#ifndef YCTC_mtut_grabList
#define YCTC_mtut_grabList   (mtut_grabList__getTC())
#endif
#endif /* mtut_grabList_DECLARED */

#ifndef mtut_grabList_SUPP_FUNCS
#define mtut_grabList_SUPP_FUNCS
void mtut_grabList__free( mtut_grabList* val, ysmff ffunc);
void mtut_grabList__copy( mtut_grabList* dest, mtut_grabList* src, ysmaf 
  afunc);
#endif /* mtut_grabList_SUPP_FUNCS */


/* interface mtut_parse declarations */
#ifndef mtut_parse_DECLARED
#define mtut_parse_DECLARED
typedef struct YCmtut_parse* mtut_parse;
yotk* mtut_parse__getTC(void);
#ifndef YCTC_mtut_parse
#define YCTC_mtut_parse   (mtut_parse__getTC())
#endif
#endif /* mtut_parse_DECLARED */

#ifndef mtut_parse_SUPP_FUNCS
#define mtut_parse_SUPP_FUNCS
void mtut_parse__free( mtut_parse* val, ysmff ffunc);
void mtut_parse__copy( mtut_parse* dest, mtut_parse* src, ysmaf afunc);
#endif /* mtut_parse_SUPP_FUNCS */

#ifndef mtut_parse_DEFINED
#define mtut_parse_DEFINED
struct yostub* mtut_parse__getStubs(void);
#define mtut_parse__stubs (mtut_parse__getStubs())
#endif /* mtut_parse_DEFINED */

CONST ysid* mtut_parse__getId(void);
#ifndef mtut_parse__id
#define mtut_parse__id   (mtut_parse__getId())
#endif
void mtut_parse_doIt( mtut_parse or, yoenv* ev, char* srcFileName, char* 
  cntOutName, ub4 frameTypes, boolean ignoreError, sysb8 startOffset, sysb8 
  endOffset, yoevt completionEvt, mtut_grab* resultOR);
void mtut_parse_doIt_nw( mtut_parse or, yoenv* ev, char* srcFileName, char* 
  cntOutName, ub4 frameTypes, boolean ignoreError, sysb8 startOffset, sysb8 
  endOffset, yoevt completionEvt, mtut_grab* resultOR, ysevt* _sem);
yopar* mtut_parse_doIt__getPars(void);
#ifndef mtut_parse_doIt_pars
#define mtut_parse_doIt_pars (mtut_parse_doIt__getPars())
#endif

void mtut_parse_whazzup( mtut_parse or, yoenv* ev, mtut_state* status, 
  mtut_grabList* working, mtut_grabList* pending);
void mtut_parse_whazzup_nw( mtut_parse or, yoenv* ev, mtut_state* status, 
  mtut_grabList* working, mtut_grabList* pending, ysevt* _sem);
yopar* mtut_parse_whazzup__getPars(void);
#ifndef mtut_parse_whazzup_pars
#define mtut_parse_whazzup_pars (mtut_parse_whazzup__getPars())
#endif

void mtut_parse_quit( mtut_parse or, yoenv* ev);
void mtut_parse_quit_nw( mtut_parse or, yoenv* ev, ysevt* _sem);
yopar* mtut_parse_quit__getPars(void);
#ifndef mtut_parse_quit_pars
#define mtut_parse_quit_pars (mtut_parse_quit__getPars())
#endif

#ifndef mtut_parse__tyimpl_DEFINED
#define mtut_parse__tyimpl_DEFINED
struct mtut_parse__tyimpl
{
  void (*doIt)( mtut_parse, yoenv*, char*, char*, ub4, boolean, sysb8, 
    sysb8, yoevt, mtut_grab*);
  void (*whazzup)( mtut_parse, yoenv*, mtut_state*, mtut_grabList*, 
    mtut_grabList*);
  void (*quit)( mtut_parse, yoenv*);
};
#endif /* mtut_parse__tyimpl_DEFINED */


/* interface mtut_mstr declarations */
#ifndef mtut_mstr_DECLARED
#define mtut_mstr_DECLARED
typedef struct YCmtut_mstr* mtut_mstr;
yotk* mtut_mstr__getTC(void);
#ifndef YCTC_mtut_mstr
#define YCTC_mtut_mstr   (mtut_mstr__getTC())
#endif
#endif /* mtut_mstr_DECLARED */

#ifndef mtut_mstr_SUPP_FUNCS
#define mtut_mstr_SUPP_FUNCS
void mtut_mstr__free( mtut_mstr* val, ysmff ffunc);
void mtut_mstr__copy( mtut_mstr* dest, mtut_mstr* src, ysmaf afunc);
#endif /* mtut_mstr_SUPP_FUNCS */

#ifndef mtut_mstr_DEFINED
#define mtut_mstr_DEFINED
struct yostub* mtut_mstr__getStubs(void);
#define mtut_mstr__stubs (mtut_mstr__getStubs())
#endif /* mtut_mstr_DEFINED */

CONST ysid* mtut_mstr__getId(void);
#ifndef mtut_mstr__id
#define mtut_mstr__id   (mtut_mstr__getId())
#endif
void mtut_mstr_hello( mtut_mstr or, yoenv* ev, mtut_parse slaveOR, yoevt* 
  activated);
void mtut_mstr_hello_nw( mtut_mstr or, yoenv* ev, mtut_parse slaveOR, 
  yoevt* activated, ysevt* _sem);
yopar* mtut_mstr_hello__getPars(void);
#ifndef mtut_mstr_hello_pars
#define mtut_mstr_hello_pars (mtut_mstr_hello__getPars())
#endif

#ifndef mtut_mstr__tyimpl_DEFINED
#define mtut_mstr__tyimpl_DEFINED
struct mtut_mstr__tyimpl
{
  void (*hello)( mtut_mstr, yoenv*, mtut_parse, yoevt*);
};
#endif /* mtut_mstr__tyimpl_DEFINED */

EXTC_END
#endif /* MTUT_IDL */
