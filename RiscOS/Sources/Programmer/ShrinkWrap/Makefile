# Makefile for ShrinkWrap
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -zM -zps1 -ff ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym

#
# Include files
#
INCLUDES = -IC:

LIBS     =

DFLAGS   =

#
# Program specific options:
#
COMPONENT = ShrinkWrap
TARGET    = aof.ShrinkWrap
OBJS      = \
 o.allerrs\
 o.Interface\
 o.RMInfo\
 o.runcode\
 o.args\
 o.buffer\
 o.command\
 o.crc32\
 o.deflate\
 o.file\
 o.find\
 o.flex\
 o.fs\
 o.gbpb\
 o.imbits\
 o.inflate\
 o.main\
 o.mem\
 o.scrap\
 o.trees\
 o.vectors\
 o.ShrinkWrap

EXPORTS   = 

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.ShrinkWrap

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} rm.* ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.* ${WFLAGS}
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned

resources:
#        ${MKDIR} ${RESDIR}.ShrinkWrap
#        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.ShrinkWrap.Messages  ${CPFLAGS}
#        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS}
        ${LD} -o $@ -aof ${OBJS} ${LIBS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.ShrinkWrap: ${OBJS} ${CLIB}
        ${LD} -o $@ -module ${OBJS} ${LIBS} ${CLIB}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
#       ${CP} hdr.${COMPONENT} $@ ${CPFLAGS}

# Dynamic dependencies:
o.allerrs: s.allerrs
o.Interface: s.Interface
o.RMInfo: s.RMInfo
o.runcode: s.runcode
o.args:	c.args
o.args:	C:h.kernel
o.args:	C:h.swis
o.args:	h.etc
o.args:	h.err
o.args:	h.mem
o.args:	h.rminfo
o.args:	h.flex
o.args:	h.constants
o.args:	h.fs
o.args:	h.buffer
o.args:	h.zip
o.args:	h.inflate
o.args:	h.unzip
o.args:	h.vectors
o.args:	h.find
o.args:	h.args
o.buffer:	c.buffer
o.buffer:	C:h.kernel
o.buffer:	C:h.swis
o.buffer:	h.flex
o.buffer:	h.etc
o.buffer:	h.crc32
o.buffer:	h.err
o.buffer:	h.fs
o.buffer:	h.buffer
o.command:	c.command
o.command:	C:h.kernel
o.command:	C:h.swis
o.command:	h.etc
o.command:	h.err
o.command:	h.mem
o.command:	h.flex
o.command:	h.fs
o.command:	h.vectors
o.command:	h.scrap
o.command:	h.buffer
o.command:	h.find
o.command:	h.main
o.command:	h.deflate
o.command:	h.command
o.crc32:	c.crc32
o.crc32:	h.crc32
o.deflate:	c.deflate
o.deflate:	C:h.kernel
o.deflate:	C:h.swis
o.deflate:	h.flex
o.deflate:	h.etc
o.deflate:	h.err
o.deflate:	h.buffer
o.deflate:	h.zip
o.deflate:	h.deflatex
o.deflate:	h.deflate
o.file:	c.file
o.file:	C:h.kernel
o.file:	C:h.swis
o.file:	h.etc
o.file:	h.err
o.file:	h.mem
o.file:	h.rminfo
o.file:	h.flex
o.file:	h.constants
o.file:	h.fs
o.file:	h.buffer
o.file:	h.scrap
o.file:	h.zip
o.file:	h.inflate
o.file:	h.unzip
o.file:	h.deflate
o.file:	h.vectors
o.file:	h.command
o.file:	h.gbpb
o.file:	h.find
o.file:	h.file
o.find:	c.find
o.find:	C:h.kernel
o.find:	C:h.swis
o.find:	h.etc
o.find:	h.err
o.find:	h.mem
o.find:	h.rminfo
o.find:	h.flex
o.find:	h.crc32
o.find:	h.constants
o.find:	h.fs
o.find:	h.buffer
o.find:	h.scrap
o.find:	h.zip
o.find:	h.inflate
o.find:	h.unzip
o.find:	h.vectors
o.find:	h.command
o.find:	h.gbpb
o.find:	h.file
o.find:	h.find
o.flex:	c.flex
o.flex:	C:h.kernel
o.flex:	C:h.swis
o.flex:	h.mem
o.flex:	h.err
o.flex:	h.flex
o.fs:	c.fs
o.fs:	C:h.kernel
o.fs:	C:h.swis
o.fs:	h.err
o.fs:	h.constants
o.fs:	h.fs
o.gbpb:	c.gbpb
o.gbpb:	C:h.kernel
o.gbpb:	C:h.swis
o.gbpb:	h.etc
o.gbpb:	h.err
o.gbpb:	h.mem
o.gbpb:	h.rminfo
o.gbpb:	h.constants
o.gbpb:	h.fs
o.gbpb:	h.vectors
o.gbpb:	h.command
o.gbpb:	h.find
o.gbpb:	h.gbpb
o.imbits:	c.imbits
o.imbits:	C:h.kernel
o.imbits:	C:h.swis
o.imbits:	h.flex
o.imbits:	h.err
o.imbits:	h.buffer
o.imbits:	h.zip
o.imbits:	h.deflatex
o.imbits:	h.deflate
o.inflate:	c.inflate
o.inflate:	C:h.kernel
o.inflate:	C:h.swis
o.inflate:	h.flex
o.inflate:	h.err
o.inflate:	h.buffer
o.inflate:	h.zip
o.inflate:	h.unzip
o.inflate:	h.inflate
o.main:	c.main
o.main:	C:h.kernel
o.main:	C:h.swis
o.main:	h.etc
o.main:	h.err
o.main:	h.mem
o.main:	h.flex
o.main:	h.vectors
o.main:	h.scrap
o.main:	h.fs
o.main:	h.find
o.main:	h.file
o.main:	h.command
o.main:	h.buffer
o.main:	h.deflate
o.main:	h.main
o.mem:	c.mem
o.mem:	C:h.kernel
o.mem:	C:h.swis
o.mem:	h.etc
o.mem:	h.err
o.mem:	h.flex
o.mem:	h.mem
o.scrap:	c.scrap
o.scrap:	C:h.kernel
o.scrap:	C:h.swis
o.scrap:	h.etc
o.scrap:	h.err
o.scrap:	h.fs
o.scrap:	h.flex
o.scrap:	h.scrap
o.trees:	c.trees
o.trees:	C:h.kernel
o.trees:	C:h.swis
o.trees:	h.flex
o.trees:	h.err
o.trees:	h.buffer
o.trees:	h.zip
o.trees:	h.deflatex
o.trees:	h.deflate
o.vectors:	c.vectors
o.vectors:	C:h.kernel
o.vectors:	C:h.swis
o.vectors:	h.etc
o.vectors:	h.constants
o.vectors:	h.err
o.vectors:	h.main
o.vectors:	h.mem
o.vectors:	h.rminfo
o.vectors:	h.fs
o.vectors:	h.gbpb
o.vectors:	h.file
o.vectors:	h.find
o.vectors:	h.args
o.vectors:	h.veclink
o.vectors:	h.vectors
