# Makefile for Boot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Jun-96  SMC          Created.
#

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

#
# Program specific options:
#
COMPONENT = Java
APP       = !Java
SOURCE    =
TARGET    =
RDIR = Resources
INSTDIR = <resource$dir>.Resources.Java

#
# Export Paths for Messages module
#


#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${INSTDIR}
        ${MKDIR} ${INSTDIR}.${APP}
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.${APP}.!Boot       ${CPFLAGS}
        ${CP} ${RDIR}.!Run       ${INSTDIR}.${APP}.!Run        ${CPFLAGS}
        ${CP} ${RDIR}.!Runimage  ${INSTDIR}.${APP}.!Runimage   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites   ${INSTDIR}.${APP}.!Sprites    ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.${APP}.!Sprites22  ${CPFLAGS}
        ${CP} ${RDIR}.About      ${INSTDIR}.${APP}.About       ${CPFLAGS}
        ${CP} ${RDIR}.Copyright  ${INSTDIR}.${APP}.Copyright   ${CPFLAGS}
        ${CP} ${RDIR}.Index      ${INSTDIR}.${APP}.Index       ${CPFLAGS}
        ${CP} ${RDIR}.lib        ${INSTDIR}.${APP}.lib         ${CPFLAGS}
        ${CP} ${RDIR}.Libraries  ${INSTDIR}.${APP}.Libraries   ${CPFLAGS}
        ${CP} ${RDIR}.Messages   ${INSTDIR}.${APP}.Messages    ${CPFLAGS}
        ${CP} ${RDIR}.Res        ${INSTDIR}.${APP}.Res         ${CPFLAGS}
        ${CP} ${RDIR}.Resources  ${INSTDIR}.${APP}.Resources   ${CPFLAGS}
        ${CP} ${RDIR}.Setvars    ${INSTDIR}.${APP}.Setvars     ${CPFLAGS}
        |
        @echo ${COMPONENT}: resource files copied

# Dynamic dependencies:
