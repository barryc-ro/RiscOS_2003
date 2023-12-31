# Makefile for callx
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-11-26	BAL		Created


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>.${COMPONENT}


# ------------------------------------------------------------------------------
# Generic options
#

include Makefiles:StdTools

CCFLAGS		= -c -zm -zps1 -Wp -depend !Depend -ffah ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS		= -depend !Depend -Stamp -quit
LBFLAGS		= -c


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:
DFLAGS		= 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= callx
TARGET		= ${COMPONENT}
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.callx\
 o.veneer\
 

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .h .c
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	${TARGET}


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_${PHASE}

clean:
	${WIPE} o.* ${WFLAGS}
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.callx dirs
	${CP}	h.callx		${LIBDIR}.h.callx	${CPFLAGS}
	${MKDIR} ${LIBDIR}.Docs
	${CP}	!ReadMe		${LIBDIR}.Docs.callx	${CPFLAGS}
	${CP}	VersionNum	${LIBDIR}.LibVersion	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} dirs
	${CP}	${TARGET}	${LIBDIR}.o.${TARGET}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o

dirs:
	${MKDIR} ${LIBDIR}.Docs
	${MKDIR} ${LIBDIR}.h
	${MKDIR} ${LIBDIR}.o
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	${AR} ${ARFLAGS} -o ${TARGET} ${OBJS}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
