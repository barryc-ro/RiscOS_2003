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
CFLAGS   = ${VARIANT} -depend !Depend ${THROWBACK} ${INCLUDES} -c -ffah -zM -zps1
SFLAGS   = ${VARIANT} -depend !Depend ${THROWBACK} ${INCLUDES} -s -ffah -zM -zps1 -DDEBUGLIB
PFLAGS   = ${VARIANT} -depend !Depend ${THROWBACK} ${INCLUDES} -c -E -C -DDEBUGLIB
OFLAGS   = ${ASMVARIANT} -depend !Depend ${THROWBACK}
INCLUDES = -IC:
CMHGFLAGS= -p ${THROWBACK}

#
# Libraries
#
CLIB     = C:o.stubs
ROMCSTUBS= RISC_OSLib:o.romcstubs
C_ABSSYM = RISC_OSLib:o.c_abssym
LIBS     = C:MPEG.o.MPEG
LIBSD    = C:MPEG.o.MPEG <Lib$Dir>.DebugLib.o.debuglibzm TCPIPLibs:o.socklib5zm TCPIPLibs:o.inetlibzm
UNIXLIB  = TCPIPLibs:o.UnixLibzm

RAMOBJS  =  o.AudGeneric  o.Audio  o.CLI o.Clk o.Co o.DivRem64 o.Fixed  o.IRQ  o.Mess o.MiscAsm o.ModHdr2  o.Module     o.PCompos  o.RecogAC3  o.RecogMPA o.ResMess  o.Service  o.STi3520L  o.Stuffing  o.Sync  o.Video  o.WSS  o.Zoran o.ZoranPara o.ZoranUcode o.ModHdr o.DWrite o.ReEntDebug o.TrapVect
ROMOBJS  =  o.AudGeneric  o.Audio  o.CLI o.Clk o.Co o.DivRem64 o.Fixed  o.IRQ  o.Mess o.MiscAsm o.ModHdr2  o.ModuleROM  o.PCompos  o.RecogAC3  o.RecogMPA            o.Service  o.STi3520L  o.Stuffing  o.Sync  o.Video  o.WSS  o.Zoran o.ZoranPara o.ZoranUcode o.ModHdr
SOBJS    =  s.AudGeneric  s.Audio  s.CLI                                s.IRQ  s.Mess                      s.Module     s.PCompos  s.RecogAC3  s.RecogMPA            s.Service  s.STi3520L  s.Stuffing  s.Sync  s.Video  s.WSS  s.Zoran    o.ModHdr
IOBJS    =  i.AudGeneric  i.Audio  i.CLI                                i.IRQ  i.Mess                      i.Module     i.PCompos  i.RecogAC3  i.RecogMPA            i.Service  i.STi3520L  i.Stuffing  i.Sync  i.Video  i.WSS  i.Zoran                          o.ModHdr
IOOBJS   = io.AudGeneric io.Audio io.CLI o.Clk o.Co o.DivRem64 o.Fixed io.IRQ io.Mess o.MiscAsm o.ModHdr2 io.Module    io.PCompos io.RecogAC3 io.RecogMPA o.ResMess io.Service io.STi3520L io.Stuffing io.Sync io.Video io.WSS io.Zoran o.ZoranPara o.ZoranUcode o.ModHdr o.DWrite o.ReEntDebug o.TrapVect
DOOBJS   = do.AudGeneric do.Audio do.CLI o.Clk o.Co o.DivRem64 o.Fixed do.IRQ do.Mess o.MiscAsm o.ModHdr2 do.Module    do.PCompos do.RecogAC3 do.RecogMPA o.ResMess do.Service do.STi3520L do.Stuffing do.Sync do.Video do.WSS do.Zoran o.ZoranPara o.ZoranUcode o.ModHdr o.DWrite o.ReEntDebug o.TrapVect


#
# Rule patterns
#
.SUFFIXES: .do .i .inst .io .hdr .h
.c.o:;          ${CC} ${CFLAGS} -o $@ $<
.s.o:;          ${AS} ${OFLAGS} -o $@ $<
.c.s:;          ${CC} ${SFLAGS} -o $@ $<
.c.i:;          ${CC} ${PFLAGS} $< > $@
.inst.io:;      ${CC} ${CFLAGS} -C++ -W -DDEBUGLIB -o $@ $<
.c.do:;         ${CC} ${CFLAGS} -DDEBUGLIB -o $@ $<
.hdr.h:;        perl Build:Hdr2H $< $@

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
        ifthere s.Zoran then rm ${SOBJS}
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
        ${RM} h.Debug
        ${RM} h.Registers
        ${RM} h.ModHdr
        @echo ${COMPONENT}: Cleaned

asm: ${SOBJS} o h.Debug h.Registers
        @echo ${COMPONENT}: Assembler listings generated

prepro: inst ${IOBJS} i o h.Debug h.Registers
        @echo ${COMPONENT}: Preprocessing complete

mccabe: irm ${IOOBJS} io o messages-${CMDHELP} ${LIBSD} ${CLIB} ${UNIXLIB}
        ${LD} -o irm.${COMPONENT} -rmf ${IOOBJS} ${LIBSD} ${CLIB} ${UNIXLIB}
        @echo ${COMPONENT}: Instrumented module built (RAM)

debug: drm ${DOOBJS} do o h.Debug h.Registers messages-${CMDHELP} ${LIBSD} ${CLIB}
        ${LD} -o drm.${COMPONENT} -rmf ${DOOBJS} ${LIBSD} ${CLIB}
        ${ATTR} -files +or +ow +wr -ww drm.${COMPONENT}
        ${RM} Messages
        @echo ${COMPONENT}: Debug module built (RAM)

${RAM_MODULE}: rm ${RAMOBJS} o h.Debug h.Registers messages-${CMDHELP} ${LIBS} ${CLIB}
        ${LD} -o $@ -rmf ${RAMOBJS} ${LIBS} ${CLIB}
        ${ATTR} -files +or +ow +wr -ww $@
        ${RM} Messages

${ROM_MODULE}: aof ${ROMOBJS} o h.Debug h.Registers ${LIBS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${ROMOBJS} ${LIBS} ${ROMCSTUBS}

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

ZoranUcode.o: bin.ZoranUcode

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
        FAppend Messages LocalRes:Messages LocalRes:CmdHelp

messages-None:
        ${CP} LocalRes:Messages Messages ${CPFLAGS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
