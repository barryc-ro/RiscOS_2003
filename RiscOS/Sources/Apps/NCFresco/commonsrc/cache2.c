/* > cache2.c

 * Old simple caching algorithms

 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "swis.h"

#include "akbd.h"
#include "alarm.h"
#include "os.h"

#include "memwatch.h"
#include "makeerror.h"
#include "util.h"
#include "verstring.h"

#include "config.h"

#define ACCESS_INTERNAL
#include "access.h"

/* ----------------------------------------------------------------------------------------------- */
/* -- scrap functions */

static char scrapname[256];
static char *scrap_leaf_ptr;
static unsigned int scrap_lastcall = (unsigned int)-1;
static int scrap_cycle;

/* ----------------------------------------------------------------------------------------------- */

static char *scrapfile_name(void);
static void cache_insert(char *url, char *file, cache_flags flags);

/* ----------------------------------------------------------------------------------------------- */

static os_error *scrapfile_init(void)
{
    os_regset r;
    os_filestr osf;
    os_error *ep = NULL;
    char buffer[256];

    scrapname[0] = 0;

    os_read_var_val("Wimp$ScrapDir", scrapname, 256);
    if (scrapname[0] == 0)
	ep = makeerror(ERR_NO_SCRAP_DIR);

    if (ep == 0)
    {
	strcpy(buffer, scrapname);
	strcat(scrapname, ".");
	strcat(scrapname, program_name);
	strcat(buffer, ".FrescOld");

	r.r[0] = 27;		/* Wipe objects */
	r.r[1] = (int) (long) buffer;
	r.r[3] = 0;

	/* This is likely to return an error, we should ignore it */
	os_swix(OS_FSControl, &r);

	r.r[0] = 25;		/* Rename objects */
	r.r[1] = (int) (long) scrapname;
	r.r[2] = (int) (long) buffer;

	os_swix(OS_FSControl, &r);

	osf.action = 8;		/* Make new directory */
	osf.name = scrapname;
	osf.start = 0;		/* Default number of directory entries */

	ep = os_file(&osf);
    }

    /* Make sure we are ready for calls to scrapname */
    strcat(scrapname, ".");
    scrap_leaf_ptr = scrapname + strlen(scrapname);

    if (ep == 0 && config_cache_keep)
    {
	char buf2[1040];		/* 1024 for the URL plus 16 for other bits */
	char *p;
	FILE *fh;
	char *bufleaf;

	bufleaf = buffer + strlen(buffer);
	strcpy(bufleaf, "./cachedump");

ACCDBGN(( "Dumped cache file is '%s'\n", buffer));
	fh = fopen(buffer, "r");

	if (fh)
	{
	    while (!feof(fh))
	    {
		if (fgets(buf2, sizeof(buf2), fh))
		{
		    p = buf2 + strlen(buf2);

		    while(isspace(p[-1]))
			p--;

		    *p = 0;

ACCDBGN(( "Line: %s\n", buf2));
		    p = strchr(buf2, '\t');
		    if (p)
		    {
			char *newname;

			*p = 0;
			strcpy(bufleaf+1, buf2);
			newname = scrapfile_name();
ACCDBGN(( "Rename from '%s' to '%s'\n", buffer, newname));
			if (rename(buffer, newname) != -1)
			{
ACCDBGN(( "Cache insert, file '%s' is URL '%s'\n", newname, p+1));
			    cache_insert(p+1, newname, cache_flag_OURS);
			}
		    }
		}
	    }

	    fclose(fh);
	}
	*bufleaf = 0;
    }

    /* Wipe away the old scrap */
    if (ep == 0)
    {
	r.r[0] = 27;		/* Wipe objects */
	r.r[1] = (int) (long) buffer;
	r.r[3] = (1 << 11) | (1 << 1);

	/* This is likely to return an error, we should ignore it */
	ep = os_swix(OS_FSControl, &r);
	if (ep)
	{
ACCDBGN(( "Clearing scrap: %s\n", ep->errmess));
	    ep = 0;
	}
    }

#if 0
    fprintf(stderr, "Name = '%s', length %d, name at 0x%p, end at 0x%p\n",
	    scrapname, strlen(scrapname), scrapname, scrap_leaf_ptr);
#endif
    return ep;
}

static char *scrapfile_name(void)
{
    int now = alarm_timenow();

    if (now == scrap_lastcall)
    {
	scrap_cycle++;
    }
    else
    {
	scrap_lastcall = now;
	scrap_cycle = 0;
    }

    sprintf(scrap_leaf_ptr, "%08x%02x", scrap_lastcall, scrap_cycle);
ACCDBGN(( "Temp file name is '%s'\n", scrapname));
    return scrapname;
}

static char *scrapfile_from_leaf(char *leaf)
{
    strcpy(scrap_leaf_ptr, leaf);

    return scrapname;
}

/* ----------------------------------------------------------------------------------------------- */
/* - cache functions */

typedef struct {
    char *url;
    char *file;
    unsigned int hash;
    cache_flags flags;
    unsigned int last_used;
    int size;			/* Kept in K, rounded UP */
    int keep_count;
} _cache_item;

static _cache_item *cache;
static int cache_size;
static int cache_data_size;	/* In K, rounded up on each file. */

#ifndef MONOTIME
#define MONOTIME		(*(unsigned int *)0x10C) /* RISC OS runery! */
#endif

#define DEFAULT_SIZE	48

/* ----------------------------------------------------------------------------------------------- */

static int cache_index(char *url);
static char *cache_lookup(char *url, int check_expire);
static os_error *cache_remove_file(int i);

/* ----------------------------------------------------------------------------------------------- */

/* internal cache functions */

static double cache_cost(_cache_item *item, unsigned int t)
{
    return ((item->size + 5.0) * ((double)t - item->last_used))/(1.0 + item->keep_count);
}

static os_error *cache_remove_file(int i)
{
    if (cache[i].url == NULL)
	return NULL;
#if 0
    fprintf(stderr, "Asked to remove item %d, url '%s', file '%s', %sours\n",
	    i, cache[i].url, cache[i].file, cache[i].flags & cache_flag_OURS ? "" : "not ");
#endif
    if (cache[i].flags & cache_flag_OURS)
    {
	remove(cache[i].file);
    }

    mm_free(cache[i].url);

    if (cache[i].file)
	mm_free(cache[i].file);

    cache_data_size -= cache[i].size;

    cache[i].url = NULL;
    cache[i].file = NULL;
    cache[i].hash = 0;
    cache[i].keep_count = 0;

    return NULL;
}

static int cache_index(char *url)
{
    int i;
    unsigned int h;

    h = string_hash(url);

    for(i = 0; i < cache_size; i++)
    {
	if (cache[i].hash == h)
	{
	    if (strcmp(url, cache[i].url) == 0)
		return i;
	}
    }

    return -1;
}

/* ----------------------------------------------------------------------------------------------- */

/* cache functions exported (to access.c mainly) */

static void cache_not_ours(char *file)
{
    int i;

    for(i = 0; i < cache_size; i++)
    {
	if (strcmp(file, cache[i].file) == 0)
	    cache[i].flags &= ~cache_flag_OURS;
    }
}

static char *cache_lookup(char *url, int check_expire)
{
    int i;

    i = cache_index(url);

    if (i < 0)
	return NULL;
    else
    {
	cache[i].last_used = MONOTIME;
	return cache[i].file;
    }
}

/* this is a dummy function as other cache schemes need to free the name returned by lookup */

static void cache_lookup_free_name(char *file)
{
    file = file;
}

static void cache_remove(char *url)
{
    int i;

    i = cache_index(url);

    if (i >= 0)
    {
	cache_remove_file(i);
    }
}

static void cache_make_room(int size)
{
    unsigned int t = MONOTIME;

    if (config_cache_size)
    {
	if (size > config_cache_size)
	    size = config_cache_size;

	while (cache_data_size + size > config_cache_size)
	{
	    int i;
	    int worst;
	    double score, s2;

	    worst = -1;
	    score = 0.0;

	    for (i = 0; i <cache_size; i++)
	    {
		if (cache[i].url == NULL)
		{
		    continue;
		}

		if ((s2 = cache_cost(&cache[i], t)) > score)
		{
		    score = s2;
		    worst = i;
		}
	    }

	    if (worst == -1)
		break;

	    cache_remove_file(worst);
	}
    }
}

static void cache_insert(char *url, char *file, cache_flags flags)
{
    os_filestr osf;
    int i;
    int size;

    osf.action = 5;
    osf.name = file;

    if (os_file(&osf) != NULL)
	return;

    size = ((osf.start + 0x3ff) >> 10);

    i = cache_index(url);
ACCDBGN(( "Cache insert index %d, file %s, flags %d\n", i, file, flags));
    if (i < 0)
    {
	int oldest;
	unsigned int oldest_t;
	int keep;

	oldest = 0;		/* In case of failer, ditch a real file rather than cause a memory abort */
	oldest_t = 0xffffffff;
	keep = 0x7ffffff;

	for (i = 0; i <cache_size; i++)
	{
ACCDBGN(( "Cache item %d, url = %p, last used = %x\n", i, cache[i].url, cache[i].last_used));
	    if (cache[i].url == NULL)
	    {
		oldest = i;
		break;
	    }

	    if (cache[i].keep_count < keep || (cache[i].keep_count == keep && cache[i].last_used < oldest_t) )
	    {
		oldest = i;
		oldest_t = cache[i].last_used;
		keep = cache[i].keep_count;
	    }
	}

	i = oldest;
    }

ACCDBGN(( "Item to be inserted into %d\n", i));

    cache_remove_file(i);

    cache_make_room(size);

    cache[i].hash = string_hash(url);
    cache[i].url = strdup(url);
    cache[i].file = strdup(file);
    cache[i].flags = flags;
    cache[i].last_used = MONOTIME;
    cache[i].keep_count = 0;
    cache[i].size = size;

    cache_data_size += cache[i].size;
}

static void cache_optimise( void )
{
    int i;

    if ( cache )
    {
        for ( i=0; i < cache_size; i++ )
        {
            if ( cache[i].url )
                cache[i].url = optimise_string( cache[i].url );
            if ( cache[i].file )
                cache[i].file = optimise_string( cache[i].file );
        }
    }
}

static void cache_keep(char *url)
{
    int i;

    i = cache_index(url);

    if (i != -1)
	cache[i].keep_count++;
}

static void cache_unkeep(char *url)
{
    int i;

    i = cache_index(url);

    if (i != -1)
    {
	if (cache[i].keep_count != 0)
	    cache[i].keep_count--;
    }
}

static void cache_dump(void)
{
    FILE *fh;
    int i;

    fh = fopen(scrapfile_from_leaf("/cachedump"), "w");
    if (fh)
    {
	for (i=0; i < cache_size; i++)
	{
	    if (cache[i].url && (cache[i].flags & cache_flag_OURS) )
	    {
		char *leaf;

		leaf = strrchr(cache[i].file, '.');

		if (leaf)
		    fprintf(fh, "%s\t%s\n", leaf+1, cache[i].url);
	    }
	}

	fclose(fh);
    }
    else
    {
	fprintf(stderr, "Could not save the cache state; dump file would not open.\n");
    }
}

/* ---------------------------------------------------------------------------------------- */

static os_error *cache_init(int size)
{
    os_error *e;

    if (size == 0)
	size = config_cache_items;

    if (size == 0)
	size = DEFAULT_SIZE;

    cache_size = size;
    cache_data_size = 0;

    cache = (_cache_item*)mm_calloc(size, sizeof(_cache_item));

    if (cache == NULL)
        return makeerror(ERR_NO_MEMORY);

    e = scrapfile_init();

    return e;
}

static void cache_tidyup(void)
{
    if (cache)
    {
	int i;

	if (config_cache_keep)
	{
	    cache_dump();
	}
	else
	{
	    if (!(akbd_pollsh() && akbd_pollctl()))
	    {
		for (i=0; i < cache_size; i++)
		{
		    if (cache[i].url)
		    {
			cache_remove_file(i);
		    }
		}
	    }
	}

	mm_free(cache);
	cache = NULL;
    }
}

/* ----------------------------------------------------------------------------------------------- */

static int cache_test(char *url)
{
    char *cfile;
    FILE *f;
ACCDBGN(( "Testing cache for %s\n", url));
    cfile = cache_lookup(url, FALSE);

    if (cfile == NULL)
    {
ACCDBGN(( "Missed\n"));
	return FALSE;
    }

    f = fopen(cfile, "r");

    if (f == NULL)
    {
ACCDBGN(( "Hit but not there\n"));
	return FALSE;
    }

    fclose(f);
ACCDBGN(( "Hit\n"));
    return TRUE;
}

/* ----------------------------------------------------------------------------------------------- */

cache_functions old_cache_functions =
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

    NULL,           /* update size */
    NULL,           /* header info */

    NULL,           /* flush */
    cache_optimise
};

/* ----------------------------------------------------------------------------------------------- */

/* cache2.c */
