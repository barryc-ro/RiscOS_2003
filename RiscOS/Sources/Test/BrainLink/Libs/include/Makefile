# Makefile for BrainLink headers
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name         Description
# ----         ----         -----------
# 2001-04-03   NBingham     Created
#

#
# Paths
#
include Stdtools

EXPORT_DIR  = <Lib$Dir>.BrainLink.h

export: export_${PHASE}
        @echo ${COMPONENT}: export complete

export_hdrs:
        ${MKDIR} ${EXPORT_DIR}
        ${CP} h.brainlink ${EXPORT_DIR}.brainlink   ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs:
        @echo ${COMPONENT}: export complete (libs)

# Dynamic dependencies:
