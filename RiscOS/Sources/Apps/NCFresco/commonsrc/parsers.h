/* -*-c-*- */

/* parsers.h */

typedef void *(*pparse_new_fn)(char *url, int ft, int encoding);
typedef int (*pparse_data_fn)(void *h, char *buffer, int len, int more);
typedef rid_header *(*pparse_rh_fn)(void *h);
typedef void (*pparse_rhvoid_fn)(void *h);
typedef rid_header *(*pparse_close_fn)(void *h, char *cfile);
typedef rid_header *(*parse_file_fn)(char *fname, char *url);

typedef struct {
    int ftype;
    parse_file_fn whole;
    pparse_new_fn new;
    pparse_data_fn data;
    pparse_rh_fn rh;
    pparse_close_fn close;
} pparse_details;

#define FILETYPE_ANY_IMAGE	0x13A6E

extern pparse_details file_parsers[];
