MKDIR   = cdir
CC      = cc
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

CFLAGS  = ${THROWBACK} -c -depend !Depend -fah -fussy ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
SQFLAGS = -nolist
WFLAGS  = ~c~v
LINKFLAGS =

#
# Libraries
#
CLIB       = CLib:o.stubs
WIMPLIB    = tbox:o.wimplib


#
# Include files
#
INCLUDES = -Itbox:,C:

DFLAGS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<

#
# GENERIC RULES
#
all: ^.TTextTest ^.TTextKill
     @echo all built

clean:
        ${RM} o.TTextTest
        ${RM} o.TTextKill
        ${RM} ^.TTextTest
        ${RM} ^.TTextKill
        @echo cleaned

#
# Static dependencies:
#
^.TTextTest: o.TTextTest ${CLIB} ${WIMPLIB} 
        ${LD} ${LINKFLAGS} -o $@ o.TTextTest ${CLIB} ${WIMPLIB} 
#        ${SQUEEZE} $@

^.TTextKill: o.TTextKill ${CLIB} ${WIMPLIB} 
        ${LD} ${LINKFLAGS} -o $@ o.TTextKill ${CLIB} ${WIMPLIB} 
#        ${SQUEEZE} $@

#---------------------------------------------------------------------------
# Dynamic dependencies:
