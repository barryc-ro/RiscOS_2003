# Makefile for Global Assembler Header files
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 26-May-94  AMcC   Created.
# 28-Jul-94  AMcC   removed build rules for h.swis (done by RISC_OSLib)
# 11-Aug-94  AMcC   Don't create an Arthur directory in Export.Hdr.Global
# 19-Oct-94  AMcC   Export ImageSize hdrs
# 31-Oct-94  AMcC   Export IO.IOMDL (IOMD for Morris)
# 03-Dec-96  RWB    Don't derive headers, import them instead.
#

#
# Paths
#
HDRDIR = <hdr$dir>.Global

# Generic options:
#
MKDIR    = cdir
AS       = aasm
CP       = copy
RM       = remove
TOKENISE = tokenise

CFLAGS   = -c -depend !Depend -IC:
AFLAGS   = -depend !Depend -stamp -quit
CPFLAGS  = ~cfr~v

TOKENS   = Hdr:Tokens

#
# Program specific options:
#
COMPONENT = HdrSrc

EXPORTS   = \
 ${HDRDIR}.CMOS \
 ${HDRDIR}.CPU.ARM600 \
 ${HDRDIR}.CPU.Generic26 \
 ${HDRDIR}.DDVMacros \
 ${HDRDIR}.Debug \
 ${HDRDIR}.DevNos \
 ${HDRDIR}.ExtraLong \
 ${HDRDIR}.FDC.FDC711 \
 ${HDRDIR}.FileTypes \
 ${HDRDIR}.FSNumbers \
 ${HDRDIR}.Heap \
 ${HDRDIR}.ImageSize.<ImageSize> \
 ${HDRDIR}.IO.GenericIO \
 ${HDRDIR}.IO.IOC \
 ${HDRDIR}.IO.IOC-A1 \
 ${HDRDIR}.IO.IOEB \
 ${HDRDIR}.IO.IOMD \
 ${HDRDIR}.IO.IOMDL \
 ${HDRDIR}.KEYV \
 ${HDRDIR}.ListOpts \
 ${HDRDIR}.MEMM.ARM600 \
 ${HDRDIR}.MEMM.MEMC1 \
 ${HDRDIR}.Machine.<Machine> \
 ${HDRDIR}.Machine.Machine \
 ${HDRDIR}.Macros \
 ${HDRDIR}.Messages \
 ${HDRDIR}.NdrDebug \
 ${HDRDIR}.NewErrors \
 ${HDRDIR}.PaletteV \
 ${HDRDIR}.PDriverReg \
 ${HDRDIR}.PDumperReg \
 ${HDRDIR}.PoduleReg \
 ${HDRDIR}.Pointer \
 ${HDRDIR}.Proc \
 ${HDRDIR}.Services \
 ${HDRDIR}.Sprite \
 ${HDRDIR}.Symbols \
 ${HDRDIR}.System \
 ${HDRDIR}.SWIs \
 ${HDRDIR}.Tokens \
 ${HDRDIR}.UpCall \
 ${HDRDIR}.VIDC.VIDC1a \
 ${HDRDIR}.VIDC.VIDC20

#
# Generic rules:
#

export: ${EXPORTS} Dirs
        @echo ${COMPONENT}: export complete

Dirs:
        ${MKDIR} <build$dir>.Export
        ${MKDIR} <build$dir>.Export.C
        ${MKDIR} <build$dir>.Export.C.h
        ${MKDIR} <build$dir>.Export.C.o
        ${MKDIR} <build$dir>.Export.Hdr
        ${MKDIR} <build$dir>.Export.Hdr.Global
        ${MKDIR} <build$dir>.Export.Hdr.Interface
        ${MKDIR} <build$dir>.Export.Hdr.Interface2
        ${MKDIR} ${HDRDIR}.CPU
        ${MKDIR} ${HDRDIR}.FDC
        ${MKDIR} ${HDRDIR}.IO
        ${MKDIR} ${HDRDIR}.ImageSize
        ${MKDIR} ${HDRDIR}.Machine
        ${MKDIR} ${HDRDIR}.MEMM
        ${MKDIR} ${HDRDIR}.VIDC

clean:
         @echo ${COMPONENT}: NOT cleaning imported headers

#
# Rules for exporting:
#

# Registration headers:
${HDRDIR}.SWIs:       Import.Global.SWIs;           ${CP} Import.Global.SWIs       $@ ${CPFLAGS}
${HDRDIR}.FileTypes:  Import.Global.FileTypes;      ${CP} Import.Global.FileTypes  $@ ${CPFLAGS}
${HDRDIR}.FSNumbers:  Import.Global.FSNumbers;      ${CP} Import.Global.FSNumbers  $@ ${CPFLAGS}
${HDRDIR}.PDriverReg: Import.Global.PDriverReg;     ${CP} Import.Global.PDriverReg $@ ${CPFLAGS}
${HDRDIR}.PDumperReg: Import.Global.PDumperReg;     ${CP} Import.Global.PDumperReg $@ ${CPFLAGS}
${HDRDIR}.PoduleReg:  Import.Global.PoduleReg;      ${CP} Import.Global.PoduleReg  $@ ${CPFLAGS}
${HDRDIR}.NewErrors:  Import.Global.NewErrors;      ${CP} Import.Global.NewErrors  $@ ${CPFLAGS}
${HDRDIR}.Services:   Import.Global.Services;       ${CP} Import.Global.Services   $@ ${CPFLAGS}
${HDRDIR}.UpCall:     Import.Global.UpCall;         ${CP} Import.Global.UpCall     $@ ${CPFLAGS}

# Machine/hardware specific headers:
${HDRDIR}.Machine.<Machine>: hdr.Machine.<Machine>; ${CP} hdr.Machine.<Machine>    $@ ${CPFLAGS}
${HDRDIR}.Machine.Machine:   hdr.Machine.Machine;   ${CP} hdr.Machine.Machine      $@ ${CPFLAGS}
${HDRDIR}.ImageSize.<ImageSize>: hdr.ImageSize.<ImageSize>; ${CP} hdr.ImageSize.<ImageSize> $@ ${CPFLAGS}
${HDRDIR}.CPU.ARM600:        hdr.CPU.ARM600;        ${CP} hdr.CPU.ARM600           $@ ${CPFLAGS}
${HDRDIR}.CPU.Generic26:     hdr.CPU.Generic26;     ${CP} hdr.CPU.Generic26        $@ ${CPFLAGS}
${HDRDIR}.FDC.FDC711:        hdr.FDC.FDC711;        ${CP} hdr.FDC.FDC711           $@ ${CPFLAGS}
${HDRDIR}.IO.GenericIO:      hdr.IO.GenericIO;      ${CP} hdr.IO.GenericIO 	   $@ ${CPFLAGS}
${HDRDIR}.IO.IOC:            hdr.IO.IOC;            ${CP} hdr.IO.IOC               $@ ${CPFLAGS}
${HDRDIR}.IO.IOC-A1:         hdr.IO.IOC-A1;         ${CP} hdr.IO.IOC-A1            $@ ${CPFLAGS}
${HDRDIR}.IO.IOEB:           hdr.IO.IOEB;           ${CP} hdr.IO.IOEB              $@ ${CPFLAGS}
${HDRDIR}.IO.IOMD:           hdr.IO.IOMD;           ${CP} hdr.IO.IOMD              $@ ${CPFLAGS}
${HDRDIR}.IO.IOMDL:          hdr.IO.IOMDL;          ${CP} hdr.IO.IOMDL             $@ ${CPFLAGS}
${HDRDIR}.MEMM.ARM600:       hdr.MEMM.ARM600;       ${CP} hdr.MEMM.ARM600          $@ ${CPFLAGS}
${HDRDIR}.MEMM.MEMC1:        hdr.MEMM.MEMC1;        ${CP} hdr.MEMM.MEMC1           $@ ${CPFLAGS}
${HDRDIR}.VIDC.VIDC1a:       hdr.VIDC.VIDC1a;       ${CP} hdr.VIDC.VIDC1a          $@ ${CPFLAGS}
${HDRDIR}.VIDC.VIDC20:       hdr.VIDC.VIDC20;       ${CP} hdr.VIDC.VIDC20          $@ ${CPFLAGS}

# Macros:
${HDRDIR}.DDVMacros:         hdr.DDVMacros;         ${CP} hdr.DDVMacros            $@ ${CPFLAGS}
${HDRDIR}.Debug:             hdr.Debug;             ${CP} hdr.Debug                $@ ${CPFLAGS}
${HDRDIR}.ExtraLong:         hdr.ExtraLong;         ${CP} hdr.ExtraLong            $@ ${CPFLAGS}
${HDRDIR}.Macros:            hdr.Macros;            ${CP} hdr.Macros               $@ ${CPFLAGS}
${HDRDIR}.NdrDebug:          hdr.NdrDebug;          ${CP} hdr.NdrDebug             $@ ${CPFLAGS}
${HDRDIR}.Proc:              hdr.Proc;              ${CP} hdr.Proc                 $@ ${CPFLAGS}

# The rest:
${HDRDIR}.CMOS:              hdr.CMOS;              ${CP} hdr.CMOS                 $@ ${CPFLAGS}
${HDRDIR}.DevNos:            hdr.DevNos;            ${CP} hdr.DevNos               $@ ${CPFLAGS}
${HDRDIR}.Heap:              hdr.Heap;              ${CP} hdr.Heap                 $@ ${CPFLAGS}
${HDRDIR}.KEYV:              hdr.KEYV;              ${CP} hdr.KEYV                 $@ ${CPFLAGS}
${HDRDIR}.ListOpts:          hdr.ListOpts;          ${CP} hdr.ListOpts             $@ ${CPFLAGS}
${HDRDIR}.Messages:          hdr.Messages;          ${CP} hdr.Messages             $@ ${CPFLAGS}
${HDRDIR}.PaletteV:          hdr.PaletteV;          ${CP} hdr.PaletteV             $@ ${CPFLAGS}
${HDRDIR}.Pointer:           hdr.Pointer;           ${CP} hdr.Pointer              $@ ${CPFLAGS}
${HDRDIR}.Sprite:            hdr.Sprite;            ${CP} hdr.Sprite               $@ ${CPFLAGS}
${HDRDIR}.Symbols:           hdr.Symbols;           ${CP} hdr.Symbols              $@ ${CPFLAGS}
${HDRDIR}.System:            hdr.System;            ${CP} hdr.System               $@ ${CPFLAGS}
${HDRDIR}.Tokens:            hdr.Tokens;            ${CP} hdr.Tokens               $@ ${CPFLAGS}

# Dynamic dependencies:
