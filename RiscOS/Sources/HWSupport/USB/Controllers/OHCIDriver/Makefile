# Makefile for OHCIDriver

# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date          Name         Description
# ----          ----         -----------
# 21-Mar-01     SNB          Recreated

COMPONENT = OHCIDriver
USBDIR = <Lib$Dir>.USB

#
# Generic options:
#
include StdTools


#
# Program specific options:
#

#------------------------------------------------------------------------
# RISC OS Build targets:
#

export: export_${PHASE}

clean:
        @echo ${COMPONENT}: no cleaning required

export_libs:
        @echo ${COMPONENT}: export complete (libs)

export_hdrs:
        ${MKDIR} ${USBDIR}.h
        ${CP} h.OHCI     ${USBDIR}.h.OHCI          ${CPFLAGS}
        ${CP} h.ohcireg  ${USBDIR}.h.ohcireg       ${CPFLAGS}
        ${CP} VersionNum ${USBDIR}.OHCIVersion     ${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

#------------------------------------------------------------------------
