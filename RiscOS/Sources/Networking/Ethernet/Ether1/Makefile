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
o.if_et:	c.if_et
o.if_et:	h.types
o.if_et:	TCPIPLibs:h.errno
o.if_et:	C:h.kernel
o.if_et:	TCPIPLibs:sys.h.mbuf
o.if_et:	h.int_hndlr
o.if_et:	TCPIPLibs:h.xcb
o.if_et:	h.ioc
o.if_et:	h.delay
o.if_et:	TCPIPLibs:sys.h.dcistructs
o.if_et:	TCPIPLibs:netinet.h.if_ether
o.if_et:	TCPIPLibs:sys.h.types
o.if_et:	TCPIPLibs:sys.h.socket
o.if_et:	TCPIPLibs:net.h.if
o.if_et:	TCPIPLibs:sys.h.mbuf
o.if_et:	TCPIPLibs:net.^.net.h.if_arp
o.if_et:	TCPIPLibs:sys.h.socket
o.if_et:	TCPIPLibs:net.h.if_arp
o.if_et:	TCPIPLibs:netinet.h.in
o.if_et:	h.e1module
o.if_et:	h.if_et
o.if_et:	C:h.kernel
o.if_et:	C:h.swis
o.e1module:	c.e1module
o.e1module:	C:h.kernel
o.e1module:	C:h.swis
o.e1module:	TCPIPLibs:h.xcb
o.e1module:	h.int_hndlr
o.e1module:	TCPIPLibs:sys.h.dcistructs
o.e1module:	TCPIPLibs:sys.h.errno
o.e1module:	TCPIPLibs:sys.^.h.errno
o.e1module:	C:h.kernel
o.e1module:	TCPIPLibs:sys.h.mbuf
o.e1module:	TCPIPLibs:netinet.h.if_ether
o.e1module:	TCPIPLibs:sys.h.types
o.e1module:	TCPIPLibs:sys.h.socket
o.e1module:	TCPIPLibs:net.h.if
o.e1module:	TCPIPLibs:sys.h.mbuf
o.e1module:	TCPIPLibs:net.^.net.h.if_arp
o.e1module:	TCPIPLibs:sys.h.socket
o.e1module:	TCPIPLibs:net.h.if_arp
o.e1module:	TCPIPLibs:netinet.h.in
o.e1module:	h.e1module
o.e1module:	h.if_et
o.support:	c.support
o.support:	TCPIPLibs:sys.h.types
o.mbuf:	c.mbuf
o.mbuf:	C:h.kernel
o.mbuf:	C:h.swis
o.mbuf:	TCPIPLibs:sys.h.param
o.mbuf:	TCPIPLibs:sys.h.sysconf
o.mbuf:	TCPIPLibs:sys.^.machine.h.machparam
o.mbuf:	TCPIPLibs:sys.h.signal
o.mbuf:	TCPIPLibs:sys.h.types
o.mbuf:	TCPIPLibs:sys.h.errno
o.mbuf:	TCPIPLibs:sys.^.h.errno
o.mbuf:	C:h.kernel
o.mbuf:	TCPIPLibs:sys.h.mbuf
o.mbuf:	TCPIPLibs:netinet.h.if_ether
o.mbuf:	TCPIPLibs:sys.h.types
o.mbuf:	TCPIPLibs:sys.h.socket
o.mbuf:	TCPIPLibs:net.h.if
o.mbuf:	TCPIPLibs:sys.h.mbuf
o.mbuf:	TCPIPLibs:net.^.net.h.if_arp
o.mbuf:	TCPIPLibs:sys.h.socket
o.mbuf:	TCPIPLibs:net.h.if_arp
o.mbuf:	TCPIPLibs:netinet.h.in
o.addcb:	c.addcb
o.addcb:	C:h.kernel
o.addcb:	C:h.swis
