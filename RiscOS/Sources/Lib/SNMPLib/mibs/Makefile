# Makefile for SNMPLib MIBs

# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date          Name         Description
# ----          ----         -----------
# 01-Jun-99     SNB          Created.

LIBDIR = <Lib$Dir>
SNMPLIBDIR = ${LIBDIR}.SNMP
MIBDIR = ${SNMPLIBDIR}.MIB

#
# Generic options:
#
MKDIR   = cdir
WIPE    = -wipe
CP      = copy
RM	= remove

WFLAGS  = ~c~v
CPFLAGS = ~cflnr~v

#
# Program specific options:
#
COMPONENT = MIBs

MIBS = \
 EtherLike-MIB\
 IANAifType-MIB\
 IF-MIB\
 IP-MIB\
 RFC-1215\
 RFC1155-SMI\
 RFC1213-MIB\
 RFC1271-MIB\
 SNMPv2-MIB\
 SNMPv2-PARTY-MIB\
 SNMPv2-SMI\
 SNMPv2-TC\
 SNMPv2-TM\
 TCP-MIB\
 UDP-MIB


FILES = ${MIBS}

all: $(FILES)

release: ${FILES} dirs
	${CP} EtherLike-MIB	${MIBDIR}.EtherLike-MIB		${CPFLAGS}
	${CP} IANAifType-MIB	${MIBDIR}.IANAifType-MIB	${CPFLAGS}
	${CP} IF-MIB		${MIBDIR}.IF-MIB   		${CPFLAGS}
	${CP} IP-MIB		${MIBDIR}.IP-MIB   		${CPFLAGS}
	${CP} RFC-1215    	${MIBDIR}.RFC-1215 		${CPFLAGS}
	${CP} RFC1155-SMI	${MIBDIR}.RFC1155-SMI		${CPFLAGS}
	${CP} RFC1213-MIB	${MIBDIR}.RFC1213-MIB		${CPFLAGS}
	${CP} RFC1271-MIB	${MIBDIR}.RFC1271-MIB		${CPFLAGS}
	${CP} SNMPv2-MIB	${MIBDIR}.SNMPv2-MIB		${CPFLAGS}
	${CP} SNMPv2-PARTY-MIB	${MIBDIR}.SNMPv2-PARTY-MIB	${CPFLAGS}
	${CP} SNMPv2-SMI	${MIBDIR}.SNMPv2-SMI		${CPFLAGS}
	${CP} SNMPv2-TC		${MIBDIR}.SNMPv2-TC		${CPFLAGS}
	${CP} SNMPv2-TM		${MIBDIR}.SNMPv2-TM		${CPFLAGS}
	${CP} TCP-MIB		${MIBDIR}.TCP-MIB		${CPFLAGS}
	${CP} UDP-MIB		${MIBDIR}.UDP-MIB		${CPFLAGS}

#------------------------------------------------------------------------
# RISC OS Build targets:
#

export: export_${PHASE}

clean:
        @echo ${COMPONENT}: no cleaning required

export_libs:
        @echo ${COMPONENT}: export complete (libs)

export_hdrs: release dirs
        @echo ${COMPONENT}: export complete (hdrs)

dirs:
        ${MKDIR} ${LIBDIR}
        ${MKDIR} ${SNMPLIBDIR}
        ${MKDIR} ${MIBDIR}

# End of RISC OS Build targets
#------------------------------------------------------------------------

