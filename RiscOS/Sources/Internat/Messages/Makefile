# Makefile for Messages module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 02-Jun-94  AMcC         Created.
# 08-Dec-94  AMcC         Adjust rules and dependencies so that
#                         tokens don't need to be generated every time
#                         - remove data.Dictionary to regenerate tokens
#

#
# Generic options:
#
AS      = aasm
CP      = copy
MKDIR   = cdir
RM      = remove
WIPE    = -wipe
MAKE    = amu

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v
TOUCH   = create
WFLAGS  = ~cr~v

#
# Program specific options:
#
COMPONENT = Messages
SOURCE    = s.Messages
TARGET    = rm.${BUILD}.Messages
RESDIR    = Resources.${LOCALE}.${BUILD}
UTILS     = Utils
DATA      = Data.${BUILD}
# Note: RESDATA mustn't be in RESDIR !!
RESDATA   = ${DATA}.Data
HELPTAGS  = ${DATA}.HelpTags
HELPMESS  = ${DATA}.HelpMess
HELPTOKENS= ${DATA}.HelpTokens
DICTTAGS  = ${DATA}.DictTags
DICTMESS  = ${DATA}.DictMess
DICTTOKENS= ${DATA}.DictTokens
DICTIONARY= ${DATA}.Dictionary

MINDICT   = Global.${LOCALE}.MinDict

#
# Utility programs:
#
TAGEXT  = ${UTILS}.TagExtract
MSGEXT  = ${UTILS}.MsgExt
MSGTOK  = ${UTILS}.MsgTok
TOKGEN  = ${UTILS}.TokGen
MSGEXT2 = ${UTILS}.MsgExt2
MSGTOK2 = ${UTILS}.MsgTok2
TOKGEN2 = ${UTILS}.TokGen2
SCANRES = ${UTILS}.ScanRes

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built for Build: ${BUILD}

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${RESDATA}
        ${RM} ${DATA}.Tokenised
        ${RM} ${HELPTAGS}
        ${RM} ${DICTTAGS}
        ${WIPE} ${RESDIR}.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned (Dictionary & HelpTokens not removed)

clean_all:
	${WIPE} Data.* ${WFLAGS}
	${WIPE} rm.* {WFLAGS}
        ${WIPE} Resources.* ${WFLAGS}
        @echo ${COMPONENT}: all cleaned

resources:
        ${MKDIR} ${RESDIR}.Resources.Global
        ${CP} Global.${LOCALE}.Messages ${RESDIR}.Resources.Global.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} ${DATA}.Tokenised dirs
        ${MKDIR} rm.${BUILD}
        set resdata ${RESDATA}
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}
        unset resdata

${DATA}.Tokenised: ${DICTIONARY} ${HELPTAGS}
        ${MKDIR} ${RESDIR}.Resources.Kernel
        ${CP} ${DICTIONARY} ${RESDIR}.Resources.Kernel.Dictionary ${CPFLAGS}
        ${MSGTOK} ${HELPTOKENS} ${HELPTAGS}
        ${MSGTOK2} ${DICTTOKENS} ${DICTTAGS}
        Print ${HELPTOKENS} { >> ${RESDIR}.Resources.Help.Messages }
        ${SCANRES} ${RESDIR} ${RESDATA}
        ${TOUCH} ${DATA}.Tokenised; settype ${DATA}.Tokenised fff

${HELPTAGS}: ${TAGEXT}
        ${TAGEXT} ${RESDIR} ${DICTTAGS} ${HELPTAGS}

${DICTIONARY}:
        ${MSGEXT} ${HELPMESS} ${HELPTAGS}
        ${MSGEXT2} ${DICTMESS} ${DICTTAGS}
        ${TOKGEN} ${HELPMESS} ${HELPTOKENS}
        ${TOKGEN2} ${DICTMESS} ${DICTTOKENS} ${DICTIONARY}

${TAGEXT}:
        Dir ${UTILS}
        ${MAKE}
        Up

dirs:
	${MKDIR} ${DATA}

# Dynamic dependencies:
rm.Morris2.Messages: Hdr:ListOpts
rm.Morris2.Messages: Hdr:Macros
rm.Morris2.Messages: Hdr:System
rm.Morris2.Messages: Hdr:SWIs
rm.Morris2.Messages: Hdr:CPU.Generic26
rm.Morris2.Messages: Hdr:IO.GenericIO
rm.Morris2.Messages: Hdr:RISCOS
rm.Morris2.Messages: Hdr:ModHand
rm.Morris2.Messages: Hdr:Services
rm.Morris2.Messages: Hdr:Proc
rm.Morris2.Messages: Hdr:ResourceFS
rm.Morris2.Messages: Version
rm.Morris2.Messages: Hdr:NDRDebug
rm.Morris4.Messages: Hdr:ListOpts
rm.Morris4.Messages: Hdr:Macros
rm.Morris4.Messages: Hdr:System
rm.Morris4.Messages: Hdr:SWIs
rm.Morris4.Messages: Hdr:CPU.Generic26
rm.Morris4.Messages: Hdr:IO.GenericIO
rm.Morris4.Messages: Hdr:RISCOS
rm.Morris4.Messages: Hdr:ModHand
rm.Morris4.Messages: Hdr:Services
rm.Morris4.Messages: Hdr:Proc
rm.Morris4.Messages: Hdr:ResourceFS
rm.Morris4.Messages: Version
rm.Morris4.Messages: Hdr:NDRDebug
rm.Omega4.Messages: Hdr:ListOpts
rm.Omega4.Messages: Hdr:Macros
rm.Omega4.Messages: Hdr:System
rm.Omega4.Messages: Hdr:SWIs
rm.Omega4.Messages: Hdr:CPU.Generic26
rm.Omega4.Messages: Hdr:IO.GenericIO
rm.Omega4.Messages: Hdr:RISCOS
rm.Omega4.Messages: Hdr:ModHand
rm.Omega4.Messages: Hdr:Services
rm.Omega4.Messages: Hdr:Proc
rm.Omega4.Messages: Hdr:ResourceFS
rm.Omega4.Messages: Version
rm.Omega4.Messages: Hdr:NDRDebug
rm.RCA4.Messages: Hdr:ListOpts
rm.RCA4.Messages: Hdr:Macros
rm.RCA4.Messages: Hdr:System
rm.RCA4.Messages: Hdr:SWIs
rm.RCA4.Messages: Hdr:CPU.Generic26
rm.RCA4.Messages: Hdr:IO.GenericIO
rm.RCA4.Messages: Hdr:RISCOS
rm.RCA4.Messages: Hdr:ModHand
rm.RCA4.Messages: Hdr:Services
rm.RCA4.Messages: Hdr:Proc
rm.RCA4.Messages: Hdr:ResourceFS
rm.RCA4.Messages: Version
rm.RCA4.Messages: Hdr:NDRDebug
rm.RCA.Messages: Hdr:ListOpts
rm.RCA.Messages: Hdr:Macros
rm.RCA.Messages: Hdr:System
rm.RCA.Messages: Hdr:SWIs
rm.RCA.Messages: Hdr:CPU.Generic26
rm.RCA.Messages: Hdr:IO.GenericIO
rm.RCA.Messages: Hdr:RISCOS
rm.RCA.Messages: Hdr:ModHand
rm.RCA.Messages: Hdr:Services
rm.RCA.Messages: Hdr:Proc
rm.RCA.Messages: Hdr:ResourceFS
rm.RCA.Messages: Version
rm.RCA.Messages: Hdr:NDRDebug
rm.RCA8.Messages: Hdr:ListOpts
rm.RCA8.Messages: Hdr:Macros
rm.RCA8.Messages: Hdr:System
rm.RCA8.Messages: Hdr:SWIs
rm.RCA8.Messages: Hdr:CPU.Generic26
rm.RCA8.Messages: Hdr:IO.GenericIO
rm.RCA8.Messages: Hdr:RISCOS
rm.RCA8.Messages: Hdr:ModHand
rm.RCA8.Messages: Hdr:Services
rm.RCA8.Messages: Hdr:Proc
rm.RCA8.Messages: Hdr:ResourceFS
rm.RCA8.Messages: Version
rm.RCA8.Messages: Hdr:NDRDebug
rm.Boca8.Messages: Hdr:ListOpts
rm.Boca8.Messages: Hdr:Macros
rm.Boca8.Messages: Hdr:System
rm.Boca8.Messages: Hdr:SWIs
rm.Boca8.Messages: Hdr:CPU.Generic26
rm.Boca8.Messages: Hdr:IO.GenericIO
rm.Boca8.Messages: Hdr:RISCOS
rm.Boca8.Messages: Hdr:ModHand
rm.Boca8.Messages: Hdr:Services
rm.Boca8.Messages: Hdr:Proc
rm.Boca8.Messages: Hdr:ResourceFS
rm.Boca8.Messages: Version
rm.Boca8.Messages: Hdr:NDRDebug
