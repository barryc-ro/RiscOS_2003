# Makefile for tgrlib

#
# Tool options:
#
CC	= cc
LIBFILE = libfile
WIPE	= -wipe
COPY	= copy
REMOVE	= remove
MKDIR	= cdir
CCFlags	= -c -depend !Depend -IC: -throwback -zM 
LFlags	= -c -o
WFlags	= ~c~v
CFlags	= ~cf~v

#
# Program specific options:
#
COMPONENT = tgrlib
DEBUG = o.debug
MSGS = o.msgs
DBMALLOC = o.dbmalloc
LIBDIR = <Lib$Dir>

#
# Rule patterns:
#
.c.o:;	${cc} ${ccflags} -o $@ $<

#
# Build
#
export:	${debug} ${msgs} ${dbmalloc}
	${libfile} ${lflags} ${component} ${debug} ${msgs} ${dbmalloc}
	${mkdir} ${libdir}.tgrlib
	${mkdir} ${libdir}.tgrlib.h
	${mkdir} ${libdir}.tgrlib.o
	${copy} h.* ${libdir}.tgrlib.h.* ${cflags}
	${copy} ${component} ${libdir}.tgrlib.o.${component} ${cflags}
	@echo ${component}: export complete

clean:
	${wipe} o.* ${wflags}
	${remove} ${component}
	@echo ${component}: cleaned

# Dynamic dependencies:
