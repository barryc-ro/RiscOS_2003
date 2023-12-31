#!/usr/local/bin/python
"""
        Generate C and ALGOL definitions from a single,	easy to edit
        configuration file.

        This is all based around initialising some dictionaries with all the 
        necessary information, and then this program generates various 
        reports based upon these dictionaries. As these are dictionaries,
        there is no need to order entries alphabetically.

        Attributes dictionary: the key is the internal attribute name and the value is a 
        tuple of three items. The first is the attribute name to match, 
        then the templates list and the finally is the parsing method.

        Elements dictionary: the key is the element name and the value is a tuple of
        4 items. The group of the element, the flags of the element, the 
        attributes list and the requisites lists of tuples.

	"-" (hypen) is a string is translated to "_" (underscore) whenever a string
	is used to derive a symbol - eg string enumeration values and #define's

	Enumerations start from one, rather than zero, so that zero can
	always be used to represent an item not present in the enumeration
	for compact representation of presence/absence and the enumeration value.


	Usage: attrgen.py <sgml> <tables> <defs> <decls> [procs]

	sgml: the definitions file in the wacky format
	tables: the C source file that holds the table defintions
	defs: the C header file holding only #defines
	decls: the C header file declaring the expected external functions
	procs: optional C source file that serves as a bootstrapping aid.

	If you get key errors, it probably means you're using a name
	you haven't defined, such as referencing an attribute that
	has not been declared.

"""

#############################################################################

copyright = '(C) Copyright ANT Limited 1996, 1997. All rights reserved.'

#############################################################################

import os, sys, string, regex, regsub

tables_file, defs_file, decls_file, procs_file = None, None, None, None
tables_name, defs_name, decls_name, procs_name = None, None, None, None

Oops = 'Oops'
curr_char = ""
curr_file = None
curr_line = ""
line_ix = 0
curr_line_number = 1
ch_lf = chr(10)
ch_cr = chr(13)
indent = 0
emitting = 0
cmd_char = '$'

string_dict = {}                # key=string, value=index into global string table
string_index = 0
templates_dict = {}             # key=templates string, value=index into templates table
templates_index = 0
attributes_dict = {}            # key=attributes string, value=index into attributes table
attributes_index = 0
requirements_dict = {}          # key=requirements string, value=index into requirements table
requirements_index = 0
attrlist_dict = {}              # key=single attribute name, value=index into attributes_list
attrlist_index = 0
elements_dict = {}              # key=element name, value=numeric id for element
enumvalues_dict = {}
userattr_dict = {}		# key=program attribute name, value=user's version

#-----------------------------------------------------------------------------

# Parse a textual file into tuple lists for internal manipulation

def raw_next_char():
        global curr_char, curr_file, curr_line, curr_line_number, line_ix

        if curr_char != "":
                x = curr_char
                curr_char = ""
                return x

        if line_ix < len(curr_line):
                x = curr_line[line_ix]
                line_ix = line_ix + 1
                return x

        if curr_file != None:
                while 1:
                        curr_line_number = curr_line_number + 1
                        curr_line = curr_file.readline()
                        line_ix = 0
                        if curr_line == "":
                                raise Oops, 'Unexpected end of file'
			if curr_line[0] == cmd_char:
				# Magic actions. Sorry!
				if curr_line[1] == 'E':
					print curr_line[2:]
				else:
					print 'Warning: unknown magic:', curr_line
			elif curr_line[0] != '#':
                                break
                return raw_next_char()
        raise Oops, 'No active file'

def next_char():
        ch = raw_next_char()
        if ch == ch_lf or ch == ch_cr:
                ch = ' '
        return ch

def peek_char():
        global curr_char
        if curr_char == "":
                curr_char = next_char()
        return curr_char        

def skip_space():
	while 1:
		x = peek_char()
		if x == ' ' or x == '\t':
			x = next_char()
		else:
			break

def expect(ch):
        skip_space()
        if next_char() != ch:
                raise Oops, 'Expected %c at line %d' % (ch, curr_line_number)

def get_simple():
        tok = ""
        while peek_char() not in ',.()':
                tok = tok + next_char()
	tok = string.strip(tok);
	if len(tok) == 0:
		raise Oops, "Simple word token expected at line %d" % curr_line_number
        return tok

space_pattern = regex.compile('[%c%c%c]' % (32, 124, 9))

def get_simple_tidy():
        tok = ""
        while peek_char() not in ',.()':
		tok = tok + next_char()
	tok = regsub.gsub(space_pattern, '', tok)
	if len(tok) == 0:
		raise Oops, "Simple word token expected at line %d" % curr_line_number
	return string.upper(tok)
	
def get_simple_tidy_uncased():
        tok = ""
        while peek_char() not in ',.()':
		tok = tok + next_char()
	tok = regsub.gsub(space_pattern, '', tok)
	if len(tok) == 0:
		raise Oops, "Simple word token expected at line %d" % curr_line_number
	# Mixed case support for LI enumeration list :-(
	if tok != string.upper(tok):
		print 'Warning: non-upper-case enumeration at line %d: %s' % (curr_line_number, tok)
	return tok
	
def get_simple_tidy_():
        tok = ""
        while peek_char() not in ',.()':
		tok = tok + next_char()
	tok = string.upper(string.strip(tok))
	if len(tok) == 0:
		raise Oops, "Simple word token expected at line %d" % curr_line_number
	return regsub.gsub(space_pattern, '_', tok)

def get_list_tidy():
        list = []
        expect('(')
        skip_space()
        if peek_char() != ')':
                while 1:
                        list.append( get_simple_tidy() )
                        skip_space()
                        if peek_char() != ',':
                                break
                        expect(',')
        expect(')')
        return list

def get_list_tidy_uncased():
        list = []
        expect('(')
        skip_space()
        if peek_char() != ')':
                while 1:
                        list.append( get_simple_tidy_uncased() )
                        skip_space()
                        if peek_char() != ',':
                                break
                        expect(',')
        expect(')')
        return list

def get_list_tidy_():
        list = []
        expect('(')
        skip_space()
        if peek_char() != ')':
                while 1:
                        list.append( get_simple_tidy_() )
                        skip_space()
                        if peek_char() != ',':
                                break
                        expect(',')
        expect(')')
        return list

def get_list_of_lists_tidy_():
        list = []
        expect('(')
        skip_space()
        if peek_char() != ')':
                while 1:
                        list.append( get_list_tidy_() )
                        skip_space()
                        if peek_char() != ',':
                                break
                        expect(',')
        expect(')')
        return list


def parse_attribute():
        expect('(')
        internal_name = get_simple_tidy()
        expect(',')
        textual_name = get_simple_tidy()
        expect(',')
        templates_list = get_list_tidy_uncased()
	templates_list.sort()
        expect(',')
        parse_method = get_simple_tidy_()
        expect(')')
        return ( internal_name, textual_name, templates_list, parse_method )

def parse_element():
        expect('(')
        textual_name = get_simple_tidy()
        expect(',')
        group = get_simple_tidy()
        expect(',')
        flags_list = get_list_tidy_()
        expect(',')
        attributes_list = get_list_tidy()
	attributes_list.sort()
        expect(',')
        requisites_list = get_list_of_lists_tidy_()
        expect(')')
        return ( textual_name, group, flags_list, attributes_list, requisites_list )


def parse_file(fname):
        global curr_line_number, curr_file
        curr_file = open(fname, "r")
        curr_line_number = 1
        attributes, elements = [], []
        skip_space()
        if get_simple() != "ATTRIBUTES":
                raise Oops, 'Expecting "ATTRIBUTES" section'
        expect('(')
        while 1:
                attributes.append( parse_attribute() )
                skip_space()
                if peek_char() != ',':
                        break
                expect(',')
        expect(')')
        skip_space()
        if get_simple() != "ELEMENTS":
                raise Oops, 'Expecting "ELEMENTS" section'
        expect('(')
        while 1:
                elements.append( parse_element() )
                skip_space()
                if peek_char() != ',':
                        break
                expect(',')
        expect(')')
        curr_file.close()
        return attributes, elements


#-----------------------------------------------------------------------------


def tidy_elem(list):
	tidy(list[0]), tidy(list[1]), 

def flat1(list):
	return string.joinfields(list, chr(1))

def flat2(list):
	r = []
	for outer in list:
		r.append( string.joinfields(outer, chr(2)) )
	return string.joinfields(r, chr(1))

def unflat1(str):
	return string.splitfields(str, chr(1))

def unflat2(str):
	list = unflat1(str)
	r = []
	for outer in list:
		r.append( string.splitfields(outer, chr(2)) )
	return r

def flat_elem(list):
	return flat1( (list[0], list[1], flat2(list[2]), flat2(list[3]), flat2(list[4])) )

def flat_attr(list):
	return flat1( (list[0], list[1], flat2(list[2]), list[3]) )

def unflat_elem(elem):
	list = unflat1(elem)
	return ( (list[0], list[1], unflat2(list[2]), unflat2(list[3]), unflat2(list[4])) )

def unflat_attr(attr):
	list = unflat1(attr)
	return ( (list[0], list[1], unflat2(list[2]), list[3]) )


#-----------------------------------------------------------------------------

# Emitters

def emit_tables(str):
	tables_file.write( indent * ' ' + str + ch_lf )

def emit_defs(str):
	defs_file.write( indent * ' ' + str + ch_lf )

def emit_decls(str):
	decls_file.write( indent * ' ' + str + ch_lf )

def emit_procs(str):
	if procs_file != None:
		procs_file.write( indent * ' ' + str + ch_lf )

#-----------------------------------------------------------------------------

def kv1_cmp(a, b):
        if a[1] < b[1]:
                return -1
        if a[1] > b[1]:
                return 1
        return 0

def emit_string_table():
        global string_dict, indent
        kv = string_dict.items()
        kv.sort( kv1_cmp )
        emit_tables( '' )
        emit_tables( '/* Global shared string table */' )
        emit_tables( '' )
        emit_tables( 'static char *string_table[%d] =' % len(kv) )
        emit_tables( '{' )
        indent = indent + 4;
        count, post = 0, ','
        for key, value in kv:
                if count == len(kv) - 1:        post = ''
                emit_tables( '/* %3d */ "%s"%s' % (count, key, post) )
#                emit_tables( 'static char* string%d = "%s";' % (count, key) )
                count = count + 1

        indent = indent - 4
        emit_tables( '};' )
        return

def emit_element_numbers():
	global elements_dict
	r = {}
	for k,v in elements_dict.items():
		r[v] = k
        emit_defs( '' )
        emit_defs( '/* One #define for each element */' )
        emit_defs( '' )
	for i in xrange(len(r.keys())):
                emit_defs( '#define HTML_%s %d' % (r[i], i) )
        emit_defs( '' )

def emit_templates_table():
        global templates_dict, indent
        emit_tables( '' )
        emit_tables( '/* Shared templates table */' )
        emit_tables( '' )
        emit_tables( 'static STRING templates_table[] =' )
        emit_tables( '{' )
        indent = indent + 4;

	x = templates_dict.items()
	x.sort( kv1_cmp )
	id = 0

	for item in x:
		list = unflat1(item[0])
                for word in list:
                        if len(word) > 0:
                                emit_tables( '/* %3d */ { %s, %d }, /* %s */' % (id, string_for(word), len(word), word))
                                id = id + 1
                emit_tables( '          { (char *) -1, 0 },' )
                id = id + 1
        indent = indent - 4
        emit_tables( '};' )

	return

        id = 0
        for realkey in templates_dict.keys():
                templates_dict[realkey] = id
                key = string.splitfields(realkey, chr(0))
                for word in key:
                        if len(word) > 0:
                                emit_tables( '/* %3d */ { %s, %d }, /* %s */' % (id, string_for(word), len(word), word) )
                                id = id + 1
                emit_tables( '          { (char *) -1, 0 },' )
                id = id + 1

def emit_attributes_table():
        global attributes_dict, indent
        emit_tables( '' )
        emit_tables( '/* Sequences of attributes possessed by elements. Shared. */' )
        emit_tables( '' )
        emit_tables( 'static ATTRIBUTE *attributes_table[] =' )
        emit_tables( '{' )
        indent = indent + 4

	list = attributes_dict.items()
	list.sort( kv1_cmp )

	id = 0
	for item in list:
		outer = unflat1(item[0])
		for word in outer:
			if len(word) == 0: continue
			emit_tables( '/* %3d */ (ATTRIBUTE *) %d, /* %s */' % (id, attrlist_dict[word], word) )
			id = id + 1
                emit_tables( '          (ATTRIBUTE *) %d,' % attrlist_dict['terminating_attribute'] )
                id = id + 1

        indent = indent - 4
        emit_tables( '};' )
	return

def emit_requirements_table():
        global requirements_dict, indent
        emit_tables( '' )
        emit_tables( '/* Global requirements table. Shared where possible */' )
        emit_tables( '' )
        emit_tables( 'static ACT_ELEM requirements_table[] =' )
        emit_tables( '{' )
        indent = indent + 4;
	list = requirements_dict.items()
	list.sort( kv1_cmp )
	id = 0
	for x in list:
		for req in unflat2(x[0]):
			if len(req) != 2: continue
			emit_tables( '/* %3d */ {%s, HTML_%s},' % (id, req[0], req[1]) )
			id = id + 1
		emit_tables( '          {0, SGML_NO_ELEMENT},' )
		id = id + 1
##        id = 0
##        for realkey in requirements_dict.keys():
##                requirements_dict[realkey] = id
##                key1 = string.splitfields(realkey, chr(0))
##                for key in key1:
##                        if len(key) == 0: continue
##                        x = string.splitfields(key, chr(1))
##                        x0, x1 = tidy_name(x[0]), tidy_name(x[1])
##                        x1 = 'HTML_' + string.upper(string.splitfields(x1, '_')[-1])
##                        emit_tables( '/* %3d */ {%s, %s},' % (id, x0, x1) )
##                        id = id + 1
##                emit_tables( '          {0, html_no_element},' )
##                id = id + 1
        indent = indent - 4
        emit_tables( '};' )

def templates_for(list):
        global templates_dict, templates_index
	str = flat1(list)
        return '(STRING *) %d' % templates_dict[str]

def string_for(str):
        global emitting, string_dict, string_index
        if not emitting:
                if not string_dict.has_key(str):
                        string_dict[str] = string_index
                        string_index = string_index + 1
        return '(char *) %d' % string_dict[str]
        return '"' + str + '"'

def attributes_for(list):
        global attributes_dict, attributes_index
	str = flat1(list)
        return '(ATTRIBUTE **) %d' % attributes_dict[str]

def requirements_for(list2):
        global emitting, requirements_dict, requirements_index
	str = flat2(list2)
        return '(ACT_ELEM *) %d' % requirements_dict[str]
        return '&requirements_table[%d]' % requirements_dict[str]

#-----------------------------------------------------------------------------


def emit_attributes(list):
        global indent, attrlist_dict, attrlist_index, attributes_dict, userattr_dict
        emit_tables( '' )
        emit_tables( '/* The attributes themselves */' )
        emit_tables( '' )
        emit_tables( 'static ATTRIBUTE attributes_list[%d] =' % (len(list)+1) )
        emit_tables( '{' )
        indent = indent + 4
        count, attrlist_index = 0, 0
	for internal, textual, templates, parse in list:
                attrlist_dict[internal] = attrlist_index
                attrlist_index = attrlist_index + 1
		userattr_dict[internal] = textual
                name_len = len(textual)
                name = string_for(textual)
                templates = templates_for(templates)
#                emit_tables( 'ATTRIBUTE %s = { {%s,%d}, %s, %s };' % (internal, name, name_len, templates, parse) )
                emit_tables( '/* %3d */ { {%s,%d}, %s, %s }, /* %s */' % (count, name, name_len, templates, parse, internal) )
                count = count + 1
        emit_tables( '{ {(char *) -1,0}, (STRING*)-1, 0 } /* terminating_attribute */' )
        attrlist_dict['terminating_attribute'] = attrlist_index
        indent = indent - 4
        emit_tables( '};' )
        return

#-----------------------------------------------------------------------------

def elements_list_sort(a, b):
        x, y = a[0], b[0]
        if x < y:       return -1
        if x > y:       return 1
        return 0

def emit_elements(list):
        global indent, emit_c, enumvalues_dict
        left = len(list)
        id = 0
        list.sort( elements_list_sort )

        emit_tables( '' )
        emit_tables( '/* The elements table */' )
        emit_tables( '' )

        emit_tables( 'ELEMENT elements[%d] = ' % left )
	emit_defs( '#define NUMBER_SGML_ELEMENTS %d' % left )
        emit_decls( 'extern ELEMENT elements[NUMBER_SGML_ELEMENTS]; ' )

	# Emit values for enumerated values now
        for textual, group, flags, attributes, reqs in list:
                internal = 'HTML_' + textual
		# First attribute is attribute zero
                aix = 0
                for w in attributes:
			if len(w) == 0: continue
			s = '#define %s' % internal
                        emit_defs( '%s_%s %d' % (s, regsub.gsub("\-", "_", userattr_dict[w]), aix) )
                        aix = aix + 1
			for x in enumvalues_dict[w]:
				# Hack so can have hyphen in enumeration value
				x = x[len(w)+1:]
				x =  '#define %s_%s%s' % (internal, userattr_dict[w], x)
				x = regsub.gsub("\-", "_", x)
				emit_defs(x)
        emit_defs( '' )
        emit_tables( '{' )

        indent = indent + 4

        for textual, group, flags, attributes, reqs in list:
                internal = 'HTML_' + textual
#                ensure_element_number(internal)
                x = []
#                for f in flags:
#                        x.append( f )
                f = string.joinfields(flags, ' | ')
                if f == '':
                        f = 'FLAG_NONE'
                emit_tables( '{   /* Element %s: number %s */' % (textual, id))
                indent = indent + 4
                # name
                emit_tables( '{%s, %d},' % (string_for(textual), len(textual)) )
                # id
                emit_tables( '%s,' % internal )
                # group
                emit_tables( 'HTML_GROUP_%s,' % group )
                # flags
                emit_tables( '%s,' % f )
                # attributes
                emit_tables( '%s,' % attributes_for(attributes) )
                # requirements
                emit_tables( '%s,' % requirements_for(reqs) )
                # open
		emit_procs( 'extern void start%s(SGMLCTX *context, ELEMENT *element, VALUES *attributes)' % string.lower(textual) )
		emit_procs( ' { generic_start(context, element, attributes); }' )
		emit_decls( 'extern void start%s(SGMLCTX *, ELEMENT *, VALUES *);' % string.lower(textual) )
                emit_tables( 'start%s,' % string.lower(textual) )
                # close
		if "FLAG_NO_CLOSE" in flags and "FLAG_CLOSE_INTERNAL" not in flags:
        	        emit_tables( 'generic_finish, /*%s */' % string.lower(textual) )
		else:
			emit_decls( 'extern void finish%s(SGMLCTX *context, ELEMENT *element);' % string.lower(textual) )
			emit_procs( 'extern void finish%s(SGMLCTX *context, ELEMENT *element)' % string.lower(textual) )
			emit_procs( ' { generic_finish(context, element); }' )
	                emit_tables( 'finish%s,' % string.lower(textual) )
                indent = indent - 4
                if left == 1:
                        emit_tables( '}' )
                else:
                        emit_tables( '},')
                left = left - 1
                id = id + 1

        indent = indent - 4
        emit_tables( '};' )
        emit_tables( '' )

	emit_decls( 'extern void generic_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes);' )
	emit_decls( 'extern void generic_finish (SGMLCTX * context, ELEMENT * element);' )

#-----------------------------------------------------------------------------

def emit_headers():

	# The C source file

        emit_tables( '/* %s */' % copyright )
        emit_tables( '' )
        emit_tables( '/* %s: generated by attrgen.py */' % tables_name )
        emit_tables( '/* Email borris@ant.co.uk for support */' )
        emit_tables( '' )
        emit_tables( '#include "sgmlparser.h"' )
#        emit_tables( '#include "htmlparser.h"' )
        emit_tables( '' )

	# The #defines only file

	z = regsub.gsub('[^a-zA-Z0-9]', '_', defs_name)
        emit_defs( '/* %s */' % copyright )
        emit_defs( '' )
        emit_defs( '/* %s: generated by attrgen.py */' % defs_name )
        emit_defs( '/* Email borris@ant.co.uk for support */' )
        emit_defs( '' )
        emit_defs( '#ifndef __%s'% z )
        emit_defs( '#define __%s'% z )
        emit_defs( '' )

	# The external function declarations (and other bits) file

	z = regsub.gsub('[^a-zA-Z0-9]', '_', decls_name)
        emit_decls( '/* %s */' % copyright )
        emit_decls( '' )
        emit_decls( '/* %s: generated by attrgen.py */' % decls_name )
        emit_decls( '/* Email borris@ant.co.uk for support */' )
        emit_decls( '' )
        emit_decls( '#ifndef __%s'% z )
        emit_decls( '#define __%s'% z )
        emit_decls( '' )
        emit_decls( '#include "sgmlparser.h"' )
        emit_decls( '' )

	# The procs file for kick starting a new SGML client

	if procs_file == None:
		return

        emit_procs( '/* %s */' % copyright )
        emit_procs( '' )
        emit_procs( '/* %s: generated by attrgen.py */' % procs_name )
        emit_procs( '/* Email borris@ant.co.uk for support */' )
        emit_procs( '' )
        emit_procs( '#include "sgmlparser.h"' )
        emit_procs( '' )

#-----------------------------------------------------------------------------

def emit_fixup_code(fname):
        emit_tables( '#include "%s"' % fname)



def emit_footers():
        emit_tables( '/* eof */' )

	emit_defs( '#endif' )
        emit_defs( '/* eof */' )

	emit_decls( '#endif' )
        emit_decls( '/* eof */' )

        emit_procs( '/* eof */' )

        return

        fobj = open(fname)
        lines = fobj.readlines()
        fobj.close()
        for line in lines:
                emit_tables( line )

#-----------------------------------------------------------------------------

# For each element that has enumerated values for one or more attributes,
# emit a #define of the form HTML_ELEMENT_ATTRIBUTE_VALUE.

def emit_enumerated_defines():
	return	

#-----------------------------------------------------------------------------
#
# Ensure each item has an entry in any necessary dictionaries.
#

#        return ( textual_name, group, flags_list, attributes_list, requisites_list )
#        return ( internal_name, textual_name, templates_list, parse_method )


def ensure_string( x ):
	global string_dict, string_index
	if not string_dict.has_key(x):
		string_dict[x] = string_index
		string_index = string_index + 1
	return

def ensure_templates_list( x, attrname, username ):
	global templates_dict, templates_index, enumvalues_dict
	id, ev= 1, []
	for s in x:
		ensure_string(s)
		ev.append( '_%s_%s %d' % (attrname, s, id) )
		id = id + 1
	enumvalues_dict[attrname] = ev
	userattr_dict[attrname] = username
	f = flat1(x)
	if not templates_dict.has_key(f):
		templates_dict[f] = templates_index
		templates_index = templates_index + len(x) + 1
	return

def check_parse_method( x ):
	return

def ensure_group( x ):
	return

def ensure_flags_list( x ):
	return

def ensure_attributes_list( x ):
	global attributes_dict, attributes_index
	f = flat1(x)
	if not attributes_dict.has_key(f):
		attributes_dict[f] = attributes_index
		attributes_index = attributes_index + len(x) + 1
	return

def ensure_requisites_list( x ):
	global requirements_dict, requirements_index
	f = flat2(x)
	if not requirements_dict.has_key(f):
		requirements_dict[f] = requirements_index
		requirements_index = requirements_index + len(x) + 1
	return

def ensure_attribute(attr):
	ensure_string( attr[1] )
	ensure_templates_list( attr[2], attr[0], attr[1] )
	check_parse_method( attr[3] )


def ensure_element_name(x):
	global elements_dict, elements_index
	ensure_string(x)
	elements_dict[x] = -1

def ensure_element(elem):
	ensure_element_name( elem[0] )
	ensure_group( elem[1] )
	ensure_flags_list( elem[2] )
	ensure_attributes_list( elem[3] )
	ensure_requisites_list( elem[4] )

def check_element(x):
	return

def do_ensuring(attr_list, elem_list):
	global elements_dict
	for x in attr_list:
		ensure_attribute(x)
	for x in elem_list:
		ensure_element(x)
	l = elements_dict.keys()
	l.sort()
	for i in xrange(len(l)):
		elements_dict[l[i]] = i
	for x in elem_list:
		check_element(x)
	return


#-----------------------------------------------------------------------------


def generate(fname):
        a,b = parse_file(fname)

	a.sort()
	b.sort()
	do_ensuring(a, b)

        emit_headers()
        emit_element_numbers()
        emit_string_table()
        emit_templates_table()
        emit_requirements_table()
        emit_attributes(a)
        emit_attributes_table()
        emit_elements(b)
	emit_enumerated_defines()

        emit_fixup_code('fixup.c')

	emit_footers()

        return

def usage():
	print __doc__
	sys.exit(1)

def main():
	global tables_file, defs_file, decls_file, procs_file
	global tables_name, defs_name, decls_name, procs_name

	print 'SGML tables generator'

	a = sys.argv
	if len(a) != 5 and len(a) != 6:
		usage()

	tables_name, defs_name, decls_name = a[2], a[3], a[4]

	tables_file = open(tables_name, 'w')
	defs_file = open(defs_name, 'w')
	decls_file = open(decls_name, 'w')

	print 'Source file:        ', a[1]
	print 'Tables file:        ', tables_name
	print '#defines file:      ', defs_name
	print 'Externals file:     ', decls_name

	if len(a) == 6:
		procs_name = a[5]
		procs_file = open(procs_name, 'w')
		print 'Procs file:         ', procs_name

	generate(a[1])
	print 'Finished processing:', a[1]


if __name__ == '__main__':
	main()

#eof
