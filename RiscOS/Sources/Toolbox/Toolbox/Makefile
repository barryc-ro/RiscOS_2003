# Makefile for Toolbox module
#
# Component specific options:
#
RAMMESSAGES = o.msgs

COMMON_OBJ = o.Modhdr_NoD o.base o.callback o.event o.filters o.filter_ven o.globals\
 o.memory o.memswis o.object o.resf o.task

ROM_OBJS = o.mainROM ${COMMON_OBJ}

RAM_OBJS = o.main ${RAMMESSAGES} ${COMMON_OBJ}

DBG_OBJS = o.Modhdr o.base o.callback od.event od.filters o.filter_ven od.globals\
 od.main od.memory o.memswis od.object od.resf od.task ${RAMMESSAGES}

#
# Get main rules
#
include C:tboxlibint.TboxMake

#
# Module specific rules:
#
resources: resources-messages
        @echo ${COMPONENT}: resource files copied to Messages module

clean: toolbox-clean
        @echo ${COMPONENT}: cleaned


#---------------------------------------------------------------------------
# Dynamic dependencies:
