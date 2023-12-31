# Makefile for NCMail TestUI
# ==========================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1998-09-16    AW      Created
# 1998-09-22	RDW	Modified to fit into build structure
# 1998-10-20	RDW	RAM module target added
# 1998-11-02	RDW	Change over to Ben's cunning 2 level makefile

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H	= <CExport$Dir>.h
EXP_C_O	= <CExport$Dir>.o
LIBDIR	= <Lib$Dir>


# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = NCMailUI
RES_COMPONENT   = NCMail
NORMAL_TARGET	= $(COMPONENT)
MEMCHECK_TARGET	= $(COMPONENT)MC
HIERPROF_TARGET	= $(COMPONENT)HP
MODULE_TARGET	= $(COMPONENT)zm
DEBUG_TARGET	= $(COMPONENT)d


# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = $(RESDIR).$(RES_COMPONENT)
RESAPP          := <Resource$Dir>.Apps.$(APP)


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR           = do mkdir -p
AS              = objasm
CC              = $(MemCheckCC) cc
C++             = $(MemCheckCC) c++
CMHG            = cmhg
CP              = copy
LD              = link
LB		= libfile
RM              = remove
SQUEEZE         = squeeze
WIPE            = -wipe


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DDFLAGS         = $(DFLAGS) -fn
DDZFLAGS        = $(DZFLAGS) -fn


# -----------------------------------------------------------------------------
# Build rules
#

all: 
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=o
	@echo $(COMPONENT): build complete

debug: 
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(DEBUG_TARGET) EXT=od
	@echo $(COMPONENT): debug build complete

preprocess:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=i
	@echo $(COMPONENT): preprocessing complete

export: export_$(PHASE)

export_hdrs:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=AvoidAMUBug
	@echo $(COMPONENT): export complete (hdrs)

export_libs:
	@echo $(COMPONENT): export complete (libs)
	
export_libsMC:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(MEMCHECK_TARGET) EXT=oMC
	@echo $(COMPONENT): export complete (MemCheck libs)
	
export_libsHP:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(HIERPROF_TARGET) EXT=oHP
	@echo $(COMPONENT): export complete (HierProf libs)

rom: 
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(MODULE_TARGET) EXT=oz
        @echo $(COMPONENT): Module built (ROM)

ram: 
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(MODULE_TARGET) EXT=ozsl
	@echo $(COMPONENT): Module built (RAM)

install_rom: 
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(MODULE_TARGET) EXT=oz

install: 
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=o
        @echo
        @echo $(COMPONENT): $(TARGET) installed

installd:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(DEBUG_TARGET) EXT=od
        @echo
        @echo $(COMPONENT): $(TARGET) (Debug) installed

installMC:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(MEMCHECK_TARGET) EXT=oMC
        @echo
        @echo $(COMPONENT): $(TARGET) (MemCheck) installed

installHP:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(HIERPROF_TARGET) EXT=oHP
        @echo
        @echo $(COMPONENT): $(TARGET) (HierProf) installed

resources:
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=AvoidAMUBug
        @echo $(COMPONENT): Resource files copied to Messages module

clean: clean_common
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=o
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=oMC
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=oHP
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=od
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=oz
	@echo $(COMPONENT): cleaned

clean_common:
	@$(RM) $(NORMAL_TARGET)
	@$(RM) $(MEMCHECK_TARGET)
	@$(RM) $(HIERPROF_TARGET)
	@$(RM) $(MODULE_TARGET)
	@$(RM) $(DEBUG_TARGET)


# -----------------------------------------------------------------------------
# Development rules
#

clean_all: clean_common
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=o
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=oMC
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=oHP
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=od
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=AvoidAMUBug EXT=i
	@amu -f MainMake $(MFLAGS) $(MAKECMDGOALS) TARGET=$(NORMAL_TARGET) EXT=oz
        @echo $(COMPONENT): Cleaned all
