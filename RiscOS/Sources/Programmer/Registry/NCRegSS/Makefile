# Makefile for NCRegistry Script Support
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# When		Who	What
# ----		---	----
# 9-Aug-96	AJS	Created
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
DATA2AOF = datatoaof 

ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
DATA2AOF_FLAGS = -L _length -S -P -DP 

INCLUDES= -IC: 

#
# Different compile options - choose one
#

# Tiny stubs, no debugging
CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DUSE_TINY -UENABLE_DEBUGGING -DRISCOS

# Tiny stubs, debugging (may be a bit dodgy)
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -DUSE_TINY -DENABLE_DEBUGGING -DRISCOS

# Shared C stubs, no debugging
#CFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -UUSE_TINY -UENABLE_DEBUGGING -DRISCOS

# Shared C stubs, debugging
#CCFLAGS  = -c -depend !Depend ${INCLUDES} -zM -Throwback -UUSE_TINY -DENABLE_DEBUGGING -DRISCOS

#
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
.SUFFIXES:	.o .c .cmhg .s .x
.c.o:;      ${CC} ${CCFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${ASFLAGS} $< $@
.x.o:;	    $(DATA2AOF) -f $< -o $@ $(DATA2AOF_FLAGS)

#
# Program specific options:
#
COMPONENT = NCRegSS
TARGET    = aof.NCRegSS
TARGETSA  = rm.NCRegSS
EXPORTS   = ${EXP_C_H}.regss_defs
OBJS      = module.o header.o regss_swis.o dynalloc.o regss_scrp.o \
	LZW.o DBUG.o ncinfox.o ncinfopx.o ncinfotx.o regss_util.o

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
${EXP_C_H}.regss_defs:	h.regss_defs
	${CP} h.regss_defs $@ ${CPFLAGS}


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
o.module:	c.module
o.module:	C:h.kernel
o.module:	C:h.swis
o.module:	C:h.s_card
o.module:	C:h.sc_const
o.module:	h.ispc
o.module:	h.regss_scrp
o.module:	h.module
o.module:	h.regss_swis
o.module:	h.dynalloc
o.module:	h.regss_defs
o.module:	h.LZW
o.module:	h.DBUG
o.regss_swis:	c.regss_swis
o.regss_swis:	C:h.swis
o.regss_swis:	C:h.kernel
o.regss_swis:	C:h.kernel
o.regss_swis:	h.ispc
o.regss_swis:	C:h.s_card
o.regss_swis:	C:h.sc_const
o.regss_swis:	C:h.reg_scbb
o.regss_swis:	h.regss_scrp
o.regss_swis:	h.regss_defs
o.regss_swis:	h.module
o.regss_swis:	h.regss_swis
o.regss_swis:	h.LZW
o.regss_swis:	h.dynalloc
o.regss_swis:	C:h.scinter
o.regss_swis:	h.DBUG
o.dynalloc:	c.dynalloc
o.dynalloc:	C:h.kernel
o.dynalloc:	C:h.swis
o.dynalloc:	h.dynalloc
o.regss_scrp:	c.regss_scrp
o.regss_scrp:	C:h.kernel
o.regss_scrp:	C:h.swis
o.regss_scrp:	h.ispc
o.regss_scrp:	C:h.s_card
o.regss_scrp:	C:h.sc_const
o.regss_scrp:	h.regss_scrp
o.regss_scrp:	C:h.reg_scbb
o.regss_scrp:	h.module
o.regss_scrp:	h.DBUG
o.LZW:	c.LZW
o.LZW:	C:h.kernel
o.LZW:	C:h.swis
o.LZW:	C:h.s_card
o.LZW:	C:h.sc_const
o.LZW:	h.regss_scrp
o.LZW:	h.module
o.LZW:	h.LZW
o.LZW:	h.DBUG
o.DBUG:	c.DBUG
o.DBUG:	H.DBUG
o.regss_util: s.regss_util
o.regss_util: s.registers
