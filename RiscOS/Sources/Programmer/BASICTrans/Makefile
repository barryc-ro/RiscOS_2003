# Makefile for BASICTrans
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 08-Jun-94  AMcC   Created.
# 08-Nov-94  AMcC   Updated to install BASIC64 (non ROM)

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = BASICTrans
EXPORTS   = ${EXP_HDR}.BASICTrans
TARGETS   = rm.BASICTrans

#
# Generic rules:
#
all: ${TARGETS}
        @echo ${COMPONENT}: module built

rom: ${TARGETS}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGETS}
        ${CP} rm.BASICTrans ${INSTDIR}.BASICTrans ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} rm.BASICTrans
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.BASIC
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.BASIC.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

rm.BASICTrans: s.TransSrc
        ${AS} ${ASFLAGS} s.TransSrc

${EXP_HDR}.BASICTrans: hdr.BASICTrans
        ${CP} hdr.BASICTrans $@ ${CPFLAGS}

# Dynamic dependencies:
rm.BASICTrans: Hdr:ListOpts
rm.BASICTrans: Hdr:Macros
rm.BASICTrans: Hdr:System
rm.BASICTrans: Hdr:SWIs
rm.BASICTrans: Hdr:CPU.Generic26
rm.BASICTrans: Hdr:IO.GenericIO
rm.BASICTrans: Hdr:RISCOS
rm.BASICTrans: Hdr:CMOS
rm.BASICTrans: Hdr:ModHand
rm.BASICTrans: Hdr:Services
rm.BASICTrans: Hdr:Proc
rm.BASICTrans: Hdr:FSNumbers
rm.BASICTrans: Hdr:NewErrors
rm.BASICTrans: Hdr:MsgTrans
