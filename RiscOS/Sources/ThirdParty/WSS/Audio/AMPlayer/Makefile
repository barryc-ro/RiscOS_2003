# Makefile for AMPlayer
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Apr-01  DSC          Created
#

COMPONENT = AMPlayer
TARGET    = AMPlayer

#
# Generic options:
#
include StdTools


CMHGNAME  = AMPlayer
DIRS = o._dirs

#
# Libraries
#
include ModuleLibs

#
# Include files
#
INCLUDES = -Itbox:,C:,C:TCPIPLibs:,C:ThirdParty.WSS.BaseLib

# LM is Long Multiply ; FP is floating point hardware present
ASDEFS   = -PD "LM SETL {FALSE}" -PD "FP SETL {TRUE}" -PD "ATPCS32 SETL {TRUE}"
CDEFS    = -DSYNTH_ASM=1 -DFP=1 -UAPPLICATION ${OPTIONS}

ASFLAGS += -cpu ARM7M -apcs 3/32 ${ASDEFS}
CFLAGS  += -ffa -fussy -Wac -Otime -zM -zps1 ${CDEFS} ${INCLUDES} -apcs 3/32bit/fpe3/fpr

#
# Program specific options:
#
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${TARGET}
RAMTARGET = rm.${TARGET}

BASELIB   = C:ThirdParty.WSS.BaseLib.o.BaseLibZM
WSSLIB    = C:ThirdParty.WSS.WSSLib.o.WSSLibZM

OBJS       = o.main o.metadata o.common o.dctfp o.dct36fp o.output o.dct64 \
             o.event o.fstream o.layer1 o.layer2 o.layer3 o.plugin o.huffman \
             o.sound o.sound8 o.sound16 o.soundsh o.soundX o.synthfp o.amplayer \
             o.tabcalc o.miscasm o.id3v1 o.id3v2 o.fpasmfp o.vbr

#
# Rule patterns
#
.SUFFIXES:
.SUFFIXES:  .o .a .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.a.o:;      ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} -o $@ s.$*


#
# build a RAM shoutcast module
#
all: ${RAMTARGET}
        @echo ${COMPONENT}: all complete

install: ${RAMTARGET} dirs
        ${CP} ${RAMTARGET} ${INSTDIR}.${TARGET} ${CPFLAGS}
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
        ${WIPE} o ${WFLAGS}
        ${WIPE} s ${WFLAGS}
        ${WIPE} tab ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${RAMTARGET}: ${OBJS} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


o.header: cmhg.header
	cmhg -o o.header -p -d @.^.Common.h.SHOUTcast cmhg.header

${DIRS}:
	${MKDIR} o
        ${MKDIR} s
	${TOUCH} $@

#o.main:; cmhg -o o.header ${CMHGFLAGS} cmhg.${CMHGNAME} $@
#o.application:; cmhg -o o.header ${CMHGFLAGS} cmhg.${CMHGNAME} $@
   
o.main o.application: h.${CMHGNAME}

o.sound: tab.vutab

tab.vutab: bin.vutab
        ${MKDIR} tab
        /bin.vutab

# Dynamic dependencies:
