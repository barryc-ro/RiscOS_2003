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
# 21-Nov-00  SBF          Added support for StripDepnd
# 22-Mar-01  SNB          Updated for portability
#

#
# Component specific details
#
COMPONENT  = InternationalKeyboard
TARGET     = IntKey
ROM_SOURCE = Source.IntKeyMod
RESOURCES  = no

.SUFFIXES:
.SUFFIXES: .o .s .c .layout

KBD       = -PD "Keyboard SETS \"${KEYBOARD}\""
ASFLAGS   = ${KBD}

include StdTools
include AAsmModule

#
# Static dependencies and specialist rules
#

LAYOUTS   = UK.s Italy.s Spain.s France.s Germany.s Esperanto.s Greece.s \
            Swed-Finl.s Denmark.s Norway.s Iceland.s Turkey.s Russia.s \
            Israel.s LatinAm.s Belgium.s Netherlands.s Japan.s \
            Switzerland.s Wales.s USA.s DvorakUK.s DvorakUSA.s

${ROM_OBJECT}: ${LAYOUTS}
${LAYOUTS}: keygen

.layout.s:;  keygen ${STDTOOLOPTIONS} ${EXTRAKEYS} $< $@
.c.o:;  ${MKDIR} o ; ${CC} ${CFLAGS} -o $@ $<

clean::
        ${WIPE} s ${WFLAGS}
        ${RM} keygen
        ${RM} keyconvert
        @echo ${COMPONENT}: cleaned

keygen: keygen.o throwback.o unicdata.o
        ${MKDIR} s
        ${CC} -o $@ keygen.o throwback.o unicdata.o

keyconvert: keyconvert.o unicdata.o
        ${MKDIR} s
        ${CC} -o $@ keyconvert.o unicdata.o

# Dynamic dependencies:
