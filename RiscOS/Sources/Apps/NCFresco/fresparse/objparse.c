/* objparse.c */

/*

 * Routines for handling the OBJECT, APPLET and EMBED tags
 * and their support tags, PARAM, NOEMBED

 */

#include "debug.h"
#include "util.h"
#include "memwatch.h"
#include "htmlparser.h"
#include "webfonts.h"

#include "objects.h"

/* -------------------------------------------------------------------------- */

static int mimetype_to_filetype(STRING s)
{
    char *ss = stringdup(s);
    int ftype;

    ftype = mime_to_file_type(ss);

    mm_free(ss);

    return ftype;
}

static int extension_to_filetype(STRING s)
{
    char *ss = s.ptr + s.nchars;
    char *dot = NULL;
    char suffix[8];
    int len;
    
    while (ss > s.ptr)
    {
	if (ss[-1] == '.')
	{
	    dot = ss;
	    break;
	}
	else
	    ss--;
    }

    if (dot == NULL)
    {
	PRSDBG(("objparse: extension no dot in '%.*s'\n", s.nchars, s.ptr));
	return -1;
    }

    len = s.ptr + s.nchars - dot;
    if (len == 0)
    {
	PRSDBG(("objparse: null extension\n"));
	return -1;
    }

    if (len > sizeof(suffix)-1)
    {
	PRSDBG(("objparse: extension too long '%.*s'\n", len, dot));
	return -1;
    }

    memcpy(suffix, dot, len);
    suffix[len] = 0;

    PRSDBG(("objparse: extension '%s'\n", suffix));

    return suffix_to_file_type(suffix);
}

/* -------------------------------------------------------------------------- */

static rid_object_item *make_base_object (HTMLCTX *me, const VALUE *classid, const VALUE *classid_type, const VALUE *data, const VALUE *data_type, const VALUE *id, BOOL always_include)
{
    int classid_ftype, data_ftype, ftype;
    rid_object_item *obj;
    rid_object_type objtype;

    /* First thing to do is see if we can handle the object type 
     */

    if (classid_type->type == value_string)
    {
	classid_ftype = mimetype_to_filetype(classid_type->u.s);
    }
    else if (classid->type == value_string)
    {
	classid_ftype = extension_to_filetype(classid->u.s);
    }
    else
	classid_ftype = -1;

    if (data_type->type == value_string)
    {
	data_ftype = mimetype_to_filetype(data_type->u.s);
    }
    else if (data->type == value_string)
    {
	data_ftype = extension_to_filetype(data->u.s);
    }
    else
	data_ftype = -1;

    PRSDBG(("objparse: code %03x data %03x\n", classid_ftype, data_ftype));

    /* SJM: 4/3/96: remove check for whether we know about plugin or not */
    ftype = classid_ftype != -1 ? classid_ftype : data_ftype;
    if (!always_include && ftype == -1)		/* Unknonw mime type or extension */
	return NULL;
				/* Known type but no plugin present */
    objtype = objects_type_test(ftype);
    if (!always_include && objtype == rid_object_type_UNKNOWN)
	return NULL;

    /* Can handle it so allocate structure and fill in values */
    obj = mm_calloc(sizeof(*obj), 1);
    obj->type = objtype;

    obj->id = valuestringdup(id);

    obj->classid_ftype = classid_ftype;
    obj->classid = valuestringdup(classid);
    obj->classid_mime_type = valuestringdup(classid_type);

    obj->data_ftype = data_ftype;
    obj->data = valuestringdup(data);
    obj->data_mime_type = valuestringdup(data_type);

/*     obj->last_object = me->object; */
    me->object = obj;

    PRSDBG(("objparse: classid '%s' data '%s'\n", strsafe(obj->classid), strsafe(obj->data)));
    PRSDBG(("objparse: new 0x%p\n", obj));
/*     PRSDBG(("objparse: stack 0x%p new 0x%p\n", obj->last_object, obj)); */

    return obj;
}

static rid_text_item_object *connect_object(HTMLCTX *me, rid_object_item *obj)
{
    rid_text_item_object *tio;
    rid_text_item *nb;

    /* Allocate text_item and fill in details */
    /* FIXME: DECLARE objects probably don't want to go in here */
    tio = mm_calloc(sizeof(*tio), 1);
    tio->object = obj;

    nb = &tio->base;
    nb->tag = rid_tag_OBJECT;
    nb->aref = me->aref;	/* Current anchor, or NULL */
    if (me->aref && me->aref->first == NULL)
	me->aref->first = nb;
    GET_ROSTYLE(nb->st);
    nb->language = UNPACK(me->sgmlctx->tos->effects_active, LANG_NUM);

    /* insert parent ptr */
    obj->text_item = nb;
    
    /* connect to stream */
    rid_text_item_connect(me->rh->curstream, nb);

    return tio;
}

static rid_flag add_to_object(rid_object_item *obj, const VALUE *standby, const VALUE *name, 
			  const VALUE *width, const VALUE *height, const VALUE *align, 
			 const VALUE *hspace, const VALUE *vspace, const VALUE *codebase)
{
    rid_flag flag = 0;

    obj->standby = valuestringdup(standby);
#if 0				/* moved to redraw routine */
    /* if no standby message given then use the file type name */
    if (obj->standby == NULL)
    {
	char *s = get_plugin_type_name(obj->classid_ftype != -1 ? obj->classid_ftype : obj->data_ftype);
	if (s)
	    obj->standby = strdup(s);
    }
#endif

#if UNICODE
    if (obj->standby && webfont_need_wide_font(obj->standby, strlen(obj->standby)))
	flag |= rid_flag_WIDE_FONT;
#endif

    obj->name = valuestringdup(name);

    obj->userwidth = *width;
    obj->userheight = *height;
    obj->userhspace = *hspace;
    obj->uservspace = *vspace;

    obj->codebase = valuestringdup(codebase);

    decode_img_align(align->type == value_enum ? align->u.i : -1, &obj->iflags, &flag);

    return flag;
}

static void unstack_object(SGMLCTX *context)
{
    HTMLCTX *me = htmlctxof(context);
/*    rid_object_item *obj = me->object;*/

/*     PRSDBG(("objparse: unstack 0x%p from 0x%p\n", obj ? obj->last_object : NULL, obj)); */
/*     if (obj) */
/* 	me->object = obj->last_object; */

    me->object = NULL;
}

static void add_param(rid_object_item *obj, const STRING *name, const VALUE *value, const VALUE *type, const VALUE *valuetype)
{
    rid_object_param *param;

    /* NAME must be already checked, VALUE can legally be void or string */
    if (!obj || value->type == value_none)
	return;

    param = mm_calloc(sizeof(*param), 1);
    param->name = stringdup(*name);
    param->value = valuestringdup(value);
    param->type = valuestringdup(type);
    param->valuetype = valuetype->type == value_enum ? valuetype->u.i : HTML_PARAM_VALUETYPE_DATA;

    PRSDBG(("objparse: param '%s'='%s' type %s valuetype %d\n", param->name, strsafe(param->value), strsafe(param->type), param->valuetype));

    /* Add to param list for this object */
    param->next = obj->params;
    obj->params = param;
}

static void add_param1(rid_object_item *obj, const char *name, const char *value, int valuetype)
{
    rid_object_param *param;

    if (!obj)
	return;

    param = mm_calloc(sizeof(*param), 1);
    param->name = strdup(name);
    param->value = strdup(value);
    param->valuetype = valuetype;

    PRSDBG(("objparse: param '%s'='%s' type %s valuetype %d\n", param->name, strsafe(param->value), strsafe(param->type), param->valuetype));

    /* Add to param list for this object */
    param->next = obj->params;
    obj->params = param;
}

/* -------------------------------------------------------------------------- */


static void applet_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    switch (reason)
    {
    case DELIVER_WORD:
    case DELIVER_UNEXPECTED:
    case DELIVER_SGML:
	string_free(&item);
	/* deliberate fall through */

	/* This lot we are ignoring */
    case DELIVER_NOP :
    case DELIVER_SPACE:
    case DELIVER_EOL:
    case DELIVER_POST_CLOSE_MARKUP:
	PRSDBG(("applet_deliver(): ignoring reason %d\n", reason));
	break;

    case DELIVER_PRE_CLOSE_MARKUP:
	/* If the current applet then remove the deliver handler and call finishapplet() */
	if (element->id == HTML_APPLET &&
	    --htmlctx->object_nesting == 0)
	{
	    PRSDBG(("applet_deliver(): passing reason %d onwards\n", reason));
	    sgml_remove_deliver(context, applet_deliver);
	    (*context->deliver) (context, reason, item, element);
	    /* Force finishapplet being called */
	    context->force_deliver = TRUE;
	}
	else
	{
	    PRSDBG(("applet_deliver(): ignoring reason %d\n", reason));
	}
	break;
	
	/* We let <PARAM> through as if normal, and count APPLET nesting level */
    case DELIVER_PRE_OPEN_MARKUP:
	switch (element->id)
	{
	case HTML_PARAM:
	    if (htmlctx->object_nesting == 1)
	    {
		PRSDBG(("applet_deliver(): passing reason %d onwards (PARAM)\n", reason));
		(*context->dlist->this_fn) (context, reason, item, element);
		/* Force the startparam() call to happen */
		context->force_deliver = TRUE;
	    }
	    else
	    {
		PRSDBG(("applet_deliver(): ignoring reason %d (nested PARAM)\n", reason));
	    }
	    break;

	case HTML_APPLET:
	    htmlctx->object_nesting++;
	    PRSDBG(("applet_deliver(): nested APPLET, count=%d\n", htmlctx->object_nesting));
	    break;
	
	default:
	    PRSDBG(("applet_deliver(): ignoring reason %d\n", reason));
	    break;
	}
	break;

    case DELIVER_POST_OPEN_MARKUP:
	if (element->id == HTML_PARAM && htmlctx->object_nesting == 1)
	{
	    PRSDBG(("applet_deliver(): passing reason %d onwards (PARAM)\n", reason));
	    (*context->dlist->this_fn) (context, reason, item, element);
	}
	else
	{
	    PRSDBG(("applet_deliver(): ignoring reason %d\n", reason));
	}
	break;

	/* This cannot be ignored and must be correctly delivered. */
    case DELIVER_EOS:
	PRSDBG(("applet_deliver(): EOS forcing restoration of original delivery handler\n"));
	sgml_unwind_deliver(context);
	PRSDBG(("applet_deliver(): doing the real EOS delivery now\n"));
	(*context->deliver) (context, reason, item, element);
	break;
    }
}

#define MIME_TYPE_JAVA	"application/java-vm"

extern void startapplet (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_object_item *obj;
    VALUE java_type, none;

    generic_start (context, element, attributes);

    java_type.type = value_string;
    java_type.u.s.nchars = sizeof(MIME_TYPE_JAVA)-1;
    java_type.u.s.ptr = MIME_TYPE_JAVA;

    none.type = value_none;

    obj = make_base_object(me, 
		&attributes->value[HTML_APPLET_CODE],
		&java_type,
		&none,
		&none,
		&attributes->value[HTML_APPLET_ID], FALSE);

    if (obj)
    {
	rid_text_item_object *tio;
	VALUE param;

	obj->element = HTML_APPLET;

	tio = connect_object(me, obj);

	tio->base.flag |= add_to_object(obj, 
		&attributes->value[HTML_APPLET_ALT],
		&attributes->value[HTML_APPLET_NAME],
		&attributes->value[HTML_APPLET_WIDTH],
		&attributes->value[HTML_APPLET_HEIGHT],
		&attributes->value[HTML_APPLET_ALIGN],
		&attributes->value[HTML_APPLET_HSPACE],
		&attributes->value[HTML_APPLET_VSPACE], 
		&attributes->value[HTML_APPLET_CODEBASE]);

	param.type = value_enum;
	param.u.i = rid_object_param_OBJECT_URL;
	add_param(obj, &(element->attributes[HTML_APPLET_ARCHIVE])->name, &attributes->value[HTML_APPLET_ARCHIVE], &none, &param);

	/* Patch in applet delivery handler */

	me->object_nesting = 1;
	sgml_install_deliver(context, &applet_deliver);
    }
}

extern void finishapplet (SGMLCTX * context, ELEMENT * element)
{
    generic_finish(context, element); 

    unstack_object(context);
}

/* -------------------------------------------------------------------------- */


static void object_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    switch (reason)
    {
    case DELIVER_WORD:
    case DELIVER_UNEXPECTED:
    case DELIVER_SGML:
	string_free(&item);
	/* deliberate fall through */

	/* This lot we are ignoring */
    case DELIVER_NOP :
    case DELIVER_SPACE:
    case DELIVER_EOL:
    case DELIVER_POST_CLOSE_MARKUP:
	PRSDBG(("object_deliver(): ignoring reason %d\n", reason));
	break;

    case DELIVER_PRE_CLOSE_MARKUP:
	if (element->id == HTML_OBJECT &&
	    --htmlctx->object_nesting == 0)
	{
	    PRSDBG(("object_deliver(): passing reason %d onwards\n", reason));
	    sgml_remove_deliver(context, &object_deliver);
	    (*context->deliver) (context, reason, item, element);
	    /* Force finishobject being called */
	    context->force_deliver = TRUE;
	}
	else
	{
	    PRSDBG(("object_deliver(): ignoring reason %d\n", reason));
	}
	break;
	
    case DELIVER_PRE_OPEN_MARKUP:
	switch (element->id)
	{
	case HTML_PARAM:
	    if (htmlctx->object_nesting == 1)
	    {
		PRSDBG(("object_deliver(): passing reason %d onwards (PARAM)\n", reason));
		(*context->dlist->this_fn) (context, reason, item, element);
		/* Force the startparam() call to happen */
		context->force_deliver = TRUE;
	    }
	    else
	    {
		PRSDBG(("object_deliver(): ignoring reason %d (nested PARAM)\n", reason));
	    }
	    break;

	case HTML_A:
	    if ( htmlctx->object_nesting == 1 && (htmlctx->object->oflags & rid_object_flag_SHAPES) != 0 )
	    {
		PRSDBG(("object_deliver(): passing reason %d onwards (A)\n", reason));
		(*context->dlist->this_fn) (context, reason, item, element);
		/* Force the starta() call to happen */
		context->force_deliver = TRUE;
	    }
	    else
	    {
		PRSDBG(("object_deliver(): not <OBJECT SHAPES=..>/nested so ignoring <A>\n"));
	    }
	    break;

	case HTML_OBJECT:
	    htmlctx->object_nesting++;
	    PRSDBG(("object_deliver(): nested <OBJECT>, nesting %d\n", htmlctx->object_nesting));
	    break;

	default:
	    PRSDBG(("object_deliver(): ignoring reason %d\n", reason));
	    break;
	}
	break;

    case DELIVER_POST_OPEN_MARKUP:
	switch (element->id )
	{
/* 	case HTML_OBJECT: Don't want to know about these here */
	case HTML_PARAM:
	    if (htmlctx->object_nesting == 1)
	    {
		PRSDBG(("object_deliver(): passing reason %d onwards <%s>\n", reason, element->name.ptr));
		(*context->dlist->this_fn) (context, reason, item, element);
	    }
	    else
	    {
		PRSDBG(("object_deliver(): ignoring reason %d (nested PARAM)\n", reason));
	    }
	    break;
	    
	case HTML_A:
	    if ( (htmlctx->object->oflags & rid_object_flag_SHAPES) != 0 )
	    {
		PRSDBG(("object_deliver(): passing reason %d onwards <%s>\n", reason, element->name.ptr));
		(*context->dlist->this_fn) (context, reason, item, element);
	    }
	    else
	    {
		PRSDBG(("object_deliver(): ignoring reason %d\n", reason));
	    }
	    break;
	    
	default:
	    PRSDBG(("object_deliver(): ignoring reason %d\n", reason));
	}
	break;

	/* This cannot be ignored and must be correctly delivered. */
    case DELIVER_EOS:
	PRSDBG(("object_deliver(): EOS forcing restoration of original delivery handler\n"));
	sgml_unwind_deliver(context);
	PRSDBG(("object_deliver(): doing the real EOS delivery now\n"));
	(*context->deliver) (context, reason, item, element);
	break;
    }
}

extern void startobject(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{ 
    HTMLCTX *me = htmlctxof(context);
    rid_object_item *obj;

    generic_start(context, element, attributes); 

    obj = make_base_object(me, 
		&attributes->value[HTML_OBJECT_CLASSID],
		&attributes->value[HTML_OBJECT_CODETYPE],
		&attributes->value[HTML_OBJECT_DATA],
		&attributes->value[HTML_OBJECT_TYPE],
		&attributes->value[HTML_OBJECT_ID], FALSE);

    if (obj)
    {
	rid_text_item_object *tio;

	obj->element = HTML_OBJECT;

	tio = connect_object(me, obj);

	tio->base.flag |= add_to_object(obj,
		&attributes->value[HTML_OBJECT_STANDBY],
		&attributes->value[HTML_OBJECT_NAME],
		&attributes->value[HTML_OBJECT_WIDTH],
		&attributes->value[HTML_OBJECT_HEIGHT],
		&attributes->value[HTML_OBJECT_ALIGN],
		&attributes->value[HTML_OBJECT_HSPACE],
		&attributes->value[HTML_OBJECT_VSPACE], 
		&attributes->value[HTML_OBJECT_CODEBASE]);

	/* Add OBJECT specific parameters */
	if (attributes->value[HTML_OBJECT_DECLARE].type == value_enum)
	    obj->oflags |= rid_object_flag_DECLARE;

	if (attributes->value[HTML_OBJECT_SHAPES].type == value_enum)
	{
	    obj->oflags |= rid_object_flag_SHAPES;
	    obj->map = mm_calloc(sizeof(*obj->map), 1);
	}
	
	obj->usemap = valuestringdup(&attributes->value[HTML_OBJECT_USEMAP]);
	obj->userborder = attributes->value[HTML_OBJECT_BORDER];

	sgml_install_deliver(context, &object_deliver);
	me->object_nesting = 1;
    }
}

extern void finishobject(SGMLCTX *context, ELEMENT *element)
{ 
    generic_finish(context, element); 
    unstack_object(context);
}

/* -------------------------------------------------------------------------- */

extern void startembed(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_object_item *obj;
    VALUE none;

    generic_start(context, element, attributes); 

    none.type = value_none;
    obj = make_base_object(me, 
		&none,
		&none,
		&attributes->value[HTML_EMBED_SRC],
		&none,
		&attributes->value[HTML_EMBED_ID], TRUE);

    if (obj)
    {
	rid_text_item_object *tio;
	VALUE param;
	
	/* EMBED is an empty element so no stacking */
	me->object = NULL;
	
	obj->element = HTML_EMBED;

	tio = connect_object(me, obj);

	tio->base.flag |= add_to_object(obj,
		&none,
		&none,
		&attributes->value[HTML_EMBED_WIDTH],
		&attributes->value[HTML_EMBED_HEIGHT],
		&none,
		&none,
		&none, 
		&none);

#if 0
	fprintf(stderr, "element %p\n->attributes %p\nstring %p %d '%.*s'",
		element, element->attributes, 
		(element->attributes[HTML_EMBED_PALETTE])->name.ptr,
		(element->attributes[HTML_EMBED_PALETTE])->name.nchars,
		(element->attributes[HTML_EMBED_PALETTE])->name.nchars,
		(element->attributes[HTML_EMBED_PALETTE])->name.ptr);
	
#endif
	param.type = value_enum;
	param.u.i = rid_object_param_OBJECT_DATA;

	/* shockwave */
	add_param(obj, &(element->attributes[HTML_EMBED_PALETTE])->name, &attributes->value[HTML_EMBED_PALETTE], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_TEXTFOCUS])->name, &attributes->value[HTML_EMBED_TEXTFOCUS], &none, &param);

	/* oracle video server */
	add_param(obj, &(element->attributes[HTML_EMBED_AUTOSTART])->name, &attributes->value[HTML_EMBED_AUTOSTART], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_AUTOPLAY])->name, &attributes->value[HTML_EMBED_AUTOPLAY], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_LOOP])->name, &attributes->value[HTML_EMBED_LOOP], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_MDSFILE])->name, &attributes->value[HTML_EMBED_MDSFILE], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_MEDIAFILE])->name, &attributes->value[HTML_EMBED_MEDIAFILE], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_PLAYFROM])->name, &attributes->value[HTML_EMBED_PLAYFROM], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_PLAYTO])->name, &attributes->value[HTML_EMBED_PLAYTO], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_SERVER])->name, &attributes->value[HTML_EMBED_SERVER], &none, &param);
	add_param(obj, &(element->attributes[HTML_EMBED_TYPE])->name, &attributes->value[HTML_EMBED_TYPE], &none, &param);

	add_param(obj, &(element->attributes[HTML_EMBED_HIDDEN])->name, &attributes->value[HTML_EMBED_HIDDEN], &none, &param);

	/* if hidden then force to 0 size */
	if (attributes->value[HTML_EMBED_HIDDEN].type != value_none)
	{
	    obj->userwidth.type = value_integer;
	    obj->userwidth.u.i = 0;
	    obj->userheight = obj->userwidth;
	}
	
	me->discard_noembed = 1;
    }
    else
	me->discard_noembed = 0;
}

#if 0
extern void finishembed(SGMLCTX *context, ELEMENT *element)
{ 
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_finish(context, element); 
    unstack_object(context);
    htmlctx->discard_noembed = 0;
}
#endif

/* -------------------------------------------------------------------------- */

extern void startbgsound(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_object_item *obj;
    VALUE none;

    generic_start(context, element, attributes); 

    none.type = value_none;
    obj = make_base_object(me, 
		&none,
		&none,
		&attributes->value[HTML_BGSOUND_SRC],
		&none,
		&none, FALSE);

    if (obj)
    {
	rid_text_item_object *tio;
	VALUE param;
	
	/* explicitly zero size */
	obj->userwidth.type = value_integer;
	obj->userwidth.u.i = 0;
	obj->userheight.type = value_integer;
	obj->userheight.u.i = 0;
    
	/* BGSOUND is an empty element so no stacking */
	me->object = NULL;
	
	obj->element = HTML_BGSOUND;

	tio = connect_object(me, obj);

	/* These are implied in BGSOUND */
	add_param1(obj, "AUTOSTART", "TRUE", rid_object_param_OBJECT_DATA);
	add_param1(obj, "HIDDEN", "TRUE", rid_object_param_OBJECT_DATA);

	param.type = value_enum;
	param.u.i = rid_object_param_OBJECT_DATA;
	add_param(obj, &(element->attributes[HTML_BGSOUND_LOOP])->name, &attributes->value[HTML_BGSOUND_LOOP], &none, &param);
    }

    me->discard_noembed = 0;
}

/* -------------------------------------------------------------------------- */


static void noembed_deliver (SGMLCTX *context, int reason, STRING item, ELEMENT *element)
{
    HTMLCTX *htmlctx = htmlctxof(context);

    switch (reason)
    {
    case DELIVER_WORD:
    case DELIVER_UNEXPECTED:
    case DELIVER_SGML:
	string_free(&item);
	/* deliberate fall through */

	/* This lot we are ignoring */
    case DELIVER_NOP :
    case DELIVER_SPACE:
    case DELIVER_POST_OPEN_MARKUP:
    case DELIVER_POST_CLOSE_MARKUP:
    case DELIVER_EOL:
	PRSDBG(("noembed_deliver(): ignoring <%s>\n", element->name.ptr));
	break;

    case DELIVER_PRE_CLOSE_MARKUP:
	if (element->id == HTML_NOEMBED &&
	    --htmlctx->object_nesting == 0)
	{
	    PRSDBG(("noembed_deliver(): passing reason %d onwards\n", reason));
	    sgml_remove_deliver(context, noembed_deliver);
	    (*context->deliver) (context, reason, item, element);
	    context->force_deliver = TRUE;
	}
	else
	{
	    PRSDBG(("noembed_deliver(): ignoring reason %d\n", reason));
	}
	break;

    case DELIVER_PRE_OPEN_MARKUP:
	if (element->id == HTML_NOEMBED)
	{
	    htmlctx->object_nesting++;
	    PRSDBG(("noembed_deliver(): nested APPLET, count=%d\n", htmlctx->object_nesting));
	}
	else
	{
	    PRSDBG(("noembed_deliver(): ignoring reason %d\n", reason));
	}
	break;

	/* This cannot be ignored and must be correctly delivered. */
    case DELIVER_EOS:
	PRSDBG(("noembed_deliver(): EOS forcing restoration of original delivery handler\n"));
	sgml_unwind_deliver(context);
	PRSDBG(("noembed_deliver(): doing the real EOS delivery now\n"));
	(*context->deliver) (context, reason, item, element);
	break;
    }
}


extern void startnoembed(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{ 
    HTMLCTX *htmlctx = htmlctxof(context);

    generic_start(context, element, attributes); 

    if (htmlctx->discard_noembed)
    {
	/* Clear as soon as it does anything */
	htmlctx->discard_noembed = 0;
#if 0
	htmlctx->unstack_noembed = 1;
#endif

	htmlctx->object_nesting = 1;
	sgml_install_deliver(context, &noembed_deliver);
    }
}

extern void finishnoembed(SGMLCTX *context, ELEMENT *element)
{ 
    generic_finish(context, element); 
}

/* -------------------------------------------------------------------------- */

/*
 * PARAM's can exist inside APPLET's or OBJECT's
 * Someone else will havce to cope with stripping out the PARAMs from unwanted OBJECTS.
 * This stripping is handled by special delivery routines installed by the 
 * startfunction of <APPLET>, etc, and removed by the finishfunction.
 */

extern void startparam(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{ 
    HTMLCTX *me = htmlctxof(context);
    rid_object_item *obj = me->object;

    generic_start(context, element, attributes); 

    PRSDBG(("objparse: param to object 0x%p\n", obj));

    if (attributes->value[HTML_PARAM_NAME].type == value_string)
	add_param(obj,
		  &attributes->value[HTML_PARAM_NAME].u.s,
		  &attributes->value[HTML_PARAM_VALUE],
		  &attributes->value[HTML_PARAM_TYPE],
		  &attributes->value[HTML_PARAM_VALUETYPE]);
}

extern void startbadparam (SGMLCTX * context, ELEMENT * element, VALUES * attributes)
{
    HTMLCTX *me = htmlctxof(context);
    rid_object_item *obj = me->object;

    generic_start (context, element, attributes);

    if (obj && attributes->value[HTML_BADPARAM_NAME].type == value_string)
	add_param(obj,
		  &attributes->value[HTML_BADPARAM_NAME].u.s,
		  &attributes->value[HTML_BADPARAM_VALUE],
		  &attributes->value[HTML_BADPARAM_TYPE],
		  &attributes->value[HTML_BADPARAM_VALUETYPE]);
}

/* -------------------------------------------------------------------------- */

extern void startbodytext(SGMLCTX *context, ELEMENT *element, VALUES *attributes)
{
    generic_start(context, element, attributes); 
}

/* -------------------------------------------------------------------------- */

/* eof objparse.c */
