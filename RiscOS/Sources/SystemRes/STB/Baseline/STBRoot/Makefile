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
INIMGDIR	= LocalUserIFRes:Images
INVODDIR	= LocalUserIFRes:VOD
INAUDDIR	= LocalUserIFRes:Audio
INSNDDIR	= LocalUserIFRes:Sounds
INMULTIDIR	= LocalUserIFRes:Multicast

#
# Destination paths and filenames for files and directories to be installed to:
#
OUTRESDIR	= $(INSTDIR)
OUTARMDIR	= $(OUTRESDIR).!ARMBoot
OUT300DIR	= $(OUTRESDIR).Content.HTML.300
OUT400DIR	= $(OUTRESDIR).Content.HTML.400
OUTHELPDIR	= $(OUTRESDIR).Content.HTML.Help
OUTJSCRDIR	= $(OUTRESDIR).Content.HTML.JScript
OUTIMGDIR	= $(OUTRESDIR).Content.HTML.images
OUTAUDDIR	= $(OUTRESDIR).Content.HTML.Audio
OUTSNDDIR	= $(OUTRESDIR).Content.Sounds
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
TRANS300DIR	= TranslateRes:300
TRANS400DIR	= TranslateRes:400
TRANSHELPDIR	= TranslateRes:Help
TRANSJSCRDIR	= TranslateRes:JScript

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
	$(CP) $(INIMGDIR).*      $(OUTIMGDIR).*      $(CPFLAGS)
	$(CP) $(INVODDIR).*      $(OUTVODDIR).*      $(CPFLAGS)
	$(CP) $(INMULTIDIR).*    $(OUTMULTIDIR).*    $(CPFLAGS)
	$(CP) $(INAUDDIR).*      $(OUTAUDDIR).*      $(CPFLAGS)
	$(CP) $(INSNDDIR).*      $(OUTSNDDIR).*      $(CPFLAGS)
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
	$(MKDIR) $(OUT300DIR)
	$(MKDIR) $(OUT400DIR)
	$(MKDIR) $(OUTHELPDIR)
	$(MKDIR) $(OUTJSCRDIR)
	$(MKDIR) $(OUTIMGDIR)
	$(MKDIR) $(OUTVODDIR)
	$(MKDIR) $(OUTMULTIDIR)
	$(MKDIR) $(OUTAUDDIR)
	$(MKDIR) $(OUTSNDDIR)
	$(MKDIR) $(OUTRESDIR).Content.Java
	$(MKDIR) $(OUTRESDIR).Content.MPEG
	$(MKDIR) $(OUTRESDIR).Content.Sounds
	$(MKDIR) $(OUTRESDIR).Public
	$(MKDIR) $(OUTRESDIR).Versions

translate: dirs
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSRESDIR).!TFTPBoot   -o $(OUTRESDIR).!TFTPBoot
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS300DIR).body/html   -o $(OUT300DIR).body/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS300DIR).home/html   -o $(OUT300DIR).home/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS300DIR).index/html  -o $(OUT300DIR).index/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS300DIR).side/html   -o $(OUT300DIR).side/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS300DIR).top/html    -o $(OUT300DIR).top/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS400DIR).body/html   -o $(OUT400DIR).body/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS400DIR).home/html   -o $(OUT400DIR).home/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS400DIR).index/html  -o $(OUT400DIR).index/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS400DIR).side/html   -o $(OUT400DIR).side/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANS400DIR).top/html    -o $(OUT400DIR).top/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHELPDIR).body/html  -o $(OUTHELPDIR).body/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHELPDIR).help/html  -o $(OUTHELPDIR).help/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHELPDIR).index/html -o $(OUTHELPDIR).index/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHELPDIR).side/html  -o $(OUTHELPDIR).side/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSHELPDIR).top/html   -o $(OUTHELPDIR).top/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSJSCRDIR).blank/html -o $(OUTJSCRDIR).blank/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSJSCRDIR).ctrl/html  -o $(OUTJSCRDIR).ctrl/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSJSCRDIR).tv/html    -o $(OUTJSCRDIR).tv/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSJSCRDIR).vod/html   -o $(OUTJSCRDIR).vod/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSJSCRDIR).plugplay   -o $(OUTJSCRDIR).plugplay/html
	$(TRANSLATE) $(TRANSFLAGS) -m $(TRANSMAPPINGS) -i $(TRANSJSCRDIR).simple     -o $(OUTJSCRDIR).simple/html
        @echo Translation Complete.

resources:
	@echo $(COMPONENT): resource files copied

# Dynamic dependencies:
