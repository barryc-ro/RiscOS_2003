#  Makefile for the various CJK support applets in the STB Baseline
# -================================================================-
#
# Initial version: 19th Feb 2002, SAR

COMPONENT = CJK
INSTAPP   = ${INSTDIR}.!${Locale}
LOCALRES  = Resources.${UserIF}.${Locale}

ROMFONTS  = ^.^.^.^.Video.Render.Fonts.ROMFonts.Fonts
TERRMOD   = ^.^.^.^.Internat.Territory.Module.Resources
IMEMOD    = ^.^.^.^.Internat.IME.${Locale}IME.Resources

IFTHERE   = IfThere
IFTHEN    = Then
IFELSE    = Else
CONCAT    = Do Print <123> >> 
CATFLAGS  = }

include Makefiles:StdTools

clean:
        @echo Component ${COMPONENT} (${Locale}) cleaned.

install:
        ${MKDIR} ${HTMLDIR}
        ${MKDIR} ${INSTAPP}.Fonts.Encodings
        ${MKDIR} ${INSTAPP}.Fonts.${INSTFONT}
        ${MKDIR} ${INSTAPP}.Modules
        ${MKDIR} ${INSTAPP}.Resources.${Locale}
        ${MKDIR} ${INSTAPP}.Resources.${Locale}IME
        ${CP}    ${LOCALRES}.${Locale}/html               ${HTMLDIR}.${Locale}/html                    ${CPFLAGS}
        ${CP}    ${LOCALRES}.!Boot                        ${INSTAPP}.!Boot                             ${CPFLAGS}
        ${CP}    ${LOCALRES}.!Run                         ${INSTAPP}.!Run                              ${CPFLAGS}
        ${CP}    ${ROMFONTS}.Encodings./Base${ENC}        ${INSTAPP}.Fonts.Encodings./Base${ENC}       ${CPFLAGS}
        ${CP}    ${ROMFONTS}.${INSTFONT}.IntMetri${ENC}   ${INSTAPP}.Fonts.${INSTFONT}.IntMetri${ENC}  ${CPFLAGS}
        ${CP}    ${ROMFONTS}.${INSTFONT}.Outlines${ENC}   ${INSTAPP}.Fonts.${INSTFONT}.Outlines${ENC}  ${CPFLAGS}
        ${IFTHERE} ${IMEMOD}.Dictionary           ${IFTHEN} ${CP}     ${IMEMOD}.Dictionary  ${INSTAPP}.Resources.${Locale}IME.Dictionary              ${CPFLAGS}
        ${IFTHERE} ${IMEMOD}                      ${IFTHEN} ${CP}     ${IMEMOD}.UK.Messages ${INSTAPP}.Resources.${Locale}IME.Messages                ${CPFLAGS}
        ${IFTHERE} ${TERRMOD}.${Locale}.Territory ${IFTHEN} ${CP}     ${TERRMOD}.${Locale}.Territory          ${INSTAPP}.Resources.${Locale}.Messages ${CPFLAGS}
        ${IFTHERE} ${TERRMOD}.${Locale}.Territory ${IFTHEN} ${CONCAT} ${INSTAPP}.Resources.${Locale}.Messages ${CATFLAGS} ${TERRMOD}.UK.Messages
        @echo Locale ${Locale} (${Locale}) installed (disc).

# Dynamic dependencies:
