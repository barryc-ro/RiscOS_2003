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
#

#
# Paths
#
EXP_HDR = <export$dir>

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
	@echo ${COMPONENT}: cleaned all

install:
	${MKDIR} ${APP}
	${CP} ${APPLICATION} ${APP}.${APPLICATION} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${CP} Defaults.${LOCALE}.!Run ${APP}.!Run ${CPFLAGS}
	${MKDIR} ${APP}.Defaults
	${CP} Defaults.${LOCALE}.${LOCALE} ${APP}.Defaults.${LOCALE} ${CPFLAGS}
	@echo ${COMPONENT}: installed

#
#
# Static dependencies:
#
${APPLICATION}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB}

#
# Dynamic dependencies:
