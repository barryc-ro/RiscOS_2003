# Makefile for ModemEMC
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 28-May-96  RWB 	  New for !ModemEMC
# 06-May-97  jmf	  Tidy up build

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
CFLAGS  =  -c -depend !Depend ${INCLUDES} ${DFLAGS} -Throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#DFLAGS  = -DDEBUG
#DFLAGS  = -DEMCREMOTE_NOMODEM

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
COMPONENT   = EMCRemote
APPNAME     = !EMCRemote
APP         = !RunImage
EXPORTS	    =
TARGET      =
TARGETSA    =
OBJS        = o.main o.support o.sc

#
# Export path for Messages module
#
RESAPP = <resource$dir>.Apps.${APPNAME}
  
#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Default target
#
all: ${APP}
	@echo ${COMPONENT}: all complete

#
# RiscOS build rules
#

resources: ${APP}
	${MKDIR} ${RESAPP}
	${CP} Resources.!Boot	    ${RESAPP}.!Boot 	${CPFLAGS}
	${CP} Resources.!Run 	    ${RESAPP}.!Run 	${CPFLAGS}
	${CP} Resources.!Sprites    ${RESAPP}.!Sprites 	${CPFLAGS}
	${CP} Resources.Defaults    ${RESAPP}.Defaults 	${CPFLAGS}
	${CP} ${APP}    	    ${RESAPP}.${APP}  	${CPFLAGS}	
	@echo ${COMPONENT}: resource files copied
	

export:
rom:
install_rom:
install:

clean:
        ${WIPE} o.* ${WFLAGS}
	${RM} ${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#
${APP}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB}

#
# Dynamic dependencies:
o.error:	c.error
o.error:	C:h.kernel
o.error:	h.constants
o.sc:	c.sc
o.sc:	C:h.kernel
o.sc:	C:h.swis
o.sc:	C:h.scinter
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.support
o.main:	C:h.kernel
o.main:	h.constants
o.main:	h.serdevvars
o.main:	h.sc
o.main:	C:h.kernel
o.main:	h.constants
o.support:	c.support
o.support:	C:h.kernel
o.support:	C:h.swis
o.support:	h.constants
