/* GENERATED FILE
 * mzscb - client stubs
 * from mzscb.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MZSCB_IDL
#include <mzscb.h>
#endif

EXTC_START

/* Client stubs for interface ::mzs::streamcb */
static ysidDecl(mzs_streamcb___id) = "IDL:mzs/streamcb:1.0";

CONST ysid* mzs_streamcb__getId(void)
{
  return (CONST ysid*)mzs_streamcb___id;
}

static CONST_DATA yotk mzs_streamcb__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'4',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','z','s','/','s','t',
  'r','e','a','m','c','b',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','z','s',':',':','s','t','r',
  'e','a','m','c','b',0x00};

yotk* mzs_streamcb__getTC(void)
{
  return (yotk*)mzs_streamcb__tc;
}


void mzs_streamcb__free( mzs_streamcb* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mzs_streamcb, (dvoid *)val, ffunc);
}

void mzs_streamcb__copy( mzs_streamcb* dest, mzs_streamcb* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mzs_streamcb, (dvoid *)dest, (dvoid *)src, afunc);
}




EXTC_END
