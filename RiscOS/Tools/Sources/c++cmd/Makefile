# File:    Makefile
# Purpose: Makefile for C++ command
# Author:  Ben Laughton
#
# History:
#
# 1998-08-23: BAL	 Created
# 09-Nov-99   NB         Added install phase for FrontEnd res.
#





# ------------------------------------------------------------------------------
# Paths
#

LIBDIR		= <Lib$Dir>
EXP_DIR		= <Build$Dir>.Library
LIBDIR	= ${INSTDIR}.Library.Acorn
INSTAPP    = ${INSTDIR}.Apps.!C++




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

LIBS =\
 $(CLIB)




# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= C:




# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= C++
TARGET		= c++
#EXPORTS		= $(EXP_HDR).$(COMPONENT)

OBJS =\
 o.args\
 o.cplusplus\




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
	$(CP)	$(TARGET)	$(EXP_DIR).$(TARGET)		$(CPFLAGS)
        @echo $(COMPONENT): export complete

clean:
	$(WIPE) o $(WFLAGS)
	$(RM) $(TARGET)
	@echo $(COMPONENT): cleaned

$(TARGET): $(OBJS) $(LIBS) local_dirs
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
	${SQZ} ${SQZFLAGS} $@

install_tool: ${TARGET}
	${CP} ${TARGET} ${LIBDIR}.${TARGET} ${CPFLAGS}
	@echo ${COMPONENT}: tool and docs installed in library

install: install_tool install_dirs
	${CP} LocalRes:!Boot ${INSTAPP}.!Boot ${CPFLAGS}
	${CP} LocalRes:!Run ${INSTAPP}.!Run ${CPFLAGS}
	${CP} LocalRes:!Help ${INSTAPP}.!Help ${CPFLAGS}
	${CP} LocalRes:!Sprites ${INSTAPP}.!Sprites ${CPFLAGS}
	${CP} LocalRes:!Sprites22 ${INSTAPP}.!Sprites22 ${CPFLAGS}
	${CP} LocalRes:Messages ${INSTAPP}.Messages ${CPFLAGS}
	${CP} LocalRes:Templates ${INSTAPP}.Templates ${CPFLAGS}
	${CC} -C++ -E >${INSTAPP}.Desc LocalRes:Desc
	@echo ${COMPONENT}: FrontEnd app install in '${INSTAPP}'

install_dirs:
	${MKDIR} ${INSTAPP}
	${MKDIR} ${INSTAPP}
	${MKDIR} ${LIBDIR}.Docs

local_dirs:
	$(MKDIR) o


# ------------------------------------------------------------------------------
# Dynamic dependencies:
