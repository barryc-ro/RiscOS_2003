# Makefile for serial
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 2001-04-03   NBingham     Created
#

#
# Paths
#
EXPORT_DIR  = <Lib$Dir>.BrainLink.serial
EXP_HDR     = <export$dir>
EXP_C_H     = <Cexport$dir>.h
EXP_C_O     = <Cexport$dir>.o

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = BLserial
TARGET   ?= ${COMPONENT}
TARGETZ   = ${TARGET}ZM

#
# Flags
#
CINCLUDES = -Itbox:,C:
CDEFINES  = ${DFLAGS} ${OPTS}
AFLAGS   += -NoCache
CFLAGS   += -Wap -ffah -g

#
# Include files
#
DIRS     = o._dirs


# Program specific options:
#
OBJS      = o.serial
OBJSZ     = oz.serial

#
# Rule patterns
#
.SUFFIXES: .z .o .c .s .oz
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -zm -zps1 -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"

#
# Build
#

${DIRS}:    
         ${MKDIR} o
         ${MKDIR} z
         ${MKDIR} oz
         ${TOUCH} $@

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} z ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${TARGETZ}
        @echo ${COMPONENT}: cleaned

export: export_${PHASE}
        @echo ${COMPONENT}: export complete

export_hdrs:
        ${MKDIR} ${EXPORT_DIR}.h
        ${CP} h.serial ${EXPORT_DIR}.h.serial   ${CPFLAGS}
        ${CP} h.ioctl ${EXPORT_DIR}.h.ioctl   ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ}
        ${MKDIR} ${EXPORT_DIR}.o
        ${CP} ${TARGET} ${EXPORT_DIR}.o.${TARGET} ${CPFLAGS}
        ${CP} ${TARGETZ} ${EXPORT_DIR}.o.${TARGETZ} ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)



# ------------------------------------------------------------------------------
# Final link
#

$(TARGET): $(OBJS) $(DIRS)
        ${AR} ${ARFLAGS} -o $@ ${OBJS}

$(TARGETZ): $(OBJSZ) $(DIRS)
        ${AR} ${ARFLAGS} -o $@ ${OBJSZ}

#
# Dynamic dependencies:
