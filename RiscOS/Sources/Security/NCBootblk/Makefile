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


ASFLAGS = -depend !Depend -Stamp -quit -module

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
INCLUDES= -IC: 

#
# Different compile options - choose one
#

# Tiny stubs, no debugging
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -DUSE_TINY -UENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS -UMANAGE_FRESCO_DATA

# Tiny stubs, debugging (may be a bit dodgy)
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -DUSE_TINY -DENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS -UMANAGE_FRESCO_DATA

# Shared C stubs, no debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -UUSE_TINY -UENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS -UMANAGE_FRESCO_DATA
 
# Shared C stubs, debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -UUSE_TINY -DENABLE_DEBUGGING -DINSECURE_SMARTCARD_OPS -UMANAGE_FRESCO_DATA



# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym

#SOCKLIB	   = InetInc:^.^.libs.socklib.socklibzm
#INETLIB	   = InetInc:^.^.libs.inetlib.inetlibzm

#
# Rule patterns
#
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@

#
# Program specific options:
#
COMPONENT = NCBootBlk
TARGET    = aof.NCBootBlk
TARGETSA  = rm.NCBootblk
EXPORTS   = ${EXP_C_H}.smcard
OBJS      = module.o header.o reg_swis.o reg_events.o enum_ddi.o \
	reg_scbb.o reg_cmds.o reg_status.o reg_utils.o \
	machine.o DBUG.o dynalloc.o reg_auth.o reg_index.o \
	asm_utils.o reg_files.o\
	BORDERDBUG.o kprintf.o \
	reg_sign.o reg_tags.o

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
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aof.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

${TARGETSA}: ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -module ${OBJS} ${CLIB} ${SOCKLIB} ${INETLIB}

#
# Exported interface headers
#
${EXP_C_H}.smcard:	h.smcard
	${CP} h.smcard $@ ${CPFLAGS}

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS} ${SOCKLIB} ${INETLIB}


#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete



# Dynamic dependencies:
