# File:    Makefile
# Purpose: Makefile for find
# Author:  Neil Bingham
#
# History:
#
# 09-Nov-99   NB         Created.
# 17-Nov-99   BAL        Builds actual find tool as well.  Gets versions and
#                          dates from VersionNum.





# ------------------------------------------------------------------------------
# Paths
#

LIBDIR		= <Lib$Dir>
LIBINSTDIR	= ${INSTDIR}.Library.Acorn
INSTAPP		= ${INSTDIR}.Apps.!Find




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
 $(CLXLIB)	\




# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= C:




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= find
TARGET		= find

OBJS =\
 o.find\




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
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	do perl Build:GetVersion Find$MajorVersion
	do sed s/VersionNumber/<Find$MajorVersion> LocalRes:Desc >${INSTAPP}.Desc
	@unset Find$MajorVersion
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'


install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBINSTDIR}
	${MKDIR} ${LIBDIR}.Docs

local_dirs:
	$(MKDIR) o


# ------------------------------------------------------------------------------
# Dynamic dependencies:
