# Makefile for Printer Definition files (PrintDefs)
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 07-Nov-94  AMcC   Updated for Black build
# 08-Nov-94  MJS    Added Stylus-Col, Stylus-Cmw, BJC-4000, DJ560C PDF's
# 30-Nov-94  MJS    Fix omission of Stylus-Cmw in install
# 30-Nov-94  MJS    Add Acorn JP-360
# 14-Dec-94  MJS    Remove Acorn JP-360 (!)
# 08-Mar-95  MJS    Add Olivetti
# 24-Jun-95  PJC    Add Canon LBP8 Mk III
#

#
# Program specific options:
#
COMPONENT  = PrintDefs
DDIR       = PrntDefn
LDIR       = ${LOCALE}
INSTDIR    = ${INSTDIR}.Printers

# Generic options:
#
MKDIR   = cdir
CP      = copy
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Printers - sorted by manufacturer, then by BitImage or PostScript
#          - the first entry in each group is a Read_Me file

FILES =\
 Version\
 ${LDIR}.Read_Me\
 ${LDIR}.Acorn\
 ${DDIR}.Acorn.BI.JP-150\
 ${LDIR}.Apple\
 ${DDIR}.Apple.BI.ImgWriteII\
 ${DDIR}.Apple.PS.LW\
 ${DDIR}.Apple.PS.LW+\
 ${DDIR}.Apple.PS.LW-II-NT\
 ${DDIR}.Apple.PS.LW-II-NTX\
 ${LDIR}.Canon\
 ${DDIR}.Canon.BI.BJ-130e\
 ${DDIR}.Canon.BI.BJC-4000\
 ${DDIR}.Canon.BI.BJC-600\
 ${DDIR}.Canon.BI.BJC-800\
 ${DDIR}.Canon.BI.BubbleJet\
 ${DDIR}.Canon.PS.LPB8MkIII.LPB8MkIII\
 ${DDIR}.Canon.PS.LPB8MkIII.FeedInst\
 ${DDIR}.Canon.PS.LPB8MkIII.Feeds.1Upper\
 ${DDIR}.Canon.PS.LPB8MkIII.Feeds.2Lower\
 ${DDIR}.Canon.PS.LPB8MkIII.Feeds.3Mixed\
 ${DDIR}.Canon.PS.LPB8MkIII.Feeds.4Duplex\
 ${DDIR}.Canon.PS.LPB8MkIII.Feeds.5Tumble\
 ${DDIR}.Canon.PS.LPB8MkIII.Feeds.6Manual\
 ${LDIR}.Citizen\
 ${DDIR}.Citizen.BI.Swift-24\
 ${LDIR}.Epson\
 ${DDIR}.Epson.BI.EX-800\
 ${DDIR}.Epson.BI.FX-80\
 ${DDIR}.Epson.BI.FX-85\
 ${DDIR}.Epson.BI.JX-80\
 ${DDIR}.Epson.BI.LQ-860\
 ${DDIR}.Epson.BI.LX-100\
 ${DDIR}.Epson.BI.LX-80\
 ${DDIR}.Epson.BI.MX-80-II\
 ${DDIR}.Epson.BI.MX-80-III\
 ${DDIR}.Epson.BI.Stylus-800\
 ${DDIR}.Epson.BI.Stylus-Cmw\
 ${DDIR}.Epson.BI.Stylus-Col\
 ${LDIR}.Fujitsu\
 ${DDIR}.Generic.BI.TextOnly\
 ${DDIR}.Generic.BI.Top_Left\
 ${DDIR}.Generic.PS.PostScript\
 ${DDIR}.Generic.PS.PstScrptFP\
 ${LDIR}.HP\
 ${DDIR}.HP.BI.DeskJet\
 ${DDIR}.HP.BI.DeskJet+\
 ${DDIR}.HP.BI.DJ500C\
 ${DDIR}.HP.BI.DJ550C\
 ${DDIR}.HP.BI.DJ560C\
 ${DDIR}.HP.BI.LasJet-4\
 ${DDIR}.HP.BI.LasJet-II\
 ${DDIR}.HP.BI.LasJet-III\
 ${DDIR}.HP.BI.PaintJet\
 ${DDIR}.HP.PS.LasJet4-PS\
 ${DDIR}.HP.PS.LJ-II-PS\
 ${DDIR}.HP.PS.LJ-IID-PS\
 ${DDIR}.HP.PS.LJ-IIID-PS\
 ${LDIR}.IBM\
 ${DDIR}.IBM.BI.Pro-X24E\
 ${DDIR}.IBM.PS.4019_17\
 ${DDIR}.IBM.PS.4019_39\
 ${DDIR}.IBM.PS.4216\
 ${LDIR}.Integrex\
 ${DDIR}.Integrex.BI.ColJet132\
 ${DDIR}.Integrex.BI.ColJet2000\
 ${LDIR}.Linotype\
 ${DDIR}.Linotype.PS.100\
 ${DDIR}.Linotype.PS.200\
 ${LDIR}.Manne-Tall\
 ${DDIR}.Manne-Tall.BI.MT-83-84\
 ${LDIR}.NEC\
 ${DDIR}.NEC.BI.P6-Plus\
 ${LDIR}.Olivetti\
 ${LDIR}.QMS\
 ${DDIR}.QMS.PS.CS-100\
 ${LDIR}.Qume\
 ${DDIR}.Qume.PS.ScripTEN\
 ${LDIR}.Panasonic\
 ${LDIR}.Star\
 ${DDIR}.Star.BI.DP-510\
 ${DDIR}.Star.BI.LC-200\
 ${DDIR}.Star.BI.LC24-200\
 ${DDIR}.Star.BI.SJ-144\
 ${LDIR}.TI\
 ${DDIR}.TI.PS.OmniLaser

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: All built (Disc}

install: ${FILES}
        |
        ${MKDIR} ${INSTDIR}
        ${CP} Version                        ${INSTDIR}.Version     ${CPFLAGS}
        ${CP} ${LDIR}.Read_Me                ${INSTDIR}.Read_Me     ${CPFLAGS}
        ${CP} ${DDIR}.Generic.PS.PostScript  ${INSTDIR}.PostScript  ${CPFLAGS}
        ${CP} ${DDIR}.Generic.PS.PstScrptFP  ${INSTDIR}.PstScrptFP  ${CPFLAGS}
        ${CP} ${DDIR}.Generic.BI.TextOnly    ${INSTDIR}.TextOnly    ${CPFLAGS}
        ${CP} ${DDIR}.Generic.BI.Top_Left    ${INSTDIR}.Top_left    ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Acorn
        ${CP} ${LDIR}.Acorn                  ${INSTDIR}.Acorn.Read_Me ${CPFLAGS}
        ${CP} ${DDIR}.Acorn.BI.JP-150        ${INSTDIR}.Acorn.JP-150  ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Apple
        ${CP} ${LDIR}.Apple                  ${INSTDIR}.Apple.Read_Me    ${CPFLAGS}
        ${CP} ${DDIR}.Apple.BI.ImgWriteII    ${INSTDIR}.Apple.ImgWriteII ${CPFLAGS}
        ${CP} ${DDIR}.Apple.PS.LW            ${INSTDIR}.Apple.LW         ${CPFLAGS}
        ${CP} ${DDIR}.Apple.PS.LW+           ${INSTDIR}.Apple.LW+        ${CPFLAGS}
        ${CP} ${DDIR}.Apple.PS.LW-II-NT      ${INSTDIR}.Apple.LW-II-NT   ${CPFLAGS}
        ${CP} ${DDIR}.Apple.PS.LW-II-NTX     ${INSTDIR}.Apple.LW-II-NTX  ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Canon
        ${CP} ${LDIR}.Canon                            ${INSTDIR}.Canon.Read_Me                ${CPFLAGS}
        ${CP} ${DDIR}.Canon.BI.BJC-4000                ${INSTDIR}.Canon.BJC-4000               ${CPFLAGS}
        ${CP} ${DDIR}.Canon.BI.BJ-130e                 ${INSTDIR}.Canon.BJ-130e                ${CPFLAGS}
        ${CP} ${DDIR}.Canon.BI.BJC-600                 ${INSTDIR}.Canon.BJC-600                ${CPFLAGS}
        ${CP} ${DDIR}.Canon.BI.BJC-800                 ${INSTDIR}.Canon.BJC-800                ${CPFLAGS}
        ${CP} ${DDIR}.Canon.BI.BubbleJet               ${INSTDIR}.Canon.BubbleJet              ${CPFLAGS}
        ${MKDIR} ${INSTDIR}.Canon.LPB8MkIII
        ${MKDIR} ${INSTDIR}.Canon.LPB8MkIII.Feeds
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.LPB8MkIII     ${INSTDIR}.Canon.LPB8MkIII.LPB8MkIII     ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.FeedInst      ${INSTDIR}.Canon.LPB8MkIII.FeedInst      ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.Feeds.1Upper  ${INSTDIR}.Canon.LPB8MkIII.Feeds.1Upper  ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.Feeds.2Lower  ${INSTDIR}.Canon.LPB8MkIII.Feeds.2Lower  ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.Feeds.3Mixed  ${INSTDIR}.Canon.LPB8MkIII.Feeds.3Mixed  ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.Feeds.4Duplex ${INSTDIR}.Canon.LPB8MkIII.Feeds.4Duplex ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.Feeds.5Tumble ${INSTDIR}.Canon.LPB8MkIII.Feeds.5Tumble ${CPFLAGS}
        ${CP} ${DDIR}.Canon.PS.LPB8MkIII.Feeds.6Manual ${INSTDIR}.Canon.LPB8MkIII.Feeds.6Manual ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Citizen
        ${CP} ${LDIR}.Citizen                ${INSTDIR}.Citizen.Read_Me  ${CPFLAGS}
        ${CP} ${DDIR}.Citizen.BI.Swift-24    ${INSTDIR}.Citizen.Swift-24 ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Epson
        ${CP} ${LDIR}.Epson                  ${INSTDIR}.Epson.Read_Me    ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.EX-800        ${INSTDIR}.Epson.EX-800     ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.FX-80         ${INSTDIR}.Epson.FX-80      ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.FX-85         ${INSTDIR}.Epson.FX-85      ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.JX-80         ${INSTDIR}.Epson.JX-80      ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.LQ-860        ${INSTDIR}.Epson.LQ-860     ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.LX-100        ${INSTDIR}.Epson.LX-100     ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.LX-80         ${INSTDIR}.Epson.LX-80      ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.MX-80-II      ${INSTDIR}.Epson.MX-80-II   ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.MX-80-III     ${INSTDIR}.Epson.MX-80-III  ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.Stylus-800    ${INSTDIR}.Epson.Stylus-800 ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.Stylus-Col    ${INSTDIR}.Epson.Stylus-Col ${CPFLAGS}
        ${CP} ${DDIR}.Epson.BI.Stylus-Cmw    ${INSTDIR}.Epson.Stylus-Cmw ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Fujitsu
        ${CP} ${LDIR}.Fujitsu                ${INSTDIR}.Fujitsu.Read_Me  ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.HP
        ${CP} ${LDIR}.HP                     ${INSTDIR}.HP.Read_Me       ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.DeskJet          ${INSTDIR}.HP.DeskJet       ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.DeskJet+         ${INSTDIR}.HP.DeskJet+      ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.DJ500C           ${INSTDIR}.HP.DJ500C        ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.DJ550C           ${INSTDIR}.HP.DJ550C        ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.DJ560C           ${INSTDIR}.HP.DJ560C        ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.LasJet-4         ${INSTDIR}.HP.LasJet-4      ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.LasJet-II        ${INSTDIR}.HP.LasJet-II     ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.LasJet-III       ${INSTDIR}.HP.LasJet-III    ${CPFLAGS}
        ${CP} ${DDIR}.HP.PS.LasJet4-PS       ${INSTDIR}.HP.LasJet4-PS    ${CPFLAGS}
        ${CP} ${DDIR}.HP.PS.LJ-II-PS         ${INSTDIR}.HP.LJ-II-PS      ${CPFLAGS}
        ${CP} ${DDIR}.HP.PS.LJ-IID-PS        ${INSTDIR}.HP.LJ-IID-PS     ${CPFLAGS}
        ${CP} ${DDIR}.HP.PS.LJ-IIID-PS       ${INSTDIR}.HP.LJ-IIID-PS    ${CPFLAGS}
        ${CP} ${DDIR}.HP.BI.PaintJet         ${INSTDIR}.HP.PaintJet      ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.IBM
        ${CP} ${LDIR}.IBM                    ${INSTDIR}.IBM.Read_Me      ${CPFLAGS}
        ${CP} ${DDIR}.IBM.PS.4019_17         ${INSTDIR}.IBM.4019/17      ${CPFLAGS}
        ${CP} ${DDIR}.IBM.PS.4019_39         ${INSTDIR}.IBM.4019/39      ${CPFLAGS}
        ${CP} ${DDIR}.IBM.PS.4216            ${INSTDIR}.IBM.4216         ${CPFLAGS}
        ${CP} ${DDIR}.IBM.BI.Pro-X24E        ${INSTDIR}.IBM.Pro-X24E     ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Integrex
        ${CP} ${LDIR}.Integrex               ${INSTDIR}.Integrex.Read_Me    ${CPFLAGS}
        ${CP} ${DDIR}.Integrex.BI.ColJet132  ${INSTDIR}.Integrex.ColJet132  ${CPFLAGS}
        ${CP} ${DDIR}.Integrex.BI.ColJet2000 ${INSTDIR}.Integrex.ColJet2000 ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Linotype
        ${CP} ${LDIR}.Linotype               ${INSTDIR}.Linotype.Read_Me ${CPFLAGS}
        ${CP} ${DDIR}.Linotype.PS.100        ${INSTDIR}.Linotype.100     ${CPFLAGS}
        ${CP} ${DDIR}.Linotype.PS.200        ${INSTDIR}.Linotype.200     ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Manne-Tall
        ${CP} ${LDIR}.Manne-Tall             ${INSTDIR}.Manne-Tall.Read_Me  ${CPFLAGS}
        ${CP} ${DDIR}.Manne-Tall.BI.MT-83-84 ${INSTDIR}.Manne-Tall.MT-83/84 ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.NEC
        ${CP} ${LDIR}.NEC                    ${INSTDIR}.NEC.Read_Me    ${CPFLAGS}
        ${CP} ${DDIR}.NEC.BI.P6-Plus         ${INSTDIR}.NEC.P6-Plus    ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Olivetti
        ${CP} ${LDIR}.Olivetti               ${INSTDIR}.Olivetti.Read_Me ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.QMS
        ${CP} ${LDIR}.QMS                    ${INSTDIR}.QMS.Read_Me    ${CPFLAGS}
        ${CP} ${DDIR}.QMS.PS.CS-100          ${INSTDIR}.QMS.CS-100     ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Qume
        ${CP} ${LDIR}.Qume                   ${INSTDIR}.Qume.Read_Me   ${CPFLAGS}
        ${CP} ${DDIR}.Qume.PS.ScripTEN       ${INSTDIR}.Qume.ScripTEN  ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Panasonic
        ${CP} ${LDIR}.Panasonic              ${INSTDIR}.Panasonic.Read_Me ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Star
        ${CP} ${LDIR}.Star                   ${INSTDIR}.Star.Read_Me   ${CPFLAGS}
        ${CP} ${DDIR}.Star.BI.DP-510         ${INSTDIR}.Star.DP-510    ${CPFLAGS}
        ${CP} ${DDIR}.Star.BI.LC-200         ${INSTDIR}.Star.LC-200    ${CPFLAGS}
        ${CP} ${DDIR}.Star.BI.LC24-200       ${INSTDIR}.Star.LC24-200  ${CPFLAGS}
        ${CP} ${DDIR}.Star.BI.SJ-144         ${INSTDIR}.Star.SJ-144    ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.TI
        ${CP} ${LDIR}.TI                     ${INSTDIR}.TI.Read_Me     ${CPFLAGS}
        ${CP} ${DDIR}.TI.PS.OmniLaser        ${INSTDIR}.TI.OmniLaser   ${CPFLAGS}
        |
        -attr -directories +wr ${INSTDIR}
        -attr -files +ol +or -ow +wr -ww ${INSTDIR}
        |
        @echo ${COMPONENT}: All installed {Disc}


clean:
        @echo ${COMPONENT}: cleaned

#---------------------------------------------------------------------------
# Dynamic dependencies:
