# Makefile for PPrimer
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 13-Apr-95  JRH          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2

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

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM ${INCLUDES} ${DFLAGS}
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

#
# Include files
#
INCLUDES = -IC:

# DFLAGS should be NULL for any non-debugging release
DFLAGS    =
#DFLAGS   = -DDEBUG

#
# Program specific options:
#
COMPONENT = PPrimer
TARGET    = aof.PPrimer
EXPORTS   = ${EXP_HDR}.${COMPONENT}
RESDIR    = <resource$dir>.Resources.${COMPONENT}
OBJS      =	\
 o.cmos		\
 o.db		\
 o.error	\
 o.file		\
 o.main		\
 o.supported	\
 o.text		\
 o.header	\
 o.misc 	\
 o.module	\
 o.usermode	\
 o.prime

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.PPrimer

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.PPrimer: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
        ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.cmos:	c.cmos
o.cmos:	C:h.kernel
o.cmos:	C:h.swis
o.cmos:	h.constants
o.cmos:	h.error
o.cmos:	C:h.kernel
o.db:	c.db
o.db:	h.constants
o.db:	h.error
o.db:	C:h.kernel
o.error:	c.error
o.error:	C:h.kernel
o.error:	C:h.swis
o.error:	h.constants
o.error:	h.error
o.error:	C:h.kernel
o.file:	c.file
o.file:	C:h.kernel
o.file:	C:h.swis
o.file:	h.constants
o.file:	h.error
o.file:	C:h.kernel
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.constants
o.main:	h.error
o.main:	C:h.kernel
o.main:	h.supported
o.main:	h.prime
o.main:	C:h.kernel
o.main:	h.misc
o.main:	C:h.kernel
o.supported:	c.supported
o.supported:	C:h.kernel
o.supported:	C:h.swis
o.supported:	h.constants
o.supported:	h.error
o.supported:	C:h.kernel
o.supported:	h.cmos
o.supported:	C:h.kernel
o.supported:	h.file
o.supported:	C:h.kernel
o.supported:	h.text
o.text:	c.text
o.text:	C:h.kernel
o.text:	C:h.swis
o.text:	h.constants
o.text:	h.error
o.text:	C:h.kernel
o.misc:	c.misc
o.misc:	C:h.kernel
o.misc:	C:h.swis
o.misc:	h.constants
o.misc:	h.error
o.misc:	C:h.kernel
o.misc:	h.text
o.misc:	h.file
o.misc:	C:h.kernel
o.misc:	h.cmos
o.misc:	C:h.kernel
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.constants
o.module:	h.error
o.module:	C:h.kernel
o.usermode: s.usermode
o.usermode: Hdr:ListOpts
o.usermode: Hdr:Macros
o.usermode: Hdr:System
o.usermode: Hdr:SWIs
o.usermode: Hdr:CPU.Generic26
o.usermode: Hdr:IO.GenericIO
o.usermode: Hdr:RISCOS
o.usermode: Hdr:EnvNumbers
o.prime:	c.prime
o.prime:	C:h.kernel
o.prime:	C:h.swis
o.prime:	h.constants
o.prime:	h.error
o.prime:	C:h.kernel
o.prime:	h.module
o.prime:	C:h.kernel
o.prime:	h.misc
o.prime:	C:h.kernel
o.prime:	h.text
o.prime:	h.usermode
o.prime:	C:h.kernel
