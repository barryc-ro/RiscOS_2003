/* GENERATED FILE
 * mzccp - public declarations
 * from /vobs/rts/src/inc/mzccp.idl
 */

#ifndef MZCCP_IDL
#define MZCCP_IDL

#ifndef SYSX_ORACLE
#include <sysx.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

EXTC_START

/**********  SEQUENCE DECLARATIONS *********/
#ifndef mzc_netproto_DECLARED
#define mzc_netproto_DECLARED
typedef struct mzc_netproto mzc_netproto;
#endif /* mzc_netproto_DECLARED */

#ifndef YCIDL_sequence_mzc_netproto_DEFINED
#define YCIDL_sequence_mzc_netproto_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_netproto* _buffer;
} YCIDL_sequence_mzc_netproto;
#ifndef YCIDL_sequence_mzc_netproto_SUPP_FUNCS
#define YCIDL_sequence_mzc_netproto_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_netproto_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_netproto_DEFINED */

#ifndef mzc_netapi_DECLARED
#define mzc_netapi_DECLARED
typedef struct mzc_netapi mzc_netapi;
#endif /* mzc_netapi_DECLARED */

#ifndef YCIDL_sequence_mzc_netapi_DEFINED
#define YCIDL_sequence_mzc_netapi_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_netapi* _buffer;
} YCIDL_sequence_mzc_netapi;
#ifndef YCIDL_sequence_mzc_netapi_SUPP_FUNCS
#define YCIDL_sequence_mzc_netapi_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_netapi_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_netapi_DEFINED */

#ifndef mzc_netif_DECLARED
#define mzc_netif_DECLARED
typedef struct mzc_netif mzc_netif;
#endif /* mzc_netif_DECLARED */

#ifndef YCIDL_sequence_mzc_netif_DEFINED
#define YCIDL_sequence_mzc_netif_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_netif* _buffer;
} YCIDL_sequence_mzc_netif;
#ifndef YCIDL_sequence_mzc_netif_SUPP_FUNCS
#define YCIDL_sequence_mzc_netif_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_netif_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_netif_DEFINED */

#ifndef mzc_pktinfo_DECLARED
#define mzc_pktinfo_DECLARED
typedef struct mzc_pktinfo mzc_pktinfo;
#endif /* mzc_pktinfo_DECLARED */

#ifndef YCIDL_sequence_mzc_pktinfo_DEFINED
#define YCIDL_sequence_mzc_pktinfo_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_pktinfo* _buffer;
} YCIDL_sequence_mzc_pktinfo;
#ifndef YCIDL_sequence_mzc_pktinfo_SUPP_FUNCS
#define YCIDL_sequence_mzc_pktinfo_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_pktinfo_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_pktinfo_DEFINED */

#ifndef mzc_link_DECLARED
#define mzc_link_DECLARED
typedef struct mzc_link mzc_link;
#endif /* mzc_link_DECLARED */

#ifndef YCIDL_sequence_mzc_link_DEFINED
#define YCIDL_sequence_mzc_link_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_link* _buffer;
} YCIDL_sequence_mzc_link;
#ifndef YCIDL_sequence_mzc_link_SUPP_FUNCS
#define YCIDL_sequence_mzc_link_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_link_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_link_DEFINED */

#ifndef mzc_chnlInfo_DECLARED
#define mzc_chnlInfo_DECLARED
typedef struct mzc_chnlInfo mzc_chnlInfo;
#endif /* mzc_chnlInfo_DECLARED */

#ifndef YCIDL_sequence_mzc_chnlInfo_DEFINED
#define YCIDL_sequence_mzc_chnlInfo_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_chnlInfo* _buffer;
} YCIDL_sequence_mzc_chnlInfo;
#ifndef YCIDL_sequence_mzc_chnlInfo_SUPP_FUNCS
#define YCIDL_sequence_mzc_chnlInfo_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_chnlInfo_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_chnlInfo_DEFINED */

#ifndef mzc_channel_DECLARED
#define mzc_channel_DECLARED
typedef struct mzc_channel mzc_channel;
#endif /* mzc_channel_DECLARED */

#ifndef YCIDL_sequence_mzc_channel_DEFINED
#define YCIDL_sequence_mzc_channel_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_channel* _buffer;
} YCIDL_sequence_mzc_channel;
#ifndef YCIDL_sequence_mzc_channel_SUPP_FUNCS
#define YCIDL_sequence_mzc_channel_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_channel_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_channel_DEFINED */

#ifndef mzc_chnlspec_DECLARED
#define mzc_chnlspec_DECLARED
typedef struct mzc_chnlspec mzc_chnlspec;
#endif /* mzc_chnlspec_DECLARED */

#ifndef YCIDL_sequence_mzc_chnlspec_DEFINED
#define YCIDL_sequence_mzc_chnlspec_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_chnlspec* _buffer;
} YCIDL_sequence_mzc_chnlspec;
#ifndef YCIDL_sequence_mzc_chnlspec_SUPP_FUNCS
#define YCIDL_sequence_mzc_chnlspec_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_chnlspec_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_chnlspec_DEFINED */


/* interface mzc_chnl declarations */
#ifndef mzc_chnl_DECLARED
#define mzc_chnl_DECLARED
typedef struct YCmzc_chnl* mzc_chnl;
yotk* mzc_chnl__getTC(void);
#ifndef YCTC_mzc_chnl
#define YCTC_mzc_chnl   (mzc_chnl__getTC())
#endif
#endif /* mzc_chnl_DECLARED */

#ifndef mzc_chnl_SUPP_FUNCS
#define mzc_chnl_SUPP_FUNCS
void mzc_chnl__free( mzc_chnl* val, ysmff ffunc);
void mzc_chnl__copy( mzc_chnl* dest, mzc_chnl* src, ysmaf afunc);
#endif /* mzc_chnl_SUPP_FUNCS */

#ifndef YCIDL_sequence_mzc_chnl_DEFINED
#define YCIDL_sequence_mzc_chnl_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_chnl* _buffer;
} YCIDL_sequence_mzc_chnl;
#ifndef YCIDL_sequence_mzc_chnl_SUPP_FUNCS
#define YCIDL_sequence_mzc_chnl_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_chnl_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_chnl_DEFINED */

#ifndef mzc_chnlPrvInfo_DECLARED
#define mzc_chnlPrvInfo_DECLARED
typedef struct mzc_chnlPrvInfo mzc_chnlPrvInfo;
#endif /* mzc_chnlPrvInfo_DECLARED */

#ifndef YCIDL_sequence_mzc_chnlPrvInfo_DEFINED
#define YCIDL_sequence_mzc_chnlPrvInfo_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_chnlPrvInfo* _buffer;
} YCIDL_sequence_mzc_chnlPrvInfo;
#ifndef YCIDL_sequence_mzc_chnlPrvInfo_SUPP_FUNCS
#define YCIDL_sequence_mzc_chnlPrvInfo_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_chnlPrvInfo_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_chnlPrvInfo_DEFINED */

#ifndef mzc_channelPrv_DECLARED
#define mzc_channelPrv_DECLARED
typedef struct mzc_channelPrv mzc_channelPrv;
#endif /* mzc_channelPrv_DECLARED */

#ifndef YCIDL_sequence_mzc_channelPrv_DEFINED
#define YCIDL_sequence_mzc_channelPrv_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_channelPrv* _buffer;
} YCIDL_sequence_mzc_channelPrv;
#ifndef YCIDL_sequence_mzc_channelPrv_SUPP_FUNCS
#define YCIDL_sequence_mzc_channelPrv_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_channelPrv_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_channelPrv_DEFINED */


/* interface mzc_chnlPrv declarations */
#ifndef mzc_chnlPrv_DECLARED
#define mzc_chnlPrv_DECLARED
typedef struct YCmzc_chnlPrv* mzc_chnlPrv;
yotk* mzc_chnlPrv__getTC(void);
#ifndef YCTC_mzc_chnlPrv
#define YCTC_mzc_chnlPrv   (mzc_chnlPrv__getTC())
#endif
#endif /* mzc_chnlPrv_DECLARED */

#ifndef mzc_chnlPrv_SUPP_FUNCS
#define mzc_chnlPrv_SUPP_FUNCS
void mzc_chnlPrv__free( mzc_chnlPrv* val, ysmff ffunc);
void mzc_chnlPrv__copy( mzc_chnlPrv* dest, mzc_chnlPrv* src, ysmaf afunc);
#endif /* mzc_chnlPrv_SUPP_FUNCS */

#ifndef YCIDL_sequence_mzc_chnlPrv_DEFINED
#define YCIDL_sequence_mzc_chnlPrv_DEFINED
typedef struct
{
  ub4 _maximum;
  ub4 _length;
  mzc_chnlPrv* _buffer;
} YCIDL_sequence_mzc_chnlPrv;
#ifndef YCIDL_sequence_mzc_chnlPrv_SUPP_FUNCS
#define YCIDL_sequence_mzc_chnlPrv_SUPP_FUNCS
#endif /* YCIDL_sequence_mzc_chnlPrv_SUPP_FUNCS */

#endif /* YCIDL_sequence_mzc_chnlPrv_DEFINED */

/******* NON-SEQUENCE DECLARATIONS *******/
#ifndef mzc_channelException_DECLARED
#define mzc_channelException_DECLARED
typedef ub4 mzc_channelException;
yotk* mzc_channelException__getTC(void);
#ifndef YCTC_mzc_channelException
#define YCTC_mzc_channelException   (mzc_channelException__getTC())
#endif
#endif /* mzc_channelException_DECLARED */

#ifndef mzc_channelException_DEFINED
#define mzc_channelException_DEFINED
#define mzc_chnlExNotImplemented ((mzc_channelException) 0)
#define mzc_chnlExOutOfMemory ((mzc_channelException) 1)
#define mzc_chnlExOutOfChannels ((mzc_channelException) 2)
#define mzc_chnlExOutOfBandwidth ((mzc_channelException) 3)
#define mzc_chnlExBadProtocol ((mzc_channelException) 4)
#define mzc_chnlExBadAddress ((mzc_channelException) 5)
#endif /* mzc_channelException_DEFINED */

#ifndef mzc_channelException_SUPP_FUNCS
#define mzc_channelException_SUPP_FUNCS
void mzc_channelException__free( mzc_channelException* val, ysmff ffunc);
void mzc_channelException__copy( mzc_channelException* dest, 
  mzc_channelException* src, ysmaf afunc);
#endif /* mzc_channelException_SUPP_FUNCS */

#ifndef MZC_EX_CHNLEX_DECLARED
#define MZC_EX_CHNLEX_DECLARED
CONST ysid* mzc_chnlEx__getId(void);
#ifndef MZC_EX_CHNLEX
#define MZC_EX_CHNLEX   (mzc_chnlEx__getId())
#endif
#endif /* MZC_EX_CHNLEX_DECLARED */

#ifndef mzc_chnlEx_DECLARED
#define mzc_chnlEx_DECLARED
typedef struct mzc_chnlEx mzc_chnlEx;
#endif /* mzc_chnlEx_DECLARED */

#ifndef mzc_chnlEx_DEFINED
#define mzc_chnlEx_DEFINED
struct mzc_chnlEx
{
  mzc_channelException channelFailType;
};
yotk* mzc_chnlEx__getTC(void);
#ifndef YCTC_mzc_chnlEx
#define YCTC_mzc_chnlEx   (mzc_chnlEx__getTC())
#endif
#endif /* mzc_chnlEx_DEFINED */

#ifndef mzc_chnlEx_SUPP_FUNCS
#define mzc_chnlEx_SUPP_FUNCS
void mzc_chnlEx__free( mzc_chnlEx* val, ysmff ffunc);
void mzc_chnlEx__copy( mzc_chnlEx* dest, mzc_chnlEx* src, ysmaf afunc);
#endif /* mzc_chnlEx_SUPP_FUNCS */

#ifndef mzc_commProperty_DECLARED
#define mzc_commProperty_DECLARED
typedef ub4 mzc_commProperty;
yotk* mzc_commProperty__getTC(void);
#ifndef YCTC_mzc_commProperty
#define YCTC_mzc_commProperty   (mzc_commProperty__getTC())
#endif
#endif /* mzc_commProperty_DECLARED */

#ifndef mzc_commProperty_SUPP_FUNCS
#define mzc_commProperty_SUPP_FUNCS
#endif /* mzc_commProperty_SUPP_FUNCS */

#ifndef mzc_propNull_DECLARED
#define mzc_propNull_DECLARED
#define mzc_propNull ((mzc_commProperty) 0)
#endif /* mzc_propNull_DECLARED */

#ifndef mzc_propDown_DECLARED
#define mzc_propDown_DECLARED
#define mzc_propDown ((mzc_commProperty) 1)
#endif /* mzc_propDown_DECLARED */

#ifndef mzc_propUp_DECLARED
#define mzc_propUp_DECLARED
#define mzc_propUp ((mzc_commProperty) 2)
#endif /* mzc_propUp_DECLARED */

#ifndef mzc_propPointcast_DECLARED
#define mzc_propPointcast_DECLARED
#define mzc_propPointcast ((mzc_commProperty) 16)
#endif /* mzc_propPointcast_DECLARED */

#ifndef mzc_propMulticast_DECLARED
#define mzc_propMulticast_DECLARED
#define mzc_propMulticast ((mzc_commProperty) 32)
#endif /* mzc_propMulticast_DECLARED */

#ifndef mzc_propBroadcast_DECLARED
#define mzc_propBroadcast_DECLARED
#define mzc_propBroadcast ((mzc_commProperty) 64)
#endif /* mzc_propBroadcast_DECLARED */

#ifndef mzc_propControl_DECLARED
#define mzc_propControl_DECLARED
#define mzc_propControl ((mzc_commProperty) 256)
#endif /* mzc_propControl_DECLARED */

#ifndef mzc_propData_DECLARED
#define mzc_propData_DECLARED
#define mzc_propData ((mzc_commProperty) 512)
#endif /* mzc_propData_DECLARED */

#ifndef mzc_propIsochronousData_DECLARED
#define mzc_propIsochronousData_DECLARED
#define mzc_propIsochronousData ((mzc_commProperty) 1024)
#endif /* mzc_propIsochronousData_DECLARED */

#ifndef mzc_propTransientConnect_DECLARED
#define mzc_propTransientConnect_DECLARED
#define mzc_propTransientConnect ((mzc_commProperty) 4096)
#endif /* mzc_propTransientConnect_DECLARED */

#ifndef mzc_propPersistantConnect_DECLARED
#define mzc_propPersistantConnect_DECLARED
#define mzc_propPersistantConnect ((mzc_commProperty) 8192)
#endif /* mzc_propPersistantConnect_DECLARED */

#ifndef mzc_propDisabled_DECLARED
#define mzc_propDisabled_DECLARED
#define mzc_propDisabled ((mzc_commProperty) 65536)
#endif /* mzc_propDisabled_DECLARED */

#ifndef mzc_propGroupDisabled_DECLARED
#define mzc_propGroupDisabled_DECLARED
#define mzc_propGroupDisabled ((mzc_commProperty) 131072)
#endif /* mzc_propGroupDisabled_DECLARED */

#ifndef mzc_logicalAddress_DECLARED
#define mzc_logicalAddress_DECLARED
typedef YCIDL_sequence_ub1 mzc_logicalAddress;
yotk* mzc_logicalAddress__getTC(void);
#ifndef YCTC_mzc_logicalAddress
#define YCTC_mzc_logicalAddress   (mzc_logicalAddress__getTC())
#endif
#endif /* mzc_logicalAddress_DECLARED */

#ifndef mzc_logicalAddress_SUPP_FUNCS
#define mzc_logicalAddress_SUPP_FUNCS
void mzc_logicalAddress__free( mzc_logicalAddress* val, ysmff ffunc);
void mzc_logicalAddress__copy( mzc_logicalAddress* dest, 
  mzc_logicalAddress* src, ysmaf afunc);
#endif /* mzc_logicalAddress_SUPP_FUNCS */


/* interface mzc_chnl declarations */
#ifndef mzc_chnl_DECLARED
#define mzc_chnl_DECLARED
typedef struct YCmzc_chnl* mzc_chnl;
yotk* mzc_chnl__getTC(void);
#ifndef YCTC_mzc_chnl
#define YCTC_mzc_chnl   (mzc_chnl__getTC())
#endif
#endif /* mzc_chnl_DECLARED */

#ifndef mzc_chnl_SUPP_FUNCS
#define mzc_chnl_SUPP_FUNCS
void mzc_chnl__free( mzc_chnl* val, ysmff ffunc);
void mzc_chnl__copy( mzc_chnl* dest, mzc_chnl* src, ysmaf afunc);
#endif /* mzc_chnl_SUPP_FUNCS */

#ifndef mzc_netproto_DECLARED
#define mzc_netproto_DECLARED
typedef struct mzc_netproto mzc_netproto;
#endif /* mzc_netproto_DECLARED */

#ifndef mzc_netproto_DEFINED
#define mzc_netproto_DEFINED
struct mzc_netproto
{
  char* name;
  char* info;
};
yotk* mzc_netproto__getTC(void);
#ifndef YCTC_mzc_netproto
#define YCTC_mzc_netproto   (mzc_netproto__getTC())
#endif
#endif /* mzc_netproto_DEFINED */

#ifndef mzc_netproto_SUPP_FUNCS
#define mzc_netproto_SUPP_FUNCS
void mzc_netproto__free( mzc_netproto* val, ysmff ffunc);
void mzc_netproto__copy( mzc_netproto* dest, mzc_netproto* src, ysmaf afunc)
  ;
#endif /* mzc_netproto_SUPP_FUNCS */

#ifndef mzc_netprotos_DECLARED
#define mzc_netprotos_DECLARED
typedef YCIDL_sequence_mzc_netproto mzc_netprotos;
yotk* mzc_netprotos__getTC(void);
#ifndef YCTC_mzc_netprotos
#define YCTC_mzc_netprotos   (mzc_netprotos__getTC())
#endif
#endif /* mzc_netprotos_DECLARED */

#ifndef mzc_netprotos_SUPP_FUNCS
#define mzc_netprotos_SUPP_FUNCS
void mzc_netprotos__free( mzc_netprotos* val, ysmff ffunc);
void mzc_netprotos__copy( mzc_netprotos* dest, mzc_netprotos* src, ysmaf 
  afunc);
#endif /* mzc_netprotos_SUPP_FUNCS */

#ifndef mzc_netapi_DECLARED
#define mzc_netapi_DECLARED
typedef struct mzc_netapi mzc_netapi;
#endif /* mzc_netapi_DECLARED */

#ifndef mzc_netapi_DEFINED
#define mzc_netapi_DEFINED
struct mzc_netapi
{
  char* name;
  YCIDL_sequence_ub1 info;
};
yotk* mzc_netapi__getTC(void);
#ifndef YCTC_mzc_netapi
#define YCTC_mzc_netapi   (mzc_netapi__getTC())
#endif
#endif /* mzc_netapi_DEFINED */

#ifndef mzc_netapi_SUPP_FUNCS
#define mzc_netapi_SUPP_FUNCS
void mzc_netapi__free( mzc_netapi* val, ysmff ffunc);
void mzc_netapi__copy( mzc_netapi* dest, mzc_netapi* src, ysmaf afunc);
#endif /* mzc_netapi_SUPP_FUNCS */

#ifndef mzc_netapis_DECLARED
#define mzc_netapis_DECLARED
typedef YCIDL_sequence_mzc_netapi mzc_netapis;
yotk* mzc_netapis__getTC(void);
#ifndef YCTC_mzc_netapis
#define YCTC_mzc_netapis   (mzc_netapis__getTC())
#endif
#endif /* mzc_netapis_DECLARED */

#ifndef mzc_netapis_SUPP_FUNCS
#define mzc_netapis_SUPP_FUNCS
void mzc_netapis__free( mzc_netapis* val, ysmff ffunc);
void mzc_netapis__copy( mzc_netapis* dest, mzc_netapis* src, ysmaf afunc);
#endif /* mzc_netapis_SUPP_FUNCS */

#ifndef mzc_netif_DECLARED
#define mzc_netif_DECLARED
typedef struct mzc_netif mzc_netif;
#endif /* mzc_netif_DECLARED */

#ifndef mzc_netif_DEFINED
#define mzc_netif_DEFINED
struct mzc_netif
{
  char* hostname;
  char* name;
  YCIDL_sequence_ub1 info;
  ub4 curbr;
  ub4 maxbr;
};
yotk* mzc_netif__getTC(void);
#ifndef YCTC_mzc_netif
#define YCTC_mzc_netif   (mzc_netif__getTC())
#endif
#endif /* mzc_netif_DEFINED */

#ifndef mzc_netif_SUPP_FUNCS
#define mzc_netif_SUPP_FUNCS
void mzc_netif__free( mzc_netif* val, ysmff ffunc);
void mzc_netif__copy( mzc_netif* dest, mzc_netif* src, ysmaf afunc);
#endif /* mzc_netif_SUPP_FUNCS */

#ifndef mzc_netifs_DECLARED
#define mzc_netifs_DECLARED
typedef YCIDL_sequence_mzc_netif mzc_netifs;
yotk* mzc_netifs__getTC(void);
#ifndef YCTC_mzc_netifs
#define YCTC_mzc_netifs   (mzc_netifs__getTC())
#endif
#endif /* mzc_netifs_DECLARED */

#ifndef mzc_netifs_SUPP_FUNCS
#define mzc_netifs_SUPP_FUNCS
void mzc_netifs__free( mzc_netifs* val, ysmff ffunc);
void mzc_netifs__copy( mzc_netifs* dest, mzc_netifs* src, ysmaf afunc);
#endif /* mzc_netifs_SUPP_FUNCS */

#ifndef mzc_pktinfo_DECLARED
#define mzc_pktinfo_DECLARED
typedef struct mzc_pktinfo mzc_pktinfo;
#endif /* mzc_pktinfo_DECLARED */

#ifndef mzc_pktinfo_DEFINED
#define mzc_pktinfo_DEFINED
struct mzc_pktinfo
{
  ub4 pref_size;
  ub4 max_size;
  ub4 modulos;
};
yotk* mzc_pktinfo__getTC(void);
#ifndef YCTC_mzc_pktinfo
#define YCTC_mzc_pktinfo   (mzc_pktinfo__getTC())
#endif
#endif /* mzc_pktinfo_DEFINED */

#ifndef mzc_pktinfo_SUPP_FUNCS
#define mzc_pktinfo_SUPP_FUNCS
void mzc_pktinfo__free( mzc_pktinfo* val, ysmff ffunc);
void mzc_pktinfo__copy( mzc_pktinfo* dest, mzc_pktinfo* src, ysmaf afunc);
#endif /* mzc_pktinfo_SUPP_FUNCS */

#ifndef mzc_pktinfos_DECLARED
#define mzc_pktinfos_DECLARED
typedef YCIDL_sequence_mzc_pktinfo mzc_pktinfos;
yotk* mzc_pktinfos__getTC(void);
#ifndef YCTC_mzc_pktinfos
#define YCTC_mzc_pktinfos   (mzc_pktinfos__getTC())
#endif
#endif /* mzc_pktinfos_DECLARED */

#ifndef mzc_pktinfos_SUPP_FUNCS
#define mzc_pktinfos_SUPP_FUNCS
void mzc_pktinfos__free( mzc_pktinfos* val, ysmff ffunc);
void mzc_pktinfos__copy( mzc_pktinfos* dest, mzc_pktinfos* src, ysmaf afunc)
  ;
#endif /* mzc_pktinfos_SUPP_FUNCS */

#ifndef mzc_link_DECLARED
#define mzc_link_DECLARED
typedef struct mzc_link mzc_link;
#endif /* mzc_link_DECLARED */

#ifndef mzc_link_DEFINED
#define mzc_link_DEFINED
struct mzc_link
{
  mzc_netproto protocol;
  mzc_netapi software;
  mzc_netif hardware;
  mzc_pktinfo packet;
};
yotk* mzc_link__getTC(void);
#ifndef YCTC_mzc_link
#define YCTC_mzc_link   (mzc_link__getTC())
#endif
#endif /* mzc_link_DEFINED */

#ifndef mzc_link_SUPP_FUNCS
#define mzc_link_SUPP_FUNCS
void mzc_link__free( mzc_link* val, ysmff ffunc);
void mzc_link__copy( mzc_link* dest, mzc_link* src, ysmaf afunc);
#endif /* mzc_link_SUPP_FUNCS */

#ifndef mzc_links_DECLARED
#define mzc_links_DECLARED
typedef YCIDL_sequence_mzc_link mzc_links;
yotk* mzc_links__getTC(void);
#ifndef YCTC_mzc_links
#define YCTC_mzc_links   (mzc_links__getTC())
#endif
#endif /* mzc_links_DECLARED */

#ifndef mzc_links_SUPP_FUNCS
#define mzc_links_SUPP_FUNCS
void mzc_links__free( mzc_links* val, ysmff ffunc);
void mzc_links__copy( mzc_links* dest, mzc_links* src, ysmaf afunc);
#endif /* mzc_links_SUPP_FUNCS */

#ifndef mzc_chnlInfo_DECLARED
#define mzc_chnlInfo_DECLARED
typedef struct mzc_chnlInfo mzc_chnlInfo;
#endif /* mzc_chnlInfo_DECLARED */

#ifndef mzc_chnlInfo_DEFINED
#define mzc_chnlInfo_DEFINED
struct mzc_chnlInfo
{
  mzc_commProperty props;
  mzc_link comm;
  ub4 bitrate;
  mzc_logicalAddress mna;
};
yotk* mzc_chnlInfo__getTC(void);
#ifndef YCTC_mzc_chnlInfo
#define YCTC_mzc_chnlInfo   (mzc_chnlInfo__getTC())
#endif
#endif /* mzc_chnlInfo_DEFINED */

#ifndef mzc_chnlInfo_SUPP_FUNCS
#define mzc_chnlInfo_SUPP_FUNCS
void mzc_chnlInfo__free( mzc_chnlInfo* val, ysmff ffunc);
void mzc_chnlInfo__copy( mzc_chnlInfo* dest, mzc_chnlInfo* src, ysmaf afunc)
  ;
#endif /* mzc_chnlInfo_SUPP_FUNCS */

#ifndef mzc_chnlInfos_DECLARED
#define mzc_chnlInfos_DECLARED
typedef YCIDL_sequence_mzc_chnlInfo mzc_chnlInfos;
yotk* mzc_chnlInfos__getTC(void);
#ifndef YCTC_mzc_chnlInfos
#define YCTC_mzc_chnlInfos   (mzc_chnlInfos__getTC())
#endif
#endif /* mzc_chnlInfos_DECLARED */

#ifndef mzc_chnlInfos_SUPP_FUNCS
#define mzc_chnlInfos_SUPP_FUNCS
void mzc_chnlInfos__free( mzc_chnlInfos* val, ysmff ffunc);
void mzc_chnlInfos__copy( mzc_chnlInfos* dest, mzc_chnlInfos* src, ysmaf 
  afunc);
#endif /* mzc_chnlInfos_SUPP_FUNCS */

#ifndef mzc_channel_DECLARED
#define mzc_channel_DECLARED
typedef struct mzc_channel mzc_channel;
#endif /* mzc_channel_DECLARED */

#ifndef mzc_channel_DEFINED
#define mzc_channel_DEFINED
struct mzc_channel
{
  mzc_chnl or;
  mzc_chnlInfo info;
};
yotk* mzc_channel__getTC(void);
#ifndef YCTC_mzc_channel
#define YCTC_mzc_channel   (mzc_channel__getTC())
#endif
#endif /* mzc_channel_DEFINED */

#ifndef mzc_channel_SUPP_FUNCS
#define mzc_channel_SUPP_FUNCS
void mzc_channel__free( mzc_channel* val, ysmff ffunc);
void mzc_channel__copy( mzc_channel* dest, mzc_channel* src, ysmaf afunc);
#endif /* mzc_channel_SUPP_FUNCS */

#ifndef mzc_channels_DECLARED
#define mzc_channels_DECLARED
typedef YCIDL_sequence_mzc_channel mzc_channels;
yotk* mzc_channels__getTC(void);
#ifndef YCTC_mzc_channels
#define YCTC_mzc_channels   (mzc_channels__getTC())
#endif
#endif /* mzc_channels_DECLARED */

#ifndef mzc_channels_SUPP_FUNCS
#define mzc_channels_SUPP_FUNCS
void mzc_channels__free( mzc_channels* val, ysmff ffunc);
void mzc_channels__copy( mzc_channels* dest, mzc_channels* src, ysmaf afunc)
  ;
#endif /* mzc_channels_SUPP_FUNCS */

#ifndef mzc_chnlreq_DECLARED
#define mzc_chnlreq_DECLARED
typedef struct mzc_chnlreq mzc_chnlreq;
#endif /* mzc_chnlreq_DECLARED */

#ifndef mzc_chnlreq_DEFINED
#define mzc_chnlreq_DEFINED
struct mzc_chnlreq
{
  mzc_commProperty props;
  mzc_netproto protocol;
  ub4 bitrate;
};
yotk* mzc_chnlreq__getTC(void);
#ifndef YCTC_mzc_chnlreq
#define YCTC_mzc_chnlreq   (mzc_chnlreq__getTC())
#endif
#endif /* mzc_chnlreq_DEFINED */

#ifndef mzc_chnlreq_SUPP_FUNCS
#define mzc_chnlreq_SUPP_FUNCS
void mzc_chnlreq__free( mzc_chnlreq* val, ysmff ffunc);
void mzc_chnlreq__copy( mzc_chnlreq* dest, mzc_chnlreq* src, ysmaf afunc);
#endif /* mzc_chnlreq_SUPP_FUNCS */

#ifndef mzc_chnlspecType_DECLARED
#define mzc_chnlspecType_DECLARED
typedef ub4 mzc_chnlspecType;
yotk* mzc_chnlspecType__getTC(void);
#ifndef YCTC_mzc_chnlspecType
#define YCTC_mzc_chnlspecType   (mzc_chnlspecType__getTC())
#endif
#endif /* mzc_chnlspecType_DECLARED */

#ifndef mzc_chnlspecType_DEFINED
#define mzc_chnlspecType_DEFINED
#define mzc_chnlspecTypeNone ((mzc_chnlspecType) 0)
#define mzc_chnlspecTypeRequest ((mzc_chnlspecType) 1)
#define mzc_chnlspecTypeChannel ((mzc_chnlspecType) 2)
#endif /* mzc_chnlspecType_DEFINED */

#ifndef mzc_chnlspecType_SUPP_FUNCS
#define mzc_chnlspecType_SUPP_FUNCS
void mzc_chnlspecType__free( mzc_chnlspecType* val, ysmff ffunc);
void mzc_chnlspecType__copy( mzc_chnlspecType* dest, mzc_chnlspecType* src, 
  ysmaf afunc);
#endif /* mzc_chnlspecType_SUPP_FUNCS */

#ifndef mzc_chnlspec_DECLARED
#define mzc_chnlspec_DECLARED
typedef struct mzc_chnlspec mzc_chnlspec;
#endif /* mzc_chnlspec_DECLARED */

#ifndef mzc_chnlspec_DEFINED
#define mzc_chnlspec_DEFINED
struct mzc_chnlspec
{
  mzc_chnlspecType _d;
  union
  {
    sb4 none;
    mzc_chnlreq req;
    mzc_channel chnl;
  } _u;
};
yotk* mzc_chnlspec__getTC(void);
#ifndef YCTC_mzc_chnlspec
#define YCTC_mzc_chnlspec   (mzc_chnlspec__getTC())
#endif
#endif /* mzc_chnlspec_DEFINED */

#ifndef mzc_chnlspec_SUPP_FUNCS
#define mzc_chnlspec_SUPP_FUNCS
void mzc_chnlspec__free( mzc_chnlspec* val, ysmff ffunc);
void mzc_chnlspec__copy( mzc_chnlspec* dest, mzc_chnlspec* src, ysmaf afunc)
  ;
#endif /* mzc_chnlspec_SUPP_FUNCS */

#ifndef mzc_chnlspecs_DECLARED
#define mzc_chnlspecs_DECLARED
typedef YCIDL_sequence_mzc_chnlspec mzc_chnlspecs;
yotk* mzc_chnlspecs__getTC(void);
#ifndef YCTC_mzc_chnlspecs
#define YCTC_mzc_chnlspecs   (mzc_chnlspecs__getTC())
#endif
#endif /* mzc_chnlspecs_DECLARED */

#ifndef mzc_chnlspecs_SUPP_FUNCS
#define mzc_chnlspecs_SUPP_FUNCS
void mzc_chnlspecs__free( mzc_chnlspecs* val, ysmff ffunc);
void mzc_chnlspecs__copy( mzc_chnlspecs* dest, mzc_chnlspecs* src, ysmaf 
  afunc);
#endif /* mzc_chnlspecs_SUPP_FUNCS */

#ifndef mzc_chnlreqx_DECLARED
#define mzc_chnlreqx_DECLARED
typedef struct mzc_chnlreqx mzc_chnlreqx;
#endif /* mzc_chnlreqx_DECLARED */

#ifndef mzc_chnlreqx_DEFINED
#define mzc_chnlreqx_DEFINED
struct mzc_chnlreqx
{
  mzc_commProperty props;
  mzc_netproto protocol;
  mzc_netapi software;
  mzc_netif hardware;
  mzc_pktinfo packet;
  ub4 bitrate;
};
yotk* mzc_chnlreqx__getTC(void);
#ifndef YCTC_mzc_chnlreqx
#define YCTC_mzc_chnlreqx   (mzc_chnlreqx__getTC())
#endif
#endif /* mzc_chnlreqx_DEFINED */

#ifndef mzc_chnlreqx_SUPP_FUNCS
#define mzc_chnlreqx_SUPP_FUNCS
void mzc_chnlreqx__free( mzc_chnlreqx* val, ysmff ffunc);
void mzc_chnlreqx__copy( mzc_chnlreqx* dest, mzc_chnlreqx* src, ysmaf afunc)
  ;
#endif /* mzc_chnlreqx_SUPP_FUNCS */


/* interface mzc_chnl declarations */
#ifndef mzc_chnl_DECLARED
#define mzc_chnl_DECLARED
typedef struct YCmzc_chnl* mzc_chnl;
yotk* mzc_chnl__getTC(void);
#ifndef YCTC_mzc_chnl
#define YCTC_mzc_chnl   (mzc_chnl__getTC())
#endif
#endif /* mzc_chnl_DECLARED */

#ifndef mzc_chnl_SUPP_FUNCS
#define mzc_chnl_SUPP_FUNCS
void mzc_chnl__free( mzc_chnl* val, ysmff ffunc);
void mzc_chnl__copy( mzc_chnl* dest, mzc_chnl* src, ysmaf afunc);
#endif /* mzc_chnl_SUPP_FUNCS */

CONST ysid* mzc_chnl__getId(void);
#ifndef mzc_chnl__id
#define mzc_chnl__id   (mzc_chnl__getId())
#endif
#ifndef mzc_chnls_DECLARED
#define mzc_chnls_DECLARED
typedef YCIDL_sequence_mzc_chnl mzc_chnls;
yotk* mzc_chnls__getTC(void);
#ifndef YCTC_mzc_chnls
#define YCTC_mzc_chnls   (mzc_chnls__getTC())
#endif
#endif /* mzc_chnls_DECLARED */

#ifndef mzc_chnls_SUPP_FUNCS
#define mzc_chnls_SUPP_FUNCS
void mzc_chnls__free( mzc_chnls* val, ysmff ffunc);
void mzc_chnls__copy( mzc_chnls* dest, mzc_chnls* src, ysmaf afunc);
#endif /* mzc_chnls_SUPP_FUNCS */


/* interface mzc_chnlPrv declarations */
#ifndef mzc_chnlPrv_DECLARED
#define mzc_chnlPrv_DECLARED
typedef struct YCmzc_chnlPrv* mzc_chnlPrv;
yotk* mzc_chnlPrv__getTC(void);
#ifndef YCTC_mzc_chnlPrv
#define YCTC_mzc_chnlPrv   (mzc_chnlPrv__getTC())
#endif
#endif /* mzc_chnlPrv_DECLARED */

#ifndef mzc_chnlPrv_SUPP_FUNCS
#define mzc_chnlPrv_SUPP_FUNCS
void mzc_chnlPrv__free( mzc_chnlPrv* val, ysmff ffunc);
void mzc_chnlPrv__copy( mzc_chnlPrv* dest, mzc_chnlPrv* src, ysmaf afunc);
#endif /* mzc_chnlPrv_SUPP_FUNCS */

#ifndef mzc_chnlPrvInfo_DECLARED
#define mzc_chnlPrvInfo_DECLARED
typedef struct mzc_chnlPrvInfo mzc_chnlPrvInfo;
#endif /* mzc_chnlPrvInfo_DECLARED */

#ifndef mzc_chnlPrvInfo_DEFINED
#define mzc_chnlPrvInfo_DEFINED
struct mzc_chnlPrvInfo
{
  char* name;
  ub4 max_channels;
  ub4 max_bitrate;
  ub4 max_channelrate;
  mzc_chnlInfos channel_types;
  mzc_channels cur_channels;
  ub4 cur_bitrate;
};
yotk* mzc_chnlPrvInfo__getTC(void);
#ifndef YCTC_mzc_chnlPrvInfo
#define YCTC_mzc_chnlPrvInfo   (mzc_chnlPrvInfo__getTC())
#endif
#endif /* mzc_chnlPrvInfo_DEFINED */

#ifndef mzc_chnlPrvInfo_SUPP_FUNCS
#define mzc_chnlPrvInfo_SUPP_FUNCS
void mzc_chnlPrvInfo__free( mzc_chnlPrvInfo* val, ysmff ffunc);
void mzc_chnlPrvInfo__copy( mzc_chnlPrvInfo* dest, mzc_chnlPrvInfo* src, 
  ysmaf afunc);
#endif /* mzc_chnlPrvInfo_SUPP_FUNCS */

#ifndef mzc_chnlPrvInfos_DECLARED
#define mzc_chnlPrvInfos_DECLARED
typedef YCIDL_sequence_mzc_chnlPrvInfo mzc_chnlPrvInfos;
yotk* mzc_chnlPrvInfos__getTC(void);
#ifndef YCTC_mzc_chnlPrvInfos
#define YCTC_mzc_chnlPrvInfos   (mzc_chnlPrvInfos__getTC())
#endif
#endif /* mzc_chnlPrvInfos_DECLARED */

#ifndef mzc_chnlPrvInfos_SUPP_FUNCS
#define mzc_chnlPrvInfos_SUPP_FUNCS
void mzc_chnlPrvInfos__free( mzc_chnlPrvInfos* val, ysmff ffunc);
void mzc_chnlPrvInfos__copy( mzc_chnlPrvInfos* dest, mzc_chnlPrvInfos* src, 
  ysmaf afunc);
#endif /* mzc_chnlPrvInfos_SUPP_FUNCS */

#ifndef mzc_channelPrv_DECLARED
#define mzc_channelPrv_DECLARED
typedef struct mzc_channelPrv mzc_channelPrv;
#endif /* mzc_channelPrv_DECLARED */

#ifndef mzc_channelPrv_DEFINED
#define mzc_channelPrv_DEFINED
struct mzc_channelPrv
{
  mzc_chnlPrv or;
  mzc_chnlPrvInfo info;
};
yotk* mzc_channelPrv__getTC(void);
#ifndef YCTC_mzc_channelPrv
#define YCTC_mzc_channelPrv   (mzc_channelPrv__getTC())
#endif
#endif /* mzc_channelPrv_DEFINED */

#ifndef mzc_channelPrv_SUPP_FUNCS
#define mzc_channelPrv_SUPP_FUNCS
void mzc_channelPrv__free( mzc_channelPrv* val, ysmff ffunc);
void mzc_channelPrv__copy( mzc_channelPrv* dest, mzc_channelPrv* src, ysmaf 
  afunc);
#endif /* mzc_channelPrv_SUPP_FUNCS */

#ifndef mzc_channelPrvs_DECLARED
#define mzc_channelPrvs_DECLARED
typedef YCIDL_sequence_mzc_channelPrv mzc_channelPrvs;
yotk* mzc_channelPrvs__getTC(void);
#ifndef YCTC_mzc_channelPrvs
#define YCTC_mzc_channelPrvs   (mzc_channelPrvs__getTC())
#endif
#endif /* mzc_channelPrvs_DECLARED */

#ifndef mzc_channelPrvs_SUPP_FUNCS
#define mzc_channelPrvs_SUPP_FUNCS
void mzc_channelPrvs__free( mzc_channelPrvs* val, ysmff ffunc);
void mzc_channelPrvs__copy( mzc_channelPrvs* dest, mzc_channelPrvs* src, 
  ysmaf afunc);
#endif /* mzc_channelPrvs_SUPP_FUNCS */


/* interface mzc_chnlPrv declarations */
#ifndef mzc_chnlPrv_DECLARED
#define mzc_chnlPrv_DECLARED
typedef struct YCmzc_chnlPrv* mzc_chnlPrv;
yotk* mzc_chnlPrv__getTC(void);
#ifndef YCTC_mzc_chnlPrv
#define YCTC_mzc_chnlPrv   (mzc_chnlPrv__getTC())
#endif
#endif /* mzc_chnlPrv_DECLARED */

#ifndef mzc_chnlPrv_SUPP_FUNCS
#define mzc_chnlPrv_SUPP_FUNCS
void mzc_chnlPrv__free( mzc_chnlPrv* val, ysmff ffunc);
void mzc_chnlPrv__copy( mzc_chnlPrv* dest, mzc_chnlPrv* src, ysmaf afunc);
#endif /* mzc_chnlPrv_SUPP_FUNCS */

#ifndef mzc_chnlPrv_DEFINED
#define mzc_chnlPrv_DEFINED
struct yostub* mzc_chnlPrv__getStubs(void);
#define mzc_chnlPrv__stubs (mzc_chnlPrv__getStubs())
#endif /* mzc_chnlPrv_DEFINED */

CONST ysid* mzc_chnlPrv__getId(void);
#ifndef mzc_chnlPrv__id
#define mzc_chnlPrv__id   (mzc_chnlPrv__getId())
#endif
mzc_chnlPrvInfo mzc_chnlPrv_GetInfo( mzc_chnlPrv or, yoenv* ev);
void mzc_chnlPrv_GetInfo_nw( mzc_chnlPrv or, yoenv* ev, ysevt* _sem);
yopar* mzc_chnlPrv_GetInfo__getPars(void);
#ifndef mzc_chnlPrv_GetInfo_pars
#define mzc_chnlPrv_GetInfo_pars (mzc_chnlPrv_GetInfo__getPars())
#endif

mzc_channel mzc_chnlPrv_Build( mzc_chnlPrv or, yoenv* ev, mzc_chnlreqx* req)
  ;
void mzc_chnlPrv_Build_nw( mzc_chnlPrv or, yoenv* ev, mzc_chnlreqx* req, 
  ysevt* _sem);
yopar* mzc_chnlPrv_Build__getPars(void);
#ifndef mzc_chnlPrv_Build_pars
#define mzc_chnlPrv_Build_pars (mzc_chnlPrv_Build__getPars())
#endif

#ifndef mzc_chnlPrv__tyimpl_DEFINED
#define mzc_chnlPrv__tyimpl_DEFINED
struct mzc_chnlPrv__tyimpl
{
  mzc_chnlPrvInfo (*GetInfo)( mzc_chnlPrv, yoenv*);
  mzc_channel (*Build)( mzc_chnlPrv, yoenv*, mzc_chnlreqx*);
};
#endif /* mzc_chnlPrv__tyimpl_DEFINED */

#ifndef mzc_chnlPrvs_DECLARED
#define mzc_chnlPrvs_DECLARED
typedef YCIDL_sequence_mzc_chnlPrv mzc_chnlPrvs;
yotk* mzc_chnlPrvs__getTC(void);
#ifndef YCTC_mzc_chnlPrvs
#define YCTC_mzc_chnlPrvs   (mzc_chnlPrvs__getTC())
#endif
#endif /* mzc_chnlPrvs_DECLARED */

#ifndef mzc_chnlPrvs_SUPP_FUNCS
#define mzc_chnlPrvs_SUPP_FUNCS
void mzc_chnlPrvs__free( mzc_chnlPrvs* val, ysmff ffunc);
void mzc_chnlPrvs__copy( mzc_chnlPrvs* dest, mzc_chnlPrvs* src, ysmaf afunc)
  ;
#endif /* mzc_chnlPrvs_SUPP_FUNCS */

EXTC_END
#endif /* MZCCP_IDL */
