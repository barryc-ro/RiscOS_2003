/* plugin.c */

#include <stdio.h>
#include <string.h>

#include "swis.h"

#include "rid.h"
#include "antweb.h"
#include "util.h"
#include "files.h"
#include "interface.h"
#include "status.h"
#include "url.h"
#include "objects.h"

#include "plugin.h"
#include "pluginfn.h"
#include "verstring.h"
#include "makeerror.h"

#ifndef LOCK_FILES
#define LOCK_FILES 0
#endif

/* ----------------------------------------------------------------------------- */

typedef enum
{
    plugin_state_EMPTY,		/* nothing happened yet */
    plugin_state_SENT_OPEN,	/* The PLUGIN_OPEN message has been sent */
    plugin_state_SENT_OPEN_2,	/* The task has been run, PLUGIN_OPEN message has been resent */
    plugin_state_OPEN,		/* The PLUGIN_OPENING message has been received */
    plugin_state_SENT_CLOSE,
    plugin_state_HAD_CLOSED,	/* The PLUGIN_CLOSED message has been received */
    plugin_state_ABORTED	/* PLUGIN never got going */
} plugin_state;

typedef enum
{
    plugin_stream_state_STARTED,
    plugin_stream_state_SENT_NEW_STREAM,
    plugin_stream_state_READY_TO_WRITE,
    plugin_stream_state_WRITING,
    plugin_stream_state_ABORTED
} plugin_stream_state;

typedef struct plugin_stream_private plugin_stream_private;
typedef	struct plugin_private plugin_private;
typedef struct plugin_string_rma_ptr plugin_string_rma_ptr;

/* ----------------------------------------------------------------------------- */

struct plugin_stream_private
{
    plugin_stream_private *next;	/* next stream in chain */
    plugin_private *pp;			/* the parent plugin's private data */

    access_handle ah;			/* for the access going on */
    char *url;				/* the url being requested */
    char *target;			/* frame target name for data */

    int notify_data;			/* handle to pass back in notify messag */
    BOOL notify;

    char *mime_type;

    plugin_stream_state stream_state;
    int stream_type;			/* as given in the flags */

    void *plugin_instance;		/* private data for the plugin */

    int offset;				/* amount of data sent to/received from the plugin */
    int size;				/* total amount of data, or 0 if not known */
    time_t last_modified;		/* from the last-modified header */

    char *file_name;			/* used when finishing off */
    int file_handle;
    int file_type;
    transfer_status status;

    int msgref;				/* wimp message ref of last stream message sent */
};

#define plugin_priv_HELPER	0x01	/* launch it as a helper */

struct plugin_private
{
    plugin next;			/* linked list, only used when helpers */

    antweb_doc *doc;			/* parent document */
    be_item parent_item;

    plugin_state state;
    plugin_box box;			/* last bounding box sent to plugin */
    int pending_reshape;		/* */
    int msgref;				/* our ref from last message sent */
    char *parameter_file;		/* pointer to parameter file */
    void *instance;			/* plugin's private word pointer */
    wimp_t task;			/* plugin's wimp task handle */

    plugin_stream_private *streams;	/* list of currently active streams */

    int play_state;			/* state returned by last BUSY message */
    int opening_flags;			/* flags from opening, busy flag gets updated */

    int priv_flags;

    struct
    {
	int data_ftype;
	char *data;
	int classid_ftype;
	char *classid;
	char *codebase;
    } objd;

    struct
    {
	void *parent;
	char *cfile;
    } helper;
};

/* ----------------------------------------------------------------------------- */

struct plugin_string_rma_ptr
{
    plugin_string_rma_ptr *prev, *next;
    plugin_private *pp;
    int msgref;
};

/* ----------------------------------------------------------------------------- */

static void plugin_stream_dispose(plugin_stream_private *psp);

/* ----------------------------------------------------------------------------- */

static plugin_string_rma_ptr *rma_ptr_list = NULL;
static plugin plugin_list = NULL;	/* all plugins and helpers */
static int helper_count = 0;		/* count of helpers open */

/* ----------------------------------------------------------------------------- */

static char *get_ptr(void *msgdata, plugin_string_value sv)
{
    return sv.offset == 0 ? 0 : sv.offset <= 236 ? (char *)msgdata + sv.offset : sv.ptr;
}

/*
 * Copy the string 's' into a block allocated from the RMA and link it into the
 * list of blocks so allocated.
 */

static void *rma_dup(const char *s)
{
    plugin_string_rma_ptr *ptr;
    int len = strlen(s) + 1;

    ptr = rma_alloc(len + sizeof(plugin_string_rma_ptr));

    ptr->prev = NULL;
    ptr->next = rma_ptr_list;
    ptr->msgref = -1;

    if (rma_ptr_list)
	rma_ptr_list->prev = ptr;

    rma_ptr_list = ptr;

    memcpy(ptr+1, s, len);

    OBJDBG(("plugin: new string  %p '%s'\n", ptr, (char *)(ptr+1)));

    return ptr+1;
}

static void free_rma_strings(int msgref, plugin_private *pp)
{
    plugin_string_rma_ptr *psrp;
    for (psrp = rma_ptr_list; psrp; psrp = psrp->next)
    {
	/* if either the msgref oor pointer match then
	 * remove from the list and free
	 */
	if (psrp->msgref == msgref || psrp->pp == pp)
	{
	    OBJDBG(("plugin: free string %p '%s' ref %x\n", psrp, (char *)(psrp+1), psrp->msgref));

	    if (psrp->prev)
		psrp->prev->next = psrp->next;
	    if (psrp->next)
		psrp->next->prev = psrp->prev;

	    rma_free(psrp);
	}
    }
}

/* ----------------------------------------------------------------------------- */

#define fsnum_resourcefs	0x2E
#define fsnum_cachefs		118

static int get_fsnum(int fh)
{
    int fs;
    _swix(OS_Args, _INR(0,1)|_OUT(2), 254, fh, &fs);
    return fs & 0xff;
}

static void *get_fsptr(int fh)
{
    void *ptr;
    _swix(OS_FSControl, _IN(0)|_IN(1)|_OUT(1), 21, fh, &ptr);
    return ptr;
}

/* ----------------------------------------------------------------------------- */

static void link(plugin *root, plugin pp)
{
    pp->next = *root;
    *root = pp;
}

static void unlink(plugin *root, plugin pp)
{
    /* unlink from list */
    if (pp == *root)
	*root = pp->next;
    else
    {
	plugin prev = *root;
	while (prev)
	{
	    if (prev->next == pp)
	    {
		prev->next = pp->next;
		break;
	    }

	    prev = prev->next;
	}
    }
}

/* ----------------------------------------------------------------------------- */

/*
 * write padding null characters to pad len to a word boundary
 */

static void write_padding(int len, FILE *f)
{
    len &= 3;
    if (len)
	for (len = 4 - len; len; len--)
	    fputc(0, f);
}

static void write_string(wimp_msgstr *mp, plugin_string_value *so, const char *data)
{
    int space_left = sizeof(*mp) - mp->hdr.size;
    int data_len = strlen(data) + 1;

    if (data_len <= space_left)
    {
	so->offset = mp->hdr.size - sizeof(mp->hdr); /* offset is relative to data start */
	strcpy(mp->data.chars + so->offset, data);
	mp->hdr.size += data_len;
    }
    else
    {
	/* copy to rma */
	so->ptr = rma_dup(data);
    }
}

static void plugin_send_message(wimp_msgstr *msg, plugin_private *pp)
{
    message_plugin_base *base = (message_plugin_base *)&msg->data;
    plugin_string_rma_ptr *psrp;

    base->instance.plugin = pp->instance;
    base->instance.parent = pp;

    msg->hdr.size = ROUND4(msg->hdr.size);

    frontend_message_send(msg, pp->task);
    pp->msgref = msg->hdr.my_ref;

    /* rma allocated pointers are linked into list, we need to write in the msg reference */
    for (psrp = rma_ptr_list; psrp && psrp->msgref == -1; psrp = psrp->next)
    {
	psrp->msgref = msg->hdr.my_ref;
	psrp->pp = pp;
    }
}


static plugin_parameter_type convert_valuetype(int valuetype)
{
    switch (valuetype)
    {
    case rid_object_param_OBJECT_DATA:
	return plugin_parameter_DATA;
    case rid_object_param_OBJECT_URL:
	return plugin_parameter_URL;
    case HTML_PARAM_VALUETYPE_DATA:
	return plugin_parameter_PARAM_DATA;
    case HTML_PARAM_VALUETYPE_REF:
	return plugin_parameter_PARAM_URL;
    case HTML_PARAM_VALUETYPE_OBJECT:
	return plugin_parameter_PARAM_OBJECT;
    }
    return plugin_parameter_PARAM_DATA;
}

/* ----------------------------------------------------------------------------- */

static void plugin_write_parameter_record(FILE *f, plugin_parameter_type paramtype, const char *name, const char *value, const char *type)
{
    int nlen, vlen, tlen;
    int val[2];

    if (value == NULL)
	return;
    if (type == NULL)
	type = "";

    nlen = strlen(name);
    vlen = strlen(value);
    tlen = strlen(type);

    val[0] = paramtype;
    val[1] = (4 + ROUND4(nlen)) + (4 + ROUND4(vlen)) + (4 + ROUND4(tlen));
    fwrite(&val, sizeof(val), 1, f);

    fwrite(&nlen, sizeof(nlen), 1, f);
    fputs(name, f);
    write_padding(nlen, f);

    fwrite(&vlen, sizeof(vlen), 1, f);
    fputs(value, f);
    write_padding(vlen, f);

    fwrite(&tlen, sizeof(tlen), 1, f);
    fputs(type, f);
    write_padding(tlen, f);
}

static void plugin_write_parameter_numeric(FILE *f, const char *name, const rid_stdunits *val)
{
    char buf[32];

    if (val == NULL)
	return;

    switch (val->type)
    {
    case value_integer:
	sprintf(buf, "%d", val->u.i);
	break;

    case value_absunit:
	sprintf(buf, "%g", val->u.f/2);
	break;

    case value_pcunit:
	sprintf(buf, "%g%%", val->u.f);
	break;

    default:
	return;
    }

    plugin_write_parameter_record(f, plugin_parameter_DATA, name, buf, NULL);
}

static void plugin_write_parameter_string(FILE *f, const char *name, const char *value)
{
    plugin_write_parameter_record(f, plugin_parameter_DATA, name, value, NULL);
}

static void plugin_write_parameter_url(FILE *f, const char *name, const char *value, const char *type)
{
    plugin_write_parameter_record(f, plugin_parameter_URL, name, value, type);
}

/* ----------------------------------------------------------------------------- */

/*
 * Write out the parameter file for a plugin.
 * FIXME: this doesn't write out the alignment as we don't have the original string available.
 */

static void plugin_write_parameter_file(FILE *f, rid_object_item *obj, const char *base_href)
{
    rid_object_param *param;

    plugin_write_parameter_record(f, plugin_parameter_SPECIAL, plugin_parameter_BASE_HREF, base_href, NULL);
    plugin_write_parameter_record(f, plugin_parameter_SPECIAL, plugin_parameter_USER_AGENT, program_name, NULL);
    plugin_write_parameter_record(f, plugin_parameter_SPECIAL, plugin_parameter_UA_VERSION, VERSION_NUMBER, NULL);
    plugin_write_parameter_record(f, plugin_parameter_SPECIAL, plugin_parameter_API_VERSION, plugin_API_VERSION, NULL);

    plugin_write_parameter_string(f, "ID", obj->id);
/*
    plugin_write_parameter_string(f, "CLASS", obj->classname);
    plugin_write_parameter_string(f, "STYLE", obj->style);
    plugin_write_parameter_string(f, "LANG", obj->lang);
    plugin_write_parameter_string(f, "DIR", obj->dir);
    */
    plugin_write_parameter_string(f, "DECLARE", obj->oflags & rid_object_flag_DECLARE ? "" : NULL);

    plugin_write_parameter_url(f,
	obj->element == HTML_APPLET ? "CODE" : "CLASSID",
        obj->classid, obj->classid_mime_type);

    plugin_write_parameter_url(f, "CODEBASE", obj->codebase, NULL);

    plugin_write_parameter_url(f,
	obj->element == HTML_EMBED ? "SRC" : "DATA",
	obj->data, obj->data_mime_type);

    plugin_write_parameter_string(f,
        obj->element == HTML_OBJECT ? "STANDBY" : "ALT",
        obj->standby);

/*     plugin_write_parameter_string(f, "ALIGN", align_name(obj->align)); */
    plugin_write_parameter_numeric(f, "HEIGHT", &obj->userheight);
    plugin_write_parameter_numeric(f, "WIDTH", &obj->userwidth);
    plugin_write_parameter_numeric(f, "BORDER", &obj->userborder);
    plugin_write_parameter_numeric(f, "HSPACE", &obj->userhspace);
    plugin_write_parameter_numeric(f, "VSPACE", &obj->uservspace);
    plugin_write_parameter_url(f, "USEMAP", obj->usemap, NULL);
    plugin_write_parameter_string(f, "SHAPES", obj->oflags & rid_object_flag_SHAPES ? "" : NULL);
    plugin_write_parameter_url(f, "NAME", obj->name, NULL);

    for (param = obj->params; param; param = param->next)
	plugin_write_parameter_record(f, convert_valuetype(param->valuetype), param->name, param->value, param->type);

    /* write terminator */
    {
	int c = plugin_parameter_TERMINATOR;
	fwrite(&c, sizeof(c), 1, f);
    }
}

static void remove_parameter_file(plugin_private *pp)
{
    if (pp->parameter_file)
    {
	OBJDBG(("plugin: remove parameter file '%s'\n", pp->parameter_file));

#if LOCK_FILES
	file_lock(pp->parameter_file, FALSE);
#endif
	remove(pp->parameter_file);
	mm_free(pp->parameter_file);
	pp->parameter_file = NULL;
    }
}

/* ----------------------------------------------------------------------------- */

static void fillin_stream(wimp_msgstr *msg, plugin_stream_private *psp, plugin_stream *stream)
{
    stream->instance.parent = psp;
    stream->instance.plugin = psp->plugin_instance;
    write_string(msg, &stream->url, psp->url);
    stream->end = psp->size;
    stream->last_modified = psp->last_modified;
    stream->notify_data = psp->notify_data;
}

static void close_stream_file(plugin_stream_private *psp)
{
    ro_fclose(psp->file_handle);
    psp->file_handle = 0;
}

/* ----------------------------------------------------------------------------- */

/*
 * Currently locate object takes the browser instance handle,
 * checks that it is in this document and then returns it or NULL.
 * This way if we want to change the handle to something other
 * than a direct pointer we can.
 */

static plugin_private *locate_object(antweb_doc *doc, plugin_private *pp)
{
    if (doc)
    {
	rid_text_item *ti;

	for (ti = rid_scan(doc->rh->stream.text_list, SCAN_THIS | SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_OBJECT);
	     ti;
	     ti = rid_scan(ti, SCAN_FWD | SCAN_RECURSE | SCAN_FILTER | rid_tag_OBJECT))
	{
	    rid_text_item_object *tio = (rid_text_item_object *)ti;

	    if (tio->object && tio->object->state.plugin.pp == pp)
		return pp;
	}

	OBJDBG(("plugin: can't locate pp %p in doc %p\n", pp, doc));
    }
    else
    {
	plugin ppp;
	for (ppp = plugin_list; ppp; ppp = ppp->next)
	    if ((ppp->priv_flags & plugin_priv_HELPER) && ppp == pp)
		return pp;

	OBJDBG(("plugin: can't locate pp %p in helper list\n", pp));
    }

    return NULL;
}

static plugin_stream_private *locate_stream(plugin_private *pp, plugin_stream_private *psp1)
{
    plugin_stream_private *psp;
    for (psp = pp->streams; psp; psp = psp->next)
	if (psp == psp1)
	    return psp;

    OBJDBG(("plugin: can't locate psp %p in pp %p\n", psp1, pp));

    return NULL;
}

/* return as helper if we launched it as a helper or it was 'converted' into a helper */

static plugin first_helper(void)
{
    plugin pp;
    for (pp = plugin_list; pp; pp = pp->next)
	if ((pp->priv_flags & plugin_priv_HELPER)/*  || (pp->opening_flags & plugin_opening_HELPER) */)
	    return pp;
    return NULL;
}

static int helpers_open(void)
{
    plugin pp;
    int count = 0;
    for (pp = plugin_list; pp; pp = pp->next)
    {
	if (((pp->priv_flags & plugin_priv_HELPER)/* || (pp->opening_flags & plugin_opening_HELPER) */) &&
	    pp->state == plugin_state_OPEN)
	{
	    count++;
	}
    }
    return count;
}

/* ----------------------------------------------------------------------------- */

/*
 * To open a plugin.
 * 1) write the parameter file
 * 2) Send message (via frontend)
 */

int plugin_send_open(plugin pp, const wimp_box *box, int open_flags)
{
    wimp_msgstr msg;
    message_plugin_open *open = (message_plugin_open *) &msg.data;

#if DEBUG
    OBJDBG(("plugin: send open %p ftype %x/%x", pp, pp->objd.classid_ftype, pp->objd.data_ftype));
    if (box)
	OBJDBG(("at %d,%d,%d,%d\n", box->x0, box->y0, box->x1, box->y1));
    else
	OBJDBG(("\n"));
#endif

    memset(open, 0, sizeof(*open));

    /* Write message header */
    msg.hdr.size = sizeof(msg.hdr) + sizeof(*open);
    msg.hdr.action = (wimp_msgaction) MESSAGE_PLUGIN_OPEN;
    msg.hdr.your_ref = 0;

    /* Build message block */
    open->flags = open_flags;
    if (box)
	open->box = *(plugin_box *)box;

    open->file_type = pp->objd.classid_ftype != -1 ? pp->objd.classid_ftype : pp->objd.data_ftype;

    if (pp->doc)
	open->window_handle = (plugin_w)frontend_get_window_handle(pp->doc->parent);

    write_string(&msg, &open->file_name, pp->parameter_file);

    plugin_send_message(&msg, pp);

    pp->state = plugin_state_SENT_OPEN;

    /* store the box aaway */
    if (box)
	pp->box = *(plugin_box *)box;

    return 1;
}

int plugin_send_reshape(plugin pp, const wimp_box *box)
{
    wimp_msgstr msg;
    message_plugin_reshape *reshape;

    /* store the box aaway */
    pp->box = *(plugin_box *)box;

    if (pp->state != plugin_state_OPEN)
    {
	OBJDBG(("plugin: pp %p - no reshape as opening not received as yet\n", pp));
	pp->pending_reshape = TRUE;
	return 0;
    }

    OBJDBG(("plugin: send reshape %p at %d,%d,%d,%d\n", pp, box->x0, box->y0, box->x1, box->y1));

    /* Build message block */
    reshape = (message_plugin_reshape *) &msg.data;

    reshape->flags = 0;
    reshape->box = *(plugin_box *)box;
    reshape->window_handle = (plugin_w)frontend_get_window_handle(pp->doc->parent);

    msg.hdr.size = sizeof(msg.hdr) + sizeof(*reshape);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_RESHAPE;
    msg.hdr.your_ref = 0;

    plugin_send_message(&msg, pp);

    return 1;
}

int plugin_send_close(plugin pp)
{
    wimp_msgstr msg;
    message_plugin_close *close = (message_plugin_close *) &msg.data;

    if (pp == NULL)
	pp = first_helper();

    OBJDBG(("plugin: send close %p state %d\n", pp, pp ? pp->state : -1));

    /* only send close if it is actually open - this stops the problem
       with the toolbar closing killing off the sound that is just
       starting up */
    if (pp == NULL || pp->state != plugin_state_OPEN)
	return 0;

    if (!pp->instance)
	return 0;

    /* Build message block */
    close->flags = 0;

    msg.hdr.size = sizeof(msg.hdr) + sizeof(*close);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_CLOSE;
    msg.hdr.your_ref = 0;

    plugin_send_message(&msg, pp);

    pp->state = plugin_state_SENT_CLOSE;

    return 1;
}

int plugin_send_focus(plugin pp)
{
    wimp_msgstr msg;
    message_plugin_focus *focus = (message_plugin_focus *) &msg.data;

    if (pp == NULL)
	pp = first_helper();

    OBJDBG(("plugin: send focus %p state %d\n", pp, pp ? pp->state : -1));

    if (pp == NULL || pp->state != plugin_state_OPEN)
	return 0;

    if ((pp->opening_flags & plugin_opening_CAN_FOCUS) == 0)
	return 0;

    /* Build message block */
    focus->flags = 0;

    msg.hdr.size = sizeof(msg.hdr) + sizeof(*focus);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_FOCUS;
    msg.hdr.your_ref = 0;

    plugin_send_message(&msg, pp);

    return 1;
}

/* ----------------------------------------------------------------------------- */

int plugin_send_action(plugin pp, int new_action)
{
    wimp_msgstr msg;
    message_plugin_action *action = (message_plugin_action *) &msg.data;

    /* if no plugin specified then use the top of the helper list */
    if (pp == NULL)
	pp = first_helper();

    OBJDBG(("plugin: send action %p action %d state %d helper %d\n",
	    pp, new_action, pp ? pp->state : -1, pp ? pp->priv_flags & plugin_priv_HELPER : -1));

    if (pp == NULL || pp->state != plugin_state_OPEN)
	return 0;

    /* Build message block */
    action->flags = 0;
    action->new_state = new_action;

    msg.hdr.size = sizeof(msg.hdr) + sizeof(*action);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_ACTION;
    msg.hdr.your_ref = 0;

    plugin_send_message(&msg, pp);

    return 1;
}

int plugin_send_abort(plugin pp)
{
    wimp_msgstr msg;
    message_plugin_abort *abort = (message_plugin_abort *) &msg.data;

    /* if no plugin specified then use the top of the helper list */
    if (pp == NULL)
	pp = first_helper();

    if (pp == NULL || pp->state != plugin_state_OPEN)
	return 0;

    OBJDBG(("plugin: send abort %p state %d\n", pp, pp->state));

    /* Build message block */
    abort->flags = 0;

    msg.hdr.size = sizeof(msg.hdr) + sizeof(*abort);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_ABORT;
    msg.hdr.your_ref = 0;

    plugin_send_message(&msg, pp);

    return 1;
}

/* ----------------------------------------------------------------------------- */

static void plugin_send_stream_new(plugin_stream_private *psp, int your_ref)
{
    wimp_msgstr msg;
    message_plugin_stream_new *snew = (message_plugin_stream_new *)&msg.data;

    OBJDBG(("plugin: send stream new %p state %d your ref %d\n", psp, psp->stream_state, your_ref));

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(*snew);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_STREAM_NEW;
    msg.hdr.your_ref = your_ref;

    snew->flags = (plugin_stream_TYPE_NORMAL << plugin_stream_TYPE_SHIFT);
    write_string(&msg, &snew->mime_type, psp->mime_type);
    snew->target.offset = 0;

    fillin_stream(&msg, psp, &snew->stream);

    plugin_send_message(&msg, psp->pp);
}

static void plugin_send_stream_destroy(plugin_stream_private *psp, int your_ref, plugin_reason reason)
{
    wimp_msgstr msg;
    message_plugin_stream_destroy *sdestroy = (message_plugin_stream_destroy *)&msg.data;

    /* Don't send destroy if known to be closed */
    if (psp->pp->state != plugin_state_OPEN)
	return;

    OBJDBG(("plugin: send stream destroy %p state %d your ref %d reason %d\n", psp, psp->stream_state, your_ref, reason));

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(*sdestroy);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_STREAM_DESTROY;
    msg.hdr.your_ref = your_ref;

    sdestroy->flags = 0;
    sdestroy->reason = reason;

    fillin_stream(&msg, psp, &sdestroy->stream);

    plugin_send_message(&msg, psp->pp);
}

#if 0
static void plugin_send_stream_written(plugin_stream_private *psp, int your_ref, int nbytes)
{
    wimp_msgstr msg;
    message_plugin_stream_written *swritten = (message_plugin_stream_written *)&msg.data;

    OBJDBG(("plugin: send stream written %p state %d your ref %d written %d bytes\n", psp, psp->stream_state, your_ref, nbytes));

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(*swritten);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_STREAM_WRITTEN;
    msg.hdr.your_ref = your_ref;

    swritten->flags = 0;

    fillin_stream(&msg, psp, &swritten->stream);

    plugin_send_message(&msg, psp->pp);
}
#endif

static void plugin_send_stream_write(plugin_stream_private *psp, int your_ref, int flags, void *data, int data_len)
{
    wimp_msgstr msg;
    message_plugin_stream_write *write = (message_plugin_stream_write *)&msg.data;

    OBJDBG(("plugin: send stream write %p state %d your ref %d\n", psp, psp->stream_state, your_ref));

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(*write);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_STREAM_WRITE;
    msg.hdr.your_ref = your_ref;

    write->flags = flags;
    write->offset = psp->offset;
    write->data.ptr = data;
    write->data_len = data_len;

    fillin_stream(&msg, psp, &write->stream);

    plugin_send_message(&msg, psp->pp);
}

static void plugin_send_stream_as_file(plugin_stream_private *psp, int your_ref, const char *cfile)
{
    wimp_msgstr msg;
    message_plugin_stream_as_file *as_file = (message_plugin_stream_as_file *)&msg.data;

    OBJDBG(("plugin: send stream as file %p state %d your ref %d as file '%s'\n", psp, psp->stream_state, your_ref, cfile));

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(*as_file);
    msg.hdr.action = (wimp_msgaction)MESSAGE_PLUGIN_STREAM_AS_FILE;
    msg.hdr.your_ref = your_ref;

    write_string(&msg, &as_file->file_name, cfile);

    fillin_stream(&msg, psp, &as_file->stream);

    plugin_send_message(&msg, psp->pp);
}

/* ----------------------------------------------------------------------------- */

/*
 * size is the total data size (or 0 if unknown)
 * so_far is the amount of data available in the file
 * it is up to us to keep count of how much data we have read from the file
 */

static void plugin_stream_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
    plugin_stream_private *psp = h;

    OBJDBGN(("plugin: stream progress psp %p status %d size %d so_far %d fh %d ftype %03x\n", psp, status, size, so_far, fh, ftype));

    psp->size = size;
    psp->status = (transfer_status)status;

    /* return if no new data */
    if (so_far <= psp->offset)
	return;

    switch (psp->stream_state)
    {
    case plugin_stream_state_STARTED:
	/* sent the new stream message */
	plugin_send_stream_new(psp, 0);
	psp->stream_state = plugin_stream_state_SENT_NEW_STREAM;
	break;

    case plugin_stream_state_READY_TO_WRITE:
	if (psp->stream_type == plugin_stream_TYPE_NORMAL || psp->stream_type == plugin_stream_TYPE_ASFILE)
	{
	    int fsnum = get_fsnum(fh);
	    switch (fsnum)
	    {
	    case fsnum_resourcefs:
		/* use a direct pointer to the data */
		plugin_send_stream_write(psp, 0, plugin_stream_write_DATA_PTR, (char *)get_fsptr(fh) + psp->offset, so_far - psp->offset);
		break;

	    case fsnum_cachefs:
		/* set flag to say ptr is an anchor */
		plugin_send_stream_write(psp, 0, plugin_stream_write_DATA_ANCHOR, get_fsptr(fh), so_far - psp->offset);
		break;

	    default:
		/* use from file */
		plugin_send_stream_write(psp, 0, plugin_stream_write_DATA_FILE, (void *)fh, so_far - psp->offset);
		break;
	    }
	    psp->stream_state = plugin_stream_state_WRITING;
	}
	break;
    }
}


static void plugin_stream_completing(plugin_stream_private *psp)
{
    OBJDBG(("plugin: stream completing psp %p size %d offset %d stream_type %d\n", psp, psp->size, psp->offset, psp->stream_type));

    if (psp->stream_state == plugin_stream_state_STARTED)
    {
	plugin_send_stream_new(psp, 0);
	psp->stream_state = plugin_stream_state_SENT_NEW_STREAM;
    }
    else switch (psp->stream_type)
    {
    case plugin_stream_TYPE_ASFILE:
	if (psp->offset < psp->size)
	    plugin_stream_progress(psp, status_COMPLETED_FILE, psp->size, psp->size, psp->file_handle, psp->file_type, psp->url);
	else
	{
	    close_stream_file(psp); /* close before sending stream as file to avoid multiple open errors */
	    plugin_send_stream_as_file(psp, 0, psp->file_name);
	    plugin_send_stream_destroy(psp, 0, plugin_reason_DONE);
	    plugin_stream_dispose(psp);
	}
	break;

    case plugin_stream_TYPE_ASFILEONLY:
	close_stream_file(psp); /* close before sending stream as file to avoid multiple open errors */
	plugin_send_stream_as_file(psp, 0, psp->file_name);
	plugin_send_stream_destroy(psp, 0, plugin_reason_DONE);
	plugin_stream_dispose(psp);
	break;

    case plugin_stream_TYPE_NORMAL:
	if (psp->offset < psp->size)
	    plugin_stream_progress(psp, status_COMPLETED_FILE, psp->size, psp->size, psp->file_handle, psp->file_type, psp->url);
	else
	{
	    plugin_send_stream_destroy(psp, 0, plugin_reason_DONE);
	    plugin_stream_dispose(psp);
	}
	break;

    case plugin_stream_TYPE_SEEK:
	break;
    }
}


static access_complete_flags plugin_stream_complete(void *h, int status, char *cfile, char *url)
{
    plugin_stream_private *psp = h;
    os_error *e;
    os_filestr fs;

    OBJDBG(("plugin: stream complete psp %p status %d file '%s'\n", psp, status, cfile));

    psp->status = (transfer_status)status;
    psp->ah = NULL;

    if ((transfer_status)status != status_COMPLETED_FILE)
    {
	plugin_send_stream_destroy(psp, 0, plugin_reason_NETWORK_ERROR);
	return 0;
    }

    /* get real size of file */
    fs.action = 5;
    fs.name = cfile;
    e = os_file(&fs);
    psp->size = fs.start;

    /* if there is more to go then reopen file  */
    if (psp->size > psp->offset)
    {
	psp->file_handle = ro_fopen(cfile, RO_OPEN_READ);

	/* and store away useful values */
	psp->file_name = strdup(cfile);
	psp->file_type = file_type(cfile);
    }

    plugin_stream_completing(psp);

    return access_CACHE;
}

/* ----------------------------------------------------------------------------- */

static void plugin_stream__detach(plugin_stream_private *psp)
{
    plugin_stream_private *item, *prev;

    /* detach from stream list */
    for (item = psp->pp->streams, prev = NULL; item; prev = item, item = item->next)
    {
	if (item == psp)
	{
	    if (prev)
		prev->next = item->next;
	    else
		psp->pp->streams = item->next;
	    break;
	}
    }
}

static void plugin_stream__dispose(plugin_stream_private *psp)
{
    OBJDBG(("plugin: stream dispose psp %p ah %p fh %d\n", psp, psp->ah, psp->file_handle));

    /* kill and free */
    if (psp->ah)
    {
	access_abort(psp->ah);
	psp->ah = NULL;
    }

    /* ensure file closed */
    close_stream_file(psp);

    mm_free(psp->file_name);
    mm_free(psp->url);
    mm_free(psp->mime_type);
    mm_free(psp);
}

static void plugin_stream_dispose(plugin_stream_private *psp)
{
    plugin_stream__detach(psp);
    plugin_stream__dispose(psp);
}

static void plugin_stream_dispose_all(plugin pp)
{
    plugin_stream_private *psp;

    OBJDBG(("plugin: stream dispose all in pp=%p\n", pp));

    psp = pp->streams;
    while (psp)
    {
	plugin_stream_private *next = psp->next;
	plugin_stream__dispose(psp);
	psp = next;
    }

    pp->streams = NULL;
}

/* ----------------------------------------------------------------------------- */

plugin_stream_private *plugin_stream_create_new(plugin pp, const char *url, const char *bfile, const char *content_type)
{
    plugin_stream_private *psp = mm_calloc(sizeof(*psp), 1);
    os_error *e;
    access_handle ah;
    access_post_info post;

    OBJDBG(("plugin: create stream new %p from objpp %p url '%s' bfile '%s'\n", psp, pp, url, strsafe(bfile)));

    /* link into stream list */
    psp->next = pp->streams;
    pp->streams = psp;

    /* fill in back pointer */
    psp->pp = pp;
    psp->url = strdup(url);

    psp->stream_state = plugin_stream_state_STARTED;

    /* start the stream going */
    post.body_file = (char *)bfile;
    post.content_type = (char *)content_type;
    e = access_url(psp->url, access_MUST_BE_FOUND /* flags */, NULL /* ofile */, &post,
		   pp->doc ? pp->doc->url : NULL,
		   plugin_stream_progress, plugin_stream_complete, psp, &ah);
    if (!e && ah)
    {
	OBJDBGN(("plugin: stream created ah %p\n", ah));
	psp->ah = ah;
    }
    else
    {
	OBJDBGN(("plugin: no stream created e '%s'\n", e ? e->errmess : ""));
	/* we don't destroy the stream here as we are still in the middle of a message session */
    }
    return psp;
}

/* ----------------------------------------------------------------------------- */

plugin plugin_new(struct rid_object_item *obj, be_doc doc, be_item ti)
{
    FILE *f;
    plugin_private *pp = mm_calloc(sizeof(*pp), 1);

    OBJDBG(("plugin: new pp %p from obj %p doc %p\n", pp, obj, doc));

    pp->parameter_file = strdup(rs_tmpnam(NULL));

    f = mmfopen(pp->parameter_file, "wb");
    if (!f)
    {
	mm_free(pp->parameter_file);
	mm_free(pp);
	return NULL;
    }

    plugin_write_parameter_file(f, obj, doc ? BASE(doc) : NULL);
    mmfclose(f);

#if LOCK_FILES
    file_lock(pp->parameter_file, TRUE);
#endif

    /* copy useful values out of 'obj' */
    pp->objd.classid = strdup(obj->classid);
    pp->objd.data = strdup(obj->data);
    pp->objd.codebase = strdup(obj->codebase);

    pp->objd.data_ftype = obj->data_ftype;
    pp->objd.classid_ftype = obj->classid_ftype;

    pp->parent_item = ti;
    pp->doc = doc;

    link(&plugin_list, pp);

    return pp;
}

void plugin_destroy(plugin pp)
{
    OBJDBG(("plugin: destroy pp %p\n", pp));

    if (pp)			/* FIX: didn't check NULL before */
    {
	plugin_stream_dispose_all(pp);

	if (pp->state < plugin_state_SENT_CLOSE)
	    plugin_send_close(pp);

	free_rma_strings(-1, pp);

	remove_parameter_file(pp);

	/* free objectcopied data */
	mm_free(pp->objd.classid);
	mm_free(pp->objd.codebase);
	mm_free(pp->objd.data);

	/* free helper info */
	if (pp->helper.cfile)
	{
#if LOCK_FILES
	    file_lock(pp->helper.cfile, FALSE);
#endif
	    mm_free(pp->helper.cfile);
	}

	/* unlink from list */
	unlink(&plugin_list, pp);

	/* remove the message handler for helpers */
	if ((pp->priv_flags & plugin_priv_HELPER) && --helper_count == 0)
	    frontend_message_remove_handler(plugin_message_handler, NULL);

	mm_free(pp);
    }
}

/* ----------------------------------------------------------------------------- */

plugin plugin_helper(const char *url, int ftype, const char *mime_type, void *parent, const char *cfile)
{
    plugin pp;
    rid_object_item obj;
    rid_object_param param;

    OBJDBG(("plugin: helper url %s ftype %x mime_type '%s'\n", url, ftype, strsafe(mime_type)));

    memset(&param, 0, sizeof(param));
    param.name = "AUTOSTART";
    param.value = "TRUE";
    param.valuetype = HTML_PARAM_VALUETYPE_DATA;

    memset(&obj, 0, sizeof(obj));
    obj.element = HTML_A;
    obj.data = strdup(url);
    obj.data_ftype = ftype;
    obj.data_mime_type = strdup(mime_type);

    obj.classid_ftype = -1;

    obj.params = &param;

    /* this adds us to the plugin_list */
    pp = plugin_new(&obj, NULL, NULL);

    if (pp)
    {
	/* add a message handler for helpers */
	if (helper_count++ == 0)
	    frontend_message_add_handler(plugin_message_handler, NULL);

	pp->helper.parent = parent;
	pp->helper.cfile = strdup(cfile);

	pp->priv_flags |= plugin_priv_HELPER;

/* 	file_lock(cfile, TRUE); */
	plugin_send_open(pp, NULL, plugin_open_HELPER);
    }

    return pp;
}

/* ----------------------------------------------------------------------------- */

void plugin_get_info(plugin pp, int *flags, int *state)
{
    if (!pp)
    {
	if (flags)
	    *flags = 0;
	if (state)
	    *state = 0;
    }

    if (flags)
    {
	int f = 0;

	if (pp->opening_flags & plugin_opening_BUSY)
	    f |= be_plugin_BUSY;

	if (pp->opening_flags & plugin_opening_CAN_ACTION)
	    f |= be_plugin_CAN_ACTION;

	if (pp->opening_flags & plugin_opening_HELPER)
	    f |= be_plugin_HELPER;

	if (pp->opening_flags & plugin_opening_CAN_FOCUS)
	    f |= be_plugin_CAN_FOCUS;

	*flags = f;
    }

    if (state)
    {
	*state = pp->state;
    }
}

/* ----------------------------------------------------------------------------- */

/* We pass the size of the file in here but the memory will already
 * have been allocated for it so we only want to know if there is
 * enough memory free in the rest of the system
 */

BOOL plugin_is_there_enough_memory(int ft, int size)
{
    int free;
    int extra = 0;

    /* if we don't know the size then we have to say yes */
    if (size == -1)
	return TRUE;

    extra += 32*1024;		/* for CodecCtl */
    extra += 128*1024;		/* for ARMovie task */
    switch (ft)
    {
    case 0xae7:			/* video replay */
    case 0xfb1:
	extra += 512*1024;
	break;

    case 0xfb2:			/* audio replay */
    case 0xfc2:
    case 0xf88:
	extra += 128*1024;
	break;

    case 0xfd4:			/* MIDI */
	extra += 128*1024;
	break;
    }

    free = get_free_memory_size();

    return extra <= free;
}

/* ----------------------------------------------------------------------------- */

#if DEBUG
void plugin_dump(void)
{
    plugin pp;
    for (pp = plugin_list; pp; pp = pp->next)
    {
	DBG(("plugin: %p: state %d %s task %x cfile '%s'\n",
	     pp, pp->state, pp->priv_flags & plugin_priv_HELPER ? "helper" : "embed", pp->task, strsafe(pp->helper.cfile)));
    }
}
#endif

/* ----------------------------------------------------------------------------- */


/*
 * Handles:
 * The 'handle' passeed to the message_handler is the 'doc' handle of the document.
 * The browser instance handle in the messages is the rid_object_item pointer.
 */

int plugin_message_handler(wimp_eventstr *e, void *handle)
{
    antweb_doc *doc = handle;
    plugin_private *pp;
    wimp_msgstr *msg;

    /* Check that it is a message and one in our range */
    if ((e->e != wimp_ESEND && e->e != wimp_ESENDWANTACK && e->e != wimp_EACK) ||
	e->data.msg.hdr.action < MESSAGE_PLUGIN_OPEN ||
	e->data.msg.hdr.action > MESSAGE_PLUGIN_OPEN+63)
	return FALSE;

    msg = &e->data.msg;

    /* Check that we have legal object pointer for this document or helper list */
    if ((pp = locate_object(doc, ((message_plugin_base *)&msg->data)->instance.parent)) == NULL)
	return FALSE;

    /* after this point we assume the message is for us and therefore claim it */
    if (e->e != wimp_EACK)
    {
	/* Free any strings referenced by this msg reference */
	free_rma_strings(msg->hdr.your_ref, NULL);

	switch (msg->hdr.action)
	{

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_OPENING:
	{
	    message_plugin_opening *opening = (message_plugin_opening *)&msg->data;

	    OBJDBG(("plugin: msg opening %p state %d from task %x\n", pp, pp->state, msg->hdr.task));

	    if (msg->hdr.your_ref == pp->msgref)
	    {
		/* Either remove the file or unlock it (and assume plugin will remove it eventually) */
		if ((opening->flags & plugin_opening_NEED_FILE) == 0)
		    remove_parameter_file(pp);
		else
		{
#if LOCK_FILES
		    file_lock(pp->parameter_file, FALSE);
#endif
		}

		switch (pp->state)
		{
		case plugin_state_SENT_OPEN:
		case plugin_state_SENT_OPEN_2:
		{
		    char *url;

		    OBJDBG(("plugin: opening flags %x - helper before %d after %d\n", opening->flags, pp->priv_flags & plugin_priv_HELPER ? 1 : 0, opening->flags & plugin_opening_HELPER ? 1 : 0));

		    pp->state = plugin_state_OPEN;
		    pp->instance = opening->instance.plugin;
		    pp->task = msg->hdr.task;
		    pp->opening_flags = opening->flags;
#if 0
		    /* if it wasn't asked to be a helper but it is  */
		    if ((opening->flags & plugin_opening_HELPER) && (pp->priv_flags & plugin_priv_HELPER) == 0)
		    {
#if 0
			/* check for enabling the helper message handler before adding to that list */
			if (helper_list == NULL)
			    frontend_message_add_handler(plugin_message_handler, NULL);

			/* then swap list */
			unlink(&plugin_list, pp);
			link(&helper_list, pp);
			/* remove from the page - can't do this as NULL means not fetched */
/* 			if (pp->parent_item) */
/* 			    ((rid_text_item_object *)pp->parent_item)->object->state.plugin.pp = NULL; */

			/* check for removing the page message handler */
			if (pp->doc && --pp->doc->object_handler_count == 0)
			    frontend_message_remove_handler(plugin_message_handler, pp->doc);
#endif
			/* transfer the view handle */
			if (pp->doc)
			    pp->helper.parent = pp->doc->parent;

			/* and zero the document and parent item handles */
			pp->doc = NULL;
			pp->parent_item = NULL;

			/* no point in a reshape now */
			pp->pending_reshape = FALSE;

			/* set the priv helper flag - so destroy works properly */
			pp->priv_flags |= plugin_priv_HELPER;
		    }
#endif
		    /* set HELPER flag in case they didn't */
		    if (pp->priv_flags & plugin_priv_HELPER)
			pp->opening_flags |= plugin_opening_HELPER;

		    /* were we waiting to send a reformat message */
		    if (pp->pending_reshape)
		    {
			plugin_send_reshape(pp, (wimp_box *)&pp->box);
		    }

		    /* Now we need to open a stream for the data/code for the plugin */
		    if (opening->flags & plugin_opening_FETCH_DATA && pp->objd.data)
		    {
			url = url_join(doc ? BASE(doc) : NULL, pp->objd.data);

			plugin_stream_create_new(pp, url, NULL, NULL);

			mm_free(url);
		    }

		    if (opening->flags & plugin_opening_FETCH_CODE && pp->objd.classid)
		    {
			url = url_join(pp->objd.codebase ? pp->objd.codebase : doc ? BASE(doc) : NULL, pp->objd.classid);

			plugin_stream_create_new(pp, url, NULL, NULL);

			mm_free(url);
		    }

		    /* tell the front end about the plugin opening */
		    frontend_view_status(pp->doc ? pp->doc->parent : pp->helper.parent, sb_status_PLUGIN, pp,
					 (pp->opening_flags & plugin_opening_BUSY) != 0, pp->play_state,
					 helpers_open());

		    break;
		}
		}
	    }
	    break;
	}

	case MESSAGE_PLUGIN_CLOSED:
	{
	    message_plugin_closed *closed = (message_plugin_closed *)&msg->data;

	    OBJDBG(("plugin: msg closed %p state %d\n", pp, pp->state));

	    pp->state = plugin_state_HAD_CLOSED;

	    plugin_stream_dispose_all(pp);

	    if (closed->flags & plugin_closed_ERROR_MSG)
	    {
		/* bug 12721: give generic message, rather than specific one */
#ifdef STBWEB
		frontend_complain(makeerrorf(ERR_BAD_FILE_TYPE,
					     get_file_type_name(pp->objd.classid_ftype != -1 ? pp->objd.classid_ftype : pp->objd.data_ftype)));
#else
		frontend_complain((os_error *)&closed->errnum);
#endif
	    }

    	    frontend_view_status(pp->doc ? pp->doc->parent : pp->helper.parent, sb_status_PLUGIN, pp,
				 FALSE, pp->play_state,
				 helpers_open());

	    /* if this is a helper then after this it ceases to exist as far as we are concerned */
	    /* only if it was opened as a helper otherwise it is still attached to the page */
/* 	    if (pp->opening_flags & plugin_opening_HELPER) */
	    if (pp->priv_flags & plugin_priv_HELPER)
		plugin_destroy(pp);
	    break;
	}

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_URL_ACCESS:
	{
	    message_plugin_url_access *url_access = (message_plugin_url_access *)&msg->data;

	    OBJDBG(("plugin: msg url_access %p state %d\n", pp, pp->state));

	    if ((url_access->flags & (plugin_url_access_POST|plugin_url_access_POST_FILE)) == plugin_url_access_POST)
		frontend_complain((os_error *)"    Can't POST from memory");
	    else
	    {
		char *target;

		target = get_ptr(url_access, url_access->target);

		if (target && target[0])
		{
		    /* open url in browser */
		    fe_post_info post, *postp;

		    /* FIXME: This doesn't allow several things
		     * 1) including headers in the POST
		     * 2) POSTing from memory
		     * 3) Get or Post Notify
		     */

		    if (url_access->flags & plugin_url_access_POST)
		    {
			post.body_file = get_ptr(url_access, url_access->data);
			post.content_type = NULL;
			postp = &post;
		    }
		    else
			postp = NULL;

		    frontend_complain(frontend_open_url(get_ptr(url_access, url_access->url),
							pp->doc ? pp->doc->parent : NULL,
							target, postp, 0));
		}
		else
		{
		    plugin_stream_private *psp;
		    /* send data to plugin */
		    psp = plugin_stream_create_new(pp, get_ptr(url_access, url_access->url), 
						   url_access->flags & plugin_url_access_POST ? get_ptr(url_access, url_access->data) : 0,
						   NULL);

		    if (psp)
		    {
			if (url_access->flags & plugin_url_access_NOTIFY)
			{
			    psp->notify_data = url_access->notify_data;
			    psp->notify = TRUE;
			}
		    }
		}
	    }
	    break;
	}

	/* ------------------------------------------------------------ */

	/* This is called in two situations
	   1) Initiated by the plugin to send data to the browser
	   2) A reply by the plugin to change the stream type
	 */

	case MESSAGE_PLUGIN_STREAM_NEW:
	{
	    message_plugin_stream_new *stream_new = (message_plugin_stream_new *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_new %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.your_ref, pp->msgref));

	    if (msg->hdr.your_ref != 0)
	    {
		/* Reply: filled in plugin instance handle and possibly a request to change the stream type */
		if ((psp = locate_stream(pp, stream_new->stream.instance.parent)) != NULL)
		{
		    if (psp->stream_state == plugin_stream_state_SENT_NEW_STREAM)
		    {
			psp->stream_type = stream_new->flags & plugin_stream_TYPE;
			psp->plugin_instance = stream_new->stream.instance.plugin;

			/* set the streaming type */
			if (psp->ah)
			    access_set_streaming(psp->ah, psp->stream_type != plugin_stream_TYPE_ASFILEONLY);

			if (psp->status == status_COMPLETED_FILE)
			    plugin_stream_completing(psp);
		    }
		}
	    }
	    else
	    {
		/* New message so they want a new stream
		 * FIXME: todo
		 */
	    }
	    break;
	}

	case MESSAGE_PLUGIN_STREAM_DESTROY:
	{
	    message_plugin_stream_destroy *stream_destroy = (message_plugin_stream_destroy *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_destroy %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if ((psp = locate_stream(pp, stream_destroy->stream.instance.parent)) != NULL)
	    {
	    }
	    break;
	}

	case MESSAGE_PLUGIN_STREAM_WRITE:
	{
	    message_plugin_stream_write *stream_write = (message_plugin_stream_write *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_write %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if ((psp = locate_stream(pp, stream_write->stream.instance.parent)) != NULL)
	    {
	    }
	    break;
	}

	case MESSAGE_PLUGIN_STREAM_WRITTEN:
	{
	    message_plugin_stream_written *stream_written = (message_plugin_stream_written *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_written %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if ((psp = locate_stream(pp, stream_written->stream.instance.parent)) != NULL)
	    {
		/* if they return < 0 bytes consumed then destroy the stream */
		if (stream_written->written < 0)
		{
		    plugin_send_stream_destroy(psp, msg->hdr.your_ref, plugin_reason_NETWORK_ERROR);
		    plugin_stream_dispose(psp);				/* psp is now dead */
		}
		else
		{
		    psp->offset += stream_written->written;
		    psp->stream_state = plugin_stream_state_READY_TO_WRITE;

		    if (psp->status == status_COMPLETED_FILE)
			plugin_stream_completing(psp);
		}
	    }
	    break;
	}

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_RESHAPE_REQUEST:
	{
 	    message_plugin_reshape_request *reshape_request = (message_plugin_reshape_request *)&msg->data;

	    OBJDBG(("plugin: msg reshape request %p state %d\n", pp, pp->state));

	    /* note this is sent in OS units */
 	    objects_resize(pp->doc, pp->parent_item, reshape_request->width*2, reshape_request->height*2);
	    break;
	}

	case MESSAGE_PLUGIN_UNLOCK:
	{
/* 	    message_plugin_unlock *unlock = (message_plugin_unlock *)&msg->data; */

	    OBJDBG(("plugin: msg unlock %p state %d\n", pp, pp->state));

	    break;
	}

	case MESSAGE_PLUGIN_STATUS:
	    if (pp->doc)
	    {
		message_plugin_status *status = (message_plugin_status *)&msg->data;

		OBJDBG(("plugin: msg status %p state %d\n", pp, pp->state));

		frontend_view_status(pp->doc->parent, sb_status_HELP,
				     get_ptr(status, status->message) );
	    }
	    break;

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_FOCUS:
	    if (pp->doc)
	    {
		/* message_plugin_focus *focus = (message_plugin_focus *)&msg->data; */

		OBJDBG(("plugin: msg focus %p state %d\n", pp, pp->state));

		backend_set_highlight(pp->doc, pp->parent_item, FALSE);
	    }
	    break;

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_BUSY:
	{
 	    message_plugin_busy *busy = (message_plugin_busy *)&msg->data;

	    OBJDBG(("plugin: msg busy %p flags %x state %d new play_state %d\n", pp, busy->flags, pp->state, busy->state));

	    if (busy->flags & plugin_busy_STATE_VALID)
		pp->play_state = busy->state;

	    if (busy->flags & plugin_busy_BUSY)
		pp->opening_flags |= plugin_opening_BUSY;
	    else
		pp->opening_flags &= ~plugin_opening_BUSY;

	    frontend_view_status(pp->doc ? pp->doc->parent : pp->helper.parent, sb_status_PLUGIN, pp,
				 (pp->opening_flags & plugin_opening_BUSY) != 0, pp->play_state, helpers_open());

	    break;
	}

	/* ------------------------------------------------------------ */

	break;
	}
    }
    else
    {
	/* Free any strings referenced by this msg reference */
	free_rma_strings(msg->hdr.my_ref, NULL);

	switch (msg->hdr.action)
	{

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_OPEN:
	{
	    message_plugin_open *open = (message_plugin_open *)&msg->data;

	    OBJDBG(("plugin: msg open bounce %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if (msg->hdr.my_ref == pp->msgref)
	    {
		/* Plugin open message has bounced, run task and try again */
		if (pp->state == plugin_state_SENT_OPEN)
		{
		    plugin ppp;

		    OBJDBG(("plugin: sent open check for already running\n"));

		    /* check and see if an application is in the process of being run for this file type already */
		    for (ppp = plugin_list; ppp; ppp = ppp->next)
			if (open->file_type == (ppp->objd.classid_ftype != -1 ? ppp->objd.classid_ftype : ppp->objd.data_ftype) &&
			    ppp->state == plugin_state_SENT_OPEN_2)
			{
			    OBJDBG(("plugin: plugin %p already started task for %03x\n", ppp, open->file_type));
			    break;
			}

		    if (!ppp)
		    {
			OBJDBG(("plugin: msg open run filetype %03x\n", open->file_type));
			frontend_plugin_start_task(open->file_type);
		    }
		    else
		    {
			OBJDBG(("plugin: already started running task for type %03x\n", open->file_type));
		    }

		    plugin_send_open(pp, (wimp_box *)&pp->box, open->flags);
		    pp->state = plugin_state_SENT_OPEN_2;
		}
		/* message has bounced a second time - mark the plugin as dead */
		else
		{
		    OBJDBG(("plugin: second bounce helper %d\n", pp->priv_flags & plugin_priv_HELPER ? 1 : 0));

		    /* if we tried as a helper then pass to frontend and remove */
		    if (pp->priv_flags & plugin_priv_HELPER)
		    {
			/* remove the call to pass_doc - if the plugin
                           has failed then anything else is unlikely
                           to work either */
/* 			frontend_pass_doc(pp->helper.parent, pp->objd.data, pp->helper.cfile, pp->objd.data_ftype); */
			plugin_destroy(pp);
			pp = NULL;
		    }
		    else
		    {
			pp->state = plugin_state_ABORTED;
			remove_parameter_file(pp);
		    }
		}
	    }
	    OBJDBG(("plugin: msg open bounce out state %d\n", pp ? pp->state : -1));
	    break;
	}

	/* ------------------------------------------------------------ */

	case MESSAGE_PLUGIN_STREAM_NEW:
	{
	    message_plugin_stream_new *stream_new = (message_plugin_stream_new *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_new bounce %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if ((psp = locate_stream(pp, stream_new->stream.instance.parent)) != NULL)
	    {
		/* abort stream? */
		plugin_stream_dispose(psp);
	    }
	    break;
	}

	case MESSAGE_PLUGIN_STREAM_DESTROY:
	{
	    message_plugin_stream_destroy *stream_destroy = (message_plugin_stream_destroy *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_destroy bounce %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if ((psp = locate_stream(pp, stream_destroy->stream.instance.parent)) != NULL)
	    {
	    }
	    break;
	}

	case MESSAGE_PLUGIN_STREAM_WRITE:
	{
	    message_plugin_stream_write *stream_write = (message_plugin_stream_write *)&msg->data;
	    plugin_stream_private *psp;

	    OBJDBG(("plugin: msg stream_write bounce %p state %d refs %d/%d\n", pp, pp->state, msg->hdr.my_ref, pp->msgref));

	    if ((psp = locate_stream(pp, stream_write->stream.instance.parent)) != NULL)
	    {
		/* abort stream? */
		plugin_stream_dispose(psp);
	    }
	    break;
	}

	/* ------------------------------------------------------------ */

	}
    }

    return TRUE;
}

/* ----------------------------------------------------------------------------- */


/* eof plugin.c */
