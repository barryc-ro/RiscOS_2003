/* forms.c - <FORM> <INPUT> <SELECT> <TEXTAREA> <OPTION> <MENU> */

/* CHANGELOG
 * 19/07/96: SJM: push_input() passes MAXLENGTH rather than MAX and MIN
 * 23/07/96: SJM: sgmltranslate added to option strings
 * 02/08/96: SJM: check that exactly one RADIO button is selected.
 * 07/08/96: SJM: check that at least one OPTION is selected.
 * 04/09/96: SJM: only check one SELECT is selected if not multiple.
 */


#include "htmlparser.h"

extern void startform (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    new_form_item(htmlctxof(context),
		  &attributes->value[HTML_FORM_ACTION],
		  &attributes->value[HTML_FORM_METHOD],
		  &attributes->value[HTML_FORM_TARGET],
		  &attributes->value[HTML_FORM_ID]);
}

#if 0
/* On the first checked set the flag, on others clear checked flag */
static void check_radio_1(rid_form_element *fe, void *handle)
{
    rid_input_item *ii = (rid_input_item *)fe;
    int *pselected = handle;

    if (ii->flags & rid_if_CHECKED)
    {
	if (*pselected)
	{
	    ii->flags &= ~rid_if_CHECKED;
	    ii->data.tick = 0;
	}
	else
	    *pselected = 1;
    }

    /* Mark as having been scanned */
    ii->flags |= rid_if_SCANNED;
}

/* This function is called for every RADIO item. 
 * It then scans the rest of the items for any in the same group to ensure only one selected
 * It marks those looked at to avoid overdoing it too much.
 */

static void check_radio(rid_form_element *fe, void *handle)
{
    rid_input_item *ii = (rid_input_item *)fe;

    /* If done already then just skip */
    if ((ii->flags & rid_if_SCANNED) == 0)
    {
	int selected = 0;

	form_element_enumerate(fe->parent, rid_form_element_INPUT, rid_it_RADIO, ii->name, check_radio_1, &selected);

	/* If no items were checked then check this one */
	if (selected == 0)
	{
	    ii->flags |= rid_if_CHECKED;
	    ii->data.tick = 1;
	}	
    }
}

#endif

extern void finishform (SGMLCTX * context, ELEMENT * element)
{
    /*HTMLCTX *me = htmlctxof(context);*/

    generic_finish (context, element);

    /* check that form items are as they should be */

    /* check that each radio group has one and only one button checked */
    /* remove this check as despite it being in the spec, other browsers don't do it and
       some crappy pages depend on non being set initially
       form_element_enumerate(me->form, rid_form_element_INPUT, rid_it_RADIO, form_element_enumerate_ALL, check_radio, NULL);
       */
}

/*****************************************************************************/

extern void startinput (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    text_item_push_input(me, 0,
			 &attributes->value[HTML_INPUT_ALIGN],
			 &attributes->value[HTML_INPUT_CHECKED],
			 &attributes->value[HTML_INPUT_DISABLED],
			 &attributes->value[HTML_INPUT_MAXLENGTH],
			 &attributes->value[HTML_INPUT_NAME],
			 &attributes->value[HTML_INPUT_SIZE],
			 &attributes->value[HTML_INPUT_SRC],
			 &attributes->value[HTML_INPUT_TYPE],
			 &attributes->value[HTML_INPUT_VALUE],
			 &attributes->value[HTML_INPUT_ID],
			 &attributes->value[HTML_INPUT_BGCOLOR],
			 &attributes->value[HTML_INPUT_SELCOLOR],
			 &attributes->value[HTML_INPUT_CURSOR],
			 &attributes->value[HTML_INPUT_NOCURSOR],
			 &attributes->value[HTML_INPUT_NUMBERS]);
}

/*****************************************************************************/

extern void startselect (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    text_item_push_select(htmlctxof(context),
			  &attributes->value[HTML_SELECT_NAME],
			  &attributes->value[HTML_SELECT_SIZE],
			  &attributes->value[HTML_SELECT_MULTIPLE],
			  &attributes->value[HTML_SELECT_ID],
			  &attributes->value[HTML_SELECT_BGCOLOR],
			  &attributes->value[HTML_SELECT_SELCOLOR],
			  &attributes->value[HTML_SELECT_NOPOPUP]);

    /*select_str_mode(htmlctxof(context));*/
}

extern void finishselect (SGMLCTX * context, ELEMENT * element)
{    
    HTMLCTX *me = htmlctxof(context);
    rid_option_item *oi;
    rid_select_item *sel;

    generic_finish (context, element);	

    if (me->form == NULL || (sel = me->form->last_select) == NULL)
	return;

    /* ANC-00373 */
    if ((sel->flags & rid_sf_MULTIPLE) == 0)
    {
	/* Check that exactly one OPTION item is checked */
	BOOL checked = FALSE;
	for (oi = sel->options; oi; oi = oi->next)
	{
	    if (oi->flags & rid_if_CHECKED)
	    {
		if (checked)
		    oi->flags &= ~rid_if_CHECKED;
		else
		    checked = TRUE;
	    }
	}

	/* if not then select the first one */
	if (!checked && sel->options)
	    sel->options->flags |= rid_if_CHECKED;
    }
}

/*****************************************************************************

  We do not care about line boundaries for options, so we gather them
  into a single string.

  */

extern void startoption (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    new_option_item(me,
		    &attributes->value[HTML_OPTION_VALUE],
		    (attributes->value[HTML_OPTION_DISABLED].type != value_none ? rid_if_DISABLED : 0) +
		    (attributes->value[HTML_OPTION_SELECTED].type != value_none ? rid_if_CHECKED : 0) );

    ASSERT(me->last_mode == HTMLMODE_BOGUS);

    me->last_mode = me->mode;

    select_str_mode(me);
}

extern void finishoption (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *me = htmlctxof(context);
    rid_option_item *opt;

    generic_finish (context, element);

    if (me->form && me->form->last_select && me->form->last_select->last_option)
    {
        STRING s;

	opt = me->form->last_select->last_option;

	PRSDBG(("Option text='%.*s'\n", me->inhand_string.bytes, me->inhand_string.ptr));

        s = string_strip_space(me->inhand_string);

        /* Expand the entities and strip newlines */
        s.bytes = sgml_translation(context, s.ptr, s.bytes, SGMLTRANS_STRIP_NEWLINES | SGMLTRANS_HASH | SGMLTRANS_AMPERSAND);

	opt->text = stringdup(s);
    }
    else
    {
	PRSDBG(("finishoption(): don't want the option ????\n"));
	string_free(&me->inhand_string);
    }

    select_last_mode(me);
}

/*****************************************************************************

  Gather the lines of a text area. We use pre_mode to get delivery a
  line at a time. Because we might be in fmt or pre mode already, we
  need to record this so that we can correctly restore the mode. As we
  cannot recurse, only a single item 'stack' is used. Look at
  pre_deliver_word() in deliver.c to see where the lines are added to
  the text area.

  */

extern void starttextarea (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    text_item_push_textarea(me,
			    &attributes->value[HTML_TEXTAREA_NAME],
			    &attributes->value[HTML_TEXTAREA_ROWS],
			    &attributes->value[HTML_TEXTAREA_COLS],
			    &attributes->value[HTML_TEXTAREA_ID],
			    &attributes->value[HTML_TEXTAREA_BGCOLOR],
			    &attributes->value[HTML_TEXTAREA_SELCOLOR],
			    &attributes->value[HTML_TEXTAREA_CURSOR]);

    ASSERT(me->last_mode == HTMLMODE_BOGUS);

    me->last_mode = me->mode;

    select_pre_mode(me);
}

extern void finishtextarea (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *me = htmlctxof(context);

    generic_finish (context, element);

    select_last_mode(me);
}

/*****************************************************************************/



/* eof forms.c */
