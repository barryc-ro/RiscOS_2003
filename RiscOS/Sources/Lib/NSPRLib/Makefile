# Makefile for NSPRLib
# ====================
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
# 10-Sep-1998	ADH	Much better system for MemLib, or any other library
#			to replace free, malloc, calloc and realloc. Define
#			USE_NSPRLIB_ALLOC_REGISTRATION to activate the
#			ROSallocs.c functions that allow registration of
#			different functions for the above memory allocation
#			calls. USEMEMLIB has been removed. By default,
#			USE_NSPRLIB_ALLOC_REGISTRATION is defined.

# -----------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
NSPRLIBDIR	= ${LIBDIR}.NSPRLib

# -----------------------------------------------------------------------------
# Generic options:
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

# -----------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:,TCPIPLibs:,RISCOSconf,NSPR.lib.ds,NSPR.pr.include,NSPR.pr.include.private,NSPR.lib.libc.include,NSPR.pr.src.io
VPATH		= RISCOSconf NSPR.lib.ds NSPR.lib.libc.src NSPR.pr.src.malloc NSPR.pr.src.misc NSPR.pr.src.io

# -----------------------------------------------------------------------------
# Compile-time options
#

DFLAGS		= -DCOMPAT_INET4  -DRISCOS  -DNSPR20  -DUSE_NSPRLIB_ALLOC_REGISTRATION
UFLAGS		= -UXP_UNIX  -UXP_OS2  -UXP_MAC

# -----------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= NSPRLib
COMPONENTZ	= NSPRLibzm
SOURCE		= ...
TARGET		= ${COMPONENT}
TARGETZ		= ${COMPONENTZ}
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.ROSallocs\
 o.NoThreads\
 o.strcat\
 o.strccmp\
 o.strchr\
 o.strcmp\
 o.strcpy\
 o.strcstr\
 o.strdup\
 o.strlen\
 o.strpbrk\
 o.strstr\
 o.plhash\
 o.plarena\
 o.prlog2\
 o.prdtoa\
 o.prlong\
 o.prprf\
 o.prmem\

OBJSZ =\
 z.ROSallocs\
 z.NoThreads\
 z.strcat\
 z.strccmp\
 z.strchr\
 z.strcmp\
 z.strcpy\
 z.strcstr\
 z.strdup\
 z.strlen\
 z.strpbrk\
 z.strstr\
 z.plhash\
 z.plarena\
 z.prlog2\
 z.prdtoa\
 z.prlong\
 z.prprf\
 z.prmem\

# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -o $@ $<

.c.z:;		@echo
		@echo Compiling $<
		@echo ${DFLAGS}
		@${CC} ${CCFLAGS} -zM -o $@ $<

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
# build the library:
#

all:		${COMPONENT} #${COMPONENTZ}

# -----------------------------------------------------------------------------
# RISC OS ROM build rules:
#

rom: ${TARGET} ${TARGETZ}
        @echo ${COMPONENT}: ROM module built

export: export_${PHASE}

install_rom: ${TARGET} ${TARGETZM}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: ROM module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} z ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGETZ}
        ${RM} local_dirs
        @echo
        @echo ${COMPONENT}: Cleaned

export_hdrs: dirs
	@echo
	${CP} NSPR.pr.include.h.*			${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} NSPR.lib.ds.h.*				${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} RISCOSconf.h.prcpucfg			${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} RISCOSconf.h.ROSallocs			${NSPRLIBDIR}.h.*		${CPFLAGS}
	${CP} NSPR.pr.include.obsolete.h.*		${NSPRLIBDIR}.obsolete.h.*	${CPFLAGS}
	${CP} NSPR.pr.include.md.h.prosdep		${NSPRLIBDIR}.md.h.*		${CPFLAGS}
	${CP} NSPR.pr.include.md.h._riscos		${NSPRLIBDIR}.md.h.*		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (headers)

export_libs: ${TARGET} ${TARGETZ} dirs
	@echo
	${CP} ${TARGET}		${NSPRLIBDIR}.o.${TARGET}	${CPFLAGS}
	${CP} ${TARGETZ}	${NSPRLIBDIR}.o.${TARGETZ}	${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Export complete (library)

local_dirs:
	${MKDIR} o
	${MKDIR} z
	${TOUCH} local_dirs

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${LIBDIR}.NSPRLib
        ${MKDIR} ${LIBDIR}.NSPRLib.h
        ${MKDIR} ${LIBDIR}.NSPRLib.md
        ${MKDIR} ${LIBDIR}.NSPRLib.md.h
        ${MKDIR} ${LIBDIR}.NSPRLib.obsolete
        ${MKDIR} ${LIBDIR}.NSPRLib.obsolete.h
        ${MKDIR} ${LIBDIR}.NSPRLib.o

# -----------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	@echo
	${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	@echo
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# -----------------------------------------------------------------------------
# Dynamic dependencies:
o.ROSallocs:	RISCOSconf.c.ROSallocs
o.ROSallocs:	RISCOSconf.h.ROSallocs
o.NoThreads:	RISCOSconf.c.NoThreads
o.NoThreads:	NSPR.pr.include.h.prtypes
o.NoThreads:	RISCOSconf.h.prcpucfg
o.NoThreads:	RISCOSconf.h.ROSallocs
o.NoThreads:	NSPR.pr.include.obsolete.h.protypes
o.NoThreads:	NSPR.pr.include.h.prerror
o.NoThreads:	NSPR.pr.include.h.prtypes
o.strcat:	NSPR.lib.libc.src.c.strcat
o.strcat:	NSPR.lib.libc.include.h.plstr
o.strcat:	NSPR.pr.include.h.prtypes
o.strcat:	RISCOSconf.h.prcpucfg
o.strcat:	RISCOSconf.h.ROSallocs
o.strcat:	NSPR.pr.include.obsolete.h.protypes
o.strccmp:	NSPR.lib.libc.src.c.strccmp
o.strccmp:	NSPR.lib.libc.include.h.plstr
o.strccmp:	NSPR.pr.include.h.prtypes
o.strccmp:	RISCOSconf.h.prcpucfg
o.strccmp:	RISCOSconf.h.ROSallocs
o.strccmp:	NSPR.pr.include.obsolete.h.protypes
o.strchr:	NSPR.lib.libc.src.c.strchr
o.strchr:	NSPR.lib.libc.include.h.plstr
o.strchr:	NSPR.pr.include.h.prtypes
o.strchr:	RISCOSconf.h.prcpucfg
o.strchr:	RISCOSconf.h.ROSallocs
o.strchr:	NSPR.pr.include.obsolete.h.protypes
o.strcmp:	NSPR.lib.libc.src.c.strcmp
o.strcmp:	NSPR.lib.libc.include.h.plstr
o.strcmp:	NSPR.pr.include.h.prtypes
o.strcmp:	RISCOSconf.h.prcpucfg
o.strcmp:	RISCOSconf.h.ROSallocs
o.strcmp:	NSPR.pr.include.obsolete.h.protypes
o.strcpy:	NSPR.lib.libc.src.c.strcpy
o.strcpy:	NSPR.lib.libc.include.h.plstr
o.strcpy:	NSPR.pr.include.h.prtypes
o.strcpy:	RISCOSconf.h.prcpucfg
o.strcpy:	RISCOSconf.h.ROSallocs
o.strcpy:	NSPR.pr.include.obsolete.h.protypes
o.strcstr:	NSPR.lib.libc.src.c.strcstr
o.strcstr:	NSPR.lib.libc.include.h.plstr
o.strcstr:	NSPR.pr.include.h.prtypes
o.strcstr:	RISCOSconf.h.prcpucfg
o.strcstr:	RISCOSconf.h.ROSallocs
o.strcstr:	NSPR.pr.include.obsolete.h.protypes
o.strdup:	NSPR.lib.libc.src.c.strdup
o.strdup:	NSPR.lib.libc.include.h.plstr
o.strdup:	NSPR.pr.include.h.prtypes
o.strdup:	RISCOSconf.h.prcpucfg
o.strdup:	RISCOSconf.h.ROSallocs
o.strdup:	NSPR.pr.include.obsolete.h.protypes
o.strdup:	NSPR.pr.include.h.prmem
o.strdup:	NSPR.pr.include.h.prtypes
o.strlen:	NSPR.lib.libc.src.c.strlen
o.strlen:	NSPR.lib.libc.include.h.plstr
o.strlen:	NSPR.pr.include.h.prtypes
o.strlen:	RISCOSconf.h.prcpucfg
o.strlen:	RISCOSconf.h.ROSallocs
o.strlen:	NSPR.pr.include.obsolete.h.protypes
o.strlen:	NSPR.pr.include.h.prtypes
o.strlen:	NSPR.pr.include.h.prlog
o.strlen:	NSPR.pr.include.h.prtypes
o.strpbrk:	NSPR.lib.libc.src.c.strpbrk
o.strpbrk:	NSPR.lib.libc.include.h.plstr
o.strpbrk:	NSPR.pr.include.h.prtypes
o.strpbrk:	RISCOSconf.h.prcpucfg
o.strpbrk:	RISCOSconf.h.ROSallocs
o.strpbrk:	NSPR.pr.include.obsolete.h.protypes
o.strstr:	NSPR.lib.libc.src.c.strstr
o.strstr:	NSPR.lib.libc.include.h.plstr
o.strstr:	NSPR.pr.include.h.prtypes
o.strstr:	RISCOSconf.h.prcpucfg
o.strstr:	RISCOSconf.h.ROSallocs
o.strstr:	NSPR.pr.include.obsolete.h.protypes
o.plhash:	NSPR.lib.ds.c.plhash
o.plhash:	NSPR.lib.ds.h.plhash
o.plhash:	NSPR.pr.include.h.prtypes
o.plhash:	RISCOSconf.h.prcpucfg
o.plhash:	RISCOSconf.h.ROSallocs
o.plhash:	NSPR.pr.include.obsolete.h.protypes
o.plhash:	NSPR.pr.include.h.prbit
o.plhash:	NSPR.pr.include.h.prtypes
o.plhash:	NSPR.pr.include.h.prlog
o.plhash:	NSPR.pr.include.h.prtypes
o.plhash:	NSPR.pr.include.h.prmem
o.plhash:	NSPR.pr.include.h.prtypes
o.plhash:	NSPR.pr.include.h.prtypes
o.plarena:	NSPR.lib.ds.c.plarena
o.plarena:	NSPR.lib.ds.h.plarena
o.plarena:	NSPR.pr.include.h.prtypes
o.plarena:	RISCOSconf.h.prcpucfg
o.plarena:	RISCOSconf.h.ROSallocs
o.plarena:	NSPR.pr.include.obsolete.h.protypes
o.plarena:	NSPR.lib.ds.h.plarenas
o.plarena:	NSPR.pr.include.h.prmem
o.plarena:	NSPR.pr.include.h.prtypes
o.plarena:	NSPR.pr.include.h.prbit
o.plarena:	NSPR.pr.include.h.prtypes
o.plarena:	NSPR.pr.include.h.prlog
o.plarena:	NSPR.pr.include.h.prtypes
o.prlog2:	NSPR.pr.src.misc.c.prlog2
o.prlog2:	NSPR.pr.include.h.prbit
o.prlog2:	NSPR.pr.include.h.prtypes
o.prlog2:	RISCOSconf.h.prcpucfg
o.prlog2:	RISCOSconf.h.ROSallocs
o.prlog2:	NSPR.pr.include.obsolete.h.protypes
o.prdtoa:	NSPR.pr.src.misc.c.prdtoa
o.prdtoa:	NSPR.pr.include.private.h.primpl
o.prdtoa:	NSPR.pr.include.h.nspr
o.prdtoa:	NSPR.pr.include.h.pratom
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	RISCOSconf.h.prcpucfg
o.prdtoa:	RISCOSconf.h.ROSallocs
o.prdtoa:	NSPR.pr.include.obsolete.h.protypes
o.prdtoa:	NSPR.pr.include.h.prbit
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prclist
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prcmon
o.prdtoa:	NSPR.pr.include.h.prmon
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prinrval
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prinrval
o.prdtoa:	NSPR.pr.include.h.prcvar
o.prdtoa:	NSPR.pr.include.h.prlock
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prinrval
o.prdtoa:	NSPR.pr.include.h.prdtoa
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prenv
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prerror
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prinet
o.prdtoa:	TCPIPLibs:sys.h.types
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	TCPIPLibs:machine.h.endian
o.prdtoa:	TCPIPLibs:machine.h.ansi
o.prdtoa:	TCPIPLibs:machine.h.types
o.prdtoa:	TCPIPLibs:sys.h.socket
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	TCPIPLibs:netinet.h.in
o.prdtoa:	TCPIPLibs:arpa.h.inet
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	TCPIPLibs:h.netdb
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	NSPR.pr.include.h.prinit
o.prdtoa:	NSPR.pr.include.h.prthread
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prinrval
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prwin16
o.prdtoa:	NSPR.pr.include.h.prinrval
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	NSPR.pr.include.h.prlong
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prtime
o.prdtoa:	NSPR.pr.include.h.prlong
o.prdtoa:	NSPR.pr.include.h.prinrval
o.prdtoa:	NSPR.pr.include.h.prinet
o.prdtoa:	NSPR.pr.include.h.prlink
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prlock
o.prdtoa:	NSPR.pr.include.h.prlog
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prlong
o.prdtoa:	NSPR.pr.include.h.prmem
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prmon
o.prdtoa:	NSPR.pr.include.h.prnetdb
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	NSPR.pr.include.h.prprf
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	NSPR.pr.include.h.prproces
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	NSPR.pr.include.h.prsystem
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prthread
o.prdtoa:	NSPR.pr.include.h.prtime
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.private.h.prpriv
o.prdtoa:	NSPR.pr.include.private.h.pprio
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	NSPR.pr.include.private.h.pprthred
o.prdtoa:	NSPR.pr.include.h.nspr
o.prdtoa:	NSPR.pr.include.md.h.prosdep
o.prdtoa:	NSPR.pr.include.h.prtypes
o.prdtoa:	NSPR.pr.include.md.h._riscos
o.prdtoa:	TCPIPLibs:h.unistd
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	TCPIPLibs:sys.h.types
o.prdtoa:	TCPIPLibs:sys.h.unistd
o.prdtoa:	TCPIPLibs:sys.h.stat
o.prdtoa:	TCPIPLibs:sys.h.time
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	TCPIPLibs:h.dirent
o.prdtoa:	TCPIPLibs:sys.h.dirent
o.prdtoa:	TCPIPLibs:sys.h.cdefs
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	NSPR.pr.include.h.prmem
o.prdtoa:	NSPR.pr.include.h.prclist
o.prdtoa:	TCPIPLibs:sys.h.types
o.prdtoa:	TCPIPLibs:sys.h.types
o.prdtoa:	TCPIPLibs:sys.h.time
o.prdtoa:	TCPIPLibs:sys.h.stat
o.prdtoa:	TCPIPLibs:sys.h.syscall
o.prdtoa:	NSPR.pr.include.obsolete.h.probslet
o.prdtoa:	NSPR.pr.include.h.prio
o.prdtoa:	TCPIPLibs:sys.h.stat
o.prdtoa:	C:h.string
o.prdtoa:	C:h.float
o.prdtoa:	C:h.math
o.prlong:	NSPR.pr.src.misc.c.prlong
o.prlong:	NSPR.pr.include.private.h.primpl
o.prlong:	NSPR.pr.include.h.nspr
o.prlong:	NSPR.pr.include.h.pratom
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	RISCOSconf.h.prcpucfg
o.prlong:	RISCOSconf.h.ROSallocs
o.prlong:	NSPR.pr.include.obsolete.h.protypes
o.prlong:	NSPR.pr.include.h.prbit
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prclist
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prcmon
o.prlong:	NSPR.pr.include.h.prmon
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prinrval
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prinrval
o.prlong:	NSPR.pr.include.h.prcvar
o.prlong:	NSPR.pr.include.h.prlock
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prinrval
o.prlong:	NSPR.pr.include.h.prdtoa
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prenv
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prerror
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prinet
o.prlong:	TCPIPLibs:sys.h.types
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	TCPIPLibs:machine.h.endian
o.prlong:	TCPIPLibs:machine.h.ansi
o.prlong:	TCPIPLibs:machine.h.types
o.prlong:	TCPIPLibs:sys.h.socket
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	TCPIPLibs:netinet.h.in
o.prlong:	TCPIPLibs:arpa.h.inet
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	TCPIPLibs:h.netdb
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	NSPR.pr.include.h.prinit
o.prlong:	NSPR.pr.include.h.prthread
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prinrval
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prwin16
o.prlong:	NSPR.pr.include.h.prinrval
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	NSPR.pr.include.h.prlong
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prtime
o.prlong:	NSPR.pr.include.h.prlong
o.prlong:	NSPR.pr.include.h.prinrval
o.prlong:	NSPR.pr.include.h.prinet
o.prlong:	NSPR.pr.include.h.prlink
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prlock
o.prlong:	NSPR.pr.include.h.prlog
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prlong
o.prlong:	NSPR.pr.include.h.prmem
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prmon
o.prlong:	NSPR.pr.include.h.prnetdb
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	NSPR.pr.include.h.prprf
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	NSPR.pr.include.h.prproces
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	NSPR.pr.include.h.prsystem
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prthread
o.prlong:	NSPR.pr.include.h.prtime
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.private.h.prpriv
o.prlong:	NSPR.pr.include.private.h.pprio
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	NSPR.pr.include.private.h.pprthred
o.prlong:	NSPR.pr.include.h.nspr
o.prlong:	NSPR.pr.include.md.h.prosdep
o.prlong:	NSPR.pr.include.h.prtypes
o.prlong:	NSPR.pr.include.md.h._riscos
o.prlong:	TCPIPLibs:h.unistd
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	TCPIPLibs:sys.h.types
o.prlong:	TCPIPLibs:sys.h.unistd
o.prlong:	TCPIPLibs:sys.h.stat
o.prlong:	TCPIPLibs:sys.h.time
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	TCPIPLibs:h.dirent
o.prlong:	TCPIPLibs:sys.h.dirent
o.prlong:	TCPIPLibs:sys.h.cdefs
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	NSPR.pr.include.h.prmem
o.prlong:	NSPR.pr.include.h.prclist
o.prlong:	TCPIPLibs:sys.h.types
o.prlong:	TCPIPLibs:sys.h.types
o.prlong:	TCPIPLibs:sys.h.time
o.prlong:	TCPIPLibs:sys.h.stat
o.prlong:	TCPIPLibs:sys.h.syscall
o.prlong:	NSPR.pr.include.obsolete.h.probslet
o.prlong:	NSPR.pr.include.h.prio
o.prlong:	TCPIPLibs:sys.h.stat
o.prprf:	NSPR.pr.src.io.c.prprf
o.prprf:	NSPR.pr.include.private.h.primpl
o.prprf:	NSPR.pr.include.h.nspr
o.prprf:	NSPR.pr.include.h.pratom
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	RISCOSconf.h.prcpucfg
o.prprf:	RISCOSconf.h.ROSallocs
o.prprf:	NSPR.pr.include.obsolete.h.protypes
o.prprf:	NSPR.pr.include.h.prbit
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prclist
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prcmon
o.prprf:	NSPR.pr.include.h.prmon
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prinrval
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prinrval
o.prprf:	NSPR.pr.include.h.prcvar
o.prprf:	NSPR.pr.include.h.prlock
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prinrval
o.prprf:	NSPR.pr.include.h.prdtoa
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prenv
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prerror
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prinet
o.prprf:	TCPIPLibs:sys.h.types
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	TCPIPLibs:machine.h.endian
o.prprf:	TCPIPLibs:machine.h.ansi
o.prprf:	TCPIPLibs:machine.h.types
o.prprf:	TCPIPLibs:sys.h.socket
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	TCPIPLibs:netinet.h.in
o.prprf:	TCPIPLibs:arpa.h.inet
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	TCPIPLibs:h.netdb
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	NSPR.pr.include.h.prinit
o.prprf:	NSPR.pr.include.h.prthread
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prinrval
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prwin16
o.prprf:	NSPR.pr.include.h.prinrval
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	NSPR.pr.include.h.prlong
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prtime
o.prprf:	NSPR.pr.include.h.prlong
o.prprf:	NSPR.pr.include.h.prinrval
o.prprf:	NSPR.pr.include.h.prinet
o.prprf:	NSPR.pr.include.h.prlink
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prlock
o.prprf:	NSPR.pr.include.h.prlog
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prlong
o.prprf:	NSPR.pr.include.h.prmem
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prmon
o.prprf:	NSPR.pr.include.h.prnetdb
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	NSPR.pr.include.h.prprf
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	NSPR.pr.include.h.prproces
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	NSPR.pr.include.h.prsystem
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prthread
o.prprf:	NSPR.pr.include.h.prtime
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.private.h.prpriv
o.prprf:	NSPR.pr.include.private.h.pprio
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	NSPR.pr.include.private.h.pprthred
o.prprf:	NSPR.pr.include.h.nspr
o.prprf:	NSPR.pr.include.md.h.prosdep
o.prprf:	NSPR.pr.include.h.prtypes
o.prprf:	NSPR.pr.include.md.h._riscos
o.prprf:	TCPIPLibs:h.unistd
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	TCPIPLibs:sys.h.types
o.prprf:	TCPIPLibs:sys.h.unistd
o.prprf:	TCPIPLibs:sys.h.stat
o.prprf:	TCPIPLibs:sys.h.time
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	TCPIPLibs:h.dirent
o.prprf:	TCPIPLibs:sys.h.dirent
o.prprf:	TCPIPLibs:sys.h.cdefs
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	NSPR.pr.include.h.prmem
o.prprf:	NSPR.pr.include.h.prclist
o.prprf:	TCPIPLibs:sys.h.types
o.prprf:	TCPIPLibs:sys.h.types
o.prprf:	TCPIPLibs:sys.h.time
o.prprf:	TCPIPLibs:sys.h.stat
o.prprf:	TCPIPLibs:sys.h.syscall
o.prprf:	NSPR.pr.include.obsolete.h.probslet
o.prprf:	NSPR.pr.include.h.prio
o.prprf:	TCPIPLibs:sys.h.stat
o.prprf:	NSPR.pr.include.h.prprf
o.prprf:	NSPR.pr.include.h.prlong
o.prprf:	NSPR.pr.include.h.prlog
o.prprf:	NSPR.pr.include.h.prmem
o.prmem:	NSPR.pr.src.malloc.c.prmem
o.prmem:	NSPR.pr.include.private.h.primpl
o.prmem:	NSPR.pr.include.h.nspr
o.prmem:	NSPR.pr.include.h.pratom
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	RISCOSconf.h.prcpucfg
o.prmem:	RISCOSconf.h.ROSallocs
o.prmem:	NSPR.pr.include.obsolete.h.protypes
o.prmem:	NSPR.pr.include.h.prbit
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prclist
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prcmon
o.prmem:	NSPR.pr.include.h.prmon
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prinrval
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prinrval
o.prmem:	NSPR.pr.include.h.prcvar
o.prmem:	NSPR.pr.include.h.prlock
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prinrval
o.prmem:	NSPR.pr.include.h.prdtoa
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prenv
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prerror
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prinet
o.prmem:	TCPIPLibs:sys.h.types
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	TCPIPLibs:machine.h.endian
o.prmem:	TCPIPLibs:machine.h.ansi
o.prmem:	TCPIPLibs:machine.h.types
o.prmem:	TCPIPLibs:sys.h.socket
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	TCPIPLibs:netinet.h.in
o.prmem:	TCPIPLibs:arpa.h.inet
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	TCPIPLibs:h.netdb
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	NSPR.pr.include.h.prinit
o.prmem:	NSPR.pr.include.h.prthread
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prinrval
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prwin16
o.prmem:	NSPR.pr.include.h.prinrval
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	NSPR.pr.include.h.prlong
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prtime
o.prmem:	NSPR.pr.include.h.prlong
o.prmem:	NSPR.pr.include.h.prinrval
o.prmem:	NSPR.pr.include.h.prinet
o.prmem:	NSPR.pr.include.h.prlink
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prlock
o.prmem:	NSPR.pr.include.h.prlog
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prlong
o.prmem:	NSPR.pr.include.h.prmem
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prmon
o.prmem:	NSPR.pr.include.h.prnetdb
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	NSPR.pr.include.h.prprf
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	NSPR.pr.include.h.prproces
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	NSPR.pr.include.h.prsystem
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prthread
o.prmem:	NSPR.pr.include.h.prtime
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.private.h.prpriv
o.prmem:	NSPR.pr.include.private.h.pprio
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	NSPR.pr.include.private.h.pprthred
o.prmem:	NSPR.pr.include.h.nspr
o.prmem:	NSPR.pr.include.md.h.prosdep
o.prmem:	NSPR.pr.include.h.prtypes
o.prmem:	NSPR.pr.include.md.h._riscos
o.prmem:	TCPIPLibs:h.unistd
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	TCPIPLibs:sys.h.types
o.prmem:	TCPIPLibs:sys.h.unistd
o.prmem:	TCPIPLibs:sys.h.stat
o.prmem:	TCPIPLibs:sys.h.time
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	TCPIPLibs:h.dirent
o.prmem:	TCPIPLibs:sys.h.dirent
o.prmem:	TCPIPLibs:sys.h.cdefs
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	NSPR.pr.include.h.prmem
o.prmem:	NSPR.pr.include.h.prclist
o.prmem:	TCPIPLibs:sys.h.types
o.prmem:	TCPIPLibs:sys.h.types
o.prmem:	TCPIPLibs:sys.h.time
o.prmem:	TCPIPLibs:sys.h.stat
o.prmem:	TCPIPLibs:sys.h.syscall
o.prmem:	NSPR.pr.include.obsolete.h.probslet
o.prmem:	NSPR.pr.include.h.prio
o.prmem:	TCPIPLibs:sys.h.stat
z.ROSallocs:	RISCOSconf.c.ROSallocs
z.ROSallocs:	RISCOSconf.h.ROSallocs
z.NoThreads:	RISCOSconf.c.NoThreads
z.NoThreads:	NSPR.pr.include.h.prtypes
z.NoThreads:	RISCOSconf.h.prcpucfg
z.NoThreads:	RISCOSconf.h.ROSallocs
z.NoThreads:	NSPR.pr.include.obsolete.h.protypes
z.NoThreads:	NSPR.pr.include.h.prerror
z.NoThreads:	NSPR.pr.include.h.prtypes
z.strcat:	NSPR.lib.libc.src.c.strcat
z.strcat:	NSPR.lib.libc.include.h.plstr
z.strcat:	NSPR.pr.include.h.prtypes
z.strcat:	RISCOSconf.h.prcpucfg
z.strcat:	RISCOSconf.h.ROSallocs
z.strcat:	NSPR.pr.include.obsolete.h.protypes
z.strccmp:	NSPR.lib.libc.src.c.strccmp
z.strccmp:	NSPR.lib.libc.include.h.plstr
z.strccmp:	NSPR.pr.include.h.prtypes
z.strccmp:	RISCOSconf.h.prcpucfg
z.strccmp:	RISCOSconf.h.ROSallocs
z.strccmp:	NSPR.pr.include.obsolete.h.protypes
z.strchr:	NSPR.lib.libc.src.c.strchr
z.strchr:	NSPR.lib.libc.include.h.plstr
z.strchr:	NSPR.pr.include.h.prtypes
z.strchr:	RISCOSconf.h.prcpucfg
z.strchr:	RISCOSconf.h.ROSallocs
z.strchr:	NSPR.pr.include.obsolete.h.protypes
z.strcmp:	NSPR.lib.libc.src.c.strcmp
z.strcmp:	NSPR.lib.libc.include.h.plstr
z.strcmp:	NSPR.pr.include.h.prtypes
z.strcmp:	RISCOSconf.h.prcpucfg
z.strcmp:	RISCOSconf.h.ROSallocs
z.strcmp:	NSPR.pr.include.obsolete.h.protypes
z.strcpy:	NSPR.lib.libc.src.c.strcpy
z.strcpy:	NSPR.lib.libc.include.h.plstr
z.strcpy:	NSPR.pr.include.h.prtypes
z.strcpy:	RISCOSconf.h.prcpucfg
z.strcpy:	RISCOSconf.h.ROSallocs
z.strcpy:	NSPR.pr.include.obsolete.h.protypes
z.strcstr:	NSPR.lib.libc.src.c.strcstr
z.strcstr:	NSPR.lib.libc.include.h.plstr
z.strcstr:	NSPR.pr.include.h.prtypes
z.strcstr:	RISCOSconf.h.prcpucfg
z.strcstr:	RISCOSconf.h.ROSallocs
z.strcstr:	NSPR.pr.include.obsolete.h.protypes
z.strdup:	NSPR.lib.libc.src.c.strdup
z.strdup:	NSPR.lib.libc.include.h.plstr
z.strdup:	NSPR.pr.include.h.prtypes
z.strdup:	RISCOSconf.h.prcpucfg
z.strdup:	RISCOSconf.h.ROSallocs
z.strdup:	NSPR.pr.include.obsolete.h.protypes
z.strdup:	NSPR.pr.include.h.prmem
z.strdup:	NSPR.pr.include.h.prtypes
z.strlen:	NSPR.lib.libc.src.c.strlen
z.strlen:	NSPR.lib.libc.include.h.plstr
z.strlen:	NSPR.pr.include.h.prtypes
z.strlen:	RISCOSconf.h.prcpucfg
z.strlen:	RISCOSconf.h.ROSallocs
z.strlen:	NSPR.pr.include.obsolete.h.protypes
z.strlen:	NSPR.pr.include.h.prtypes
z.strlen:	NSPR.pr.include.h.prlog
z.strlen:	NSPR.pr.include.h.prtypes
z.strpbrk:	NSPR.lib.libc.src.c.strpbrk
z.strpbrk:	NSPR.lib.libc.include.h.plstr
z.strpbrk:	NSPR.pr.include.h.prtypes
z.strpbrk:	RISCOSconf.h.prcpucfg
z.strpbrk:	RISCOSconf.h.ROSallocs
z.strpbrk:	NSPR.pr.include.obsolete.h.protypes
z.strstr:	NSPR.lib.libc.src.c.strstr
z.strstr:	NSPR.lib.libc.include.h.plstr
z.strstr:	NSPR.pr.include.h.prtypes
z.strstr:	RISCOSconf.h.prcpucfg
z.strstr:	RISCOSconf.h.ROSallocs
z.strstr:	NSPR.pr.include.obsolete.h.protypes
z.plhash:	NSPR.lib.ds.c.plhash
z.plhash:	NSPR.lib.ds.h.plhash
z.plhash:	NSPR.pr.include.h.prtypes
z.plhash:	RISCOSconf.h.prcpucfg
z.plhash:	RISCOSconf.h.ROSallocs
z.plhash:	NSPR.pr.include.obsolete.h.protypes
z.plhash:	NSPR.pr.include.h.prbit
z.plhash:	NSPR.pr.include.h.prtypes
z.plhash:	NSPR.pr.include.h.prlog
z.plhash:	NSPR.pr.include.h.prtypes
z.plhash:	NSPR.pr.include.h.prmem
z.plhash:	NSPR.pr.include.h.prtypes
z.plhash:	NSPR.pr.include.h.prtypes
z.plarena:	NSPR.lib.ds.c.plarena
z.plarena:	NSPR.lib.ds.h.plarena
z.plarena:	NSPR.pr.include.h.prtypes
z.plarena:	RISCOSconf.h.prcpucfg
z.plarena:	RISCOSconf.h.ROSallocs
z.plarena:	NSPR.pr.include.obsolete.h.protypes
z.plarena:	NSPR.lib.ds.h.plarenas
z.plarena:	NSPR.pr.include.h.prmem
z.plarena:	NSPR.pr.include.h.prtypes
z.plarena:	NSPR.pr.include.h.prbit
z.plarena:	NSPR.pr.include.h.prtypes
z.plarena:	NSPR.pr.include.h.prlog
z.plarena:	NSPR.pr.include.h.prtypes
z.prlog2:	NSPR.pr.src.misc.c.prlog2
z.prlog2:	NSPR.pr.include.h.prbit
z.prlog2:	NSPR.pr.include.h.prtypes
z.prlog2:	RISCOSconf.h.prcpucfg
z.prlog2:	RISCOSconf.h.ROSallocs
z.prlog2:	NSPR.pr.include.obsolete.h.protypes
z.prdtoa:	NSPR.pr.src.misc.c.prdtoa
z.prdtoa:	NSPR.pr.include.private.h.primpl
z.prdtoa:	NSPR.pr.include.h.nspr
z.prdtoa:	NSPR.pr.include.h.pratom
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	RISCOSconf.h.prcpucfg
z.prdtoa:	RISCOSconf.h.ROSallocs
z.prdtoa:	NSPR.pr.include.obsolete.h.protypes
z.prdtoa:	NSPR.pr.include.h.prbit
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prclist
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prcmon
z.prdtoa:	NSPR.pr.include.h.prmon
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prinrval
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prinrval
z.prdtoa:	NSPR.pr.include.h.prcvar
z.prdtoa:	NSPR.pr.include.h.prlock
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prinrval
z.prdtoa:	NSPR.pr.include.h.prdtoa
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prenv
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prerror
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prinet
z.prdtoa:	TCPIPLibs:sys.h.types
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	TCPIPLibs:machine.h.endian
z.prdtoa:	TCPIPLibs:machine.h.ansi
z.prdtoa:	TCPIPLibs:machine.h.types
z.prdtoa:	TCPIPLibs:sys.h.socket
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	TCPIPLibs:netinet.h.in
z.prdtoa:	TCPIPLibs:arpa.h.inet
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	TCPIPLibs:h.netdb
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	NSPR.pr.include.h.prinit
z.prdtoa:	NSPR.pr.include.h.prthread
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prinrval
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prwin16
z.prdtoa:	NSPR.pr.include.h.prinrval
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	NSPR.pr.include.h.prlong
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prtime
z.prdtoa:	NSPR.pr.include.h.prlong
z.prdtoa:	NSPR.pr.include.h.prinrval
z.prdtoa:	NSPR.pr.include.h.prinet
z.prdtoa:	NSPR.pr.include.h.prlink
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prlock
z.prdtoa:	NSPR.pr.include.h.prlog
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prlong
z.prdtoa:	NSPR.pr.include.h.prmem
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prmon
z.prdtoa:	NSPR.pr.include.h.prnetdb
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	NSPR.pr.include.h.prprf
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	NSPR.pr.include.h.prproces
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	NSPR.pr.include.h.prsystem
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prthread
z.prdtoa:	NSPR.pr.include.h.prtime
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.private.h.prpriv
z.prdtoa:	NSPR.pr.include.private.h.pprio
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	NSPR.pr.include.private.h.pprthred
z.prdtoa:	NSPR.pr.include.h.nspr
z.prdtoa:	NSPR.pr.include.md.h.prosdep
z.prdtoa:	NSPR.pr.include.h.prtypes
z.prdtoa:	NSPR.pr.include.md.h._riscos
z.prdtoa:	TCPIPLibs:h.unistd
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	TCPIPLibs:sys.h.types
z.prdtoa:	TCPIPLibs:sys.h.unistd
z.prdtoa:	TCPIPLibs:sys.h.stat
z.prdtoa:	TCPIPLibs:sys.h.time
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	TCPIPLibs:h.dirent
z.prdtoa:	TCPIPLibs:sys.h.dirent
z.prdtoa:	TCPIPLibs:sys.h.cdefs
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	NSPR.pr.include.h.prmem
z.prdtoa:	NSPR.pr.include.h.prclist
z.prdtoa:	TCPIPLibs:sys.h.types
z.prdtoa:	TCPIPLibs:sys.h.types
z.prdtoa:	TCPIPLibs:sys.h.time
z.prdtoa:	TCPIPLibs:sys.h.stat
z.prdtoa:	TCPIPLibs:sys.h.syscall
z.prdtoa:	NSPR.pr.include.obsolete.h.probslet
z.prdtoa:	NSPR.pr.include.h.prio
z.prdtoa:	TCPIPLibs:sys.h.stat
z.prdtoa:	C:h.string
z.prdtoa:	C:h.float
z.prdtoa:	C:h.math
z.prlong:	NSPR.pr.src.misc.c.prlong
z.prlong:	NSPR.pr.include.private.h.primpl
z.prlong:	NSPR.pr.include.h.nspr
z.prlong:	NSPR.pr.include.h.pratom
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	RISCOSconf.h.prcpucfg
z.prlong:	RISCOSconf.h.ROSallocs
z.prlong:	NSPR.pr.include.obsolete.h.protypes
z.prlong:	NSPR.pr.include.h.prbit
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prclist
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prcmon
z.prlong:	NSPR.pr.include.h.prmon
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prinrval
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prinrval
z.prlong:	NSPR.pr.include.h.prcvar
z.prlong:	NSPR.pr.include.h.prlock
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prinrval
z.prlong:	NSPR.pr.include.h.prdtoa
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prenv
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prerror
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prinet
z.prlong:	TCPIPLibs:sys.h.types
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	TCPIPLibs:machine.h.endian
z.prlong:	TCPIPLibs:machine.h.ansi
z.prlong:	TCPIPLibs:machine.h.types
z.prlong:	TCPIPLibs:sys.h.socket
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	TCPIPLibs:netinet.h.in
z.prlong:	TCPIPLibs:arpa.h.inet
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	TCPIPLibs:h.netdb
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	NSPR.pr.include.h.prinit
z.prlong:	NSPR.pr.include.h.prthread
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prinrval
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prwin16
z.prlong:	NSPR.pr.include.h.prinrval
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	NSPR.pr.include.h.prlong
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prtime
z.prlong:	NSPR.pr.include.h.prlong
z.prlong:	NSPR.pr.include.h.prinrval
z.prlong:	NSPR.pr.include.h.prinet
z.prlong:	NSPR.pr.include.h.prlink
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prlock
z.prlong:	NSPR.pr.include.h.prlog
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prlong
z.prlong:	NSPR.pr.include.h.prmem
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prmon
z.prlong:	NSPR.pr.include.h.prnetdb
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	NSPR.pr.include.h.prprf
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	NSPR.pr.include.h.prproces
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	NSPR.pr.include.h.prsystem
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prthread
z.prlong:	NSPR.pr.include.h.prtime
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.private.h.prpriv
z.prlong:	NSPR.pr.include.private.h.pprio
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	NSPR.pr.include.private.h.pprthred
z.prlong:	NSPR.pr.include.h.nspr
z.prlong:	NSPR.pr.include.md.h.prosdep
z.prlong:	NSPR.pr.include.h.prtypes
z.prlong:	NSPR.pr.include.md.h._riscos
z.prlong:	TCPIPLibs:h.unistd
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	TCPIPLibs:sys.h.types
z.prlong:	TCPIPLibs:sys.h.unistd
z.prlong:	TCPIPLibs:sys.h.stat
z.prlong:	TCPIPLibs:sys.h.time
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	TCPIPLibs:h.dirent
z.prlong:	TCPIPLibs:sys.h.dirent
z.prlong:	TCPIPLibs:sys.h.cdefs
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	NSPR.pr.include.h.prmem
z.prlong:	NSPR.pr.include.h.prclist
z.prlong:	TCPIPLibs:sys.h.types
z.prlong:	TCPIPLibs:sys.h.types
z.prlong:	TCPIPLibs:sys.h.time
z.prlong:	TCPIPLibs:sys.h.stat
z.prlong:	TCPIPLibs:sys.h.syscall
z.prlong:	NSPR.pr.include.obsolete.h.probslet
z.prlong:	NSPR.pr.include.h.prio
z.prlong:	TCPIPLibs:sys.h.stat
z.prprf:	NSPR.pr.src.io.c.prprf
z.prprf:	NSPR.pr.include.private.h.primpl
z.prprf:	NSPR.pr.include.h.nspr
z.prprf:	NSPR.pr.include.h.pratom
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	RISCOSconf.h.prcpucfg
z.prprf:	RISCOSconf.h.ROSallocs
z.prprf:	NSPR.pr.include.obsolete.h.protypes
z.prprf:	NSPR.pr.include.h.prbit
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prclist
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prcmon
z.prprf:	NSPR.pr.include.h.prmon
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prinrval
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prinrval
z.prprf:	NSPR.pr.include.h.prcvar
z.prprf:	NSPR.pr.include.h.prlock
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prinrval
z.prprf:	NSPR.pr.include.h.prdtoa
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prenv
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prerror
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prinet
z.prprf:	TCPIPLibs:sys.h.types
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	TCPIPLibs:machine.h.endian
z.prprf:	TCPIPLibs:machine.h.ansi
z.prprf:	TCPIPLibs:machine.h.types
z.prprf:	TCPIPLibs:sys.h.socket
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	TCPIPLibs:netinet.h.in
z.prprf:	TCPIPLibs:arpa.h.inet
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	TCPIPLibs:h.netdb
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	NSPR.pr.include.h.prinit
z.prprf:	NSPR.pr.include.h.prthread
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prinrval
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prwin16
z.prprf:	NSPR.pr.include.h.prinrval
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	NSPR.pr.include.h.prlong
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prtime
z.prprf:	NSPR.pr.include.h.prlong
z.prprf:	NSPR.pr.include.h.prinrval
z.prprf:	NSPR.pr.include.h.prinet
z.prprf:	NSPR.pr.include.h.prlink
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prlock
z.prprf:	NSPR.pr.include.h.prlog
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prlong
z.prprf:	NSPR.pr.include.h.prmem
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prmon
z.prprf:	NSPR.pr.include.h.prnetdb
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	NSPR.pr.include.h.prprf
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	NSPR.pr.include.h.prproces
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	NSPR.pr.include.h.prsystem
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prthread
z.prprf:	NSPR.pr.include.h.prtime
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.private.h.prpriv
z.prprf:	NSPR.pr.include.private.h.pprio
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	NSPR.pr.include.private.h.pprthred
z.prprf:	NSPR.pr.include.h.nspr
z.prprf:	NSPR.pr.include.md.h.prosdep
z.prprf:	NSPR.pr.include.h.prtypes
z.prprf:	NSPR.pr.include.md.h._riscos
z.prprf:	TCPIPLibs:h.unistd
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	TCPIPLibs:sys.h.types
z.prprf:	TCPIPLibs:sys.h.unistd
z.prprf:	TCPIPLibs:sys.h.stat
z.prprf:	TCPIPLibs:sys.h.time
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	TCPIPLibs:h.dirent
z.prprf:	TCPIPLibs:sys.h.dirent
z.prprf:	TCPIPLibs:sys.h.cdefs
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	NSPR.pr.include.h.prmem
z.prprf:	NSPR.pr.include.h.prclist
z.prprf:	TCPIPLibs:sys.h.types
z.prprf:	TCPIPLibs:sys.h.types
z.prprf:	TCPIPLibs:sys.h.time
z.prprf:	TCPIPLibs:sys.h.stat
z.prprf:	TCPIPLibs:sys.h.syscall
z.prprf:	NSPR.pr.include.obsolete.h.probslet
z.prprf:	NSPR.pr.include.h.prio
z.prprf:	TCPIPLibs:sys.h.stat
z.prprf:	NSPR.pr.include.h.prprf
z.prprf:	NSPR.pr.include.h.prlong
z.prprf:	NSPR.pr.include.h.prlog
z.prprf:	NSPR.pr.include.h.prmem
z.prmem:	NSPR.pr.src.malloc.c.prmem
z.prmem:	NSPR.pr.include.private.h.primpl
z.prmem:	NSPR.pr.include.h.nspr
z.prmem:	NSPR.pr.include.h.pratom
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	RISCOSconf.h.prcpucfg
z.prmem:	RISCOSconf.h.ROSallocs
z.prmem:	NSPR.pr.include.obsolete.h.protypes
z.prmem:	NSPR.pr.include.h.prbit
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prclist
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prcmon
z.prmem:	NSPR.pr.include.h.prmon
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prinrval
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prinrval
z.prmem:	NSPR.pr.include.h.prcvar
z.prmem:	NSPR.pr.include.h.prlock
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prinrval
z.prmem:	NSPR.pr.include.h.prdtoa
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prenv
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prerror
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prinet
z.prmem:	TCPIPLibs:sys.h.types
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	TCPIPLibs:machine.h.endian
z.prmem:	TCPIPLibs:machine.h.ansi
z.prmem:	TCPIPLibs:machine.h.types
z.prmem:	TCPIPLibs:sys.h.socket
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	TCPIPLibs:netinet.h.in
z.prmem:	TCPIPLibs:arpa.h.inet
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	TCPIPLibs:h.netdb
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	NSPR.pr.include.h.prinit
z.prmem:	NSPR.pr.include.h.prthread
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prinrval
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prwin16
z.prmem:	NSPR.pr.include.h.prinrval
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	NSPR.pr.include.h.prlong
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prtime
z.prmem:	NSPR.pr.include.h.prlong
z.prmem:	NSPR.pr.include.h.prinrval
z.prmem:	NSPR.pr.include.h.prinet
z.prmem:	NSPR.pr.include.h.prlink
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prlock
z.prmem:	NSPR.pr.include.h.prlog
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prlong
z.prmem:	NSPR.pr.include.h.prmem
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prmon
z.prmem:	NSPR.pr.include.h.prnetdb
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	NSPR.pr.include.h.prprf
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	NSPR.pr.include.h.prproces
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	NSPR.pr.include.h.prsystem
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prthread
z.prmem:	NSPR.pr.include.h.prtime
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.private.h.prpriv
z.prmem:	NSPR.pr.include.private.h.pprio
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	NSPR.pr.include.private.h.pprthred
z.prmem:	NSPR.pr.include.h.nspr
z.prmem:	NSPR.pr.include.md.h.prosdep
z.prmem:	NSPR.pr.include.h.prtypes
z.prmem:	NSPR.pr.include.md.h._riscos
z.prmem:	TCPIPLibs:h.unistd
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	TCPIPLibs:sys.h.types
z.prmem:	TCPIPLibs:sys.h.unistd
z.prmem:	TCPIPLibs:sys.h.stat
z.prmem:	TCPIPLibs:sys.h.time
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	TCPIPLibs:h.dirent
z.prmem:	TCPIPLibs:sys.h.dirent
z.prmem:	TCPIPLibs:sys.h.cdefs
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	NSPR.pr.include.h.prmem
z.prmem:	NSPR.pr.include.h.prclist
z.prmem:	TCPIPLibs:sys.h.types
z.prmem:	TCPIPLibs:sys.h.types
z.prmem:	TCPIPLibs:sys.h.time
z.prmem:	TCPIPLibs:sys.h.stat
z.prmem:	TCPIPLibs:sys.h.syscall
z.prmem:	NSPR.pr.include.obsolete.h.probslet
z.prmem:	NSPR.pr.include.h.prio
z.prmem:	TCPIPLibs:sys.h.stat
