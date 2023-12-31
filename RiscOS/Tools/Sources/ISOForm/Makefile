# File:    Makefile
# Purpose: Makefile for ISOForm
# Author:  Neil Bingham
#
# History:
#
# 09-Nov-99   NB         Created.
#





# ------------------------------------------------------------------------------
# Paths
#

LIBDIR		= <Lib$Dir>
LIBINSTDIR       = ${INSTDIR}.Library.Misc
INSTAPP         = ${INSTDIR}.Apps.!${COMPONENT}




# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= mkdir -p
AS		= objasm
CP		= copy
CC		= cc
RM		= remove
LD		= link
WIPE		= -wipe
SQZ		= squeeze

CCFLAGS		= -c -depend !Depend -ffah -strict\
		  -I$(INCLUDES) $(DFLAGS) $(THROWBACK) $(CCEXTRA)
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~clfnr~v
LBFLAGS		= -c
WFLAGS		= ~CF~V
LDFLAGS		= -c++
SQZFLAGS	= -f




# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
CLXLIB		= C:CLX.o.clxlib

LIBS =\
 $(CLIB) 	\




# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= C:




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= ISOForm
TARGET		= isoform

OBJS =\
 o.file			\
 o.main			\
 o.opts			\
 o.phase1		\
 o.phase23		\
 o.phsupp		\
 o.scan			\
 o.scsi			\
 o.store		\
 o.tofile		\
 o.toscsi		\

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .c
.c.o:;		$(CC)  $(CCFLAGS)  -o $@ $<




# ------------------------------------------------------------------------------
# Build rules
#

all:	$(TARGET)
        @echo $(COMPONENT): build complete

export: $(TARGET)
        @echo $(COMPONENT): export complete

clean:
	$(WIPE) o $(WFLAGS)
	$(RM) o
	$(RM) $(TARGET)
	@echo $(COMPONENT): cleaned

$(TARGET): $(OBJS) $(LIBS) local_dirs
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
	${SQZ} ${SQZFLAGS} $@

install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBINSTDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: tool and docs installed in library

install: install_tool install_dirs
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'

install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBINSTDIR}
	${MKDIR} ${LIBDIR}.Docs

local_dirs:
	$(MKDIR) o


# ------------------------------------------------------------------------------
# Dynamic dependencies:
