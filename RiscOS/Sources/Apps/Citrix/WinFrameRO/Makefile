# Makefile for ICAClient - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!ICAClient
RESRES = <Resource$Dir>.Resources.ICAClient
SRC = LocalRes:
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
COMPONENT = ICAClient
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
	@echo ** Exporting non-translatable resource files
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.ini
	SetMacro WinFrameLocalRes$path Resources.$(USERIF).$(LOCALE).,Resources.$(USERIF).UK.,Resources.UK.
	${CP} WinFrameLocalRes:!Boot       ${RESAPP}.!Boot    ${CPFLAGS}
	${CP} WinFrameLocalRes:!Help       ${RESAPP}.!Help    ${CPFLAGS}
	${CP} WinFrameLocalRes:!Run        ${RESAPP}.!Run     ${CPFLAGS}
	${CP} WinFrameLocalRes:!Sprites    ${RESAPP}.!Sprites ${CPFLAGS}
	${CP} WinFrameLocalRes:EnsureTB    ${RESRES}.EnsureTB ${CPFLAGS}
	${CP} WinFrameLocalRes:Messages    ${RESRES}.Messages ${CPFLAGS}
	${CP} WinFrameLocalRes:Res         ${RESRES}.Res      ${CPFLAGS}
	${CP} WinFrameLocalRes:Splash      ${RESRES}.Splash   ${CPFLAGS}
	${CP} WinFrameLocalRes:ini.AppSrv   ${RESRES}.ini.AppSrv   ${CPFLAGS}
	${CP} WinFrameLocalRes:ini.Module   ${RESRES}.ini.Module   ${CPFLAGS}
	${CP} WinFrameLocalRes:ini.WFClient ${RESRES}.ini.WFClient ${CPFLAGS}
        UnSet WinFrameLocalRes$path
        @echo ${COMPONENT}: resource files copied

export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete(libs)

export_hdrs:
	${CP} h.icaclient ${EXP_HDR}.h.icaclient ${CPFLAGS}
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
