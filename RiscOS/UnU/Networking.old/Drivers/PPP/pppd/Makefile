CC=		CC
LD=		Link
LIB=		libfile
AS=		objasm

CCFLAGS=	-pcc -depend !Depend -c -ITCPIPLibs:,C: -ffh\
                -DNO_DRAND48 \
                -DVJC -zM -zps1 -DDEBUGMAIN # -DDEBUG -DDEBUGALL
LDFLAGS=	-aof -o $@
OBJS=		o.main o.magic o.fsm o.lcp o.ipcp o.upap o.chap \
		o.auth o.options o.md5 o.sys-riscos o.minixlib \
		o.dea_crypt o.dea_data

.SUFFIXES:	.o .c
.c.o:;		$(cc) $(ccflags) -o $@ $<

o.pppd:		$(OBJS)
		$(LD) $(LDFLAGS) $(OBJS)

o.dea_data:	s.dea_data
		$(AS) s.dea_data o.dea_data
		
# Dynamic dependencies:
