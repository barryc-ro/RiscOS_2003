# Makefile for touchscreen
# =======================
#

# -----------------------------------------------------------------------------
# Change list
#

# Date		Name	Description
# ====		====	===========


# -----------------------------------------------------------------------------
# Paths
#

LIBDIR = <Lib$Dir>



# -----------------------------------------------------------------------------
# Generic options
#
include Makefiles:StdTools

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -ff ${INCLUDES} ${BUILDFLAGS} ${THROWBACK} ${OPTIONS}
C++FLAGS  = -c -depend !Depend -ff ${INCLUDES} ${BUILDFLAGS} ${THROWBACK} ${OPTIONS}
LDFLAGS =  -c++
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr
CMHGFLAGS = -p ${OPTIONS}


# -----------------------------------------------------------------------------
# Libraries
#
include Makefiles:ModuleLibs

LIBS = ${NET5LIBS} ${C++LIB} C:o.Stubs




# -----------------------------------------------------------------------------
# Include files
#


# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT = calibration
TARGET    = calibration

OBJS = o.main o.on_screen o.graphics



# -----------------------------------------------------------------------------
# Export Paths for Messages module
#



# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .c .c++ 
.c.o:;		$(CC)  $(CFLAGS)  -o $@ $<
.c++.o:;	$(C++) $(C++FLAGS) -o $@ $<



# -----------------------------------------------------------------------------
# Build a relocatable module
#



# -----------------------------------------------------------------------------
# RISC OS ROM build rules
#

all: ${TARGET}
        @echo ${COMPONENT}: rom module built

clean:
	${RM} ${TARGET}
	${WIPE} o.* ${WFLAGS}


${TARGET}: ${OBJS} ${LIBS}
	${LD} -o $@ ${OBJS} ${LIBS} ${LDFLAGS}


# -----------------------------------------------------------------------------
# Static dependencies
#
o.main:

# -----------------------------------------------------------------------------
# ROM target (re-linked at ROM Image build time)
#



# -----------------------------------------------------------------------------
# Final link for the ROM Image (using given base address)
#



# -----------------------------------------------------------------------------
# Dynamic dependencies:
o.main: c++.main h.calibrate h.on_screen h.calibrate C:h.swis C:h.kernel
