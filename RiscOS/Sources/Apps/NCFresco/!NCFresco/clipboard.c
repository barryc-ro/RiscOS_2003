/* > clipboard.c

 * © SJ Middleton, 1993

 */

#include <stdio.h>
#include <string.h>

#include "wimp.h"

#include "memwatch.h"
#include "clipboard.h"
#include "interface.h"

#define no_err(e)   frontend_fatal_error(e)

enum
{

    datareq_Bounced,
    datareq_SentDataSave,
    datareq_SentDataLoad,
    datareq_LoadedOK,

    datareq_SentRequest
};

static clipboard_itemstr        clip__item = { 0 };

static const clipboard_itemstr  *clip__list = NULL;
static clipboard_destroyfn      clip__destroy = 0;

static BOOL         clip__hasclipboard = FALSE;
static void         *clip__anchor = NULL;
static void         *clip__handle;

/*static mem_allocfn  clip__alloc = c_alloc; */
/*static mem_freefn   clip__free = c_free; */

static int          clip__msgid = -1;
static int          clip__xferstate = datareq_Bounced;
/*static wimp_msgstr  clip__msg; */

/* ------------------------------------------------------------------------------ */

static const clipboard_itemstr *find_item(int filetype)
{
    const clipboard_itemstr *item;
    for (item = clip__list; item->filetype != -1; item++)
        if (item->filetype == filetype)
            return item;
    return NULL;
}

static const clipboard_itemstr *list_bestitem(const int *list)
{
    int filetype;
    while ((filetype = *list++) != -1)
    {
        const clipboard_itemstr *item = find_item(filetype);
        if (item)
            return item;
    }

    /* pdh: changed to return first one in case of failure */
    return clip__list;
}

/* ------------------------------------------------------------------------------ */

#if 0
static int list_contains(int filetype, const int *list)
{
    int type = filetype;
    if (list) while ((type = *list++) != -1)
        if (type == filetype)
            break;
    return type;
}

static int list_bestmatch(const int *list)
{
    const clipboard_itemstr *item = list_bestitem(list);
    return item ? item->filetype : -1;
}
#endif

/* ------------------------------------------------------------------------------- */

BOOL clipboard_eventhandler(wimp_eventstr *e, void *handle)
{
    if (e->e == wimp_ESEND || e->e == wimp_ESENDWANTACK)
    {
        wimp_msgstr *mp = &e->data.msg;
        switch (mp->hdr.action)
        {
            case Message_ClaimEntity:
                /* someone is claiming the clipboard */
                /* if it is not use and we have it then we should destroy it */
		STBDBG(("clip: claimed by task %x flags %x\n", mp->hdr.task, mp->data.words[0]));
                if (mp->data.words[0] & claimentity_Clipboard &&
                    clip__hasclipboard &&
                    mp->hdr.my_ref != clip__msgid)
                {
                    clipboard_Destroy();
                    return TRUE;
                }
                break;

            case Message_DataRequest:
            {
                /* someone wants the clipboard data, giving a list of file types */
                /* if we have it then send them a DATASAVE message */
                wimp_msgdatarequest *dp = (wimp_msgdatarequest *)&mp->data;
#if DEBUG
{
    int *fp = dp->filetype;
    STBDBG(("clip: datarequest from task %x flags %x types [", mp->hdr.task, dp->flags));
    while (*fp != -1)
        STBDBG((" %03x", *fp++));
    STBDBG((" ]\n"));
}
#endif
                if (clip__hasclipboard && (dp->flags & datarequest_Clipboard) )
                {
                    wimp_msgstr msg;
                    wimp_msgdatasave *datasave = &msg.data.datasave;
                    const clipboard_itemstr *item = clip__list ? list_bestitem(dp->filetype) : &clip__item;

/*                     if ( !item ) */
/*                         break; */

                    datasave->w = dp->w;
                    datasave->i = dp->i;
                    datasave->x = dp->x;
                    datasave->y = dp->y;
                    datasave->estsize = item->estsize;
                    datasave->type = item->filetype;
                    strcpy(datasave->leaf, "clipboard");

                    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(wimp_msgdatasave);
                    msg.hdr.your_ref = mp->hdr.my_ref;
                    msg.hdr.action = wimp_MDATASAVE;
                    no_err(wimp_sendmessage(wimp_ESENDWANTACK, &msg, mp->hdr.task));
                    clip__msgid = msg.hdr.my_ref;
                    clip__xferstate = datareq_SentDataSave;

		    STBDBG(("clip: sent datasave size %d type %03x to %x\n", datasave->estsize, datasave->type, mp->hdr.task));
                    return TRUE;
                }
                break;
            }

            case wimp_MDATASAVEOK:
            {
                /* someone has replied to our DATASAVE message */
                /* save the data and send them a DATALOAD */
                const clipboard_itemstr *item = clip__list ? find_item(mp->data.datasaveok.type) : &clip__item;
		STBDBG(("clip: datasaveok type %03x from task %x to '%s'\n", mp->data.datasaveok.type, mp->hdr.task, mp->data.datasaveok.name));
                if (item && item->save && item->save(mp->data.datasaveok.name, mp->data.datasaveok.type, clip__handle))
                {
                    wimp_msgstr msg;
                    os_filestr ofs;

                    msg = *mp;
                    msg.hdr.your_ref = mp->hdr.my_ref;
                    msg.hdr.action = wimp_MDATALOAD;

                    ofs.action = 0x17;
                    ofs.name = mp->data.datasaveok.name;
                    os_file(&ofs);
                    msg.data.dataload.type = ofs.reserved[0];
                    msg.data.dataload.size = ofs.start;

                    no_err(wimp_sendmessage(wimp_ESENDWANTACK, &msg, mp->hdr.task));
                    clip__msgid = msg.hdr.my_ref;
                    clip__xferstate = datareq_SentDataLoad;
		    STBDBG(("clip: sent dataload type %03x to task %x to '%s'\n", mp->data.dataload.type, mp->hdr.task, mp->data.dataload.name));
               }
                break;
            }

            case wimp_MDATALOADOK:
		STBDBG(("clip: dataloadok from task %x\n", mp->hdr.task));
                clip__xferstate = datareq_LoadedOK;
                break;

        }
    }
    else if (e->e == wimp_EACK)
    {
        /* a message has bounced - see if it was our last one */
        wimp_msgstr *mp = &e->data.msg;
        if (mp->hdr.my_ref == clip__msgid)
        {
	    STBDBG(("clip: msgtype 0x%x bounced\n", mp->hdr.action));
            clip__xferstate = datareq_Bounced;
        }
    }
    return FALSE;
    handle = handle;
}

/* ------------------------------------------------------------------------------ */

/*
 * If we don't already have the clipboard then broadcast the ClaimEntity message
 * and record that we own it.
 */

static void claim_clipboard(void)
{
    STBDBG(("clip: claim - have %d\n", clip__hasclipboard));
    if (!clip__hasclipboard)
    {
        wimp_msgstr msg;
        msg.hdr.size = sizeof(wimp_msghdr) + 4;
        msg.hdr.your_ref = 0;
        msg.hdr.action = Message_ClaimEntity;
        msg.data.words[0] = claimentity_Clipboard;
        no_err(wimp_sendmessage(wimp_ESEND, &msg, 0));

        /* preserve msgid so we can ignore this message */
        clip__msgid = msg.hdr.my_ref;
        clip__hasclipboard = TRUE;
    }
}

/* ------------------------------------------------------------------------------ */

static void *clipboard__Create(const clipboard_itemstr *list, clipboard_destroyfn destroyfn, int filetype, const void *data, int size, void *handle)
{
    STBDBG(("clip: create\n"));
    /* if we have it then discard it before creating a new one */
    clipboard_Destroy();

    /* resize or allocate the new data block */
    if (clip__anchor == NULL)
        clip__anchor = mm_malloc(size);
    else
        clip__anchor = mm_realloc(clip__anchor, size);

    /* then fill in all the appropriate fields */
    if (clip__anchor)
    {
        if (data)
            memcpy(clip__anchor, data, size);
        clip__item.estsize = size;
        clip__item.filetype = filetype;

        clip__list = list;
        clip__destroy = destroyfn;
        clip__handle = handle;
        claim_clipboard();
    }

    return clip__anchor;
}

/* ------------------------------------------------------------------------------ */

/*
 * Register one filetype with data
 */

void *clipboard_Create(int filetype, const void *data, int size)
{
    return clipboard__Create(NULL, 0, filetype, data, size, NULL);
}

/*
 * Register list by function
 */

void *clipboard_CreateList(const clipboard_itemstr *list, clipboard_destroyfn destroyfn, const void *data, int size, void *handle)
{
    return clipboard__Create(list, destroyfn, -1, data, size, handle);
}

/* ------------------------------------------------------------------------------ */

void clipboard_Destroy(void)
{
    STBDBG(("clip: destroy - have %d %p\n", clip__hasclipboard, clip__destroy));
    if (clip__hasclipboard)
    {
        if (clip__destroy)
        {
            clip__destroy(clip__handle);
            clip__destroy = 0;
        }

        clip__handle = NULL;

        mm_free(clip__anchor);
        clip__anchor = NULL;

        clip__hasclipboard = FALSE;
        clip__list = NULL;
    }
}

/* ------------------------------------------------------------------------------- */

void *clipboard_Data(int *psize)
{
    if (psize)
        *psize = clip__item.estsize;
    return clip__anchor;
}

BOOL clipboard_Local(void)
{
    return clip__hasclipboard;
}

/* ------------------------------------------------------------------------------ */

static int copy_list(int *listto, const int *listfrom)
{
    int type, *lpto = listto;
    if (listfrom) while ((type = *listfrom++) != -1)
        *lpto++ = type;
    *lpto++ = -1;
    return lpto - listto - 1;
}

/*
 * If datasave == NULL then we set window to -1 and so DATASAVE message returned
 * (if any) wil come throught to our unknown event handler.
 * Otherwise it will get delivered to the appropriate window.
 */

static void send_request(const int *filetype_list, const wimp_msgdatasave *datasave)
{
    int                 nitems;
    wimp_msgstr         msg;
    wimp_msgdatarequest *dp = (wimp_msgdatarequest *)&msg.data;

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(wimp_msgdatarequest);
    msg.hdr.action = Message_DataRequest;
    msg.hdr.your_ref = 0;

    dp->flags = datarequest_Clipboard;
    if (datasave)
    {
        dp->w = datasave->w;
        dp->i = datasave->i;
        dp->x = datasave->x;
        dp->y = datasave->y;
    }
    else
        dp->w = 0;

    nitems = copy_list(dp->filetype, filetype_list);

    msg.hdr.size = sizeof(wimp_msghdr) + sizeof(wimp_msgdatarequest) + nitems*4;
    no_err(wimp_sendmessage(wimp_ESEND /*WANTACK*/, &msg, 0));

    clip__msgid = msg.hdr.my_ref;
    clip__xferstate = datareq_SentRequest;
}

BOOL clipboard_Paste(const int *filetype_list, const wimp_msgdatasave *datasave)
{
    if (!clip__hasclipboard)
    {
        send_request(filetype_list, datasave);
        return FALSE;
    }
    return TRUE;
}

/* ------------------------------------------------------------------------------ */

/* eof clipboard.c */
