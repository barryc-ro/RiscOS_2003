# Makefile for Interlace
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 14-Sep-94  JRH          Created.
# 14-Feb-95  TMD          Re-integrated versions
# 21-Feb-95  TMD	  Made modes 50,51,55 in a single binary
# 17-Mar-95  JRH	  Stopped all target from doing an install
# 17-Oct-95  TMD	  Removed obsolete variants 't','u','v' from disc build
# 18-Jul-96  TMD	  Added 'y' variant for STB1 MPEG2
# 05-Feb-97  TMD	  Added 'z' variant for ART
#

#
# Paths
#
EXP_HDR = <export$dir>

#
# Generic options:
#
MKDIR   = cdir
AS      = aasm
CP      = copy
RM      = remove
WIPE    = wipe
CCFLAGS = -c -depend !Depend -IC:
ASFLAGS = -depend !Depend -Stamp -quit -module
CPFLAGS = ~cfr~v

#
# Program specific options:
#
COMPONENT = Interlace
ROMTARGET =	rm.${MACHINE}.Interlace

DISCTARGETS =	rm.Interls \
		rm.Interlw \
		rm.Interlx \
		rm.Interly \
		rm.Interlz

INSTROMTARGET = ${INSTDIR}.${COMPONENT}

# This is a nasty hack to stop amu from complaining when INSTDIR is not set.
HACK=.hack.^

INSTALLTARGETS = ${INSTDIR}.Interls${HACK} \
		 ${INSTDIR}.Interlw${HACK} \
		 ${INSTDIR}.Interlx${HACK} \
		 ${INSTDIR}.Interly${HACK} \
		 ${INSTDIR}.Interlz${HACK}

.SUFFIXES: .Hdr .rm ${HACK}

#
# Generic rules:
#
clean:
	-${WIPE} rm.* ~CR~V
        @echo ${COMPONENT}: rom modules cleaned

rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom modules built

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTROMTARGET} ${CPFLAGS}
        @echo ${COMPONENT}: rom modules installed

all: ${DISCTARGETS}
	@echo ${COMPONENT}: disc modules built

install: ${INSTALLTARGETS}
	@echo ${COMPONENT}: disc modules installed

${INSTDIR}.Interls${HACK}: rm.Interls
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlt${HACK}: rm.Interlt
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlu${HACK}: rm.Interlu
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlv${HACK}: rm.Interlv
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlw${HACK}: rm.Interlw
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interly${HACK}: rm.Interly
	${CP} $? $@ ${CPFLAGS}

${INSTDIR}.Interlz${HACK}: rm.Interlz
	${CP} $? $@ ${CPFLAGS}


#
# ROM variants
#
rm.Omega.Interlace: rm.Interlx
        ${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.Falcon.Interlace: rm.Interlw
        ${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}

rm.Peregrine.Interlace: rm.Interly
	${MKDIR} rm.${MACHINE}
	${CP} $? $@ ${CPFLAGS}
	
.Hdr.rm:; ${AS} ${ASFLAGS} -From $< -To $@

# Dynamic dependencies:
