# Makefile for a URL fetcher module
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 14-Jul-00  SNB    Created
#
RAMMESSAGES = o.msgs

DIRS = o._dirs

COMMONLIBS = C:LDAP.o.LDAPzm C:LDAP.o.LBERzm

COMMON_OBJ = o.connect o.headers o.memory o.protocol\
 o.readdata o.ses_ctrl o.start o.status o.stop o.utils o.writedata o.LDAPHdr\
 ${COMMONLIBS}

ROM_OBJS = o.moduleROM ${COMMON_OBJ}

RAM_OBJS = o.module ${COMMON_OBJ} ${RAMMESSAGES}

DBG_OBJS = ${RAMMESSAGES} od.module od.connect od.headers od.memory od.protocol\
 od.readdata od.ses_ctrl od.start od.status od.stop od.utils od.writedata o.LDAPHdr ${COMMONLIBS}

CINCLUDES = -IC:LDAP -IC:AsmUtils

#
# Get main rules
#
include C:Fetchers.FetchMake

clean: fetcher-clean
        ${RM} o
        @echo ${COMPONENT}: cleaned

resources: resources-messages
        @echo ${COMPONENT}: resource files copied to Messages module

${DIRS}:
        ${MKDIR} o
        ${TOUCH} $@

o.module: h.LDAPHdr

#---------------------------------------------------------------------------
# Dynamic dependencies:
