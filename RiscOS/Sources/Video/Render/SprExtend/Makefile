# Makefile for SprExtend
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name    Description
# ----       ----    -----------
# 25-May-94  AMcC    Created.
# 12-Aug-94  AMcC    Updated for modified SpriteExtend (JPEG support)
# 29-Oct-94  AMcC    Exports hdr.SprExtend (SWI defs)
#                    Data file now called CFSIjpeg (was CFSI-jpeg)
#                    - exported to ChangeFSI at install time
# 01-Feb-95  GPS     Now compiles with cc505
# 13-Feb-95  GPS     CFSIjpeg now compiles with cc505
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
AASM    = aasm
CC      = cc
CP      = copy
LINK    = link
# -d does debug tables
MKDIR   = cdir
OBJASM  = objasm
RM      = remove
WIPE    = -wipe
SED     = Sed

# To switch between making debugging and non-debugging versions of SprExtend:
#   change the #define of DEBUG at the top of c.PutScaled
#   change the SETL of 'debug' in sources.SprExtend.

ASFLAGS  = -depend !Depend -Stamp -quit -module
OBJFLAGS = -depend !Depend -Stamp -quit
CFLAGS   = -depend !Depend ${INCLUDES} ${DFLAGS}
DFLAGS   = -DRISCOS -DASMHUFF
#DFLAGS   = -DRISCOS -DASMHUFF -DDEBUG
CPFLAGS  = ~cfr~v
WFLAGS   = ~c~v

INCLUDES = -IC:

#
# Libraries
#
CLIB = CLIB:o.stubs

#
# Program specific options:
#
COMPONENT = SprExtend
SOURCE    = Sources.SprExtend
TARGET    = rm.SprExtend
LOCALMSG  = rm.SprExtendM
EXPORTS   = ${EXP_HDR}.SprExtend

SFILES =\
 s.jhdr\
 s.putscaled\
 s.stdpalette\
 s.yuvtabs

# Extra utilites (generate assembler code)
MAKEJHDR   = utils.makejhdr
GENPALETTE = utils.genpalette
GENYUVTABS = utils.genyuvtabs

# Needed for ChangeFSI
CFSI-JPEG  = data.CFSIjpeg

#
# Rule patterns
#
.SUFFIXES: .o .c .sources

.c.o:;       ${CC} -c ${CFLAGS} -o $@ $<
.sources.o:; ${OBJASM} ${OBJFLAGS} -from $< -to $@

#
# Rules:
#
all: ${CFSI-JPEG} ${LOCALMSG}
        @echo ${COMPONENT}: all built

install: ${CFSI-JPEG}
        ${CP} ${CFSI-JPEG} ${INSTDIR}.CFSIjpeg ${CPFLAGS}
        Access ${INSTDIR}.CFSIjpeg lr/r
        @echo ${COMPONENT}: CFSIjpeg file installed

cfsi-jpeg: ${CFSI-JPEG}
        @echo ${COMPONENT}: CFSIjpeg file generated

#
# Generic rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${RM} ${TARGET}
        ${RM} ${LOCALMSG}
        ${RM} ${GENPALETTE}
        ${RM} ${GENYUVTABS}
        ${RM} ${MAKEJHDR}
        ${RM} ${CFSI-JPEG}
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} s.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

${TARGET}: ${SOURCE} ${SFILES}
        ${AASM} ${ASFLAGS} -From ${SOURCE} -To $@

# Get the Messages from somewhere other than Resources:$.Resources.SprExtend
${LOCALMSG}: Sources.LocalMsgs ${SFILES}
        ${AASM} ${ASFLAGS} -From Sources.LocalMsgs -To $@

# Export the SWI definitions to Hdr:
${EXP_HDR}.SprExtend: hdr.SprExtend
        ${CP} hdr.SprExtend $@ ${CPFLAGS}
#
# Generate the SFILES
#
s.putscaled: c.putscaled
        ${CC} -s ${CFLAGS} -ff -zps1 -DEMBED c.putscaled
        @echo ${COMPONENT}: making s.putscaled aasm compatible
        ${SED} s/__rt_sdiv/|__rt_sdiv|/g < s.putscaled > s.temp
        ${SED} s/_swix/|_swix|/g < s.temp > s.putscaled

s.yuvtabs: ${GENYUVTABS}
        ${GENYUVTABS} > s.yuvtabs

s.stdpalette: ${GENPALETTE}
        ${GENPALETTE} > s.stdpalette

s.jhdr: ${MAKEJHDR}
        ${MAKEJHDR} > s.jhdr

${GENPALETTE}: o.genpalette ${CLIB}
        ${LINK} -o $@ o.genpalette ${CLIB}

${GENYUVTABS}: o.genyuvtabs ${CLIB}
        ${LINK} -o $@ o.genyuvtabs ${CLIB}

${MAKEJHDR}: o.makejhdr ${CLIB}
        ${LINK} -o $@ o.makejhdr ${CLIB}
#
# For ChangeFSI:
#
${CFSI-JPEG}: Sources.cfsi-jpeg s.jhdr s.rojpeg s.stdpalette
        ${AASM} ${ASFLAGS} -From Sources.cfsi-jpeg -To $@
        SetType $@ Data

# needed for the ChangeFSI inclusion:
s.rojpeg: c.rojpeg
        ${CC} -s ${CFLAGS} -ff -zps1 -DEMBED -DCFSI c.rojpeg
        ${SED} s/__rt_sdiv/|__rt_sdiv|/g < s.rojpeg > s.temp
        ${CP} s.temp s.rojpeg ${CPFLAGS}
#        Run w.ZapS-ROJ

#
# Other bits:
#
j2s: o.j2s o.jdhuff o.jrevdct o.jcconv o.diffuse
        ${LINK} -o j2s o.j2s o.jdhuff o.jrevdct o.jcconv o.diffuse ${CLIB}

o.diffuse: sources.diffuseo s.stdpalette
        ${OBJASM} ${OBJFLAGS} -from sources.diffuseo -to $@

o.jcconv: sources.jcconvo s.yuvtabs
        ${OBJASM} ${OBJFLAGS} -from sources.jcconvo -to $@

o.jdhuff: sources.jdhuffo s.jhdr
        ${OBJASM} ${OBJFLAGS} -from sources.jdhuffo -to $@

o.jrevdct: sources.jrevdcto
        ${OBJASM} ${OBJFLAGS} -from sources.jrevdcto -to $@

put: o.put
        ${LINK} -o put o.put ${CLIB}

jput: o.jput
        ${LINK} -o jput o.jput ${CLIB}

tdct: o.testdct o.idcto
        ${LINK} -o tdct o.testdct o.idcto ${CLIB}

o.swiv: sources.swiv

# Dynamic dependencies:
