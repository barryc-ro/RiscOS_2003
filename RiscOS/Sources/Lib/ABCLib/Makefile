# Makefile for ABCLib
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Jun-00  SNB          Created.
#

#
# Paths
#
LIBDIR = <Lib$Dir>.ABCLib

#
# Generic options:
#
include Makefiles:StdTools

VIA   = Via
MAKE  = @amu_machine -f ^.^.SubMake VIA=^.^.${VIA}

#
# Program specific options:
#
COMPONENT  = ABCLib
TARGET     = alf.${COMPONENT}
LIBS       = ${TARGET}

LOCALDIRS  = alf._dirs

#
# RISC OS ROM build rules:
#
rom: export_libs
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: export_libs
        @echo ${COMPONENT}: rom module installed

all:    export_libs export_hdrs
        @echo ${COMPONENT}: built

clean:
        ${CD} src.action     ; ${MAKE} -f ^.^.SubMake clean DIR=src.action     ; ${CD} ^.^
        ${CD} src.adjuster   ; ${MAKE} -f ^.^.SubMake clean DIR=src.adjuster   ; ${CD} ^.^
        ${CD} src.button     ; ${MAKE} -f ^.^.SubMake clean DIR=src.button     ; ${CD} ^.^
        ${CD} src.colourdbox ; ${MAKE} -f ^.^.SubMake clean DIR=src.colourdbox ; ${CD} ^.^
        ${CD} src.colourmenu ; ${MAKE} -f ^.^.SubMake clean DIR=src.colourmenu ; ${CD} ^.^
        ${CD} src.dcs        ; ${MAKE} -f ^.^.SubMake clean DIR=src.dcs        ; ${CD} ^.^
        ${CD} src.display    ; ${MAKE} -f ^.^.SubMake clean DIR=src.display    ; ${CD} ^.^
        ${CD} src.draggable  ; ${MAKE} -f ^.^.SubMake clean DIR=src.draggable  ; ${CD} ^.^
        ${CD} src.fileinfo   ; ${MAKE} -f ^.^.SubMake clean DIR=src.fileinfo   ; ${CD} ^.^
        ${CD} src.fontdbox   ; ${MAKE} -f ^.^.SubMake clean DIR=src.fontdbox   ; ${CD} ^.^
        ${CD} src.fontmenu   ; ${MAKE} -f ^.^.SubMake clean DIR=src.fontmenu   ; ${CD} ^.^
        ${CD} src.gadget     ; ${MAKE} -f ^.^.SubMake clean DIR=src.gadget     ; ${CD} ^.^
        ${CD} src.iconbar    ; ${MAKE} -f ^.^.SubMake clean DIR=src.iconbar    ; ${CD} ^.^
        ${CD} src.menu       ; ${MAKE} -f ^.^.SubMake clean DIR=src.menu       ; ${CD} ^.^
        ${CD} src.numrange   ; ${MAKE} -f ^.^.SubMake clean DIR=src.numrange   ; ${CD} ^.^
        ${CD} src.object     ; ${MAKE} -f ^.^.SubMake clean DIR=src.object     ; ${CD} ^.^
        ${CD} src.option     ; ${MAKE} -f ^.^.SubMake clean DIR=src.option     ; ${CD} ^.^
        ${CD} src.popup      ; ${MAKE} -f ^.^.SubMake clean DIR=src.popup      ; ${CD} ^.^
        ${CD} src.printdbox  ; ${MAKE} -f ^.^.SubMake clean DIR=src.printdbox  ; ${CD} ^.^
        ${CD} src.proginfo   ; ${MAKE} -f ^.^.SubMake clean DIR=src.proginfo   ; ${CD} ^.^
        ${CD} src.quit       ; ${MAKE} -f ^.^.SubMake clean DIR=src.quit       ; ${CD} ^.^
        ${CD} src.radio      ; ${MAKE} -f ^.^.SubMake clean DIR=src.radio      ; ${CD} ^.^
        ${CD} src.saveas     ; ${MAKE} -f ^.^.SubMake clean DIR=src.saveas     ; ${CD} ^.^
        ${CD} src.scale      ; ${MAKE} -f ^.^.SubMake clean DIR=src.scale      ; ${CD} ^.^
        ${CD} src.slider     ; ${MAKE} -f ^.^.SubMake clean DIR=src.slider     ; ${CD} ^.^
        ${CD} src.stringset  ; ${MAKE} -f ^.^.SubMake clean DIR=src.stringset  ; ${CD} ^.^
        ${CD} src.task       ; ${MAKE} -f ^.^.SubMake clean DIR=src.task       ; ${CD} ^.^
        ${CD} src.toolbox    ; ${MAKE} -f ^.^.SubMake clean DIR=src.toolbox    ; ${CD} ^.^
        ${CD} src.window     ; ${MAKE} -f ^.^.SubMake clean DIR=src.window     ; ${CD} ^.^
        ${CD} src.writable   ; ${MAKE} -f ^.^.SubMake clean DIR=src.writable   ; ${CD} ^.^
        ${WIPE} alf ${WFLAGS}
        ${RM} ${VIA}
        @echo ${COMPONENT}: cleaned

export_hdrs: export_dirs
        ${CP} h          ${LIBDIR}.h           ${CPFLAGS}
        ${CP} VersionNum ${LIBDIR}.LibVersion  ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} export_dirs ${LOCALDIRS}
        ${CP} alf ${LIBDIR}.o ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

${LOCALDIRS}:
        ${MKDIR} alf
        ${TOUCH} $@

export_dirs:
        ${MKDIR} ${LIBDIR}

${LIBS}: NormalLib
        @|

NormalLib:
        ${RM} ${VIA}
        ${TOUCH} ${VIA}
        ${SETTYPE} ${VIA} Text
        ${CD} src.action     ; ${MAKE} -f ^.^.SubMake all DIR=src.action     ; ${CD} ^.^
        ${CD} src.adjuster   ; ${MAKE} -f ^.^.SubMake all DIR=src.adjuster   ; ${CD} ^.^
        ${CD} src.button     ; ${MAKE} -f ^.^.SubMake all DIR=src.button     ; ${CD} ^.^
        ${CD} src.colourdbox ; ${MAKE} -f ^.^.SubMake all DIR=src.colourdbox ; ${CD} ^.^
        ${CD} src.colourmenu ; ${MAKE} -f ^.^.SubMake all DIR=src.colourmenu ; ${CD} ^.^
        ${CD} src.dcs        ; ${MAKE} -f ^.^.SubMake all DIR=src.dcs        ; ${CD} ^.^
        ${CD} src.display    ; ${MAKE} -f ^.^.SubMake all DIR=src.display    ; ${CD} ^.^
        ${CD} src.draggable  ; ${MAKE} -f ^.^.SubMake all DIR=src.draggable  ; ${CD} ^.^
        ${CD} src.fileinfo   ; ${MAKE} -f ^.^.SubMake all DIR=src.fileinfo   ; ${CD} ^.^
        ${CD} src.fontdbox   ; ${MAKE} -f ^.^.SubMake all DIR=src.fontdbox   ; ${CD} ^.^
        ${CD} src.fontmenu   ; ${MAKE} -f ^.^.SubMake all DIR=src.fontmenu   ; ${CD} ^.^
        ${CD} src.gadget     ; ${MAKE} -f ^.^.SubMake all DIR=src.gadget     ; ${CD} ^.^
        ${CD} src.iconbar    ; ${MAKE} -f ^.^.SubMake all DIR=src.iconbar    ; ${CD} ^.^
        ${CD} src.menu       ; ${MAKE} -f ^.^.SubMake all DIR=src.menu       ; ${CD} ^.^
        ${CD} src.numrange   ; ${MAKE} -f ^.^.SubMake all DIR=src.numrange   ; ${CD} ^.^
        ${CD} src.object     ; ${MAKE} -f ^.^.SubMake all DIR=src.object     ; ${CD} ^.^
        ${CD} src.option     ; ${MAKE} -f ^.^.SubMake all DIR=src.option     ; ${CD} ^.^
        ${CD} src.popup      ; ${MAKE} -f ^.^.SubMake all DIR=src.popup      ; ${CD} ^.^
        ${CD} src.printdbox  ; ${MAKE} -f ^.^.SubMake all DIR=src.printdbox  ; ${CD} ^.^
        ${CD} src.proginfo   ; ${MAKE} -f ^.^.SubMake all DIR=src.proginfo   ; ${CD} ^.^
        ${CD} src.quit       ; ${MAKE} -f ^.^.SubMake all DIR=src.quit       ; ${CD} ^.^
        ${CD} src.radio      ; ${MAKE} -f ^.^.SubMake all DIR=src.radio      ; ${CD} ^.^
        ${CD} src.saveas     ; ${MAKE} -f ^.^.SubMake all DIR=src.saveas     ; ${CD} ^.^
        ${CD} src.scale      ; ${MAKE} -f ^.^.SubMake all DIR=src.scale      ; ${CD} ^.^
        ${CD} src.slider     ; ${MAKE} -f ^.^.SubMake all DIR=src.slider     ; ${CD} ^.^
        ${CD} src.stringset  ; ${MAKE} -f ^.^.SubMake all DIR=src.stringset  ; ${CD} ^.^
        ${CD} src.task       ; ${MAKE} -f ^.^.SubMake all DIR=src.task       ; ${CD} ^.^
        ${CD} src.toolbox    ; ${MAKE} -f ^.^.SubMake all DIR=src.toolbox    ; ${CD} ^.^
        ${CD} src.window     ; ${MAKE} -f ^.^.SubMake all DIR=src.window     ; ${CD} ^.^
        ${CD} src.writable   ; ${MAKE} -f ^.^.SubMake all DIR=src.writable   ; ${CD} ^.^
        ${AR} -c -o ${TARGET} -v ${VIA}
        

# Dynamic dependencies:
