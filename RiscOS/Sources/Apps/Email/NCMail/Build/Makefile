# Makefile for NCMail Engine
# =========================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1998-09-22    RDW     Created
# 1998-09-28	RDW	LIBS completed for module build
# 1998-10-08	RDW	NETLINKLIB added

# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H	= <CExport$Dir>.h
EXP_C_O	= <CExport$Dir>.o
LIBDIR	= <Lib$Dir>


# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = NCMail
APP             = !$(COMPONENT)
MODULE          = rm.$(APP)
ROM_MODULE      = aof.$(COMPONENT)
LPATH           = LocalUserIFRes:
APPENGINE       = Engine

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = $(RESDIR).$(COMPONENT)
RESAPP          = <Resource$Dir>.Apps.$(APP)


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR           = cdir
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

AFLAGS          = $(THROWBACK) -depend !Depend -nocache -stamp -quit
CFLAGS          = $(THROWBACK) -depend !Depend -ffa $(INCLUDES) -wp
C++FLAGS        = $(THROWBACK) -depend !Depend -ffa $(INCLUDES) -wp
LDFLAGS		= -c++
CPFLAGS         = ~cfr~v~n
SQFLAGS         = -f
WFLAGS          = ~cf~vr
LBFLAGS		= -c


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DDFLAGS         = $(DFLAGS) -fn
DDZFLAGS        = $(DZFLAGS) -fn


# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap

C++LIB          = $(LIBDIR).C++Lib.o.c++lib

EVENTLIB	= $(LIBDIR).tboxlibs.o.eventlib
TOOLBOXLIB	= $(LIBDIR).tboxlibs.o.toolboxlib
WIMPLIB		= $(LIBDIR).tboxlibs.o.wimplib

ACORNPOP3	= $(EXP_C_O).AcornPOP3
ACORNSMTP	= $(EXP_C_O).AcornSMTP
ELIB		= $(LIBDIR).Email.EmailLib.o.EmailLib
MBOXLIB		= $(LIBDIR).Email.Mailbox.o.Mailbox
NETLINKLIB	= $(LIBDIR).Email.NetLink.o.NetLink
SOCKLIB		= $(LIBDIR).TCPIPLibs.o.socklib5
DEBUGLIB	= $(LIBDIR).DebugLib.o.debuglib
INETLIB		= $(LIBDIR).TCPIPLibs.o.inetlib
UNIXLIB		= $(LIBDIR).TCPIPLibs.o.unixlib
NBLIB	        = $(LIBDIR).NBLib.o.nblib
ERRORLIB	= $(LIBDIR).ErrorLib.o.ErrorLib

LIBS = \
 $(SOCKLIB)\
 $(DEBUGLIB)\
 $(INETLIB)\
 $(UNIXLIB)\
 $(NBLIB)\
 $(ERRORLIB)\
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(MBOXLIB)\
 $(NETLINKLIB)\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)\
 $(EXTRALIBS)\
 $(C++LIB)\
 $(CLIB)\

#these are not complete
LIBSM = \
 $(NBLIB)zm\
 $(ERRORLIB)zm\
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)m\
 $(MBOXLIB)zm\
 $(NETLINKLIB)zm\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)zm\
 $(EXTRALIBS)\
 $(C++LIB)\


LIBSD = \
 $(SOCKLIB)\
 $(DEBUGLIB)\
 $(INETLIB)\
 $(UNIXLIB)\
 $(NBLIB)\
 $(ERRORLIB)\
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(MBOXLIB)d\
 $(NETLINKLIB)\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)\
 $(EXTRALIBS)\
 $(C++LIB)\
 $(CLIB)\


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES        = -Itbox:,C:,<Lib$Dir>

# Include these in the ROM module
#RESFILES = \
# $(LDIR).!Sprites22 \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS =  o.NCMailUI o.Engine

OBJSD = od.NCMailUI od.Engine

OBJSZ=  oz.NCMailUI oz.Engine


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c .c++

.c.o:;          @echo
                @echo Compiling $<
                @echo $(DFLAGS)
                @$(CC) $(CFLAGS) $(DFLAGS)        -c -o $@ $<

.c.oz:;         @echo
                @echo Compiling $<
                @echo $(DZFLAGS)
                @$(CC) $(CFLAGS) $(DZFLAGS)       -c -zM -DROM -o $@ $<

.c.od:;         @echo
                @echo Compiling $<
                @echo $(DDFLAGS)
                @$(CC) $(CFLAGS) $(DDFLAGS)       -c -g -o $@ $<

.c.odz:;        @echo
                @echo Compiling $<
                @echo $(DDZFLAGS)
                @$(CC) $(CFLAGS) $(DDZFLAGS)      -c -zM -DROM -o $@ $<

.c++.o:;        @echo
                @echo Compiling $<
                @echo $(DFLAGS)
                @$(C++) $(C++FLAGS) $(DFLAGS)     -c -o $@ $<

.c++.oz:;       @echo
                @echo Compiling $<
                @echo $(DZFLAGS)
                @$(C++) $(C++FLAGS) $(DZFLAGS)    -c -zM -DROM -o $@ $<

.c++.od:;       @echo
                @echo Compiling $<
                @echo $(DDFLAGS)
                @$(C++) $(C++FLAGS) $(DDFLAGS)    -c +g -o $@ $<

.c++.odz:;      @echo
                @echo Compiling $<
                @echo $(DDZFLAGS)
                @$(C++) $(C++FLAGS) $(DDZFLAGS)   -c -zM -DROM -o $@ $<

.s.o:;          @echo
                @echo Assembling $<
                @echo $(AFLAGS)
                @$(AS) $(AFLAGS) $< $@

.s.oz:;         @echo
                @echo Assembling $<
                @echo $(AFLAGS)
                @$(AS) $(AFLAGS) $< $@

.s.od:;         @echo
                @echo Assembling $<
                @echo $(AFLAGS)
                @$(AS) $(AFLAGS) -g $< $@

.s.odz:;        @echo
                @echo Assembling $<
                @echo $(AFLAGS)
                @$(AS) $(AFLAGS) -g $< $@


# -----------------------------------------------------------------------------
# Main rules
#

export: export_$(PHASE) $(UI) $(APPENGINE) localdirs

export_hdrs: 
	@echo $(COMPONENT): export complete (hdrs)		$(CPFLAGS)

export_libs: 
	@echo $(COMPONENT): export complete (libs)

all:	$(UI) $(APPENGINE)
        @echo $(COMPONENT): Application built (Disc)

rom: $(ROM_MODULE) $(UI) $(APPENGINE)
        @echo $(COMPONENT): Module built (ROM)

install_common: $(UI) $(APPENGINE) o.dirs
	@echo

install: $(TARGET) install_common
        @echo
        $(CP) $(TARGET)                         $(INSTDIR).!RunImage            $(CPFLAGS)
        @echo $(COMPONENT): Application installed to $(INSTDIR)

installd: $(TARGET) install_common
        @echo
        $(CP) $(TARGET)                         $(INSTDIR).!RunImage            $(CPFLAGS)
        @echo $(COMPONENT): Debug application installed $(INSTDIR)

install_rom: $(ROM_MODULE)
        $(CP) $(ROM_MODULE) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Module installed (ROM)

resources: 	$(UI) $(APPENGINE) o.dirs
	@echo $(RESDIR)
        $(MKDIR) $(RESDIR)
        $(MKDIR) $(RESAPP)
	
        @echo
        @echo $(COMPONENT): Resource files copied to Messages module

clean:  $(APPENGINE) cleanWebMail cleanTestUI
        @echo starting
        $(WIPE) abs             $(WFLAGS)
        $(WIPE) aof             $(WFLAGS)
        $(WIPE) linked          $(WFLAGS)
        $(WIPE) map             $(WFLAGS)
        $(WIPE) o               $(WFLAGS)
        $(WIPE) od              $(WFLAGS)
        $(WIPE) oz              $(WFLAGS)
        $(WIPE) odz             $(WFLAGS)
        $(WIPE) rm              $(WFLAGS)
        $(WIPE) syms            $(WFLAGS)
        $(RM)   s.ModuleWrap
	$(RM)   syms
        $(RM)   o.dirs
        @echo
        @echo $(COMPONENT): Cleaned

o.dirs:
        @$(MKDIR) abs
        @$(MKDIR) aof
        @$(MKDIR) linked
        @$(MKDIR) map
        @$(MKDIR) o
        @$(MKDIR) od
        @$(MKDIR) oz
        @$(MKDIR) odz
        @$(MKDIR) rm
        @$(MKDIR) syms
        @$(MKDIR) Targets
        @$(MKDIR) Targets.$(APP)
        create o.dirs
        stamp  o.dirs

localdirs:
	@$(MKDIR) h

# -----------------------------------------------------------------------------
# Development rules
#

linkmap: $(OBJS) $(LIBS)
        $(LD) -map -o null:x $(OBJS) $(LIBS) > map.linked

map:
        $(LD) -map -bin -base 0 -o null: $(OBJS) $(LIBS) > map.base0

clean_all: clean $(APPENGINE) $(UI)
        $(WIPE) Targets         $(WFLAGS)
        @echo $(COMPONENT): Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#

rom_link:
        $(LD) -o linked.$(COMPONENT) -rmf -base $(ADDRESS) \
                $(ROM_MODULE) $(ABSSYM) -map > map.$(COMPONENT)
        $(CP) linked.$(COMPONENT) $(LINKDIR).$(COMPONENT) $(CPFLAGS)
        @echo $(COMPONENT): rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName $(RESFILES)
        $(AS) $(AFLAGS) s.ModuleWrap $@

s.ModuleWrap: $(WRAPPER)
        $(CP) $(WRAPPER) $@ $(CPFLAGS)


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: 
        @echo
        $(LD) $(LDFLAGS) -s syms.!RI -o $@ $(OBJS) $(LIBS)
        $(SQUEEZE) $(SQFLAGS) $@

abs.!RI_D: 
        @echo
        $(LD) $(LDFLAGS) -debug -s syms.!RI_D -o $@ $(OBJSD) $(LIBSD)

$(MODULE): oz.ModuleWrap $(LIBSM) $(CLIB) o.dirs
        @echo
        $(LD) -s syms.RAMMod -o $@ -module oz.ModuleWrap $(OBJSZ) $(LIBSM) $(CLIB)

$(ROM_MODULE): oz.ModuleWrap $(ROMCSTUBS) $(LIBSM) o.dirs
        @echo
        $(LD) -s syms.ROMMod -o $@ -aof oz.ModuleWrap $(OBJSZ) $(LIBSM) $(ROMCSTUBS)

rm.$(APP)D: odz.ModuleWrap  $(LIBSM) o.dirs
        @echo
        $(LD) -s syms.ROMDMod -o $@ -module oz.ModuleWrap $(OBJSDZ) $(LIBSM)


Engine:
	@echo Calling Engine Makefile
	dir ^.Engine
	@amu -f Makefile $(MFLAGS)
	@echo Returned from Engine Makefile
	dir ^.Build

$(UI):
	@echo Calling $(UI) Makefile
	dir ^.$(UI)
	@amu -f Makefile $(MFLAGS)
	@echo Returned from $(UI) Makefile
	dir ^.Build

cleanWebMail:
	@echo Calling WebMail Makefile
	dir ^.WebMail
	@amu -f Makefile $(MFLAGS)
	@echo Returned from WebMail Makefile
	dir ^.Build

cleanTestUI:
	@echo Calling TestUI Makefile
	dir ^.TestUI
	@amu -f Makefile $(MFLAGS)
	@echo Returned from TestUI Makefile
	dir ^.Build


# -----------------------------------------------------------------------------
# Dynamic dependencies:
