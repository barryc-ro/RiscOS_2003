# Makefile for WinFrame - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!WinFrame
RESRES = <Resource$Dir>.Resources.WinFrame
SRC = Resources.${Locale}
#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = -copy
RM      = remove
LD	= link
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = WinFrame
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

resources:
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.ini
	${CP} ${SRC}.!Boot       ${RESAPP}.!Boot    ${CPFLAGS}
	${CP} ${SRC}.!Help       ${RESAPP}.!Help    ${CPFLAGS}
	${CP} ${SRC}.!Run        ${RESAPP}.!Run     ${CPFLAGS}
	${CP} ${SRC}.!Sprites    ${RESAPP}.!Sprites ${CPFLAGS}
	${CP} ${SRC}.Sprites     ${RESRES}.Sprites ${CPFLAGS}
	${CP} ${SRC}.Messages    ${RESRES}.Messages ${CPFLAGS}
	${CP} ${SRC}.Res         ${RESRES}.Res      ${CPFLAGS}
	${CP} ${SRC}.ini.AppSrv   ${RESRES}.ini.AppSrv   ${CPFLAGS}
	${CP} ${SRC}.ini.Module   ${RESRES}.ini.Module   ${CPFLAGS}
	${CP} ${SRC}.ini.WFClient ${RESRES}.ini.WFClient ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#	${CP} ${SRC}.Splash      ${RESRES}.Splash   ${CPFLAGS}

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete(libs)

export_hdrs:
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
