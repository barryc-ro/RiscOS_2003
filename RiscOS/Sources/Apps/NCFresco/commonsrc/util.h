/* -*-c-*- */

/* util.h */

#ifndef __stdlib_h
#include "stdlib.h"
#endif

#ifndef __stdio_h
#include "stdio.h"
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

#define NOT_USED(a)	a=a

extern char *strdup(const char *s);
#define mm_strdup strdup
extern char *strndup(const char *s, size_t maxlen);
extern int strncasecomp(const char *a, const char *b, size_t n);
extern int strcasecomp(const char *s1, const char *s2);
extern char *strcasestr(const char *s1, const char *s2);
extern BOOL strnearly( const char *s1, size_t len1,
                       const char *s2, size_t len2, size_t hownear );
extern char *strlwr(char *s);

extern char *strcatx_with_leeway(char *s, const char *s1, int leeway);
extern char *strcatx1(char *s, const char *s1);
extern char *strcatx(char *s, const char *s1);
extern char *strtrim(char *s);

/* Copy a string without overrunning but make sure it is terminated */
void strncpysafe(char *s1, const char *s2, int n);
/* Cat two strings safely, given the length of the desination buffer */
void strlencat(char *s1, const char *s2, int len);

extern char *strdup_gstrans(const char *input);
extern BOOL gstrans_not_null(const char *input);
extern char *strdup_unescaped(const char *src);

unsigned int string_hash(const char *s);

/* Try and reallocate in lower memory (returns s if it can't) */
char *optimise_string( char *s );

/* Try and reallocate in lower memory, returns TRUE if it could */
extern BOOL optimise_block( void **pblock, int size );

int suffix_to_file_type(const char *suffix);
int mime_to_file_type(const char *mime);

extern int file_and_object_type_real(const char *fname, int *obj_type);
extern int file_and_object_type(const char *fname, int *obj_type);
extern int file_type_real(const char *fname);
extern int file_type(const char *fname);
extern int file_last_modified(const char *fname);
extern os_error *file_copy( const char *from, const char *to ); /* force */

int set_file_type(const char *fname, int ft);
extern int path_is_directory(const char *path);
extern char *get_file_type_name(int ftype);
extern char *get_plugin_type_name(int ftype);

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

typedef struct
{
    char *name;
    char *value;
} name_value_pair;

extern int parse_http_header(char *header_data, const char *tags[], name_value_pair *output, int output_size);

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

extern int make_month( const char *str );   /* returns 1-12, or 0 for unknown */
extern unsigned long HTParseTime (const char *str);
extern int find_closest_colour(int colour, const int *palette, int n_entries);

extern os_error *file_lock(const char *file_name, int lock);

extern void shuffle_array(void *base, size_t esize, size_t xsize, size_t asize, size_t bsize);

#ifdef STBWEB
extern os_error *ensure_modem_line(void);
#else
#define ensure_modem_line() 0
#endif

/*extern void null_free(void **vpp);*/
#define STRING_FREE(s) nullfree((void **)(s));

typedef enum
{
    key_action_NO_ACTION,
    key_action_NEWLINE,			/* newline, submit if only this is the onyl TEXT/PASSWD */
    key_action_NEWLINE_SUBMIT_ALWAYS,	/* newline, always submit if TEXT or PASSWD */
    key_action_NEWLINE_SUBMIT_LAST,	/* newline, submit if the last TEXT or PASSWD */
    key_action_DELETE_LEFT,
    key_action_DELETE_RIGHT,
    key_action_DELETE_ALL,
    key_action_DELETE_ALL_AREA,
    key_action_DELETE_TO_END,
    key_action_DELETE_TO_START,
    key_action_LEFT,
    key_action_RIGHT,
    key_action_UP,
    key_action_DOWN,
    key_action_START_OF_LINE,
    key_action_END_OF_LINE,
    key_action_START_OF_AREA,
    key_action_END_OF_AREA,
    key_action_LEFT_OR_OFF,
    key_action_RIGHT_OR_OFF
} input_key_action;

typedef struct
{
    int key;
    input_key_action action;
} input_key_map;



extern void set_input_key_map(input_key_map *map);
extern input_key_action lookup_key_action(int key);

extern int cmos_op(int bit_start, int n_bits, int new_val, BOOL write);

extern int nvram_read(const char *tag, int *val);
extern int nvram_write(const char *tag, int new_val);

/* base sounds defined in soundfx module */
#define soundfx_WIN_OPEN	0x01
#define soundfx_WIN_CLOSE	0x02
#define soundfx_TB_OPEN		0x03
#define soundfx_TB_CLOSE	0x04
#define soundfx_ACTION_OK	0x05
#define soundfx_ACTION_FAIL	0x06
#define soundfx_BROWSER_BACK	0x00010001
#define soundfx_BROWSER_HOME	0x00010002
#define soundfx_BROWSER_SOUND_ON	0x00010003
#define soundfx_BROWSER_SOUND_OFF	0x00010004
#define soundfx_BROWSER_FORWARD	0x00010005

/* mappgins from slightly more generic browser conditions to the above map */
#define snd_NONE		0

#define snd_TOOLBAR_SHOW	soundfx_TB_OPEN
#define snd_TOOLBAR_HIDE	soundfx_TB_CLOSE
#define snd_TOOLBAR_SHOW_SUB	0
#define snd_TOOLBAR_HIDE_SUB	0

#define snd_HISTORY_SHOW	soundfx_ACTION_OK
#define snd_HISTORY_BACK	soundfx_BROWSER_BACK
#define snd_HISTORY_FORWARD	soundfx_BROWSER_FORWARD

#define snd_HOTLIST_SHOW	soundfx_ACTION_OK
#define snd_HOTLIST_DELETE_SHOW	soundfx_ACTION_OK
#define snd_HOTLIST_ADD		soundfx_ACTION_OK
#define snd_HOTLIST_REMOVE	soundfx_ACTION_OK

#define snd_HOME		soundfx_BROWSER_HOME
#define snd_SEARCH		soundfx_ACTION_OK
#define snd_OFFLINE		soundfx_ACTION_OK
#define snd_ABORT		soundfx_ACTION_OK
#define snd_RELOAD		soundfx_ACTION_OK

#define snd_OPEN_URL_SHOW	soundfx_ACTION_OK
#define snd_HELP_SHOW		soundfx_ACTION_OK
#define snd_PASSWORD_SHOW	soundfx_ACTION_OK
#define snd_INFO_SHOW		soundfx_ACTION_OK
#define snd_FIND_SHOW		soundfx_ACTION_OK
#define snd_DISPLAY_OPTIONS_SHOW 0
#define snd_PRINT_OPTIONS_SHOW	soundfx_ACTION_OK
#define snd_PRINT_FRAMES_SHOW	soundfx_ACTION_OK
#define snd_RELATED_SHOW	soundfx_ACTION_OK
#define snd_URL_SHOW		soundfx_ACTION_OK

#define snd_SCROLL_LINE		0
#define snd_SCROLL_PAGE		0
#define snd_SCROLL_LIMIT	0
#define snd_SCROLL_FAILED	soundfx_ACTION_FAIL

#define snd_CHANGE_FRAME	0
#define snd_CHANGE_HIGHLIGHT	0

#define snd_WARN_NO_SCROLL	soundfx_ACTION_FAIL
#define snd_WARN_NO_FIELD	soundfx_ACTION_FAIL
#define snd_WARN_BAD_KEY	soundfx_ACTION_FAIL

#define snd_MENU_SHOW		soundfx_ACTION_OK
#define snd_MENU_HIDE		soundfx_ACTION_OK
#define snd_MENU_SELECT		soundfx_ACTION_OK

#define snd_CHECKBOX_TOGGLE	0
#define snd_RADIO_TOGGLE	0
#define snd_FORM_SUBMIT		0
#define snd_FORM_RESET		0

#define snd_LINK_FOLLOW		0

#define snd_MODE_MAP_START	0
#define snd_MODE_MAP_END	0

#define snd_PRINT_START		0
#define snd_PRINT_END		0

#define snd_BEEPS_ON		0
#define snd_BEEPS_OFF		0

#define snd_SOUND_ON		soundfx_BROWSER_SOUND_ON
#define snd_SOUND_OFF		soundfx_BROWSER_SOUND_OFF

#define snd_ERROR		soundfx_ACTION_FAIL

#define snd_FIND_FAILED		soundfx_ACTION_FAIL

#define snd_GENERIC_BACK	soundfx_BROWSER_BACK

typedef int sound_event_t;

#ifdef STBWEB
extern void sound_event(sound_event_t event_num);
#else
#define sound_event(e) /*NOT_USED(e)*/
#endif

extern void pointer_set_position(int x, int y);
extern void pointer_limit(int x0, int y0, int x1, int y1);

/*

   The RISC OS tmpnam function has problems executing under the debugger, so we
   have a variant of our own. This doesn't apply under other OSs.
   Either way, rs_tmpnam (ROM SAFE tmpnam) is always a better choice.

*/

#ifdef RISCOS

extern char *rs_tmpnam(char *s);

#else

#define rs_tmpnam tmpnam

#endif

/* Set the window flags for a window WITHOUT deleting and recreating it. Only
 * works on the New Wimp (WindowManager 3.90)
 */
os_error *wimp_set_wind_flags( wimp_w w, wimp_wflags bic, wimp_wflags eor );


extern char *xfgets(FILE *in);
extern void fskipline(FILE *in);

extern int get_free_memory_size(void);

/* stdio wrappers that allocate a buffer using mm_malloc.c */

#ifdef BUILDERS

#define mmfopen(a,b)	fopen(a,b)
#define mmfclose(a)	fclose(a)

#else

extern FILE *mmfopen(const char *file, const char *mode);
extern void mmfclose(FILE *f);

#endif

#ifdef FRESCO
int mkdir( const char *dir, int mode );
#endif

/* eof util.h */
