# 
# Makefile for DumpView application (Omnitel capture buffer decoder).

#
# Paths
#
EXP_HDR = <CExport$Dir>.h

#
# Generic options:
#
MKDIR   = cdir
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
WIPE    = -wipe

AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -depend !Depend -Throwback ${INCLUDES} ${DFLAGS}
#CFLAGS  = -c  ${INCLUDES} ${DFLAGS}
CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs

#
# Include files
#
INCLUDES = -IC: -I^.^.par -I^.services -I^.dl -I^.ph -I^.ph_b  -I^.ph_d \
	   -I^.ns -I^.cc

#DFLAGS   = -dDEBUG
DFLAGS   =

#
# Program specific options:
#
COMPONENT = DumpView
APPNAME	  = !DumpView
APP       = !RunImage
EXPORTS   =
TARGET    =
TARGETSA  =

OBJS =	dumpview.o \
 	dcod.o \
 	dcod_ent.o \
	dcod_acu.o \
	dcod_app.o \
	dcod_atv.o \
	dcod_cc.o \
	dcod_cf.o \
	dcod_dc.o \
	dcod_dl.o \
	dcod_dte.o \
	dcod_ec.o \
	dcod_fm.o \
	dcod_fs.o \
	dcod_ft.o \
	dcod_gst.o \
	dcod_ns.o \
	dcod_ph.o \
	dcod_phr.o \
	dcod_sm.o \
	dcod_sts.o \
	dcod_t30.o \
	dcod_x25.o \
	dcod_xap.o \
	dcod_api.o \
	dcod_aal.o \
	dcod_smi.o


#
# Rule patterns
#
.SUFFIXES: .oz .od
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# Default target
#
all: ${APP}
	@echo ${COMPONENT}: all complete

#
# RiscOS build rules
#

resources: ${APP}
	@echo ${COMPONENT}: resource files copied
	

export:
rom:
install_rom:
install:

clean:
        ${WIPE} o.* ${WFLAGS}
	${RM} ${APP}
        @echo ${COMPONENT}: cleaned

#
# Static dependencies:
#
${APP}: ${OBJS}
        ${LD} -o $@ ${OBJS} ${CLIB}

#
# Dynamic dependencies:
