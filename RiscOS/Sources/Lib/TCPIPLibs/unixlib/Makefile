#
# Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
#
# Makefile for unixlib.a
#
# Makefile,v
# Revision 1.1  95/04/20  09:50:39  kwelton
# Initial revision
#
#
LIBDIR = <Lib$Dir>

#
# Generic options:
#
MKDIR   = cdir
WIPE    = -wipe
RM      = -remove
CP      = copy
CC	= cc
AS	= objasm
LD	= link
LIB	= libfile
CMHG	= cmhg

WFLAGS  = ~c~v
CPFLAGS = ~cflnr~v

INCLUDES=	TCPIPLibs:,C:

CFLAGS=		-depend !Depend -Wp -zps0 -I${INCLUDES} -fah -Fn -DINET -DCOMPAT_INET4
CFLAGSZM=	-depend !Depend -Wp -zps1 -I${INCLUDES} -ffah -DINET -DCOMPAT_INET4 -zM

CCZM=		${CC} ${CFLAGSZM}

CFILES=		access.c chdir.c chmod.c close.c filestat.c flushinput.c \
		fstat.c getdtabsiz.c getegid.c geteuid.c getgroups.c \
		gethostnam.c getlogin.c getpass.c getpid.c getpwent.c \
		getpwnam.c getpwuid.c getuid.c getusernam.c getwd.c \
		gtimeofday.c herror.c index.c ioctl.c killfile.c lseek.c \
		memops.c osreadc.c read.c readdir.c readv.c string.c \
		stubs.c write.c writev.c xgets.c xperror.c xputchar.c \
		getopt.c strsep.c kvm.c err.c errlst.c xstrerror.c

OFILES=		access.o chdir.o chmod.o close.o filestat.o flushinput.o \
		fstat.o getdtabsiz.o getegid.o geteuid.o getgroups.o \
		gethostnam.o getlogin.o getpass.o getpid.o getpwent.o \
		getpwnam.o getpwuid.o getuid.o getusernam.o getwd.o \
		gtimeofday.o herror.o index.o ioctl.o killfile.o lseek.o \
		memops.o osreadc.o read.o readdir.o readv.o string.o \
		stubs.o write.o writev.o xgets.o xperror.o xputchar.o \
		getopt.o strsep.o kvm.o err.o errlst.o xstrerror.o

ZFILES=		z.access z.chdir z.chmod z.close z.filestat z.flushinput \
		z.fstat z.getdtabsiz z.getegid z.geteuid z.getgroups \
		z.gethostnam z.getlogin z.getpass z.getpid z.getpwent \
		z.getpwnam z.getpwuid z.getuid z.getusernam z.getwd \
		z.gtimeofday z.herror z.index z.ioctl z.killfile z.lseek \
		z.memops z.osreadc z.read z.readdir z.readv z.string \
		z.stubs z.write z.writev z.xgets z.xperror z.xputchar \
		z.getopt z.strsep z.kvm z.err z.errlst z.xstrerror

.SUFFIXES:	.z .o .c .s

.c.z:;	     ${CCZM} -c -o $@ $<
.c.o:;       ${CC} ${CFLAGS} -c -o $@ $<

COMPONENT = unixlib
LIBDIR = <Lib$Dir>

all:	unixlib unixlibzm

unixlib:	${OFILES} local_dirs
	${LIB} -c unixlib ${OFILES}
	${LIB} -o unixlib

unixlibzm:	${ZFILES} local_dirs
	${LIB} -c unixlibzm ${ZFILES}
	${LIB} -o unixlibzm

export: export_${PHASE}

export_hdrs:
	${CP} h.unixlib	TCPIPLibs:h.unixlib	${CPFLAGS}
        @echo ${COMPONENT}: export complete (hdrs)

export_libs: unixlib unixlibzm dirs
	${CP} unixlib	TCPIPLibs:o.unixlib	${CPFLAGS}
	${CP} unixlibzm	TCPIPLibs:o.unixlibzm	${CPFLAGS}

dirs:
	${MKDIR} $(LIBDIR)
	${MKDIR} $(LIBDIR).TCPIPLibs
	${MKDIR} $(LIBDIR).TCPIPLibs.o

local_dirs:
        ${MKDIR} o
        ${MKDIR} z
        Create local_dirs

clean:
	${WIPE} o.* ${WFLAGS}
	${WIPE} z.* ${WFLAGS}
	${RM} local_dirs
	${RM} unixlib
	${RM} unixlibzm


# Dynamic dependencies:
o.getegid:	c.getegid
o.getegid:	h.unixlib
o.getegid:	h.stubs
o.geteuid:	c.geteuid
o.geteuid:	h.unixlib
o.geteuid:	h.stubs
o.getgroups:	c.getgroups
o.getgroups:	h.unixlib
o.getgroups:	h.stubs
o.getlogin:	c.getlogin
o.getlogin:	h.unixlib
o.getpass:	c.getpass
o.getpass:	h.unixlib
o.getpid:	c.getpid
o.getpid:	h.unixlib
o.getpid:	h.stubs
o.getpwent:	c.getpwent
o.getpwent:	h.unixlib
o.getuid:	c.getuid
o.getuid:	h.unixlib
o.getuid:	h.stubs
o.herror:	c.herror
o.herror:	h.unixlib
o.index:	c.index
o.index:	h.unixlib
o.ioctl:	c.ioctl
o.memops:	c.memops
o.memops:	h.unixlib
o.string:	c.string
o.string:	h.unixlib
o.xgets:	c.xgets
o.xgets:	h.unixlib
o.getopt:	c.getopt
o.getopt:	h.unixlib
o.errlst:	c.errlst
o.errlst:	h.unixlib
o.xstrerror:	c.xstrerror
o.xstrerror:	h.unixlib
z.getegid:	c.getegid
z.getegid:	h.unixlib
z.getegid:	h.stubs
z.geteuid:	c.geteuid
z.geteuid:	h.unixlib
z.geteuid:	h.stubs
z.getgroups:	c.getgroups
z.getgroups:	h.unixlib
z.getgroups:	h.stubs
z.getlogin:	c.getlogin
z.getlogin:	h.unixlib
z.getpass:	c.getpass
z.getpass:	h.unixlib
z.getpid:	c.getpid
z.getpid:	h.unixlib
z.getpid:	h.stubs
z.getpwent:	c.getpwent
z.getpwent:	h.unixlib
z.getuid:	c.getuid
z.getuid:	h.unixlib
z.getuid:	h.stubs
z.herror:	c.herror
z.herror:	h.unixlib
z.index:	c.index
z.index:	h.unixlib
z.ioctl:	c.ioctl
z.memops:	c.memops
z.memops:	h.unixlib
z.string:	c.string
z.string:	h.unixlib
z.xgets:	c.xgets
z.xgets:	h.unixlib
z.getopt:	c.getopt
z.getopt:	h.unixlib
z.errlst:	c.errlst
z.errlst:	h.unixlib
z.xstrerror:	c.xstrerror
z.xstrerror:	h.unixlib
o.osreadc:	c.osreadc
o.osreadc:	C:h.kernel
o.osreadc:	h.unixlib
z.osreadc:	c.osreadc
z.osreadc:	C:h.kernel
z.osreadc:	h.unixlib
o.chdir:	c.chdir
o.chdir:	C:h.kernel
o.chdir:	C:h.swis
o.chdir:	h.unixlib
o.flushinput:	c.flushinput
o.flushinput:	C:h.kernel
o.flushinput:	C:h.swis
o.flushinput:	h.unixlib
o.gethostnam:	c.gethostnam
o.gethostnam:	C:h.kernel
o.gethostnam:	C:h.swis
o.gethostnam:	h.unixlib
o.gethostnam:	h.stubs
o.getusernam:	c.getusernam
o.getusernam:	C:h.kernel
o.getusernam:	C:h.swis
o.getusernam:	h.unixlib
o.getusernam:	h.stubs
o.getwd:	c.getwd
o.getwd:	C:h.kernel
o.getwd:	C:h.swis
o.getwd:	h.unixlib
o.killfile:	c.killfile
o.killfile:	C:h.kernel
o.killfile:	C:h.swis
o.killfile:	h.unixlib
o.readdir:	c.readdir
o.readdir:	C:h.kernel
o.readdir:	C:h.swis
o.readdir:	h.unixlib
o.xputchar:	c.xputchar
o.xputchar:	C:h.kernel
o.xputchar:	C:h.swis
o.xputchar:	h.unixlib
z.chdir:	c.chdir
z.chdir:	C:h.kernel
z.chdir:	C:h.swis
z.chdir:	h.unixlib
z.flushinput:	c.flushinput
z.flushinput:	C:h.kernel
z.flushinput:	C:h.swis
z.flushinput:	h.unixlib
z.gethostnam:	c.gethostnam
z.gethostnam:	C:h.kernel
z.gethostnam:	C:h.swis
z.gethostnam:	h.unixlib
z.gethostnam:	h.stubs
z.getusernam:	c.getusernam
z.getusernam:	C:h.kernel
z.getusernam:	C:h.swis
z.getusernam:	h.unixlib
z.getusernam:	h.stubs
z.getwd:	c.getwd
z.getwd:	C:h.kernel
z.getwd:	C:h.swis
z.getwd:	h.unixlib
z.killfile:	c.killfile
z.killfile:	C:h.kernel
z.killfile:	C:h.swis
z.killfile:	h.unixlib
z.readdir:	c.readdir
z.readdir:	C:h.kernel
z.readdir:	C:h.swis
z.readdir:	h.unixlib
z.xputchar:	c.xputchar
z.xputchar:	C:h.kernel
z.xputchar:	C:h.swis
z.xputchar:	h.unixlib
o.access:	c.access
o.access:	TCPIPLibs:h.errno
o.access:	TCPIPLibs:sys.h.errno
o.access:	C:h.kernel
o.access:	C:h.kernel
o.access:	C:h.swis
o.access:	TCPIPLibs:h.unistd
o.access:	TCPIPLibs:sys.h.cdefs
o.access:	TCPIPLibs:sys.h.types
o.access:	TCPIPLibs:sys.h.cdefs
o.access:	TCPIPLibs:machine.h.endian
o.access:	TCPIPLibs:machine.h.ansi
o.access:	TCPIPLibs:machine.h.types
o.access:	TCPIPLibs:sys.h.unistd
o.access:	h.unixlib
o.chmod:	c.chmod
o.chmod:	TCPIPLibs:h.errno
o.chmod:	TCPIPLibs:sys.h.errno
o.chmod:	C:h.kernel
o.chmod:	C:h.kernel
o.chmod:	C:h.swis
o.chmod:	h.unixlib
o.close:	c.close
o.close:	h.unixlib
o.close:	TCPIPLibs:h.socklib
o.close:	TCPIPLibs:sys.h.types
o.close:	TCPIPLibs:sys.h.cdefs
o.close:	TCPIPLibs:machine.h.endian
o.close:	TCPIPLibs:machine.h.ansi
o.close:	TCPIPLibs:machine.h.types
o.close:	TCPIPLibs:sys.h.socket
o.close:	TCPIPLibs:sys.h.cdefs
o.close:	TCPIPLibs:sys.h.stat
o.close:	TCPIPLibs:sys.h.time
o.close:	TCPIPLibs:sys.h.cdefs
o.close:	TCPIPLibs:sys.h.cdefs
o.close:	TCPIPLibs:sys.h.time
o.close:	TCPIPLibs:sys.h.uio
o.close:	TCPIPLibs:sys.h.cdefs
o.close:	C:h.kernel
o.filestat:	c.filestat
o.filestat:	TCPIPLibs:h.errno
o.filestat:	TCPIPLibs:sys.h.errno
o.filestat:	C:h.kernel
o.filestat:	C:h.kernel
o.filestat:	C:h.swis
o.filestat:	h.unixlib
o.fstat:	c.fstat
o.fstat:	h.unixlib
o.fstat:	TCPIPLibs:h.socklib
o.fstat:	TCPIPLibs:sys.h.types
o.fstat:	TCPIPLibs:sys.h.cdefs
o.fstat:	TCPIPLibs:machine.h.endian
o.fstat:	TCPIPLibs:machine.h.ansi
o.fstat:	TCPIPLibs:machine.h.types
o.fstat:	TCPIPLibs:sys.h.socket
o.fstat:	TCPIPLibs:sys.h.cdefs
o.fstat:	TCPIPLibs:sys.h.stat
o.fstat:	TCPIPLibs:sys.h.time
o.fstat:	TCPIPLibs:sys.h.cdefs
o.fstat:	TCPIPLibs:sys.h.cdefs
o.fstat:	TCPIPLibs:sys.h.time
o.fstat:	TCPIPLibs:sys.h.uio
o.fstat:	TCPIPLibs:sys.h.cdefs
o.fstat:	C:h.kernel
o.getdtabsiz:	c.getdtabsiz
o.getdtabsiz:	h.unixlib
o.getdtabsiz:	TCPIPLibs:h.socklib
o.getdtabsiz:	TCPIPLibs:sys.h.types
o.getdtabsiz:	TCPIPLibs:sys.h.cdefs
o.getdtabsiz:	TCPIPLibs:machine.h.endian
o.getdtabsiz:	TCPIPLibs:machine.h.ansi
o.getdtabsiz:	TCPIPLibs:machine.h.types
o.getdtabsiz:	TCPIPLibs:sys.h.socket
o.getdtabsiz:	TCPIPLibs:sys.h.cdefs
o.getdtabsiz:	TCPIPLibs:sys.h.stat
o.getdtabsiz:	TCPIPLibs:sys.h.time
o.getdtabsiz:	TCPIPLibs:sys.h.cdefs
o.getdtabsiz:	TCPIPLibs:sys.h.cdefs
o.getdtabsiz:	TCPIPLibs:sys.h.time
o.getdtabsiz:	TCPIPLibs:sys.h.uio
o.getdtabsiz:	TCPIPLibs:sys.h.cdefs
o.getdtabsiz:	C:h.kernel
o.getpwnam:	c.getpwnam
o.getpwnam:	TCPIPLibs:h.pwd
o.getpwnam:	TCPIPLibs:sys.h.types
o.getpwnam:	TCPIPLibs:sys.h.cdefs
o.getpwnam:	TCPIPLibs:machine.h.endian
o.getpwnam:	TCPIPLibs:machine.h.ansi
o.getpwnam:	TCPIPLibs:machine.h.types
o.getpwnam:	TCPIPLibs:sys.h.cdefs
o.getpwnam:	h.stubs
o.getpwnam:	h.unixlib
o.getpwuid:	c.getpwuid
o.getpwuid:	TCPIPLibs:h.pwd
o.getpwuid:	TCPIPLibs:sys.h.types
o.getpwuid:	TCPIPLibs:sys.h.cdefs
o.getpwuid:	TCPIPLibs:machine.h.endian
o.getpwuid:	TCPIPLibs:machine.h.ansi
o.getpwuid:	TCPIPLibs:machine.h.types
o.getpwuid:	TCPIPLibs:sys.h.cdefs
o.getpwuid:	h.unixlib
o.getpwuid:	h.stubs
o.gtimeofday:	c.gtimeofday
o.gtimeofday:	C:h.kernel
o.gtimeofday:	TCPIPLibs:sys.h.time
o.gtimeofday:	TCPIPLibs:sys.h.cdefs
o.gtimeofday:	TCPIPLibs:sys.h.types
o.gtimeofday:	TCPIPLibs:sys.h.cdefs
o.gtimeofday:	TCPIPLibs:machine.h.endian
o.gtimeofday:	TCPIPLibs:machine.h.ansi
o.gtimeofday:	TCPIPLibs:machine.h.types
o.gtimeofday:	h.unixlib
o.lseek:	c.lseek
o.lseek:	TCPIPLibs:h.errno
o.lseek:	TCPIPLibs:sys.h.errno
o.lseek:	C:h.kernel
o.lseek:	h.unixlib
o.read:	c.read
o.read:	h.unixlib
o.read:	TCPIPLibs:h.socklib
o.read:	TCPIPLibs:sys.h.types
o.read:	TCPIPLibs:sys.h.cdefs
o.read:	TCPIPLibs:machine.h.endian
o.read:	TCPIPLibs:machine.h.ansi
o.read:	TCPIPLibs:machine.h.types
o.read:	TCPIPLibs:sys.h.socket
o.read:	TCPIPLibs:sys.h.cdefs
o.read:	TCPIPLibs:sys.h.stat
o.read:	TCPIPLibs:sys.h.time
o.read:	TCPIPLibs:sys.h.cdefs
o.read:	TCPIPLibs:sys.h.cdefs
o.read:	TCPIPLibs:sys.h.time
o.read:	TCPIPLibs:sys.h.uio
o.read:	TCPIPLibs:sys.h.cdefs
o.read:	C:h.kernel
o.readv:	c.readv
o.readv:	TCPIPLibs:sys.h.types
o.readv:	TCPIPLibs:sys.h.cdefs
o.readv:	TCPIPLibs:machine.h.endian
o.readv:	TCPIPLibs:machine.h.ansi
o.readv:	TCPIPLibs:machine.h.types
o.readv:	TCPIPLibs:sys.h.uio
o.readv:	TCPIPLibs:sys.h.cdefs
o.readv:	h.unixlib
o.readv:	TCPIPLibs:h.socklib
o.readv:	TCPIPLibs:sys.h.types
o.readv:	TCPIPLibs:sys.h.socket
o.readv:	TCPIPLibs:sys.h.cdefs
o.readv:	TCPIPLibs:sys.h.stat
o.readv:	TCPIPLibs:sys.h.time
o.readv:	TCPIPLibs:sys.h.cdefs
o.readv:	TCPIPLibs:sys.h.cdefs
o.readv:	TCPIPLibs:sys.h.time
o.readv:	TCPIPLibs:sys.h.uio
o.readv:	C:h.kernel
o.stubs:	c.stubs
o.stubs:	TCPIPLibs:h.pwd
o.stubs:	TCPIPLibs:sys.h.types
o.stubs:	TCPIPLibs:sys.h.cdefs
o.stubs:	TCPIPLibs:machine.h.endian
o.stubs:	TCPIPLibs:machine.h.ansi
o.stubs:	TCPIPLibs:machine.h.types
o.stubs:	TCPIPLibs:sys.h.cdefs
o.stubs:	h.stubs
o.write:	c.write
o.write:	h.unixlib
o.write:	TCPIPLibs:h.socklib
o.write:	TCPIPLibs:sys.h.types
o.write:	TCPIPLibs:sys.h.cdefs
o.write:	TCPIPLibs:machine.h.endian
o.write:	TCPIPLibs:machine.h.ansi
o.write:	TCPIPLibs:machine.h.types
o.write:	TCPIPLibs:sys.h.socket
o.write:	TCPIPLibs:sys.h.cdefs
o.write:	TCPIPLibs:sys.h.stat
o.write:	TCPIPLibs:sys.h.time
o.write:	TCPIPLibs:sys.h.cdefs
o.write:	TCPIPLibs:sys.h.cdefs
o.write:	TCPIPLibs:sys.h.time
o.write:	TCPIPLibs:sys.h.uio
o.write:	TCPIPLibs:sys.h.cdefs
o.write:	C:h.kernel
o.writev:	c.writev
o.writev:	TCPIPLibs:sys.h.types
o.writev:	TCPIPLibs:sys.h.cdefs
o.writev:	TCPIPLibs:machine.h.endian
o.writev:	TCPIPLibs:machine.h.ansi
o.writev:	TCPIPLibs:machine.h.types
o.writev:	TCPIPLibs:sys.h.uio
o.writev:	TCPIPLibs:sys.h.cdefs
o.writev:	TCPIPLibs:h.socklib
o.writev:	TCPIPLibs:sys.h.types
o.writev:	TCPIPLibs:sys.h.socket
o.writev:	TCPIPLibs:sys.h.cdefs
o.writev:	TCPIPLibs:sys.h.stat
o.writev:	TCPIPLibs:sys.h.time
o.writev:	TCPIPLibs:sys.h.cdefs
o.writev:	TCPIPLibs:sys.h.cdefs
o.writev:	TCPIPLibs:sys.h.time
o.writev:	TCPIPLibs:sys.h.uio
o.writev:	C:h.kernel
o.writev:	h.unixlib
o.xperror:	c.xperror
o.xperror:	TCPIPLibs:sys.h.types
o.xperror:	TCPIPLibs:sys.h.cdefs
o.xperror:	TCPIPLibs:machine.h.endian
o.xperror:	TCPIPLibs:machine.h.ansi
o.xperror:	TCPIPLibs:machine.h.types
o.xperror:	TCPIPLibs:h.unistd
o.xperror:	TCPIPLibs:sys.h.cdefs
o.xperror:	TCPIPLibs:sys.h.types
o.xperror:	TCPIPLibs:sys.h.unistd
o.xperror:	TCPIPLibs:sys.h.errno
o.xperror:	C:h.kernel
o.xperror:	h.unixlib
o.strsep:	c.strsep
o.strsep:	TCPIPLibs:sys.h.cdefs
o.strsep:	h.unixlib
o.kvm:	c.kvm
o.kvm:	TCPIPLibs:sys.h.types
o.kvm:	TCPIPLibs:sys.h.cdefs
o.kvm:	TCPIPLibs:machine.h.endian
o.kvm:	TCPIPLibs:machine.h.ansi
o.kvm:	TCPIPLibs:machine.h.types
o.kvm:	TCPIPLibs:h.kvm
o.kvm:	TCPIPLibs:h.nlist
o.kvm:	TCPIPLibs:sys.h.cdefs
o.kvm:	TCPIPLibs:sys.h.cdefs
o.kvm:	h.unixlib
o.err:	c.err
o.err:	TCPIPLibs:h.err
o.err:	TCPIPLibs:machine.h.ansi
o.err:	TCPIPLibs:sys.h.cdefs
o.err:	h.unixlib
o.err:	TCPIPLibs:h.socklib
o.err:	TCPIPLibs:sys.h.types
o.err:	TCPIPLibs:sys.h.cdefs
o.err:	TCPIPLibs:machine.h.endian
o.err:	TCPIPLibs:machine.h.ansi
o.err:	TCPIPLibs:machine.h.types
o.err:	TCPIPLibs:sys.h.socket
o.err:	TCPIPLibs:sys.h.cdefs
o.err:	TCPIPLibs:sys.h.stat
o.err:	TCPIPLibs:sys.h.time
o.err:	TCPIPLibs:sys.h.cdefs
o.err:	TCPIPLibs:sys.h.cdefs
o.err:	TCPIPLibs:sys.h.time
o.err:	TCPIPLibs:sys.h.uio
o.err:	TCPIPLibs:sys.h.cdefs
o.err:	C:h.kernel
o.err:	TCPIPLibs:h.riscos
z.access:	c.access
z.access:	TCPIPLibs:h.errno
z.access:	TCPIPLibs:sys.h.errno
z.access:	C:h.kernel
z.access:	C:h.kernel
z.access:	C:h.swis
z.access:	TCPIPLibs:h.unistd
z.access:	TCPIPLibs:sys.h.cdefs
z.access:	TCPIPLibs:sys.h.types
z.access:	TCPIPLibs:sys.h.cdefs
z.access:	TCPIPLibs:machine.h.endian
z.access:	TCPIPLibs:machine.h.ansi
z.access:	TCPIPLibs:machine.h.types
z.access:	TCPIPLibs:sys.h.unistd
z.access:	h.unixlib
z.chmod:	c.chmod
z.chmod:	TCPIPLibs:h.errno
z.chmod:	TCPIPLibs:sys.h.errno
z.chmod:	C:h.kernel
z.chmod:	C:h.kernel
z.chmod:	C:h.swis
z.chmod:	h.unixlib
z.close:	c.close
z.close:	h.unixlib
z.close:	TCPIPLibs:h.socklib
z.close:	TCPIPLibs:sys.h.types
z.close:	TCPIPLibs:sys.h.cdefs
z.close:	TCPIPLibs:machine.h.endian
z.close:	TCPIPLibs:machine.h.ansi
z.close:	TCPIPLibs:machine.h.types
z.close:	TCPIPLibs:sys.h.socket
z.close:	TCPIPLibs:sys.h.cdefs
z.close:	TCPIPLibs:sys.h.stat
z.close:	TCPIPLibs:sys.h.time
z.close:	TCPIPLibs:sys.h.cdefs
z.close:	TCPIPLibs:sys.h.cdefs
z.close:	TCPIPLibs:sys.h.time
z.close:	TCPIPLibs:sys.h.uio
z.close:	TCPIPLibs:sys.h.cdefs
z.close:	C:h.kernel
z.filestat:	c.filestat
z.filestat:	TCPIPLibs:h.errno
z.filestat:	TCPIPLibs:sys.h.errno
z.filestat:	C:h.kernel
z.filestat:	C:h.kernel
z.filestat:	C:h.swis
z.filestat:	h.unixlib
z.fstat:	c.fstat
z.fstat:	h.unixlib
z.fstat:	TCPIPLibs:h.socklib
z.fstat:	TCPIPLibs:sys.h.types
z.fstat:	TCPIPLibs:sys.h.cdefs
z.fstat:	TCPIPLibs:machine.h.endian
z.fstat:	TCPIPLibs:machine.h.ansi
z.fstat:	TCPIPLibs:machine.h.types
z.fstat:	TCPIPLibs:sys.h.socket
z.fstat:	TCPIPLibs:sys.h.cdefs
z.fstat:	TCPIPLibs:sys.h.stat
z.fstat:	TCPIPLibs:sys.h.time
z.fstat:	TCPIPLibs:sys.h.cdefs
z.fstat:	TCPIPLibs:sys.h.cdefs
z.fstat:	TCPIPLibs:sys.h.time
z.fstat:	TCPIPLibs:sys.h.uio
z.fstat:	TCPIPLibs:sys.h.cdefs
z.fstat:	C:h.kernel
z.getdtabsiz:	c.getdtabsiz
z.getdtabsiz:	h.unixlib
z.getdtabsiz:	TCPIPLibs:h.socklib
z.getdtabsiz:	TCPIPLibs:sys.h.types
z.getdtabsiz:	TCPIPLibs:sys.h.cdefs
z.getdtabsiz:	TCPIPLibs:machine.h.endian
z.getdtabsiz:	TCPIPLibs:machine.h.ansi
z.getdtabsiz:	TCPIPLibs:machine.h.types
z.getdtabsiz:	TCPIPLibs:sys.h.socket
z.getdtabsiz:	TCPIPLibs:sys.h.cdefs
z.getdtabsiz:	TCPIPLibs:sys.h.stat
z.getdtabsiz:	TCPIPLibs:sys.h.time
z.getdtabsiz:	TCPIPLibs:sys.h.cdefs
z.getdtabsiz:	TCPIPLibs:sys.h.cdefs
z.getdtabsiz:	TCPIPLibs:sys.h.time
z.getdtabsiz:	TCPIPLibs:sys.h.uio
z.getdtabsiz:	TCPIPLibs:sys.h.cdefs
z.getdtabsiz:	C:h.kernel
z.getpwnam:	c.getpwnam
z.getpwnam:	TCPIPLibs:h.pwd
z.getpwnam:	TCPIPLibs:sys.h.types
z.getpwnam:	TCPIPLibs:sys.h.cdefs
z.getpwnam:	TCPIPLibs:machine.h.endian
z.getpwnam:	TCPIPLibs:machine.h.ansi
z.getpwnam:	TCPIPLibs:machine.h.types
z.getpwnam:	TCPIPLibs:sys.h.cdefs
z.getpwnam:	h.stubs
z.getpwnam:	h.unixlib
z.getpwuid:	c.getpwuid
z.getpwuid:	TCPIPLibs:h.pwd
z.getpwuid:	TCPIPLibs:sys.h.types
z.getpwuid:	TCPIPLibs:sys.h.cdefs
z.getpwuid:	TCPIPLibs:machine.h.endian
z.getpwuid:	TCPIPLibs:machine.h.ansi
z.getpwuid:	TCPIPLibs:machine.h.types
z.getpwuid:	TCPIPLibs:sys.h.cdefs
z.getpwuid:	h.unixlib
z.getpwuid:	h.stubs
z.gtimeofday:	c.gtimeofday
z.gtimeofday:	C:h.kernel
z.gtimeofday:	TCPIPLibs:sys.h.time
z.gtimeofday:	TCPIPLibs:sys.h.cdefs
z.gtimeofday:	TCPIPLibs:sys.h.types
z.gtimeofday:	TCPIPLibs:sys.h.cdefs
z.gtimeofday:	TCPIPLibs:machine.h.endian
z.gtimeofday:	TCPIPLibs:machine.h.ansi
z.gtimeofday:	TCPIPLibs:machine.h.types
z.gtimeofday:	h.unixlib
z.lseek:	c.lseek
z.lseek:	TCPIPLibs:h.errno
z.lseek:	TCPIPLibs:sys.h.errno
z.lseek:	C:h.kernel
z.lseek:	h.unixlib
z.read:	c.read
z.read:	h.unixlib
z.read:	TCPIPLibs:h.socklib
z.read:	TCPIPLibs:sys.h.types
z.read:	TCPIPLibs:sys.h.cdefs
z.read:	TCPIPLibs:machine.h.endian
z.read:	TCPIPLibs:machine.h.ansi
z.read:	TCPIPLibs:machine.h.types
z.read:	TCPIPLibs:sys.h.socket
z.read:	TCPIPLibs:sys.h.cdefs
z.read:	TCPIPLibs:sys.h.stat
z.read:	TCPIPLibs:sys.h.time
z.read:	TCPIPLibs:sys.h.cdefs
z.read:	TCPIPLibs:sys.h.cdefs
z.read:	TCPIPLibs:sys.h.time
z.read:	TCPIPLibs:sys.h.uio
z.read:	TCPIPLibs:sys.h.cdefs
z.read:	C:h.kernel
z.readv:	c.readv
z.readv:	TCPIPLibs:sys.h.types
z.readv:	TCPIPLibs:sys.h.cdefs
z.readv:	TCPIPLibs:machine.h.endian
z.readv:	TCPIPLibs:machine.h.ansi
z.readv:	TCPIPLibs:machine.h.types
z.readv:	TCPIPLibs:sys.h.uio
z.readv:	TCPIPLibs:sys.h.cdefs
z.readv:	h.unixlib
z.readv:	TCPIPLibs:h.socklib
z.readv:	TCPIPLibs:sys.h.types
z.readv:	TCPIPLibs:sys.h.socket
z.readv:	TCPIPLibs:sys.h.cdefs
z.readv:	TCPIPLibs:sys.h.stat
z.readv:	TCPIPLibs:sys.h.time
z.readv:	TCPIPLibs:sys.h.cdefs
z.readv:	TCPIPLibs:sys.h.cdefs
z.readv:	TCPIPLibs:sys.h.time
z.readv:	TCPIPLibs:sys.h.uio
z.readv:	C:h.kernel
z.stubs:	c.stubs
z.stubs:	TCPIPLibs:h.pwd
z.stubs:	TCPIPLibs:sys.h.types
z.stubs:	TCPIPLibs:sys.h.cdefs
z.stubs:	TCPIPLibs:machine.h.endian
z.stubs:	TCPIPLibs:machine.h.ansi
z.stubs:	TCPIPLibs:machine.h.types
z.stubs:	TCPIPLibs:sys.h.cdefs
z.stubs:	h.stubs
z.write:	c.write
z.write:	h.unixlib
z.write:	TCPIPLibs:h.socklib
z.write:	TCPIPLibs:sys.h.types
z.write:	TCPIPLibs:sys.h.cdefs
z.write:	TCPIPLibs:machine.h.endian
z.write:	TCPIPLibs:machine.h.ansi
z.write:	TCPIPLibs:machine.h.types
z.write:	TCPIPLibs:sys.h.socket
z.write:	TCPIPLibs:sys.h.cdefs
z.write:	TCPIPLibs:sys.h.stat
z.write:	TCPIPLibs:sys.h.time
z.write:	TCPIPLibs:sys.h.cdefs
z.write:	TCPIPLibs:sys.h.cdefs
z.write:	TCPIPLibs:sys.h.time
z.write:	TCPIPLibs:sys.h.uio
z.write:	TCPIPLibs:sys.h.cdefs
z.write:	C:h.kernel
z.writev:	c.writev
z.writev:	TCPIPLibs:sys.h.types
z.writev:	TCPIPLibs:sys.h.cdefs
z.writev:	TCPIPLibs:machine.h.endian
z.writev:	TCPIPLibs:machine.h.ansi
z.writev:	TCPIPLibs:machine.h.types
z.writev:	TCPIPLibs:sys.h.uio
z.writev:	TCPIPLibs:sys.h.cdefs
z.writev:	TCPIPLibs:h.socklib
z.writev:	TCPIPLibs:sys.h.types
z.writev:	TCPIPLibs:sys.h.socket
z.writev:	TCPIPLibs:sys.h.cdefs
z.writev:	TCPIPLibs:sys.h.stat
z.writev:	TCPIPLibs:sys.h.time
z.writev:	TCPIPLibs:sys.h.cdefs
z.writev:	TCPIPLibs:sys.h.cdefs
z.writev:	TCPIPLibs:sys.h.time
z.writev:	TCPIPLibs:sys.h.uio
z.writev:	C:h.kernel
z.writev:	h.unixlib
z.xperror:	c.xperror
z.xperror:	TCPIPLibs:sys.h.types
z.xperror:	TCPIPLibs:sys.h.cdefs
z.xperror:	TCPIPLibs:machine.h.endian
z.xperror:	TCPIPLibs:machine.h.ansi
z.xperror:	TCPIPLibs:machine.h.types
z.xperror:	TCPIPLibs:h.unistd
z.xperror:	TCPIPLibs:sys.h.cdefs
z.xperror:	TCPIPLibs:sys.h.types
z.xperror:	TCPIPLibs:sys.h.unistd
z.xperror:	TCPIPLibs:sys.h.errno
z.xperror:	C:h.kernel
z.xperror:	h.unixlib
z.strsep:	c.strsep
z.strsep:	TCPIPLibs:sys.h.cdefs
z.strsep:	h.unixlib
z.kvm:	c.kvm
z.kvm:	TCPIPLibs:sys.h.types
z.kvm:	TCPIPLibs:sys.h.cdefs
z.kvm:	TCPIPLibs:machine.h.endian
z.kvm:	TCPIPLibs:machine.h.ansi
z.kvm:	TCPIPLibs:machine.h.types
z.kvm:	TCPIPLibs:h.kvm
z.kvm:	TCPIPLibs:h.nlist
z.kvm:	TCPIPLibs:sys.h.cdefs
z.kvm:	TCPIPLibs:sys.h.cdefs
z.kvm:	h.unixlib
z.err:	c.err
z.err:	TCPIPLibs:h.err
z.err:	TCPIPLibs:machine.h.ansi
z.err:	TCPIPLibs:sys.h.cdefs
z.err:	h.unixlib
z.err:	TCPIPLibs:h.socklib
z.err:	TCPIPLibs:sys.h.types
z.err:	TCPIPLibs:sys.h.cdefs
z.err:	TCPIPLibs:machine.h.endian
z.err:	TCPIPLibs:machine.h.ansi
z.err:	TCPIPLibs:machine.h.types
z.err:	TCPIPLibs:sys.h.socket
z.err:	TCPIPLibs:sys.h.cdefs
z.err:	TCPIPLibs:sys.h.stat
z.err:	TCPIPLibs:sys.h.time
z.err:	TCPIPLibs:sys.h.cdefs
z.err:	TCPIPLibs:sys.h.cdefs
z.err:	TCPIPLibs:sys.h.time
z.err:	TCPIPLibs:sys.h.uio
z.err:	TCPIPLibs:sys.h.cdefs
z.err:	C:h.kernel
z.err:	TCPIPLibs:h.riscos
