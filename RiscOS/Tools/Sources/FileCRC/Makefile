# Makefile for NCAccessManager
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 11/2/1999	DSC	Created
# 17/2/1999	DSC	Preproccess & Instrumentation targets added.
#

#
# Generic options:
#
MKDIR     = cdir
AS        = objasm
CC        = cc
CMHG      = cmhg
CP        = copy
LD        = link
RM        = remove
WIPE      = -wipe
CD        = dir
DEFMOD    = DefMod
AR        = LibFile
THROWBACK = -Throwback
STRIP     = stripdepnd


CPFLAGS   = ~cfr~v
WFLAGS    = ~c~v
CMHGFLAGS = -p -depend !Depend -throwback 
INCLUDES  = -IC: 
LDFLAGS	  = $(EXTRALDFLAGS) -c++

#
# Different compile options - choose one
#
# use PRODUCTION_TEST_BUILD if you want to build the version used for
#  production test software, along with it's compiler flags, bugs etcetera.


# Override any options by setting the OPTS in the Build options

CCFLAGS = -c ${THROWBACK} ${CCEXTRA} -depend !Depend ${INCLUDES} ${OPTS}





# Libraries
#
LIBDIR	   = <Lib$Dir>
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.c_AbsSym
SOCKLIB	   = TCPIPLibs:o.socklib5 
INETLIB	   = TCPIPLibs:o.inetlib
LIBS       = <Lib$Dir>.tboxlibs.o.wimplib
C++LIB     = <Lib$Dir>.c++lib.o.c++libz

#
# Rule patterns
#
.SUFFIXES:  .oz .s .c .c+ .i .cmhg .inst
.c.o:;      ${CC} ${CCFLAGS} -o $@ $< 
.c.i:;      $(CC) $(CFLAGS) -C -E $< >> $@
.c++.i:;    $(C++) $(C++FLAGS) -c -C -E $< >> $@
.i.inst:;   $(CC) $(CFLAGS) -C++ -c -o $@ $<
.c.oz:;     ${CC} ${CCFLAGS} -o $@ $< ${NCMAFLAGS}
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@  -Predefine "ROM_BUILD SETL {FALSE}"
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
        $(LD) $(LDFLAGS) -o targets.filecrci ${OBJSINST} inst.instlib ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${EXTRALIBS}
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
${TARGETSA}: ${OBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${EXTRALIBS}
        ${LD} $(LDFLAGS) -o $@ ${OBJS} ${CLIB} ${LIBS} ${LIBSD} ${SOCKLIB} ${INETLIB} ${RLIB} ${EXTRALIBS}




# Dynamic dependencies:
o.dbug: c.dbug H.DBUG
o.filecrc: c.filecrc C:h.kernel C:h.swis C:h.os h.CRC h.Display h.FileCRC h.DBUG h.types VersionNum
o.display: c.display C:h.os C:h.kernel h.typedefs h.display C:h.kernel C:h.swis C:h.assert h.dbug
