#Makefile for UDP stream control module

CC= cc
OBJASM= objasm

CFLAGS=  -IC:,TCPIPLibs: -throwback -DINET -DCOMPAT_INET4 -Wp -Otime -c -zM -list 
CLIB=    c:o.stubs

SRCS=   c.module c.bufs c.mbuf

OBJS=   o.module o.bufs o.mbuf o.irqs TCPIPLibs:o.inetlibzm TCPIPLibs:o.socklibzm

udpv:    ${OBJS} Makefile udpvHdr
            cmhg udpvHdr o.udpvhdr
            link -map -o UDPv -rmf o.udpvhdr ${OBJS} ${CLIB} { > linkmap }
            settype linkmap text
            access UDPv wr/r

module.o:   c.module 
            cc ${CFLAGS} c.module

bufs.o:     c.bufs
             ${CC} ${CFLAGS} c.bufs

mbuf.o:     c.mbuf
             ${CC} ${CFLAGS} c.mbuf

o.irqs:     s.irqs
            objasm -from s.irqs -to o.irqs -stamp -quit
