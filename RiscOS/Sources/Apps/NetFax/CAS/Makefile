# Skeleton OBJASM/C Makefile for a modulewrapped application
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 17 Sep 1997	RWB		Created

#
# Paths
#
# EXP_HDR = <export$dir>.^.Interface2
# EXP_C_H = <Cexport$dir>.h
# EXP_C_O = <Cexport$dir>.o

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
CD      = dir
CHMOD   = access

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -depend !Depend ${THROWBACK} ${INCLUDES} ${DFLAGS}
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

DFLAGS  = ${DEBUG}
#
# Libraries
#
CLIB       = CLIB:o.stubs CLIB:o.inetlibzm
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.

# Program specific options:
#
COMPONENT = AdminUpdate
APP       = !${COMPONENT}
TARGET    = aof.${COMPONENT}
TARGETSA  = !RunImage
RMTARGET  = rm.${COMPONENT}
# EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJSSA    = o.main o.process o.export
OBJS      = ${OBJSSA} o.modulewrap

#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@


# build a relocatable module:
#
all: ${RMTARGET}
	@echo ${COMPONENT}: all complete
	
#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

# export: ${EXPORTS}
#         @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install_sa: ${TARGETSA}
        ${MKDIR} ${APP}
        ${CP} ${TARGETSA} ${APP}.${TARGETSA} ${CPFLAGS}
        ${CP} Resources.* ${APP}.* ${CPFLAGS}
        @echo ${COMPONENT}: installed

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.*    ${WFLAGS}
        ${RM} ${RMTARGET}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
        
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} 
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

${TARGETSA}: ${OBJSSA}  
        ${LD} -o $@ ${OBJSSA} ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB}
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Relocatable module target
#
${RMTARGET}: ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}

# ${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
#         ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Need to set flag to allow RAM loading of module
#
o.ModuleWrap: s.ModuleWrap s.AppName
	${AS} ${AFLAGS} -predefine "REMOVE_RAMLOAD_CHECK SETA 1" s.ModuleWrap $@
#
# Dynamic dependencies:
o.modulewrap: s.ModuleWrap
o.modulewrap: s.AppName
o.modulewrap: Hdr:ListOpts
o.modulewrap: Hdr:Macros
o.modulewrap: Hdr:System
o.modulewrap: Hdr:SWIs
o.modulewrap: Hdr:CPU.Generic26
o.modulewrap: Hdr:IO.GenericIO
o.modulewrap: Hdr:RISCOS
o.modulewrap: Hdr:ModHand
o.modulewrap: Hdr:EnvNumbers
o.modulewrap: Hdr:Proc
o.modulewrap: Hdr:FSNumbers
o.modulewrap: Hdr:NewErrors
o.modulewrap: Hdr:Services
o.modulewrap: Hdr:ResourceFS
o.modulewrap: Hdr:MsgTrans
o.modulewrap: Hdr:Variables
o.modulewrap: Hdr:UpCall
o.modulewrap: s.ResFiles
o.main:	c.main
o.main:	h.defs
o.main:	C:h.kernel
o.main:	h.inlines
o.main:	C:h.swis
o.main:	h.wimp
o.main:	h.defs
o.process:	c.process
o.process:	h.defs
o.process:	C:h.kernel
o.process:	h.inlines
o.process:	C:h.swis
o.process:	h.wimp
o.process:	h.defs
o.process:	C:h.inetlib
o.process:	C:sys.h.types
o.process:	C:sys.h.cdefs
o.process:	C:machine.h.endian
o.process:	C:machine.h.ansi
o.process:	C:machine.h.types
o.process:	C:netinet.h.in
o.export:	c.export
o.export:	h.addrbookif
o.export:	h.defs
o.export:	C:h.kernel
o.export:	C:h.inetlib
o.export:	C:sys.h.types
o.export:	C:sys.h.cdefs
o.export:	C:machine.h.endian
o.export:	C:machine.h.ansi
o.export:	C:machine.h.types
o.export:	C:netinet.h.in
o.export:	C:h.swis
o.export:	h.wimp
o.export:	h.defs
