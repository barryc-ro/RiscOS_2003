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
CCFLAGS  = ${THROWBACK} -c -depend !Depend ${INCLUDES} ${DFLAGS} -zM -ffah
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v

TOKENS   = Hdr:Tokens

#
# Libraries
#
CLIB      = CLIB:o.stubs
TGRLIB    = C:tgrlib.o.tgrlib
TBOXLIB   = tbox:o.eventlibm tbox:o.toolboxlib tbox:o.renderlib tbox:o.wimplib
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_AbsSym

#
# Include files
#
INCLUDES = -ITbox:,C:

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
.cmhg.o:;    ${CMHG} -p -d @.h.module $< $@

# Specific rules

messages.o: Res.UK.Messages
        resgen messages_file @.o.messages @.Res.UK.Messages Resources.${COMPONENT}.Messages

module.h: cmhg.Hdr
        ${CMHG} -p -d @.h.module @.cmhg.Hdr -o @.o.Hdr 

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
        ${RM} h.module
        ${RM} ${MODULE}
        ${RM} ${TARGET}
        ${WIPE} rm.* ${WFLAGS}
        ${RM} rm
        ${WIPE} linked.* ${WFLAGS}
        ${RM} linked
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${RM} aof
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
        ${LD} -o $@ -aof ${OBJS} ${TGRLIB} ${TBOXLIB} ${ROMCSTUBS}

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
