# Makefile for International Keyboard
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 23-Aug-94  AMcC         Updated to build International Version (from Opal)
# 01-Sep-98  KJB          Changed for new keygen tool
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC	= cc
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit ${KBD} -To $@ -From
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v

#
# Program specific options:
#
COMPONENT = IntKey
SOURCE    = Source.IntKeyMod
LAYOUTS   = s.UK s.Italy s.Spain s.France s.Germany s.Esperanto s.Greece \
            s.Swed-Finl s.Denmark s.Norway s.Iceland s.Turkey s.Russia \
            s.Israel s.LatinAm s.Belgium s.Netherlands s.Japan \
            s.Switzerland s.Wales s.USA s.DvorakUK s.DvorakUSA
TARGET    = rm.IntKey
OBJECT    = o.IntKey

KBD       = -PD "Keyboard SETS \"${KEYBOARD}\""

.SUFFIXES:
.SUFFIXES: .o .s .c .layout

.layout.s:;  keygen ${THROWBACK} $< $@
.c.o:;	${MKDIR} o ; ${CC} ${CCFLAGS} -o $@ $<

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
        ${WIPE} o ${WFLAGS}
        ${WIPE} s ${WFLAGS}
        ${RM} ${TARGET}
	${RM} keygen
	${RM} keyconvert
        @echo ${COMPONENT}: cleaned

keygen: o.keygen o.throwback o.unicdata
	${MKDIR} s
	${LD} -o keygen o.keygen o.throwback o.unicdata C:o.Stubs

keyconvert: o.keyconvert o.unicdata
	${MKDIR} s
	${LD} -o keyconvert o.keyconvert o.unicdata C:o.Stubs

${LAYOUTS}: keygen

${OBJECT}: ${SOURCE} ${LAYOUTS}
        If "<IKHG$Dir>"="" Then Set IKHG$Dir ^.!IKHG
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

${TARGET}: ${OBJECT}
        ${LD} ${LDFLAGS} ${OBJECT}
        SetType $@ &FFA

# Dynamic dependencies:
