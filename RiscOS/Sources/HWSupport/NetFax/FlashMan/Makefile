# Makefile for FlashMan
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

AFLAGS  = -depend !Depend -Stamp -quit
# DFLAGS  = -DDEBUG
CFLAGS  = -c -depend !Depend -zM -throwback -zps1 -ff -Ospace ${INCLUDES} ${DFLAGS}
CFLAGS_D = -c -depend !Depend -zM -throwback -zps1 -ff -Ospace -DDEBUGLIB ${INCLUDES} ${DFLAGS}
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

DEBUGLIB   = <Lib$Dir>.DebugLib.o.debuglib
FLASHLIB_T = <Lib$Dir>.Netfax.FlashLib.o.flashlib_t
FLASHLIB   = <Lib$Dir>.Netfax.FlashLib.o.flashlib

#
# Include files
#
INCLUDES = -IC:


#
# Program specific options:
#
COMPONENT = FlashMan
TARGET    = aof.FlashMan
TARGET_D  = aof.FlashManT
TARGETSA  = rm.FlashMan
TARGETSA_D = rm.FlashManT

OBJS      = o.Header o.Module o.FlashMan o.misc ${FLASHLIB}

OBJS_D  = o.Header od.Module od.FlashMan od.misc ${DEBUGLIB} TCPIPLibs:o.socklib5 \
TCPIPLibs:o.unixlib TCPIPLibs:o.inetlib ${FLASHLIB_T}

EXPORTS   = 

#
# Rule patterns
#
.SUFFIXES: .o .od .s .h .cmhg .c .z
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.od:;     $(CC) ${CFLAGS_D} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# RISC OS ROM build rules:
#
all: ${TARGETSA} $(TARGETSA_D)
        @echo ${COMPONENT}: module built

debug: $(TARGETSA_D)
        @echo ${COMPONENT}: debug module built

normal: ${TARGETSA}
        @echo ${COMPONENT}: normal module built

rom: ${TARGET} $(TARGET_D)
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
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


#
# Stand-alone module
#
${TARGETSA}: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB}

${TARGETSA_D}: ${OBJS_D} ${CLIB}
        ${LD} -o $@ -module ${OBJS_D} ${CLIB}
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

${TARGET_D}: ${OBJS_D} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS_D} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
