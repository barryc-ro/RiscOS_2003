# Makefile for ModemTerm
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-Jul-95  RWB 	  New for !ModemTest
# 23-Dec-97  RLT          Modified makefile to extract correct default and !Run file for the
#                         selected country.
# 23-Aug-99  NCE          Exports app to resourcesFS for ROM builds

#
# Paths
#
EXP_HDR = <export$dir>
RESDIR  = <Resource$Dir>.Resources2.Test
#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
STRIP   = -stripdepnd

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS} -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
DFLAGS  = -DDEBUG
#DFLAGS  =
#
# Libraries
#
CLIB       = CLIB:o.stubs

#
# Include files
#
INCLUDES = -IC:

# Program specific options:
#
COMPONENT   = ModemTest
APPLICATION = !RunImage
APP         = !${COMPONENT}

OBJS      =	\
o.main		\
o.support	\
o.screen	\
o.error		\
o.bufman	\
o.Serial	\
o.serial_lib

#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${APPLICATION}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
	${RM} ${APPLICATION}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${WIPE} ${APP}.Defaults ${WFLAGS}
	${WIPE} ${APP} ${WFLAGS}
	@echo Stripping dependencies from makefile...
        ${STRIP}
	@echo ${COMPONENT}: cleaned all

install: ${APPLICATION}
	${MKDIR} ${APP}
	${CP} ${APPLICATION} ${APP}.${APPLICATION} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${CP} Defaults.${LOCALE}.!Run ${APP}.!Run ${CPFLAGS}
	${MKDIR} ${APP}.Defaults
	${CP} Defaults.${LOCALE}.${LOCALE} ${APP}.Defaults.${LOCALE} ${CPFLAGS}
	@echo ${COMPONENT}: installed

resources:
        ${APPLICATION}
	${MKDIR} ${APP}
	${CP} ${APPLICATION} ${APP}.${APPLICATION} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${CP} Defaults.${LOCALE}.!Run ${APP}.!Run ${CPFLAGS}
	${MKDIR} ${APP}.Defaults
	${CP} Defaults.${LOCALE}.${LOCALE} ${APP}.Defaults.${LOCALE} ${CPFLAGS}
	${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
	@echo ${COMPONENT}: built & installed


#
#
# Static dependencies:
#
${APPLICATION}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB}

#
# Dynamic dependencies:
