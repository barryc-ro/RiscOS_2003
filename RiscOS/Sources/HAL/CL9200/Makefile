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
COMPONENT = CL9200
TARGET    = bin.CL9200
DBGTARGET = aif.CL9200
GPATARGET = gpa.CL9200
EXPORTS   = 

OBJECTS   = o.Boot o.Interrupts o.Timers o.IIC o.NVMemory o.Video o.Matrix o.USB #o.MachineID \
           # o.SIB

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
