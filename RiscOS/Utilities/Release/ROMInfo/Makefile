# Skeleton OBJASM/C Makefile for application
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 17 Sep 1997	RWB		Created
# 09 Dec 1997	RLT		Modified to build !ROMSplit


#
# Paths
#
EXP_HDR = <export$dir>
#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe
CD	= dir
SQ	= squeeze
CHMOD   = attr ${CHMODFLAGS}

AFLAGS = -depend !Depend -Stamp -quit -To $@ -From
CFLAGS  = -c -depend !Depend ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v
DFLAGS  = ${DEBUG}
CHMODFLAGS = +ow +or +wr -ww

#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
EVENTLIB   = Tbox:o.eventlib
TOOLBOXLIB = Tbox:o.toolboxlib
WIMPLIB	   = Tbox:o.wimplib
#
# Include files
#
INCLUDES = -ITbox: -IC:


# Program specific options:
#
COMPONENT = ROMSplit
TARGET    = !RunImage
APP	  = ${INSTDIR}.!${COMPONENT}
ROMSPLITCMD = Do ${TARGET} -command
INPUTFILE = -input_file Test.EtherI16
OUTPUTFILE = -output_file Test.Split
GNUDIFF = GNU.Diff
GNUDIFFFLAGS = -q -s -t -b
ORIG_ROM_0 = Test.ROM_0
ORIG_ROM_1 = Test.ROM_1
NEW_ROM_0 = Test.Split0
NEW_ROM_1 = Test.Split1


OBJS      = o.main	\
            o.messages


#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS}   ${AFLAGS} $< $@

#
# Build
#
all: ${TARGET}
	${SQ} ${TARGET}
        @echo ${COMPONENT}: all complete

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} o
        ${RM} ${TARGET}
        ${RM} ${NEW_ROM_0}
        ${RM} ${NEW_ROM_1}
        @echo ${COMPONENT}: cleaned

install: ${TARGET}
	${MKDIR} ${APP}
	${CP} ${TARGET} ${APP}.${TARGET} ${CPFLAGS}
	${CP} LocalRes:* ${APP}.* ${CPFLAGS}
        ${CHMOD} ${APP}
	@echo ${COMPONENT}: installed

test: 	${TARGET}
	@Echo
	@Echo This script should output two files, Split0 and Split1, that are
	@Echo  identical to ROM_0 and ROM_1 in the same directory.
	@Echo
	${ROMSPLITCMD} ${INPUTFILE} ${OUTPUTFILE}
	@Echo
	@Echo Attempting to diff the images using GNU.Diff in Library.
	${GNUDIFF} ${ORIG_ROM_0} ${NEW_ROM_0} ${GNUDIFFFLAGS}
	${GNUDIFF} ${ORIG_ROM_1} ${NEW_ROM_1} ${GNUDIFFFLAGS}
	@echo ${COMPONENT}: tests complete.

o.dirs:
	${MKDIR} o

#
# Target
#
${TARGET}: ${OBJS} o.dirs
        ${LD} -o $@ ${CLIB} ${EVENTLIB} ${TOOLBOXLIB} ${WIMPLIB} ${OBJS}
#
# Dynamic dependencies:
