# Translate Makefile
#
# Neil Bingham (99-02-17).
#
# 99-02-17	Created.
#
TARGET 		= Target.Translate

CC 		= cc
LD 		= Link
ASM 		= ObjAsm
CMHG	 	= CMHG
LB  		= LibFile
RM  		= Remove
WIPE 		= -Wipe
MKDIR 		= cdir

LIBDIR 		= <Lib$Dir>
LIBRARYDIR 	= <Build$Dir>.Library
CFLAGS 		= -c -depend !Depend -IC: -fah $(THROWBACK)
CPPFLAGS 	= -c -depend !Depend -IC: $(THROWBACK)
LDFLAGS		= -aif -c++
ASMFLAGS 	= -NoCache -depend !Depend
CMHGFLAGS 	= -depend !Depend $(THROWBACK)
WIPEFLAGS 	= ~CFR~V

#Libraries
CLIB		= CLib:o.stubs
C++LIB          = $(LIBDIR).C++Lib.o.c++lib
EVENTLIB	= $(LIBDIR).tboxlibs.o.eventlib
TOOLBOXLIB	= $(LIBDIR).tboxlibs.o.toolboxlib
WIMPLIB		= $(LIBDIR).tboxlibs.o.wimplib

EMAILCOMMONLIB	= $(LIBDIR).Email.Common.o.Common
SOCKLIB		= $(LIBDIR).TCPIPLibs.o.socklib5
DEBUGLIB	= $(LIBDIR).DebugLib.o.debuglib
INETLIB		= $(LIBDIR).TCPIPLibs.o.inetlib
UNIXLIB		= $(LIBDIR).TCPIPLibs.o.unixlib
NBLIB	        = $(LIBDIR).NBLib.o.nblib
ERRORLIB	= $(LIBDIR).ErrorLib.o.ErrorLib

DFLAGS 		= $(EXTRADFLAGS)


OBJS = 		\
 o.main 	\
 o.translate 	\
 o.htmlescape	\

LIBS = 		\
 C:o.stubs	\
 $(EXTRALIBS)	\
 $(NBLIB)	\

.SUFFIXES:	.o .c
.c.o:;		$(CC) $(CFLAGS) $(DFLAGS) -o $@ $<

all:    $(TARGET)

clean:
	$(RM)		$(TARGET)
	$(WIPE)	o	$(WIPEFLAGS)
	$(WIPE)	Target	$(WIPEFLAGS)

o.dirs:
	$(MKDIR) o
	$(MKDIR) Target
	create o.dirs
	stamp  o.dirs

$(TARGET): $(OBJS) $(LIBS) o.dirs
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

# Dynamic dependencies:
