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

#
# Include files
#
INCLUDES  = -IC:SSLeay,C:TCPIPLibs,C:
DFLAGS    = -DRISCOS -DPKCS11 -D_ACORN

#
# Program specific options:
#
COMPONENT  = cryptolib
TARGET     = ${COMPONENT}
SOURCE     = 
EXPORTS    = 

OBJS = pkcs11.o rsa_id2.o

HDRS = pkcs11.h pkcs11f.h pkcs11t.h rsa_id2.h

LIBS = ${TARGET}

#
# Rule patterns
#
.c.o:;     ${CC} ${CCFLAGS} -ff -zM -o $@ $<
.cmhg.o:;  ${CM} ${CMFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMFLAGS} -d $@ $<

#
# build a the library:
#
all:	${LIBS}

#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom module built

export: export_${PHASE}

install_rom: ${TARGET}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${COMPONENT}
        @echo ${COMPONENT}: cleaned

export_hdrs: ${HDRS} dirs
	${CP} h.pkcs11 ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	${CP} h.pkcs11t ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	${CP} h.pkcs11f ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	${CP} h.rsa_id2 ${CRYPTOLIBDIR}.h.* ${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${LIBS} dirs
	${CP} ${TARGET}  ${CRYPTOLIBDIR}.o.* ${CPFLAGS}
        @echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${CRYPTOLIBDIR}
        ${MKDIR} ${CRYPTOLIBDIR}.h
        ${MKDIR} ${CRYPTOLIBDIR}.o

#
# Final link
#
${TARGET}: ${OBJS} local_dirs
        ${LB} ${LBFLAGS} -o ${COMPONENT} ${OBJS}

# Dynamic dependencies:
o.pkcs11:	c.pkcs11
o.pkcs11:	h.pkcs11
o.pkcs11:	h.pkcs11t
o.pkcs11:	h.pkcs11f
o.pkcs11:	h.pkcs11f
o.pkcs11:	h.pkcs11f
o.pkcs11:	C:h.swis
o.pkcs11:	C:h.kernel
o.pkcs11:	h.ro_mod
o.pkcs11:	c.pkcs11
o.pkcs11:	h.pkcs11
o.pkcs11:	h.pkcs11t
o.pkcs11:	h.pkcs11f
o.pkcs11:	h.pkcs11f
o.pkcs11:	h.pkcs11f
o.pkcs11:	C:h.swis
o.pkcs11:	C:h.kernel
o.pkcs11:	h.ro_mod
o.rsa_id2:	c.rsa_id2
o.rsa_id2:	C:SSLeay.h.cryptlib
o.rsa_id2:	C:SSLeay.h.e_os
o.rsa_id2:	C:SSLeay.h.ro_options
o.rsa_id2:	C:TCPIPLibs.h.socklib
o.rsa_id2:	C:TCPIPLibs.sys.h.types
o.rsa_id2:	C:TCPIPLibs.sys.h.cdefs
o.rsa_id2:	C:TCPIPLibs.machine.h.endian
o.rsa_id2:	C:TCPIPLibs.machine.h.ansi
o.rsa_id2:	C:TCPIPLibs.machine.h.types
o.rsa_id2:	C:TCPIPLibs.sys.h.socket
o.rsa_id2:	C:TCPIPLibs.sys.h.cdefs
o.rsa_id2:	C:TCPIPLibs.sys.h.stat
o.rsa_id2:	C:TCPIPLibs.sys.h.time
o.rsa_id2:	C:TCPIPLibs.sys.h.cdefs
o.rsa_id2:	C:TCPIPLibs.sys.h.cdefs
o.rsa_id2:	C:TCPIPLibs.sys.h.time
o.rsa_id2:	C:TCPIPLibs.sys.h.uio
o.rsa_id2:	C:TCPIPLibs.sys.h.cdefs
o.rsa_id2:	C:h.kernel
o.rsa_id2:	C:SSLeay.h.crypto
o.rsa_id2:	C:SSLeay.h.stack
o.rsa_id2:	C:SSLeay.h.buffer
o.rsa_id2:	C:SSLeay.h.bio
o.rsa_id2:	C:SSLeay.h.crypto
o.rsa_id2:	C:SSLeay.h.err
o.rsa_id2:	C:SSLeay.h.bn
o.rsa_id2:	C:SSLeay.h.rsa
o.rsa_id2:	C:SSLeay.h.bn
o.rsa_id2:	C:SSLeay.h.crypto
o.rsa_id2:	C:SSLeay.h.rand
o.rsa_id2:	h.rsa_id2
o.rsa_id2:	C:SSLeay.h.rsa
o.rsa_id2:	h.pkcs11
o.rsa_id2:	h.pkcs11t
o.rsa_id2:	h.pkcs11f
o.rsa_id2:	h.pkcs11f
o.rsa_id2:	h.pkcs11f
