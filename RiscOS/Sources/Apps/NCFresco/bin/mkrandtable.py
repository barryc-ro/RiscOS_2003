#!/usr/local/bin/python
"""Meta example of difficult HTML tables.

(C) Copyright ANT Limited 1997. All rights reserved.

Usage: %s [options]

--count N	Perform N iterations. Default is 1.
--spawn	P	Spawn program P on each table generated.
		Default is just to print the table.

"""

import os, sys

from random import *
from whrandom import *

list123 = [ 1,2,3 ]
list1234 = [ 1,2,3,4 ]

rowspan_list = [ '', '', ' rowspan=0', ' rowspan=2', ' rowspan=3', ' rowspan=4' ]
colspan_list = [ '', '', ' colspan=0', ' colspan=2', ' colspan=3', ' colspan=4' ]

def usage():
	print __doc__ % sys.argv[0]
	sys.exit(1)

def generate_random_width():
	p = choice(list1234)
	if p == 1 :
		return ' width=%d' % (random() * 200 + random() * 200 + random() * 200)
	elif p == 2:
		return ' width=%d%%' % (random() * 111)
	else:
		return ''

def generate_random_rowspan():
	return choice(rowspan_list)

def generate_random_colspan():
	return choice(colspan_list)

def generate_random_table_attributes():
	return generate_random_width()

def generate_random_cell_attributes():
	return generate_random_rowspan() + generate_random_colspan() + generate_random_width()

def generate_random_row_attributes():
	return ''

def generate_random_cell(depth):
	depth = depth + 2
	print depth * ' ',
	print '<TD%s> .... .... ........' % generate_random_cell_attributes()
	if random() < 0.1:
		generate_random_table(depth)

def generate_random_row(depth):
	depth = depth + 2
	print depth * ' ',
	print '<TR%s>' % generate_random_row_attributes()
	for i in xrange(choice(list1234)):
		generate_random_cell(depth)

def generate_random_table(depth):
	if depth != 0: depth = depth + 2
	print depth * ' ',
	print '<TABLE%s>' % generate_random_table_attributes()
	for i in xrange(choice(list123)):
		generate_random_row(depth)
	print depth * ' ',
	print '</TABLE>'

if __name__ == "__main__":
	generate_random_table(0)

# EOF
