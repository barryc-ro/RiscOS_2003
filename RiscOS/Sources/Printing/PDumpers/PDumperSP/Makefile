# Makefile for PDumperSP (Simple Printer dumper that outputs monochrome sprites)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 27-May-94  AMcC         Created.
# 27 Jun 1997 RWB	  Build seperatly from rest of dumpers
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = PDumperSP
EXPORT    = ${EXP_HDR}.PDumper
SOURCE    = s.Header
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
clean:
	${RM} ${TARGET}
	${RM} rm

all: ${TARGET}
        @echo ${COMPONENT}: module built
        
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORT}
        @echo ${COMPONENT}: export complete

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
	Access ${INSTDIR}.${COMPONENT} LR/r
        @echo ${COMPONENT}: installed (disc)

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed 

${TARGET}: ${SOURCE}
	${MKDIR} rm
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}

${EXP_HDR}.PDumper: ^.hdr.PDumper
        ${CP} ^.hdr.PDumper $@ ${CPFLAGS}

# Dynamic dependencies:
