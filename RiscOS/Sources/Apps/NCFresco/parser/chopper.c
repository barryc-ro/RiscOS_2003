/* chopper.c - split text stream into whitespace and words */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

#include "sgmlparser.h"
#if UNICODE
# include "unictype.h"
#endif

/*****************************************************************************

    Receive a sequence of characters as input.  Generate a series of "chopped
    up" character sequences as output.  Each sequence is either a word
    containing no whitespace characters or a sequence of just whitespace
    characters.  Supplying an empty string flushes any buffered characters.

    Different chopping modes are supported.  The primary mode (fmt) is non
    whitespace sequences and whitespace sequences. Pre mode treats only end
    of line type characters (CR and LF) as whitespace and other
    normally whitespace characters as part of the word - ie get delivered
    entire lines rather than entire words. Tabs needs scanning for when
    operating in premode. str mode simply passes arriving characters
    onwards - it's used for things like <TITLE>.

    The STRINGs generated need to have their associated memory freed once
    finished with. The STRING supplied is not freed by the chopper routines.

    13th May 1996. Borris. Spruced up for main Fresco inclusion.
    12th Jun 1996. Borris. Revised definitions.

*/

#define TIDY(c)	    ( ( ((c)<32 && (c)!=9) || (c)==127 ) ? '.' : (c) )

/*****************************************************************************

  End Of Line State Machine. This has no relevance to normal
  operation. It's purpose is to keep a best-guess track of the current
  line count for error reporting. IF no error reporting is being performed,
  calls to here can be safely omitted.

  */

static void eol_sm(sgml_chopper_state *st, UCHARACTER c, int *line)
{
    /* end of line spotting and counting state machine */
    switch (st->s2)
    {
    case 0:			/* No state */
	if (c == '\n')		/* LF convention, CR ignored */
	{
	    st->s2 = 1;
	    *line += 1;
	}
	else if (c == '\r')	/* CR convention, LF ignored */
	{
	    st->s2 = 2;
	    *line += 1;
	}
	break;			/* Else wait to can tell */

    case 1:
	if (c == '\n')
	    *line += 1;
	break;

    case 2:
	if (c == '\n')
	    *line += 1;
	break;
    }
}

/*****************************************************************************

  Normal formatted (fmt) mode word chopper. Any character less than 33
  is considered to be whitespace, although we track end of line type
  characters so that we can report a line number with errors and
  warnings. This delivers either DELIVER_WORD or DELIVER_SPACE.

 */

#if UNICODE

/*
 * s1 = chop_state
 * s2 = eol state
 * s3 = was last non-space char ideographic
 * s4 = was last char nobreaking
 */

typedef enum
{
    UNDECIDED,			/* first three as old state machine */
    LETTER,
    SPACE,
    PSTART,
    PEND,
    MARK
} chop_state;

static chop_state decode_ctype(int ctype)
{
    switch (ctype)
    {
    case unictype_SEPARATOR_SPACE:
    case unictype_SEPARATOR_PARA:
	return SPACE;
    case unictype_PUNCTUATION_OPEN:
	return PSTART;
    case unictype_PUNCTUATION_CLOSE:
	return PEND;
    case unictype_PUNCTUATION_DASH:
    case unictype_UNKNOWN:
    case unictype_LETTER:
    case unictype_NUMBER:
    case unictype_SYMBOL:
    case unictype_OTHER:
	return LETTER;
    case unictype_MARK:
	return MARK;
    }
    return LETTER;
}

extern void sgml_fmt_word_chopper(SGMLCTX *context, USTRING input)
{
    sgml_chopper_state *st = &context->chopper_state;
    int ix;

    /*PRSDBGN(("sgml_fmt_word_chopper(): %.*s\n", input.nchars, input.ptr));*/

    if (input.nchars == 0)
    {
	/*PRSDBGN(("sgml_fmt_word_chopper(): flushing\n"));*/

	switch ((chop_state)st->s1)
	{
	case UNDECIDED:
	    break;

	case LETTER:
	case PSTART:
	case PEND:
	case MARK:
	    ASSERT(context->prechop.ix > 0);

	    (*context->deliver)
		(
		    context,
		    st->s4 ? DELIVER_WORD_NOBREAK : DELIVER_WORD,
		    mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix),
		    NULL
		    );
	    break;

	case SPACE:
	    if (!st->s3)
		context->deliver(context, DELIVER_SPACE, space_string, NULL);
	    break;
	}

	context->prechop.ix = 0;
	st->s1 = UNDECIDED;
	return;
    }

    for (ix = 0; ix < input.nchars; ix++)
    {
	UCHARACTER c = input.ptr[ix];
	int ctype;		/* the type of this character */
	chop_state new_state;	/* the state after this character */
	BOOL deliver = FALSE;	/* shall er deliver the inhand data */
	BOOL ideo;		/* is this character ideographic */
	BOOL nobreak = FALSE;	/* we'd like this letter to stick to the last one */
	
	eol_sm(st, c, &context->line);

	ctype = unictype_lookup((UCS2)c);
	new_state = decode_ctype(ctype);
	ideo = unictype_is_ideograph((UCS2)c);


	if (new_state == SPACE)
	{
	    if (c > ' ')	/* SPACE > ' ' means nbsp or ideographic so maintain */
		new_state = LETTER;
	    else
		c = ' ';
	}

	/* if changing between ideo and non-ideo then always create new word */
	if (st->s1 != SPACE && new_state != SPACE)
	{
	    if (ideo != st->s3)
		deliver = TRUE;
	}

	/* then check for whether word splitting should occur */
	switch ((chop_state)st->s1)
	{
	case UNDECIDED:
	    ASSERT(context->prechop.ix == 0);
	    break;

	case LETTER:			/* in a word */
	    switch (new_state)
	    {
	    case PSTART:
	    case LETTER:
		if (ideo)
		    deliver = TRUE;
		break;

	    case MARK:
	    case PEND:
		nobreak = TRUE;
		break;
		
	    case SPACE:
		deliver = TRUE;
		break;
	    }
	    break;

	case PSTART:
	    switch (new_state)
	    {
	    case LETTER:
	    case PSTART:
	    case PEND:
	    case MARK:
		nobreak = TRUE;
		break;

	    case SPACE:
		deliver = TRUE;
		break;
	    }
	    break;
	    
	case PEND:
	    switch (new_state)
	    {
	    case LETTER:
	    case PSTART:
		if (ideo)
		    deliver = TRUE;
		break;

	    case MARK:
	    case PEND:
		nobreak = TRUE;
		break;

	    case SPACE:
		deliver = TRUE;
		break;
	    }
	    break;
	    
	case MARK:
	    switch (new_state)
	    {
	    case LETTER:
	    case PSTART:
		if (ideo)
		    deliver = TRUE;
		break;

	    case PEND:
	    case MARK:
		nobreak = TRUE;
		break;

	    case SPACE:
		deliver = TRUE;
		break;
	    }
	    break;
	    
	case SPACE:
	    switch (new_state)
	    {
	    case LETTER:
	    case PSTART:
	    case PEND:
	    case MARK:
		deliver = TRUE;
		break;

	    case SPACE:
		break;
	    }
	    break;
	}

	if (deliver)
	{
	    /* We only want to deliver this space if either
	     *   The non-space before it was non-ideo, or
	     *   The current char is non-ideo
	     */
	    if ((chop_state)st->s1 == SPACE)
	    {
		if (!ideo || !st->s3)
		    context->deliver(
			context,
			DELIVER_SPACE,
			space_string,
			NULL
			);
	    }
	    else
	    {
		context->deliver(
		    context,
		    st->s4 ? DELIVER_WORD_NOBREAK : DELIVER_WORD,
		    mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix),
		    NULL
		    );
	    }

	    context->prechop.ix = 0;
	}

	st->s1 = (int)new_state;
	if (new_state != SPACE)
	    st->s3 = ideo;
	st->s4 = nobreak;
	
	add_to_prechop_buffer(context, c);
    }
}

#else

extern void sgml_fmt_word_chopper(SGMLCTX *context, USTRING input)
{
    sgml_chopper_state *st = &context->chopper_state;
    int ix;

    /*PRSDBGN(("sgml_fmt_word_chopper(): %.*s\n", input.nchars, input.ptr));*/

    if (input.nchars == 0)
    {
	/*PRSDBGN(("sgml_fmt_word_chopper(): flushing\n"));*/

	switch (st->s1)
	{
	case 0:
	    break;

	case 1:
	    ASSERT(context->prechop.ix > 0);

	    (*context->deliver)
		(
		    context,
		    DELIVER_WORD,
		    mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix),
		    NULL
		    );
	    break;

	case 2:
	    context->deliver(context, DELIVER_SPACE, space_string, NULL);
	    break;
	}

	context->prechop.ix = 0;
	st->s1 = 0;
	return;
    }

    for (ix = 0; ix < input.nchars; ix++)
    {
	UCHARACTER c = input.ptr[ix];
	const BOOL ws = c < 33 || c == 127;

	eol_sm(st, c, &context->line);

	/* Having handled EOL, which whitespace character we have */
	/* no longer matters - it's just a word seperator now. */

	switch (st->s1)
	{
	case 0:			/* no state, decide */
	    ASSERT(context->prechop.ix == 0);
	    st->s1 = ws ? (c = ' ', 2 /* space */) : 1 /* word */;
	    break;

	case 1:			/* in a word */
	    if ( ws )
	    {
		(*context->deliver)
		    (
			context,
			DELIVER_WORD,
			mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix),
			NULL
			);
		st->s1 = 2;	/* space */
		context->prechop.ix = 0;
		c = ' ';
	    }
	    break;

	case 2:			/* in a space */
	    if ( ! ws )
	    {
		(*context->deliver)
		    (
			context,
			DELIVER_SPACE,
			space_string,
			NULL
			);
		st->s1 = 1;
		context->prechop.ix = 0;
	    }
	    break;
	}

	add_to_prechop_buffer(context, c);
    }
}
#endif

/*****************************************************************************

  PRE formatting mode word chopper. In this mode, we are not
  interested in words and seperators.  Rather, we are interested in
  retaining whitespace precisely and knowing where line ends are.

  We need to know the end of line convention. WE then DELIVER_WORD for
  the contents of a line and DELIVER_EOL for the end of line.

  */

extern void sgml_pre_word_chopper(SGMLCTX *context, USTRING input)
{
    sgml_chopper_state *st = &context->chopper_state;
    int ix;

    PRSDBGN(("sgml_pre_word_chopper(): '%.*s' (%d,%d,%d,%d)\n",
	     input.nchars, input.ptr, st->s1,st->s2,st->s3,st->s4 ));

    if (input.nchars == 0)
    {
	PRSDBGN(("sgml_pre_word_chopper(): flushing\n"));

	switch (st->s1)
	{
	case 0:			/* Deciding - nothing to do */
	    break;

	case 1:			/* EOL */
	case 2:
	    context->deliver ( context, DELIVER_EOL, eol_string, NULL );
	    break;

	case 3:
	    context->deliver ( context, DELIVER_WORD, mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix), NULL );
	    break;
	}

	context->prechop.ix = 0;
	st->s1 = 0;

	return;
    }

    for (ix = 0; ix < input.nchars && !context->pending_close; ix++) /* SJM: added check for close pending */
    {
	UCHARACTER c = input.ptr[ix];
	BOOL discard = FALSE;

	eol_sm(st, c, &context->line);

	switch (st->s1)
	{
	case 0:
	    ASSERT(context->prechop.ix == 0);

	    if (c == '\r')
	    {
		st->s1 = 1;
		discard = TRUE;
	    }
	    else if (c == '\n')
	    {
		st->s1 = 2;
		discard = TRUE;
	    }
	    else
	    {
		st->s1 = 3;
	    }
	    break;

	case 1:		/* Seen CR */
	    PRSDBGN(("context->prechop.ix == %d", context->prechop.ix == 0));
	    ASSERT(context->prechop.ix == 0);
	    if (c == '\r')
	    {
		/* Deliver existing EOL, (Mac) start new one, state stays same */
		(*context->deliver) ( context, DELIVER_EOL, eol_string, NULL );
		context->prechop.ix = 0;
		discard = TRUE;
	    }
	    else if (c == '\n')
	    {
		/* Now got CRLF (MSDOS) so deliver ONE EOL and reset */
		(*context->deliver) ( context, DELIVER_EOL, eol_string, NULL );
		context->prechop.ix = 0;
		st->s1 = 0;
		discard = TRUE;
	    }
	    else
	    {
		/* Only CR (Mac). Deliver and start new word */
		(*context->deliver) ( context, DELIVER_EOL, eol_string, NULL );
		context->prechop.ix = 0;
		st->s1 = 3;
	    }
	    break;

	case 2:		/* Seen LF */
	    PRSDBGN(("context->prechop.ix = %d\n", context->prechop.ix));
	    ASSERT(context->prechop.ix == 0);
	    if (c == '\n')
	    {
		/* Deliver existing EOL, (UNIX, RISC OS) start new one, state stays same */
		(*context->deliver) ( context, DELIVER_EOL, eol_string, NULL );
		context->prechop.ix = 0;
		discard = TRUE;
	    }
	    else if (c == '\r')
	    {
		/* Now got LFCR (unknown) so deliver ONE EOL and reset */
		/* Might happen with an LF doc partially editted on MSDOS? */
		(*context->deliver) ( context, DELIVER_EOL, eol_string, NULL );
		context->prechop.ix = 0;
		st->s1 = 0;
		discard = TRUE;
	    }
	    else
	    {
		/* Only LF (UNIX, RISC OS). Deliver and start new word */
		(*context->deliver) ( context, DELIVER_EOL, eol_string, NULL );
		context->prechop.ix = 0;
		st->s1 = 3;
	    }
	    break;

	case 3:			/* Seen not EOL character */
	    ASSERT(context->prechop.ix > 0);
	    if (c == '\r')
	    {
		(*context->deliver) ( context, DELIVER_WORD, mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix), NULL );
		context->prechop.ix = 0;
		st->s1 = 1;
		discard = TRUE;
	    }
	    else if (c == '\n')
	    {
		(*context->deliver) ( context, DELIVER_WORD, mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix), NULL );
		context->prechop.ix = 0;
		st->s1 = 2;
		discard = TRUE;
	    }
	    break;
	}

	if (! discard)
	    add_to_prechop_buffer(context, TIDY(c));
    }
#if DEBUG
    if (context->pending_close)
    {
	PRSDBG(("sgml_pre_word_chopper(%p): aborted after %d of %d nchars\n", context, ix, input.nchars));
    }
#endif
}

/*****************************************************************************

  The basic string word chopper. We still need to direct characters
  through the EOL stuff if we want the number to be accurate (all
  chopped characters need to do this in fact).  Apart from that, we
  just hand the group of characters received straight on, except that
  we remove translate control characters to spaces. We should never
  have anything buffered here.

*/

extern void sgml_str_word_chopper(SGMLCTX *context, USTRING input)
{
    sgml_chopper_state *st = &context->chopper_state;
    int ix;

    /*PRSDBGN(("sgml_str_word_chopper(): %.*s\n", input.nchars, input.ptr));*/

    if (input.nchars == 0)
    {
	/* Flush actions */

	ASSERT(context->prechop.ix == 0);

	return;
    }

    context->prechop.ix = 0;

    for (ix = 0; ix < input.nchars; ix++)
    {
	const UCHARACTER c = input.ptr[ix];

	eol_sm(st, c, &context->line);

	/* This might well want spaces stripping */

        /* pdh: translate tabs to spaces here */
        /* sjm: removed translation as that will prevent tab expansion later */
	if (c != 0)
	    add_to_prechop_buffer(context, c);
    }

    (*context->deliver) ( context, DELIVER_WORD, mkstringu(context->encoding_write, context->prechop.data, context->prechop.ix), NULL );

    context->prechop.ix = 0;
}

/*****************************************************************************/

extern void sgml_reset_chopper_state(SGMLCTX *context)
{
    PRSDBGN(("sgml_reset_chopper_state(%p): ->prechop.ix %d\n", context, context->prechop.ix));

    context->chopper_state.s1 =
	context->chopper_state.s2 =
	context->chopper_state.s3 =
	context->chopper_state.s4 = 0;

    context->prechop.ix = 0;
}

/*****************************************************************************/

/* eof chopper.c */
