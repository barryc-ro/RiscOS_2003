#!/usr/local/bin/python
"""Sort out things in a directory tree that I find useful

--unix          Set links from RISCOS c,h,s files to unix positioned files.

--path <path>	Specify the directory tree to operate on. Defaults to current
		directory.

(C) ANT Limited 1996. All rights reserved. Version 1.0. Support, if available,
via borris@ant.co.uk.
"""

import os, sys, string, regex, regsub, james
from stat import *

# st_mode, st_ino, st_dev, st_nlink, st_uid, st_gid, st_size, st_atime, st_mtime, st_ctime
# 0        1       2       3         4       5       6        7         8         9

#############################################################################

def parse_args(argv):
	mode, path = 'help', ''
	while len(argv) > 0:
		a, argv = argv[0], argv[1:]
		if a == '--help':
			mode = 'help'
		elif a == '--unix':
			mode = 'unix'
		else:
			path = a
	if path == '':
		print __doc__
		print 'A path must be supplied.'
		sys.exit(1)
	return mode, path

#############################################################################


def do_riscos_file(path, file):
	ext, root = file[-1], file[:-2]
	z = os.path.join( path, ext)
	if os.path.exists(z):
		if not os.path.isdir(z):
			print z, "exists but isn't a directory - skipping."
			return
	else:
		os.mkdir(z, 0750)
	src = os.path.join( os.path.join( path, ext ), root )
#	dst = os.path.join( path, root ) + '.' + ext
	dst = os.path.join( os.pardir, root ) + '.' + ext
	if os.path.islink(src):
		print src, 'is a soft link aleady - updating.'
		os.unlink(src)
	if os.path.exists(src):
		print dst, 'for', src, 'already exists - skipping.'
		return
	print 'symlink', dst, src
	os.symlink(dst, src)
	return


# Current directory is one to "bump up", eg wibble/c
def do_riscos_dir(path):
	done = 0
	f,d,o = james.list_fdo(path)
	for file in f:
		if len(file) < 3:
			continue
		if file[-2] != '.':
			continue
		l = file[-1]
		if l != 's' and l != 'c' and l != 'h':
			continue
		if not done:
			print 'Creating RISC OS soft links for', path
			done = 1
		do_riscos_file(path, file)
	for dir in d:
		do_riscos_dir( os.path.join(path, dir) )
	return


# dir is a source-like directory requiring files moving
# out of into into path with .dir as their extension.
def do_riscos_shove(path, dir):
	lower = os.path.join(path, dir)
	f,d,l,o = james.list_fdlo(lower)
	for file in f:
		if file[-1] == '~':
			dest = os.path.join( path, file[:-1] + '.' + dir + '~' )
		else:
			dest = os.path.join( path, file + '.' + dir )
		src = os.path.join(lower, file)
		if os.path.islink(dest):
			continue
		if os.path.exists(dest):
			print "Can't link", src
			print 'to', dest, 'as it already exists - skipping.'
			continue
		#print 'Linking', src, 'to', dest
		os.symlink(src, dest)
	return

riscos_xlat_list = [ 'c', 'h', 's' ]

def do_riscos_munge(path):
	done = 0
	f,d,l,o = james.list_fdlo(path)
	for dir in d:
		if dir in riscos_xlat_list:
			if not done:
				print 'Making softlinks for ', path
				done = 1
			do_riscos_shove(path, dir)
		else:
			do_riscos_munge( os.path.join(path, dir) )
	return


def do_riscos(path):
	do_riscos_munge(path)
	return

#############################################################################

def main(argv):
	mode, path = parse_args(argv[1:])
	if path == os.curdir:
		path = os.getcwd()
	if mode == 'help':
		print __doc__
		sys.exit(1)
	sl = os.lstat(path)
	if not S_ISDIR( sl[0] ):
		print __doc__
		print path, 'is not a directory'
		sys.exit(1)
	do_riscos(path)


if __name__ == "__main__":
	main(sys.argv)

# eof sortout.py
