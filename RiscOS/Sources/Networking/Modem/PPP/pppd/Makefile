CC=		CC
LD=		Link
LIB=		libfile
AS=		objasm

# To compile a version of PPP which uses the blockdrivers uncomment the next
# line

BLOCKDRIVERS=	-DBLOCKDRIVERS

CCFLAGS=	-Wp -depend !Depend -c -ITCPIPLibs:,C: -ffah\
                -DNO_DRAND48 -DCOMPAT_INET4\
                -DVJC -zM -zps1 ${BLOCKDRIVERS}\
                #-DDEBUG -DDEBUGMAIN -DDEBUGCHAP -DDEBUG_TML
                
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
