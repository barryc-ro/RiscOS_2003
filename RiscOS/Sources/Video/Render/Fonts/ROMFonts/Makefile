# Makefile for ROMFonts
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 05-Oct-99  SNB          Added ability to build STB Baseline !Fonts application.
#                         Can build UK-only, UK & Japanese, or UK, Japanese & Chinese
#                         by passing in OPTIONS=J, OPTIONS=C or OPTIONS=CJ to the makefile.
#                         UK will build without any OPTIONS flag.
#

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
RM      = remove
LD      = link
WIPE    = -wipe
MKDIR   = mkdir -p
CHMOD   = attr ${CHMODFLAGS}
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -o $@
LDFLAGS = -bin -o $@
CPFLAGS = ~cfr~v
WFLAGS  = ~cfr~v
CHMODFLAGS = +ow +or +wr -ww

#
# Program specific options:
#
SOURCE    = s.ROMFonts
TARGET    = rm.${COMPONENT}

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
	${MKDIR} <Resource$Dir>.Fonts
        @Set Alias$MFD ${MKDIR} <Resource$Dir>.Fonts.%0
	@Set Alias$CFF ${CP} Fonts.%0 <Resource$Dir>.Fonts.%0 ${CPFLAGS}
	Obey Config.${USERIF}
        @echo ${COMPONENT}: resource files copied

install_:
	Obey Config.DiscBuild.$(USERIF) UK

install_J:
	Obey Config.DiscBuild.$(USERIF) Japan
        @echo Japanese installed.

install_C:
	Obey Config.DiscBuild.$(USERIF) China
        @echo Chinese installed.

install_CJ: install_C install_J
        @echo Unicode installed.

install_JC: install_C install_J
        @echo Unicode installed.

install: install_$(OPTIONS) dirs aliases
        $(CP) LocalUserIFRes:!Boot $(INSTDIR).!Boot $(CPFLAGS)
        $(CHMOD) $(INSTDIR)
        @echo $(COMPONENT): installed to $(INSTDIR)

aliases: 
        @Set Alias$MFD $(MKDIR) $(INSTDIR).%0
	@Set Alias$CFF $(CP) Fonts.%0 $(INSTDIR).%0 $(CPFLAGS)

dirs:
	$(MKDIR) $(INSTDIR)
        
o.ROMFonts: ${SOURCE}
        ${MKDIR} o
        ${AS} ${ASFLAGS} ${SOURCE}

rm.ROMFonts: o.ROMFonts
        ${LD} ${LDFLAGS} o.ROMFonts
        SetType ${TARGET} &FFA

o.MaruGothic:
	ResGen Data o.MaruGothic -via via.MaruGothic
rm.MaruGothic: o.MaruGothic
	${LD} ${LDFLAGS} o.MaruGothic

# Dynamic dependencies:
