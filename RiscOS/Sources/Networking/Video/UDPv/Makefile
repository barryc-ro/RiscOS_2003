#Makefile for UDP stream control module

CC= cc
OBJASM=         $.library.objasm

CFLAGS=  -IC: -DRISCOS -Darm -DACORN -c -pcc -zM 
CLIB=    clib:o.stubs

SRCS=   c.module c.stubs c.bufs c.mbuf c.inetfn

OBJS=   o.module o.stubs o.bufs o.mbuf o.inetfn o.irqs

udpv:    ${OBJS} Makefile udpvHdr
            cmhg udpvHdr o.udpvhdr
            link -map -o UdpV -rmf o.udpvhdr ${OBJS} ${CLIB} { > linkmap }

module.o:   c.module 
            cc ${CFLAGS} c.module

stubs.o:     c.stubs
             ${CC} ${CFLAGS} c.stubs

bufs.o:     c.bufs
             ${CC} ${CFLAGS} c.bufs

inetfn.o:     c.inetfn
             ${CC} ${CFLAGS} c.inetfn

mbuf.o:     c.mbuf
             ${CC} ${CFLAGS} c.mbuf

o.irqs:     s.irqs
            objasm -from s.irqs -to o.irqs -stamp -quit
