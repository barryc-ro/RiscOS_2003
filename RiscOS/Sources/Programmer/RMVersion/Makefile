# Makefile for RMVersion
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-Jan-98  SNB          Created.
#

LIBDIR = <Lib$Dir>

#
# Generic options:
#
MKDIR   = cdir
AR	= libfile
AS      = objasm
CC	= cc
LD	= link
CP      = copy
RM      = remove
TOUCH	= create
WIPE	= -wipe
RM	= remove

CCFLAGS = -c -depend !Depend ${THROWBACK} -IC:
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
ASFLAGS	= -depend !Depend -Stamp -quit ${THROWBACK} -o $@ RMVersion.s
ARFLAGS = -c

LDFLAGS_A=-o $@
LDFLAGS_U=-bin -base 0 -o $@

CLIB		= Clib:o.Stubs

#
# Program specific options:
#
COMPONENT = RMVersion
SOURCE    = s.RMVersion
TARGET_U  = RMVersion
TARGET_A  = TestApp

OBJ_A	= RMVersionA.o
OBJ_Z	= RMVersionZ.o
OBJ_U	= RMVersionU.o

OBJS	= ${OBJ_A} ${OBJ_Z} ${OBJ_U}
HDRS	= ${COMPONENT}

#
# Rule patterns
#
.c.o:;	${CC} ${CCFLAGS} -o $@ c.$*


#
# Generic rules:
#
export: export_${PHASE}

clean:
	${WIPE} o ${WFLAGS}
	${RM} ${TARGET_U}
	${RM} ${TARGET_A}
        @echo ${COMPONENT}: cleaned

export_libs: export_docs ${OBJ_A} ${OBJ_Z} dirs
	${AR} ${ARFLAGS} -o ${LIBDIR}.${COMPONENT}.o.RMVersion ${OBJ_A}
	${AR} ${ARFLAGS} -o ${LIBDIR}.${COMPONENT}.o.RMVersionZ ${OBJ_Z}
        @echo ${COMPONENT}: export complete (libs)

export_hdrs: export_docs ${HDRS} dirs
	${CP} h ${LIBDIR}.${COMPONENT}.h ${CPFLAGS}
	${CP} VersionNum ${LIBDIR}.${COMPONENT}.VersionNum ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_docs: dirs
	${CP} VersionNum ${LIBDIR}.${COMPONENT}.VersionNum ${CPFLAGS}
	${CP} Docs ${LIBDIR}.${COMPONENT}.Docs ${CPFLAGS}

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${LIBDIR}.${COMPONENT}
	${MKDIR} ${LIBDIR}.${COMPONENT}.o


all: ${TARGET_U} ${TARGET_A} ${OBJS}

${TARGET_U}: ${OBJ_U}
	${LD} ${LDFLAGS_U} ${OBJ_U}
	settype $@ Utility

${TARGET_A}: ${OBJ_A} TestApp.o
	${LD} ${LDFLAGS_A} ${OBJ_A} TestApp.o ${CLIB}
	
${OBJ_A}: ${SOURCE} o.dirs
	${AS} ${ASFLAGS} -PreDefine "utility SETL 1=0"
${OBJ_U}: ${SOURCE} o.dirs
	${AS} ${ASFLAGS} -PreDefine "utility SETL 1=1"
${OBJ_Z}: ${SOURCE} o.dirs
	${AS} ${ASFLAGS} -PreDefine "utility SETL 1=0" \
		-PreDefine "module SETL 1=1"

o.dirs:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
