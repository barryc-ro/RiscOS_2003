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
COMPONENT	= ScreenFade


ROM_AOF_FILE	= @.!ROM_AOF.Output_M


all:
	Echo	Component $(COMPONENT): AMU target required.


# rom_aof_file is a label for an internal target, consisting of all source 
# files partially linked with all required libraries and RISCOSLIB:o.romcstubs 
# (not C:o.stubs).
#
rom_aof:
	Obey		@.!ROM_AOF.!Run



# Now all main targets required during ROM builds.


clean:
	Every.Every2	-c "@.!*.Sources @.!*.Output* @.!*.Makefile"	Wipe %0 ~cr~v
	Remove		linked.$(COMPONENT)
	Remove		$(ROM_AOF_FILE)
	Echo		Component $(COMPONENT): 'clean' finished.


export:
	Echo		Component $(COMPONENT): 'export' finished.


resources:
	IfThere		LocalRes:	Then Copy LocalRes: <resource$dir>.Resources.$(COMPONENT) ~cfr~v
	Echo		Component $(COMPONENT): 'resources' finished.


rom:	rom_aof
	Echo		Component $(COMPONENT): 'rom' finished.


install_rom:	rom_aof
	Copy		$(ROM_AOF_FILE) $(INSTDIR).$(COMPONENT) ~cfr~v
	Echo		Component $(COMPONENT): 'install_rom' finished.


rom_link:	rom_aof
	CDir		linked
        Link		-o linked.$(COMPONENT) -rmf -base ${ADDRESS} $(ROM_AOF_FILE) RISC_OSLib:o.AbsSym
        Copy		linked.$(COMPONENT) ${LINKDIR}.$(COMPONENT) ~cfr~v
	Echo		Component $(COMPONENT): 'rom_link' finished.





# Dynamic dependencies:
