/* > utils.c
 *
 */

#include "windows.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "swis.h"
#include "kernel.h"

#include "tboxlibs/wimp.h"
#include "tboxlibs/wimplib.h"

#include "version.h"
#include "utils.h"

#include "../inc/client.h"
#include "../inc/logapi.h"

/* --------------------------------------------------------------------------------------------- */

static void *message_block = NULL;

void utils_init(void *message_ptr)
{
    message_block = message_ptr;
}

/* --------------------------------------------------------------------------------------------- */

#ifdef DEBUG
static void add_file_and_line(_kernel_oserror *e, const char *file, int line)
{
    int len = strlen(e->errmess);
    sprintf(e->errmess + len, " (%s: %d)", file, line);
}
#endif

#ifdef DEBUG
void err_fatal_(_kernel_oserror *e, const char *file, int line)
#else
void err_fatal(_kernel_oserror *e)
#endif
{
    if (e)
    {
	_kernel_oserror ee = *e;
#ifdef DEBUG
	add_file_and_line(&ee, file, line);
	fprintf(stderr, "error 0x%x %s\n", ee.errnum, ee.errmess);
	fflush(stderr);
#endif
	wimp_report_error(&ee, Wimp_ReportError_OK, APP_NAME " " VERSION_STRING);
	exit(EXIT_FAILURE);
    }
}

#ifdef DEBUG
_kernel_oserror *err_report_(_kernel_oserror *e, const char *file, int line)
#else
_kernel_oserror *err_report(_kernel_oserror *e)
#endif
{
    if (e)
    {
	_kernel_oserror ee = *e;
#ifdef DEBUG
	add_file_and_line(&ee, file, line);
	fprintf(stderr, "error 0x%x %s\n", ee.errnum, ee.errmess);
	fflush(stderr);
#endif
	wimp_report_error(&ee, Wimp_ReportError_OK, APP_NAME " " VERSION_STRING);
    }
    return e;
}

#ifdef DEBUG
void msg_report_(char *s, const char *file, int line)
#else
void msg_report(char *s)
#endif
{
    _kernel_oserror e;

    e.errnum = 0;
    strncpy(e.errmess, s, sizeof(e.errmess));

#ifdef DEBUG
    add_file_and_line(&e, file, line);
    fprintf(stderr, "msg %s\n", e.errmess);
    fflush(stderr);
#endif
    wimp_report_error(&e, Wimp_ReportError_OK, APP_NAME " " VERSION_STRING);
}

/* --------------------------------------------------------------------------------------------- */

char *utils_msgs_lookup(char *tag)
{
    char *result;

    if (message_block &&
	_swix(MessageTrans_Lookup, _INR(0,7) | _OUT(2),
	      message_block, tag, 0, 0, 0, 0, 0, 0,
	      &result) == NULL)
    {
	return result;
    }

    return tag;
}

/* --------------------------------------------------------------------------------------------- */

void *rma_alloc(int size)
{
    char *ptr;
    if (_swix(OS_Module, _IN(0)|_IN(3)|_OUT(2),
	      6, size,
	      &ptr) != NULL)
	return NULL;

    return ptr;
}

void rma_free(void *ptr)
{
    _swix(OS_Module, _IN(0)|_IN(2), 7, ptr);
}

/* --------------------------------------------------------------------------------------------- */

void setenv(char *var_name, char *value)
{
  _swix(OS_SetVarVal, _INR(0,4),
	var_name, 
	value ? value : NULL,
	value ? strlen(value) : 0,
	0, 0);
}

/* --------------------------------------------------------------------------------------------- */

#ifdef DEBUG
/* For when linking with debug memlib */
char *caller(int level)
{
    return "";
}
#endif
    
/* --------------------------------------------------------------------------------------------- */

#define GSTRANS_BUFSIZE	1024

char *extract_and_expand(const char *in, int len)
{
    char *out, *o, *translated;
    const char *s;
    int i, n, flags;
    
    TRACE((TC_UI, TT_API4, "extract_and_expand: %p %d", in, len));

    if (len == -1)
	len = strlen(in);

    if (len == 0)
	return NULL;

    TRACE((TC_UI, TT_API4, "extract_and_expand: '%.*s'", len, in));

    /* unescaped length will never be greater than input length */
    if ((out = malloc(len + 1)) == NULL)
	return NULL;

    /* unescape string to 'out' */
    o = out;
    s = in;
    for (i = 0; i < len; i++)
    {
	int c = *s++;
	if (c == '%' && i < len - 2)
	{
	    char bytes[3];
	    bytes[0] = *s++;
	    bytes[1] = *s++;
	    bytes[2] = 0;
	    *o++ = (char)strtoul(bytes, 0, 16);
	    i += 2;
	}
	else
	{
	    *o++ = c;
	}
    }
    *o++ = 0;
    
    TRACE((TC_UI, TT_API1, "extract_and_expand: out '%s'", out));

    /* allocate buffer for GSTrans */
    translated = calloc(GSTRANS_BUFSIZE, 1);

    if (_swix(OS_GSTrans, _INR(0,2) | _OUT(2) | _OUT(_FLAGS),
	      out, translated, GSTRANS_BUFSIZE | (1<<30) | (1U<<31),	/* don't convert |<nn>, don't strip double quotes */
		 &n, &flags) != NULL ||
	n == GSTRANS_BUFSIZE ||
	(flags & _C) != 0)
    {
	/* on error or overflow then return original 'out' string */
	free(translated);
	return realloc(out, o - out);
    }

    TRACE((TC_UI, TT_API1, "extract_and_expand: translated '%s' n %d", translated, n));

    free(out);
    
    /* ensure termination and trim string */
    translated[n] = 0;
    translated = realloc(translated, n+1);

    TRACE((TC_UI, TT_API1, "extract_and_expand: translated '%s' n %d", translated, n));

    return translated;
}

/*
 * If this function changes from putting the last element on the head of the list then
 *  open_url's fresco patch needs changing
 *  something needs to take account of the duplication of Address fields that can occur.
 */

void add_element(arg_element **pargs, char *name, char *value)
{
    arg_element *el = calloc(sizeof(*el), 1);

    el->name = name;
    el->value = value;

    el->next = *pargs;
    *pargs = el;

    TRACE((TC_UI, TT_API1, "add_element: '%s' = '%s'", name, value));
}

void free_elements(arg_element **pargs)
{
    while (*pargs)
    {
	arg_element *a = *pargs;

	*pargs = a->next;

	free(a->name);
	free(a->value);
	free(a);
    }
}

void parse_args(arg_element **pargs, const char *s)
{
    do
    {
	const char *start = s + (*s == '?' || *s == '&' ? 1 : 0);
	const char *equals = strchr(start, '=');
	const char *end = strchr(start, '&');
	
	char *name, *value;
	    
	/* only use equals if within the start - end gap */
	if (end && equals > end)
	    equals = NULL;
	    
	name = extract_and_expand(start, equals ? equals - start : end ? end - start : -1);
	value = equals ? extract_and_expand(equals + 1, end ? end - (equals + 1) : -1) : NULL;

	/* this call takes over the name and value strings so they don't need freeing */
	add_element(pargs, name, value);
	
	s = end;
    }
    while (s);
}

/* --------------------------------------------------------------------------------------------- */

#define PPP_AlterSettings		0x4B620
#define PPP_Status			0x4B621

static int timeout__count = 0;

void timeout_disable(void)
{
    if (timeout__count++ == 0)
    {
	_swix(PPP_AlterSettings, _INR(0,2), 0, 0, 0);
    }
}

void timeout_enable(void)
{
    if (--timeout__count == 0)
    {
	int def_linedrop;
	if (_swix(PPP_Status, _INR(0,1) | _OUT(2), 0, 0, &def_linedrop) == NULL)
	    _swix(PPP_AlterSettings, _INR(0,2), 0, 0, def_linedrop);
    }
}

/* --------------------------------------------------------------------------------------------- */

/* eof utils.c */
