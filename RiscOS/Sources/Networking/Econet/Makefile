# Makefile for Econet
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 28-Jun-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR    = cdir
AS       = aasm
CP       = copy
MODGEN   = modgen
RM       = remove
TOKENISE = tokenise

AFLAGS   = -depend !Depend -stamp -closeexec -quit
CPFLAGS  = ~cfr~v

TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT   = Econet
SOURCE      = NewOS
SOURCE200   = OldOS
TARGET      = rm.Econet
TARGET200   = rm.Econet200
HDR         = hdr.${COMPONENT}
EXPORTS     = ${EXP_HDR}.Econet

MESSAGESMOD = rm.EconetMsgs
MESSAGES    = Resources.${LOCALE}.Messages
RESPATH     = Resources.Econet.Messages
MODNAME     = EconetMessages
MODHELP     = EconetMessages

#
# Rules:
#
all: ${TARGET}
        @echo ${COMPONENT}: ${TARGET} built

all200: ${TARGET200}
        @echo ${COMPONENT}: ${TARGET200} built

#
# Generic Build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: $(TARGET)
        $(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo ${COMPONENT}: rom module installed

install: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${TARGET200}
        ${RM} ${MESSAGESMOD}
        ${RM} setversion
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages ${RESDIR}.${COMPONENT}.Messages ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# Assemble the Econet module
#
${TARGET}: ${SOURCE}
        ${AS} ${SOURCE} $@ ${AFLAGS} -module

${TARGET200}: ${SOURCE200}
        ${AS} ${SOURCE200} $@ ${AFLAGS} -module

#
# Export hdr files
#
${EXP_HDR}.Econet: hdr.Econet
        ${CP} hdr.Econet $@ ${CPFLAGS}

# 
# Generate an Econet Messages module
# Note: need to set alias to ensure that <Version> is expanded
#
msgmod: ${MESSAGESMOD}
        @echo ${COMPONENT}: messages module built

${MESSAGESMOD}: ${MESSAGES} setversion
        setversion
        set alias$modgen_ver ${MODGEN} $@ ${MODNAME} ${MODHELP} <Version> ${MESSAGES} ${RESPATH}
        modgen_ver
        unset alias$modgen
        unset Version

setversion: Time+Date
        ${AS} utils.getversion $@ ${AFLAGS}
        settype setversion obey

maketime:
        MakeTime

version:
        Version

#---------------------------------------------------------------------------
# Dynamic dependencies:
