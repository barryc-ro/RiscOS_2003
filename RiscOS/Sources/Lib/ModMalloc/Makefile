# Template makefile for library in RISC OS source tree.

# This is the only project-specific part of this makefile
#
COMPONENT	= ModMalloc

# Copy is used in a number of place in the same way, so we put
#appropriate flags into a macro:
CopyFlags	= ~cfr~v


# -------------------------------------------------------
# Now all the different targets we can be asked to build:
# -------------------------------------------------------


all:
	Echo	Library $(COMPONENT): Please supply a target.


clean:
	Wipe o.Lib* ~c~r~v
	Echo Library $(COMPONENT): 'clean' finished.


clean_all:	clean
	Every.Every2	-c "@.o.Sources.!*.Sources @.o.Sources.!*.Makefile" wipe %0 ~c~vr
	Echo		Library $(COMPONENT): 'clean_all' finished.


export:	export_$(PHASE)

export_hdrs:
	CDir		<Lib$Dir>.$(COMPONENT)
	Copy		@.h		<Lib$Dir>.$(COMPONENT).h		$(CopyFlags)
	Copy		@.$(COMPONENT)	<Lib$Dir>.$(COMPONENT).$(COMPONENT)	$(CopyFlags)
	Echo		Library $(COMPONENT): 'export_hdrs' finished.


export_libs:
	CDir		<Lib$Dir>.$(COMPONENT)
	CDir		<Lib$Dir>.$(COMPONENT).o
	Every.Every2	-c @.o.Sources.!* Obey %0.!Run
	Every.Every2	-c @.o.lib* copy %0 <Lib$Dir>.$(COMPONENT).o.%1		$(CopyFlags)
	Echo		Library $(COMPONENT): 'export_libs' finished.


resources:
	Echo		Library $(COMPONENT): 'resources' finished.

rom:
	Echo		Library $(COMPONENT): 'rom' finished.

install_rom:
	Echo		Library $(COMPONENT): 'install_rom' finished.

rom_link:
	Echo		Library $(COMPONENT): 'rom_link' finished.

ram:
	Echo		Library $(COMPONENT): 'ram' finished.



# Dynamic dependencies:
