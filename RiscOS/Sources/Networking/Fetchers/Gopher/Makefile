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

COMMON_OBJ = o.commands o.connect o.processdir\
 o.readdata o.ses_ctrl o.start o.status o.stop o.utils o.GopherHdr

ROM_OBJS = o.moduleROM ${COMMON_OBJ}

RAM_OBJS = o.module ${COMMON_OBJ} o.msgs

DBG_OBJS = o.msgs od.module od.commands od.connect od.processdir\
 od.readdata od.ses_ctrl od.start od.status od.stop od.utils o.GopherHdr

#
# Get main rules
#
include C:Fetchers.FetchMake

clean: fetcher-clean
        @echo ${COMPONENT}: cleaned


IMGFILES=LocalRes:Dir LocalRes:File

resources: LocalRes:Messages ${IMGFILES}
        ${MKDIR} ${RESDIR}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.Messages ${CPFLAGS}
        ${CP} LocalRes:Dir       ${RESDIR}.Dir      ${CPFLAGS}
        ${CP} LocalRes:File      ${RESDIR}.File     ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

o.msgs: LocalRes:Messages ${IMGFILES}
        TokenCheck LocalRes:Messages
        resgen messages_file $@ LocalRes:Messages ${RESFSDIR}.Messages \
          LocalRes:Dir        ${RESFSDIR}.Dir \
          LocalRes:File       ${RESFSDIR}.File


#---------------------------------------------------------------------------
# Dynamic dependencies:
