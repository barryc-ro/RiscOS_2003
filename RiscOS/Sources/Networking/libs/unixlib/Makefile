# -*-Text-*-
#
# $Header$
# $Source$
#
# Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
#
# Makefile for unixlib.a
#
# $Log$
# Revision 1.1  95/04/20  09:50:39  kwelton
# Initial revision
# 
#
LEVEL=		../..
INCLUDES=	INetInc:,C:

CC=		cc
AS=		objasm
LD=		link
LIB=		libfile

CMHG=		cmhg

CFLAGS=		-depend !Depend -pcc -zps0 -I${INCLUDES} -fah -Fn -DINET
CFLAGSZM=	-depend !Depend -pcc -zps1 -I${INCLUDES} -ffah -DINET -zM

CCZM=		${CC} ${CFLAGSZM}

RMFLAGS=	-f
RM=		rm ${RMFLAGS}

CFILES=		access.c chdir.c chmod.c close.c filestat.c flushinput.c \
		fstat.c getdtabsiz.c getegid.c geteuid.c getgroups.c \
		gethostnam.c getlogin.c getpass.c getpid.c getpwent.c \
		getpwnam.c getpwuid.c getuid.c getusernam.c getwd.c \
		gtimeofday.c herror.c index.c ioctl.c killfile.c lseek.c \
		memops.c osreadc.c read.c readdir.c readv.c string.c \
		stubs.c write.c writev.c xgets.c xperror.c xputchar.c

OFILES=		access.o chdir.o chmod.o close.o filestat.o flushinput.o \
		fstat.o getdtabsiz.o getegid.o geteuid.o getgroups.o \
		gethostnam.o getlogin.o getpass.o getpid.o getpwent.o \
		getpwnam.o getpwuid.o getuid.o getusernam.o getwd.o \
		gtimeofday.o herror.o index.o ioctl.o killfile.o lseek.o \
		memops.o osreadc.o read.o readdir.o readv.o string.o \
		stubs.o write.o writev.o xgets.o xperror.o xputchar.o

ZFILES=		z.access z.chdir z.chmod z.close z.filestat z.flushinput \
		z.fstat z.getdtabsiz z.getegid z.geteuid z.getgroups \
		z.gethostnam z.getlogin z.getpass z.getpid z.getpwent \
		z.getpwnam z.getpwuid z.getuid z.getusernam z.getwd \
		z.gtimeofday z.herror z.index z.ioctl z.killfile z.lseek \
		z.memops z.osreadc z.read z.readdir z.readv z.string \
		z.stubs z.write z.writev z.xgets z.xperror z.xputchar

.SUFFIXES:	.z .o .c .s

.c.z:;	     ${CCZM} -c -o $@ $<
.c.o:;       ${CC} ${CFLAGS} -c -o $@ $<

all:	unixlib unixlibzm 

unixlib:	${OFILES} Makefile
	${LIB} -c unixlib ${OFILES}
	${LIB} -o unixlib

unixlibzm:	${ZFILES} Makefile
	${LIB} -c unixlibzm ${ZFILES}
	${LIB} -o unixlibzm

clean:
	-Wipe o.* ~C~V
	-Wipe z.* ~C~V
	Remove unixlib
	Remove unixlibzm


# Dynamic dependencies:
o.access:	c.access
o.access:	INetInc:h.errno
o.access:	C:h.kernel
o.access:	C:h.kernel
o.access:	C:h.swis
o.access:	INetInc:sys.h.fcntl
o.access:	h.unixlib
o.chdir:	c.chdir
o.chdir:	C:h.kernel
o.chdir:	C:h.swis
o.chdir:	h.unixlib
o.chmod:	c.chmod
o.chmod:	INetInc:h.errno
o.chmod:	C:h.kernel
o.chmod:	C:h.kernel
o.chmod:	C:h.swis
o.chmod:	h.unixlib
o.close:	c.close
o.close:	h.unixlib
o.filestat:	c.filestat
o.filestat:	INetInc:h.errno
o.filestat:	C:h.kernel
o.filestat:	C:h.kernel
o.filestat:	C:h.swis
o.filestat:	h.unixlib
o.flushinput:	c.flushinput
o.flushinput:	C:h.kernel
o.flushinput:	C:h.swis
o.flushinput:	h.unixlib
o.fstat:	c.fstat
o.fstat:	h.unixlib
o.getdtabsiz:	c.getdtabsiz
o.getdtabsiz:	h.unixlib
o.getegid:	c.getegid
o.getegid:	h.unixlib
o.getegid:	h.stubs
o.geteuid:	c.geteuid
o.geteuid:	h.unixlib
o.geteuid:	h.stubs
o.getgroups:	c.getgroups
o.getgroups:	h.unixlib
o.getgroups:	h.stubs
o.gethostnam:	c.gethostnam
o.gethostnam:	C:h.kernel
o.gethostnam:	C:h.swis
o.gethostnam:	h.unixlib
o.gethostnam:	h.stubs
o.getlogin:	c.getlogin
o.getlogin:	h.unixlib
o.getpass:	c.getpass
o.getpass:	h.unixlib
o.getpid:	c.getpid
o.getpid:	h.unixlib
o.getpid:	h.stubs
o.getpwent:	c.getpwent
o.getpwent:	h.unixlib
o.getpwnam:	c.getpwnam
o.getpwnam:	INetInc:h.pwd
o.getpwnam:	h.stubs
o.getpwnam:	h.unixlib
o.getpwuid:	c.getpwuid
o.getpwuid:	INetInc:h.pwd
o.getpwuid:	h.unixlib
o.getpwuid:	h.stubs
o.getuid:	c.getuid
o.getuid:	h.unixlib
o.getuid:	h.stubs
o.getusernam:	c.getusernam
o.getusernam:	C:h.kernel
o.getusernam:	C:h.swis
o.getusernam:	h.unixlib
o.getusernam:	h.stubs
o.getwd:	c.getwd
o.getwd:	C:h.kernel
o.getwd:	C:h.swis
o.getwd:	h.unixlib
o.gtimeofday:	c.gtimeofday
o.gtimeofday:	C:h.kernel
o.gtimeofday:	INetInc:sys.h.time
o.gtimeofday:	INetInc:sys.h.types
o.gtimeofday:	h.unixlib
o.herror:	c.herror
o.herror:	h.unixlib
o.index:	c.index
o.index:	h.unixlib
o.ioctl:	c.ioctl
o.ioctl:	h.unixlib
o.killfile:	c.killfile
o.killfile:	C:h.kernel
o.killfile:	C:h.swis
o.killfile:	h.unixlib
o.lseek:	c.lseek
o.lseek:	INetInc:h.errno
o.lseek:	C:h.kernel
o.lseek:	h.unixlib
o.memops:	c.memops
o.memops:	h.unixlib
o.osreadc:	c.osreadc
o.osreadc:	C:h.kernel
o.osreadc:	C:h.swis
o.osreadc:	h.unixlib
o.read:	c.read
o.read:	h.unixlib
o.readdir:	c.readdir
o.readdir:	C:h.kernel
o.readdir:	C:h.swis
o.readdir:	h.unixlib
o.readv:	c.readv
o.readv:	INetInc:sys.h.types
o.readv:	INetInc:sys.h.uio
o.readv:	h.unixlib
o.string:	c.string
o.string:	h.unixlib
o.stubs:	c.stubs
o.stubs:	INetInc:h.pwd
o.stubs:	h.stubs
o.write:	c.write
o.write:	h.unixlib
o.writev:	c.writev
o.writev:	INetInc:sys.h.types
o.writev:	INetInc:sys.h.uio
o.writev:	h.unixlib
o.xgets:	c.xgets
o.xgets:	h.unixlib
o.xperror:	c.xperror
o.xperror:	INetInc:h.errno
o.xperror:	C:h.kernel
o.xperror:	h.unixlib
o.xputchar:	c.xputchar
o.xputchar:	C:h.kernel
o.xputchar:	C:h.swis
o.xputchar:	h.unixlib
z.access:	c.access
z.access:	INetInc:h.errno
z.access:	C:h.kernel
z.access:	C:h.kernel
z.access:	C:h.swis
z.access:	INetInc:sys.h.fcntl
z.access:	h.unixlib
z.chdir:	c.chdir
z.chdir:	C:h.kernel
z.chdir:	C:h.swis
z.chdir:	h.unixlib
z.chmod:	c.chmod
z.chmod:	INetInc:h.errno
z.chmod:	C:h.kernel
z.chmod:	C:h.kernel
z.chmod:	C:h.swis
z.chmod:	h.unixlib
z.close:	c.close
z.close:	h.unixlib
z.filestat:	c.filestat
z.filestat:	INetInc:h.errno
z.filestat:	C:h.kernel
z.filestat:	C:h.kernel
z.filestat:	C:h.swis
z.filestat:	h.unixlib
z.flushinput:	c.flushinput
z.flushinput:	C:h.kernel
z.flushinput:	C:h.swis
z.flushinput:	h.unixlib
z.fstat:	c.fstat
z.fstat:	h.unixlib
z.getdtabsiz:	c.getdtabsiz
z.getdtabsiz:	h.unixlib
z.getegid:	c.getegid
z.getegid:	h.unixlib
z.getegid:	h.stubs
z.geteuid:	c.geteuid
z.geteuid:	h.unixlib
z.geteuid:	h.stubs
z.getgroups:	c.getgroups
z.getgroups:	h.unixlib
z.getgroups:	h.stubs
z.gethostnam:	c.gethostnam
z.gethostnam:	C:h.kernel
z.gethostnam:	C:h.swis
z.gethostnam:	h.unixlib
z.gethostnam:	h.stubs
z.getlogin:	c.getlogin
z.getlogin:	h.unixlib
z.getpass:	c.getpass
z.getpass:	h.unixlib
z.getpid:	c.getpid
z.getpid:	h.unixlib
z.getpid:	h.stubs
z.getpwent:	c.getpwent
z.getpwent:	h.unixlib
z.getpwnam:	c.getpwnam
z.getpwnam:	INetInc:h.pwd
z.getpwnam:	h.stubs
z.getpwnam:	h.unixlib
z.getpwuid:	c.getpwuid
z.getpwuid:	INetInc:h.pwd
z.getpwuid:	h.unixlib
z.getpwuid:	h.stubs
z.getuid:	c.getuid
z.getuid:	h.unixlib
z.getuid:	h.stubs
z.getusernam:	c.getusernam
z.getusernam:	C:h.kernel
z.getusernam:	C:h.swis
z.getusernam:	h.unixlib
z.getusernam:	h.stubs
z.getwd:	c.getwd
z.getwd:	C:h.kernel
z.getwd:	C:h.swis
z.getwd:	h.unixlib
z.gtimeofday:	c.gtimeofday
z.gtimeofday:	C:h.kernel
z.gtimeofday:	INetInc:sys.h.time
z.gtimeofday:	INetInc:sys.h.types
z.gtimeofday:	h.unixlib
z.herror:	c.herror
z.herror:	h.unixlib
z.index:	c.index
z.index:	h.unixlib
z.ioctl:	c.ioctl
z.ioctl:	h.unixlib
z.killfile:	c.killfile
z.killfile:	C:h.kernel
z.killfile:	C:h.swis
z.killfile:	h.unixlib
z.lseek:	c.lseek
z.lseek:	INetInc:h.errno
z.lseek:	C:h.kernel
z.lseek:	h.unixlib
z.memops:	c.memops
z.memops:	h.unixlib
z.osreadc:	c.osreadc
z.osreadc:	C:h.kernel
z.osreadc:	C:h.swis
z.osreadc:	h.unixlib
z.read:	c.read
z.read:	h.unixlib
z.readdir:	c.readdir
z.readdir:	C:h.kernel
z.readdir:	C:h.swis
z.readdir:	h.unixlib
z.readv:	c.readv
z.readv:	INetInc:sys.h.types
z.readv:	INetInc:sys.h.uio
z.readv:	h.unixlib
z.string:	c.string
z.string:	h.unixlib
z.stubs:	c.stubs
z.stubs:	INetInc:h.pwd
z.stubs:	h.stubs
z.write:	c.write
z.write:	h.unixlib
z.writev:	c.writev
z.writev:	INetInc:sys.h.types
z.writev:	INetInc:sys.h.uio
z.writev:	h.unixlib
z.xgets:	c.xgets
z.xgets:	h.unixlib
z.xperror:	c.xperror
z.xperror:	INetInc:h.errno
z.xperror:	C:h.kernel
z.xperror:	h.unixlib
z.xputchar:	c.xputchar
z.xputchar:	C:h.kernel
z.xputchar:	C:h.swis
z.xputchar:	h.unixlib
