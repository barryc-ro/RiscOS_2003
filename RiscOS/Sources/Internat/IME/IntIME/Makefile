# Makefile for InternationalIME
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Oct-98  RSP          Created

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
CD	= dir
AR	= libfile
TOUCH   = create
MODSQZ  = modsqz

CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ffa -ec -Wp ${INCLUDES} ${OPTIONS}
CMHGFLAGS = -p
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EXP_HDR    = <Export$Dir>
EXP_C      = <CExport$Dir>.h

#
# Include files
#
INCLUDES = -IC:,C:Unicode

# Program specific options:
#
COMPONENT = IntIME
RESCOMPONENT = IME
TARGET    = aof.${COMPONENT}
EXPORTS   = ${EXP_HDR}.IME ${EXP_C}.IME

OBJS =  o.CModuleHdr o.IntIME

#
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} o.dirs
        @echo ${COMPONENT}: complete

#
# RISC OS ROM build rules:
#
rom: ${TARGET} o.dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS} o.dirs
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} o.dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        ${MKDIR} ${RESDIR}.${RESCOMPONENT}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages    ${RESDIR}.${RESCOMPONENT}.Messages   ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${RM} h.CModuleHdr
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

o.dirs:
	${MKDIR} o
        ${TOUCH} o.dirs
	
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
	${MKDIR} o
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}u: ${OBJS} ${CLIB}
	${MKDIR} rm
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}

rm.${COMPONENT}: rm.${COMPONENT}u
        ${MODSQZ} rm.${COMPONENT}u $@

${EXP_HDR}.IME: hdr.IME
        ${CP} hdr.IME $@ ${CPFLAGS}

${EXP_C}.IME: h.Public
        ${CP} h.Public $@ ${CPFLAGS}

#
# Special rules
#

o.IntIME: h.CModuleHdr
        
#
# Dynamic dependencies:
