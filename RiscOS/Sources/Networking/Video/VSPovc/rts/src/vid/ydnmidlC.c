/* GENERATED FILE
 * ydnmidl - client stubs
 * from /vobs/mx/pub/ydnmidl.idl
 */

#ifndef SYSI_ORACLE
#include <sysi.h>
#endif
#ifndef YO_ORACLE
#include <yo.h>
#endif

#ifndef YDNMIDL_IDL
#include <ydnmidl.h>
#endif

EXTC_START

static CONST_DATA yotk ydnmNameComponent__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'d',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1a,'I','D','L',':','y','d','n','m','N','a',
  'm','e','C','o','m','p','o','n','e','n','t',':','1','.','0',
  0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','d','n','m','N'
  ,'a','m','e','C','o','m','p','o','n','e','n','t',0x00,0x00,0x00
  ,0x00,0x02,0x00,0x00,0x00,0x03,'i','d',0x00,0x00,0x00,0x00,0x00
  ,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,'k','i','n','d'
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00
  };

yotk* ydnmNameComponent__getTC(void)
{
  return (yotk*)ydnmNameComponent__tc;
}

void ydnmNameComponent__free( ydnmNameComponent* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmNameComponent, (dvoid *)val, ffunc);
}

void ydnmNameComponent__copy( ydnmNameComponent* dest, ydnmNameComponent* 
  src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmNameComponent, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk ydnmName__tc[] = 
  {0x00,0x00,0x00,0x15,0x00,0x00,0x00,0xa8,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x11,'I','D','L',':','y','d','n','m','N','a'
  ,'m','e',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0b,':',':','y','d','n','m','N','a','m','e',0x00,0x00,0x00,
  0x00,0x00,0x13,0x00,0x00,0x00,'t',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x0f,0x00,0x00,0x00,'d',0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x1a,'I','D','L',':','y','d','n','m','N','a','m','e','C'
  ,'o','m','p','o','n','e','n','t',':','1','.','0',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x14,':',':','y','d','n','m','N','a','m','e'
  ,'C','o','m','p','o','n','e','n','t',0x00,0x00,0x00,0x00,0x02
  ,0x00,0x00,0x00,0x03,'i','d',0x00,0x00,0x00,0x00,0x00,0x12,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x05,'k','i','n','d',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00};

yotk* ydnmName__getTC(void)
{
  return (yotk*)ydnmName__tc;
}

void ydnmName__free( ydnmName* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmName, (dvoid *)val, ffunc);
}

void ydnmName__copy( ydnmName* dest, ydnmName* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmName, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk ydnmBindingType__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x18,'I','D','L',':','y','d','n','m','B','i'
  ,'n','d','i','n','g','T','y','p','e',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x12,':',':','y','d','n','m','B','i','n','d','i',
  'n','g','T','y','p','e',0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00
  ,0x00,0x00,0x0b,'y','d','n','m','O','b','j','e','c','t',0x00
  ,0x00,0x00,0x00,0x00,0x0c,'y','d','n','m','C','o','n','t','e'
  ,'x','t',0x00};

yotk* ydnmBindingType__getTC(void)
{
  return (yotk*)ydnmBindingType__tc;
}

void ydnmBindingType__free( ydnmBindingType* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmBindingType, (dvoid *)val, ffunc);
}

void ydnmBindingType__copy( ydnmBindingType* dest, ydnmBindingType* src, 
  ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmBindingType, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk ydnmBinding__tc[] = 
  {0x00,0x00,0x00,0x0f,0x00,0x00,0x01,'4',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x14,'I','D','L',':','y','d','n','m','B','i',
  'n','d','i','n','g',':','1','.','0',0x00,0x00,0x00,0x00,0x0e
  ,':',':','y','d','n','m','B','i','n','d','i','n','g',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x11,'n','a','m','e'
  ,'_','y','d','n','m','B','i','n','d','i','n','g',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'d',0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x1a,'I','D','L',':','y','d','n','m','N'
  ,'a','m','e','C','o','m','p','o','n','e','n','t',':','1','.'
  ,'0',0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','d','n',
  'm','N','a','m','e','C','o','m','p','o','n','e','n','t',0x00
  ,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,'i','d',0x00,0x00,0x00
  ,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,'k',
  'i','n','d',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x11,'t','y','p','e','_','y','d','n'
  ,'m','B','i','n','d','i','n','g',0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x11,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x18,'I','D','L',':','y','d','n','m','B','i','n','d','i'
  ,'n','g','T','y','p','e',':','1','.','0',0x00,0x00,0x00,0x00
  ,0x12,':',':','y','d','n','m','B','i','n','d','i','n','g','T'
  ,'y','p','e',0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00
  ,0x0b,'y','d','n','m','O','b','j','e','c','t',0x00,0x00,0x00
  ,0x00,0x00,0x0c,'y','d','n','m','C','o','n','t','e','x','t',
  0x00};

yotk* ydnmBinding__getTC(void)
{
  return (yotk*)ydnmBinding__tc;
}

void ydnmBinding__free( ydnmBinding* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmBinding, (dvoid *)val, ffunc);
}

void ydnmBinding__copy( ydnmBinding* dest, ydnmBinding* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmBinding, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk ydnmBindingList__tc[] = 
  {0x00,0x00,0x00,0x15,0x00,0x00,0x01,0x84,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x18,'I','D','L',':','y','d','n','m','B','i'
  ,'n','d','i','n','g','L','i','s','t',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x12,':',':','y','d','n','m','B','i','n','d','i',
  'n','g','L','i','s','t',0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x00
  ,0x00,0x01,'D',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x00,
  0x00,0x01,'4',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x14,'I','D'
  ,'L',':','y','d','n','m','B','i','n','d','i','n','g',':','1'
  ,'.','0',0x00,0x00,0x00,0x00,0x0e,':',':','y','d','n','m','B'
  ,'i','n','d','i','n','g',0x00,0x00,0x00,0x00,0x00,0x00,0x02,
  0x00,0x00,0x00,0x11,'n','a','m','e','_','y','d','n','m','B',
  'i','n','d','i','n','g',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f
  ,0x00,0x00,0x00,'d',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1a,
  'I','D','L',':','y','d','n','m','N','a','m','e','C','o','m',
  'p','o','n','e','n','t',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x14,':',':','y','d','n','m','N','a','m','e','C','o'
  ,'m','p','o','n','e','n','t',0x00,0x00,0x00,0x00,0x02,0x00,0x00
  ,0x00,0x03,'i','d',0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x00,0x05,'k','i','n','d',0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11
  ,'t','y','p','e','_','y','d','n','m','B','i','n','d','i','n'
  ,'g',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,
  0x5c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,'I','D','L',':'
  ,'y','d','n','m','B','i','n','d','i','n','g','T','y','p','e'
  ,':','1','.','0',0x00,0x00,0x00,0x00,0x12,':',':','y','d','n'
  ,'m','B','i','n','d','i','n','g','T','y','p','e',0x00,0x00,0x00
  ,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x0b,'y','d','n','m','O'
  ,'b','j','e','c','t',0x00,0x00,0x00,0x00,0x00,0x0c,'y','d','n'
  ,'m','C','o','n','t','e','x','t',0x00,0x00,0x00,0x00,0x00};

yotk* ydnmBindingList__getTC(void)
{
  return (yotk*)ydnmBindingList__tc;
}

void ydnmBindingList__free( ydnmBindingList* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmBindingList, (dvoid *)val, ffunc);
}

void ydnmBindingList__copy( ydnmBindingList* dest, ydnmBindingList* src, 
  ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmBindingList, (dvoid *)dest, (dvoid *)src, afunc);
}

static CONST_DATA yotk ydnmNotFoundReason__tc[] = 
  {0x00,0x00,0x00,0x11,0x00,0x00,0x00,'w',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1b,'I','D','L',':','y','d','n','m','N','o',
  't','F','o','u','n','d','R','e','a','s','o','n',':','1','.',
  '0',0x00,0x00,0x00,0x00,0x00,0x15,':',':','y','d','n','m','N'
  ,'o','t','F','o','u','n','d','R','e','a','s','o','n',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x0d,'m','i','s'
  ,'s','i','n','g','_','n','o','d','e',0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x0c,'n','o','t','_','c','o','n','t','e','x','t',
  0x00,0x00,0x00,0x00,0x0b,'n','o','t','_','o','b','j','e','c'
  ,'t',0x00};

yotk* ydnmNotFoundReason__getTC(void)
{
  return (yotk*)ydnmNotFoundReason__tc;
}

void ydnmNotFoundReason__free( ydnmNotFoundReason* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmNotFoundReason, (dvoid *)val, ffunc);
}

void ydnmNotFoundReason__copy( ydnmNotFoundReason* dest, 
  ydnmNotFoundReason* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmNotFoundReason, (dvoid *)dest, (dvoid *)src, afunc);
    
}

static ysidDecl(ydnmNotFound___id) = "IDL:ydnmNotFound:1.0";

CONST ysid* ydnmNotFound__getId(void)
{
  return (CONST ysid*)ydnmNotFound___id;
}

static CONST_DATA yotk ydnmNotFound__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x01,0x98,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x15,'I','D','L',':','y','d','n','m','N','o'
  ,'t','F','o','u','n','d',':','1','.','0',0x00,0x00,0x00,0x00
  ,0x00,0x00,0x00,0x0f,':',':','y','d','n','m','N','o','t','F'
  ,'o','u','n','d',0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00
  ,0x11,'w','h','y','_','y','d','n','m','N','o','t','F','o','u'
  ,'n','d',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00
  ,'w',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1b,'I','D','L',':'
  ,'y','d','n','m','N','o','t','F','o','u','n','d','R','e','a'
  ,'s','o','n',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x15,':'
  ,':','y','d','n','m','N','o','t','F','o','u','n','d','R','e'
  ,'a','s','o','n',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00
  ,0x00,0x00,0x0d,'m','i','s','s','i','n','g','_','n','o','d',
  'e',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,'n','o','t','_',
  'c','o','n','t','e','x','t',0x00,0x00,0x00,0x00,0x0b,'n','o'
  ,'t','_','o','b','j','e','c','t',0x00,0x00,0x00,0x00,0x00,0x12
  ,'r','e','s','t','_','y','d','n','m','N','o','t','F','o','u'
  ,'n','d',0x00,0x00,0x00,0x00,0x00,0x00,0x15,0x00,0x00,0x00,0xa8
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,'I','D','L',':','y'
  ,'d','n','m','N','a','m','e',':','1','.','0',0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x0b,':',':','y','d','n','m','N','a','m'
  ,'e',0x00,0x00,0x00,0x00,0x00,0x13,0x00,0x00,0x00,'t',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,'d',0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x1a,'I','D','L',':','y','d','n','m'
  ,'N','a','m','e','C','o','m','p','o','n','e','n','t',':','1'
  ,'.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','d',
  'n','m','N','a','m','e','C','o','m','p','o','n','e','n','t',
  0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x03,'i','d',0x00,0x00
  ,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05
  ,'k','i','n','d',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

yotk* ydnmNotFound__getTC(void)
{
  return (yotk*)ydnmNotFound__tc;
}

void ydnmNotFound__free( ydnmNotFound* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmNotFound, (dvoid *)val, ffunc);
}

void ydnmNotFound__copy( ydnmNotFound* dest, ydnmNotFound* src, ysmaf afunc)
  
{
  yotkCopyVal( YCTC_ydnmNotFound, (dvoid *)dest, (dvoid *)src, afunc);
}

static ysidDecl(ydnmCannotProceed___id) = "IDL:ydnmCannotProceed:1.0";

CONST ysid* ydnmCannotProceed__getId(void)
{
  return (CONST ysid*)ydnmCannotProceed___id;
}

static CONST_DATA yotk ydnmCannotProceed__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x01,'l',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1a,'I','D','L',':','y','d','n','m','C','a',
  'n','n','o','t','P','r','o','c','e','e','d',':','1','.','0',
  0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','d','n','m','C'
  ,'a','n','n','o','t','P','r','o','c','e','e','d',0x00,0x00,0x00
  ,0x00,0x02,0x00,0x00,0x00,0x16,'c','t','x','_','y','d','n','m'
  ,'C','a','n','n','o','t','P','r','o','c','e','e','d',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'<',0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x1a,'I','D','L',':','y','d','n','m','N'
  ,'a','m','i','n','g','C','o','n','t','e','x','t',':','1','.'
  ,'0',0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','d','n',
  'm','N','a','m','i','n','g','C','o','n','t','e','x','t',0x00
  ,0x00,0x00,0x00,0x17,'r','e','s','t','_','y','d','n','m','C'
  ,'a','n','n','o','t','P','r','o','c','e','e','d',0x00,0x00,0x00
  ,0x00,0x00,0x15,0x00,0x00,0x00,0xa8,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00,0x11,'I','D','L',':','y','d','n','m','N','a','m',
  'e',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,
  ':',':','y','d','n','m','N','a','m','e',0x00,0x00,0x00,0x00,
  0x00,0x13,0x00,0x00,0x00,'t',0x00,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x0f,0x00,0x00,0x00,'d',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x1a,'I','D','L',':','y','d','n','m','N','a','m','e','C','o'
  ,'m','p','o','n','e','n','t',':','1','.','0',0x00,0x00,0x00,
  0x00,0x00,0x00,0x14,':',':','y','d','n','m','N','a','m','e',
  'C','o','m','p','o','n','e','n','t',0x00,0x00,0x00,0x00,0x02
  ,0x00,0x00,0x00,0x03,'i','d',0x00,0x00,0x00,0x00,0x00,0x12,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x05,'k','i','n','d',0x00,0x00
  ,0x00,0x00,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00,0x00
  ,0x00,0x00};

yotk* ydnmCannotProceed__getTC(void)
{
  return (yotk*)ydnmCannotProceed__tc;
}

void ydnmCannotProceed__free( ydnmCannotProceed* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmCannotProceed, (dvoid *)val, ffunc);
}

void ydnmCannotProceed__copy( ydnmCannotProceed* dest, ydnmCannotProceed* 
  src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmCannotProceed, (dvoid *)dest, (dvoid *)src, afunc);
}

static ysidDecl(ydnmInvalidName___id) = "IDL:ydnmInvalidName:1.0";

CONST ysid* ydnmInvalidName__getId(void)
{
  return (CONST ysid*)ydnmInvalidName___id;
}

static CONST_DATA yotk ydnmInvalidName__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'<',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x18,'I','D','L',':','y','d','n','m','I','n',
  'v','a','l','i','d','N','a','m','e',':','1','.','0',0x00,0x00
  ,0x00,0x00,0x12,':',':','y','d','n','m','I','n','v','a','l',
  'i','d','N','a','m','e',0x00,0x00,0x00,0x00,0x00,0x00,0x00};

yotk* ydnmInvalidName__getTC(void)
{
  return (yotk*)ydnmInvalidName__tc;
}

static ysidDecl(ydnmAlreadyBound___id) = "IDL:ydnmAlreadyBound:1.0";

CONST ysid* ydnmAlreadyBound__getId(void)
{
  return (CONST ysid*)ydnmAlreadyBound___id;
}

static CONST_DATA yotk ydnmAlreadyBound__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'@',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x19,'I','D','L',':','y','d','n','m','A','l',
  'r','e','a','d','y','B','o','u','n','d',':','1','.','0',0x00
  ,0x00,0x00,0x00,0x00,0x00,0x00,0x13,':',':','y','d','n','m',
  'A','l','r','e','a','d','y','B','o','u','n','d',0x00,0x00,0x00
  ,0x00,0x00,0x00};

yotk* ydnmAlreadyBound__getTC(void)
{
  return (yotk*)ydnmAlreadyBound__tc;
}

static ysidDecl(ydnmNotEmpty___id) = "IDL:ydnmNotEmpty:1.0";

CONST ysid* ydnmNotEmpty__getId(void)
{
  return (CONST ysid*)ydnmNotEmpty___id;
}

static CONST_DATA yotk ydnmNotEmpty__tc[] = 
  {0x00,0x00,0x00,0x16,0x00,0x00,0x00,'8',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x15,'I','D','L',':','y','d','n','m','N','o',
  't','E','m','p','t','y',':','1','.','0',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x0f,':',':','y','d','n','m','N','o','t','E',
  'm','p','t','y',0x00,0x00,0x00,0x00,0x00,0x00};

yotk* ydnmNotEmpty__getTC(void)
{
  return (yotk*)ydnmNotEmpty__tc;
}

/* Client stubs for interface ::ydnmInitialNamingContext */
static ysidDecl(ydnmInitialNamingContext___id) = 
  "IDL:ydnmInitialNamingContext:1.0";

CONST ysid* ydnmInitialNamingContext__getId(void)
{
  return (CONST ysid*)ydnmInitialNamingContext___id;
}

static CONST_DATA yotk ydnmInitialNamingContext__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'K',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,'!','I','D','L',':','y','d','n','m','I','n','i'
  ,'t','i','a','l','N','a','m','i','n','g','C','o','n','t','e'
  ,'x','t',':','1','.','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x1b,':',':','y','d','n','m','I','n','i','t','i','a','l','N'
  ,'a','m','i','n','g','C','o','n','t','e','x','t',0x00};

yotk* ydnmInitialNamingContext__getTC(void)
{
  return (yotk*)ydnmInitialNamingContext__tc;
}


void ydnmInitialNamingContext__free( ydnmInitialNamingContext* val, ysmff 
  ffunc)
{
  yotkFreeVal( YCTC_ydnmInitialNamingContext, (dvoid *)val, ffunc);
}

void ydnmInitialNamingContext__copy( ydnmInitialNamingContext* dest, 
  ydnmInitialNamingContext* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmInitialNamingContext, (dvoid *)dest, (dvoid *)src, 
    afunc);
}


yopar* ydnmInitialNamingContext_get__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmInitialNamingContext_get");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmInitialNamingContext_get", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_ydnmNamingContext;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

ydnmNamingContext ydnmInitialNamingContext_get( ydnmInitialNamingContext or,
   yoenv* ev)
{
  ydnmNamingContext _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmInitialNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmInitialNamingContext__tyimpl*) yoLocalObj( (
    CORBA_Object)or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, 
        ydnmInitialNamingContext__id);
      _result = (*(ydnmNamingContext (*)( ydnmInitialNamingContext, yoenv*))
        _f)(or, ev);
    }
    else
      _result = (*_impl->get)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmInitialNamingContext_get_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmInitialNamingContext_get_nw( ydnmInitialNamingContext or, yoenv* 
  ev, ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "get", TRUE, _sem, (sword)0, 
    ydnmInitialNamingContext_get_pars, (dvoid **)0);
}


/* Client stubs for interface ::ydnmNamingContext */
static ysidDecl(ydnmNamingContext___id) = "IDL:ydnmNamingContext:1.0";

CONST ysid* ydnmNamingContext__getId(void)
{
  return (CONST ysid*)ydnmNamingContext___id;
}

static CONST_DATA yotk ydnmNamingContext__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'<',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1a,'I','D','L',':','y','d','n','m','N','a',
  'm','i','n','g','C','o','n','t','e','x','t',':','1','.','0',
  0x00,0x00,0x00,0x00,0x00,0x00,0x14,':',':','y','d','n','m','N'
  ,'a','m','i','n','g','C','o','n','t','e','x','t',0x00};

yotk* ydnmNamingContext__getTC(void)
{
  return (yotk*)ydnmNamingContext__tc;
}


void ydnmNamingContext__free( ydnmNamingContext* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmNamingContext, (dvoid *)val, ffunc);
}

void ydnmNamingContext__copy( ydnmNamingContext* dest, ydnmNamingContext* 
  src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmNamingContext, (dvoid *)dest, (dvoid *)src, afunc);
}


yopar* ydnmNamingContext_bind__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_bind");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_bind", (ub4)6);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_ydnmName;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcObject;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_ydnmNotFound;
    _pars[3].mode = YOMODE_EXCEPT;
    _pars[3].tk = (yotk*)YCTC_ydnmCannotProceed;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_ydnmInvalidName;
    _pars[5].mode = YOMODE_EXCEPT;
    _pars[5].tk = (yotk*)YCTC_ydnmAlreadyBound;
    _pars[6].mode = YOMODE_INVALID;
    _pars[6].tk = (yotk*)yoTcNull;
    _pars[6].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void ydnmNamingContext_bind( ydnmNamingContext or, yoenv* ev, ydnmName* n, 
  CORBA_Object obj)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, ydnmNamingContext__id);
      (*(void (*)( ydnmNamingContext, yoenv*, ydnmName*, CORBA_Object))_f)(
        or, ev, n, obj);
    }
    else
      (*_impl->bind)(or, ev, n, obj);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_bind_nw( or, ev, n, obj, (ysevt*)_sem);
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

void ydnmNamingContext_bind_nw( ydnmNamingContext or, yoenv* ev, ydnmName* 
  n, CORBA_Object obj, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *) n;
  _parvec[1] = (dvoid *)&obj;
  yoSendReq( (dvoid *)or, ev, "bind", TRUE, _sem, (sword)2, 
    ydnmNamingContext_bind_pars, _parvec);
}

yopar* ydnmNamingContext_rebind__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_rebind");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_rebind", (ub4)5);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_ydnmName;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcObject;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_ydnmNotFound;
    _pars[3].mode = YOMODE_EXCEPT;
    _pars[3].tk = (yotk*)YCTC_ydnmCannotProceed;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_ydnmInvalidName;
    _pars[5].mode = YOMODE_INVALID;
    _pars[5].tk = (yotk*)yoTcNull;
    _pars[5].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void ydnmNamingContext_rebind( ydnmNamingContext or, yoenv* ev, ydnmName* n,
   CORBA_Object obj)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, ydnmNamingContext__id);
      (*(void (*)( ydnmNamingContext, yoenv*, ydnmName*, CORBA_Object))_f)(
        or, ev, n, obj);
    }
    else
      (*_impl->rebind)(or, ev, n, obj);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_rebind_nw( or, ev, n, obj, (ysevt*)_sem);
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

void ydnmNamingContext_rebind_nw( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n, CORBA_Object obj, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *) n;
  _parvec[1] = (dvoid *)&obj;
  yoSendReq( (dvoid *)or, ev, "rebind", TRUE, _sem, (sword)2, 
    ydnmNamingContext_rebind_pars, _parvec);
}

yopar* ydnmNamingContext_bind_context__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_bind_context");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_bind_context", (ub4)6);
      
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)YCTC_ydnmName;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcObject;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_ydnmNotFound;
    _pars[3].mode = YOMODE_EXCEPT;
    _pars[3].tk = (yotk*)YCTC_ydnmCannotProceed;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_ydnmInvalidName;
    _pars[5].mode = YOMODE_EXCEPT;
    _pars[5].tk = (yotk*)YCTC_ydnmAlreadyBound;
    _pars[6].mode = YOMODE_INVALID;
    _pars[6].tk = (yotk*)yoTcNull;
    _pars[6].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void ydnmNamingContext_bind_context( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n, CORBA_Object obj)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, ydnmNamingContext__id);
      (*(void (*)( ydnmNamingContext, yoenv*, ydnmName*, CORBA_Object))_f)(
        or, ev, n, obj);
    }
    else
      (*_impl->bind_context)(or, ev, n, obj);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_bind_context_nw( or, ev, n, obj, (ysevt*)_sem);
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

void ydnmNamingContext_bind_context_nw( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n, CORBA_Object obj, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *) n;
  _parvec[1] = (dvoid *)&obj;
  yoSendReq( (dvoid *)or, ev, "bind_context", TRUE, _sem, (sword)2, 
    ydnmNamingContext_bind_context_pars, _parvec);
}

yopar* ydnmNamingContext_resolve__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_resolve");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_resolve", (ub4)5);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcObject;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)YCTC_ydnmName;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_ydnmNotFound;
    _pars[3].mode = YOMODE_EXCEPT;
    _pars[3].tk = (yotk*)YCTC_ydnmCannotProceed;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_ydnmInvalidName;
    _pars[5].mode = YOMODE_INVALID;
    _pars[5].tk = (yotk*)yoTcNull;
    _pars[5].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

CORBA_Object ydnmNamingContext_resolve( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n)
{
  CORBA_Object _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)3, (dvoid *)_impl, ydnmNamingContext__id);
      _result = (*(CORBA_Object (*)( ydnmNamingContext, yoenv*, ydnmName*))
        _f)(or, ev, n);
    }
    else
      _result = (*_impl->resolve)(or, ev, n);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_resolve_nw( or, ev, n, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmNamingContext_resolve_nw( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) n;
  yoSendReq( (dvoid *)or, ev, "resolve", TRUE, _sem, (sword)1, 
    ydnmNamingContext_resolve_pars, _parvec);
}

yopar* ydnmNamingContext_unbind__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_unbind");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_unbind", (ub4)5);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcObject;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)YCTC_ydnmName;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_ydnmNotFound;
    _pars[3].mode = YOMODE_EXCEPT;
    _pars[3].tk = (yotk*)YCTC_ydnmCannotProceed;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_ydnmInvalidName;
    _pars[5].mode = YOMODE_INVALID;
    _pars[5].tk = (yotk*)yoTcNull;
    _pars[5].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

CORBA_Object ydnmNamingContext_unbind( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n)
{
  CORBA_Object _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)4, (dvoid *)_impl, ydnmNamingContext__id);
      _result = (*(CORBA_Object (*)( ydnmNamingContext, yoenv*, ydnmName*))
        _f)(or, ev, n);
    }
    else
      _result = (*_impl->unbind)(or, ev, n);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_unbind_nw( or, ev, n, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmNamingContext_unbind_nw( ydnmNamingContext or, yoenv* ev, 
  ydnmName* n, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) n;
  yoSendReq( (dvoid *)or, ev, "unbind", TRUE, _sem, (sword)1, 
    ydnmNamingContext_unbind_pars, _parvec);
}

yopar* ydnmNamingContext_new_context__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_new_context");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_new_context", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_ydnmNamingContext;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

ydnmNamingContext ydnmNamingContext_new_context( ydnmNamingContext or, 
  yoenv* ev)
{
  ydnmNamingContext _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)5, (dvoid *)_impl, ydnmNamingContext__id);
      _result = (*(ydnmNamingContext (*)( ydnmNamingContext, yoenv*))_f)(or,
         ev);
    }
    else
      _result = (*_impl->new_context)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_new_context_nw( or, ev, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmNamingContext_new_context_nw( ydnmNamingContext or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "new_context", TRUE, _sem, (sword)0, 
    ydnmNamingContext_new_context_pars, (dvoid **)0);
}

yopar* ydnmNamingContext_bind_new_context__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_bind_new_context");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_bind_new_context", (
      ub4)5);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)YCTC_ydnmNamingContext;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)YCTC_ydnmName;
    _pars[2].mode = YOMODE_EXCEPT;
    _pars[2].tk = (yotk*)YCTC_ydnmNotFound;
    _pars[3].mode = YOMODE_EXCEPT;
    _pars[3].tk = (yotk*)YCTC_ydnmCannotProceed;
    _pars[4].mode = YOMODE_EXCEPT;
    _pars[4].tk = (yotk*)YCTC_ydnmInvalidName;
    _pars[5].mode = YOMODE_INVALID;
    _pars[5].tk = (yotk*)yoTcNull;
    _pars[5].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

ydnmNamingContext ydnmNamingContext_bind_new_context( ydnmNamingContext or, 
  yoenv* ev, ydnmName* n)
{
  ydnmNamingContext _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)6, (dvoid *)_impl, ydnmNamingContext__id);
      _result = (*(ydnmNamingContext (*)( ydnmNamingContext, yoenv*, 
        ydnmName*))_f)(or, ev, n);
    }
    else
      _result = (*_impl->bind_new_context)(or, ev, n);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_bind_new_context_nw( or, ev, n, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmNamingContext_bind_new_context_nw( ydnmNamingContext or, yoenv* ev,
   ydnmName* n, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) n;
  yoSendReq( (dvoid *)or, ev, "bind_new_context", TRUE, _sem, (sword)1, 
    ydnmNamingContext_bind_new_context_pars, _parvec);
}

yopar* ydnmNamingContext_destroy__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_destroy");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_destroy", (ub4)1);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_EXCEPT;
    _pars[0].tk = (yotk*)YCTC_ydnmNotEmpty;
    _pars[1].mode = YOMODE_INVALID;
    _pars[1].tk = (yotk*)yoTcNull;
    _pars[1].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void ydnmNamingContext_destroy( ydnmNamingContext or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)7, (dvoid *)_impl, ydnmNamingContext__id);
      (*(void (*)( ydnmNamingContext, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->destroy)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_destroy_nw( or, ev, (ysevt*)_sem);
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

void ydnmNamingContext_destroy_nw( ydnmNamingContext or, yoenv* ev, ysevt* 
  _sem)
{
  yoSendReq( (dvoid *)or, ev, "destroy", TRUE, _sem, (sword)0, 
    ydnmNamingContext_destroy_pars, (dvoid **)0);
}

yopar* ydnmNamingContext_list__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmNamingContext_list");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmNamingContext_list", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_IN;
    _pars[0].tk = (yotk*)yoTcUlong;
    _pars[1].mode = YOMODE_OUT;
    _pars[1].tk = (yotk*)YCTC_ydnmBindingList;
    _pars[2].mode = YOMODE_OUT;
    _pars[2].tk = (yotk*)YCTC_ydnmBindingIterator;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void ydnmNamingContext_list( ydnmNamingContext or, yoenv* ev, ub4 count, 
  ydnmBindingList* bl, ydnmBindingIterator* bi)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmNamingContext__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmNamingContext__tyimpl*) yoLocalObj( (CORBA_Object)or, 
    (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)8, (dvoid *)_impl, ydnmNamingContext__id);
      (*(void (*)( ydnmNamingContext, yoenv*, ub4, ydnmBindingList*, 
        ydnmBindingIterator*))_f)(or, ev, count, bl, bi);
    }
    else
      (*_impl->list)(or, ev, count, bl, bi);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmNamingContext_list_nw( or, ev, count, bl, bi, (ysevt*)_sem);
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

void ydnmNamingContext_list_nw( ydnmNamingContext or, yoenv* ev, ub4 count, 
  ydnmBindingList* bl, ydnmBindingIterator* bi, ysevt* _sem)
{
  dvoid * _parvec[3];

  _parvec[0] = (dvoid *)&count;
  _parvec[1] = (dvoid *) bl;
  _parvec[2] = (dvoid *) bi;
  yoSendReq( (dvoid *)or, ev, "list", TRUE, _sem, (sword)3, 
    ydnmNamingContext_list_pars, _parvec);
}


/* Client stubs for interface ::ydnmBindingIterator */
static ysidDecl(ydnmBindingIterator___id) = "IDL:ydnmBindingIterator:1.0";

CONST ysid* ydnmBindingIterator__getId(void)
{
  return (CONST ysid*)ydnmBindingIterator___id;
}

static CONST_DATA yotk ydnmBindingIterator__tc[] = 
  {0x00,0x00,0x00,0x0e,0x00,0x00,0x00,'>',0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x1c,'I','D','L',':','y','d','n','m','B','i',
  'n','d','i','n','g','I','t','e','r','a','t','o','r',':','1',
  '.','0',0x00,0x00,0x00,0x00,0x16,':',':','y','d','n','m','B'
  ,'i','n','d','i','n','g','I','t','e','r','a','t','o','r',0x00
  };

yotk* ydnmBindingIterator__getTC(void)
{
  return (yotk*)ydnmBindingIterator__tc;
}


void ydnmBindingIterator__free( ydnmBindingIterator* val, ysmff ffunc)
{
  yotkFreeVal( YCTC_ydnmBindingIterator, (dvoid *)val, ffunc);
}

void ydnmBindingIterator__copy( ydnmBindingIterator* dest, 
  ydnmBindingIterator* src, ysmaf afunc)
{
  yotkCopyVal( YCTC_ydnmBindingIterator, (dvoid *)dest, (dvoid *)src, afunc)
    ;
}


yopar* ydnmBindingIterator_next_one__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmBindingIterator_next_one");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmBindingIterator_next_one", (ub4)2);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcBoolean;
    _pars[1].mode = YOMODE_OUT;
    _pars[1].tk = (yotk*)YCTC_ydnmBinding;
    _pars[2].mode = YOMODE_INVALID;
    _pars[2].tk = (yotk*)yoTcNull;
    _pars[2].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

boolean ydnmBindingIterator_next_one( ydnmBindingIterator or, yoenv* ev, 
  ydnmBinding* b)
{
  boolean _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmBindingIterator__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmBindingIterator__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)0, (dvoid *)_impl, ydnmBindingIterator__id)
        ;
      _result = (*(boolean (*)( ydnmBindingIterator, yoenv*, ydnmBinding*))
        _f)(or, ev, b);
    }
    else
      _result = (*_impl->next_one)(or, ev, b);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmBindingIterator_next_one_nw( or, ev, b, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmBindingIterator_next_one_nw( ydnmBindingIterator or, yoenv* ev, 
  ydnmBinding* b, ysevt* _sem)
{
  dvoid * _parvec[1];

  _parvec[0] = (dvoid *) b;
  yoSendReq( (dvoid *)or, ev, "next_one", TRUE, _sem, (sword)1, 
    ydnmBindingIterator_next_one_pars, _parvec);
}

yopar* ydnmBindingIterator_next_n__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmBindingIterator_next_n");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmBindingIterator_next_n", (ub4)3);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_RETURN;
    _pars[0].tk = (yotk*)yoTcBoolean;
    _pars[1].mode = YOMODE_IN;
    _pars[1].tk = (yotk*)yoTcUlong;
    _pars[2].mode = YOMODE_OUT;
    _pars[2].tk = (yotk*)YCTC_ydnmBindingList;
    _pars[3].mode = YOMODE_INVALID;
    _pars[3].tk = (yotk*)yoTcNull;
    _pars[3].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

boolean ydnmBindingIterator_next_n( ydnmBindingIterator or, yoenv* ev, ub4 
  count, ydnmBindingList* bl)
{
  boolean _result;
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmBindingIterator__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmBindingIterator__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)1, (dvoid *)_impl, ydnmBindingIterator__id)
        ;
      _result = (*(boolean (*)( ydnmBindingIterator, yoenv*, ub4, 
        ydnmBindingList*))_f)(or, ev, count, bl);
    }
    else
      _result = (*_impl->next_n)(or, ev, count, bl);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmBindingIterator_next_n_nw( or, ev, count, bl, (ysevt*)_sem);
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
      ysSemSynch( (ysevt*)_sem, (dvoid *)&_result);
    }
    yseFinally
      yoFilterRunEx((dvoid *)or, ev, YOFLTR_CRCV, yseExid, yseExobj);
    yseEnd
  }
  return _result;
}

void ydnmBindingIterator_next_n_nw( ydnmBindingIterator or, yoenv* ev, ub4 
  count, ydnmBindingList* bl, ysevt* _sem)
{
  dvoid * _parvec[2];

  _parvec[0] = (dvoid *)&count;
  _parvec[1] = (dvoid *) bl;
  yoSendReq( (dvoid *)or, ev, "next_n", TRUE, _sem, (sword)2, 
    ydnmBindingIterator_next_n_pars, _parvec);
}

yopar* ydnmBindingIterator_destroy__getPars(void)
{
  yopar* _pars = (yopar*)0;

  _pars = yoParsGet( "ydnmBindingIterator_destroy");
  if ( _pars == (yopar*)0 )
  {
    yopard* _desc = yoPardCreate( "ydnmBindingIterator_destroy", (ub4)0);
    _pars = _desc->pars;
    _pars[0].mode = YOMODE_INVALID;
    _pars[0].tk = (yotk*)yoTcNull;
    _pars[0].sz = (ub4)0;
    yoParsSize( _pars);
    yoParsSet( _desc);
  }
  return _pars;
}

void ydnmBindingIterator_destroy( ydnmBindingIterator or, yoenv* ev)
{
  ysevt* noreg _sem = (ysevt*)0;
  struct ydnmBindingIterator__tyimpl* _impl;
  yowiden _widen = (yowiden)0;

  NOREG(_sem);
  _impl = (struct ydnmBindingIterator__tyimpl*) yoLocalObj( (CORBA_Object)
    or, (yowiden*)&_widen);
  if ( _impl )
  {
    if ( _widen )
    {
      yogfp _f = (*_widen)( (ub4)2, (dvoid *)_impl, ydnmBindingIterator__id)
        ;
      (*(void (*)( ydnmBindingIterator, yoenv*))_f)(or, ev);
    }
    else
      (*_impl->destroy)(or, ev);
  }
  else
  {
    yoFilterRunEx((dvoid *)or, ev, YOFLTR_CSND, (CONST  char*)0, (dvoid *)0)
      ;
    yseTry
    {
      _sem = ysSemCreate((dvoid *)0);
      ydnmBindingIterator_destroy_nw( or, ev, (ysevt*)_sem);
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

void ydnmBindingIterator_destroy_nw( ydnmBindingIterator or, yoenv* ev, 
  ysevt* _sem)
{
  yoSendReq( (dvoid *)or, ev, "destroy", TRUE, _sem, (sword)0, 
    ydnmBindingIterator_destroy_pars, (dvoid **)0);
}



EXTC_END
