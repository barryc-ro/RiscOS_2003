#CC = 		MemCheck.CC cc
CC =		cc
C++ =		c++
LD = 		Link
ASM =	 	ObjAsm
CMHG =	 	CMHG

CFLAGS =	-c -fna -depend !Depend -I${INCLUDES} -throwback
C++FLAGS = 	-c -depend !Depend -fna -Wp -I${INCLUDES} -throwback
LDFLAGS	= 	-aif -c++
ASMFLAGS =	-throwback -NoCache -depend !Depend
CMHGFLAGS = 	-depend !Depend -throwback -IC:

INCLUDES =	C:

TARGET = 	!RunImage

OBJECTS	=\
 o.error\
 o.main

LIBS =\
 <Lib$Dir>.tboxlibs.o.toolboxlib\
 <Lib$Dir>.tboxlibs.o.eventlib\
 <Lib$Dir>.tboxlibs.o.wimplib\
 C:o.Stubs\

 
.SUFFIXES:	.o .s .c
.s.o:;		$(ASM) $(ASMFLAGS) -o $@ $<
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.c++.o:;	$(C++) $(C++FLAGS) -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LIBS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

# Dynamic dependencies:
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	C:tboxlibs.h.event
o.main:	C:tboxlibs.h.wimp
o.main:	C:tboxlibs.h.toolbox
o.main:	C:tboxlibs.h.quit
o.main:	C:tboxlibs.h.window
o.main:	C:tboxlibs.h.gadgets
o.main:	C:tboxlibs.h.toolbox
o.main:	C:tboxlibs.h.wimp
o.main:	C:tboxlibs.h.wimplib
o.main:	C:h.dialler
o.main:	h.error
o.main:	C:h.kernel
o.main:	h.global
o.main:	C:tboxlibs.h.toolbox
