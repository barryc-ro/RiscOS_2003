# Makefile for Lucent Dragnet2 Controllerless modem

# ***********************************
# ***    C h a n g e   L i s t    ***
# ***********************************
# Date          Name         Description
# ----          ----         -----------
# 30-03-01      MSM          Original
# 05-04-01      MSM          Dragnet2 initial version

COMPONENT       = CtrlsModem
TARGET          = CtrlsModem

DIRS            = o._dirs
RAM_OBJS        = allglobs.o atcmd.o atcmd1.o atfax.o atmain.o atparse.o ats.o atsync.o \
		atv25ter.o atv9x.o atvoice.o cpcptini.o dp56clnt.o dp56kv.o dp56kv1.o dpcall.o \
		dpcmd1.o dpcntlv.o dpdl56v0.o dpdl56v1.o dpdl56v2.o dpdlv8bv.o dpdraco.o \
		dpdspinc.o dpdspinv.o dpfax.o dpfdsp.o dpiov.o dpmain.o dpsync.o dptest.o \
		dpv8bis.o dpv8bis1.o dpvoice.o dpvpcmd0.o dpvpcmd3.o dpvpcmd4.o dpvts.o \
		iocid.o iodata.o iodce.o iodte.o iodte1.o iofax.o ioiadpcm.o iomain.o \
		iosync.o iovoice.o lapm_bg.o lapm_ctl.o lapm_dc1.o lapm_dce.o lapm_dte.o \
		lapm_lbb.o lapm_lib.o llmain.o llutil.o llutil1v.o mnp_5a.o mnp_bg.o mnp_ctl.o \
		mnp_dce.o mnp_dte.o mnp_lib.o v42bis.o vp_slice.o vpcm.o vpcmcnst.o vpgetgn.o \
		riscos.o riscosutil.o riscosasm.o modulehdr.o
ROM_OBJS        = ${RAM_OBJS}
RAM_LIBS        = ${DEBUGLIB} ${NET5LIBS} ${CALLXLIB} ${ASMUTILS}
ROM_LIBS        = ${RAM_LIBS}

#CFLAGS          = -zM -zps1 -ffah
CFLAGS = -zM -zps1 -ff -Wbd
#CDEFINES = -DDEBUGLIB -DEMULATE
CDEFINES = -DDEBUGLIB -DPODULEBASED
#CDEFINES = -DDEBUGLIB

CINCLUDES = -I. -IOS: -IC:

include StdTools
MODSQZ = echo Not squeezing: 
include ModStdRule
include ModuleLibs
include RAMCModule
include ROMCModule


clean:
        ${WIPE} aof ${WFLAGS}
        ${WIPE} o ${WFLAGS}
        ${WIPE} rm ${WFLAGS}
        ${WIPE} linked ${WFLAGS}
        @${ECHO} ${COMPONENT}: cleaned

${DIRS}:
        ${MKDIR} o
        ${MKDIR} h
        ${TOUCH} $@

cmodule.o:      modhead.h

# Dynamic dependencies:
