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
TARGET    = rm.${LOCALE}.${BUILD}.Messages
PROCESSDIR= Processed.${LOCALE}.${BUILD}
RESDIR    = Resources.${LOCALE}.${BUILD}
UTILS     = Utils
DATA      = Data.${LOCALE}.${BUILD}
# Note: RESDATA mustn't be in PROCESSDIR !!
RESDATA   = ${DATA}.Data
HELPTAGS  = ${DATA}.HelpTags
HELPMESS  = ${DATA}.HelpMess
HELPTOKENS= ${DATA}.HelpTokens
DICTTAGS  = ${DATA}.DictTags
DICTMESS  = ${DATA}.DictMess
DICTTOKENS= ${DATA}.DictTokens
DICTIONARY= ${DATA}.Dictionary
DIRS      = ${DATA}.Dirs

MINDICT   = Global.${LOCALE}.MinDict
MINDICTT  = Global.${LOCALE}.MinDictT

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

#
# Clean: clean current build and locale, except dictionary and HelpTokens
#
clean:
        ${RM} ${TARGET}
        ${RM} ${RESDATA}
        ${RM} ${DATA}.Tokenised
        ${RM} ${HELPTAGS}
        ${RM} ${DICTTAGS}
        ${WIPE} Resources.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned (Dictionary & HelpTokens not removed)

#
# Clean_all: totally clean all builds and locales
#
clean_all:
	${WIPE} Data.* ${WFLAGS}
	${WIPE} rm.* ${WFLAGS}
        ${WIPE} Resources.* ${WFLAGS}
	${WIPE} Processed.* ${WFLAGS}
        @echo ${COMPONENT}: all cleaned

resources:
        ${MKDIR} ${RESDIR}.Resources.Global
        ${CP} Global.${LOCALE}.Messages ${RESDIR}.Resources.Global.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} ${DATA}.Tokenised ${DIRS}
        ${SCANRES} ${PROCESSDIR} ${RESDATA}
        set resdata ${RESDATA}
        ${AS} ${ASFLAGS} -To $@ -From ${SOURCE}
        unset resdata

#
# Tokenise the Messages file using Help and Dictionary Tokens
#
${DATA}.Tokenised: ${DICTIONARY} ${HELPTOKENS} ${HELPTAGS}
        Print ${HELPTOKENS} { >> ${PROCESSDIR}.Resources.Help.Messages }
        ${MSGTOK} ${HELPTOKENS} ${HELPTAGS}
        ${CP} ${DICTIONARY} ${PROCESSDIR}.Resources.Kernel.Dictionary ${CPFLAGS}
        ${MSGTOK2} ${DICTTOKENS} ${DICTTAGS}
        ${TOUCH} ${DATA}.Tokenised; settype ${DATA}.Tokenised fff

#
# Copy Local Resources, then extract Help and Dictionary token tags
#
${HELPTAGS}: ${TAGEXT} ${DIRS}
        @echo Copying Resources to Processed directory for tokenising ...
        ${CP} Resources Processed ${CPFLAGS}
        ${TAGEXT} ${PROCESSDIR} ${DICTTAGS} ${HELPTAGS}

#
# Help tokens
#
${HELPTOKENS}:
        ${MSGEXT} ${HELPMESS} ${HELPTAGS}
        ${TOKGEN} ${HELPMESS} ${HELPTOKENS}

#
# Use minimum Dictionary tokens:
#
#${DICTIONARY}: ${MINDICT}
#        ${CP} ${MINDICT}  ${DICTIONARY} ${CPFLAGS}
#        ${CP} ${MINDICTT} ${DICTTOKENS} ${CPFLAGS}

#
# Use optimal Dictionary tokens:
#
${DICTIONARY}:
        ${MSGEXT2} ${DICTMESS} ${DICTTAGS}
        ${TOKGEN2} ${DICTMESS} ${DICTTOKENS} ${DICTIONARY}

${TAGEXT}:
        Dir ${UTILS}
        ${MAKE}
        Up

${DIRS}:
	${MKDIR} Data
	${MKDIR} Data.${LOCALE}
	${MKDIR} ${DATA}
	${MKDIR} rm
	${MKDIR} rm.${LOCALE}
	${MKDIR} rm.${LOCALE}.${BUILD}
	${MKDIR} Processed
	${MKDIR} Processed.${LOCALE}
	${MKDIR} Processed.${LOCALE}.${BUILD}
	${TOUCH} ${DIRS}

# Dynamic dependencies:
