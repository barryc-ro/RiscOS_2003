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
