/* httpsave.c

 * Mechanismss to allow saving and loading of user configuration files
 * over http.

 * In quiet periods, or periodically, user config files.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memwatch.h"
#include "util.h"
#include "url.h"

#include "access.h"
#include "status.h"

#include "httpsave.h"

/* ----------------------------------------------------------------------------- */

struct httpsave_context
{
    BOOL fetch;			/* is this a fetch or a post */
    int file_mask;		/* which files are to be fetched or have been sent */

    httpsave_fetched_fn callback_fetched; /* callback functions and handle */
    httpsave_sent_fn callback_sent;
    void *callback_handle;

    char *temp_file;		/* fetching: where we unpack each file to */
				/* sending:  where we build the POST data in */
    FILE *temp_f;		/* file handle for above */

    access_handle access_h;	/* access handle for fetch or send */
};

/* ----------------------------------------------------------------------------- */

static char *httpsave_handler_url = NULL;

/*
 * This table must agree with the tag numbers in the enumeration
 */

static const char *tag_names[choicesfile_N_TAGS] =
{
    "COOKIES",
    "PASSWORDS",
    "PLUGINS",
    "HOTLIST",
    "CONFIG"
};

/* ----------------------------------------------------------------------------- */

static void hs_dispose(httpsave_t *ph)
{
    httpsave_t h = *ph;
    if (h)
    {
	HTSDBG(("hs_dispose(): h %p\n", h));

	if (h->access_h)
	    access_abort(h->access_h);

	if (h->temp_f)
	    mmfclose(h->temp_f);

	if (h->temp_file)
	{
	    remove(h->temp_file);
	    mm_free(h->temp_file);
	}

	mm_free(h);

	*ph = NULL;
    }
}

/* ----------------------------------------------------------------------------- */

static void hs_fetch_complete(httpsave_t h, const char *cfile)
{
    FILE *f;
    char *cfile_buf, *s;
    int cfile_size;

    h->temp_file = mm_strdup(rs_tmpnam(NULL));

    /* read file in */
    f = mmfopen(cfile, "r");
    if (!f)
	return;

    fseek(f, 0, SEEK_END);
    cfile_size = (int)ftell(f);
    cfile_buf = mm_malloc(cfile_size);

    fseek(f, 0, SEEK_SET);
    fread(cfile_buf, cfile_size, 1, f);

    mmfclose(f);

    /* process it into sections */
    s = strtok(cfile_buf, "=");
    if (s) do
    {
	char *tag_name = s;
	char *data = strtok(NULL, "&");
	int tag;

	/* find which tag this is */
	for (tag = 0; tag < choicesfile_N_TAGS; tag++)
	{
	    if (strcasecomp(tag_names[tag], tag_name) == 0)
		break;
	}

	/* if recognised then process the section */
	if (tag < choicesfile_N_TAGS)
	{
	    if ((h->temp_f = mmfopen(h->temp_file, "w")) != NULL)
	    {
		url_escape_to_file(data, h->temp_f);

		mmfclose(h->temp_f);
		h->temp_f = 0;

		if (!h->callback_fetched(h, tag, h->temp_file, h->callback_handle))
		    remove(h->temp_file);
	    }
	}

    }
    while ((s = strtok(NULL, "=")) != NULL);

    mm_free(cfile_buf);
}

/* ----------------------------------------------------------------------------- */

static void hs_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
}

/* ----------------------------------------------------------------------------- */

static access_complete_flags hs_complete(void *hh, int status, char *cfile, char *url)
{
    httpsave_t h = hh;

    HTSDBG(("hs_complete(): handle %p status %d cfile '%s' url '%s'\n", hh, status, cfile, url));

    h->access_h = NULL;

    if (h->fetch)
    {
	if (h->callback_fetched)
	{
	    if (status == status_COMPLETED_FILE)
		hs_fetch_complete(h, cfile);
	    else
		h->callback_fetched(h, -1, NULL, h->callback_handle);
	}
    }
    else
    {
	if (h->callback_sent)
	{
	    if (status == status_COMPLETED_FILE)
		h->callback_sent(h, h->file_mask, h->callback_handle);
	    else
		h->callback_sent(h, 0, h->callback_handle);
	}
    }

    hs_dispose(&h);

    return 0;
}

/* ----------------------------------------------------------------------------- */

/*
 * Open a file ready for saving
 */

httpsave_t httpsave_open(httpsave_sent_fn fn, void *handle)
{
    httpsave_t h;

    HTSDBG(("httpsave_open(): fn %d handle %p\n", (int)fn, handle));

    h = mm_calloc(sizeof(*h), 1);

    h->callback_sent = fn;
    h->callback_handle = handle;

    h->temp_file = mm_strdup(rs_tmpnam(NULL));
    h->temp_f = mmfopen(h->temp_file, "w");

    h->fetch = FALSE;

    if (!h->temp_f)
    {
	hs_dispose(&h);
    }

    HTSDBG(("httpsave_open(): returns %p\n", h));

    return h;
}

/*
 * Add a file to the form data ready for posting
 */

BOOL httpsave_add_file(httpsave_t h, int tag, const char *file_name)
{
    FILE *f;
    BOOL success;

    HTSDBG(("httpsave_add_file(): h %p tag %d file '%s'\n", h, tag, file_name));

    /* validate the arguments */
    if (!h || !h->temp_f)
	return FALSE;

    if (tag < 0 || tag >= choicesfile_N_TAGS)
	return FALSE;

    if (h->file_mask && (1<<tag))
	return FALSE;

    if (h->fetch)
	return FALSE;

    /* write out the tag name, preceeded by & if not the first */
    if (h->file_mask != 0)
	fputc('&', h->temp_f);

    if (fputs(tag_names[tag], h->temp_f) == EOF)
	return FALSE;

    fputc('=', h->temp_f);

    if ((f = mmfopen(file_name, "r")) == NULL)
	return FALSE;

    /* write out the data, escaped */
    success = url_escape_file_to_file(f, h->temp_f);

    mmfclose(f);

    if (success)
	h->file_mask |= (1<<tag);

    return success;;
}


/*
 * Close the file and send it
 */

os_error *httpsave_close_and_send(httpsave_t h)
{
    os_error *e;

    HTSDBG(("httpsave_close_and_send(): h %p\n", h));

    if (h->temp_f)
    {
	mmfclose(h->temp_f);
	h->temp_f = NULL;
    }

    e = access_url(httpsave_handler_url, access_NOCACHE, NULL, h->temp_file, NULL,
		   hs_progress, hs_complete, h, &h->access_h);

    if (e || !h->access_h)
	hs_dispose(&h);

    return e;
}

/* ----------------------------------------------------------------------------- */

/*
 * Initiate a fetch of the files whose bits are set in the file_mask.
 * When the fetch is done the fetched_fn will be called once for each file
 * after it has been unpacked.
 */


httpsave_t httpsave_fetch(int file_mask, httpsave_fetched_fn fn, void *handle)
{
    httpsave_t h;
    char url[1024];
    int i;
    BOOL need_amp;
    os_error *e;

    HTSDBG(("httpsave_fetch(): file_mask %xfn %x handle %p\n", file_mask, (int)fn, handle));

    if (file_mask == 0)
	return NULL;

    /* fill in structure */
    h = mm_calloc(sizeof(*h), 1);

    h->file_mask = file_mask;

    h->callback_fetched = fn;
    h->callback_handle = handle;

    /* build URL to fetch from */
    strcpy(url, httpsave_handler_url);
    strcat(url, "?");

    need_amp = FALSE;
    for (i = 0; i < choicesfile_N_TAGS; i++)
    {
	if (file_mask & (1<<i))
	{
	    if (need_amp)
		strcat(url, "&");
	    else
		need_amp = TRUE;

	    strcat(url, tag_names[i]);
	}
    }

    /* go fetch */
    e = access_url(url, access_NOCACHE, NULL, NULL, NULL,
		   hs_progress, hs_complete, h, &h->access_h);

    if (e || !h->access_h)
	hs_dispose(&h);

    HTSDBG(("httpsave_fetch(): returns %p e %s\n", h, e ? e->errmess : ""));

    return h;
}

/* ----------------------------------------------------------------------------- */

/*
 * Abort either a fetch or a send
 */

void httpsave_abort(httpsave_t h)
{
    HTSDBG(("httpsave_abort(): h %p\n", h));

    hs_dispose(&h);
}

/* ----------------------------------------------------------------------------- */

/*
 * Initialise this code with the URL to use as a handler.
 */

void httpsave_set_url(const char *handler_url)
{
    HTSDBG(("httpsave_set_url(): url '%s'\n", handler_url));

    mm_free(httpsave_handler_url);
    httpsave_handler_url = mm_strdup(handler_url);
}

/* ----------------------------------------------------------------------------- */

/* eof httpsave.c */
