# Makefile for a Toolbox Object module
#
# $Id$
#
# Component specific options:
#
RAMRESOURCES = o.msgs

COMMON_OBJ = o.Modhdr_NoD o.create o.delete o.events o.getstate\
 o.hide o.miscop o.show o.task

ROM_OBJS = o.mainROM ${COMMON_OBJ}

RAM_OBJS = o.main ${RAMRESOURCES} ${COMMON_OBJ}

DBG_OBJS = od.main ${RAMRESOURCES} o.Modhdr od.create od.delete od.events od.getstate\
 od.hide od.miscop od.show od.task

#
# Get main rules
#
include C:tboxlibint.TboxMake

#
# Module specific rules:
#
resources: LocalRes:DCS.Messages LocalREs:DCS.Res LocalRes:Quit.Messages LocalRes:Quit.Res
        ${CP} Resources.${LOCALE}.*   ${RESDIR}.*  ${CPFLAGS}
        @echo ${COMPONENT}: resource files copied to Messages module

${RAMRESOURCES}: LocalRes:DCS.Messages LocalRes:DCS.Res LocalRes:Quit.Messages LocalRes:Quit.Res
        resgen res_files $@ \
          LocalRes:DCS.Res Resources.DCS.Res \
          LocalRes:DCS.Messages Resources.DCS.Messages \
          LocalRes:Quit.Res Resources.Quit.Res \
          LocalRes:Quit.Messages Resources.Quit.Messages

clean: toolbox-clean
        @echo ${COMPONENT}: cleaned

#---------------------------------------------------------------------------
# Dynamic dependencies:
