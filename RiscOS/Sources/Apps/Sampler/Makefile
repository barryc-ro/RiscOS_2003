# Makefile for Sampler
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 29-Apr-98  RML    Created

#
# Program specific options:
#
COMPONENT   = Sampler
APP         = !Sampler
RDIR        = Resources
LDIR        = ${RDIR}.${LOCALE}

#
# Generic options:
#
MKDIR   = cdir
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
 ${RDIR}.Sprites \
 ${RDIR}.!RunImage \
 ${LDIR}.Messages \
 ${LDIR}.Res

OBJS =\
  o.main \
  o.sample \
  o.common

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


install: ${FILES}
         @echo
         @echo ${COMPONENT}: Creating application ${APP}
         ${MKDIR} ${INSTDIR}.${APP}
         ${CP} ${RDIR}.!Boot      ${INSTDIR}.${APP}.!Boot      ${CPFLAGS}
         ${CP} ${LDIR}.!Help      ${INSTDIR}.${APP}.!Help      ${CPFLAGS}
         ${CP} ${RDIR}.!Run       ${INSTDIR}.${APP}.!Run       ${CPFLAGS}
         ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.${APP}.!Sprites22 ${CPFLAGS}
         ${CP} ${RDIR}.Sprites    ${INSTDIR}.${APP}.Sprites    ${CPFLAGS}
         ${CP} ${RDIR}.!RunImage  ${INSTDIR}.${APP}.!RunImage  ${CPFLAGS} 
         ${CP} ${LDIR}.Messages   ${INSTDIR}.${APP}.Messages   ${CPFLAGS}
         ${CP} ${LDIR}.Res        ${INSTDIR}.${APP}.Res        ${CPFLAGS}
         Access ${INSTDIR}.${APP}.* wr/r
         @echo
         @echo ${COMPONENT}: Application installed {Disc}

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} ${APP}.* ${WFLAGS}
        ${RM} ${RDIR}.!RunImage
        ${RM} ${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

Resources.!RunImage: ${OBJS} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${CLIB}
       ${LD} -o $@ ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${OBJS}


#---------------------------------------------------------------------------
# Dynamic dependencies:
