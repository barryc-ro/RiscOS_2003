# Makefile for PHCIDriver

COMPONENT  = PHCIDriver
TARGET     = PHCIDriver
ROM_SOURCE = init.s

AIFDBG     ?= aif.${MACHINE}.${TARGET}
GPADBG     ?= gpa.${MACHINE}.${TARGET}
ASFLAGS    += -G

include StdTools
include AAsmModule

${AIFDBG}: ${DBG_OBJECT}
        ${MKDIR} aif.${MACHINE}
        ${LD} -aif -bin -d -o ${AIFDBG} ${DBG_OBJECT}

${GPADBG}: ${AIFDBG}
        ${MKDIR} gpa.${MACHINE}
        ToGPA -s ${AIFDBG} ${GPADBG}

exception: ${GPADBG}
        find_error ${TARGET}

# Dynamic dependencies:
