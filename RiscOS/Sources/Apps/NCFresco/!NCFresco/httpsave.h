/* httpsave.h

 * Mechanismss to allow saving and loading of user configuration files
 * over http.

 * In quiet periods, or periodically, user config files.
 
 */

#ifndef __httpsave_h
# define __httpsave_h

#define choicesfile_COOKIES	0
#define choicesfile_PASSWORDS	1
#define choicesfile_PLUGINS	2
#define choicesfile_HOTLIST	3
#define choicesfile_CONFIG	4

#define choicesfile_N_TAGS	5

#define choicesfile_tag_COOKIES	(1<<choicesfile_COOKIES)
#define choicesfile_tag_PASSWORDS	(1<<choicesfile_PASSWORDS)
#define choicesfile_tag_PLUGINS	(1<<choicesfile_PLUGINS)
#define choicesfile_tag_HOTLIST	(1<<choicesfile_HOTLIST)
#define choicesfile_tag_CONFIG	(1<<choicesfile_CONFIG)

typedef struct httpsave_context *httpsave_t;

/*
 * The httpsave_t handle wil become invalid when this function returns.
 * which is the id from above
 * file_name is the tempoirary file it is stored in.
 * handle is the handle passed in to fetch.
 * return TRUE if we have disposed of the temporary file.
 *        FALSE if this module should remove it.
 */

typedef BOOL (*httpsave_fetched_fn)(httpsave_t h, int which, const char *file_name, void *handle);


/*
 * The httpsave_t handle wil become invalid when these functions return.
 * file_mask is the mask as passed in of the files that were sent.
 * If it is zero then the send failed for some reason.
 * handle is the handle passed in to fetch.
 */


typedef void (*httpsave_sent_fn)(httpsave_t h, int file_mask, void *handle);

/* ----------------------------------------------------------------------------- */

extern void httpsave_set_url(const char *handler_url);
extern void httpsave_abort(httpsave_t h);
extern httpsave_t httpsave_fetch(int file_mask, httpsave_fetched_fn fn, void *handle);
extern os_error *httpsave_close_and_send(httpsave_t h);
extern BOOL httpsave_add_file(httpsave_t h, int tag, const char *file_name);
extern httpsave_t httpsave_open(httpsave_sent_fn fn, void *handle);

/* ----------------------------------------------------------------------------- */

#endif

/* eof httpsave.h */
