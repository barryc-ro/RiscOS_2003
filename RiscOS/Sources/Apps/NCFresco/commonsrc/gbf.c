/* gbf.c -*-C-*- Global Behaviour Flags
 *
 * (C) Copyright ANT Limited 1997. All rights reserved.
 *
 */

#include "gbf.h"
#include "htmlparser.h"


int gbf_flags =	( GBF_TABLES_UNEXPECTED * 1 ) +
		( GBF_FVPR * 1 )
;

extern int gbf_active(int gbf)
{
    return gbf_flags & gbf;
}

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

/* eof */
