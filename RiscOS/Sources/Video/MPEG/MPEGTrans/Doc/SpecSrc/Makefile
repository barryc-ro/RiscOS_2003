# Makefile for MPEGSystem/Transport specs
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name    Description
# ----       ----    -----------
# 08-Jan-99  BJGA    Created.
# 11-Jan-99  BJGA    Re-jigged to use "munge" absolute.
#

#
# Document specific options:
#

DOCS     = ^.MPEGSystem/html ^.MPEGTransport/html
PICS     = ^.Images.Block/gif

#
# Generic options:
#

MKDIR    = cdir
CC       = cc
CP       = copy
INTERGIF = /@.intergif
LD       = link
RM       = remove
WIPE     = -wipe
CFLAGS   = -c -IC:
CPFLAGS  = ~cfr~v
IFLAGS   = -i -t
WFLAGS   = ~cfr~v

#
# Rule patterns:
#

.SUFFIXES: .c .o .draw .gif

.c.o:
        ${MKDIR} o
        ${CC} ${CFLAGS} -o $@ $<

.draw.gif:
        ${MKDIR} gif
        ${INTERGIF} ${IFLAGS} -o $@ $<

#
# Main rules:
#

all: ${PICS} ${DOCS}
        @echo All built

clean:
        ${RM} munge
        ${WIPE} o ${WFLAGS}
        ${WIPE} html ${WFLAGS}
        ${WIPE} gif ${WFLAGS}
        ${RM} ^.MPEGSystem/html
        ${RM} ^.MPEGTransport/html
        ${WIPE} ^.Images ${WFLAGS}

#
# Static dependencies:
#

^.Images.Block/gif: gif.Block
        ${MKDIR} ^.Images
        ${CP} gif.Block ^.Images.Block/gif ${CPFLAGS}

^.MPEGSystem/html: html.MPEGSystem
        ${CP} html.MPEGSystem $@ ${CPFLAGS}
        SetType $@ HTML

^.MPEGTransport/html: html.MPEGTrans
        ${CP} html.MPEGTrans $@ ${CPFLAGS}
        SetType $@ HTML

html.MPEGSystem: source.Common munge
        ${MKDIR} html
        munge -DSYS source.Common -o $@

html.MPEGTrans: source.Common munge
        ${MKDIR} html
        munge -DTRANS source.Common -o $@

munge: o.munge
        ${LD} ${LDFLAGS} -o $@ o.munge C:o.stubs
