# Makefile for DebugLib
#

# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <Export$Dir>
LIBDIR		= <Lib$Dir>
EXPORT_DIR	= <Lib$Dir>.DebugLib


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= ${MemCheckCC} cc
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
WIPE		= -wipe
AOF2ALF		= aof.AOF2ALF

CCFLAGS		= -c -depend !Depend -ffah $(INCLUDES) $(DFLAGS) $(THROWBACK)
ASFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~cf~nr~v
LBFLAGS		= -o -c
LNFLAGS		= -aof
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Libraries
#

LIBDIR		= <Lib$Dir>
CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym

REMOTEDBLIB	= $(LIBDIR).debug.o.lremote
PDEBUG		= $(LIBDIR).PDebug.o.Lib_PL
PDEBUGZM	= $(LIBDIR).PDebug.o.Lib_M_PL
TRACELIB	= libs.t_pl
TRACELIBZM	= libs.t_m_pl

# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -Itbox:,C:
DFLAGS		=
#DFLAGS		= -g

# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= DebugLib
TARGET		= DebugLib
TARGETZ		= DebugLibzm
TARGETD		= DebugLibD
TARGETMC	= DebugLibMC
TARGETMCD	= DebugLibMCD
AOFFile		= AOF_File
AOFFileZ	= AOF_FileZ
EXPORTS		= $(EXP_HDR).$(COMPONENT)

OBJS =\
 o.debug\
 o.serial_lib\
 o.buffering\
 o.misc\
 o.options\
 o.debugit\
 o.file\
 o.pdebug\
 o.printf\
 o.serial\
 o.socket\
 o.tml\
 o.tracker\
 o.parse\

OBJSZ =\
 oz.debug\
 oz.serial_lib\
 oz.buffering\
 oz.misc\
 oz.options\
 oz.debugit\
 oz.file\
 oz.pdebug\
 oz.printf\
 oz.serial\
 oz.socket\
 oz.tml\
 oz.tracker\
 oz.parse\

OBJSD =\
 od.debug\
 od.serial_lib\
 od.buffering\
 od.misc\
 od.options\
 od.debugit\
 od.file\
 od.pdebug\
 od.printf\
 od.serial\
 od.socket\
 od.tml\
 od.tracker\
 od.parse\

OBJSMC =\
 oMC.debug\
 oMC.serial_lib\
 oMC.buffering\
 oMC.misc\
 oMC.options\
 oMC.debugit\
 oMC.file\
 oMC.pdebug\
 oMC.printf\
 oMC.serial\
 oMC.socket\
 oMC.tml\
 oMC.tracker\
 oMC.parse\

OBJSMCD =\
 oMCD.debug\
 oMCD.serial_lib\
 oMCD.buffering\
 oMCD.misc\
 oMCD.options\
 oMCD.debugit\
 oMCD.file\
 oMCD.pdebug\
 oMCD.printf\
 oMCD.serial\
 oMCD.socket\
 oMCD.tml\
 oMCD.tracker\
 oMCD.parse\

LIBS = \
 $(PDEBUG) \
 $(TRACELIB) \
 $(REMOTEDBLIB) \

LIBSZ = \
 $(PDEBUGZM) \
 $(TRACELIBZM) \
 $(REMOTEDBLIB)zm \

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .oz .od .oMC .oMCD .s .h .c
.c.o:;		$(CC) $(CCFLAGS) ${EXTRAFLAGS} -o $@ $<
.c.oz:;		$(CC) $(CCFLAGS) ${EXTRAFLAGS} -zm -o $@ $<
.c.od:;		${CC} ${CCFLAGS} ${EXTRAFLAGS} -DDEBUG -fn -o $@ $<
.c.oMC:;	${CC} ${CCFLAGS} ${EXTRAFLAGS} -fn -o $@ $<
.c.oMCD:;	${CC} ${CCFLAGS} ${EXTRAFLAGS} -DDEBUG -fn -o $@ $<
.s.o:;		$(AS) $(ASFLAGS) -o $@ $<


# ------------------------------------------------------------------------------
# Build all the library
#

all:		${TARGET} ${TARGETZ}

dbg:		${TARGETD}

memcheck:	${TARGETMC} ${TARGETMCD}

# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_$(PHASE)

install_rom: $(TARGET)
	$(CP) 	$(TARGET) $(INSTDIR).$(COMPONENT) 		$(CPFLAGS)
	@echo $(COMPONENT): rom module installed

clean:
	$(WIPE) o $(WFLAGS)
	$(WIPE) oz $(WFLAGS)
	${WIPE} od ${WFLAGS}
	${WIPE} oMC ${WFLAGS}
	${WIPE} oMCD ${WFLAGS}
	$(RM) $(TARGET)
	$(RM) $(TARGETZ)
	${RM} ${TARGETD}
	${RM} ${TARGETMC}
	${RM} ${TARGETMCD}
	@echo $(COMPONENT): cleaned

export_hdrs: h.DebugLib dirs
	$(CP)	h.DebugLib	$(EXPORT_DIR).h.DebugLib 	$(CPFLAGS)
	$(CP)   VersionNum      $(EXPORT_DIR).VersionNum 	$(CPFLAGS)
	@echo * $(COMPONENT): export complete (hdrs)

export_libs: $(TARGET) $(TARGETZ) dirs
	$(CP)	$(TARGET)	$(EXPORT_DIR).o.$(TARGET)	$(CPFLAGS)
	$(CP)	$(TARGETZ)	$(EXPORT_DIR).o.$(TARGETZ)	$(CPFLAGS)
	@echo * $(COMPONENT): export complete (libs)

o.dirs:
	$(MKDIR) o
	${MKDIR} od
	${MKDIR} oMC
	${MKDIR} oMCD
	$(MKDIR) oz
	create o.dirs
	stamp  o.dirs

dirs:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(EXPORT_DIR)
	$(MKDIR) $(EXPORT_DIR).h
	$(MKDIR) $(EXPORT_DIR).o

# ------------------------------------------------------------------------------
# Final link
#

$(TARGET): $(OBJS) $(LIBS) o.dirs
	$(LD) $(LNFLAGS) -o $(AOFFile) $(OBJS) $(LIBS)
        $(LB) $(LBFLAGS) $(TARGET) $(AOFFile)
        $(RM) $(AOFFile)

$(TARGETZ): $(OBJSZ) $(LIBSZ) o.dirs
	$(LD) $(LNFLAGS) -o $(AOFFileZ) $(OBJSZ) $(LIBSZ)
        $(LB) $(LBFLAGS) $(TARGETZ) $(AOFFileZ)
        $(RM) $(AOFFileZ)

$(TARGETD): $(OBJSD) $(LIBS) o.dirs
	$(LD) $(LNFLAGS) -o $(AOFFile) $(OBJSD) $(LIBS)
	$(LB) $(LBFLAGS) $(TARGETD) $(AOFFile)
	$(RM) $(AOFFile)

$(TARGETMC): $(OBJSMC) $(LIBS) o.dirs
	$(LD) $(LNFLAGS) -o $(AOFFile) $(OBJSMC) $(LIBS)
	$(LB) $(LBFLAGS) $(TARGETMC) $(AOFFile)
	$(RM) $(AOFFile)

$(TARGETMCD): $(OBJSMCD) $(LIBS) o.dirs
	$(LD) $(LNFLAGS) -o $(AOFFile) $(OBJSMCD) $(LIBS)
	$(LB) $(LBFLAGS) $(TARGETMCD) $(AOFFile)
	$(RM) $(AOFFile)

$(EXP_HDR).$(COMPONENT): hdr.$(COMPONENT)
	$(CP) hdr.$(COMPONENT) $@ $(CPFLAGS)


# ------------------------------------------------------------------------------
# Dynamic dependencies:
