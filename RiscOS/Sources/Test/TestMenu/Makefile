# Makefile for TestMenu
#
#
# Paths
#
EXP_HDR = <export$dir>
RESDIR  = <Resource$Dir>.Resources2.Test
#
# Generic options:
#
MKDIR   = cdir
#AS      = aasm
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
STRIP   = -stripdepnd
CD	= dir


#AFLAGS  = -depend !Depend -Stamp -quit -To $@ -From
AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -Wp ${INCLUDES} -DSYS${SYSTEM}=1 -DMACH${MACHINE}=1 ${DFLAGS} ${EXT_FLAGS}
#LDFLAGS =
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#DFLAGS  = ${DEBUG}
BFLAGS  = 31

## Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#
# Include files
#



# Program specific options:
#
COMPONENT = TestMenu
TARGET    = !RunImage
APP	  = !${COMPONENT}
CRUNCHED  = crunched
OM        = o

OBJS      = ${OM}.Main ${OM}.support 


.SUFFIXES: .${OM}

#
# Rule patterns
#
.c.${OM}:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.${OM}:;   ${CMHG} -o $@ $<
.s.${OM}:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE}	o.*		${WFLAGS}
        @echo ${COMPONENT}: cleaned

clean_all: clean
	${RM} ${TARGET}
	${WIPE}	${APP}		${WFLAGS}
	${WIPE} o		${WFLAGS}
	${WIPE} rm		${WFLAGS}
        @echo Stripping dependencies from makefile...
        ${STRIP}
	@echo ${COMPONENT}: cleaned all

resources: ${TARGET}
        @echo ${COMPONENT}: all complete

dirs:
	${MKDIR} o
	${MKDIR} rm
	
#
# Target
#
${TARGET}: ${OBJS} dirs
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB}
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} Resources.* ${APP}.* ${CPFLAGS}
	${MKDIR} ${RESDIR}
	${CP} ${APP} ${RESDIR}.${APP} ${CPFLAGS}
	@echo ${COMPONENT}: built & installed
        

# Dynamic dependencies:
