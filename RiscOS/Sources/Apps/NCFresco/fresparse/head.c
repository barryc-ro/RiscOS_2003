/* head.c - <TITLE> <ISINDEX> <BASE> <STYLE> <SCRIPT> <META> <LINK> */

/* CHANGE LOG:
 * 02/07/96: SJM: subtly changed META handling to avoid unnamed CONTENT from being added.
 * 18/07/96: SJM: fixed bug in META (used name rather than content).
 * 23/07/96: SJM: sgmltranslate added to title strings
 * 08/08/96: SJM: changed the way parse_http_header works
 * 23/08/96: DAF: <TITLE> appends and can occur multiply.
 */


#include "memwatch.h"
#include "util.h"
#include "htmlparser.h"

#if UNICODE
#include "encoding.h"
#endif

/* Collect text into a temporary string */

extern void starttitle (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *htmlctx = (HTMLCTX *)context->clictx;

    generic_start (context, element, attributes);

    /* Actually this should replace any entities that are found in the string */
    select_str_mode(htmlctx);
}

/* Copy the string as the title. Multiple titles append */

extern void finishtitle (SGMLCTX * context, ELEMENT * element)
{
    HTMLCTX *htmlctx = (HTMLCTX *)context->clictx;
    STRING ss;

    generic_finish (context, element);

    ss = string_strip_space(htmlctx->inhand_string);

    if (ss.bytes)
    {
        /* Expand the entities and strip newlines (we've already stripped spaces) */
	/* removed as entities are already expanded and space and controls are already stripped */
/*         ss.bytes = sgml_translation(context, ss.ptr, ss.bytes, SGMLTRANS_STRIP_NEWLINES | SGMLTRANS_HASH | SGMLTRANS_AMPERSAND | SGMLTRANS_STRIP_CTRL); */

	if (htmlctx->rh->title != NULL)
	{
	    const int o = strlen(htmlctx->rh->title);
	    char *new = mm_realloc(htmlctx->rh->title, o + ss.bytes + 1);

	    if (new != NULL)
	    {
		htmlctx->rh->title = new;
		memcpy(new + o, ss.ptr, ss.bytes);
		new[o + ss.bytes] = 0;
	    }
	}
	else
	{
	    htmlctx->rh->title = stringdup(ss);
	}

	PRSDBG(("Set title to '%s'\n", htmlctx->rh->title));

	string_free(&htmlctx->inhand_string);
    }
    else
    {
	PRSDBG(("No title give, ensuring pointer is NULL"));
	htmlctx->rh->title = NULL;
    }

    select_fmt_mode(htmlctx);
}


extern void startisindex (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    VALUE none;
    generic_start (context, element, attributes);

    me->rh->flags |= rid_hf_ISINDEX;
    none.type = value_none;

    text_item_push_hr(me, &none, &none, &none, &none);

    /* attributes are destroyed as this recurses */
    push_fake_search_form(me, &attributes->value[HTML_ISINDEX_PROMPT]);

    text_item_push_hr(me, &none, &none, &none, &none);
}

extern void startbase (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    VALUE *vp;

    generic_start (context, element, attributes);

    vp = &attributes->value[HTML_BASE_HREF];
    if (vp->type == value_string)
	me->rh->base = stringdup(vp->u.s);

    vp = &attributes->value[HTML_BASE_TARGET];
    if (vp->type == value_string)
    {
	nullfree((void**)&me->basetarget);
	me->basetarget = stringdup(vp->u.s);
    }
}

extern void startstyle (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    select_str_mode(htmlctxof(context));
}

extern void startscript (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    generic_start (context, element, attributes);

    select_str_mode(htmlctxof(context));
}

extern void startmeta (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);

    generic_start (context, element, attributes);

    if ( attributes->value[HTML_META_NAME].type == value_string ||
        attributes->value[HTML_META_HTTP_EQUIV].type == value_string )
    {
	rid_meta_item *m = mm_calloc(sizeof(rid_meta_item), 1);

	m->name = valuestringdup(&attributes->value[HTML_META_NAME]);
	m->httpequiv = valuestringdup(&attributes->value[HTML_META_HTTP_EQUIV]);
	m->content = valuestringdup(&attributes->value[HTML_META_CONTENT]);
	rid_meta_connect(me->rh, m);

#if UNICODE
	/* only set encoding from here if not set by HTTP header or user */
	if (strcasecomp((m->httpequiv ? m->httpequiv : m->name), "CONTENT-TYPE") == 0)
	{
	    int encoding = parse_content_type_header(m->content);
	    rid_header *rh = me->rh;

	    /* encoding could return 0 if it's unsupported - in which case ignore it */
	    if (encoding && rh->encoding_source == rid_encoding_source_USER)
	    {
		/* write value into the rid header */
		rh->encoding = encoding;
		rh->encoding_source = rid_encoding_source_META;

		mm_free(rh->language);
		rh->language = strdup(encoding_default_language(encoding));
		rh->language_num = lang_name_to_num(rh->language);

		DBG(("set_encoding META %d language %s (%d)\n", encoding, rh->language, rh->language_num));
		
		/* set stream to have encoding updated */
		sgml_set_encoding(context, encoding);
	    }
	    else
	    {
		DBG(("set_encoding META %d ignored\n", encoding));
	    }
	}
#endif
    }
}

/* <LINK> has the same attributes as <A>. There should be some */
/* difference between them, but I don't know what (italic?) */

extern void startlink (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    VALUE none;

    generic_start (context, element, attributes);

    none.type = value_none;
    new_aref_item(me,
		  &attributes->value[HTML_LINK_HREF],
		  &attributes->value[HTML_LINK_NAME],
		  &attributes->value[HTML_LINK_REL],
		  &none,
		  &attributes->value[HTML_LINK_TITLE]);
    /* starta (context, element, attributes); */

    /* SJM: stop the rest of the document getting linked... */
    me->aref = NULL;
}

#if 0
extern void finishbase (SGMLCTX * context, ELEMENT * element)
{    generic_finish (context, element);	}
#endif

