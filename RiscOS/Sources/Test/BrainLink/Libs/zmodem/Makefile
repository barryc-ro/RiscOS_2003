# Makefile for zmodem
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
EXPORT_DIR ?= <Lib$Dir>.BrainLink.zmodem
EXP_HDR     = <export$dir>
EXP_C_H     = <Cexport$dir>.h
EXP_C_O     = <Cexport$dir>.o

#
# Generic options:
#
include Makefiles:StdTools

COMPONENT = BLzmodem
TARGET   ?= ${COMPONENT}
TARGETZ   = ${TARGET}ZM

#
# Flags
#
CINCLUDES = -Itbox:,C:,<Obey$Dir>.^
CDEFINES  = ${DFLAGS} ${OPTS}
AFLAGS   += -NoCache
CFLAGS   += -Wap -ffah -g

#
# Include files
#
DIRS     = o._dirs


# Program specific options:
#
OBJS      = o.crctab o.ftpglue o.mzmodem o.rzmodem o.szmodem o.zm
OBJSZ     = oz.crctab oz.ftpglue oz.mzmodem oz.rzmodem oz.szmodem oz.zm

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
        ${CP} h.crctab ${EXPORT_DIR}.h.crctab   ${CPFLAGS}
        ${CP} h.def ${EXPORT_DIR}.h.def   ${CPFLAGS}
        ${CP} h.ftpglue ${EXPORT_DIR}.h.ftpglue  ${CPFLAGS}
        ${CP} h.zmextra ${EXPORT_DIR}.h.zmextra   ${CPFLAGS}
        ${CP} h.zmodem ${EXPORT_DIR}.h.zmodem   ${CPFLAGS}
	@echo Obey: 
	@echo <Obey$Dir>
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
