# Makefile for MPTMail - resource export only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!MPTMail
RESRES = <Resource$Dir>.Resources.MPTMail

#
# Generic options:
#
MKDIR   = mkdir -p
CP      = copy
SQ      = squeeze
RM      = remove
LD	= link
TR	= translate

CCFLAGS = -c -depend !Depend -IC:
CPFLAGS = ~cfr~v
# TRFLAGS = $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë" -m NCFrescoROTransRes:Mappings
SQFLAGS = 

#
# Program specific options:
#
COMPONENT = MPTMail
TARGET    = aof.${COMPONENT}

# Binary Build Environment augmenting rules
BBETYPE   = local
bbe-local: bbe-generic
  BBE_Export_Dir abs
  BBE_Export_Dir Docs
  BBE_Export_File !MkRes


#
# Generic rules:
#
rom: 
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned

resources:
	Set MPTMailRORes$Path Resources.<UserIF>.<Locale>.
	${MKDIR} ${RESAPP}
	${CP} MPTMailRORes:!Boot                ${RESAPP}.!Boot       ${CPFLAGS}
	${CP} MPTMailRORes:!Run                 ${RESAPP}.!Run        ${CPFLAGS}
	${CP} MPTMailRORes:!Sprites             ${RESAPP}.!Sprites    ${CPFLAGS}
	${CP} MPTMailRORes:Messages             ${RESAPP}.Messages    ${CPFLAGS}
	${CP} MPTMailRORes:HTML                 ${RESAPP}.HTML        ${CPFLAGS}
	${CP} abs.mail                          ${RESAPP}.mail        ${CPFLAGS}
	Unset MPTMailRORes$Path
        @echo ${COMPONENT}: resource files copied

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete (libs)

export_hdrs:
        @echo ${COMPONENT}: export complete (hdrs)

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        @echo ${COMPONENT}: rom_link complete

# Dynamic dependencies:
