# TestStart Makefile
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = -remove
WIPE    = -wipe
CD	= dir
CHMOD	= access

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~cf~vr
CHMODFLAGS = RW/R

DFLAGS  = -DUSE_TINY ${DEBUG}
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = C:debug.o.remotezm
#
# Include files
#
INCLUDES = -IC:

# Program specific options:
#
COMPONENT = TestStart
TARGET    = aof.${COMPONENT}
RMTARGET  = rm.${COMPONENT}

OBJS      =	\
o.module	\
o.header        \
o.Pop 		\
o.dongle	\

#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS}   ${AFLAGS} $< $@


# build a relocatable module:
#
all: ${RMTARGET} dirs
	@echo ${COMPONENT}: all complete
	
#
# RISC OS ROM build rules:
#

dirs:
        ${MKDIR} o
	${MKDIR} aof
	${MKDIR} rm
	${MKDIR} linked

rom: ${TARGET} dirs
        @echo ${COMPONENT}: rom complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o      ${WFLAGS}
        ${WIPE} aof    ${WFLAGS}
        ${WIPE} rm     ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.header
        ${RM} ${RMTARGET}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
        
# dummy target
h.header: o.header ;@

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${DIRS} ${OBJS} ${ROMCSTUBS} 
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Relocatable module target
#
${RMTARGET}: ${DIRS} ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}


# Extra dependencies:
o.module: h.header

#
# Dynamic dependencies:
