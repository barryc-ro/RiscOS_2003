# MakeFile for Sound Control module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 26-Nov-97  TGR          Version 0.00 created
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = SoundCtrl
TITLE     = SoundControl
TARGET    = aof.SoundCtrl
SOURCE    = s.SoundCtrl
MODULE    = rm.SoundCtrl
EXPORTS   = ${EXP_HDR}.${COMPONENT}

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = tlink
RM      = remove
TOKENISE = tokenise
WIPE    = -wipe

ASFLAGS  = -depend !Depend -Stamp -quit
CCFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -zM -ffah
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v

TOKENS   = Hdr:Tokens

#
# Libraries
#
CLIB      = CLIB:o.stubs
TGRLIB    = CommonLib:commonlibm
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -ICommonLib:,C:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${TITLE}

OBJS = Hdr.o\
 base.o\
 delay.o\
 dmatfr.o\
 globals.o\
 irq.o\
 main.o\
 messages.o\
 ss.o\
 swi.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.cmhg.o:;   ${CMHG} -d @.h.module $< $@

# Specific rules

messages.o: Res.UK.Messages
        resgen messages_file @.o.messages @.Res.UK.Messages Resources.${TITLE}.Messages

#swi.o: module.h

module.h: cmhg.Hdr
        cmhg -d @.h.module @.cmhg.Hdr -o @.o.Hdr 
        

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @|${MKDIR} ${RESDIR}
        @|${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${MODULE}
        ${RM} ${TARGET}
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned



local_dirs:
        ${MKDIR} o
        ${MKDIR} rm
        ${MKDIR} aof
        ${MKDIR} linked

${MODULE}: ${OBJS} ${CLIB} ${TGRLIB} local_dirs
        ${LD} -rmf -o $@ ${OBJS} ${TGRLIB}


#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} local_dirs
        ${LD} -o $@ -aof ${OBJS} ${TGRLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
	${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


#${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


#---------------------------------------------------------------------------
# Dynamic dependencies:
o.base: s.base
o.delay: s.delay
o.delay: hdr:ListOpts
o.delay: hdr:Macros
o.delay: hdr:System
o.delay: Hdr:SWIs
o.delay: Hdr:CPU.Generic26
o.delay: Hdr:IO.GenericIO
o.delay: Hdr:RISCOS
o.dmatfr: s.dmatfr
o.dmatfr: hdr:ListOpts
o.dmatfr: hdr:Macros
o.dmatfr: hdr:System
o.dmatfr: Hdr:SWIs
o.dmatfr: Hdr:CPU.Generic26
o.dmatfr: Hdr:IO.GenericIO
o.dmatfr: Hdr:RISCOS
o.dmatfr: hdr.CVars
o.dmatfr: hdr.globals
o.dmatfr: hdr.SS
o.dmatfr: hdr.DMATfr
o.globals:	c.globals
o.globals:	h.globals
o.globals:	h.SoundCtrl
o.irq: s.irq
o.irq: hdr:ListOpts
o.irq: hdr:Macros
o.irq: hdr:System
o.irq: Hdr:SWIs
o.irq: Hdr:CPU.Generic26
o.irq: Hdr:IO.GenericIO
o.irq: Hdr:RISCOS
o.irq: hdr.SS
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	CommonLib:h.defines
o.main:	C:h.kernel
o.main:	CommonLib:h.dbmalloc
o.main:	CommonLib:h.debug
o.main:	CommonLib:h.msgs
o.main:	C:h.kernel
o.main:	CommonLib:h.swiextra
o.main:	C:h.swis
o.main:	h.module
o.main:	h.globals
o.main:	h.SoundCtrl
o.main:	h.errors
o.main:	h.ss
o.main:	h.irq
o.main:	h.delay
o.ss:	c.ss
o.ss:	h.ss
o.swi:	c.swi
o.swi:	C:h.kernel
o.swi:	C:h.swis
o.swi:	CommonLib:h.defines
o.swi:	C:h.kernel
o.swi:	CommonLib:h.swiextra
o.swi:	C:h.swis
o.swi:	CommonLib:h.debug
o.swi:	CommonLib:h.msgs
o.swi:	C:h.kernel
o.swi:	h.SoundCtrl
o.swi:	h.SS
o.swi:	h.module
o.swi:	h.errors
o.swi:	h.globals
o.swi:	h.SoundCtrl
o.swi:	h.DMA
o.swi:	h.ss
o.swi:	h.dmatfr
o.swi:	h.irq
