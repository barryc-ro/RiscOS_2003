/* > cookie.c

 * http://www.netscape.com/newsref/std/cookie_spec.html
 *
 * 26/3/96: SJM: Started


 * Port numbers
 * In netscape 3
 *
 *  - A cookie cannot have an explicit port number in the DOMAIN= tag or
 *    else it will be rejected.
 *
 *  - A cookie with an explicit domain of acorn.co.uk will be sent to
 *    bungle.acorn.co.uk and bungle.acorn.co.uk:80
 *
 *  - A cookie with an implied domain of acorn.co.uk is treated as being
 *    distinct from one with an implied domain of acorn.co.uk:80 and the
 *    cookies from each will not be sent to one another.
 *
 * So a cookie with no domain specified is given a port number of PORT_NONE.
 * A cookie with a domain specified is given a port number of PORT_ANY.
 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef STBWEB_BUILD
#include "http.h"
#else
#include "../http/httppub.h"
#endif

#include "os.h"
#include "memwatch.h"
#include "url.h"
#include "util.h"
#include "cookie.h"
#include "version.h"

typedef struct cookie_item cookie_item;
typedef struct cookie_domain cookie_domain;

#define cookie_SECURE		0x00000001	/* must be sent over secure channel */

#define PORT_NONE		(-1)
#define PORT_ANY		(-2)
#define PORT_UNSPECIFIED	(-3)

struct cookie_item
{
    cookie_item *next;          /* link to next cookie */

    char *name;                 /* name of this cookie */
    char *value;                /* value of this cookie */
    char *path;                 /* path to match to decide on sending the cookie */

    int flags;
    unsigned long expires;             /* when does this expire, UINT_MAX means at end of session */
    unsigned long used;		/* time we last used this cookie */
};

struct cookie_domain
{
    cookie_domain *next;        /* link to next domain */
    char *domain;               /* as originally passed in the set-cookie header */
    int port;			/* port number originally specified with domain or PORT_UNSPECIFIED */
    int cookie_count;           /* number of cookies in the cookie_list */
    cookie_item *cookie_list;   /* cookies in the domain */
};

/* maximum numbers defined in netscape spec */
/* we trim the cookie size when we receive it originally */
/* we check the numbers whenever we add a new cookie */
/* if either of the numbers are exceeded then this triggers an expiry operation */

#define MAX_COOKIES             300
#define MAX_COOKIES_PER_DOMAIN  20
#define MAX_COOKIE_SIZE         (4*1024)

/* ---------------------------------------------------------------------------------------------------- */

static cookie_domain *domain_list = NULL;   /* top level unordered list of domains */
static int cookie_count = 0;                /* total count of cookies stored */

/* these top level domains mean we only check for 2 dots in a domain name rather than 3 */
static const char *special_domains[] = { "COM", "EDU", "GOV", "INT", "MIL", "NET", "ORG", NULL };

/* ---------------------------------------------------------------------------------------------------- */

/*
 * get the time correcting for the fact that time() returns localtime not gmtime
 */

static unsigned long get_time(void)
{
    time_t t = time(NULL);
    struct tm tt, *ttp = gmtime(&t);

/*     CKIDBG(("cookie: time %x gmtime @%p\n", t, ttp)); */

    if (ttp == NULL)
	return t;

/*     CKIDBG(("cookie: dst %d\n", ttp->tm_isdst)); */

/*     if (!ttp->tm_isdst) */
/* 	return t; */

    tt = *ttp;
    tt.tm_isdst = 0;

    return (unsigned long) mktime(&tt);
}

/* ---------------------------------------------------------------------------------------------------- */

/*
 * if netloc has a port number then insert a null over the :
 * and return the port number.
 */

static int strip_port_from_domain(char *netloc)
{
    int port;
    char *colon;

    port = PORT_UNSPECIFIED;

    if (netloc && (colon = strrchr(netloc, ':')) != NULL)
    {
	port = atoi(colon+1);
	*colon = '\0';
    }

    return port;
}

/* ---------------------------------------------------------------------------------------------------- */

static void cookie_free(cookie_item *c)
{
    if (c)
    {
	CKIDBG(("cookie: freeing 0x%p expiry=%lx %s=%s\n", c, c->expires, c->name, c->value));

	mm_free(c->name);
        mm_free(c->value);
        mm_free(c->path);
        mm_free(c);
    }
}

static void cookie_unlink_and_free(cookie_domain *d, cookie_item *last_c, cookie_item *c)
{
    if (last_c)
        last_c->next = c->next;
    else
        d->cookie_list = c->next;

    cookie_free(c);
    d->cookie_count--;
    cookie_count--;
}

/*
 * Check the cookies in this domain to see if any hav expired, if so remove them.
 * If must_free_one is true then if no others are deleted we must free the oldest
 */

static void cookie_check_expiry_domain(cookie_domain *d, unsigned long now, BOOL must_free_one)
{
    cookie_item *c, *last;
    cookie_item *oldest_c = NULL, *oldest_last = NULL;

    CKIDBG(("cookie: check expiry now=%lx domain=%s n=%d\n", now, d->domain, d->cookie_count));

    c = d->cookie_list;
    last = NULL;
    while (c)
    {
        cookie_item *next = c->next;

	if (now >= c->expires)
	{
            cookie_unlink_and_free(d, last, c);
	    must_free_one = FALSE;
	}
        else
	{
	    if (oldest_c == NULL || oldest_c->used > c->used)
	    {
		oldest_last = last;
		oldest_c = c;
	    }

	    last = c;
	}

        c = next;
    }

    if (must_free_one)
	cookie_unlink_and_free(d, oldest_last, oldest_c);
}

#if 0
/*
 * Check all domains for expired cookies
 */

static void cookie_check_expiry(unsigned long now)
{
    cookie_domain *d;

    CKIDBG(("cookie: check expiry\n"));

    for (d = domain_list; d; d = d->next)
        cookie_check_expiry_domain(d, now, FALSE);
}
#endif

/* ---------------------------------------------------------------------------------------------------- */

/*
 * Find a matching domain name in the global domain list
 */

static cookie_domain *find_domain(const char *domain, int port)
{
    cookie_domain *d;

    /* search for the domain */
    for (d = domain_list; d; d = d->next)
    {
        if (strcasecomp(domain, d->domain) == 0 &&
	    port == d->port)
            break;
    }

    return d;
}

/*
 * See 'subdomain' is a 'subdomain' of 'domain'
 *
 * eg om92.acorn.co.uk is a subdomain of acorn.co.uk
 *
 */

static BOOL tail_compare(const char *subdomain, const char *domain)
{
    int len1 = strlen(domain);
    int len2 = strlen(subdomain);
    int start = len2 - len1;

    if (start < 0)		/* subdomain is shorter than set domain */
	return FALSE;

    if (start == 0)		/* subdomain is same length as set domain */
	return strcasecomp(subdomain, domain) == 0;

    if (domain[0] != '.')
	if (subdomain[start-1] != '.')	/* Not really a subdomain just a name prefix */
	    return FALSE;

    return strcasecomp(&subdomain[start], domain) == 0;
}

/*
 * Search a list of strings for a matching string
 */

static int match_strings(const char *match, const char *strings[])
{
    int i;
    for (i = 0; strings[i]; i++)
    {
        if (strcasecomp(match, strings[i]) == 0)
            return i;
    }
    return -1;
}

/* ---------------------------------------------------------------------------------------------------- */

/*
 * This routine must do several things
 * If domain is set then
 *   check it is part of the hosts domain
 *   check it has requisite number of components
 * If domain is unset then
 *   use hosts domain
 * If path is unset
 *   use path of url (don't strip leaf name off)

 * It takes any pointer in and writes out malloced pointers
 */

static BOOL get_domain_and_path(char *domain_in, const char *path_in, const char *url, char **domain_out, char **path_out, int *port_out)
{
    char *scheme, *netloc, *path, *params, *query, *frag;
    int url_port;

#if 0	/* not used now */
    /* if null url passed in then we were loading from the cookie file */
    /* there shouldn't be any errors in this... */
    if (!url)
    {
        if (domain_in == NULL || path_in == NULL)
        {
            CKIDBG(( "cookie: null domain or path on reading from file\n"));
            return FALSE;
        }

        *domain_out = strdup(domain_in);
	*port_out = strip_port_from_domain(*domain_out);

        *path_out = strdup(path_in);
        return TRUE;
    }
#endif

    url_parse((char *)url, &scheme, &netloc, &path, &params, &query, &frag);

    if (netloc == NULL)
    {
	CKIDBG(( "cookie: null domain int input URL\n"));
	url_free_parts(scheme, netloc, path, params, query, frag);
	return FALSE;
    }

    /* strip off the port number from the domain */
    url_port = strip_port_from_domain(netloc);

    /* if we have a domain possed in */
    if (domain_in)
    {
        const char *s;
        const char *top_level;
        int c, dots;
	int port;

	port = strip_port_from_domain(domain_in);

	/* if the cookie domain has no port specified then match otherwise only if the ports are the same */
        if ((port == PORT_UNSPECIFIED || port == url_port) &&
	    !tail_compare(netloc, domain_in))
        {
            CKIDBG(( "cookie: attempt to set cookie for wrong domain\n  domain '%s'\n  cookie '%s'\n", netloc, domain_in));
            return FALSE;
        }

        /* count the dots and find the top level domain */
	/* skip the first character as it confuses the count */
        s = domain_in + 1;
        top_level = NULL;
        dots = 0;

	for (c = *s++; c; c = *s++)
            if (c == '.')
            {
                top_level = s;
                dots++;
            }

        /* check right number of dots */
        if (dots == 0 || (dots == 1 && match_strings(top_level, special_domains) == -1))
        {
            CKIDBG(( "cookie: too high level domain '%s' %d dots\n", domain_in, dots));
	    url_free_parts(scheme, netloc, path, params, query, frag);
            return FALSE;
        }

        *domain_out = strdup(domain_in);
	*port_out = port == PORT_UNSPECIFIED ? PORT_ANY : port;
    }
    else
    {
        *domain_out = netloc;
	*port_out = url_port == PORT_UNSPECIFIED ? PORT_NONE : url_port;
        netloc = NULL;
    }

    if (path_in)
    {
        *path_out = strdup(path_in);
    }
    else
    {
	/* 30/10/96: Disagreement of specs here
	 * cookie spec says use the full path of the document carrying the cookie
	 * netscape/msie operation is to use root /
	 * state management spec is to use the parent dir of the document
	 * we'll use current practise for now
	 */

#if 0	/* ANC-00358: leaf name shouldn't be stripped off after all */
	/* strip off the leaf name from the document path */
        char *slash = strrchr(path, '/');
        if (slash && slash != path && slash != path + strlen(path))
            *slash = '\0';
#endif
#if 0
        *path_out = path;
        path = NULL;
#endif
	*path_out = strdup("/");
    }

    url_free_parts(scheme, netloc, path, params, query, frag);

    return TRUE;
}

/* ---------------------------------------------------------------------------------------------------- */

static void cookie_add(char *name, char *value, char *domain, char *path, unsigned long expires, int secure, unsigned long used, int port)
{
    cookie_domain *d;
    cookie_item *c, *last_c, *new_c;
    unsigned long time_now = get_time();

    CKIDBG(("cookie: add name '%s' value '%s' domain '%s' path '%s' expires %x secure %d\n",
        strsafe(name), strsafe(value), strsafe(domain), strsafe(path), (int)expires, secure));
    CKIDBG(("cookie: current time %lx\n", time_now));

    d = find_domain(domain, port);

    if (d)
    {
        CKIDBG(( "cookie: found domain\n"));

        /* if found then free the domain we passed in */
        mm_free(domain);
    }
    else
    {
        CKIDBG(( "cookie: new domain\n"));

        /* if not found then allocate a new domain */
        d = mm_calloc(sizeof(cookie_domain), 1);
        d->domain = domain;
	d->port = port;

        /* add domain on the front of the list */
        d->next = domain_list;
        domain_list = d;
    }

    /* search for the right insert point */
    last_c = NULL;
    new_c = NULL;
    for (c = d->cookie_list; c; c = c->next)
    {
        int cmp = strcasecomp(path, c->path);

        if (cmp > 0)
        {
            /* add new cookie before current point */
            if (expires > time_now)
            {
                new_c = mm_calloc(sizeof(cookie_item), 1);
                new_c->next = c;
                CKIDBG(( "cookie: add before '%s'\n", c->path));
            }
            else
	    {
                CKIDBG(( "cookie: already expired (within) %ld %ld\n", expires, time_now));

		mm_free(name);
		mm_free(value);
		mm_free(path);
	    }
            break;
        }

        /* if the path matches */
        if (cmp == 0)
        {
            /* if name is the same then overwrite new values */
            /* unless it has expired (ie explicit delete) */
            if (strcasecomp(c->name, name) == 0)
            {
                mm_free(name);
                mm_free(path);

                if (expires <= time_now)
                {
                    mm_free(value);
                    cookie_unlink_and_free(d, last_c, c);

                    CKIDBG(( "cookie: already expired (delete '%s')\n", name));
                }
                else
                {
                    mm_free(c->value);
                    c->value = value;

                    c->expires = expires;
		    if (secure)
			c->flags |= cookie_SECURE;
		    else
			c->flags &= ~cookie_SECURE;
		    c->used = used ? used : time_now;

                    CKIDBG(( "cookie: overwrite '%s' with '%s'\n", c->name, c->value));
                }
                /* c != NULL and new_c = NULL so nothing more will happen */
                break;
            }
            /* otherwise go on to the next one */
        }

        last_c = c;
    }

    /* if reached the end then allocate a new cookie */
    if (c == NULL)
    {
        CKIDBG(( "cookie: add at end\n"));

        new_c = mm_calloc(sizeof(cookie_item), 1);
    }

    /* if we have a new cookie then fill in and link in */
    if (new_c)
    {
        if (expires <= time_now)
        {
            CKIDBG(( "cookie: already expired (at end) %ld %ld\n", expires, time_now));

            mm_free(name);
            mm_free(value);
            mm_free(path);
        }
        else
        {
            CKIDBG(( "cookie: link in\n"));

            d->cookie_count++;
            cookie_count++;

            new_c->name = name;
            new_c->value = value;
            new_c->path = path;
            new_c->expires = expires;
            new_c->flags = secure ? cookie_SECURE : 0;
	    new_c->used = used ? used : time_now;

            if (last_c)
                last_c->next = new_c;
            else
                d->cookie_list = new_c;

	    /* check for any cookies expiring */
	    cookie_check_expiry_domain(d, time_now, d->cookie_count > MAX_COOKIES_PER_DOMAIN);

#if 0
	    /* check for exceeding limits */
            if (cookie_count > MAX_COOKIES)
                cookie_check_expiry(time_now);
#endif
        }
    }
}

/*
 * The cookies are ordered in alphabetical order by path
 * with more qualified paths first.
 *   eg /gerbil/next, /gerbil, /fred, /
 * the names for the same path are unordered.
 */

/* ---------------------------------------------------------------------------------------------------- */

void cookie_received_header(const char *header, const char *url)
{
    static const char *cookie_tags[] = { "DOMAIN", "PATH", "EXPIRES", "SECURE", NULL };
    name_value_pair vals[5];

    char *header_copy = strdup(header);

    char *name, *value, *domain, *path;
    unsigned long expires;
    int secure;
    int port;

    CKIDBG(( "cookie: header '%s' from '%s'\n", strsafe(header), strsafe(url)));

    parse_http_header(header_copy, cookie_tags, vals, sizeof(vals)/sizeof(vals[0]));

    CKIDBG(( "cookie: vals '%s=%s' domain '%s' path '%s' expires '%s' '%s'\n",
        strsafe(vals[4].name), strsafe(vals[4].value), strsafe(vals[0].value), strsafe(vals[1].value), strsafe(vals[2].value), vals[3].value ? "secure" : ""));

    /* must have a name=value to be valid */
    if (vals[4].name == NULL)
    {
        mm_free(header_copy);
        return;
    }

    /* get the domain and path, checking that its valid */
    if (!get_domain_and_path(vals[0].value, vals[1].value, url, &domain, &path, &port))
    {
        mm_free(header_copy);
        return;
    }

    /* check max size and truncate if exceeded */
    {
	int nlen = strlen(vals[4].name);
	int vlen = strlen(vals[4].value);

	if (nlen > MAX_COOKIE_SIZE)
	{
	    vals[4].name[MAX_COOKIE_SIZE] = '\0';
	    vals[4].value[0] = '\0';
	}
	else if (nlen+1+vlen > MAX_COOKIE_SIZE)
	{
	    vals[4].value[MAX_COOKIE_SIZE-(nlen+1)] = '\0';
	}
    }

    name = strdup(vals[4].name);
    value = strdup(vals[4].value);

    /* decode the expiry time and secure flag */
    expires = vals[2].value ? HTParseTime(vals[2].value) : ULONG_MAX;
    secure = vals[3].value != NULL;

    mm_free(header_copy);

    /* add to list */
    cookie_add(name, value, domain, path, expires, secure, 0, port);
}

/* ---------------------------------------------------------------------------------------------------- */

static int cookie_len = 0;
static http_header_item cookie_http_header =
{
    NULL,
    "Cookie",
    NULL
};

http_header_item *cookie_add_headers(http_header_item *hlist, const char *url, int secure)
{
    cookie_domain *d;
    cookie_item *c;
    char *scheme, *netloc, *path, *params, *query, *frag;
    int len_netloc, len_path;
    unsigned long time_now = get_time();
    BOOL quote_strings = FALSE;
    int port;

    CKIDBG(( "cookie: get cookies for '%s'\n", url));

    /* free last used cookie value */
    mm_free(cookie_http_header.value);
    cookie_http_header.value = NULL;
    cookie_len = 0;

    /* check which URL we are coming from */
    url_parse((char *)url, &scheme, &netloc, &path, &params, &query, &frag);

    if (netloc == NULL)
        netloc = strdup("");
    if (path == NULL)
        path = strdup("/");

    port = strip_port_from_domain(netloc);

    len_netloc = strlen(netloc);
    len_path = strlen(path);

    for (d = domain_list; d; d = d->next)
    {
        int start;
	cookie_item *last_c;

        /* check host */
	if (d->port == PORT_ANY ||
	    d->port == port ||
	    (d->port == PORT_NONE && port == PORT_UNSPECIFIED))
	{
	    start = len_netloc - strlen(d->domain);
	    if (start < 0 || strcasecomp(d->domain, netloc + start) != 0)
		continue;
	}
	else
	    continue;

        CKIDBG(( "cookie: found matching domain '%s'\n", d->domain));

        for (c = d->cookie_list, last_c = NULL; c; last_c = c, c = c->next)
        {
            int len;
	    char *quote;

            /* check secure first as it's quickest */
            if ((c->flags & cookie_SECURE) && !secure)
            {
                CKIDBG(( "cookie: requires secure channel\n"));
                continue;
            }

	    if (c->expires <= time_now)
	    {
                CKIDBG(( "cookie: expired\n"));
		cookie_unlink_and_free(d, last_c, c);

		/* reset c so that the loop will still work */
		/* if no more then break out of inner loop */
		c = last_c ? last_c : d->cookie_list;
		if (c == NULL)
		    break;

                continue;
	    }

            /* check path - didn't use to be case-sensitive! */
            if (strncmp(c->path, path, strlen(c->path)) != 0)
            {
                CKIDBG(( "cookie: path '%s' doesn't match '%s'\n", c->path, path));
                continue;
            }

            CKIDBG(( "cookie: send %s=%s\n", c->name, c->value));

	    /* update the last used time */
	    c->used = time_now;

	    /* add cookie to string */
            len = strlen(c->name) + 1 + strlen(c->value);

	    /* if we already have a cookie then add 2 for '; ' */
            if (cookie_len)
                len += 2;

	    if (quote_strings)
	    {
		len += 2;
		quote = "\"";
	    }
	    else
		quote = "";

            if (cookie_http_header.value)
                cookie_http_header.value = mm_realloc(cookie_http_header.value, cookie_len + len + 1);
            else
                cookie_http_header.value = mm_malloc(cookie_len + len + 1);

            sprintf(cookie_http_header.value + cookie_len, "%s%s=%s%s%s", cookie_len ? "; " : "", c->name, quote, c->value, quote);

            cookie_len += len;
        }
    }

    url_free_parts(scheme, netloc, path, params, query, frag);

    /* if we added any cookies then add to header_item list */
    if (cookie_http_header.value)
    {
        cookie_http_header.next = hlist;
        hlist = &cookie_http_header;
    }

    return hlist;
}

void cookie_free_headers(void)
{
    mm_free(cookie_http_header.value);
    cookie_http_header.value = NULL;
}

#ifdef STBWEB
static void cookie_optimise_domain(cookie_domain *d)
{
    cookie_item *c, *last_c;

    CKIDBG(("cookie: optimise domain '%s'\n", d->domain));

    for (last_c = NULL, c = d->cookie_list; c; last_c = c, c = c->next)
    {
	if (optimise_block((void **)&c, sizeof(*c)))
	{
	    if (last_c)
		last_c->next = c;
	    else
		d->cookie_list = c;
	}

 	c->name = optimise_string(c->name);
 	c->value = optimise_string(c->value);
 	c->path = optimise_string(c->path);
    }
}

void cookie_optimise(void)
{
    cookie_domain *d, *last_d;

    CKIDBG(("cookie: optimise\n"));

    for (last_d = NULL, d = domain_list; d; last_d = d, d = d->next)
    {
	if (optimise_block((void **)&d, sizeof(*d)))
	{
	    if (last_d)
		last_d->next = d;
	    else
		domain_list = d;
	}

 	d->domain = optimise_string(d->domain);
	
	cookie_optimise_domain(d);
    }
}
#endif

/* ---------------------------------------------------------------------------------------------------- */

#ifdef STBWEB
void cookie_dispose_all(void)
{
    cookie_domain *d = domain_list;
    while (d)
    {
        cookie_domain *next = d->next;

        /* free all the cookies */
        cookie_check_expiry_domain(d, UINT_MAX, FALSE);

	mm_free(d->domain);
        mm_free(d);
        d = next;
    }
    domain_list = NULL;
}
#endif

/* ---------------------------------------------------------------------------------------------------- */

#define SEPARATORS	" \t\n\r"

static void cookie_read_line(char *buf)
{
    char *s;
    char *name, *value, *domain, *path;
    unsigned long expires, used;
    BOOL secure;
    int port;

    name = strdup(strtok(buf, SEPARATORS));
    value = strdup(strtok(NULL, SEPARATORS));

    /* special logic to store difference between no ports with and without explicit domains */
    domain = strdup(strtok(NULL, SEPARATORS));
    if ((s = strrchr(domain, ':')) != NULL)
    {
	port = s[1] == '\0' ? PORT_NONE : atoi(s+1);
	s[0] = '\0';
    }
    else
    {
	port = PORT_ANY;
    }

    path = strdup(strtok(NULL, SEPARATORS));

    /* The flags word, capital means set lower case means clear */
    s = strtok(NULL, SEPARATORS);
    secure = strchr(s, 'S') != NULL;

    s = strtok(NULL, SEPARATORS);
    expires = strtoul(s, NULL, 16);

    s = strtok(NULL, SEPARATORS);
    used = strtoul(s, NULL, 16);

    cookie_add(name, value, domain, path, expires, secure, used, port);
}

/* ---------------------------------------------------------------------------------------------------- */

/*#define COOKIE_FILE "<"PROGRAM_NAME"$Cookies>" */

#define FORMAT_IDENT    "Format: "
#define FORMAT_NUMBER   2

void cookie_write_file(const char *file_name)
{
    FILE *f;

    CKIDBG(( "cookie: write '%s'\n", file_name));

    if (!gstrans_not_null(file_name))
	return;

    f = mmfopen(file_name, "w");
    if (f)
    {
        cookie_domain *d;
        cookie_item *c;

        time_t now = get_time();
	struct tm *gmtt = gmtime(&now);

        fprintf(f, "# "PROGRAM_NAME" Cookie file\n");
        fprintf(f, "# Written on %s %s", gmtt ? "GMT" : "Local", gmtt ? asctime(gmtt) : asctime(localtime(&now)));
	fprintf(f, "Format: 2\n");

        /* write each cookie out in the header format */
        for (d = domain_list; d; d = d->next)
        {
            CKIDBG(( "cookie: domain '%s'\n", d->domain));

            for (c = d->cookie_list; c; c = c->next)
            {
                CKIDBG(( "cookie: path '%s'\n", c->path));

                /* if no expiry then they are not saved and vanish at session end */
                /* if they have expired also don't save them */
                if (c->expires != ULONG_MAX && c->expires > (unsigned long)now)
                {
		    /* Format two writer */
#if 1
                    fprintf(f, "%s\t%s\t%s", c->name, c->value, d->domain);
		    switch (d->port)
		    {
		    case PORT_ANY:
			break;

		    case PORT_UNSPECIFIED:
			fprintf(f, ":"); /* use a blank : to tell the explicit domain case */
			break;

		    default:
			fprintf(f, ":%d", d->port);
			break;
		    }

		    fprintf(f, "\t%s\t%c\t%08lx\t%08lx\n",
			    c->path,
			    c->flags & cookie_SECURE ? 'S' : 's',
			    c->expires, c->used);
#endif
		    /* Format two writer end */

		    /* Format one writer */
#if 0
#if 0
    	            struct tm *gmt;
    	            char buf[40];
#endif
                    fprintf(f, "%s=\"%s\"; domain=\"%s\"; path=\"%s\"", c->name, c->value, d->domain, c->path);
#if 0
		    gmt = gmtime(&c->expires);
        	    if (gmt)
        	        strftime(buf, sizeof(buf), "\"%a, %d %b %Y %H:%M:%S GMT\"", gmt);
        	    fputs("; expires=", f);
        	    fputs(buf, f);
#else
		    fprintf(f, "; expires=%08x", c->expires);
#endif
                    if (c->secure)
                        fputs("; secure", f);
                    fputc('\n', f);
#endif
		    /* Format one writer end */
                }
            }
        }

        mmfclose(f);
    }

    CKIDBG(( "cookie: write close\n"));
}

#define BUF_SIZE (5*1024)

void cookie_read_file(const char *file_name)
{
    FILE *f;

    CKIDBG(( "cookie: read '%s'\n", file_name));

    if (file_type(file_name) == -1)
	return;

    f = mmfopen(file_name, "r");
    if (f)
    {
        char *buf = mm_malloc(BUF_SIZE);
	int version = 0;

        do
        {
            if (fgets(buf, BUF_SIZE, f) == NULL)
                break;

            if (buf[0] == '#')
                continue;

            if (strncasecomp(buf, FORMAT_IDENT, sizeof(FORMAT_IDENT)-1) == 0)
            {
                version = atoi(&buf[sizeof(FORMAT_IDENT)-1]);
                if (version > FORMAT_NUMBER)
		{
		    usrtrc( "cookie: file version too high\n");
                    break;
		}
		CKIDBG(( "cookie: format %d\n", version));
                continue;
            }

            if (buf[0]) switch (version)
	    {
	    case 1:
                cookie_received_header(buf, NULL);
		break;

	    case 2:
		cookie_read_line(buf);
		break;
	    }
        }
        while (!feof(f));

        mm_free(buf);
        mmfclose(f);
    }

    CKIDBG(( "cookie: read close\n"));
}

/* ---------------------------------------------------------------------------------------------------- */

/* eof cookie.c */
