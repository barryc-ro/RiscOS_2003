# Makefile for !Boot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 01-Sep-95  JRH    Created
#

#
# Program specific options:
#
COMPONENT  = Boot
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.${APP}
RESAPP = <resource$dir>.${APP}

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
SQUISH    = squish
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit -module
CPFLAGS = ~cfr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v

FILES =			\
 ${LDIR}.!Boot		\
 ${LDIR}.!Run		\
 ${LDIR}.!RunImage	\
 ${LDIR}.CDFSMap	\
 ${LDIR}.GoOn		\
 ${LDIR}.PlayNTSC	\
 ${LDIR}.PlayPAL

# Resources to be included in the ROM module (not in Messages module}
RESFILES =	\
 ${LDIR}.!Boot		\
 ${LDIR}.!Run		\
 ${LDIR}.!RunImage	\
 ${LDIR}.CDFSMap	\
 ${LDIR}.GoOn		\
 ${LDIR}.PlayNTSC	\
 ${LDIR}.PlayPAL

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc}

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM}

install: ${FILES}
        ${MKDIR} ${INSTDIR}.${APP}
        ${CP} ${LDIR}.!Boot      ${INSTDIR}.${APP}.!Boot             ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${INSTDIR}.${APP}.!Run              ${CPFLAGS}
        ${CP} ${LDIR}.!RunImage  ${INSTDIR}.${APP}.!RunImage         ${CPFLAGS}
        ${CP} ${LDIR}.CDFSMap    ${INSTDIR}.${APP}.CDFSMap           ${CPFLAGS}
        ${CP} ${LDIR}.GoOn       ${INSTDIR}.${APP}.GoOn              ${CPFLAGS}
        ${CP} ${LDIR}.PlayNTSC   ${INSTDIR}.${APP}.PlayNTSC          ${CPFLAGS}
        ${CP} ${LDIR}.PlayPAL    ${INSTDIR}.${APP}.PlayPAL           ${CPFLAGS}
        Access ${INSTDIR}.${APP}.* lr/r
        @echo ${COMPONENT}: Application installed {Disc}

install_rom:
        @echo ${COMPONENT}: Module installed {ROM}

resources: ${RESFILES}
        ${MKDIR} ${RESDIR}
        ${CP} ${LDIR}.!Boot      ${RESDIR}.!Boot             ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${RESDIR}.!Run              ${CPFLAGS}
        ${CP} ${LDIR}.!RunImage  ${RESDIR}.!RunImage         ${CPFLAGS}
        ${CP} ${LDIR}.CDFSMap    ${RESDIR}.CDFSMap           ${CPFLAGS}
        ${CP} ${LDIR}.GoOn       ${RESDIR}.GoOn              ${CPFLAGS}
        ${CP} ${LDIR}.PlayNTSC   ${RESDIR}.PlayNTSC          ${CPFLAGS}
        ${CP} ${LDIR}.PlayPAL    ${RESDIR}.PlayPAL           ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        @echo ${COMPONENT}: cleaned


#---------------------------------------------------------------------------
# Dynamic dependencies:
