# Makefile for Standby
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR  = <export$dir>2
EXPC_HDR = <CExport$Dir>.h

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
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ffah -strict ${CFLAGSEXTRA} ${INCLUDES} ${DFLAGS}
CMHGFLAGS = -p -depend !Depend
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
ABSSYM    = RISC_OSLib:o.c_AbsSym

#
# Include files
#
INCLUDES = -IC:

DFLAGS   = -DSTANDBY_INITIAL_STATE=${INITIAL_STATE}

#
# Program specific options:
#
COMPONENT = Standby
TARGET    = aof.${COMPONENT}
RAMTARGET = rm.${COMPONENT}
OBJS      = header.o module.o veneer.o standby.o task.o cstart.o robase.o
EXPORTS   = ${EXP_HDR}.${COMPONENT} ${EXPC_HDR}.${COMPONENT}
HDR       = h.header
LIBS      = <Lib$Dir>.tboxlibs.o.wimplib

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .cmhg .h
.c.o:;		${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;	${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;		${AS} ${AFLAGS} $< $@


HDR: cmhg.header

#
# build a relocatable module:
#
all: ${RAMTARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
	${RM} ${HDR}
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${RAMTARGET}
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.Standby
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.Standby.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied


#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${HDR} ${LIBS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${RAMTARGET}: ${OBJS} ${HDR} ${LIBS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: export.hdr.${COMPONENT}
        ${CP} export.hdr.${COMPONENT} $@ ${CPFLAGS}

${EXPC_HDR}.${COMPONENT}: export.h.${COMPONENT}
        ${CP} export.h.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
