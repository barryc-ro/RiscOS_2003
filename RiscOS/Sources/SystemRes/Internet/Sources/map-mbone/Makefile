
TARGET=         Map-MBone


INCLUDES=	C:,TCPIPLibs:
CC=		cc
RM=             remove
WFLAGS=         fr~c~v
WIPE=           -wipe

CFLAGS=		-Wp -I$(INCLUDES) -DGRAPHLET -c ${THROWBACK} -depend !Depend

OBJS=		o.${TARGET} o.igmp o.inet o.kern

LIBS=		C:o.Stubs TCPIPLibs:o.Unixlib TCPIPLibs:o.Inetlib \
		TCPIPLibs:o.Socklib5

.SUFFIXES:	.c .o
.c.o:;		$(CC) $(CFLAGS) -o $@ $<

${TARGET}:	$(OBJS) $(LIBS)
		Link -o $@ $(OBJS) $(LIBS)
		Squeeze $@

clean:;		${RM} ${TARGET}
		${WIPE} o.* ${WFLAGS}
	
# Dynamic dependencies:
