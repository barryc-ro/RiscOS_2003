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
TARGET    = rm.${SYSTEM}.Messages
RESDIR    = Resources.${LOCALE}.${SYSTEM}
UTILS     = Utils
DATA      = Data
# Note: RESDATA mustn't be in RESDIR !!
RESDATA   = ${DATA}.Data
HELPTAGS  = ${DATA}.HelpTags
HELPMESS  = ${DATA}.HelpMess
HELPTOKENS= ${DATA}.HelpTokens
DICTTAGS  = ${DATA}.DictTags
DICTMESS  = ${DATA}.DictMess
DICTTOKENS= ${DATA}.DictTokens
DICTIONARY= ${DATA}.Dictionary

MINDICT   = ${DATA}.MinDict

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
        @echo ${COMPONENT}: rom module built for System: ${SYSTEM}

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
        ${WIPE} Resources.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned (Dictionary & HelpTokens not removed)

clean_all:
        ${RM} ${TARGET}
        ${RM} ${RESDATA}
        ${RM} ${DATA}.Tokenised
        ${RM} ${HELPTAGS}
        ${RM} ${HELPMESS}
        ${RM} ${HELPTOKENS}
        ${RM} ${DICTTAGS}
        ${RM} ${DICTMESS}
        ${RM} ${DICTTOKENS}
        ${RM} ${DICTIONARY}
        ${WIPE} Resources.* ${WFLAGS}
        @echo ${COMPONENT}: all cleaned

resources:
        ${MKDIR} ${RESDIR}.Resources.Global
        ${CP} Global.${LOCALE}.Messages ${RESDIR}.Resources.Global.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} ${DATA}.Tokenised
        ${MKDIR} rm.${SYSTEM}
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
