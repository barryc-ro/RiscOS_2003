/* gbf.c -*-C-*- Global Behaviour Flags
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 * In the long run, this probably wants to exist in !Fresco and !NCFresco.
 * For now, let's just get NCFresco going and let the desktop version have
 * the same behaviour. Once we've got a release done, we can improve this.
 */

#include "htmlparser.h"
#include "gbf.h"

/*****************************************************************************/

#if defined(STBWEB)
int gbf_flags =	( GBF_TABLES_UNEXPECTED * 1	) +
		( GBF_FVPR * 1			) +
		( GBF_GUESS_ELEMENTS * 0	) +
		( GBF_GUESS_ATTRIBUTES * 1	) +
		( GBF_GUESS_ENUMERATIONS * 1	) +
		( GBF_TRANSLATE_UNDEF_CHARS * 0 ) +
		( GBF_NEW_FORMATTER * 1		) +
		( GBF_AUTOFIT * 1		) +
		( GBF_AUTOFIT_ALL_TEXT * 1	) +
		( GBF_NETSCAPE_OVERLAPS * 0	) +
		( GBF_HARD_TABLES * 0		) +
		( GBF_EARLYIMGFETCH * 0		) +
		( GBF_LOW_MEMORY * 0		) +
		( GBF_ANTI_TWITTER * 1		) +
		( GBF_SI1_PCT * 1		) +
		( GBF_CAPTIONS * 0		) +
		( GBF_RELATIVE_TABLE * 0	) +
		( GBF_MINWIDTH_A * 0		) +
		( GBF_MINWIDTH_B * 1		) +
		( GBF_MINWIDTH_C * 0		) +
		( GBF_MINWIDTH_D * 0		)
;

#elif defined(BUILDERS)
int gbf_flags =	( GBF_TABLES_UNEXPECTED * 1	) +
		( GBF_FVPR * 0			) +
		( GBF_GUESS_ELEMENTS * 0	) +
		( GBF_GUESS_ATTRIBUTES * 1	) +
		( GBF_GUESS_ENUMERATIONS * 1	) +
		( GBF_TRANSLATE_UNDEF_CHARS * 0 ) +
		( GBF_NEW_FORMATTER * 1		) +
		( GBF_AUTOFIT * 0		) +
		( GBF_AUTOFIT_ALL_TEXT * 0	) +
		( GBF_NETSCAPE_OVERLAPS * 0	) +
		( GBF_HARD_TABLES * 0		) +
		( GBF_EARLYIMGFETCH * 0		) +
		( GBF_LOW_MEMORY * 0		) +
		( GBF_ANTI_TWITTER * 0		) +
		( GBF_SI1_PCT * 1		) +
		( GBF_CAPTIONS * 0		) +
		( GBF_FILES_IN_ONE_GO * 0       ) +
		( GBF_RELATIVE_TABLE * 0	) +
		( GBF_MINWIDTH_A * 0		) +
		( GBF_MINWIDTH_B * 1		) +
		( GBF_MINWIDTH_C * 0		) +
		( GBF_MINWIDTH_D * 0		)
;

#elif defined(FRESCO) && !defined(PRODUCTION)
/* Desktop fresco: no fvpr, no autofit */
int gbf_flags =	( GBF_TABLES_UNEXPECTED * 1	) +
		( GBF_FVPR * 0			) +
		( GBF_GUESS_ELEMENTS * 0	) +
		( GBF_GUESS_ATTRIBUTES * 1	) +
		( GBF_GUESS_ENUMERATIONS * 1	) +
		( GBF_TRANSLATE_UNDEF_CHARS * 0 ) +
		( GBF_NEW_FORMATTER * 1		) +
		( GBF_AUTOFIT * 0		) +
		( GBF_AUTOFIT_ALL_TEXT * 0	) +
		( GBF_NETSCAPE_OVERLAPS * 0	) +
		( GBF_HARD_TABLES * 0		) +
		( GBF_EARLYIMGFETCH * 0		) +
		( GBF_LOW_MEMORY * 0		) +
		( GBF_ANTI_TWITTER * 0		) +
		( GBF_SI1_PCT * 1		) +
		( GBF_CAPTIONS * 0		) +
		( GBF_FILES_IN_ONE_GO * 1       ) +
		( GBF_RELATIVE_TABLE * 0	) +
		( GBF_MINWIDTH_A * 0		) +
		( GBF_MINWIDTH_B * 1		) +
		( GBF_MINWIDTH_C * 0		) +
		( GBF_MINWIDTH_D * 0		)
;

#endif /* Build variant switch */

/*****************************************************************************/

#if ! ( defined(FRESCO) && defined(PRODUCTION) )

extern int gbf_active(int gbf)
{
    return gbf_flags & gbf;
}

#endif /* Whether want runtime gbf_action() facilities */

/*****************************************************************************/

/* DAF: 970320: Sorry about this! */

extern void gbf_init(void)
{
    if (gbf_active(GBF_TABLES_UNEXPECTED))
    {
	elements[HTML_TABLE].flags &= ~ FLAG_NO_TEXT;
	elements[HTML_TH].flags &= ~ FLAG_TEXT;
	elements[HTML_TD].flags &= ~ FLAG_TEXT;
	elements[HTML_BADTH].flags &= ~ FLAG_TEXT;
	elements[HTML_BADTD].flags &= ~ FLAG_TEXT;
    }
    else
    {
	elements[HTML_TABLE].flags |=  FLAG_NO_TEXT;
	elements[HTML_TH].flags |= FLAG_TEXT;
	elements[HTML_TD].flags |= FLAG_TEXT;
	elements[HTML_BADTH].flags |= FLAG_TEXT;
	elements[HTML_BADTD].flags |= FLAG_TEXT;
    }
}

/*****************************************************************************/

/* eof */





