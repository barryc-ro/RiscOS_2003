#!/usr/local/bin/python
"""Scan CVS controlled directory, flagged potential problems.

(C) ANT Limited 1997. All rights reserved.

Author: borris@ant.co.uk
Version: 18th February 1997

When using CVS, RISC OS, Linux and NFS, problems can easily occur with
files called blah and blah,ffd. This can create immense problems if
not caught and properly handled. This utility scans a directory tree
under CVS control and reports on such clashes.

Usage:
	%s [options] [directories]

Available options:

-d			Run diff on files that clash
--diff			Run diff on files that clash
-v			Verbose messages
--verbose		Verbose messages

"""

import os, sys, string, regex

IGN1 = [ '.', '..', 'CVS' ]
verbose = 0
diff = 0
rc = 0

#-----------------------------------------------------------------------------

def usage():
	print __doc__ % sys.argv[0]
	sys.exit(2)

#-----------------------------------------------------------------------------





# >>> print string.splitfields('/.cvsignore/1.1.1.1/Wed Feb  5 14:26:51 1997//', '/')
# ['', '.cvsignore', '1.1.1.1', 'Wed Feb  5 14:26:51 1997', '', '']
# Giving dictionary, keyed on name, value being [rev,date] tuple

def get_cvs_entries_dictionary(dirname):
	dict = {}	
	try:
		fobj = open( os.path.join( os.path.join(dirname, 'CVS'), 'Entries') )
		for whole in fobj.readlines():
			while whole and whole[-1] in string.whitespace:
				whole = whole[:-1]
			#print '"%s"' % whole
			if whole != 'D':
				[d1, name, rev, date, d2, d3] = string.splitfields(whole, '/')
				dict[name] = [ rev, date ]
		fobj.close()
	except IOError:
		pass

	return dict


def check_directory(dirname, files):
	global rc

	entries = get_cvs_entries_dictionary(dirname)

	#print 'ENTRIES', entries

	for file in files:
		tuple = string.splitfields(file, ',')
		if len(tuple) == 1:
			continue
		f1, f2 = os.path.join(dirname, file), os.path.join(dirname, tuple[0])
		if os.path.exists( f2 ) and not os.path.islink(f2):
			print f1, 'CLASHES WITH', f2
			z = 0
			if entries.has_key(file):
				if verbose: print f1, 'IS UNDER CVS CONTROL:', entries[file]
				z = z + 1
			if entries.has_key(tuple[0]):
				if verbose: print f2, 'IS UNDER CVS CONTROL:', entries[tuple[0]]
				z = z + 1
			if z == 2:
				print 'DOUBLE WARNING - BOTH FILES ARE ALREADY UNDER CVS CONTROL'
				print 'THIS MUST BE RESOLVED IMMEDIATELY OR THINGS WILL GO WRONG.'
			if diff:
				# ignore blank lines, report identical, try hard
				cmd = 'diff -Bds %s %s' % (f1, f2)
				print cmd
				os.system(cmd)
			rc = 1
			print
	return

def recurse_on_directory(dirname):

	files, dirs = [], []

	for leaf in os.listdir( dirname ):
		full = os.path.join( dirname, leaf )
		if os.path.islink( full ):
			pass
		elif os.path.isfile( full ):
			if leaf == '.cvsignore': continue
			files.append( leaf )
		elif os.path.isdir( full ):
			if leaf == '.' or leaf == '..' or leaf == 'CVS': continue
			dirs.append( full )

	check_directory(dirname, files)

	for dir in dirs:
		recurse_on_directory(dir)

	return

#-----------------------------------------------------------------------------

def main():
	global verbose, diff

	total_warnings = 0;
	cwd = os.getcwd()
	pdl = []

	for a in sys.argv[1:]:
		if a[0] == '-':
			if a == '-v' or a == '--verbose':
				verbose = 1
			elif a == '-d' or a == '--diff':
				diff = 1
			else:	
				usage()
			continue

		pdl.append( os.path.join(cwd, a) )

	if len(pdl) == 0:
		pdl = [ cwd ]

	dirlist = []
	for pd in pdl:
		if os.path.isdir(pd):
			if os.path.basename(pd) not in IGN1:
				dirlist.append(pd)
			else:
				if verbose: print pd, 'being skipped'
		else:
			if verbose: print pd, 'is not a directory - ignoring'

	for dir in dirlist:
		recurse_on_directory(dir)

	if verbose: print 'Finished'
	sys.exit(rc)

if __name__ == '__main__':
	main()

# EOF
