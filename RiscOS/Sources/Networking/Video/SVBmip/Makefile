# Makefile for SVBmip
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 17-Mar-99  SNB          Created
# 20-Sep-99  SAR          Forced fussy compilation

COMPONENT = SVBmip
SWI_PREFIX = SVB
CMHG_HDR   = SVBmip
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}D
EXPORTS = <CExport$Dir>.h.${COMPONENT}

#
# Paths
#
EXP_HDR = <export$dir>

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}
RESFSDIR   = Resources.${COMPONENT}

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
MODSQZ  = modsqz
WIPE    = -wipe
CD      = dir
TOUCH   = stamp
AWK     = do awk

CPFLAGS = ~cfr~v
WFLAGS = ~c~v

#
# Include files
#
INCLUDES=       -ITCPIPLibs:,C:

#
# Libraries
#
CLIB            = Clib:o.Stubs
ROMCSTUBS       = RISCOSLIB:o.romcstubs
ABSSYM          = RISC_OSLib:o.AbsSym
INETLIB         = TCPIPLibs:o.inetlibzm
SOCKLIB         = TCPIPLibs:o.socklibzm

DEPEND          = -depend !Depend
DBGFLAGS        = -DTRACE
DEFINES         = -DRISCOS -DCOMPAT_INET4 -DRISCOS_ATM25 -DRISCOS_MPEG

ASFLAGS         = ${DEPEND} ${THROWBACK}
CFLAGS          = -c -fussy -ffah -zM -zps1 -Wp ${INCLUDES} ${DEFINES} ${DEPEND} ${THROWBACK}
CMHGFLAGS       = ${DEPEND} ${THROWBACK} -p
LDFLAGS         = -rmf -c++

ROM_OBJS        = or.Module or.mipSWI or.Parser or.Channels o.SVBmip
RAM_OBJS        = o.Module  o.mipSWI  o.Parser  o.Channels  o.SVBmip
DBG_OBJS        = od.Module od.mipSWI od.Parser od.Channels o.SVBmip

#
# Rule patterns
#
.SUFFIXES:
.SUFFIXES:      .o .od .or .s .c .h .cmhg
.c.o:;          ${CC} ${CFLAGS} -o $@ $<
.c.or:;         ${CC} ${CFLAGS} -DROM -o $@ $<
.c.od:;         ${CC} ${CFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:;       ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;       ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;          ${AS} ${ASFLAGS} -o $@ $<

#
# Static dependencies
#
Module.c: SVBmip.h

#
# Main rules:
#
#
all: ${RAM_MODULE} o._dirs
        @echo ${COMPONENT}: Module built (RAM)

ramtrace: ${DBG_MODULE} o._dirs
        @echo ${COMPONENT}: Module built (TRACE)

rom: ${ROM_MODULE} o._dirs
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources:
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} or ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.${CMHG_HDR} ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o._dirs:
        ${MKDIR} o
        ${MKDIR} od
        ${MKDIR} or
        create $@

export: ${EXPORTS}
        @echo
        @echo ${COMPONENT}: export complete


#
# Targets
#
${RAM_MODULE}:  ${RAM_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${MKDIR} rm
        $(LD) -o $@ -rmf ${RAM_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${MODSQZ} $@
        Access $@ RW/R

${ROM_MODULE}:  ${ROM_OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
        ${MKDIR} aof
        $(LD) -o $@ -aof ${ROM_OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}

${DBG_MODULE}:  ${DBG_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${MKDIR} rm
        $(LD) -o $@ ${LDFLAGS} ${DBG_OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        Access $@ RW/R

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${EXPORTS}: h.Export h.${CMHG_HDR}
        @echo this is $@
        ${CP} h.Export $@ ${CPFLAGS}
        Do ${AWK} -- "/.define ${SWI_PREFIX}\_00/,/endif/" h.${CMHG_HDR} >>$@

# Dynamic dependencies:
