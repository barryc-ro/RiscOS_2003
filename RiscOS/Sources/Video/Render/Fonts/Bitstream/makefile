# CSPModule OBJASM/C Makefile
#
# *****************************************
# ***       C h a n g e   L i s t       ***
# *****************************************
# Date       	Name         	Description
# ----       	----         	-----------
# 17 Sep 1997	RWB		Created

#
# Paths
#
EXP_HDR = <export$dir>.^.Interface2
EXP_C_H = <Cexport$dir>.h
EXP_C_O = <Cexport$dir>.o

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
CD	= dir
CHMOD	= access

AFLAGS     = -depend !Depend ${THROWBACK} -Stamp -quit
CFLAGS     = -c -depend !Depend ${THROWBACK} -zM -ff ${INCLUDES} ${DFLAGS} -ec
CMHGFLAGS  = -p ${DFLAGS} ${THROWBACK} ${INCLUDES}
CPFLAGS    = ~cfr~v
WFLAGS     = ~c~vr
CHMODFLAGS = RW/R

DFLAGS  = -DUSE_TINY ${DEBUG} -DCSP_MAX_DYNAMIC_FONTS=3  -DINCL_TPS=0  -DSTRUCTALIGN=4  -DCSP_DEBUG=0 
#
# Libraries
#
CLIB       = CLIB:o.stubs
RLIB       = RISCOSLIB:o.risc_oslib
RSTUBS     = RISCOSLIB:o.rstubs
ROMSTUBS   = RISCOSLIB:o.romstubs
ROMCSTUBS  = RISCOSLIB:o.romcstubs
ABSSYM     = RISC_OSLib:o.AbsSym
REMOTEDB   = <Lib$Dir>.debug.o.remotezm
#
# Include files
#
INCLUDES = -IC:,<Lib$Dir>.,lib.

# Program specific options:
#
COMPONENT = CSPModule
TARGET    = aof.${COMPONENT}
RMTARGET  = rm.${COMPONENT}
EXPORTS   = ${EXP_C_H}.${COMPONENT}

OBJS      =	\
o.CACHEMGR	\
o.CM_BMP	\
o.CSPPD		\
o.csp_anti	\
o.CSP_API	\
o.CSP_BMP	\
o.CSP_CBX	\
o.CSP_CRV	\
o.CSP_DBG	\
o.CSP_DIR	\
o.CSP_EMB	\
o.CSP_FBX	\
o.CSP_GPS	\
o.CSP_GRY	\
o.CSP_MEM	\
o.CSP_NFT	\
o.CSP_OTL	\
o.CSP_PDR	\
o.CSP_PFR	\
o.CSP_QCK	\
o.CSP_STK	\
o.CSP_STR	\
o.csp_swi	\
o.CSP_TCB	\
o.CSP_UTL	\
o.fontlist	\
o.TEXTOUT	\
o.cmodhdr

#
# Rule patterns
#
.c.o:;      ${CC}   ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS}   ${AFLAGS} $< $@


# build a relocatable module:
#
all: ${RMTARGET}
	@echo ${COMPONENT}: all complete
	
#
# RISC OS ROM build rules:
#
rom: ${TARGET}
        @echo ${COMPONENT}: rom complete

export: ${EXPORTS}
        @echo ${COMPONENT}: export complete

install_rom: ${TARGET}
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.*      ${WFLAGS}
        ${WIPE} linked.* ${WFLAGS}
        ${WIPE} map.*    ${WFLAGS}
        ${RM} ${RMTARGET}
        ${RM} ${TARGET}
        ${RM} h.CModHdr
        ${RM} ${TARGET}
        @echo ${COMPONENT}: cleaned
        
#
# ROM target (re-linked at ROM Image build time)
#
${TARGET}: ${OBJS} ${ROMCSTUBS} 
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${TARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Relocatable module target
#
${RMTARGET}: ${OBJS}
        ${LD} -rmf -o $@ ${OBJS} ${CLIB}
        ${CHMOD} rm.${COMPONENT} ${CHMODFLAGS}

${EXP_C_H}.${COMPONENT}:	h.${COMPONENT}
        ${CP} h.${COMPONENT} $@ ${CPFLAGS}

#
# Dynamic dependencies:
o.CACHEMGR:	c.CACHEMGR
o.CACHEMGR:	lib.h.csp_int
o.CACHEMGR:	lib.h.csp_api
o.CACHEMGR:	lib.h.csp_opt
o.CACHEMGR:	lib.h.btstypes
o.CACHEMGR:	lib.h.spglobal
o.CACHEMGR:	lib.h.cachemgr
o.CACHEMGR:	lib.h.cm_opt
o.CM_BMP:	c.CM_BMP
o.CM_BMP:	lib.h.csp_int
o.CM_BMP:	lib.h.csp_api
o.CM_BMP:	lib.h.csp_opt
o.CM_BMP:	lib.h.btstypes
o.CM_BMP:	lib.h.spglobal
o.CM_BMP:	lib.h.cachemgr
o.CM_BMP:	lib.h.cm_opt
o.CSPPD:	c.CSPPD
o.CSPPD:	lib.h.btstypes
o.CSPPD:	lib.h.csp_api
o.CSPPD:	lib.h.csp_opt
o.CSPPD:	lib.h.graphics
o.CSPPD:	lib.h.csp_pdr
o.csp_anti:	c.csp_anti
o.CSP_API:	c.CSP_API
o.CSP_API:	lib.h.csp_int
o.CSP_API:	lib.h.csp_api
o.CSP_API:	lib.h.csp_opt
o.CSP_API:	lib.h.btstypes
o.CSP_API:	lib.h.spglobal
o.CSP_API:	lib.h.cachemgr
o.CSP_API:	lib.h.cm_opt
o.CSP_BMP:	c.CSP_BMP
o.CSP_BMP:	lib.h.csp_int
o.CSP_BMP:	lib.h.csp_api
o.CSP_BMP:	lib.h.csp_opt
o.CSP_BMP:	lib.h.btstypes
o.CSP_BMP:	lib.h.spglobal
o.CSP_CBX:	c.CSP_CBX
o.CSP_CBX:	lib.h.csp_int
o.CSP_CBX:	lib.h.csp_api
o.CSP_CBX:	lib.h.csp_opt
o.CSP_CBX:	lib.h.btstypes
o.CSP_CBX:	lib.h.spglobal
o.CSP_CRV:	c.CSP_CRV
o.CSP_CRV:	lib.h.csp_int
o.CSP_CRV:	lib.h.csp_api
o.CSP_CRV:	lib.h.csp_opt
o.CSP_CRV:	lib.h.btstypes
o.CSP_CRV:	lib.h.spglobal
o.CSP_DBG:	c.CSP_DBG
o.CSP_DBG:	lib.h.csp_int
o.CSP_DBG:	lib.h.csp_api
o.CSP_DBG:	lib.h.csp_opt
o.CSP_DBG:	lib.h.btstypes
o.CSP_DBG:	lib.h.spglobal
o.CSP_DIR:	c.CSP_DIR
o.CSP_DIR:	lib.h.csp_int
o.CSP_DIR:	lib.h.csp_api
o.CSP_DIR:	lib.h.csp_opt
o.CSP_DIR:	lib.h.btstypes
o.CSP_DIR:	lib.h.spglobal
o.CSP_EMB:	c.CSP_EMB
o.CSP_EMB:	lib.h.csp_int
o.CSP_EMB:	lib.h.csp_api
o.CSP_EMB:	lib.h.csp_opt
o.CSP_EMB:	lib.h.btstypes
o.CSP_EMB:	lib.h.spglobal
o.CSP_FBX:	c.CSP_FBX
o.CSP_FBX:	lib.h.csp_int
o.CSP_FBX:	lib.h.csp_api
o.CSP_FBX:	lib.h.csp_opt
o.CSP_FBX:	lib.h.btstypes
o.CSP_FBX:	lib.h.spglobal
o.CSP_GPS:	c.CSP_GPS
o.CSP_GPS:	lib.h.csp_int
o.CSP_GPS:	lib.h.csp_api
o.CSP_GPS:	lib.h.csp_opt
o.CSP_GPS:	lib.h.btstypes
o.CSP_GPS:	lib.h.spglobal
o.CSP_GRY:	c.CSP_GRY
o.CSP_GRY:	lib.h.csp_int
o.CSP_GRY:	lib.h.csp_api
o.CSP_GRY:	lib.h.csp_opt
o.CSP_GRY:	lib.h.btstypes
o.CSP_GRY:	lib.h.spglobal
o.CSP_MEM:	c.CSP_MEM
o.CSP_MEM:	lib.h.csp_int
o.CSP_MEM:	lib.h.csp_api
o.CSP_MEM:	lib.h.csp_opt
o.CSP_MEM:	lib.h.btstypes
o.CSP_MEM:	lib.h.spglobal
o.CSP_NFT:	c.CSP_NFT
o.CSP_NFT:	lib.h.csp_int
o.CSP_NFT:	lib.h.csp_api
o.CSP_NFT:	lib.h.csp_opt
o.CSP_NFT:	lib.h.btstypes
o.CSP_NFT:	lib.h.spglobal
o.CSP_NFT:	lib.h.cachemgr
o.CSP_NFT:	lib.h.cm_opt
o.CSP_OTL:	c.CSP_OTL
o.CSP_OTL:	lib.h.csp_int
o.CSP_OTL:	lib.h.csp_api
o.CSP_OTL:	lib.h.csp_opt
o.CSP_OTL:	lib.h.btstypes
o.CSP_OTL:	lib.h.spglobal
o.CSP_PDR:	c.CSP_PDR
o.CSP_PDR:	lib.h.csp_api
o.CSP_PDR:	lib.h.csp_opt
o.CSP_PDR:	lib.h.graphics
o.CSP_PDR:	lib.h.csp_pdr
o.CSP_PFR:	c.CSP_PFR
o.CSP_PFR:	lib.h.csp_int
o.CSP_PFR:	lib.h.csp_api
o.CSP_PFR:	lib.h.csp_opt
o.CSP_PFR:	lib.h.btstypes
o.CSP_PFR:	lib.h.spglobal
o.CSP_QCK:	c.CSP_QCK
o.CSP_QCK:	lib.h.csp_int
o.CSP_QCK:	lib.h.csp_api
o.CSP_QCK:	lib.h.csp_opt
o.CSP_QCK:	lib.h.btstypes
o.CSP_QCK:	lib.h.spglobal
o.CSP_STK:	c.CSP_STK
o.CSP_STK:	lib.h.csp_int
o.CSP_STK:	lib.h.csp_api
o.CSP_STK:	lib.h.csp_opt
o.CSP_STK:	lib.h.btstypes
o.CSP_STK:	lib.h.spglobal
o.CSP_STR:	c.CSP_STR
o.CSP_STR:	lib.h.csp_int
o.CSP_STR:	lib.h.csp_api
o.CSP_STR:	lib.h.csp_opt
o.CSP_STR:	lib.h.btstypes
o.CSP_STR:	lib.h.spglobal
o.csp_swi:	c.csp_swi
o.csp_swi:	C:h.kernel
o.csp_swi:	lib.h.csp_api
o.csp_swi:	lib.h.csp_opt
o.csp_swi:	lib.h.draw
o.csp_swi:	lib.h.speedo
o.csp_swi:	lib.h.useropt
o.csp_swi:	lib.h.btstypes
o.csp_swi:	h.fontprots
o.CSP_TCB:	c.CSP_TCB
o.CSP_TCB:	lib.h.csp_int
o.CSP_TCB:	lib.h.csp_api
o.CSP_TCB:	lib.h.csp_opt
o.CSP_TCB:	lib.h.btstypes
o.CSP_TCB:	lib.h.spglobal
o.CSP_TCB:	lib.h.cachemgr
o.CSP_TCB:	lib.h.cm_opt
o.CSP_UTL:	c.CSP_UTL
o.CSP_UTL:	lib.h.csp_int
o.CSP_UTL:	lib.h.csp_api
o.CSP_UTL:	lib.h.csp_opt
o.CSP_UTL:	lib.h.btstypes
o.CSP_UTL:	lib.h.spglobal
o.fontlist:	c.fontlist
o.fontlist:	h.fontprots
o.TEXTOUT:	c.TEXTOUT
o.TEXTOUT:	lib.h.btstypes
o.TEXTOUT:	lib.h.csp_api
o.TEXTOUT:	lib.h.csp_opt
o.TEXTOUT:	lib.h.graphics
