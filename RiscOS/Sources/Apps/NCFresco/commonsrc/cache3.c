/* > cache3.c

 * New multi directory cache functions.

 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "swis.h"

#include "akbd.h"
#include "alarm.h"
#include "os.h"

#include "memwatch.h"
#include "makeerror.h"
#include "util.h"
#include "verstring.h"
#if MEMLIB
#include "flexwrap.h"
#endif /* MEMLIB */

#include "config.h"

#define ACCESS_INTERNAL
#include "access.h"

#ifndef LOCK_FILES
#define LOCK_FILES 0
#endif

#ifndef USE_FILEWATCH
#define USE_FILEWATCH 0
#endif

/* ----------------------------------------------------------------------------------------------- */

#if MEMLIB
# define url_ptr(cc)	(urlblock + (cc)->urloffset)
# define url_set(cc)	((cc)->urloffset)
#else /* MEMLIB */
# define url_ptr(cc)	((cc)->url)
# define url_set(cc)	((cc)->url)
#endif /* MEMLIB */

/* ----------------------------------------------------------------------------------------------- */

#define N_FILES_PER_DIR 75

#define FORMAT_STRING	"Format: "

#define CACHE_FORMAT	3

/* ----------------------------------------------------------------------------------------------- */

static void cache_dump_dir_write(int dir);

/* ----------------------------------------------------------------------------------------------- */
/* - cache functions */

typedef struct
{
#if !MEMLIB
    char *url;
#else /* MEMLIB */
    int urloffset;
#endif /* MEMLIB */

    char file_num;
    cache_flags flags;
    unsigned short keep_count;

    unsigned int hash;
    unsigned int last_used;
    int size;			/* Kept in K, rounded UP */
    struct
    {
	unsigned date, last_modified, expires;
	int encoding;
    } header;
} cache_item;

typedef struct
{
    int dir_num;
    int item_count;
    int oldest;

    cache_item *items;
} cache_dir;

static cache_dir *cache;

#if MEMLIB
static char *urlblock = NULL;

#endif /* MEMLIB */
#ifndef MONOTIME
#define MONOTIME		(*(unsigned int *)0x10C) /* RISC OS runery! */
#endif

#define DEFAULT_SIZE	N_FILES_PER_DIR

#define NO_FILE         (255)

/* ----------------------------------------------------------------------------------------------- */
/* -- scrap functions */

static int cache_ndirs;
static int cache_size;
static int cache_data_size = 0;	/* In K, rounded up on each file. */
static int cache_data_count = 0;

static char scrapname[256];
static char *scrap_leaf_ptr;

/* ----------------------------------------------------------------------------------------------- */

/* create a new directory if needed */
static void cache_create_dir(int dir)
{
    cache_dir *dp = &cache[dir];

    if (dp->items == NULL)
    {
        int i;

        ACCDBG(( " create %d", dir));

        dp->items = mm_calloc(sizeof(dp->items[0]), N_FILES_PER_DIR);
        dp->dir_num = dir;
        dp->item_count = 0;
        dp->oldest = 0;

        for (i = 0; i < N_FILES_PER_DIR; i++)
            dp->items[i].file_num = NO_FILE;

        sprintf(scrap_leaf_ptr, "%02d", dir);
        _swix(OS_File, _INR(0,1)|_IN(4), 8, scrapname, 0);
    }
}

/* ----------------------------------------------------------------------------------------------- */

static double cache_cost(cache_item *item, unsigned int t)
{
    return ((item->size + 5.0) * ((double)t - item->last_used))/(1.0 + item->keep_count);
}

/* ----------------------------------------------------------------------------------------------- */

static cache_dir *cache_dir_ptr(int i)
{
    return &cache[i / N_FILES_PER_DIR];
}

static cache_item *cache_item_ptr(int i)
{
    cache_dir *dir = &cache[i / N_FILES_PER_DIR];
    return dir->items ? &dir->items[i % N_FILES_PER_DIR] : NULL;
}

/* ----------------------------------------------------------------------------------------------- */

static char *index_file_name(int dir, int i)
{
    sprintf(scrap_leaf_ptr, "%02d.%02d", dir, i);
    return scrapname;
}

#if 0
static char *index_file_name2(int i)
{
    return index_file_name(i / N_FILES_PER_DIR, i % N_FILES_PER_DIR);
}
#endif

/* ----------------------------------------------------------------------------------------------- */

#if 0
static BOOL cache_scan_for_url(cache_dir *dir, cache_item *item, int n, void *handle)
{
    unsigned int *vars = handle;
    unsigned int h = vars[0];
    char *url = (char *)vars[1];
    return item->hash == h && strcmp(url_ptr(item), url) == 0;
}

static BOOL cache_scan_for_oldest(cache_dir *dir, cache_item *cc, int n, void *handle)
{
    int *vars = handle;

    cache_item *oldest_cc = (cache_item *)vars[0];

    if (cc->file_num == NO_FILE)
    {
        vars[0] = (int)cc;
        vars[1] = n;
        vars[2] = (int)dir;
        return TRUE;
    }

    if (cc->url &&
        ((cc->keep_count < oldest_cc->keep_count) ||
        (cc->keep_count == oldest_cc->keep_count && cc->last_used < oldest_cc->last_used) ) )
    {
        vars[0] = (int)cc;
        vars[1] = n;
        vars[2] = (int)dir;
    }

    return FALSE;
}
#endif

static BOOL is_older(const cache_item *oldest_item, const cache_item *item)
{
    if (url_ptr(oldest_item) == NULL)
        return TRUE;

    if (url_set(item) &&
        ((item->keep_count < oldest_item->keep_count) ||
        (item->keep_count == oldest_item->keep_count && item->last_used < oldest_item->last_used) ) )
        return TRUE;

    return FALSE;
}

static void cache_update_oldest(cache_dir *dp)
{
    int i;
    cache_item *item = dp->items;
    cache_item *item_oldest = dp->items;

    dp->oldest = 0;

    for (i = 0; i < N_FILES_PER_DIR; i++, item++)
    {
        if (item->file_num == NO_FILE)
        {
            dp->oldest = i;
            item_oldest = item;
            return;
        }

        if ((item->flags & cache_flag_OURS) &&
            is_older(item_oldest, item))
        {
            dp->oldest = i;
            item_oldest = item;
        }
    }
}

/* ----------------------------------------------------------------------------------------------- */

#if 0
typedef BOOL (*cache_scan_fn)(cache_dir *dir, cache_item *item, int n, void *handle);

static int cache_scan(cache_scan_fn fn, void *handle, cache_item **item_out, cache_dir **dirp_out)
{
    cache_dir *dirp = cache;
    int dir, i, n;

    n = 0;
    for (dir = 0; dir < cache_ndirs; dir++, dirp++)
    {
        cache_item *item = dirp->items;
        if (item) for (i = 0; i < N_FILES_PER_DIR; i++, item++, n++)
        {
            if (fn(dirp, item, n, handle))
            {
                if (item_out)
                    *item_out = item;
                if (dirp_out)
                    *dirp_out = dirp;

                return dir*N_FILES_PER_DIR + i;
            }
        }
    }

    if (item_out)
        *item_out = NULL;
    if (dirp_out)
        *dirp_out = NULL;

    return -1;
}
#endif
/* ----------------------------------------------------------------------------------------------- */

static cache_item *cache_ptr_from_url(char *url, cache_dir **dir_out)
{
    cache_dir *dirp = cache;
    int dir, i, n;
    int h = string_hash(url);

    n = 0;
    for (dir = 0; dir < cache_ndirs; dir++, dirp++)
    {
        cache_item *item = dirp->items;
        if (item) for (i = 0; i < N_FILES_PER_DIR; i++, item++, n++)
        {
            if (item->hash == h && strcmp(url_ptr(item), url) == 0)
            {
                if (dir_out)
                    *dir_out = dirp;

                return item;
            }
        }
    }

    if (dir_out)
        *dir_out = NULL;

    return NULL;
}

static cache_item *cache_ptr_from_file(char *file, cache_dir **dir_out)
{
    cache_item *item;
    int dir, i;

    scrap_leaf_ptr[0] = 0;
    if (strncasecomp(file, scrapname, scrap_leaf_ptr - scrapname) != 0)
    {
        usrtrc( "++++ Oops tried to lookup unknown scrap file '%s'\n", file);
        return NULL;
    }

    dir = i = -1;
    sscanf(file + (scrap_leaf_ptr - scrapname), "%02d.%02d", &dir, &i);

    item = NULL;
    if (dir != -1 && i != -1)
    {
        cache_dir *dp = &cache[dir];

        if (dir_out)
            *dir_out = dp;

        if (dp->items)
            item = &dp->items[i];
    }
    return item;
}

/* ----------------------------------------------------------------------------------------------- */

static BOOL cache_remove_file(cache_item *cc, cache_dir *dir)
{
    if (cc == NULL || !url_set(cc))
	return TRUE;

    if (cc->flags & cache_flag_OURS)
    {
        if (cc->file_num != NO_FILE)
	{
	    char *cfile = index_file_name(dir->dir_num, cc->file_num);

	    ACCDBG(("cache_remove_file: '%s' '%s'\n", urlblock + cc->urloffset, cfile));

#if LOCK_FILES
	    file_lock(cfile, FALSE);
#endif
	    if (remove(cfile))
	    {
		int type;
		/* if file is open then this counts as a fail and it can't be removed */
		/* if it just wasn't there then it is a success */
		if (_swix(OS_File, _INR(0,1) | _OUT(0), 0x11, cfile, &type) != NULL ||
		    type != 0)
		{
		    cc->flags &= ~cache_flag_OURS;
		    return FALSE;
		}
	    }
	}
    }

    cache_data_size -= cc->size;

#if !MEMLIB
    mm_free(cc->url);
    cc->url = NULL;
#else /* MEMLIB */
    SubFlex_Free( &cc->urloffset, &urlblock );
#endif /* MEMLIB */

    cc->file_num = NO_FILE;
    cc->hash = 0;
    cc->keep_count = 0;
    cc->size = 0;

    dir->item_count--;
    cache_data_count--;

    cache_update_oldest(dir);

    return TRUE;
}

/* ----------------------------------------------------------------------------------------------- */

/* cache functions exported (to access.c mainly) */

static char *cache_lookup(char *url, int check_expire)
{
    cache_dir *dir;
    cache_item *cc = cache_ptr_from_url(url, &dir);

    ACCDBG(("Cache check for %s: %s\n", url, cc ? "hit" : "MISS" ));

    if (cc)
    {
	if (check_expire)
	{
	    if ((cc->header.expires <= cc->header.date) || (cc->header.expires <= time(NULL)))
		return NULL;
	}

	cc->last_used = MONOTIME;

        cache_update_oldest(dir);

        return strdup(index_file_name(dir->dir_num, cc->file_num));
    }
    return NULL;
}

static void cache_lookup_free_name(char *file)
{
    mm_free(file);
}

static void cache_not_ours(char *file)
{
    cache_item *cc = cache_ptr_from_file(file, NULL);
    if (cc)
        cc->flags &= ~cache_flag_OURS;
}

static void cache_remove(char *url)
{
    cache_dir *dir;
    cache_item *cc = cache_ptr_from_url(url, &dir);
    if (cc)
        cache_remove_file(cc, dir);
}

static void cache_make_room(int size)
{
    unsigned int t = MONOTIME;
    int limit = config_cache_size;

    if (config_cache_size)
    {
	if (size > limit)
	    size = limit;

	ACCDBG(("cache_make_room: limit %d size %d\n", limit, size));

	while (cache_data_size + size > limit)
	{
	    int i;
	    int worst;
	    double score, s2;

	    worst = -1;
	    score = 0.0;

	    for (i = 0; i < cache_size; i++)
	    {
	        cache_item *cc = cache_item_ptr(i);
		if (cc == NULL || !url_set(cc))
		{
		    continue;
		}

		if ((s2 = cache_cost(cc, t)) > score)
		{
		    score = s2;
		    worst = i;
		}
	    }

            /* can't delete anything - no point in trying any further */
	    if (worst == -1)
		break;

	    cache_remove_file(cache_item_ptr(worst), cache_dir_ptr(worst));
	}
    }
}

static BOOL cache_insert_data(cache_dir *dir, cache_item *cc, char *url, int file_num, cache_flags flags, int size)
{
#if MEMLIB
    int len = strlen(url)+1;

#endif /* MEMLIB */
    cc->hash = string_hash(url);
#if !MEMLIB
    cc->url = strdup(url);
#else /* MEMLIB */

    ACCDBGN(("cache3: inserting %s\n", url));

    if ( SubFlex_Alloc( &cc->urloffset, len, &urlblock ) != NULL )
    {
        cc->urloffset = 0;
        return FALSE;
    }

    memcpy( urlblock + cc->urloffset, url, len );
#endif /* MEMLIB */

    if (cc->file_num != file_num)
    {
        cc->file_num = file_num;
        dir->item_count++;
        cache_data_count++;
    }

    cc->flags = flags;
    cc->last_used = MONOTIME;
    cc->keep_count = 0;

    /* convert to Kbytes rounded up */
    size = (size + 1023)/1024;

    cc->size = size;
    cache_data_size += size;
    return TRUE;
}

static void cache_optimise( void )
{
    int i;

    if ( cache )
    {
#if MEMLIB
	int n = 0;
        for (i = 0; i < cache_ndirs; i++)
	{
	    cache_dir *dp = &cache[i];
	    if (dp->items)
	    {
		if (optimise_block((void **)&dp->items, sizeof(dp->items[0])*N_FILES_PER_DIR))
		{
		    cache_item *cc;
		    int file;

#if DEBUG
		    ACCDBGN(("Before optimisation: block %p\n", urlblock));
		    for (file = 0, cc = dp->items; file < N_FILES_PER_DIR; file++, cc++)
			if (cc->urloffset)
			    ACCDBGN(("%2d: off %5d '%s'\n", file, cc->urloffset, urlblock + cc->urloffset));
#endif
		    
		    for (file = 0, cc = dp->items; file < N_FILES_PER_DIR; file++, cc++)
			SubFlex_Reanchor(&cc->urloffset, &urlblock);

#if DEBUG
		    ACCDBGN(("After optimisation: block %p\n", urlblock));
		    for (file = 0, cc = dp->items; file < N_FILES_PER_DIR; file++, cc++)
			if (cc->urloffset)
			    ACCDBGN(("%2d: off %5d '%s'\n", file, cc->urloffset, urlblock + cc->urloffset));
#endif
		    n++;
		}
		else
		    /* since all the cache_directories are the same
                       size if we can't move one then give up on them
                       all */
		    break;
	    }
	}

#if DEBUG
	ACCDBG(("cache3: optimised %d cache structures\n", n));
#endif

#else
        for (i = 0; i < cache_size; i++)
        {
	    cache_item *cc = cache_item_ptr(i);
	    if ( cc && cc->url )
		cc->url = optimise_string( cc->url );
        }
#endif /* !MEMLIB */
    }
}

static void cache_insert(char *url, char *file, cache_flags flags)
{
    os_filestr osf;
    cache_item *cc;
    cache_dir *dir;
    int first_dir;

    if (file == NULL)
	return;

    osf.action = 5;
    osf.name = file;
    if (os_file(&osf) != NULL)
	return;

    /* first remove anything also known by this URL */
    cc = cache_ptr_from_url(url, &dir);
    if (cc)
    {
        cache_remove_file(cc, dir);
        first_dir = dir->dir_num;
    }
    else
        first_dir = -1;

    /* then check and see if the entry describing this file is still there */
    cc = cache_ptr_from_file(file, &dir);
    if (cc)
    {
        /* if we don't know the size as it comes in then set it to 1K */
        if ( !cache_insert_data( dir, cc, url, cc->file_num, flags,
                                flags & cache_flag_IGNORE_SIZE ? 1
                                                               : osf.start) )
        {                                                                                   /* out of memory, errr, just return I suppose */
            return;
        }

	/* if it seems to be a script then set it to expired else never expire */
	if (strchr(url, '?') != NULL ||
	    strstr(url, "cgi-bin") != NULL ||
	    strstr(url, ".cgi") != NULL)
	{
	    cc->header.expires = 0;
	}
	else
	    cc->header.expires = UINT_MAX;
    }
    else
    {
        ACCDBG(( "cache: can't find entry for '%s'\n", file));
    }

    /* check for any necessary trimming */
    cache_make_room(0);

    if (dir)
        cache_update_oldest(dir);

    /* dump out the cache details */
    if (config_cache_keep_uptodate)
    {
#if !MEMLIB
        cache_dump_dir_write(dir->dir_num);
        if (first_dir != -1 && first_dir != dir->dir_num)
#else /* MEMLIB */
        if ( dir )
            cache_dump_dir_write(dir->dir_num);

        if ( first_dir != -1 && (!dir || first_dir != dir->dir_num) )
#endif /* MEMLIB */
            cache_dump_dir_write(first_dir);			/* this used to be dir->dir_num */
    }
}

static void cache_keep(char *url)
{
    cache_dir *dir;
    cache_item *cc = cache_ptr_from_url(url, &dir);
    if (cc)
    {
	cc->keep_count++;
        cache_update_oldest(dir);
    }
}

static void cache_unkeep(char *url)
{
    cache_dir *dir;
    cache_item *cc = cache_ptr_from_url(url, &dir);
    if (cc && cc->keep_count != 0)
    {
	cc->keep_count--;
        cache_update_oldest(dir);
    }
}

/* ---------------------------------------------------------------------------------------- */

static void cache_update_size(char *cfile)
{
    cache_item *cc = cache_ptr_from_file(cfile, NULL);
    if (cc)
    {
        int size;
	if (_swix(OS_File, _INR(0,1) | _OUT(4), 0x11, cfile, &size) == NULL)
	{
	    size = (size + 1023) / 1024;    /* to Kbytes */
	    cache_data_size += size - cc->size;
	    cc->size = size;
	}
    }
}

static void cache_header_info(char *url, unsigned date, unsigned last_modified, unsigned expires, int encoding)
{
    cache_item *cc = cache_ptr_from_url(url, NULL);
    if (cc)
    {
	cc->header.date = date;
	cc->header.last_modified = last_modified;

	/* Only write expiry if we have one as otherwise it will overwrite the default
	 * we set based on whether it's a script or not
	 */
	if (expires != UINT_MAX)
	    cc->header.expires = expires;

	cc->header.encoding = encoding;
    }
}

static BOOL cache_get_header_info(char *url, unsigned *date, unsigned *last_modified, unsigned *expires, int *encoding)
{
    cache_item *cc = cache_ptr_from_url(url, NULL);
    if (cc)
    {
	if (date)
	    *date = cc->header.date;
	if (last_modified)
	     *last_modified = cc->header.last_modified;
	if (expires)
	    *expires = cc->header.expires;
	if (encoding)
	    *encoding = cc->header.encoding;

	return TRUE;
    }
    return FALSE;
}

/* ---------------------------------------------------------------------------------------- */

#define SEPS	" \n\r\t"

static void cache_dump_dir_read(int dir)
{
    FILE *fh;
    time_t now = time(NULL);

    sprintf(scrap_leaf_ptr, "%02d./cachedump", dir);
    fh = mmfopen(scrapname, "r");

    ACCDBG(("Reading cache dump %s\n", scrapname ));

    if (fh)
    {
        int sizes[75];
        char buffer[512];
        int nread, offset = 0;
	int format;

	cache_create_dir(dir);

        /* scan directory for files present */
        sprintf(scrap_leaf_ptr, "%02d", dir);

        /* set all sizes to -1 */
        memset(sizes, 0xFF, sizeof(sizes));

        do
        {
            int i;
            char *s;

            if ((os_error *)_swix(OS_GBPB, _INR(0,6) | _OUTR(3,4),
                12, scrapname, buffer, 75, offset, sizeof(buffer), NULL,
                &nread, &offset) != NULL)
                break;

            for (i = 0, s = buffer; i < nread; i++, s += ((24 + strlen(s+24)+1 + 3) &~ 3))
            {
                int size = ((int *)s)[2];
                int index = atoi(s + 24);

                if (index >= 0 && index < 75)
                    sizes[index] = size;

                /* fprintf(stderr, "cache: i %d found '%s' index %d\n", i, s+24, index); */
            }
        }
        while (offset != -1);

        /* scan cache dump file */
	format = 1;
        while (!feof(fh))
        {
	    char buf2[1240];		/* 1024 for the URL plus 200 for other bits */

	    if (fgets(buf2, sizeof(buf2), fh))
	    {
	        char *p;
	        int index;
	        char *url = NULL;
		cache_item *cc;

		if (strncmp(buf2, FORMAT_STRING, sizeof(FORMAT_STRING)-1) == 0)
		{
		    format = atoi(buf2 + sizeof(FORMAT_STRING)-1);

		    /* If this was written by a later version of Fresco then ignore it */
		    if (format > CACHE_FORMAT)
			break;

		    continue;
		}

		/* ACCDBG(( "cache: read '%s'\n", buf2)); */

		/* decode line */
                p = strtok(buf2, SEPS);
                index = atoi(p);

		if (sizes[index] == -1)
		    continue;

		cc = &cache[dir].items[index];

		switch (format)
		{
		case 1:
		{
		    int i;
		    for (i = 0; i < 4; i++)
			p = strtok(NULL, SEPS);    /* size, last used, keep, flags */
		    cc->header.expires = UINT_MAX;
		    break;
		}

		case 2:
		case 3:
                    p = strtok(NULL, SEPS);
		    if (p) cc->header.date = (unsigned)strtoul(p, NULL, 16);
                    p = strtok(NULL, SEPS);
		    if (p) cc->header.last_modified = (unsigned)strtoul(p, NULL, 16);
                    p = strtok(NULL, SEPS);
		    if (p) cc->header.expires = (unsigned)strtoul(p, NULL, 16);

		    if (format >= 3)
		    {
			p = strtok(NULL, SEPS);
			if (p) cc->header.encoding = (int)strtoul(p, NULL, 16);
		    }
		    break;
		}

		url = strtok(NULL, SEPS);

                /* insert into cache structure - if not expired */
                if (/* cc->header.expires > now && */
		    url && strncasecomp(url, "file:", sizeof("file:")-1) != 0
#ifdef STBWEB
                    && strncasecomp(url, "ncint:", sizeof("ncint:")-1) != 0
                    && strncasecomp(url, "ncfrescointernal:", sizeof("ncfrescointernal:")-1) != 0
#endif
		    && strchr(url, ':') != NULL			/* check that this is a valid URL */
                    )
                {
                    /* Ignore errors */
		    cache_insert_data(&cache[dir], cc, url, index, cache_flag_OURS, sizes[index]);
                }
            }
        }

	mmfclose(fh);
    }
}

/*
 * Dump format:
 *  <leaf name><tab><URL>
 */

static void cache_dump_dir_write(int dir)
{
    FILE *fh;
    cache_dir *dp = &cache[dir];

    if (dp->items == NULL)
        return;

    sprintf(scrap_leaf_ptr, "%02d./cachedump", dir);
    fh = mmfopen(scrapname, "w");
    if (fh)
    {
        int i;
        cache_item *cc = dp->items;

	fprintf(fh, FORMAT_STRING "%d\n", CACHE_FORMAT);

	for (i = 0; i < N_FILES_PER_DIR; i++, cc++)
	{
	    if (url_set(cc) && cc->file_num != NO_FILE && (cc->flags & cache_flag_OURS) )
	    {
#if CACHE_FORMAT == 1
		fprintf(fh, "%02d" "\t%d" "\t%x" "\t%d" "\t%d" "\t%s" "\n",
			cc->file_num, cc->size, cc->last_used, cc->keep_count, cc->flags & cache_flag_OURS, url_ptr(cc)
			);
#elif CACHE_FORMAT == 2
		fprintf(fh, "%02d" "\t%x" "\t%x" "\t%x" "\t%s" "\n",
			cc->file_num,
			cc->header.date, cc->header.last_modified, cc->header.expires,
			url_ptr(cc)
			);
#elif CACHE_FORMAT == 3
		fprintf(fh, "%02d" "\t%x" "\t%x" "\t%x" "\t%d" "\t%s" "\n",
			cc->file_num,
			cc->header.date, cc->header.last_modified, cc->header.expires, cc->header.encoding,
			url_ptr(cc)
			);

#else
# error "unknown cache format"
#endif
	    }
	}

	mmfclose(fh);
    }
    else
    {
	ACCDBG(( "Could not save the cache state; dump file would not open.\n"));
    }
}

static void cache_dump_dir_wipe(int dir)
{
    os_regset r;
    cache_dir *dp;
    cache_item *cc;
    int i;

    sprintf(scrap_leaf_ptr, "%02d.*", dir);

    r.r[0] = 27;		/* Wipe objects */
    r.r[1] = (int) scrapname;
    r.r[3] = 0;
    os_swix(OS_FSControl, &r);

#ifndef STBWEB
    dp = &cache[dir];

    ACCDBG(("dump_dir_wipe: dp items %p\n", dp->items));

    cc = dp->items;
    if (cc)
    {
	for (i = 0; i < N_FILES_PER_DIR; i++, cc++)
	{
#if !MEMLIB
	    ACCDBG(("dump_dir_wipe: cc->url %p\n", cc->url));
	    mm_free(cc->url);
#else /* MEMLIB */
	    ACCDBG(("dump_dir_wipe: cc->url %p\n", urlblock + cc->urloffset));
	    SubFlex_Free( &cc->urloffset, &urlblock );
#endif /* MEMLIB */
	}

	ACCDBG(("dump_dir_wipe: dp items %p\n", dp->items));
	mm_free(dp->items);
    }
    memset(dp, 0, sizeof(*dp));
#endif
}

#if DEBUG
#define TIME_FORMAT	"%a, %d %b %Y %H:%M:%S GMT"
void cache_debug(void)
{
    int dir, file;
    for (dir = 0; dir < cache_ndirs; dir++)
    {
        cache_item *cc = cache[dir].items;

	if (cc) for (file = 0; file < N_FILES_PER_DIR; file++, cc++)
	{
	    if (cc->urloffset && cc->file_num != NO_FILE)
	    {
		char rbuf[32];
		strftime(rbuf, sizeof(rbuf), TIME_FORMAT, gmtime((const time_t *)&cc->header.expires));
		DBG(("cache: %02d.%02d %s '%s' 0x%x\n", dir, file, rbuf, urlblock + cc->urloffset, cc->flags));
	    }
	}
    }
}
#endif

/* ---------------------------------------------------------------------------------------- */

static os_error *scrapfile_init(void)
{
    os_regset r;
    os_error *ep = NULL;
    char buffer[256];

    /* init the scrap name field */
    scrapname[0] = 0;
    os_read_var_val("Wimp$ScrapDir", scrapname, 256);

    strcpy(buffer, scrapname);

    strcat(scrapname, ".");
    strcat(scrapname, program_name);

    /* if not keeping old stuff then rename it out of the way */
    if (!config_cache_keep)
    {
	strcat(buffer, ".FrescOld");

	r.r[0] = 27;		/* Wipe objects */
	r.r[1] = (int) buffer;
	r.r[3] = 0;
	os_swix(OS_FSControl, &r);

	r.r[0] = 25;		/* Rename objects */
	r.r[1] = (int) scrapname;
	r.r[2] = (int) buffer;
	os_swix(OS_FSControl, &r);
    }

    /* make root scrap dir */
    ep = (os_error *) _swix(OS_File, _INR(0,1)|_IN(4), 8, scrapname, 0);
    if (ep)
        return ep;

    /* set up the scrap ptrs */
    strcat(scrapname, ".");
    scrap_leaf_ptr = scrapname + strlen(scrapname);

    /* if we are keeping old stuff then check what's there */
    if (config_cache_keep)
    {
        int i;
        for (i = 0; i < cache_ndirs; i++)
            cache_dump_dir_read(i);
    }

    return ep;
}

static char *scrapfile_name(void)
{
    int dir;
    int oldest_file, oldest_dir;
    cache_item *oldest_item;
    cache_dir *oldest_dp;

    do
    {
	oldest_file = oldest_dir = 0;
	oldest_item = cache_item_ptr(0);
	oldest_dp = cache_dir_ptr(0);

	for (dir = 0; dir < cache_ndirs; dir++)
	{
	    cache_dir *dp = &cache[dir];

	    if (dp->items == NULL)
		cache_create_dir(dir);

	    if (dp->item_count != N_FILES_PER_DIR ||
		is_older(oldest_item, &dp->items[dp->oldest]))
	    {
		oldest_file = dp->oldest;
		oldest_dir = dir;

		oldest_dp   = dp;
		oldest_item = &dp->items[dp->oldest];

		if (dp->item_count != N_FILES_PER_DIR)
		    break;
	    }
	}

	/* if remove fails (eg file is open) then mark the file as not ours, redo oldest, and start again */
	/* This should only happen after a crash (ie infrequently) */
	if (!cache_remove_file(oldest_item, oldest_dp))
	{
	    ACCDBG(( "scrapfile: cant remove %02d.%02d '%s'\n", oldest_dir, oldest_file, scrapname));

	    oldest_item->flags &= ~cache_flag_OURS;

	    cache_update_oldest(oldest_dp);

	    oldest_item = NULL;
	}
    }
    while (oldest_item == NULL);

    /* mark allocated */
    oldest_item->file_num = oldest_file;
    oldest_item->last_used = MONOTIME;

    /* increment file counts */
    oldest_dp->item_count++;
    cache_data_count++;

    cache_update_oldest(oldest_dp);

    index_file_name(oldest_dir, oldest_file);
    ACCDBG(( "scrapfile: empty   %02d.%02d '%s'\n", oldest_dir, oldest_file, scrapname));

    return scrapname;
}

/* ---------------------------------------------------------------------------------------- */

#if USE_FILEWATCH

#define FileWatch_RegisterInterest              0x4D240
#define FileWatch_DeRegisterInterest            0x4D241
#define FileWatch_Poll                          0x4D242

static int filewatcher_handle = 0;

static void filewatcher_poll(int called_at, void *handle)
{
    char buffer[256];
    int file_count;
    os_error *e;

    do
    {
        char *buf_end;

	ACCDBGN(("filewatcher_poll: calling handle %d\n", filewatcher_handle));

        e = (os_error *) _swix(FileWatch_Poll, _INR(0,3) | _OUT(2)|_OUT(4),
            0, filewatcher_handle, buffer, sizeof(buffer),
            &buf_end, &file_count);

	ACCDBGN(("filewatcher_poll: returned e %p buf_end %p file_count %d\n", e, buf_end, file_count));

        if (!e)
        {
            char *s = buffer;
            while (s < buf_end)
            {
                cache_dir *dir;
                cache_item *item = cache_ptr_from_file(s, &dir);

		ACCDBG(("filewatcher_poll: remove '%s' item %p dir %p '%s'\n", s, item, dir, item->urloffset ? urlblock + item->urloffset : "<none>"));

                if (item)
                    cache_remove_file(item, dir);

                s += strlen(s) + 1;
            }
        }
	else
	{
	    ACCDBG(("filewatcher_poll: error %x '%s'\n", e->errnum, e->errmess));
	}

    }
    while (!e && file_count);

    alarm_set(alarm_timenow() + 100, filewatcher_poll, handle);
}

static void filewatcher_init(void)
{
    ACCDBG(("filewatcher_init:"));

    if (strncasecomp(scrapname, "cache:", 6) == 0)
    {
        int reasons[2];

        reasons[0] = 6;      /* delete */
        reasons[1] = -1;     /* terminator */

        *scrap_leaf_ptr = 0;

	ACCDBG((" register interest in '%s'", scrapname));

        if (_swix(FileWatch_RegisterInterest, _INR(0,2)|_OUT(0),
            0, reasons, scrapname, &filewatcher_handle) == NULL)
        {
	    ACCDBG((" handle %d", filewatcher_handle));
            filewatcher_poll(0, (void *)filewatcher_handle);
        }
    }
    ACCDBG(("\n"));
}

static void filewatcher_final(void)
{
    if (filewatcher_handle)
    {
	alarm_removeall((void *)filewatcher_handle);

        _swix(FileWatch_DeRegisterInterest, _INR(0,1), 0, filewatcher_handle);
        filewatcher_handle = 0;
    }
}

#endif

/* ---------------------------------------------------------------------------------------- */

static int cache_test(char *url)
{
    cache_dir *dir;
    cache_item *item = cache_ptr_from_url(url, &dir);
    int type;

    if (item == NULL)
        return 0;

#if USE_FILEWATCH
    if (filewatcher_handle)
    {
	type = url_set(item) ? 1 : 0;
    }
    else
#endif
    {
	type = 0;
	_swix(OS_File, _INR(0,1)|_OUT(0), 0x11, index_file_name(dir->dir_num, item->file_num), &type);
    }

    if ((type & 1) == 0)
	return access_test_NOT_PRESENT;

    if ((item->header.expires < item->header.date) || (item->header.expires <= time(NULL)))
	return access_test_EXPIRED;

    return access_test_PRESENT;
}

/* ---------------------------------------------------------------------------------------- */

static void cache_free_mem(void)
{
#if !MEMLIB
    int dir, file;
#else /* MEMLIB */
    int dir;
#endif /* MEMLIB */
    for (dir = 0; dir < cache_ndirs; dir++)
    {
	cache_dir *dp = &cache[dir];
	if (dp->items)
	{
#if !MEMLIB
	    cache_item *cc = dp->items;
	    for (file = 0; file < N_FILES_PER_DIR; file++, cc++)
	    {
		mm_free(cc->url);
	    }

#endif /* not MEMLIB */
	    mm_free(dp->items);
	}
    }

    mm_free(cache);
    cache = NULL;
#if MEMLIB
    MemFlex_Free( &urlblock );
#endif /* MEMLIB */
}

static os_error *cache_init(int size)
{
    os_error *e;

    ACCDBG(("cache3 initialising\n"));

    MemCheck_RegisterMiscBlock((void *)0x10c, 4);

    if (size == 0)
	size = config_cache_items;

    if (size == 0)
	size = DEFAULT_SIZE;

    cache_data_size = 0;

    cache_ndirs = (size-1)/N_FILES_PER_DIR + 1;
    cache_size = cache_ndirs*N_FILES_PER_DIR;

    cache = (cache_dir *)mm_calloc(cache_ndirs, sizeof(cache_dir));

#if !MEMLIB
    e = scrapfile_init();
#else /* MEMLIB */
    e = SubFlex_Initialise( &urlblock );

    if (!e)
        e = scrapfile_init();
#endif /* MEMLIB */
#if USE_FILEWATCH
    if (!e) filewatcher_init();
#endif
    return e;
}


static void cache_tidyup(void)
{
    ACCDBG(("cache: tidyup\n"));

#if USE_FILEWATCH
    filewatcher_final();
#endif
    
    if (cache)
    {
	if (config_cache_keep)
	{
	    /* I was only writing out if not kepot up to date - but it's worth doing always for total consistency */
	    int i;
	    for (i = 0; i < cache_ndirs; i++)
		cache_dump_dir_write(i);
	}
	else
	{
	    if (!(akbd_pollsh() && akbd_pollctl()))
	    {
	        int i;
		for (i = 0; i < cache_size; i++)
		{
		    cache_item *cc = cache_item_ptr(i);
		    if (cc && url_set(cc))
			cache_remove_file(cc, cache_dir_ptr(i));
		}
	    }
	}

	/* free all memory used */
	cache_free_mem();
    }
}

static void cache_flush(void)
{
    int dir;
    for (dir = 0; dir < cache_ndirs; dir++)
	cache_dump_dir_wipe(dir);
}

/* ----------------------------------------------------------------------------------------------- */

cache_functions cachefs_cache_functions =
{
    cache_insert,
    cache_remove,

    cache_keep,
    cache_unkeep,

    cache_not_ours,

    cache_lookup,
    cache_lookup_free_name,

    cache_init,
    cache_tidyup,

    scrapfile_name,

    cache_test,

    cache_update_size,
    cache_header_info,

    cache_flush,
    cache_optimise,

    cache_get_header_info
};

/* ----------------------------------------------------------------------------------------------- */

/* cache3.c */
