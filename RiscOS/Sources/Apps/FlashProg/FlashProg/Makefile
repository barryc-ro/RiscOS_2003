# Makefile for FlashProg
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 23-Jul-1996  JRH          Created.
# 07-Feb-1997  RWB	    Improved clean for CVS
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
DESTROY = destroy
#IGNORE  = IgnoreErr

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
DEBUGLIB  = <Lib$Dir>.DebugLib.o.debuglibzm

#
# DebugLib support :- comment out for production.
#
#DFLAGS  = -fn -DDEBUGLIB
#LIBSD = \
 ${DEBUGLIB} \
 TCPIPLibs:o.socklib5 \
 TCPIPLibs:o.unixlib \
 TCPIPLibs:o.inetlib \

#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.


#
# Program specific options:
#
COMPONENT = flashprog
TARGET    = aof.flashprog
TARGETSA  = rm.flashprog
OBJS      = o.header o.module o.flashprog
EXPORTS   =

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# RISC OS ROM build rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: module built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install: ${TARGETSA}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${IGNORE} ${DESTROY} ${OBJS}
        ${RM} ${TARGET}
        ${RM} ${TARGETSA}
        ${RM} linked.${COMPONENT}
        ${RM} map.${COMPONENT}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


#
# Stand-alone module
#
${TARGETSA}: ${OBJS} ${CLIB} ${LIBSD}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${LIBSD}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
