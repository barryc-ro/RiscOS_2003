# Makefile for a URL fetcher module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 10-Feb-98  SNB    Created
# 06-Oct-98  SNB    Updated to use LocalRes and link 5.08
#
RAMMESSAGES = o.msgs

COMMON_OBJ = o.commands o.connect o.headers o.protocol\
 o.readdata o.ses_ctrl o.start o.status o.stop o.utils o.writedata o.GenericHdr

ROM_OBJS = o.moduleROM ${COMMON_OBJ}

RAM_OBJS = o.module ${COMMON_OBJ} ${RAMMESSAGES}

DBG_OBJS = ${RAMMESSAGES} od.module od.commands od.connect od.headers od.protocol\
 od.readdata od.ses_ctrl od.start od.status od.stop od.utils od.writedata o.GenericHdr

#
# Get main rules
#
include C:Fetchers.FetchMake

clean: fetcher-clean
        ${MKDIR} od
        @echo ${COMPONENT}: cleaned

resources: resources-messages
        @echo ${COMPONENT}: resource files copied to Messages module

#---------------------------------------------------------------------------
# Dynamic dependencies:
