# Makefile for ColourPicker
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 23-Nov-99  SNB          Created

COMPONENT = Picker
RAM_MODULE = rm.${COMPONENT}
ROM_MODULE = aof.${COMPONENT}
DBG_MODULE = rm.${COMPONENT}D

FULL_COMPONENT = ColourPicker

#
# Paths
#
EXP_HDR = <export$dir>

VPATH=@ Support011

#
# Export Paths for Messages module
#
RESDIR = <resource$dir>.Resources2.${COMPONENT}
RESFSDIR   = Resources.${COMPONENT}

#
# Generic options:
#
MKDIR   = do mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
CD      = dir
TOUCH   = stamp
WINEDIT = /Apps:!WinEdit

CPFLAGS = ~cfr~v
WFLAGS  = f~c~vr

#
# Include files
#
INCLUDES=       -IC:AsmUtils,Support011,icons,OS:,C:

#
# Libraries
#
CLIB            = Clib:o.Stubs
ROMCSTUBS       = RISCOSLIB:o.romcstubs
ABSSYM          = RISC_OSLib:o.AbsSym
OSLIB           = OSLib:o.OSLib
ASMUTILS        = C:AsmUtils.o.AsmUtilsZM

DEPEND          = -depend !Depend
DBGFLAGS        = -DTRACE
DEFINES         = 
CENV            = ${DEPEND} ${THROWBACK}

ASFLAGS         = ${DEPEND} ${THROWBACK}
CFLAGS          = -W -c -ffh -zM -zps1 -Wcp ${INCLUDES} ${DEFINES} ${CENV}
CMHGFLAGS       = ${DEPEND} ${THROWBACK} -p

OTHER_OBJS      = o.veneer o.header

ROM_OBJS        = or.cmyk  or.dialogue  or.helpreply  or.hsv              \
                  or.main  or.model     or.rgb        or.tables           \
                           or.callback  or.lookup     or.realloc          \
                  or.task  or.riscos    or.window     or.resource         \
                  or.icon  or.steppable or.relocate   ${OTHER_OBJS}
RAM_OBJS        = o.cmyk   o.dialogue   o.helpreply   o.hsv               \
                  o.main   o.model      o.rgb         o.tables    ${RESF} \
                           o.callback   o.lookup      o.realloc           \
                  o.task   o.riscos     o.window      o.resource          \
                  o.icon   o.steppable  o.relocate    ${OTHER_OBJS}
DBG_OBJS        = od.cmyk  od.dialogue  od.helpreply  od.hsv              \
                  od.main  od.model     od.rgb        od.tables   ${RESF} \
                  od.m     od.callback  od.lookup     od.realloc          \
                  od.task  od.riscos    od.window     od.resource         \
                  od.icon  od.steppable od.relocate   ${OTHER_OBJS}
                  
RESF            = o.msgs

RESOURCEFILES   = \
  Resources.CMYK.Messages\
  Resources.CMYK.Templates\
  Resources.HSV.Messages\
  Resources.HSV.Templates\
  Resources.RGB.Messages\
  Resources.RGB.Templates\
  Resources.Messages\
  Resources.Sprites\
  Resources.Templates

#
# Rule patterns
#
.SUFFIXES:      .o .od .or .s .c .cmhg .h
.c.o:;          ${CC} ${CFLAGS} -o $@ $<
.c.or:;         ${CC} ${CFLAGS} -DROM -o $@ $<
.c.od:;         ${CC} ${CFLAGS} ${DBGFLAGS} -o $@ $<
.cmhg.o:;       ${CMHG} ${CMHGFLAGS} -o $@ $< -d h.header
.cmhg.h:;       ${CMHG} ${CMHGFLAGS} -d $@ $< -o null:
.cmhg.od:;      ${CMHG} ${CMHGFLAGS} -o $@ $< -d h.header ${DBGFLAGS}
.s.o:;          ${AS} ${ASFLAGS} -o $@ $<

header.h: header.o
        ${TOUCH} $@
        
o.header od.header: VersionNum

ICONS=icons.h.dialogue_i icons.h.cmyk_i icons.h.rgb_i icons.h.hsv_i

icons.h.dialogue_i: Resources.Templates
        ${WINEDIT} "-h??=define dialogue_%w_%n ((wimp_i) %i)" Resources.Templates >$@
        perl Resources.perlscript $@

icons.h.cmyk_i: Resources.CMYK.Templates
        ${WINEDIT} "-h??=define cmyk_%w_%n ((wimp_i) %i)" Resources.CMYK.Templates >$@
        perl Resources.perlscript $@

icons.h.rgb_i: Resources.RGB.Templates
        ${WINEDIT} "-h??=define rgb_%w_%n ((wimp_i) %i)" Resources.RGB.Templates >$@
        perl Resources.perlscript $@

icons.h.hsv_i: Resources.HSV.Templates
        ${WINEDIT} "-h??=define hsv_%w_%n ((wimp_i) %i)" Resources.HSV.Templates >$@
        perl Resources.perlscript $@

#
# Main rules:
#
#
all: ${RAM_MODULE} o.dirs
        @echo ${FULL_COMPONENT}: Module built (RAM)

ramtrace: ${DBG_MODULE} o.dirs
        @echo ${FULL_COMPONENT}: Module built (TRACE)

rom: ${ROM_MODULE} o.dirs
        @echo ${FULL_COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${FULL_COMPONENT}: Module installed (ROM)

resources: ${RESOURCEFILES}
        ${MKDIR} ${RESDIR}
        ${MKDIR} ${RESDIR}.CMYK
        ${MKDIR} ${RESDIR}.HSV
        ${MKDIR} ${RESDIR}.RGB
        ${CP} Resources.CMYK.Messages   ${RESDIR}.CMYK.Messages  ${CPFLAGS}
        ${CP} Resources.CMYK.Templates  ${RESDIR}.CMYK.Templates ${CPFLAGS}
        ${CP} Resources.HSV.Templates   ${RESDIR}.HSV.Templates  ${CPFLAGS}
        ${CP} Resources.HSV.Messages    ${RESDIR}.HSV.Messages   ${CPFLAGS}
        ${CP} Resources.RGB.Templates   ${RESDIR}.RGB.Templates  ${CPFLAGS}
        ${CP} Resources.RGB.Messages    ${RESDIR}.RGB.Messages   ${CPFLAGS}
        ${CP} Resources.Messages        ${RESDIR}.Messages       ${CPFLAGS}
        ${CP} Resources.Sprites         ${RESDIR}.Sprites        ${CPFLAGS}
        ${CP} Resources.Templates       ${RESDIR}.Templates      ${CPFLAGS}
        @echo ${FULL_COMPONENT}: resource files copied to Messages module

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${WIPE} or ${WFLAGS}
        ${WIPE} aof ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} h.header ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} icons ${WFLAGS}
        @echo ${FULL_COMPONENT}: cleaned

o.dirs:
        ${MKDIR} o
        ${MKDIR} od
        ${MKDIR} or
        ${MKDIR} icons
        ${MKDIR} icons.h
        create $@

export:
        ${CP} hdr.ColourPick ${EXP_HDR}.ColourPick ${CPFLAGS}
        @echo ${FULL_COMPONENT}: export complete


#
# Targets
#
${RAM_MODULE}:  ${RAM_OBJS} ${CLIB} ${OSLIB} ${ICONS} ${ASMUTILS}
        ${MKDIR} rm
        $(LD) -o $@ -rmf ${RAM_OBJS} ${CLIB} ${OSLIB} ${ASMUTILS}
        ${MODSQZ} $@
        Access $@ RW/R

${ROM_MODULE}:  ${ROM_OBJS} ${OSLIB} ${ROMCSTUBS} ${ICONS} ${ASMUTILS}
        ${MKDIR} aof
        $(LD) -o $@ -aof ${ROM_OBJS} ${OSLIB} ${ROMCSTUBS} ${ASMUTILS}

${DBG_MODULE}:  ${DBG_OBJS} ${CLIB} ${OSLIB} ${ICONS} ${ASMUTILS}
        ${MKDIR} rm
        $(LD) -o $@ -rmf ${DBG_OBJS} ${OSLIB} ${CLIB} ${ASMUTILS}
        Access $@ RW/R

${RESF}: ${RESOURCEFILES}
        resgen resourcefs_files $@ \
 Resources.CMYK.Messages   ${RESFSDIR}.CMYK.Messages    \
 Resources.CMYK.Templates  ${RESFSDIR}.CMYK.Templates   \
 Resources.HSV.Templates   ${RESFSDIR}.HSV.Templates    \
 Resources.HSV.Messages    ${RESFSDIR}.HSV.Messages     \
 Resources.RGB.Templates   ${RESFSDIR}.RGB.Templates    \
 Resources.RGB.Messages    ${RESFSDIR}.RGB.Messages     \
 Resources.Messages        ${RESFSDIR}.Messages         \
 Resources.Sprites         ${RESFSDIR}.Sprites          \
 Resources.Templates       ${RESFSDIR}.Templates

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROM_MODULE} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete


# Dynamic dependencies:
