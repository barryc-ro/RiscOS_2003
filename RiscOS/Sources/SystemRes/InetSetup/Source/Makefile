# Makefile for !InetSetup
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 08-Oct-96  KJB    Created
#

#
# Program specific options:
#
COMPONENT  = InetSetup
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
INSTDIR    = ${INSTDIR}.${APP}

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -stamp -quit
CFLAGS  = ${THROWBACK} -c -depend !Depend -ffa -gflv -Wp ${INCLUDES} ${DFLAGS} -DOSLib_4_8 -DPlugIn
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB = CLIB:o.stubs
OSLIB = OSLib:o.OSLib
ILIB = TCPIPLibs:o.inetlib

#
# Include files
#
INCLUDES = -IOS:,C:,TCPIPLibs:

DFLAGS   = 

FILES = \
 ${RDIR}.!Boot\
 ${LDIR}.!Help\
 ${LDIR}.!Run\
 aif.!RunImage\
 ${LDIR}.!Sprites\
 ${LDIR}.!Sprites22\
 ${LDIR}.Blanks.Routes\
 ${LDIR}.Blanks.SetUpNet\
 ${LDIR}.Blanks.User\
 ${RDIR}.CoSprite22\
 ${LDIR}.Messages\
 ${LDIR}.Res\
 ${LDIR}.Sprites\
 ${LDIR}.Sprites22

OBJS = AUN.o Detect.o Diagnose.o event.o FileUtils.o IfsDbox.o Load.o\
       Main.o ModUtils.o Save.o SmallDrag.o swis.o tbevent.o wimpevent.o\
       wimpmsg.o x.o

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} -o $@ $<

#
# Main rules:
#
# Application
#
all: ${FILES}
        @echo ${COMPONENT}: Application built (Disc)

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        ${MKDIR} ${INSTDIR}.AutoSense
        ${CP} ${RDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${LDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} aif.!RunImage      ${INSTDIR}.!RunImage  ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites   ${INSTDIR}.!Sprites   ${CPFLAGS}
        ${CP} ${LDIR}.!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} ${LDIR}.Blanks     ${INSTDIR}.Blanks     ${CPFLAGS}
        ${CP} ${RDIR}.CoSprite22 ${INSTDIR}.CoSprite22 ${CPFLAGS}
        ${CP} ${LDIR}.Messages   ${INSTDIR}.Messages   ${CPFLAGS}
        ${CP} ${LDIR}.Res        ${INSTDIR}.Res        ${CPFLAGS}
        ${CP} ${LDIR}.Sprites    ${INSTDIR}.Sprites    ${CPFLAGS}
        ${CP} ${LDIR}.Sprites22  ${INSTDIR}.Sprites22  ${CPFLAGS}
        Access ${INSTDIR} /
        Access ${INSTDIR}.* wr/r
        Access ${INSTDIR}.Blanks.* wr/r
        @echo ${COMPONENT}: Application installed (Disc)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} aif ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#
aif.!RunImage: ${OBJS} ${CLIB} ${OSLIB} ${ILIB}
        ${MKDIR} aif
        ${LD} -o $@ ${OBJS} ${CLIB} ${OSLIB} ${ILIB} ${LDFLAGS}
        ${SQUEEZE} $@


#---------------------------------------------------------------------------
# Dynamic dependencies:
