# Makefile for System
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name   Description
# ----           ----   -----------
# 08-Nov-94      AMcC Updated for Black build
# 15th Nov 1994  JRC  Added SysPaths utility, replaced old 'abs' and 'rm'
#                     directories with a 'Modules' drectory. New 'abs'
#                     directory used to build SysPaths in
# 30-Jan-95      AMcC No longer installs 'recent' modules in 'Modules'
#                     Installs 360.Modules
# 31-Jan-95      AMcC No longer installs !Sprites23. Modules writable by owner
#
# 03-Jul-96      KJB  Added 370 directory
#
# 24-Apr-97      KJB  Made all copies file-by-file to prevent copying CVS
#                     directories. Removed 200 and 300 directories.
#                     Made SysPaths set Boot$OSVersion if not already set,
#                     and correct "3.50" to "350".

VPATH = @ <Support$Dir>

#
# Program specific options:
#
COMPONENT  = System
APP        = !${COMPONENT}
RDIR       = Resources
LDIR       = ${RDIR}.${LOCALE}
INSTDIR    = ${INSTDIR}.${APP}

# Generic options:
#
MKDIR   = cdir
AS      = objasm
ATTR    = -Attr
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS  = ${THROWBACK} -c -depend !Depend -fah ${INCLUDES} ${DFLAGS} -wp
CPFLAGS = ~cfr~v
SQFLAGS = -f
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLib:o.Stubs
OSLIB     = OSLib:o.OSLib

#
# Include files
#
INCLUDES  = -ISupport:,OS:

MODULES = \
      310.Modules.ABCLib \
      310.Modules.ABIMod \
      310.Modules.BASIC64 \
      310.Modules.Colours \
      310.Modules.DDEUtils \
      310.Modules.DDT \
      310.Modules.FrontEnd \
      310.Modules.Network.URL.File \
      310.Modules.Network.URL.URL \
      310.Modules.SCSIFiler \
      310.Modules.SerialUtil \
      370.Modules.DDEUtils \
      370.Modules.DDT

FILES =\
 ${LDIR}.!Boot\
 ${LDIR}.!Help\
 ${RDIR}.!Run\
 ${RDIR}.!Sprites\
 ${RDIR}.!Sprites22\
 ${MODULES}\
 abs.SysPaths

OBJS = o.main o.x

#
# Rule patterns
#
.SUFFIXES: .oz .od .asm
.c.o:;    ${CC} ${CFLAGS} -ff -o $@ $<
.c.oz:;   ${CC} ${CFLAGS} -ff -zM -zps1 -o $@ $<
.c.od:;   ${CC} ${CFLAGS} -DTRACE=1 -g -o $@ $<
.asm.o:;  ${AS} ${AFLAGS} $< $@
.asm.od:; ${AS} ${AFLAGS} -g $< $@

#
# Main rules:
#
all: ${FILES}
        @echo ${COMPONENT}: All built (Disc)

install: ${FILES}
        ${MKDIR} ${INSTDIR}
        ${CP} ${LDIR}.!Boot      ${INSTDIR}.!Boot      ${CPFLAGS}
        ${CP} ${LDIR}.!Help      ${INSTDIR}.!Help      ${CPFLAGS}
        ${CP} ${RDIR}.!Run       ${INSTDIR}.!Run       ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites   ${INSTDIR}.!Sprites   ${CPFLAGS}
        ${CP} ${RDIR}.!Sprites22 ${INSTDIR}.!Sprites22 ${CPFLAGS}
        ${CP} abs.SysPaths       ${INSTDIR}.SysPaths   ${CPFLAGS}
        |
        ${MKDIR} ${INSTDIR}.Modules
        ${MKDIR} ${INSTDIR}.310
        ${MKDIR} ${INSTDIR}.310.Modules
        ${MKDIR} ${INSTDIR}.350
        ${MKDIR} ${INSTDIR}.350.Modules
        ${MKDIR} ${INSTDIR}.360
        ${MKDIR} ${INSTDIR}.360.Modules
        ${MKDIR} ${INSTDIR}.370
        ${MKDIR} ${INSTDIR}.370.Modules
        ${CP} 310.Modules.ABCLib           ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.ABIMod           ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.BASIC64          ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.Colours          ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.DDEUtils         ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.DDT              ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.FrontEnd         ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.SCSIFiler        ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.SerialUtil       ${INSTDIR}.310.Modules.* ${CPFLAGS}
        ${CP} 310.Modules.Network.URL.File ${INSTDIR}.310.Modules.Network.URL.* ${CPFLAGS}
        ${CP} 310.Modules.Network.URL.URL  ${INSTDIR}.310.Modules.Network.URL.* ${CPFLAGS}
        ${CP} 370.Modules.DDEUtils         ${INSTDIR}.370.Modules.* ${CPFLAGS}
        ${CP} 370.Modules.DDT              ${INSTDIR}.370.Modules.* ${CPFLAGS}
        ${CP} Modules.SSound               ${INSTDIR}.Modules.*     ${CPFLAGS} # shouldn't be in here
        |
        ${ATTR} -directories +wr ${INSTDIR}
        ${ATTR} -files +ol +or -ow +wr -ww ${INSTDIR}
        ${ATTR} -files -ol +or +ow +wr -ww ${INSTDIR}.Modules
        ${ATTR} -files -ol +or +ow +wr -ww ${INSTDIR}.310
        ${ATTR} -files -ol +or +ow +wr -ww ${INSTDIR}.350
        ${ATTR} -files -ol +or +ow +wr -ww ${INSTDIR}.360
        ${ATTR} -files -ol +or +ow +wr -ww ${INSTDIR}.370
        |
        @echo ${COMPONENT}: All installed (Disc)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} oz.* ${WFLAGS}
        ${WIPE} od.* ${WFLAGS}
        ${RM} abs.SysPaths
        @echo ${COMPONENT}: cleaned

clean_all:
        Access ${INSTDIR}               rw/
        Access ${INSTDIR}.*             rw/
        Access ${INSTDIR}.200.Modules   rw/
        Access ${INSTDIR}.200.Modules.* rw/
        Access ${INSTDIR}.300.Modules   rw/
        Access ${INSTDIR}.300.Modules.* rw/
        Access ${INSTDIR}.310.Modules   rw/
        Access ${INSTDIR}.310.Modules.* rw/
        Access ${INSTDIR}.350.Modules   rw/
        Access ${INSTDIR}.350.Modules.* rw/
        -Destroy ${OBJS} ${OBJSD} abs.SysPaths ${INSTDIR}.!Boot \
              ${INSTDIR}.!Help ${INSTDIR}.!Run ${INSTDIR}.!Sprites \
              ${INSTDIR}.!Sprites22 ${INSTDIR}.SysPaths
        Wipe ${INSTDIR}.200 ${WFLAGS}R
        Wipe ${INSTDIR}.300 ${WFLAGS}R
        Wipe ${INSTDIR}.310 ${WFLAGS}R
        Wipe ${INSTDIR}.350 ${WFLAGS}R
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#
abs.SysPaths: ${OBJS} ${OSLIB} ${CLIB}
        ${LD} -o $@ ${OBJS} ${OSLIB} ${CLIB}
        ${SQUEEZE} ${SQFLAGS} $@

abs.SysPathsD: ${OBJSD} ${OSLIB} ${CLIB}
        ${LD} -o $@ -debug ${OBJSD} ${OSLIB} ${CLIB}

#---------------------------------------------------------------------------
# Dynamic dependencies:
o.main:	c.main
o.main:	OS:h.macros
o.main:	OS:h.os
o.main:	OS:h.types
o.main:	OS:h.osgbpb
o.main:	OS:h.fileswitch
o.main:	OS:h.osmodule
o.main:	Support:h.x
o.x:	<Support$Dir>.c.x
o.x:	C:h.kernel
o.x:	C:h.swis
o.x:	OS:h.macros
o.x:	OS:h.os
o.x:	OS:h.types
o.x:	<Support$Dir>.h.x
