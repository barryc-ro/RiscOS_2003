# Makefile for STBRoot
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name       Description
# ----         ----       -----------
# 09-Aug-1999  SNB        Created from OmegaNC2UI
# 29-Mar-2000  SAR        Updated.
#

COMPONENT = STBRoot

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
CHMODRW		= attr $(CHMODRWFLAGS)
TRANSLATE	= translate

#
# Paths and filenames for files and directories within this component:
#
INRESDIR	= LocalUserIFRes:Root
INARMDIR	= LocalUserIFRes:ARMBoot
INHTMLDIR	= LocalUserIFRes:HTML
INIMGDIR	= LocalUserIFRes:Images
INVODDIR	= LocalUserIFRes:VOD
INMULTIDIR	= LocalUserIFRes:Multicast

#
# Destination paths and filenames for files and directories to be installed to:
#
OUTRESDIR	= $(INSTDIR)
OUTARMDIR	= $(OUTRESDIR).!ARMBoot
OUTHTMLDIR	= $(INSTDIR).Content.HTML
OUTIMGDIR	= $(OUTHTMLDIR).images
OUTVODDIR	= $(OUTIMGDIR).vod
OUTMULTIDIR	= $(OUTIMGDIR).multicast

#
# Flags and translation macros:
#
CPFLAGS		= ~cfr~v
WFLAGS		= ~c~v
CHMODFLAGS	= +ow +or +wr -ww
CHMODRWFLAGS	= +ow +or +wr -ww
TRANSFLAGS	= $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë"
TRANSMAPPINGS	= TranslateRes:Mappings
TRANSRESDIR	= TranslateRes:Root
TRANSHTMLDIR	= TranslateRes:HTML

#
# Rule patterns:
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
	$(CP) $(INRESDIR).*      $(OUTRESDIR).*      $(CPFLAGS)
	$(CP) $(INARMDIR).*      $(OUTARMDIR).*      $(CPFLAGS)
	$(CP) $(INHTMLDIR).*     $(OUTHTMLDIR).*     $(CPFLAGS)
	$(CP) $(INIMGDIR).*      $(OUTIMGDIR).*      $(CPFLAGS)
	$(CP) $(INVODDIR).*      $(OUTVODDIR).*      $(CPFLAGS)
	$(CP) $(INMULTIDIR).*    $(OUTMULTIDIR).*    $(CPFLAGS)
	$(CHMOD) $(INSTDIR)
	$(CHMODRW) $(OUTRESDIR).Public
	@echo $(COMPONENT): RAM module installed

clean:
	@echo $(COMPONENT): cleaned

local_clean:
	$(WIPE) Translated $(WFLAGS)
	@echo $(COMPONENT): cleaned local resources directory

dirs:
	$(MKDIR) $(OUTRESDIR)
	$(MKDIR) $(OUTARMDIR)
	$(MKDIR) $(OUTRESDIR).Apps
	$(MKDIR) $(OUTRESDIR).Content
	$(MKDIR) $(OUTHTMLDIR)
	$(MKDIR) $(OUTIMGDIR)
	$(MKDIR) $(OUTVODDIR)
	$(MKDIR) $(OUTMULTIDIR)
	$(MKDIR) $(OUTRESDIR).Content.Java
	$(MKDIR) $(OUTRESDIR).Content.MPEG
	$(MKDIR) $(OUTRESDIR).Content.Sounds
	$(MKDIR) $(OUTRESDIR).Public
	$(MKDIR) $(OUTRESDIR).Versions

translate: dirs
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRESDIR).!TFTPBoot   -o $(OUTRESDIR).!TFTPBoot
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).body/html  -o $(OUTHTMLDIR).body/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).ctrl/html  -o $(OUTHTMLDIR).ctrl/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).home/html  -o $(OUTHTMLDIR).home/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).index/html -o $(OUTHTMLDIR).index/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).side/html  -o $(OUTHTMLDIR).side/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).top/html   -o $(OUTHTMLDIR).top/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).tv/html    -o $(OUTHTMLDIR).tv/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHTMLDIR).vod/html   -o $(OUTHTMLDIR).vod/html
        @echo Translation Complete.

resources:
	@echo $(COMPONENT): resource files copied

# Dynamic dependencies:
