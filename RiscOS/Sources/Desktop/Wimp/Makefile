# Makefile for Wimp
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name    Description
# ----       ----    -----------
# 25-May-94  AMcC    Created.
# 18-Oct-94  AMcC    Template3D now copied to Resources as 'Templates'
#                    Now correctly copies Tools3D to Resources:...Tools
# 26-Jun-95  JRH     Now copies STB (ie hacked-down) versions of Tools,
#                    Templates and Sprites. Doesn't copy Sprites22
# 23-Oct-98  BJGA    Changed to use objasm, so that we can override the
#                    choice of options file using an argument to amu
# 03-Aug-99  BJGA    No longer produces build errors if some resources are absent
#

#
# Program specific options:
#
COMPONENT = WindowManager
TARGET    = Wimp
HEADER1   = ${TARGET}
HEADER2   = WimpSpace
RESOURCES = manual

SA_FLAGS  =
ASFLAGS   = -cpu 3 -NoWarn -PreDefine "Options SETS \"${OPTIONS}\""

include StdTools
include AAsmModule

resources: resources-wimp-${CMDHELP}
        @echo ${COMPONENT}: resource files copied

resources-wimp_common:
        ${MKDIR} ${RESFSDIR}
        IfThere LocalRes:<UserIF>.Messages  Then ${CP} LocalRes:<UserIF>.Messages  ${RESFSDIR}.Messages  ${CPFLAGS} Else IfThere LocalRes:Messages Then ${CP} LocalRes:Messages ${RESFSDIR}.Messages ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Templates Then ${CP} LocalRes:<UserIF>.Templates ${RESFSDIR}.Templates ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Sprites   Then ${CP} LocalRes:<UserIF>.Sprites   ${RESFSDIR}.Sprites   ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Sprites22 Then ${CP} LocalRes:<UserIF>.Sprites22 ${RESFSDIR}.Sprites22 ${CPFLAGS}
        IfThere LocalRes:<UserIF>.TileN     Then ${CP} LocalRes:<UserIF>.TileN     ${RESFSDIR}.TileN     ${CPFLAGS}
        IfThere LocalRes:<UserIF>.TileV     Then ${CP} LocalRes:<UserIF>.TileV     ${RESFSDIR}.TileV     ${CPFLAGS}
        IfThere LocalRes:<UserIF>.Tools     Then ${CP} LocalRes:<UserIF>.Tools     ${RESFSDIR}.Tools     ${CPFLAGS}

resources-wimp-None: resources-wimp_common
        @|

resources-wimp-: resources-wimp_common
        @IfThere LocalRes:CmdHelp Then ${CAT} LocalRes:CmdHelp { >> ${RESFSDIR}.Messages }

BBETYPE = wimp
bbe-wimp: bbe-generic-resources-get-alias
        BBE_Export_File_In_Dir Resources.${LOCALE} Messages
        BBE_Export_File_In_Dir Resources.${LOCALE} CmdHelp
        BBE_Export_Dir Resources.${LOCALE}.${USERIF}

# Dynamic dependencies:
