# This file was created by Megamake.
# Copyright (C) Advanced RISC Machines 1992.

TARGET=ncc

#CLXPATH=../../../../RiscOS/Sources/Lib/CLX/ccsolaris
CLX=${CLXPATH}/clx.a

CFLAGS=-g -c -O -I${CLXPATH}  -DNO_VERSION_STRINGS -DEXTENSION_SYSV -DHOST_WANTS_NO_BANNER=1 $(CFL)
CC=gcc
DERIVED=headers.c peeppat.c datenow.h errors.h

STANDARDOBJ=aaof.o asd.o asm.o
RISCiXOBJ=aoutobj.o dbx.o uasm.o

OBJALL=driver.o gen.o mcdep.o peephole.o aetree.o bind.o \
builtin.o cg.o codebuf.o compiler.o cse.o csescan.o sr.o flowgraf.o \
jopprint.o misc.o regalloc.o regsets.o store.o lex.o pp.o sem.o simplify.o \
syn.o vargen.o version.o ${STANDARDOBJ}

EXTDIR1=${CLXPATH}
EXTLIB1=${CLX}

HSOURCE=mcdpriv.h mcvsn.h ops.h target.h aeops.h aetree.h bind.h builtin.h \
cg.h cgdefs.h codebuf.h compiler.h cse.h cseguts.h defaults.h defs.h \
drivhelp.h flowgraf.h host.h globals.h version.h jopcode.h mcdep.h mipvsn.h \
regalloc.h regsets.h sr.h store.h util.h xrefs.h fevsn.h lex.h pp.h sem.h \
simplify.h syn.h vargen.h options.h 

HDRFILES = assert.h ctype.h errno.h float.h limits.h locale.h math.h \
setjmp.h signal.h stdarg.h stddef.h stdio.h stdlib.h string.h time.h 

HDRPATHS = ../cl/stdh/assert.h ../cl/stdh/ctype.h ../cl/stdh/errno.h \
../cl/stdh/float.h ../cl/stdh/limits.h ../cl/stdh/locale.h \
../cl/stdh/math.h ../cl/stdh/setjmp.h ../cl/stdh/signal.h \
../cl/stdh/stdarg.h ../cl/stdh/stddef.h ../cl/stdh/stdio.h \
../cl/stdh/stdlib.h ../cl/stdh/string.h ../cl/stdh/time.h 

noarg: ${TARGET}
	@echo "Made ${TARGET} (the default target)"

# Dependencies
aaof.o:	aaof.c
aaof.o:	mcdpriv.h
aaof.o:	ops.h
aaof.o:	target.h
aaof.o:	errors.h
aaof.o:	options.h
aaof.o:	$(EXTDIR1)/aof.h
aaof.o:	$(EXTDIR1)/chunkfmt.h
aaof.o:	$(EXTDIR1)/ieeeflt.h
aaof.o:	bind.h
aaof.o:	builtin.h
aaof.o:	cgdefs.h
aaof.o:	cg.h
aaof.o:	codebuf.h
aaof.o:	defaults.h
aaof.o:	defs.h
aaof.o:	globals.h
aaof.o:	host.h
aaof.o:	jopcode.h
aaof.o:	mcdep.h
aaof.o:	store.h
aaof.o:	version.h
aaof.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLaaof) aaof.c -o aaof.o
aoutobj.o:	aoutobj.c
aoutobj.o:	globals.h
aoutobj.o:	host.h
aoutobj.o:	options.h
aoutobj.o:	target.h
aoutobj.o:	defaults.h
aoutobj.o:	defs.h
aoutobj.o:	aout.h
aoutobj.o:	mcdep.h
aoutobj.o:	cgdefs.h
aoutobj.o:	jopcode.h
aoutobj.o:	mcdpriv.h
aoutobj.o:	codebuf.h
aoutobj.o:	xrefs.h
aoutobj.o:	store.h
aoutobj.o:	errors.h
	$(CC) $(CFLAGS) $(CFLaoutobj) aoutobj.c -o aoutobj.o
asd.o:	asd.c
asd.o:	mcdpriv.h
asd.o:	target.h
asd.o:	errors.h
asd.o:	options.h
asd.o:	sem.h
asd.o:	$(EXTDIR1)/asdfmt.h
asd.o:	$(EXTDIR1)/ieeeflt.h
asd.o:	aeops.h
asd.o:	bind.h
asd.o:	builtin.h
asd.o:	cgdefs.h
asd.o:	codebuf.h
asd.o:	defaults.h
asd.o:	defs.h
asd.o:	globals.h
asd.o:	host.h
asd.o:	jopcode.h
asd.o:	mcdep.h
asd.o:	regalloc.h
asd.o:	store.h
asd.o:	util.h
asd.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLasd) asd.c -o asd.o
asm.o:	asm.c
asm.o:	mcdpriv.h
asm.o:	ops.h
asm.o:	target.h
asm.o:	errors.h
asm.o:	options.h
asm.o:	$(EXTDIR1)/disass.h
asm.o:	$(EXTDIR1)/ieeeflt.h
asm.o:	builtin.h
asm.o:	cgdefs.h
asm.o:	codebuf.h
asm.o:	defaults.h
asm.o:	defs.h
asm.o:	globals.h
asm.o:	host.h
asm.o:	jopcode.h
asm.o:	mcdep.h
asm.o:	store.h
asm.o:	version.h
asm.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLasm) asm.c -o asm.o
dbx.o:	dbx.c
dbx.o:	aout.h
dbx.o:	globals.h
dbx.o:	host.h
dbx.o:	options.h
dbx.o:	target.h
dbx.o:	defaults.h
dbx.o:	defs.h
dbx.o:	mcdep.h
dbx.o:	cgdefs.h
dbx.o:	jopcode.h
dbx.o:	mcdpriv.h
dbx.o:	store.h
dbx.o:	codebuf.h
dbx.o:	aeops.h
dbx.o:	xrefs.h
dbx.o:	util.h
dbx.o:	regalloc.h
dbx.o:	bind.h
dbx.o:	sem.h
dbx.o:	errors.h
	$(CC) $(CFLAGS) $(CFLdbx) dbx.c -o dbx.o
gen.o:	gen.c
gen.o:	mcdpriv.h
gen.o:	ops.h
gen.o:	target.h
gen.o:	errors.h
gen.o:	options.h
gen.o:	$(EXTDIR1)/ieeeflt.h
gen.o:	aeops.h
gen.o:	bind.h
gen.o:	builtin.h
gen.o:	cgdefs.h
gen.o:	cg.h
gen.o:	codebuf.h
gen.o:	defaults.h
gen.o:	defs.h
gen.o:	flowgraf.h
gen.o:	globals.h
gen.o:	host.h
gen.o:	jopcode.h
gen.o:	mcdep.h
gen.o:	regalloc.h
gen.o:	store.h
gen.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLgen) gen.c -o gen.o
mcdep.o:	mcdep.c
mcdep.o:	mcdpriv.h
mcdep.o:	target.h
mcdep.o:	options.h
mcdep.o:	$(EXTDIR1)/ieeeflt.h
mcdep.o:	cgdefs.h
mcdep.o:	codebuf.h
mcdep.o:	compiler.h
mcdep.o:	defaults.h
mcdep.o:	defs.h
mcdep.o:	globals.h
mcdep.o:	host.h
mcdep.o:	jopcode.h
mcdep.o:	mcdep.h
mcdep.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLmcdep) mcdep.c -o mcdep.o
peephole.o:	mcdpriv.h
peephole.o:	ops.h
peephole.o:	peephole.c
peephole.o:	target.h
peephole.o:	errors.h
peephole.o:	options.h
peephole.o:	peeppat.c
peephole.o:	$(EXTDIR1)/ieeeflt.h
peephole.o:	aeops.h
peephole.o:	cgdefs.h
peephole.o:	cg.h
peephole.o:	codebuf.h
peephole.o:	defaults.h
peephole.o:	defs.h
peephole.o:	globals.h
peephole.o:	host.h
peephole.o:	jopcode.h
peephole.o:	mcdep.h
peephole.o:	regalloc.h
peephole.o:	store.h
peephole.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLpeephole) peephole.c -o peephole.o
aetree.o:	target.h
aetree.o:	options.h
aetree.o:	$(EXTDIR1)/ieeeflt.h
aetree.o:	aeops.h
aetree.o:	aetree.c
aetree.o:	aetree.h
aetree.o:	builtin.h
aetree.o:	defaults.h
aetree.o:	defs.h
aetree.o:	globals.h
aetree.o:	host.h
aetree.o:	store.h
	$(CC) $(CFLAGS) $(CFLaetree) aetree.c -o aetree.o
bind.o:	target.h
bind.o:	errors.h
bind.o:	options.h
bind.o:	lex.h
bind.o:	sem.h
bind.o:	vargen.h
bind.o:	$(EXTDIR1)/ieeeflt.h
bind.o:	aeops.h
bind.o:	aetree.h
bind.o:	bind.c
bind.o:	bind.h
bind.o:	builtin.h
bind.o:	cgdefs.h
bind.o:	codebuf.h
bind.o:	defaults.h
bind.o:	defs.h
bind.o:	globals.h
bind.o:	host.h
bind.o:	store.h
bind.o:	util.h
bind.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLbind) bind.c -o bind.o
builtin.o:	target.h
builtin.o:	options.h
builtin.o:	sem.h
builtin.o:	$(EXTDIR1)/ieeeflt.h
builtin.o:	aeops.h
builtin.o:	aetree.h
builtin.o:	bind.h
builtin.o:	builtin.c
builtin.o:	builtin.h
builtin.o:	defaults.h
builtin.o:	defs.h
builtin.o:	globals.h
builtin.o:	host.h
builtin.o:	store.h
	$(CC) $(CFLAGS) $(CFLbuiltin) builtin.c -o builtin.o
cg.o:	target.h
cg.o:	errors.h
cg.o:	options.h
cg.o:	sem.h
cg.o:	simplify.h
cg.o:	$(EXTDIR1)/ieeeflt.h
cg.o:	aeops.h
cg.o:	aetree.h
cg.o:	bind.h
cg.o:	builtin.h
cg.o:	cg.c
cg.o:	cgdefs.h
cg.o:	cg.h
cg.o:	codebuf.h
cg.o:	cse.h
cg.o:	defaults.h
cg.o:	defs.h
cg.o:	flowgraf.h
cg.o:	globals.h
cg.o:	host.h
cg.o:	jopcode.h
cg.o:	mcdep.h
cg.o:	regalloc.h
cg.o:	regsets.h
cg.o:	sr.h
cg.o:	store.h
cg.o:	util.h
cg.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLcg) cg.c -o cg.o
codebuf.o:	target.h
codebuf.o:	errors.h
codebuf.o:	options.h
codebuf.o:	$(EXTDIR1)/ieeeflt.h
codebuf.o:	aeops.h
codebuf.o:	bind.h
codebuf.o:	builtin.h
codebuf.o:	cgdefs.h
codebuf.o:	codebuf.c
codebuf.o:	codebuf.h
codebuf.o:	defaults.h
codebuf.o:	defs.h
codebuf.o:	globals.h
codebuf.o:	host.h
codebuf.o:	jopcode.h
codebuf.o:	mcdep.h
codebuf.o:	store.h
codebuf.o:	util.h
codebuf.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLcodebuf) codebuf.c -o codebuf.o
compiler.o:	target.h
compiler.o:	errors.h
compiler.o:	options.h
compiler.o:	lex.h
compiler.o:	pp.h
compiler.o:	sem.h
compiler.o:	syn.h
compiler.o:	vargen.h
compiler.o:	$(EXTDIR1)/fname.h
compiler.o:	$(EXTDIR1)/ieeeflt.h
compiler.o:	aeops.h
compiler.o:	aetree.h
compiler.o:	bind.h
compiler.o:	builtin.h
compiler.o:	cgdefs.h
compiler.o:	cg.h
compiler.o:	compiler.c
compiler.o:	compiler.h
compiler.o:	defaults.h
compiler.o:	defs.h
compiler.o:	globals.h
compiler.o:	host.h
compiler.o:	jopcode.h
compiler.o:	mcdep.h
compiler.o:	store.h
compiler.o:	version.h
compiler.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLcompiler) compiler.c -o compiler.o
cse.o:	target.h
cse.o:	errors.h
cse.o:	options.h
cse.o:	$(EXTDIR1)/ieeeflt.h
cse.o:	aeops.h
cse.o:	builtin.h
cse.o:	cgdefs.h
cse.o:	cg.h
cse.o:	cse.c
cse.o:	cseguts.h
cse.o:	cse.h
cse.o:	defaults.h
cse.o:	defs.h
cse.o:	flowgraf.h
cse.o:	globals.h
cse.o:	host.h
cse.o:	jopcode.h
cse.o:	mcdep.h
cse.o:	regalloc.h
cse.o:	regsets.h
cse.o:	store.h
	$(CC) $(CFLAGS) $(CFLcse) cse.c -o cse.o
csescan.o:	target.h
csescan.o:	errors.h
csescan.o:	options.h
csescan.o:	$(EXTDIR1)/ieeeflt.h
csescan.o:	aeops.h
csescan.o:	builtin.h
csescan.o:	cgdefs.h
csescan.o:	cg.h
csescan.o:	cseguts.h
csescan.o:	cse.h
csescan.o:	csescan.c
csescan.o:	defaults.h
csescan.o:	defs.h
csescan.o:	flowgraf.h
csescan.o:	globals.h
csescan.o:	host.h
csescan.o:	jopcode.h
csescan.o:	mcdep.h
csescan.o:	regalloc.h
csescan.o:	regsets.h
csescan.o:	store.h
	$(CC) $(CFLAGS) $(CFLcsescan) csescan.c -o csescan.o
driver.o:	target.h
driver.o:	options.h
driver.o:	$(EXTDIR1)/fname.h
driver.o:	$(EXTDIR1)/ieeeflt.h
driver.o:	$(EXTDIR1)/prgname.h
driver.o:	cgdefs.h
driver.o:	compiler.h
driver.o:	defaults.h
driver.o:	defs.h
driver.o:	driver.c
driver.o:	drivhelp.h
driver.o:	globals.h
driver.o:	host.h
driver.o:	jopcode.h
driver.o:	mcdep.h
driver.o:	version.h
	$(CC) $(CFLAGS) $(CFLdriver) driver.c -o driver.o
flowgraf.o:	target.h
flowgraf.o:	errors.h
flowgraf.o:	options.h
flowgraf.o:	simplify.h
flowgraf.o:	$(EXTDIR1)/ieeeflt.h
flowgraf.o:	aeops.h
flowgraf.o:	builtin.h
flowgraf.o:	cgdefs.h
flowgraf.o:	cg.h
flowgraf.o:	codebuf.h
flowgraf.o:	defaults.h
flowgraf.o:	defs.h
flowgraf.o:	flowgraf.c
flowgraf.o:	flowgraf.h
flowgraf.o:	globals.h
flowgraf.o:	host.h
flowgraf.o:	jopcode.h
flowgraf.o:	mcdep.h
flowgraf.o:	regalloc.h
flowgraf.o:	regsets.h
flowgraf.o:	sr.h
flowgraf.o:	store.h
flowgraf.o:	util.h
flowgraf.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLflowgraf) flowgraf.c -o flowgraf.o
jopprint.o:	target.h
jopprint.o:	options.h
jopprint.o:	$(EXTDIR1)/ieeeflt.h
jopprint.o:	aeops.h
jopprint.o:	aetree.h
jopprint.o:	cgdefs.h
jopprint.o:	defaults.h
jopprint.o:	defs.h
jopprint.o:	flowgraf.h
jopprint.o:	globals.h
jopprint.o:	host.h
jopprint.o:	jopcode.h
jopprint.o:	jopprint.c
	$(CC) $(CFLAGS) $(CFLjopprint) jopprint.c -o jopprint.o
misc.o:	target.h
misc.o:	errors.h
misc.o:	options.h
misc.o:	lex.h
misc.o:	$(EXTDIR1)/ieeeflt.h
misc.o:	aeops.h
misc.o:	aetree.h
misc.o:	defaults.h
misc.o:	defs.h
misc.o:	globals.h
misc.o:	host.h
misc.o:	misc.c
misc.o:	store.h
misc.o:	util.h
	$(CC) $(CFLAGS) $(CFLmisc) misc.c -o misc.o
regalloc.o:	target.h
regalloc.o:	errors.h
regalloc.o:	options.h
regalloc.o:	sem.h
regalloc.o:	$(EXTDIR1)/ieeeflt.h
regalloc.o:	aeops.h
regalloc.o:	builtin.h
regalloc.o:	cgdefs.h
regalloc.o:	cg.h
regalloc.o:	defaults.h
regalloc.o:	defs.h
regalloc.o:	flowgraf.h
regalloc.o:	globals.h
regalloc.o:	host.h
regalloc.o:	jopcode.h
regalloc.o:	mcdep.h
regalloc.o:	regalloc.c
regalloc.o:	regalloc.h
regalloc.o:	regsets.h
regalloc.o:	sr.h
regalloc.o:	store.h
	$(CC) $(CFLAGS) $(CFLregalloc) regalloc.c -o regalloc.o
regsets.o:	target.h
regsets.o:	options.h
regsets.o:	$(EXTDIR1)/ieeeflt.h
regsets.o:	cgdefs.h
regsets.o:	defaults.h
regsets.o:	defs.h
regsets.o:	globals.h
regsets.o:	host.h
regsets.o:	regsets.c
regsets.o:	regsets.h
regsets.o:	store.h
	$(CC) $(CFLAGS) $(CFLregsets) regsets.c -o regsets.o
sr.o:	target.h
sr.o:	errors.h
sr.o:	options.h
sr.o:	$(EXTDIR1)/ieeeflt.h
sr.o:	aeops.h
sr.o:	bind.h
sr.o:	builtin.h
sr.o:	cgdefs.h
sr.o:	cg.h
sr.o:	defaults.h
sr.o:	defs.h
sr.o:	flowgraf.h
sr.o:	globals.h
sr.o:	host.h
sr.o:	jopcode.h
sr.o:	mcdep.h
sr.o:	regalloc.h
sr.o:	regsets.h
sr.o:	sr.c
sr.o:	sr.h
sr.o:	store.h
	$(CC) $(CFLAGS) $(CFLsr) sr.c -o sr.o
store.o:	target.h
store.o:	errors.h
store.o:	options.h
store.o:	$(EXTDIR1)/ieeeflt.h
store.o:	cgdefs.h
store.o:	defaults.h
store.o:	defs.h
store.o:	globals.h
store.o:	host.h
store.o:	jopcode.h
store.o:	mcdep.h
store.o:	store.c
store.o:	store.h
	$(CC) $(CFLAGS) $(CFLstore) store.c -o store.o
uasm.o:	uasm.c
uasm.o:	globals.h
uasm.o:	host.h
uasm.o:	options.h
uasm.o:	target.h
uasm.o:	defaults.h
uasm.o:	defs.h
uasm.o:	mcdep.h
uasm.o:	cgdefs.h
uasm.o:	jopcode.h
uasm.o:	store.h
uasm.o:	codebuf.h
uasm.o:	builtin.h
uasm.o:	xrefs.h
uasm.o:	ops.h
uasm.o:	version.h
uasm.o:	mcdpriv.h
uasm.o:	errors.h
	$(CC) $(CFLAGS) $(CFLuasm) uasm.c -o uasm.o
version.o:	mcvsn.h
version.o:	target.h
version.o:	options.h
version.o:	fevsn.h
version.o:	$(EXTDIR1)/ieeeflt.h
version.o:	datenow.h
version.o:	defaults.h
version.o:	defs.h
version.o:	globals.h
version.o:	host.h
version.o:	mipvsn.h
version.o:	version.c
version.o:	version.h
	$(CC) $(CFLAGS) $(CFLversion) version.c -o version.o
lex.o:	target.h
lex.o:	errors.h
lex.o:	options.h
lex.o:	lex.c
lex.o:	lex.h
lex.o:	pp.h
lex.o:	$(EXTDIR1)/ieeeflt.h
lex.o:	aeops.h
lex.o:	bind.h
lex.o:	defaults.h
lex.o:	defs.h
lex.o:	globals.h
lex.o:	host.h
lex.o:	store.h
lex.o:	util.h
	$(CC) $(CFLAGS) $(CFLlex) lex.c -o lex.o
pp.o:	target.h
pp.o:	errors.h
pp.o:	headers.c
pp.o:	options.h
pp.o:	pp.c
pp.o:	pp.h
pp.o:	syn.h
pp.o:	$(EXTDIR1)/ieeeflt.h
pp.o:	defaults.h
pp.o:	defs.h
pp.o:	globals.h
pp.o:	host.h
pp.o:	store.h
	$(CC) $(CFLAGS) $(CFLpp) pp.c -o pp.o
sem.o:	target.h
sem.o:	errors.h
sem.o:	options.h
sem.o:	sem.c
sem.o:	sem.h
sem.o:	$(EXTDIR1)/ieeeflt.h
sem.o:	aeops.h
sem.o:	aetree.h
sem.o:	bind.h
sem.o:	builtin.h
sem.o:	defaults.h
sem.o:	defs.h
sem.o:	globals.h
sem.o:	host.h
sem.o:	store.h
sem.o:	util.h
	$(CC) $(CFLAGS) $(CFLsem) sem.c -o sem.o
simplify.o:	target.h
simplify.o:	errors.h
simplify.o:	options.h
simplify.o:	sem.h
simplify.o:	simplify.c
simplify.o:	simplify.h
simplify.o:	$(EXTDIR1)/ieeeflt.h
simplify.o:	aeops.h
simplify.o:	aetree.h
simplify.o:	bind.h
simplify.o:	builtin.h
simplify.o:	defaults.h
simplify.o:	defs.h
simplify.o:	globals.h
simplify.o:	host.h
simplify.o:	store.h
	$(CC) $(CFLAGS) $(CFLsimplify) simplify.c -o simplify.o
syn.o:	target.h
syn.o:	errors.h
syn.o:	options.h
syn.o:	lex.h
syn.o:	pp.h
syn.o:	sem.h
syn.o:	simplify.h
syn.o:	syn.c
syn.o:	syn.h
syn.o:	vargen.h
syn.o:	$(EXTDIR1)/ieeeflt.h
syn.o:	aeops.h
syn.o:	aetree.h
syn.o:	bind.h
syn.o:	builtin.h
syn.o:	cgdefs.h
syn.o:	codebuf.h
syn.o:	defaults.h
syn.o:	defs.h
syn.o:	globals.h
syn.o:	host.h
syn.o:	jopcode.h
syn.o:	mcdep.h
syn.o:	store.h
syn.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLsyn) syn.c -o syn.o
vargen.o:	target.h
vargen.o:	errors.h
vargen.o:	options.h
vargen.o:	lex.h
vargen.o:	sem.h
vargen.o:	simplify.h
vargen.o:	syn.h
vargen.o:	vargen.c
vargen.o:	vargen.h
vargen.o:	$(EXTDIR1)/ieeeflt.h
vargen.o:	aeops.h
vargen.o:	aetree.h
vargen.o:	bind.h
vargen.o:	builtin.h
vargen.o:	cgdefs.h
vargen.o:	codebuf.h
vargen.o:	defaults.h
vargen.o:	defs.h
vargen.o:	globals.h
vargen.o:	host.h
vargen.o:	jopcode.h
vargen.o:	mcdep.h
vargen.o:	regalloc.h
vargen.o:	store.h
vargen.o:	util.h
vargen.o:	xrefs.h
	$(CC) $(CFLAGS) $(CFLvargen) vargen.c -o vargen.o

${TARGET}: $(OBJALL)
${TARGET}: $(HSOURCE)
${TARGET}: $(DERIVED)
${TARGET}: $(EXTLIB1)
	$(CC) -o ${TARGET} $(OBJALL) $(EXTLIB1)
	@echo "Made ${TARGET}"

install: ${TARGET}
	install ${TARGET} ${RISCOS_BUILD_TOOLS}/armcc
	strip ${RISCOS_BUILD_TOOLS}/armcc

errors.h: genhdrs miperrs.h feerrs.h mcerrs.h
	./genhdrs -eerrors.h -qmiperrs.h -qfeerrs.h -qmcerrs.h -s
headers.c: genhdrs $(HDRFILES)
	./genhdrs -o headers.c -I../ccsolaris/ $(HDRFILES)
genhdrs: genhdrs.c
	$(CC) -o genhdrs genhdrs.c
peeppat.c: ../arm/peeppat peepgen
	./peepgen ../arm/peeppat peeppat.c

#peepgen: peepgen.c jopcode.h mcdpriv.h
#	$(CC) -o peepgen peepgen.c -I../mip -I../arm
peepgen: peepgen.c jopcode.h mcdpriv.h
	$(CC) -o peepgen peepgen.c

clean:
	-rm ${OBJALL}
	-rm ${TARGET} peepgen genhdrs
	-rm $(DERIVED)
	@echo cleaned ${TARGET}

realclean: clean
	-find . -type l -print | xargs rm
	@echo cleaned ${TARGET} links

links:
	./mklinks
	@echo made ${TARGET} links

datenow.h: FRC
	touch -f datenow.h
FRC:
