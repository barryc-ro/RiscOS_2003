/* > utils.c
 *
 */

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

/* eof utils.c */
