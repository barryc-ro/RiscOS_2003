# MakeFile for CDManager
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
COMPONENT = CDManager
SOURCE    = s.CDManager
TARGET    = aof.CDManager
MODULE    = rm.CDManager
EXPORTS   = ${EXP_HDR}.${COMPONENT}

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD	= link
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
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_AbsSym

COMMONLIB = <CommonLib$Path>

#
# Include files
#
INCLUDES = -IC:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}
LDIR = Res.${LOCALE}

OBJS = main.o	 \
       swicode.o \
       veneer.o	 \
       modhdr.o	 \
       resources.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.cmhg.o:;    ${CMHG} -p -d @.h.module $< $@

# Specific rules

resources.o: Res.UK.Messages
        resgen messages_file @.o.resources @.Res.UK.Messages Resources.${COMPONENT}.Messages

swi.o: module.h

module.h: cmhg.Hdr
        cmhg -p -d @.h.module @.cmhg.Hdr -o @.o.Hdr 
        
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
        @${MKDIR} ${RESDIR}
        @${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${TARGET}
        ${RM} h.module
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${RM} aof
        ${WIPE} linked.* ${WFLAGS}
        ${RM} linked
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${MODULE}: ${OBJS} ${TGRLIB}
        ${MKDIR} rm
        ${LD} -rmf -o $@ ${OBJS} ${TGRLIB} ${CLIB}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${TGRLIB} ${ROMCSTUBS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${TGRLIB} ${ROMCSTUBS}

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
