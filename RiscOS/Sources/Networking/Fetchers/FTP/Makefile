# Makefile for FTP
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 10-Feb-98  SNB    Created
# 06-Oct-98  SNB    Updated to use LocalRes and link 5.08
#

COMMON_OBJ = o.commands o.connect o.header o.processdir o.protocol\
 o.readdata o.rewrite o.ses_ctrl o.start o.status o.stop o.utils o.FTPHdr

ROM_OBJS = o.moduleROM ${COMMON_OBJ}

RAM_OBJS = o.module ${COMMON_OBJ} o.msgs

DBG_OBJS = od.module od.commands od.connect od.header od.processdir\
 od.protocol od.readdata od.rewrite od.ses_ctrl od.start od.status\
 od.stop od.utils o.FTPHdr o.msgs

#
# Get main rules
#
include C:Fetchers.FetchMake

clean: fetcher-clean
        ${MKDIR} od
        @echo ${COMPONENT}: cleaned


IMGFILES=LocalRes:Dir LocalRes:DirGrey LocalRes:File LocalRes:FileGrey

resources: LocalRes:Messages ${IMGFILES}
        ${MKDIR} ${RESDIR}
        TokenCheck LocalRes:Messages
        ${CP} LocalRes:Messages  ${RESDIR}.Messages ${CPFLAGS}
        ${CP} LocalRes:Dir       ${RESDIR}.Dir      ${CPFLAGS}
        ${CP} LocalRes:DirGrey   ${RESDIR}.DirGrey  ${CPFLAGS}
        ${CP} LocalRes:File      ${RESDIR}.File     ${CPFLAGS}
        ${CP} LocalRes:FileGrey  ${RESDIR}.FileGrey ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

o.msgs: LocalRes:Messages ${IMGFILES}
        TokenCheck LocalRes:Messages
        resgen messages_file $@ LocalRes:Messages ${RESFSDIR}.Messages \
          LocalRes:Dir        ${RESFSDIR}.Dir \
          LocalRes:DirGrey    ${RESFSDIR}.DirGrey \
          LocalRes:File       ${RESFSDIR}.File \
          LocalRes:FileGrey   ${RESFSDIR}.FileGrey


#---------------------------------------------------------------------------
# Dynamic dependencies:
