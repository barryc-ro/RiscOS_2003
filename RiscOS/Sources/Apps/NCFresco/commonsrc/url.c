/* -*-c-*- */

/* url.c */

/*
 * 31/05/96: SJM: Made url_parse cope with null pointers being passed in
 * 07/06/96: SJM: Added url_canonicalise, url_canonicalise_path and built into url_join
 * 11/06/96: SJM: escape_cat and escape_to_file now use '+' for ' ' rather than %20 (see HTML2 spec)
 * 23/08/96: SJM: check for null url argument in url_join
 */

/* Manipulate URLs */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "memwatch.h"

#include "util.h"
#include "url.h"
#include "os.h"
#include "config.h"

/* static char *hexchars = "0123456789abcdef"; */
static char *hexchars = "0123456789ABCDEF";

#define MALLOC_MAGIC	0x3e694c3c

#if 0
static void strinfo(char *tag, char *s)
{
    int sl = strlen(s);

    fprintf(stderr, "String %s='%s' at 0x%p is %d long needs 0x%x bytes, prev words 0x%x, 0x%x\n",
	    tag, s, s, sl, (sl+4)&(~3), ((int*)s)[-1], ((int*)s)[-2]);

    if ((sl+4)&(~3) > (((int*)s)[-1] & 0xffffff) )
	fprintf(stderr, "\n***** String too long *****\n\n");

    if ((sl+4)&(~3) != (((int*)s)[-1] & 0xffffff) )
	fprintf(stderr, "***** String allocation not right *****\n");

    if ( ((int*)s)[-2] != MALLOC_MAGIC)
	fprintf(stderr, "\n***** Bad magic number *****\n\n");
}
#endif

int url_parse(const char *url, char **scheme, char **netloc, char **path, char **params, char **query, char **fragment)
{
    char temp[8];
    char *copy;
    char *p, *q, *qq;

    /* Strip off any leading space */
    while (*url && isspace(*url))
	url++;

    /* Strip off any leading "URL:" */
    strncpysafe(temp, url, 5);
    if (strcasecomp(temp, "URL:") == 0)
	url += 4;

    copy = strdup(url);

    /* Strip trailing white space.  Do this on the copy to avoid altering the original */
    p = copy + strlen(copy);
    while (isspace(p[-1]))
	p--;
    *p = 0;

    if (scheme)   *scheme   = NULL;
    if (netloc)   *netloc   = NULL;
    if (path)     *path     = NULL;
    if (params)   *params   = NULL;
    if (query)    *query    = NULL;
    if (fragment) *fragment = NULL;

    p = copy;

    q = strpbrk(p, ":;/#?");
    if (q && q[0] == ':')
    {
	*q = 0;
	if (p[0] && scheme)
	{
	    *scheme = strdup(p);
	}
	p = q+1;
    }

    if (p[0] == '/' && p[1] == '/')
    {
	q = strchr(p+2, '/');
	if (q)
	    *q = 0;

	if (p[2] && netloc)
	{
	    *netloc = strdup(p+2);
	}

	if (q)
	{
	    *q = '/';
	    p = q;
	}
	else
	    p += strlen(p);
    }

    q = strrchr(p, '#');	/* A slash in a freagment name needs to be escaped but we can have a hash in a path */
    qq = strrchr(p, '/');
    if (q && (qq == NULL || q > qq)) /* Have a hash and either no slash or only a slash before the hash */
    {
	if (q[1] && fragment)
	{
	    *fragment = strdup(q+1);
	}
	*q = 0;
    }

    q = strchr(p, '?');
    if (q)
    {
	/* An empty query is different to having no query at all */
	if (query)
	    *query = strdup(q+1);
	*q = 0;
    }

    q = strchr(p, ';');
    if (q)
    {
	if (q[1] && params)
	{
	    *params = strdup(q+1);
	}
	*q = 0;
    }

    if (*p && path)
    {
	*path = strdup(p);
    }

    mm_free(copy);

    return 1;
}

#define LEEWAY 64
static void url_ensure_buffer_size(char **b, int *size, int used, int more)
{
    if (*size < used + more + 1)
    {
	*size = *size + more + LEEWAY;
	*b = mm_realloc(*b, *size);
    }
}
#undef LEEWAY

char *url_unparse(const char *scheme, const char *netloc, const char *path, const char *params, const char *query, const char *fragment)
{
    char *b, *res;
    int i;
    int l;
    int buf_size;

    i = 0;
    buf_size = MAX_URL_LEN;
    b = mm_malloc(buf_size);

    if (scheme)
    {
	l = strlen(scheme);
	url_ensure_buffer_size(&b, &buf_size, i, l+1);
	strcpy(b+i, scheme);
	i += l;
	b[i++] = ':';
    }

    if (netloc)
    {
	l = strlen(netloc);
	url_ensure_buffer_size(&b, &buf_size, i, l+2);
	b[i++] = '/';
	b[i++] = '/';
	strcpy(b+i, netloc);
	i += l;
    }

    if (path)
    {
	l = strlen(path);
	url_ensure_buffer_size(&b, &buf_size, i, l);
	strcpy(b+i, path);
	i += l;
    }

    if (params)
    {
	l = strlen(params);
	url_ensure_buffer_size(&b, &buf_size, i, l+1);
	b[i++] = ';';
	strcpy(b+i, params);
	i += l;
    }

    if (query)
    {
	l = strlen(query);
	url_ensure_buffer_size(&b, &buf_size, i, l+1);
	b[i++] = '?';
	strcpy(b+i, query);
	i += l;
    }

    if (fragment)
    {
	l = strlen(fragment);
	url_ensure_buffer_size(&b, &buf_size, i, l+1);
	b[i++] = '#';
	strcpy(b+i, fragment);
	i += l;
    }

    b[i] = 0;

    res = strdup(b);

    mm_free(b);

    return res;
}

char *url_join(const char *base, const char *url)
{
    char *bscheme, *bnetloc, *bpath, *bparams, *bquery, *bfragment;
    char *scheme, *netloc, *path, *params, *query, *fragment;
    char *uscheme, *unetloc, *upath, *uquery;
    char *result;
    char buffer[MAX_URL_LEN];

    if (base == NULL || url == NULL)
    {
	char *url_out = strdup(url ? url : base);
	url_canonicalise(&url_out);
	return url_out;
    }

    url_parse(url, &scheme, &netloc, &path, &params, &query, &fragment);
    if (scheme && netloc)
    {
	char *url_out = strdup(url);
	/* Quick drop-out when there is no joining */
	url_free_parts(scheme, netloc, path, params, query, fragment);

	url_canonicalise(&url_out);
	return url_out;
    }

    url_parse(base, &bscheme, &bnetloc, &bpath, &bparams, &bquery, &bfragment);

    uscheme = scheme;
    unetloc = netloc;
    upath = path;
    uquery = query;

    if (scheme == NULL)
    {
	uscheme = bscheme;
    }
    else
    {
	if (bscheme == NULL || strcmp(scheme, bscheme) != 0 )
	{
	    goto done;
	}
    }

    if (netloc)
    {
	goto done;
    }

    unetloc = bnetloc;

    if (path && path[0] == '/')
    {
	goto done;
    }

    if (path && bpath && bpath[0] != '/' && bpath[0] != 0)
    {
	goto done;
    }

    if (path == NULL)
    {
	upath = bpath;
	if (query == NULL)
	    uquery = bquery;
	goto done;
    }
    else
    {
	/* If we got here then we have a relative path */
	char *p;

	p = strrchr(bpath, '/');
	if (p != NULL)
	    *p = 0;

	if (bpath && bpath[0])
	    strcpy(buffer, bpath); /* No '/' on the end */
	else
	    buffer[0] = 0;	/* No '/' on the end */

	p = path;

	while (p && *p)
	{
	    char *q;
	    
	    q = strchr(p, '/');
	    if (q)
		*q++ = 0;

	    if (p[0] == '.' && p[1] == '.' && p[2] == 0)
	    {
		char *s = strrchr(buffer, '/');
		if (s)
		    *s = 0;
		/* No '/' on the end */
	    }
	    else if (p[0] != '.' || p[1] != 0) /* The '/' has by now been turned to a 0 */
	    {	
		strcat(buffer, "/");
		strcat(buffer, p);
		/* No '/' on the end */
	    }
	    
	    p = q;
	}

	if ((p)	||		/* We stopped because *p was zero.  This happens when we have a trailing '/' */
	    (buffer[0] == 0))	/* Can't leave it empty */
	{
	    strcat(buffer, "/");
	}

	upath = buffer;
    }
    

 done:
    {
      char *cpath;
      if (url_canonicalise_path(upath, &cpath))
        upath = cpath;

      result = url_unparse(uscheme, unetloc, upath, params, uquery, fragment);

      mm_free(cpath);
    }
    url_free_parts(bscheme, bnetloc, bpath, bparams, bquery, bfragment);
    url_free_parts(scheme, netloc, path, params, query, fragment);
    
    return result;
}

void url_free_parts(char *scheme, char *netloc, char *path, char *params, char *query, char *fragment)
{
    if (scheme)
	mm_free(scheme);
    if (netloc)
	mm_free(netloc);
    if (path)
	mm_free(path);
    if (params)
	mm_free(params);
    if (query)
	mm_free(query);
    if (fragment)
	mm_free(fragment);
}

extern char *url_path_trans(const char *s, int topath)
{
    char buffer[MAX_FILE_NAME];
    char *p;
    const char *q;

    p = buffer;
    q = s;

    if (topath)
    {
	*p++ = '/';
    }
    else
    {
	if (*q == '/')
	    q++;
    }

    while (*q)
    {
	char c = *q++;
	if (c == '/')
	{
	    *p++ = '.';
	}
	else if (c == '.')
	{
	    *p++ = '/';
	}
	else if (c == '#')
	{
	    *p++ = '*';
	}
	else if (c == '*')
	{
	    *p++ = '#';
	}
	else
	{
	    *p++ = c;
	}
    }

    *p = 0;

    return strdup(buffer);
}


char *url_path_to_riscos(const char *s)
{
    return url_path_trans(s, 0);
}

char *url_riscos_to_path(const char *s)
{
    return url_path_trans(s, 1);
}

char *url_leaf_name(const char *url)
{
    char *scheme, *netloc, *path, *params, *query, *fragment;
    char *slash, *dot;
    char *result;

    if (url == NULL)
	return NULL;

    url_parse(url, &scheme, &netloc, &path, &params, &query, &fragment);

    if (path)
    {       
	slash = strrchr(path, '/');
	if (slash && slash[1] == 0)
	{
	    *slash = 0;
	    slash = strrchr(path, '/');
	}
    }
    else
	slash = NULL;

    if (slash == NULL)
    {
	if (path == NULL || *path == 0)
	{
	    url_free_parts(scheme, netloc, path, params, query, fragment);
	    return NULL;
	}
	else
	{
	    slash = path;
	}
    }
    else
    {
	slash++;
    }

    /* Skip over leading commas */
    while (*slash == ',')
	slash++;

    if (slash[0])
	dot = strrchr(slash+1, '.');
    else
	dot = NULL;

    if (config_truncate_suffix && dot)
	*dot = 0;

    result = strdup(slash);

    for(dot = result; *dot; dot++)
    {
	if (*dot == '.')
	    *dot = '/';
    }
   
    if (config_truncate_length > 0 && strlen(result) > config_truncate_length)
	result[config_truncate_length] = 0;
 
    url_free_parts(scheme, netloc, path, params, query, fragment);

    return result;
}

/*
 * Note these two following functions are currently only used for escaping data
 * from forms (for putting in a query string or POSTing). As such they do special
 * things with space and plus. If they were used for just encoding a bare URL
 * then you might need to treat things differently.
 */

char *url_escape_chars(const char *s, const char *escapes)
{
    char buffer[1024];
    char c;
    char *outp;

    outp = buffer;

    while ( (c = *s++) != 0)
    {
	if (c < 32 || strchr(escapes, c) != 0)
	{
	    *outp++ = '%';
	    *outp++ = hexchars[(c>>4) & 0xf];
	    *outp++ = hexchars[(c>>0) & 0xf];
	}
	else
	{
	    *outp++ = c;
	}
    }

    *outp = 0;

    return strdup(buffer);
}

#define URL_UNRESERVED_CHARS	"$-_.+!*'(),"
/* #define FORM_UNRESERVED_CHARS	"$-_.!*'()," */ /* don't want + as it is used for ' ' */ 
#define FORM_UNRESERVED_CHARS	"*-.@_"

void url_escape_cat(char *buffer, const char *in, int len)
{
    int i,j;
    int sl;

    sl = strlen(buffer);

    len -= sl;
    len--;
    if (len <= 0)
	return;

    buffer += sl;

    for(i=j=0; in[i] && len; i++,j++)
    {
	int c = in[i];
        if (c == ' ')
        {
	    buffer[j] = '+';
	    len--;
        }
	else if (isalnum(c) || strchr(FORM_UNRESERVED_CHARS, c) != 0) /* SJM: let some chars through unchanged */
	{
	    buffer[j] = c;
	    len--;
	}
	else
	{
	    if (len < 3)
	    {
		len = 0;
	    }
	    else
	    {
		buffer[j++] = '%';
		buffer[j++] = hexchars[(c>>4) & 0xf];
		buffer[j  ] = hexchars[(c>>0) & 0xf];
	    }
	}
    }
    buffer[j] = 0;
}

void url_escape_to_file(const char *s, FILE *f)
{
    char c;

    while ( (c=*s++) != 0 && !ferror(f))
    {
	if (c == ' ')
        {
	    fputc('+', f);
        }
	else if (isalnum(c) || strchr(FORM_UNRESERVED_CHARS, c) != 0) /* SJM: let some chars through unchanged */
	{
	    fputc(c, f);
	}
	else
	{
	    fputc('%', f);
	    fputc(hexchars[(c>>4) & 0xf], f);
	    fputc(hexchars[(c>>0) & 0xf], f);
	}
    }
}

BOOL url_escape_file_to_file(FILE *in, FILE *out)
{
    /*char c;*/
    int c;

    /* EOF WILL NOT FIT IN A char, SO THIS TEST IS */
    /* USELESS IF c IS A char. */

    while ( (c=fgetc(in)) != EOF && !ferror(out))
    {
	if (c == ' ')
        {
	    fputc('+', out);
        }
	else if (isalnum(c) || strchr(FORM_UNRESERVED_CHARS, c) != 0) /* SJM: let some chars through unchanged */
	{
	    fputc(c, out);
	}
	else
	{
	    fputc('%', out);
	    fputc(hexchars[(c>>4) & 0xf], out);
	    fputc(hexchars[(c>>0) & 0xf], out);
	}
    }

    return !ferror(out) && !ferror(in);
}

/*
 * Strip out an /./ or /../ type bits
 */

int url_canonicalise_path(const char *path, char **path_out_ptr)
{
    const char *s;
    char *s_out, *path_out;
    enum { state_NEW, state_SLASH, state_DOT, state_DOT_DOT } state;
    BOOL changed;
    int c;

    if (path == NULL)
    {
        *path_out_ptr = strdup("/");
        return 1;
    }

    s = path;
    s_out = path_out = strdup(path);

#if DEBUG && 0
    fprintf(stderr, "path_in %p, %s, length %d\n", path, path, strlen(path)+1);
    fprintf(stderr, "path_out %p, length %d\n", path_out, strlen(path_out)+1);
#endif
    
    state = state_NEW;
    changed = FALSE;
/*     while ((c = *s++) != 0) */
    do
    {
	c = *s++;
#if DEBUG && 0
	fprintf(stderr, "s %p s_out %p c %c state %d\n", s, s_out, c, state);
#endif
        if (c) switch (state)
        {
            case state_NEW:
                if (c == '/')
                    state = state_SLASH;
                break;

            case state_SLASH:
                if (c == '.')
                    state = state_DOT;
                else
                    state = state_NEW;
                break;

            case state_DOT:
                if (c == '/')
                {
                    s_out -= 2;
                    changed = TRUE;
                }
                else if (c == '.')
                    state = state_DOT_DOT;
                else
                    state = state_NEW;
                break;

            case state_DOT_DOT:
                if (c == '/')
                {
                    s_out -= 3;                         /* points to '/' */
                    while (s_out > path_out)
                    {
                        s_out--;
                        if (*s_out == '/')
                            break;
                    }
                    changed = TRUE;
                }
                else
                    state = state_NEW;
                break;
        }
        *s_out++ = c;
    }
    while (c);
/*     *s_out++ = 0; */

    if (!changed)
    {
        mm_free(path_out);
        path_out = NULL;
    }

    *path_out_ptr = path_out;

    return changed;
}

void url_canonicalise(char **url)
{
    char *scheme, *netloc, *path, *params, *query, *fragment;
    char *path_out;

    url_parse(*url, &scheme, &netloc, &path, &params, &query, &fragment);

    if (url_canonicalise_path(path, &path_out))
    {
        mm_free(*url);
        *url = url_unparse(scheme, netloc, path_out, params, query, fragment);
        mm_free(path_out);
    }

    url_free_parts(scheme, netloc, path, params, query, fragment);
}

/* eof url.c */
