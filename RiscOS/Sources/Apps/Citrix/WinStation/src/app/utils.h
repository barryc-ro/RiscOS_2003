/* > utils.h
 *
 */

#ifndef __utils_h
# define __utils_h

#ifndef __kernel_h
#include "kernel.h"
#endif

/* --------------------------------------------------------------------------------------------- */

#define NOT_USED(x)	x = x
#define ROUND4(a)	(((a)+3)&~3)
#define ROUND2(a)	(((a)+1)&~1)
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))

/* --------------------------------------------------------------------------------------------- */

extern void utils_init(void *message_ptr);

/* --------------------------------------------------------------------------------------------- */

extern char *utils_msgs_lookup(char *tag);

#ifdef DEBUG

#define err_fatal(e) err_fatal_(e, __FILE__, __LINE__)
extern void err_fatal_(_kernel_oserror *e, const char *file, int line);

#define err_report(e) err_report_(e, __FILE__, __LINE__)
extern _kernel_oserror *err_report_(_kernel_oserror *e, const char *file, int line);

#define msg_report(e) msg_report_(e, __FILE__, __LINE__)
extern void msg_report_(char *s, const char *file, int line);

#else

extern void err_fatal(_kernel_oserror *e);
extern _kernel_oserror *err_report(_kernel_oserror *e);
extern void msg_report(char *s);

#endif

/* --------------------------------------------------------------------------------------------- */

extern void *rma_alloc(int size);
extern void rma_free(void *ptr);

/* --------------------------------------------------------------------------------------------- */

typedef struct arg_element arg_element;

struct arg_element
{
    arg_element *next;

    char *name;
    char *value;
};

extern void parse_args(arg_element **pargs, const char *s);
extern void free_elements(arg_element **pargs);
extern void add_element(arg_element **pargs, char *name, char *value);
extern char *extract_and_expand(const char *in, int len);

/* --------------------------------------------------------------------------------------------- */

extern void timeout_disable(void);
extern void timeout_enable(void);

/* --------------------------------------------------------------------------------------------- */

#endif

/* eof utils.h */

