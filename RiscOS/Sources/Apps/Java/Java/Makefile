# Makefile for Boot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Jun-96  SMC          Created.
# 10-Aug-99  SNB	  Updated to use RESDIR for resources: and INSTDIR for install:
#

#
# Generic options:
#
MKDIR   = mkdir -p
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
#INSTDIR = <resource$dir>.Resources.Java
ROMRESDIR	= $(RESDIR).$(COMPONENT)

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
        ${MKDIR} ${ROMRESDIR}
        ${MKDIR} ${ROMRESDIR}.${APP}
        ${CP} ${RDIR}.!Boot      ${ROMRESDIR}.${APP}.!Boot       ${CPFLAGS}
        ${CP} ${RDIR}.!Run       ${ROMRESDIR}.${APP}.!Run        ${CPFLAGS}
        ${CP} ${RDIR}.!Runimage  ${ROMRESDIR}.${APP}.!Runimage   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites   ${ROMRESDIR}.${APP}.!Sprites    ${CPFLAGS}
        ${CP} ${RDIR}.About      ${ROMRESDIR}.${APP}.About       ${CPFLAGS}
        ${CP} ${RDIR}.Copyright  ${ROMRESDIR}.${APP}.Copyright   ${CPFLAGS}
        ${CP} ${RDIR}.Index      ${ROMRESDIR}.${APP}.Index       ${CPFLAGS}
        ${CP} ${RDIR}.lib        ${ROMRESDIR}.${APP}.lib         ${CPFLAGS}
        ${CP} ${RDIR}.Libraries  ${ROMRESDIR}.${APP}.Libraries   ${CPFLAGS}
        ${CP} ${RDIR}.Messages   ${ROMRESDIR}.${APP}.Messages    ${CPFLAGS}
        ${CP} ${RDIR}.Res        ${ROMRESDIR}.${APP}.Res         ${CPFLAGS}
        ${CP} ${RDIR}.Resources  ${ROMRESDIR}.${APP}.Resources   ${CPFLAGS}
        ${CP} ${RDIR}.Setvars    ${ROMRESDIR}.${APP}.Setvars     ${CPFLAGS}
        |
        @echo ${COMPONENT}: resource files copied

install:
        ${MKDIR} ${INSTDIR}
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot       ${CPFLAGS}
        ${CP} ${RDIR}.!Run       ${INSTDIR}.!Run        ${CPFLAGS}
        ${CP} ${RDIR}.!Runimage  ${INSTDIR}.!Runimage   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites   ${INSTDIR}.!Sprites    ${CPFLAGS}
        ${CP} ${RDIR}.About      ${INSTDIR}.About       ${CPFLAGS}
        ${CP} ${RDIR}.Copyright  ${INSTDIR}.Copyright   ${CPFLAGS}
        ${CP} ${RDIR}.Index      ${INSTDIR}.Index       ${CPFLAGS}
        ${CP} ${RDIR}.lib        ${INSTDIR}.lib         ${CPFLAGS}
        ${CP} ${RDIR}.Libraries  ${INSTDIR}.Libraries   ${CPFLAGS}
        ${CP} ${RDIR}.Messages   ${INSTDIR}.Messages    ${CPFLAGS}
        ${CP} ${RDIR}.Res        ${INSTDIR}.Res         ${CPFLAGS}
        ${CP} ${RDIR}.Resources  ${INSTDIR}.Resources   ${CPFLAGS}
        ${CP} ${RDIR}.Setvars    ${INSTDIR}.Setvars     ${CPFLAGS}
        @echo ${COMPONENT}: install phase complete

# Dynamic dependencies:
