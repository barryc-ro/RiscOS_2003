# Makefile for OSVersion
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-May-96  SMC          Created.
# 10 Sep 1997 RWB	  Switch Version number on System and UserIF
#

#
# Paths
#
EXP_HDR = <export$dir>
#
# Generic options:
#
MKDIR   = cdir
AS      = do objasm
LN	= link
CP      = copy
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
AFLAGS = -depend !Depend -Stamp -quit
LFLAGS = -o $@ -rmf
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Program specific options:
#
COMPONENT = OSVersion
SOURCE    = s.ModHead
TARGET    = rm.${SYSTEM}.${UserIF}.OSVersion
EXPORTS   =

OBJS = o.ModHead

#
# Rule patterns
#
.s.o:;      ${AS} ${AFLAGS} $< $@ -Predefine "Date SETS \"${DATE}\"" -Predefine "Version SETS \"${VERSION}\"" -Predefine "Name SETS \"${NAME}\"" -Predefine "Build SETS \"${BUILDNAME}\"" 

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

clean:
        ${RM} ${TARGET}
        ${WIPE} o.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${OBJS}
	${MKDIR} rm.${SYSTEM}
	${MKDIR} rm.${SYSTEM}.${UserIF}
	${LN} ${LFLAGS} ${OBJS}

# Dynamic dependencies:
o.ModHead: s.ModHead
o.ModHead: Hdr:ListOpts
o.ModHead: Hdr:Macros
o.ModHead: Hdr:System
o.ModHead: Hdr:SWIs
o.ModHead: Hdr:CPU.Generic26
o.ModHead: Hdr:IO.GenericIO
o.ModHead: Hdr:RISCOS
o.ModHead: Hdr:ModHand
o.ModHead: Hdr:Machine.<Machine>
o.ModHead: Hdr:Machine.Machine
o.ModHead: Hdr:Proc
