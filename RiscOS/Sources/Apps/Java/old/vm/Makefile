# Makefile for completing Java ROM build
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name   Description
# ----       ----   -----------
# 22/4/97  RLougher Created
#

#
# Component specific options:
#
COMPONENT  = java
ROM_MODULE = aof.$(COMPONENT)

RESDIR = <resource$dir>.Resources2.$(COMPONENT)

SYMFILTER = filter
GENINDEX = genindex
FIDDLE = fiddle

#
# Generic options:
#
MKDIR   = cdir
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
ANSILIB   = CLib:o.ansilib
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.c_abssym

OBJ       = image
SYM       = symbols
ENTRY     = entries
INDEX     = index

VM        = vm
JPEG      = jpeg
MMEDIA    = mmedia
NET       = net
AWT       = awt

OBJS = \
	$(OBJ).$(VM) \
	$(OBJ).$(AWT) \
	$(OBJ).$(NET) \
	$(OBJ).$(JPEG) \
	$(OBJ).$(MMEDIA)

ENTRIES = \
	$(ENTRY).$(AWT) \
	$(ENTRY).$(JPEG) \
	$(ENTRY).$(NET) \
	$(ENTRY).$(MMEDIA)

SYMBOLS = syms

#
# Main rules:
#

rom: ${ROM_MODULE}
        @echo ${COMPONENT}: Module built (ROM)

install_rom: ${ROM_MODULE}
        ${CP} ${ROM_MODULE} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: Module installed (ROM)

resources: indexes
	$(MKDIR) $(RESDIR)
	$(CP) $(INDEX).* $(RESDIR).* $(CPFLAGS)
	@echo $(COMPONENT): resource files copied to Messages module


indexes: $(SYMBOLS) $(OBJS) $(ENTRIES)
	$(MKDIR) $(SYM)
	$(MKDIR) $(INDEX)
        $(SYMFILTER) $(SYMBOLS) $(SYM).$(VM) $(AWT) $(JPEG) $(MMEDIA) $(NET)
        $(GENINDEX) $(SYM).$(VM) $(INDEX).$(VM) 
        $(GENINDEX) $(SYM).$(AWT) $(INDEX).$(AWT)
        $(GENINDEX) $(SYM).$(JPEG) $(INDEX).$(JPEG)
        $(GENINDEX) $(SYM).$(NET) $(INDEX).$(NET)
        $(GENINDEX) $(SYM).$(MMEDIA) $(INDEX).$(MMEDIA)
        
clean:
        ${WIPE} $(INDEX) ${WFLAGS}
        ${WIPE} $(SYM) ${WFLAGS}
        $(WIPE) $(SYMBOLS) $(WFLAGS)
        ${WIPE} aof ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        ${WIPE} map ${WFLAGS}
        ${WIPE} fiddled ${WFLAGS}
        @echo ${COMPONENT}: cleaned

$(SYMBOLS): $(OBJS)
	$(MKDIR) aof
	$(FIDDLE) fiddle_syms $(ROMCSTUBS) fiddled
        ${LD} -o $(ROM_MODULE) -aof -symbols syms0 ${OBJS} fiddled
	awk "$1~/(._stub$)|(^__DLL__ENTRIES_.)/ {print $1, $2}" syms0 \
	    { > $@ }
	
${ROM_MODULE}: $(OBJS)
	$(MKDIR) aof
	$(FIDDLE) fiddle_syms $(ROMCSTUBS) fiddled
        ${LD} -o $@ -symbols $(SYMBOLS) -aof ${OBJS} fiddled

# final link for ROM Image (using given base address)
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} \
		$(ABSSYM) ${ROM_MODULE} -map > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#---------------------------------------------------------------------------
# Dynamic dependencies:
