

TARGET=         ShowStat
DIRS=		o._dirs


INCLUDES=	C:,TCPIPLibs:
CC=		cc
RM=             remove
WFLAGS=         fr~c~v
WIPE=           -wipe

CFLAGS=		-I$(INCLUDES) -c ${THROWBACK} -depend !Depend -Wpan

OBJS=		o.main o.msgs

LIBS=		C:o.Stubs TCPIPLibs:o.Unixlib TCPIPLibs:o.Inetlib \
		TCPIPLibs:o.Socklib5

.SUFFIXES:	.c .o
.c.o:;		$(CC) $(CFLAGS) -o $@ $<

${TARGET}:	$(OBJS) $(LIBS) ${DIRS}
		Link -o $@ $(OBJS) $(LIBS)
		Squeeze $@

clean:;		${RM} ${TARGET}
		${WIPE} o ${WFLAGS}

${DIRS}:
		cdir o
		create $@
	
# Dynamic dependencies:
