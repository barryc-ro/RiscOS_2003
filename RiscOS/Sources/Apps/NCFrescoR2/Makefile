# Makefile for NCFresco2, HTTP2, WebFTP2 - resource export and final link only
#

#
# Paths
#
EXP_HDR = <CExport$Dir>
RESAPP = <Resource$Dir>.Apps.!NCFresco
RESRES = <Resource$Dir>.Resources.NCFresco
RESRES_ALT = <Resource$Dir>.Resources.NCFrescoA
INSTALLDIR = ${INSTDIR}.!NCFresco
#
# Generic options:
#
MKDIR   = mkdir -p
AS      = aasm
CP      = copy
SQ      = squeeze
RM      = remove
WIPE    = -wipe
LD      = link
TR      = translate
GETVERS = perl bin.GetVersion

CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module -To $@ -From
CPFLAGS = ~cfr~v
TRFLAGS = ${THROWBACK} -marko "{<" -markc ">}" -htmlo "êê" -htmlc "ëë" -m NCFrescoROTransRes:Mappings
SQFLAGS = 
WFLAGS  = ~CFR~V

#
# Program specific options:
#
COMPONENT ?= NCFresco2
TARGET    ?= NCFresco

ABSSYM    = RISC_OSLib:o.c_abssym

#
# Generic rules:
#
rom: 
        @echo ${COMPONENT}: rom module built

clean:
        ${WIPE} linked ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources: resources_${TARGET}

resources_:
        @echo Need a defined target!

resources_NCFresco: resources_NCFresco_${RES} resources_NCFresco_base
        @echo ${COMPONENT}: all resource files copied

resources_NCFresco_:
        @echo ${COMPONENT}: dummy alt resource files copied

resources_NCFresco_alt:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
	Set NCFrescoROAltRes$Path NCFrescoRORes:Alt.,NCFrescoRORes:
	Set NCFrescoROAltTransRes$Path NCFrescoROTransRes:Alt.,NCFrescoROTransRes:
	${MKDIR} ${RESRES_ALT}
	IfThere NCFrescoRORes:Alt.Sprites Then \
	  ${CP} NCFrescoRORes:Alt.Sprites          ${RESRES_ALT}.Sprites  ${CPFLAGS}
	${TR} -i NCFrescoROAltTransRes:Messages -o ${RESRES_ALT}.Messages ${TRFLAGS}
	${CP} NCFrescoROAltRes:Res                 ${RESRES_ALT}.Res      ${CPFLAGS}
	Unset NCFrescoRORes$Path
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoROAltRes$Path
	Unset NCFrescoROAltTransRes$Path
        @echo ${COMPONENT}: alt resource files copied

resources_NCFresco_base:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
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
	${CP} NCFrescoRORes:fontmap/txt 	${RESRES}.fontmap/txt ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${RESRES}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${RESRES}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${RESRES}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favscats  -o ${RESRES}.html.favscats  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${RESRES}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${RESRES}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${RESRES}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.alert     -o ${RESRES}.html.alert     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.prompt    -o ${RESRES}.html.prompt    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.okcncl    -o ${RESRES}.html.okcncl    ${TRFLAGS}
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

resources_HTTP:
        @echo ${COMPONENT}: resources files copied

resources_WebFTP:
        @echo ${COMPONENT}: resources files copied

install: install_${TARGET}

install_:
        @echo Need a defined target!

install_NCFresco:
	Set NCFrescoRORes$Path Resources.${USERIF}.${LOCALE}.${UI_VARIANT}.,Resources.${USERIF}.${LOCALE}.,Resources.Common.${LOCALE}.,Resources.${USERIF}.UK.${UI_VARIANT}.,Resources.${USERIF}.UK.,Resources.Common.UK.
	Set NCFrescoROTransRes$Path Translate.${USERIF}.${LOCALE}.${UI_VARIANT}.,Translate.${USERIF}.${LOCALE}.,Translate.Common.${LOCALE}.,Translate.${USERIF}.UK.${UI_VARIANT}.,Translate.${USERIF}.UK.,Translate.Common.UK.
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
	${CP} NCFrescoRORes:fontmap/txt  	${INSTALLDIR}.fontmap/txt ${CPFLAGS}
	${GETVERS} NCFrescoRORes:Welcome ${INSTALLDIR}.Welcome VersionNum
	${TR} -i NCFrescoROTransRes:html.badurl    -o ${INSTALLDIR}.html.badurl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favs      -o ${INSTALLDIR}.html.favs      ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favscats  -o ${INSTALLDIR}.html.favscats  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.favsd     -o ${INSTALLDIR}.html.favsd     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.hista     -o ${INSTALLDIR}.html.hista     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.histr     -o ${INSTALLDIR}.html.histr     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.alert     -o ${INSTALLDIR}.html.alert     ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.prompt    -o ${INSTALLDIR}.html.prompt    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.okcncl    -o ${INSTALLDIR}.html.okcncl    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.password  -o ${INSTALLDIR}.html.password  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.printopts -o ${INSTALLDIR}.html.printopts ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslcert   -o ${INSTALLDIR}.html.sslcert   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslpin    -o ${INSTALLDIR}.html.sslpin    ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.sslserv   -o ${INSTALLDIR}.html.sslserv   ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.ssltoken  -o ${INSTALLDIR}.html.ssltoken  ${TRFLAGS}
	${TR} -i NCFrescoROTransRes:html.url       -o ${INSTALLDIR}.html.url       ${TRFLAGS}
	${SQ} abs.!RunIm-${TYPE}                      ${INSTALLDIR}.!RunImage      ${SQFLAGS}
	Unset NCFrescoROTransRes$Path
	Unset NCFrescoRORes$Path
        @echo ${COMPONENT}: resource files copied

install_HTTP:
        ${CP} rm.HTTP ${INSTDIR}.${TARGET}mod ${CPFLAGS}
        ${CP} rm.HTTPs ${INSTDIR}.${TARGET}smod ${CPFLAGS}
        ${CP} rm.HTTPsx ${INSTDIR}.${TARGET}sxmod ${CPFLAGS}

install_WebFTP:
        ${CP} rm.WebFTP ${INSTDIR}.${TARGET} ${CPFLAGS}

export: export_${TARGET}

export_NCFresco: export_NCFresco_${PHASE}

export_NCFresco_libs:
        @echo ${COMPONENT}: export complete(libs)

export_NCFresco_hdrs:
        ${CP} h.plugin ${EXP_HDR}.h.plugin ${CPFLAGS}
        ${CP} h.ANT_URL ${EXP_HDR}.h.ANT_URL ${CPFLAGS}
        ${CP} h.ANT_Msg ${EXP_HDR}.h.ANT_Msg ${CPFLAGS}
        @echo ${COMPONENT}: export complete(hdrs)

export_HTTP:
        @echo ${COMPONENT}: export complete

export_WebFTP:
        @echo ${COMPONENT}: export complete

#
# Final link for the ROM Image (using given base address)
#

rom_link: rom_link_${TARGET}

rom_link_NCFresco: dirs
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} aof.ncfre-${TYPE} ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link for ${TYPE} build complete

rom_link_HTTP: dirs
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} aof.HTTP${TYPE} ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link for ${TYPE} build complete

rom_link_WebFTP: dirs
        ${LD} -o linked.${TARGET} -rmf -base ${ADDRESS} aof.WebFTP ${ABSSYM}
        ${CP} linked.${TARGET} ${LINKDIR}.${TARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

dirs:
        ${MKDIR} linked

# Dynamic dependencies:
