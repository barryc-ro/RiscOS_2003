# Makefile for AMU
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 16-Oct-98  SJM          Created.
# 09-Nov-99  NB           Added install: rule to build FrontEnd res.
#

#
# Paths
#

CLXDIR  = C:CLX
LIBDIR  = ${INSTDIR}.Library.Acorn
INSTAPP = ${INSTDIR}.Apps.!AMU

CINCLUDES  = -IC: -I${CLXDIR}
CDEFINES   = -DDDE
CFLAGS     = -Wc -ffah
LDFLAGS    =

#
# Generic options:
#
include StdTools
include AppLibs
include AppStdRule

DIRS=o._dirs

#
# Program specific options:
#
COMPONENT  = amu
TARGET     = amu

OBJS       = amu.o md.o macros.o parse.o readmk.o functions.o patterns.o utility.o execute.o wildcard.o dirsearch.o
LIBS       = ${CLXLIB} ${CLIB}

#
# build a the library:
#
all:    ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed
        
install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: tool installed in library

install: install_tool install_dirs
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	${TIDYDESC}  ${INSTAPP}.Desc ${INSTAPP}.Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'

install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBDIR}.Docs

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        ${RM} dbgreadmk
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
        ${CHMOD} a+rx $@


# Static dependencies:

# These are the confidence tests - all three makefiles should report that all tests passed.
# If they fail, use "run @.amu -f test.assigntest -n" (or whatever file contained the failed
# test and look at the output.
tests: ${TARGET}
        @echo Assignment tests ...
        @run @.${TARGET} -f test.assigntest
        @echo Substitution tests ...
        @run @.${TARGET} -f test.substest
        @echo Function tests ...
        @run @.${TARGET} -f test.functest
        @echo Conditionals tests ...
        @run @.${TARGET} -f test.condtest

dbgreadmk: o.dbgreadmk o.utility
        ${LD} ${LDFLAGS} -o $@ o.dbgreadmk o.utility ${LIBS}

o.dbgreadmk: readmk.c
        ${CC} ${CFLAGS} -DDEBUG -o $@ readmk.c

# Dynamic dependencies:
