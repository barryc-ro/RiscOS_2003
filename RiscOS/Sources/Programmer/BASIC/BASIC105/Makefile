# Makefile for BASIC105
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date        Name   	Description
# ----        ----   	-----------
# 08-Jun-94   AMcC   	Created.
# 08-Nov-94   AMcC   	Updated to install BASIC64 (non ROM)
# 27 Jun 1997 RWB	Split two targets into seperate makefiles

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
CD	= dir
RM      = remove
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

TOKENISE = tokenise
TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = BASIC105
TARGET    = rm.${COMPONENT}
SOURCE    = ModuleAB

#
# Generic rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: module built

rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
	${RM} ${TARGET}
        ${RM} rm
        @echo ${COMPONENT}: cleaned

resources: resources-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources_common:
        ${MKDIR} ${RESDIR}.BASIC
        ${CP} ^.Resources.${LOCALE}.Messages ${RESDIR}.BASIC.Messages ${CPFLAGS}

resources-None: resources_common
	@

resources-: resources_common
	print ^.Resources.${LOCALE}.CmdHelp { >> ${RESDIR}.BASIC.Messages }

${TARGET}: ^.${SOURCE}
	${MKDIR} rm
	${CD} ^
        ${AS} ${ASFLAGS} -To ${COMPONENT}.${TARGET} -From ${SOURCE}
        ${CD} ${COMPONENT}

# Dynamic dependencies:
BASIC105.rm.BASIC105: BASIC
BASIC105.rm.BASIC105: Fp
BASIC105.rm.BASIC105: Fp2
BASIC105.rm.BASIC105: Expr
BASIC105.rm.BASIC105: Factor
BASIC105.rm.BASIC105: Funct
BASIC105.rm.BASIC105: Stmt
BASIC105.rm.BASIC105: Stmt2
BASIC105.rm.BASIC105: Array
BASIC105.rm.BASIC105: Assembler
BASIC105.rm.BASIC105: Lexical
BASIC105.rm.BASIC105: Command
BASIC105.rm.BASIC105: ErrorMsgs
