# Makefile for NCMAlib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 15 Feb 2001  DSC          Created
#

#
# Paths
#
EXPORT_DIR  = <Lib$Dir>.NCMALib
EXP_HDR     = <export$dir>
EXP_C_H     = <Cexport$dir>.h
EXP_C_O     = <Cexport$dir>.o

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = NCMALib
TARGET   ?= ${COMPONENT}
TARGETZ   = ${TARGET}ZM

#
# Flags
#
CINCLUDES = -IC:TCPIPLibs
CDEFINES  = ${DFLAGS} ${OPTS}
CFLAGS   += -Wap -ffah

#
# Include files
#
DIRS     = o._dirs


# Program specific options:
#
OBJS      = o.dynalloc o.kprintf o.ncma_cache o.ncma_tags o.ncma_utils o.ncma_bootb o.dbugzm o.ncma_exec o.ncma_fx
OBJSZ     = oz.dynalloc oz.kprintf oz.ncma_cache oz.ncma_tags oz.ncma_utils oz.ncma_bootb oz.dbugzm oz.ncma_exec oz.ncma_fx

#
# Rule patterns
#
.SUFFIXES: .z .o .c .s .oz
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;     ${CC} ${CFLAGS} -zm -zps1 -o $@ $<

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
        ${CP} h.ncma_bootb ${EXPORT_DIR}.h.ncma_bootb   ${CPFLAGS}
        ${CP} h.ncma_tags  ${EXPORT_DIR}.h.ncma_tags    ${CPFLAGS}
        ${CP} h.ncma_utils ${EXPORT_DIR}.h.ncma_utils   ${CPFLAGS}
        ${CP} h.ncma_scard ${EXPORT_DIR}.h.ncma_scard   ${CPFLAGS}
        ${CP} h.ncma_exec  ${EXPORT_DIR}.h.ncma_exec    ${CPFLAGS}
        ${CP} h.ncma_fx    ${EXPORT_DIR}.h.ncma_fx      ${CPFLAGS}
        ${CP} h.dynalloc   ${EXPORT_DIR}.h.dynalloc     ${CPFLAGS}
        ${CP} h.smtags     ${EXP_C_H}.smtags            ${CPFLAGS}
        ${CP} h.smcard     ${EXP_C_H}.smcard            ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} ${TARGETZ}
        ${MKDIR} ${EXPORT_DIR}.o
        ${CP} ${TARGET} ${EXPORT_DIR}.o.${TARGET} ${CPFLAGS}
        ${CP} ${TARGETZ} ${EXPORT_DIR}.o.${TARGETZ} ${CPFLAGS}
        ${CP} VersionNum ${EXPORT_DIR}.LibVersion ${CPFLAGS}
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
