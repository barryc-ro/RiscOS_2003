/* GENERATED FILE
 * mtut - client stubs
 * from mtut.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef MTUT_IDL
#include <mtut.h>
#endif

EXTC_START

static CONST_DATA yotk mtut_slaveExc__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x8d,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x16,'I','D','L',':','m','t','u','t','/','s'
  ,'l','a','v','e','E','x','c',':','1','.','0',0x00,0x00,0x00,
  0x00,0x00,0x00,0x11,':',':','m','t','u','t',':',':','s','l',
  'a','v','e','E','x','c',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04
  ,0x00,0x00,0x00,0x0c,'s','l','a','v','e','O','u','t','M','e'
  ,'m',0x00,0x00,0x00,0x00,0x0e,'s','l','a','v','e','B','a','d'
  ,'P','a','r','a','m',0x00,0x00,0x00,0x00,0x00,0x00,0x0e,'s',
  'l','a','v','e','C','h','i','l','l','O','u','t',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x15,'s','l','a','v','e','J','u','s','t','D'
  ,'o','n','t','L','i','k','e','Y','o','u',0x00};

yotk* mtut_slaveExc__getTC(void)
{
  return (yotk*)mtut_slaveExc__tc;
}

void mtut_slaveExc__free( mtut_slaveExc* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_slaveExc, (dvoid *)val, ffunc);
}

void mtut_slaveExc__copy( mtut_slaveExc* dest, mtut_slaveExc* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mtut_slaveExc, (dvoid *)dest, (dvoid *)src, afunc);
}

static ysidDecl(mtut_slave___id) = "IDL:mtut/slave:1.0";

CONST ysid* mtut_slave__getId(void)
{
  return (CONST ysid*)mtut_slave___id;
}

static CONST_DATA yotk mtut_slave__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,0xd9,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x13,'I','D','L',':','m','t','u','t','/','s'
  ,'l','a','v','e',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x0e
  ,':',':','m','t','u','t',':',':','s','l','a','v','e',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x09,'f','a','i','l'
  ,'T','y','p','e',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00
  ,0x00,0x00,0x8d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x16,'I',
  'D','L',':','m','t','u','t','/','s','l','a','v','e','E','x',
  'c',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x11,':',':'
  ,'m','t','u','t',':',':','s','l','a','v','e','E','x','c',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x0c,'s',
  'l','a','v','e','O','u','t','M','e','m',0x00,0x00,0x00,0x00,
  0x0e,'s','l','a','v','e','B','a','d','P','a','r','a','m',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x0e,'s','l','a','v','e','C','h','i'
  ,'l','l','O','u','t',0x00,0x00,0x00,0x00,0x00,0x00,0x15,'s',
  'l','a','v','e','J','u','s','t','D','o','n','t','L','i','k',
  'e','Y','o','u',0x00};

yotk* mtut_slave__getTC(void)
{
  return (yotk*)mtut_slave__tc;
}

void mtut_slave__free( mtut_slave* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_slave, (dvoid *)val, ffunc);
}

void mtut_slave__copy( mtut_slave* dest, mtut_slave* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_slave, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_fmt__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,'k',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x11,'I','D','L',':','m','t','u','t','/','f',
  'm','t',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c
  ,':',':','m','t','u','t',':',':','f','m','t',0x00,0x00,0x00,
  0x00,0x04,0x00,0x00,0x00,0x09,'f','m','t','E','r','r','o','r'
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,'f','m','t','M','p'
  ,'e','g','1','S','S',0x00,0x00,0x00,0x00,0x00,0x0a,'f','m','t'
  ,'M','p','e','g','2','T',0x00,0x00,0x00,0x00,0x00,0x00,0x07,
  'f','m','t','A','V','I',0x00};

yotk* mtut_fmt__getTC(void)
{
  return (yotk*)mtut_fmt__tc;
}

void mtut_fmt__free( mtut_fmt* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_fmt, (dvoid *)val, ffunc);
}

void mtut_fmt__copy( mtut_fmt* dest, mtut_fmt* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_fmt, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_m1ssGuts__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'L',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x16,'I','D','L',':','m','t','u','t','/','m',
  '1','s','s','G','u','t','s',':','1','.','0',0x00,0x00,0x00,0x00
  ,0x00,0x00,0x11,':',':','m','t','u','t',':',':','m','1','s',
  's','G','u','t','s',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
  0x00,0x00,0x00,0x08,'n','o','t','h','i','n','g',0x00,0x00,0x00
  ,0x00,0x0a};

yotk* mtut_m1ssGuts__getTC(void)
{
  return (yotk*)mtut_m1ssGuts__tc;
}

void mtut_m1ssGuts__free( mtut_m1ssGuts* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_m1ssGuts, (dvoid *)val, ffunc);
}

void mtut_m1ssGuts__copy( mtut_m1ssGuts* dest, mtut_m1ssGuts* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mtut_m1ssGuts, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_m2tGuts__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'H',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','t','u','t','/','m',
  '2','t','G','u','t','s',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','t','u','t',':',':','m','2',
  't','G','u','t','s',0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
  0x08,'n','o','t','h','i','n','g',0x00,0x00,0x00,0x00,0x0a};

yotk* mtut_m2tGuts__getTC(void)
{
  return (yotk*)mtut_m2tGuts__tc;
}

void mtut_m2tGuts__free( mtut_m2tGuts* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_m2tGuts, (dvoid *)val, ffunc);
}

void mtut_m2tGuts__copy( mtut_m2tGuts* dest, mtut_m2tGuts* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mtut_m2tGuts, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_aviGuts__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'H',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','t','u','t','/','a',
  'v','i','G','u','t','s',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','t','u','t',':',':','a','v',
  'i','G','u','t','s',0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
  0x08,'n','o','t','h','i','n','g',0x00,0x00,0x00,0x00,0x0a};

yotk* mtut_aviGuts__getTC(void)
{
  return (yotk*)mtut_aviGuts__tc;
}

void mtut_aviGuts__free( mtut_aviGuts* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_aviGuts, (dvoid *)val, ffunc);
}

void mtut_aviGuts__copy( mtut_aviGuts* dest, mtut_aviGuts* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mtut_aviGuts, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_specifics__tc[] = 
  {0x00,0x00,0x00,0x10,0x00,0x00,0x01,0xd0,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x17,'I','D','L',':','m','t','u','t','/','s'
  ,'p','e','c','i','f','i','c','s',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x12,':',':','m','t','u','t',':',':','s','p','e',
  'c','i','f','i','c','s',0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00
  ,0x00,0x00,'k',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,'I','D'
  ,'L',':','m','t','u','t','/','f','m','t',':','1','.','0',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,':',':','m','t','u','t',
  ':',':','f','m','t',0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,
  0x09,'f','m','t','E','r','r','o','r',0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x0b,'f','m','t','M','p','e','g','1','S','S',0x00
  ,0x00,0x00,0x00,0x00,0x0a,'f','m','t','M','p','e','g','2','T'
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x07,'f','m','t','A','V','I',
  0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x03,0x00,0x00,
  0x00,0x01,0x00,0x00,0x00,0x05,'m','1','s','s',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'L',0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x16,'I','D','L',':','m','t','u','t','/'
  ,'m','1','s','s','G','u','t','s',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x11,':',':','m','t','u','t',':',':','m','1'
  ,'s','s','G','u','t','s',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x01,0x00,0x00,0x00,0x08,'n','o','t','h','i','n','g',0x00,0x00
  ,0x00,0x00,0x0a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x04,'m',
  '2','t',0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'H',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x15,'I','D','L',':','m','t','u','t'
  ,'/','m','2','t','G','u','t','s',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x10,':',':','m','t','u','t',':',':','m'
  ,'2','t','G','u','t','s',0x00,0x00,0x00,0x00,0x01,0x00,0x00,
  0x00,0x08,'n','o','t','h','i','n','g',0x00,0x00,0x00,0x00,0x0a
  ,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x04,'a','v','i',0x00,0x00
  ,0x00,0x00,0x0f,0x00,0x00,0x00,'H',0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x15,'I','D','L',':','m','t','u','t','/','a','v','i'
  ,'G','u','t','s',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x10,':',':','m','t','u','t',':',':','a','v','i','G','u'
  ,'t','s',0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x08,'n','o'
  ,'t','h','i','n','g',0x00,0x00,0x00,0x00,0x0a};

yotk* mtut_specifics__getTC(void)
{
  return (yotk*)mtut_specifics__tc;
}

void mtut_specifics__free( mtut_specifics* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_specifics, (dvoid *)val, ffunc);
}

void mtut_specifics__copy( mtut_specifics* dest, mtut_specifics* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mtut_specifics, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_header__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x02,0xc8,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x14,'I','D','L',':','m','t','u','t','/','h'
  ,'e','a','d','e','r',':','1','.','0',0x00,0x00,0x00,0x00,0x0f
  ,':',':','m','t','u','t',':',':','h','e','a','d','e','r',0x00
  ,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x0b,'b','i','t','s'
  ,'P','e','r','S','e','c',0x00,0x00,0x00,0x00,0x00,0x05,0x00,
  0x00,0x00,0x0f,'h','e','i','g','h','t','I','n','P','i','x','e'
  ,'l','s',0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x0e,'w'
  ,'i','d','t','h','I','n','P','i','x','e','l','s',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x0f,'p','e','l','A','s'
  ,'p','e','c','t','R','a','t','i','o',0x00,0x00,0x00,0x00,0x00
  ,0x03,0x00,0x00,0x00,0x11,'f','r','a','m','e','s','P','e','r'
  ,'K','i','l','o','S','e','c',0x00,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x05,0x00,0x00,0x00,0x0d,'l','a','s','t','T','i','m','e','S'
  ,'e','e','n',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x00
  ,0x00,0x05,'a','l','s','o',0x00,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x10,0x00,0x00,0x01,0xd0,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x17,'I','D','L',':','m','t','u','t','/','s','p','e','c','i'
  ,'f','i','c','s',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x12
  ,':',':','m','t','u','t',':',':','s','p','e','c','i','f','i'
  ,'c','s',0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,'k'
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,'I','D','L',':','m'
  ,'t','u','t','/','f','m','t',':','1','.','0',0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x0c,':',':','m','t','u','t',':',':','f'
  ,'m','t',0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x09,'f','m'
  ,'t','E','r','r','o','r',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0b,'f','m','t','M','p','e','g','1','S','S',0x00,0x00,0x00,
  0x00,0x00,0x0a,'f','m','t','M','p','e','g','2','T',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x07,'f','m','t','A','V','I',0x00,0x00,
  0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x01,
  0x00,0x00,0x00,0x05,'m','1','s','s',0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x0f,0x00,0x00,0x00,'L',0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x16,'I','D','L',':','m','t','u','t','/','m','1','s'
  ,'s','G','u','t','s',':','1','.','0',0x00,0x00,0x00,0x00,0x00
  ,0x00,0x11,':',':','m','t','u','t',':',':','m','1','s','s','G'
  ,'u','t','s',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00
  ,0x00,0x08,'n','o','t','h','i','n','g',0x00,0x00,0x00,0x00,0x0a
  ,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x04,'m','2','t',0x00,0x00
  ,0x00,0x00,0x0f,0x00,0x00,0x00,'H',0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x15,'I','D','L',':','m','t','u','t','/','m','2','t'
  ,'G','u','t','s',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x10,':',':','m','t','u','t',':',':','m','2','t','G','u'
  ,'t','s',0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x08,'n','o'
  ,'t','h','i','n','g',0x00,0x00,0x00,0x00,0x0a,0x00,0x00,0x00
  ,0x03,0x00,0x00,0x00,0x04,'a','v','i',0x00,0x00,0x00,0x00,0x0f
  ,0x00,0x00,0x00,'H',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x15,
  'I','D','L',':','m','t','u','t','/','a','v','i','G','u','t',
  's',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,
  ':',':','m','t','u','t',':',':','a','v','i','G','u','t','s',
  0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x08,'n','o','t','h'
  ,'i','n','g',0x00,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x08,'c'
  ,'m','p','D','a','t','a',0x00,0x00,0x00,0x00,0x13,0x00,0x00,
  0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x00,0x00,
  0x00,0x00};

yotk* mtut_header__getTC(void)
{
  return (yotk*)mtut_header__tc;
}

void mtut_header__free( mtut_header* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_header, (dvoid *)val, ffunc);
}

void mtut_header__copy( mtut_header* dest, mtut_header* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_header, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_tagList__tc[] = 
  {0x00,0x00,0x00,0x15,0x00,0x00,0x00,'H',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','m','t','u','t','/','t',
  'a','g','L','i','s','t',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,':',':','m','t','u','t',':',':','t','a',
  'g','L','i','s','t',0x00,0x00,0x00,0x00,0x13,0x00,0x00,0x00,
  0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,
  0x00};

yotk* mtut_tagList__getTC(void)
{
  return (yotk*)mtut_tagList__tc;
}

void mtut_tagList__free( mtut_tagList* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_tagList, (dvoid *)val, ffunc);
}

void mtut_tagList__copy( mtut_tagList* dest, mtut_tagList* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_mtut_tagList, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk mtut_state__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,'}',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x13,'I','D','L',':','m','t','u','t','/','s',
  't','a','t','e',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x0e
  ,':',':','m','t','u','t',':',':','s','t','a','t','e',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x0d,'s','t','a','t'
  ,'e','U','n','k','n','o','w','n',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x0a,'s','t','a','t','e','I','d','l','e',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x0d,'s','t','a','t','e','E','l','H','o','s'
  ,'e','d',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,'s','t','a'
  ,'t','e','W','o','r','k','i','n','g',0x00};

yotk* mtut_state__getTC(void)
{
  return (yotk*)mtut_state__tc;
}

void mtut_state__free( mtut_state* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_state, (dvoid *)val, ffunc);
}

void mtut_state__copy( mtut_state* dest, mtut_state* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_state, (dvoid *)dest, (dvoid *)src, afunc);
}

/* Client stubs for interface ::mtut::grab */
static ysidDecl(mtut_grab___id) = "IDL:mtut/grab:1.0";

CONST ysid* mtut_grab__getId(void)
{
  return (CONST ysid*)mtut_grab___id;
}

static CONST_DATA yotk mtut_grab__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'-',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x12,'I','D','L',':','m','t','u','t','/','g',
  'r','a','b',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x0d
  ,':',':','m','t','u','t',':',':','g','r','a','b',0x00};

yotk* mtut_grab__getTC(void)
{
  return (yotk*)mtut_grab__tc;
}


void mtut_grab__free( mtut_grab* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_grab, (dvoid *)val, ffunc);
}

void mtut_grab__copy( mtut_grab* dest, mtut_grab* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_grab, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* mtut_grab_porQua__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtut_grab_porQua");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtut_grab_porQua", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_OUT;
    _pars[0].tk = (yotk*)YCTC_mtut_state;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mtut_grab_porQua( mtut_grab or, yoenv* ev, mtut_state* status)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mtut_grab__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtut_grab__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mtut_grab__id);
      (*(void (*)( mtut_grab, yoenv*, mtut_state*))_f)(or, ev, status);
    }
    else
      (*_impl->porQua)(or, ev, status);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtut_grab_porQua_nw( or, ev, status, (ysevt*)_sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mtut_grab_porQua_nw( mtut_grab or, yoenv* ev, mtut_state* status, 
  ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) status;
  yoSendReq( (dvoid *)or, ev, "porQua", TRUE, _sem, (sword)1, 
    mtut_grab_porQua_pars, _parvec);
}

yopar* mtut_grab_tags__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtut_grab_tags");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtut_grab_tags", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_OUT;
    _pars[0].tk = (yotk*)YCTC_mtut_header;
    _pars[1].mode = YOMODE_OUT;
    _pars[1].tk = (yotk*)YCTC_mtut_tagList;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_mtut_slave;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mtut_grab_tags( mtut_grab or, yoenv* ev, mtut_header* hdr, 
  mtut_tagList* tags)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mtut_grab__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtut_grab__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, mtut_grab__id);
      (*(void (*)( mtut_grab, yoenv*, mtut_header*, mtut_tagList*))_f)(or, 
        ev, hdr, tags);
    }
    else
      (*_impl->tags)(or, ev, hdr, tags);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtut_grab_tags_nw( or, ev, hdr, tags, (ysevt*)_sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mtut_grab_tags_nw( mtut_grab or, yoenv* ev, mtut_header* hdr, 
  mtut_tagList* tags, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *) hdr;
  _parvec[1] = (dvoid *) tags;
  yoSendReq( (dvoid *)or, ev, "tags", TRUE, _sem, (sword)2, 
    mtut_grab_tags_pars, _parvec);
}


static CONST_DATA yotk mtut_grabList__tc[] = 
  {0x00,0x00,0x00,0x15,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x16,'I','D','L',':','m','t','u','t','/','g'
  ,'r','a','b','L','i','s','t',':','1','.','0',0x00,0x00,0x00,
  0x00,0x00,0x00,0x11,':',':','m','t','u','t',':',':','g','r',
  'a','b','L','i','s','t',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13
  ,0x00,0x00,0x00,'@',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,
  0x00,0x00,0x00,'-',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,'I'
  ,'D','L',':','m','t','u','t','/','g','r','a','b',':','1','.'
  ,'0',0x00,0x00,0x00,0x00,0x00,0x00,0x0d,':',':','m','t','u',
  't',':',':','g','r','a','b',0x00,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00};

yotk* mtut_grabList__getTC(void)
{
  return (yotk*)mtut_grabList__tc;
}

void mtut_grabList__free( mtut_grabList* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_grabList, (dvoid *)val, ffunc);
}

void mtut_grabList__copy( mtut_grabList* dest, mtut_grabList* src, ysmaf 
  afunc)
{
  yotkCopyVal( YCTC_mtut_grabList, (dvoid *)dest, (dvoid *)src, afunc);
}

/* Client stubs for interface ::mtut::parse */
static ysidDecl(mtut_parse___id) = "IDL:mtut/parse:1.0";

CONST ysid* mtut_parse__getId(void)
{
  return (CONST ysid*)mtut_parse___id;
}

static CONST_DATA yotk mtut_parse__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'.',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x13,'I','D','L',':','m','t','u','t','/','p',
  'a','r','s','e',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x0e
  ,':',':','m','t','u','t',':',':','p','a','r','s','e',0x00};

yotk* mtut_parse__getTC(void)
{
  return (yotk*)mtut_parse__tc;
}


void mtut_parse__free( mtut_parse* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_parse, (dvoid *)val, ffunc);
}

void mtut_parse__copy( mtut_parse* dest, mtut_parse* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_parse, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* mtut_parse_doIt__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtut_parse_doIt");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtut_parse_doIt", (ub4)9);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)yoTcString;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcString;
    _pars[2].mode = YOMODE_IN;
    _pars[2].tk = (yotk*)yoTcUlong;
    _pars[3].mode = YOMODE_IN;
    _pars[3].tk = (yotk*)yoTcBoolean;
    _pars[4].mode = YOMODE_IN;
    _pars[4].tk = (yotk*)yoTcLongLong;
    _pars[5].mode = YOMODE_IN;
    _pars[5].tk = (yotk*)yoTcLongLong;
    _pars[6].mode = YOMODE_IN;
    _pars[6].tk = (yotk*)YCTC_yoevt;
    _pars[7].mode = YOMODE_OUT;
    _pars[7].tk = (yotk*)YCTC_mtut_grab;
    _pars[8].mode = YOMODE_EXCEPT;
    _pars[8].tk = (yotk*)YCTC_mtut_slave;
    _pars[9].mode = YOMODE_INVALID;
    _pars[9].tk = (yotk*)yoTcNull;
    _pars[9].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mtut_parse_doIt( mtut_parse or, yoenv* ev, char* srcFileName, char* 
  cntOutName, ub4 frameTypes, boolean ignoreError, sysb8 startOffset, sysb8 
  endOffset, yoevt completionEvt, mtut_grab* resultOR)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mtut_parse__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtut_parse__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mtut_parse__id);
      (*(void (*)( mtut_parse, yoenv*, char*, char*, ub4, boolean, sysb8, 
        sysb8, yoevt, mtut_grab*))_f)(or, ev, srcFileName, cntOutName, 
        frameTypes, ignoreError, startOffset, endOffset, completionEvt, 
        resultOR);
    }
    else
      (*_impl->doIt)(or, ev, srcFileName, cntOutName, frameTypes, 
        ignoreError, startOffset, endOffset, completionEvt, resultOR);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtut_parse_doIt_nw( or, ev, srcFileName, cntOutName, frameTypes, 
        ignoreError, startOffset, endOffset, completionEvt, resultOR, (
        ysevt*)_sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mtut_parse_doIt_nw( mtut_parse or, yoenv* ev, char* srcFileName, char* 
  cntOutName, ub4 frameTypes, boolean ignoreError, sysb8 startOffset, sysb8 
  endOffset, yoevt completionEvt, mtut_grab* resultOR, ysevt* _sem)
{
  dvoid * _parvec[8];

  _parvec[0] = (dvoid *)&srcFileName;
  _parvec[1] = (dvoid *)&cntOutName;
  _parvec[2] = (dvoid *)&frameTypes;
  _parvec[3] = (dvoid *)&ignoreError;
  _parvec[4] = (dvoid *)&startOffset;
  _parvec[5] = (dvoid *)&endOffset;
  _parvec[6] = (dvoid *)&completionEvt;
  _parvec[7] = (dvoid *) resultOR;
  yoSendReq( (dvoid *)or, ev, "doIt", TRUE, _sem, (sword)8, 
    mtut_parse_doIt_pars, _parvec);
}

yopar* mtut_parse_whazzup__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtut_parse_whazzup");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtut_parse_whazzup", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_OUT;
    _pars[0].tk = (yotk*)YCTC_mtut_state;
    _pars[1].mode = YOMODE_OUT;
    _pars[1].tk = (yotk*)YCTC_mtut_grabList;
    _pars[2].mode = YOMODE_OUT;
    _pars[2].tk = (yotk*)YCTC_mtut_grabList;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mtut_parse_whazzup( mtut_parse or, yoenv* ev, mtut_state* status, 
  mtut_grabList* working, mtut_grabList* pending)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mtut_parse__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtut_parse__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, mtut_parse__id);
      (*(void (*)( mtut_parse, yoenv*, mtut_state*, mtut_grabList*, 
        mtut_grabList*))_f)(or, ev, status, working, pending);
    }
    else
      (*_impl->whazzup)(or, ev, status, working, pending);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtut_parse_whazzup_nw( or, ev, status, working, pending, (ysevt*)_sem)
        ;
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mtut_parse_whazzup_nw( mtut_parse or, yoenv* ev, mtut_state* status, 
  mtut_grabList* working, mtut_grabList* pending, ysevt* _sem)
{
  dvoid * _parvec[3];

  _parvec[0] = (dvoid *) status;
  _parvec[1] = (dvoid *) working;
  _parvec[2] = (dvoid *) pending;
  yoSendReq( (dvoid *)or, ev, "whazzup", TRUE, _sem, (sword)3, 
    mtut_parse_whazzup_pars, _parvec);
}

yopar* mtut_parse_quit__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtut_parse_quit");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtut_parse_quit", (ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mtut_parse_quit( mtut_parse or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mtut_parse__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtut_parse__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, mtut_parse__id);
      (*(void (*)( mtut_parse, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->quit)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtut_parse_quit_nw( or, ev, (ysevt*)_sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mtut_parse_quit_nw( mtut_parse or, yoenv* ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "quit", TRUE, _sem, (sword)0, 
    mtut_parse_quit_pars, (dvoid **)0);
}


/* Client stubs for interface ::mtut::mstr */
static ysidDecl(mtut_mstr___id) = "IDL:mtut/mstr:1.0";

CONST ysid* mtut_mstr__getId(void)
{
  return (CONST ysid*)mtut_mstr___id;
}

static CONST_DATA yotk mtut_mstr__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'-',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x12,'I','D','L',':','m','t','u','t','/','m',
  's','t','r',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x0d
  ,':',':','m','t','u','t',':',':','m','s','t','r',0x00};

yotk* mtut_mstr__getTC(void)
{
  return (yotk*)mtut_mstr__tc;
}


void mtut_mstr__free( mtut_mstr* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_mtut_mstr, (dvoid *)val, ffunc);
}

void mtut_mstr__copy( mtut_mstr* dest, mtut_mstr* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_mtut_mstr, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* mtut_mstr_hello__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "mtut_mstr_hello");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "mtut_mstr_hello", (ub4)2);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_mtut_parse;
    _pars[1].mode = YOMODE_OUT;
    _pars[1].tk = (yotk*)YCTC_yoevt;
    _pars[2].mode = YOMODE_INVALID;
    _pars[2].tk = (yotk*)yoTcNull;
    _pars[2].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void mtut_mstr_hello( mtut_mstr or, yoenv* ev, mtut_parse slaveOR, yoevt* 
  activated)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct mtut_mstr__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct mtut_mstr__tyimpl*) yoLocalObj( (CORBA_Object)or, (
    yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, mtut_mstr__id);
      (*(void (*)( mtut_mstr, yoenv*, mtut_parse, yoevt*))_f)(or, ev, 
        slaveOR, activated);
    }
    else
      (*_impl->hello)(or, ev, slaveOR, activated);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      mtut_mstr_hello_nw( or, ev, slaveOR, activated, (ysevt*)_sem);
    }
    yseCatchAll
    {
      CONST ysid* _exid;
      dvoid * _exbody;

      _exid = yseExid;
      _exbody = yseExobj;
      yseTry
      {
        yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, _exid, _exbody);
      }
      yseFinally
      {
        if ( _sem )
          ysSemDestroy( (ysevt*)_sem);
      }
      yseEnd
      yseRethrow;
    }
    yseEnd
    yseTry
    {
      ysSemSynch( (ysevt*)_sem, (dvoid *)0);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
}

void mtut_mstr_hello_nw( mtut_mstr or, yoenv* ev, mtut_parse slaveOR, 
  yoevt* activated, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *)&slaveOR;
  _parvec[1] = (dvoid *) activated;
  yoSendReq( (dvoid *)or, ev, "hello", TRUE, _sem, (sword)2, 
    mtut_mstr_hello_pars, _parvec);
}



EXTC_END
