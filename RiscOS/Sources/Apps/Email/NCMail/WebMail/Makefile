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

COMPONENT       = NCMailUI
APP             = !NCMail
LPATH            = LocalUserIFRes:
MODULE		= rm.!NCMail
ROM_MODULE	= aof.NCMail
LPATH            = LocalUserIFRes:
#TARGET		= NCMailUI
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
CFLAGS          = $(THROWBACK) -depend !Depend -ffah $(INCLUDES) -wp
C++FLAGS        = $(THROWBACK) -depend !Depend -ffa $(INCLUDES) -wp
LDFLAGS		= -c++ -aof
CPFLAGS         = ~cfr~v~n
SQFLAGS         = -f
WFLAGS          = ~cf~vr
LBFLAGS		= -c -o


# -----------------------------------------------------------------------------
# Flags
#

# These are for the standard Desktop build (plus debug)

DDFLAGS         = $(DFLAGS) -fn
DDZFLAGS        = $(DZFLAGS) -fn


# -----------------------------------------------------------------------------
# Libraries
#



LIBS = 


#these are not complete
LIBSM = 



LIBSD = 



# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES        = -Itbox:,C:,<Lib$Dir>,<Lib$Dir>.Email.Mailbox

FILES = \
 $(LPATH)Messages \
 $(TARGET)


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = 	o.EscapeHTML 	\
        o.InBox 	\
        o.Parser 	\
        o.WebMail	\
	o.BrowserIF	\
	o.Entry		\
	o.ReadMsg	\
	o.PopUp		\
	o.Feedback	\
 	o.Compose	\
	o.SendMsg	\
	o.MailTo 	\
	o.Reply		\
	o.Errors

OBJSD = od.EscapeHTML 	\
        od.InBox 	\
        od.Parser 	\
        od.WebMail	\
	od.BrowserIF	\
	od.Entry	\
	od.ReadMsg	\
	od.Feedback	\
	od.PopUp	\
	od.SendMsg	\
	od.Compose	\
	od.MailTo 	\
	od.Reply	\
	od.Errors

OBJSZ = oz.EscapeHTML 	\
        oz.InBox 	\
        oz.Parser 	\
        oz.WebMail	\
	oz.BrowserIF	\
	oz.Entry	\
	oz.ReadMsg	\
	oz.Feedback	\
	oz.Compose	\
	oz.PopUp	\
	oz.SendMsg	\
	oz.MailTo 	\
	oz.Reply	\
	oz.Errors


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
	@$(CP)	h.NCMailUI	^.Build.h.NCMailUI			$(CPFLAGS)
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
	$(CP) $(ROM_MODULE) ^.Build.oz.NCMailUI	$(CPFLAGS)

install_common: $(FILES)
        @echo
        $(CP) $(LPATH)*                  ^.Build.$(INSTDIR).* $(CPFLAGS)

install: install_common
        @echo
        @echo $(COMPONENT): Application installed to ^.Build.$(INSTDIR)


installd: install_common
        @echo
        @echo $(COMPONENT): Debug application installed ^.Build.$(INSTDIR)

resources:
	@echo $(RESDIR)
        $(MKDIR) $(RESDIR)
        $(MKDIR) $(RESAPP)
        $(CP) $(LPATH)Messages          $(RESDIR).Messages      $(CPFLAGS)
        @echo
        @echo $(COMPONENT): Resource files copied to Messages module

clean:
        @echo starting
        $(WIPE) o               $(WFLAGS)
        $(WIPE) od              $(WFLAGS)
        $(WIPE) oz              $(WFLAGS)
        $(WIPE) odz             $(WFLAGS)
        $(RM)   o.dirs
	$(RM) ^.Build.h.NCMailUI
        @echo
        @echo $(COMPONENT): Cleaned

o.dirs:
        @$(MKDIR) ^.Build.h
        @$(MKDIR) ^.Build.o
        @$(MKDIR) ^.Build.od
        @$(MKDIR) ^.Build.oz
        @$(MKDIR) o
        @$(MKDIR) od
        @$(MKDIR) oz
        @$(MKDIR) odz
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
	$(LB) $(LBFLAGS) ^.Build.o.NCMailUI $(OBJS)

$(ROM_MODULE): $(OBJSZ) o.dirs
	$(LB) $(LBFLAGS) ^.Build.oz.NCMailUI $(OBJSZ)

$(MODULE): $(OBJSZ) o.dirs
	$(LB) $(LBFLAGS) ^.Build.oz.NCMailUI $(OBJSZ)

abs.!RI_D: $(OBJSD) o.dirs
	$(LB) $(LBFLAGS) ^.Build.od.NCMailUI $(OBJSD)


# -----------------------------------------------------------------------------
# Dynamic dependencies:
