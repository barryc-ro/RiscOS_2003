# Program specific parts

COMPONENT = InterLoad
OBJS      =	\
  o.interload

PDOBJS= oz.pdebug o.debug ${CMHGOBJ}

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
STRIP   = stripdepnd


AFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CFLAGS  = -c -wdp -ffcah -zps1 ${INCLUDES} -depend !Depend -Darm \
          -DDEBUGON=$(DBUG) ${THROWBACK}
CPCCFLAGS=-c -pcc -ffa -zps1 ${INCLUDES} -DDEBUGON=$(DBUG)


CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym


#
# Include files
#
INCLUDES = -IC:

#
# Debug flag
#
DBUG = 0
#
#
# Program specific options:
#
TARGET    = abs.$(COMPONENT)
ROMTARGET = aof.$(COMPONENT)
CMHGOBJ   = oz.modhead
#
# Rule patterns
#
.SUFFIXES:  .o .oz .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.c.oz:;      ${CC} -zm ${CFLAGS} -o $@ $<
.cmhg.oz:;   ${CMHG} -p -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@

all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} abs ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} oz ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

dirs:
	${MKDIR} o
	${MKDIR} oz

#
# Target 
#
${TARGET}: ${OBJS}
	${MKDIR} abs
        ${LD} -o $@ ${OBJS} ${CLIB}
        access $@ rw/r
#        $(MODSQZ) $(TARGET)

rm.pdebug: ${PDOBJS}
	${MKDIR} rm
        ${LD} -rmf -o $@ ${PDOBJS} ${CLIB}
        access $@ rw/r

oz.pdebug: oz.modhead

#
# ROM Target 
#
${ROMTARGET}: ${OBJS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}
        access $@ rw/r

#
# RISC OS disc build rules:
#
install: install_dirs all
	$(CP)    abs.${COMPONENT}  $(INSTDIR).${COMPONENT}  $(CPFLAGS)
	@echo $(COMPONENT): installed to $(INSTDIR)

install_dirs:
	$(MKDIR) $(INSTDIR)

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} >
        access linked.${COMPONENT} rw/r
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
