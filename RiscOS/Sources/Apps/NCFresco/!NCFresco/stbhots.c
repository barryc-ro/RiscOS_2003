/* -*-c-*- */

/* hotlist.c */

/* Deal with hotlists */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
typedef struct hotlist_info hotlist_info;

#define hotlist_DELETE_PENDING	0x01

struct hotlist_item
{
    hotlist_item *next;
    char *url;
    char *title;
    time_t last_used;
    int flags;
};

struct hotlist_info
{
    int format;
    int record_size;
};

static hotlist_item *hotlist_list = NULL, *hotlist_last = NULL;
static BOOL hotlist_changed = FALSE;
static int hotlist_count = 0;

/* ---------------------------------------------------------------------- */

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

static void hotlist__unlink(hotlist_item *last, hotlist_item *item)
{
    if (last)
	last->next = item->next;
    else
	hotlist_list = item->next;

    if (hotlist_last == item)
	hotlist_last = last;
}

static void hotlist__unlink_and_free(hotlist_item *last, hotlist_item *item)
{
    hotlist__unlink(last, item);
    hotlist__free_item(item);

    hotlist_changed = TRUE;
    hotlist_count--;
}

static void hotlist__add(char *url, char *title, time_t t, BOOL in_order)
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
    item->last_used = t;

    hotlist_changed = TRUE;
    hotlist_count++;
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
	    hotlist__unlink_and_free(last, item);
	}
	else
	{
	    last = item;
	}

	item = next;
    }
}

/* ---------------------------------------------------------------------- */

static void hotlist__remove_oldest(void)
{
    hotlist_item *item, *last;

    time_t lru_time = INT_MAX;
    hotlist_item *lru_item = NULL, *lru_last = NULL;

    for (item = hotlist_list, last = NULL; item; )
    {
	if (lru_time > item->last_used)
	{
	    lru_time = item->last_used;
	    lru_item = item;
	    lru_last = last;
	}
	last = item;
	item = item->next;
    }

    if (lru_item)
	hotlist__unlink_and_free(lru_last, lru_item);
}

static void hotlist__trim_length(void)
{
    if (config_hots_length > 0 && hotlist_count > config_hots_length)
	hotlist__remove_oldest();
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

static void hotlist__flush_pending_delete(void)
{
    hotlist_item *item = hotlist_list, *last = NULL;

    while (item)
    {
	hotlist_item *next = item->next;

	if (item->flags & hotlist_DELETE_PENDING)
	    hotlist__unlink_and_free(last, item);
	else
	    last = item;

	item = next;
    }
}

void hotlist_remove_list(const char *list_orig)
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
		    /* mark for removal */
		    item->flags |= hotlist_DELETE_PENDING;
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

static void hotlist__read_header(FILE *in, hotlist_info *info)
{
    int format = 0;
    int record_size = 2;

    while (!feof(in) && !ferror(in))
    {
	char *s = xfgets(in);
	if (s[0] != '#')
	{
	    if (strncmp(s, "Format:", sizeof("Format:")-1) == 0)
		format = atoi(s + sizeof("Format:")-1);
	    else if (strncmp(s, "Record:", sizeof("Record:")-1) == 0)
		record_size = atoi(s + sizeof("Record:")-1);
	    else if (strncmp(s, "Data", sizeof("Data")-1) == 0)
		break;
	    /* if no format has been seen then give up on header scan at this point */
	    else if (format == 0)
		break;
	}
	mm_free(s);
    }

    /* if we didn't get new format mark then reset to beginning of file */
    if (format == 0)
	fseek(in, 0, SEEK_SET);

    /* write out values */
    if (info)
    {
	info->format = format;
	info->record_size = record_size;
    }
}

static void hotlist__read(FILE *in)
{
    hotlist_info info;

    hotlist__read_header(in, &info);

    while (!feof(in) && !ferror(in))
    {
	char *url, *title;
	int i, last_used = 0;

	url = xfgets(in);
	title = xfgets(in);

	if (info.record_size >= 3)
	    fskipline(in);

	if (info.record_size >= 4)
	{
	    char *s = xfgets(in);
	    last_used = (time_t)strtoul(s, NULL, 16);
	    mm_free(s);
	}

	if (url)
	    hotlist__add(url, title, last_used, FALSE);

	/* skip any extra unused records */
	for (i = 4; i < info.record_size; i++)
	    fskipline(in);
    }
    hotlist__sort();
}

static void hotlist__write_header(FILE *in)
{
    fprintf(in, "# "PROGRAM_NAME" favorites\n");
    fprintf(in, "Format: 1\n");
    fprintf(in, "Record: 4\n");
    fprintf(in, "Data\n");
}

static void hotlist__write(FILE *out)
{
    hotlist_item *item;

    hotlist__write_header(out);
    
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

	    /* Supposedly the icon name */
	    fputc('\n', out);

	    /* Extra information */
	    fprintf(out, "%08x\n", item->last_used);
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

    STBDBG(("hotlist_read: %s\n", file));

    f = mmfopen(file, "r");
    if (f)
    {
	hotlist__read(f);
	mmfclose(f);

	hotlist__trim_length();
	hotlist__sort();
    }

    hotlist_changed = FALSE;

    return f != NULL;
}

BOOL hotlist_write(const char *file)
{
    FILE *f;

    STBDBG(("hotlist_write: %s\n", file));

    f = mmfopen(file, "w");
    if (f)
    {
	hotlist__write(f);
	mmfclose(f);
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

    hotlist__add(strdup(url), strdup(title), time(NULL), TRUE);
    hotlist__trim_length();
    hotlist__sort();

/*     if ((ep = ensure_modem_line()) != NULL) */
/* 	return ep; */
    
    if (hotlist_changed && !hotlist_write(config_hotlist_file))
	return makeerror(ERR_CANT_OPEN_HOTLIST);

    return NULL;
}

os_error *hotlist_remove(const char *url)
{
    os_error *ep;
    
    sound_event(snd_HOTLIST_REMOVE);

    hotlist__remove(NULL, url);
    
/*     if ((ep = ensure_modem_line()) != NULL) */
/* 	return ep; */
    
    if (hotlist_changed && !hotlist_write(config_hotlist_file))
	return makeerror(ERR_CANT_OPEN_HOTLIST);

    return NULL;
}

os_error *hotlist_flush_pending_delete(void)
{
    os_error *ep;
    
    hotlist__flush_pending_delete();

/*     if ((ep = ensure_modem_line()) != NULL) */
/* 	return ep; */
    
    if (hotlist_changed && !hotlist_write(config_hotlist_file))
	return makeerror(ERR_CANT_OPEN_HOTLIST);

    return NULL;
}

static void hotlist__write_list(FILE *fout, BOOL del)
{
    hotlist_item *item;
    int i;

    for (item = hotlist_list, i = 0; item; item = item->next, i++)
    {
	char *ttl = item->title ? item->title : item->url;

	if (del)
 	    fprintf(fout, msgs_lookup("favsd.I"), i, i, ttl, i, i, item->flags & hotlist_DELETE_PENDING ? "CHECKED" : "");
	else
 	    fprintf(fout, msgs_lookup("favs.I"), i, i, ttl);
    }
}

void hotlist_write_list(FILE *fout, void *handle)
{
    hotlist__write_list(fout, FALSE);
}

void hotlist_write_delete_list(FILE *fout, void *handle)
{
    hotlist__write_list(fout, TRUE);
}

/* ---------------------------------------------------------------------- */

void hotlist_return_url(int index, char **url)
{ 
    hotlist_item *item;
    int i;
    for (item = hotlist_list, i = 0; item && i < index; item = item->next, i++)
	;

    if (item)
    {
	/* update last used time */
	item->last_used = time(NULL);

	/* write out the file as its changed */
	hotlist_write(config_hotlist_file);

	/* write out the URL we've found */
	*url = strdup(item->url);
    }
    else
	*url = NULL;
}

/* ---------------------------------------------------------------------- */

void hotlist_optimise(void)
{
    hotlist_item *item, *last;

    for (last = NULL, item = hotlist_list; item; last = item, item = item->next)
    {
	hotlist_item *was = item;

	if (optimise_block((void **)&item, sizeof(*item)))
	{
	    if (last)
		last->next = item;
	    else
		hotlist_list = item;

	    if (hotlist_last == was)
		hotlist_last = item;
	}

	item->url = optimise_string(item->url);
	item->title = optimise_string(item->title);
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
