/* -*-c-*- */

/* cookie.h */

extern void cookie_read_file(const char *file_name);
extern void cookie_write_file(const char *file_name);

extern void cookie_received_header(const char *header, const char *url);

extern http_header_item *cookie_add_headers(http_header_item *hlist, const char *url, int secure);

extern void cookie_dispose_all(void);
extern void cookie_free_headers(void);
extern void cookie_optimise(void);

/* eof cookie.h */
