/* -*-c-*- */

/* hotlist.c */

/* Deal with hotlists */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msgs.h"
#include "os.h"
#include "swis.h"

#include "config.h"
#include "makeerror.h"
#include "filetypes.h"

#include "hotlist.h"
#include "version.h"
#include "util.h"
#include "memwatch.h"
#include "url.h"
#include "urlopen.h"

typedef struct hotlist_item hotlist_item;

struct hotlist_item
{
    hotlist_item *next;
    char *url;
    char *title;
};

static hotlist_item *hotlist_list = NULL, *hotlist_last = NULL;
static BOOL hotlist_changed = FALSE;

/* ---------------------------------------------------------------------- */

static char *xfgets_without_nl(FILE *in)
{
    char buffer[1024];
    int len;

    if (fgets(buffer, sizeof(buffer), in) == NULL)
	return NULL;

    len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n')
	buffer[len-1] = '\0';

    return len <= 1 ? NULL : strdup(buffer);
}

static char *title_or_url(const hotlist_item *item)
{
    return item->title ? item->title : item->url;
}

/* ---------------------------------------------------------------------- */

static int hotlist__compare(const void *o1, const void *o2)
{
    const hotlist_item **item1 = (const hotlist_item **)o1;
    const hotlist_item **item2 = (const hotlist_item **)o2;
    return strcasecomp(title_or_url(*item1), title_or_url(*item2));
}

static hotlist_item *hotlist__find(const char *url, const char *title)
{
    hotlist_item *item;

    for (item = hotlist_list; item; item = item->next)
    {
	if ((url == NULL || item->url == NULL || strcmp(url, item->url) == 0) &&
	    (title == NULL || item->title == NULL || strcmp(title, item->title) == 0))
	{
	    break;
	}
    }
    return item;
}

/* ---------------------------------------------------------------------- */

static void hotlist__free_item(hotlist_item *item)
{
    mm_free(item->url);
    mm_free(item->title);
    mm_free(item);
}

static void hotlist__free(void)
{
    hotlist_item *item;

    item = hotlist_list;
    while (item)
    {
	hotlist_item *next = item->next;

	hotlist__free_item(item);

	item = next;
    }

    hotlist_list = hotlist_last = NULL;
}

/* ---------------------------------------------------------------------- */

static void hotlist__add(char *url, char *title, BOOL in_order)
{
    hotlist_item *item;

    /* if we already have this URL in the list then ignore the request */
    if (hotlist__find(url, NULL) != NULL)
	return;
    
    item = mm_calloc(sizeof(hotlist_item), 1);

    /* add to list */
    if (hotlist_last)
    {
	hotlist_last->next = item;
	hotlist_last = item;
    }
    else
    {
	hotlist_last = hotlist_list = item;
    }
    
    /* fill in data */
    item->url = url;
    item->title = title;

    hotlist_changed = TRUE;
}

static void hotlist__unlink(hotlist_item *last, hotlist_item *item)
{
    if (last)
	last->next = item->next;
    else
	hotlist_list = item->next;

    if (hotlist_last == item)
	hotlist_last = last;
}

static void hotlist__remove(const char *url, const char *title)
{
    hotlist_item *item, *last;

    for (item = hotlist_list, last = NULL; item; )
    {
	hotlist_item *next = item->next;

	if ((url == NULL || item->url == NULL || strcmp(url, item->url) == 0) &&
	    (title == NULL || item->title == NULL || strcmp(title, item->title) == 0))
	{
	    hotlist__unlink(last, item);
	    hotlist__free_item(item);

	    hotlist_changed = TRUE;
	}
	else
	{
	    last = item;
	}

	item = next;
    }
}

/* ---------------------------------------------------------------------- */

static void hotlist__sort(void)
{
    hotlist_item *item;
    int i, count;
    hotlist_item **item_list, **item_copy;

    /* count the number of items in the list */
    count = 0;
    for (item = hotlist_list; item; item = item->next)
	count++;

    /* exit unless at least two items */
    if (count < 2)
	return;
    
    /* allocate an array to hold pointers to each of the hotlist_items */
    item_list = mm_calloc(sizeof(*item_list), count);

    /* fill in pointers to each item in the array */
    item_copy = item_list;
    for (item = hotlist_list; item; item = item->next)
	*item_copy++ = item;

    /* sort the list */
    qsort(item_list, count, sizeof(*item_list), hotlist__compare);

    /* rewrite the links according to the sort order */
    hotlist_last = hotlist_list = *item_list;
    for (item_copy = item_list+1, i = 1; i < count; i++, item_copy++)
    {
	hotlist_last->next = *item_copy;
	hotlist_last = *item_copy;
    }
    hotlist_last->next = NULL;

    /* free the temporary sort array */
    mm_free(item_list);
}

/* ---------------------------------------------------------------------- */

void hotlist__remove_list(const char *list_orig)
{
    hotlist_item *item = hotlist_list, *last = NULL;
    char *list_copy = strdup(list_orig), *list;
    int current = 0;

    list = strtok(list_copy, "&");	/* init strtok, skip initial & */
    if (list) do
    {
	if (isdigit(*list))		/* check we don't have another parameter */
	{
	    int index = atoi(list);

	    while (item && index >= current)
	    {
		hotlist_item *next = item->next;
	    
		if (index == current)
		{
		    /* remove */
		    hotlist__unlink(last, item);
		    hotlist__free_item(item);

		    hotlist_changed = TRUE;
		}
		else
		{
		    last = item;
		}

		/* always increment current as query info is referenced to original list */
		current++;

		item = next;
	    }
	}
    }
    while ((list = strtok(NULL, "&")) != NULL);

    mm_free(list_copy);
}

/* ---------------------------------------------------------------------- */

static void hotlist__read(FILE *in)
{
    while (!feof(in) && !ferror(in))
    {
	char *url, *title;

	url = xfgets_without_nl(in);
	title = xfgets_without_nl(in);

	if (url)
	    hotlist__add(url, title, FALSE);
    }
    hotlist__sort();
}

static void hotlist__write(FILE *out)
{
    hotlist_item *item;

    for (item = hotlist_list; item; item = item->next)
    {
	STBDBG(("hotlist__write: item %p url %p title %p\n", item, item->url, item->title));

	STBDBG(("hotlist__write: url %s\n", item->url));
	STBDBG(("hotlist__write: title %s\n", item->title));

	if (item->url)
	{
	    fputs(item->url, out);
	    fputc('\n', out);

	    if (item->title)
		fputs(item->title, out);
	    fputc('\n', out);
	}
    }
}

/* ---------------------------------------------------------------------- */

static os_error *hotlist__changed_message(void)
{
    wimp_msgstr msg;
    msg.hdr.action = (wimp_msgaction)wimp_MURLHOTNEW;
    msg.hdr.size = sizeof(wimp_msghdr);
    msg.hdr.your_ref = 0;
    return wimp_sendmessage(wimp_ESEND, &msg, 0);
}

/* ---------------------------------------------------------------------- */

BOOL hotlist_read(const char *file)
{
    FILE *f;
    f = fopen(file, "r");
    if (f)
    {
	hotlist__read(f);
	fclose(f);

	hotlist__sort();
    }

    hotlist_changed = FALSE;

    return f != NULL;
}

BOOL hotlist_write(const char *file)
{
    FILE *f;
    f = fopen(file, "w");
    if (f)
    {
	hotlist__write(f);
	fclose(f);
    }

    hotlist_changed = FALSE;

    /* send wimp message to inform others */
    hotlist__changed_message();
    
    return f != NULL;
}

os_error *hotlist_add(const char *url, const char *title)
{
    os_error *ep;

    if (url == NULL)
	return NULL;
    
    sound_event(snd_HOTLIST_ADD);

    hotlist__add(strdup(url), strdup(title), TRUE);
    hotlist__sort();

    if ((ep = ensure_modem_line()) != NULL)
	return ep;
    
    if (hotlist_changed && !hotlist_write(config_hotlist_file))
	return makeerror(ERR_CANT_OPEN_HOTLIST);

    return NULL;
}

os_error *hotlist_remove(const char *url)
{
    os_error *ep;
    
    sound_event(snd_HOTLIST_REMOVE);

    hotlist__remove(NULL, url);
    
    if ((ep = ensure_modem_line()) != NULL)
	return ep;
    
    if (hotlist_changed && !hotlist_write(config_hotlist_file))
	return makeerror(ERR_CANT_OPEN_HOTLIST);

    return NULL;
}

os_error *hotlist_remove_list(const char *list)
{
    os_error *ep;
    
    hotlist__remove_list(list);
    
    if ((ep = ensure_modem_line()) != NULL)
	return ep;
    
    if (hotlist_changed && !hotlist_write(config_hotlist_file))
	return makeerror(ERR_CANT_OPEN_HOTLIST);

    return NULL;
}

void hotlist_write_list(FILE *fout, BOOL del)
{
    hotlist_item *item;
    int i;

    for (item = hotlist_list, i = 0; item; item = item->next, i++)
    {
	char *ttl = item->title ? item->title : item->url;

	if (del)
 	    fprintf(fout, msgs_lookup("hotsdI"), i, i, ttl, i, i);
	else
	{
	    fprintf(fout, msgs_lookup("hotsI2a"), i);
	    url_escape_to_file(item->url, fout);
	    fprintf(fout, msgs_lookup("hotsI2b"), ttl);
	}
    }
}

/* ---------------------------------------------------------------------- */

void hotlist_init(void)
{
    hotlist_read(config_hotlist_file);
}

void hotlist_shutdown(void)
{
    hotlist__free();
}

/* ---------------------------------------------------------------------- */

/* eof stbhots.c */
