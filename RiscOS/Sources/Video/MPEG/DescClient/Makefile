# Makefile for DescClient
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date         Name    Description
# ----         ----    -----------
# 22-Jan-2003  SAR     Created from the DummyVerifier module
# 06-Feb-2003  SAR     Updated to take DEBUG, XOR and TEL build options
# 06-Feb-2003  SAR     Integrated with descrambling stuff from Telefonica

COMPONENT = DescClient
TARGET    = ${COMPONENT}
OBJS      = ModHdr.o Module.o DWrite.o ${TELLIB}
LIBS      =
#
# If ${DEBUG} = -DDEBUG=1      Then this build will output debug to screen (16bpp)
# If ${XOR}   = -DXOR_DEMO=1   Then this build will perform demo XOR descrambling
# If ${TEL}   = -DTELEFONICA=1 Then this build will perform the Telefonica descrambling
#
CFLAGS     = -ffah -zM ${DEBUG} ${XOR} ${TEL}
CMHGFLAGS += ${DEBUG} ${XOR} ${TEL}

include Makefiles:StdTools
include Makefiles:ModuleLibs
include Makefiles:ModStdRule
include Makefiles:RAMCModule
include Makefiles:ROMCModule

Module.o: Module.c ModHdr.h
        ${CC} ${CFLAGS} -o $@ Module.c

clean:
        ${RM} h.ModHdr
        ${WIPE} aof ${WFLAGS}
        ${WIPE} o.DWrite ${WFLAGS}
        ${WIPE} o.ModHdr ${WFLAGS}
        ${WIPE} o.Module ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        @echo ${COMPONENT}: cleaned

# Dynamic dependencies:
