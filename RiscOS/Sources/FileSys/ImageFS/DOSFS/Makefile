# Makefile for DOSFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 25-May-94  AMcC         Created.
# 30-Aug-94  AMcC         Added rom_link rule

# For objasm 3.00 (comment out if using earlier assembler)
APCS_NONE = -apcs none

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

AFLAGS  = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -ffah -zps1 -zM ${INCLUDES} ${DFLAGS}
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

# DFLAGS should be NULL for any non-debugging release
DFLAGS    =
#DFLAGS    = -dTRACE -dTUBE=0

#
# Program specific options:
#
COMPONENT = DOSFS
TARGET    = aof.DOSFS
EXPORTS   = ${EXP_HDR}.DOSFS

OBJS =\
 o.DOSFS\
 o.DOSFSops\
 o.DOSFSctl\
 o.DOSnaming\
 o.FSerrors\
 o.TIMEconv\
 o.wcompare\
 o.debug\
 o.MsgTrans\
 o.Statics\
 o.StaticDefs\
 o.init

#
# Rule patterns
#
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a relocatable module:
#
all: rm.DOSFS

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
        ${RM} h.DOSFShdr
        @echo ${COMPONENT}: cleaned

resources:
        ${MKDIR} ${RESDIR}.${COMPONENT}
        ${CP} Resources.${LOCALE}.Messages  ${RESDIR}.${COMPONENT}.Messages  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied

#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: cmhg.DOSFShdr ${OBJS} ${ROMCSTUBS}
        ${CMHG} cmhg.DOSFShdr o.DOSFShdr
        ${LD} -o $@ -aof o.DOSFShdr ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

rm.DOSFS: cmhg.DOSFShdr ${OBJS} ${CLIB}
        ${CMHG} cmhg.DOSFShdr o.DOSFShdr
        ${LD} -o $@ -module o.DOSFShdr ${OBJS} ${CLIB}

${EXP_HDR}.DOSFS: hdr.DOSFS
        ${CP} hdr.DOSFS $@ ${CPFLAGS}

# Static dependencies:

# recreate h.DOSFShdr if necessary
o.DOSFS: h.DOSFShdr
h.DOSFShdr: cmhg.DOSFShdr
        ${CMHG} cmhg.DOSFShdr o.DOSFShdr h.DOSFShdr
        ${RM} o.DOSFShdr

# DOSFS C Support functions (defines C register definitions)
# Workaround required when using Objasm 3.00
o.DOSFSctl: s.DOSFSctl
        ${AS} ${AFLAGS} ${APCS_NONE} s.DOSFSctl $@

# Dynamic dependencies:
