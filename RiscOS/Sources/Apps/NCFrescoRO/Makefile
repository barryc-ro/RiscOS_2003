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
TR	= translator
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
TRFLAGS = $(THROWBACK) -o "´´" -c "ªª" -htmlo "êê" -htmlc "ëë" -t Translate.<UserIF>.<Locale>.Tokenised

#
# Program specific options:
#
COMPONENT = NCFresco
TARGET    = aof.${COMPONENT}
TARGETU   = aof.${COMPONENT}u

ABSSYM    = RISC_OSLib:o.c_abssym

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
	${CP} NCFrescoRORes:MimeMap     ${RESRES}.MimeMap  ${CPFLAGS}
	${CP} NCFrescoRORes:Config      ${RESRES}.Config   ${CPFLAGS}
	${CP} NCFrescoRORes:html.badurl    ${RESRES}.html.badurl    ${CPFLAGS}
	${CP} NCFrescoRORes:html.favs      ${RESRES}.html.favs      ${CPFLAGS}
	${CP} NCFrescoRORes:html.favsd     ${RESRES}.html.favsd     ${CPFLAGS}
	${CP} NCFrescoRORes:html.hista     ${RESRES}.html.hista     ${CPFLAGS}
	${CP} NCFrescoRORes:html.histr     ${RESRES}.html.histr     ${CPFLAGS}
	${CP} NCFrescoRORes:html.jsalert   ${RESRES}.html.jsalert   ${CPFLAGS}
	${CP} NCFrescoRORes:html.jsprompt  ${RESRES}.html.jsprompt  ${CPFLAGS}
	${CP} NCFrescoRORes:html.jsokcncl  ${RESRES}.html.jsokcncl  ${CPFLAGS}
	${CP} NCFrescoRORes:html.password  ${RESRES}.html.password  ${CPFLAGS}
	${CP} NCFrescoRORes:html.printopts ${RESRES}.html.printopts ${CPFLAGS}
	${CP} NCFrescoRORes:html.sslcert   ${RESRES}.html.sslcert   ${CPFLAGS}
	${CP} NCFrescoRORes:html.sslpin    ${RESRES}.html.sslpin    ${CPFLAGS}
	${CP} NCFrescoRORes:html.sslserv   ${RESRES}.html.sslserv   ${CPFLAGS}
	${CP} NCFrescoRORes:html.ssltoken  ${RESRES}.html.ssltoken  ${CPFLAGS}
	${CP} NCFrescoRORes:html.url       ${RESRES}.html.url       ${CPFLAGS}
	Unset NCFrescoRORes$Path
        @echo ${COMPONENT}: resource files copied

resources_translated:
	Set NCFrescoRORes$Path Resources.<UserIF>.<Locale>.,Resources.Common.<Locale>.,Resources.<UserIF>.UK.,Resources.Common.UK.
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.html
	${CP} NCFrescoRORes:!BootROM    ${RESAPP}.!Boot    ${CPFLAGS}
	${CP} NCFrescoRORes:!RunROM     ${RESAPP}.!Run     ${CPFLAGS}
	${CP} NCFrescoRORes:!SpritesROM ${RESAPP}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:SpritesROM  ${RESRES}.Sprites  ${CPFLAGS}
	${TR} -m TranslateRes:Messages  -s ${RESRES}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res         ${RESRES}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:Welcome     ${RESRES}.Welcome  ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     ${RESRES}.MimeMap  ${CPFLAGS}
	${CP} NCFrescoRORes:Config      ${RESRES}.Config   ${CPFLAGS}
	${TR} -m TranslateRes:html.badurl    -s ${RESRES}.html.badurl    ${TRFLAGS}
	${TR} -m TranslateRes:html.favs      -s ${RESRES}.html.favs      ${TRFLAGS}
	${TR} -m TranslateRes:html.favsd     -s ${RESRES}.html.favsd     ${TRFLAGS}
	${TR} -m TranslateRes:html.hista     -s ${RESRES}.html.hista     ${TRFLAGS}
	${TR} -m TranslateRes:html.histr     -s ${RESRES}.html.histr     ${TRFLAGS}
	${TR} -m TranslateRes:html.jsalert   -s ${RESRES}.html.jsalert   ${TRFLAGS}
	${TR} -m TranslateRes:html.jsprompt  -s ${RESRES}.html.jsprompt  ${TRFLAGS}
	${TR} -m TranslateRes:html.jsokcncl  -s ${RESRES}.html.jsokcncl  ${TRFLAGS}
	${TR} -m TranslateRes:html.optfonts  -s ${RESRES}.html.optfonts  ${TRFLAGS}
	${TR} -m TranslateRes:html.opttext   -s ${RESRES}.html.opttext  ${TRFLAGS}
	${TR} -m TranslateRes:html.password  -s ${RESRES}.html.password  ${TRFLAGS}
	${TR} -m TranslateRes:html.printopts -s ${RESRES}.html.printopts ${TRFLAGS}
	${TR} -m TranslateRes:html.sslcert   -s ${RESRES}.html.sslcert   ${TRFLAGS}
	${TR} -m TranslateRes:html.sslpin    -s ${RESRES}.html.sslpin    ${TRFLAGS}
	${TR} -m TranslateRes:html.sslserv   -s ${RESRES}.html.sslserv   ${TRFLAGS}
	${TR} -m TranslateRes:html.ssltoken  -s ${RESRES}.html.ssltoken  ${TRFLAGS}
	${TR} -m TranslateRes:html.url       -s ${RESRES}.html.url       ${TRFLAGS}
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
