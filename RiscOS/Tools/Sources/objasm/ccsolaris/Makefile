# This file was created by Megamake.
# Copyright (C) Advanced RISC Machines 1992.

COMPONENT=armasm

#CLXPATH=../../../../RiscOS/Sources/Lib/CLX/ccsolaris
CLX=${CLXPATH}/clx.a

CFLAGS=-c -O2 -I$(CLXPATH)  $(CFL) -DHOST_WANTS_NO_BANNER=1 -D__acorn
CC=gcc
DERIVED= datenow.h

OBJALL=aasm.o apply.o applysub.o asdsupp.o asm.o cli.o code.o condasm.o \
errors.o expr.o exprstor.o formatio.o fpio.o getdir.o getline.o globvars.o \
initdir.o listing.o literals.o locallab.o macrotyp.o mapping.o nametyp.o \
occur.o opcode.o p1dir.o p1line.o p2dir.o p2line.o stats.o store.o symdump.o \
tables.o tokens.o vars.o 

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

HSOURCE=aofvars.h apply.h applysub.h asd.h asdasm.h asdsupp.h asm.h cli.h \
code.h condasm.h constant.h errors.h expr.h exprstor.h exprtyp.h formatio.h \
fpio.h getdir.h getline.h globvars.h hostdep.h initdir.h listing.h \
literals.h locallab.h macrotyp.h message.h mapping.h nametyp.h occur.h opcode.h \
options.h p1dir.h p1line.h p2dir.h p2line.h stats.h store.h symdump.h \
tables.h tokens.h vars.h version.h 


noarg: ${COMPONENT}
	@echo "Made ${COMPONENT} (the default target)"

# Dependencies
aasm.o:	aasm.c
aasm.o:	asm.h
aasm.o:	cli.h
aasm.o:	$(EXTDIR1)/aof.h
aasm.o:	constant.h
aasm.o:	datenow.h
aasm.o:	errors.h
aasm.o:	formatio.h
aasm.o:	getline.h
aasm.o:	globvars.h
aasm.o:	hostdep.h
aasm.o:	initdir.h
aasm.o:	listing.h
aasm.o:	message.h
aasm.o:	nametyp.h
aasm.o:	options.h
aasm.o:	symdump.h
aasm.o:	tables.h
aasm.o:	vars.h
aasm.o:	version.h
	$(CC) $(CFLAGS) $(CFLaasm) aasm.c -o aasm.o
apply.o:	apply.c
apply.o:	apply.h
apply.o:	applysub.h
apply.o:	constant.h
apply.o:	errors.h
apply.o:	expr.h
apply.o:	exprstor.h
apply.o:	exprtyp.h
apply.o:	message.h
apply.o:	nametyp.h
apply.o:	tables.h
	$(CC) $(CFLAGS) $(CFLapply) apply.c -o apply.o
applysub.o:	applysub.c
applysub.o:	applysub.h
applysub.o:	constant.h
applysub.o:	errors.h
applysub.o:	exprstor.h
applysub.o:	exprtyp.h
applysub.o:	message.h
applysub.o:	nametyp.h
	$(CC) $(CFLAGS) $(CFLapplysub) applysub.c -o applysub.o
asdsupp.o:	aofvars.h
asdsupp.o:	asdsupp.c
asdsupp.o:	asdsupp.h
asdsupp.o:	$(EXTDIR1)/aof.h
asdsupp.o:	$(EXTDIR1)/aout.h
asdsupp.o:	$(EXTDIR1)/asdfmt.h
asdsupp.o:	$(EXTDIR1)/bytesex.h
asdsupp.o:	code.h
asdsupp.o:	constant.h
asdsupp.o:	globvars.h
asdsupp.o:	nametyp.h
asdsupp.o:	store.h
asdsupp.o:	tables.h
	$(CC) $(CFLAGS) $(CFLasdsupp) asdsupp.c -o asdsupp.o
asm.o:	aofvars.h
asm.o:	asdsupp.h
asm.o:	asm.c
asm.o:	asm.h
asm.o:	cli.h
asm.o:	$(EXTDIR1)/aof.h
asm.o:	$(EXTDIR1)/aout.h
asm.o:	$(EXTDIR1)/bytesex.h
asm.o:	$(EXTDIR1)/fname.h
asm.o:	code.h
asm.o:	condasm.h
asm.o:	constant.h
asm.o:	datenow.h
asm.o:	errors.h
asm.o:	formatio.h
asm.o:	getline.h
asm.o:	globvars.h
asm.o:	hostdep.h
asm.o:	initdir.h
asm.o:	listing.h
asm.o:	literals.h
asm.o:	locallab.h
asm.o:	macrotyp.h
asm.o:	message.h
asm.o:	nametyp.h
asm.o:	opcode.h
asm.o:	options.h
asm.o:	p1dir.h
asm.o:	p1line.h
asm.o:	p2line.h
asm.o:	stats.h
asm.o:	store.h
asm.o:	tables.h
	$(CC) $(CFLAGS) $(CFLasm) asm.c -o asm.o
cli.o:	aofvars.h
cli.o:	asm.h
cli.o:	cli.c
cli.o:	cli.h
cli.o:	$(EXTDIR1)/aof.h
cli.o:	$(EXTDIR1)/aout.h
cli.o:	$(EXTDIR1)/bytesex.h
cli.o:	$(EXTDIR1)/chunkfmt.h
cli.o:	$(EXTDIR1)/fname.h
cli.o:	$(EXTDIR1)/prgname.h
cli.o:	constant.h
cli.o:	datenow.h
cli.o:	errors.h
cli.o:	formatio.h
cli.o:	getline.h
cli.o:	globvars.h
cli.o:	hostdep.h
cli.o:	message.h
cli.o:	nametyp.h
cli.o:	options.h
cli.o:	store.h
cli.o:	version.h
	$(CC) $(CFLAGS) $(CFLcli) cli.c -o cli.o
code.o:	aofvars.h
code.o:	asm.h
code.o:	$(EXTDIR1)/aof.h
code.o:	$(EXTDIR1)/aout.h
code.o:	$(EXTDIR1)/bytesex.h
code.o:	$(EXTDIR1)/chunkfmt.h
code.o:	code.c
code.o:	code.h
code.o:	constant.h
code.o:	errors.h
code.o:	getline.h
code.o:	globvars.h
code.o:	listing.h
code.o:	message.h
code.o:	nametyp.h
code.o:	occur.h
code.o:	options.h
code.o:	store.h
code.o:	tables.h
code.o:	version.h
	$(CC) $(CFLAGS) $(CFLcode) code.c -o code.o
condasm.o:	condasm.c
condasm.o:	condasm.h
condasm.o:	constant.h
condasm.o:	errors.h
condasm.o:	getline.h
condasm.o:	globvars.h
condasm.o:	macrotyp.h
condasm.o:	message.h
condasm.o:	nametyp.h
condasm.o:	tables.h
	$(CC) $(CFLAGS) $(CFLcondasm) condasm.c -o condasm.o
errors.o:	asm.h
errors.o:	cli.h
errors.o:	$(EXTDIR1)/aof.h
errors.o:	condasm.h
errors.o:	constant.h
errors.o:	errors.c
errors.o:	errors.h
errors.o:	formatio.h
errors.o:	getline.h
errors.o:	globvars.h
errors.o:	listing.h
errors.o:	message.h
errors.o:	nametyp.h
	$(CC) $(CFLAGS) $(CFLerrors) errors.c -o errors.o
expr.o:	aofvars.h
expr.o:	apply.h
expr.o:	$(EXTDIR1)/aof.h
expr.o:	$(EXTDIR1)/aout.h
expr.o:	code.h
expr.o:	constant.h
expr.o:	errors.h
expr.o:	expr.c
expr.o:	expr.h
expr.o:	exprstor.h
expr.o:	exprtyp.h
expr.o:	getline.h
expr.o:	globvars.h
expr.o:	locallab.h
expr.o:	message.h
expr.o:	nametyp.h
expr.o:	occur.h
expr.o:	p1line.h
expr.o:	tables.h
expr.o:	tokens.h
expr.o:	vars.h
	$(CC) $(CFLAGS) $(CFLexpr) expr.c -o expr.o
exprstor.o:	constant.h
exprstor.o:	exprstor.c
exprstor.o:	exprstor.h
exprstor.o:	exprtyp.h
exprstor.o:	message.h
exprstor.o:	nametyp.h
	$(CC) $(CFLAGS) $(CFLexprstor) exprstor.c -o exprstor.o
formatio.o:	$(EXTDIR1)/fname.h
formatio.o:	constant.h
formatio.o:	datenow.h
formatio.o:	errors.h
formatio.o:	formatio.c
formatio.o:	formatio.h
formatio.o:	getline.h
formatio.o:	globvars.h
formatio.o:	hostdep.h
formatio.o:	message.h
formatio.o:	nametyp.h
formatio.o:	options.h
	$(CC) $(CFLAGS) $(CFLformatio) formatio.c -o formatio.o
fpio.o:	$(EXTDIR1)/ieeeflt.h
fpio.o:	constant.h
fpio.o:	fpio.c
fpio.o:	fpio.h
fpio.o:	message.h
fpio.o:	p1line.h
	$(CC) $(CFLAGS) $(CFLfpio) fpio.c -o fpio.o
getdir.o:	asm.h
getdir.o:	cli.h
getdir.o:	$(EXTDIR1)/aof.h
getdir.o:	condasm.h
getdir.o:	constant.h
getdir.o:	errors.h
getdir.o:	expr.h
getdir.o:	exprtyp.h
getdir.o:	getdir.c
getdir.o:	getdir.h
getdir.o:	getline.h
getdir.o:	globvars.h
getdir.o:	listing.h
getdir.o:	macrotyp.h
getdir.o:	message.h
getdir.o:	nametyp.h
getdir.o:	options.h
getdir.o:	store.h
getdir.o:	tables.h
	$(CC) $(CFLAGS) $(CFLgetdir) getdir.c -o getdir.o
getline.o:	aofvars.h
getline.o:	asm.h
getline.o:	$(EXTDIR1)/aof.h
getline.o:	$(EXTDIR1)/aout.h
getline.o:	constant.h
getline.o:	errors.h
getline.o:	formatio.h
getline.o:	getline.c
getline.o:	getline.h
getline.o:	globvars.h
getline.o:	macrotyp.h
getline.o:	message.h
getline.o:	nametyp.h
getline.o:	options.h
getline.o:	tables.h
	$(CC) $(CFLAGS) $(CFLgetline) getline.c -o getline.o
globvars.o:	aofvars.h
globvars.o:	$(EXTDIR1)/aof.h
globvars.o:	$(EXTDIR1)/aout.h
globvars.o:	constant.h
globvars.o:	datenow.h
globvars.o:	globvars.c
globvars.o:	globvars.h
globvars.o:	hostdep.h
globvars.o:	options.h
	$(CC) $(CFLAGS) $(CFLglobvars) globvars.c -o globvars.o
hostdep.o:	datenow.h
hostdep.o:	hostdep.c
hostdep.o:	hostdep.h
hostdep.o:	options.h
	$(CC) $(CFLAGS) $(CFLhostdep) hostdep.c -o hostdep.o
initdir.o:	$(EXTDIR1)/aof.h
initdir.o:	constant.h
initdir.o:	errors.h
initdir.o:	globvars.h
initdir.o:	initdir.c
initdir.o:	initdir.h
initdir.o:	message.h
initdir.o:	nametyp.h
initdir.o:	store.h
initdir.o:	tables.h
initdir.o:	vars.h
	$(CC) $(CFLAGS) $(CFLinitdir) initdir.c -o initdir.o
listing.o:	aofvars.h
listing.o:	$(EXTDIR1)/aof.h
listing.o:	$(EXTDIR1)/aout.h
listing.o:	constant.h
listing.o:	errors.h
listing.o:	formatio.h
listing.o:	getline.h
listing.o:	globvars.h
listing.o:	listing.c
listing.o:	listing.h
listing.o:	message.h
listing.o:	nametyp.h
listing.o:	p1line.h
listing.o:	vars.h
	$(CC) $(CFLAGS) $(CFLlisting) listing.c -o listing.o
literals.o:	aofvars.h
literals.o:	$(EXTDIR1)/aof.h
literals.o:	$(EXTDIR1)/aout.h
literals.o:	code.h
literals.o:	constant.h
literals.o:	errors.h
literals.o:	globvars.h
literals.o:	literals.c
literals.o:	literals.h
literals.o:	message.h
literals.o:	nametyp.h
literals.o:	store.h
literals.o:	tables.h
	$(CC) $(CFLAGS) $(CFLliterals) literals.c -o literals.o
locallab.o:	aofvars.h
locallab.o:	$(EXTDIR1)/aof.h
locallab.o:	$(EXTDIR1)/aout.h
locallab.o:	constant.h
locallab.o:	errors.h
locallab.o:	exprtyp.h
locallab.o:	globvars.h
locallab.o:	locallab.c
locallab.o:	locallab.h
locallab.o:	message.h
locallab.o:	nametyp.h
locallab.o:	p1line.h
locallab.o:	store.h
locallab.o:	tables.h
locallab.o:	tokens.h
locallab.o:	vars.h
	$(CC) $(CFLAGS) $(CFLlocallab) locallab.c -o locallab.o
macrotyp.o:	asm.h
macrotyp.o:	$(EXTDIR1)/aof.h
macrotyp.o:	condasm.h
macrotyp.o:	constant.h
macrotyp.o:	errors.h
macrotyp.o:	formatio.h
macrotyp.o:	getline.h
macrotyp.o:	globvars.h
macrotyp.o:	initdir.h
macrotyp.o:	listing.h
macrotyp.o:	locallab.h
macrotyp.o:	macrotyp.c
macrotyp.o:	macrotyp.h
macrotyp.o:	message.h
macrotyp.o:	nametyp.h
macrotyp.o:	occur.h
macrotyp.o:	p1line.h
macrotyp.o:	store.h
macrotyp.o:	tables.h
macrotyp.o:	vars.h
	$(CC) $(CFLAGS) $(CFLmacrotyp) macrotyp.c -o macrotyp.o
nametyp.o:	constant.h
nametyp.o:	nametyp.c
nametyp.o:	nametyp.h
nametyp.o:	store.h
	$(CC) $(CFLAGS) $(CFLnametyp) nametyp.c -o nametyp.o
occur.o:	asm.h
occur.o:	$(EXTDIR1)/aof.h
occur.o:	condasm.h
occur.o:	constant.h
occur.o:	formatio.h
occur.o:	getline.h
occur.o:	globvars.h
occur.o:	message.h
occur.o:	nametyp.h
occur.o:	occur.c
occur.o:	occur.h
occur.o:	store.h
occur.o:	tables.h
	$(CC) $(CFLAGS) $(CFLoccur) occur.c -o occur.o
opcode.o:	constant.h
opcode.o:	globvars.h
opcode.o:	message.h
opcode.o:	nametyp.h
opcode.o:	opcode.c
opcode.o:	opcode.h
opcode.o:	store.h
	$(CC) $(CFLAGS) $(CFLopcode) opcode.c -o opcode.o
p1dir.o:	aofvars.h
p1dir.o:	asdsupp.h
p1dir.o:	asm.h
p1dir.o:	$(EXTDIR1)/aof.h
p1dir.o:	$(EXTDIR1)/aout.h
p1dir.o:	code.h
p1dir.o:	condasm.h
p1dir.o:	constant.h
p1dir.o:	errors.h
p1dir.o:	expr.h
p1dir.o:	exprtyp.h
p1dir.o:	formatio.h
p1dir.o:	fpio.h
p1dir.o:	getdir.h
p1dir.o:	getline.h
p1dir.o:	globvars.h
p1dir.o:	initdir.h
p1dir.o:	listing.h
p1dir.o:	literals.h
p1dir.o:	locallab.h
p1dir.o:	macrotyp.h
p1dir.o:	message.h
p1dir.o:	nametyp.h
p1dir.o:	occur.h
p1dir.o:	p1dir.c
p1dir.o:	p1dir.h
p1dir.o:	p1line.h
p1dir.o:	store.h
p1dir.o:	tables.h
p1dir.o:	tokens.h
p1dir.o:	vars.h
	$(CC) $(CFLAGS) $(CFLp1dir) p1dir.c -o p1dir.o
p1line.o:	aofvars.h
p1line.o:	$(EXTDIR1)/aof.h
p1line.o:	$(EXTDIR1)/aout.h
p1line.o:	code.h
p1line.o:	condasm.h
p1line.o:	constant.h
p1line.o:	errors.h
p1line.o:	expr.h
p1line.o:	exprtyp.h
p1line.o:	formatio.h
p1line.o:	fpio.h
p1line.o:	getline.h
p1line.o:	globvars.h
p1line.o:	listing.h
p1line.o:	literals.h
p1line.o:	locallab.h
p1line.o:	macrotyp.h
p1line.o:	message.h
p1line.o:	nametyp.h
p1line.o:	occur.h
p1line.o:	opcode.h
p1line.o:	p1dir.h
p1line.o:	p1line.c
p1line.o:	p1line.h
p1line.o:	tables.h
p1line.o:	tokens.h
p1line.o:	vars.h
	$(CC) $(CFLAGS) $(CFLp1line) p1line.c -o p1line.o
p2dir.o:	aofvars.h
p2dir.o:	asm.h
p2dir.o:	$(EXTDIR1)/aof.h
p2dir.o:	$(EXTDIR1)/aout.h
p2dir.o:	code.h
p2dir.o:	condasm.h
p2dir.o:	constant.h
p2dir.o:	errors.h
p2dir.o:	expr.h
p2dir.o:	exprtyp.h
p2dir.o:	formatio.h
p2dir.o:	fpio.h
p2dir.o:	getdir.h
p2dir.o:	getline.h
p2dir.o:	globvars.h
p2dir.o:	initdir.h
p2dir.o:	listing.h
p2dir.o:	literals.h
p2dir.o:	locallab.h
p2dir.o:	macrotyp.h
p2dir.o:	message.h
p2dir.o:	nametyp.h
p2dir.o:	p1line.h
p2dir.o:	p2dir.c
p2dir.o:	p2dir.h
p2dir.o:	store.h
p2dir.o:	tables.h
p2dir.o:	tokens.h
p2dir.o:	vars.h
	$(CC) $(CFLAGS) $(CFLp2dir) p2dir.c -o p2dir.o
p2line.o:	aofvars.h
p2line.o:	$(EXTDIR1)/aof.h
p2line.o:	$(EXTDIR1)/aout.h
p2line.o:	code.h
p2line.o:	condasm.h
p2line.o:	constant.h
p2line.o:	errors.h
p2line.o:	expr.h
p2line.o:	exprtyp.h
p2line.o:	formatio.h
p2line.o:	fpio.h
p2line.o:	getline.h
p2line.o:	globvars.h
p2line.o:	listing.h
p2line.o:	literals.h
p2line.o:	locallab.h
p2line.o:	macrotyp.h
p2line.o:	message.h
p2line.o:	nametyp.h
p2line.o:	opcode.h
p2line.o:	p1line.h
p2line.o:	p2dir.h
p2line.o:	p2line.c
p2line.o:	p2line.h
p2line.o:	tables.h
p2line.o:	tokens.h
p2line.o:	vars.h
	$(CC) $(CFLAGS) $(CFLp2line) p2line.c -o p2line.o
stats.o:	constant.h
stats.o:	stats.c
stats.o:	stats.h
	$(CC) $(CFLAGS) $(CFLstats) stats.c -o stats.o
store.o:	message.h
store.o:	store.c
store.o:	store.h
	$(CC) $(CFLAGS) $(CFLstore) store.c -o store.o
symdump.o:	aofvars.h
symdump.o:	$(EXTDIR1)/aof.h
symdump.o:	$(EXTDIR1)/aout.h
symdump.o:	code.h
symdump.o:	constant.h
symdump.o:	formatio.h
symdump.o:	getline.h
symdump.o:	globvars.h
symdump.o:	message.h
symdump.o:	nametyp.h
symdump.o:	occur.h
symdump.o:	stats.h
symdump.o:	store.h
symdump.o:	symdump.c
symdump.o:	symdump.h
symdump.o:	tables.h
symdump.o:	vars.h
	$(CC) $(CFLAGS) $(CFLsymdump) symdump.c -o symdump.o
tables.o:	aofvars.h
tables.o:	$(EXTDIR1)/aof.h
tables.o:	$(EXTDIR1)/aout.h
tables.o:	code.h
tables.o:	constant.h
tables.o:	errors.h
tables.o:	globvars.h
tables.o:	message.h
tables.o:	nametyp.h
tables.o:	stats.h
tables.o:	store.h
tables.o:	tables.c
tables.o:	tables.h
	$(CC) $(CFLAGS) $(CFLtables) tables.c -o tables.o
tokens.o:	aofvars.h
tokens.o:	$(EXTDIR1)/aof.h
tokens.o:	$(EXTDIR1)/aout.h
tokens.o:	constant.h
tokens.o:	errors.h
tokens.o:	exprstor.h
tokens.o:	exprtyp.h
tokens.o:	getline.h
tokens.o:	globvars.h
tokens.o:	locallab.h
tokens.o:	macrotyp.h
tokens.o:	message.h
tokens.o:	nametyp.h
tokens.o:	occur.h
tokens.o:	options.h
tokens.o:	p1line.h
tokens.o:	tables.h
tokens.o:	tokens.c
tokens.o:	tokens.h
tokens.o:	vars.h
	$(CC) $(CFLAGS) $(CFLtokens) tokens.c -o tokens.o
vars.o:	aofvars.h
vars.o:	$(EXTDIR1)/aof.h
vars.o:	$(EXTDIR1)/aout.h
vars.o:	constant.h
vars.o:	errors.h
vars.o:	getline.h
vars.o:	globvars.h
vars.o:	macrotyp.h
vars.o:	message.h
vars.o:	nametyp.h
vars.o:	tables.h
vars.o:	vars.c
vars.o:	vars.h
	$(CC) $(CFLAGS) $(CFLvars) vars.c -o vars.o

# Dependencies to /nfs/sun1/bb/xdevt/release/bin/topcc from local to build directories

${COMPONENT}: $(OBJALL)
${COMPONENT}: $(HSOURCE)
${COMPONENT}: $(DERIVED)
${COMPONENT}: $(EXTLIB1)
	$(CC) -o ${COMPONENT} $(OBJALL) $(EXTLIB1) -s
	@echo "Made ${COMPONENT}"

install: ${COMPONENT}
	install ${COMPONENT} ${RISCOS_BUILD_TOOLS}/${COMPONENT}
	strip ${RISCOS_BUILD_TOOLS}/${COMPONENT}

clean:
	-rm ${OBJALL}
	-rm ${COMPONENT}
	-rm $(DERIVED)
	@echo cleaned ${COMPONENT}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${COMPONENT} links

links:
	./mklinks
	@echo made ${COMPONENT} links

datenow.h: FRC
	touch -f datenow.h
FRC:
