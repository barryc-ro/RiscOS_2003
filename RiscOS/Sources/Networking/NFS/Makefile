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
MKDIR   = mkdir -p
AS      = objasm
CC      = cc
CMHG    = cmhg
CP      = copy
LD      = link
RM      = remove
MODSQZ  = modsqz
WIPE    = -wipe
RPCGEN  = rpcgen
TOUCH   = create
SED     = sed

COMPONENT = NFS
CMHGNAME  = nfscmhg

XCOMPONENT = NFSSTB

DEPENDS = -depend !Depend ${THROWBACK}
DEFINES = -DPACKETSIZE_COMMAND ${COMPAT} ${OPTIONS} -Darm -DRPC_SVC=0 -DRPC_HDR=1

AFLAGS = ${DEPENDS} -Stamp -quit
CFLAGS  = -ffcah -c -zM -zps1 -wp ${INCLUDES} ${DEPENDS} ${DEFINES}
RPCCCFLAGS = -ff -c -zM -zps1 -wp ${INCLUDES} ${DEPENDS} ${DEFINES} ${RPCCCFLAGS_EXTRA} 
CMHGFLAGS = -p ${DEPENDS}
RPCGENFLAGS = -i 0 -DWANT_NFS3 -debuglib

CPFLAGS = ~cfr~v
WFLAGS  = ~c~v

DIRS=od._dirs

TEMPORARY=tmpfile

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
ASMLIB    = C:AsmUtils.o.AsmUtilsZM


#
# Internet version compatibility
#
COMPAT= -DCOMPAT_INET4

#
# Include files
#
INCLUDES = -ITCPIPLibs:,C:,C:AsmUtils

#
# Debug flag
#
DBUG = 0

#
# Program specific options:
#
TARGET    = rm.${COMPONENT}
DTARGET   = rm.D${COMPONENT}
XTARGET   = rm.${XCOMPONENT}
CMHGOBJ   = o.$(CMHGNAME)
ROMTARGET = aof.${COMPONENT}

OBJS      =	  \
  $(CMHGOBJ)   	  \
  o.xdr_manual    \
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
  o.Static        \
  o.allerrs       \
  o.Hourglass     \
  o.extensions    \
  o.access        \
  o.nfsrw         \
  o.rquota_xdr    \
  o.nfs_prot_clnt \
  o.nfs_prot_xdr  \
  o.pcnfsd_clnt   \
  o.pcnfsd_xdr    \
  o.mount_clnt    \
  o.mount_xdr     \
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
  o.fh3

DEBOBJS      =	  \
  $(CMHGOBJ)   	  \
  od.xdr_manual   \
  od.printfuncs    \
  od.chains        \
  od.enumdir       \
  od.enumdir2      \
  od._Args         \
  od._Close        \
  od._File         \
  od._Func         \
  od._GBPB         \
  od._GetBytes     \
  od._Open         \
  od._PutBytes     \
  o.Interface     \
  od.Static        \
  o.allerrs       \
  od.Hourglass     \
  od.extensions    \
  od.access        \
  od.nfsrw         \
  od.rquota_xdr    \
  od.nfs_prot_clnt \
  od.nfs_prot_xdr  \
  od.pcnfsd_clnt   \
  od.pcnfsd_xdr    \
  od.mount_clnt    \
  od.mount_xdr     \
  od.ModuleWrap    \
  od.Commands      \
  od.Utils         \
  od.Clients       \
  od.ChangeInfo    \
  od.PutBytesX     \
  od.pathmunge     \
  od.Cache         \
  od.V2support     \
  od.memory        \
  od.fh3

HDRS = \
  mount.h         \
  nfs_prot.h      \
  pcnfsd.h        \
  rquota.h
  
#
# Rule patterns
#
.SUFFIXES:  .o .od .s .c .h .x .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} ${CMHGFLAGS} -o $@ $< h.$*
.s.o:;      ${AS} ${AFLAGS} $< $@
.c.od:;     ${CC} -DDEBUGLIB ${CFLAGS} -o $@ $<


#
# build a NFS module
#
all: ${TARGET}
        @echo ${COMPONENT}: all complete

install: ${TARGET} dirs
        ${CP} ${TARGET} ${INSTDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: install complete

dirs:
	${MKDIR} ${INSTDIR}

trace: ${DTARGET} ${DIRS}
        @echo ${COMPONENT}: (TRACE) all complete

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
        ${WIPE} od ${WFLAGS}
        ${RM} ${TARGET}
        ${RM} ${XTARGET}
        ${RM} ${DTARGET}
        ${RM} ${ROMTARGET}
        ${RM} linked.${COMPONENT}
        ${RM} h.${CMHGNAME}
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${XTARGET} ${OBJS} ${HDRS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ASMLIB}
        ${LD} -o $@ -rmf ${OBJS} ${RPCLIB} ${DEBUGLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB} ${ASMLIB}
        $(MODSQZ) $(TARGET)
        Access $@ RW/R


${DTARGET}: ${DEBOBJS} ${HDRS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ASMLIB}
        ${LD} -o $@ -rmf ${DEBOBJS} ${RPCLIB} ${DEBUGLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${CLIB} ${ASMLIB}
        |sortsymtab syms linked.syms
        Access $@ RW/R


${DIRS}:
	${MKDIR} od
	${TOUCH} $@

#
# ROM Target 
#
${ROMTARGET}: ${OBJS} ${HDRS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ASMLIB}
        ${LD} -o $@ -aof ${OBJS} ${RPCLIB} ${UNIXLIB} ${INETLIB} ${SOCKLIB} ${ROMCSTUBS} ${ASMLIB}

#
# Final link for the ROM Image (using given base address)
#
rom_link: ${ROMTARGET}
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

# NOTE: nfs_prot_xdr.c defines a recursive xdr routine for the entry types.  If used
# with a sufficiently large directory, this causes the SVC stack to overflow.  There
# are iterative versions in xdr_manual.c which replace these routines.  We use sed
# to alter the name of the functions in the code that rpcgen generates to avoid
# duplicate symbol definitions.

c.mount_clnt   :  x.mount ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -l -o $@ mount.x
c.mount_xdr    :  x.mount ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -c -o $@ mount.x
c.nfs_prot_clnt:  x.nfs_prot ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -l -o $@ nfs_prot.x
c.nfs_prot_xdr :  x.nfs_prot ${HDRS}
	${RPCGEN} ${RPCGENFLAGS} -c -o ${TEMPORARY} nfs_prot.x
	${SED} "s/^xdr_entry/obs_xdr_entry/" < ${TEMPORARY} > $@
	${RM} ${TEMPORARY}
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

o.xdr_manual: c.xdr_manual
	${CC} ${RPCCCFLAGS} -o $@ $*.c
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

od.xdr_manual: c.xdr_manual
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.rquota_xdr: c.rquota_xdr
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.rquota_clnt: c.rquota_clnt
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.pcnfsd_xdr: c.pcnfsd_xdr
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.pcnfsd_clnt: c.pcnfsd_clnt
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.mount_xdr: c.mount_xdr
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.mount_clnt: c.mount_clnt
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.nfs_prot_xdr: c.nfs_prot_xdr
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c
od.nfs_prot_clnt: c.nfs_prot_clnt
	${CC} -DDEBUGLIB ${RPCCCFLAGS} -o $@ $*.c


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
        @stamp $@

h.nfscmhg:	$(CMHGOBJ)
        @stamp $@

o.Commands:     h.nfscmhg
o.ModuleWrap:   h.nfscmhg
od.Commands:    h.nfscmhg
od.ModuleWrap:  h.nfscmhg

# Note, cannot replace -bin -base 0 with -rmf here otherwise you get all sorts
# of extra relocation code inserted into the final image.
${XTARGET}: o.${XCOMPONENT}
	${LD} -o $@ -bin -base 0 o.${XCOMPONENT}
	SetType $@ Module
	

o.V2support: c.V2support
        ${CC} ${RPCCCFLAGS} -o $@ $*.c



# Dynamic dependencies:
