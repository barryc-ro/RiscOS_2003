/* forms.c - <FORM> <INPUT> <SELECT> <TEXTAREA> <OPTION> <MENU> */

/* CHANGELOG
 * 19/07/96: SJM: push_input() passes MAXLENGTH rather than MAX and MIN
 * 23/07/96: SJM: sgmltranslate added to option strings
 * 02/08/96: SJM: check that exactly one RADIO button is selected.
 * 07/08/96: SJM: check that at least one OPTION is selected.
 * 04/09/96: SJM: only check one SELECT is selected if not multiple.
 * 27/02/97: SJM: Moved text_item_push_input() code into here.
 * 29/04/97: SJM: Changed INPUT ww and hh to being VALUEs.
 */


#include "htmlparser.h"
#include "webfonts.h"

#if UNICODE
#include "Unicode/charsets.h"
#endif

/* extern void translate_escaped_text(char *src, char *dest, int len); */

extern void startform (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    new_form_item(htmlctxof(context),
		  &attributes->value[HTML_FORM_ACTION],
		  &attributes->value[HTML_FORM_METHOD],
		  &attributes->value[HTML_FORM_TARGET],
		  &attributes->value[HTML_FORM_ID],
		  &attributes->value[HTML_FORM_ENCTYPE],
		  &attributes->value[HTML_FORM_ACCEPT_CHARSET]);
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
    rid_text_item_input *new;
    rid_text_item *nb = NULL;
    rid_input_item *in;
    rid_input_tag tag = (rid_input_tag) -1;

    VALUE *checked;
    VALUE *disabled;
    VALUE *nocursor;
    VALUE *numbers;
    VALUE *type;
    VALUE *maxlength;
    VALUE *size;
/*     VALUE *width, *height; */
    VALUE *tabindex;
    VALUE *border;

    generic_start (context, element, attributes);

    PRSDBG(("text_item_push_input(%p)\n", me));

    type = &attributes->value[HTML_INPUT_TYPE];
    switch (type->type == value_enum ? type->u.i : HTML_INPUT_TYPE_TEXT)
    {
    case HTML_INPUT_TYPE_TEXT:
	tag = rid_it_TEXT;
	break;
    case HTML_INPUT_TYPE_PASSWORD:
	tag = rid_it_PASSWD;
	break;
    case HTML_INPUT_TYPE_CHECKBOX:
	tag = rid_it_CHECK;
	break;
    case HTML_INPUT_TYPE_RADIO:
	tag = rid_it_RADIO;
	break;
    case HTML_INPUT_TYPE_IMAGE:
	tag = rid_it_IMAGE;
	break;
    case HTML_INPUT_TYPE_HIDDEN:
	tag = rid_it_HIDDEN;
	break;
    case HTML_INPUT_TYPE_SUBMIT:
	tag = rid_it_SUBMIT;
	break;
    case HTML_INPUT_TYPE_RESET:
	tag = rid_it_RESET;
	break;
    case HTML_INPUT_TYPE_BUTTON:
	tag = rid_it_BUTTON;
	break;
    }

    in = mm_calloc(1, sizeof(*in));

    if (tag != rid_it_HIDDEN)
    {
	new = mm_calloc(1, sizeof(*new));
	new->input = in;
	nb = &(new->base);
	in->base.display = nb;
    }
    else
	new = NULL;

    in->base.tag = rid_form_element_INPUT;
    in->tag = tag;

    in->base.id = valuestringdup(&attributes->value[HTML_INPUT_ID]);
    htmlriscos_colour(&attributes->value[HTML_INPUT_BGCOLOR], &in->base.colours.back);
    htmlriscos_colour(&attributes->value[HTML_INPUT_SELCOLOR], &in->base.colours.select);
    htmlriscos_colour(&attributes->value[HTML_INPUT_CURSOR], &in->base.colours.cursor);

    nocursor = &attributes->value[HTML_INPUT_NOCURSOR];
    if (nocursor->type != value_none)
	in->flags |= rid_if_NOCURSOR;

    /* some nasty NCFresco special code */
    numbers = &attributes->value[HTML_INPUT_NUMBERS];
    if (numbers->type != value_none)
    {
	in->flags |= rid_if_NUMBERS;
	if (numbers->type == value_enum && numbers->u.i == HTML_INPUT_NUMBERS_PBX)
	    in->flags |= rid_if_PBX;
    }

    checked = &attributes->value[HTML_INPUT_CHECKED];
    if (checked->type != value_none)
	in->flags |= rid_if_CHECKED;

    disabled = &attributes->value[HTML_INPUT_DISABLED];
    if (disabled->type != value_none)
	in->flags |= rid_if_DISABLED;

    in->name = valuestringdup(&attributes->value[HTML_INPUT_NAME]);
    in->value = valuestringdup(&attributes->value[HTML_INPUT_VALUE]);

    if (tag == rid_it_SUBMIT || tag == rid_it_RESET || tag == rid_it_BUTTON || tag == rid_it_RADIO || tag == rid_it_CHECK)
    {
	in->src = valuestringdup(&attributes->value[HTML_INPUT_BORDERIMAGE]);
	in->src_sel = valuestringdup(&attributes->value[HTML_INPUT_SELIMAGE]);
    }
    else
	in->src = valuestringdup(&attributes->value[HTML_INPUT_SRC]);

    size = &attributes->value[HTML_INPUT_SIZE];
#if 0
    in->xsize = in->ysize = -1;
    /* NOTE: If SHORTISH is defined as short, rather than int, then */
    /* this scanf needs to be %hd if stray memory is not to be written! */

    if (size->type == value_string)
    {
	sscanf(size->u.s.ptr, "%hd,%hd", &in->xsize, &in->ysize);
    }
    if (in->xsize == -1)
	in->xsize = 20;
#else
    in->xsize = size->type == value_integer ? size->u.i : -1;
#endif

    tabindex = &attributes->value[HTML_INPUT_TABINDEX];
    in->base.tabindex = tabindex->type == value_integer ? tabindex->u.i : -2; /* have to use -2 as default as -1 means unselectable */

#if 1
    in->ww = attributes->value[HTML_INPUT_WIDTH];
    in->hh = attributes->value[HTML_INPUT_HEIGHT];
#else
    width = &attributes->value[HTML_INPUT_WIDTH];
    in->ww = width;

    height = &attributes->value[HTML_INPUT_HEIGHT];
    in->hh = height->type == value_integer ? height->u.i : -1;
#endif

    border = &attributes->value[HTML_INPUT_BORDER];
    in->bw = border->type == value_integer ? border->u.i : 2; /* default border of 2 pixels on all input items */

    maxlength = &attributes->value[HTML_INPUT_MAXLENGTH];
    if (maxlength->type == value_integer)
	in->max_len = maxlength->u.i;

    if (in->max_len == 0)
	in->max_len = 256;

    if (me->form)
	rid_form_element_connect(me->form, &in->base);

    switch (tag)
    {
    case rid_it_CHECK:
    case rid_it_RADIO:
	in->data.radio.tick = ((in->flags & rid_if_CHECKED) != 0);
	break;
    case rid_it_TEXT:
    case rid_it_PASSWD:
#if UNICODE
	if (context->enc_num_write == csUTF8)
	    in->data.str = mm_malloc(in->max_len*6 + 1); /* allocate maximum possible in UTF8 */
	else
#endif
	    in->data.str = mm_malloc(in->max_len + 1); /* SJM: Add 1 for the terminating null, was added to max_len originally */
	if (in->value)
	{
/* 	    translate_escaped_text(in->value, in->data.str, in->max_len + 1); */ /* add one here as len is len of output buffer */
	    strcpy(in->data.str, in->value);
	}
	else
	{
	    in->data.str[0] = 0;
	}

#ifdef STBWEB
	/* we've used WIDTH as a pixel width and some morons out there use WIDTH when they mean SIZE
	 * so WIDTH is only understood if SIZE is also specified
	 */
	if (in->ww.type != value_none && in->xsize == -1)
	    in->ww.type = value_none;
#endif
	break;
    case rid_it_IMAGE:
    {
	VALUE *align = &attributes->value[HTML_INPUT_ALIGN];
	decode_img_align(align->type == value_enum ? align->u.i : -1, &in->data.image.flags, &nb->flag);
	break;
    }
    }

    if (nb)
    {
	nb->tag = rid_tag_INPUT;
/* 	if (flags & rid_flag_LINE_BREAK) */
/* 	    nb->flag |= rid_flag_LINE_BREAK; */
#if NEW_BREAKS
	if (me->mode == HTMLMODE_PRE || me->no_break) /* We need to be able to have both flags set */
	    SET_BREAK(nb->flag, rid_break_MUST_NOT);
#else
	if (me->mode == HTMLMODE_PRE || me->no_break) /* We need to be able to have both flags set */
	    nb->flag |= rid_flag_NO_BREAK;
#endif

#if UNICODE
	if (in->value && webfont_need_wide_font(in->value, strlen(in->value)))
	    nb->flag |= rid_flag_WIDE_FONT;
#endif

	nb->aref = me->aref;	/* Current anchor, or NULL */
	if (me->aref && me->aref->first == NULL)
	    me->aref->first = nb;
	GET_ROSTYLE(nb->st);
	nb->language = UNPACK(me->sgmlctx->tos->effects_active, LANG_NUM);

	rid_text_item_connect(me->rh->curstream, nb);
    }
}

/*****************************************************************************/

extern void startselect (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    set_lang(context, &attributes->value[HTML_SELECT_LANG]);

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

    /* mark that we are finished */
    sel->flags |= rid_sf_FINISHED;
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
		    &attributes->value[HTML_OPTION_LANG],
		    (attributes->value[HTML_OPTION_DISABLED].type != value_none ? rid_if_DISABLED : 0) +
		    (attributes->value[HTML_OPTION_SELECTED].type != value_none ? rid_if_CHECKED : 0));

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
        s = me->inhand_string;

        /* Expand the entities and strip newlines */
/*       s.nchars = sgml_translation(context, s.ptr, s.nchars, SGMLTRANS_STRIP_NEWLINES | SGMLTRANS_HASH | SGMLTRANS_AMPERSAND | SGMLTRANS_STRIP_CTRL); */

	/* SJM: FIXME: check this still works, entity translation should have been done in state machine */
	opt->text = stringdup(string_strip_space(s));

	PRSDBG(("Option text='%s'\n", opt->text));

	/* check for wide font necessity and record in main item */
	PRSDBG(("finishoption: item %p flags %x\n", me->rh->curstream->text_last, me->rh->curstream->text_last->flag));

	if ((me->rh->curstream->text_last->flag & rid_flag_WIDE_FONT) == 0)
	{
	    if (webfont_need_wide_font(s.ptr, s.nchars))
	    {
		PRSDBG(("finishoption: wide\n"));
		me->rh->curstream->text_last->flag |= rid_flag_WIDE_FONT;
	    }
	}
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
			    &attributes->value[HTML_TEXTAREA_CURSOR],
			    &attributes->value[HTML_TEXTAREA_TABINDEX],
			    &attributes->value[HTML_TEXTAREA_WRAP]);

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

extern void startlabel (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *htmlctx = htmlctxof(context);
    VALUE none, empty;
    rid_aref_item *aref;

    generic_start (context, element, attributes);

    none.type = value_none;
    empty.type = value_void;
    aref = new_aref_item(htmlctx,
			 attributes->value[HTML_LABEL_FOR].type == value_string ? &attributes->value[HTML_LABEL_FOR] : &empty,
			 attributes->value[HTML_LABEL_ID].type == value_string ? &attributes->value[HTML_LABEL_ID] : &empty,
			 &none, &none, &none);
    aref->flags |= rid_aref_LABEL;
}

extern void finishlabel (SGMLCTX * context, ELEMENT * element)
{
    generic_finish (context, element);

    finisha(context, element);
}

/*****************************************************************************/

/* eof forms.c */
