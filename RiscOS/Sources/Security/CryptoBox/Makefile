# Makefile for CryptoBox
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 06-Oct-97  SJM          Created.
#

#
# Paths
#
EXP_HDR = <export$dir>
LIBDIR = <Lib$Dir>
CRYPTOLIBDIR = ${LIBDIR}.CryptoBox
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
TOUCH   = create
WIPE    = -wipe

CCFLAGS = -c -depend !Depend -fah -wp ${INCLUDES} ${DFLAGS} ${THROWBACK}
ASFLAGS = -depend !Depend -Stamp -quit ${THROWBACK}
CPFLAGS = ~cf~v
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
CPPLIB    = C:C++Lib.o.c++libz

#
# Include files
#
INCLUDES  = -IC:SSLeay,C:TCPIPLibs,C:
DFLAGS    = -DRISCOS -DPKCS11 -D_ACORN

#
# Program specific options:
#
COMPONENT  = CryptoBox
TARGET     = aof.${COMPONENT}
SOURCE     = 
EXPORTS    = 
LIB        = CryptoLib
LIBD       = CryptoLibD

OBJS = pkcs11.o rsa_id2.o
OBJSD = od.pkcs11 od.rsa_id2
HDRS = pkcs11.h pkcs11f.h pkcs11t.h rsa_id2.h

#
# Rule patterns
#
.SUFFIXES: .od

.c.o:;     ${CC} ${CCFLAGS} -ffah -zM -o $@ $<
.c.od:;    ${CC} ${CCFLAGS} -zM -o $@ $< -DDEBUG
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${LIB} ${LIBD} ${TARGET}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o ${WFLAGS}
        ${WIPE} od ${WFLAGS}
        ${RM} ${LIB}
        ${RM} ${LIBD}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} export_dirs
	${CP} h.pkcs11 ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	${CP} h.pkcs11t ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	${CP} h.pkcs11f ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	${CP} h.rsa_id2 ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIB} ${LIBD} export_dirs
	${CP} ${LIB}  ${CRYPTOLIBDIR}.o.* ${CPFLAGS}
	${CP} ${LIBD} ${CRYPTOLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

o.local_dirs:
	${MKDIR} o
	${MKDIR} od
	${TOUCH} $@

export_dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${CRYPTOLIBDIR}
        ${MKDIR} ${CRYPTOLIBDIR}.h
        ${MKDIR} ${CRYPTOLIBDIR}.o

#
# Final link for the ROM Image (using given base address)
#
rom_link:
	${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${TARGET} ${CPPLIB} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

${LIB}: ${OBJS} o.local_dirs
        ${LB} ${LBFLAGS} -o ${LIB} ${OBJS}

${LIBD}: ${OBJSD} o.local_dirs
        ${LB} ${LBFLAGS} -o ${LIBD} ${OBJSD}

# Dynamic dependencies:
