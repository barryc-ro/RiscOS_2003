# Makefile for Mercury
# =========================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1997-12-17    BAL     Created


# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H	= <CExport$Dir>.h
EXP_C_O	= <CExport$Dir>.o
LIBDIR	= <Lib$Dir>


# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = MBoxTest
APP             = !MBoxTest
MODULE          = rm.!MBoxTest
ROM_MODULE      = aof.MBoxTest
LDIR            = Resources.$(LOCALE)

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


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

#DFLAGS          =
DDFLAGS         = $(DFLAGS) -fn
#DZFLAGS         =
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
COMMON		= $(LIBDIR).Email.Common.o.Common
ELIB		= $(LIBDIR).Email.EmailLib.o.EmailLib
MBOXLIB		= $(LIBDIR).Email.Mailbox.o.Mailbox


LIBS = \
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(MBOXLIB)d\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)\
 $(COMMON)\
 $(EXTRALIBS)\
 $(C++LIB)\
 $(CLIB)\


LIBSM = \
 $(WIMPLIB) \
 $(TOOLBOXLIB) \
 $(EVENTLIB)m \
 $(ACORNPOP3)\
 $(COMMON)\
 $(C++LIB)\


LIBSD = \
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(MBOXLIB)d\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)\
 $(COMMON)\
 $(EXTRALIBS)\
 $(C++LIB)\
 $(CLIB)\


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES        = -Itbox:,C:,<Lib$Dir>,<Lib$Dir>.Email.Mailbox

FILES = \
 $(LDIR).!Boot \
 $(LDIR).!Run \
 $(LDIR).!Sprites22 \
 $(LDIR).Messages \
 $(LDIR).Sprites22 \
 $(TARGET)

# Include these in the ROM module
RESFILES = \
 $(LDIR).!Sprites22 \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS =\
 o.Engine\
 o.main



OBJSD =\
 od.Engine\
 od.main


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
                @$(CC) $(CFLAGS) $(DZFLAGS)       -c -zM -zps1 -DROM -o $@ $<

.c.od:;         @echo
                @echo Compiling $<
                @echo $(DDFLAGS)
                @$(CC) $(CFLAGS) $(DDFLAGS)       -c -g -o $@ $<

.c.odz:;        @echo
                @echo Compiling $<
                @echo $(DDZFLAGS)
                @$(CC) $(CFLAGS) $(DDZFLAGS)      -c -zM -zps1 -DROM -o $@ $<

.c++.o:;        @echo
                @echo Compiling $<
                @echo $(DFLAGS)
                @$(C++) $(C++FLAGS) $(DFLAGS)     -c -o $@ $<

.c++.oz:;       @echo
                @echo Compiling $<
                @echo $(DZFLAGS)
                @$(C++) $(C++FLAGS) $(DZFLAGS)    -c -zM -zps1 -DROM -o $@ $<

.c++.od:;       @echo
                @echo Compiling $<
                @echo $(DDFLAGS)
                @$(C++) $(C++FLAGS) $(DDFLAGS)    -c +g -o $@ $<

.c++.odz:;      @echo
                @echo Compiling $<
                @echo $(DDZFLAGS)
                @$(C++) $(C++FLAGS) $(DDZFLAGS)   -c -zM -zps1 -DROM -o $@ $<

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

all: $(FILES)
        @echo $(COMPONENT): Application built (Disc)

rom: $(ROM_MODULE)
        @echo $(COMPONENT): Module built (ROM)

install_common: $(FILES)
        @echo
        $(CP) $(LDIR).!Boot                     $(INSTDIR).!Boot                $(CPFLAGS)
        $(CP) $(TARGET)                         $(INSTDIR).!RunImage            $(CPFLAGS)
        $(CP) $(LDIR).!Sprites22                $(INSTDIR).!Sprites22           $(CPFLAGS)
        $(CP) $(LDIR).Messages                  $(INSTDIR).Messages             $(CPFLAGS)
        $(CP) $(LDIR).Sprites22                 $(INSTDIR).Sprites22            $(CPFLAGS)

install: install_common
        $(CP) $(LDIR).!Run              $(INSTDIR).!Run         $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Application installed to $(INSTDIR)


#        -Access $(INSTDIR).!Boot        lr/r
#        -Access $(INSTDIR).!Run         lr/r
#        -Access $(INSTDIR).!RunImage    lr/r
#        -Access $(INSTDIR).Messages     lr/r

installd: install_common
        $(CP) $(LDIR).!RunD             $(INSTDIR).!Run         $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Debug application installed $(INSTDIR)

install_rom: $(ROM_MODULE)
        $(CP) $(ROM_MODULE) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Module installed (ROM)

resources:
	@echo $(RESDIR)
        $(MKDIR) $(RESDIR)
        $(MKDIR) $(RESAPP)
        $(CP) $(LDIR).ROM.!Run          $(RESAPP).!Run          $(CPFLAGS)
        $(CP) $(LDIR).!Sprites22        $(RESDIR).!Sprites22    $(CPFLAGS)
        $(CP) $(LDIR).Messages          $(RESDIR).Messages      $(CPFLAGS)
        $(CP) $(LDIR).Res               $(RESDIR).Res           $(CPFLAGS)
        $(CP) $(LDIR).Sprites22         $(RESDIR).Sprites22     $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Resource files copied to Messages module

clean:
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
	$(WIPE) syms		$(WFLAGS)
        $(RM)   s.AppName
        $(RM)   s.ModuleWrap
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
        @$(MKDIR) Targets.!MBoxTest
        create o.dirs
        stamp  o.dirs


# -----------------------------------------------------------------------------
# Development rules
#

linkmap: $(OBJS) $(LIBS)
        $(LD) -map -o null:x $(OBJS) $(LIBS) > map.linked

map:
        $(LD) -map -bin -base 0 -o null: $(OBJS) $(LIBS) > map.base0

clean_all: clean
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

s.AppName: $(LDIR).Messages
        awk -f awk.AppName $(LDIR).Messages > $@


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: $(OBJS) $(LIBS) o.dirs
        @echo
        $(LD) $(LDFLAGS) -s syms.!RI -o $@ $(OBJS) $(LIBS)
        $(SQUEEZE) $(SQFLAGS) $@

abs.!RI_D: $(OBJSD) $(LIBSD) o.dirs
        @echo
        $(LD) $(LDFLAGS) -debug -s syms.!RI_D -o $@ $(OBJSD) $(LIBSD)

$(MODULE): oz.ModuleWrap $(OBJSZ) $(LIBSM) $(CLIB) o.dirs
        @echo
        $(LD) -s syms.RAMMod -o $@ -module oz.ModuleWrap $(OBJSZ) $(LIBSM) $(CLIB)

$(ROM_MODULE): oz.ModuleWrap $(OBJSZ) $(ROMCSTUBS) $(LIBSM) o.dirs
        @echo
        $(LD) -s syms.ROMMod -o $@ -aof oz.ModuleWrap $(OBJSZ) $(LIBSM) $(ROMCSTUBS)

rm.!MBoxTestD: odz.ModuleWrap $(OBJSDZ) $(LIBSM) o.dirs
        @echo
        $(LD) -s syms.ROMDMod -o $@ -module oz.ModuleWrap $(OBJSDZ) $(LIBSM)


# -----------------------------------------------------------------------------
# Dynamic dependencies:

