## Makefile.am -- Process this file with automake to product Makefile.in

## make will enter each of these directories in the order specified and
## execute the Makefile.  The '.' directory is processed *last* unless
## listed explicitly in SUBDIRS
SUBDIRS = config DebugLib src

EXTRA_DIST           = VersionNum

## The following files are deleted on "make maintainer-clean"
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure config-h.in \
                       stamp-h.in acinclude.m4 $(AUX_DIST) \
                       config.h.in config.log config.status
