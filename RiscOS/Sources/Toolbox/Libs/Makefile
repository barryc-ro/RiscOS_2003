# Makefile for toolbox libraries
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 10 Oct 1997   RWB		Tidied up based on previous trunk version

# ------------------------------------------------------------------------------
# Paths
#

LIBDIR		= <Lib$Dir>
# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
CD		= dir
AS		= objasm
CP		= copy
RM		= remove
WIPE		= -wipe
MAKE		= amu_machine

CPFLAGS		= ~CLFNR~V
WFLAGS		= ~CFR~V

COMPONENT	= tboxlibs

# -------------------------------------------------------
# Now all the different targets we can be asked to build:
# -------------------------------------------------------

clean:
	${CD}   toolboxlib
	${MAKE} clean
	${CD}   ^
	${CD}   wimplib
	${MAKE} clean
	${CD}   ^
	${CD}   flexlib
	${MAKE} clean
	${CD}   ^
	${CD}   eventlib
	${MAKE} clean
	${CD}   ^
	${CD}   renderlib
	${MAKE} clean
	${CD}   ^
	@echo	Library $(COMPONENT): clean complete.


export:	export_$(PHASE)

export_hdrs:
	${CD}   toolboxlib
	${MAKE} export PHASE=hdrs
	${CD}   ^
	${CD}   wimplib
	${MAKE} export PHASE=hdrs
	${CD}   ^
	${CD}   flexlib
	${MAKE} export PHASE=hdrs
	${CD}   ^
	${CD}   eventlib
	${MAKE} export PHASE=hdrs
	${CD}   ^
	${CD}   renderlib
	${MAKE} export PHASE=hdrs
	${CD}   ^
	@echo	Library $(COMPONENT): export headers complete.
	

export_libs:
	${CD}   toolboxlib
	${MAKE} export PHASE=libs
	${CD}   ^
	${CD}   wimplib
	${MAKE} export PHASE=libs
	${CD}   ^
	${CD}   flexlib
	${MAKE} export PHASE=libs
	${CD}   ^
	${CD}   eventlib
	${MAKE} export PHASE=libs
	${CD}   ^
	${CD}   renderlib
	${MAKE} export PHASE=libs
	${CD}   ^
	@echo	Library $(COMPONENT): export libraries complete.


# Dynamic dependencies:
