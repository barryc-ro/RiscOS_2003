# Makefile for UILib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-10-07	BAL		Created


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
NFXLIBDIR	= <Lib$Dir>.Netfax
EXPORT_DIR	= <Lib$Dir>.Netfax.UILib


# ------------------------------------------------------------------------------
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
WIPE		= -wipe

CCFLAGS		= -c -depend !Depend -ffah ${INCLUDES} ${DFLAGS}
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~clfnr~v
LBFLAGS		= -c
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:,<Lib$Dir>.DebugLib,<Lib$Dir>.MsgLib
DFLAGS		= 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= UILib
TARGET		= ${COMPONENT}
TARGETZ		= ${COMPONENT}zm
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.animations\
 o.font\
 o.help\
 o.openwinmsg\
 o.windows\
 

OBJSZ =\
 z.animations\
 z.font\
 z.help\
 z.openwinmsg\
 z.windows\


# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .z .s .h .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zm -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${TARGET} ${TARGETZ}


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_${PHASE}

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} z ${WFLAGS}
	${RM} ${TARGET}
	${RM} ${TARGETZ}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.UILib dirs
	${CP}	h.UILib		${EXPORT_DIR}.h.UILib	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ} dirs
	${CP}	${TARGET}	${EXPORT_DIR}.o.${TARGET}	${CPFLAGS}
	${CP}	${TARGETZ}	${EXPORT_DIR}.o.${TARGETZ}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o
	${MKDIR} z

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${NFXLIBDIR}
	${MKDIR} ${EXPORT_DIR}
	${MKDIR} ${EXPORT_DIR}.h
	${MKDIR} ${EXPORT_DIR}.o
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS}

${TARGETZ}: ${OBJSZ} local_dirs
	${LB} ${LBFLAGS} -o ${TARGETZ} ${OBJSZ}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.masking:	c.masking
o.masking:	h.include
o.masking:	tbox:h.toolbox
o.masking:	C:h.kernel
o.masking:	tbox:h.wimp
o.masking:	tbox:h.window
o.masking:	tbox:h.gadgets
o.masking:	tbox:h.menu
o.masking:	tbox:h.event
o.masking:	tbox:h.wimp
o.masking:	tbox:h.wimplib
o.masking:	tbox:h.gadgets
o.masking:	tbox:h.iconbar
o.masking:	tbox:h.textarea
o.masking:	C:h.kernel
o.masking:	C:h.swis
o.masking:	<Lib$Dir>.MsgLib.h.MsgLib
o.masking:	<Lib$Dir>.DebugLib.h.DebugLib
o.masking:	h.masking
z.masking:	c.masking
z.masking:	h.include
z.masking:	tbox:h.toolbox
z.masking:	C:h.kernel
z.masking:	tbox:h.wimp
z.masking:	tbox:h.window
z.masking:	tbox:h.gadgets
z.masking:	tbox:h.menu
z.masking:	tbox:h.event
z.masking:	tbox:h.wimp
z.masking:	tbox:h.wimplib
z.masking:	tbox:h.gadgets
z.masking:	tbox:h.iconbar
z.masking:	tbox:h.textarea
z.masking:	C:h.kernel
z.masking:	C:h.swis
z.masking:	<Lib$Dir>.MsgLib.h.MsgLib
z.masking:	<Lib$Dir>.DebugLib.h.DebugLib
z.masking:	h.masking
o.animations:	c.animations
o.animations:	h.include
o.animations:	tbox:h.toolbox
o.animations:	C:h.kernel
o.animations:	tbox:h.wimp
o.animations:	tbox:h.window
o.animations:	tbox:h.gadgets
o.animations:	tbox:h.menu
o.animations:	tbox:h.event
o.animations:	tbox:h.wimp
o.animations:	tbox:h.wimplib
o.animations:	tbox:h.gadgets
o.animations:	tbox:h.iconbar
o.animations:	tbox:h.textarea
o.animations:	C:h.kernel
o.animations:	C:h.swis
o.animations:	C:msglib.h.MsgLib
o.animations:	C:debuglib.h.DebugLib
o.animations:	h.windows
o.animations:	h.animations
o.animations:	h.globals
o.font:	c.font
o.font:	h.include
o.font:	tbox:h.toolbox
o.font:	C:h.kernel
o.font:	tbox:h.wimp
o.font:	tbox:h.window
o.font:	tbox:h.gadgets
o.font:	tbox:h.menu
o.font:	tbox:h.event
o.font:	tbox:h.wimp
o.font:	tbox:h.wimplib
o.font:	tbox:h.gadgets
o.font:	tbox:h.iconbar
o.font:	tbox:h.textarea
o.font:	C:h.kernel
o.font:	C:h.swis
o.font:	C:msglib.h.MsgLib
o.font:	C:debuglib.h.DebugLib
o.font:	h.font
o.font:	h.globals
o.help:	c.help
o.help:	h.include
o.help:	tbox:h.toolbox
o.help:	C:h.kernel
o.help:	tbox:h.wimp
o.help:	tbox:h.window
o.help:	tbox:h.gadgets
o.help:	tbox:h.menu
o.help:	tbox:h.event
o.help:	tbox:h.wimp
o.help:	tbox:h.wimplib
o.help:	tbox:h.gadgets
o.help:	tbox:h.iconbar
o.help:	tbox:h.textarea
o.help:	C:h.kernel
o.help:	C:h.swis
o.help:	C:msglib.h.MsgLib
o.help:	C:debuglib.h.DebugLib
o.help:	h.help
o.openwinmsg:	c.openwinmsg
o.openwinmsg:	h.include
o.openwinmsg:	tbox:h.toolbox
o.openwinmsg:	C:h.kernel
o.openwinmsg:	tbox:h.wimp
o.openwinmsg:	tbox:h.window
o.openwinmsg:	tbox:h.gadgets
o.openwinmsg:	tbox:h.menu
o.openwinmsg:	tbox:h.event
o.openwinmsg:	tbox:h.wimp
o.openwinmsg:	tbox:h.wimplib
o.openwinmsg:	tbox:h.gadgets
o.openwinmsg:	tbox:h.iconbar
o.openwinmsg:	tbox:h.textarea
o.openwinmsg:	C:h.kernel
o.openwinmsg:	C:h.swis
o.openwinmsg:	C:msglib.h.MsgLib
o.openwinmsg:	C:debuglib.h.DebugLib
o.openwinmsg:	h.openwinmsg
o.windows:	c.windows
o.windows:	h.include
o.windows:	tbox:h.toolbox
o.windows:	C:h.kernel
o.windows:	tbox:h.wimp
o.windows:	tbox:h.window
o.windows:	tbox:h.gadgets
o.windows:	tbox:h.menu
o.windows:	tbox:h.event
o.windows:	tbox:h.wimp
o.windows:	tbox:h.wimplib
o.windows:	tbox:h.gadgets
o.windows:	tbox:h.iconbar
o.windows:	tbox:h.textarea
o.windows:	C:h.kernel
o.windows:	C:h.swis
o.windows:	C:msglib.h.MsgLib
o.windows:	C:debuglib.h.DebugLib
o.windows:	h.font
o.windows:	h.windows
o.windows:	h.globals
z.animations:	c.animations
z.animations:	h.include
z.animations:	tbox:h.toolbox
z.animations:	C:h.kernel
z.animations:	tbox:h.wimp
z.animations:	tbox:h.window
z.animations:	tbox:h.gadgets
z.animations:	tbox:h.menu
z.animations:	tbox:h.event
z.animations:	tbox:h.wimp
z.animations:	tbox:h.wimplib
z.animations:	tbox:h.gadgets
z.animations:	tbox:h.iconbar
z.animations:	tbox:h.textarea
z.animations:	C:h.kernel
z.animations:	C:h.swis
z.animations:	C:msglib.h.MsgLib
z.animations:	C:debuglib.h.DebugLib
z.animations:	h.windows
z.animations:	h.animations
z.animations:	h.globals
z.font:	c.font
z.font:	h.include
z.font:	tbox:h.toolbox
z.font:	C:h.kernel
z.font:	tbox:h.wimp
z.font:	tbox:h.window
z.font:	tbox:h.gadgets
z.font:	tbox:h.menu
z.font:	tbox:h.event
z.font:	tbox:h.wimp
z.font:	tbox:h.wimplib
z.font:	tbox:h.gadgets
z.font:	tbox:h.iconbar
z.font:	tbox:h.textarea
z.font:	C:h.kernel
z.font:	C:h.swis
z.font:	C:msglib.h.MsgLib
z.font:	C:debuglib.h.DebugLib
z.font:	h.font
z.font:	h.globals
z.help:	c.help
z.help:	h.include
z.help:	tbox:h.toolbox
z.help:	C:h.kernel
z.help:	tbox:h.wimp
z.help:	tbox:h.window
z.help:	tbox:h.gadgets
z.help:	tbox:h.menu
z.help:	tbox:h.event
z.help:	tbox:h.wimp
z.help:	tbox:h.wimplib
z.help:	tbox:h.gadgets
z.help:	tbox:h.iconbar
z.help:	tbox:h.textarea
z.help:	C:h.kernel
z.help:	C:h.swis
z.help:	C:msglib.h.MsgLib
z.help:	C:debuglib.h.DebugLib
z.help:	h.help
z.openwinmsg:	c.openwinmsg
z.openwinmsg:	h.include
z.openwinmsg:	tbox:h.toolbox
z.openwinmsg:	C:h.kernel
z.openwinmsg:	tbox:h.wimp
z.openwinmsg:	tbox:h.window
z.openwinmsg:	tbox:h.gadgets
z.openwinmsg:	tbox:h.menu
z.openwinmsg:	tbox:h.event
z.openwinmsg:	tbox:h.wimp
z.openwinmsg:	tbox:h.wimplib
z.openwinmsg:	tbox:h.gadgets
z.openwinmsg:	tbox:h.iconbar
z.openwinmsg:	tbox:h.textarea
z.openwinmsg:	C:h.kernel
z.openwinmsg:	C:h.swis
z.openwinmsg:	C:msglib.h.MsgLib
z.openwinmsg:	C:debuglib.h.DebugLib
z.openwinmsg:	h.openwinmsg
z.windows:	c.windows
z.windows:	h.include
z.windows:	tbox:h.toolbox
z.windows:	C:h.kernel
z.windows:	tbox:h.wimp
z.windows:	tbox:h.window
z.windows:	tbox:h.gadgets
z.windows:	tbox:h.menu
z.windows:	tbox:h.event
z.windows:	tbox:h.wimp
z.windows:	tbox:h.wimplib
z.windows:	tbox:h.gadgets
z.windows:	tbox:h.iconbar
z.windows:	tbox:h.textarea
z.windows:	C:h.kernel
z.windows:	C:h.swis
z.windows:	C:msglib.h.MsgLib
z.windows:	C:debuglib.h.DebugLib
z.windows:	h.font
z.windows:	h.windows
z.windows:	h.globals
