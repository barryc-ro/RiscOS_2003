/* -*-c-*- */

/* access.c */

/*
 * 26/3/96 SJM Added cookie support code in access_init, tidyup, http_fetch_start, http_fetch_alarm
 * 02/4/96 SJM Exported cache_insert and scrapfile_name.
 * 23/4/96 SJM Merged new changes
 * 16/5/96 SJM Made proxying ensure that the path is non-null
 * 23/5/96 SJM new cacheing scheme. cache_ functions moved to cache2. All cache_ calls routed through cache function table
 *             All cache functions now only access from here. Some more access wrappers to them for images.c and backend.c
 * 04/6/96 SJM keep count of cookies received in access_item and optionally write out cookie file at the end
 * 10/6/96 SJM changed User-Agent string to use PROGRAM_TITLE rather than PROGRAM_NAME
 * 14/6/96 SJM New config option to keep auth file up to date. Added saving to http_auth_rerequest
 *             find_realm checks for whether a scheme is supported before returning it. Now can cope with
 *             multiple WWW_AUTHENTICATE headers for different schemes. Took out error generation when user cancels authentication
 * 20/6/96 SJM Added auth auto-saving to ftp done. Started the auto redialling stuff.
 * 24/6/96 SJM Added visdelay around access_url(). Fixed memory leak in access_url (early returns).
 * 11/7/96 SJM new call to check if file type can be handled before downloading (only in http at present).
 * 15/8/96 NvS Added a 'Pragma: no-cache' line to the sent headers when a reload access is made
 * 30/8/96 SJM check the date etc headers and pass to cache code
 * 02/9/96 SJM new access flag to control whether type is checked or not
 * 3/9/96  NvS Changed debug fprintfs to use the debug.h functions
 */

/* Functions for accessing URLs */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "memwatch.h"

#include "msgs.h"
#include "os.h"
#include "alarm.h"
#include "wimp.h"
#include "swis.h"
#include "akbd.h"
#include "sprite.h"
#include "visdelay.h"

/* Need to know about a struct sockaddr_in */
#include "sys/types.h"
#include "netinet/in.h"

#include "urlopen.h"
#define ACCESS_INTERNAL
#include "access.h"
#include "url.h"
#include "util.h"
#include "status.h"
#include "gopher.h"
#include "makeerror.h"
#include "auth.h"
#include "config.h"
#include "licence.h"
#include "interface.h"
#include "filetypes.h"
#include "dir2html.h"
#include "files.h"

#include "version.h"
#include "verstring.h"

#include "resolve.h"
#include "dir2html.h"

#ifdef STBWEB_BUILD
#include "webftp.h"
#include "http.h"
#include "webgopher.h"
#else
#include "../webftp/webftp.h"
#include "../http/httppub.h"
#include "../webgopher/gopherpub.h"
#endif

#include "cookie.h"
#include "images.h"
#include "gbf.h"

#ifdef CBPROJECT
#include "cbtoolbar.h"
#endif


/* A file which gets displayed instead of all remote connections in file-
 * only versions of Fresco
 */
#define FILEONLY_EXCUSE "<Fresco$Dir>.Docs.nonet/html"

extern void translate_escaped_text(char *in, char *out, int len);

#ifndef DUMP_HEADERS
#define DUMP_HEADERS 1
#endif

#ifndef SEND_HOST
#define SEND_HOST 1
#endif

#ifndef SSL_UI
#define SSL_UI 0
#endif

#ifdef STBWEB
#define INTERNAL_URLS 1
#else
#define INTERNAL_URLS 0
#endif

#ifndef LOCK_FILES
#define LOCK_FILES 0
#endif

#ifndef POLL_INTERVAL
#define POLL_INTERVAL	10	/* centi-seconds */
#endif

#ifndef FILE_POLL_INTERVAL
#define FILE_POLL_INTERVAL	3	/* centi-seconds */
#endif

#ifdef FILEONLY
#define FILE_INITIAL_CHUNK      0x10000 /* 64K */
#define FILE_MAX_CHUNK          0x10000 /* 64K */
#else
#define FILE_INITIAL_CHUNK	0x400
#define FILE_MAX_CHUNK		0x4000	/* 16K */
#endif

#define FTP_IDLE_TIME	(60000)	/* = 10 * 60 * 100 = 10 minutes in centi-seconds */

#define access_type_HTTP	1
#define access_type_GOPHER	2
#define access_type_FTP		3
#define access_type_FILE	4
#define access_type_INTERNAL	5

#define HTTP_PORT		80
#define HTTPS_PORT		443

typedef struct _access_item {
    int access_type;
    access_url_flags flags;
    char *dest_host;		/* This can be from the URL or from a proxy pointer */
    struct sockaddr_in addr;	/* The socket address for the current connection */
    char *request_string;	/* This will be either the path or the whole URL for a proxy */
    char *url;
    char *ofile;
    short ftype, ft_is_set;	/* 16 bits for file type and a flag to say it has been set */
    char *referer;
    alarm_handler next_fn;
    access_progress_fn progress;
    access_complete_fn complete;
    void *h;
    void *transport_handle;
    BOOL hadwholepart;          /* used for serverpush (see http_fetch_alarm) */
    BOOL try_prefix, try_suffix;/* used for fancy DNS resolve */
    int suffix_num;
    struct _access_item *redirect;
    struct _access_item *next, *prev;
    union {
	struct {
	    char *body_file;
	    char *body_type;
	    int had_auth;	/* Set when the access was made with authentication */
	    int had_proxy_auth;	/* Set when the proxy access was made with authentication */
	    int had_passwd;     /* Set if we hav eto ask the user for a password */
	    fe_passwd pw;	/* Used while we are waiting for a userid/passwd pair. */
            int cookie_count;   /* number of cookies received in headers */
	    unsigned last_modified;	/* from headers */
	    unsigned expires;		/* from headers */
	    unsigned date;		/* from headers */
#if SSL_UI
	    struct
	    {
		BOOL checked_headers;
		fe_ssl fe;	/* frontend handle */
		fe_ssl_info info;
	    } ssl;
#endif
	} http;
	struct {
	    int gopher_tag;
	} gopher;
	struct {
	    char *user;
	    char *passwd;
	    fe_passwd pw;	/* Used while we are waiting for a userid/passwd pair. */
	    int had_passwd;     /* Set if we hav eto ask the user for a password */
	    realm rr;           /* If we've built a realm for this uid/passwd */
	} ftp;
	struct {
	    int fh;
	    int chunk;
	    int size;
	    int so_far;
	    char *fname;
	    int ofh;
	    unsigned last_modified;	/* from file */
	} file;
	struct {
	    char *cfile;
	} internal;
    } data;
#ifdef STBWEB
    struct
    {
        alarm_handler continue_fn;
	int had_error;
    } redial;
#endif
} access_item;


/***************************************************************************/

char *access_schemes[] = {
    "file",
#ifndef FILEONLY
    "http",
    "https",            /* pdh: added this */
    "gopher",
    "ftp",
    "icontype",
#endif

    NULL
    };

/***************************************************************************/

static void access_free_item(access_handle d);
static void access_unlink(access_handle h);
static void access_link(access_handle h);
static void access_reschedule(alarm_handler fn, access_handle d, int dt);

#ifndef FILEONLY
static os_error *gopher_set_file_type(char *fname, char tag, int *ftptr);
static void access_redirect_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url);
static access_complete_flags access_redirect_complete(void *h, int status, char *cfile, char *url);
static void access_http_dns_alarm(int at, void *h);
static void access_http_fetch_alarm(int at, void *h);
static void access_gopher_dns_alarm(int at, void *h);
static void access_gopher_fetch_alarm(int at, void *h);
static void access_ftp_dns_alarm(int at, void *h);
static void access_ftp_fetch_alarm(int at, void *h);
static os_error *access_http_fetch_start(access_handle d);
static void access_reschedule(alarm_handler fn, access_handle d, int dt);
static void access_http_fetch_done(access_handle d, http_status_args *si);
static os_error *gopher_set_file_type(char *fname, char tag, int *ftptr);
#endif

static void access_file_fetch_alarm(int at, void *h);

/***************************************************************************/

static cache_functions *cache = NULL;

/***************************************************************************/

#define AUTHORIZATION_HEADER		"Authorization"
#define PROXY_AUTHORIZATION_HEADER	"Proxy-Authorization"

access_handle access_pending_list;

#ifndef FILEONLY

static int access_done_flag;	/* For forground fetches */

static http_header_item user_agent_hdr = {
    NULL,
    "User-Agent",
#if 0
    PROGRAM_TITLE"/"VERSION_NUMBER SPECIAL_SUFFIX
#endif
};

#ifndef NO_LICENSEE
static http_header_item licence_hdr = {
    NULL,
    "X-Licensee",
    NULL
    };
#endif

static http_header_item content_type_hdr = {
    NULL,
    "Content-type",
    NULL
/*     "application/x-www-form-urlencoded" */
    };

static http_header_item accept_type_hdr = {
    NULL,
    "Accept",
    "text/html, image/gif, image/jpeg, image/pjpeg, image/png, */*; q=.2"
    };

static http_header_item accept_language_hdr = {
    NULL,
    "Accept-Language",
    NULL
    };

static http_header_item authenticate_hdr = {
    NULL,
    AUTHORIZATION_HEADER,
    NULL
    };

static http_header_item proxy_authenticate_hdr = {
    NULL,
    PROXY_AUTHORIZATION_HEADER,
    NULL
    };

static http_header_item referer_hdr = {
    NULL,
    "Referer",
    NULL
    };

static http_header_item from_hdr = {
    NULL,
    "From",
    NULL
    };

static http_header_item nocache_hdr = {
    NULL,
    "Pragma",
    "no-cache"
    };

#if SEND_HOST
static http_header_item host_hdr = {
    NULL,
    "Host",
    NULL
    };
#endif

#endif

/* ------------------------------------------------------------------------------------- */

#if defined(STBWEB)

/* ensure line isn't used at the moment generally hence the #if 0 */
# if 0

/* however it is used in a few places so has been moved and renamed to util.c */
#  define ensure_line() ensure_modem_line()
#  define access_do_redial(d,f)  0

# else

#ifdef CBPROJECT
int frontend_interface_state(void)
{
    int st;
    InetDial_Status( &st, NULL, NULL );
    return st == inetdial_ONLINE ? 1 : 0;
}
#endif

static void access_redial_alarm(int at, void *h);

static BOOL access_do_redial(access_handle d, alarm_handler continue_fn)
{
    /* are we already redialling */
    if (d->redial.continue_fn)
        return TRUE;

    /* is the interface down */
    if (frontend_interface_state() == fe_interface_DOWN && !d->redial.had_error)
    {
#ifdef CBPROJECT
        InetDial_Connect();
#else
        os_cli(PROGRAM_NAME"Redial");
#endif

        if (d->progress)
            d->progress(d->h, status_REDIALLING, -1, -1, 0, -1, NULL);

        d->redial.continue_fn = continue_fn;

        access_reschedule(access_redial_alarm, d, POLL_INTERVAL);

        return TRUE;
    }

    /* no - return normal error */
    return FALSE;
}

static void access_redial_alarm(int at, void *h)
{
    access_handle d = h;

    switch (frontend_interface_state())
    {
    case fe_interface_DOWN:
	if (d->progress)
	    d->progress(d->h, status_REDIALLING, -1, -1, 0, -1, NULL);
	access_reschedule(&access_redial_alarm, d, POLL_INTERVAL);
	break;

    case fe_interface_UP:
	if (d->progress)
	    d->progress(d->h, status_DNS, -1, -1, 0, -1, NULL);

	access_reschedule(d->redial.continue_fn, d, POLL_INTERVAL);
        d->redial.continue_fn = 0;
	break;

    case fe_interface_ERROR:
	ACCDBG(("Redial failed\n"));
#if 1
	if (d->complete)
	    d->complete(d->h, status_FAIL_REDIAL, NULL, d->url);

	access_done_flag = 1;

	access_unlink(d);
	access_free_item(d);
#else
	if (d->progress)
	    d->progress(d->h, status_DNS, -1, -1, 0, -1, NULL);

	access_reschedule(d->redial.continue_fn, d, POLL_INTERVAL);
        d->redial.continue_fn = 0;

	d->redial.had_error = TRUE;
#endif
	break;
    }
}

#  define ensure_line() NULL

# endif

#else

/* Fresco */

# define access_do_redial(d,f)  0
# define access_redial_alarm    0
# define ensure_line()		NULL

#endif

/* ------------------------------------------------------------------------------------- */

#ifndef FILEONLY
/*
Fancy DNS resolving

Note prefix is www (http), ftp (ftp), gopher (gopher)

If failure and try suffix and try prefix are both clear
  If has no dots on it then
    Set try suffix flag
    Set try prefix flag
    Append first suffix
    Append prefix

  Else If it has dots in it
    If it doesn't have the correct prefix then
      Set try prefix flag
      Append prefix
    Else
      fail

Else if try suffix is set
  If more suffices in list then
    Replace suffix with next suffix in list
  Else
    fail

Else
  fail
*/

static char *suffix_replace(char *name, const char *orig, const char *new)
{
    int len = strlen(name);
    int len_o = strlen(orig);
    int len_n = strlen(new);
    char *out;

    if (len_o > 0 && len > len_o && strcmp(name + len - len_o, orig) == 0)
    {
	if (len_o >= len_n)
	{
	    strcpy(name + len - len_o, new);
	    return name;
	}

	name[len - len_o] = 0;
    }

    out = strdup(name);
    mm_free(name);
    out = strcatx(out, new);

    return out;
}

static BOOL access_try_fancy_resolve(access_handle d, const char *prefix)
{
    ACCDBG(("access_try_fancy_resolve: d %p prefix %d/'%s' suffix %d/%d)\n", d, d->try_prefix, prefix, d->try_suffix, d->suffix_num));

    if (!d->try_suffix && !d->try_prefix)
    {
	if (strchr(d->url, '.') == NULL)
	{
	    /* if not dots then add prefix and suffix */
	    d->try_prefix = TRUE;
	    d->try_suffix = TRUE;
	}
	else if (strncasecomp(d->dest_host, prefix, strlen(prefix)) != 0)
	{
	    /* if not correct prefix try adding prefix */
	    d->try_prefix = TRUE;
	}
	else
	{
	    /* if dots and correct prefix then can do no more */
	    ACCDBG(("access_try_fancy_resolve: dots and correct prefix\n"));
	    return FALSE;
	}

	if (d->try_prefix)
	{
	    char *host = strdup(prefix);

	    host = strcatx(host, d->dest_host);

	    mm_free(d->dest_host);
	    d->dest_host = host;
	}
    }
    else if (!d->try_suffix)
    {
	/* if we had tried just new prefix then can do no more */
	ACCDBG(("access_try_fancy_resolve: already added prefix\n"));
	return FALSE;
    }

    if (d->try_suffix)
    {
	if (d->suffix_num < config_url_suffix[0])
	{
	    d->dest_host = suffix_replace(d->dest_host,
					  d->suffix_num ? (char *)config_url_suffix[d->suffix_num] : "",
					  (char *)config_url_suffix[d->suffix_num+1]);
	    d->suffix_num++;
	}
	else
	{
	    /* if tried all suffixes then can do no more */
	    ACCDBG(("access_try_fancy_resolve: tried all suffixes\n"));
	    return FALSE;
	}
    }

    return TRUE;
}
#endif

/* ------------------------------------------------------------------------------------- */

#ifndef FILEONLY

static int access_can_handle_file_type(int ft, int size)
{
    ACCDBG(("access_can_handle_file_type: ft %03x size %d\n", ft, size));

    if (ft == FILETYPE_HTML || ft == FILETYPE_GOPHER || ft == FILETYPE_TEXT ||
        image_type_test(ft))
    {
	ACCDBG(("access_can_handle_file_type: standard type\n"));
	return 0;
    }

    if (frontend_plugin_handle_file_type(ft))
    {
#ifdef STBWEB
	if (!plugin_is_there_enough_memory(ft, size))
	{
	    ACCDBG(("access_can_handle_file_type: not enough plugin memory\n"));
	    return status_FAIL_LOCAL;
	}
#endif

	ACCDBG(("access_can_handle_file_type: OK plugin file type\n"));
	return 0;
    }

    if (frontend_can_handle_file_type(ft))
    {
	ACCDBG(("access_can_handle_file_type: OK frontend file type\n"));
	return 0;
    }

    ACCDBG(("access_can_handle_file_type: bad file type\n"));
    return status_BAD_FILE_TYPE;
}

static char *access_host_name_only(char *url)
{
    char *p, *q;
    char *scheme, *netloc, *path, *params, *query, *fragment;

    url_parse(url, &scheme, &netloc, &path, &params, &query, &fragment);

    p = strchr(netloc, '@');
    p = p ? p+1 : netloc;
    q = strchr(p, ':');
    if (q)
	*q = 0;

    q = strdup(p);

    url_free_parts(scheme, netloc, path, params, query, fragment);

    return q;
}

#endif

static void access_free_item(access_handle d)
{
    ACCDBG(("acc%p: free_item dest_host '%s' url '%s'\n", d, strsafe(d->dest_host), d->url));

    FREE(d->dest_host);
    FREE(d->request_string);
    FREE(d->url);
    FREE(d->referer);
    FREE(d->ofile);

    switch(d->access_type)
    {
    case access_type_HTTP:
	FREE(d->data.http.body_file);
#if SSL_UI
	FREE(d->data.http.ssl.info.issuer);
	FREE(d->data.http.ssl.info.subject);
	FREE(d->data.http.ssl.info.cipher);
#endif
	break;
    case access_type_FTP:
	FREE(d->data.ftp.user);
	FREE(d->data.ftp.passwd);
	break;
    case access_type_FILE:
	FREE(d->data.file.fname);
	break;
    case access_type_INTERNAL:
	FREE(d->data.internal.cfile);
	break;
    }

    mm_free(d);
    ACCDBG(("access; free finished\n"));
}

/* Unlinks but does not free */
static void access_unlink(access_handle h)
{
    if (h->prev)
	h->prev->next = h->next;
    else
	access_pending_list = h->next;

    if (h->next)
	h->next->prev = h->prev;
}

static void access_link(access_handle h)
{
    h->next = access_pending_list;
    h->prev = NULL;

    if (h->next)
	h->next->prev = h;

    access_pending_list = h;
}

static os_error *access_write_wimp_icon(char *name, char *fname)
{
    os_error *ep = NULL;
    os_regset r;
    sprite_area *area;
    sprite_header *sph;
    sprite_id id;
    ACCDBGN(( "Saving icon '%s' into '%s'\n", name, fname));
    ep = os_swix(Wimp_BaseOfSprites, &r);

    area = (sprite_area *) (long) r.r[1];

    id.tag = sprite_id_name;
    id.s.name = name;

    ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);

    if (ep)
    {
	area = (sprite_area *) (long) r.r[0];
	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
    }

    if (ep)
    {
	id.s.name = "small_xxx";
	ep = sprite_select_rp(area, &(id), (sprite_ptr *) &sph);
    }

    if (ep == 0)
    {
	int fh;
	os_gbpbstr gpb;
	sprite_area sa;

	r.r[0] = 0x8f;
	r.r[1] = (int) (long) fname;

	ep = os_find(&r);

	fh = ep ? 0 : r.r[0];

	if (ep == 0)
	{
	    sa.number = 1;
	    sa.sproff = 16;
	    sa.freeoff = sph->next + 16;

	    gpb.action = 2;
	    gpb.file_handle = fh;
	    gpb.data_addr = &sa.number;
	    gpb.number = sizeof(sa) - sizeof(sa.size);

	    ep = os_gbpb(&gpb);
	}

	if (ep == 0)
	{
	    gpb.action = 2;
	    gpb.file_handle = fh;
	    gpb.data_addr = sph;
	    gpb.number = sph->next;

	    ep = os_gbpb(&gpb);
	}

	if (fh)
	{
	    os_error *ep2;

	    r.r[0] = 0;
	    r.r[1] = fh;

	    ep2 = os_find(&r);

	    if (ep == 0)
		ep = ep2;
	}

	if (ep == 0)
	    set_file_type(fname, FILETYPE_SPRITE);
    }

    return ep;
}

/* ------------------------------------------------------------ */

static int access_progress_flush(void *handle, const char *cfile, const char *url, access_progress_fn progress)
{
    int fh, size;

    if (!progress)
	return TRUE;

    if ((fh = ro_fopen(cfile, RO_OPEN_READ)) == 0)
	return FALSE;

    ACCDBG(("access_progress_flush: file %s fh %d\n", cfile, fh));

    size = ro_get_extent(fh);
    progress(handle, status_GETTING_BODY, size, size, fh, file_type_real(cfile), (char *)url);

    ro_fclose(fh);
    ACCDBG(("access_progress_flush: close %d error '%s'\n", fh, ro_ferror()));

    return TRUE;
}

/* ------------------------------------------------------------ */

static access_progress_fn lastfn;

static void access_redirect_progress(void *h, int status, int size, int so_far, int fh, int ftype, char *url)
{
    access_handle d = (access_handle) h;

    lastfn = d->progress;	/* defeat armcc 4.71 bug */

    ACCDBG(("acc%p: access_redirect_progress(status=%d) realacc=%p\n", d, status, d->h ));

    if (d->progress)
	d->progress(d->h, status, size, so_far, fh, ftype, url);
}

static access_complete_flags access_redirect_complete(void *h, int status, char *cfile, char *url)
{
    access_complete_flags cache_it;

    access_handle d = (access_handle) h;

    ACCDBG(("acc%p: access_redirect_complete(status=%d) realacc=%p\n", d, status, d->h ));

    ACCDBG(("access_redirect_complete: ah%p\n", d));

    cache_it = d->complete(d->h, status, cfile, url); /* Pass up the real URL, not the one they requested */
    d->redirect = NULL;

    /* Optionally update cookies and users if new ones were added */
    /* Check here for anything that arrived before the redirect */
    if (config_cookie_uptodate && d->data.http.cookie_count)
        cookie_write_file(config_cookie_file);

    if (d->data.http.had_passwd && config_passwords_uptodate)
        auth_write_realms(config_auth_file, config_auth_file_crypt ? auth_passwd_UUCODE : auth_passwd_PLAIN);

    access_unlink(d);
    ACCDBG(("acc%p: redirect_complete calls free_item\n",d));
    access_free_item(d);

    return cache_it;
}

#ifndef FILEONLY

static os_error *gopher_set_file_type(char *fname, char tag, int *ftptr)
{
    os_filestr osf;
    int ft;

    switch (tag)
    {
    case '0':
	ft = FILETYPE_TEXT;
	break;
    case '1':
    case '7':
	ft = FILETYPE_GOPHER;
	break;
    case 'h':
	ft = FILETYPE_HTML;
	break;
    default:
	ft = FILETYPE_DATA;
	break;
    }

    osf.action = 18;
    osf.name = fname;
    osf.loadaddr = ft;

    if (ftptr)
	*ftptr = ft;

    return os_file(&osf);
}

static void access_gopher_close(void *handle, int flags)
{
    gopher_close_args goc;

    goc.in.handle = handle;
    goc.in.flags = flags;

    os_swix(WebGopher_Close, (os_regset *) &goc);
}

static os_error *access_gopher_fetch_start(access_handle d)
{
    gopher_open_args goo;
    os_error *ep = NULL;

    goo.in.addr = &d->addr;
    goo.in.object = d->request_string;
    goo.in.fname = d->ofile ? d->ofile : cache->scrapfile_name();

    ep = os_swix(WebGopher_Open, (os_regset *) &goo);

    d->transport_handle = goo.out.handle;

    return ep;
}

/* ------------------------------------------------------------ */

static void access_ftp_close(void *handle, int flags)
{
    ftp_close_args ftpc;

    ftpc.in.session = handle;
    ftpc.in.flags = flags;

    os_swix(WebFTP_Close, (os_regset *) &ftpc);
}

static os_error *access_ftp_fetch_start(access_handle d)
{
    ftp_open_args ftpo;
    os_error *ep = NULL;
    BOOL put = (d->flags & access_UPLOAD);

    ACCDBGN(( "Opening connection: host=0x%08x, port=0x%04x\n", (int) d->addr.sin_addr.s_addr, d->addr.sin_port));
    ftpo.in.addr = &d->addr;
    ftpo.in.user_name = d->data.ftp.user;
    ftpo.in.passwd = d->data.ftp.passwd;
    ftpo.in.acct = NULL;
    ftpo.in.path = d->request_string;
    ftpo.in.local_name = d->ofile ? d->ofile : cache->scrapfile_name();
    ftpo.in.flags = put ? webftp_open_ftpcmd : 0;
    ftpo.in.maxidle = FTP_IDLE_TIME;
    ftpo.in.ftp_cmd = put ? FTP_CMD_PUT : FTP_CMD_GET;

    ACCDBG(( "WebFTP_Open(%s)\n", put ? "PUT" : "GET" ));

    ep = os_swix(WebFTP_Open, (os_regset *) &ftpo);

    d->transport_handle = ftpo.out.session;

    return ep;
}

/* ------------------------------------------------------------ */

static void access_http_close(void *handle, int flags)
{
    http_close_args httpc;

    httpc.in.handle = handle;
    httpc.in.flags = flags;

    os_swix(HTTP_Close, (os_regset *) &httpc);
}


static os_error *access_http_fetch_start(access_handle d)
{
    http_header_item *hlist = NULL;
    os_error *ep = NULL;
    http_open_args httpo;

    user_agent_hdr.value = ua_name;
    user_agent_hdr.next = hlist;
    hlist = &user_agent_hdr;

    accept_type_hdr.next = hlist;
    hlist = &accept_type_hdr;

    if (d->data.http.body_file)
    {
        content_type_hdr.value = d->data.http.body_type
                                   ? d->data.http.body_type
                                   : "application/x-www-form-urlencoded";
	content_type_hdr.next = hlist;
	hlist = &content_type_hdr;
    }

    if (d->referer)
    {
	referer_hdr.next = hlist;
	referer_hdr.value = d->referer;
	hlist = &referer_hdr;
    }
#ifndef NO_LICENSEE
    if (licensee_string)
    {
	licence_hdr.next = hlist;
	licence_hdr.value = licensee_string;
	hlist = &licence_hdr;
    }
#endif
    if (config_email_addr)
    {
	from_hdr.next = hlist;
	from_hdr.value = config_email_addr;
	hlist = &from_hdr;
    }
    if ( config_language_preference )
    {
        accept_language_hdr.next = hlist;
        accept_language_hdr.value = config_language_preference;
        hlist = &accept_language_hdr;
    }

    /* normal authentication */
    if ((authenticate_hdr.value = auth_lookup_string(d->url)) != NULL)
    {
	authenticate_hdr.next = hlist;

	hlist = &authenticate_hdr;
	d->data.http.had_auth = 1;
    }

    /* proxy authentication */
    if ((d->flags & access_PROXY) && (proxy_authenticate_hdr.value = auth_lookup_string(d->dest_host)) != NULL)
    {
	proxy_authenticate_hdr.next = hlist;

	hlist = &proxy_authenticate_hdr;
	d->data.http.had_proxy_auth = 1;
    }

    /* pragmas */
    if (d->flags & access_NOCACHE)
    {
	nocache_hdr.next = hlist;
	hlist = &nocache_hdr;
    }

    /* cookies */
    if (config_cookie_enable)
        hlist = cookie_add_headers(hlist, d->url, d->flags & access_SECURE ? 1 : 0);

#if SEND_HOST
    /* Host header - we need ts to correct results out of some hosts these days */
    {
	host_hdr.next = hlist;

	mm_free(host_hdr.value);
	if (d->flags & access_PROXY)
	    host_hdr.value = access_host_name_only(d->url);
	else
	    host_hdr.value = strdup(d->dest_host);

	hlist = &host_hdr;
    }
#endif

    ACCDBG(("Opening connection to host 0x%08x, port 0x%04x, request '%s', handle %p\n",
	    (int) d->addr.sin_addr.s_addr, d->addr.sin_port, d->request_string, d));

    httpo.in.addr = &d->addr;
    httpo.in.object = d->request_string;
    httpo.in.headers = hlist;
    httpo.in.fname = d->ofile ? d->ofile : cache->scrapfile_name();
    httpo.in.bname = d->data.http.body_file;
    httpo.in.flags = 0;
    if ( d->flags & access_SECURE)
    {
	httpo.in.flags |= http_open_flags_SECURE;

	if ( d->flags & access_PROXY )
	    httpo.in.flags |= http_open_flags_TUNNEL;
    }
    if ( d->flags & (access_PRIORITY | access_MAX_PRIORITY))
        httpo.in.flags |= http_open_flags_PRIORITY;
    if ( d->flags & access_IMAGE )
        httpo.in.flags |= http_open_flags_IMAGE;

#if DEBUG
    {
	http_header_item *h;
	ACCDBG(("HTTP_Open flags %x\n", httpo.in.flags));
	for (h = hlist; h; h = h->next)
	{
	    ACCDBGN(( " %s: %s\n", h->key, h->value));
	}
    }
#endif

    ep = os_swix(HTTP_Open, (os_regset *) &httpo);

    ACCDBG(("HTTP_Open returned %p\n", ep ));

#if SEND_HOST
    /* pdh: HTTP module takes copies of these, obviously, so we can free what
     * we just strdup'd
     */
    mm_free( host_hdr.value );
    host_hdr.value = NULL;
#endif

    mm_free(proxy_authenticate_hdr.value);
    proxy_authenticate_hdr.value = NULL;

    mm_free(authenticate_hdr.value);
    authenticate_hdr.value = NULL;

    cookie_free_headers();

    if (ep == NULL)
	d->transport_handle = httpo.out.handle;

    return ep;
}

#endif /*ndef FILEONLY */

static void access_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;

    d->next_fn(at, h);
}

static void access_reschedule(alarm_handler fn, access_handle d, int dt)
{
    if (d->flags & access_PENDING_FREE)
    {
	ACCDBG(("acc%p: reschedule calls free_item\n", d ));
	access_free_item(d);
    }
    else
    {
	d->next_fn = fn;

	if ((d->flags & access_FORGROUND) == 0)
	    alarm_set(alarm_timenow() + dt, &access_alarm, d);
    }
}

#ifndef FILEONLY

static void access_http_dns_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    int status;
    os_error *ep;

    ACCDBG(("HTTP resolving name '%s'\n", d->dest_host));

    ep = ensure_line();
    if (ep == NULL)
	ep = netloc_resolve(d->dest_host, (d->flags & access_SECURE) ? HTTPS_PORT : HTTP_PORT,
			&status, &d->addr);
    if (ep == NULL)
    {
    }
    else if (access_do_redial(d, access_http_dns_alarm))
    {
        return;
    }
    else if (access_try_fancy_resolve(d, "www."))
    {
	/* will try again with different host - clear header */
	ep = NULL;
    }
    else
    {
	ACCDBG(("HTTP resolve failed, error:%s\n", ep->errmess));

	if (d->complete)
	    d->complete(d->h,status_FAIL_DNS, ep->errmess, d->url);

	access_done_flag = 1;

	ACCDBG(("acc%p: dns_alarm calls free (DNS failed)\n",d));
	access_unlink(d);
	access_free_item(d);

	return;
    }

    if (status == 0)
    {
	ACCDBG(("HTTP resolve OK, starting fetch\n"));

	ep = ensure_line();
	if (ep == NULL)
	    ep = access_http_fetch_start(d);
	if (ep == NULL)
	{
	    ACCDBG(("HTTP fetch started OK.\n"));

	    if (d->progress)
		d->progress(d->h, status_CONNECTING, -1, -1, 0, -1, NULL);
	    access_reschedule(&access_http_fetch_alarm, d, POLL_INTERVAL);
	}
        else if (access_do_redial(d, access_http_dns_alarm))
        {
            return;
        }
	else
	{
	    ACCDBG(( "HTTP: Failed connection with error '%s'\n", ep->errmess));

	    if (d->complete)
		d->complete(d->h, status_FAIL_CONNECT, ep->errmess, d->url);

	    access_done_flag = 1;

	    access_unlink(d);
	    access_free_item(d);
	}
    }
    else
    {
	ACCDBG(("HTTP DNS still going on\n"));

	if (d->progress)
	    d->progress(d->h, status_DNS, -1, -1, 0, -1, NULL);
	access_reschedule(&access_http_dns_alarm, d, POLL_INTERVAL);
    }
}

static void access_http_auth_rerequest(access_handle d, realm rr, auth_requester req)
{
    switch (req)
    {
    case auth_req_WWW:
    {
	char *scheme, *netloc, *path, *params, *query, *fragment;
	char *prefix;

	ACCDBG(("access: auth rerequest d=%p url '%s'\n", d, d->url));

	url_parse(d->url, &scheme, &netloc, &path, &params, &query, &fragment);

	prefix = strrchr(path, '/');
	if (prefix)
	    prefix[1] = 0;

	prefix = url_unparse(scheme, netloc, path, 0, 0, 0);
	url_free_parts(scheme, netloc, path, params, query, fragment);

	auth_add(prefix, rr);

	mm_free(prefix);
	break;
    }

    case auth_req_PROXY:
	ACCDBG(("access: auth proxy rerequest d=%p url '%s'\n", d, d->dest_host));
	auth_add(d->dest_host, rr);
	break;
    }

    access_http_close(d->transport_handle, http_close_DELETE_FILE);	/* Don't delete body; we still need it */
    d->transport_handle = NULL;

    access_url(d->url, d->flags, d->ofile, d->data.http.body_file,
               d->data.http.body_type,
               d->url,
	       &access_redirect_progress, &access_redirect_complete, d, &(d->redirect));
}

static int access_http_find_realm(http_header_item *list, char **realm_name, char **realm_type, auth_requester requester)
{
    char *r_name = NULL, *r_type = NULL;

    for ( ; list; list = list->next)
    {
	if ((requester == auth_req_WWW && strcasecomp("WWW-AUTHENTICATE", list->key) == 0) ||
	    (requester == auth_req_PROXY && strcasecomp("PROXY-AUTHENTICATE", list->key) == 0))
	{
	    char *temp;
	    char *type;
	    char *name;

            /* parse authenticate line */
	    type = temp = strdup(list->value);

	    while (isspace(*type))
		type++;

	    type = strtok(type, " \t");

	    name = strtok(NULL, "=");
	    name = strtok(NULL, "\"");

	    ACCDBGN(( "%s type='%s', realm='%s'\n", list->key, type, name));
            /* free last saved name/type */
            mm_free(r_name);
            mm_free(r_type);

            /* save this name/type, use none here as it makes auth easier */
	    r_name = strdup(name ? name : "<none>");
	    r_type = strdup(type);

            /* free containing string */
	    mm_free(temp);

            /* if we support this type then go to finish */
            if (auth_supported(type))
                break;
	}
    }

    /* write out what we have found -
     * will either be a supported authentication type
     * or else the last header found
     */

    *realm_name = r_name;
    *realm_type = r_type;

    return r_type != NULL;
}

static void access_http_passwd_callback(fe_passwd pw, void *handle, char *user, char *password)
{
    access_handle d = (access_handle) handle;
    realm rr = NULL;
    http_status_args si;
    char *realm_name, *realm_type;
    os_error *ep;

    ACCDBG(("access: passwd callback d=%p\n", handle));

    d->data.http.pw = NULL;

    si.in.handle = d->transport_handle;

    ep = os_swix(HTTP_Status, (os_regset *) &si);
    if (ep)
    {
	frontend_complain(ep);
	memset(&si, 0, sizeof(si));
	si.out.status = status_FAIL_REQUEST;
	access_http_fetch_done(d, &si);
	return;
    }

    if (user)
    {
	access_http_find_realm(si.out.headers, &realm_name, &realm_type, si.out.rc == 401 ? auth_req_WWW : auth_req_PROXY);

	rr = auth_add_realm(realm_name, realm_type, user, password);
	mm_free(realm_name);
	mm_free(realm_type);
    }

    if (rr == NULL)
    {
        /* only give error if the find_realm failed, not if the user cancelled */
        if (user)
	    frontend_complain(makeerror(ERR_BAD_AUTH));

	access_http_fetch_done(d, &si);
    }
    else
    {
        d->data.http.had_passwd = 1;        /* flag that we have added a new passwd to the list */
	access_http_auth_rerequest(d, rr, si.out.rc == 401 ? auth_req_WWW : auth_req_PROXY);
    }
}

#if SSL_UI

/* This is called by the frontend. If the user OK'd the transaction
 * then the verified field is set to Yes. If they cancelled then it
 * is left at No. */

static void access_http_ssl_callback(void *handle, BOOL verified)
{
    access_handle d = handle;

    d->data.http.ssl.fe = 0;

    if (verified)
    {
	/* If the user said OK then just continue from where we were */
	/* But what happens if we'd timed out */
	access_http_fetch_alarm(0, d);
    }
    else
    {
	http_status_args si;

	memset(&si, 0, sizeof(si));
	si.out.status = status_FAIL_VERIFY;

	access_http_fetch_done(d, &si);
    }
}
#endif

static void access_http_fetch_done(access_handle d, http_status_args *si)
{
    int cache_it;
    char *cfile;
    MemCheck_checking checking;

    /* Time to stop */
    ACCDBG(( "acc%p: Transfer complete for %s status %d rc %d \n", d, d->url, si->out.status, si->out.rc));

    MemCheck_RegisterMiscBlock(si->out.fname, 256);
    cfile = strdup(si->out.fname);
    MemCheck_UnRegisterMiscBlock(si->out.fname);

    /* send the rest of the data to the client */
    if (si->out.status == status_COMPLETED_FILE && d->progress)
    {
	ACCDBGN(("access: final size %d bytes (est %d bytes) - flushing\n", si->out.data_so_far, si->out.data_size));
	d->progress(d->h, status_GETTING_BODY, si->out.data_so_far, si->out.data_so_far, si->out.ro_fh, d->ftype, d->url);
	ACCDBGN(("access: flushed\n"));
    }

    /* Temporary insertion in case the url is re-requested inside the completed call (e.g. for a GIF) */
    cache->insert(d->url, cfile, cache_flag_OURS | (si->out.data_size == -1 ? cache_flag_IGNORE_SIZE : 0));

    ACCDBGN(("access: inserted (and locked) %s\n", cfile));

#if LOCK_FILES
    /* lock the file before closing it */
    file_lock(cfile, TRUE);
#endif

    /* The http close does not need to delete the file as we have already if it was removed from the cache */
    access_http_close(d->transport_handle, http_close_DELETE_BODY );

    ACCDBGN(("access: closed\n"));

    /* What? A bug, in some Netscape software? Surely not.
     * If we've got an error, but we were in a server-push situation and
     * the previous part finished fine, kid ourselves that the previous part
     * was the final one. Unlike most Netscape bugs, they've posted the source
     * code for this one on the Web:
     *      http:/www.netscape.com/assist/net_sites/mozilla/doit.c
     */

    if ( si->out.status != status_COMPLETED_FILE
         && d->hadwholepart )
        si->out.status = status_COMPLETED_FILE;

    if (d->complete)
	cache_it = d->complete(d->h,
			       (d->flags & access_MUST_BE_FOUND) && si->out.rc != 200 ? status_FAIL_FOUND : si->out.status,
			       si->out.status == status_COMPLETED_FILE ? cfile : NULL,
			       d->url);
    else
	cache_it = 0;

    access_done_flag = 1;

#if LOCK_FILES
    /* unlock the file unless client asked for it to be lept locked */
    if ((cache_it & access_LOCK) == 0)
    {
	ACCDBGN(("access: unlocked '%s'\n", cfile));
	file_lock(cfile, FALSE);
    }
#endif

    if (si->out.status == status_COMPLETED_FILE && si->out.rc == 200 )
    {
	if ( cache_it & access_KEEP )
	    cache->keep(d->url);
    }

    if (cache_it & access_OURS)
	cache->not_ours(cfile);

    /* It was put in the cache before the completed call but we take it out if we don't need it */
    if ((si->out.status != status_COMPLETED_FILE) ||
	(si->out.rc != 200) ||
	((cache_it & access_CACHE) == 0) )
    {
	cache->remove(d->url);
    }
    else
    {   /* might have to reread size at this point */
        if (si->out.data_size == -1 && cache->update_size)
            cache->update_size(cfile);

	if (cache->header_info)
	    cache->header_info(d->url, d->data.http.date, d->data.http.last_modified, d->data.http.expires);
    }

    mm_free(cfile);

    /* Optionally update cookies and users if new ones were added */
    if (config_cookie_uptodate && d->data.http.cookie_count)
        cookie_write_file(config_cookie_file);

    if (d->data.http.had_passwd && config_passwords_uptodate)
        auth_write_realms(config_auth_file, config_auth_file_crypt ? auth_passwd_UUCODE : auth_passwd_PLAIN);

    access_unlink(d);
    access_free_item(d);
}

#endif /*ndef FILEONLY */

#ifdef MemCheck_MEMCHECK
static void memcheck_register_list(http_header_item *list)
{
    MemCheck_checking checking = MemCheck_SetChecking(0, 0);

    for (; list; list = list->next)
    {
	MemCheck_RegisterMiscBlock(list, sizeof(*list));
	MemCheck_RegisterMiscBlock(list->key, strlen(list->key)+1);
	MemCheck_RegisterMiscBlock(list->value, strlen(list->value)+1);
    }

    MemCheck_RestoreChecking(checking);
}

static void memcheck_unregister_list(http_header_item *list)
{
    http_header_item *next = NULL;

    for (; list; list = next)
    {
	MemCheck_UnRegisterMiscBlock(list->key);
	MemCheck_UnRegisterMiscBlock(list->value);
	next = list->next;
	MemCheck_UnRegisterMiscBlock(list);
    }
}
#else
#define memcheck_register_list(a)
#define memcheck_unregister_list(a)
#endif

#ifndef FILEONLY

static void access_http_fetch_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    http_status_args si;
    os_error *ep;

    si.in.handle = d->transport_handle;

    ep = os_swix(HTTP_Status, (os_regset *) &si);

    ACCDBG(("HTTP status %d, handle %p, rc %d\n", si.out.status, d, si.out.rc));

    if (ep)
    {
	memset(&si, 0, sizeof(si));
#ifdef STBWEB
	si.out.status = status_FAIL_LOCAL;
#else
	si.out.status = status_FAIL_REQUEST;
	frontend_complain(ep);
#endif
	access_http_fetch_done(d, &si);
	return;
    }

    if ((si.out.status >= status_GETTING_BODY) && (d->ft_is_set == 0) )
    {
	http_setfiletype_args httpft;

	httpft.in.handle = d->transport_handle;
	os_swix(HTTP_SetFileType, (os_regset *) &httpft);

        ACCDBG(("HTTP set filetype %d\n", httpft.out.ftype ));

	d->ftype = httpft.out.ftype;
	d->ft_is_set = 1;

    	si.in.handle = d->transport_handle;
	os_swix(HTTP_Status, (os_regset*) &si);		/* fh may have changed */

	/* this fixes a problem where a file is typed as octet-stream or
	 * something unknown rather than its real type but it has an extension
	 */
	if (d->ftype == 0xffd)
	{
	    char *slash = strrchr(d->url, '/');
	    char *dot = strrchr(d->url, '.');
	    int ft = -1;

	    if (dot && (slash == NULL || slash < dot))
		ft = suffix_to_file_type(dot+1);

	    if (ft != -1)
	    {
		d->ftype = ft;
		set_file_type(si.out.fname, ft);
	    }
	}

	if (d->flags & access_CHECK_FILE_TYPE)
	{
	    int s = access_can_handle_file_type(d->ftype, si.out.data_size);
	    if (s)
		si.out.status = s;
	}
    }

#if SSL_UI
    /* check the fake SSL headers returned */
    if (si.out.status > status_AUTHENTICATING &&
	(d->flags & access_SECURE) &&
	!d->data.http.ssl.checked_headers)
    {
	http_header_item *list;

	d->data.http.ssl.checked_headers = TRUE;
	d->data.http.ssl.info.verified = -1;

	ACCDBG(("access_http_fetch_alarm: checking SSL headers\n"));

	for (list = si.out.headers; list; list = list->next)
	{
 	    ACCDBG(("%s: %s\n", list->key, list->value));

	    if (strcasecomp(SSL_HEADER_VERIFIED, list->key) == 0)
		d->data.http.ssl.info.verified = strcasecomp(SSL_HEADER_YES, list->value) == 0;
	    else if (strcasecomp(SSL_HEADER_CERT_ISSUER, list->key) == 0)
		d->data.http.ssl.info.issuer = strdup(list->value);
	    else if (strcasecomp(SSL_HEADER_CERT_SUBJECT, list->key) == 0)
		d->data.http.ssl.info.subject = strdup(list->value);
	    else if (strcasecomp(SSL_HEADER_CIPHER, list->key) == 0)
		d->data.http.ssl.info.cipher = strdup(list->value);
	}

	if (d->data.http.ssl.info.verified == 0)
	{
	    d->data.http.ssl.fe = frontend_ssl_raise(access_http_ssl_callback, &d->data.http.ssl.info, d);
	    return;
	}
    }
#endif

    if ( (si.out.status >= status_COMPLETED_FILE)
         && (si.out.status != status_COMPLETED_PART) )
    {
	http_header_item *list;
	BOOL done = TRUE;

	memcheck_register_list(si.out.headers);

#if DUMP_HEADERS
	usrtrc("\nHeaders for 0x%x '%s'\n", d->flags, d->url);
	for (list = si.out.headers; list; list = list->next)
	{
 	    usrtrc("%s: %s\n", list->key, list->value);
	}
	usrtrc("\n");
#endif
	if ((si.out.rc / 100) == 3)
	{
	    char *new_url = NULL;

	    for (list = si.out.headers; list; list = list->next)
	    {
		if (strcasecomp("SET-COOKIE", list->key) == 0)
                {
		    if (config_cookie_enable)
		    {
			cookie_received_header(list->value, d->url);
			d->data.http.cookie_count++;
		    }
		}
		else if (strcasecomp("LOCATION", list->key) == 0)
		{
		    if (new_url == NULL)
			new_url = url_join(d->url, list->value);
		}
	    }

            memcheck_unregister_list(si.out.headers);

	    /* don't do the location till finished scanning in case there are any cookies */
	    if (new_url)
	    {
		os_error *ep;

		/* If we use Netscape posting we delete the body and do a GET */
		access_http_close(d->transport_handle,
				  (config_broken_formpost ?
				   http_close_DELETE_FILE | http_close_DELETE_BODY :
				   http_close_DELETE_FILE) );
		d->transport_handle = NULL;

		ep = access_url(new_url, d->flags, d->ofile,
				config_broken_formpost ? NULL : d->data.http.body_file,
				config_broken_formpost ? NULL : d->data.http.body_type,
				d->url,
				&access_redirect_progress, &access_redirect_complete,
				d, &(d->redirect));

		/* punt moved to backend_open_url so need one here in case someone redirects us to
		 * an unknown scheme
		 */
		if (ep && ep->errnum == ANTWEB_ERROR_BASE + ERR_USED_HELPER)
		    frontend_url_punt(NULL, new_url, d->data.http.body_file);

		mm_free(new_url);
		done = FALSE;
            }
        }
        else
        {
	    for (list = si.out.headers; list; list = list->next)
	    {
		if (strcasecomp("SET-COOKIE", list->key) == 0)
                {
		    if (config_cookie_enable)
		    {
			cookie_received_header(list->value, d->url);
			d->data.http.cookie_count++;
		    }
		}
		else if (strcasecomp("LAST-MODIFIED", list->key) == 0)
		{
		    d->data.http.last_modified = (unsigned)HTParseTime(list->value);
		}
		else if (strcasecomp("EXPIRES", list->key) == 0)
		{
		    d->data.http.expires = (unsigned)HTParseTime(list->value);
		}
		else if (strcasecomp("DATE", list->key) == 0)
		{
		    d->data.http.date = (unsigned)HTParseTime(list->value);
		}
	    }


	    /* 401 is UNAUTHORIZED, 407 is PROXY-AUTHENTICATE */
	    if (si.out.rc == 401 || si.out.rc == 407)
	    {
	        char *type;
	        char *realm_name;

	    /* Authentication failed.  There are three cases: a) we
	     * have no idea about this realm and we need a new realm
	     * entry, a new mapping to the realm from this URL and
	     * user and passwd data, b) we know about the realm but we
	     * did not know that this URL is in that realm; we need to
	     * add the mapping and c) we think we know both URL and
	     * realm but it seems to be wrong.
	     */

	        if (access_http_find_realm(si.out.headers, &realm_name, &type, si.out.rc == 401 ? auth_req_WWW : auth_req_PROXY))
	        {
		    realm rr;

		    rr = auth_lookup_realm(realm_name);

		    mm_free(type);

		    if (rr)
		    {
		        if ((si.out.rc == 401 && d->data.http.had_auth) || (si.out.rc == 407 && d->data.http.had_proxy_auth))
		        {
#ifndef STBWEB
			    /* removed this from NCFresco as the new
                               error reporting scheme causes the
                               password box to not be displayed */
			    frontend_complain(makeerror(ERR_BAD_PASSWD));
#endif
			    rr = NULL;
		        }
		    }

		    if (rr == NULL)
		    {
		        char *hname = si.out.rc == 401 ? access_host_name_only(d->url) : strdup(d->dest_host);
		        d->data.http.pw = frontend_passwd_raise(access_http_passwd_callback,
							    d, NULL, realm_name, hname);
		        mm_free(hname);
		    }
		    else
		    {
		        access_http_auth_rerequest(d, rr, si.out.rc == 401 ? auth_req_WWW : auth_req_PROXY);
		    }

		    mm_free(realm_name);

    		    done = FALSE;
                }

	        /* If we can't authenticate, just show the error to the user */
	    }

            memcheck_unregister_list(si.out.headers);
	}

	if (done)
	    access_http_fetch_done(d, &si);
    }
    else
    {
        /* pdh: removed the rc=2xx condition, 'cos long 404 pages weren't
         * being displayed. (e.g. www.infoseek.com, all of whose pages are
         * 404 in some kind of lumpen attempt at cache evasion)
         *
         * pdh: me again, reinstated condition (it meant bits of 302 redirect
         * pages turned up in the main document) but rephrased it as rc != 3xx
	 *
	 * sjm: not readable if it is a authentication challenge to stop bits of challenge
	 * pages getting inserted into the real page.
	 */
	int readable = d->ft_is_set && ((si.out.rc / 100) != 3) && si.out.rc != 401 && si.out.rc != 407;

        ACCDBG(("Calling progress function (st=%d data=%d/%d rd=%d ft=%d)\n",
                si.out.status, si.out.data_so_far, si.out.data_size, readable,
                d->ftype ));

	/* Keep going */
	if ( d->progress &&
	     (!d->hadwholepart || si.out.status==status_COMPLETED_PART) )
	    d->progress(d->h,
	                d->hadwholepart ? status_COMPLETED_PART : si.out.status,
	                d->hadwholepart ? si.out.data_so_far : si.out.data_size,
	                si.out.data_so_far, si.out.ro_fh,
	                readable ? d->ftype : -1, d->url );

        if ( si.out.status == status_COMPLETED_PART )
        {
            http_completedpart_args cpa;

            d->hadwholepart = TRUE;

            cpa.in.handle = d->transport_handle;
            cpa.in.flags = 0;
            cpa.in.newfname = NULL;

            os_swix(HTTP_CompletedPart, (os_regset *) &cpa);

            ACCDBG(("Completed a part!\n"));
        }

	access_reschedule(&access_http_fetch_alarm, d, POLL_INTERVAL);
    }
}

/* ------------------------------------------------------------ */

static void access_gopher_dns_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    int status;
    os_error *ep;

    ep = ensure_line();
    if (ep == NULL)
	ep = netloc_resolve(d->dest_host, 70, &status, &d->addr);
    if (ep == NULL)
    {
    }
    else if (access_do_redial(d, access_gopher_dns_alarm))
    {
	return;
    }
    else if (access_try_fancy_resolve(d, "gopher."))
    {
	/* will try again with different host - clear header */
	ep = NULL;
    }
    else
    {
	ACCDBG(("Gopher resolve failed, error:%s\n", ep->errmess));

	if (d->complete)
	    d->complete(d->h,status_FAIL_DNS, ep->errmess, d->url);

	access_done_flag = 1;

	access_unlink(d);
	access_free_item(d);

	return;
    }

    if (status == 0)
    {
	ep = ensure_line();
	if (ep == NULL)
	    ep = access_gopher_fetch_start(d);
	if (ep == NULL)
	{
	    if (d->progress)
		d->progress(d->h, status_CONNECTING, -1, -1, 0, -1, NULL);
	    access_reschedule(&access_gopher_fetch_alarm, d, POLL_INTERVAL);
	}
	else if (access_do_redial(d, access_gopher_dns_alarm))
	{
	    return;
	}
	else
	{
	    ACCDBG(( "Gopher: Failed connection with error '%s'\n", ep->errmess));

	    if (d->complete)
		d->complete(d->h, status_FAIL_CONNECT, ep->errmess, d->url);

	    access_done_flag = 1;

	    access_unlink(d);
	    access_free_item(d);
	}
    }
    else
    {
	if (d->progress)
	    d->progress(d->h, status_DNS, -1, -1, 0, -1, NULL);
	access_reschedule(&access_gopher_dns_alarm, d, POLL_INTERVAL);
    }
}

static void access_gopher_fetch_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    gopher_status_args gos;
    int cache_it;
    os_error *ep;
    char *cfile;

    gos.in.handle = d->transport_handle;

    ep = os_swix(WebGopher_Status, (os_regset *) &gos);

    if (ep)
    {
	ACCDBG(( "WebGopher_Status: %s\n", ep->errmess));
	memset(&gos, 0, sizeof(gos));
	gos.out.status = status_FAIL_REQUEST;
    }

    if ((gos.out.status >= status_GETTING_BODY) && (d->ft_is_set == 0) )
    {
	int ft;
	gopher_set_file_type(gos.out.fname, d->data.gopher.gopher_tag, &ft);
	d->ftype = (short) ft;
	d->ft_is_set = 1;

	if (d->flags & access_CHECK_FILE_TYPE)
	{
	    int s = access_can_handle_file_type(ft, gos.out.data_size);
	    if (s)
		gos.out.status = s;
	}
    }

    if (gos.out.status >= status_COMPLETED_FILE)
    {
	ACCDBG(( "acc%p: gopher transfer complete, status %d\n", d, gos.out.status));
	/* Time to stop */

	cfile = strdup(gos.out.fname);

	/* Temporary insertion in case the url is re-requested inside the completed call (e.g. for a GIF) */
	cache->insert(d->url, cfile, cache_flag_OURS);

	if (gos.out.status == status_COMPLETED_FILE && d->progress)
	{
	    ACCDBG(( "acc%p: calling gopher progress (%d bytes)\n", d,
	             gos.out.data_so_far ));
	    d->progress(d->h, status_GETTING_BODY, gos.out.data_so_far, gos.out.data_so_far, gos.out.ro_fh, d->ftype, d->url);
	}

#if LOCK_FILES
	/* lock the file before closing it */
	file_lock(cfile, TRUE);
#endif
	/* The gopher close does not need to delete the file as we have already if it was removed from the cache */
	access_gopher_close(d->transport_handle, 0);

	if (d->complete)
	    cache_it = d->complete(d->h, gos.out.status, gos.out.status == status_COMPLETED_FILE ? cfile : NULL, d->url);
	else
	    cache_it = 0;

	access_done_flag = 1;

#if LOCK_FILES
	/* unlock the file unless client asked for it to be lept locked */
	if ((cache_it & access_LOCK) == 0)
	{
	    ACCDBGN(("access: unlocked '%s'\n", cfile));
	    file_lock(cfile, FALSE);
	}
#endif
	if (gos.out.status == status_COMPLETED_FILE )
	{
	    if ( cache_it & access_KEEP )
		cache->keep(d->url);
	}

	if (cache_it & access_OURS)
	    cache->not_ours(cfile);

	/* It was put in the cache before the completed call but we take it out if we don't need it */
	if (gos.out.status != status_COMPLETED_FILE || (cache_it & access_CACHE) == 0)
	    cache->remove(d->url);

        mm_free(cfile);

	access_unlink(d);

	access_free_item(d);
    }
    else
    {
	int readable = d->ft_is_set;

	/* Keep going */
	if (d->progress)
	    d->progress(d->h, gos.out.status, -1, gos.out.data_so_far, gos.out.ro_fh, readable ? d->ftype : -1, d->url );
	access_reschedule(&access_gopher_fetch_alarm, d, POLL_INTERVAL);
    }
}

/* ------------------------------------------------------------ */

