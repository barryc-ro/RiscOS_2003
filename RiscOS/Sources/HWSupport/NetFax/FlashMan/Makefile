# Makefile for DayFlashMan
# ========================
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H = <CExport$dir>.h

# ------------------------------------------------------------------------------
# Generic options:
#

MKDIR		= cdir
AS		= objasm
CC		= ${MemCheckCC} cc
CMHG		= cmhg
CP		= copy
LD		= ${MemCheckLD} link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE    	= -wipe

AFLAGS		= -depend !Depend -Stamp -quit ${THROWBACK}
CFLAGS		= -c -depend !Depend -zM -zps1 -ff -Ospace ${THROWBACK} ${INCLUDES}
CFLAGS_D	= -c -depend !Depend -zM -zps1 -f -Ospace ${THROWBACK} -DDEBUGLIB \
 ${INCLUDES}
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v

DFLAGS		=

# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>

CLIB		= CLIB:o.stubs
CPPLIB		= CPPLib:o.c++lib
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_AbsSym
WRAPPER		= RISC_OSLib:s.ModuleWrap

DEBUGLIB	= ${LIBDIR}.DebugLib.o.debuglibzm
FLASHLIB_T	= ${LIBDIR}.Netfax.FlashLib.o.flashlib_t
FLASHLIB	= ${LIBDIR}.Netfax.FlashLib.o.flashlib

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -IC:


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= FlashMan
TARGET		= aof.FlashMan
TARGET_D	= aof.FlashManT
TARGETSA	= rm.FlashMan
TARGETSA_D	= rm.FlashManT

OBJS = \
 o.Header \
 o.Module \
 o.FlashMan \
 o.misc \

OBJS_D = \
 o.Header \
 od.Module \
 od.FlashMan \
 od.misc \

LIBS = ${FLASHLIB}

LIBS_D = ${DEBUGLIB} TCPIPLibs:o.socklib5 TCPIPLibs:o.unixlib TCPIPLibs:o.inetlib \
 ${FLASHLIB_T}

EXPORTS = 

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .od .s .h .cmhg .c .z
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} -o $@ $<
.c.od:;		$(CC) ${CFLAGS_D} ${DFLAGS} -o $@ $<
.s.o:;		${AS} ${AFLAGS} $@ $<
.s.od:;		${AS} ${AFLAGS} $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $< 

# ------------------------------------------------------------------------------
# Main rules
#

all: ${TARGETSA} $(TARGETSA_D)
        @echo ${COMPONENT}: module built

debug: $(TARGETSA_D)
        @echo ${COMPONENT}: debug module built

normal: ${TARGETSA}
        @echo ${COMPONENT}: normal module built

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

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} rm
	${MKDIR} linked
	${MKDIR} aof
	${MKDIR} map
	create o.dirs
	stamp o.dirs

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


# ------------------------------------------------------------------------------
# Stand-alone module
#

${TARGETSA}: ${OBJS} ${LIBS} ${CLIB} o.dirs
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${TARGETSA_D}: ${OBJS_D} ${LIBS_D} ${CLIB} o.dirs
        ${LD} -o $@ -module ${OBJS_D} ${LIBS_D} ${CLIB}

# ------------------------------------------------------------------------------
# ROM target (re-linked at ROM Image build time)
#

${TARGET}: ${OBJS} ${LIBS} ${ROMCSTUBS} o.dirs
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

${TARGET_D}: ${OBJS_D} ${LIBS_D} ${ROMCSTUBS} o.dirs
        ${LD} -o $@ -aof ${OBJS_D} ${LIBS_D} ${ROMCSTUBS}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} \
              > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# 29/7/1998:
# Removed: |truncate map.${COMPONENT} linked.${COMPONENT} from rom_link
#

# Dynamic dependencies:
