# Makefile for NCFresco - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!NCFresco
RESRES = <Resource$Dir>.Resources.NCFresco
RESRES_ALT = <Resource$Dir>.Resources.NCFrescoA
INSTDIR = <Install$Dir>.!NCFresco
#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
SQ      = squeeze
RM      = remove
LD	= link
TR	= translate
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
TRFLAGS = $(THROWBACK) -marko "´´" -markc "ªª" -htmlo "êê" -htmlc "ëë" -m NCFrescoROTransRes:Mappings
SQFLAGS = 

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

resources: resources_${RES} resources_base
        @echo ${COMPONENT}: all resource files copied

resources_:
        @echo ${COMPONENT}: dummy alt resource files copied

resources_alt:
	Set NCFrescoRORes$Path Resources.<UserIF>.<Locale>.,Resources.Common.<Locale>.,Resources.<UserIF>.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.<UserIF>.<Locale>.,Translate.Common.<Locale>.,Translate.<UserIF>.UK.,Translate.Common.UK.
	Set NCFrescoROAltRes$Path NCFrescoRORes:Alt.,NCFrescoRORes:
	Set NCFrescoROAltTransRes$Path NCFrescoROTransRes:Alt.,NCFrescoROTransRes:
	${MKDIR} ${RESRES_ALT}
	${CP} NCFrescoROAltRes:SpritesROM          ${RESRES_ALT}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROAltTransRes:Messages -o ${RESRES_ALT}.Messages ${TRFLAGS}
	${CP} NCFrescoROAltRes:Res                 ${RESRES_ALT}.Res      ${CPFLAGS}
	Unset NCFrescoRORes$Path
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoROAltRes$Path
	Unset NCFrescoROAltTransRes$Path
        @echo ${COMPONENT}: alt resource files copied

resources_base:
	Set NCFrescoRORes$Path Resources.<UserIF>.<Locale>.,Resources.Common.<Locale>.,Resources.<UserIF>.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.<UserIF>.<Locale>.,Translate.Common.<Locale>.,Translate.<UserIF>.UK.,Translate.Common.UK.
	${MKDIR} ${RESAPP}
	${MKDIR} ${RESRES}
	${MKDIR} ${RESRES}.html
	${CP} NCFrescoRORes:!BootROM    	${RESAPP}.!Boot    ${CPFLAGS}
	${CP} NCFrescoRORes:!RunROM     	${RESAPP}.!Run     ${CPFLAGS}
	${CP} NCFrescoRORes:!SpritesROM 	${RESAPP}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:SpritesROM  	${RESRES}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${RESRES}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res         	${RESRES}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:Welcome     	${RESRES}.Welcome  ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     	${RESRES}.MimeMap  ${CPFLAGS}
	${CP} NCFrescoRORes:Config      	${RESRES}.Config   ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${RESRES}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${RESRES}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${RESRES}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${RESRES}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${RESRES}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsalert   -o ${RESRES}.html.jsalert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsprompt  -o ${RESRES}.html.jsprompt  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsokcncl  -o ${RESRES}.html.jsokcncl  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${RESRES}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${RESRES}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslcert   -o ${RESRES}.html.sslcert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslpin    -o ${RESRES}.html.sslpin    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslserv   -o ${RESRES}.html.sslserv   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.ssltoken  -o ${RESRES}.html.ssltoken  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${RESRES}.html.url       ${TRFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
        @echo ${COMPONENT}: resource files copied

install:
	Set NCFrescoRORes$Path Resources.<UserIF>.<Locale>.,Resources.Common.<Locale>.,Resources.<UserIF>.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.<UserIF>.<Locale>.,Translate.Common.<Locale>.,Translate.<UserIF>.UK.,Translate.Common.UK.
	${MKDIR} ${INSTDIR}
	${MKDIR} ${INSTDIR}.html
	${CP} NCFrescoRORes:!Boot    		${INSTDIR}.!Boot    ${CPFLAGS}
	IfThere NCFrescoRORes:!Help Then ${CP} NCFrescoRORes:!Help ${INSTDIR}.!Help  ${CPFLAGS}
	${CP} NCFrescoRORes:!Run     		${INSTDIR}.!Run     ${CPFLAGS}
	${CP} NCFrescoRORes:!Sprites 		${INSTDIR}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:Sprites  		${INSTDIR}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${INSTDIR}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res         	${INSTDIR}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:Welcome     	${INSTDIR}.Welcome  ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     	${INSTDIR}.MimeMap  ${CPFLAGS}
	-${CP} NCFrescoRORes:Config      	${INSTDIR}.Config   ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${INSTDIR}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${INSTDIR}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${INSTDIR}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${INSTDIR}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${INSTDIR}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsalert   -o ${INSTDIR}.html.jsalert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsprompt  -o ${INSTDIR}.html.jsprompt  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsokcncl  -o ${INSTDIR}.html.jsokcncl  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.optfonts  -o ${INSTDIR}.html.optfonts  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.opttext   -o ${INSTDIR}.html.opttext   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${INSTDIR}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${INSTDIR}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslcert   -o ${INSTDIR}.html.sslcert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslpin    -o ${INSTDIR}.html.sslpin    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslserv   -o ${INSTDIR}.html.sslserv   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.ssltoken  -o ${INSTDIR}.html.ssltoken  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${INSTDIR}.html.url       ${TRFLAGS}
	${SQ} abs.!RunImage-${ITYPE}                  ${INSTDIR}.!RunImage      ${SQFLAGS}
	Unset NCFrescoROTransRes$Path
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
