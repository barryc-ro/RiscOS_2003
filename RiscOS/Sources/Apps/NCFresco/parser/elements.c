/* elements.c - Basic element manipulation */
/* (C) Copyright ANT Limited 1996. All rights reserved. */

/* CHANGELOG
 * 22/7/96: SJM: added call to sgml_translation() to attribute parsing.
 * 21/8/96: SJM: Nasty bit in perform_element_close for /P
 * 23/5/96: SJM: check the guessed elements in parse_then_perform_element_open().
 */

#include "sgmlparser.h"
#include "profile.h"

extern void report_start (SGMLCTX * context, ELEMENT * element, VALUES * attributes);
extern void report_finish (SGMLCTX * context, ELEMENT * element);

#define BIT_SET(base, bit)	(base)[(bit)/32] |= (1<<((bit)%32))
#define BIT_CLR(base, bit)	(base)[(bit)/32] &= ~(1<<((bit)%32))
#define BIT_TST(base, bit)	((base)[(bit)/32] &  (1<<((bit)%32)))

static BOOL perform_element_open(SGMLCTX *context, ELEMENT *element, VALUES *values);
/*static void perform_element_close(SGMLCTX *context, ELEMENT *element);*/

/*****************************************************************************

  Free any additional storage associated with the supplied attributes.
  The

  */

static void sgml_free_attributes(VALUES *values_array)
{
    VALUE *values = &values_array->value[0];
    int ix;

    /*PRSDBGN(("sgml_free_attributes(): Clearing attributes\n"));*/

    for (ix = 0; ix < SGML_MAXIMUM_ATTRIBUTES; ix++)
    {
	if (values[ix].type != value_none)
	{
	    switch (values[ix].type)
	    {
		/* No additional memory to free */
	    case value_none:
	    case value_integer:
	    case value_tuple:
	    case value_enum:
	    case value_void:
	    case value_absunit:
	    case value_relunit:
	    case value_pcunit:
		break;
		/* Have additional memory to free */
	    case value_string:
		if ( values[ix].u.s.ptr != NULL )
		{
		    mm_free(values[ix].u.s.ptr);
		    values[ix].u.s.ptr = NULL;
		}
		break;
	    case value_stdunit_list:
		if ( values[ix].u.l.items != NULL )
		{
		    mm_free(values[ix].u.l.items);
		    values[ix].u.l.items = NULL;
		}
		break;
	    }

	    values[ix].type = value_none;
	}
    }
}

/*****************************************************************************

  Supply default values for an element. Currently, this mechanism is
  not used and we simply ensure the attributes are empty. Much fun
  could be had with defaults here.

  */

extern BOOL default_attributes(SGMLCTX *context, ELEMENT *element, VALUES *values)
{
    int i;

    memset(values, 0, sizeof(*values));

    for (i = 0; i < SGML_MAXIMUM_ATTRIBUTES; i++)
    {
	if (value_none != 0)
	    values->value[i].type = value_none;
	/* This is only necessary if we are reconstructing the */
	/* original attributes order */
	values->value[i].tag = -1;
    }

    /* Return FALSE if defaults depend upon the element itself */

    return TRUE;
}

typedef VALUE (*parse_table_fn) (SGMLCTX *, ATTRIBUTE *, USTRING);
static parse_table_fn parse_table [] =
{
    sgml_do_parse_void ,
    sgml_do_parse_enum ,
    sgml_do_parse_enum_void ,
    sgml_do_parse_string ,
    sgml_do_parse_integer ,
    sgml_do_parse_stdunit,
    sgml_do_parse_string_void,
    sgml_do_parse_enum_tuple,
    sgml_do_parse_integer_void,
    sgml_do_parse_stdunit_void,
    sgml_do_parse_stdunit_list,
    sgml_do_parse_enum_string,
    sgml_do_parse_bool,
    sgml_do_parse_colour
};


/*****************************************************************************

  See if an element is open, checking the current tos and also the
  array of non-stacking open elements (currently only FORM).

  */

static BOOL is_element_open(SGMLCTX *context, const int tag)
{
    return element_bit_set (context->tos->elements_open, tag) ||
	element_bit_set (context->dont_stack_elements_open, tag);
}

/*****************************************************************************

  See if an element is open, but constrain search within the innermost
  open container. It should be possible to do this considerably more
  efficiently.

  */

static BOOL open_within_container(SGMLCTX *context, const tag)
{
    register BOOL seen, stop;
    STACK_ITEM *item;

    /* Quickly reject the easy case */
    if ( ! is_element_open(context, tag) )
	return FALSE;

    seen = FALSE;
    stop = FALSE;

    for (item = context->tos; item != NULL && !seen && !stop; item = item->outer)
    {
	ELEMENT *elem = &context->elements[item->element];
	
	PINC_OPEN_WITHIN_CONTAINER_CHECKS;

	stop = (elem->flags & FLAG_CONTAINER) != 0 && (elem->flags & FLAG_FULL_UNWIND) == 0;
	seen = elem->id == tag;
    }

    return seen;
}

/*****************************************************************************/

static void force_close_to_matching (SGMLCTX *context, ELEMENT *element)
{
    PRSDBG(("force_close_to_matching: %s\n", elements_name(context, element->id)));

    while (TRUE)
    {
        STACK_ITEM *nos;

        for (nos = context->tos; nos != NULL; nos = nos->outer) 
        {
            if (nos->element == element->id)
                 break;

            if ((context->elements[nos->element].flags & FLAG_DONT_FORCE_CLOSE) == 0)
                 break;
        }
        /* nos now points to the first stack element that we can force close or it points to
           our element.
         */

        if (!element_bit_set (nos->elements_open, element->id)) break;
        
        pull_stack_item_to_top_correcting_effects (context, nos);

        if (nos->element == element->id) 
        {
            /* We've reached the element we wanted to close */

            perform_element_close (context, element); break;
        }
        else
        {
            int *phantom = &context->tos->element;
            ELEMENT *elem = &context->elements[*phantom];
            BOOL b = (elem->flags & FLAG_CONTAINER) != 0 &&
                     (element->flags & FLAG_FULL_UNWIND) == 0;
         
	    if (element->group != HTML_GROUP_NONE) b &= elem->group == element->group;
            /* Note that this means an element may be closed */
            /* when it's *NOT* the context->tos element. */
         
            PRSDBG(("perform_element_close(%s): force closing element </%s>\n",
             element->name.ptr, elem->name.ptr));
            perform_element_close(context, elem);
            *phantom = - *phantom;
            sgml_note_missing_close(context, elem);
            PRSDBG(("perform_element_close(%s): done force close </%s>\n",
                    element->name.ptr, elem->name.ptr));
    	    /* Leave this in: Borris: 10/9/96 */
            if (b) break;
        }
    }
}

/*****************************************************************************/

#if DEBUG
static char *action_names [11] =
{
    "one_of_enclosing",
    "last_one_of_enclosing",
    "enclosed_within",
    "preceeded_by",
    "close_any_open",
    "post_clear_seen_flag",
    "should_contain",
    "quietly_close_any_open"
    "container_enclosed_within",
    "invent_only_within",
    "replace_with",
    "replaces_bad"
};
#endif

/* DAF: 970710: Decided that we actually need to iterate over the list
 * of rules for each element to cater for certain examples of bad
 * nesting. tabtests/misc60.html illustrates the sort of problem to be
 * dealt with. The conceptual model is now that each element has a
 * list of rules. This list repeats indefinitely.
 * ensure_pre_requisites evaluates this list of rules until it
 * completes an entire cycle without applying any of the rules. */

static ELEMENT *ensure_pre_requisites (SGMLCTX *context, ELEMENT *element)
{
    VALUES dummy_values;
    ACT_ELEM *req;
    const BOOL old_apply = context->applying_rules;

    BOOL had_enclosing = FALSE;
    BOOL init_dummy = FALSE;
    BOOL ok = TRUE;
    BOOL can_invent_within = FALSE;
    BOOL applied_rule;
    ELEMENT *invent = NULL;

    context->applying_rules = TRUE;

    do
    {
	applied_rule = FALSE;

	for (req = element->requirements; req->element != SGML_NO_ELEMENT; req++)
	{
	    const action = req->action, tag = req->element;
	    ELEMENT *other_elem = &context->elements[tag];

	    PRSDBGN(("Ensure prereqs <%s> act %s, elem %s\n",
		     element->name.ptr, action_names[action], other_elem->name.ptr));

	    switch (action)
	    {
	    case ONE_OF_ENCLOSING:
		if ( is_element_open(context, tag) )
		    had_enclosing = TRUE;
		break;
	    case LAST_ONE_OF_ENCLOSING:
		if ( is_element_open(context, tag) )
		    had_enclosing = TRUE;
		if (! had_enclosing)
		{
		    if (element->group != other_elem->group || invent == NULL || can_invent_within)
		    {
			PRSDBG(("ensure_pre_requisites(%s): %s not contained in %s\n",
				element->name.ptr,element->name.ptr, other_elem->name.ptr));
			if (! init_dummy)
			    init_dummy = default_attributes(context, other_elem, &dummy_values);
			sgml_note_missing_open (context, other_elem);
			applied_rule |= perform_element_open (context, other_elem, &dummy_values);
			had_enclosing = FALSE;
		    }
		    else
			ok = FALSE;
		}
		break;
	    case ENCLOSED_WITHIN:
		if ( !is_element_open (context, tag) )
		{
		    if (element->group != other_elem->group || invent == NULL || can_invent_within)
		    {
			PRSDBGN(("ensure_pre_requisites(%s): %s not contained in %s\n",
				 element->name.ptr, element->name.ptr, other_elem->name.ptr));
			if (! init_dummy)
			    init_dummy = default_attributes(context, other_elem, &dummy_values);
			sgml_note_missing_open (context, other_elem);
			applied_rule |= perform_element_open (context, other_elem, &dummy_values);
		    }
		    else
			ok = FALSE;
		}
		break;
	    case PRECEEDED_BY:
		if ( element_bit_clear (context->tos->elements_seen, tag) )
		{
		    PRSDBGN(("ensure_pre_requisites(%s): %s not preceeded by %s\n",
			     element->name.ptr,element->name.ptr,  other_elem->name.ptr));
		    if (! init_dummy)
			init_dummy = default_attributes(context, other_elem, &dummy_values);
		    sgml_note_missing_open (context, other_elem);
		    applied_rule |= perform_element_open (context, other_elem, &dummy_values);
		    sgml_note_missing_close (context, other_elem);
		    perform_element_close (context, other_elem);
		}
		break;
	    case CLOSE_ANY_OPEN:
		if ( open_within_container(context, tag) )
		    /* while ( is_element_open(context, tag) ) */
		{
		    STACK_ITEM *old_tos = context->tos;
		    ELEMENT *oe = &context->elements[old_tos->element];

		    PRSDBGN(("ensure_pre_requisite(%s): implied closure </%s>\n",
			     element->name.ptr, oe->name.ptr));

		    if ( (element->flags & FLAG_CLOSE_OPTIONAL) == 0 )
			sgml_note_missing_close (context, oe);

		    force_close_to_matching (context, other_elem);

		    if (old_tos != context->tos)
			applied_rule = TRUE;
		}
		break;
	    case QUIETLY_CLOSE_ANY_OPEN:
		if ( open_within_container(context, tag) )
		    /* if ( is_element_open(context, tag) ) --DL: This is implied by the above */
		{
		    STACK_ITEM *old_tos = context->tos;

		    PRSDBGN(("ensure_pre_requisite%s(): quiet implied closure </%s>\n",
			     element->name.ptr, other_elem->name.ptr));

		    if ( (element->flags & FLAG_CLOSE_OPTIONAL) == 0 )
			sgml_note_missing_close (context, other_elem);

		    force_close_to_matching (context, other_elem);

		    if (old_tos != context->tos)
			applied_rule = TRUE;
		}
		break;
	    case CONTAINER_ENCLOSED_WITHIN:
		if (context->tos != NULL)
		{
		    ELEMENT *within = &context->elements[context->tos->element];

		    if ( (within->flags & FLAG_CONTAINER) != 0 &&
			 within->group == element->group &&
			 within != other_elem)
		    {
			PRSDBGN(("ensure_pre_requisites(%s): container enclosed within: inserting <%s>\n",
				 element->name.ptr, other_elem->name.ptr));
			if (! init_dummy)
			    init_dummy = default_attributes(context, other_elem, &dummy_values);
			sgml_note_missing_open (context, other_elem);
			applied_rule |= perform_element_open (context, other_elem, &dummy_values);
		    }
		}
		break;
	    case INVENT_ONLY_WITHIN:
		can_invent_within = can_invent_within || is_element_open (context, tag);
		break;
	    case REPLACE_WITH:
		invent = other_elem;
		break;
	    }
	}
	
#if DEBUG
	if (applied_rule)
	{
	    PRSDBG(("ensure_pre_requisites: REAPPLYING RULESET\n"));
	}
#endif
	    
    } while (applied_rule);

    context->applying_rules = old_apply;

    return ok ? element : invent;
}

static void ensure_post_requisites (SGMLCTX *context, ELEMENT *element)
{
    VALUES dummy_values;
    BOOL init_dummy = FALSE;
    ACT_ELEM *req;
    const BOOL old_apply = context->applying_rules;

    context->applying_rules = TRUE;

    for (req = element->requirements; req->element != SGML_NO_ELEMENT; req++)
    {
	const action = req->action, tag = req->element;
	ELEMENT *other_elem = &context->elements[tag];

	PRSDBGN(("Ensure postrqs <%s> act %s, elem %s\n",
		element->name.ptr, action_names[action], other_elem->name.ptr));

	switch (action)
	{
	case POST_CLEAR_SEEN_FLAG:
	    PRSDBGN(("ensure_post_requisites(%s): clearing flag for %s\n",
		    element->name.ptr, other_elem->name.ptr));
	    element_clear_bit (context->tos->elements_seen, other_elem->id);
	    break;
	case SHOULD_CONTAIN:
	    if ( element_bit_clear (context->tos->elements_seen, other_elem->id) )
	    {
		PRSDBGN(("ensure_post_requisites(%s): inserting <%s></%s>\n",
			element->name.ptr, other_elem->name.ptr, other_elem->name.ptr));
		if (! init_dummy)
		    init_dummy = default_attributes(context, other_elem, &dummy_values);
		sgml_note_missing_open (context, other_elem);
		perform_element_open (context, other_elem, &dummy_values);
		sgml_note_missing_close (context, other_elem);
		perform_element_close (context, other_elem);
	    }
	    break;
	}
    }
    
    context->applying_rules = old_apply;
}

static BOOL is_bad_close (SGMLCTX *context, ELEMENT *element)
{
    ACT_ELEM *req = (&context->elements[context->tos->element])->requirements;

    return req->action==REPLACES_BAD && req->element == element->id;
}

/*****************************************************************************

  Markup such as tables has non-repeatability requirements (such as
  <CAPTION>), but we also need to handle nested tables. The actions
  done here are really part of the SGML client's definition, but are
  hardcoded here for ease (both of specification in attrconf and
  attrgen.py) and speed.

  SJM: Beware the elements that lurk in context->dont_stack_elements_open.
  Currently only FORM but if anything else goes in this code might need
  additions.
  
  */

static void special_container_open_actions(SGMLCTX *context, ELEMENT *element)
{
    if (element->group == HTML_GROUP_TABLE)
    {
	PRSDBGN(("Resetting table seen values on open of <%s>\n", element->name.ptr));

	element_clear_bit (context->tos->elements_open, HTML_CAPTION);
	element_clear_bit (context->tos->elements_open, HTML_COLGROUPSECTION);
	element_clear_bit (context->tos->elements_open, HTML_COLGROUP);
	element_clear_bit (context->tos->elements_open, HTML_COL);
	element_clear_bit (context->tos->elements_open, HTML_THEAD);
	element_clear_bit (context->tos->elements_open, HTML_TFOOT);
	element_clear_bit (context->tos->elements_open, HTML_TBODY);
	element_clear_bit (context->tos->elements_open, HTML_TR);
	element_clear_bit (context->tos->elements_open, HTML_TH);
	element_clear_bit (context->tos->elements_open, HTML_TD);

	element_clear_bit (context->tos->elements_seen, HTML_CAPTION);
	element_clear_bit (context->tos->elements_seen, HTML_COLGROUPSECTION);
	element_clear_bit (context->tos->elements_seen, HTML_COLGROUP);
	element_clear_bit (context->tos->elements_seen, HTML_COL);
	element_clear_bit (context->tos->elements_seen, HTML_THEAD);
	element_clear_bit (context->tos->elements_seen, HTML_TFOOT);
	element_clear_bit (context->tos->elements_seen, HTML_TBODY);
	element_clear_bit (context->tos->elements_seen, HTML_TR);
	element_clear_bit (context->tos->elements_seen, HTML_TH);
	element_clear_bit (context->tos->elements_seen, HTML_TD);

	context->report.has_tables = TRUE;
    }
    else if (element->group == HTML_GROUP_COLGROUP)
    {
    }
    else if (element->group == HTML_GROUP_FORM)
    {
	PRSDBGN(("Resetting form seen values open\n"));

	element_clear_bit (context->tos->elements_open, HTML_INPUT);
	element_clear_bit (context->tos->elements_open, HTML_SELECT);
	element_clear_bit (context->tos->elements_open, HTML_OPTION);
	element_clear_bit (context->tos->elements_open, HTML_TEXTAREA);

	element_clear_bit (context->tos->elements_seen, HTML_INPUT);
	element_clear_bit (context->tos->elements_seen, HTML_SELECT);
	element_clear_bit (context->tos->elements_seen, HTML_OPTION);
	element_clear_bit (context->tos->elements_seen, HTML_TEXTAREA);

	context->report.has_forms = TRUE;
    }
    else if (element->group == HTML_GROUP_HTML)
    {
    }
    else if (element->group == HTML_GROUP_HEAD)
    {
    }
    else if (element->group == HTML_GROUP_BODY)
    {
	/* DAF: 970627: Be cautious: had to seperate <BODY> with its
           attributes from the element other elements insist on being
           contained with to trigger an automatic title, etc. If you
           want to add something here, you probably need to make a
           change elsewhere as well. */
    }
    else if (element->group == HTML_GROUP_SELECT)
    {
    }
    else if (element->group == HTML_GROUP_LIST)
    {
	PRSDBGN(("Resetting list seen values open\n"));

	element_clear_bit (context->tos->elements_open, HTML_LI);
	element_clear_bit (context->tos->elements_open, HTML_DT);
	element_clear_bit (context->tos->elements_open, HTML_DD);

	element_clear_bit (context->tos->elements_seen, HTML_LI);
	element_clear_bit (context->tos->elements_seen, HTML_DT);
	element_clear_bit (context->tos->elements_seen, HTML_DD);
    }
    else
    {
	PRSDBG(( "Bad special open actions for %s\n", element->name.ptr));
	ASSERT(0==1);
    }

    PRSDBGN(("special_container_open_actions: on exit the stack looks like this:\n"));
    /*dump_stack(context);*/
}

static void special_container_close_actions(SGMLCTX *context, ELEMENT *element)
{
    if (element->group == HTML_GROUP_TABLE)
    {
	PRSDBGN(("Resetting table seen values on close\n"));

	element_clear_bit (context->tos->elements_seen, HTML_CAPTION);
	element_clear_bit (context->tos->elements_seen, HTML_COLGROUPSECTION);
	element_clear_bit (context->tos->elements_seen, HTML_COLGROUP);
	element_clear_bit (context->tos->elements_seen, HTML_COL);
	element_clear_bit (context->tos->elements_seen, HTML_THEAD);
	element_clear_bit (context->tos->elements_seen, HTML_TFOOT);
	element_clear_bit (context->tos->elements_seen, HTML_TBODY);
	element_clear_bit (context->tos->elements_seen, HTML_TR);
	element_clear_bit (context->tos->elements_seen, HTML_TH);
	element_clear_bit (context->tos->elements_seen, HTML_TD);
    }
    else if (element->group == HTML_GROUP_COLGROUP)
    {
    }
    else if (element->group == HTML_GROUP_FORM)
    {
	PRSDBGN(("Resetting form seen values on close\n"));

	element_clear_bit (context->tos->elements_seen, HTML_INPUT);
	element_clear_bit (context->tos->elements_seen, HTML_SELECT);
	element_clear_bit (context->tos->elements_seen, HTML_OPTION);
	element_clear_bit (context->tos->elements_seen, HTML_TEXTAREA);
    }
    else if (element->group == HTML_GROUP_HTML)
    {
    }
    else if (element->group == HTML_GROUP_HEAD)
    {
    }
    else if (element->group == HTML_GROUP_BODY)
    {
    }
    else if (element->group == HTML_GROUP_SELECT)
    {
    }
    else if (element->group == HTML_GROUP_LIST)
    {
	PRSDBGN(("Resetting list seen values on close\n"));

	element_clear_bit (context->tos->elements_seen, HTML_LI);
	element_clear_bit (context->tos->elements_seen, HTML_DT);
	element_clear_bit (context->tos->elements_seen, HTML_DD);
    }
    else
    {
	PRSDBG(("Bad special close actions for %s", element->name.ptr));
	ASSERT(0==1);
    }

    PRSDBGN(("special_container_close_actions: on exit the stack looks like this:\n"));
    /*dump_stack(context);*/
}

/*****************************************************************************

  Return a BOOL indicating whether we changed any state (TRUE) or not
  (FALSE). The rules processing uses this to determine whether a
  further iteration is required.  If TRUE is returned, then the
  overall ruleset must have taken a step closer to the final state, or
  we will spin. */

static BOOL perform_element_open(SGMLCTX *context, ELEMENT *element, VALUES *values)
{
    ELEMENT *other, *bad;
    BOOL nests;
    BOOL did_anything = FALSE;

    PRSDBGN(("<@%s>\n", element->name.ptr));

    TASSERT(context->magic == SGML_MAGIC);

    if (context->tos &&
	((other = &context->elements[context->tos->element])->flags & FLAG_AUTO_CLOSE) != 0 )
    {
	PRSDBG(("Performing autoclose on %s\n", other->name.ptr));
	perform_element_close(context, other);
    }

    if ( (element->flags & FLAG_ONLY_ONCE) != 0 &&
	 element_bit_set (context->tos->elements_seen, element->id) )
    {
	sgml_note_not_repeatable (context, element);
	return did_anything;
    }

    /* This doesn't really belong here */
    if ( (element->flags & FLAG_BLOCK_LEVEL) != 0 &&
	 element_bit_set (context->tos->elements_open, HTML_P) )
    {
	perform_element_close (context, &context->elements [HTML_P]);
#if SGML_REPORTING
	sgml_note_message (context, "A paragraph close would be less ambiguous for <%s>",
			   element->name.ptr);
#endif
    }

    bad = ensure_pre_requisites (context, element);

    if (bad == NULL)
    {
	return did_anything;
    }

    if (element->id != bad->id)
    {
        /* element has been remapped to a 'bad' variant */
        PRSDBG(("perform_element_open(%s): bad - opening %s instead\n", 
                element->name.ptr, bad->name.ptr));
        did_anything |= perform_element_open (context, bad, values);
        return did_anything;
    }

    context->deliver(context, DELIVER_PRE_OPEN_MARKUP, empty_string, element);

    did_anything = TRUE;

    if (context->dlist == NULL || context->force_deliver)
    {
	/* This doesn't really belong here either */
	
	if (context->callback.pre_open != NULL)
	    context->callback.pre_open(context, element);
    }

    sgml_note_good_open (context, element);

    nests = (element->flags & (FLAG_NO_CLOSE | FLAG_CLOSE_INTERNAL)) != FLAG_NO_CLOSE;

    /* This is a real hack to handle <p> the way it */
    /* gets used. Stricly, <p>...</p> is a bracketing */
    /* piece of markup, but it invariably gets used as */
    /* just <p> as seperating markup. This won't work, */
    /* however, when <p align=right>...</p> is used. */

    if ( element->id == HTML_P )
    {
	int i;

	for (nests = FALSE, i = 0; !nests && i < SGML_MAXIMUM_ATTRIBUTES; i++)
	{
	    if ( values->value[i].type != value_none )
		nests = TRUE;
	}

	if (nests)
	{
	    PRSDBG(("Implementing <P ...> as a nesting element\n"));
	}
	else
	{
	    PRSDBGN(("Implementing <P> as a non-nesting element\n"));
	}
    }
    /* Likewise for <A NAME=> ... don't require a </A> */
    else if ( element->id == HTML_A )
    {
	if ( values->value[HTML_A_HREF].type == value_none )
	{
	    nests = FALSE;
	}

	if (nests)
	{
	    PRSDBG(("Implementing <A ...> as a nesting element\n"));
	}
	else
	{
	    PRSDBG(("Implementing <A> as a non-nesting element\n"));
	}
    }

#if SGML_REPORTING
    report_start(context, element, values);
#endif

    if (nests)
    {
	/* ### THE PUSH HAPPENS HERE ### */
	push_stack (context, element);
	element_set_bit (context->tos->elements_open, element->id);

	if ( (element->flags & FLAG_CONTAINER) != 0 )
	    special_container_open_actions (context, element);
    }

    if ( (element->flags & FLAG_END_TAG) != 0 )
    {
	PRSDBGN(("Match: end tag only\n"));
	context->tos->matching_mode = match_end_tag;
    }
    else if ( (element->flags & FLAG_NO_TEXT) != 0 )
    {
	PRSDBGN(("Match: no textual characters expected\n"));
	context->tos->matching_mode = match_unexpected;
    }
    else if ( (element->flags & FLAG_TEXT) != 0 )
    {
	PRSDBGN(("Match: textual characters accepted\n"));
	context->tos->matching_mode = 0;
    }

    element_set_bit (context->tos->elements_seen, element->id);

    if (context->dlist == NULL || context->force_deliver)
    {
	/* Call the element specific actions */
	(*element->element_open) (context, element, values);

	context->force_deliver = FALSE;
    }

    /* Do things like post space/break actions */
    (*context->deliver) (context, DELIVER_POST_OPEN_MARKUP, empty_string, element);

    return did_anything;
}

/*****************************************************************************/

extern void perform_element_close(SGMLCTX *context, ELEMENT *element)
{
    ELEMENT *other;

    ASSERT(context->magic == SGML_MAGIC);

    PRSDBGN(("<@/%s>\n", element->name.ptr));

    if (context->tos &&
	((other = &context->elements[context->tos->element])->flags & FLAG_AUTO_CLOSE) != 0 )
    {
	/* Avoid auto-close if got </STYLE> */
	if (element->id != other->id)
	{
	    PRSDBG(("Performing autoclose on %s\n", other->name.ptr));
	    perform_element_close(context, other);
	}
    }

    if ( (element->flags & (FLAG_NO_CLOSE | FLAG_CLOSE_INTERNAL)) == FLAG_NO_CLOSE )
    {
#if SGML_REPORTING
	sgml_note_message(context, "No closing markup expected for <%s>", element->name.ptr);
#endif
    }
    else if ( context->tos->element == element->id || -context->tos->element == element->id )
                /* DL: DONT_STACK rule abolished */
    {
	sgml_note_good_close(context, element);

	ensure_post_requisites (context, element);

	(*context->deliver) (context, DELIVER_PRE_CLOSE_MARKUP, empty_string, element);

	if ( (element->flags & FLAG_CONTAINER) != 0 )
	    special_container_close_actions (context, element);

	if (context->dlist == NULL || context->force_deliver)
	{
	    (*element->element_close) (context, element);
	    context->force_deliver = FALSE;
	}

	/* ### THE POP HAPPENS HERE ### */
	pop_stack (context);

	report_finish(context, element);

	/* Post space/break stuff */
	(*context->deliver) (context, DELIVER_POST_CLOSE_MARKUP, empty_string, element);
    }
    else if (is_bad_close (context, element))
    {
        PRSDBG(("perform_element_close(%s): bad - closing %s instead\n", 
                element->name.ptr, &context->elements[context->tos->element].name.ptr));

        perform_element_close (context, &context->elements[context->tos->element]);
    }   
    else if ( element_bit_set (context->tos->elements_open, element->id) )
    {
        /* DL: removed old OUT_OF_ORDER_CLOSE - obsolete experiment ? */

            ASSERT(context->tos != NULL);
      
	    if (element->flags & FLAG_OUT_OF_ORDER_CLOSE)
	    {
                /* The following replaces the phantom close scheme with a reordered close */
                  STACK_ITEM *matching_close = find_element_in_stack (context, element);
  
                if (matching_close != NULL)
                {
                    PRSDBG(("perform_element_close(%s): pulling close to top of stack\n",
                            element->name.ptr));
                    pull_stack_item_to_top_correcting_effects (context, matching_close);
                    perform_element_close (context, element);
                    sgml_note_missing_close (context, element);
                }
                /* Otherwise we saw it earlier and the elements_open is lying */
            }
            else if (element_bit_set (context->tos->elements_open, element->id))
                force_close_to_matching (context, element);
    }
    else
    {
	BOOL closed_early = FALSE;
	STACK_ITEM *inner;

	if (context->tos == NULL)
	{
	    PRSDBG(("No stack - close without open?\n"));
	}
	else
	{
	    inner = context->tos->inner;
	    while ( inner != NULL )
	    {
		if (inner->element == -element->id)
		{
		    inner->element = -inner->element;
		    closed_early = TRUE;
		    break;
		}
		inner = inner->inner;
	    }
	}

	/* SJM: Horrible idea to ensure that /P items always get called to ensure line breaks */
	if (element->id == HTML_P)
	{
	    (*context->deliver) (context, DELIVER_PRE_CLOSE_MARKUP, empty_string, element);

	    if (context->dlist == NULL)
	    {
		(*element->element_close) (context, element);
	    }

	    (*context->deliver) (context, DELIVER_POST_CLOSE_MARKUP, empty_string, element);
	}
	else if (closed_early)
	{
	    /*note_message(context, "Already popped matching </%s>", element->name.ptr);*/
	}
	else
	{
	    PRSDBGN(("perform_element_close(%s): close without open!\n",
		    element->name.ptr));
	    sgml_note_close_without_open (context, element);
	}
    }
}

/*****************************************************************************/

/* This was so much more elegant in the original ALGOL version. */

static void parse_then_perform_element_open(SGMLCTX *context)
{
    USTRING s, attribute_string, value_string;
    ELEMENT *element;
    UCHARACTER *inhand = context->inhand.data;
    int six = 1, eix = 2, nix, elem_number, attribute_number, ap = 1;
    VALUES values;
    int guessed[(SGML_MAXIMUM_ATTRIBUTES+31)/32];

    ASSERT(context->magic == SGML_MAGIC);

    ASSERT(eix <= context->inhand.ix);

    PRSDBGN(("\n-----------------------------------------------------------------------------\nparse_then_perform_element_open '%.*s'\n", context->inhand.ix, inhand));

    /* clear out the guessed array */
    memset(guessed, 0, sizeof(guessed));
    
    while ( is_element_body_character( inhand[eix] ) )
	eix++;

    s.ptr = inhand + six;
    s.nchars = eix - six;
    elem_number = find_element(context, s);

    if (elem_number == SGML_NO_ELEMENT)
    {
#if SGML_REPORTING
	sgml_note_unknown_element(context, "Unknown element <%.*s>", min(s.nchars, MAXSTRING), usafe(s));
#endif

	/* Zero the attribute values; the 'done' code tries to free them! */
	memset(&values, 0, sizeof(values));

	goto done;
    }

    element = &context->elements[elem_number];

    default_attributes(context, element, &values);

    nix = eix;

    while (1)
    {
	BOOL this_guessed;

	ASSERT(nix <= context->inhand.ix);

	six = nix;
	while ( is_whitespace( inhand[six] ) )
	    six++;

	if ( inhand[six] == '>' )
	    break;

	ASSERT( is_attribute_start_character( inhand[six] ));

	eix = six + 1;
	while ( is_attribute_body_character( inhand [eix] ) )
	    eix ++;

	attribute_string.ptr = &inhand[six];
	attribute_string.nchars = eix - six;

	six = eix;
	while ( is_whitespace( inhand [six] ) )
	    six++;

	if ( inhand[six] == '=' )
	{
	    six += 1;
	    while (is_whitespace( inhand [six] ) )
		six++;

	    if ( inhand[six] == single_quote || inhand[six] == double_quote )
	    {
		/* A quoted string will contain everything but the quotes (including newlines etc) */
		if ( inhand [six + 1] == inhand[six] )
		{
		    eix = six;
		    nix = six + 2;
		}
		else
		{
		    eix = six + 2;
		    while ( inhand [eix] != inhand [six] )
			eix++;
		    six += 1;
		    nix = eix + 1;
		}
	    }
	    else if ( inhand[six] == '>' )
	    {
		/* no attribute at all */
		eix = nix = six;
	    }
	    else
	    {
		/* no quotes around attribute literal */
		ASSERT( is_value_start_character ( inhand[six] ) );
		eix = six + 1;
		while ( is_value_body_character( inhand[eix] ) )
		    eix++;
		if ( inhand[eix] == single_quote || inhand[eix] == double_quote )
		{
		    nix = eix + 1;
		}
		else
		{
		    nix = eix;
		}
	    }
	}
	else if ( inhand[six] == '>' )
	{
	    eix = nix = six;
	}
	else if (is_attribute_start_character ( inhand [six] ) )
	{
	    eix = nix = six;
	}
	else
	{
	    ASSERT(0==1);
	}

	value_string.ptr = &inhand[six];
	value_string.nchars = eix - six;

	attribute_number = find_attribute (context, element, attribute_string, &this_guessed);

	if (attribute_number == SGML_NO_ATTRIBUTE)
	{
#if SGML_REPORTING
	    sgml_note_unknown_attribute(context, "Unknown attribute in <%s %.*s>",
					element->name.ptr,
					min(attribute_string.nchars, MAXSTRING),
					usafe(attribute_string));
#endif
	}
	/* only ignore if duplicated and the first try wasn't a guess */
	else if ( values.value[attribute_number].type != value_none && !BIT_TST(guessed, attribute_number) )
	{
#if SGML_REPORTING
	    sgml_note_message(context, "Duplicated attribute in <%s %.*s>%s",
			      element->name.ptr,
			      min(attribute_string.nchars, MAXSTRING),
			      usafe(attribute_string),
			      this_guessed ? " (guessed)" : "");
#endif
	}
	else
	{
	    ATTRIBUTE *attribute = element->attributes[attribute_number];

	    /* set the guessed bit */
	    if (this_guessed)
	    {
		BIT_SET(guessed, attribute_number);
	    }
	    
	    /* This should supply attribute values expanding entities */
 	    value_string.nchars = sgml_translation(context, value_string.ptr, value_string.nchars,
 				     SGMLTRANS_AMPERSAND | SGMLTRANS_HASH | SGMLTRANS_STRIP_NEWLINES | SGMLTRANS_STRICT);

	    values.value [attribute_number] =
		(*parse_table [attribute->parse])
		(context, attribute, value_string );
	    /* This numbers from 1, so 0 means not present */
	    values.value [attribute_number].tag = ap++;
	    PRSDBGN(("Assigning tag %d to attribute number %d, type %d @ %p\n",
		    ap-1, attribute_number, values.value[attribute_number].type, &values.value[attribute_number]));
	}
    }

    perform_element_open (context, element, &values);

done:
    sgml_free_attributes(&values);

    return;
}

static void parse_then_perform_element_close(SGMLCTX *context)
{
    USTRING s;
    ELEMENT *element;
    UCHARACTER *inhand = context->inhand.data;
    int six = 2, eix = 3;
    int elem_number;

    ASSERT(context->magic == SGML_MAGIC);

#if DEBUG
    s.ptr = inhand;
    s.nchars = context->inhand.ix;
    PRSDBGN(("\n-----------------------------------------------------------------------------\nparse_then_perform_element_close '%.*s'\n",
	     context->inhand.ix, usafe(s)));
#endif
    
    while ( is_element_body_character( inhand[eix] ) )
	eix++;

    s.ptr = inhand + six;
    s.nchars = eix - six;

    elem_number = find_element (context, s);

    if ( elem_number == SGML_NO_ELEMENT )
    {
#if SGML_REPORTING
	sgml_note_unknown_element(context, "Unknown element <%.*s>", min(s.nchars, MAXSTRING), usafe(s));
#endif
	goto done;
    }

    element = &context->elements[elem_number];
    six = eix;
    while ( is_whitespace( inhand[six] ) )
	six++;

    if ( inhand[six] != '>' )
    {
#if SGML_REPORTING
	sgml_note_message(context, "</%s> has unexpected attributes", element->name.ptr);
#endif
/*  goto done; SJM: 21/05/97: removed this goto. Even if it does have
			      unexpected attributes we still probably
			      want to obey the close */
    }

    perform_element_close(context, element);

done:
    return;
}


/*****************************************************************************

  do_got_element(): Have recognised some markup. Decide what sort and
  despatch actions appropriately.

  The ordering of things here is quite important.

  1) The chopper must be flushed first, as it operates
     semi-asynchronously form the other DELIVER_BLAH items otherwise.
  2) The relevant markup routine is called. See below.
  3) The inhand buffering is cleared.

  Markup open is performed as

  1) The element's pre-requisites are performed
  2) DELIVER_PRE_MARKUP - any pre break/strip actions happen here.
  3) A stack item is pushed if the element can be closed
  4) special_container_open_actions() might be called
  ?) select new matching mode
  6) Record the element as open
  7) Call the element specific open function
  8) DELIVER_POST_MARKUP

  Markup close is performed as:

  1) post requisites are performed
  2) DELIVER_PRE_MARKUP
  3) maybe call special_container_close_actions()
  4) Call the element specific close function
  5) pop the stack
  6) DELIVER_POST_MARKUP

  Note that DELIVER_SGML has no DELIVER_PRE_MARKUP or DELIVER_POST_MARKUP
  deliveries.

  */

extern void do_got_element(SGMLCTX *context)
{
    static USTRING s = { NULL, 0 };

    /* inhand holds precisely the open < to the close > */

    UBUFFER *inhand = &context->inhand;
    UCHARACTER sw = inhand->data[1];

    ASSERT(context->magic == SGML_MAGIC);
    ASSERT( inhand->data != NULL );
    ASSERT( inhand->ix >= 2 );

    (*context->chopper) (context, s); /* Flush */

    if (sw == '!')
    {
	(*context->deliver) (context,
			     DELIVER_SGML,
			     mkstringu(context->encoding_write, context->inhand.data, context->inhand.ix), NULL );
	clear_inhand(context);
    }
    else if (sw == '/')
    {
	parse_then_perform_element_close(context);
    }
    else if (sw == '>')
    {
	/* Elide <> to nothing */
#if SGML_REPORTING
	sgml_note_message(context, "Warning: empty markup '<>'");
#endif
    }
    else
    {
	parse_then_perform_element_open(context);

	PRSDBG(("do_got_element: set strip initial new line, matching mode=%d, tos->element %d (%d)\n",
		context->tos->matching_mode,
		context->tos->element, HTML_PRE));
#if 0
	/* SJM: removed because of problems with elements within PRE zone stripping newlines */
	context->strip_initial_nl = context->strip_initial_cr = 1;
#endif
    }

    clear_inhand(context);

    context->state = get_state_proc(context);
}

/* eof elements.c */
