# Makefile for FlashPod application.
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 04-Sep-97  jfarrell	  Created
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
#DFLAGS  = -DDEBUG
DFLAGS  = 
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:


#
# Program specific options:
#
COMPONENT = FlashPod
TARGETSA  = ${COMPONENT}
OBJS      = main.o fileio.o amd29f.o stuff.o csleep.o chksum.o
EXPORTS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# RISC OS ROM build rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: app built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGETSA}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: app installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGETSA}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


#
# App
#
${TARGETSA}: ${OBJS} ${CLIB}
        ${LD} ${LDFLAGS} -o $@ ${OBJS} ${CLIB}


# Dynamic dependencies:
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.error
o.main:	h.csleep
o.main:	h.fileio
o.main:	h.flashpod
o.main:	h.amd29f
o.main:	h.flashpod
o.main:	h.chksum
o.main:	C:h.kernel
o.main:	C:h.swis
o.fileio:	c.fileio
o.fileio:	h.fileio
o.amd29f:	c.amd29f
o.amd29f:	h.amd29f
o.amd29f:	h.flashpod
o.amd29f:	h.csleep
o.stuff: s.stuff
o.stuff: Hdr:ListOpts
o.stuff: Hdr:Macros
o.stuff: Hdr:System
o.stuff: Hdr:SWIs
o.stuff: Hdr:CPU.Generic26
o.stuff: Hdr:IO.GenericIO
o.stuff: Hdr:RISCOS
o.stuff: Hdr:Debug
o.stuff: Hdr:Machine.<Machine>
o.stuff: Hdr:Machine.Machine
o.stuff: Hdr:IO.IOMDL
o.stuff: Hdr:MEMM.ARM600
o.csleep:	c.csleep
o.csleep:	C:h.kernel
o.csleep:	C:h.swis
o.chksum:	c.chksum
o.chksum:	C:h.kernel
o.chksum:	C:h.swis
