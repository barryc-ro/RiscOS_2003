# Makefile for SSLRO
# The real SSL is cross-compiled.
# This contains the exported headers and libraries
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
#

#
# Generic options:
#
MKDIR   = cdir
CP      = copy
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
SSLDIR    = <Lib$Dir>.SSL

#
# Program specific options:
#
COMPONENT = SSLRO

# RISC OS ROM build rules:
#
export:
	${mkdir} ${SSLDIR}
        ${CP} h ${SSLDIR}.h ${CPFLAGS}
        ${CP} o ${SSLDIR}.o ${CPFLAGS}
        @echo ${COMPONENT}: export complete

clean:
        @echo ${COMPONENT}: cleaned

# Dynamic dependencies:
