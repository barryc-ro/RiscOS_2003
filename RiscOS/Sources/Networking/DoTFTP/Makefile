# Makefile for DoTFTP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20-Sep-99  KJB          Created.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
LD	= link
RM      = remove
SETTYPE = settype
MKDIR	= cdir
TOUCH	= create
WIPE	= -wipe
CCFLAGS = -c -depend !Depend ${THROWBACK} -IC:
ASFLAGS = -depend !Depend ${THROWBACK}
CPFLAGS = ~cfr~v
WPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = DoTFTP
SOURCE    = s.DoTFTP
OBJECT    = o.DoTFTP
TARGET    = rm.DoTFTP

.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WPFLAGS}
	${WIPE} rm ${WPFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${OBJECT} o._dirs
        ${LD} -bin -o $@ ${OBJECT}
	${SETTYPE} $@ &FFA

o._dirs:
	${MKDIR} o
	${MKDIR} rm
	${TOUCH} o._dirs

# Dynamic dependencies:
