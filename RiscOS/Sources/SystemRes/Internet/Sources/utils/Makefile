# Makefile
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 08-Oct-96  KJB    Created
#

COMPONENT = !Internet

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
SQUEEZE = squeeze
WIPE    = -wipe

AFLAGS  = ${THROWBACK} -depend !Depend -stamp -quit
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v


FILES=utl.CheckMem utl.ReadCMOSIP utl.RMFind utl.TriggerCBs utl.X

#
# Rule patterns
#
.SUFFIXES:  .o .s
.s.o:;      ${AS} ${AFLAGS} -o $@ $<

#
# Main rules:
#
# Utilities
#
all: ${FILES}
        @echo ${COMPONENT}: Utilties built (Disc)

clean:
        ${WIPE} o.* ${WFLAGS}
        ${WIPE} utl.* ${WFLAGS}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#

utl.CheckMem: o.CheckMem
	${LD} -bin -o $@ $?
	SetType $@ &FFC


utl.X: o.X
	${LD} -bin -o $@ $?
	SetType $@ &FFC


utl.TriggerCBs: o.TriggerCBs
	${LD} -bin -o $@ $?
	SetType $@ &FFC


utl.RMFind: o.RMFind
	${LD} -bin -o $@ $?
	SetType $@ &FFC


utl.ReadCMOSIP: o.ReadCMOSIP
	${LD} -bin -o $@ $?
	SetType $@ &FFC


#---------------------------------------------------------------------------
# Dynamic dependencies:
o.ReadCMOSIP: s.ReadCMOSIP
o.CheckMem: s.CheckMem
o.X: s.X
o.RMFind: s.RMFind
