/* parser/entities.c - Processing of character entities */
/* (C) ANT Limited 1996. All rights reserved */

/* CHANGE LOG:
 * 02/07/96: SJM: SGMLTRANS_HASH case used in_ptr[1] rather than in_ptr[0].
 *                Removed percent decoding from entity_recognition.
 * 18/07/96: SJM: added option to translate undefined latin1 keys to pc style ones
 *		  changed some variable names to get rid of winges.
 * 25/07/96: SJM: set used=TRUE in sgml_translation's new fields that strip newlines
 *		  and change + to ' '
 */

#ifdef STDALONE
#include "htmlcheck.h"
#else
#include "htmlparser.h"
#endif

#include "gbf.h"
#include "util.h"

/** THIS TABLE MUST BE IN strncmp() ORDER FOR THIS TO WORK **/
/** ALSO, NO NAME MAY ALSO BE A STEM OF ANOTHER NAME */

/** entities all in lower case */

char *entity_names[] =
{
  "aacute",     /* small a, acute accent */
  "acirc",      /* small a, circumflex accent */
  "acute",      /* acute accent */
  "aelig",      /* small ae diphthong (ligature) */
  "agrave",     /* small a, grave accent */
  "amp",       	/* ampersand */
  "aring",      /* small a, ring */
  "atilde",     /* small a, tilde */
  "auml",       /* small a, dieresis or umlaut mark */
  "bdquo",	/* double low quote */
  "brvbar",     /* broken (vertical) bar */
  "ccedil",     /* small c, cedilla */
  "cedil",      /* cedilla */
  "cent",       /* cent sign */
  "copy",       /* copyright sign */
  "curren",     /* general currency sign */
  "dagger",
  "deg",        /* degree sign */
  "divide",     /* divide sign */
  "eacute",     /* small e, acute accent */
  "ecirc",      /* small e, circumflex accent */
  "egrave",     /* small e, grave accent */
  "eth",        /* small eth, Icelandic */
  "euml",       /* small e, dieresis or umlaut mark */
  "frac12",     /* fraction one-half */
  "frac14",     /* fraction one-quarter */
  "frac34",     /* fraction three-quarters */
  "gt",        	/* greater than */
  "iacute",     /* small i, acute accent */
  "icirc",      /* small i, circumflex accent */
  "iexcl",      /* inverted exclamation mark */
  "igrave",     /* small i, grave accent */
  "iquest",     /* inverted question mark */
  "iuml",       /* small i, dieresis or umlaut mark */
  "laquo",      /* angle quotation mark, left */
  "ldquo",	/* double quote, left */
  "lsaquo",     /* single angle quotation mark, left */
  "lsquo",	/* single quote, left */
  "lt",        	/* greater than */
  "macr",       /* macron */
  "micro",      /* micro sign */
  "middot",     /* middle dot */
  "nbsp",       /* no-break space */
  "not",        /* not sign */
  "ntilde",     /* small n, tilde */
  "oacute",     /* small o, acute accent */
  "ocirc",      /* small o, circumflex accent */
  "ograve",     /* small o, grave accent */
  "ordf",       /* ordinal indicator, feminine */
  "ordm",       /* ordinal indicator, masculine */
  "oslash",     /* small o, slash */
  "otilde",     /* small o, tilde */
  "ouml",       /* small o, dieresis or umlaut mark */
  "para",       /* pilcrow (paragraph sign) */
  "permil",			/* per mille */
  "plusmn",     /* plus-or-minus sign */
  "pound",      /* pound sterling sign */
  "quot",       /* double quote sign - June 94 */
  "raquo",	/* angle quotation mark, right */
  "rdquo",	/* double quote, right */
  "reg",        /* registered sign */
  "rsaquo",     /* single angle quotation mark, right */
  "rsquo",	/* single quote, right */
  "sect",       /* section sign */
  "shy",        /* soft hyphen */
  "sup1",       /* superscript one */
  "sup2",       /* superscript two */
  "sup3",       /* superscript three */
  "szlig",      /* small sharp s, German (sz ligature) */
  "thorn",      /* small thorn, Icelandic */
  "times",      /* multiply sign */
  "trade",      /* trademark sign */
  "uacute",     /* small u, acute accent */
  "ucirc",      /* small u, circumflex accent */
  "ugrave",     /* small u, grave accent */
  "uml",        /* umlaut (dieresis) */
  "uuml",       /* small u, dieresis or umlaut mark */
  "yacute",     /* small y, acute accent */
  "yen",        /* yen sign */
  "yuml"       /* small y, dieresis or umlaut mark */
};

/* 	Entity values -- for ISO Latin 1 local representation
**
**	This MUST match exactly the table above
**	lower case / upper case
*/
static UCHARACTER ISO_Latin1[] = {
	'\341', '\301', /* small a, acute accent */
	'\342', '\302', /* small a, circumflex accent */
	'\264', '\264', /* acute accent */
	'\346', '\306', /* small ae diphthong (ligature) */
	'\340', '\300', /* small a, grave accent */
	'\046', '\046',	/* ampersand */
	'\345', '\305', /* small a, ring */
	'\343', '\303', /* small a, tilde */
	'\344', '\304', /* small a, dieresis or umlaut mark */
#if UNICODE
	8222, 8222,	/* double quote, low */
#else
	150, 150,	/* double quote, low */
#endif
	'\246', '\246', /* broken (vertical) bar */
	'\347', '\307', /* small c, cedilla */
	'\270', '\270', /* cedilla */
	'\242', '\242', /* cent sign */
	'\251', '\251', /* copyright sign */
	'\244', '\244', /* general currency sign */
#if UNICODE
	8224, 8225,	/* dagger, Dagger */
#else
	156, 157,	/* dagger, Dagger */
#endif
	'\260', '\260', /* degree sign */
	'\367', '\367', /* divide sign */
	'\351', '\311', /* small e, acute accent */
	'\352', '\312', /* small e, circumflex accent */
	'\350', '\310', /* small e, grave accent */
	'\360', '\320', /* small eth, Icelandic */
	'\353', '\313', /* small e, dieresis or umlaut mark */
	'\275', '\275', /* fraction one-half */
	'\274', '\274', /* fraction one-quarter */
	'\276', '\276', /* fraction three-quarters */
	'\076', '\076',	/* greater than */
	'\355', '\315', /* small i, acute accent */
	'\356', '\316', /* small i, circumflex accent */
	'\241', '\241', /* inverted exclamation mark */
	'\354', '\314', /* small i, grave accent */
	'\277', '\277', /* inverted question mark */
	'\357', '\317', /* small i, dieresis or umlaut mark */
	'\253', '\253', /* angle quotation mark, left */
#if UNICODE
	8220, 8220,	/* double quote, left */
#else
	148, 148,	/* double quote, left */
#endif
#if UNICODE
	8249, 8249,	/* single angle quotation mark, left */
	8216, 8216,	/* single quote, left */
#else
	146, 146,	/* single angle quotation mark, left */
	144, 144,	/* single quote, left */
#endif
	'\074', '\074',	/* less than */
	'\257', '\257', /* macron */
	'\265', '\265', /* micro sign */
	'\267', '\267', /* middle dot */
	'\240', '\240', /* no-break space */
	'\254', '\254', /* not sign */
	'\361', '\321', /* small n, tilde */
	'\363', '\323', /* small o, acute accent */
	'\364', '\324', /* small o, circumflex accent */
	'\362', '\322', /* small o, grave accent */
	'\252', '\252', /* ordinal indicator, feminine */
	'\272', '\272', /* ordinal indicator, masculine */
	'\370', '\330', /* small o, slash */
	'\365', '\325', /* small o, tilde */
	'\366', '\326', /* small o, dieresis or umlaut mark */
	'\266', '\266', /* pilcrow (paragraph sign) */
#if UNICODE
	8240, 8240,		/* permille */
#else
	142, 142
#endif
	'\261', '\261', /* plus-or-minus sign */
	'\243', '\243', /* pound sterling sign */
	'\042', '\042', /* double quote sign - June 94 */
	'\273', '\273', /* angle quotation mark, right */
#if UNICODE
	8221, 8221,	/* double quote, right */
#else
	149, 149,	/* double quote, right */
#endif
	'\256', '\256', /* registered sign */

#if UNICODE
	8250, 8250,	/* single angle quotation mark, right */
	8217, 8217,	/* single quote, right */
#else
	147, 147,	/* single angle quotation mark, right */
	145, 145,	/* single quote, right */
#endif
	'\247', '\247', /* section sign */
	'\255', '\255', /* soft hyphen */
	'\271', '\271', /* superscript one */
	'\262', '\262', /* superscript two */
	'\263', '\263', /* superscript three */
	'\337', '\337', /* small sharp s, German (sz ligature) */
	'\376', '\336', /* small thorn, Icelandic */
	'\327', '\327', /* multiply sign */
#if UNICODE
	8482, 8482,	/* trademark sign */
#else
	'\215', '\215', /* trademark sign */
#endif
	'\372', '\332', /* small u, acute accent */
	'\373', '\333', /* small u, circumflex accent */
	'\371', '\331', /* small u, grave accent */
	'\250', '\250', /* umlaut (dieresis) */
	'\374', '\334', /* small u, dieresis or umlaut mark */
	'\375', '\335', /* small y, acute accent */
	'\245', '\245', /* yen sign */
	'\377', '\337' /* small y, dieresis or umlaut mark */
};

#if UNICODE

/* An excerpt from the Micorsoft.cp1252 table */

static UCHARACTER pc_translate_keys[32] =
{
    0xFFFD, 0xFFFD, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
    0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0xFFFD, 0xFFFD, 0xFFFD, 
    0xFFFD, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
    0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0xFFFD, 0xFFFD, 0x0178
};

#else
/*
 * keys with names below have been translated to the appropriate key
 * in the PC/Mac keyset.
 */

static char pc_translate_keys[32] =
{
    ' ',    /* 128: */
    ' ',    /* 129: */
    ',',    /* 130: sq lower */
    'f',    /* 131: guilder */
    150,    /* 132: dq lower */
    140,    /* 133: ellipsis */
    156,    /* 134: dagger */
    157,    /* 135: dbl dagger */
    '^',    /* 136: circumflex */
    142,    /* 137: per mill */
    'S',    /* 138: S hacek*/
    146,    /* 139: laquo */
    154,    /* 140: OE */
    ' ',    /* 141: */
    ' ',    /* 142: */
    ' ',    /* 143: */
    ' ',    /* 144: */
    144,    /* 145: sq o */
    145,    /* 146: sq c */
    148,    /* 147: dq o */
    149,    /* 148: dq c */
    143,    /* 149: small bullet */
    151,    /* 150: en dash */
    152,    /* 151: em dash */
    '~',    /* 152: tilde accent */
    141,    /* 153: TM */
    's',    /* 154: s hacek */
    147,    /* 155: raquo */
    155,    /* 156: oe */
    ' ',    /* 157: */
    ' ',    /* 158: */
    ' '     /* 159: */
};
#endif

char convert_undefined_key_code(char c)
{
    if (c >= 128 && c < 160)
	return pc_translate_keys[c-128];
    return c;
}

/*****************************************************************************/

/* ASSERT: ain is key object, bin is array member */
/* will fail if this is not the case. I think ANSI defines this. */

static int entity_compare_function(const void *ain, const void *bin)
{
    const UCHARACTER *ap = (const UCHARACTER *) ain;
    const char **bp = (const char **)bin;

    PRSDBGN(("entity_compare_function('%s', '%s')\n", ap, *bp));

    return strnicmpu( ap, *bp, strlen(*bp));
}

extern int sgml_translation(SGMLCTX *context, UCHARACTER *in_ptr, int in_bytes, int rules)
{
#if DEBUG
    UCHARACTER *orig_ptr = in_ptr;
#endif
    UCHARACTER *out_ptr;
    int out_bytes;

    if (context == NULL)
    {
	rules &= ~ SGMLTRANS_WARNINGS;
    }
    else
    {
	ASSERT(context->magic == SGML_MAGIC);
    }
#if 1
    if (in_ptr == NULL || in_bytes == 0)
	return 0;
#endif
    /* Someone is triggering this. They shouldn't */
    ASSERT(in_ptr != NULL);

    PRSDBGN(("sgml_translation('%.*s', %d, 0x%x)\n",
	    in_bytes, in_ptr, in_bytes, rules));

    for (out_ptr = in_ptr, out_bytes = 0; in_bytes > 0; in_bytes--)
    {
	BOOL used = FALSE;

	const UCHARACTER c = *in_ptr++;
	
	if ((c == '\n' || c == '\r') && (rules & SGMLTRANS_STRIP_NEWLINES) != 0)
	{
            /* throw them away */
	    used = TRUE;
	}
#if 0
	/* None of this is used anymore - see url.c instead */
	if (c == '+' && (rules & SGMLTRANS_PLUS_TO_SPACE) != 0)
	{
	    *out_ptr++ = ' ';
	    out_bytes++;
	    used = TRUE;
	}
	else if (c == '%' && (rules & SGMLTRANS_PERCENT) != 0)
	{
	    if (in_bytes >= 2 && in_ptr[0] < 256 && in_ptr[1] < 256)
	    {
		const char c1 = tolower(in_ptr[0]), c2 = tolower(in_ptr[1]);
		if ( isxdigit((int)c1) && isxdigit((int)c2) )
		{
		    const char x =
			(char)
			(
			    (
				( (c1 > '9') ? (c1 - 'a' + 10) : (c1 - '0') )
				* 16
				) +
			    (
				(c2 > '9') ? (c2 - 'a' + 10) : (c2 - '0')

				)
			    );
		    if ( ! ( x == 127 || ( x < 32 && ! isspace((int)(char) x) ) ) )
		    {
			*out_ptr++ = x;
			out_bytes++;
			used = TRUE;
			in_ptr += 2;
			in_bytes -= 2;
/* 			PRSDBG(("Recognised as '%c'\n", x)); */
		    }
		}
	    }

#if SGML_REPORTING
	    if ( !used && (rules & SGMLTRANS_WARNINGS) != 0 )
		sgml_note_message(context,
				  "Insufficient characters to expand %%XX sequence");
#endif
	}
	else
#endif
	    if (c == '&' &&
		 (rules & SGMLTRANS_HASH) != 0 &&
		 in_bytes > 1 &&
		 (in_ptr[0] == '#'/*  || isdigit(in_ptr[0]) */))
	    /* SJM: 30/09/97. Don't allow numbers without a hash. I
	       don't know why I put this in in the first place but
	       NS doesn't do it */
	{
	    UCHARACTER *end;
	    long x;
	    int base;

	    PRSDBG(("Trying to do numeric entity\n"));

	    if (in_ptr[0] == '#')
	    {
		in_bytes--;
		in_ptr++;
	    }

	    base = 10;
	    if (in_bytes > 0 && in_ptr[0] == 'x')
	    {
		in_bytes--;
		in_ptr++;
		base = 16;
	    }

	    x = in_bytes >= 1 ? ustrtol(in_ptr, &end, base) : (end = in_ptr, -1);

	    if (end != in_ptr &&
		x > 0 &&
		x != 127 &&
#if !UNICODE
		x < 256 &&
		(x >= 32 || isspace((char)x))
#else
		(x >= 32 || (x < 256 && isspace((char)x)))
#endif
		)
	    {
#ifdef STBWEB
		/* unfortunately we still need this whether unicode tables in use or not */
		x = convert_undefined_key_code((int)x);
#endif
		*out_ptr++ = (UCHARACTER) x;
		out_bytes++;

		in_bytes -= end - in_ptr;
		in_ptr = end;

                /* pdh: added this 'if' because &#163; was leaving the ;
                 * in titles
                 */
		if (in_bytes > 0 && *in_ptr == ';')
		{
		    in_ptr++;
		    in_bytes--;
		}

		used = TRUE;
	    }

#if SGML_REPORTING
	    if ( !used && (rules & SGMLTRANS_WARNINGS) != 0 )
		sgml_note_message(context,
				  "Unrecognised character entity (&entity;) name '%.*s'",
				  min(MAXSTRING, in_bytes),
				  in_ptr - 1);
#endif
	}
	else if (c == '&' && (rules & SGMLTRANS_AMPERSAND) != 0)
	{
	    if (in_bytes >= 1)
	    {
		/* This could, theoretically, touch characters beyond */
		/* allocated space. This would require something that */
		/* appeared to be an entity for ALL it's characters (ie */
		/* no trailling ; or NULL bytes) and the very next bytes */
		/* was not valid memory. I think this is unlikely. */
		/* Also relies upon there being no entity name that is */
		/* also a stem for another entity name. */
		BOOL upper_case = in_ptr[0] < 128 && isupper(in_ptr[0]);
		char **matchp = bsearch(in_ptr,
					entity_names,
					sizeof(entity_names) / sizeof(entity_names[0]),
					sizeof(entity_names[0]),
					entity_compare_function);

		if (matchp != NULL)
		{
		    int len = strlen( *matchp );
		    BOOL semicolon = in_bytes > len && in_ptr[len] == ';';

		    if (semicolon || (rules & SGMLTRANS_STRICT) == 0)
		    {
			if (semicolon)
			    len++;

			in_ptr += len;
			in_bytes -= len;

			*out_ptr++ = ISO_Latin1[ (matchp - entity_names)*2 + (upper_case ? 1 : 0) ];
			out_bytes++;

			used = TRUE;
		    }
		}
	    }

#if SGML_REPORTING
	    if (! used && (rules & SGMLTRANS_WARNINGS) != 0 )
		sgml_note_message(context,
				  "Unrecognised character entity (&entity;) name '%.*s'",
				  min(MAXSTRING, in_bytes),
				  in_ptr - 1);
#endif
       	}
#if 0
	    else if ((c < 32 || c == 127) && (rules & SGMLTRANS_STRIP_CTRL) != 0)
	{
	    /* SJM: I'm not sure whether taking NULLs out would mess stuff up */
	    if (c != 0)
	    {
		*out_ptr++ = ' ';
		out_bytes++;
		used = TRUE;
	    }
	}
#endif
	
	if (! used)
	{
	    *out_ptr++ = c;
	    out_bytes++;
	}
    }

    PRSDBGN(("sgml_translation() returns '%.*s', %d bytes\n",
	     out_bytes, orig_ptr, out_bytes));

    return out_bytes;
}

extern void entity_recognition(SGMLCTX *context)
{
    const int len = sgml_translation(context,
				     context->inhand.data,
				     context->inhand.ix,
				     SGMLTRANS_AMPERSAND |
				     SGMLTRANS_HASH |
				     SGMLTRANS_WARNINGS);
    context->inhand.ix = len;

    push_inhand(context);
}

/* eof entities.c */

