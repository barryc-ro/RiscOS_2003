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


AFLAGS = -depend !Depend -Stamp -quit
CFLAGS  = -c -zM -wdp -ffcah -zps1 ${INCLUDES} -depend !Depend -Darm -DDEBUGON=$(DBUG) -DPACKETSIZE_COMMAND $(STB)
CPCCFLAGS=-c -zM -pcc -ffa -zps1 ${INCLUDES} -DDEBUGON=$(DBUG) -DPACKETSIZE_COMMAND $(STB)


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

#
# Include files
#
INCLUDES = -IC:,INetInc:

#
# Debug flag
#
DBUG = 0
#
# Uncomment the following lines if we are building NFSSTB
# rather than vanilla RISC OS NFS
#
STB       = -DNETACCESSCOUNT
CMHGOBJ   = o.nfsstbcmhg
COMPONENT = NFSSTB
#
# and uncomment the following lines if we are building RISC OS NFS
#CMHGOBJ   = o.nfscmhg
#COMPONENT = NFS
#
#
# Program specific options:
#
TARGET    = rm.NFS
ROMTARGET = aof.NFS
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
  o.rquota	  \
  ^.libs.rpc.o.rpc_datazm   \
  ^.libs.rpc.rpclibzm       \
  ^.libs.unixlib.unixlibzm  \
  ^.libs.inetlib.inetlibzm  \
  ^.libs.socklib.socklibzm

#
# Rule patterns
#
#  .SUFFIXES:  .o .s .c .h .cmhg
.c.o:;      ${CC} ${CFLAGS} -o $@ $<
.cmhg.o:;   ${CMHG} -o $@ $< h.$*
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
        ${RM} ${ROMTARGET}
        ${RM} map.NFSSTB
	${CD} ^.libs.inetlib
	amu_machine clean
	${CD} ^.rpc
	amu_machine clean
	${CD} ^.socklib
	amu_machine clean
	${CD} ^.unixlib
	amu_machine clean
	${CD} ^.^.NFS
        @echo ${COMPONENT}: cleaned

#
# Target 
#
${TARGET}: ${OBJS}
        ${LD} -o $@ -rmf ${OBJS} ${CLIB}
        $(MODSQZ) $(TARGET)

#
# ROM Target 
#
${ROMTARGET}: ${OBJS}
        ${LD} -o $@ -aof ${OBJS} ${ROMCSTUBS}
        
#
# Final link for the ROM Image (using given base address)
#
rom_link:
        ${MKDIR} linked
        ${MKDIR} map
        ${LD} -o linked.${COMPONENT} -map -bin -base ${ADDRESS} ${ROMTARGET} ${ABSSYM} > map.${COMPONENT}
        truncate map.${COMPONENT} linked.${COMPONENT}
        ${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
        @echo ${COMPONENT}: rom_link complete
#
# Some rules that need explicitly declaring
#

o.mount_clnt :  c.mount_clnt
                $(CC) ${CPCCFLAGS} c.mount_clnt

o.mount_xdr :   c.mount_xdr
                $(CC) ${CPCCFLAGS} c.mount_xdr

h.nfscmhg:	$(CMHGOBJ)

# The following dependancies should make the files required from other build frameworks

^.libs.inetlib.inetlibzm  : 
	${CD} ^.libs.inetlib
	amu_machine inetlibzm
	back
	
^.libs.rpc.o.rpc_datazm: ^.libs.rpc.rpclibzm
	@echo

^.libs.rpc.rpclibzm   : 
	${CD} ^.libs.rpc
	amu_machine rpclibzm
	back
^.libs.socklib.socklibzm  :
	${CD} ^.libs.socklib
	amu_machine socklibzm
	back
^.libs.unixlib.unixlibzm  :
	${CD} ^.libs.unixlib
	amu_machine unixlibzm
	back


# Dynamic dependencies:
