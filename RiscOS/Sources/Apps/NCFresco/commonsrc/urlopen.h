/* -*-c-*- */

#ifndef __urlopen_h
#define __urlopen_h

#define wimp_MOPENURL	0x4AF80
#define wimp_MURLHOTADD	0x4AF81
#define wimp_MURLHOTNEW	0x4AF82
#define wimp_MURLHOTDEL	0x4AF83
#define wimp_MNCFRESCO	0x4AF84

#if 1

typedef union {
  char *ptr;
  int offset;
} string_value;

typedef union {
    char url[236];
    struct {
        int tag;
        string_value url;
        int flags;
        string_value body_file;
        string_value target;
    } indirect;
} urlopen_data;

#else

typedef union {
    char url[236];
    struct {
	int tag;
	char *url;
    } indirect;
} urlopen_data;

#endif

typedef struct {
    char *url;
    char *title;
    char appname[32];
} hotlist_msg_data;

typedef struct
{
    int reason;
    int flags;			/* all reserved */
} ncfresco_msg_data;


#define ncfresco_reason_LOAD_DATA	0
#define ncfresco_reason_DIE		1
/* 2 is reserved */
#define ncfresco_reason_READ_CONFIG	3

/* LOAD_DATA flags */
#define ncfresco_loaddata_NOT_ALL	0x00000001
#define ncfresco_loaddata_CONFIG	0x00000002 /* all loads are assumed if NOT_ALL is clear */
#define ncfresco_loaddata_COOKIES	0x00000004
#define ncfresco_loaddata_PASSWORDS	0x00000008
#define ncfresco_loaddata_HOTLIST	0x00000010
#define ncfresco_loaddata_PLUGINS	0x00000020
#define ncfresco_loaddata_ALLOW		0x00000040
#define ncfresco_loaddata_FLUSH		0x00000080 /* flush is assumed if NOT_ALL is clear */

#endif /* __urlopen_h */
