# Makefile for NCAccessManager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When          Who     What
# ----          ---     ----
# 11/2/1999     DSC     Created
# 17/2/1999     DSC     Preproccess & Instrumentation targets added.
#

#
# Generic options:
#

LDFLAGS = $(EXTRALDFLAGS) -c++

#
# Different compile options - choose one
#
# use PRODUCTION_TEST_BUILD if you want to build the version used for
#  production test software, along with it's compiler flags, bugs etcetera.


# Override any options by setting the OPTS in the Build options

CDEFINES = -DRISCOS ${CCEXTRA} ${OPTS}
include Makefiles:StdTools
include Makefiles:AppStdRule


# Libraries
#
include Makefiles:AppLibs

#
# Rule patterns
#
.SUFFIXES:  .oz .s .c .c+ .i .cmhg .inst
.c.i:;      $(CC) $(CFLAGS) -C -E $< >> $@
.c++.i:;    $(C++) $(C++FLAGS) -c -C -E $< >> $@
.i.inst:;   $(CC) $(CFLAGS) -C++ -c -o $@ $<
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.inst:;   ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
.s.oz:;     ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {TRUE}"

#
# Program specific options:
#
COMPONENT = FileCRC
TARGETSA  = Targets.FileCRC
OBJS      = dbug.o filecrc.o display.o crc.o
OBJSI     = i.dbug i.filecrc i.display
OBJSINST  = inst.display inst.filecrc inst.dbug inst.crc

LIBSD     = ${DEBUGLIB} ${NET5LIBS}

#
# Component build rules
#

# o.resources: s.resources ${NOTROMRESDEP}

#
# Generic rules:
#
all: ${TARGETSA} local_dirs
        @echo ${COMPONENT}: all complete

preprocess: ${OBJSI} local_dirs
        @echo ${COMPONENT}: preprocess build complete

instrument: ${OBJSINST} inst.instlib
        $(LD) $(LDFLAGS) -o targets.filecrci ${OBJSINST} inst.instlib ${CLIB} ${LIBS} ${LIBSD} ${RLIB} ${EXTRALIBS}
        @echo ${COMPONENT}: instrument build complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} i.* ${WFLAGS}
        ${WIPE} targets.* ${WFLAGS}
        ${WIPE} inst.* ${WFLAGS}
        ${RM} o
        ${RM} i
        ${RM} targets
        ${RM} linked
        ${RM} inst
        ${STRIP}
        @echo ${COMPONENT}: cleaned

local_dirs:
        ${MKDIR} o
        ${MKDIR} i
        ${MKDIR} targets
        ${MKDIR} inst

#
# Executable target (static link)
#
${TARGETSA}: ${OBJS} ${CLIB} ${LIBS} ${LIBSD} ${RLIB} ${EXTRALIBS}
        ${LD} $(LDFLAGS) -o $@ ${OBJS} ${CLIB} ${LIBS} ${LIBSD} ${RLIB} ${EXTRALIBS}

# Dynamic dependencies:
