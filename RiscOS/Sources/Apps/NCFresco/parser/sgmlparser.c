/* sgmlparser.c - top level of the parser */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

#include "sgmlparser.h"

/*****************************************************************************/

#if SGML_REPORTING

extern void sgml_note_unknown_element (SGMLCTX * context, const char *fmt, ...) 
{
    va_list list;
    va_start(list, fmt);
    (*context->callback.note_unknown_element) (context, fmt, list);
    va_end(list);
}

extern void sgml_note_unknown_attribute (SGMLCTX * context, const char *fmt, ...) 
{
    va_list list;
    va_start(list, fmt);
    (*context->callback.note_unknown_attribute) (context, fmt, list);
    va_end(list);
}

extern void sgml_note_bad_attribute (SGMLCTX * context, const char *fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    (*context->callback.note_bad_attribute) (context, fmt, list);
    va_end(list);
}

extern void sgml_note_message(SGMLCTX * context, const char *fmt, ... )
{
    va_list list;
    va_start(list, fmt);
    (*context->callback.note_message) (context, fmt, list);
    va_end(list);
}

#endif

/*****************************************************************************

    Feed new characters into the SGML parser.  Any number of characters can
    be supplied.

*/

extern void sgml_feed_characters(SGMLCTX *context, const char *buffer, int bytes)
{
    int i;

    ASSERT(context->magic == SGML_MAGIC);

    PRSDBGN(("sgml_feed_characters(): '%.*s'\n", bytes, buffer));

    for (i = 0; i < bytes; i++, buffer++)
    {
	/* optionally convert undefined keys to PC keymap */
	const char c = convert_undefined_key_code(*buffer);
	add_char_to_inhand(context, c);
#if 0
	PRSDBGN(("sgml_feed_characters(): '%c' in '%s'\n", c, get_state_name(context->state)));
#endif
	(*context->state) (context, c);
    }
}

/*****************************************************************************

    An SGML context has finished.  Flushes any intervening buffers and
    performs automatic closes of any open markup. You should call here to 
    finish the parsing and then call sgml_free_context() to release the 
    resources.

*/

extern void sgml_stream_finished (SGMLCTX *context)
{
    static STRING s = { NULL, 0 };
    
    ASSERT(context->magic == SGML_MAGIC);

    if ((context->state == state_comment_wait_dash_1 || context->state == state_comment_wait_dash_2) 
        && context->comment_anchor != -1)
    {
        int stream_end = context->inhand.ix; context->inhand.ix = context->comment_anchor;
        do_got_element (context);

        PRSDBG(("End of document suggests missing comment close - backtracking\n"));
        sgml_note_missing_close(context, &context->elements[context->tos->element]);
        PRSDBG(("Recovering in %s\n", get_state_name (context->state)));

        sgml_feed_characters (context, &context->inhand.data [context->comment_anchor], stream_end - context->comment_anchor);

        sgml_stream_finished (context);
    }
    else
    {
        PRSDBGN(("SGML stream (%p) finished in %s. Flushing chopper\n", context, get_state_name (context->state)));
    
        /* Flush existing chopped stuff */
        (*context->chopper) (context, s);
        
        while (context->tos != NULL && context->tos->outer != NULL )
        {
    	   ELEMENT *closing = &context->elements[context->tos->element];
    
    	   PRSDBG(("End of document implies </%s>", closing->name.ptr));
    	   sgml_note_missing_close(context, closing);
    
    	   perform_element_close(context, closing);
        }
    
        /* Finally, announce end of stream */
        (*context->deliver) (context, DELIVER_EOS, empty_string, NULL);
   }
}


extern void sgml_free_report(SGMLCTX *context)
{
    string_list_free(context->report.messages);
    string_list_free(context->report.unknown_elements);
    string_list_free(context->report.unknown_attributes);
    string_list_free(context->report.bad_attributes);
}

/*****************************************************************************

    Free a context structure and associated resources, now that it is
    finished with. This does not free the client context information.
  
*/

extern void sgml_free_context(SGMLCTX *context)
{
    PRSDBGN(("sgml_free_context(%p)\n", context));

    ASSERT(context->magic == SGML_MAGIC);

    sgml_free_stack(context->tos);
    context->tos = NULL;
/*    sgml_free_attributes( &context->values[0] );*/
    free_buffer(&context->inhand);
    free_buffer(&context->prechop);

    sgml_free_report(context);

    mm_free(context);
}

/*****************************************************************************

    Create a new SGML context to operate from. Most of the structure is 
    initialised. The caller needs to supply the remaining values.

*/

extern SGMLCTX * sgml_new_context(void)
{
    SGMLCTX *context = mm_calloc(1, sizeof(SGMLCTX));

    if (context == NULL)
	return NULL;

    context->magic = SGML_MAGIC;
    context->tos = mm_calloc(1, sizeof(STACK_ITEM));
    ASSERT(context->tos != NULL);
    context->inhand.data = mm_calloc(1, 4096);
    context->inhand.max = 4096;
    
    reset_tokeniser_state(context);
    reset_lexer_state(context);
    sgml_reset_chopper_state(context);

#if 0
    /* Can't call as not yet filled in. Caller must do this itself */
    PRSDBG(("Calling %p\n", context->callback.sgml_reset_report));
    (*context->callback.sgml_reset_report) (context);
    PRSDBG(("Returned\n"));
#endif

    return context;
}

/*****************************************************************************

  These are currently placeholders, but they might be required.

  FIXME: work out where attributes get freed and ensure this is done
  in a recursion friendly fashion.

  */

extern void sgml_recursion_warning_pre(SGMLCTX *context)
{
    PRSDBGN(("Entering recursion\n"));
/*    sgml_free_attributes( &context->values[0] );*/
}

extern void sgml_recursion_warning_post(SGMLCTX *context)
{
    PRSDBGN(("Left recursion\n"));
}

/*****************************************************************************

  Install and remove sgml_deliver_fn handlers, using context

 */

extern void sgml_install_deliver(SGMLCTX *context, sgml_deliver_fn new)
{
    sgml_deliver_list *dp = mm_calloc(1, sizeof(*dp));

    PRSDBGN(("sgml_install_deliver(%p, %p)\n", context, new));

    dp->this_fn = context->deliver;
    context->deliver = new;
    dp->previous = context->dlist;
    context->dlist = dp;
}

extern void sgml_remove_deliver(SGMLCTX *context, sgml_deliver_fn current)
{
    sgml_deliver_list *dp = context->dlist;

    PRSDBGN(("sgml_remove_deliver(%p, %p)\n", context, current));

    ASSERT(dp != NULL);
    ASSERT(current == context->deliver);

    context->deliver = dp->this_fn;
    context->dlist = dp->previous;
    mm_free(dp);
}

extern void sgml_unwind_deliver(SGMLCTX *context)
{
    PRSDBGN(("sgml_unwind_deliver(%p)\n", context));

    while (context->dlist)
	sgml_remove_deliver(context, context->deliver);	/* SJM: is this right? */
/* 	sgml_remove_deliver(context, context->dlist->this_fn); */
}

/*****************************************************************************/

/* eof sgmlparser.c */



