# Makefile for NCFresco - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!NCFresco
RESRES = <Resource$Dir>.Resources.NCFresco
RESRES_ALT = <Resource$Dir>.Resources.NCFrescoA
INSTALLDIR = $(INSTDIR).!NCFresco
#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
CP      = copy
SQ      = squeeze
RM      = remove
LD	= link
TR	= translate
GETVERS = perl bin.GetVersion

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
        ${CP} ${TARGET} ${INSTALLDIR}.${COMPONENT} ${CPFLAGS}
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
	${CP} NCFrescoROAltRes:Sprites             ${RESRES_ALT}.Sprites  ${CPFLAGS}
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
	${CP} NCFrescoRORes:!Sprites    	${RESAPP}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:Sprites     	${RESRES}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${RESRES}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res         	${RESRES}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     	${RESRES}.MimeMap  ${CPFLAGS}
	${CP} NCFrescoRORes:Config      	${RESRES}.Config   ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${RESRES}.Welcome VersionNum
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
	${MKDIR} ${INSTALLDIR}
	${MKDIR} ${INSTALLDIR}.html
	${CP} NCFrescoRORes:!Boot    		${INSTALLDIR}.!Boot    ${CPFLAGS}
	IfThere NCFrescoRORes:!Help Then ${CP} NCFrescoRORes:!Help ${INSTALLDIR}.!Help  ${CPFLAGS}
	${CP} NCFrescoRORes:!Run     		${INSTALLDIR}.!Run     ${CPFLAGS}
	${CP} NCFrescoRORes:!Sprites 		${INSTALLDIR}.!Sprites ${CPFLAGS}
	${CP} NCFrescoRORes:Sprites  		${INSTALLDIR}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROTransRes:Messages -o ${INSTALLDIR}.Messages ${TRFLAGS}
	${CP} NCFrescoRORes:Res         	${INSTALLDIR}.Res      ${CPFLAGS}
	${CP} NCFrescoRORes:MimeMap     	${INSTALLDIR}.MimeMap  ${CPFLAGS}
	-${CP} NCFrescoRORes:Config      	${INSTALLDIR}.Config   ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${INSTALLDIR}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${INSTALLDIR}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${INSTALLDIR}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${INSTALLDIR}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${INSTALLDIR}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${INSTALLDIR}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsalert   -o ${INSTALLDIR}.html.jsalert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsprompt  -o ${INSTALLDIR}.html.jsprompt  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.jsokcncl  -o ${INSTALLDIR}.html.jsokcncl  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.optfonts  -o ${INSTALLDIR}.html.optfonts  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.opttext   -o ${INSTALLDIR}.html.opttext   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${INSTALLDIR}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${INSTALLDIR}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslcert   -o ${INSTALLDIR}.html.sslcert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslpin    -o ${INSTALLDIR}.html.sslpin    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslserv   -o ${INSTALLDIR}.html.sslserv   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.ssltoken  -o ${INSTALLDIR}.html.ssltoken  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${INSTALLDIR}.html.url       ${TRFLAGS}
	${SQ} abs.!RunIma-${ITYPE}                  ${INSTALLDIR}.!RunImage      ${SQFLAGS}
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
