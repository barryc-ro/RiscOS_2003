# MakeFile for CDFSDriver
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
COMPONENT = CDFSDriver
SOURCE    = s.CDFSDriver
TARGET    = aof.CDFSDriver
MODULE    = rm.CDFSDriver
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

OBJS = o.Hdr\
       o.messages\
       o.cdfsswi\
       o.swi\
       o.globals\
       o.cdfsdriver\
       o.command\
       o.aux\
       o.drive

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.cmhg.o:;    ${CMHG} -d @.h.module $< $@

# Specific rules

messages.o: Res.UK.Messages
        resgen messages_file @.o.messages @.Res.UK.Messages Resources.${COMPONENT}.Messages

swi.o: module.h

module.h: cmhg.Hdr
        cmhg -d @.h.module @.cmhg.Hdr -o @.o.Hdr

#
# Generic rules:
#
rom: ${TARGET} module.h
        @echo ${COMPONENT}: rom module built

export: 
	@|${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        @${MKDIR} ${RESDIR}
        @${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} h.module
        ${RM} ${TARGET}
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${RM} linked
        ${WIPE} aof.* ${WFLAGS}
        ${RM} aof
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


${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


#---------------------------------------------------------------------------
# Dynamic dependencies:
o.cdfsswi: s.cdfsswi
o.globals:	c.globals
o.globals:	h.globals
o.aux:	c.aux
o.aux:	C:h.kernel
o.aux:	C:h.swis
o.aux:	C:tgrlib.h.debug
o.aux:	C:tgrlib.h.defines
o.aux:	C:h.kernel
o.aux:	h.aux
o.aux:	h.cdmgr
o.aux:	h.cdfsdriver
o.cdfsdriver:	c.cdfsdriver
o.cdfsdriver:	C:h.kernel
o.cdfsdriver:	C:h.swis
o.cdfsdriver:	C:tgrlib.h.defines
o.cdfsdriver:	C:h.kernel
o.cdfsdriver:	C:tgrlib.h.dbmalloc
o.cdfsdriver:	C:tgrlib.h.debug
o.cdfsdriver:	C:tgrlib.h.msgs
o.cdfsdriver:	C:h.kernel
o.cdfsdriver:	h.module
o.cdfsdriver:	h.cdfsswi
o.cdfsdriver:	h.swiextra
o.cdfsdriver:	C:h.swis
o.drive:	c.drive
o.drive:	C:h.kernel
o.drive:	C:h.swis
o.drive:	C:tgrlib.h.defines
o.drive:	C:h.kernel
o.drive:	C:tgrlib.h.debug
o.drive:	C:tgrlib.h.msgs
o.drive:	C:h.kernel
o.drive:	h.swiextra
o.drive:	C:h.swis
o.drive:	h.cdmgr
o.drive:	h.module
o.drive:	h.errors
o.drive:	h.globals
o.drive:	h.drive
o.swi:	c.swi
o.swi:	C:h.kernel
o.swi:	C:h.swis
o.swi:	C:tgrlib.h.defines
o.swi:	C:h.kernel
o.swi:	C:tgrlib.h.debug
o.swi:	C:tgrlib.h.msgs
o.swi:	C:h.kernel
o.swi:	h.standards
o.swi:	h.swiextra
o.swi:	C:h.swis
o.swi:	h.cdmgr
o.swi:	h.cdfsdriver
o.swi:	h.module
o.swi:	h.errors
o.swi:	h.globals
o.swi:	h.aux
o.swi:	h.drive
o.command:	c.command
o.command:	C:h.kernel
o.command:	C:h.swis
o.command:	C:tgrlib.h.defines
o.command:	C:h.kernel
o.command:	C:tgrlib.h.debug
o.command:	C:tgrlib.h.msgs
o.command:	C:h.kernel
o.command:	h.ascii
o.command:	h.standards
o.command:	h.swiextra
o.command:	C:h.swis
o.command:	h.cdfs3
o.command:	h.cdmgr
o.command:	h.cdfsdriver
o.command:	h.module
o.command:	h.errors
o.command:	h.globals
o.command:	h.aux
o.command:	h.drive
