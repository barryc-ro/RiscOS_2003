# Makefile for JapanIME
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Oct-98  KJB          Created
# 08-Oct-98  KJB          Added Squeeze and Messages rules

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
AR	= libfile
TOUCH   = create
MODSQZ  = modsqz

# defines for CHINA, TAIWAN, KOREA, TANKANJI_CNV, TANBUN, RENBUN1 and DEBUG added because
# VJE code does, for example, #IF CHINA == 1 which swamps the throwback with warnings
OPTIONS = -Dunix=1 -DSTRCT_CN=1 -DPROTO=1 -DROMDIC=1 -DCHINA=0 -DTAIWAN=0 -DKOREA=0 \
          -DTANKANJI_CNV=0 -DTANBUN=0 -DRENBUN1=0 -DDEBUG=0 -DTHIN_CSR=1

CFLAGS  = -c -depend !Depend ${THROWBACK} -zM -ffa -ec -Wps ${INCLUDES} ${OPTIONS} ${OPTS}
CMHGFLAGS = -p
CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
ULIB       = C:Unicode.o.ucodelibz

#
# Include files
#
INCLUDES = -Ivjesrc,^.IntIME,C:,unixlibs,C:Unicode

# Program specific options:
#
COMPONENT = JapanIME
TARGET    = aof.${COMPONENT}

OBJS =  o.CModuleHdr o.JapanIME o.vmjapanlbz 

VJEOBJS = \
        vjesrc.o.wmain vjesrc.o.wtango vjesrc.o.waddchr \
        vjesrc.o.wmenu vjesrc.o.wcnvbuf\
        vjesrc.o.wsetbuf vjesrc.o.wsetsub vjesrc.o.wsub \
        vjesrc.o.wstring vjesrc.o.wstrtb\
        vjesrc.o.wcode vjesrc.o.wcodesub \
        vjesrc.o.henkan vjesrc.o.henkan1 vjesrc.o.henkan2 \
        vjesrc.o.henkan3 vjesrc.o.henkan4 vjesrc.o.henkan5\
        vjesrc.o.bunpou51 vjesrc.o.bunpou52 vjesrc.o.bunpou53 \
        vjesrc.o.cdlnk vjesrc.o.cnv5 vjesrc.o.cnv5tb\
        vjesrc.o.gakushu7 vjesrc.o.kansuuji vjesrc.o.libsub \
        vjesrc.o.opendic vjesrc.o.stg vjesrc.o.strsub\
        vjesrc.o.vjetb vjesrc.o.vjetb1 vjesrc.o.vjetb2 \
        vjesrc.o.vjetb3 vjesrc.o.vjetb4 vjesrc.o.ysrcdic5 \
        vjesrc.o.ysrcsram vjesrc.o.ywrtdic vjesrc.o.tankan

# of the above: cnv5tb, kansuuji, vjetb, vjetb1, vjetb2, vjetb3, vjetb4 and wstrtb
# do not seem to be linked in to the module

#
# Rule patterns
#

.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;   ${CMHG} ${CMHGFLAGS} -d $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.${COMPONENT} o.dirs
        @echo ${COMPONENT}: complete

#
# RISC OS ROM build rules:
#
rom: ${TARGET} o.dirs
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS} o.dirs
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET} o.dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages    ${RESDIR}.${COMPONENT}.Messages   ${CPFLAGS}
        ${CP} Resources.Dictionary ${RESDIR}.${COMPONENT}.Dictionary ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} vjesrc.o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${RM} h.CModuleHdr
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

o.dirs:
	${MKDIR} o
	${MKDIR} vjesrc.o
        ${TOUCH} o.dirs
	
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ULIB} ${ROMCSTUBS}
	${MKDIR} o
	${MKDIR} aof
        ${LD} -o $@ -aof ${OBJS} ${ULIB} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Target
#
rm.${COMPONENT}u: ${OBJS} ${ULIB} ${CLIB}
	${MKDIR} rm
        ${LD} -rmf -o $@ ${OBJS} ${ULIB} ${CLIB}

rm.${COMPONENT}: rm.${COMPONENT}u
        ${MODSQZ} rm.${COMPONENT}u $@

#
# Special rules
#

o.vmjapanlbz: ${VJEOBJS}
	${AR} -c -o o.vmjapanlbz ${VJEOBJS}

o.JapanIME: h.CModuleHdr
        
#
# Dynamic dependencies:
