/* -*-c-*- */

/* access.h */

/* Functions for accessing URLs */

#ifndef __access_h
#define __access_h

#ifndef __urlopen_h
#include "urlopen.h"
#endif

typedef int access_complete_flags;
#define access_CACHE	0x01
#define access_KEEP	0x02
#define access_OURS	0x04	/* Take the cache entry out but don't delete the file */
#define access_LOCK	0x08	/* Keep the file locked until we unlock it explicitly */

typedef int access_url_flags;
#define access_NOCACHE		0x01
#define access_FORGROUND	0x02
#define access_CHECK_EXPIRE	0x04
#define access_CHECK_FILE_TYPE	0x08
#define access_PRIORITY         0x10    /* jump the HTTP keep-alive queue? */
#define access_IMAGE            0x20    /* http fetch prefix first? */
#define access_NO_STREAM        0x40    /* don't need data streamed - just file */
#define access_MAX_PRIORITY     0x80    /* load whole image if possible */
#define access_UPLOAD           0x100   /* use PUT (only works for ftp) */
#define access_MUST_BE_FOUND    0x200   /* return error status if we didn't get the file we were expecting (ie rc != 200) */
#define access_INTERNAL_FLAGS	0x0ffff

/* Flags used internally but not used in the initial call */
#define access_SECURE		0x010000	/* The access will be started on a secure socket */
#define access_PROXY		0x020000	/* The access goes via a proxy */
#define access_PENDING_FREE	0x040000	/* It will die when it unthreads */
#define access_IS_DIRECTORY     0x080000	/* FTP fetched a directory */
#define access_FROM_CACHE	0x100000	/* This stream is from the cache */

/* A list of the schemes we support */
extern char *access_schemes[];

typedef void (*access_progress_fn)(void *h, int status, int size, int so_far, int fh, int ftype, char *url);

/* Note that that file name is not 'yours' and will disappear after
 * your function returns.  The function can set the access_CACHE bit
 * in the return value to ask the access system to hold on to the
 * file.  If the access_KEEP bit is set it will keep the file longer.
 * If the access_KEEP flag was set then the owner of the complete
 * function should call the access_unkeep() function at a later time
 * to decrement the lock count.  The fact that the KEEP flag was set
 * does not guarantee the file will be kept, just makes it more
 * likely.  If the file is needed again the access_url() function
 * should be called again, this time with a complete function that
 * returns zero.
 * */

typedef access_complete_flags (*access_complete_fn)(void *h, int status, char *cfile, char *url);

typedef struct _access_item *access_handle;

/*

 * The access_url function is given a url to fetch and endevoures to
 * fetch the item either from cache or from the location given.  If
 * the item is in the cache the complete function is called and the
 * value NULL is returned in the result variable.  If the item needs
 * to be fetched this operation is started and a handle to the
 * transfer is given in the result; while this is taking place the
 * access_progress function will be called periodically until the
 * transfer is complete and then the complete function is called with
 * the name of the cache file the item has been stored in.
 * The ofile value can be given to specify a destination file.
 */

typedef struct
{
    char *body_file;
    char *content_type;
} access_post_info;

#define mimetype_FORM_URLENCODED	"application/x-www-form-urlencoded"
#define mimetype_MULTIPART_FORM		"multipart/form-data"

extern os_error *access_url(char *url, access_url_flags flags, char *ofile, access_post_info *post,
			    char *referer,
			    access_progress_fn progress, access_complete_fn complete,
			    void *h, access_handle *result);

/* Call this to abort an existing connection */
extern os_error *access_abort(access_handle h);

#ifdef BUILDERS
#define access_fromcache(h) TRUE
#else
BOOL access_fromcache( access_handle h );
#endif

#define access_test_NOT_PRESENT	0
#define access_test_PRESENT	1
#define access_test_EXPIRED	2
extern int access_test_cache(char *url);

extern int access_url_type_and_size(char *url, int *ftypep, int *sizep);

/* wrapper cache functions */
typedef char cache_flags;
#define cache_flag_OURS         0x01
#define cache_flag_IGNORE_SIZE  0x02

extern void access_unkeep(char *url);
extern void access_remove(char *url);
extern void access_insert(char *url, char *file, cache_flags flags);
extern char *access_scrapfile(void);
extern void access_set_header_info(char *url, unsigned date, unsigned last_modified, unsigned expires);
extern BOOL access_get_header_info(char *url, unsigned *date, unsigned *last_modified, unsigned *expires);

/* Give the init function the number of files that can be cached */
extern os_error *access_init(int size);
extern int access_safe_to_quit(void);
extern os_error *access_tidyup(void);
extern void access_flush_cache(void);

char *access_cache_lookup( char *url );
void access_cache_lookup_free( char *fname );

/* Try and move the cache data structures down into lower memory so
 * we can shrink the wimpslot
 */
extern void access_optimise_cache( void );

extern void access_abort_all_items(void);
extern BOOL access_is_scheme_supported(const char *scheme);
extern void access_set_streaming(access_handle d, int stream);
extern BOOL access_was_directory( access_handle d );

/*
 * Calls HTTP_Status to read the headers for the current connection
 * Actually returns http_header_item *
 */
extern void *access_get_headers(access_handle d);

extern int access_get_ftype(access_handle d);

#ifdef ACCESS_INTERNAL

typedef struct
{
    void (*insert)(char *url, char *file, cache_flags flags);
    void (*remove)(char *url);

    void (*keep)(char *url);
    void (*unkeep)(char *url);

    void (*not_ours)(char *url);

    char *(*lookup)(char *url, int check_expire);
    void (*lookup_free_name)(char *url);

    os_error *(*init)(int size);
    void (*tidyup)(void);

    char *(*scrapfile_name)(void);

    int (*test)(char *url);

    void (*update_size)(char *url);

    void (*header_info)(char *url, unsigned date, unsigned last_modified, unsigned expires);

    void (*flush)(void);
    void (*optimise)(void);

    BOOL (*get_header_info)(char *url, unsigned *date, unsigned *last_modified, unsigned *expires);
} cache_functions;

extern cache_functions old_cache_functions;
extern cache_functions cachefs_cache_functions;

#endif

#endif /* __access_h */
