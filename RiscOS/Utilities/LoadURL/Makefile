# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT	= LoadURL
APP		= !LoadURL
MODULE		= rm.!LoadURL
ROM_MODULE	= aof.LoadURL

# -----------------------------------------------------------------------------
# Export Paths for Messages module
#

RESDIR          = $(RESDIR).$(COMPONENT)
RESAPP          = <Resource$Dir>.Apps.$(APP)

# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= $(MemCheckCC) cc
C++		= c++
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe
STRIP		= stripdepnd

AFLAGS		= -depend !Depend -nocache -stamp -quit $(THROWBACK)
CFLAGS		= -depend !Depend -ffah -I$(INCLUDES) -wp $(THROWBACK)
C++FLAGS	= -c -depend !Depend -ffa -wP\
		  -I$(INCLUDES) $(DFLAGS) $(THROWBACK) $(C++EXTRA)
LDFLAGS		= -aif -c++
CPFLAGS		= ~cfr~v~n
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DFLAGS		= -fn -DDEBUGLIB
#DFLAGS		= -fn
DDFLAGS		= -fn -DDEBUGLIB
DZFLAGS		= -fn -DDEBUGLIB
DDZFLAGS	= -fn -DDEBUGLIB


# -----------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>

CLIB		= C:o.stubs
CPPLIB		= $(LIBDIR).C++Lib.o.c++lib
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
WRAPPER		= RISC_OSLib:s.ModuleWrap

EVENTLIB	= tbox:o.eventlib
TOOLBOXLIB	= tbox:o.toolboxlib
WIMPLIB		= tbox:o.wimplib

MSGLIB		= $(LIBDIR).MsgLib.o.MsgLib
DEBUGLIB	= $(LIBDIR).DebugLib.o.DebugLib
ERRORLIB	= $(LIBDIR).ErrorLib.o.ErrorLib
NBLIB		= $(LIBDIR).NBLib.o.NBLib
MEMCHECKLIB	= MemCheck:o.MC
EMAILLIB	= $(LIBDIR).Email.EmailLib.o.EmailLib

LIBSM =\
 $(NBLIB)zm\
 $(MSGLIB)zm\
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)m\
 $(ERRORLIB)zm\
 $(MEMCHECKLIB)\
 $(DEBUGLIB)zm\
 TCPIPLibs:o.socklib5zm\
 TCPIPLibs:o.inetlibzm\
 $(EMAILLIB)zm\
 $(CPPLIB)z


LIBS =\
 $(NBLIB)\
 $(MSGLIB)\
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(ERRORLIB)\
 $(CLIB)\
 $(MEMCHECKLIB)\
 $(DEBUGLIB)\
 TCPIPLibs:o.socklib5\
 TCPIPLibs:o.inetlib\
 $(EMAILLIB)\
 $(CPPLIB)

#LIBSD =\

# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= C:,<Lib$Dir>

FILES =\
 LocalRes:!Boot\
 LocalRes:!Run\
 LocalRes:!Help\
 LocalRes:!Sprites22\
 LocalRes:Messages\
 LocalRes:Res\
 $(TARGET)

# Include these in the ROM module
RESFILES =\
 LocalRes:!Sprites22\

DIRS = o._dirs

# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS =\
 o.eventmgr\
 o.main\
 o.browserif\
 o.Cmain

OBJSZ =\
 oz.eventmgr\
 oz.main\
 oz.browserif\
 oz.Cmain

#OBJSZ =\

#OBJSD =\

#OBJSDZ =\


# -----------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .odz .s .c

.c.o:;		@echo
		@echo Compiling $<
		@echo $(DFLAGS)
		@$(CC) $(CFLAGS) $(DFLAGS) $(EXTRAFLAGS) -c -o $@ $<

.c.oz:;		@echo
		@echo Compiling $<
		@echo $(DZFLAGS)
		@$(CC) $(CFLAGS) $(DZFLAGS) $(EXTRAFLAGS) -c -zM -zps1 -DROM -o $@ $<

.c.od:;		@echo
		@echo Compiling $<
		@echo $(DDFLAGS)
		@$(CC) $(CFLAGS) $(DDFLAGS) $(EXTRAFLAGS) -c -o $@ $<

.c.odz:;	@echo
		@echo Compiling $<
		@echo $(DDZFLAGS)
		@$(CC) $(CFLAGS) $(DDZFLAGS) $(EXTRAFLAGS) -c -zM -zps1 -DROM -o $@ $<

.s.o:;		@echo
		@echo Assembling $<
		@echo $(AFLAGS)
		@$(AS) $(AFLAGS) $< $@

.c++.o:;	$(C++) $(C++FLAGS) -fn -o $@ $<
.c++.oz:;	$(C++) $(C++FLAGS) -zM -o $@ $<


# -----------------------------------------------------------------------------
# Main rules
#

all: $(FILES)
	@echo $(COMPONENT): Application built (Disc)

rom: $(ROM_MODULE)
	@echo $(COMPONENT): Module built (ROM)

install_common: $(FILES)
	@echo
	$(CP) LocalRes:!Boot			$(INSTDIR).!Boot		$(CPFLAGS)
	$(CP) LocalRes:!Help			$(INSTDIR).!Help	   	$(CPFLAGS)
	$(CP) LocalRes:!Sprites22		$(INSTDIR).!Sprites22   	$(CPFLAGS)
	$(CP) LocalRes:Messages			$(INSTDIR).Messages		$(CPFLAGS)
	$(CP) LocalRes:Res			$(INSTDIR).Res			$(CPFLAGS)

install: install_common
	$(CP) LocalRes:!Run		$(INSTDIR).!Run		$(CPFLAGS)
	$(CP) $(TARGET)			$(INSTDIR).!RunImage	$(CPFLAGS)
	-Access $(INSTDIR)		lr/r
	-Access $(INSTDIR).!Boot	lr/r
	-Access $(INSTDIR).!Run		lr/r
	-Access $(INSTDIR).!RunImage	lr/r
	-Access $(INSTDIR).!Help	lr/r
	-Access $(INSTDIR).Messages	lr/r
	-Access $(INSTDIR).Res		lr/r
	@echo
	@echo $(COMPONENT): Application installed to $(INSTDIR)

installd: install_common
	$(CP) LocalRes:!RunT		$(INSTDIR).!RunT	$(CPFLAGS)
	$(CP) $(TARGET)			$(INSTDIR).!RunImageT	$(CPFLAGS)
	@echo
	@echo $(COMPONENT): LoadURL application installed $(INSTDIR)

install_rom: $(ROM_MODULE)
	$(CP) $(ROM_MODULE) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
	@echo
	@echo $(COMPONENT): Module installed (ROM)

resources:
	$(MKDIR) $(RESDIR)
	$(MKDIR) $(RESAPP)
	$(MKDIR) $(RESDIR).User
	$(CP) LocalRes:ROM.!Run		$(RESAPP).!Run		$(CPFLAGS)
	$(CP) LocalRes:!Sprites22	$(RESDIR).!Sprites22	$(CPFLAGS)
	$(CP) LocalRes:Messages		$(RESDIR).Messages	$(CPFLAGS)
	$(CP) LocalRes:Res		$(RESDIR).Res		$(CPFLAGS)
	@echo
	@echo $(COMPONENT): Resource files copied to Messages module

clean:
	@echo starting
	$(WIPE)	abs		$(WFLAGS)
	$(WIPE)	aof		$(WFLAGS)
	$(WIPE)	linked		$(WFLAGS)
	$(WIPE)	map		$(WFLAGS)
	$(WIPE)	o		$(WFLAGS)
	$(WIPE)	od		$(WFLAGS)
	$(WIPE)	oz		$(WFLAGS)
	$(WIPE)	odz		$(WFLAGS)
	$(WIPE) syms		$(WFLAGS)
	$(WIPE) Targets		$(WFLAGS)
	$(RM)	s.AppName
	$(RM)	s.ModuleWrap
	$(STRIP)
	@echo
	@echo $(COMPONENT): Cleaned

$(DIRS):
	@$(MKDIR) abs
	@$(MKDIR) aof
	@$(MKDIR) linked
	@$(MKDIR) map
	@$(MKDIR) o
	@$(MKDIR) od
	@$(MKDIR) oz
	@$(MKDIR) odz
	@$(MKDIR) syms
	@$(MKDIR) Targets
	@$(MKDIR) Targets.!LoadURL
	create $(DIRS)
	stamp  $(DIRS)


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: $(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) -map -o null:x $(OBJS) $(LIBS) > map.linked

map:
	$(LD) $(LDFLAGS) -map -bin -base 0 -o null: $(OBJS) $(LIBS) > map.base0

clean_all: clean
	$(WIPE)	Targets		$(WFLAGS)
	@echo $(COMPONENT): Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#

rom_link:
	$(LD) $(LDFLAGS)-o linked.$(COMPONENT) -rmf -base $(ADDRESS) \
		$(ROM_MODULE) $(ABSSYM) -map > map.$(COMPONENT)
	$(CP) linked.$(COMPONENT) $(LINKDIR).$(COMPONENT) $(CPFLAGS)
	@echo $(COMPONENT): rom_link complete

oz.ModuleWrap: s.ModuleWrap s.AppName $(RESFILES)
	$(AS) $(AFLAGS) s.ModuleWrap $@

s.ModuleWrap: $(WRAPPER)
	$(CP) $(WRAPPER) $@ $(CPFLAGS)

s.AppName: LocalRes:Messages
	awk -f awk.AppName LocalRes:Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: $(OBJS) $(LIBS) $(DIRS)
	@echo
	$(LD) $(LDFLAGS) -s syms.!RI -o $@ $(OBJS) $(LIBS)
	$(SQUEEZE) $(SQFLAGS) $@

abs.!RI_D: $(OBJSD) $(LIBSD) $(DIRS)
	@echo
	$(LD) $(LDFLAGS) -s syms.!RI_D -o $@ $(OBJSD) $(LIBSD)

$(MODULE): oz.ModuleWrap $(OBJSZ) $(LIBSM) $(CLIB) $(DIRS)
	@echo
	$(LD) $(LDFLAGS) -s syms.RAMMod -o $@ -module oz.ModuleWrap $(OBJSZ) $(LIBSM) $(CLIB)

$(ROM_MODULE): oz.ModuleWrap $(OBJSZ) $(ROMCSTUBS) $(LIBSM) $(DIRS)
	@echo
	$(LD) $(LDFLAGS) -s syms.ROMMod -o $@ -aof oz.ModuleWrap $(OBJSZ) $(LIBSM) $(ROMCSTUBS)

rm.!GUID: odz.ModuleWrap $(OBJSDZ) $(LIBSM) $(DIRS)
	@echo
	$(LD) $(LDFLAGS) -s syms.ROMDMod -o $@ -module oz.ModuleWrap $(OBJSDZ) $(LIBSM)


# -----------------------------------------------------------------------------
# Dynamic dependencies:
