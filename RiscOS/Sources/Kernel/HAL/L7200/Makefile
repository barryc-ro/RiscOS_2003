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
ARMASMFLAGS = -depend !Depend -g -APCS 3/nofp/noswst -cpu 4T ${THROWBACK} -PD "${BOARD} SETL {TRUE}"
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
