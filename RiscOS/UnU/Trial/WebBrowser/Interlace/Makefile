# Makefile for Interlw
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01/11/95   RWB	  created

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
COMPONENT = Interlw
TWITTER   = Interlw
#
# Generic rules:
#
rom: 
        @echo ${COMPONENT}: rom module built

install_rom: 
        ${CP} ${TWITTER} ${INSTDIR}.${TWITTER} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned


# Dynamic dependencies:
