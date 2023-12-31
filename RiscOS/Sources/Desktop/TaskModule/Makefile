# $Id
#
# This makefile builds the various targets of RISC OS modules required 
# during a ROM build.
#
# It only does the ROM-specific things such as copying resources and 
# AOF files.
#
# It is assumed that the application-directory '!ROM_AOF' is present,
# and '!ROM_AOF.!Run' compiles all source files and creates a 
# partially-linked AOF file '!ROM_AOF.Output_M', which is linked with
# all required libraries and RISCOSLIB:o.romcstubs rather than 
# C:o.stubs
#
# Things are done in this way so that different builds of the component 
# can be inside !* directories, for debugging purposes. Only the 
# '!ROM_AOF' directory is used in actual ROM builds (Note that putting 
# the ROM build into the root directory while having debug builds in 
# sub-directories doesn't work as AMU's VPATH will cause the ROM 
# build's .o files to be used in Debug builds.)


# This is the only project-specific thing in this makefile.
#
COMPONENT	= TaskModule

EXP_HDR = <CExport$Dir>

CPFLAGS = ~cfr~v

RAM_MOD_FILE	= @.!Module.Output_M
ROM_AOF_FILE	= @.!ROM_AOF.Output_M


all:
	Echo	Component $(COMPONENT): AMU target required.


ram_mod:
	Obey		@.!Module.!Run

# rom_aof_file is a label for an internal target, consisting of all source 
# files partially linked with all required libraries and RISCOSLIB:o.romcstubs 
# (not C:o.stubs).
#
rom_aof:
	Obey		@.!ROM_AOF.!Run

# Now all main targets required during ROM builds.

clean:
	Remove		linked.$(COMPONENT)
	Remove		map.$(COMPONENT)
	Remove		$(ROM_AOF_FILE)
	Every.Every2	-c "@.!*.h @.!*.Sources @.!*.linkout @.!*.Output* @.!*.Makefile"	Wipe %0 ~cr~v
	Echo		Component $(COMPONENT): 'clean' finished.


export: export_${PHASE}

export_libs:
	@echo ${COMPONENT}: export complete(libs)

export_hdrs:
	Copy		Sources.h.TaskModule ${EXP_HDR}.h.TaskModule ${CPFLAGS}
	Echo		Component $(COMPONENT): 'export' finished.


resources:
	IfThere		@.resources.$(LOCALE)	Then Copy @.resources.$(LOCALE) <resource$dir>.Resources.$(COMPONENT) ~cfr~v
	Echo		Component $(COMPONENT): 'resources' finished.


ram:	ram_mod
	Echo		Component $(COMPONENT): 'ram' finished.


rom:	rom_aof
	Echo		Component $(COMPONENT): 'rom' finished.


install:	ram
	mkdir -p	$(INSTDIR)
	Copy		$(RAM_MOD_FILE) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
	Echo		Component $(COMPONENT): 'install' (disc) finished.


install_rom:	rom_aof
	Copy		$(ROM_AOF_FILE) $(INSTDIR).$(COMPONENT) $(CPFLAGS)
	Echo		Component $(COMPONENT): 'install' (rom) finished.


rom_link:	rom_aof
	CDir		linked
	CDir		map
        Link		-o linked.$(COMPONENT) -rmf -base ${ADDRESS} $(ROM_AOF_FILE) RISC_OSLib:o.AbsSym
        Copy		linked.$(COMPONENT) ${LINKDIR}.$(COMPONENT) ~cfr~v
	Echo		Component $(COMPONENT): 'rom_link' finished.

# Dynamic dependencies:
