# Makefile for libjpeg
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 1998-03-16 KJB          Created.
# 2001-06-13 ADH          Wipes o and z directories as well as contents.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CP      = copy
CC      = cc
CM      = cmhg
RM      = remove
LD      = link
LB      = libfile
WIPE    = wipe

CCFLAGS = -c -depend !Depend -ff -throwback ${INCLUDES} ${DFLAGS}
ASFLAGS = -depend !Depend -Stamp -quit
CPFLAGS = ~cfr~v
LBFLAGS = -c
WFLAGS  = ~CFR~V

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
INCLUDES  = -IC:

#
# Symbols to predefine
#
DFLAGS    =

#
# Program specific options:
#
COMPONENT  = o.libjpeg
COMPONENTZ = o.libjpeg-zm
SOURCE     = ...
TARGET     = ${COMPONENT}
TARGETZ    = ${COMPONENTZ}
EXPORTS    = ${EXP_HDR}.${COMPONENT}

OBJS =\
 o.jdapimin\
 o.jdapistd\
 o.jdtrans\
 o.jdmaster\
 o.jdinput\
 o.jdmarker\
 o.jdhuff\
 o.jdphuff\
 o.jdmainct\
 o.jdcoefct\
 o.jdpostct\
 o.jddctmgr\
 o.jidctint\
 o.jdsample\
 o.jdcolor\
 o.jquant2\
 o.jdmerge\
 o.jcomapi\
 o.jutils\
 o.jerror\
 o.jmemmgr\
 o.jmemflex

OBJSZ =\
 z.jdapimin\
 z.jdapistd\
 z.jdtrans\
 z.jdmaster\
 z.jdinput\
 z.jdmarker\
 z.jdhuff\
 z.jdphuff\
 z.jdmainct\
 z.jdcoefct\
 z.jdpostct\
 z.jddctmgr\
 z.jidctint\
 z.jdsample\
 z.jdcolor\
 z.jquant2\
 z.jdmerge\
 z.jcomapi\
 z.jutils\
 z.jerror\
 z.jmemmgr\
 z.jmemflex

#
# Rule patterns
#
# Need to clear the built-in suffixes first before setting our own.
.SUFFIXES:
.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CCFLAGS} -o $@ $<
.c.z:;     ${CC} ${CCFLAGS} -zM -o $@ $<
.s.o:;     ${AS} ${ASFLAGS} -o $@ $<
.s.z:;     ${AS} ${ASFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${COMPONENT} ${COMPONENTZ}

#
# RISC OS ROM build rules:
#
clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} z ${WFLAGS}
        @echo ${COMPONENT}: cleaned

o.local_dirs:
	${MKDIR} o
	${MKDIR} z
        Create o.local_dirs

#
# Final link
#
${TARGET}: ${OBJS} o.local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

${TARGETZ}: ${OBJSZ} o.local_dirs
	${LB} ${LBFLAGS} -o ${COMPONENTZ} ${OBJSZ}


# Dynamic dependencies:
o.jdapimin: c.jdapimin h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdapistd: c.jdapistd h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdtrans: c.jdtrans h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdmaster: c.jdmaster h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdinput: c.jdinput h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdmarker: c.jdmarker h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdhuff: c.jdhuff h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jdhuff
o.jdphuff: c.jdphuff h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jdhuff
o.jdmainct: c.jdmainct h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdcoefct: c.jdcoefct h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdpostct: c.jdpostct h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jddctmgr: c.jddctmgr h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jdct
o.jdsample: c.jdsample h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdcolor: c.jdcolor h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jquant2: c.jquant2 h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jdmerge: c.jdmerge h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jcomapi: c.jcomapi h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jutils: c.jutils h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
o.jerror: c.jerror h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jversion h.jerror h.jerror
o.jmemmgr: c.jmemmgr h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jmemsys
o.jmemflex: c.jmemflex h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jmemsys C:h.flex
z.jdapimin: c.jdapimin h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdapistd: c.jdapistd h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdtrans: c.jdtrans h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdmaster: c.jdmaster h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdinput: c.jdinput h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdmarker: c.jdmarker h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdhuff: c.jdhuff h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jdhuff
z.jdphuff: c.jdphuff h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jdhuff
z.jdmainct: c.jdmainct h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdcoefct: c.jdcoefct h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdpostct: c.jdpostct h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jddctmgr: c.jddctmgr h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jdct
z.jidctint: s.jidctint
z.jdsample: c.jdsample h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdcolor: c.jdcolor h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jquant2: c.jquant2 h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jdmerge: c.jdmerge h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jcomapi: c.jcomapi h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jutils: c.jutils h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror
z.jerror: c.jerror h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jversion h.jerror h.jerror
z.jmemmgr: c.jmemmgr h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jmemsys
z.jmemflex: c.jmemflex h.jinclude h.jconfig h.jpeglib h.jmorecfg h.jpegint h.jerror h.jmemsys C:h.flex
