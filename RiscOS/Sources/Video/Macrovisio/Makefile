# Makefile for Macrovision
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name       Description
# ----       ----       -----------
# 23-Mar-01  SNB        Recreated.
#
COMPONENT = Macrovisio
TARGET    = Macrovisio
RESOURCES = no

DATAFILE  = Data.YUVTable
ASFLAGS   = -PD "DataTableFile SETS \"${DATAFILE}\""

include StdTools
include AAsmModule

#
# Static dependencies
#
${ROM_OBJECT}: ${DATAFILE}

DATAFILEGENERATOR = GenYUVTab

clean::
        ${RM} ${DATAFILE}
        ${RM} ${DATAFILEGENERATOR}
        @echo ${COMPONENT}: module cleaned

${DATAFILE}: ${DATAFILEGENERATOR}
        ${DATAFILEGENERATOR} $@

${DATAFILEGENERATOR}: ${DATAFILEGENERATOR}.c
        ${MKDIR} o
        ${CC} -o $@ ${DATAFILEGENERATOR}

# Dynamic dependencies:
