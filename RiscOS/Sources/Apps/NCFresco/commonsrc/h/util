/* -*-c-*- */

/* util.h */

#ifndef __stdlib_h
#include "stdlib.h"
#endif

#ifndef __wimp_h
#include "wimp.h"
#endif

/* FREE is a macro that does a safe free on a pointer */
/* The item remains a single (now compound) statement but care should be taken with dangling elses */
#define FREE(x)	if (x) mm_free(x)

/* definitely non-null string */
#define strsafe(s) ((s) ? (s) : "")

#define MIN(a,b)  ((a) < (b) ? (a) : (b))
#define MAX(a,b)  ((a) > (b) ? (a) : (b))

#define ROUND4(a)	(((a)+3)&~3)
#define ROUND2(a)	(((a)+1)&~1)

extern char *strdup(const char *s);
extern char *strndup(const char *s, size_t maxlen);
extern int strncasecomp(const char *a, const char *b, size_t n);
extern int strcasecomp(const char *s1, const char *s2);
extern char *strcasestr(const char *s1, const char *s2);

/* Copy a string without overrunning but make sure it is terminated */
void strncpysafe(char *s1, const char *s2, int n);
/* Cat two strings safely, given the length of the desination buffer */
void strlencat(char *s1, const char *s2, int len);

extern char *strdup_gstrans(const char *input);
extern BOOL gstrans_not_null(const char *input);

unsigned int string_hash(const char *s);

/* Try and reallocate in lower memory (returns s if it can't) */
char *optimise_string( char *s );

int suffix_to_file_type(char *suffix);
int mime_to_file_type(char *mime);

int file_type(const char *fname);
int set_file_type(char *fname, int ft);
extern int path_is_directory(char *path);

char *reduce_file_name(char *fname, char *temp, char *pathname);

void uuencode(char *in, char *out, int out_len);
void uudecode(char *bufcoded, unsigned char *bufplain, int outbufsize);

void *rma_alloc(size_t n);
void rma_free(void *p);

/* Increment the count of users that don't want the flex blocks to shift */
void flexmem_noshift(void);
/* Decrement the count of users that don't want the flex blocks to shift */
void flexmem_shift(void);

/* wait for all mouse buttons to be released */
extern int wait_for_release(int max);

#if 1
typedef struct
{
    char *name;
    char *value;
} name_value_pair;

extern int parse_http_header(char *header_data, const char *tags[], name_value_pair *output, int output_size);
#else
extern void parse_http_header(char *header_data, const char *tags[], char *values[]);
#endif

extern char *skip_space(const char *s);


extern int pmatch2(char *s, char *p);
extern int pattern_match(char *s, char *pat, int cs);
extern void translate_escaped_text(char *src, char *dest, int len);
extern void translate_escaped_form_text(char *src, char *dest, int len);

extern os_error *write_text_in_box(int handle, const char *str, void *box);
extern os_error *write_text_in_box_height(const char *str, int width, int handle, int *height);

extern int kbd_pollalt(void);

/* Manipulation of integer lists */

extern void share_span_evenly(int *list, const int start, const int span, const int width);
extern void ensure_span_evenly(int *list, const int start, const int span, const int width);


#define NOTINIT				-1

#define SET_LIST(ptr, num, val)		set_list(ptr, num, val)
#define ENSURE_RANGE(ptr, num, amt)	ensure_range(ptr, num, amt)
#define MIN_MERGE(lhs, rhs, num)	min_merge(lhs, rhs, num)
#define MAX_MERGE(lhs, rhs, num)	max_merge(lhs, rhs, num)
#define RANGE_SPREAD(ptr, num, amt)	range_spread(ptr, num, amt)
#define PRINT_LIST(ptr, num, pre)	print_list(ptr, num, pre)
#define LIST_LOWER(ptr, num, max)	list_lower(ptr, num, max)
#define SUM_LIST(ptr, num)		sum_list(ptr, num)
#define ENSURE_RIGHTWARDS(ptr, num, from, to, amt) ensure_rightwards(ptr, num, from, to, amt)

extern void list_lower(int *ptr, const int num, const int max);
extern void print_list(int *ptr, const int num, const char *pre);
extern void set_list(int *ptr, const int num, const int val);
extern void range_spread(int *ptr, const int num, const int amt);
extern void ensure_range(int *ptr, const int num, const int amt);
extern void min_merge(int *lhs, int *rhs, const int num);
extern void max_merge(int *lhs, int *rhs, const int num);
extern int sum_list(int *ptr, const int num);
extern void ensure_rightwards(int *ptr, const int num, const int from, const int to, const int amt);

extern BOOL coords_intersection(const wimp_box *box1, const wimp_box *box2, wimp_box *out_box);
extern BOOL coords_union(const wimp_box *box1, const wimp_box *box2, wimp_box *out_box);

extern unsigned long HTParseTime (const char *str);
extern int find_closest_colour(int colour, const int *palette, int n_entries);

extern os_error *file_lock(const char *file_name, int lock);

extern void shuffle_array(void *base, size_t esize, size_t xsize, size_t asize, size_t bsize);

#ifdef STBWEB
extern os_error *ensure_modem_line(void);
#else
#define ensure_modem_line() 0
#endif

/* eof util.h */
