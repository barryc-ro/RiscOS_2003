# Makefile for BaseLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 2001-04-03   NB           Created
#

#
# Paths
#
EXPORT_DIR  = <Lib$Dir>.ThirdParty.WSS.BaseLib
EXP_HDR     = <export$dir>
EXP_C_H     = <Cexport$dir>.h
EXP_C_O     = <Cexport$dir>.o

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = BaseLib
TARGET   ?= ${COMPONENT}
TARGETZ   = ${TARGET}ZM

#
# Flags
#
CINCLUDES = -IC:TCPIPLibs
CDEFINES  = ${DFLAGS} ${OPTS}
AFLAGS   += -NoCache
CFLAGS   += -Wap -ffah

#
# Include files
#
DIRS     = o._dirs


# Program specific options:
#
OBJS      = o.debug o.error o.gethost o.heap o.memory                     	\
	    o.strext o.tcpbsd o.thread o.time o.blocked o.blockedCS       	\
	    o.callback o.caller o.gethost_ o.nsetjmp o.stack
OBJSZ     = oz.debug oz.error oz.gethost oz.heap oz.memory                  	\
	    oz.strext oz.tcpbsd oz.thread oz.time oz.blocked 	                \
	    oz.blockedCS oz.callback oz.caller oz.gethost_ oz.nsetjmp oz.stack

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
        ${CP} h.blocked ${EXPORT_DIR}.h.blocked   ${CPFLAGS}
        ${CP} h.callback ${EXPORT_DIR}.h.callback   ${CPFLAGS}
        ${CP} h.caller ${EXPORT_DIR}.h.caller   ${CPFLAGS}
        ${CP} h.debug ${EXPORT_DIR}.h.debug   ${CPFLAGS}
        ${CP} h.error ${EXPORT_DIR}.h.error   ${CPFLAGS}
        ${CP} h.errorxxx ${EXPORT_DIR}.h.errorxxx   ${CPFLAGS}
        ${CP} h.memory ${EXPORT_DIR}.h.memory   ${CPFLAGS}
        ${CP} h.nsetjmp ${EXPORT_DIR}.h.nsetjmp   ${CPFLAGS}
        ${CP} h.strext ${EXPORT_DIR}.h.strext   ${CPFLAGS}
        ${CP} h.tcp ${EXPORT_DIR}.h.tcp   ${CPFLAGS}
        ${CP} h.thread ${EXPORT_DIR}.h.thread   ${CPFLAGS}
        ${CP} h.time ${EXPORT_DIR}.h.time   ${CPFLAGS}
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
