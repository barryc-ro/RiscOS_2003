# Makefile for snmpapps
#
# ***********************************
# ***	 C h a n g e   L i s t	  ***
# ***********************************
# Date	     Name   Description
# ----	     ----   -----------
# 01-Jun-99  SNB    Created
#
.SUFFIXES: .c .o

#
# Component specific options:
#
COMPONENT = SNMPapps

EXPDIR = ${INSTDIR}

DIRS=o._dirs
TARGETDIR=aif._dirs

COMPONENT1  = snmpget
COMPONENT2  = snmpgetnext
COMPONENT3  = snmpset
COMPONENT4  = snmpstatus
COMPONENT5  = snmptest
COMPONENT6  = snmptranslate
COMPONENT7  = snmptrap
COMPONENT8  = snmpdelta
COMPONENT9  = snmpbulkwalk
COMPONENT10 = snmpwalk

TARGET1  = aif.${COMPONENT1}
TARGET2  = aif.${COMPONENT2}
TARGET3  = aif.${COMPONENT3}
TARGET4  = aif.${COMPONENT4}
TARGET5  = aif.${COMPONENT5}
TARGET6  = aif.${COMPONENT6}
TARGET7  = aif.${COMPONENT7}
TARGET8  = aif.${COMPONENT8}
TARGET9  = aif.${COMPONENT9}
TARGET10 = aif.${COMPONENT10}

TARGETS = ${TARGET1} ${TARGET2} ${TARGET3} ${TARGET4} ${TARGET5}\
          ${TARGET6} ${TARGET7} ${TARGET8} ${TARGET9} ${TARGET10}

BINACCESS = WR/R

#
# Generic options:
#
CHMOD	= access
MKDIR	= cdir
CC	= cc
CMHG	= cmhg
CP	= copy
LD	= link
RM	= remove
SQZ	= squeeze
WIPE	= -wipe

CPFLAGS = ~cfr~v
WFLAGS  = ~c~vr

SQZFLAGS = -f
CFLAGS	 = -c -depend !Depend ${THROWBACK} ${INCLUDES} -Wpan ${DFLAGS}
DFLAGS	 = -D${SYSTEM} -DRISCOS -DHAVE_CONFIG_H
LDFLAGS  =
INCLUDES = -ITCPIPLibs:,C:SNMP,C:SNMP.config,C:


LIBS = C:SNMP.o.snmplib ${NETLIBS} C:o.stubs
NETLIBS = TCPIPLibs:o.unixlib TCPIPLibs:o.socklib TCPIPLibs:o.inetlib

OFILES1  = snmpget.o snmp_parse_args.o
OFILES2  = snmpgetnext.o snmp_parse_args.o
OFILES3  = snmpset.o snmp_parse_args.o
OFILES4  = snmpstatus.o snmp_parse_args.o
OFILES5  = snmptest.o snmp_parse_args.o
OFILES6  = snmptranslate.o snmp_parse_args.o
OFILES7  = snmptrap.o snmp_parse_args.o
OFILES8  = snmpdelta.o snmp_parse_args.o
OFILES9  = snmpbulkwalk.o snmp_parse_args.o
OFILES10 = snmpwalk.o snmp_parse_args.o

#
# Main rules:
#
#
all: ${TARGETS} ${DIRS}
	@echo ${COMPONENT}: targets built

install: all
	${MKDIR} ${EXPDIR}
	${SQZ} ${TARGET1} ${EXPDIR}.${COMPONENT1} ${SQZFLAGS}
	${SQZ} ${TARGET2} ${EXPDIR}.${COMPONENT2} ${SQZFLAGS}
	${SQZ} ${TARGET3} ${EXPDIR}.${COMPONENT3} ${SQZFLAGS}
	${SQZ} ${TARGET4} ${EXPDIR}.${COMPONENT4} ${SQZFLAGS}
	${SQZ} ${TARGET5} ${EXPDIR}.${COMPONENT5} ${SQZFLAGS}
	${SQZ} ${TARGET6} ${EXPDIR}.${COMPONENT6} ${SQZFLAGS}
	${SQZ} ${TARGET7} ${EXPDIR}.${COMPONENT7} ${SQZFLAGS}
	${SQZ} ${TARGET8} ${EXPDIR}.${COMPONENT8} ${SQZFLAGS}
	${SQZ} ${TARGET9} ${EXPDIR}.${COMPONENT9} ${SQZFLAGS}
	${SQZ} ${TARGET10} ${EXPDIR}.${COMPONENT10} ${SQZFLAGS}
	Access ${EXPDIR}.${COMPONENT1} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT2} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT3} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT4} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT5} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT6} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT7} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT8} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT9} ${BINACCESS}
	Access ${EXPDIR}.${COMPONENT10} ${BINACCESS}
        @echo *** ${COMPONENT} installation complete

clean:
	${WIPE} o ${WFLAGS}
	${WIPE} aif ${WFLAGS}
	@echo ${COMPONENT}: cleaned

${TARGET1}: ${OFILES1} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES1} ${LIBS}
	Access $@ RW/R

${TARGET2}: ${OFILES2} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES2} ${LIBS}

${TARGET3}: ${OFILES3} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES3} ${LIBS}

${TARGET4}: ${OFILES4} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES4} ${LIBS}

${TARGET5}: ${OFILES5} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES5} ${LIBS}

${TARGET6}: ${OFILES6} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES6} ${LIBS}

${TARGET7}: ${OFILES7} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES7} ${LIBS}

${TARGET8}: ${OFILES8} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES8} ${LIBS}

${TARGET9}: ${OFILES9} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES9} ${LIBS}

${TARGET10}: ${OFILES10} ${LIBS} ${TARGETDIR}
	${LD} ${LDFLAGS} -o $@ ${OFILES10} ${LIBS}

.c.o:
        ${CC} $(CFLAGS) -o $@ $<

${TARGETDIR}: ${DIRS}
	${MKDIR} aif
	create $@

${DIRS}:
	${MKDIR} o
	create $@

# Dynamic dependencies:
