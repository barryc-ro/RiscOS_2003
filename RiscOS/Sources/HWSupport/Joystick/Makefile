# Makefile for Joystick
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 22-Aug-96  TMD	  Adapted for multiple hardware variants
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = Joystick
SOURCE    = s.Front
TARGET    = rm.${MACHINE}.Joystick
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RDIR      = Resources
LDIR      = ${RDIR}.${LOCALE}


#
# Export Paths for Messages module
#
RESDIR  = <resource$dir>.Resources2.${COMPONENT}

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

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

resources:
        ${MKDIR} ${RESDIR}
        ${CP} ${LDIR}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

${TARGET}: ${SOURCE}
	cdir rm.${MACHINE}
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
rm.Joystick: Hdr:ListOpts
rm.Joystick: Hdr:Macros
rm.Joystick: Hdr:System
rm.Joystick: Hdr:SWIs
rm.Joystick: Hdr:CPU.Generic26
rm.Joystick: Hdr:IO.GenericIO
rm.Joystick: Hdr:RISCOS
rm.Joystick: Hdr:Machine.<Machine>
rm.Joystick: Hdr:ModHand
rm.Joystick: Hdr:MsgTrans
rm.Joystick: Hdr:NdrDebug
rm.Joystick: Hdr:Podule
rm.Joystick: Version
rm.Joystick: s.ModHead
rm.Joystick: s.ticky
rm.Joystick: s.SWI
rm.Joystick: s.Service
rm.Joystick: s.Tail
rm.Peregrine.Joystick: Hdr:ListOpts
rm.Peregrine.Joystick: Hdr:Macros
rm.Peregrine.Joystick: Hdr:System
rm.Peregrine.Joystick: Hdr:SWIs
rm.Peregrine.Joystick: Hdr:CPU.Generic26
rm.Peregrine.Joystick: Hdr:IO.GenericIO
rm.Peregrine.Joystick: Hdr:RISCOS
rm.Peregrine.Joystick: Hdr:Machine.<Machine>
rm.Peregrine.Joystick: Hdr:Machine.<Machine>
rm.Peregrine.Joystick: Hdr:ModHand
rm.Peregrine.Joystick: Hdr:MsgTrans
rm.Peregrine.Joystick: Hdr:NdrDebug
rm.Peregrine.Joystick: Hdr:Podule
rm.Peregrine.Joystick: Version
rm.Peregrine.Joystick: s.ModHead
rm.Peregrine.Joystick: s.ticky
rm.Peregrine.Joystick: s.SWI
rm.Peregrine.Joystick: s.Service
rm.Peregrine.Joystick: s.Tail
