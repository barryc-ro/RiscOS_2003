# Makefile for LoadCMOS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name       Description
# ----           ----       -----------
# 27th Oct 1994  JC         Created.
#

#
# Generic options:
#
MKDIR   = mkdir -p
CC      = cc -ffach -wp
CP      = copy
CPFLAGS = ~cfr~v
OBJASM  = ObjAsm ${THROWBACK} -depend !Depend -stamp -quit
CMHG    = CMHG
LINK    = Link

#
# Libraries:
#

#
# Program specific options:
#
COMPONENT = LoadCMOS
TARGET    = aof.LoadCMOS

OBJS = o.main

ASMS = s.main

#
# Generic rules {used by top-level build}:
#
export:
        @echo ${COMPONENT}: export complete

clean:
       -Destroy LoadCMOS ${OBJS} ${OBJSD}
        @echo ${COMPONENT}: cleaned

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${HFILES} ${OSLIB} ${RSTUBS}
        ${LINK} -o $@ -aof ${OBJS} ${OSLIB} ${RSTUBS}

# Extra {development} rules:
#

all: LoadCMOS
   @Echo Made all

asms: ${ASMS}

release: LoadCMOS
   %Copy LoadCMOS Boot:Library.LoadCMOS ~CFLNRV
   @Echo Made release

#Internal targets
LoadCMOS: ${OBJS} CLib:o.Stubs
   ${LINK} -output LoadCMOS ${OBJS} CLib:o.Stubs
   Access LoadCMOS WR/R

#General rules
.SUFFIXES: .cmhg .c .debug .o .s

.c.o:
   ${CC} ${THROWBACK} ${CFLAGS} -depend !Depend -c -ff $<

# Dynamic dependencies:
