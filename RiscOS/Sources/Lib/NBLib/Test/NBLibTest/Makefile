CC =		cc
LD = 		Link
ASM =	 	ObjAsm
CMHG =	 	CMHG
LB =		LibFile
RM =		Remove
WIPE =		-Wipe
MKDIR = 	cdir

LIBDIR =	<Lib$Dir>
CFLAGS =	-c -depend !Depend -IC: -DDEBUGLIB -fah -strict -throwback
CPPFLAGS = 	-c -depend !Depend -IC: -throwback
LDFLAGS	= 	-aif -c++
ASMFLAGS =	-throwback -NoCache -depend !Depend
CMHGFLAGS = 	-depend !Depend -throwback -IC:
WIPEFLAGS =	~C~V

DEBUGLIB =	${LIBDIR}.Debuglib.o.debuglib
NBLIB =		${LIBDIR}.NBLib.o.NBLib

TARGET = 	NBLibTest

OBJECTS	= \
 o.main \
 o.dirstst \
 o.files \
 o.internal \
 o.misc \
 o.stringtst \

LIBS = \
 C:o.stubs \
 ${NBLIB} \

.SUFFIXES:	.o .c
.c.o:;		${CC} ${CFLAGS} -o $@ $<

all:    ${TARGET}

clean:
	${RM}		${TARGET}
	${WIPE}	o	${WIPEFLAGS}


o.dirs:
	${MKDIR} o
	create o.dirs
	stamp  o.dirs

${TARGET}: ${OBJECTS} ${LIBS} o.dirs
	${LD} ${LDFLAGS} -o $@ ${OBJECTS} ${LIBS}
	
# Dynamic dependencies:
o.main:	c.main
o.main:	h.Main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	tbox:h.wimp
o.main:	tbox:h.wimplib
o.main:	tbox:h.toolbox
o.main:	C:NBLib.h.NBLib
o.main:	C:tboxlibs.h.Toolbox
o.main:	h.Files
o.main:	h.DirsTst
o.main:	h.Misc
o.main:	h.StringTst
o.dirstst:	c.dirstst
o.dirstst:	h.DirsTst
o.dirstst:	h.Main
o.dirstst:	C:h.kernel
o.dirstst:	C:h.swis
o.dirstst:	tbox:h.wimp
o.dirstst:	tbox:h.wimplib
o.dirstst:	tbox:h.toolbox
o.dirstst:	C:NBLib.h.NBLib
o.dirstst:	C:tboxlibs.h.Toolbox
o.dirstst:	h.Internal
o.files:	c.files
o.files:	h.Files
o.files:	h.Main
o.files:	C:h.kernel
o.files:	C:h.swis
o.files:	tbox:h.wimp
o.files:	tbox:h.wimplib
o.files:	tbox:h.toolbox
o.files:	C:NBLib.h.NBLib
o.files:	C:tboxlibs.h.Toolbox
o.files:	h.Internal
o.internal:	c.internal
o.internal:	h.Main
o.internal:	C:h.kernel
o.internal:	C:h.swis
o.internal:	tbox:h.wimp
o.internal:	tbox:h.wimplib
o.internal:	tbox:h.toolbox
o.internal:	C:NBLib.h.NBLib
o.internal:	C:tboxlibs.h.Toolbox
o.internal:	h.Internal
o.misc:	c.misc
o.misc:	h.Misc
o.misc:	h.Main
o.misc:	C:h.kernel
o.misc:	C:h.swis
o.misc:	tbox:h.wimp
o.misc:	tbox:h.wimplib
o.misc:	tbox:h.toolbox
o.misc:	C:NBLib.h.NBLib
o.misc:	C:tboxlibs.h.Toolbox
o.misc:	h.Internal
o.stringtst:	c.stringtst
o.stringtst:	h.StringTst
o.stringtst:	h.Main
o.stringtst:	C:h.kernel
o.stringtst:	C:h.swis
o.stringtst:	tbox:h.wimp
o.stringtst:	tbox:h.wimplib
o.stringtst:	tbox:h.toolbox
o.stringtst:	C:NBLib.h.NBLib
o.stringtst:	C:tboxlibs.h.Toolbox
o.stringtst:	h.Internal
