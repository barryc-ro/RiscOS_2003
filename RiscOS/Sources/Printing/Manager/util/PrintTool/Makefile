# Project:   !PrintTool
# Whose sole purpose in life is to allow palettes to be tuned
# to an acceptable level.

#
# Paths
#
EXP_HDR = <export$dir>

# Libraries
#
CLIB       = CLIB:o.stubs
ABCLIB     = CLIB:o.ABCLib
CPPLIB	   = CppLib:o.c++lib
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
TOOLBOXLIB = TBox:o.toolboxlib
EVENTLIB   = TBox:o.eventlib
WIMPLIB	   = TBox:o.wimplib

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
C++	= c++
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir

# Debug flags

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DEBUG} -Throwback
C++FLAGS  = -c -depend !Depend ${INCLUDES} ${DEBUG} -Throwback
Linkflags = -aif -c++ -o $@
ObjAsmflags = -throwback -NoCache -depend !Depend
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
#
# Include files
#
INCLUDES = -IC:

COMPONENT = Printtool
TARGET    = !RunImage

OBJS      =	\
main.o		\
calib.o		\
display.o	\
drawfile.o	\
tboxclass.o	\
walkth.o	\
error.o		\
palchoose.o	\
prtablegen.o

#
# Rule patterns
#
.c++.o:;    ${C++} ${CFLAGS} -o $@ $<
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET} dirs
        @echo ${COMPONENT}: all complete
dirs:
        ${MKDIR} o

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} ${TARGET} ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Final targets:
${TARGET}: ${OBJS} ${EVENTLIB} ${ABCLIB} ${WIMPLIB}
        ${LD} ${Linkflags} ${OBJS} ${COBJ} ${CLIB} ${CPPLIB} ${EVENTLIB} ${ABCLIB} ${WIMPLIB}


# Dynamic dependencies:
o.main:	c++.main
o.main:	h.main
o.main:	C:h.swis
o.main:	C:h.kernel
o.main:	C:h.kernel
o.main:	h.error
o.main:	h.main
o.main:	C:h.trace
o.main:	h.tboxclass
o.main:	C:h.toolbox
o.main:	C:h.task
o.main:	C:h.wimp
o.main:	C:h.window
o.main:	C:h.gadgets
o.main:	C:h.quit
o.main:	C:h.menu
o.main:	h.palchoose
o.main:	h.main
o.main:	h.error
o.main:	C:h.kernel
o.main:	C:h.gadgets
o.main:	C:h.window
o.main:	h.walkth
o.main:	C:h.kernel
o.main:	C:h.wimp
o.main:	C:h.window
o.main:	h.main
o.main:	C:h.menu
o.main:	C:h.toolbox
o.main:	C:h.window
o.main:	C:h.quit
o.main:	C:h.menu
o.calib:	c++.calib
o.calib:	h.palgen.udpalette
o.calib:	h.display
o.calib:	h.main
o.calib:	C:h.swis
o.calib:	C:h.kernel
o.calib:	C:h.kernel
o.calib:	h.error
o.calib:	h.main
o.calib:	C:h.trace
o.calib:	C:h.gadgets
o.calib:	C:h.toolbox
o.calib:	C:h.task
o.calib:	C:h.wimp
o.calib:	C:h.window
o.calib:	C:h.gadgets
o.calib:	h.palgen.prtablegen
o.calib:	h.palgen.udpalette
o.display:	c++.display
o.display:	h.display
o.display:	h.main
o.display:	C:h.swis
o.display:	C:h.kernel
o.display:	C:h.kernel
o.display:	h.error
o.display:	h.main
o.display:	C:h.trace
o.display:	C:h.gadgets
o.display:	C:h.toolbox
o.display:	C:h.task
o.display:	C:h.wimp
o.display:	C:h.window
o.display:	C:h.gadgets
o.display:	h.palgen.prtablegen
o.display:	h.palgen.udpalette
o.display:	C:h.kernel
o.display:	h.walkth
o.display:	C:h.kernel
o.display:	C:h.wimp
o.display:	C:h.window
o.display:	h.main
o.display:	C:h.menu
o.drawfile:	c++.drawfile
o.drawfile:	C:h.kernel
o.drawfile:	C:h.drawfile
o.drawfile:	C:h.wimp
o.drawfile:	h.walkth
o.drawfile:	C:h.kernel
o.drawfile:	C:h.wimp
o.drawfile:	C:h.window
o.drawfile:	C:h.toolbox
o.drawfile:	C:h.task
o.drawfile:	C:h.gadgets
o.drawfile:	h.main
o.drawfile:	C:h.swis
o.drawfile:	C:h.kernel
o.drawfile:	h.error
o.drawfile:	h.main
o.drawfile:	C:h.trace
o.drawfile:	C:h.menu
o.drawfile:	h.tboxclass
o.drawfile:	C:h.toolbox
o.drawfile:	C:h.window
o.drawfile:	C:h.quit
o.drawfile:	C:h.menu
o.drawfile:	h.palchoose
o.drawfile:	h.main
o.drawfile:	h.error
o.drawfile:	C:h.kernel
o.drawfile:	C:h.gadgets
o.drawfile:	C:h.window
o.drawfile:	h.walkth
o.tboxclass:	c++.tboxclass
o.tboxclass:	h.main
o.tboxclass:	C:h.swis
o.tboxclass:	C:h.kernel
o.tboxclass:	C:h.kernel
o.tboxclass:	h.error
o.tboxclass:	h.main
o.tboxclass:	C:h.trace
o.tboxclass:	h.tboxclass
o.tboxclass:	C:h.toolbox
o.tboxclass:	C:h.task
o.tboxclass:	C:h.wimp
o.tboxclass:	C:h.window
o.tboxclass:	C:h.gadgets
o.tboxclass:	C:h.quit
o.tboxclass:	C:h.menu
o.tboxclass:	h.palchoose
o.tboxclass:	h.main
o.tboxclass:	h.error
o.tboxclass:	C:h.kernel
o.tboxclass:	C:h.gadgets
o.tboxclass:	C:h.window
o.tboxclass:	h.walkth
o.tboxclass:	C:h.kernel
o.tboxclass:	C:h.wimp
o.tboxclass:	C:h.window
o.tboxclass:	h.main
o.tboxclass:	C:h.menu
o.tboxclass:	C:h.menu
o.tboxclass:	C:h.toolbox
o.walkth:	c++.walkth
o.walkth:	h.walkth
o.walkth:	C:h.kernel
o.walkth:	C:h.wimp
o.walkth:	C:h.window
o.walkth:	C:h.toolbox
o.walkth:	C:h.task
o.walkth:	C:h.gadgets
o.walkth:	h.main
o.walkth:	C:h.swis
o.walkth:	C:h.kernel
o.walkth:	h.error
o.walkth:	h.main
o.walkth:	C:h.trace
o.walkth:	C:h.menu
o.walkth:	h.error
o.walkth:	h.main
o.walkth:	C:h.kernel
o.error:	c.error
o.error:	C:h.kernel
o.error:	h.error
o.error:	h.main
o.error:	C:h.swis
o.error:	C:h.kernel
o.error:	h.error
o.error:	C:h.trace
o.error:	C:h.swis
o.palchoose:	c++.palchoose
o.palchoose:	h.palchoose
o.palchoose:	h.main
o.palchoose:	C:h.swis
o.palchoose:	C:h.kernel
o.palchoose:	C:h.kernel
o.palchoose:	h.error
o.palchoose:	h.main
o.palchoose:	C:h.trace
o.palchoose:	h.error
o.palchoose:	C:h.kernel
o.palchoose:	C:h.gadgets
o.palchoose:	C:h.toolbox
o.palchoose:	C:h.task
o.palchoose:	C:h.wimp
o.palchoose:	C:h.window
o.palchoose:	C:h.gadgets
o.palchoose:	h.display
o.palchoose:	h.main
o.palchoose:	C:h.gadgets
o.palchoose:	C:h.window
o.palchoose:	h.palgen.prtablegen
o.palchoose:	h.palgen.udpalette
o.prtablegen:	c.prtablegen
o.prtablegen:	h.palgen.prtablegen
