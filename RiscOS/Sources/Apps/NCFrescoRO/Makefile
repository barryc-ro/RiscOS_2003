# Makefile for NCFresco - resource export and final link only
#

#
# Paths
#
EXP_HDR = <cexport$dir>
RESAPP = <resource$Dir>.App
RESRES = <resource$Dir>.Resources
SRCCOMMON = Resources.Common
SRCLOCALE = Resources.<System>.<Locale>
#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
LD	= link
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = NCFresco
TARGET    = aof.${COMPONENT}

ABSSYM    = RISC_OSLib:o.AbsSym

#
# Generic rules:
#
rom: 
        @echo ${COMPONENT}: rom module built

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        @echo ${COMPONENT}: cleaned

resources1:
	${CP} ${SRCCOMMON}.!BootROM    {RESAPP}.!Boot    ${CPFLAGS}
	${CP} ${SRCCOMMON}.Welcome     {RESRES}.Welcome  ${CPFLAGS}
	${CP} ${SRCCOMMON}.SetScrap    {RESRES}.SetScrap ${CPFLAGS}
	${CP} ${SRCCOMMON}.MimeMap     {RESRES}.MimeMap  ${CPFLAGS}
	${CP} ${SRCLOCALE}.!RunROM     {RESAPP}.!Run     ${CPFLAGS}
	${CP} ${SRCLOCALE}.!SpritesROM {RESAPP}.!Sprites ${CPFLAGS}
	${CP} ${SRCLOCALE}.Config      {RESRES}.Config   ${CPFLAGS}
	${CP} ${SRCLOCALE}.Messages    {RESRES}.Message  ${CPFLAGS}
	${CP} ${SRCLOCALE}.Res         {RESRES}.Res      ${CPFLAGS}
	${CP} ${SRCLOCALE}.Sprites     {RESRES}.Sprites ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

resources:
	${CP} Resources.* <resource$dir>.* ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete(libs)

export_hdrs:
        ${CP} h.plugin ${EXP_HDR}.h.plugin ${CPFLAGS}
        @echo ${COMPONENT}: export complete(hdrs)

#
# Final link for the ROM Image (using given base address)
#
rom_link: dirs
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

dirs:
	${MKDIR} linked
	${MKDIR} map

# Dynamic dependencies:
