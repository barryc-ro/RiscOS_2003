# Makefile for SNMPLib headers

# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date          Name         Description
# ----          ----         -----------
# 01-Jun-99     SNB          Created.

LIBDIR = <Lib$Dir>
SNMPLIBDIR = ${LIBDIR}.SNMP

#
# Generic options:
#
MKDIR   = cdir
WIPE    = -wipe
CP      = copy
RM	= remove

WFLAGS  = ~c~v
CPFLAGS = ~cflnr~v

#
# Program specific options:
#
COMPONENT = headers

HDRS = \
  config.s.h.riscos\
  config.m.h.generic\
  \
  config.h.config\
  config.h.mib_module_config\
  config.h.version\
  \
  h.acl \
  h.asn1 \
  h.context \
  h.int64 \
  h.libsnmp \
  h.md5 \
  h.mib \
  h.parse \
  h.party \
  h.read_config \
  h.snmp \
  h.snmp_api \
  h.snmp_client \
  h.snmp_debug \
  h.snmp_impl \
  h.system \
  h.ucd-snmp-includes \
  h.vacm \
  h.view 

FILES = ${HDRS}

all: $(FILES)

release: ${FILES}
   ${CP} h          ${SNMPLIBDIR}.h           ${CPFLAGS}
   ${CP} agent      ${SNMPLIBDIR}.agent       ${CPFLAGS}
   ${CP} config     ${SNMPLIBDIR}.config      ${CPFLAGS}
   ${CP} mibgroup   ${SNMPLIBDIR}.mibgroup    ${CPFLAGS}

#------------------------------------------------------------------------
# RISC OS Build targets:
#

export: export_${PHASE}

clean:
        @echo ${COMPONENT}: no cleaning required

export_libs:
        @echo ${COMPONENT}: export complete (libs)

export_hdrs: ${HDRS} dirs
        ${CP} h          ${SNMPLIBDIR}.h           ${CPFLAGS}
   	${CP} agent      ${SNMPLIBDIR}.agent       ${CPFLAGS}
        ${CP} config     ${SNMPLIBDIR}.config      ${CPFLAGS}
   	${CP} mibgroup   ${SNMPLIBDIR}.mibgroup    ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${SNMPLIBDIR}

# End of RISC OS Build targets
#------------------------------------------------------------------------

