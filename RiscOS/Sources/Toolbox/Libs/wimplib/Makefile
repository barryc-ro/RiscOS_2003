# Makefile for wimplib
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date		Name		Description
# ----		----		-----------
# 1997-09-03	BAL		Created
# 09 Oct 1997   RWB		Removed some reduntant phases
#


# ------------------------------------------------------------------------------
# Paths
#

EXP_HDR		= <export$dir>
LIBDIR		= <Lib$Dir>


# ------------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CP		= copy
CC		= cc
CM		= cmhg
RM		= remove
LD		= link
LB		= libfile
WIPE		= -wipe

CFLAGS		= -c -depend !Depend -ffah -strict ${INCLUDES} ${DFLAGS}
AFLAGS		= -depend !Depend -Stamp -quit
CPFLAGS		= ~CLFNR~V
LBFLAGS		= -c
WFLAGS		= ~CFR~V


# ------------------------------------------------------------------------------
# Libraries
#

CLIB		= CLIB:o.stubs
RLIB		= RISCOSLIB:o.risc_oslib
RSTUBS		= RISCOSLIB:o.rstubs
ROMSTUBS	= RISCOSLIB:o.romstubs
ROMCSTUBS	= RISCOSLIB:o.romcstubs
ABSSYM		= RISC_OSLib:o.AbsSym


# ------------------------------------------------------------------------------
# Include files
#

INCLUDES	= -IC:
DFLAGS		= 


# ------------------------------------------------------------------------------
# Program specific options:
#

COMPONENT	= wimplib
TARGET		= ${COMPONENT}
EXPORTS		= ${EXP_HDR}.${COMPONENT}

OBJS		=\
 o.waddmsg\
 o.wbasesp\
 o.wblockcpy\
 o.wclosedown\
 o.wclosete\
 o.wclosew\
 o.wcommw\
 o.wcreatei\
 o.wcreatem\
 o.wcreatesm\
 o.wcreatew\
 o.wdecodem\
 o.wdeletei\
 o.wdeletew\
 o.wdragbox\
 o.wforcere\
 o.wforcere2\
 o.wgetcarp\
 o.wgeticst\
 o.wgetmenu\
 o.wgetpoi\
 o.wgetrec\
 o.wgetwinf\
 o.wgetwinfni\
 o.wgetwino\
 o.wgetwst\
 o.wgetwst2\
 o.winit\
 o.wloadte\
 o.wopente\
 o.wopenw\
 o.wopenw2\
 o.wplotic\
 o.wpoll\
 o.wpollidle\
 o.wprocessk\
 o.wreadpal\
 o.wredraww\
 o.wremmsg\
 o.wreporte\
 o.wresizeic\
 o.wsendm\
 o.wsetcarp\
 o.wsetcol\
 o.wsetcolm\
 o.wsetext\
 o.wsetfon\
 o.wseticst\
 o.wsetmode\
 o.wsetpal\
 o.wsetpos\
 o.wslotsize\
 o.wspriteop\
 o.wstartt\
 o.wsysinfo\
 o.wtextcol\
 o.wtextop\
 o.wtransbl\
 o.wupdatew\
 o.wwhichi\
 o.!!version
 

# ------------------------------------------------------------------------------
# Rule patterns
#

.SUFFIXES: .o .s .h .cmhg .c .z
.c.o:;     ${CC} ${CFLAGS} -o $@ $<
.s.o:;     ${AS} ${AFLAGS} -o $@ $<
.cmhg.o:;  ${CM} ${CMHGFLAGS} -o $@ $<
.cmhg.h:;  ${CM} ${CMHGFLAGS} -d $@ $<


# ------------------------------------------------------------------------------
# RISC OS ROM build rules
#

export: export_${PHASE}

clean:
	${WIPE} o.* ${WFLAGS}
	${RM} ${TARGET}
	@echo ${COMPONENT}: cleaned

export_hdrs: h.wimplib dirs
	${CP}	h.wimplib	${LIBDIR}.tboxlibs.h.wimplib	${CPFLAGS}
	@echo ${COMPONENT}: export complete (hdrs)

export_libs: ${TARGET} dirs
	${CP}	${TARGET}	${LIBDIR}.tboxlibs.o.${TARGET}	${CPFLAGS}
	@echo ${COMPONENT}: export complete (libs)

local_dirs:
	${MKDIR} o

dirs:
	${MKDIR} ${LIBDIR}
	${MKDIR} ${LIBDIR}.tboxlibs
	${MKDIR} ${LIBDIR}.tboxlibs.h
	${MKDIR} ${LIBDIR}.tboxlibs.o
        

# ------------------------------------------------------------------------------
# Final link
#

${TARGET}: ${OBJS} local_dirs
	${LB} ${LBFLAGS} -o ${TARGET} ${OBJS}

${EXP_HDR}.${COMPONENT}: hdr.${COMPONENT}
	${CP} hdr.${COMPONENT} $@ ${CPFLAGS}


# ------------------------------------------------------------------------------
# Dynamic dependencies:
o.wgetwino: s.wgetwino
o.wgetwst: s.wgetwst
o.wgetwst2: s.wgetwst2
o.winit: s.winit
o.wloadte: s.wloadte
o.wopente: s.wopente
o.wopenw: s.wopenw
o.wopenw2: s.wopenw2
o.wplotic: s.wplotic
o.wpoll: s.wpoll
o.wpollidle: s.wpollidle
o.wprocessk: s.wprocessk
o.wreadpal: s.wreadpal
o.wredraww: s.wredraww
o.wremmsg: s.wremmsg
o.wreporte: s.wreporte
o.wresizeic: s.wresizeic
o.wsendm: s.wsendm
o.wsetcarp: s.wsetcarp
o.wsetcol: s.wsetcol
o.wsetcolm: s.wsetcolm
o.wsetext: s.wsetext
o.wsetfon: s.wsetfon
o.wseticst: s.wseticst
o.wsetmode: s.wsetmode
o.wsetpal: s.wsetpal
o.wsetpos: s.wsetpos
o.wslotsize: s.wslotsize
o.wspriteop: s.wspriteop
o.wstartt: s.wstartt
o.wsysinfo: s.wsysinfo
o.wtextcol: s.wtextcol
o.wtextop: s.wtextop
o.wtransbl: s.wtransbl
o.wupdatew: s.wupdatew
o.wwhichi: s.wwhichi
o.!!version: s.!!version
o.waddmsg: s.waddmsg
o.wbasesp: s.wbasesp
o.wblockcpy: s.wblockcpy
o.wclosedown: s.wclosedown
o.wclosete: s.wclosete
o.wclosew: s.wclosew
o.wcommw: s.wcommw
o.wcreatei: s.wcreatei
o.wcreatem: s.wcreatem
o.wcreatesm: s.wcreatesm
o.wcreatew: s.wcreatew
o.wdecodem: s.wdecodem
o.wdeletei: s.wdeletei
o.wdeletew: s.wdeletew
o.wdragbox: s.wdragbox
o.wforcere: s.wforcere
o.wforcere2: s.wforcere2
o.wgetcarp: s.wgetcarp
o.wgeticst: s.wgeticst
o.wgetmenu: s.wgetmenu
o.wgetpoi: s.wgetpoi
o.wgetrec: s.wgetrec
o.wgetwinf: s.wgetwinf
o.wgetwinfni: s.wgetwinfni
