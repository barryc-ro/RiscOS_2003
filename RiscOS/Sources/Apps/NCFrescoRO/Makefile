# Makefile for NCFresco - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!NCFresco
RESRES = <Resource$Dir>.Resources.NCFresco
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
COMPONENT = NCFresco
TARGET    = aof.${COMPONENT}
TARGETU   = aof.${COMPONENT}u

ABSSYM    = RISC_OSLib:o.abssym

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
	Set NCFrescoRORes$Path Resources.<UserIF>.<Locale>.,Resources.Common.<Locale>.,Resources.<UserIF>.UK.,Resources.Common.UK.
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.html
	${CP} NCFrescoRORes:!BootROM    ${RESAPP}.!Boot    ${CPFLAGS}
	${CP} NCFrescoRORes:!RunROM     ${RESAPP}.!Run     ${CPFLAGS}
	${CP} NCFrescoRORes:!SpritesROM ${RESAPP}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:SpritesROM  ${RESRES}.Sprites  ${CPFLAGS}
	${CP} NCFrescoRORes:Messages    ${RESRES}.Messages ${CPFLAGS}
	${CP} NCFrescoRORes:Res         ${RESRES}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:Welcome     ${RESRES}.Welcome  ${CPFLAGS}
	${CP} NCFrescoRORes:SetScrap    ${RESRES}.SetScrap ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     ${RESRES}.MimeMap  ${CPFLAGS}
	${CP} NCFrescoRORes:Config      ${RESRES}.Config   ${CPFLAGS}
	${CP} NCFrescoRORes:html.badurl   ${RESRES}.html.badurl   ${CPFLAGS}
	${CP} NCFrescoRORes:html.sslcert  ${RESRES}.html.sslcert  ${CPFLAGS}
	${CP} NCFrescoRORes:html.sslpin   ${RESRES}.html.sslpin   ${CPFLAGS}
	${CP} NCFrescoRORes:html.sslserv  ${RESRES}.html.sslserv  ${CPFLAGS}
	${CP} NCFrescoRORes:html.ssltoken ${RESRES}.html.ssltoken ${CPFLAGS}
	${CP} NCFrescoRORes:html.jsalert  ${RESRES}.html.jsalert  ${CPFLAGS}
	${CP} NCFrescoRORes:html.jsprompt ${RESRES}.html.jsprompt ${CPFLAGS}
	${CP} NCFrescoRORes:html.jsokcncl ${RESRES}.html.jsokcncl ${CPFLAGS}
	Unset NCFrescoRORes$Path
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
rom_link: rom_link_${TYPE}
        @echo ${COMPONENT}: rom module built

rom_link_unicode: dirs
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGETU} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link for Unicode complete

rom_link_: dirs
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

dirs:
	${MKDIR} linked

# Dynamic dependencies:
