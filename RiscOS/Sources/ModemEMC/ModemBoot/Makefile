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

#
# Main rules:
#
resources: ${RESFILES}
        ${MKDIR} ${RESDIR}
        ${CP} ${LDIR}.*          ${RESDIR}.*             ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module


#---------------------------------------------------------------------------
# Dynamic dependencies:
