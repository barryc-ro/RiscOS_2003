# Makefile for Touchscreen driver
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-00  SNB          Created
#

#
# Paths
#
EXP_HDR = <cexport$dir>

#
# Generic options:
#

COMPONENT = TouchScreen
CMHGNAME  = header


DEFINES = ${COMPAT} ${OPTIONS}

CFLAGS    = -ffca -c -zM -zps1 -wp ${DEFINES} ${INCLUDES}
C++FLAGS    = -ffca -c -zM -zps1 -wp ${DEFINES} ${INCLUDES}
CMHGFLAGS = -p ${DEPENDS}

include Makefiles:StdTools

DIRS = o._dirs
#
# Libraries
#
include Makefiles:ModuleLibs

RAMLIBS = ${DEBUGLIB} ${NET5LIBS} ${C++LIB} ${CLIB} ${ASMUTILS}
ROMLIBS = ${DEBUGLIB} ${NET5LIBS} ${C++LIB} ${ROMCSTUBS} ${ASMUTILS}


#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

local_dirs:
	$(MKDIR) i
	$(MKDIR) inst
	$(MKDIR) io
	

OBJS      =\
 o.driver\
 o.sensor\
 o.touchscr\
 $(CMHGOBJ)
 
 OBJSI =\
  i.driver\
  i.sensor\
  i.touchscr
  
 OBJSIO =\
  io.driver\
  io.sensor\
  io.touchscr\
  io.$(CMHGNAME)\
  io.inst
 

#
# Rule patterns
#
.SUFFIXES:  .o .s .c .c++ .h .i .cmhg .inst .io

# INFERENCE RULES
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c++.o:;    ${C++} ${C++FLAGS} -o $@ $<
.c++.i:;    ${C++} ${C++FLAGS} -C++ -E $< >> $@
.c.i:;      ${CC} ${CFLAGS} -c -C -E $< >> $@
.inst.io:;  ${C++} ${C++FLAGS} -C++ -W -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.io:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@



#
# build the module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: all complete

dirs:
	${MKDIR} ${INSTDIR}
#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} i.*	${WFLAGS}
        ${WIPE} io.*    ${WFLAGS}
        ${RM} i
        ${RM} io
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned
        
  
preprocess: $(OBJSI) local_dirs
	@echo $(COMPONENT): preprocess build complete
	

instrument: $(OBJSIO) local_dirs 
        ${LD} -c++ -o $@ -rmf ${OBJSIO} ${RAMLIBS}
	@echo $(COMPONENT): instrument build complete
	
     

#
# Target 
#
${TARGET}: ${OBJS} ${RAMLIBS} ${DIRS}
	${MKDIR} rm
        ${LD} -c++ -o $@ -rmf ${OBJS} ${RAMLIBS}
        $(MODSQZ) $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${ROMLIBS}
	${MKDIR} aof
        ${LD} -c++ -o $@ -aof ${OBJS} ${ROMLIBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -c++ -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

o.driver: VersionNum h.${CMHGNAME}

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

# Dynamic dependencies:
