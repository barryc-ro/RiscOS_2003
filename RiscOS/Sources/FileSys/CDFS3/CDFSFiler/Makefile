# MakeFile for generic module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 08-Apr-98  TGR          Version 0.00 created
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = CDFSFiler
SOURCE    = s.CDFSFiler
TARGET    = aof.CDFSFiler
MODULE    = rm.CDFSFiler
EXPORTS   = ${EXP_HDR}.${COMPONENT}

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
TOKENISE = tokenise
WIPE    = -wipe

ASFLAGS  = ${THROWBACK} -depend !Depend -Stamp -quit
CCFLAGS  = ${THROWBACK} -c -depend !Depend ${INCLUDES} ${DFLAGS} -zM -fah
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v

TOKENS   = Hdr:Tokens

#
# Libraries
#
CLIB      = CLIB:o.stubs
TGRLIB    = CommonLib:commonlibm
TBOXLIB   = tbox:o.eventlibm tbox:o.toolboxlib tbox:o.renderlib tbox:o.wimplib
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -ITbox:,CommonLib:,C:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}

OBJS = aux.o\
       base.o\
       cdfiler.o\
       events.o\
       globals.o\
       Hdr.o\
       messages.o\
       res.o\
       task.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.cmhg.o:;    ${CMHG} -d @.h.module $< $@

# Specific rules

messages.o: Res.UK.Messages
        resgen messages_file @.o.messages @.Res.UK.Messages Resources.${COMPONENT}.Messages

module.h: cmhg.Hdr
        ${CMHG} -d @.h.module @.cmhg.Hdr -o @.o.Hdr 

res.o: Res.!Sprites22
        resgen res_file @.o.res @.Res.UK.Res Resources.CDFSFiler.Res @.Res.!Sprites22 Resources.CDFSFiler.!Sprites22 
        
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

${MODULE}: ${OBJS} ${TGRLIB} local_dirs
        ${LD} -rmf -o $@ ${OBJS} ${TGRLIB} ${CLIB} ${TBOXLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} local_dirs
        ${LD} -o $@ -aof ${OBJS} ${TGRLIB} ${TBOXLIB} 

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
o.aux:	c.aux
o.aux:	C:h.kernel
o.aux:	C:h.swis
o.aux:	CommonLib:h.defines
o.aux:	C:h.kernel
o.aux:	CommonLib:h.debug
o.aux:	CommonLib:h.msgs
o.aux:	C:h.kernel
o.aux:	h.shareswis
o.aux:	h.main
o.aux:	h.aux
o.aux:	CommonLib:h.defines
o.aux:	h.task
o.aux:	C:h.kernel
o.aux:	Tbox:h.toolbox
o.aux:	Tbox:h.wimp
o.aux:	h.gui
o.base: s.base
o.cdfiler:	c.cdfiler
o.cdfiler:	C:h.kernel
o.cdfiler:	C:h.swis
o.cdfiler:	CommonLib:h.defines
o.cdfiler:	C:h.kernel
o.cdfiler:	CommonLib:h.debug
o.cdfiler:	CommonLib:h.dbmalloc
o.cdfiler:	CommonLib:h.msgs
o.cdfiler:	C:h.kernel
o.cdfiler:	h.shareswis
o.cdfiler:	h.main
o.cdfiler:	h.aux
o.cdfiler:	CommonLib:h.defines
o.cdfiler:	h.cdfiler
o.cdfiler:	h.task
o.cdfiler:	C:h.kernel
o.cdfiler:	Tbox:h.toolbox
o.cdfiler:	Tbox:h.wimp
o.cdfiler:	h.gui
o.events:	c.events
o.events:	C:h.kernel
o.events:	C:h.swis
o.events:	Tbox:h.toolbox
o.events:	Tbox:h.wimp
o.events:	Tbox:h.iconbar
o.events:	Tbox:h.menu
o.events:	Tbox:h.gadgets
o.events:	Tbox:h.event
o.events:	Tbox:h.wimp
o.events:	CommonLib:h.defines
o.events:	C:h.kernel
o.events:	CommonLib:h.dbmalloc
o.events:	CommonLib:h.debug
o.events:	CommonLib:h.msgs
o.events:	C:h.kernel
o.events:	h.cdswis
o.events:	h.shareswis
o.events:	h.main
o.events:	h.gui
o.events:	h.aux
o.events:	CommonLib:h.defines
o.events:	h.task
o.events:	C:h.kernel
o.events:	Tbox:h.toolbox
o.events:	h.gui
o.events:	h.events
o.events:	Tbox:h.toolbox
o.events:	Tbox:h.wimp
o.globals:	c.globals
o.globals:	h.main
o.task:	c.task
o.task:	C:h.kernel
o.task:	C:h.swis
o.task:	Tbox:h.toolbox
o.task:	Tbox:h.wimp
o.task:	Tbox:h.iconbar
o.task:	Tbox:h.menu
o.task:	Tbox:h.event
o.task:	Tbox:h.wimp
o.task:	CommonLib:h.defines
o.task:	C:h.kernel
o.task:	CommonLib:h.dbmalloc
o.task:	CommonLib:h.debug
o.task:	CommonLib:h.msgs
o.task:	C:h.kernel
o.task:	h.cdswis
o.task:	h.main
o.task:	h.gui
o.task:	h.aux
o.task:	CommonLib:h.defines
o.task:	h.task
o.task:	C:h.kernel
o.task:	Tbox:h.toolbox
o.task:	h.gui
o.task:	h.events
o.task:	Tbox:h.toolbox
o.task:	Tbox:h.wimp
