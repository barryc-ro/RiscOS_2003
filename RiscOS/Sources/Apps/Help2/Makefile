# Makefile for Help2
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 31-Mar-98  RML    Created
# 27-Apr-98  RML    ROM build rules added

#
# Program specific options:
#
COMPONENT   = Help
APP         = !Help
MODULE      = rm.${COMPONENT}
ROM_MODULE  = aof.${COMPONENT}
RDIR        = Resources
LDIR        = ${RDIR}.${LOCALE}
MODULE_HELP = "!Help"
VERSION     = 3.06
FILELIST    = ${RDIR}.Filelist

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources.${COMPONENT}
RESAPP = <resource$dir>.Apps.${APP}

#
# Generic options:
#
MKDIR   = cdir
MODGEN  = modgen
CC      = cc
LD      = link
CP      = copy
RM      = remove
SQUISH  = squish
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit -module
CFLAGS  = ${THROWBACK} -depend !Depend -ffa ${INCLUDES} 
CPFLAGS = ~cfr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v

#
# Libraries
#
CLIB       = CLib:o.stubs
EVENTLIB   = tbox:o.eventlib
TOOLBOXLIB = tbox:o.toolboxlib
WIMPLIB    = tbox:o.wimplib
WRAPPER    = RISC_OSLib:s.ModuleWrap

#
# Include files
#
INCLUDES = -Itbox:,C:


FILES =\
 ${RDIR}.!Boot \
 ${LDIR}.!Help \
 ${RDIR}.!Run \
 ${RDIR}.!Sprites22 \
 ${RDIR}.!RunImage \
 ${LDIR}.Messages \
 ${LDIR}.Res

OBJS =\
  o.main \
  o.help \
  o.common

# Resources to be included in the ROM module (not in Messages module}
#RESFILES =\
# ${RDIR}.!RunImage

#
# Rule patterns
#
.SUFFIXES: .o

.c.o:; @echo
       @echo Compiling $<
       @${CC} ${CFLAGS} ${DFLAGS}        -c -o $@ $<
 
#
# Main rules:
#

rom: ${MODULE}
        @echo ${COMPONENT}: rom module built

install_rom: ${MODULE}
        ${CP} ${MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

install: ${FILES}
         @echo
         @echo ${COMPONENT}: Creating application ${APP}
         ${MKDIR} ${INSTDIR}.${APP}
         ${CP} ${RDIR}.!Boot      ${INSTDIR}.${APP}.!Boot      ${CPFLAGS}
         ${CP} ${LDIR}.!Help      ${INSTDIR}.${APP}.!Help      ${CPFLAGS}
         ${CP} ${RDIR}.!Run       ${INSTDIR}.${APP}.!Run       ${CPFLAGS}
         ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.${APP}.!Sprites22 ${CPFLAGS}
         ${CP} ${RDIR}.!RunImage  ${INSTDIR}.${APP}.!RunImage  ${CPFLAGS} 
         ${CP} ${LDIR}.Messages   ${INSTDIR}.${APP}.Messages   ${CPFLAGS}
         ${CP} ${LDIR}.Res        ${INSTDIR}.${APP}.Res        ${CPFLAGS}
         Access ${INSTDIR}.${APP}.* wr/r
         @echo
         @echo ${COMPONENT}: Application installed {Disc}

resources-C:
	${MKDIR} ${RESAPP}
        ${MKDIR} ${RESDIR}
        |
        ${CP} ${LDIR}.!Help         ${RESAPP}.!Help     ${CPFLAGS}
        ${CP} ${RDIR}.ROM.!Run      ${RESAPP}.!Run      ${CPFLAGS}
        ${CP} ${RDIR}.ROM.!Boot     ${RESAPP}.!Boot     ${CPFLAGS}
        |
        ${CP} ${RDIR}.!Sprites22    ${RESDIR}.!Sprites22 ${CPFLAGS}
        ${CP} ${LDIR}.Res           ${RESDIR}.Res       ${CPFLAGS}

resources-RES: 
        ${MKDIR} ${RESDIR}

resources: resources-${TYPE}
        ${CP} ${LDIR}.Messages      ${RESDIR}.Messages  ${CPFLAGS}
        |
        @echo ${COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} ${APP}.* ${WFLAGS}
        ${RM} ${MODULE}
        ${RM} ${RDIR}.!RunImage
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

Resources.!RunImage: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${CLIB}
       ${LD} -o $@ ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${OBJS}

${MODULE}: ${FILES}
           ${MODGEN} $@ ${MODULE} ${MODULE_HELP} ${VERSION} -via ${FILELIST}

#---------------------------------------------------------------------------
# Dynamic dependencies:
