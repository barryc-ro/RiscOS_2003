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
ARMASMFLAGS = -depend !Depend -g -APCS 3/nofp/noswst -cpu 4T ${THROWBACK}
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = L7200
TARGET    = bin.L7200
DBGTARGET = aif.L7200
GPATARGET = gpa.L7200
EXPORTS   = 

OBJECTS   = o.Boot o.Timers o.IIC o.NVMemory o.MachineID o.Interrupts o.Matrix \
            o.SIB o.Video o.UART o.USB

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

resources:
        @echo ${COMPONENT}: no resources

${TARGET}: ${OBJECTS}
	Link -bin -o $@ ${OBJECTS}

${DBGTARGET}: ${OBJECTS}
        Link -bin -aif -d -o $@ ${OBJECTS}

${GPATARGET}: ${DBGTARGET}
	ToGPA -s ${DBGTARGET} $@

# Dynamic dependencies:
o.Serial: s.Serial Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:Proc Hdr:OSEntries hdr.StaticWS hdr.Hardware
o.Boot: s.Boot Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:Proc Hdr:OSEntries Hdr:HALEntries hdr.StaticWS hdr.Hardware
o.Timers: s.Timers Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:OSEntries Hdr:HALEntries hdr.StaticWS hdr.Hardware
o.IIC: s.IIC Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:Proc Hdr:HALEntries Hdr:OSEntries hdr.StaticWS hdr.Hardware
o.MachineID: s.MachineID
o.Interrupts: s.Interrupts Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:OSEntries hdr.StaticWS hdr.Hardware hdr.Video
o.Matrix: s.Matrix Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:OSEntries hdr.StaticWS hdr.Hardware
o.SIB: s.SIB Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:OSEntries hdr.StaticWS hdr.Hardware
o.Video: s.Video Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:IO.IOMD Hdr:Proc Hdr:OSEntries Hdr:HALEntries hdr.StaticWS hdr.Video hdr.Hardware
o.UART: s.UART Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:Proc Hdr:OSEntries hdr.StaticWS hdr.Hardware
o.USB: s.USB Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:Proc Hdr:HALEntries Hdr:OSEntries hdr.StaticWS hdr.Hardware
o.NVMemory: s.NVMemory Hdr:ListOpts Hdr:Macros Hdr:System Hdr:SWIs Hdr:CPU.Generic26 Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:CPU.Generic32 Hdr:IO.GenericIO Hdr:RISCOS Hdr:Machine.<Machine> Hdr:Machine.Machine Hdr:ImageSize.<ImageSize> Hdr:Proc Hdr:HALEntries Hdr:OSEntries hdr.StaticWS hdr.Hardware
