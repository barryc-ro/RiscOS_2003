# Makefile for ParallelJoystick
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Aug-96  SMC          Created.
# 28-Nov-96  NDT	  Fixed header file export
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Program specific options:
#
COMPONENT = ParaJoy
SOURCE    = s.Front
TARGET    = rm.ParaJoy
EXPORTS   = ${EXP_HDR}.Joystick
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
        ${AS} ${ASFLAGS} ${SOURCE}

${EXP_HDR}.Joystick: hdr.Joystick
        ${CP} hdr.Joystick $@ ${CPFLAGS}

# Dynamic dependencies:
rm.ParaJoy: Hdr:ListOpts
rm.ParaJoy: Hdr:Macros
rm.ParaJoy: Hdr:System
rm.ParaJoy: Hdr:SWIs
rm.ParaJoy: Hdr:CPU.Generic26
rm.ParaJoy: Hdr:IO.GenericIO
rm.ParaJoy: Hdr:RISCOS
rm.ParaJoy: Hdr:Machine.<Machine>
rm.ParaJoy: Hdr:ModHand
rm.ParaJoy: Hdr:MsgTrans
rm.ParaJoy: Hdr:NdrDebug
rm.ParaJoy: Hdr:Podule
rm.ParaJoy: Hdr:Parallel
rm.ParaJoy: Version
rm.ParaJoy: s.ModHead
rm.ParaJoy: s.SWI
rm.ParaJoy: s.Tail
