# Makefile for !Make
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 01-Mar-01  SNB          Created
#

#
# Paths
#
COMPONENT = make
TARGET    = make

CFLAGS    = -Wpc -ffah -IC:
DIRS      = o._dirs

INSTAPP   = ${INSTDIR}.Apps.!Make

#
# Program specific options:
#
OBJS       = slist.o dlist.o utils.o MKinfo.o MKmem.o MKtxtwin.o MKtask.o \
             MKoutput.o MKmenus.o MKinit.o MKmain.o MKbkg.o MKiconbar.o \
             MKproject.o MKsources.o MKtargets.o MKtools.o MKoptions.o \
             MKcmdline.o MKmakefile.o MKfname.o MKexit.o MKmembers.o
LIBS       = ${RLIB} ${CLIB}

include Makefiles:StdTools
include Makefiles:AppLibs
include Makefiles:AppStdRule

#
# build a the library:
#
all:    ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed
        
install: ${TARGET}
	${MKDIR} ${INSTAPP}.choices.toolflags
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CP} ${TARGET} ${INSTDIR}.!RunImage ${CPFLAGS}
	@echo ${COMPONENT}: !${COMPONENT} app install in '${INSTAPP}'

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: 
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: 
        @echo ${COMPONENT}: export complete (libs)

${DIRS}:
	${MKDIR} o
	${TOUCH} $@

#
# Final link
#
${TARGET}: ${OBJS} ${LIBS} ${DIRS}
        ${LD} ${LDFLAGS} -o ${COMPONENT} ${OBJS} ${LIBS}
        ${SQZ} ${SQZFLAGS} $@
        ${CHMOD} a+rx $@

# Dynamic dependencies:
