# Makefile for BootCommands
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date           Name       Description
# ----           ----       -----------
# 12th Sep 1994  JC         Created.
#

#
# Generic options:
#
MKDIR   = cdir
CC      = cc -IOS: -fach -wp -DBOOTCMDS
CP      = copy
CPFLAGS = ~cfr~v
OBJASM  = ObjAsm ${THROWBACK} -depend !Depend -stamp -quit
CMHG    = CMHG
LINK    = Link 
ZM      = -zM

STUBS   = CLib:o.Stubs

#
# Libraries:
#
OSLIB  = OSLib:OSLib.o
RSTUBS = RISC_OSLib:o.romcstubs
ABSSYM = RISC_OSLib:o.abssym

#
# Program specific options:
#
#COMPONENT  = Set in Components/Installation file to 'rom' or 'Repeat'
TARGET = aof.BootCmds
RESDIR = <resource$dir>.Resources2.BootCmds

OBJS = o.files o.header o.main o.Messages o.Repeat o.riscos

OBJSD = o.header o.hostfs debug.m debug.main debug.trace \
      debug.riscos debug.files o.Repeat o.Messages debug.svc

ROM_OBJS = o.files o.header o.main o.Repeat o.riscos

ASMS = s.main

RESOURCES = o.Messages o.Repeat

#
# Generic rules {used by top-level build}:
#

all: ${COMPONENT}; @

rom: ${TARGET}
        @echo BootCmds: rom module built

Repeat: Resources.Repeat; @
        @echo Repeat: programme built

export: 
        @echo BootCmds: export complete

install: install_${COMPONENT}; @

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.BootCmds ${CPFLAGS}
        @echo BootCmds: rom module installed

install_Repeat: Resources.Repeat
        ${CP} Resources.Repeat ${INSTDIR}.Repeat ${CPFLAGS}
        @echo Repeat: installed

clean:
       -Destroy RM.BootCmds ${OBJS} ${OBJSD} ${TARGET} map.BootCmds linked.BootCmds \
        o.repeatcmd Resources.Repeat
        @echo BootCmds: cleaned

resources:
        ${MKDIR} ${RESDIR}
        ${CP} Resources.Messages ${RESDIR}.Messages ${CPFLAGS}
        @echo BootCmds: Resources copied to Messages module

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${ROM_OBJS} ${OSLIB} ${RSTUBS}
        ${LINK} -o $@ -aof ${ROM_OBJS} ${OSLIB} ${RSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LINK} -o linked.BootCmds -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} \
              -map > map.BootCmds
        truncate map.BootCmds linked.BootCmds
        ${CP} linked.BootCmds ${LINKDIR}.BootCmds ${CPFLAGS}
        @echo BootCmds: rom_link complete

#
# Extra {development} rules:
#

asms: ${ASMS}

release: RM.BootCmds
   %Copy RM.BootCmds Root:RM.JCoxhead.BootCmds ~CFLNRV
   /RM:BootCmds
   @Echo Made release

#Internal targets
RM.BootCmds: ${OBJS} ${OSLIB} ${STUBS}
   ${LINK} -rmf -output RM.BootCmds ${OBJS} ${OSLIB} ${STUBS}
   Access RM.BootCmds WR/R

RM.BootCmdsD: ${OBJSD} ${OSLIB} ${STUBS}
   ${LINK} -rmf -output RM.BootCmdsD ${OBJSD} ${OSLIB} ${STUBS}
   Access RM.BootCmdsD WR/R

o.Messages: Resources.Messages
   ResGen Messages o.Messages Resources.Messages Resources.BootCmds.Messages

o.Repeat: Resources.Repeat
   ResGen Repeat o.Repeat Resources.Repeat Resources.BootCmds.Repeat

Resources.Repeat: o.repeatcmd ${OSLIB} ${STUBS}
   ${LINK} -output Resources.Repeat o.repeatcmd ${OSLIB} ${STUBS}

o.repeatcmd: c.repeatcmd
   ${CC} ${THROWBACK} -depend !Depend -c -ff c.repeatcmd

debug.trace: h.hostfs

#General rules
.SUFFIXES:   .cmhg .c .debug .o .asm .def
.c.o:;       ${CC} ${THROWBACK} -depend !Depend -c ${ZM} -ff $<
.asm.o:;     ${OBJASM} -stamp -quit $< $@
.asm.debug:; ${OBJASM} -pd "TRACE SETL {TRUE}" -pd "STANDALONE SETL {TRUE}" -stamp -quit $< $@
.c.debug:;   ${CC} ${THROWBACK} -depend !Depend -c ${ZM} -ff -DTRACE=1 -DSTANDALONE -o $@ $<
.cmhg.o:;    ${CMHG} $< $@
.c.s:;       ${CC} ${THROWBACK} -depend !Depend -s ${ZM} -ff $<
.def.o:;     DefMod -l -o l.$* < $<
             LibFile -c -o $@ -via ViaFile
             Wipe l.$* ~C~FR~V
.def.h:;     DefMod -h < $< > $@
.def.s:;     DefMod -s < $< > $@

# Dynamic dependencies:
o.files: asm.files
o.files: OS:Hdr.Types
o.main:	c.main
o.main:	C:h.kernel
o.main:	OS:h.econet
o.main:	OS:h.types
o.main:	OS:h.os
o.main:	OS:h.macros
o.main:	OS:h.messagetrans
o.main:	OS:h.netfs
o.main:	OS:h.fileswitch
o.main:	OS:h.os
o.main:	OS:h.osargs
o.main:	OS:h.osbyte
o.main:	OS:h.osfile
o.main:	OS:h.osfscontrol
o.main:	OS:h.osfind
o.main:	OS:h.osgbpb
o.main:	OS:h.osmodule
o.main:	OS:h.resourcefs
o.main:	OS:h.territory
o.main:	h.riscos
o.main:	h.jc_trace
o.main:	h.files
o.main:	h.main
o.main:	C:h.kernel
o.main:	h.lookup
o.main:	h.resource
o.main:	OS:h.wimp
o.main:	OS:h.osspriteop
o.main:	OS:h.font
o.main:	h.run
o.main:	OS:h.os
o.repeatcmd:	c.repeatcmd
o.repeatcmd:	C:h.kernel
o.repeatcmd:	OS:h.os
o.repeatcmd:	OS:h.types
o.repeatcmd:	OS:h.osfile
o.repeatcmd:	OS:h.fileswitch
o.repeatcmd:	OS:h.osfscontrol
o.repeatcmd:	OS:h.osgbpb
o.repeatcmd:	OS:h.wimp
o.repeatcmd:	OS:h.osspriteop
o.repeatcmd:	OS:h.font
o.repeatcmd:	OS:h.wimpspriteop
o.riscos:	c.riscos
o.riscos:	OS:h.messagetrans
o.riscos:	OS:h.types
o.riscos:	OS:h.os
o.riscos:	OS:h.macros
o.riscos:	OS:h.os
o.riscos:	OS:h.territory
o.riscos:	OS:h.wimp
o.riscos:	OS:h.osspriteop
o.riscos:	OS:h.font
o.riscos:	h.m
o.riscos:	h.realloc
o.riscos:	C:h.trace
o.riscos:	h.riscos
o.riscos:	C:h.trace
