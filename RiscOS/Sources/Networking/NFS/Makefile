# Makefile for NFS
#
# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date       Name         Description
# ----       ----         -----------
# 05-May-95  RWB          Mangled.
# 09 Feb 96  DNW          Now make a 'h' file from the cmhg info
#                         Makes decodeing the *cmd easier
# 12-Mar-97  jmf	  Upgrade to Internet 5 libraries.
# 11-Nov-98  SNB          Replaced NFSSTB entity (post Spinner merge) with dummy
#

#
# Paths
#
EXP_HDR = <export$dir>

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
MODSQZ  = modsqz
WIPE    = -wipe
CD	= dir

#
# Uncomment this to add the access counting to the *NFSInfo output
#
#OPTIONS   = -DNETACCESSCOUNT

COMPONENT = NFS
CMHGNAME  = nfscmhg

XCOMPONENT = NFSSTB

DEPENDS = -depend !Depend ${THROWBACK}
DEFINES = -DPACKETSIZE_COMMAND -DDEBUGON=$(DBUG) ${COMPAT} ${OPTIONS} -Darm

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -c -zM -wdp -ffcah -zps1 ${INCLUDES} ${DEPENDS} ${DEFINES}
CPCCFLAGS=-c -zM -pcc -ffa -zps1 ${INCLUDES} ${DEPENDS} ${DEFINES}
CMHGFLAGS = -p ${DEPENDS}

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

#
# Libraries
#
CLIB      = CLIB:o.stubs
RLIB      = RISCOSLIB:o.risc_oslib
RSTUBS    = RISCOSLIB:o.rstubs
ROMSTUBS  = RISCOSLIB:o.romstubs
ROMCSTUBS = RISCOSLIB:o.romcstubs
ABSSYM    = RISC_OSLib:o.AbsSym
RPCLIB    = TCPIPLibs:rpc.o.rpc_datazm TCPIPLibs:rpc.o.rpclibzm
INETLIB   = TCPIPLibs:o.inetlibzm
SOCKLIB   = TCPIPLibs:o.socklibzm
UNIXLIB   = TCPIPLibs:o.unixlibzm


#
# Internet version compatibility
#
COMPAT= -DCOMPAT_INET4

#
# Include files
#
INCLUDES = -IC:,TCPIPLibs:

#
# Debug flag
#
DBUG = 0

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
XTARGET   = rm.${XCOMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

OBJS      =	  \
  $(CMHGOBJ)   	  \
  o.ModuleWrap    \
  o.Commands      \
  o.Utils         \
  o.Clients       \
  o.ChangeInfo    \
  o.PutBytesX     \
  o.pathmunge     \
  o.Cache         \
  o.printfuncs    \
  o.chains        \
  o.enumdir       \
  o._Args         \
  o._Close        \
  o._File         \
  o._Func         \
  o._GBPB         \
  o._GetBytes     \
  o._Open         \
  o._PutBytes     \
  o.Interface     \
  o.RMInfo        \
  o.Static        \
  o.allerrs       \
  o.nfs_prot_c    \
  o.nfs_prot_x    \
  o.pcnfsd_c      \
  o.pcnfsd_x      \
  o.mount_clnt    \
  o.mount_xdr     \
  o.Hourglass     \
  o.extensions    \
  o.access        \
  o.nfsrw         \
  o.db 		  \
  o.rquota

#
# Rule patterns
#
#  .SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@

#
# build a NFS module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

#
# RISC OS ROM build rules:
#
rom: ${ROMTARGET}
        @echo ${COMPONENT}: rom module built

export: 
        @echo ${COMPONENT}: export complete

install_rom: ${ROMTARGET}
        ${CP} ${ROMTARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom module installed

clean:
        ${WIPE} o.* ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${XTARGET}
        ${RM} ${ROMTARGET}
        ${RM} linked.${COMPONENT}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${XTARGET} ${OBJS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB}
        ${LD} -o $@ -rmf ${OBJS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB}
        $(MODSQZ) $(TARGET)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB}
        ${LD} -o $@ -aof ${OBJS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ROMCSTUBS}

#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${LD} -o linked.${COMPONENT} -rmf -base ${ADDRESS} ${ROMTARGET} ${ABSSYM}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete

#
# Some rules that need explicitly declaring
#
o.mount_clnt :  c.mount_clnt
                $(CC) ${CPCCFLAGS} c.mount_clnt

o.mount_xdr :   c.mount_xdr
                $(CC) ${CPCCFLAGS} c.mount_xdr

h.nfsstbcmhg:	$(CMHGOBJ)
	@

h.nfscmhg:	$(CMHGOBJ)
	@

o.Commands:     h.nfscmhg
o.ModuleWrap:   h.nfscmhg

# Note, cannot replace -bin -base 0 with -rmf here otherwise you get all sorts
# of extra relocation code inserted into the final image.
${XTARGET}: o.${XCOMPONENT}
	${LD} -o $@ -bin -base 0 o.${XCOMPONENT}
	SetType $@ Module
	
# Dynamic dependencies:
