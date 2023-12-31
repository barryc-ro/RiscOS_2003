# Makefile for PDumperZ11
# =======================
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H = <CExport$dir>.h

# ------------------------------------------------------------------------------
# Generic options:
#

MKDIR		= mkdir -p
AS		= objasm
CC		= ${MemCheckCC} cc
CMHG		= cmhg
CP		= copy
LD		= link #${MemCheckLD} link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE    	= -wipe
STRIP   	= -stripdepnd

AFLAGS		= -depend !Depend -Stamp -quit ${THROWBACK}
CFLAGS		= -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${THROWBACK} 
CPFLAGS		= ~cfr~v
CMHGFLAGS	= -depend !Depend -p -throwback ${DFLAGS}
WFLAGS		= ~c~v

DFLAGS		= -DNOPRAGMA -DCAIRO -DPACE
DDFLAGS		= -DNOPRAGMA -DCAIRO -DPACE -fn -DDEBUG

# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>
CLIB		= CLIB:o.stubs
CPPLIB		= CPPLib:o.c++lib
RLIB		= ${LIBDIR}.RISC_OSLIB.o.risc_oslib
RSTUBS		= ${LIBDIR}.RISC_OSLIB.o.rstubs
ROMSTUBS	= ${LIBDIR}.RISC_OSLIB.o.romstubs
ROMCSTUBS	= ${LIBDIR}.RISC_OSLIB.o.romcstubs
ABSSYM		= ${LIBDIR}.RISC_OSLib.o.c_AbsSym
WRAPPER		= ${LIBDIR}.RISC_OSLib.s.ModuleWrap

LIBS = \

LIBSD = \

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES = -IC:


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT 	= PDumperZ11
TARGET		= aof.PDumperZ11
TARGET_D	= aof.dPDumperZ11
TARGETSA	= rm.PDumperZ11
TARGETSA_D	= rm.dPDumperZ11

TARGETPRINTSPR	= abs.PrintSpr
TARGETUNDUMP	= abs.UnDump

OBJS =\
 o.fccairo \
 o.fcintfc \
 o.fcprint \
 o.fcrotate \
 o.module \
 o.riscos \
 o.rm \
 o.errors \
 o.veneer \
 o.pdumper \
 o.pdsupport \
 o.lexsupport \
 o.rawsprite \
 o.lscairo \
 o.daheap

OBJSD =\
 od.fccairo \
 od.fcintfc \
 od.fcprint \
 od.fcrotate \
 od.module \
 od.riscos \
 o.rm \
 o.errors \
 o.veneer \
 od.pdumper \
 od.pdsupport \
 od.lexsupport \
 od.rawsprite \
 od.lscairo \
 od.daheap

PSOBJS = \
 o.printspr \

UDOBJS = \
 o.undump \


EXPORTS =

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES:	.o .od .s .h .cmhg .c .z
.c.o:;		${CC} ${CFLAGS} ${DFLAGS} ${EXTRAFLAGS} -o $@ $<
.c.od:;		$(CC) ${CFLAGS} ${DDFLAGS} ${EXTRAFLAGS} -o $@ $<
.s.o:;		${AS} ${AFLAGS} -to $@ $<
.cmhg.o:;	${CMHG} ${CMHGFLAGS} -o $@ $<

# ------------------------------------------------------------------------------
# Main rules
#

all: ${TARGETSA}
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
        ${MKDIR} ${INSTDIR}
        ${CP} ${TARGETSA} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

PrintSpr: ${TARGETPRINTSPR}
	${CP} ${TARGETPRINTSPR} ${INSTDIR}.PrintSpr ${CPFLAGS}
	@echo PrintSpr built

UnDump: ${TARGETUNDUMP}
	${CP} ${TARGETUNDUMP} ${INSTDIR}.UnDump ${CPFLAGS}
	@echo UnDump built

o.dirs:
	${MKDIR} o
	${MKDIR} od
	${MKDIR} rm
	${MKDIR} linked
	${MKDIR} aof
	${MKDIR} map
	${MKDIR} abs
	${MKDIR} Targets
	create o.dirs
	stamp o.dirs

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
	${WIPE} abs ${WFLAGS}
	${WIPE} Targets ${WFLAGS}
	${STRIP}
        @echo ${COMPONENT}: cleaned

resources:
        @echo ${COMPONENT}: resource files copied


# ------------------------------------------------------------------------------
# Stand-alone module
#

${TARGETSA}: ${OBJS} ${LIBS} ${CLIB} ${EXTRALIBS} o.dirs
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${EXTRALIBS} ${CLIB}

${TARGETSA_D}: ${OBJSD} ${LIBSD} ${EXTRALIBS} ${CLIB} o.dirs
        ${LD} -o $@ -module ${OBJSD} ${LIBSD} ${EXTRALIBS} ${CLIB}

# ------------------------------------------------------------------------------
# Extra bits

${TARGETPRINTSPR}: ${PSOBJS} ${CLIB}
	${LD} -o $@ ${PSOBJS} ${CLIB}

${TARGETUNDUMP}: ${UDOBJS} ${CLIB}
	${LD} -o $@ ${UDOBJS} ${CLIB}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (re-linked at ROM Image build time)
#

${TARGET}: ${OBJS} ${LIBS} ${EXTRALIBS} ${ROMCSTUBS} o.dirs 
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${EXTRALIBS} ${ROMCSTUBS}

${TARGET_D}: ${OBJSD} ${LIBSD} ${EXTRALIBS} ${ROMCSTUBS} o.dirs
        ${LD} -o $@ -aof ${OBJSD} ${LIBSD} ${EXTRALIBS} ${ROMCSTUBS}

# ------------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#

rom_link:
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
