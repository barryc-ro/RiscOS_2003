# Makefile for EtherS
#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = |modsqz
WIPE    = -wipe
CD	= dir

# MULTITABLESIZE : a single box isn't likely to be interested in receiving more than this number of multicast
# channels simultaneously.  Setting this too high will incur unecessary overhead.  n/a if HASHED_MULTICAST.

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -zps1 -Wp -zM -I${INCLUDES} ${OPTIONS} -DMULTITABLESIZE=8 \
	${DEBUG} -depend !Depend ${THROWBACK} -DCOMPAT_INET4


CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
INETLIB   = TCPIPLibs:o.inetlibzm

#
# Include files
#
INCLUDES=	C:,TCPIPLibs:

COMPONENT = EtherS
TARGET    = rm.EtherS
ROMTARGET = aof.EtherS
OBJS      = if_es.o esmodule.o support.o poduleirq.o mbuf.o eshdr.o

#
# Rule patterns
#
#  .SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.cmhg.h:;   ${CMHG} -p -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

esmodule.o if_es.o: eshdr.h

#
# Build target
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} h.eshdr
        ${RM} ${TARGET}
        ${RM} ${ROMTARGET}
        ${RM} map.${COMPONENT}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB} ${INETLIB}
        $(MODSQZ) $(TARGET)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${INETLIB}
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
