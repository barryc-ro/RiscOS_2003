# Makefile for GenericCD Messages module utilities
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-Oct-94  SMC          Created.
#

#
# Generic options:
#
CC      = cc
LD      = link
RM      = remove
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -IC:
WFLAGS  = ~cr~v

#
# Libraries
#
CLIB    = CLIB:o.stubs

#
# Program specific options:
#
TARGETS = ScanRes

#
# Rule patterns
#
.c.o:;  ${CC} ${CCFLAGS} -o $@ $<

#
# Generic rules:
#
all: ${TARGETS}

clean:
        ${RM} ScanRes
        ${WIPE} o.* ${WFLAGS}

ScanRes: o.ScanRes
        ${LD} -o $@ o.ScanRes ${CLIB}

# Dynamic dependencies:
o.ScanRes:	c.ScanRes
o.ScanRes:	C:h.swis
o.ScanRes:	C:h.kernel
