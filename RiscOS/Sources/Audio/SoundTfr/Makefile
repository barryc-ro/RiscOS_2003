# MakeFile for SoundTfr module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 26-Nov-97  TGR          Version 0.00 created for SoundControl
# 15-Jul-98  RML          Changed for SoundTfr
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = SoundTfr
TITLE     = SoundTfr
TARGET    = aof.SoundTfr
SOURCE    = s.SoundTfr
MODULE    = rm.SoundTfr
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
CCFLAGS  = -c -depend !Depend ${INCLUDES} ${THROWBACK} ${DFLAGS} -zM -fah
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

OBJS = Hdr.o \
 main.o \
 swi.o \
 global.o \
 tfr.o \
 callback.o \
 wav.o \
 messages.o

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
