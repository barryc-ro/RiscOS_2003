# Makefile for CryptoBox
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Oct-97  SJM          Created.
#

#
# Paths
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
TOUCH   = create
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -fah -wp ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CPFLAGS = ~cf~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
CPPLIB    = C:C++Lib.o.c++libz

#
# Include files
#
INCLUDES  = C:
DFLAGS    = 

#
# Program specific options:
#
COMPONENT  = WebSigner
TARGET     = aof.${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS = data.o messages.o

#
# Rule patterns
#
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.c.o:;     ${CC} ${CCFLAGS} -ff -zM -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} rm.${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} aof
	${MKDIR} rm

dirs:
	${MKDIR} linked

#
# Final link for the ROM Image (using given base address)
#
rom_link: dirs
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${CPPLIB} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

o.data:
	resgen Messages o.data -via LocalRes:viafile

${TARGET}: ${OBJS} local_dirs
	${LD} -aof -o $@ ${OBJS}

ram: rm.${COMPONENT}
        @echo ${COMPONENT}: ram module built

rm.${COMPONENT}: ${OBJS} local_dirs
	${LD} -rmf -o $@ ${OBJS}

# Dynamic dependencies:
