# Makefile for NCBootblock
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 7-Apr-97	AJS	Created
#

#
# Paths
#
EXP_HDR = <export$dir>
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
DEFMOD  = DefMod
AR	= LibFile
TOUCH   = create

ASFLAGS = -depend !Depend -Stamp -quit -module

THROWBACK = -throwback

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: 

#
# Different compile options - choose one

# Tiny stubs, no debugging
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -DUSE_TINY -UREMOTE_DEBUG -UENABLE_DEBUGGING \
	-DSTAT_SMARTCARD -UOLD_STYLE_STAT
		

# Tiny stubs, debugging (may be a bit dodgy)
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -DUSE_TINY -UREMOTE_DEBUG -DREMOTE_DEBUG -DENABLE_DEBUGGING \
	-DSTAT_SMARTCARD -UOLD_STYLE_STAT
		

# Shared C stubs, no debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -UUSE_TINY -UREMOTE_DEBUG -UENABLE_DEBUGGING \
	-DSTAT_SMARTCARD -UOLD_STYLE_STAT
		
# Shared C stubs, debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM ${THROWBACK} -UUSE_TINY -UREMOTE_DEBUG -DENABLE_DEBUGGING \
	-DSTAT_SMARTCARD -UOLD_STYLE_STAT
	


# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
#DBUGLIB    = C:DBUG.o.DBUG

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -p -o $@ $<
.cmhg.h:;   ${CMHG} -p -d $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@

#
# Program specific options:
#
COMPONENT = NCBBTwin
TARGET    = aof.NCBBTwin
TARGETSA  = rm.NCBBTwin
EXPORTS   = ${EXP_C_H}.smcardCCIJ
OBJS      = module.o header.o reg_swis.o reg_events.o  \
	reg_scbb.o reg_cmds.o reg_status.o reg_utils.o \
	machine.o dynalloc.o reg_auth.o \
	asm_utils.o \
	kprintf.o
	

#
# Generic rules:
#
all: ${TARGETSA}
        @echo ${COMPONENT}: all complete

rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.header
        @echo ${COMPONENT}: cleaned

localdirs.o:
	$(MKDIR) o
	$(MKDIR) aof
	$(MKDIR) rm
	${TOUCH} $@

#
# Executable target (static link)
#

${TARGETSA}: ${OBJS} ${CLIB} ${DBUGLIB} localdirs.o header.h
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${DBUGLIB}

#
# Exported interface headers
#
${EXP_C_H}.smcardCCIJ:	h.smcardCCIJ
	${CP} h.smcardCCIJ $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} localdirs.o header.h
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} 


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
