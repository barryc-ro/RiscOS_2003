# Makefile for Europe variant of ISDN
#
# This Makefile just defines variant-dependent macros that are used by the
# main Makefile in the main directory.


VARIANT= VARIANT_CFLAGS=-DVARIANT_US SUFFIX=_ DEBUG=-DD_ASSERT


# Commands
#

CD = 	dir
MAKE=	amu $(MFLAGS)


# RiscOS build targets
#

resources export install rom install_rom clean all:
        @$(CD) ^.^
        @$(MAKE) $(VARIANT)
# Dynamic dependencies:
