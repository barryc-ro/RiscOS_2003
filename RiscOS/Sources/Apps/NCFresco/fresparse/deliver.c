/* deliver.c - handle basic token delivery from SGML parser */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

/* This file does the deliver stuff. This receives a stream of tokens, */
/* mainly DELIVER_WORD, DELIVER_SPACE, DELIVER_EOL and DELIVER_MARKUP, */
/* and adds text items appropriatetly to the current text stream. */
/* Hopefully all decisions about what whitespace are held here */

/*****************************************************************************

  Either hang onto the string or free it!

*****************************************************************************/

#include "htmlparser.h"
#include "util.h"
#include "tables.h"





static void append_new(HTMLCTX *htmlctx, int reason, STRING item)
{
    /* These two don't apply anymore? */
    /* FIXME: doesn't do second space if preceeded by period */
    /* FIXME: doesn't do align char splitting */

    STRING t = stringcat(htmlctx->inhand_string, item);

    string_free(&item);
    string_free(&htmlctx->inhand_string);

    htmlctx->inhand_string = t;
    htmlctx->inhand_reason = reason;
}

static void deliver_nop(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    /* General case is now don't need a free action */
    /*nullfree((void**)&item.ptr);*/
}

#define fmt_deliver_nop deliver_nop
#define pre_deliver_nop deliver_nop
#define str_deliver_nop deliver_nop


/*****************************************************************************/
/*                                                                           */
/*                       F M T     M O D E   D E L I V E R Y                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************

  A word has arrived if normal formatting mode. String needs freeing
  at some stage.

  */

static void fmt_deliver_word(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;

    if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
#if UNICODE
	PRSDBGN(("fmt_deliver_word(): word already stashed - push word %s\n", reason == DELIVER_WORD_NOBREAK ? "nobreak" : ""));

#if NEW_BREAKS
	text_item_push_word(htmlctx, reason == DELIVER_WORD_NOBREAK ? rid_break_MUST_NOT : 0, WITHOUT_SPACE);
#else
	text_item_push_word(htmlctx, reason == DELIVER_WORD_NOBREAK ? rid_flag_NO_BREAK : 0, WITHOUT_SPACE);
#endif

	/* No free - data retained for later */
	htmlctx->inhand_string = item;
	htmlctx->inhand_reason = reason;
#else
	/* Could push word with text_item_push_word(htmlctx, rid_flag_NO_BREAK, WITHOUT_SPACE); */
	PRSDBGN(("fmt_deliver_word(): word already stashed - appending\n"));
	/* Freeing done in append_new() */
	append_new(htmlctx, DELIVER_WORD, item);
#endif
    }
    else if (htmlctx->inhand_reason == DELIVER_SPACE)
    {
	/* If have space indicates, can't also have any markup after it and */
	/* before here that could cause space, stripping, so know can just */
	/* push with space. */
	PRSDBGN(("fmt_deliver_word(): word-space already stashed - pushing them\n"));
	text_item_push_word(htmlctx, 0, WITH_SPACE);
	/* No free - data retained for later */
	htmlctx->inhand_string = item;
	htmlctx->inhand_reason = reason;
    }
    else
    {
	PRSDBGN(("fmt_deliver_word(): stashing a new word\n"));
	/* No free - data retained for later */
	htmlctx->inhand_string = item;
	htmlctx->inhand_reason = reason;
    }

    /* Encountering a word takes us out of strip space mode */
    htmlctx->strip_space = FALSE;
}

/*****************************************************************************

  Deliver space in fmt mode. Space is simply a word seperator here,
  and is only recorded in state variables. The word pushing routine
  can append a single space character if asked to. The string should
  not be freed.

 */

static void fmt_deliver_space(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;
	
    if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_SPACE || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
	if (htmlctx->strip_space)
	{
	    PRSDBGN(("fmt_deliver_space(): dropping due to strip_space\n"));
	}
	else
	{
	    PRSDBGN(("fmt_deliver_space(): appending space flag\n"));
	    htmlctx->inhand_reason = DELIVER_SPACE;
	}
    }
    else if ( (htmlctx->last_reason == DELIVER_POST_OPEN_MARKUP ||
	       htmlctx->last_reason == DELIVER_POST_CLOSE_MARKUP) &&
	      ! htmlctx->strip_space)
    {
	/* Handle the space in "Word</tt> Another" correctly */

	rid_text_stream *stream = htmlctx->rh->curstream;

	if (stream != NULL && 
	    stream->text_last != NULL &&
#if NEW_BREAKS
	    GET_BREAK(stream->text_last->flag) != rid_break_MUST
#else
	    (stream->text_last->flag & rid_flag_LINE_BREAK) == 0
#endif
	    )
	{
#if 1
	    /* This bit tries to stop multiple spaces separated only by markup from occurring
	     * Quite possibly not the most efficient way to do it though it seems to do the job.
	     */
	    int reason = DELIVER_SPACE;
	    
	    /* if last item was a word with a space at the end then we drop this space */
	    if (stream->text_last->tag == rid_tag_TEXT)
	    {
#if NEW_BREAKS
		if (stream->text_last->flag & rid_flag_SPACE)
		    reason = DELIVER_NOP;
#else
		char *s;
		flexmem_noshift();

		s = htmlctx->rh->texts.data + ((rid_text_item_text *)stream->text_last)->data_off;
		if (s[strlen(s)-1] == ' ')
		    reason = DELIVER_NOP;

		flexmem_shift();
#endif	    
	    }

	    if (reason == DELIVER_NOP)
	    {
		PRSDBGN(("fmt_deliver_space(): previous word has space so dropping\n"));
		htmlctx->inhand_reason = DELIVER_NOP;
	    }
	    else
	    {
		PRSDBGN(("fmt_deliver_space(): have previous word without line-break flag\n"));
		PRSDBGN(("Setting this to no-break and pushing a space-only word\n"));
		/*stream->text_last->flag |= rid_flag_NO_BREAK;*/
		htmlctx->inhand_reason = DELIVER_SPACE;
	    }
#else
	    PRSDBGN(("fmt_deliver_space(): have previous word without line-break flag\n"));
	    PRSDBGN(("Setting this to no-break and pushing a space-only word\n"));
	    /*stream->text_last->flag |= rid_flag_NO_BREAK;*/
	    htmlctx->inhand_reason = DELIVER_SPACE;
#endif
	}
	else
	{
	    PRSDBGN(("fmt_deliver_space(): can't join space to earlier so dropping\n"));
	    htmlctx->inhand_reason = DELIVER_NOP;
	}
    }
    else
    {
	PRSDBGN(("fmt_deliver_space(): nothing stashed so dropping\n"));
	htmlctx->inhand_reason = DELIVER_NOP;
    }
}

/*****************************************************************************

  Just before the function for the markup. Handle space stripping and
  line breaks for just before the element. The element is being opened.

*/

static void fmt_deliver_pre_open_markup(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;
    BOOL want_break = (elem->flags & FLAG_PRE_BREAK) != 0,
	want_space = (elem->flags & FLAG_STRIP_SPACE_PRE) == 0 && ! htmlctx->strip_space,
	want_nobreak = FALSE;

    if ( (elem->flags & FLAG_BLOCK_LEVEL) != 0 )
    {
	PRSDBGN(("Block level element: %s\n", elem->name.ptr));
	want_break = TRUE;
	want_space = FALSE;
    }
    else if ((htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK) && (elem->flags & FLAG_PRE_BREAK) == 0 )
    {
	want_space = FALSE;
	want_nobreak = TRUE;
    }

    if (elem->flags & FLAG_MANUAL_BREAK) /* SJM:  */
	want_break = FALSE;

    PRSDBGN(("fmt_deliver_pre_open_markup(): want_space %d, want_break %d, want_nobreak %d\n",
	     want_space, want_break, want_nobreak));

    if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_SPACE || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
#if NEW_BREAKS
	text_item_push_word(htmlctx, 
			    (want_break ? rid_break_MUST : want_nobreak ? rid_break_MUST_NOT : 0),
			    want_space ? WITH_SPACE : WITHOUT_SPACE);
	
#else
	text_item_push_word(htmlctx, 
			    (want_break ? rid_flag_LINE_BREAK : 0) |
			    (want_nobreak ? rid_flag_NO_BREAK : 0),
			    want_space ? WITH_SPACE : WITHOUT_SPACE);
#endif
    }

    /* if (want_break || elem->flags & FLAG_BLOCK_LEVEL) != 0) */
    if ((elem->flags & (FLAG_BLOCK_LEVEL | FLAG_MANUAL_BREAK)) == FLAG_BLOCK_LEVEL) /* SJM:  */
	text_item_ensure_break(htmlctx);

    htmlctx->inhand_reason = DELIVER_NOP;

    /* Space stripping from before the markup does not alter */
    /* space stripping after the markup */
    htmlctx->strip_space = FALSE;
}

/*****************************************************************************

  Just before the function for the markup. Handle space stripping and
  line breaks for just before the element. The element is being closed.

*/

static void fmt_deliver_pre_close_markup(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;
    BOOL want_break = (elem->flags & FLAG_PRE_BREAK) != 0,
	want_space = (elem->flags & FLAG_STRIP_SPACE_PRE) == 0 && ! htmlctx->strip_space,
	want_nobreak = FALSE;

    if ( (elem->flags & FLAG_BLOCK_LEVEL) != 0 )
    {
	PRSDBGN(("Block level element: %s\n", elem->name.ptr));
	want_break = FALSE;
	want_space = FALSE;
    }
    else if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
	want_nobreak = TRUE;
	want_space = FALSE;
    }

     /* Borris: Added curstream NULL check - ties up with finishcaption etc */
     if (htmlctx->rh->curstream != NULL && htmlctx->rh->curstream->text_last == NULL)
	want_break = FALSE;

    if (elem->flags & FLAG_MANUAL_BREAK) /* SJM:  */
	want_break = FALSE;

    PRSDBGN(("fmt_deliver_pre_close_markup(%s): want_space %d, want_break %d\n",
	     elem->name.ptr, want_space, want_break));

    if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_SPACE || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
#if NEW_BREAKS
	text_item_push_word(htmlctx, 
			    (want_break ? rid_break_MUST : want_nobreak ? rid_break_MUST_NOT : 0),
			    want_space ? WITH_SPACE : WITHOUT_SPACE);
	
#else
	text_item_push_word(htmlctx, 
			    (want_break ? rid_flag_LINE_BREAK : 0) |
			    (want_nobreak ? rid_flag_NO_BREAK : 0),
			    want_space ? WITH_SPACE : WITHOUT_SPACE);
#endif
    
    htmlctx->inhand_reason = DELIVER_NOP;

    /* Space stripping from before the markup does not alter */
    /* space stripping after the markup */
    htmlctx->strip_space = FALSE;
}

/*****************************************************************************

  Just after the markup function. No space stripping to be performed yet,
  although we might have flag that we want space stripping. The element
  is being opened.

 */

static void fmt_deliver_post_open_markup(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;
    BOOL want_break = 0 /*(elem->flags & FLAG_PRE_BREAK) != 0*/ ,
	want_space = (elem->flags & FLAG_STRIP_SPACE_PRE) == 0 && ! htmlctx->strip_space;

    if ( (elem->flags & FLAG_BLOCK_LEVEL) != 0 )
    {
	want_break = FALSE;
	want_space = FALSE;
    }

    if (elem->flags & FLAG_MANUAL_BREAK) /* SJM:  */
	want_break = FALSE;

    if (want_break && 
	htmlctx->rh->curstream->text_last != NULL && 
	htmlctx->rh->curstream->text_last != htmlctx->rh->curstream->text_list )
    {
	PRSDBG(("fmt_deliver_post_open_markup(): setting rid_flag_LINE_BREAK\n"));
#if NEW_BREAKS
	SET_BREAK(htmlctx->rh->curstream->text_last->flag, rid_break_MUST);
#else
	htmlctx->rh->curstream->text_last->flag |= rid_flag_LINE_BREAK;
#endif
    }

    htmlctx->inhand_reason = DELIVER_NOP;

    /* Set space stripping for subsequent words/spaces */
    htmlctx->strip_space = ! want_space;
}

/*****************************************************************************

  Just after the markup function. No space stripping to be performed yet,
  although we might have flag that we want space stripping. The element is
  being closed.

 */

static void fmt_deliver_post_close_markup(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;
    BOOL want_break = (elem->flags & FLAG_PRE_BREAK) != 0,
	want_space = (elem->flags & FLAG_STRIP_SPACE_PRE) == 0 && ! htmlctx->strip_space;

    if ( (elem->flags & FLAG_BLOCK_LEVEL) != 0 )
    {
	want_break = TRUE;
	want_space = FALSE;
    }

    if (elem->flags & FLAG_MANUAL_BREAK) /* SJM:  */
	want_break = FALSE;

    if (want_break && 
	htmlctx->rh->curstream->text_last != NULL && 
	htmlctx->rh->curstream->text_last != htmlctx->rh->curstream->text_list )
    {
	PRSDBGN(("fmt_deliver_post_close_markup(): setting rid_flag_LINE_BREAK\n"));
#if NEW_BREAKS
	SET_BREAK(htmlctx->rh->curstream->text_last->flag, rid_break_MUST);
#else
	htmlctx->rh->curstream->text_last->flag |= rid_flag_LINE_BREAK;
#endif
    }

    /* if ( (elem->flags & FLAG_BLOCK_LEVEL) != 0 ) */
    /* 	text_item_ensure_break(htmlctx); */
    if ((elem->flags & (FLAG_BLOCK_LEVEL | FLAG_MANUAL_BREAK)) == FLAG_BLOCK_LEVEL) /* SJM:  */
	text_item_ensure_break(htmlctx);

    htmlctx->inhand_reason = DELIVER_NOP;

    /* Set space stripping for subsequent words/spaces */
    htmlctx->strip_space = ! want_space;
}

/*****************************************************************************

  Some general SGML has been found in format mode. To reuse the
  pre/post markup routines, we have an artificial element that we can
  reference. This should work out sensibly. HTML_SGML exists for this
  purpose.

  */

static void fmt_deliver_sgml(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    static STRING s = { NULL, 0 };

    PRSDBGN(("fmt_deliver_sgml(): pretending had <SGML> item '%.*s'\n", item.nchars, item.ptr));

    fmt_deliver_pre_open_markup( context, DELIVER_PRE_OPEN_MARKUP,  s, &context->elements[HTML_SGML] );
    nullfree((void**)&item.ptr);
    fmt_deliver_post_open_markup(context, DELIVER_POST_OPEN_MARKUP, s, &context->elements[HTML_SGML] );
}

/*****************************************************************************

  23rd August 1996. Borris: update of unexpected text stuff. We now
  wander back up the element stack looking for containers (all
  elements that can generate FLAG_NO_TEXT must also have
  FLAG_CONTAINER for now). For each container, and no container, we
  have a text string to supply to pseudo_html() to put us in a
  position that text is acceptable. We then deliver the unexpected
  text in a sensible context. Leading whitespace is removed and may
  causing nothing to happen. The original string supplied is always
  freed.

  */

typedef struct
{
    int element;
    char *insert;
} intstr;

static intstr autofix[] =
{
    { HTML_TABLE, "<td>" },
    { HTML_HTML, "<body>" },
    { HTML_HEAD, "<body>" },
    { HTML_SELECT, "<option>" },
    { SGML_NO_ELEMENT, "<body>" }
};

static void deliver_unexpected(SGMLCTX *context, int reason, STRING orig_item, ELEMENT *elem)
{
    HTMLCTX *me = htmlctxof(context);
    STRING item = orig_item;

    PRSDBGN(("deliver_unexpected(): '%.*s'\n", item.nchars, item.ptr ));

    while (item.nchars > 0)
    {
	if ( *item.ptr <= 32 )
	{
	    item.ptr++;
	    item.nchars--;
	}
	else
	    break;
    }

    if (item.nchars != 0)
    {
	STACK_ITEM *stacked = context->tos;
	int i, tag = SGML_NO_ELEMENT;

	while ( stacked != NULL && (context->elements[stacked->element].flags & FLAG_NO_TEXT) == 0 )
	{
	    stacked = stacked->outer;
	}

	if (stacked != NULL)
	    tag = stacked->element;

	for (i = 0; i < sizeof(autofix) / sizeof(autofix[0]); i++)
	{
	    if ( autofix[i].element == tag )
	    {
		PRSDBG(("deliver_unexpected(): fixing with %s\n", autofix[i].insert));
		pseudo_html(me, autofix[i].insert);
		PRSDBG(("deliver_unexpected(): TOS is <%s>\n",
			context->tos == NULL ? "EMPTY" :
			context->elements[context->tos->element].name.ptr));
		/* Fully unstacked - cater for phantom closure */
		if (context->tos != NULL && context->tos->element >= 0)
		{
		    sgml_feed_characters_ascii(context, item.ptr, item.nchars);
		}
		else
		{
		    PRSDBG(("Ignoring text after </html>\n"));
		}
		break;
	    }
	}
    }

    nullfree((void**)&orig_item.ptr);
}

#define fmt_deliver_unexpected  deliver_unexpected

/*****************************************************************************

  Doesn't not happen during fmt mode.

 */

#define fmt_deliver_eol		deliver_nop

/*****************************************************************************

  There should be nothing present when the stream is closed, as the
  last thing should be closing markup. However, things like HotLists
  use of the parser on files that are straight text means that it's
  worth pushing anything that *might* exist.

  */

static void fmt_deliver_eos(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;

    if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
	text_item_push_word(htmlctx, 0, WITHOUT_SPACE);
    }
    else if (htmlctx->inhand_reason == DELIVER_SPACE)
    {
	text_item_push_word(htmlctx, 0, WITHOUT_SPACE);
    }
}

/*****************************************************************************/
/*                                                                           */
/*                       P R E     M O D E   D E L I V E R Y                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************

  A word in this context is a portion of a line or an entire line. It would
  always be an entire line if not for  <PRE> <B>X</B> </PRE> support.
  Have to do tab expansion here. Attempt to concatentate, but will probably
  end up having nothing to append to.

  Note that this routine also gets hijacked by some specific elements as
  a convenient means of obtaining the data they want in the unit they
  want (lines).

  SJM: 11/05/97: separated out the routines to do tab expansions and concatenation
  to ensure TEXTAREA and OPTION are expanded and fixed memory leaks due to not always
  freeing the item passed in.

  Then added new textarea code.
  
  */

static void pre_deliver_word(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;

    if (context->tos->element == HTML_TEXTAREA)
    {
	PRSDBG(("Textarea line='%.*s'\n", item.nchars, item.ptr));

	if (htmlctx->form && htmlctx->form->last_text)
	{
	    rid_textarea_item *tai;
	    STRING null;

	    STRING text;
	    int off;
	    tai = htmlctx->form->last_text;

	    PRSDBG(("pre_deliver_word: mz.used %d\n", tai->default_text.used));

	    null.nchars = 0;
	    text = get_tab_expanded_string(item, null);
	    if ((off = memzone_alloc(&tai->default_text, text.nchars + 1)) != -1)
	    {
		char *s;

		flexmem_noshift();

		s = tai->default_text.data + off;
		memcpy(s, text.ptr, text.nchars);
		s[text.nchars] = '\n';

		flexmem_shift();
	    }

	    PRSDBG(("pre_deliver_word: text.nchars %d mz.used %d off %d\n", text.nchars, tai->default_text.used, off));

	    string_free(&text);
	}
	else
	{
	    PRSDBG(("pre_deliver_word(): TEXTAREA line being discarded\n"));
/* 	    mm_free(item.ptr); */
	}
	string_free(&item);
    }
    else if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
	/* Need to free new data once used. Need to expand */
	/* tabs according to how many characters in hand */

	STRING t;

	t = get_tab_expanded_string(item, htmlctx->inhand_string);

	string_free(&item);
	string_free(&htmlctx->inhand_string);

	htmlctx->inhand_string = t;
	htmlctx->inhand_reason = DELIVER_WORD;
    }
    else
    {
	STRING t;
	STRING null;

	null.nchars = 0;
	t = get_tab_expanded_string(item, null);

	string_free(&item);

	htmlctx->inhand_string = t;
	htmlctx->inhand_reason = DELIVER_WORD;
    }
}

/*****************************************************************************

  We don't get DELIVER_SPACE in pre mode.

 */

#define pre_deliver_space deliver_nop

/*****************************************************************************

  Map onto fmt_deliver_sgml(). Should be okay as should be working with an
  element that doesn't want space/break stuff (ie HTML_SGML).

  */

#define pre_deliver_sgml fmt_deliver_sgml

/*****************************************************************************

  Just before the function for the markup. Space stripping and line breaks
  are not done in pre mode - space is exact and lines are via DELIVER_EOL.
  There's a whole class of elements that are not valid within <PRE> that
  might give undesirable results otherwise (eg space/break stuff).

*/

static void pre_deliver_pre_open_markup(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;

    if (htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK)
    {
#if NEW_BREAKS
	text_item_push_word(htmlctx, rid_break_MUST_NOT, WITHOUT_SPACE);
#else
	text_item_push_word(htmlctx, rid_flag_NO_BREAK, WITHOUT_SPACE);
#endif
    }

    htmlctx->inhand_reason = DELIVER_NOP;

    /* Space stripping from before the markup does not alter */
    /* space stripping after the markup */
    htmlctx->strip_space = FALSE;
}

#define pre_deliver_pre_close_markup pre_deliver_pre_open_markup

/*****************************************************************************

  Nothing to do here - no space or break stuff apply.

  */

#define pre_deliver_post_open_markup deliver_nop
#define pre_deliver_post_close_markup deliver_nop

/*****************************************************************************

  Not certain how this could happen. Drop them anyway.

  */

#define pre_deliver_unexpected deliver_unexpected

/*****************************************************************************

  Precise one to one correspondence between calls here and end of lines in
  documents local convention.

  */

static void pre_deliver_eol(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;
    
    /* SJM: 07Aug97 don't want newlines from textareas being displayed! */
    if (context->tos->element != HTML_TEXTAREA)
#if NEW_BREAKS
	text_item_push_word(htmlctx, rid_break_MUST, WITHOUT_SPACE);
#else
	text_item_push_word(htmlctx, rid_flag_LINE_BREAK, WITHOUT_SPACE);
#endif
	
    htmlctx->inhand_reason = DELIVER_NOP;
}

/*****************************************************************************/

static void pre_deliver_eos(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    HTMLCTX *htmlctx = context->clictx;

    /* SJM: 07Aug97 not sure what this does but we'll exclude textarea's from it anyway */
    if ((htmlctx->inhand_reason == DELIVER_WORD || htmlctx->inhand_reason == DELIVER_WORD_NOBREAK) && context->tos->element != HTML_TEXTAREA)
    {
#if NEW_BREAKS
	text_item_push_word(htmlctx, rid_break_MUST_NOT, WITHOUT_SPACE);
#else
	text_item_push_word(htmlctx, rid_flag_NO_BREAK, WITHOUT_SPACE);
#endif
    }

    htmlctx->inhand_reason = DELIVER_NOP;
}

/*****************************************************************************/
/*                                                                           */
/*                  S T R I N G    M O D E   D E L I V E R Y                 */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************

  String mode - accumulate whatever text goes by. As you can see, a pretty
  simple mode!

*/

static void str_deliver_word(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    /* Freeing done in append_new() */
    append_new(context->clictx, DELIVER_WORD, item);
}

static void str_deliver_sgml(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
    nullfree((void**)&item.ptr);
}

/*****************************************************************************

  Delivery is synchronous for str mode - the in hand string always holds
  the up to date stuff - the caller empties the string when appropriate.

  */

#define str_deliver_space deliver_nop
#define str_deliver_pre_open_markup deliver_nop
#define str_deliver_post_open_markup deliver_nop
#define str_deliver_pre_close_markup deliver_nop
#define str_deliver_post_close_markup deliver_nop
#define str_deliver_unexpected deliver_unexpected
#define str_deliver_eol deliver_nop
#define str_deliver_eos deliver_nop

/*****************************************************************************

  Fudge.

  */

extern void my_sgml_pre_open(SGMLCTX *context, ELEMENT *element)
{
    /* Automatic insertion of <TR> for <TD> and <TH> where no empty */
    /* cell is available is a retrospective decision. The timing of */
    /* where this occurs is critical */

    if (element->id == HTML_TH || element->id == HTML_TD)
    {
	pre_thtd_warning(htmlctxof(context));
    }
}

/*****************************************************************************

    Only DELIVER_WORD has a real string associated with it. This must
    be freed at some stage. DELIVER_SPACE and DELIVER_EOL get
    hardcoded " " and "\n" strings that should not be freed. The other
    methods do not receive real strings - they'll get some arbitary
    string that should not be examined.

    DELIVER_UNEXPECTED also gets the unexpected string, which needs
    freeing. Such deliveries might get hardcoded out if not wanted at all.

    DELIVER_SGML gets the <!whatever> string passed and this needs freeing.

    These are UTF8 strings so treat with care.

*/

extern void sgml_deliver(SGMLCTX *context, int reason, STRING item, ELEMENT *elem)
{
#if DEBUG
    static char *names[NUM_DELIVER_METHODS] =
    {
	"NOP ", "WORD", "SPC ",
	"PREO ", "PSTO", 
	"PREC ", "PSTC", 
	"UNXP",
	"SGML", "EOL ", "EOS ",
	"WDNB"
    };
#endif

    typedef void (*table_fn) (SGMLCTX *context, int reason, STRING item, ELEMENT *elem);
    static table_fn table [NUM_HTMLMODES * NUM_DELIVER_METHODS] =
    {
	fmt_deliver_nop,        pre_deliver_nop,        str_deliver_nop,
	fmt_deliver_word,       pre_deliver_word,       str_deliver_word,
	fmt_deliver_space,      pre_deliver_space,      str_deliver_space,
	fmt_deliver_pre_open_markup,  pre_deliver_pre_open_markup,  str_deliver_pre_open_markup,
	fmt_deliver_post_open_markup, pre_deliver_post_open_markup, str_deliver_post_open_markup,
	fmt_deliver_pre_close_markup, pre_deliver_pre_close_markup, str_deliver_pre_close_markup,
	fmt_deliver_post_close_markup,pre_deliver_post_close_markup,str_deliver_post_close_markup,
	fmt_deliver_unexpected, pre_deliver_unexpected, str_deliver_unexpected,
	fmt_deliver_sgml,       pre_deliver_sgml,       str_deliver_sgml,
	fmt_deliver_eol,        pre_deliver_eol,        str_deliver_eol,
	fmt_deliver_eos,        pre_deliver_eos,        str_deliver_eos,
	fmt_deliver_word,       pre_deliver_word,       str_deliver_word
    };

    HTMLCTX *me = htmlctxof(context);

    ASSERT(context->magic == SGML_MAGIC);
    ASSERT(me->magic == HTML_MAGIC);

    PRSDBGN(("sgml_deliver(%p, %s, '%.*s')\n",
	    context,
	    names[reason],
	    reason == DELIVER_UNEXPECTED ? 0 : item.nchars, item.ptr
	    ));

    /* Choose and invoke the appropriate handler */
    (* table[reason * NUM_HTMLMODES + me->mode] )
    (context, reason, item, elem);

    me->last_reason = reason;
}

/*****************************************************************************/



/* eof deliver.c */

