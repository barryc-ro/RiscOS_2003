#Makefile for Ether1 Module

CC= cc

INCLUDES=  -ITCPIPLibs:,C:
CFLAGS=  ${INCLUDES} -depend !Depend -throwback -ff -zps1 -c -pcc -zM -throwback -DKERNEL -DINET #-DDEBUG
CLIB=    c:o.stubs

SRCS=   c.if_et c.e1module c.support c.mbuf c.addcb c.svcprintf

OBJS=   o.if_et o.e1module o.support o.poduleirq o.iocopy o.handlink o.mbuf o.mlink o.addcb #o.svcprintf

Ether1:     ${OBJS} Makefile EtHdr
        cmhg EtHdr o.ethdr
        link -o rm.Ether1 -rmf o.ethdr ${OBJS} ${CLIB}
        ModSqz rm.Ether1

clean:
        remove rm.Ether1
        remove o.e1module
        remove o.if_et
        remove o.support
        remove o.poduleirq
        remove o.iocopy
        remove o.ethdr
        remove o.handlink
        remove o.mbuf
        remove o.mlink
        remove o.addcb
        remove o.svcprintf

e1module.o:   c.e1module 
            cc ${CFLAGS} c.e1module

if_et.o:    c.if_et 
            cc ${CFLAGS} c.if_et

support.o:  c.support 
            cc ${CFLAGS} c.support
            
addcb.o:  c.addcb
            cc ${CFLAGS} c.addcb
            
mbuf.o:  c.mbuf
            cc ${CFLAGS} c.mbuf
            
svcprintf.o: c.svcprintf
            cc ${CFLAGS} c.svcprintf            

o.poduleirq: s.poduleirq
       objasm -from s.poduleirq -to o.poduleirq -stamp -quit

o.iocopy: s.iocopy
       objasm -from s.iocopy -to o.iocopy -stamp -quit

o.handlink: s.handlink
        objasm -from s.handlink -to o.handlink -stamp -quit
        
o.mlink: s.mlink
        objasm -from s.mlink -to o.mlink -stamp -quit
         
# Dynamic dependencies:
