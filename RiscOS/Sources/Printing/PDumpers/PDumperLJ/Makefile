# Makefile for PDumperLJ (Printer dumper for LaserJets)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 27-May-94  AMcC         Created.
#

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
COMPONENT = PDumperLJ
SOURCE    = s.Header
TARGET    = ^.rm.${COMPONENT}

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: all built

${TARGET}: ${SOURCE}
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}

# Dynamic dependencies:
