# File:    Makefile
# Purpose: Makefile for C++ command
# Author:  Ben Laughton
#
# History:
#
# 1998-08-23: BAL
# Created




# ------------------------------------------------------------------------------
# Paths
#

LIBDIR		= <Lib$Dir>
EXP_DIR		= <Build$Dir>.Library




# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
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

local_dirs:
	$(MKDIR) o


# ------------------------------------------------------------------------------
# Dynamic dependencies:
