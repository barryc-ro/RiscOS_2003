# File:    Makefile
# Purpose: Makefile for Common email library
# Author:  Ben Laughton
#
# History:
#
# 1998-08-26: BAL
# Created

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>
EXP_DIR		= <Lib$Dir>.Email.Common


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= $(MemCheckCC) cc
C++		= $(MemCheckCC) c++
#C++		= g++
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
WIPE		= -wipe

CCFLAGS		= -c -depend !Depend -ffah -wP\
		  -I$(INCLUDES) $(DFLAGS) $(THROWBACK) $(CCEXTRA)
C++FLAGS	= -c -depend !Depend -ffa -wP\
		  -I$(INCLUDES) $(DFLAGS) $(THROWBACK) $(C++EXTRA)
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~clfnr~v
LBFLAGS		= -c
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= C:,<Lib$Dir>.C++Lib,<Lib$Dir>


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= Common
TARGET		= Common
TARGETD		= Commond
TARGETZ		= Commonzm

OBJS =




# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .od .z .s .h .c .c++
.c.o:;		$(CC)  $(CCFLAGS)  -o $@ $<
.c.od:;		$(CC)  $(CCFLAGS)  -g -o $@ $<
.c.z:;		$(CC)  $(CCFLAGS)  -zm -zps1 -o $@ $<
.c++.o:;	$(C++) $(C++FLAGS) -o $@ $<
.c++.od:;	$(C++) $(C++FLAGS) +g -o $@ $<
.s.o:;		$(AS)  $(ASFLAGS)  -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:	$(TARGET)

debug:	$(TARGETD) export_hdrs
	$(CP)	$(TARGETD)	$(EXP_DIR).o.$(TARGETD)		$(CPFLAGS)

# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

#test:
#	echo $(OBJS) { > tf }
#	sed s/o./od./g tf { > tf2 }

export: export_$(PHASE)

#install_rom: $(TARGET)
#	$(CP) $(TARGET) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
#	echo $(COMPONENT): rom module installed

clean:
	@echo $(COMPONENT): cleaned

export_hdrs: dirs
	$(CP)	h.*		$(EXP_DIR).h.*			$(CPFLAGS)
	@echo $(COMPONENT): export complete (hdrs)		$(CPFLAGS)

#export_libs: $(TARGET) dirs
#	$(CP)	$(TARGET)	$(EXP_DIR).o.$(TARGET)		$(CPFLAGS)
#	@echo $(COMPONENT): export complete (libs)

dirs:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(EXP_DIR)
	$(MKDIR) $(EXP_DIR).h


# ------------------------------------------------------------------------------
# Final link
#

#$(TARGET): $(OBJS) $(LOCALOBJS) $(MBOXCBOBJS) $(POP3OBJS) $(SENDQOBJS) local_dirs
#	$(LB) $(LBFLAGS) -o $@ $(OBJS) $(LOCALOBJS) $(MBOXCBOBJS) $(POP3OBJS)\
#	                       $(SENDQOBJS)
#
#$(TARGETD): $(OBJSD) $(LOCALOBJSD) $(MBOXCBOBJSD) $(POP3OBJSD) $(SENDQOBJSD) local_dirs_d
#	$(LB) $(LBFLAGS) -o $@ $(OBJSD) $(LOCALOBJSD) $(MBOXCBOBJSD)\
#	                       $(POP3OBJSD) $(SENDQOBJSD)
#
#$(TARGETZ): $(OBJSZ) local_dirs
#	$(LB) $(LBFLAGS) -o $(TARGETZ) $(OBJSZ)
#
#$(EXP_HDR).$(COMPONENT): hdr.$(COMPONENT)
#	$(CP) hdr.$(COMPONENT) $@ $(CPFLAGS)


# ------------------------------------------------------------------------------
# Dynamic dependencies:
