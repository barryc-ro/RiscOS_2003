# Makefile for /vobs/mx/src, only the relevant stuff (omits msm)
#
# dbrower   11/27/95 - merge with 3.0
# dbrower   06/25/96 - jigger for 3.2
# dbrower   07/18/96 - remove mzmgs special case (bug 383396)
# dbrower   07/23/96 - merge with 3.0_dev.
# kcoleman  09/16/96 - make sure mnmsgc gets built before mnidlc in x-compile
# dbrower   10/15/96 - remove misc.

ifndef HOSTARCH
	HOSTARCH = $(ARCH)
endif

# these don't need the idl compiler
NOIDLDIRS = ys mt mn

# these need the idl compiler; ye before yr and yd.
IDLDIRS = yc yo ye yr yd

.PHONY : all lib share compile link clean cleanbin cleanobj cleanlib idlhdrs \
	olint boot idlboot idldirs msm resource install_lib install_bin

all:    idl boot idlboot idldirs

idl	:
	@if [ "${ARCH}" != "${HOSTARCH}" ]; then \
		echo Building idl compiler on ${HOSTARCH}; \
		for d in ${NOIDLDIRS}; \
		do \
			( cd $$d; $(MAKE) ARCH=$(HOSTARCH) lib ); \
		done; \
		( cd ys; $(MAKE) ARCH=$(HOSTARCH) all ); \
		( cd yo; $(MAKE) ARCH=$(HOSTARCH) libyotk ); \
		( cd yr; $(MAKE) ARCH=$(HOSTARCH) libyr ); \
		( cd yc; $(MAKE) ARCH=$(HOSTARCH) all ); \
	fi

boot	:
	for d in $(NOIDLDIRS); \
        do \
		( cd $$d; $(MAKE) ARCH=$(ARCH) lib compile ); \
	done

idlboot:
	(cd ys; $(MAKE) all)
	(cd yo; $(MAKE) libyotk)
	(cd yr; $(MAKE) libyr)
	(cd yc; $(MAKE) all)
	(cd ../inc; $(MAKE) all)

idldirs:
	( cd ../inc; $(MAKE) ARCH=$(ARCH) $@ )
	for d in $(NOIDLDIRS) $(IDLDIRS); \
        do \
		( cd $$d; $(MAKE) ARCH=$(ARCH) all ); \
	done

promote resource install_bin install_lib lib compile link \
clean cleanbin cleanobj cleanlib idlhdrs olint :
	(cd ../inc; $(MAKE) ARCH=$(ARCH) $@ )
	for d in $(NOIDLDIRS) $(IDLDIRS); \
        do \
		( cd $$d; $(MAKE) ARCH=$(ARCH) $@ ); \
	done

share :
	for d in $(NOIDLDIRS) $(IDLDIRS); \
        do \
		( cd $$d; $(MAKE) ARCH=$(ARCH) SHARE=yes share ); \
	done

