# Makefile for MsgTrans
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
LD      = link
CP      = copy
RM      = remove
WIPE    = -wipe
TOUCH   = create
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
ASDEBUG = -PD "AltTitleAndSWI SETL {TRUE}"
CPFLAGS = ~cfr~v
WFLAGS  = fr~c~v
LDFLAGS = -rmf

#
# Program specific options:
#
COMPONENT = MsgTrans
SOURCE    = s.MsgTrans
OBJ       = o.MsgTrans
TARGET    = rm.MsgTrans
DBGTARGET = rm.DbgMsgTrans
EXPORTS   = ${EXP_HDR}.MsgTrans \
            ${EXP_HDR}.MsgMenus
DIRS      = o._dirs
DBGOBJ    = o.DbgMsgTrans

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${OBJ}
        ${LD} ${LDFLAGS} -o $@ ${OBJ}


${OBJ}: ${SOURCE} ${DIRS}
        ${AS} ${ASFLAGS} ${SOURCE}

${DBGTARGET}: ${DBGOBJ}
        ${LD} ${LDFLAGS} -o $@ ${DBGOBJ}

${DBGOBJ}: ${SOURCE} ${DIRS}
        ${AS} ${ASDEBUG} ${ASFLAGS} ${SOURCE}

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

${EXP_HDR}.MsgTrans: hdr.MsgTrans
        ${CP} hdr.MsgTrans $@ ${CPFLAGS}

${EXP_HDR}.MsgMenus: hdr.MsgMenus
        ${CP} hdr.MsgMenus $@ ${CPFLAGS}

# Dynamic dependencies:
