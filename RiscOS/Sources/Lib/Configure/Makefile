# Makefile for ConfigureLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 10-Nov-97      BJGA   Created
# 09-Dec-97      BJGA   Added entries for new source files cmos.c and str.c
#                       and converted to create library from object files
# 17-Dec-97      BJGA   Tidied up
# 06-Apr-98      BJGA   Renamed ConfigureLib; added export facilities
#

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
ATTR    = -attr
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe
LIBFILE = libfile

CFLAGS  =  -c -depend !Depend -ffa -gflv ${INCLUDES} -throwback
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
LIBFILEFLAGS = -o -c

#
# Include files
#
INCLUDES = -ITbox:,C:

OBJS = @.o.cmos @.o.error @.o.misc @.o.str

LIB = @.o.ConfigLib

#
# Rule patterns
#
.c.o:;  ${CC} ${CFLAGS} -o $@ $<

#
# Main rules:
#
all:    ${LIB}

${LIB}: ${OBJS}
        ${LIBFILE} ${LIBFILEFLAGS} $@ ${OBJS}
        @echo ConfigureLib built (Disc)

export_hdrs: @.h.cmos @.h.error @.h.message @.h.misc @.h.str
        ${MKDIR} <Lib$Dir>.ConfigLib
        ${MKDIR} <Lib$Dir>.ConfigLib.h
        ${CP} @.h <Lib$Dir>.ConfigLib.h ${CPFLAGS}
        @echo ConfigureLib export complete (hdrs)

export_libs: ${LIB}
        ${MKDIR} <Lib$Dir>.ConfigLib
        ${MKDIR} <Lib$Dir>.ConfigLib.o
        ${CP} ${LIB} <Lib$Dir>.ConfigLib.o.ConfigLib ${CPFLAGS}
        @echo ConfigureLib export complete (libs)

clean:
        ${WIPE} o.* ${WFLAGS}
        @echo ConfigureLib cleaned

#---------------------------------------------------------------------------
# Dynamic dependencies:
