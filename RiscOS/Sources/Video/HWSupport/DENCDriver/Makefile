# Makefile for DENCDriver module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 10-Sep-99      BJGA   Created
# 22-Oct-99      BJGA   Added McCabe targets
# 28-Oct-99      BJGA   Added debug build
#

#
# Program specific options:
#
COMPONENT  = DENCDriver
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}

#
# Generic options:
#
MKDIR   = mkdir -p
CC      = cc
ATTR    = attr
AS      = objasm
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
CFLAGS   = -depend !Depend ${THROWBACK} ${INCLUDES} -c -ffah -zM
SFLAGS   = -depend !Depend ${THROWBACK} ${INCLUDES} -s
PFLAGS   = -depend !Depend ${THROWBACK} ${INCLUDES} -c -E -C -DDEBUGLIB
INCLUDES = -IC:
OFLAGS   = -depend !Depend ${THROWBACK}
CMHGFLAGS= -p ${THROWBACK}

#
# Libraries
#
CLIB     = C:o.stubs
ROMCSTUBS= RISC_OSLib:o.romcstubs
C_ABSSYM = RISC_OSLib:o.c_abssym
LIBSD    = <Lib$Dir>.DebugLib.o.debuglibzm TCPIPLibs:o.socklib5zm TCPIPLibs:o.inetlibzm
UNIXLIB  = TCPIPLibs:o.UnixLibzm

RAMOBJS  =  o.CLI  o.Config  o.Mess  o.Module    o.ResMess  o.Service  o.SWI o.ModHdr
ROMOBJS  =  o.CLI  o.Config  o.Mess  o.ModuleROM            o.Service  o.SWI o.ModHdr
SOBJS    =  s.CLI  s.Config  s.Mess  s.Module               s.Service  s.SWI o.ModHdr
IOBJS    =  i.CLI  i.Config  i.Mess  i.Module               i.Service  i.SWI o.ModHdr
IOOBJS   = io.CLI io.Config io.Mess io.Module    o.ResMess io.Service io.SWI o.ModHdr
DOOBJS   = do.CLI do.Config do.Mess do.Module    o.ResMess do.Service do.SWI o.ModHdr

KEYS     = key.610PAL key.610NTSC key.701PAL key.701NTSC


#
# Rule patterns
#
.SUFFIXES: .do .i .inst .io .key .keysrc
.c.o:;          ${CC} ${CFLAGS} -o $@ $<
.s.o:;          ${AS} ${OFLAGS} -o $@ $<
.c.s:;          ${CC} ${SFLAGS} -o $@ $<
.c.i:;          ${CC} ${PFLAGS} $< > $@
.inst.io:;      ${CC} ${CFLAGS} -C++ -W -DDEBUGLIB -o $@ $<
.c.do:;         ${CC} ${CFLAGS} -DDEBUGLIB -o $@ $<
.keysrc.key:;   /Tools.MakeKey $@ $<

#
# Main rules:
#
#
ram: ${RAM_MODULE}
        @echo ${COMPONENT}: Module built (RAM)

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_ram: ${RAM_MODULE}
        ${MKDIR} ${INSTDIR}
        ${CP} ${RAM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}.${COMPONENT}
        @echo ${COMPONENT}: Module installed (RAM)

install_rom: ${ROM_MODULE}
        ${MKDIR} ${INSTDIR}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        ${ATTR} -files +or +ow +wr -ww ${INSTDIR}.${COMPONENT}
        @echo ${COMPONENT}: Module installed (ROM)

resources: messages-${CMDHELP}
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        ${RM} Messages
        @echo ${COMPONENT}: Resource files copied

clean:
        ifthere Messages then ${WIPE} Messages ${WFLAGS}
        ifthere linked   then ${WIPE} linked   ${WFLAGS}
        ifthere inst     then ${WIPE} inst     ${WFLAGS}
        ifthere aof      then ${WIPE} aof      ${WFLAGS}
        ifthere drm      then ${WIPE} drm      ${WFLAGS}
        ifthere irm      then ${WIPE} irm      ${WFLAGS}
        ifthere key      then ${WIPE} key      ${WFLAGS}
        ifthere do       then ${WIPE} do       ${WFLAGS}
        ifthere io       then ${WIPE} io       ${WFLAGS}
        ifthere rm       then ${WIPE} rm       ${WFLAGS}
        ifthere i        then ${WIPE} i        ${WFLAGS}
        ifthere o        then ${WIPE} o        ${WFLAGS}
        ifthere s        then ${WIPE} s        ${WFLAGS}
        ${RM} h.ModHdr
        stripdepnd
        @echo ${COMPONENT}: Cleaned

keys: ${KEYS} key
        @echo ${COMPONENT}: Macrovision keys built

asm: ${SOBJS} s o
        @echo ${COMPONENT}: Assembler listings generated

prepro: inst ${IOBJS} i o
        @echo ${COMPONENT}: Preprocessing complete

mccabe: irm ${IOOBJS} io o messages-${CMDHELP} ${LIBSD} ${CLIB} ${UNIXLIB}
        ${LD} -o irm.${COMPONENT} -rmf ${IOOBJS} ${LIBSD} ${CLIB} ${UNIXLIB}
        @echo ${COMPONENT}: Instrumented module built (RAM)

debug: drm ${DOOBJS} do o messages-${CMDHELP} ${LIBSD} ${CLIB}
        ${LD} -o drm.${COMPONENT} -rmf ${DOOBJS} ${LIBSD} ${CLIB}
        @echo ${COMPONENT}: Debug module built (RAM)

${RAM_MODULE}: rm ${RAMOBJS} o messages-${CMDHELP} ${CLIB}
        ${LD} -o $@ -rmf ${RAMOBJS} ${CLIB}
        ${ATTR} -files +or +ow +wr -ww $@
        ${RM} Messages

${ROM_MODULE}: aof ${ROMOBJS} o ${ROMCSTUBS}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS}

rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${C_ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

ModHdr.o: cmhg.ModHdr
	${CMHG} ${CMHGFLAGS} -o $@ $? h.ModHdr

ResMess.o: LocalRes:Messages LocalRes:CmdHelp
	ResGen ResMess_ResourcesFiles o.ResMess Messages Resources.DENCDriver.Messages

ModuleROM.o: Module.c
        ${CC} ${CFLAGS} -DROM_MODULE -o ModuleROM.o Module.c

inst:
	${MKDIR} inst

aof:
	${MKDIR} aof

drm:
	${MKDIR} drm

irm:
	${MKDIR} irm

key:
        ${MKDIR} key

do:
	${MKDIR} do

io:
	${MKDIR} io

rm:
	${MKDIR} rm

i:
	${MKDIR} i

o:
	${MKDIR} o

s:
	${MKDIR} s

messages-:
        /Tools.FAppend Messages LocalRes:Messages LocalRes:CmdHelp

messages-None:
        ${CP} LocalRes:Messages Messages ${CPFLAGS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
