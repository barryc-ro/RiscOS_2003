# Makefile for Help
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 05-Nov-94  AMcC   Updated for Black build
#

#
# Program specific options:
#
COMPONENT  = Help
APP        = !${COMPONENT}
ROM_MODULE = rm.${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
SQUISH  = squish
WIPE    = -wipe
STRIP   = stripdepnd

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit -module
CPFLAGS = ~cfr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v

FILES =\
 ${RDIR}.!Boot \
 LocalRes:!Help \
 LocalRes:!Run \
 ${RDIR}.!RunImage \
 LocalRes:Messages \
 LocalRes:Templates 

# Resources to be included in the ROM module (not in Messages module}
RESFILES =\
 ${RDIR}.!RunImage

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc}

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM}

install: ${FILES} resources-BAS
        ${MKDIR} ${INSTDIR}.${APP}
        |
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.${APP}.!Boot     ${CPFLAGS}
        ${CP} LocalRes:!Help     ${INSTDIR}.${APP}.!Help     ${CPFLAGS}
        ${CP} LocalRes:!Run      ${INSTDIR}.${APP}.!Run      ${CPFLAGS}
        ${CP} ${RDIR}.!RunImage  ${INSTDIR}.${APP}.!RunImage ${CPFLAGS}
        ${CP} LocalRes:Messages  ${INSTDIR}.${APP}.Messages  ${CPFLAGS}
        ${CP} LocalRes:Templates ${INSTDIR}.${APP}.Templates ${CPFLAGS}
        |
        Access ${INSTDIR}.${APP}.* lr/r
        |
        @echo ${COMPONENT}: Application installed {Disc}

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed {ROM}

resources: ${LDIR}.ROM.!RunLink
        ${MKDIR} ${RESAPP}
        ${MKDIR} ${RESDIR}
        |
        ${CP} LocalRes:!Help         ${RESAPP}.!Help     ${CPFLAGS}
        ${CP} LocalRes:ROM.!Run      ${RESAPP}.!Run      ${CPFLAGS}
        |
        ${CP} ${LDIR}.ROM.!RunLink   ${RESDIR}.!RunLink  ${CPFLAGS}
        ${CP} LocalRes:Messages      ${RESDIR}.Messages  ${CPFLAGS}
        ${CP} LocalRes:Templates     ${RESDIR}.Templates ${CPFLAGS}
        |
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${RM} ${ROM_MODULE}
        ${RM} ${RDIR}.!RunImage
        ${RM} ${LDIR}.ROM.!RunLink
        ${RM} s.Version
        ${WIPE} crunched.* ${WFLAGS}
        ${STRIP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

${ROM_MODULE}: s.Main s.Version ${RESFILES}
        ${AS} ${AFLAGS} -from s.Main -to $@

s.Version: LocalRes:Messages
        awk -f awk.Version LocalRes:Messages > $@

${RDIR}.!RunImage: crunched.!RunImage
        ${SQUISH} ${SQFLAGS} -from crunched.!RunImage -to $@

crunched.!RunImage: bas.!RunImage
        crunch.!RunImage; BASIC

${LDIR}.ROM.!RunLink: bas.!RunLink
        ${SQUISH} ${SQFLAGS} -from bas.!RunLink -to $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
