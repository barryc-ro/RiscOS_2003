# Makefile for NCMail Engine
# =========================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date          Name    Description
# ====          ====    ===========
# 1998-09-16    AW      Created
# 1998-09-22	RDW	Modified to fit into build structure


# ------------------------------------------------------------------------------
# Paths
#

EXP_C_H	= <CExport$Dir>.h
EXP_C_O	= <CExport$Dir>.o
LIBDIR	= <Lib$Dir>


# -----------------------------------------------------------------------------
# Program specific options
#

COMPONENT       = Engine
APP             = !NCMail
MODULE		= rm.!NCMail
ROM_MODULE	= aof.NCMail
LPATH            = LocalUserIFRes:
#TARGET		= Engine
#TARGETD		= $(TARGET)d
#TARGETZ		= $(TARGET)z

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
CFLAGS          = $(THROWBACK) -depend !Depend -ffa $(INCLUDES) -wp -DENGINE
C++FLAGS        = $(THROWBACK) -depend !Depend -ffa $(INCLUDES) -wp -DENGINE
LDFLAGS		= -c++ -aof
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
SOCKLIB		= $(LIBDIR).TCPIPLibs.o.socklib5
DEBUGLIB	= $(LIBDIR).DebugLib.o.debuglib
INETLIB		= $(LIBDIR).TCPIPLibs.o.inetlib
UNIXLIB		= $(LIBDIR).TCPIPLibs.o.unixlib


LIBS = \
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(MBOXLIB)\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)\
 $(EXTRALIBS)\
 $(C++LIB)\
 $(CLIB)\
 $(SOCKLIB)\
 $(DEBUGLIB)\
 $(INETLIB)\
 $(UNIXLIB)\


#these are not complete
LIBSM = \
 $(WIMPLIB) \
 $(TOOLBOXLIB) \
 $(EVENTLIB)m \
 $(ACORNPOP3)\
 $(C++LIB)\


LIBSD = \
 $(WIMPLIB)\
 $(TOOLBOXLIB)\
 $(EVENTLIB)\
 $(MBOXLIB)d\
 $(ACORNPOP3)\
 $(ACORNSMTP)\
 $(ELIB)\
 $(EXTRALIBS)\
 $(C++LIB)\
 $(CLIB)\
 $(SOCKLIB)\
 $(DEBUGLIB)\
 $(INETLIB)\
 $(UNIXLIB)\


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES        = -Itbox:,C:,<Lib$Dir>,<Lib$Dir>.Email.Mailbox

#FILES = \
# $(LPATH)!Boot \
# $(LPATH)!Run \
# $(LPATH)!Sprites22 \
# $(LPATH)Messages \
# $(LPATH)Sprites22 \
# $(TARGET)

FILES = \
 $(LPATH)!Run \
 $(TARGET)

# Include these in the ROM module
#RESFILES = \
# $(LPATH)!Sprites22 \


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = o.engcbf o.engdownld o.engfile o.enginit o.engevent \
       o.engmanager o.engparse o.engprocess o.engsend \
       o.engstr o.enguiapi o.enguser

OBJSD = od.engcbf od.engdownld od.engfile od.engevent \
        od.enginit od.engmanager od.engparse od.engprocess \
        od.engsend od.engstr od.enguiapi od.enguser

OBJSZ=  oz.engcbf oz.engdownld oz.engfile oz.engevent \
        oz.enginit oz.engmanager oz.engparse oz.engprocess \
        oz.engsend oz.engstr oz.enguiapi oz.enguser



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

export: export_$(PHASE)

export_hdrs:
	@$(CP)	h.enguiapi	^.Build.h.enguiapi			$(CPFLAGS)
	@$(CP)	h.engtypes	^.Build.h.engtypes			$(CPFLAGS)
	@echo $(COMPONENT): export complete (hdrs)		$(CPFLAGS)

export_libs: 
	@echo $(COMPONENT): export complete (libs)

rom: $(ROM_MODULE)
        @echo $(COMPONENT): Module built (ROM)

all: library
        @echo $(COMPONENT): all built (Disc)

library: $(TARGET) $(TARGETZ) o.dirs
		@echo ${COMPONENT}: complete

debug: $(TARGETD)
		@echo ${COMPONENT}: debug complete

install_rom: $(ROM_MODULE)
	$(CP) $(ROM_MODULE) ^.Build.oz.Engine	$(CPFLAGS)

install_common: $(FILES)
        @echo

install: install_common
        $(CP) $(LPATH)!Run              ^.Build.$(INSTDIR).!Run         $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Application installed to ^.Build.$(INSTDIR)

installd: install_common
        $(CP) $(LPATH)!RunD             ^.Build.$(INSTDIR).!Run         $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Debug application installed ^.Build.$(INSTDIR)

resources:
	@echo $(RESDIR)
        $(MKDIR) $(RESDIR)
        $(MKDIR) $(RESAPP)
        $(CP) $(LPATH)ROM.!Run          $(RESAPP).!Run          $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Resource files copied to Messages module

clean:
        @echo starting
        $(WIPE) o               $(WFLAGS)
        $(WIPE) od              $(WFLAGS)
        $(WIPE) oz              $(WFLAGS)
        $(WIPE) odz             $(WFLAGS)
        $(WIPE) syms             $(WFLAGS)
        $(RM)   syms
        $(RM)   o.dirs
	$(RM)   ^.Build.h.enguiapi
	$(RM)   ^.Build.h.engtypes
        @echo
        @echo $(COMPONENT): Cleaned

o.dirs:
        @$(MKDIR) o
        @$(MKDIR) od
        @$(MKDIR) oz
        @$(MKDIR) odz
        @$(MKDIR) syms
        create o.dirs
        stamp  o.dirs


# -----------------------------------------------------------------------------
# Development rules
#

clean_all: clean
        @echo $(COMPONENT): Cleaned all


# -----------------------------------------------------------------------------
# Final linkfor ROM Image (using given base address)
#


# -----------------------------------------------------------------------------
# Static dependencies
#

abs.!RI: $(OBJS) o.dirs
	$(LD) $(LDFLAGS) -o ^.Build.o.Engine $(OBJS)

$(ROM_MODULE): $(OBJSZ) o.dirs
	$(LD) $(LDFLAGS) -o ^.Build.oz.Engine $(OBJSZ)

$(MODULE): $(OBJSZ) o.dirs
	$(LD) $(LDFLAGS) -o ^.Build.oz.Engine $(OBJSZ)

abs.!RI_D: $(OBJSD) o.dirs
	$(LD) $(LDFLAGS) -o ^.Build.od.Engine $(OBJSD)


# -----------------------------------------------------------------------------
# Dynamic dependencies:
