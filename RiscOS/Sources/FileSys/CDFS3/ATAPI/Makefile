# MakeFile for ATAPI
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
COMPONENT = ATAPI
SOURCE    = s.ATAPI
TARGET    = aof.ATAPI
MODULE    = rm.ATAPI
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

#
# Include files
#
INCLUDES = -IC:

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}

OBJS = main.o \
       modhdr.o \
       resources.o \
       register.o \
       atapi.o \
       drives.o

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
        ${RM} ${TARGET}
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${MODULE}: ${OBJS} ${TGRLIB}
        ${MKDIR} tm
        ${LD} -rmf -o $@ ${OBJS} ${TGRLIB}

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
o.main:	c.main
o.main:	h.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	C:h.kernel
o.main:	h.defines
o.main:	C:h.kernel
o.main:	h.debug
o.main:	h.msgs
o.main:	C:h.kernel
o.main:	h.dbmalloc
o.main:	h.driverops
o.main:	h.atapiops
o.main:	h.drives
o.main:	h.resources
o.main:	h.register
o.resources:	c.resources
o.resources:	h.resources
o.register:	c.register
o.register:	h.main
o.register:	C:h.swis
o.register:	C:h.kernel
o.register:	C:h.kernel
o.register:	h.defines
o.register:	C:h.kernel
o.register:	h.debug
o.register:	h.msgs
o.register:	C:h.kernel
o.register:	h.dbmalloc
o.register:	h.driverops
o.register:	h.atapiops
o.register:	h.drives
o.register:	h.register
o.register:	h.atapi
o.register:	h.drives
o.register:	h.cdmgr
o.atapi:	c.atapi
o.atapi:	h.main
o.atapi:	C:h.swis
o.atapi:	C:h.kernel
o.atapi:	C:h.kernel
o.atapi:	h.defines
o.atapi:	C:h.kernel
o.atapi:	h.debug
o.atapi:	h.msgs
o.atapi:	C:h.kernel
o.atapi:	h.dbmalloc
o.atapi:	h.driverops
o.atapi:	h.atapiops
o.atapi:	h.drives
o.atapi:	h.register
o.atapi:	h.atapi
o.atapi:	h.drives
o.drives:	c.drives
o.drives:	h.main
o.drives:	C:h.swis
o.drives:	C:h.kernel
o.drives:	C:h.kernel
o.drives:	h.defines
o.drives:	C:h.kernel
o.drives:	h.debug
o.drives:	h.msgs
o.drives:	C:h.kernel
o.drives:	h.dbmalloc
o.drives:	h.driverops
o.drives:	h.atapiops
o.drives:	h.drives
o.drives:	h.cdmgr
o.drives:	h.drives
