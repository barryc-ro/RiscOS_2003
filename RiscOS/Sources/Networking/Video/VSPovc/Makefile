# Makefile for VSPovc (Video Server Protocol / Oracle Video Client) module
# ========================================================================
#

# -----------------------------------------------------------------------------
# Change List
#

# Date		Name	Description
# ====		====	===========
# 1999-04-16	ADH	Created; distant relative of Browse Makefile.
#			Try not to break the comment syntax. It's used
#			for folding in capable editors.
# 1999-04-21	ADH	Got basic module build working, so started on
#			automatically generating the associated local
#			libraries too. IDL to C code generation is now
#			centralised as a result.
# 1999-04-22	ADH	Added capability to build IDL compiler, mnidlc.
#			Requires some libraries to be built twice, with
#			and without '-zM' on the CC command line.
# 1999-04-26	ADH	Added capability to build Messages file
#			compiler, msgc, and build relevant .mot files
#			with it for itself and the IDL compiler.


# -----------------------------------------------------------------------------
# Program specific options / locations
#

IDLTEMP		= FromIDL.Raw
IDLOUT		= FromIDL

RTSPUBDIR	= OMN.rts.pub
MXPUBDIR	= OMN.mx.pub
MXSRCDIR	= OMN.mx.src
MXMSGDIR	= OMN.mx.mesg

ROMXDIRPREFIX	= OMN-RISCOS.mx.src
ROMXDIRSUFFIX	= sosd.riscos

MXINCPATH	= ${MXSRCDIR}.inc
YOLIBPATH	= ${MXSRCDIR}.yo
YRLIBPATH	= ${MXSRCDIR}.yr
YSLIBPATH	= ${MXSRCDIR}.ys ${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}
MNLIBPATH	= ${MXSRCDIR}.mn ${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}
MTLIBPATH	= ${MXSRCDIR}.mt ${ROMXDIRPREFIX}.mt.${ROMXDIRSUFFIX}

VPATH		= @ OMN-Others.rts.src.vid FromIDL
COMPONENT	= VSPovc
ROM_MODULE	= aof.VSPovc
MODULE		= VSPovc
LDIR		= Resources.${LOCALE}.${SYSTEM}


# -----------------------------------------------------------------------------
# Include files (libraries and resources)
#

INCLUDES	= -IC:,mx_pri:,mx_pub:,mx_inc:,rts:,vs_inc:,TCPIPlibs:
IDLINCLUDES	= -I <OMNBuild$Dir>.mx.pub.idl.
MSGCINCLUDES	= -I <OMNBuild$Dir>.mx.mesg.ys

FILES = \
 ${TARGET}


# -----------------------------------------------------------------------------
# Include files (objects)
#

OBJS = \
	oz.debug \
	oz.module \
	oz.mzscl \
	oz.mzsclii \
	oz.mzswrap \
	oz.umdn \
	oz.vspc \
	oz.VSPHdr


# -----------------------------------------------------------------------------
# Generic options
#

MKDIR		= cdir
AS		= objasm
CC		= cc
CMHG		= cmhg
IDL		= mnidlc
MSGC		= msgc
CP		= copy
LD		= link
LIB		= libfile
RM		= remove
SQUEEZE		= squeeze
WIPE		= -wipe

# CFLAGS doesn't include '-zM' as this is in the '.c.oz' rule later.

AFLAGS		= ${THROWBACK} -depend !Depend -nocache -stamp -quit
CFLAGS		= ${THROWBACK} -c -depend !Depend -fna -wp ${INCLUDES}
CMHGFLAGS	= -p
IDLFLAGS	= -a coa -S ${IDLINCLUDES} -R "ys.log.msg-path=${MXMSGDIR}.yc"
MSGCFLAGS	= ${MSGCINCLUDES}
LIBFLAGS	= -c -o
CPFLAGS		= ~cfr~v
SQFLAGS		= -f
WFLAGS		= ~cf~vr


# -----------------------------------------------------------------------------
# Build options
#

# MODULE_DEBUG           enable debugging when defined, else all is disabled
#                        (can't just use "DEBUG" as this clashes with some stuff
#                        in MediaNet which won't compile if this is defined!)
# EOS                    sets series of system variables detailing what caused
#                        video to stop (OVCdebug$*)
# SOS                    same as EOS but for start of stream
# USEOLDREAD_POS         forces old read_pos() routine to be used
# USE_MN_CALLBACKS       forces module to define a MediaNet callback routine
# SILENT_NETWORK_ERRORS  stops serious network errors from being reported on
#                        screen

BASEFLAGS	= -DRISCOS -DCOMPAT_INET4
DAPPFLAGS	= ${BASEFLAGS} -DSILENT_NETWORK_ERRORS
DMODFLAGS	= ${BASEFLAGS} -DSYSTEM -UMODULE_DEBUG -DSILENT_NETWORK_ERRORS


# -----------------------------------------------------------------------------
# Libraries
#

CLIB		= CLib:o.stubs
ROMCSTUBS	= RISC_OSLib:o.romcstubs
ABSSYM		= RISC_OSLib:o.c_abssym
INETLIB		= TCPIPLibs:o.inetlib
SOCKLIB		= TCPIPLibs:o.socklib
DEBUGLIB	= <Lib$Dir>.Debuglib.o.debuglib

IDLLIB		= @.o.idllib
YOLIB		= @.o.yolib
MNLIB		= @.o.mnlib
MTLIB		= @.o.mtlib
YSLIB		= @.o.yslib
YRLIB		= @.o.yrlib

# For the IDL compiler:

LIBS = \
	${YRLIB} \
	${YSLIB} \
	${DEBUGLIB} \
	${CLIB}

# For the target module:

LIBSZM = \
	${MNLIB}zm \
	${MTLIB}zm \
	${YRLIB}zm \
	${YOLIB}zm \
	${YSLIB}zm \
	${IDLLIB}zm \
	${CLIB} \
	${DEBUGLIB}zm \
	${INETLIB}zm \
	${SOCKLIB}zm


# -----------------------------------------------------------------------------
# IDL compilation
#

# A list of all dependencies between required source files and their
# generator IDL files, including how to build the source. Currently if
# we have all the header files, we'll also have every source file that's
# needed too; so only list the headers.

IDLS = \
	h.mkd \
	h.mzacom \
	h.mzalgctn \
	h.mzc \
	h.mzcch \
	h.mzs \
	h.mzscli \
	h.mzz \
	h.ydyoidl \
	h.yoevt \
	h.yoidl \
	h.yostd \
	h.yotrans

c.mkdC				h.mkd:		mnidlc ${RTSPUBDIR}.idl.mkd
						IDLToC		mkd		${RTSPUBDIR}.idl
						IDLExport	mkd
c.mzacomC			h.mzacom:	mnidlc ${RTSPUBDIR}.idl.mzacom
						IDLToC		mzacom		${RTSPUBDIR}.idl
						IDLExport	mzacom
c.mzalgctnC			h.mzalgctn:	mnidlc ${RTSPUBDIR}.idl.mzalgctn
						IDLToC		mzalgctn	${RTSPUBDIR}.idl
						IDLExport	mzalgctn
c.mzcC				h.mzc:		mnidlc ${RTSPUBDIR}.idl.mzc
						IDLToC		mzc		${RTSPUBDIR}.idl
						IDLExport	mzc
c.mzcchC			h.mzcch:	mnidlc ${RTSPUBDIR}.idl.mzcch
						IDLToC		mzcch		${RTSPUBDIR}.idl
						IDLExport	mzcch
c.mzsC				h.mzs:		mnidlc ${RTSPUBDIR}.idl.mzs
						IDLToC		mzs		${RTSPUBDIR}.idl
						IDLExport	mzs
c.mzscliC	c.mzscliS	h.mzscli:	mnidlc ${RTSPUBDIR}.idl.mzscli
						IDLToC		mzscli		${RTSPUBDIR}.idl
						IDLExport	mzscli
c.mzzC				h.mzz:		mnidlc ${RTSPUBDIR}.idl.mzz
						IDLToC		mzz		${RTSPUBDIR}.idl
						IDLExport	mzz

c.yostdC			h.yostd:	mnidlc ${MXPUBDIR}.idl.yostd
						IDLToC		yostd		${MXPUBDIR}.idl
						IDLExport	yostd

c.ydyoidlC			h.ydyoidl:	mnidlc ${MXINCPATH}.idl.ydyoidl
						IDLToC		ydyoidl		${MXINCPATH}.idl
						IDLExport	ydyoidl
c.yoidlC	c.yoidlS	h.yoidl:	mnidlc ${MXINCPATH}.idl.yoidl
						IDLToC		yoidl		${MXINCPATH}.idl
						IDLExport	yoidl
c.yotransC			h.yotrans:	mnidlc ${MXINCPATH}.idl.yotrans
						IDLToC		yotrans		${MXINCPATH}.idl
						IDLExport	yotrans

c.yoevtC	c.yoevtS	h.yoevt:	mnidlc ${YOLIBPATH}.idl.yoevt
						IDLToC		yoevt		${YOLIBPATH}.idl
						IDLExport	yoevt


# -----------------------------------------------------------------------------
# Rule patterns
#

# The first line of the two clears existing definitions, as otherwise
# we'd just add to the presets

.SUFFIXES:
.SUFFIXES: .o .oz .s .c .h .cmhg .mot .msg

.c.o:;		@echo
		@echo Compiling $<
		@echo ${DAPPFLAGS}
		@${CC} ${CFLAGS} ${DAPPFLAGS} -o $@ $<

.c.oz:;		@echo
		@echo Compiling $<
		@echo ${DMODFLAGS}
		@${CC} -zM ${CFLAGS} ${DMODFLAGS} -o $@ $<

.s.o .s.oz:;	@echo
		@echo Assembling $<
		@echo ${AFLAGS}
		@${AS} ${AFLAGS} $< $@

.cmhg.oz:;	@echo
		@echo CMHG generating $@
		@echo ${CMHGFLAGS}
		@${CMHG} ${CMHGFLAGS} -o $@ $<

.cmhg.h:;	@echo
		@echo CMHG generating $@
		@echo ${CMHGFLAGS}
		@${CMHG} ${CMHGFLAGS} -d $@ $<

.msg.mot:;	@echo
		@echo Compiling Messages file $<
		@IfThere $@/mot Then Delete $@/mot
		${MSGC} ${MSGCFLAGS} -o $@ $<
		@${CP} $@/mot $@  ${CPFLAGS}~nd
		@${CP} $@ $*/mot  ${CPFLAGS}~n


# -----------------------------------------------------------------------------
# Main rules
#

all: ${FILES}
	@echo ${COMPONENT}: Module built (RAM)

rom: ${ROM_MODULE}
	@echo ${COMPONENT}: Module built (ROM)

install_common: ${FILES}
	@echo
	${CP} ${TARGET}		${INSTDIR}.${COMPONENT}		${CPFLAGS}~n
	Access ${INSTDIR}.${COMPONENT} wr/r

install: install_common
	@echo
	@echo ${COMPONENT}: Module installed (RAM)
	@echo

install_rom: ${ROM_MODULE}
	${CP} ${ROM_MODULE}	${INSTDIR}.${COMPONENT}		${CPFLAGS}
	@echo
	@echo ${COMPONENT}: Module installed (ROM)
	@echo

# Mmm, fun... :-)

o.dirs_dirs:
	@echo
	@echo Creating build directories
	@${MKDIR} o
	@${MKDIR} oz
	@${MKDIR} aof
	@${MKDIR} linked
	@${MKDIR} map
	@${MKDIR} rm
	@${MKDIR} syms
	@${MKDIR} FromIDL
	@${MKDIR} FromIDL.c
	@${MKDIR} FromIDL.h
	@${MKDIR} FromIDL.o
	@${MKDIR} FromIDL.oz
	@${MKDIR} FromIDL.Raw
	@${MKDIR} ${MXSRCDIR}.yc.o
	@${MKDIR} ${MXSRCDIR}.yo.o
	@${MKDIR} ${MXSRCDIR}.yo.oz
	@${MKDIR} ${MXSRCDIR}.mn.o
	@${MKDIR} ${MXSRCDIR}.mn.oz
	@${MKDIR} ${MXSRCDIR}.mt.o
	@${MKDIR} ${MXSRCDIR}.mt.oz
	@${MKDIR} ${MXSRCDIR}.ys.o
	@${MKDIR} ${MXSRCDIR}.ys.oz
	@${MKDIR} ${MXSRCDIR}.yr.o
	@${MKDIR} ${MXSRCDIR}.yr.oz
	@${MKDIR} ${MXMSGDIR}.ys.mot
	@${MKDIR} ${MXMSGDIR}.yc.mot
	@${MKDIR} ${ROMXDIRPREFIX}.yc.o
	@${MKDIR} ${ROMXDIRPREFIX}.yc.oz
	@${MKDIR} ${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}.o
	@${MKDIR} ${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}.oz
	@${MKDIR} ${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.o
	@${MKDIR} ${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.oz
	create o.dirs_dirs
	stamp  o.dirs_dirs


# -----------------------------------------------------------------------------
# Development rules
#

clean:
	@echo
	${WIPE}	o					${WFLAGS}
	${WIPE}	oz					${WFLAGS}
	${WIPE}	aof					${WFLAGS}
	${WIPE}	linked					${WFLAGS}
	${WIPE}	map					${WFLAGS}
	@echo
	@echo ${COMPONENT}: Partially cleaned

clean_all: clean
	@echo
	${WIPE}	msgc					${WFLAGS}
	${WIPE}	mnidlc					${WFLAGS}
	${WIPE}	rm					${WFLAGS}
	${WIPE}	syms					${WFLAGS}
	${WIPE}	FromIDL					${WFLAGS}
	${WIPE}	${MXSRCDIR}.yc.o			${WFLAGS}
	${WIPE}	${MXSRCDIR}.yo.o			${WFLAGS}
	${WIPE}	${MXSRCDIR}.yo.oz			${WFLAGS}
	${WIPE}	${MXSRCDIR}.mn.o			${WFLAGS}
	${WIPE}	${MXSRCDIR}.mn.oz			${WFLAGS}
	${WIPE}	${MXSRCDIR}.mt.o			${WFLAGS}
	${WIPE}	${MXSRCDIR}.mt.oz			${WFLAGS}
	${WIPE}	${MXSRCDIR}.ys.o			${WFLAGS}
	${WIPE}	${MXSRCDIR}.ys.oz			${WFLAGS}
	${WIPE}	${MXSRCDIR}.yr.o			${WFLAGS}
	${WIPE}	${MXSRCDIR}.yr.oz			${WFLAGS}
	${WIPE}	${MXMSGDIR}.ys.*/mot			${WFLAGS}
	${WIPE}	${MXMSGDIR}.ys.mot			${WFLAGS}
	${WIPE}	${MXMSGDIR}.yc.*/mot			${WFLAGS}
	${WIPE}	${MXMSGDIR}.yc.mot			${WFLAGS}
	${WIPE}	${ROMXDIRPREFIX}.yc.o			${WFLAGS}
	${WIPE}	${ROMXDIRPREFIX}.yc.oz			${WFLAGS}
	${WIPE}	${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}.o	${WFLAGS}
	${WIPE}	${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}.oz	${WFLAGS}
	${WIPE}	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.o	${WFLAGS}
	${WIPE}	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.oz	${WFLAGS}
	${WIPE}	${COMPONENT}				${WFLAGS}
	@echo
	@echo ${COMPONENT}: Cleaned all


# -----------------------------------------------------------------------------
# Final link for ROM Image (using given base address)
#

rom_link:
	${LD} -o linked.${COMPONENT} -bin -base ${ADDRESS} \
		${ROM_MODULE} ${ABSSYM} -map > map.${COMPONENT}
	truncate map.${COMPONENT} linked.${COMPONENT}
	${CP} linked.${COMPONENT} ${LINKDIR}.${COMPONENT} ${CPFLAGS}
	@echo ${COMPONENT}: rom_link complete


# -----------------------------------------------------------------------------
# Specialised static dependencies
#

# First, a rule to invoke CMHG to generate the module header file.

oz.module: vsphdr.h

# For some specific source files there are headers which must be
# generated from IDL. These rules say how to do it.

MZSWRAPIDLLIST = \
	h.mzz \
	h.mzs \
	h.mzscli \
	h.mzalgctn \
	h.mkd \
	h.mzcch \
	h.mzc

oz.mzswrap: ${MZSWRAPIDLLIST}

# -----------------------------------------------------------------------------
# Building the Messages compiler, msgc
#

# s0mnmsgc is tricky; it is needed to build various binaries and you need
# a different CC command line each time. Need to specify exactly how to
# build this, then, for each case where it is needed.

OMN.mx.src.ys.o.s0ysmain:;	@echo
				@echo Compiling s0ysmain for ysmsgc
				@echo ${DAPPFLAGS} -DENTRY_POINT=ysmsgc
				@${CC} ${CFLAGS} ${DAPPFLAGS} -DENTRY_POINT=ysmsgc \
				-o ${MXSRCDIR}.ys.o.s0ysmain \
				${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.c.s0ysmain

MSGCOBJS = \
	${MXSRCDIR}.ys.o.s0ysmain \
	${MXSRCDIR}.ys.o.ysmsgc

MSGCALL = ${LIBS} ${MSGCOBJS}

# Don't ever write a dependency on msgc; always use ${MSGC} instead.

msgc: ${MSGCALL}
	@echo
	@echo Linking Messages file compiler:
	${LD} -s syms.msgc -o $@ ${MSGCALL}
	@echo
	@echo The Messages file compiler will complain twice when asked to
	@echo build its own Messages for the first time. Despite the warnings
	@echo it gives, it should still complete its task and should not show
	@echo similar warnings for subsequent Messages file compilations.

MSGC = \
	${MXMSGDIR}.ys.mot.OMNMSGC \
	msgc


# -----------------------------------------------------------------------------
# Building the IDL compiler, mnidlc
#

# The compiler front end. Parsing and semantic analysis

LIBYCFEOBJS = \
	${MXSRCDIR}.yc.o.ycosyn \
	${MXSRCDIR}.yc.o.ycosax \
	${MXSRCDIR}.yc.o.ycpp \
	${MXSRCDIR}.yc.o.ycppdir \
	${MXSRCDIR}.yc.o.ycppexp \
	${MXSRCDIR}.yc.o.ycppget \
	${MXSRCDIR}.yc.o.ycppif \
	${MXSRCDIR}.yc.o.ycrec \
	${MXSRCDIR}.yc.o.ycprs \
	${MXSRCDIR}.yc.o.ycsem \
	${MXSRCDIR}.yc.o.ycsemty \
	${MXSRCDIR}.yc.o.ycsemco \
	${MXSRCDIR}.yc.o.ycsemif \
	${MXSRCDIR}.yc.o.ycifr \
	${MXSRCDIR}.yc.o.pxslax \
	${MXSRCDIR}.yc.o.ycprfree \
	${MXSRCDIR}.yc.o.ycsemprg \
	${MXSRCDIR}.yc.o.ycutil \
	${MXSRCDIR}.yc.o.ycsym

# The C language mapping code generator.

LIBYCCOBJS = \
	${MXSRCDIR}.yc.o.yccmap \
	${ROMXDIRPREFIX}.yc.o.yccsrv \
	${MXSRCDIR}.yc.o.yccclnt \
	${MXSRCDIR}.yc.o.ycctmpl \
	${MXSRCDIR}.yc.o.yccname \
	${MXSRCDIR}.yc.o.ycchdr \
	${MXSRCDIR}.yc.o.yccfp

# C++ language mappings, needed as the C language generator has to avoid C++
# keywords and once you pull in one of the C++ files, you end up having to
# use them all. yccpclnt has an error which our compiler faults completely
# (and IMHO correctly, but presumably Oracle's doesn't!), so there's a
# RISC OS specific variant for this one file.

LIBYCCPOBJS = \
	${MXSRCDIR}.yc.o.yccpmap \
	${MXSRCDIR}.yc.o.yccphdr \
	${MXSRCDIR}.yc.o.yccpname \
	${MXSRCDIR}.yc.o.yccptvar \
	${ROMXDIRPREFIX}.yc.o.yccpclnt

# The ITL language mapping code generator. Again, more problems with static
# function declarations where no related function body exists, so a RISC OS
# specific variant exists.

LIBYCIOBJS = \
	${ROMXDIRPREFIX}.yc.o.ycimap \
	${MXSRCDIR}.yc.o.ycihdr \
	${MXSRCDIR}.yc.o.yciclnt

# s0mnmsgc is tricky; it is needed to build various binaries and you need
# a different CC command line each time. Need to specify exactly how to
# build this, then, for each case where it is needed.

OMN.mx.src.yc.o.s0ysmain:;	@echo
				@echo Compiling s0ysmain for ycMain
				@echo ${DAPPFLAGS} -DENTRY_POINT=ycMain
				@${CC} ${CFLAGS} ${DAPPFLAGS} -DENTRY_POINT=ycMain \
				-o ${MXSRCDIR}.yc.o.s0ysmain \
				${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.c.s0ysmain

# Other miscellaneous files needed for a minimal build. You need bits of
# yolib, but can't use it all, as a full build requires an IDL compiler...

LIBYCMOBJS = \
	${MXSRCDIR}.yc.o.s0ysmain \
	${MXSRCDIR}.yc.o.ycmain \
	${MXSRCDIR}.yo.o.yotk \
	${MXSRCDIR}.yo.o.yotkg \
	${MXSRCDIR}.yo.o.yosx

# Cumulative requirements and the main build rule.

LIBYCALL = ${LIBS} ${LIBYCMOBJS} ${LIBYCIOBJS} ${LIBYCCPOBJS} ${LIBYCCOBJS} ${LIBYCFEOBJS}

# We need msgc, the messages file compiler and its own messages
# to generate the .mot file holding text resources for the IDL
# compiler.

mnidlc: ${LIBYCALL} ${MXMSGDIR}.yc.mot.OMNIDLC ${MSGC}
	@echo
	@echo Linking IDL compiler:
	${LD} -s syms.mnidlc -o $@ ${LIBYCALL}
	@echo
	@echo Setting up IDL complier macro commands:
	@echo
	*Set Alias$IDLToC         ${IDL} ${IDLFLAGS} -o ${IDLTEMP} %1.%0
	*Set Alias$IDLExportCStub ${CP} ${IDLTEMP}.%0C/c ${IDLOUT}.c.%0C ${CPFLAGS}
	*Set Alias$IDLExportSStub ${CP} ${IDLTEMP}.%0S/c ${IDLOUT}.c.%0S ${CPFLAGS}
	*Set Alias$IDLExportHdr   ${CP} ${IDLTEMP}.%0/h ${IDLOUT}.h.%0 ${CPFLAGS}
	*Set Alias$IDLExport      IDLExportHdr %0|MIDLExportCStub %0|MIDLExportSStub %0
	@echo


# -----------------------------------------------------------------------------
# Support library construction: idllib
#

IDLLIBOBJSZ = \
	FromIDL.oz.mkdC \
	FromIDL.oz.mzacomC \
	FromIDL.oz.mzalgctnC \
	FromIDL.oz.mzcC \
	FromIDL.oz.mzcchC \
	FromIDL.oz.mzsC \
	FromIDL.oz.mzscliC \
	FromIDL.oz.mzscliS \
	FromIDL.oz.mzzC \
	FromIDL.oz.ydyoidlC \
	FromIDL.oz.yoevtC \
	FromIDL.oz.yoevtS \
	FromIDL.oz.yoidlC \
	FromIDL.oz.yoidlS \
	FromIDL.oz.yostdC \
	FromIDL.oz.yotransC

o.idllibzm: ${IDLLIBOBJSZ}
	${LIB} ${LIBFLAGS} o.idllibzm ${IDLLIBOBJSZ}
	@echo
	@echo IDL library built for modules


# -----------------------------------------------------------------------------
# Support library construction: yolib
#

YOLIBOBJS = \
	${MXSRCDIR}.yo.o.yo \
	${MXSRCDIR}.yo.o.yoboa \
	${MXSRCDIR}.yo.o.yocoa \
	${MXSRCDIR}.yo.o.yocy \
	${MXSRCDIR}.yo.o.yoevt \
	${MXSRCDIR}.yo.o.yogiop \
	${MXSRCDIR}.yo.o.yoorb \
	${MXSRCDIR}.yo.o.yorc5x \
	${MXSRCDIR}.yo.o.yort \
	${MXSRCDIR}.yo.o.yosx \
	${MXSRCDIR}.yo.o.yot \
	${MXSRCDIR}.yo.o.yotk \
	${MXSRCDIR}.yo.o.yotkg \
	${MXSRCDIR}.yo.o.yotkx \
	${MXSRCDIR}.yo.o.yox \
	${MXSRCDIR}.yo.o.yoyd

YOLIBOBJSZ = \
	${MXSRCDIR}.yo.oz.yo \
	${MXSRCDIR}.yo.oz.yoboa \
	${MXSRCDIR}.yo.oz.yocoa \
	${MXSRCDIR}.yo.oz.yocy \
	${MXSRCDIR}.yo.oz.yoevt \
	${MXSRCDIR}.yo.oz.yogiop \
	${MXSRCDIR}.yo.oz.yoorb \
	${MXSRCDIR}.yo.oz.yorc5x \
	${MXSRCDIR}.yo.oz.yort \
	${MXSRCDIR}.yo.oz.yosx \
	${MXSRCDIR}.yo.oz.yot \
	${MXSRCDIR}.yo.oz.yotk \
	${MXSRCDIR}.yo.oz.yotkg \
	${MXSRCDIR}.yo.oz.yotkx \
	${MXSRCDIR}.yo.oz.yox \
	${MXSRCDIR}.yo.oz.yoyd

o.yolib: ${YOLIBOBJS}
	@echo
	${LIB} ${LIBFLAGS} o.yolib ${YOLIBOBJS}
	@echo
	@echo YO library built for applications

o.yolibzm: ${YOLIBOBJSZ}
	@echo
	${LIB} ${LIBFLAGS} o.yolibzm ${YOLIBOBJSZ}
	@echo
	@echo YO library built for modules


# -----------------------------------------------------------------------------
# Support library construction: yslib
#

YSLIBOBJS = \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.o.sysfp \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.o.sysi \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.o.sysrld \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.o.systhr \
	${MXSRCDIR}.ys.o.ys \
	${MXSRCDIR}.ys.o.ysb8 \
	${MXSRCDIR}.ys.o.ysbv \
	${MXSRCDIR}.ys.o.ysc \
	${MXSRCDIR}.ys.o.yscksm \
	${MXSRCDIR}.ys.o.yse \
	${MXSRCDIR}.ys.o.ysfe \
	${MXSRCDIR}.ys.o.ysfmt \
	${MXSRCDIR}.ys.o.ysfo \
	${MXSRCDIR}.ys.o.yshsh \
	${MXSRCDIR}.ys.o.ysid \
	${MXSRCDIR}.ys.o.ysl \
	${MXSRCDIR}.ys.o.yslog \
	${MXSRCDIR}.ys.o.yslst \
	${MXSRCDIR}.ys.o.ysm \
	${MXSRCDIR}.ys.o.ysmdmp \
	${MXSRCDIR}.ys.o.ysmsg \
	${MXSRCDIR}.ys.o.ysr \
	${MXSRCDIR}.ys.o.ysrand \
	${MXSRCDIR}.ys.o.ysrarg \
	${MXSRCDIR}.ys.o.ysrgx \
	${MXSRCDIR}.ys.o.ysrprs \
	${MXSRCDIR}.ys.o.yssp \
	${MXSRCDIR}.ys.o.ysstr \
	${MXSRCDIR}.ys.o.ysthr \
	${MXSRCDIR}.ys.o.ystm \
	${MXSRCDIR}.ys.o.ysv \
	${MXSRCDIR}.ys.o.ysver \
	${MXSRCDIR}.ys.o.ysx \
	o.umdn

YSLIBOBJSZ = \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.oz.sysfp \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.oz.sysi \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.oz.sysrld \
	${ROMXDIRPREFIX}.ys.${ROMXDIRSUFFIX}.oz.systhr \
	${MXSRCDIR}.ys.oz.ys \
	${MXSRCDIR}.ys.oz.ysb8 \
	${MXSRCDIR}.ys.oz.ysbv \
	${MXSRCDIR}.ys.oz.ysc \
	${MXSRCDIR}.ys.oz.yscksm \
	${MXSRCDIR}.ys.oz.yse \
	${MXSRCDIR}.ys.oz.ysfe \
	${MXSRCDIR}.ys.oz.ysfmt \
	${MXSRCDIR}.ys.oz.ysfo \
	${MXSRCDIR}.ys.oz.yshsh \
	${MXSRCDIR}.ys.oz.ysid \
	${MXSRCDIR}.ys.oz.ysl \
	${MXSRCDIR}.ys.oz.yslog \
	${MXSRCDIR}.ys.oz.yslst \
	${MXSRCDIR}.ys.oz.ysm \
	${MXSRCDIR}.ys.oz.ysmdmp \
	${MXSRCDIR}.ys.oz.ysmsg \
	${MXSRCDIR}.ys.oz.ysr \
	${MXSRCDIR}.ys.oz.ysrand \
	${MXSRCDIR}.ys.oz.ysrarg \
	${MXSRCDIR}.ys.oz.ysrgx \
	${MXSRCDIR}.ys.oz.ysrprs \
	${MXSRCDIR}.ys.oz.yssp \
	${MXSRCDIR}.ys.oz.ysstr \
	${MXSRCDIR}.ys.oz.ysthr \
	${MXSRCDIR}.ys.oz.ystm \
	${MXSRCDIR}.ys.oz.ysv \
	${MXSRCDIR}.ys.oz.ysver \
	${MXSRCDIR}.ys.oz.ysx \
	o.umdn

o.yslib: ${YSLIBOBJS}
	@echo
	${LIB} ${LIBFLAGS} o.yslib ${YSLIBOBJS}
	@echo
	@echo YS library built for applications

o.yslibzm: ${YSLIBOBJSZ}
	@echo
	${LIB} ${LIBFLAGS} o.yslibzm ${YSLIBOBJSZ}
	@echo
	@echo YS library built for modules


# -----------------------------------------------------------------------------
# Support library construction: yrlib
#

YRLIBOBJS = \
	${MXSRCDIR}.yr.o.yr \
	${MXSRCDIR}.yr.o.yrc \
	${MXSRCDIR}.yr.o.yrdst \
	${MXSRCDIR}.yr.o.yrdump \
	${MXSRCDIR}.yr.o.yrtk \
	${MXSRCDIR}.yr.o.yrwrite

YRLIBOBJSZ = \
	${MXSRCDIR}.yr.oz.yr \
	${MXSRCDIR}.yr.oz.yrc \
	${MXSRCDIR}.yr.oz.yrdst \
	${MXSRCDIR}.yr.oz.yrdump \
	${MXSRCDIR}.yr.oz.yrtk \
	${MXSRCDIR}.yr.oz.yrwrite

o.yrlib: ${YRLIBOBJS}
	@echo
	${LIB} ${LIBFLAGS} o.yrlib ${YRLIBOBJS}
	@echo
	@echo YR library built for applications

o.yrlibzm: ${YRLIBOBJSZ}
	@echo
	${LIB} ${LIBFLAGS} o.yrlibzm ${YRLIBOBJSZ}
	@echo
	@echo YR library built for modules


# -----------------------------------------------------------------------------
# Support library construction: mtlib
#

MTLIBOBJSZ = \
	${MXSRCDIR}.mt.oz.mtrgx \
	${MXSRCDIR}.mt.oz.mtsp \
	${MXSRCDIR}.mt.oz.mtl \
	${MXSRCDIR}.mt.oz.mtcctx

o.mtlibzm: ${MTLIBOBJSZ}
	@echo
	${LIB} ${LIBFLAGS} o.mtlibzm ${MTLIBOBJSZ}
	@echo
	@echo MT library built for modules


# -----------------------------------------------------------------------------
# Support library construction: mnlib
#

# Base objects

MNLIBGENOBJSZ = \
	${MXSRCDIR}.mn.oz.mn \
	${MXSRCDIR}.mn.oz.mna \
	${MXSRCDIR}.mn.oz.mne \
	${MXSRCDIR}.mn.oz.mnm \
	${MXSRCDIR}.mn.oz.mnn \
	${MXSRCDIR}.mn.oz.mno \
	${MXSRCDIR}.mn.oz.mnt \
	${MXSRCDIR}.mn.oz.mntr \
	${MXSRCDIR}.mn.oz.mnts

# RPC objects

MNLIBRPCOBJSZ = \
	${MXSRCDIR}.mn.oz.mnr \
	${MXSRCDIR}.mn.oz.mnrc \
	${MXSRCDIR}.mn.oz.mnrs \
	${MXSRCDIR}.mn.oz.mnx \
	${MXSRCDIR}.mn.oz.mznt \
	${MXSRCDIR}.mn.oz.mnb \
	${MXSRCDIR}.mn.oz.mzmgs

# Name server objects

MNLIBNSOBJSZ = \
	${MXSRCDIR}.mn.oz.mzn \
	${MXSRCDIR}.mn.oz.mznc

# Process server objects

MNLIBSCOBJSZ = \
	${MXSRCDIR}.mn.oz.mnsc \
	${MXSRCDIR}.mn.oz.mnscc

# Utility objects

MNLIBUTLOBJSZ = \
	${MXSRCDIR}.mn.oz.mnpsl

# OSD objects

MNLIBOSDOBJSZ = \
	${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}.oz.smnudp \
	${ROMXDIRPREFIX}.mn.${ROMXDIRSUFFIX}.oz.smni

MNLIBOBJSZ = \
	${MNLIBGENOBJSZ} \
	${MNLIBRPCOBJSZ} \
	${MNLIBNSOBJSZ} \
	${MNLIBSCOBJSZ} \
	${MNLIBUTLOBJSZ} \
	${MNLIBOSDOBJSZ}

o.mnlibzm: ${MNLIBOBJSZ}
	@echo
	${LIB} ${LIBFLAGS} o.mnlibzm ${MNLIBOBJSZ}
	@echo
	@echo MN library built for modules


# -----------------------------------------------------------------------------
# Top level rules
#

# These rely on AMU's reverse order building - various source files are
# created from IDL before the LIBS or OBJS are constructed. Limitations
# of AMU regarding the suffixes rules and generated objects make this
# necessary. In addition, automatic creation of temporary directories
# removed by the clean[_all] rule is done by 'o.dirs_dirs'.

${MODULE}: ${OBJS} ${LIBSZM} ${IDLS} o.dirs_dirs
	@echo
	${LD} -s syms.VSPovcRAM -rmf -o $@ ${OBJS} ${LIBSZM}

${ROM_MODULE}: ${OBJS} ${ROMCSTUBS} ${LIBSZM} ${IDLS} o.dirs_dirs
	@echo
	${LD} -s syms.VSPovcROM -o $@ -aof ${OBJS} ${LIBSZM} ${ROMCSTUBS}


# -----------------------------------------------------------------------------
# Dynamic dependencies:
