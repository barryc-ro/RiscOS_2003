/* -*-c-*- */

#include <stdio.h>

/* url.h */

/* Manipulate URLs */

/* These functions return TRUE is the URLs passed or out are valid */

int url_parse(const char *url, char **scheme, char **netloc, char **path, char **params, char **query, char **fragment);
char *url_unparse(const char *scheme, const char *netloc, const char *path, const char *params, const char *query, const char *fragment);
char *url_join(const char *base, const char *url);
void url_free_parts(char *scheme, char *netloc, char *path, char *params, char *query, char *fragment);

char *url_riscos_to_path(const char *riscos);
char *url_path_to_riscos(const char *path);

char *url_leaf_name(const char *url);

char *url_escape_chars(const char *s, const char *escapes);
void url_escape_cat(char *buffer, const char *in, int len);
void url_escape_to_file(const char *s, FILE *f);
BOOL url_escape_file_to_file(FILE *in, FILE *out);

extern char *url_path_trans(const char *s, int topath);

/* returns 1 if *path_ptr was changed */
int url_canonicalise_path(const char *path, char **path_out_ptr);
void url_canonicalise(char **url);

#define MAX_URL_LEN 4096
#define MAX_FILE_NAME 256
