# Makefile for InetSuite
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 30-Oct-95  RWB          Created.
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
COMPONENT = InetSuite
INETSUITE = WrapInet
#
# Generic rules:
#
rom: 
        @echo ${COMPONENT}: rom module built

install_rom: 
        ${CP} ${INETSUITE} ${INSTDIR}.${INETSUITE} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned


# Dynamic dependencies:
