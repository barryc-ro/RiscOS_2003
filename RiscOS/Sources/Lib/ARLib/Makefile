# Makefile for ARLib libraries
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date          Name            Description
# ----          ----            -----------
# 14 Nov 2000   BJGA            Created from Toolbox libraries Makefile
#

# ------------------------------------------------------------------------------
# Paths
#

LIBDIR          = <Lib$Dir>

# ------------------------------------------------------------------------------
# Generic options
#

MKDIR           = mkdir -p
CD              = dir
AS              = objasm
CP              = copy
RM              = remove
WIPE            = -wipe
MAKE            = amu_machine

CPFLAGS         = ~CLFNR~V
WFLAGS          = ~CFR~V

COMPONENT       = ARLib

# ------------------------------------------------------------------------------
# Build rules
#

clean:
        ${CD}   ARLib
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   DBox
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Draw
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   File
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Mem
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Replay
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Spr
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Struc
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Sys
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        ${CD}   Wimp
        ${MAKE} clean
        stripdepnd
        ${CD}   ^
        @echo   Library $(COMPONENT): clean complete.

export: export_$(PHASE)

export_hdrs:
        ${CD}   ARLib
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   DBox
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Draw
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   File
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Mem
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Replay
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Spr
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Struc
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Sys
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        ${CD}   Wimp
        ${MAKE} export PHASE=hdrs
        ${CD}   ^
        @echo   Library $(COMPONENT): export headers complete.

export_libs:
        ${CD}   ARLib
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   DBox
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Draw
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   File
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Mem
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Replay
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Spr
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Struc
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Sys
        ${MAKE} export PHASE=libs
        ${CD}   ^
        ${CD}   Wimp
        ${MAKE} export PHASE=libs
        ${CD}   ^
        @echo   Library $(COMPONENT): export libraries complete.

# Dynamic dependencies:
