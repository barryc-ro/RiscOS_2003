# -*-Text-*-
#
# $Header$
# $Source$
#
# Copyright (c) 1994 Acorn Computers Ltd., Cambridge, England
#
# Makefile for Ether2 Module
#
# :RCS Log discontinued:
#
LEVEL=		..
INCLUDES=	${LEVEL}/include

CC=		armcc
AS=		armasm -I/riscos/ashdr
LD=		armlink

RMFLAGS=	-f
RM=		rm ${RMFLAGS}

CMHG=		cmhg

DEBUG=
CFLAGS=		-pcc -Fn -zM -I${INCLUDES},. -DKERNEL -DINET \
	${DEBUG} # -DSUPPORT_TRADITIONAL_MBUFS

CLIB=		/riscos/lib/stubs.o

OBJS=		if_en.o e2module.o support.o poduleirq.o en_io.o mbuf.o 
#	svcprintf.o

CFILES=		if_en.c e2module.c support.c mbuf.c svcprintf.c

Ether2,ffa:	Makefile EnHdr ${OBJS}
	cmhg EnHdr enhdr.o
	${LD} -o Ether2,ffa -rmf -s link.sym enhdr.o ${OBJS} ${CLIB}

etags:	;	etags ${CFILES} ${INCLUDES}/*.h ${INCLUDES}/*/*.h *.h

clean:
	${RM} *.o a.out/*.o *~ *.bak TAGS link.sym Ether2,ffa

e2module.o:	e2module.c
	${CC} -c ${CFLAGS} e2module.c

if_en.o:	if_en.c
	${CC} -c ${CFLAGS} if_en.c

support.o:	support.c
	${CC} -c ${CFLAGS} support.c

mbuf.o:		mbuf.c
	${CC} -c ${CFLAGS} mbuf.c

svcprintf.o:	svcprintf.c
	${CC} -c ${CFLAGS} svcprintf.c

poduleirq.o:	poduleirq.s
	${AS} -from poduleirq.s -to poduleirq.o

en_io.o:	en_io.s
	${AS} -from en_io.s -to en_io.o

depend:		${CFILES}
	${RM} makedep; (echo -n '# generated: '; date) >makedep
	echo '#' >> makedep
	${CC} -M ${CFLAGS} ${CFILES} | \
	    awk ' { if ($$1 != prev) \
		{ if (rec != "") print rec; \
		  printf "%s:\t%s\n", substr($$1, 1, length($$1)-3), substr($$1, 1, length($$1)-1); \
		  rec = $$0; \
		  prev = $$1; \
		} \
		else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
		else rec = rec " " $$2 } } \
		END { print rec } ' >> makedep
	echo '/^# DO NOT DELETE THIS LINE/+1,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep
	echo '#' >> Makefile
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
# generated: Fri Dec  9 15:05:04 WET 1994
#
if_en:	if_en.o
if_en.o:	if_en.c ../include/kernel.h ../include/swis.h ../include/delay.h
if_en.o:	../include/sys/types.h ../include/sys/dcistructs.h
if_en.o:	../include/sys/mbuf.h ../include/sys/errno.h ../include/sys/../errno.h
if_en.o:	../include/int_hndlr.h ../include/xcb.h ../include/ioc.h if_en.h
if_en.o:	e2module.h
e2module:	e2module.o
e2module.o:	e2module.c ../include/kernel.h ../include/swis.h ../include/xcb.h
e2module.o:	../include/sys/dcistructs.h ../include/sys/errno.h
e2module.o:	../include/sys/../errno.h ../include/sys/mbuf.h
e2module.o:	../include/netinet/if_ether.h ../include/sys/types.h
e2module.o:	../include/sys/socket.h ../include/net/if.h ../include/sys/mbuf.h
e2module.o:	../include/net/../net/if_arp.h ../include/sys/socket.h
e2module.o:	../include/net/if_arp.h ../include/netinet/in.h e2module.h
support:	support.o
support.o:	support.c ../include/sys/types.h
mbuf:	mbuf.o
mbuf.o:	mbuf.c ../include/kernel.h ../include/swis.h ../include/sys/param.h
mbuf.o:	../include/sys/sysconf.h ../include/sys/../machine/machparam.h
mbuf.o:	../include/sys/signal.h ../include/sys/arm/fp.h ../include/sys/types.h
mbuf.o:	../include/sys/errno.h ../include/sys/../errno.h ../include/sys/mbuf.h
mbuf.o:	../include/netinet/if_ether.h ../include/sys/types.h
mbuf.o:	../include/sys/socket.h ../include/net/if.h ../include/sys/mbuf.h
mbuf.o:	../include/net/../net/if_arp.h ../include/sys/socket.h
mbuf.o:	../include/net/if_arp.h ../include/netinet/in.h
svcprintf:	svcprintf.o
svcprintf.o:	svcprintf.c ../include/kernel.h ../include/swis.h
svcprintf.o:	../include/svcdebug.h
#
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
