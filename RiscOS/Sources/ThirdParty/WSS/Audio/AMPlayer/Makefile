# Makefile for AMPlayer
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 09-Apr-01  DSC          Created
# 02-May-01  SNB          Introduced the variant builds
#

COMPONENT ?= WSS_AMPlayer
TARGET    ?= AMPlayer

# This component can build six different variants:
#   StrongARM =>  TARGET = AMPlayer    => Long Multiply; no Floating Point; APCS-32
#   ARM 6/7   =>  TARGET = AMPlayer6   => no Long Multiply; no Floating Point; APCS-32
#   7500FE    =>  TARGET = AMPlayerFP  => no Long Multiply; Floating Point; APCS-32
#   StrongARM =>  TARGET = AMPlay26    => Long Multiply; no Floating Point; APCS-R
#   ARM 6/7   =>  TARGET = AMPlay266   => no Long Multiply; no Floating Point; APCS-R
#   7500FE    =>  TARGET = AMPlay26FP  => no Long Multiply; Floating Point; APCS-R

#
# Generic options that are common to all assemblies/compilations
#
CINCLUDES = -Itbox:,C:,C:TCPIPLibs:,C:ThirdParty.WSS.BaseLib
CFLAGS    = -ffa -fussy -Wac -Otime -zM -zps1 -UAPPLICATION ${OPTIONS}
ASFLAGS   = -cpu ARM7M -PD "ATPCS32 SETL {CONFIG}<>26" ${ASOPTIONS}

#
# Generic options:
#
include StdTools
include ModuleLibs

ASFLAGS_LM_noFP   = -PD "LM SETL {TRUE}"  -PD "FP SETL {FALSE}"
ASFLAGS_noLM_noFP = -PD "LM SETL {FALSE}" -PD "FP SETL {FALSE}"
ASFLAGS_noLM_FP   = -PD "LM SETL {FALSE}" -PD "FP SETL {TRUE}"

CFLAGS_LM_noFP    = -DSYNTH_ASM=1 -DFP=0
CFLAGS_noLM_noFP  = -DSYNTH_ASM=0 -DFP=0
CFLAGS_noLM_FP    = -DSYNTH_ASM=1 -DFP=1

CMHGNAME          = AMPlayer
DIRS              = o._dirs

#
# Program specific options:
#
CMHGOBJ   = ${CMHGNAME}.o
ROMTARGET = aof.${TARGET}
RAMTARGET = rm.${TARGET}

BASELIB   = C:ThirdParty.WSS.BaseLib.o.BaseLibZM
WSSLIB    = C:ThirdParty.WSS.WSSLib.o.WSSLibZM

#
# Rule patterns
#
.SUFFIXES:
.SUFFIXES:  .o .o_6 .o_fp .o26 .o26_6 .o26_fp .s .a .c .h .cmhg

.c.o:;      ${CC} ${CFLAGS} ${CFLAGS_LM_noFP}   -o $@ $< -apcs 3/32/fpe3/fpr
.c.o_6:;    ${CC} ${CFLAGS} ${CFLAGS_noLM_noFP} -o $@ $< -apcs 3/32/fpe3/fpr
.c.o_fp:;   ${CC} ${CFLAGS} ${CFLAGS_noLM_FP}   -o $@ $< -apcs 3/32/fpe3/fpr
.c.o26:;    ${CC} ${CFLAGS} ${CFLAGS_LM_noFP}   -o $@ $< -apcs 3/26/fpe2/fpr
.c.o26_6:;  ${CC} ${CFLAGS} ${CFLAGS_noLM_noFP} -o $@ $< -apcs 3/26/fpe2/fpr
.c.o26_fp:; ${CC} ${CFLAGS} ${CFLAGS_noLM_FP}   -o $@ $< -apcs 3/26/fpe2/fpr
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< -32bit   -DATPCS=1
.cmhg.o_6:; ${CMHG} ${CMHGFLAGS} -o $@ $< -32bit   -DATPCS=1
.cmhg.o_fp:;${CMHG} ${CMHGFLAGS} -o $@ $< -32bit   -DATPCS=1
.cmhg.o26:;   ${CMHG} ${CMHGFLAGS} -o $@ $< -26bit -DATPCS=0
.cmhg.o26_6:; ${CMHG} ${CMHGFLAGS} -o $@ $< -26bit -DATPCS=0
.cmhg.o26_fp:;${CMHG} ${CMHGFLAGS} -o $@ $< -26bit -DATPCS=0
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.a.o:;      ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} ${ASFLAGS_LM_noFP} -o $@ s.$*
.a.o_6:;    ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} ${ASFLAGS_noLM_noFP} -o $@ s.$*
.a.o_fp:;   ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} ${ASFLAGS_noLM_FP} -o $@ s.$*
.a.o26:;    ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} ${ASFLAGS_LM_noFP} -o $@ s.$*
.a.o26_6:;  ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} ${ASFLAGS_noLM_noFP} -o $@ s.$*
.a.o26_fp:;  ${CC} -E >s.$* $<
            /bin.asclean s.$*
            ${AS} ${ASFLAGS} ${ASFLAGS_noLM_FP} -o $@ s.$*

.s.o:; ${AS} ${ASFLAGS} ${ASFLAGS_LM_noFP} -o $@ s.$*
.s.o_6:; ${AS} ${ASFLAGS} ${ASFLAGS_noLM_noFP} -o $@ s.$*
.s.o_fp:; ${AS} ${ASFLAGS} ${ASFLAGS_noLM_FP} -o $@ s.$*
.s.o26:; ${AS} ${ASFLAGS} ${ASFLAGS_LM_noFP} -o $@ s.$*
.s.o26_6:; ${AS} ${ASFLAGS} ${ASFLAGS_noLM_noFP} -o $@ s.$*
.s.o26_fp:; ${AS} ${ASFLAGS} ${ASFLAGS_noLM_FP} -o $@ s.$*

OBJSFP    = \
 o_fp.main o.metadata o.common o_fp.output o_fp.dct64 o_fp.dctfp \
 o_fp.dct36fp o.event o.fstream o.layer1 o.layer2 o_fp.layer3 o.plugin o.huffman \
 o.sound o.sound8 o.sound16 o_6.soundsh o.soundX o_fp.synthfp o.${CMHGNAME} \
 o.tabcalc o.miscasm o.id3v1 o.id3v2 o.fpasmfp o.vbr

OBJS6     = \
 o.main o.metadata o.common o_6.output o_6.dct64 \
 o.event o.fstream o.layer1 o.layer2 o_6.layer3 o.plugin o.huffman \
 o.sound o.sound8 o.sound16 o_6.soundsh o.soundX ${CMHGOBJ} \
 o.tabcalc o.miscasm o.id3v1 o.id3v2 o_6.fpasm6 o.vbr

OBJS      = \
 o.main o.metadata o.common o.dct o.output o.dct64 o.dct36 \
 o.event o.fstream o.layer1 o.layer2 o.layer3 o.plugin o.huffman \
 o.sound o.sound8 o.sound16 o.soundsh o.soundX ${CMHGOBJ} \
 o.tabcalc o.miscasm o.id3v1 o.id3v2 o.fpasm o.synthasm o.vbr

OBJSFP26  = \
 o26_fp.main o26.metadata o26.common o26_fp.output o26_fp.dct64 o26_fp.dctfp \
 o26_fp.dct36fp o26.event o26.fstream o26.layer1 o26.layer2 o26_fp.layer3 o26.plugin o26.huffman \
 o26.sound o26.sound8 o26.sound16 o26_6.soundsh o26.soundX o26_fp.synthfp o26_fp.${CMHGNAME} \
 o26.tabcalc o26.miscasm o26.id3v1 o26.id3v2 o26.fpasmfp o26.vbr

OBJS626   = \
 o26.main o26.metadata o26.common o26_6.output o26_6.dct64 \
 o26.event o26.fstream o26.layer1 o26.layer2 o26_6.layer3 o26.plugin o26.huffman \
 o26.sound o26.sound8 o26.sound16 o26_6.soundsh o26.soundX o26_6.${CMHGNAME} \
 o26.tabcalc o26.miscasm o26.id3v1 o26.id3v2 o26_6.fpasm6 o26.vbr

OBJS26    = \
 o26.main o26.metadata o26.common o26.dct o26.output o26.dct64 o26.dct36 \
 o26.event o26.fstream o26.layer1 o26.layer2 o26.layer3 o26.plugin o26.huffman \
 o26.sound o26.sound8 o26.sound16 o26.soundsh o26.soundX o26.${CMHGNAME} \
 o26.tabcalc o26.miscasm o26.id3v1 o26.id3v2 o26.fpasm o26.synthasm o26.vbr

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
        ${WIPE} o_6 ${WFLAGS}
        ${WIPE} o_fp ${WFLAGS}
        ${WIPE} o26 ${WFLAGS}
        ${WIPE} o26_6 ${WFLAGS}
        ${WIPE} o26_fp ${WFLAGS}
        ${WIPE} s ${WFLAGS}
        ${WIPE} tab ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Targets
#
rm.AMPlayer: ${OBJS} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

rm.AMPlayer6: ${OBJS6} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS6} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

rm.AMPlayerFP: ${OBJSFP} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJSFP} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

rm.AMPlay26: ${OBJS26} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS26} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

rm.AMPlay266: ${OBJS626} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJS626} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

rm.AMPlay26FP: ${OBJSFP26} ${BASELIB} ${NET5LIBS} ${CLIB} ${DIRS}
        ${MKDIR} rm
        ${LD} -o $@ -c++ -rmf ${OBJSFP26} ${BASELIB} ${NET5LIBS} ${CLIB}
        $(MODSQZ) $@
        ${CHMOD} a+rx $@

#
# ROM Targets
#
aof.AMPlayer: ${OBJS} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

aof.AMPlayer6: ${OBJS6} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS6} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

aof.AMPlayerFP: ${OBJSFP} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJSFP} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

aof.AMPlay26: ${OBJS26} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS26} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

aof.AMPlay266: ${OBJS626} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS626} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

aof.AMPlay26FP: ${OBJSFP26} ${BASELIB} ${WSSLIB} ${NET5LIBS} ${ROMCSTUBS} ${DIRS}
        ${MKDIR} aof
        ${LD} -o $@ -aof ${OBJSFP26} ${WSSLIB} ${BASELIB} ${NET5LIBS} ${ROMCSTUBS}

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
        ${MKDIR} o_6
        ${MKDIR} o_fp
        ${MKDIR} o26
        ${MKDIR} o26_6
        ${MKDIR} o26_fp
        ${MKDIR} s
	${TOUCH} $@

o.main o.application: h.${CMHGNAME}
o.sound: tab.vutab
o_6.main o_6.application: h.${CMHGNAME}
o_6.sound: tab.vutab
o_fp.main o_fp.application: h.${CMHGNAME}
o_fp.sound: tab.vutab
o26.main o26.application: h.${CMHGNAME}
o26.sound: tab.vutab
o26_6.main o26_6.application: h.${CMHGNAME}
o26_6.sound: tab.vutab
o26_fp.main o26_fp.application: h.${CMHGNAME}
o26_fp.sound: tab.vutab

tab.vutab: bin.vutab
        ${MKDIR} tab
        /bin.vutab

# Dynamic dependencies:
