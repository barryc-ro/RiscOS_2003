# Makefile for STBBoot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name       Description
# ----         ----       -----------
# 09-Aug-99    SNB        Created from OmegaNC2UI
# 29-Mar-2000  SAR        Updated.
#

COMPONENT = STBBoot

#
# Generic options:
#
AWK		= awk
MKDIR		= mkdir -p
AS		= objasm
CC		= cc
CMHG		= cmhg
CP		= copy
LD		= link
RM		= remove
MODSQZ		= modsqz
WIPE		= -wipe
CD		= dir
TOUCH		= create
CHMOD		= attr $(CHMODFLAGS)
TRANSLATE	= translate

#
# Paths and filenames for files and directories within this component:
#
INRESDIR	= LocalUserIFRes:Applet
INVSNFILE	= LocalUserIFRes:Version
INCHOICEDIR	= LocalUserIFRes:Choices
INPREDIR	= LocalUserIFRes:PreDesktop
INDESKDIR	= LocalUserIFRes:Desktop
INLIBDIR	= LocalUserIFRes:Library
INUTILDIR	= LocalUserIFRes:Utilities

#
# Destination paths and filenames for files and directories to be installed to:
#
OUTRESDIR	= $(INSTDIR).$(RES)
OUTVSNDIR	= $(INSTDIR).$(VERSN)
OUTVSNFILE	= $(OUTVSNDIR).$(COMPONENT)
OUTCHOICEDIR	= $(OUTRESDIR).$(CHOICES)
OUTPREDIR	= $(OUTRESDIR).$(PRE)
OUTDESKDIR	= $(OUTRESDIR).$(DESK)
OUTLIBDIR	= $(OUTRESDIR).$(LIB)
OUTUTILDIR	= $(OUTRESDIR).$(UTIL)

#
# Flags and translation macros:
#
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v
CHMODFLAGS	= +ow +or +wr -ww
TRANSFLAGS	= $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"
TRANSMAPPINGS	= TranslateRes:Mappings
TRANSRESDIR	= TranslateRes:Applet
TRANSDESKDIR	= TranslateRes:Desktop
TRANSUTILDIR	= TranslateRes:Utilities

#
# Rule patterns
#
.SUFFIXES:	.o .s .c .h .cmhg
.c.o:;		$(CC) $(CFLAGS) -o $@ $<
.cmhg.o:;	$(CMHG) $(CMHGFLAGS) -o $@ $<
.cmhg.h:;	$(CMHG) $(CMHGFLAGS) -d $@ $<
.s.o:;		$(AS) $(AFLAGS) $< $@

#
# Generic rules:
#
rom: $(TARGET)
	@echo $(COMPONENT): ROM module built

export: $(EXPORTS)
	@echo $(COMPONENT): export complete

install_rom: $(TARGET)
	@echo $(COMPONENT): ROM module installed

install: translate dirs
	@echo Installing top level...
	$(CP) $(INRESDIR).*      $(OUTRESDIR).*      $(CPFLAGS)
	$(CP) $(INCHOICEDIR).*   $(OUTCHOICEDIR).*   $(CPFLAGS)
	$(CP) $(INPREDIR).*      $(OUTPREDIR).*      $(CPFLAGS)
	$(CP) $(INDESKDIR).*     $(OUTDESKDIR).*     $(CPFLAGS)
	$(CP) $(INLIBDIR).*      $(OUTLIBDIR).*      $(CPFLAGS)
	$(CP) $(INUTILDIR).*     $(OUTUTILDIR).*     $(CPFLAGS)
	$(CP) $(INVSNFILE)       $(OUTVSNFILE)       $(CPFLAGS)
	Do $(AWK) -- "/.define.*CMHG.*/" VersionNum >>$(OUTVSNFILE)
	$(CHMOD) $(INSTDIR)
	@echo $(COMPONENT): RAM module installed

clean:
	@echo $(COMPONENT): cleaned

local_clean:
	$(WIPE) Translated $(WFLAGS)
	@echo $(COMPONENT): cleaned local resources directory

dirs:
	$(MKDIR) $(OUTRESDIR).Resources.300.Misc
	$(MKDIR) $(OUTRESDIR).Resources.400.Misc
	$(MKDIR) $(OUTCHOICEDIR)
	$(MKDIR) $(OUTLIBDIR)
	$(MKDIR) $(OUTUTILDIR)
	$(MKDIR) $(OUTDESKDIR).300
	$(MKDIR) $(OUTDESKDIR).400
	$(MKDIR) $(OUTPREDIR).300
	$(MKDIR) $(OUTPREDIR).400
	$(MKDIR) $(OUTVSNDIR)

translate: dirs
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRESDIR).!Run        -o $(OUTRESDIR).!Run
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSUTILDIR).Desktop    -o $(OUTUTILDIR).Desktop
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSUTILDIR).PreDesktop -o $(OUTUTILDIR).PreDesktop
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSDESKDIR).NCFresco   -o $(OUTDESKDIR).NCFresco
	@echo Translation Complete.

resources:
	@echo $(COMPONENT): resource files copied

# Dynamic dependencies:
