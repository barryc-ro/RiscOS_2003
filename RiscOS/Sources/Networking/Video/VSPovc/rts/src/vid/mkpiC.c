/* GENERATED FILE
 * mkpi - client stubs
 * from /vobs/rts/src/vid/mkpi.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MKPI_IDL
#include <mkpi.h>
#endif

EXTC_START

/* Client stubs for interface ::mkpi::stmCB */
static ysidDecl(mkpi_stmCB___id) = "IDL:mkpi/stmCB:1.0";

CONST ysid* mkpi_stmCB__getId(void)
{
  return (CONST ysid*)mkpi_stmCB___id;
}

static CONST_DATA yotk mkpi_stmCB__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'.',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x13,'I','D','L',':','m','k','p','i','/','s',
  't','m','C','B',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x0e
  ,':',':','m','k','p','i',':',':','s','t','m','C','B',0x00};

yotk* mkpi_stmCB__getTC(void)
{
  return (yotk*)mkpi_stmCB__tc;
}


void mkpi_stmCB__free( mkpi_stmCB* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpi_stmCB, (dvoid *)val, ffunc);
}

void mkpi_stmCB__copy( mkpi_stmCB* dest, mkpi_stmCB* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mkpi_stmCB, (dvoid *)dest, (dvoid *)src, afunc);
}



/* Client stubs for interface ::mkpi::stm */
static ysidDecl(mkpi_stm___id) = "IDL:mkpi/stm:1.0";

CONST ysid* mkpi_stm__getId(void)
{
  return (CONST ysid*)mkpi_stm___id;
}

static CONST_DATA yotk mkpi_stm__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,',',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x11,'I','D','L',':','m','k','p','i','/','s',
  't','m',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c
  ,':',':','m','k','p','i',':',':','s','t','m',0x00};

yotk* mkpi_stm__getTC(void)
{
  return (yotk*)mkpi_stm__tc;
}


void mkpi_stm__free( mkpi_stm* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpi_stm, (dvoid *)val, ffunc);
}

void mkpi_stm__copy( mkpi_stm* dest, mkpi_stm* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mkpi_stm, (dvoid *)dest, (dvoid *)src, afunc);
}



/* Client stubs for interface ::mkpi::gbl */
static ysidDecl(mkpi_gbl___id) = "IDL:mkpi/gbl:1.0";

CONST ysid* mkpi_gbl__getId(void)
{
  return (CONST ysid*)mkpi_gbl___id;
}

static CONST_DATA yotk mkpi_gbl__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,',',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x11,'I','D','L',':','m','k','p','i','/','g',
  'b','l',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c
  ,':',':','m','k','p','i',':',':','g','b','l',0x00};

yotk* mkpi_gbl__getTC(void)
{
  return (yotk*)mkpi_gbl__tc;
}


void mkpi_gbl__free( mkpi_gbl* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mkpi_gbl, (dvoid *)val, ffunc);
}

void mkpi_gbl__copy( mkpi_gbl* dest, mkpi_gbl* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mkpi_gbl, (dvoid *)dest, (dvoid *)src, afunc);
}




EXTC_END
