# NDS Support Makefile

#
# Standard makefile elements
#
COMPONENT = NDSVerifier
TARGET    = ${COMPONENT}
DIRS      = o._dirs
OBJS      = o.modhead o.ndssupport o.cstart o.ndsmsgr o.ndsstubs o.ndcport
BASE_LIBS = ${CLIB} ${TBOXLIBS} ${DEBUGLIB} ${NET5LIBS} ${CALLXLIB}
LIBS      = ${BASE_LIBS} lib.VER_LIB lib.math_lib

include StdTools
include ModStdRule
include RAMCModule
include ModuleLibs

#
# Debug targets
#
AIFDBG = aif.${COMPONENT}
GPADBG = gpa.${COMPONENT}

CFLAGS    += -throwback -zM -g
CINCLUDES = -IOS:,C:,tbox:
ASFLAGS   += -G


${AIFDBG}: ${OBJS} ${LIBS} ${DIRS}
        link -base 0 -aif -bin -d -o $@ ${OBJS} ${LIBS}

${GPADBG}: ${AIFDBG}
        togpa ${AIFDBG} $@

exception: ${GPADBG}
        unmodsqz ${RAM_MODULE}
        @find_error ${COMPONENT}

#
# This is to build a test harness which we have control over, as we don't
# have access to the sources for the actual library of code we are
# supporting.
#
rm.test1: ${RAM_OBJS} ${BASE_LIBS} Test.o.test1 ${DIRS}
        ${LD} ${LDFLAGS} ${LDRAMFLAGS} -o $@ -rmf ${RAM_OBJS} ${BASE_LIBS} Test.o.test1


clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
        ${WIPE} gpa ${WFLAGS}
        ${WIPE} Test.o ${WFLAGS}
        ${RM}   h.modhead
        stripdepnd

o._dirs:
        ${MKDIR} o
        ${MKDIR} Test.o
        ${MKDIR} h
        ${MKDIR} gpa
        ${MKDIR} aif
        ${MKDIR} rm
        ${TOUCH} o._dirs

ndssupport.o:   modhead.h

# Dynamic dependencies:
