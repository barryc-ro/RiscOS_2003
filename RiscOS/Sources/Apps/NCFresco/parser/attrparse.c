/* attrparse.c - Attribute parsing routines */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

/*****************************************************************************

    Functions for parsing the different structures of attributes.  You can
    always use the basic string parser and parse things yourself if there's
    nothing suitable here, but adding a routine others can benefit from is
    preferred.

    13th May 1996. Borris. Spruced up for inclusion in main Fresco build.
    29th May 1996. Borris. Added additional attribute types, such as lists
    			   stdunit items.
    5/7/96: SJM: Added ENUM CASE, fixed stdunit_list
    11/7/96: SJM: Made stdunit forgiving of crap after specifier.
    16/9/96: SJM: parse_integer now always returns an integer even if the value was crap.
		  parse_tuple accepts comma separated values

    PARSE_VOID,
    PARSE_ENUM,
    PARSE_ENUM_VOID,
    PARSE_STRING,
    PARSE_INTEGER,
    PARSE_STDUNIT,
    PARSE_STRING_VOID,
    PARSE_ENUM_TUPLE,
    PARSE_INTEGER_VOID,
    PARSE_STDUNIT_VOID,
    PARSE_STDUNIT_LIST
    PARSE_ENUM_CASE (not used any more)

*/

#include "sgmlparser.h"
#include "gbf.h"
#include "util.h"

#ifdef PLOTCHECK
#include "rectplot.h"
#endif

/*****************************************************************************

    Parse a void value - this is used for attributes that do not have any
    associated value.

*/

extern VALUE sgml_do_parse_void(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v;

    if (string.nchars > 0)
    {
#if SGML_REPORTING
	sgml_note_bad_attribute(context,
				"Attribute %s does not take values - ignored", attribute->name.ptr);
#endif
    }

    v.type = value_void;
    return v;
}


/*****************************************************************************

    Like sgml_do_parse_enum() but it is permitted for no value to be
    supplied (a void value is returned if no value is supplied).

*/

VALUE sgml_do_parse_enum_void(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v;
    STRING *list = attribute->templates;
    /*int w;*/

    if (string.nchars == 0)
    {
	v.type = value_void;
	return v;
    }

    for (v.u.i = 0; list->ptr != NULL; list++, v.u.i++)
    {
	if ( list->nchars == string.nchars &&
	        strnicmpu(string.ptr, list->ptr, string.nchars)
	     == 0)
	{
	    v.type = value_enum;
	    v.u.i++;		/* enumerations from one */
	    return v;
	}
    }

    if (gbf_active(GBF_GUESS_ENUMERATIONS))
    {
	int dist;
	PRSDBGN(("Attempting to guess enumeration '%.*s'\n", string.nchars, usafe(string)));

        /* pdh: less OOC guessing. Note that this is always case-insensitive.
         */
 	/* sjm: even less keen, tries distance 1 before 2 */
	for (dist = 1; dist <= 2; dist++)
	{
	    list = attribute->templates;

	    for ( v.u.i = 0; list->ptr; list++, v.u.i++ )
	    {
		if ( ustrnearly( string.ptr, string.nchars, 
				list->ptr, list->nchars, dist ) )
		{
		    v.type = value_enum;
		    v.u.i++;		/* enumerations from one */
		    PRSDBGN(("Guessed '%.*s' for '%.*s'\n",
		          list->nchars, list->ptr, string.nchars, string.ptr));
		    return v;
		}
	    }
	}

	PRSDBGN(("Could not guess a matching enumeration\n"));
    }

    /* SJM: 27Feb98: changed to return void rather than none if no enum matches */
    v.type = value_void;
    v.u.i = 0;
    return v;
}

/*****************************************************************************

    Parse an enumerated value. The elements array supplied lists the
    permitted values. This routine matches one or complains.

*/

extern VALUE sgml_do_parse_enum(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_enum_void(context, attribute, string);

    if (v.type != value_enum)
	goto bad;

    return v;

bad:
#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad enumeration value '%.*s' for attribute %s",
			    min(string.nchars, MAXSTRING), usafe(string), attribute->name.ptr);
#endif

    v.type = value_none;
    v.u.i = 0;
    return v;
}

/*****************************************************************************/

extern VALUE sgml_do_parse_string_void(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v;

    if (string.nchars == 0)
    {
	v.type = value_void;
	return v;
    }

    /* @@@@ NvS: I think that this should be translating entities in the string,
       SJM: entities handled at lower level 
       SJM: now we handle them here, after converting to UTF8.
       */

    v.type = value_string;
    v.u.s = mkstringu(context->encoding_write, string.ptr, string.nchars);
    
/*     v.u.s.nchars = sgml_translation(context, v.u.s.ptr, v.u.s.nchars, */
/* 				   SGMLTRANS_AMPERSAND | SGMLTRANS_HASH | SGMLTRANS_STRIP_NEWLINES); */

    return v;
}

/*****************************************************************************

    Parse a string value for an attribute.  The string cannot be empty.

*/

extern VALUE sgml_do_parse_string(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_string_void(context, attribute, string);

    if (v.type == value_string)
	return v;

#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad string value '%.*s'",  min(string.nchars, MAXSTRING), usafe(string));
#endif

    v.type = value_none;
    return v;
}

/*****************************************************************************

    Parses a value consisting of a machine signed int.

*/


extern VALUE sgml_do_parse_integer_void(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string_in)
{
    UCHARACTER *ptr;
    VALUE v;
    USTRING string;

    string = ustring_strip_space(string_in);

    if (string.nchars == 0)
    {
	v.type = value_void;
	return v;
    }

    /* pdh: bodge bodge bodge */
    if ( !strnicmpu( string.ptr, "no", 2 ) )
    {
        v.u.i = 0;
        v.type = value_integer;
        return v;
    }

    v.u.i = (int)ustrtol(string.ptr, &ptr, 10);

    if (ptr == string.ptr)
	goto bad;

    v.type = value_integer;
    return v;

bad:
#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad integer value '%.*s'",  min(string.nchars, MAXSTRING), string);
#endif

    v.type = value_integer;	/* used to be void */
    return v;
}

extern VALUE sgml_do_parse_integer(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_integer_void(context, attribute, string);

    if (v.type == value_integer)
	return v;

#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad integer value '%.*s'",  min(string.nchars, MAXSTRING), usafe(string));
#endif

    v.type = value_none;
    return v;
}

/*****************************************************************************

    Parse one of the standard units used by HTML.

    Notionally, pixels are at 90dpi, and osunits are at 180dpi.  Points are
    72dpi, or 2.5 OSunits/point.  Convert the measurement such that we have
    either a PX measurement (but actually in osunits) or a MULT relative
    value. There are 6pi to the inch. Might return an absolute unit or a
    relative unit, depending upon any qualification.

    The distinction between PCENT and MULT is now retained, as frames
    have different behaviour for % and * values.

*/

#if 0
extern VALUE sgml_do_parse_stdunit(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_stdunit_void(context, attribute, string);

    if (v.type != value_none)
	return v;

    sgml_note_bad_attribute(context, "Bad unit value '%.*s'",  min(string.nchars, MAXSTRING), string);
    v.type = value_none;
    return v;
}
#endif


/*****************************************************************************

FIXME: This needs writing.

This is designed specifically for colour specification in HTML. We either
get 3 nchars of colour numeric tuple or an enumerated name. IF we get an
enumerated name, we set only the 4th byte with the enumeration index
and leave the remaining three nchars as zero.

*/

/*****************************************************************************

    Parse a colour tuple, as used in HTML's <BODY BACKGROUND=...>.  No
    endianness conversion is performed on the value.

*/

extern VALUE sgml_do_parse_colour_tuple(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v;

    if (string.nchars > 0 && string.ptr[0] == '#')
    {
	string.ptr++;
	string.nchars--;
    }

    if (string.nchars > 0)
    {
	BITS b = 0;
	int ix;

	for (ix = 0; ix < string.nchars; ix++)
	{
	    int c = string.ptr[ix];

	    PRSDBGN(("parse tuple: acc %x, ch %c\n", b, c));

	    if (c >= 256)
	    {
		goto bad;
	    }
	    else if ( isxdigit( (int) c) )
	    {
		b *= 16;	/* SJM: 02/10/96 this was outside the if () */

		if (c <= '9')
		    b += c - '0';
		else
		    b += (toupper(c) - 'A') + 10;
	    }
	    else if ( isspace(c) || c == ',')
	    {
		/* Be tolerant of whitespace - eg AA BB CC */
		;
	    }
	    else
	    {
		goto bad;
	    }
	}

	v.type = value_tuple;
	v.u.b = b;
    }
    else
    {
	goto bad;
    }

    return v;

bad:
    /* Delay message */
/*
    sgml_note_bad_attribute(context, "Bad colour tuple '%.*s'", min(string.nchars, MAXSTRING), string.ptr);
*/
    v.type = value_none;
    return v;
}

extern VALUE sgml_do_parse_enum_tuple(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_colour_tuple(context, attribute, string);

    if (v.type == value_tuple)
	return v;

    v = sgml_do_parse_enum(context, attribute, string);

    if (v.type == value_enum)
	return v;

#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad colour tuple '%.*s'", min(string.nchars, MAXSTRING), usafe(string));
#endif

    v.type = value_none;
    return v;
}

/* N.B. A absolute stdunit value is adjusted to be in OS units */


static VALUE sgml__do_parse_stdunit_void(SGMLCTX *context, ATTRIBUTE *attribute, STRING string_in)
{
    VALUE v;
    char *ptr;
    STRING string;

    string = string_strip_space(string_in);

    if (string.nchars == 0)
    {
	v.type = value_void;
	return v;
    }

    /* shortcut for '*' as with no numerals it would need special casing anyway */
    if (string.nchars == 1 && string.ptr[0] == '*')
    {
        v.type = value_relunit;
        v.u.f = 1;
        return v;
    }

    v.u.f = strtod(string.ptr, &ptr);

    if (ptr == string.ptr || string.ptr + string.nchars < ptr)
	goto bad;
    else
    {
	string.nchars -= ptr - string.ptr;
	string.ptr = ptr;
    }

    string = string_strip_start(string);

    if (string.nchars == 0
         || ( string.nchars >= 3 && strnicmp("PIX", string.ptr, 3) == 0 ) )
    {
	/* Implicitly a stdunit is in pixels but we convert to OS units */
	v.type = value_absunit;
	v.u.f *= 2.0;
    }
    else if (string.nchars >= 2 && strnicmp("PT", string.ptr, 2) == 0)
    {
	v.type = value_absunit;
	v.u.f *= 180.0 / 72.0;
    }
    else if (string.nchars >= 2 && strnicmp("PI", string.ptr, 2) == 0)
    {
	v.type = value_absunit;
	v.u.f *= 180.0 / 6.0;
    }
    else if (string.nchars >= 2 && strnicmp("PX", string.ptr, 2) == 0)
    {
	v.type = value_absunit;
	v.u.f *= 2.0;
    }
    else if (string.nchars >= 2 && strnicmp("IN", string.ptr, 2) == 0)
    {
	v.type = value_absunit;
	v.u.f *= 180.0;
    }
    else if (string.nchars >= 2 && strnicmp("CM", string.ptr, 2) == 0)
    {
	v.type = value_absunit;
	v.u.f *= 180.0 / 2.54;
    }
    else if (string.nchars >= 2 && strnicmp("MM", string.ptr, 2) == 0)
    {
	v.type = value_absunit;
	v.u.f *= 180.0 / 25.4;
    }
    else if (string.nchars >= 1 && string.ptr[0] == '%')
    {
	v.type = value_pcunit;
	/* DAF: 970519: Global behaviour - no individual percentage
           bigger than 100% is acceptable. Clip rather than ignore */
	if (v.u.f > 100.0)
	    v.u.f = 100.0;
#if DEBUG && 0
	v.type = value_none;	/* temp hack! */
#endif
    }
    else if (string.nchars >= 1 && string.ptr[0] == '*')
    {
	v.type = value_relunit;
    }
    else
    {
	goto bad;
    }

#ifdef PLOTCHECK
    {
	/*extern int fwidth_scale;*/
	if (v.type == value_absunit)
	{
	    v.u.f /= fwidth_scale;
	    /*v.u.f += .99;*/
	}
    }
#endif

    return v;

bad:
#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad unit value '%.*s'",
			    min(string_in.nchars, MAXSTRING), string_in.ptr);
#endif
    
    v.type = value_none;
    return v;
}

extern VALUE sgml_do_parse_stdunit_void(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string_in)
{
    VALUE val;
    STRING string_copy;

    string_copy = mkstringu(context->encoding_write, string_in.ptr, string_in.nchars);

    val = sgml__do_parse_stdunit_void(context, attribute, string_copy);

    string_free(&string_copy);
    
    return val;
}

extern VALUE sgml_do_parse_stdunit(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_stdunit_void(context, attribute, string);

    switch (v.type)
    {
    case value_absunit:
    case value_relunit:
    case value_pcunit:
	return v;
    }

#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad unit value '%.*s'",  min(string.nchars, MAXSTRING), usafe(string));
#endif

    v.type = value_none;
    return v;
}

/*
 * a list is a comma separated list of stdunits.
 *
 * COORDS lists are always of pixels (currently)
 * FRAMESET lists are pixels, pcent, mult

 */

#if defined(MemCheck_MEMCHECK)
/* Memcheck doesn't like this fn for some reason */
#pragma -c0
#endif

extern VALUE sgml_do_parse_stdunit_list(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string_in)
{
    VALUE v;
    int i;
    char *copy, *use;
    STRING string;

    string = mkstringu(context->encoding_write, string_in.ptr, string_in.nchars);

    if (string.nchars == 0)
        goto bad;

    v.type = value_stdunit_list;
    v.u.l.num = string_count_tokens(string, " ,\t\r\n");
    v.u.l.items = mm_calloc(v.u.l.num, sizeof(*v.u.l.items));

    use = copy = stringdup(string);

    PRSDBG(("stdunitlist: %d elements\n", v.u.l.num));

    for (i = 0; i < v.u.l.num; i++)
    {
        STRING el;

	el.ptr = strtok(use, " ,\t\r\n");
	use = NULL;

	if (el.ptr)
	{
	    el.nchars = strlen(el.ptr);

	    PRSDBG(("stdunitlist: element '%.*s'\n", el.nchars, el.ptr));

	    /* this will deal with excess whitespace */
	    v.u.l.items[i] = sgml__do_parse_stdunit_void(context, attribute, el);
	    if (v.u.l.items[i].type == value_void)
		v.u.l.items[i].type = value_none;

	    PRSDBG(("stdunitlist: value    %g\n", v.u.l.items[i].u.f));
	}
	else
	{
	    v.u.l.items[i].type = value_none;
	}
    }

    mm_free(copy);
    string_free(&string);

    return v;

bad:
#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad stdunit list - empty");
#endif
    string_free(&string);

    v.type = value_none;
    return v;
}

#if defined(MemCheck_MEMCHECK)
/* Memcheck doesn't like this fn for some reason */
#pragma -c1
#endif

/*****************************************************************************/


extern VALUE sgml_do_parse_enum_string(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_enum(context, attribute, string);

    if (v.type == value_enum)
	return v;

    return sgml_do_parse_string(context, attribute, string);
}

/*****************************************************************************/

extern VALUE sgml_do_parse_bool(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v;

    if (string.nchars == 0)
    {
	v.type = value_void;
	return v;
    }

    switch (string.ptr[0])
    {
    case 'n':
    case 'N':
    case '0':
	v.type = value_bool;
	v.u.i = 0;
	break;

    case 'y':
    case 'Y':
    case '1':
	v.type = value_bool;
	v.u.i = 1;
	break;

    default:
#if SGML_REPORTING
	sgml_note_bad_attribute(context, "Bad bool value '%.*s'",  min(string.nchars, MAXSTRING), usafe(string));
#endif
	v.type = value_none;
	break;
    }

    return v;
}

/*****************************************************************************/

extern VALUE sgml_do_parse_colour(SGMLCTX *context, ATTRIBUTE *attribute, USTRING string)
{
    VALUE v = sgml_do_parse_colour_tuple(context, attribute, string);
    STRING string_copy;

    if (v.type == value_tuple)
	return v;

    string_copy = mkstringu(context->encoding_write, string.ptr, string.nchars);

    v = colour_lookup(string_copy);

    string_free(&string_copy);

    if (v.type == value_tuple)
	return v;

#if SGML_REPORTING
    sgml_note_bad_attribute(context, "Bad colour tuple '%.*s'", min(string_copy.nchars, MAXSTRING), usafe(string));
#endif

    return v;
}

/*****************************************************************************/

/* eof attrparse.c */
