# Makefile for eventlib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-09-18	BAL		Created
#


# ------------------------------------------------------------------------------
# Paths
#

LIBDIR		= <Lib$Dir>


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
CP		= copy
RM		= remove
WIPE		= -wipe

CPFLAGS		= ~CLFNR~V
WPFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= toolboxlib
TARGET		= o.toolboxlib


# ------------------------------------------------------------------------------
# Build rules
#

all:	$(TARGET)
	@echo $(COMPONENT): library built

export: export_$(PHASE)

export_hdrs: dirs
	$(CP)	h.*		$(LIBDIR).tboxlibs.h.*		$(CPFLAGS)
	@echo $(COMPONENT): export complete (hdrs)

export_libs: $(TARGET) dirs
	$(CP)	$(TARGET)	$(LIBDIR).tboxlibs.$(TARGET)	$(CPFLAGS)
	@echo $(COMPONENT): export complete (libs)

clean:
	every.every2 -ci	@.sources.*.o wipe %0.*		$(WPFLAGS)
	$(RM) $(TARGET)
	@echo $(COMPONENT): cleaned

local_dirs:
	$(MKDIR) o

dirs:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(LIBDIR).tboxlibs
	$(MKDIR) $(LIBDIR).tboxlibs.h
	$(MKDIR) $(LIBDIR).tboxlibs.o
        
$(TARGET): local_dirs
	Obey	@.!Mk

# Dynamic dependencies:
Sources.ActionButt.o.getclicksh:	Sources.ActionButt.c.getclicksh
Sources.ActionButt.o.getclicksh:	C:h.kernel
Sources.ActionButt.o.getclicksh:	@.h.toolbox
Sources.ActionButt.o.getclicksh:	h.wimp
Sources.ActionButt.o.getclicksh:	@.h.gadgets
