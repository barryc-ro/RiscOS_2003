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
RPCGEN  = rpcgen

COMPONENT = NFS
CMHGNAME  = nfscmhg

XCOMPONENT = NFSSTB

DEPENDS = -depend !Depend ${THROWBACK}
DEFINES = -DPACKETSIZE_COMMAND -DDEBUGON=$(DBUG) ${COMPAT} ${OPTIONS} -Darm -DRPC_SVC=0 -DRPC_HDR=1

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ffcah -c -zM -zps1 -wp ${INCLUDES} ${DEPENDS} ${DEFINES}
RPCCCFLAGS = -ff -c -zM -zps1 -wp ${INCLUDES} ${DEPENDS} ${DEFINES} ${RPCCCFLAGS_EXTRA} 
CMHGFLAGS = -p ${DEPENDS}
RPCGENFLAGS = -i 0 -DWANT_NFS3

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
DEBUGLIB  = C:DebugLib.o.DebugLibZM


#
# Internet version compatibility
#
COMPAT= -DCOMPAT_INET4

#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:

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
  o.printfuncs    \
  o.chains        \
  o.enumdir       \
  o.enumdir2      \
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
  o.Hourglass     \
  o.extensions    \
  o.access        \
  o.nfsrw         \
  o.db 		  \
  o.rquota_xdr    \
  o.nfs_prot_clnt \
  o.nfs_prot_xdr  \
  o.pcnfsd_clnt   \
  o.pcnfsd_xdr    \
  o.mount_clnt    \
  o.mount_xdr     \
  o.SixtyFour     \
  o.ModuleWrap    \
  o.Commands      \
  o.Utils         \
  o.Clients       \
  o.ChangeInfo    \
  o.PutBytesX     \
  o.pathmunge     \
  o.Cache         \
  o.V2support     \
  o.memory        \
  o.memswis       \
  o.fh3

HDRS = \
  mount.h         \
  nfs_prot.h      \
  pcnfsd.h        \
  rquota.h
  
#
# Rule patterns
#
.SUFFIXES:  .o .s .c .h .x .cmhg
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

clean: autogen-clean
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
${TARGET}: ${XTARGET} ${OBJS} ${HDRS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB}
        ${LD} -o $@ -rmf ${OBJS} ${RPCLIB} ${DEBUGLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB}
        $(MODSQZ) $(TARGET)
        Access $@ RW/R

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${HDRS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB}
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
# Some rules that need explicitly declaring because AMU is crap
#

# One area per function.  Helps linker throw away all the unused auto-generated code.
# The compiler cannot typically optimise register usage here because eventually all
# XDR related functions end up inside rpclib:xdr.c anyway.
RPCCCFLAGS_EXTRA = -zo

c.mount_clnt   :  x.mount ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -l -o $@ mount.x
c.mount_xdr    :  x.mount ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -c -o $@ mount.x
c.nfs_prot_clnt:  x.nfs_prot ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -l -o $@ nfs_prot.x
c.nfs_prot_xdr :  x.nfs_prot ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -c -o $@ nfs_prot.x
c.pcnfsd_clnt  :  x.pcnfsd ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -l -o $@ pcnfsd.x
c.pcnfsd_xdr   :  x.pcnfsd ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -c -o $@ pcnfsd.x
c.rquota_clnt  :  x.rquota ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -l -o $@ rquota.x
c.rquota_xdr   :  x.rquota ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -c -o $@ rquota.x

h.rquota: x.rquota
	${RPCGEN} ${RPCGENFLAGS} -h -o $@ $*.x
h.pcnfsd: x.pcnfsd
	${RPCGEN} ${RPCGENFLAGS} -h -o $@ $*.x
h.nfs_prot: x.nfs_prot
	${RPCGEN} ${RPCGENFLAGS} -h -o $@ $*.x
h.mount: x.mount
	${RPCGEN} ${RPCGENFLAGS} -h -o $@ $*.x

o.rquota_xdr: c.rquota_xdr
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.rquota_clnt: c.rquota_clnt
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.pcnfsd_xdr: c.pcnfsd_xdr
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.pcnfsd_clnt: c.pcnfsd_clnt
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.mount_xdr: c.mount_xdr
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.mount_clnt: c.mount_clnt
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.nfs_prot_xdr: c.nfs_prot_xdr
	${CC} ${RPCCCFLAGS} -o $@ $*.c
o.nfs_prot_clnt: c.nfs_prot_clnt
	${CC} ${RPCCCFLAGS} -o $@ $*.c

autogen-clean:
	${RM} h.rquota
	${RM} h.pcnfsd
	${RM} h.nfs_prot
	${RM} h.mount
	${RM} c.mount_clnt
	${RM} c.nfs_prot_clnt
	${RM} c.pcnfsd_clnt
	${RM} c.rquota_clnt
	${RM} c.mount_xdr
	${RM} c.nfs_prot_xdr
	${RM} c.rquota_xdr
	${RM} c.pcnfsd_xdr

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
