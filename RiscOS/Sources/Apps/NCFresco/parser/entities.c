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

/** THIS TABLE MUST BE IN strcasecmp() ORDER FOR THIS TO WORK **/
/** ALSO, NO NAME MAY ALSO BE A STEM OF ANOTHER NAME */
/** also, no name may only differ in case from another */

typedef struct
{
    const char *name, *name_upper;
    UCHARACTER lower, upper;	/* maybe char or short depending on UNICODE */
    char match;
} entity_t;

#define case_EXACT		0	/* must match the case exactly always, reserved for some known future entities */
#define case_IRRELEVANT		1	/* exact=must match as is */
#define case_UPPER_FIRST	2	/* first char determines case of result. exact=rest oif chars must be lower case also */
#define case_UPPER_ALL		3	/* first char determines case of result. exact=rest of chars must match that case */

static entity_t entity_names[] =
{
  {
      "aacute",	NULL,	/* small a, acute accent */
      '\341', '\301',	/* small a, acute accent */
      case_UPPER_FIRST
  },
  {
      "acirc",	NULL,	/* small a, circumflex accent */
      '\342', '\302',	/* small a, circumflex accent */
      case_UPPER_FIRST
  },
  {
      "acute",	NULL,	/* acute accent */
      '\264', '\264',	/* acute accent */
      case_IRRELEVANT
  },
  {
      "aelig",	"AElig",/* small ae diphthong (ligature) */
      '\346', '\306',	/* small ae diphthong (ligature) */
      case_UPPER_FIRST
  },
  {
      "agrave",	NULL,	/* small a, grave accent */
      '\340', '\300',	/* small a, grave accent */
      case_UPPER_FIRST
  },
  {
      "amp", NULL,      	/* ampersand */
      '\046', '\046',	/* ampersand */
      case_IRRELEVANT
  },
  {
      "aring",	NULL,	/* small a, ring */
      '\345', '\305',	/* small a, ring */
      case_UPPER_FIRST
  },
  {
      "atilde",	NULL,	/* small a, tilde */
      '\343', '\303',	/* small a, tilde */
      case_UPPER_FIRST
  },
  {
      "auml",	NULL,	/* small a, dieresis or umlaut mark */
      '\344', '\304',	/* small a, dieresis or umlaut mark */
      case_UPPER_FIRST
  },
  {
      "bdquo",	NULL,	/* double low quote */
#if UNICODE
      8222, 8222,	/* double quote, low */
#else
      150, 150,
#endif
      case_IRRELEVANT
  },
  {
      "brvbar", NULL,    /* broken (vertical) bar */
      '\246', '\246', /* broken (vertical) bar */
      case_IRRELEVANT
  },
  {
      "ccedil", NULL,    /* small c, cedilla */
      '\347', '\307', /* small c, cedilla */
      case_UPPER_FIRST
  },
  {
      "cedil", NULL,     /* cedilla */
      '\270', '\270', /* cedilla */
      case_IRRELEVANT
  },
  {
      "cent", NULL,      /* cent sign */
      '\242', '\242', /* cent sign */
      case_IRRELEVANT
  },
  {
      "copy", NULL,      /* copyright sign */
      '\251', '\251', /* copyright sign */
      case_IRRELEVANT
  },
  {
      "curren", NULL,    /* general currency sign */
      '\244', '\244', /* general currency sign */
      case_IRRELEVANT
  },
  {
      "dagger", NULL,
#if UNICODE
      8224, 8225,	/* dagger, Dagger */
#else
      156, 157,	/* dagger, Dagger */
#endif
      case_UPPER_FIRST
  },
  {
      "deg", NULL,       /* degree sign */
      '\260', '\260', /* degree sign */
      case_IRRELEVANT
  },
  {
      "divide", NULL,    /* divide sign */
      '\367', '\367', /* divide sign */
      case_IRRELEVANT
  },
  {
      "eacute", NULL,    /* small e, acute accent */
      '\351', '\311', /* small e, acute accent */
      case_UPPER_FIRST
  },
  {
      "ecirc", NULL,     /* small e, circumflex accent */
      '\352', '\312', /* small e, circumflex accent */
      case_UPPER_FIRST
  },
  {
      "egrave", NULL,    /* small e, grave accent */
      '\350', '\310', /* small e, grave accent */
      case_UPPER_FIRST
  },
  {
      "eth", "ETH",	/* small eth, Icelandic */
      '\360', '\320',	/* small eth, Icelandic */
      case_UPPER_ALL
  },
  {
      "euml", NULL,      /* small e, dieresis or umlaut mark */
      '\353', '\313', /* small e, dieresis or umlaut mark */
      case_UPPER_FIRST
  },
  {
      "frac12", NULL,    /* fraction one-half */
      '\275', '\275', /* fraction one-half */
      case_IRRELEVANT
  },
  {
      "frac14", NULL,    /* fraction one-quarter */
      '\274', '\274', /* fraction one-quarter */
      case_IRRELEVANT
  },
  {
      "frac34", NULL,    /* fraction three-quarters */
      '\276', '\276', /* fraction three-quarters */
      case_IRRELEVANT
  },
  {
      "gt", NULL,       	/* greater than */
      '\076', '\076',	/* greater than */
      case_IRRELEVANT
  },
  {
      "hellip", NULL,  	/* horizontal ellipsis */
#if UNICODE
      0x2026, 0x2026,
#else
      140, 140,
#endif
      case_IRRELEVANT
  },
  {
      "iacute", NULL,    /* small i, acute accent */
      '\355', '\315', /* small i, acute accent */
      case_UPPER_FIRST
  },
  {
      "icirc", NULL,     /* small i, circumflex accent */
      '\356', '\316', /* small i, circumflex accent */
      case_UPPER_FIRST
  },
  {
      "iexcl", NULL,     /* inverted exclamation mark */
      '\241', '\241', /* inverted exclamation mark */
      case_IRRELEVANT
  },
  {
      "igrave", NULL,    /* small i, grave accent */
      '\354', '\314', /* small i, grave accent */
      case_UPPER_FIRST
  },
  {
      "iquest", NULL,    /* inverted question mark */
      '\277', '\277', /* inverted question mark */
      case_IRRELEVANT
  },
  {
      "iuml", NULL,      /* small i, dieresis or umlaut mark */
      '\357', '\317', /* small i, dieresis or umlaut mark */
      case_UPPER_FIRST
  },
  {
      "laquo", NULL,     /* angle quotation mark, left */
      '\253', '\253', /* angle quotation mark, left */
      case_IRRELEVANT
  },
  {
      "ldquo",	NULL,/* double quote, left */
#if UNICODE
      8220, 8220,	/* double quote, left */
#else
      148, 148,	/* double quote, left */
#endif
      case_IRRELEVANT
  },
  {
      "lsaquo", NULL,    /* single angle quotation mark, left */
#if UNICODE
      8249, 8249,	/* single angle quotation mark, left */
#else
      146, 146,	/* single angle quotation mark, left */
#endif
      case_IRRELEVANT
  },
  {
      "lsquo", NULL,/* single quote, left */
#if UNICODE
      8216, 8216,	/* single quote, left */
#else
      144, 144,	/* single quote, left */
#endif
      case_IRRELEVANT
  },
  {
      "lt", NULL,       	/* greater than */
      '\074', '\074',	/* less than */
      case_IRRELEVANT
  },
  {
      "macr", NULL,      /* macron */
      '\257', '\257', /* macron */
      case_IRRELEVANT
  },
  {
      "mdash",	NULL,/* mdash */
#if UNICODE
      8212, 8212,	/* mdash */
#else
      152, 152,
#endif
      case_IRRELEVANT
  },
  {
      "micro", NULL,     /* micro sign */
      '\265', '\265', /* micro sign */
      case_IRRELEVANT
  },
  {
      "middot", NULL,    /* middle dot */
      '\267', '\267', /* middle dot */
      case_IRRELEVANT
  },
  {
      "nbsp", NULL,      /* no-break space */
      '\240', '\240', /* no-break space */
      case_IRRELEVANT
  },
  {
      "ndash",	NULL,/* en dash */
#if UNICODE
      8211, 8211,	/* en dash */
#else
      153, 153,
#endif
      case_IRRELEVANT
  },
  {
      "not", NULL,       /* not sign */
      '\254', '\254', /* not sign */
      case_IRRELEVANT
  },
  {
      "ntilde", NULL,    /* small n, tilde */
      '\361', '\321', /* small n, tilde */
      case_UPPER_FIRST
  },
  {
      "oacute", NULL,    /* small o, acute accent */
      '\363', '\323', /* small o, acute accent */
      case_UPPER_FIRST
  },
  {
      "ocirc", NULL,     /* small o, circumflex accent */
      '\364', '\324', /* small o, circumflex accent */
      case_UPPER_FIRST
  },
  {
      "oelig", "OElig",  /* small oe diphthong (ligature) */
#if UNICODE
      339, 338,	/* oe, OE */
#else
      155, 154,
#endif
      case_UPPER_FIRST
  },
  {
      "ograve", NULL,    /* small o, grave accent */
      '\362', '\322', /* small o, grave accent */
      case_UPPER_FIRST
  },
  {
      "ordf", NULL,      /* ordinal indicator, feminine */
      '\252', '\252', /* ordinal indicator, feminine */
      case_IRRELEVANT
  },
  {
      "ordm", NULL,      /* ordinal indicator, masculine */
      '\272', '\272', /* ordinal indicator, masculine */
      case_IRRELEVANT
  },
  {
      "oslash", NULL,    /* small o, slash */
      '\370', '\330', /* small o, slash */
      case_UPPER_FIRST
  },
  {
      "otilde", NULL,    /* small o, tilde */
      '\365', '\325', /* small o, tilde */
      case_UPPER_FIRST
  },
  {
      "ouml", NULL,      /* small o, dieresis or umlaut mark */
      '\366', '\326', /* small o, dieresis or umlaut mark */
      case_UPPER_FIRST
  },
  {
      "para", NULL,      /* pilcrow (paragraph sign) */
      '\266', '\266', /* pilcrow (paragraph sign) */
      case_IRRELEVANT
  },
  {
      "permil",	NULL,		/* per mille */
#if UNICODE
      8240, 8240,	/* permille */
#else
      142, 142,
#endif
      case_IRRELEVANT
  },
  {
      "plusmn", NULL,    /* plus-or-minus sign */
      '\261', '\261', /* plus-or-minus sign */
      case_IRRELEVANT
  },
  {
      "pound", NULL,     /* pound sterling sign */
      '\243', '\243', /* pound sterling sign */
      case_IRRELEVANT
  },
  {
      "quot", NULL,      /* double quote sign - June 94 */
      '\042', '\042', /* double quote sign - June 94 */
      case_IRRELEVANT
  },
  {
      "raquo",	NULL,/* angle quotation mark, right */
      '\273', '\273', /* angle quotation mark, right */
      case_IRRELEVANT
  },
  {
      "rdquo",	NULL,/* double quote, right */
#if UNICODE
      8221, 8221,	/* double quote, right */
#else
      149, 149,	/* double quote, right */
#endif
      case_IRRELEVANT
  },
  {
      "reg", NULL,       /* registered sign */
      '\256', '\256', /* registered sign */
      case_IRRELEVANT
  },
  {
      "rsaquo", NULL,    /* single angle quotation mark, right */
#if UNICODE
      8250, 8250,	/* single angle quotation mark, right */
#else
      147, 147,	/* single angle quotation mark, right */
#endif
      case_IRRELEVANT
  },
  {
      "rsquo", NULL, /* single quote, right */
#if UNICODE
      8217, 8217,	/* single quote, right */
#else
      145, 145,	/* single quote, right */
#endif
      case_IRRELEVANT
  },
#if 0				/* can't display this in Latin1 so don't recognize it */
  {
      "scaron",	NULL,	/* s caron */
#if UNICODE
      353, 352,		/* s caron, S caron */
#else
      's', 'S',
#endif
      case_UPPER_FIRST
  },
#endif
  {
      "sect", NULL,      /* section sign */
      '\247', '\247', /* section sign */
      case_IRRELEVANT
  },
  {
      "shy", NULL,       /* soft hyphen */
      '\255', '\255', /* soft hyphen */
      case_IRRELEVANT
  },
  {
      "sup1", NULL,      /* superscript one */
      '\271', '\271', /* superscript one */
      case_IRRELEVANT
  },
  {
      "sup2", NULL,      /* superscript two */
      '\262', '\262', /* superscript two */
      case_IRRELEVANT
  },
  {
      "sup3", NULL,      /* superscript three */
      '\263', '\263', /* superscript three */
      case_IRRELEVANT
  },
  {
      "szlig", NULL,     /* small sharp s, German (sz ligature) */
      '\337', '\337', /* small sharp s, German (sz ligature) */
      case_UPPER_FIRST
  },
  {
      "thorn", "THORN", /* small thorn, Icelandic */
      '\376', '\336',	/* small thorn, Icelandic */
      case_UPPER_ALL
  },
  {
      "times", NULL,     /* multiply sign */
      '\327', '\327', /* multiply sign */
      case_IRRELEVANT
  },
  {
      "trade", NULL,     /* trademark sign */
#if UNICODE
      8482, 8482,	/* trademark sign */
#else
      '\215', '\215', /* trademark sign */
#endif
      case_IRRELEVANT
  },
  {
      "uacute", NULL,    /* small u, acute accent */
      '\372', '\332', /* small u, acute accent */
      case_UPPER_FIRST
  },
  {
      "ucirc", NULL,     /* small u, circumflex accent */
      '\373', '\333', /* small u, circumflex accent */
      case_UPPER_FIRST
  },
  {
      "ugrave", NULL,    /* small u, grave accent */
      '\371', '\331', /* small u, grave accent */
      case_UPPER_FIRST
  },
  {
      "uml", NULL,       /* umlaut (dieresis) */
      '\250', '\250', /* umlaut (dieresis) */
      case_IRRELEVANT
  },
  {
      "uuml", NULL,      /* small u, dieresis or umlaut mark */
      '\374', '\334',	/* small u, dieresis or umlaut mark */
      case_UPPER_FIRST
  },
  {
      "yacute", NULL,   /* small y, acute accent */
      '\375', '\335',	/* small y, acute accent */
      case_UPPER_FIRST
  },
  {
      "yen", NULL,      /* yen sign */
      '\245', '\245',	/* yen sign */
      case_IRRELEVANT
  },
  {
      "yuml", NULL,      /* small y, dieresis or umlaut mark */
#if UNICODE
      '\377', 376,	/* small y, dieresis or umlaut mark */
#else
      '\377', 'Y',	/* small y, dieresis or umlaut mark (no capital) */
#endif
      case_UPPER_FIRST
  }
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

static UCHARACTER pc_translate_keys[32] =
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

UCHARACTER convert_undefined_key_code(UCHARACTER c)
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
    const entity_t *bp = (const entity_t *)bin;

    PRSDBGN(("entity_compare_function('%s', '%s')\n", ap, bp->name));

    return strnicmpu( ap, bp->name, strlen(bp->name));
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

	    /* strip hash char */
	    in_bytes--;
	    in_ptr++;

	    base = 10;
	    if (in_bytes > 0 && in_ptr[0] == 'x')
	    {
		in_bytes--;
		in_ptr++;
		base = 16;
	    }

	    x = in_bytes >= 1 ? ustrtol(in_ptr, &end, base) : (end = in_ptr, -1);

	    PRSDBG(("got entity '0x%lx' in %p end %p\n", x, in_ptr, end));

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
		BOOL upper_case = FALSE;
		const entity_t *matchp = bsearch(in_ptr,
					entity_names,
					sizeof(entity_names) / sizeof(entity_names[0]),
					sizeof(entity_names[0]),
					entity_compare_function);

		if (matchp != NULL)
		{
		    if ((rules & SGMLTRANS_STRICT) || matchp->match == case_EXACT)
		    {
			if (strcmpu(in_ptr, matchp->name) == 0)
			    upper_case = FALSE;
			else
			{
			    if (matchp->name_upper)
			    {
				if (strcmpu(in_ptr, matchp->name_upper) == 0)
				    upper_case = TRUE;
				else
				    matchp = NULL;
			    }
			    else if (matchp->match == case_UPPER_FIRST)
			    {
				if (in_ptr[0] < 128 && isupper(in_ptr[0]) && 
				    strcmpu(&in_ptr[1], &matchp->name[1]) == 0)
				    upper_case = TRUE;
				else
				    matchp = NULL;
			    }
			    else
				matchp = NULL;
			}
		    }
		    else
		    {
			switch (matchp->match)
			{
			case case_IRRELEVANT:
			    break;

			case case_UPPER_ALL:
			case case_UPPER_FIRST:
			    upper_case = in_ptr[0] < 128 && isupper(in_ptr[0]);
			    break;
			}
		    }
		}
		
		if (matchp != NULL)
		{
		    int len = strlen( matchp->name );
		    BOOL semicolon = in_bytes > len && in_ptr[len] == ';';

		    if (semicolon || (rules & SGMLTRANS_STRICT) == 0)
		    {
			if (semicolon)
			    len++;

			in_ptr += len;
			in_bytes -= len;

			*out_ptr++ = upper_case ? matchp->upper : matchp->lower;
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

