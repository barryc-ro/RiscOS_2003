
INCLUDES = -I$(top_srcdir) @PROJECTINCLUDES@

lib_LTLIBRARIES = libdebuglib.la

# First, the standard components of DebugLib
libdebuglib_la_SOURCES = \
        include.h globals.h parse.c parse.h misc.c misc.h debug.c debug.h \
        options.c options.h buffering.c buffering.h \
        \
        brainlink.c brainlink.h \
        tml.c tml.h \
        tracker.c tracker.h \
        debugit.c debugit.h \
        file.c file.h \
        printf.c printf.h \
        serial.c serial.h serial_lib.h serial_lib.c \
        pdebug.c pdebug.h \
        dadebug.c dadebug.h \
        socket.c socket.h \
        dbgsyslog.c dbgsyslog.h \
        myprintf.c myprintf.h \
        ptvlog.c ptvlog.h \
        remotedb.c remotedb.h
                         
# Extra linker flags.
libdebuglib_la_LDFLAGS =

MAINTAINERCLEANFILES = Makefile.in
CLEANFILES           = 
EXTRA_DIST           = 
