# Makefile for hourmake
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 23-Apr-98  RML    Created

#
# Program specific options:
#
COMPONENT  = hourmake

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

#
# Include files
#
INCLUDES = -IC:

FILES =\
 obj.hourmake

OBJS =\
  o.hourmake

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

all:     ${FILES}
         @echo ${COMPONENT}: Application made.

clean:
        ${WIPE} o.*          ${WFLAGS}
        ${RM}   obj.hourmake
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

obj.hourmake: ${OBJS} ${CLIB}
       ${LD} -o $@ ${CLIB} ${OBJS}

#---------------------------------------------------------------------------
# Dynamic dependencies:
