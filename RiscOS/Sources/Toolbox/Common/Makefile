# Makefile for tboxlib and debuglib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09 Oct 1997  RWB 	  Tidied up old Toolbox.Common build
#

#
# Paths
#
LIBDIR		= <Lib$Dir>
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
LIBFILE = libfile

CFLAGS   = -c -depend !Depend -Wap -zps0 ${INCLUDES} -fah -Fn ${DFLAGS}
CFLAGSZM = -c -depend !Depend -Wap -zps0 ${INCLUDES} -fah -Fn ${DFLAGS} -zM
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v
LIBFILEFLAGS = -c -o $@

CCZM=		${CC} ${CFLAGSZM}

#
# Libraries
#
CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym
#
# Include files
#
INCLUDES 	= -IC:,TBox:
DFLAGS		= 

# Program specific options:
#
TBOXLIB   = tboxlib
TBOXLIBZM = tboxlibzm
DEBUGLIB  = debuglib
DEBUGLIBZM = debuglibzm

EXPORTS     = ${LIBDIR}.tboxlibs

OBJ      = o.mem o.messages o.string32 o.rmensure o.objsupport
OBJZM    = z.mem z.messages z.string32 z.rmensure z.objsupport

DBG_OBJ	  = o.debug o.slist
DBG_OBJZM = z.debug z.slist

#
# Rule patterns
#
.SUFFIXES: .z .o .c .s
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.z:;      ${CCZM} -c -o $@ $<

#
# Build
#

${TBOXLIB}: ${OBJ}
   ${LIBFILE} ${LIBFILEFLAGS} ${OBJ}

${TBOXLIBZM}: ${OBJZM}
   ${LIBFILE} ${LIBFILEFLAGS} ${OBJZM}

${DEBUGLIB}: ${OBJ} ${DBG_OBJ}
   ${LIBFILE} ${LIBFILEFLAGS} ${OBJ} ${DBG_OBJ}

${DEBUGLIBZM}: ${OBJZM} ${DBG_OBJAM}
   ${LIBFILE} ${LIBFILEFLAGS} ${OBJZM} ${DBG_OBJAM}

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} z.* ${WFLAGS}
	${WIPE} ${DEBUGLIB} ${WFLAGS}
	${WIPE} ${DEBUGLIBZM} ${WFLAGS}
	${WIPE} ${TBOXLIB} ${WFLAGS}
	${WIPE} ${TBOXLIBZM} ${WFLAGS}
        @echo ${DEBUGLIB} and ${TBOXLIB}: cleaned

export: export_${PHASE}

export_hdrs: dirs
        ${CP} objects.* ${EXPORTS}.*   ${CPFLAGS}
        @echo ${DEBUGLIB} and ${TBOXLIB}: export complete (hdrs)

export_libs: ${TBOXLIB} ${TBOXLIBZM} ${DEBUGLIB} ${DEBUGLIBZM} dirs
        ${CP} ${TBOXLIB}    ${EXPORTS}.o.${TBOXLIB}    ${CPFLAGS}
        ${CP} ${TBOXLIBZM}  ${EXPORTS}.o.${TBOXLIBZM}  ${CPFLAGS}
        ${CP} ${DEBUGLIB} ${EXPORTS}.o.${DEBUGLIB} ${CPFLAGS}
        ${CP} ${DEBUGLIBZM} ${EXPORTS}.o.${DEBUGLIBZM} ${CPFLAGS}
        @echo ${DEBUGLIB} and ${TBOXLIB}: export complete (libs)


dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${EXPORTS}
	${MKDIR} ${EXPORTS}.h
	${MKDIR} ${EXPORTS}.o
#
# Dynamic dependencies:
o.slist:	c.slist
o.slist:	h.slist
o.slist:	h.mem
z.mem:	c.mem
z.mem:	C:h.kernel
z.mem:	C:h.swis
z.mem:	h.debug
z.mem:	h.mem
z.messages:	c.messages
z.messages:	C:h.kernel
z.messages:	C:h.swis
z.messages:	h.const
z.messages:	h.macros
z.messages:	TBox:h.toolbox
z.messages:	h.services
z.messages:	h.debug
z.messages:	h.mem
z.messages:	h.twimp
z.messages:	C:h.kernel
z.messages:	h.style
z.messages:	h.messages
z.messages:	C:h.kernel
z.string32:	c.string32
z.string32:	h.string32
z.rmensure:	c.rmensure
z.rmensure:	C:h.kernel
z.rmensure:	C:h.swis
z.rmensure:	h.rmensure
z.rmensure:	h.macros
z.objsupport:	c.objsupport
z.objsupport:	C:h.kernel
z.objsupport:	C:h.swis
z.objsupport:	h.debug
z.objsupport:	TBox:h.toolbox
z.objsupport:	TBox:h.window
z.objsupport:	h.twimp
z.objsupport:	C:h.kernel
z.objsupport:	h.mem
z.objsupport:	h.objsupport
z.objsupport:	TBox:h.toolbox
z.objsupport:	TBox:h.window
o.mem:	c.mem
o.mem:	C:h.kernel
o.mem:	C:h.swis
o.mem:	h.debug
o.mem:	h.mem
o.messages:	c.messages
o.messages:	C:h.kernel
o.messages:	C:h.swis
o.messages:	h.const
o.messages:	h.macros
o.messages:	TBox:h.toolbox
o.messages:	h.services
o.messages:	h.debug
o.messages:	h.mem
o.messages:	h.twimp
o.messages:	C:h.kernel
o.messages:	h.style
o.messages:	h.messages
o.messages:	C:h.kernel
o.string32:	c.string32
o.string32:	h.string32
o.rmensure:	c.rmensure
o.rmensure:	C:h.kernel
o.rmensure:	C:h.swis
o.rmensure:	h.rmensure
o.rmensure:	h.macros
o.objsupport:	c.objsupport
o.objsupport:	C:h.kernel
o.objsupport:	C:h.swis
o.objsupport:	h.debug
o.objsupport:	TBox:h.toolbox
o.objsupport:	TBox:h.window
o.objsupport:	h.twimp
o.objsupport:	C:h.kernel
o.objsupport:	h.mem
o.objsupport:	h.objsupport
o.objsupport:	TBox:h.toolbox
o.objsupport:	TBox:h.window
o.debug:	c.debug
o.debug:	h.debug
o.debug:	C:h.swis
o.debug:	C:h.kernel
o.debug:	C:h.kernel
