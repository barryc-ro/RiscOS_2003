/* statemach.c */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

#include "sgmlparser.h"

#ifndef WHITESPACE_BEFORE_COMMENT_START
#define WHITESPACE_BEFORE_COMMENT_START !defined(STBWEB)
#endif

#ifndef CORRECT_SGML_COMMENTS
#define CORRECT_SGML_COMMENTS !defined(STBWEB)
#endif

/*****************************************************************************

  When we are sending a text file through the parser, we don't want to
  *ever* be capable of matching any markup, no matter how seemingly
  obscure (until you try to document it!). Once this state is entered,
  characters are only ever fed through the chopper. This means
  everything operates one character at a time. The route is fairly simple
  nested functions with not more than 4 words of args, so we'll not try
  to clutter this up.

  */

extern void state_stuck_text(SGMLCTX *context, UCHARACTER input)
{
    push_inhand(context);

    if (input < 32)
    {
	static USTRING s = { NULL, 0 };

	(*context->chopper) (context, s); /* Flush */
    }
}

extern void state_end_found_element (SGMLCTX *context, UCHARACTER input)
{
        if ( is_whitespace (input) )
        {
                ;
        }
        else if ( input == '>')
        {
                do_got_element (context) ;
        }
        else
        {
                push_inhand (context);
                context->state = state_end_tag_only ;
        }
}


extern void state_end_find_element_body (SGMLCTX *context, UCHARACTER input)
{
        if ( is_element_body_character (input))
        {
                ;
        }
        else if ( strnicmpu(&context->inhand.data[2],
                  context->elements[context->tos->element].name.ptr,
                  context->elements[context->tos->element].name.nchars ) == 0 )
        {
                if ( is_whitespace (input) )
                {
                        context->state = state_end_found_element;
                }
                else if ( input == '>' )
                {
                        do_got_element (context);
                }
                else
                {
                        push_inhand (context);
                        context->state = state_end_tag_only;
                }
        }
        else
        {
                push_inhand (context);
                context->state = state_end_tag_only;
        }
}

extern void state_end_find_element (SGMLCTX *context, UCHARACTER input)
{
        if ( is_element_start_character (input) )
        {
                context->state = state_end_find_element_body;
        }
#if 0
	/* SJM: this might be needed but I;m not sure right now */
	else if ( is_whitespace (input) )
	{
	    /* SJM: allow whitespace in close tag </   element>
	     *      remove it to make rest easier
	     */
	    context->inhand.ix--;
	}
#endif
	else
        {
                push_inhand (context);
                context->state = state_end_tag_only;
        }
}

/*****************************************************************************/


extern void state_end_find_slash (SGMLCTX *context, UCHARACTER input)
{
        if ( input == '/' )
        {
                context->state = state_end_find_element;
        }
        else
	{
		push_inhand (context);
		context->state = state_end_tag_only;
	}
}

extern void state_some_sgml_command (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='>' )
	{
		do_got_element (context);
	}
}


/*****************************************************************************/

/* new scheme that correctly parses correct comments
 * and doesn't try and do anything clever with bad comments.
 * except we will handle arbitrary numbers of dashes.
 *
 * DL: now for some support for bad comments. If we reach the end of
 * the stream with a comment open, we backtrack to the first ">" following
 * the comment open. This is not 'correct' but is probably friendly in
 * many circumstances. 
 * The method is to drop a global anchor (the inhand index) when a ">" is 
 * swallowed but we are not ready to close the comment. The anchor is set to -1
 * when there is no such. The recovery is performed in sgml_stream_finished
 * where the characters after the anchor are fed in again, from a closed stat.
 * Any changes to this part of the state machine should consider the effects
 * on comment recovery.

    Here is the state machine just for comments:
                 /--------+-------------+--------------------------> got element
              ">"|     ">"|          ">"|                
      "<!"       |"-"     |"-"          | ?           "-"         "-"
       ---> maybe ---> pre ---> dash_1_0 --->  dash_1 ---> dash_2 ---> maybe
           |              |             \     |      |           |
           \--------------/           "-"\    \------<-----------/ /---> got element
                   ?                      \      ?              ">"|"-"
                                           \-------------> dash_2_0 ---> maybe

    Note the change to the state machine to allow several comments in one bit of
    SGML - the comment_wait_close state is no longer reached.
    The anchor is cleared on entering maybe and dropped on the first > while in dash_1 or dash_2.

 */

/* <!-- ... -- */
extern void state_comment_wait_close (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='>' )
	{ do_got_element(context); }
	else if ( input == '-')
	{ context->state = state_comment_wait_close; }
	else
	{ context->state = state_comment_wait_dash_1; }
        /* DL: I can't see how this is correct - got to get to waiting for
           new comment start somehow!
         */
}

/* <!-- ... - */
extern void state_comment_wait_dash_2 (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='-' )
	{
#if 0
	    context->state = state_comment_maybe /* state_comment_wait_close */;
#else
	    context->state = state_comment_wait_close;
#endif
	}
	else if ( input == '>' && context->comment_anchor == -1 )
        { context->comment_anchor = context->inhand.ix; }
	else
	{
	    context->state = state_comment_wait_dash_1;
	}
}

/* <!--- waiting for - or > having got here through unbroken series of dashes */
extern void state_comment_wait_dash_2_0 (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='-' )
	{
#if CORRECT_SGML_COMMENTS
	    context->state = state_comment_maybe /* state_comment_wait_close */;
#else
	    context->state = state_comment_wait_close;
#endif
	}
	else if ( input == '>' )
        {   do_got_element (context); }
	else
	{
	    context->state = state_comment_wait_dash_1_0;
	}
}

/* <!-- ... swallow anything except another dash */
extern void state_comment_wait_dash_1 (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='-' )
	{ context->state = state_comment_wait_dash_2; }
	else if ( input == '>' && context->comment_anchor == -1 )
        { context->comment_anchor = context->inhand.ix; }
        else
	{ ; }
}

/* <!-- swallow anything except another dash or closing > otherwise go to state dash_1 */
extern void state_comment_wait_dash_1_0 (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='-' )
	{ context->state = state_comment_wait_dash_2_0; }
	else if ( input == '>')
        { do_got_element (context); }
        else
	{ context->state = state_comment_wait_dash_1; }
}

/* <!- */
extern void state_comment_pre_initial (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='-' )
	{
            context->comment_anchor = -1;
	    context->state = state_comment_wait_dash_1_0;
	}
	else if ( input =='>' )
	{
	    do_got_element (context);
	}
	else
	{
	    context->state = state_comment_maybe;
#if SGML_REPORTING
	    sgml_note_message (context, ("Perhaps a ''-'' was omitted?")) ;
#endif
	}
}

/* <! */
extern void state_comment_maybe (SGMLCTX *context, UCHARACTER input)
{
#if WHITESPACE_BEFORE_COMMENT_START
    if ( is_whitespace (input) )
	{ ; }
	else
#endif
	    if ( input =='>' )
	{ do_got_element (context); }
	else if ( input =='-' )
	{ context->state = state_comment_pre_initial; }
	else
	{ context->state = state_some_sgml_command; }
}

/*****************************************************************************/


extern void state_unquoted_value (SGMLCTX *context, UCHARACTER input)
{
	if ( is_value_body_character (input) )
	{ ; }
	else if ( is_whitespace (input) )
	{ context->state = state_find_attribute_name; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else if ( input ==single_quote || input ==double_quote )
	{
#if SGML_REPORTING
		sgml_note_message (context, "No balancing opening quote for attribute value");
#endif
		context->state = state_find_attribute_name ;
	}
	else
	{
		context->state = state_badly_formed_tag;
		context->inhand.ix--;				/* drop this character */
#if SGML_REPORTING
		sgml_note_message (context, "Character '%c' not valid in value body", input) ;
#endif
	}
}

extern void state_single_quoted_value (SGMLCTX *context, UCHARACTER input)
{
	if ( input ==single_quote )
	{ context->state = state_find_attribute_name; }
        else if (input == '>' )
        {
#if SGML_REPORTING
	    sgml_note_message(context, "Warning: '>' within quoted attribute value"); 
#endif
	}
}

extern void state_double_quoted_value (SGMLCTX *context, UCHARACTER input)
{
	if ( input ==double_quote )
	{ context->state = state_find_attribute_name; }
        else if (input == '>' )
        { 
#if SGML_REPORTING
	    sgml_note_message(context, "Warning: '>' within quoted attribute value"); 
#endif
	}
}

extern void state_find_attribute_value (SGMLCTX *context, UCHARACTER input)
{
	if ( is_whitespace (input) )
	{ ; }
	else if ( input ==single_quote )
	{ context->state = state_single_quoted_value; }
	else if ( input ==double_quote )
	{ context->state = state_double_quoted_value; }
	else if ( is_value_start_character (input) )
	{ context->state = state_unquoted_value; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else
	{
		context->state = state_badly_formed_tag;
		context->inhand.ix--;
#if SGML_REPORTING
		sgml_note_message (context, 
				   "Character '%c' is not valid to start an attribute value", 
				   input) ;
#endif
	}
}

extern void state_find_attribute_equals (SGMLCTX *context, UCHARACTER input)
{
	if ( is_whitespace (input) )
	{ ; }
	else if ( input =='=' )
	{ context->state = state_find_attribute_value; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else if ( is_attribute_start_character (input) )
	{ context->state = state_in_attribute_name; }
	else
	{
		context->state = state_badly_formed_tag;
		context->inhand.ix--;
#if SGML_REPORTING
		sgml_note_message (context, ("Expected '=' character")) ;
#endif
	}
}

extern void state_in_attribute_name (SGMLCTX *context, UCHARACTER input)
{
	if ( is_attribute_body_character (input) )
	{ ; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else if ( is_whitespace (input) )
	{ context->state = state_find_attribute_equals; }
	else if ( input =='=' )
	{ context->state = state_find_attribute_value; }
	else
	{
		context->state = state_badly_formed_tag;
		context->inhand.ix--;
#if SGML_REPORTING
		sgml_note_message (context, "Character '%c' is not valid in an attribute name", input) ;
#endif
	}
}

extern void state_find_attribute_name (SGMLCTX *context, UCHARACTER input)
{
	if ( is_attribute_start_character (input) )
	{ context->state = state_in_attribute_name; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else if ( is_whitespace (input) )
	{ ; }
	else
	{
		context->state = state_badly_formed_tag;
		context->inhand.ix--;
#if SGML_REPORTING
		sgml_note_message (context,
				   "Character '%c' is not valid to start an attribute name",
				   input) ;
#endif
	}
}

extern void state_in_element_name (SGMLCTX *context, UCHARACTER input)
{
	if ( is_element_body_character (input) )
	{ ; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else if ( is_whitespace (input) )
	{ context->state = state_find_attribute_name; }
	else
	{
		context->state = state_really_badly_formed_tag;
#if SGML_REPORTING
		sgml_note_message (context, "Character '%c' is not valid in an element name", input) ;
#endif
	}
}

/* Seen the </ for some closing markup */

extern void state_had_markup_open_slash (SGMLCTX *context, UCHARACTER input)
{
	if ( is_element_start_character (input) )
	{ context->state = state_in_element_name; }
	else if ( is_whitespace (input) )
	{ ; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else
	{
		/* Not valid, so assume not markup */
#if SGML_REPORTING
		sgml_note_message (context, ("Does not look like markup"));
#endif
		push_inhand (context);
		context->state = state_all_tags ;
	}
}

/* Seen the < for some markup. Decide what's happening */

extern void state_had_markup_open (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='/' )
	{ context->state = state_had_markup_open_slash; }
	else if ( input =='!'  )
	{ context->state = state_comment_maybe; }
	else if ( is_element_start_character (input) )
	{ context->state = state_in_element_name; }
	else if ( input =='>' )
	{ do_got_element (context); }
	else
	{
		/* Not valid, so assume not markup */
#if SGML_REPORTING
		sgml_note_message (context, ("Does not look like markup"));
#endif
		push_inhand (context);
		context->state = state_all_tags ;
	}
}


/*****************************************************************************

This matching is crude - if we have something such as &gtX we should
generate &X, but we fail to match gtX as a glyph name. Could be fixed
but requires extra work thinking or extra work during execution matching
on a character by character basis.

*/

static void state_find_entity(SGMLCTX *context, UCHARACTER input)
{
        if ( ! is_entity_character(input) )
        {
               	context->inhand.ix--;
#if 0
		if ( context->inhand.data[context->inhand.ix - 1] == ';' )
                if (input == ';')
       		 	context->inhand.ix--;		/* remove optional semicolon */
#endif
/*printf("entity '%.*s' -- ", min(MAXSTRING, context->inhand.ix), context->inhand.data);*/
                entity_recognition(context);
	PRSDBG(("state_find_entity: done recog\n"));
                context->state = get_state_proc(context);
	PRSDBG(("state_find_entity: got state %p\n", context->state));
                if (input != ';')
                {
        		add_char_to_inhand(context, input);
        		(*context->state) (context, input);
                }
       }
}

/*****************************************************************************/


extern void state_badly_formed_tag (SGMLCTX *context, UCHARACTER input)
{
    /* SJM: idea to get round bad markup dumping whole tag.
     * When in bad markup state we stay here until we find whitespace
     * then recommence with find attribute name, unless we hit tag end
     * in which case we do got element.
     * We drop any characters whilst we are in this state.
     * This means we lose the bacldy formed markup message but each entry to this state
     * will have added a message anyway.
     */
	if ( is_whitespace(input) )
	{
	    context->state = state_find_attribute_name;	/* go back to usual parsing looking for a name */
	}
	else if ( input =='>' )
	{
	    do_got_element(context);	/* finish element as usual */
	}
	else
	{
	    context->inhand.ix--;	/* remove the bad character from inhand */
	}
}

extern void state_really_badly_formed_tag (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='>' )
	{
#if SGML_REPORTING
	    {
		USTRING s;
		s.ptr = context->inhand.data;
		s.nchars = context->inhand.ix;
		sgml_note_message (context, "Badly formed markup '%.*s'",
				   min(MAXSTRING, context->inhand.ix), usafe(s));
	    }
#endif
		clear_inhand (context);
		context->state = get_state_proc (context);
	}
}

extern void state_end_tag_only (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='<' )
	{
		push_bar_last_inhand (context);
		context->state = state_end_find_slash ;
	}
#if 1
	/* I don't think this should be active - reenabled 'cos I
	 * don't know what I took it out and it saves later processing
	 */
	else if (input == '&' )
	{
                push_bar_last_inhand (context);
		context->state = state_find_entity;
	}
#endif
	else
	{ push_inhand (context); }
}

extern void state_markup_only (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='<' )
	{
		push_bar_last_inhand (context);
		context->state = state_had_markup_open ;
	}
	else
	{
#if SGML_REPORTING
		if ( ! is_whitespace (input) )
		{
			sgml_note_message (context, "Text is not valid at this point");
			sgml_note_unexpected_character (context) ;
		}
#endif

                /* String will be freed */                
                (*context->deliver)
                (
                        context,
                        DELIVER_UNEXPECTED,
                        mkstringu(context->encoding_write, context->inhand.data, context->inhand.ix),
			NULL
                );
		clear_inhand (context) ;
	}
}

extern void state_all_tags (SGMLCTX *context, UCHARACTER input)
{
	if ( input =='<' )
	{
		push_bar_last_inhand (context);
		context->state = state_had_markup_open ;
	}
	else if (input == '&' )
	{
                push_bar_last_inhand (context);
		context->state = state_find_entity;
	}
	else
	{ push_inhand (context); }
}


/*****************************************************************************/

#if DEBUG

typedef struct { state_fn fn; char *name; } state_name_str;

static state_name_str state_names[] =
{
    { state_all_tags, "state_all_tags" },
    { state_badly_formed_tag, "state_badly_formed_tag" },
    { state_really_badly_formed_tag, "state_really_badly_formed_tag" },
    { state_comment_maybe, "state_comment_maybe" },
    { state_comment_pre_initial, "state_comment_pre_initial" },
    { state_comment_wait_dash_1, "state_comment_wait_dash_1" },
    { state_comment_wait_dash_2, "state_comment_wait_dash_2" },
    { state_comment_wait_dash_1_0, "state_comment_wait_dash_1_0" },
    { state_comment_wait_dash_2_0, "state_comment_wait_dash_2_0" },
    { state_comment_wait_close, "state_comment_wait_close" },
    { state_double_quoted_value, "state_double_quoted_value" },
    { state_end_find_element, "state_end_find_element" },
    { state_end_find_element_body, "state_end_find_element_body" },
    { state_end_find_slash, "state_end_find_slash" },
    { state_end_found_element, "state_end_found_element" },
    { state_end_tag_only, "state_end_tag_only" },
    { state_find_attribute_equals, "state_find_attribute_equals" },
    { state_find_attribute_name, "state_find_attribute_name" },
    { state_find_attribute_value, "state_find_attribute_value" },
    { state_find_entity, "state_find_entity" },
    { state_had_markup_open, "state_had_markup_open" },
    { state_in_attribute_name, "state_in_attribute_name" },
    { state_in_element_name, "state_in_element_name" },
    { state_markup_only, "state_markup_only" },
    { state_single_quoted_value, "state_single_quoted_value" },
    { state_some_sgml_command, "state_some_sgml_command" },
    { state_stuck_text, "state_stuck_text" },
    { state_unquoted_value, "state_unquoted_value" },
    { state_had_markup_open_slash, "state_had_markup_open_slash" },
    { 0, 0 }
};

char *get_state_name(state_fn fn)
{
    state_name_str *sp;
    for (sp = state_names; sp; sp++)
	if (sp->fn == fn)
	    return sp->name;
    return "<unknown>";
}
#endif

extern state_fn get_state_proc(SGMLCTX *context)
{
    /*PRSDBGN(("get_state_proc(): %d as input\n", context->tos->matching_mode));*/

    switch (context->tos->matching_mode)
    {
    case 0:
	return state_all_tags;
	break;

    case match_end_tag:
	return state_end_tag_only; /* Assumed to be chopper */
	break;

    case match_unexpected:
	return state_markup_only;
	break;

    case match_perm_text:	/* Used for .txt filetype */
	return state_stuck_text;
	break;
    }

    PRSDBG(("get_state_proc(): bad state value %d\n", context->tos->matching_mode));
    return state_markup_only;
}

/* eof statemach.c */
