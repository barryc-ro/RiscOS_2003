/* GENERATED FILE
 * mkcex - public declarations
 * from mkcex.idl
 */

#ifndef MKCEX_IDL
#define MKCEX_IDL

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

EXTC_START

/******* NON-SEQUENCE DECLARATIONS *******/
#ifndef mkc_exceptionType_DECLARED
#define mkc_exceptionType_DECLARED
typedef ub4 mkc_exceptionType;
yotk* mkc_exceptionType__getTC(void);
#ifndef YCTC_mkc_exceptionType
#define YCTC_mkc_exceptionType   (mkc_exceptionType__getTC())
#endif
#endif /* mkc_exceptionType_DECLARED */

#ifndef mkc_exceptionType_DEFINED
#define mkc_exceptionType_DEFINED
#define mkc_nonIndexed ((mkc_exceptionType) 0)
#define mkc_streamIncompatible ((mkc_exceptionType) 1)
#define mkc_fileOpenError ((mkc_exceptionType) 2)
#define mkc_outOfMemory ((mkc_exceptionType) 3)
#define mkc_badMember ((mkc_exceptionType) 4)
#define mkc_badContext ((mkc_exceptionType) 5)
#define mkc_noSuchMember ((mkc_exceptionType) 6)
#define mkc_overEnd ((mkc_exceptionType) 7)
#define mkc_cacheError ((mkc_exceptionType) 8)
#define mkc_badPosition ((mkc_exceptionType) 9)
#define mkc_badIndexing ((mkc_exceptionType) 10)
#define mkc_segmentError ((mkc_exceptionType) 11)
#define mkc_badFormat ((mkc_exceptionType) 12)
#define mkc_seqDone ((mkc_exceptionType) 13)
#define mkc_badTag ((mkc_exceptionType) 14)
#define mkc_badPresRate ((mkc_exceptionType) 15)
#define mkc_badRange ((mkc_exceptionType) 16)
#define mkc_badContent ((mkc_exceptionType) 17)
#define mkc_badFile ((mkc_exceptionType) 18)
#define mkc_serverInternal ((mkc_exceptionType) 19)
#define mkc_playFailure ((mkc_exceptionType) 20)
#define mkc_noRepos ((mkc_exceptionType) 21)
#define mkc_badOutput ((mkc_exceptionType) 22)
#define mkc_badInputArg ((mkc_exceptionType) 23)
#define mkc_badTagVersion ((mkc_exceptionType) 24)
#define mkc_badTagMagic ((mkc_exceptionType) 25)
#endif /* mkc_exceptionType_DEFINED */

#ifndef mkc_exceptionType_SUPP_FUNCS
#define mkc_exceptionType_SUPP_FUNCS
void mkc_exceptionType__free( mkc_exceptionType* val, ysmff ffunc);
void mkc_exceptionType__copy( mkc_exceptionType* dest, mkc_exceptionType* 
  src, ysmaf afunc);
#endif /* mkc_exceptionType_SUPP_FUNCS */

#ifndef MKC_EX_INTERNAL_DECLARED
#define MKC_EX_INTERNAL_DECLARED
CONST ysid* mkc_internal__getId(void);
#ifndef MKC_EX_INTERNAL
#define MKC_EX_INTERNAL   (mkc_internal__getId())
#endif
#endif /* MKC_EX_INTERNAL_DECLARED */

#ifndef mkc_internal_DECLARED
#define mkc_internal_DECLARED
typedef struct mkc_internal mkc_internal;
#endif /* mkc_internal_DECLARED */

#ifndef mkc_internal_DEFINED
#define mkc_internal_DEFINED
struct mkc_internal
{
  mkc_exceptionType failType;
};
yotk* mkc_internal__getTC(void);
#ifndef YCTC_mkc_internal
#define YCTC_mkc_internal   (mkc_internal__getTC())
#endif
#endif /* mkc_internal_DEFINED */

#ifndef mkc_internal_SUPP_FUNCS
#define mkc_internal_SUPP_FUNCS
void mkc_internal__free( mkc_internal* val, ysmff ffunc);
void mkc_internal__copy( mkc_internal* dest, mkc_internal* src, ysmaf afunc)
  ;
#endif /* mkc_internal_SUPP_FUNCS */

EXTC_END
#endif /* MKCEX_IDL */