static void access_ftp_dns_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    int status;
    os_error *ep;

    ACCDBGN(( "Resolving name '%s'\n", d->dest_host));

    ep = ensure_line();
    if (ep == NULL)
	ep = netloc_resolve(d->dest_host, 21, &status, &d->addr);
    if (ep == NULL)
    {
    }
    else if (access_do_redial(d, access_ftp_dns_alarm))
    {
	return;
    }
    else if (access_try_fancy_resolve(d, "ftp."))
    {
	/* will try again with different host - clear header */
	ep = NULL;
    }
    else
    {
	ACCDBG(("FTP resolve failed, error:%s\n", ep->errmess));

	if (d->complete)
	    d->complete(d->h,status_FAIL_DNS, ep->errmess, d->url);

	access_done_flag = 1;

	access_unlink(d);
	access_free_item(d);

	return;
    }

    if (status == 0)
    {
	ACCDBGN(( "Got address\n"));
	ep = ensure_line();
	if (ep == NULL)
	    ep = access_ftp_fetch_start(d);

	if (ep == NULL)
	{
	    if (d->progress)
		d->progress(d->h, status_CONNECTING, -1, -1, 0, -1, NULL);
	    access_reschedule(&access_ftp_fetch_alarm, d, POLL_INTERVAL);
	}
	else if (access_do_redial(d, access_ftp_dns_alarm))
	{
	    return;
	}
	else
	{
	    ACCDBG(( "FTP: Failed connection with error '%s'\n", ep->errmess));

	    if (d->complete)
		d->complete(d->h, status_FAIL_CONNECT, ep->errmess, d->url);

	    access_done_flag = 1;

	    access_unlink(d);
	    access_free_item(d);
	}
    }
    else
    {
	ACCDBGN(( "Name not here yet\n"));
	if (d->progress)
	    d->progress(d->h, status_DNS, -1, -1, 0, -1, NULL);
	access_reschedule(&access_ftp_dns_alarm, d, POLL_INTERVAL);
    }
}

static void access_ftp_fetch_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    ftp_status_args ftps;
    int cache_it;
    os_regset *rs = (os_regset *) &ftps;
    os_error *ep = NULL;
    int status;
    ACCDBGN(( "Getting connection status\n"));
    ftps.in.session = d->transport_handle;

    ep = os_swix(WebFTP_Status, rs);

    if (ep)
    {
	ACCDBGN(( "WebFTP_Status: %s\n", ep->errmess));
	memset(&ftps, 0, sizeof(ftps));
	ftps.out.status = status_FAIL_REQUEST;
    }

    status = ftps.out.status;

    if ((status >= status_GETTING_BODY) && (d->ft_is_set == 0) )
    {
	char *slash = strrchr(d->url, '/');
	char *dot = strrchr(d->url, '.');
	int ft = -1;

	if ((status == status_COMPLETED_DIR) ||
	    (status == status_GETTING_DIRECTORY) )
	    ft = FILETYPE_HTML;
	else if (status == status_FAIL_REQUEST)
	    ft = FILETYPE_TEXT;
	else
	{
	    if (dot && (slash == NULL || slash < dot))
		ft = suffix_to_file_type(dot+1);

	    if (ft == -1)
	    {
		char *end;

		dot = strrchr(d->url, ',');
		if (dot && (slash == NULL || slash < dot))
		{
		    ft = (int) strtol(dot+1, &end, 16);
		    if (*end != 0 || end != (dot+4))
			ft = -1;
		}

		if (ft == -1)
		    ft = FILETYPE_TEXT;
	    }
	}

	set_file_type(ftps.out.local_name, ft);
	d->ftype = ft;
	d->ft_is_set = 1;

	if (d->flags & access_CHECK_FILE_TYPE)
	{
	    int s = access_can_handle_file_type(ft, ftps.out.data_total);
	    if (s)
	    {
		status = s;
		ftps.out.status = (transfer_status)status;
	    }
	}
    }

    if (status >= status_COMPLETED_FILE)
    {
	char *cfile;

	ACCDBGN(( "Transfer complete for %s\n", d->url));
	/* Time to stop */

	cfile = strdup(ftps.out.local_name);

	/* Temporary insertion in case the url is re-requested inside the completed call (e.g. for a GIF) */
	cache->insert(d->url, cfile, cache_flag_OURS);

	if (status == status_COMPLETED_DIR)
	{
	    d->flags |= access_IS_DIRECTORY;
	    status = status_COMPLETED_FILE;
	}

	if (status == status_COMPLETED_FILE && d->progress)
	{
	    d->progress(d->h, status_GETTING_BODY, ftps.out.data_so_far, ftps.out.data_so_far, ftps.out.ro_handle, d->ftype, d->url);
	}

#if LOCK_FILES
	/* lock the file before closing it */
	file_lock(cfile, TRUE);
#endif
	/* The ftp close does not need to delete the file as we have already if it was removed from the cache */
	access_ftp_close(d->transport_handle, 0);

	if (d->complete)
	    cache_it = d->complete(d->h, status, status == status_COMPLETED_FILE ? cfile : NULL, d->url);
	else
	    cache_it = 0;

	access_done_flag = 1;

#if LOCK_FILES
	/* unlock the file unless client asked for it to be lept locked */
	if ((cache_it & access_LOCK) == 0)
	{
	    ACCDBGN(("access: unlocked '%s'\n", cfile));
	    file_lock(cfile, FALSE);
	}
#endif

	if (status == status_COMPLETED_FILE || status == status_COMPLETED_DIR )
	{
	    if ( cache_it & access_KEEP )
		cache->keep(d->url);
	}

	if (cache_it & access_OURS)
	    cache->not_ours(cfile);

	/* It was put in the cache before the completed call but we take it out if we don't need it */
	if (((status != status_COMPLETED_FILE) &&
	     (status != status_COMPLETED_DIR) ) ||
	    (cache_it & access_CACHE) == 0 )
	    cache->remove(d->url);

	mm_free(cfile);

	if ( status == status_FAIL_PASSWORD )
	{
	    char *scheme, *netloc, *realm_name;

	    ACCDBG(("FTP password was wrong! Forgetting realm!\n"));
            url_parse(d->url, &scheme, &netloc, NULL, NULL, NULL, NULL);

            realm_name = url_unparse(scheme, netloc, 0, 0, 0, 0);

            auth_forget_realm( auth_lookup_realm(realm_name) );

            mm_free( realm_name );
            mm_free( scheme );
            mm_free( netloc );
	}
	else if (d->data.ftp.had_passwd && config_passwords_uptodate)
	{
            auth_write_realms( config_auth_file,
                               config_auth_file_crypt ? auth_passwd_UUCODE
                                                      : auth_passwd_PLAIN);
        }

	access_unlink(d);

	access_free_item(d);
    }
    else
    {
	int readable = d->ft_is_set;

	/* Keep going */
	if (d->progress)
	    d->progress(d->h, status, ftps.out.data_total, ftps.out.data_so_far, ftps.out.ro_handle, readable ? d->ftype : -1, d->url );
	access_reschedule(&access_ftp_fetch_alarm, d, POLL_INTERVAL);
    }
}

static int access_ftp_check_pw(char *netloc, char **userp, char **pwdp, char **hostp)
{
    char *user;
    char *passwd;
    char *host;
    char *at, *colon;
    ACCDBGN(( "Looking for user id and password in '%s'\n", netloc));
    at = strchr(netloc, '@');
    if (at)
    {
	*at = 0;
	host = strdup(at + 1);
	colon = strchr(netloc, ':');
	if (colon)
	{
	    *colon = 0;
	    passwd = strdup(colon + 1);
	}
	else
	    passwd = NULL;
	user = strdup(netloc);
    }
    else
    {
	user = strdup("anonymous");
	if (config_email_addr)
	    passwd = strdup(config_email_addr);
	else
	{
	    char buffer[64];
	    char buffer2[64];
	    char *p;
	    int len;

	    strcpy(buffer, PROGRAM_NAME"@");
	    len = strlen(buffer);

	    /* Read an old style host name */
	    os_read_var_val("Inet$HostName", buffer + len, sizeof(buffer) - len);
	    /* Read a new style domain */
	    os_read_var_val("Inet$LocalDomain", buffer2, sizeof(buffer));
	    /* If we have a new style domain */
	    if (buffer2[0])
	    {
		if ((p = strchr(buffer, '.')) != NULL)
		{
		    /* If the host name has a dot the overwrite the tail */
		    strcpy(p + 1, buffer2);
		}
		else
		{
		    /* If there is no dot append one if there was something and then append the domain name */
		    if (buffer[len] != 0)
			strcat(buffer, ".");
		    strcat(buffer, buffer2);
		}
	    }
	    /* After all that, if there is no host name then remove the '@' */
	    if (buffer[len] == 0)
		buffer[len-1] = 0;

	    passwd = strdup(buffer);
	}
	host = strdup(netloc);
    }
#if 0
    fprintf(stderr, "User='%s', passwd='%s', host='%s'\n",
	    user, passwd ? passwd : "<none>", host);
#endif
    if (hostp)
	*hostp = host;
    else
	mm_free(host);

    if (userp)
	*userp = user;
    else
	mm_free(user);

    if (pwdp)
	*pwdp = passwd;
    else
	mm_free(passwd);

    return (passwd != NULL);
}

#endif /*ndef FILEONLY */

#define COPY_DATA_SIZE 4096

static void access_copy_data(int inf, int outf, int start, int end)
{
    os_gbpbstr gps;
    char *buffer;
    int len;

    buffer = mm_malloc(COPY_DATA_SIZE);	/* SJM: from auto to malloc */

    while (start < end)
    {
	len = end - start;
	if (len > COPY_DATA_SIZE)
	    len = COPY_DATA_SIZE;

	gps.action = 3;
	gps.file_handle = inf;
	gps.data_addr = buffer;
	gps.number = len;
	gps.seq_point = start;

	os_gbpb(&gps);

	gps.action = 1;
	gps.file_handle = outf;
	gps.data_addr = buffer;
	gps.number = len;
	gps.seq_point = start;

	os_gbpb(&gps);

	start += len;
    }

    mm_free(buffer);
}

/* Returns TRUE if there is more data to come
 * FALSE if that is the end and 'd' has been freed
 */

static BOOL access_file_fetch(access_handle d)
{
    access_progress_fn progress = d->flags & access_NO_STREAM ? 0 : d->progress;

    if ( !progress || (d->data.file.so_far + d->data.file.chunk >= d->data.file.size))
    {
	ACCDBGN(("file_fetch(): d %p fname '%s' from %d to %d last transfer\n", d, d->data.file.fname, d->data.file.so_far, d->data.file.size));

	if (d->data.file.ofh)
	{
	    access_copy_data(d->data.file.fh, d->data.file.ofh,
			     d->data.file.so_far, d->data.file.size);
	}

	/* SJM addition */
	if (progress)
	{
	    d->data.file.so_far = d->data.file.size;
	    progress(d->h, status_GETTING_BODY,
		    d->data.file.size, d->data.file.so_far,
		    d->data.file.ofh ? d->data.file.ofh : d->data.file.fh,
		    d->ftype, d->url );
	}

	/* We are done */
	if (d->data.file.fh)
	{
	    ACCDBGN(("access_file_fetch: close %d\n", d->data.file.fh));
	    ro_fclose(d->data.file.fh);
	    d->data.file.fh = 0;
	}

	if (d->data.file.ofh)
	{
	    ACCDBGN(("access_file_fetch: close %d\n", d->data.file.ofh));
	    ro_fclose(d->data.file.ofh);
	    d->data.file.ofh = 0;
	}

#ifndef FILEONLY
	/* only update the header info if this is a file: read, not from the cache */
	if ((d->flags & access_FROM_CACHE) == 0 && cache->header_info)
	    cache->header_info(d->url, time(NULL), d->data.file.last_modified, UINT_MAX);
#endif
	d->complete(d->h, status_COMPLETED_FILE, d->ofile ? d->ofile : d->data.file.fname, d->url);

	access_unlink(d);
	access_free_item(d);

	return FALSE;
    }
    else
    {
	ACCDBGN(("file_fetch(): d %p fname '%s' from %d to %d\n", d, d->data.file.fname, d->data.file.so_far, d->data.file.so_far + d->data.file.chunk));

	/* More to do */
	if (d->data.file.ofh)
	{
	    access_copy_data(d->data.file.fh, d->data.file.ofh,
			     d->data.file.so_far, d->data.file.so_far + d->data.file.chunk);
	}

	d->data.file.so_far += d->data.file.chunk;

	progress(d->h, status_GETTING_BODY,
		    d->data.file.size, d->data.file.so_far,
		    d->data.file.ofh ? d->data.file.ofh : d->data.file.fh,
		    d->ftype, d->url );

	if (d->data.file.chunk < FILE_MAX_CHUNK)
	    d->data.file.chunk <<= 1;

	access_reschedule(&access_file_fetch_alarm, d, FILE_POLL_INTERVAL);
    }

    return TRUE;
}

static void access_file_fetch_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    ACCDBGN(("access_file_fetch_alarm(): at %d\n", at));
    access_file_fetch(d);
}

BOOL access_fromcache( access_handle h )
{
    return h->access_type == access_type_FILE;
}

static os_error *access_new_file(const char *file, int ft, char *url, access_url_flags flags, char *ofile,
				 access_progress_fn progress, access_complete_fn complete,
				 void *h, access_handle *result)
{
    access_handle d = NULL;
    os_error *ep = NULL;
    int fh = _kernel_osfind(0x4f, (char *)file); /* use kernel_osfind deliberately so we can get the error properly */

    ACCDBG(("access_new_file: file %s fh %d ft %03x flags %x\n", file, fh, ft, flags));

    if (fh > 0)
    {
	if (ft == -1)
	    ft = file_type(file);

	d = mm_calloc(1, sizeof(*d));

	d->access_type = access_type_FILE;
	d->flags = flags;
	d->url = strdup(url);
	if (ofile)
	{
	    d->ofile = strdup(ofile);
	    d->data.file.ofh = ro_fopen(ofile, RO_OPEN_WRITE);
	    if ( d->data.file.ofh == 0 )
	    {
	        /* pdh: failed to open the file. This is a real error and the
		 * public should be told!
		 */
	        ep = (os_error*)_kernel_last_oserror();
		mm_free( d->url );
		mm_free( d );
		*result = NULL;
		return ep;
	    }
	    set_file_type(ofile, ft);

	    ACCDBG(("access_new_file: ofile %s fh %d\n", ofile, d->data.file.ofh));
	}

	d->progress = progress;
	d->complete = complete;
	d->h = h;
	access_link(d);

	d->data.file.fname = strdup(file);
	d->ftype = ft;
	d->data.file.fh = fh;

	d->data.file.size = ro_get_extent(fh);
	d->data.file.last_modified = file_last_modified(file);

	/* if it is a priority item then fetch the whole thing right now (eg background image) */
	if ( (flags & access_MAX_PRIORITY) || gbf_active(GBF_FILES_IN_ONE_GO) )
	{
	    d->data.file.chunk = d->data.file.size;
	    if (!access_file_fetch(d))
		d = NULL;
	}
	/* if this is an image then immediately process the first few bytes of the image to get the size */
	else if (flags & access_IMAGE)
	{
	    d->data.file.chunk = 2048;
	    if (access_file_fetch(d))
		d->data.file.chunk = FILE_INITIAL_CHUNK;
	    else
		d = NULL;
	}
	/* otherwise schedule a read for sometime later (this includes PRIORITY items) */
	else
	{
	    d->data.file.chunk = FILE_INITIAL_CHUNK;
	    access_reschedule(&access_file_fetch_alarm, d, FILE_POLL_INTERVAL);
	}
    }
    else
    {
	ep = (os_error *)_kernel_last_oserror();
    }

    *result = d;

    if ( ep )
    {
        ACCDBG(("access_new_file: returning error %s\n", ep->errmess ));
    }

    return ep;
}

/* ------------------------------------------------------------ */

int access_url_type_and_size(char *url, int *ftypep, int *sizep)
{
    char *scheme, *netloc, *path, *params, *query, *fragment;
    char *cfile, *cfile1;
    int ft = -1;

    *sizep = 0;

#ifdef FILEONLY
    cfile = NULL;
#else
    cfile1 = cache->lookup(url, FALSE);     /* cfile1 is original ptr so it can be freed */
    cfile = cfile1;
#endif

    url_parse(url, &scheme, &netloc, &path, &params, &query, &fragment);

    if (cfile == NULL && (strcasecomp(scheme, "file") == 0))
    {
	cfile = url_path_to_riscos(path);

	if (path_is_directory(cfile))
	{
	    *ftypep = FILETYPE_HTML;
	    return 0;
	}
    }

    if (cfile == NULL && (strcasecomp(scheme, "icontype") == 0))
    {
	*ftypep = FILETYPE_SPRITE;
	*sizep = 488;		/* They should all be this size if they have a mask */

	return 0;
    }

    if (cfile)
    {
	os_filestr ofs;
	os_error *ep;
	os_regset r;

	ofs.action = 5;
	ofs.name = cfile;

	ep = os_file(&ofs);

	if (ep == 0)
	    *sizep = ofs.start;

	if (ep || (ofs.action != 1 && ofs.action != 3))
	    ft = -1;
	else
	{
	    r.r[0] = 38;
	    r.r[1] = (int) (long) ofs.name;
	    r.r[2] = ofs.loadaddr;
	    r.r[3] = ofs.execaddr;
	    r.r[4] = ofs.start;
	    r.r[5] = ofs.end;
	    r.r[6] = 1;

	    ep = os_swix(OS_FSControl, &r);
	    if (ep)
		ft = -1;
	    else
		ft = r.r[2];
	}
    }

    if (ft == -1)
    {
       	char *dot, *slash;

	slash = strrchr(path, '/');
	dot = strrchr(path, '.');

	if (dot != NULL && slash < dot && dot[1] != 0)
	{
	    dot++;

	    ft = suffix_to_file_type(dot);
	}
    }

    *ftypep = ft;

    url_free_parts(scheme, netloc, path, params, query, fragment);

#ifndef FILEONLY
    cache->lookup_free_name(cfile1);
#endif

    return (cfile != NULL);
}

#ifndef FILEONLY

static int access_match_host(char *host, char *matchlist)
{
    char *list;
    char *p;
    int hit;
    int len, l2;

    /* Skip past any user name or password */
    p = strchr(host, '@');
    if (p)
	host = p+1;

    /* No list? No match. */
    if (matchlist == 0)
	return 0;

    /* Better take a copy; strtok mangles the string */
    list = strdup(matchlist);
    /* And we are about to mangle the net location */
    host = strdup(host);

    /* Stop at a colon */
    p = strchr(host, ':');
    if (p)
	*p = 0;

    /* We need the length lots for doing tail matches */
    len = strlen(host);

    hit = 0;
    /* Let the domains be comma or space sepatared */
    p = strtok(list, " ,\n");

    while (p && !hit)
    {
	l2 = strlen(p);
	/* It's a hit is it's long enough for a tail match and we have such a match */
	hit = ((l2 <= len) && (strcasecomp(p, host + len - l2) == 0));

	/* If we hit save the bother of another token */
	if (!hit)
	    p = strtok(NULL, " ,\n");
    }

    mm_free(list);
    mm_free(host);

    return hit;
}

/* ------------------------------------------------------------ */

static os_error *access_new_http(char *url, access_url_flags flags, char *ofile, char *bfile, char *bfiletype, char *referer,
				 access_progress_fn progress, access_complete_fn complete,
				 void *h, access_handle *result, char *dest_host, char *request_string)
{
    access_handle d;
    ACCDBGN(( "Making new http access object flags=%x\n", flags));
    d = mm_calloc(1, sizeof(*d));

    d->access_type = access_type_HTTP;
    d->flags = flags;
    d->url = strdup(url);
    if (ofile)
	d->ofile = strdup(ofile);
    d->referer = strdup(referer);
    d->data.http.body_file = strdup(bfile);
    d->data.http.body_type = bfiletype; /* note no strdup */
    d->progress = progress;
    d->complete = complete;
    d->h = h;

    d->dest_host = strdup(dest_host);
    d->request_string = strdup(request_string);

    d->data.http.expires = UINT_MAX;
    d->data.http.last_modified = 0;
    d->data.http.date = 0;

    access_link(d);

    *result = d;

    access_done_flag = 0;

    access_http_dns_alarm(0, d);

    if (flags & access_FORGROUND)
    {
	while (!access_done_flag)
	    d->next_fn(0, d);
/* 	*result = 0; */
    }

    /* If dns fails immediately then 'd' will have been freed already */
    if (access_done_flag)
	*result = 0;

    return NULL;
}

static os_error *access_new_ftp(char *url, access_url_flags flags, char *ofile, char *referer,
				access_progress_fn progress, access_complete_fn complete,
				void *h, access_handle *result, char *netloc, char *path)
{
    access_handle d;
    os_error *ep = NULL;

    if (config_proxy_ftp_on &&
	config_proxy_ftp &&
	!access_match_host(netloc, config_proxy_ftp_ignore))
    {
	char *aurl;

	aurl = url_unparse("ftp", netloc, path, 0, 0, 0);

	ep = access_new_http(url, flags | access_PROXY, ofile, NULL, NULL,
	                     referer,
			     progress, complete, h,
			     result, config_proxy_ftp, aurl);
	mm_free(aurl);
    }
    else
    {
#if 0
	fprintf(stderr, "FTP access\n");
	fprintf(stderr, "Making new access object\n");
#endif
	d = mm_calloc(1, sizeof(*d));

	d->access_type = access_type_FTP;
	d->flags = flags;
	d->url = strdup(url);
	if (ofile)
	    d->ofile = strdup(ofile);
	d->progress = progress;
	d->complete = complete;
	d->h = h;
	access_link(d);

	if (access_ftp_check_pw(netloc, &(d->data.ftp.user), &(d->data.ftp.passwd),
				&(d->dest_host) ) == 0)
	{
	    d->data.ftp.passwd = strdup("");
	}

	d->request_string = strdup(path ? path : "/");
#if 0
	fprintf(stderr, "Host '%s'\nRequest '%s'\nUser '%s'\nPasswd '%s'\n",
		d->dest_host, d->request_string, d->data.ftp.user, d->data.ftp.passwd);
#endif
	*result = d;

	access_done_flag = 0;

	access_ftp_dns_alarm(0, d);

	if (flags & access_FORGROUND)
	{
	    while (!access_done_flag)
		d->next_fn(0, d);
	    /* *result = 0; */
	}

	/* If dns fails immediately then 'd' will have been freed already */
	if (access_done_flag)
	    *result = 0;
    }

    return ep;
}

static void access_ftp_passwd_callback(fe_passwd pw, void *handle, char *user, char *password)
{
    access_handle d = (access_handle) handle;
    realm rr = NULL;
    char *realm_name;
    char *scheme, *netloc, *path, *params, *query, *fragment;

    url_parse(d->url, &scheme, &netloc, &path, &params, &query, &fragment);

    realm_name = url_unparse(scheme, netloc, 0, 0, 0, 0);
    ACCDBGN(( "New realm: %s\n", realm_name));
    d->data.ftp.pw = NULL;

    if (user)
    {
	rr = auth_add_realm(realm_name, "BASIC", user, password);
    }

    if (rr == NULL)
    {
	frontend_complain(makeerror(ERR_BAD_AUTH));
	/* @@@@ What do we do here ? */
	d->complete(d->h, status_FAIL_PASSWORD, NULL, d->url);

	access_done_flag = 1;

	access_unlink(d);
	access_free_item(d);
    }
    else
    {
	auth_add(realm_name, rr);
	d->data.ftp.had_passwd = 1;
	d->data.ftp.rr = rr;

	access_url(d->url, d->flags, d->ofile, NULL, NULL, d->url,
		   &access_redirect_progress, &access_redirect_complete, d, &(d->redirect));
    }

    mm_free(realm_name);
    url_free_parts(scheme, netloc, path, params, query, fragment);
}

static os_error *access_ftp_login(char *url, access_url_flags flags, char *ofile, char *referer,
				  access_progress_fn progress, access_complete_fn complete,
				  void *h, access_handle *result, char *user, char *path)
{
    access_handle d;
    os_error *ep = NULL;
    char *hname = access_host_name_only(url);

#if 0
    fprintf(stderr, "FTP login\n");
    fprintf(stderr, "Making new access object\n");
#endif
    d = mm_calloc(1, sizeof(*d));

    d->access_type = access_type_FTP;
    d->flags = flags;
    d->url = strdup(url);
    if (ofile)
	d->ofile = strdup(ofile);
    d->progress = progress;
    d->complete = complete;
    d->h = h;
    access_link(d);

    d->data.ftp.pw = frontend_passwd_raise(access_ftp_passwd_callback, d, user, "FTP", hname);

    mm_free(hname);

    *result = d;

    return ep;
}

#endif /*ndef FILEONLY */

#if INTERNAL_URLS

static void access_internal_fetch_alarm(int at, void *h)
{
    access_handle d = (access_handle) h;
    cache_flags fl;
    char *file = d->ofile ? d->ofile : d->data.internal.cfile;

    ACCDBG(( "Internal fetch alarm\n"));

    if (d->ofile == NULL)
    {
	cache->insert(d->url, file, cache_flag_OURS);
	if (cache->header_info)
	{
	    unsigned now = (unsigned)time(NULL);
	    cache->header_info(d->url, now, now, 0);
	}
    }

    if (d->progress)
	access_progress_flush(d->h, file, d->url, d->progress);

    fl = 0;
    if (d->complete)
	fl = d->complete(d->h, status_COMPLETED_FILE, file, d->url);

    ACCDBG(( "Internal cache flags %x\n", fl));

    if (fl & access_KEEP)
	cache->keep(d->url);

    if ((fl & access_OURS) && d->ofile == NULL)
	cache->not_ours(file);

    if ((fl & access_CACHE) == 0)
	cache->remove(d->url);

    access_unlink(d);
    access_free_item(d);
}

static os_error *access_new_internal(char *url, const char *path, const char *query,
				     access_url_flags flags, char *ofile, char *bfile, char *referer,
				 access_progress_fn progress, access_complete_fn complete,
				 void *h, access_handle *result)
{
    char *file, *new_url;
    access_handle d;
    os_error *ep;
    int rtype;

    file = strdup(ofile ? ofile : cache->scrapfile_name());

    ACCDBG(( "Making new internal fetch flags=%x path='%s' query='%s' cache file='%s'\n", flags, path, strsafe(query), file));

    new_url = NULL;
    d = NULL;
    ep = NULL;

    rtype = frontend_internal_url(path, query, bfile, referer, file, &new_url, &flags);
    switch (rtype)
    {
    case fe_internal_url_ERROR:
	ep = makeerrorf(ERR_CANT_GET_URL, url);
	break;

    case fe_internal_url_REDIRECT:
	ACCDBG(( "Redirect to '%s' (%p)\n", strsafe(new_url), new_url));

	if (new_url)
	{
	    d = mm_calloc(1, sizeof(*d));

	    d->access_type = access_type_INTERNAL;
	    d->flags = flags;

	    d->url = new_url;
	    new_url = NULL;

	    if (ofile)
	    {
		d->ofile = file;
		file = NULL;
	    }

	    d->progress = progress;
	    d->complete = complete;
	    d->h = h;
	    access_link(d);

	    ep = access_url(d->url, d->flags, d->ofile, NULL, NULL, referer,
			    &access_redirect_progress, &access_redirect_complete,
			    d, &(d->redirect));

	    if (!ep && d->redirect == NULL)
	    {
		ACCDBG(("access_new_internal(): redirect returned quickly\n"));
		/* in this case 'd' has been freed already by the redirect code! */
		d = NULL;
	    }
	}
	break;

    case fe_internal_url_NO_ACTION:
	ep = makeerror(ERR_NO_ACTION);
	break;

    case fe_internal_url_NEW:
	ACCDBG(( "File to load\n"));

	d = mm_calloc(1, sizeof(*d));

	d->access_type = access_type_INTERNAL;
	d->flags = flags;

	d->url = strdup(url);
	d->ofile = strdup(ofile);

	d->data.internal.cfile = file;
	file = NULL;

	d->progress = progress;
	d->complete = complete;
	d->h = h;

	access_link(d);

	access_reschedule(&access_internal_fetch_alarm, d, FILE_POLL_INTERVAL);
	break;

    case fe_internal_url_HELPER:
	ep = makeerror(ERR_NO_ACTION);
	break;
    }

    if (ep && d)
    {
	/* if this call returned a want ot use helper error then do the punt and convert to a no action */
	if (ep->errnum == ANTWEB_ERROR_BASE + ERR_USED_HELPER)
	{
	    frontend_url_punt(NULL, d->url, bfile);
	    ep = makeerror(ERR_NO_ACTION);
	}

	ACCDBG(("access_new_internal(): error, freeing access handle %p\n", d));
	access_unlink(d);
	access_free_item(d);
	d = NULL;
    }

    mm_free(file);
    *result = d;

    return ep;
}
#endif

#if 0
static void write_buf(char *buffer, const char *path, const char *params, const char *query)
{
    if (path)
	strcpy(buffer, path);
    else
	strcpy(buffer, "/");

    if (params)
    {
	strcat(buffer, ";");
	strcat(buffer, params);
    }

    if (query)
    {
	strcat(buffer, "?");
	strcat(buffer, query);
    }
}
#endif

extern char timeoutbuf[20];

os_error *access_url(char *url, access_url_flags flags, char *ofile,
                     char *bfile, char *bfiletype, char *referer,
		     access_progress_fn progress, access_complete_fn complete,
		     void *h, access_handle *result)
{
    access_handle d;
    char *cfile;
    os_error *ep = NULL;
    BOOL file_missing;

    visdelay_begin();

    /* this prevents internal state flags from being passed back in from relocates (like proxy and secure flags) */
    flags &= access_INTERNAL_FLAGS;

    usrtrc( "access_url: %s from %s flags 0x%x\n", url, referer ? referer : "<none>", flags);

    *result = 0;

#if TIMEOUT
    url = *timeoutbuf ? timeoutbuf : url;
#endif

#ifndef FILEONLY
    /* NO cacheing in fileonly version */

    if (flags & (access_NOCACHE|access_UPLOAD))
    {
	cache->remove(url);
	cfile = NULL;
    }
    else
	cfile = cache->lookup(url, flags & access_CHECK_EXPIRE ? 1 : 0);

    if (cfile)
    {
	file_missing = FALSE;

	if (ofile)
	{
	    os_regset r;

	    r.r[0] = 26;
	    r.r[1] = (int) (long) cfile;
	    r.r[2] = (int) (long) ofile;
	    r.r[3] = (1 << 1);		/* Ensure files are overwriten */

	    ep = os_swix(OS_FSControl, &r);

	    /* sjm: check here for file not being present */
	    if (ep && (ep->errnum & 0xff) == 214)
	    {
		file_missing = TRUE;
		ep = NULL;
	    }
	}

	/* if the file is in cache then feed to the file routines
	 */

	if (ep == NULL && !file_missing)
	{
	    ep = access_new_file(cfile, -1/* file_type(cfile) */, url, flags | access_FROM_CACHE, ofile, progress, complete, h, result);
	    if (ep)
		file_missing = TRUE;
	}

	if (file_missing)
	{
	    ACCDBGN(("Cache hit '%s', file missing, removing the cache entry e %x '%s'.\n", cfile, ep ? ep->errnum : 0, ep ? ep->errmess : ""));
	    cache->remove(url);
	    ep = NULL;
	}
	else
	{
	    ACCDBGN(("Cache hit '%s', streaming the cache file\n", cfile));
	}

	/* cache file is not needed at this point, either it doesn't exist or we've fed it to new_file() */
	cache->lookup_free_name(cfile);
	cfile = NULL;
    }
    else
	file_missing = TRUE;
#else
    file_missing = TRUE;
#endif /*ndef FILEONLY */

    /* if cache is no good then go and fetch it for real */
    if (file_missing)
    {
	char *scheme, *netloc, *path, *params, *query, *fragment;

	url_parse(url, &scheme, &netloc, &path, &params, &query, &fragment);
	ACCDBGN(( "Cache miss... trying to fetch file\n"));

	/* forcibly block all file: URLs from being referers */
	if (referer && strncasecomp(referer, "file:", sizeof("file:")-1) == 0)
	    referer = NULL;

	if (netloc && netloc[0] && !auth_check_allow_deny(netloc))
	{
	    ep = makeerror(ERR_ACCESS_DENIED);
	}

	if (ep)
	{
	    /* Pass on trying to handle URL.  We can't just return because we need to tidy up */
	}
	else if (strcasecomp(scheme, "file") == 0)
	{
	    int ft = -1;

	    cfile = url_path_to_riscos(path);
	    if (cfile[strlen(cfile)-1] == '.')
		cfile[strlen(cfile)-1] = 0;

#if 0 /* defined(STBWEB) && !defined(CBPROJECT) */
	    /* This is not used anymore as CSFS will do tis own redialling */
	    /* if file is not in resourcefs and not in cachefs then ensure the modem line
	     * if ensure fails then report error
	     */
	    {
		char buffer[256];
		ep = (os_error *)_swix(OS_FSControl, _INR(0,5), 0x25, cfile, buffer, 0, 0, sizeof(buffer));

		ACCDBG(("canonicalise: '%s'\n", buffer));

		if (ep == NULL &&
		    strncasecomp(buffer, "resources:", sizeof("resources:")-1) != 0 &&
		    strncasecomp(buffer, "cache:", sizeof("cache:")-1) != 0)
		{
		    ep = ensure_modem_line();
		}
	    }
#endif
	    /* if no errors in ensuring modem line trhen check object and file type */
	    if (!ep)
	    {
		int obj_type;
		ft = file_and_object_type(cfile, &obj_type);

		/* if file is not found try looking for file of correct filetype without extension */
		if (obj_type == 0)
		{
		    char *slash = strrchr(cfile, '/');
		    char *dot = strrchr(cfile, '.');

		    /* if we have a valid extension */
		    if (slash && (dot == NULL || dot < slash))
		    {
		        int newft = suffix_to_file_type( slash+1 ); /* file type from extension */

			/* if we couldn't find the file then see if we can find one with the extensions stripped off */
			*slash = 0;
			ft = file_and_object_type(cfile, &obj_type);

			if (obj_type && newft != ft)
			    obj_type = 0;
		    }
		}

		/* if still not file found then return error */
		if (obj_type == 0)
		    ep = makeerrorf(ERR_CANT_READ_FILE, url);

#ifndef STBWEB
                /* pdh: Desktop Fresco wanted this, I don't know whether
                 * NCFresco does or not
		 * sjm: well it might but that's what the chunk of code above does (or was meant to)
                 */
		if ( ft == FILETYPE_DOS || ft == FILETYPE_DATA
		     || ft == FILETYPE_TEXT )
                {
                    char *slash = strrchr( cfile, '/' );
		    char *dot = strrchr(cfile, '.');

		    if (slash && (dot == NULL || dot < slash))
		    {
		        int newft = suffix_to_file_type( slash+1 );
		        if ( newft != -1 )
		            ft = newft;
		    }
                }
#endif

		/* if an image fs then guess whether we should treat it as a file or directory */
		if (!ep && obj_type == 3 && !frontend_can_handle_file_type(ft) && !frontend_plugin_handle_file_type(ft))
		    ft = FILETYPE_DIRECTORY;
	    }

	    if (ep || ft == -1)
	    {
	    }
	    else if (ft == FILETYPE_DIRECTORY)
	    {
		char *path;

		path = cfile;
		cfile = strdup(ofile ? ofile : cache->scrapfile_name() );

		ep = dir2html(path, cfile, 0);

		if (ep == NULL)
		{
#ifdef FILEONLY
		    access_progress_flush(h, cfile, url, progress);
                    complete( h, status_COMPLETED_FILE, cfile, url );
                    remove( cfile );
#else
                    /* never cache in fileonly version */
		    access_complete_flags fl;

		    cache->insert(url, cfile, cache_flag_OURS);

		    access_progress_flush(h, cfile, url, progress);
		    fl = complete(h, status_COMPLETED_FILE, cfile, url);

		    if (fl & access_OURS)
			cache->not_ours(cfile);

		    if ((fl & access_CACHE) == 0)
			cache->remove(url);
#endif
		}

		mm_free(path);
		/* cfile will be freed at end of section */
		*result = NULL;
	    }
	    else if (ft == FILETYPE_URL)
	    {
		char *new_url;	/* SJM removed auto array */
		FILE *fh;

		fh = mmfopen(cfile, "r");
		if (fh && (new_url = xfgets(fh)) != NULL)
		{
		    d = mm_calloc(1, sizeof(*d));

		    d->access_type = access_type_FILE;
		    d->flags = flags;
		    d->url = strdup(url);
		    d->ofile = strdup(ofile);

		    d->progress = progress;
		    d->complete = complete;
		    d->h = h;
		    access_link(d);

		    ep = access_url(new_url, d->flags, d->ofile, NULL, NULL, referer,
				    &access_redirect_progress, &access_redirect_complete,
				    d, &(d->redirect));

		    mm_free(new_url);
		}
		else
		{
		    ep = makeerrorf(ERR_CANT_READ_FILE, url);
		}

		if (fh)
		    mmfclose(fh);
	    }
	    else if (ft != FILETYPE_HTML && ft != FILETYPE_GOPHER && ft != FILETYPE_TEXT && !image_type_test(ft))
	    {
 		access_progress_flush(h, cfile, url, progress);
		complete(h, status_COMPLETED_FILE, cfile, url);

		*result = NULL;
	    }
	    else
	    {
#ifdef STBWEB
		flags |= access_MAX_PRIORITY;
#endif
		ep = access_new_file(cfile, ft, url, flags, ofile, progress, complete, h, result);
		if ( ep )
		    ACCDBG(("access_new_file returns %s\n", ep->errmess ));
	    }

	    mm_free(cfile);
	}
	else if (strcasecomp(scheme, "icontype") == 0)
	{
	    access_complete_flags fl;
	    char sname[32];
	    ACCDBGN(( "Fetching icontype '%s'\n", path));
	    cfile = strdup(ofile ? ofile : cache->scrapfile_name());

	    if (path[0] == ',')
	    {
		strcpy(sname, "small_");
		strlencat(sname, path+1, sizeof(sname));
/* 		sprintf(sname, "small_%s", path+1); */
	    }
	    else if (path[0] == '.')
	    {
		int ft = suffix_to_file_type(path+1);

		sprintf(sname, "small_%03x", ft == -1 ? 0xfff : ft);
	    }
	    else if (strcasecomp(path, "directory") == 0)
	    {
		strcpy(sname, "small_dir");
	    }
	    else
	    {
		strncpysafe(sname, path, sizeof(sname));
		strlencat(sname, "icon", sizeof(sname));
	    }

	    ep = access_write_wimp_icon(sname, cfile);

	    if (ep == NULL)
	    {
#ifdef FILEONLY
		access_progress_flush(h, cfile, url, progress);
                complete( h, status_COMPLETED_FILE, cfile, url );
                remove( cfile );
#else
		cache->insert(url, cfile, cache_flag_OURS);

		access_progress_flush(h, cfile, url, progress);
		fl = complete(h, status_COMPLETED_FILE, cfile, url);

	        if (fl & access_OURS)
		    cache->not_ours(cfile);

                if ((fl & access_CACHE) == 0)
    		    cache->remove(url);
#endif
            }

            mm_free(cfile);

	    *result = NULL;
	}
	else if (strcasecomp(scheme, "http") == 0)
	{
#ifdef FILEONLY
	    access_progress_flush(h, FILEONLY_EXCUSE, url, progress);
            complete( h, status_COMPLETED_FILE, FILEONLY_EXCUSE, url );
            *result = NULL;
#else
	    if (config_proxy_http_on &&
		config_proxy_http &&
		!access_match_host(netloc, config_proxy_http_ignore))
	    {
		ep = access_new_http(url, flags | access_PROXY, ofile, bfile,
		                     bfiletype,
		                     referer, progress, complete, h, result, config_proxy_http, url);
	    }
	    else
	    {
		char *buffer = url_unparse(NULL, NULL, path ? path : "/", params, query, NULL);
		ep = access_new_http(url, flags, ofile, bfile, bfiletype,
		                     referer, progress, complete, h, result, netloc, buffer);
		mm_free(buffer);
	    }
#endif /* ndef FILEONLY */
	}
	else if (strcasecomp(scheme, "https") == 0)
	{
#ifdef FILEONLY
	    access_progress_flush(h, FILEONLY_EXCUSE, url, progress);
            complete( h, status_COMPLETED_FILE, FILEONLY_EXCUSE, url );
            *result = NULL;
#else
	    flags |= access_SECURE;

	    /* pdh: Fixed this to support a different proxy for https */

	    if (config_proxy_https_on &&
		config_proxy_https &&
		!access_match_host(netloc, config_proxy_https_ignore))
	    {
		ep = access_new_http(url, flags | access_PROXY, ofile, bfile, bfiletype, referer, progress, complete, h, result, config_proxy_https, url);
	    }
	    else
	    {
		char *buffer = url_unparse(NULL, NULL, path ? path : "/", params, query, NULL);
		ep = access_new_http(url, flags, ofile, bfile, bfiletype, referer, progress, complete, h, result, netloc, buffer);
		mm_free(buffer);
	    }
#endif /* ndef FILEONLY */
	}
	else if (strcasecomp(scheme, "gopher") == 0)
	{
#ifdef FILEONLY
	    access_progress_flush(h, FILEONLY_EXCUSE, url, progress);
            complete( h, status_COMPLETED_FILE, FILEONLY_EXCUSE, url );
            *result = NULL;
#else
	    if (config_proxy_gopher_on &&
		config_proxy_gopher &&
		!access_match_host(netloc, config_proxy_gopher_ignore))
	    {
		ep = access_new_http(url, flags | access_PROXY, ofile, bfile, bfiletype, referer, progress, complete, h, result, config_proxy_gopher, url);
	    }
	    else
	    {
		if (path && path[0] && path[1] == '7' && query == NULL)
		{
		    /* A query link with no request; fake the link page */
		    char *cfile;
		    FILE *fh;

		    cfile = strdup(ofile ? ofile : cache->scrapfile_name());

		    fh = mmfopen(cfile, "w");
		    if (fh == NULL)
		    {
			*result = 0;
			ep = makeerrorf(ERR_CANT_READ_FILE, url);
		    }
		    else
		    {
			access_complete_flags fl;

			fprintf(fh, "<h1>Searchable Gopher Index</h1><isindex>\n");
			mmfclose(fh);

			set_file_type(cfile, FILETYPE_HTML);

			*result = NULL;

			access_progress_flush(h, cfile, url, progress);
			fl = complete(h, status_COMPLETED_FILE, cfile, url);

			if (fl & access_CACHE)
			{
			    cache->insert(url, cfile, cache_flag_OURS);
			}

			if (fl & access_KEEP)
			{
			    cache->keep(url);
			}
		    }

                    mm_free(cfile);
		}
		else
		{
		    char *buffer = NULL;

		    ACCDBGN(( "Making new access object\n"));
		    d = mm_calloc(1, sizeof(*d));

		    d->access_type = access_type_GOPHER;
		    d->flags = flags;
		    d->url = strdup(url);
		    if (ofile)
			d->ofile = strdup(ofile);
		    d->progress = progress;
		    d->complete = complete;
		    d->h = h;
		    access_link(d);

		    d->dest_host = strdup(netloc);

		    if (path && path[0] && path[1])
		    {
			buffer = strcatx1(strdup_unescaped(path + 2), NULL);
/* 			translate_escaped_text(path + 2, buffer, sizeof(buffer)); */
			d->data.gopher.gopher_tag = path[1];
		    }
		    else
		    {
/* 			buffer[0] = 0; */
			d->data.gopher.gopher_tag = '1';
		    }

		    if (query)
		    {
			char *eq;

			buffer = strcatx1(buffer, "\t");

			eq = strchr(query, '=');
			if (eq)
			{
			    buffer = strcatx1(buffer, eq+1);
			}
			else
			{
			    buffer = strcatx1(buffer, query);
			}
		    }

		    d->request_string = strtrim(buffer);

		    *result = d;

		    access_done_flag = 0;

		    access_gopher_dns_alarm(0, d);

		    if (flags & access_FORGROUND)
		    {
			while (!access_done_flag)
			    d->next_fn(0, d);
			/* *result = 0; */
		    }

		    /* If dns fails immediately then 'd' will have been freed already */
		    if (access_done_flag)
			*result = 0;
		}
	    }
#endif /* ndef FILEONLY */
	}
	else if (strcasecomp(scheme, "ftp") == 0 /* && path[strlen(path)-1] != '/' */ )
	{
#ifdef FILEONLY
	    access_progress_flush(h, FILEONLY_EXCUSE, url, progress);
            complete( h, status_COMPLETED_FILE, FILEONLY_EXCUSE, url );
            *result = NULL;
#else
	    char *at, *colon;

	    at = strchr(netloc, '@');
	    colon = strchr(netloc, ':');

	    if (at && (colon == 0 || colon > at))
	    {
		auth_type type;
		auth_lookup_result r;
		char *user;
		char *passwd;
		realm rr;
		ACCDBGN(( "Need to find a password\n"));
		r = auth_lookup(url, &type, &user, &passwd);

		if (r != auth_lookup_SUCCESS)
		{
		    char temp[256];
		    sprintf(temp, "ftp://%s/", netloc);

		    rr = auth_lookup_realm(temp);

		    if (rr)
		    {
			auth_add(temp, rr);
			r = auth_lookup(url, &type, &user, &passwd);
		    }
		}

		if (r == auth_lookup_SUCCESS)
		{
		    char *nlbuf;
		    nlbuf = strcatx1(NULL, user);
		    if (passwd)
		    {
			nlbuf = strcatx1(nlbuf, ":");
			nlbuf = strcatx1(nlbuf, passwd);
		    }
		    nlbuf = strcatx1(nlbuf, "@");
		    nlbuf = strcatx1(nlbuf, at+1);

/* 		    sprintf(nlbuf, "%s%s%s@%s", */
/* 			    user, */
/* 			    passwd ? ":" : "", */
/* 			    passwd ? passwd : "", */
/* 			    at+1 ); */
		    ACCDBGN(( "Password lookup succeded, new netloc is '%s'\n", nlbuf));
		    ep = access_new_ftp(url, flags, ofile, referer,
					progress, complete, h,
					result, nlbuf, path);
		    mm_free(nlbuf);
		}
		else
		{
		    *at = 0;
		    ep = access_ftp_login(url, flags, ofile, referer,
					  progress, complete, h,
					  result, netloc, path);
		}
	    }
	    else
	    {
		ep = access_new_ftp(url, flags, ofile, referer,
				    progress, complete, h,
				    result, netloc, path);
	    }
#endif /* ndef FILEONLY */
	}
	else if (strcasecomp(scheme, "mailto") == 0)
	{
#ifdef FILEONLY
	    access_progress_flush(h, FILEONLY_EXCUSE, url, progress);
            complete( h, status_COMPLETED_FILE, FILEONLY_EXCUSE, url );
            *result = NULL;
#else
	    char *proxy = NULL;
	    if (config_proxy_mailto_on &&
		config_proxy_mailto &&
		(proxy = strdup_gstrans(config_proxy_mailto)) != NULL &&
		proxy[0])
	    {
		char *buffer;
		char *scheme1, *netloc1, *path1, *params1, *query1, *frag1;
		char *new_url;

		url_parse(proxy, &scheme1, &netloc1, &path1, &params1, &query1, &frag1);

		new_url = url_unparse(scheme1, netloc1, path1 ? path1 : "/", params1, query1, frag1);

		buffer = strcatx1(NULL, new_url);

		/* ensure current URL ends with & or ? before adding query information */
		switch (buffer[strlen(buffer)-1])
		{
		case '&':
		case '?':
		case '=':
		    break;
		default:
		    buffer = strcatx1(buffer, "?");
		    break;
		}

		/* add on the mailto details */
		if (path)
		{
		    buffer = strcatx1(buffer, "to=");
		    buffer = strcatx1(buffer, path);
		}

		if (query)
		{
		    if (path)
			buffer = strcatx1(buffer, "&");
		    buffer = strcatx1(buffer, query);
		}

		/* fetch direct or via proxy? */
		if (strcasecomp(scheme1, "http") == 0 &&
		    config_proxy_http_on &&
		    config_proxy_http &&
		    !access_match_host(netloc1, config_proxy_http_ignore))
		{
		    ep = access_new_http(buffer, flags | access_PROXY, ofile, bfile, bfiletype, referer, progress, complete, h, result, config_proxy_http, buffer);
		}
		else
		{
		    int offset = strlen(scheme1) + sizeof("//:")-1 + strlen(netloc1);
		    ep = access_new_http(buffer, flags, ofile, bfile, bfiletype, referer, progress, complete, h, result, netloc1, buffer + offset);
		}

		url_free_parts(scheme1, netloc1, path1, params1, query1, frag1);
		mm_free(new_url);
		mm_free(buffer);
	    }
	    else
		ep = makeerror( ERR_USED_HELPER );

	    ACCDBG(("access_url: mailto: config '%s' gstransed '%s'\n",
		    config_proxy_mailto ? config_proxy_mailto : "<null>",
		    proxy ? proxy : "<null>"));

	    mm_free(proxy);
#endif /*ndef FILEONLY */
	}

#if INTERNAL_URLS
	else if (strcasecomp(scheme, "ncfrescointernal") == 0 || strcasecomp(scheme, "ncint") == 0)
	{
	    ep = access_new_internal(url, path, query, flags, ofile, bfile, referer,
				    progress, complete, h, result);
	}
#endif

	else
	{
	    /* If we get here then the transport is unknown */
#if 0
            /* moved this function into backend_open_url() so can include fe_view */
            frontend_url_punt(url, bfile);
#endif

	    ep = makeerror(ERR_USED_HELPER);
	}
	ACCDBGN(( "Freeing parts\n"));
	url_free_parts(scheme, netloc, path, params, query, fragment);
    }

    if (ep)
    {
	*result = NULL;
	ACCDBG(("access_url: returning error %s\n", ep->errmess ));
    }

    visdelay_end();

    return ep;
}

static os_error *access_cache_init(int size)
{
    char *scrap;

    cache = &old_cache_functions;

    scrap = getenv("Wimp$ScrapDir");
    if (scrap == NULL || scrap[0] == 0)
	return makeerror(ERR_NO_SCRAP_DIR);

#ifndef FILEONLY
/*    if (strncasecomp(scrap, "cache:", 6) == 0) */
    if (config_cache_items > 48)
        cache = &cachefs_cache_functions;
#endif

    return cache->init(size);
}

os_error *access_init(int size)
{
    os_error *ep;

    access_pending_list = NULL;

    ep = access_cache_init(size);

    if (!ep && config_cookie_enable)
        cookie_read_file(config_cookie_file);

    return ep;
}

#ifdef STBWEB
int access_safe_to_quit(void)
{
    return (access_pending_list == NULL);
}
#endif

static void access_abort_item(access_handle d)
{
    alarm_removeall(d);

    switch (d->access_type)
    {
#ifndef FILEONLY
    case access_type_HTTP:
	if (d->transport_handle)
	{
	    access_http_close(d->transport_handle, http_close_DELETE_FILE | http_close_DELETE_BODY );
	    d->transport_handle = 0;
	}
	if (d->data.http.pw)
	{
	    frontend_passwd_dispose(d->data.http.pw);
	    d->data.http.pw = 0;
	}
#if SSL_UI
	if (d->data.http.ssl.fe)
	{
	    frontend_ssl_dispose(d->data.http.ssl.fe);
	    d->data.http.ssl.fe = 0;
	}
#endif
	break;
    case access_type_GOPHER:
	if (d->transport_handle)
	    access_gopher_close(d->transport_handle, http_close_DELETE_FILE);
	break;
    case access_type_FTP:
	if (d->transport_handle)
	    access_ftp_close(d->transport_handle, http_close_DELETE_FILE);
	if (d->data.ftp.pw)
	    frontend_passwd_dispose(d->data.ftp.pw);
	break;
#endif
    case access_type_FILE:
	if (d->data.file.fh)
	{
	    ACCDBGN(("access_abort_item: close %d\n", d->data.file.fh));
	    ro_fclose(d->data.file.fh);
	    d->data.file.fh = 0;
	}
	break;
    case access_type_INTERNAL:
	break;
    }
}

void access_abort_all_items(void)
{
    access_handle d;

    ACCDBG(("access: abort all items\n"));

    d = access_pending_list;

    while (d)
    {
	access_handle dd = d->next;

	access_abort_item(d);
	access_free_item(d);

	d = dd;
    }

    access_pending_list = NULL;
}

os_error *access_tidyup(void)
{
    ACCDBG(("access: tidyup\n"));

    access_abort_all_items();

#ifndef FILEONLY

    if (cache)
        cache->tidyup();

    if (config_cookie_enable)
        cookie_write_file(config_cookie_file);

    mm_free(proxy_authenticate_hdr.value);
    proxy_authenticate_hdr.value = NULL;

    mm_free(authenticate_hdr.value);
    authenticate_hdr.value = NULL;

#if SEND_HOST
    mm_free(host_hdr.value);
    host_hdr.value = NULL;
#endif

#endif
    return NULL;
}

os_error *access_abort(access_handle d)
{
    BOOL threaded;

    if ( !d )
        return NULL;

    threaded = !alarm_anypending(d);

    ACCDBG(("acc%p: abort called, caller(1)=%s\n", d, caller(1)));

    if (d->redirect)
    {
	ACCDBG(("Aborting redirection\n"));
	access_abort(d->redirect);
    }

    ACCDBG(("Removing alarms\n"));
    alarm_removeall(d);

    /* If alarms were asyncronouse we would have to use this order */
    ACCDBG(("Unlinking\n"));
    access_unlink(d);

    access_abort_item(d);

    if (threaded)
    {
	ACCDBG(("access threaded, delaying free\n"));
	d->flags |= access_PENDING_FREE;
    }
    else
    {
	access_free_item(d);
    }

    ACCDBG(("Access_abort done.\n"));

    return NULL;
}

void *access_get_headers(access_handle d)
{
    http_status_args si;

    if (!d)
	return NULL;

    si.in.handle = d->transport_handle;
    if (os_swix(HTTP_Status, (os_regset *) &si) != NULL)
	return NULL;

    return si.out.headers;
}


/* wrappers for exporting cache functions */

void access_unkeep(char *url)
{
#ifndef FILEONLY
    cache->unkeep(url);
#endif
}

void access_remove(char *url)
{
#ifndef FILEONLY
    cache->remove(url);
#endif
}

#ifdef STBWEB
void access_insert(char *url, char *file_name, cache_flags flags)
{
#ifndef FILEONLY
    cache->insert(url, file_name, flags);
#endif
}
#endif

#ifdef STBWEB
char *access_scrapfile(void)
{
#ifdef FILEONLY
    return "<Wimp$Scrap>";
#else
    return cache->scrapfile_name();
#endif
}
#endif

int access_test_cache(char *url)
{
#ifdef FILEONLY
    return 0;
#else
    return cache->test(url);
#endif
}

#ifdef STBWEB
void access_flush_cache(void)
{
#ifndef FILEONLY
    if (cache->flush)
	cache->flush();
#endif
}
#endif

#ifdef STBWEB
void access_set_header_info(char *url, unsigned date, unsigned last_modified, unsigned expires)
{
#ifndef FILEONLY
    if (cache->header_info)
	cache->header_info(url, date, last_modified, expires);
#endif
}
#endif

BOOL access_get_header_info(char *url, unsigned *date, unsigned *last_modified, unsigned *expires)
{
#ifndef FILEONLY
    if (cache->get_header_info)
	return cache->get_header_info(url, date, last_modified, expires);
#endif
    return FALSE;
}

void access_optimise_cache(void)
{
#ifndef FILEONLY
    if (cache->optimise)
	cache->optimise();
#endif
}

char *access_cache_lookup( char *url )
{
    return cache->lookup( url, FALSE );
}

void access_cache_lookup_free( char *fname )
{
    cache->lookup_free_name( fname );
}

#ifdef STBWEB
BOOL access_is_scheme_supported(const char *scheme)
{
    int i;

    for (i = 0; access_schemes[i]; i++)
    {
        if (strcasecomp(scheme, access_schemes[i]) == 0)
	    return TRUE;
    }

    if (strcasecomp(scheme, "mailto") == 0 &&
	config_proxy_mailto_on &&
	gstrans_not_null(config_proxy_mailto))
	return TRUE;

#if INTERNAL_URLS
    if (strcasecomp(scheme, "ncfrescointernal") == 0 || strcasecomp(scheme, "ncint") == 0)
	return TRUE;
#endif

    return FALSE;
}
#endif

void access_set_streaming(access_handle d, int stream)
{
    if (stream)
	d->flags &= ~access_NO_STREAM;
    else
	d->flags |= access_NO_STREAM;
}

BOOL access_was_directory( access_handle d )
{
    return ( d->flags & access_IS_DIRECTORY ) != 0;
}

int access_get_ftype(access_handle d)
{
    return d ? d->ftype : -1;
}

/* eof access.c */
