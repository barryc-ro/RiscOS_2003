# Makefile for LDAP library include files
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 20-Jul-2000 SNB          Created.

#
# Paths
#
LIBDIR = <Lib$Dir>.LDAP
COMPONENT .= LDAP

include Makefiles:StdTools

export_hdrs: export_dirs
        ${CP} h ${LIBDIR}.h ${CPFLAGS}
        ${CP} ^.VersionNum ${LIBDIR}.LibVersion ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export: export_${PHASE}

clean:
        @echo ${COMPONENT}: cleaned

export_libs:
        @echo ${COMPONENT}: export complete (libs)

export_dirs:
        ${MKDIR} ${LIBDIR}

# Dynamic dependencies:
