# Makefile for PDumperLE
# ======================
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
LD		= link #${MemCheckLD} link
RM		= remove
MODSQZ		= modsqz
RESGEN		= resgen
WIPE    	= -wipe

AFLAGS		= -depend !Depend -Stamp -quit ${THROWBACK}
CFLAGS		= -c -depend !Depend -zM -zps1 -ffah ${INCLUDES} ${THROWBACK} 
CPFLAGS		= ~cfr~v
CMHGFLAGS	= -depend !Depend -p -throwback ${DFLAGS}
WFLAGS		= ~c~v

DFLAGS		= -DKELVIN -DRISCOS
DDFLAGS		= -DKELVIN -DRISCOS -fn -DDEBUG

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

COMPONENT 	= PDumperLE
TARGET		= aof.dPDumperLE
TARGET_D	= aof.PDumperLE
TARGETSA	= rm.PDumperLE
TARGETSA_D	= rm.dPDumperLE

TARGETPRINTSPR	= abs.PrintSpr
TARGETUNDUMP	= abs.UnDump

OBJS = \
 o.errors \
 o.fcintfc \
 o.fckelvin \
 o.fcprint \
 o.fcrotate \
 o.module \
 o.riscos \
 o.pdumper \
 o.pdsupport \
 o.lexsupport \
 o.rawsprite \
 o.rm \
 o.veneer \

OBJSD = \
 o.errors \
 od.fcintfc \
 od.fckelvin \
 od.fcprint \
 od.fcrotate \
 od.module \
 od.riscos \
 od.pdumper \
 od.pdsupport \
 od.lexsupport \
 od.rawsprite \
 o.rm \
 o.veneer \

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
        ${LD} -o linked.${COMPONENT} -map -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM} \
              > map.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
o.errors: s.errors
o.fcintfc:	c.fcintfc
o.fcintfc:	h.common
o.fcintfc:	C:h.kernel
o.fcintfc:	C:h.swis
o.fcintfc:	h.config
o.fcintfc:	h.riscos
o.fcintfc:	h.module
o.fcintfc:	C:h.kernel
o.fcintfc:	h.hbpxface
o.fcintfc:	h.riscos
o.fcintfc:	h.hbpfunc
o.fcintfc:	h.hbpxface
o.fcintfc:	h.fckelvin
o.fcintfc:	h.fclocal
o.fcintfc:	h.riscos
o.fcintfc:	h.fcvers
o.fcintfc:	h.hbpxface
o.fcintfc:	h.hbpfunc
o.fcintfc:	h.fckelvin
o.fckelvin:	c.fckelvin
o.fckelvin:	h.common
o.fckelvin:	C:h.kernel
o.fckelvin:	C:h.swis
o.fckelvin:	h.config
o.fckelvin:	h.riscos
o.fckelvin:	h.module
o.fckelvin:	C:h.kernel
o.fckelvin:	h.hbpxface
o.fckelvin:	h.riscos
o.fckelvin:	h.hbpfunc
o.fckelvin:	h.hbpxface
o.fckelvin:	h.fckelvin
o.fckelvin:	h.fclocal
o.fckelvin:	h.riscos
o.fckelvin:	h.fcvers
o.fckelvin:	h.hbpxface
o.fckelvin:	h.hbpfunc
o.fckelvin:	h.fckelvin
o.fcprint:	c.fcprint
o.fcprint:	h.common
o.fcprint:	C:h.kernel
o.fcprint:	C:h.swis
o.fcprint:	h.config
o.fcprint:	h.riscos
o.fcprint:	h.module
o.fcprint:	C:h.kernel
o.fcprint:	h.hbpxface
o.fcprint:	h.riscos
o.fcprint:	h.hbpfunc
o.fcprint:	h.hbpxface
o.fcprint:	h.fckelvin
o.fcprint:	h.fclocal
o.fcprint:	h.riscos
o.fcprint:	h.fcvers
o.fcprint:	h.hbpxface
o.fcprint:	h.hbpfunc
o.fcprint:	h.fckelvin
o.fcrotate:	c.fcrotate
o.fcrotate:	h.common
o.fcrotate:	C:h.kernel
o.fcrotate:	C:h.swis
o.fcrotate:	h.config
o.fcrotate:	h.riscos
o.fcrotate:	h.module
o.fcrotate:	C:h.kernel
o.fcrotate:	h.hbpxface
o.fcrotate:	h.riscos
o.fcrotate:	h.hbpfunc
o.fcrotate:	h.hbpxface
o.fcrotate:	h.fckelvin
o.fcrotate:	h.fclocal
o.fcrotate:	h.riscos
o.fcrotate:	h.fcvers
o.fcrotate:	h.hbpxface
o.fcrotate:	h.hbpfunc
o.fcrotate:	h.fckelvin
o.module:	c.module
o.module:	h.common
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	h.config
o.module:	h.riscos
o.module:	h.module
o.module:	C:h.kernel
o.module:	h.hbpxface
o.module:	h.riscos
o.module:	h.hbpfunc
o.module:	h.hbpxface
o.module:	h.fckelvin
o.module:	h.fclocal
o.module:	h.riscos
o.module:	h.fcvers
o.module:	h.hbpxface
o.module:	h.hbpfunc
o.module:	h.fckelvin
o.module:	h.kelvswis
o.module:	h.pdumper
o.module:	C:h.kernel
o.riscos:	c.riscos
o.riscos:	h.common
o.riscos:	C:h.kernel
o.riscos:	C:h.swis
o.riscos:	h.config
o.riscos:	h.riscos
o.riscos:	h.module
o.riscos:	C:h.kernel
o.riscos:	h.hbpxface
o.riscos:	h.riscos
o.riscos:	h.hbpfunc
o.riscos:	h.hbpxface
o.riscos:	h.fckelvin
o.riscos:	h.fclocal
o.riscos:	h.riscos
o.riscos:	h.fcvers
o.riscos:	h.hbpxface
o.riscos:	h.hbpfunc
o.riscos:	h.fckelvin
o.pdumper:	c.pdumper
o.pdumper:	h.common
o.pdumper:	C:h.kernel
o.pdumper:	C:h.swis
o.pdumper:	h.config
o.pdumper:	h.riscos
o.pdumper:	h.module
o.pdumper:	C:h.kernel
o.pdumper:	h.hbpxface
o.pdumper:	h.riscos
o.pdumper:	h.hbpfunc
o.pdumper:	h.hbpxface
o.pdumper:	h.fckelvin
o.pdumper:	h.fclocal
o.pdumper:	h.riscos
o.pdumper:	h.fcvers
o.pdumper:	h.hbpxface
o.pdumper:	h.hbpfunc
o.pdumper:	h.fckelvin
o.pdumper:	h.pdumper
o.pdumper:	C:h.kernel
o.pdumper:	h.pdsupport
o.pdumper:	C:h.kernel
o.pdumper:	h.lexsupport
o.pdumper:	C:h.kernel
o.pdumper:	h.hbpxface
o.pdumper:	h.kelvswis
o.pdumper:	h.rawsprite
o.pdumper:	C:h.kernel
o.pdsupport:	c.pdsupport
o.pdsupport:	h.common
o.pdsupport:	C:h.kernel
o.pdsupport:	C:h.swis
o.pdsupport:	h.config
o.pdsupport:	h.riscos
o.pdsupport:	h.module
o.pdsupport:	C:h.kernel
o.pdsupport:	h.hbpxface
o.pdsupport:	h.riscos
o.pdsupport:	h.hbpfunc
o.pdsupport:	h.hbpxface
o.pdsupport:	h.fckelvin
o.pdsupport:	h.fclocal
o.pdsupport:	h.riscos
o.pdsupport:	h.fcvers
o.pdsupport:	h.hbpxface
o.pdsupport:	h.hbpfunc
o.pdsupport:	h.fckelvin
o.pdsupport:	h.pdsupport
o.pdsupport:	C:h.kernel
o.pdsupport:	C:h.swis
o.lexsupport:	c.lexsupport
o.lexsupport:	h.common
o.lexsupport:	C:h.kernel
o.lexsupport:	C:h.swis
o.lexsupport:	h.config
o.lexsupport:	h.riscos
o.lexsupport:	h.module
o.lexsupport:	C:h.kernel
o.lexsupport:	h.hbpxface
o.lexsupport:	h.riscos
o.lexsupport:	h.hbpfunc
o.lexsupport:	h.hbpxface
o.lexsupport:	h.fckelvin
o.lexsupport:	h.fclocal
o.lexsupport:	h.riscos
o.lexsupport:	h.fcvers
o.lexsupport:	h.hbpxface
o.lexsupport:	h.hbpfunc
o.lexsupport:	h.fckelvin
o.lexsupport:	h.lexsupport
o.lexsupport:	C:h.kernel
o.lexsupport:	h.hbpxface
o.lexsupport:	h.kelvswis
o.rawsprite:	c.rawsprite
o.rawsprite:	h.rawsprite
o.rawsprite:	C:h.kernel
o.rawsprite:	C:h.kernel
o.rm:	cmhg.rm
o.veneer: s.veneer
od.fcintfc:	c.fcintfc
od.fcintfc:	h.common
od.fcintfc:	C:h.kernel
od.fcintfc:	C:h.swis
od.fcintfc:	h.config
od.fcintfc:	h.riscos
od.fcintfc:	h.module
od.fcintfc:	C:h.kernel
od.fcintfc:	h.hbpxface
od.fcintfc:	h.riscos
od.fcintfc:	h.hbpfunc
od.fcintfc:	h.hbpxface
od.fcintfc:	h.fckelvin
od.fcintfc:	h.fclocal
od.fcintfc:	h.riscos
od.fcintfc:	h.fcvers
od.fcintfc:	h.hbpxface
od.fcintfc:	h.hbpfunc
od.fcintfc:	h.fckelvin
od.fckelvin:	c.fckelvin
od.fckelvin:	h.common
od.fckelvin:	C:h.kernel
od.fckelvin:	C:h.swis
od.fckelvin:	h.config
od.fckelvin:	h.riscos
od.fckelvin:	h.module
od.fckelvin:	C:h.kernel
od.fckelvin:	h.hbpxface
od.fckelvin:	h.riscos
od.fckelvin:	h.hbpfunc
od.fckelvin:	h.hbpxface
od.fckelvin:	h.fckelvin
od.fckelvin:	h.fclocal
od.fckelvin:	h.riscos
od.fckelvin:	h.fcvers
od.fckelvin:	h.hbpxface
od.fckelvin:	h.hbpfunc
od.fckelvin:	h.fckelvin
od.fcprint:	c.fcprint
od.fcprint:	h.common
od.fcprint:	C:h.kernel
od.fcprint:	C:h.swis
od.fcprint:	h.config
od.fcprint:	h.riscos
od.fcprint:	h.module
od.fcprint:	C:h.kernel
od.fcprint:	h.hbpxface
od.fcprint:	h.riscos
od.fcprint:	h.hbpfunc
od.fcprint:	h.hbpxface
od.fcprint:	h.fckelvin
od.fcprint:	h.fclocal
od.fcprint:	h.riscos
od.fcprint:	h.fcvers
od.fcprint:	h.hbpxface
od.fcprint:	h.hbpfunc
od.fcprint:	h.fckelvin
od.fcrotate:	c.fcrotate
od.fcrotate:	h.common
od.fcrotate:	C:h.kernel
od.fcrotate:	C:h.swis
od.fcrotate:	h.config
od.fcrotate:	h.riscos
od.fcrotate:	h.module
od.fcrotate:	C:h.kernel
od.fcrotate:	h.hbpxface
od.fcrotate:	h.riscos
od.fcrotate:	h.hbpfunc
od.fcrotate:	h.hbpxface
od.fcrotate:	h.fckelvin
od.fcrotate:	h.fclocal
od.fcrotate:	h.riscos
od.fcrotate:	h.fcvers
od.fcrotate:	h.hbpxface
od.fcrotate:	h.hbpfunc
od.fcrotate:	h.fckelvin
od.module:	c.module
od.module:	h.common
od.module:	C:h.kernel
od.module:	C:h.swis
od.module:	h.config
od.module:	h.riscos
od.module:	h.module
od.module:	C:h.kernel
od.module:	h.hbpxface
od.module:	h.riscos
od.module:	h.hbpfunc
od.module:	h.hbpxface
od.module:	h.fckelvin
od.module:	h.fclocal
od.module:	h.riscos
od.module:	h.fcvers
od.module:	h.hbpxface
od.module:	h.hbpfunc
od.module:	h.fckelvin
od.module:	h.kelvswis
od.module:	h.pdumper
od.module:	C:h.kernel
od.riscos:	c.riscos
od.riscos:	h.common
od.riscos:	C:h.kernel
od.riscos:	C:h.swis
od.riscos:	h.config
od.riscos:	h.riscos
od.riscos:	h.module
od.riscos:	C:h.kernel
od.riscos:	h.hbpxface
od.riscos:	h.riscos
od.riscos:	h.hbpfunc
od.riscos:	h.hbpxface
od.riscos:	h.fckelvin
od.riscos:	h.fclocal
od.riscos:	h.riscos
od.riscos:	h.fcvers
od.riscos:	h.hbpxface
od.riscos:	h.hbpfunc
od.riscos:	h.fckelvin
od.pdumper:	c.pdumper
od.pdumper:	h.common
od.pdumper:	C:h.kernel
od.pdumper:	C:h.swis
od.pdumper:	h.config
od.pdumper:	h.riscos
od.pdumper:	h.module
od.pdumper:	C:h.kernel
od.pdumper:	h.hbpxface
od.pdumper:	h.riscos
od.pdumper:	h.hbpfunc
od.pdumper:	h.hbpxface
od.pdumper:	h.fckelvin
od.pdumper:	h.fclocal
od.pdumper:	h.riscos
od.pdumper:	h.fcvers
od.pdumper:	h.hbpxface
od.pdumper:	h.hbpfunc
od.pdumper:	h.fckelvin
od.pdumper:	h.pdumper
od.pdumper:	C:h.kernel
od.pdumper:	h.pdsupport
od.pdumper:	C:h.kernel
od.pdumper:	h.lexsupport
od.pdumper:	C:h.kernel
od.pdumper:	h.hbpxface
od.pdumper:	h.kelvswis
od.pdumper:	h.rawsprite
od.pdumper:	C:h.kernel
od.pdsupport:	c.pdsupport
od.pdsupport:	h.common
od.pdsupport:	C:h.kernel
od.pdsupport:	C:h.swis
od.pdsupport:	h.config
od.pdsupport:	h.riscos
od.pdsupport:	h.module
od.pdsupport:	C:h.kernel
od.pdsupport:	h.hbpxface
od.pdsupport:	h.riscos
od.pdsupport:	h.hbpfunc
od.pdsupport:	h.hbpxface
od.pdsupport:	h.fckelvin
od.pdsupport:	h.fclocal
od.pdsupport:	h.riscos
od.pdsupport:	h.fcvers
od.pdsupport:	h.hbpxface
od.pdsupport:	h.hbpfunc
od.pdsupport:	h.fckelvin
od.pdsupport:	h.pdsupport
od.pdsupport:	C:h.kernel
od.pdsupport:	C:h.swis
od.lexsupport:	c.lexsupport
od.lexsupport:	h.common
od.lexsupport:	C:h.kernel
od.lexsupport:	C:h.swis
od.lexsupport:	h.config
od.lexsupport:	h.riscos
od.lexsupport:	h.module
od.lexsupport:	C:h.kernel
od.lexsupport:	h.hbpxface
od.lexsupport:	h.riscos
od.lexsupport:	h.hbpfunc
od.lexsupport:	h.hbpxface
od.lexsupport:	h.fckelvin
od.lexsupport:	h.fclocal
od.lexsupport:	h.riscos
od.lexsupport:	h.fcvers
od.lexsupport:	h.hbpxface
od.lexsupport:	h.hbpfunc
od.lexsupport:	h.fckelvin
od.lexsupport:	h.lexsupport
od.lexsupport:	C:h.kernel
od.lexsupport:	h.hbpxface
od.lexsupport:	h.kelvswis
od.rawsprite:	c.rawsprite
od.rawsprite:	h.rawsprite
od.rawsprite:	C:h.kernel
od.rawsprite:	C:h.kernel
o.printspr:	c.printspr
o.printspr:	h.common
o.printspr:	C:h.kernel
o.printspr:	C:h.swis
o.printspr:	h.config
o.printspr:	h.riscos
o.printspr:	h.module
o.printspr:	C:h.kernel
o.printspr:	h.hbpxface
o.printspr:	h.riscos
o.printspr:	h.hbpfunc
o.printspr:	h.hbpxface
o.printspr:	h.fckelvin
o.printspr:	h.fclocal
o.printspr:	h.riscos
o.printspr:	h.fcvers
o.printspr:	h.hbpxface
o.printspr:	h.hbpfunc
o.printspr:	h.fckelvin
o.printspr:	h.kelvswis
