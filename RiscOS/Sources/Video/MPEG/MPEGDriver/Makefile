# Makefile for MPEGDriver module
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
COMPONENT  = MPEGDriver
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
SFLAGS   = -depend !Depend ${THROWBACK} ${INCLUDES} -s -DDEBUGLIB
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

RAMOBJS  =  o.Audio  o.CLI o.Clk o.Co  o.IRQ  o.Mess o.MiscAsm o.ModHdr2  o.Module    o.ResMess  o.Service  o.STi3520L  o.Stuffing  o.Video  o.WSS o.ModHdr
ROMOBJS  =  o.Audio  o.CLI o.Clk o.Co  o.IRQ  o.Mess o.MiscAsm o.ModHdr2  o.ModuleROM            o.Service  o.STi3520L  o.Stuffing  o.Video  o.WSS o.ModHdr
SOBJS    =  s.Audio  s.CLI             s.IRQ  s.Mess                      s.Module               s.Service  s.STi3520L  s.Stuffing  s.Video  s.WSS o.ModHdr
IOBJS    =  i.Audio  i.CLI             i.IRQ  i.Mess                      i.Module               i.Service  i.STi3520L  i.Stuffing  i.Video  i.WSS o.ModHdr
IOOBJS   = io.Audio io.CLI o.Clk o.Co io.IRQ io.Mess o.MiscAsm o.ModHdr2 io.Module    o.ResMess io.Service io.STi3520L io.Stuffing io.Video io.WSS o.ModHdr
DOOBJS   = do.Audio do.CLI o.Clk o.Co do.IRQ do.Mess o.MiscAsm o.ModHdr2 do.Module    o.ResMess do.Service do.STi3520L do.Stuffing do.Video do.WSS o.ModHdr


#
# Rule patterns
#
.SUFFIXES: .do .i .inst .io
.c.o:;          ${CC} ${CFLAGS} -o $@ $<
.s.o:;          ${AS} ${OFLAGS} -o $@ $<
.c.s:;          ${CC} ${SFLAGS} -o $@ $<
.c.i:;          ${CC} ${PFLAGS} $< > $@
.inst.io:;      ${CC} ${CFLAGS} -C++ -W -DDEBUGLIB -o $@ $<
.c.do:;         ${CC} ${CFLAGS} -DDEBUGLIB -o $@ $<

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
        ifthere s.Module then rm ${SOBJS}
        ifthere Messages then ${WIPE} Messages ${WFLAGS}
        ifthere linked   then ${WIPE} linked   ${WFLAGS}
        ifthere inst     then ${WIPE} inst     ${WFLAGS}
        ifthere aof      then ${WIPE} aof      ${WFLAGS}
        ifthere drm      then ${WIPE} drm      ${WFLAGS}
        ifthere irm      then ${WIPE} irm      ${WFLAGS}
        ifthere do       then ${WIPE} do       ${WFLAGS}
        ifthere io       then ${WIPE} io       ${WFLAGS}
        ifthere rm       then ${WIPE} rm       ${WFLAGS}
        ifthere i        then ${WIPE} i        ${WFLAGS}
        ifthere o        then ${WIPE} o        ${WFLAGS}
        ${RM} h.Registers
        ${RM} h.ModHdr
        stripdepnd
        @echo ${COMPONENT}: Cleaned

asm: ${SOBJS} o h.Registers
        @echo ${COMPONENT}: Assembler listings generated

prepro: inst ${IOBJS} i o h.Registers
        @echo ${COMPONENT}: Preprocessing complete

mccabe: irm ${IOOBJS} io o messages-${CMDHELP} ${LIBSD} ${CLIB} ${UNIXLIB}
        ${LD} -o irm.${COMPONENT} -rmf ${IOOBJS} ${LIBSD} ${CLIB} ${UNIXLIB}
        @echo ${COMPONENT}: Instrumented module built (RAM)

debug: drm ${DOOBJS} do o h.Registers messages-${CMDHELP} ${LIBSD} ${CLIB}
        ${LD} -o drm.${COMPONENT} -rmf ${DOOBJS} ${LIBSD} ${CLIB}
        ${ATTR} -files +or +ow +wr -ww $@
        ${RM} Messages
        @echo ${COMPONENT}: Debug module built (RAM)

${RAM_MODULE}: rm ${RAMOBJS} o h.Registers messages-${CMDHELP} ${CLIB}
        ${LD} -o $@ -rmf ${RAMOBJS} ${CLIB}
        ${ATTR} -files +or +ow +wr -ww $@
        ${RM} Messages

${ROM_MODULE}: aof ${ROMOBJS} o h.Registers ${ROMCSTUBS}
        ${LD} -o $@ -aof ${ROMOBJS} ${ROMCSTUBS}

rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${C_ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

ModHdr.o: cmhg.ModHdr
	${CMHG} ${CMHGFLAGS} -o $@ $? h.ModHdr

Registers.h: Registers.s
        aasm -quit -from $? -to $@
        settype h.Registers text

ResMess.o: LocalRes:Messages LocalRes:CmdHelp
	ResGen ResMess_ResourcesFiles o.ResMess Messages Resources.MPEGDriver.Messages

ModuleROM.o: Module.c
        ${CC} ${CFLAGS} -DROM_MODULE -o ModuleROM.o Module.c

inst:
	${MKDIR} inst

drm:
	${MKDIR} drm

irm:
	${MKDIR} irm

aof:
	${MKDIR} aof

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

messages-:
        /Tools.FAppend Messages LocalRes:Messages LocalRes:CmdHelp

messages-None:
        ${CP} LocalRes:Messages Messages ${CPFLAGS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
