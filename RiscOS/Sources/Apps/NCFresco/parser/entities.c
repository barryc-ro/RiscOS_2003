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

#if 1

/** entities all in lower case */

static const char * const entity_names[] =
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
  "bdquo",      /* bottom double quote (note name inconsistency with sbquo) */
  "brvbar",     /* broken (vertical) bar */
  "ccedil",     /* small c, cedilla */
  "cedil",      /* cedilla */
  "cent",       /* cent sign */
  "copy",       /* copyright sign */
  "curren",     /* general currency sign */
  "dagger",     /* dagger (UC is double-dagger) */
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
  "ldquo",      /* left double quote (66) */
  "lsaquo",     /* left single angle quote */
  "lsquo",      /* left single quote (6) */
  "lt",        	/* greater than */
  "macr",       /* macron */
  "mdash",      /* em rule (v long dash) */
  "micro",      /* micro sign */
  "middot",     /* middle dot */
  "nbsp",       /* no-break space */
  "ndash",      /* en rule (long dash) */
  "not",        /* not sign */
  "ntilde",     /* small n, tilde */
  "oacute",     /* small o, acute accent */
  "ocirc",      /* small o, circumflex accent */
  "oelig",      /* oe ligature (diphthong) */
  "ograve",     /* small o, grave accent */
  "ordf",       /* ordinal indicator, feminine */
  "ordm",       /* ordinal indicator, masculine */
  "oslash",     /* small o, slash */
  "otilde",     /* small o, tilde */
  "ouml",       /* small o, dieresis or umlaut mark */
  "para",       /* pilcrow (paragraph sign) */
  "permil",     /* per mille */
  "plusmn",     /* plus-or-minus sign */
  "pound",      /* pound sterling sign */
  "quot",       /* double quote sign - June 94 */
  "raquo",      /* angle quotation mark, right */
  "rdquo",      /* right double quote (99) */
  "reg",        /* registered sign */
  "rsaquo",     /* right single angle quote */
  "rsquo",      /* right single quote (9) */
  "sbquo",      /* single bottom quote (note name inconsistency width dbquo) */
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
static const char ISO_Latin1[] = {
	'\341', '\301', /* small a, acute accent */
	'\342', '\302', /* small a, circumflex accent */
	'\264', '\264', /* acute accent */
	'\346', '\306', /* small ae diphthong (ligature) */
	'\340', '\300', /* small a, grave accent */
	'\046', '\046',	/* ampersand */
	'\345', '\305', /* small a, ring */
	'\343', '\303', /* small a, tilde */
	'\344', '\304', /* small a, dieresis or umlaut mark */
#ifdef __acorn
        '\x96', '\x96', /* bottom double quote */
#else
        '\x84', '\x84',
#endif
	'\246', '\246', /* broken (vertical) bar */
	'\347', '\307', /* small c, cedilla */
	'\270', '\270', /* cedilla */
	'\242', '\242', /* cent sign */
	'\251', '\251', /* copyright sign */
	'\244', '\244', /* general currency sign */
#ifdef __acorn
        '\x9C', '\x9D', /* dagger/double dagger */
#else
        '\x86', '\x87',
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
#ifdef __acorn      /* Acorn Extended Latin */
        '\x94', '\x94', /* left double quote (66) */
        '\x92', '\x92', /* left single angle quote */
        '\x90', '\x90', /* left single quote (6) */
#else               /* Certainly right on PC and Mac, maybe elsewhere too */
        '\x93', '\x93', /* left double quote (66) */
        '\x8B', '\x8B', /* left single angle quote */
        '\x91', '\x91', /* left single quote (6) */
#endif
	'\074', '\074',	/* less than */
	'\257', '\257', /* macron */
#ifdef __acorn
	'\x98', '\x98', /* em rule */
#else
	'\x97', '\x97', /* em rule */
#endif
	'\265', '\265', /* micro sign */
	'\267', '\267', /* middle dot */
	'\240', '\240', /* no-break space */
#ifdef __acorn
	'\x99', '\x99', /* en rule */
#else
	'\x96', '\x96', /* en rule */
#endif
	'\254', '\254', /* not sign */
	'\361', '\321', /* small n, tilde */
	'\363', '\323', /* small o, acute accent */
	'\364', '\324', /* small o, circumflex accent */
#ifdef __acorn
        '\x9B', '\x9A', /* oe ligature */
#else
        '\x9C', '\x8C', /* oe ligature */
#endif
	'\362', '\322', /* small o, grave accent */
	'\252', '\252', /* ordinal indicator, feminine */
	'\272', '\272', /* ordinal indicator, masculine */
	'\370', '\330', /* small o, slash */
	'\365', '\325', /* small o, tilde */
	'\366', '\326', /* small o, dieresis or umlaut mark */
	'\266', '\266', /* pilcrow (paragraph sign) */
#ifdef __acorn
        '\x8E', '\x8E', /* per mille */
#else
        '\x89', '\x89',
#endif
	'\261', '\261', /* plus-or-minus sign */
	'\243', '\243', /* pound sterling sign */
	'\042', '\042', /* double quote sign - June 94 */
	'\273', '\273', /* angle quotation mark, right */
#ifdef __acorn
        '\x95', '\x95', /* right double quote (99) */
#else
        '\x94', '\x94', /* right double quote (99) */
#endif
	'\256', '\256', /* registered sign */
#ifdef __acorn
        '\x93', '\x93', /* right single angle quote */
        '\x91', '\x91', /* right single quote (9) */
        '\x2C', '\x2C', /* single bottom quote (not in Acorn Extended Latin, we use comma) */
#else
        '\x9B', '\x9B', /* right single angle quote */
        '\x92', '\x92', /* right single quote (9) */
        '\x82', '\x82', /* single bottom quote */
#endif
	'\247', '\247', /* section sign */
	'\255', '\255', /* soft hyphen */
	'\271', '\271', /* superscript one */
	'\262', '\262', /* superscript two */
	'\263', '\263', /* superscript three */
	'\337', '\337', /* small sharp s, German (sz ligature) */
	'\376', '\336', /* small thorn, Icelandic */
	'\327', '\327', /* multiply sign */
#ifdef __acorn
	'\215', '\215', /* trademark sign */
#else
	'\x99', '\x99', /* trademark sign */
#endif
	'\372', '\332', /* small u, acute accent */
	'\373', '\333', /* small u, circumflex accent */
	'\371', '\331', /* small u, grave accent */
	'\250', '\250', /* umlaut (dieresis) */
	'\374', '\334', /* small u, dieresis or umlaut mark */
	'\375', '\335', /* small y, acute accent */
	'\245', '\245', /* yen sign */
#ifdef __acorn
	'\377', '\377' /* small y, dieresis or umlaut mark */
#else
	'\377', '\x9F' /* small y, dieresis or umlaut mark */
#endif
};

#else

char *entity_names[] =
{
  "AElig",      /* capital AE diphthong (ligature) */
  "Aacute",     /* capital A, acute accent */
  "Acirc",      /* capital A, circumflex accent */
  "Agrave",     /* capital A, grave accent */
  "Aring",      /* capital A, ring */
  "Atilde",     /* capital A, tilde */
  "Auml",       /* capital A, dieresis or umlaut mark */
  "Ccedil",     /* capital C, cedilla */
  "ETH",        /* capital Eth, Icelandic */
  "Eacute",     /* capital E, acute accent */
  "Ecirc",      /* capital E, circumflex accent */
  "Egrave",     /* capital E, grave accent */
  "Euml",       /* capital E, dieresis or umlaut mark */
  "Iacute",     /* capital I, acute accent */
  "Icirc",      /* capital I, circumflex accent */
  "Igrave",     /* capital I, grave accent */
  "Iuml",       /* capital I, dieresis or umlaut mark */
  "Ntilde",     /* capital N, tilde */
  "Oacute",     /* capital O, acute accent */
  "Ocirc",      /* capital O, circumflex accent */
  "Ograve",     /* capital O, grave accent */
  "Oslash",     /* capital O, slash */
  "Otilde",     /* capital O, tilde */
  "Ouml",       /* capital O, dieresis or umlaut mark */
  "THORN",      /* capital THORN, Icelandic */
  "Uacute",     /* capital U, acute accent */
  "Ucirc",      /* capital U, circumflex accent */
  "Ugrave",     /* capital U, grave accent */
  "Uuml",       /* capital U, dieresis or umlaut mark */
  "Yacute",     /* capital Y, acute accent */
  "aacute",     /* small a, acute accent */
  "acirc",      /* small a, circumflex accent */
  "acute",      /* acute accent */
  "aelig",      /* small ae diphthong (ligature) */
  "agrave",     /* small a, grave accent */
  "amp",       	/* ampersand */
  "aring",      /* small a, ring */
  "atilde",     /* small a, tilde */
  "auml",       /* small a, dieresis or umlaut mark */
  "brvbar",     /* broken (vertical) bar */
  "ccedil",     /* small c, cedilla */
  "cedil",      /* cedilla */
  "cent",       /* cent sign */
  "copy",       /* copyright sign */
  "curren",     /* general currency sign */
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
  "plusmn",     /* plus-or-minus sign */
  "pound",      /* pound sterling sign */
  "quot",       /* double quote sign - June 94 */
  "raquo",      /* angle quotation mark, right */
  "reg",        /* registered sign */
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
*/
char * ISO_Latin1[] = {
 	"\306", /* capital AE diphthong (ligature) */
	"\301", /* capital A, acute accent */
	"\302", /* capital A, circumflex accent */
	"\300", /* capital A, grave accent */
	"\305", /* capital A, ring */
	"\303", /* capital A, tilde */
	"\304", /* capital A, dieresis or umlaut mark */
	"\307", /* capital C, cedilla */
	"\320", /* capital Eth, Icelandic */
	"\311", /* capital E, acute accent */
	"\312", /* capital E, circumflex accent */
	"\310", /* capital E, grave accent */
	"\313", /* capital E, dieresis or umlaut mark */
	"\315", /* capital I, acute accent */
	"\316", /* capital I, circumflex accent */
	"\314", /* capital I, grave accent */
	"\317", /* capital I, dieresis or umlaut mark */
	"\321", /* capital N, tilde */
	"\323", /* capital O, acute accent */
	"\324", /* capital O, circumflex accent */
	"\322", /* capital O, grave accent */
	"\330", /* capital O, slash */
	"\325", /* capital O, tilde */
	"\326", /* capital O, dieresis or umlaut mark */
	"\336", /* capital THORN, Icelandic */
	"\332", /* capital U, acute accent */
	"\333", /* capital U, circumflex accent */
	"\331", /* capital U, grave accent */
	"\334", /* capital U, dieresis or umlaut mark */
	"\335", /* capital Y, acute accent */
	"\341", /* small a, acute accent */
	"\342", /* small a, circumflex accent */
	"\264", /* acute accent */
	"\346", /* small ae diphthong (ligature) */
	"\340", /* small a, grave accent */
	"\046",	/* ampersand */
	"\345", /* small a, ring */
	"\343", /* small a, tilde */
	"\344", /* small a, dieresis or umlaut mark */
	"\246", /* broken (vertical) bar */
	"\347", /* small c, cedilla */
	"\270", /* cedilla */
	"\242", /* cent sign */
	"\251", /* copyright sign */
	"\244", /* general currency sign */
	"\260", /* degree sign */
	"\367", /* divide sign */
	"\351", /* small e, acute accent */
	"\352", /* small e, circumflex accent */
	"\350", /* small e, grave accent */
	"\360", /* small eth, Icelandic */
	"\353", /* small e, dieresis or umlaut mark */
	"\275", /* fraction one-half */
	"\274", /* fraction one-quarter */
	"\276", /* fraction three-quarters */
	"\076",	/* greater than */
	"\355", /* small i, acute accent */
	"\356", /* small i, circumflex accent */
	"\241", /* inverted exclamation mark */
	"\354", /* small i, grave accent */
	"\277", /* inverted question mark */
	"\357", /* small i, dieresis or umlaut mark */
	"\253", /* angle quotation mark, left */
	"\074",	/* less than */
	"\257", /* macron */
	"\265", /* micro sign */
	"\267", /* middle dot */
	"\240", /* no-break space */
	"\254", /* not sign */
	"\361", /* small n, tilde */
	"\363", /* small o, acute accent */
	"\364", /* small o, circumflex accent */
	"\362", /* small o, grave accent */
	"\252", /* ordinal indicator, feminine */
	"\272", /* ordinal indicator, masculine */
	"\370", /* small o, slash */
	"\365", /* small o, tilde */
	"\366", /* small o, dieresis or umlaut mark */
	"\266", /* pilcrow (paragraph sign) */
	"\261", /* plus-or-minus sign */
	"\243", /* pound sterling sign */
	"\042", /* double quote sign - June 94 */
	"\273", /* angle quotation mark, right */
	"\256", /* registered sign */
	"\247", /* section sign */
	"\255", /* soft hyphen */
	"\271", /* superscript one */
	"\262", /* superscript two */
	"\263", /* superscript three */
	"\337", /* small sharp s, German (sz ligature) */
	"\376", /* small thorn, Icelandic */
	"\327", /* multiply sign */
	"\215", /* trademark sign */
	"\372", /* small u, acute accent */
	"\373", /* small u, circumflex accent */
	"\371", /* small u, grave accent */
	"\250", /* umlaut (dieresis) */
	"\374", /* small u, dieresis or umlaut mark */
	"\375", /* small y, acute accent */
	"\245", /* yen sign */
	"\377" /* small y, dieresis or umlaut mark */
};

#endif

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
    char *ap = (char *) ain, **bp = (char **)bin;

    PRSDBGN(("entity_compare_function('%s', '%s')\n", ap, *bp));

    return strncasecomp( ap, *bp, strlen(*bp));
}

/* FIXME: THESE NEED TO ALTER THE BEHAVIOUR */
#if 0
#define SGMLTRANS_WARNINGS	16 /* Send warnings down sgml_note_message() */
#define SGMLTRANS_STRICT	32 /* Remove invalid translations */
#endif

extern int sgml_translation(SGMLCTX *context, char *in_ptr, int in_bytes, int rules)
{
#if DEBUG
    char *orig_ptr = in_ptr;
#endif
    char *out_ptr;
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
	const char c = *in_ptr++;

	if ((c == '\n' || c == '\r') && (rules & SGMLTRANS_STRIP_NEWLINES) != 0)
	{
            /* throw them away */
	    used = TRUE;
	}
	if (c == '+' && (rules & SGMLTRANS_PLUS_TO_SPACE) != 0)
	{
	    *out_ptr++ = ' ';
	    out_bytes++;
	    used = TRUE;
	}
	else if (c == '%' && (rules & SGMLTRANS_PERCENT) != 0)
	{
	    if (in_bytes >= 2)
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
#if 0
			PRSDBG(("Recognised as '%c'\n", x));
#endif
		    }
		}
	    }

#if SGML_REPORTING
	    if ( !used && (rules & SGMLTRANS_WARNINGS) != 0 )
		sgml_note_message(context,
				  "Insufficient characters to expand %%XX sequence");
#endif
	}
	else if (c == '&' &&
		 (rules & SGMLTRANS_HASH) != 0 &&
		 in_bytes > 1 &&
		 (in_ptr[0] == '#'/*  || isdigit(in_ptr[0]) */))
	    /* SJM: 30/09/97. Don't allow numbers without a hash. I
	       don't know why I put this in in the first place but
	       NS doesn't do it */
	{
	    char *end;
	    long x;

	    PRSDBG(("Trying to do numeric entity\n"));

	    if (in_ptr[0] == '#')
	    {
		in_bytes--;
		in_ptr++;
	    }

	    x = in_bytes >= 1 ? strtol(in_ptr, &end, 10) : (end = in_ptr, -1);

	    if (end != in_ptr &&
		x > 0 &&
		x != 127 &&
		x < 256 &&
		(x >= 32 || isspace((char)x))
		)
	    {
		if (gbf_active(GBF_TRANSLATE_UNDEF_CHARS))
		    x = convert_undefined_key_code((int)x);

		*out_ptr++ = (char) x;
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
		BOOL upper_case = isupper(in_ptr[0]);
		char **matchp = bsearch(in_ptr,
					entity_names,
					sizeof(entity_names) / sizeof(entity_names[0]),
					sizeof(entity_names[0]),
					entity_compare_function);

		if (matchp != NULL)
		{
		    const int len = strlen( *matchp );
		    in_ptr += len;
		    in_bytes -= len;
		    if (in_bytes > 0 && *in_ptr == ';')
		    {
			in_ptr++;
			in_bytes--;
		    }
		    *out_ptr++ = ISO_Latin1[ (matchp - (char**)(entity_names))*2 + (upper_case ? 1 : 0) ];
		    out_bytes++;
		    used = TRUE;
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
    const int new_entity = sgml_translation(context,
				     context->inhand.data,
				     context->inhand.ix,
/*				     SGMLTRANS_PERCENT | */ /* Precent shouldn't be expanded in HTML I don't think*/
				     SGMLTRANS_AMPERSAND |
				     SGMLTRANS_HASH |
				     SGMLTRANS_WARNINGS);
    context->inhand.ix = new_entity;
    push_inhand(context);
}

/* eof entities.c */

