# Makefile for JSLib
# ==================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 28-Apr-1998	ADH	Created.
# 19-May-1998	ADH	Uses MemLib if USEMEMLIB is defined; MemLib needs to
#			be initialised through MemHeap_Initialise by the
#			client of the library. It isn't done by the library
#			itself in case the client wants to use the same heap.
#			So, place '-DUSEMEMLIB' on the end of DFLAGS below
#			to make the library use MemLib
# 10-Sep-1998	ADH	USEMEMLIB removed, changes made to NSPRLib to allow
#			much more flexible registration of arbitrary malloc()
#			family style functions. See NSPRLib for more. If
#			JSLib's own malloc() etc. calls are to be replaced,
#			add '-DUSE_NSPRLIB_ALLOC_REGISTRATION' to DFLAGS
#			(this is done by default).

# -----------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
JSLIBDIR	= ${LIBDIR}.JSLib

# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= cc
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
TOUCH		= create
WIPE		= -wipe

CCFLAGS		= -c -depend !Depend -ffah -throwback -Wp ${INCLUDES} ${DFLAGS} ${UFLAGS}
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~cfr~v
LBFLAGS		= -c
WFLAGS		= ~CFR~V

# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
INETLIB		= TCPIPLibs:o.inetlib
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym
NSPRLIB		= C:NSPRLib.o.NSPRLib
MEMLIB		= C:MemLib.o.MemLib

LIBSI = \
 ${CLIB} \
 ${INETLIB} \
 ${NSPRLIB} \
 ${MEMLIB}

LIBSID = \
 ${CLIB} \
 ${INETLIB} \
 ${NSPRLIB} \
 ${MEMLIB}

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:,TCPIPLibs:,C:NSPRLib,RISCOSconf,Core
VPATH		= RISCOSconf Core

# -----------------------------------------------------------------------------
# Compile-time options
#

DFLAGS		= -DCOMPAT_INET4  -DRISCOS  -DNSPR20  -DJSFILE  -DUSE_NSPRLIB_ALLOC_REGISTRATION
UFLAGS		= -UXP_UNIX  -UXP_OS2  -UXP_MAC

# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= JSLib
COMPONENTZ	= JSLibzm
COMPONENTI	= !RunImage
COMPONENTID	= !RunImageD
SOURCE		= ...
TARGET		= ${COMPONENT}
TARGETZ		= ${COMPONENTZ}
TARGETI		= ${COMPONENTI}
TARGETID	= ${COMPONENTID}
EXPORTS		= ${EXP_HDR}.${COMPONENT}

# Library objects for applications

OBJS =\
 o.SetFPSR\
 o.jsaddr\
 o.jsapi\
 o.jsarray\
 o.jsatom\
 o.jsbool\
 o.jscntxt\
 o.jsdate\
 o.jsdbgapi\
 o.jsemit\
 o.jsfun\
 o.jsgc\
 o.jsinterp\
 o.jsmath\
 o.jsnum\
 o.jsobj\
 o.jsopcode\
 o.jsparse\
 o.jsregexp\
 o.jsscan\
 o.jsscope\
 o.jsscript\
 o.jsstr\
 o.prmjtime
# o.jslock\ - thread locking, not needed right now (contents are compiled out anyway)

# Library objects for modules

OBJSZ =\
 o.SetFPSR\
 z.jsaddr\
 z.jsapi\
 z.jsarray\
 z.jsatom\
 z.jsbool\
 z.jscntxt\
 z.jsdate\
 z.jsdbgapi\
 z.jsemit\
 z.jsfun\
 z.jsgc\
 z.jsinterp\
 z.jslock\
 z.jsmath\
 z.jsnum\
 z.jsobj\
 z.jsopcode\
 z.jsparse\
 z.jsregexp\
 z.jsscan\
 z.jsscope\
 z.jsscript\
 z.jsstr\
 z.prmjtime
# z.jslock\ - thread locking, not needed right now (contents are compiled out anyway)

# Stand-alone interpreter objects, non-debug

OBJSI =\
 o.SetFPSR\
 oi.js\
 oi.jsaddr\
 oi.jsapi\
 oi.jsarray\
 oi.jsatom\
 oi.jsbool\
 oi.jscntxt\
 oi.jsdate\
 oi.jsdbgapi\
 oi.jsemit\
 oi.jsfun\
 oi.jsgc\
 oi.jsinterp\
 oi.jsmath\
 oi.jsnum\
 oi.jsobj\
 oi.jsopcode\
 oi.jsparse\
 oi.jsregexp\
 oi.jsscan\
 oi.jsscope\
 oi.jsscript\
 oi.jsstr\
 oi.prmjtime

# Stand-alone interpreter objects, debug

OBJSID =\
 o.SetFPSR\
 o.svcprint\
 oid.js\
 oid.jsaddr\
 oid.jsapi\
 oid.jsarray\
 oid.jsatom\
 oid.jsbool\
 oid.jscntxt\
 oid.jsdate\
 oid.jsdbgapi\
 oid.jsemit\
 oid.jsfun\
 oid.jsgc\
 oid.jsinterp\
 oid.jsmath\
 oid.jsnum\
 oid.jsobj\
 oid.jsopcode\
 oid.jsparse\
 oid.jsregexp\
 oid.jsscan\
 oid.jsscope\
 oid.jsscript\
 oid.jsstr\
 oid.prmjtime

# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oi .oid .s .h .cmhg .c .z
.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -o $@ $<

.c.z:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -zM -o $@ $<

.c.oi:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -o $@ $<

.c.oid:;	@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -DTRACE -g -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo ${ASFLAGS}
		@${AS} ${ASFLAGS} -o $@ $<

.cmhg.o:;	@echo
		@echo Compiling $<
		@echo ${CMFLAGS}
		@${CM} ${CMFLAGS} -o $@ $<

.cmhg.h:;	@echo
		@echo Compiling $<
		@echo ${CMFLAGS}
		@${CM} ${CMFLAGS} -d $@ $<

# -----------------------------------------------------------------------------
# Build the library
#

all:		${COMPONENT} ${COMPONENTZ}

# -----------------------------------------------------------------------------
# RISC OS ROM build rules
#

rom: ${TARGET} ${TARGETZ}
        @echo ${COMPONENT}: ROM module built

export: export_${PHASE}

install_rom: ${TARGET} ${TARGETZM}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ROM module installed
        @echo

clean:
        ${WIPE} o   ${WFLAGS}
        ${WIPE} z   ${WFLAGS}
        ${WIPE} oi  ${WFLAGS}
        ${WIPE} oid ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGETZ}
        ${RM} ${TARGETI}
        ${RM} ${TARGETID}
        ${RM} local_dirs
        @echo
        @echo ${COMPONENT}: Cleaned
        @echo

export_hdrs: dirs
	@echo
	${CP} Core.h.*			${JSLIBDIR}.h.*		${CPFLAGS}
	${CP} RISCOSconf.h.SetFPSR	${JSLIBDIR}.h.*		${CPFLAGS}
	${CP} RISCOSconf.h.JSLibAPI	${JSLIBDIR}.h.*		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (headers)
	@echo

export_libs: ${TARGET} ${TARGETZ} dirs
	@echo
	${CP} ${TARGET}		${JSLIBDIR}.o.${TARGET}		${CPFLAGS}
	${CP} ${TARGETZ}	${JSLIBDIR}.o.${TARGETZ}	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (library)
	@echo

build_interpreter: ${TARGETI} dirs
	@echo
	@echo ${COMPONENT}: Build complete (non-debug stand alone interpreter)
	@echo

build_interpreter_d: ${TARGETID} dirs
	@echo
	@echo ${COMPONENT}: Build complete (debug stand alone interpreter)
	@echo

local_dirs:
	${MKDIR} o
	${MKDIR} z
        ${MKDIR} oi
        ${MKDIR} oid
	${TOUCH} local_dirs

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.JSLib
        ${MKDIR} ${LIBDIR}.JSLib.h
        ${MKDIR} ${LIBDIR}.JSLib.o

# -----------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	@echo
	${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	@echo
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

${TARGETI}: ${OBJSI} local_dirs
	@echo
	${LD} -o ${COMPONENTI} ${OBJSI} ${LIBSI}

${TARGETID}: ${OBJSID} local_dirs
	@echo
	${LD} -debug -o ${COMPONENTID} ${OBJSID} ${LIBSID}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# -----------------------------------------------------------------------------
# Dynamic dependencies:
