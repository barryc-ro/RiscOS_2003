# Makefile for Kernel
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
C_EXP_HDR = <cexport$dir>.Global.h

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
ARMASM  = objasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend ${THROWBACK} -Stamp -quit -To $@ -From
ARMASMFLAGS = -depend !Depend -g -APCS 3/nofp/noswst ${THROWBACK}
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = IOMD
TARGET    = bin.IOMD
DBGTARGET = aif.IOMD
GPATARGET = gpa.IOMD
EXPORTS   = 

OBJECTS   = o.Boot o.IIC o.NVRAM o.Video o.Interrupts o.MachineID o.Timers

.s.o:;	${ARMASM} ${ARMASMFLAGS} $< $@

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

debug: ${GPATARGET}
        @echo ${COMPONENT}: debug module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
	${RM} ROM
        ${RM} ${TARGET}
	-Wipe o.* ~C~V
	-Wipe aif.* ~C~V
	-Wipe gpa.* ~C~V
        @echo ${COMPONENT}: cleaned

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        Set Kernel$Messages LocalRes:Messages
        IfThere LocalRes:<UserIF>.Messages Then Set Kernel$Messages LocalRes:<UserIF>.Messages
        TokenCheck LocalRes:<UserIF>.Messages
        ${CP} <Kernel$Messages>  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        UnSet Kernel$Messages

resources-None: resources_common
	@

resources-: resources_common
        TokenCheck LocalRes:Messages
        print LocalRes:CmdHelp { >> ${RESDIR}.${COMPONENT}.Messages }

${TARGET}: ${OBJECTS}
	Link -bin -o $@ ${OBJECTS}

${DBGTARGET}: ${OBJECTS}
        Link -bin -aif -d -o $@ ${OBJECTS}

${GPATARGET}: ${DBGTARGET}
	ToGPA -s ${DBGTARGET} $@

# Dynamic dependencies:
o.Boot: s.Boot Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:CPU.ARM600 Hdr:IO.IOMD Hdr:IO.IOMD Hdr:MEMM.ARM600 Hdr:VIDC.VIDC20 Hdr:Proc Hdr:OSEntries Hdr:HALEntries hdr.StaticWS
o.IIC: s.IIC Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:OSEntries Hdr:HALEntries hdr.StaticWS
o.NVRAM: s.NVRAM Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:OSEntries Hdr:HALEntries hdr.StaticWS
o.Video: s.Video Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:Proc Hdr:OSEntries Hdr:HALEntries hdr.StaticWS hdr.Video
o.Interrupts: s.Interrupts Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:DevNos Hdr:OSEntries Hdr:HALEntries hdr.StaticWS
o.MachineID: s.MachineID Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:Proc Hdr:OSEntries Hdr:HALEntries hdr.StaticWS
o.Timers: s.Timers Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:OSEntries Hdr:HALEntries hdr.StaticWS
