# Makefile for Wrapper
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16/8/95  Rich Buckley   Modified from UARTDeviceDriver makefile
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
ASFLAGS = -depend !Depend -Stamp -quit -module -Throwback -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT  = Wrapper
SOURCE     = s.main
TARGET     = rm.Wrapper

RESFILES =			\
  Resources.Boot.!Run           \
  Resources.Boot.GoOn		\
  Resources.Boot.Desktop	\
  Resources.Boot.EnableMse

# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

#
# Makes a stand-alone version with messages bound in
#
all: ${TARGET} 
        @echo ${COMPONENT}: Stand alone module built

${TARGET}: ${SOURCE} ${RESFILES}
        ${AS} ${ASFLAGS} ${SOURCE}

# Dynamic dependencies:
rm.Wrapper: Hdr:ListOpts
rm.Wrapper: Hdr:Macros
rm.Wrapper: Hdr:System
rm.Wrapper: Hdr:SWIs
rm.Wrapper: Hdr:CPU.Generic26
rm.Wrapper: Hdr:IO.GenericIO
rm.Wrapper: Hdr:RISCOS
rm.Wrapper: Hdr:Services
rm.Wrapper: Hdr:Proc
rm.Wrapper: Hdr:ResourceFS
rm.Wrapper: s.version
