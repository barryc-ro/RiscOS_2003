# Makefile for SuperSample
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Mar-01  SNB          Recreated.
#

COMPONENT  = SuperSample
TARGET     = Super
HEADER1    = Super
RESOURCES  = no

MATRIX1    = Matrix1.s
MATRIX2    = Matrix2.s

include StdTools
include AAsmModule

clean::
        @${RM} ${MATRIX1}
        @${RM} ${MATRIX2}
        ${RM} Matrix1
        ${RM} Matrix2
        @${RM} s.Matrix1
        @${RM} s.Matrix2

${ROM_OBJECT} ${SA_OBJECT} ${DBG_OBJECT}: ${MATRIX1} ${MATRIX2}

.SUFFIXES: .c .s

.c.s:
        ${MKDIR} o
        ${CC} ${STDTOOLOPTIONS} -o $* $<
        $* $@

# Dynamic dependencies:
