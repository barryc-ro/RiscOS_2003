/* robits/dumbfs.c - Wrapper layer for dumb filing systems that don't have softlinks. */
/* (C) ANT Limited 1995. All rights reserved. */

/* This library provides a wrapping layer around the use of filenames to   */
/* provide simple support for soft links and long filenames on filing      */
/* systems that do not support such things.  A simple textual database     */
/* file is maintained in each directory to record the necessary            */
/* translation information.  Softlinks are followed to a built in maximum  */
/* depth.                                                                  */
/*                                                                         */
/* The file format is kept as simple as possible.  Names passed are parsed */
/* to determine which directory to examine to find any existing database   */
/* file.  New entries can be created through the appropriate mode strings  */
/* to dumbfs_fopen().  There is no attempt at file locking for the         */
/* database file - the file is cached and the timestamp checked to         */
/* possibly trigger reloading of the file as a robustness feature          */
/*                                                                         */
/* The format of the database file is as follows:                          */
/*                                                                         */
/* Blank lines and lines beginning with # are ignored.  All other lines    */
/* specify a single entry.  Each entry consists of three fields.  The      */
/* first field is a single character specifying what type of entry is      */
/* being dealt with.  There is no seperating character after this field.   */
/* The second field is the name presented by dumbfs to the client.  It is  */
/* seperate from the third field by a single space.  The third field is    */
/* the filename to "use further" - for a file or directory, this is the    */
/* name to use on the local filing system; for a softlink, the last        */
/* element of the fullname is replaced with this string and then another   */
/* lookup performed.  .  These are the type, fullname and local name       */
/* fields.  Defined type values are:                                       */

#define DUMBFS_FILETYPE_CH      'F'    /* A file                           */
#define DUMBFS_DIRTYPE_CH       'D'    /* A directory                      */
#define DUMBFS_LINKTYPE_CH      'L'    /* A softlink type thing            */

#define MAX_FNAME_LEN           10

#define SIZEOF_FILETIME         8

typedef struct
{
        int                     type;
        int                     full_len;
        char                    *full;
        int                     local_len;
        char                    *local;
} dumbfs_item;

typedef struct
{
        int                     name_len;
        char                    load_time[SIZEOF_FILETIME];
        char                    *name;
        int                     entries;
        dumbfs_item             *items;
} dumbfs_db;

typedef struct { int i; char *cp; } ICP;

#define CACHEELTS               10

static dumbfs_db cache[CACHEELTS];


#include <stdio.h>
#include <ctype.h>
#include "kernel.h"
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>

/***************************************************************************/

static char * strjoin(char *a, char *b)
{
        char *c = malloc(strlen(a) + strlen(b) + 1);
        if (c != NULL)
        {
                strcpy(c, a);
                strcat(c, b);
        }
        return c;
}

/***************************************************************************/

static void nullfree(void **vpp)
{
        if (vpp != NULL)
        {
                if (*vpp != NULL)
                {
                        free(*vpp);
                        *vpp = NULL;
                }
        }
}

/***************************************************************************/

/* Parse a RISC OS filename to find the directory and filename components  */
/* if either pointer is NULL, the other will be NULL and a memory error    */
/* prevented operation.  A directory will always be present - even if just */
/* CUR_DIR.  Filename will always point to at least a null character.  The */
/* strings should be freed via free() when finished with.  The original    */
/* string is never altered.                                                */

static void parse_filename(const char *origname, char **dirname, char **filename)
{
        char *cp;
        char *name;
        int last_dir = 0, len1;

        *dirname = NULL;
        *filename = NULL;

        name = malloc(strlen(origname)+1);
        if (name == NULL)
                return;

        strcpy(name, origname);

        cp = strrchr(name, '.');
        if (cp != NULL && cp[1] == 0)
        {
                cp[0] = 0;
                cp = strrchr(name, '.');
        }

        if (cp == NULL)
        {
                *dirname = malloc(strlen(name)+1+1);
                *filename = malloc(1);
                if (*dirname == NULL || *filename == NULL)
                        goto nomem;
                strcpy(*dirname, name);
                strcat(*dirname, ".");
                *filename[0] = 0;
        }
        else
        {
                *filename = malloc(strlen(cp+1)+1);
                if (*filename == NULL)
                        goto nomem;
                strcpy(*filename, cp + 1);
                cp[1] = 0;
                *dirname = malloc(strlen(name));
                if (*dirname == NULL)
                        goto nomem;
                strcpy(*dirname, name);
        }

done:
        free(name);
        return;

nomem:
        nullfree((void**)dirname);
        nullfree((void**)filename);

        goto done;
}

/***************************************************************************/

static void free_cache_elt(dumbfs_db *dp)
{
        int i;

        for (i = 0; i < dp->entries; i++)
        {
                nullfree((void**)&dp->items[i].full);
                nullfree((void**)&dp->items[i].local);
        }

        nullfree((void**)&dp->items);
        nullfree((void**)&dp->name);

        memset(dp, 0, sizeof(*dp));
}

/***************************************************************************/

/* Return one of:  (0, non-existent or error), (S_IFREG, file), (S_IFDIR,  */
/* directory).                                                             */

static int local_stat(char *fname)
{

}



/***************************************************************************/

static int save_db(dumbfs_db *dp)
{
        FILE *fp = fopen(dp->name, "w");
        int i;
        dumbfs_item *item;

        if (fp == NULL)
        	return 0;

        for (item = dp->items, i = 0; i < dp->entries; i++, item++)
        {
                switch (item->type)
                {
                case 0:
                        fprintf(fp, "%s\n", item->full);
                        break;
                case S_IFDIR:
                        fprintf(fp, "D%s %s\n", item->full, item->local);
                        break;
                case S_IFREG:
                        fprintf(fp, "F%s %s\n", item->full, item->local);
                        break;
                case S_IFLNK:
                        fprintf(fp, "L%s %s\n", item->full, item->local);
                        break;
                default:
                        break;
                }
        }

        fclose(fp);

	return 1;
}


/***************************************************************************/

static int load_db(dumbfs_db *dp, char *dbname)
{
        char line[DUMBFS_MAXLINE], c, *cp;
        FILE *fp = fopen(dbname, "r");
        int i;
        dumbfs_item *item;

        assert(dp->name_len == 0);
        assert(dp->name == NULL);
        assert(dp->items == NULL);

        dp->name = strjoin(dbname, "");
        if (dp->name == NULL)
                goto bad;

        dp->name_len = strlen(dp->name);

        if (fp != NULL)
        {
                while ( fgets(line, sizeof(line), fp) != NULL )
                {

                        i = strlen(line);
                        if (i > 0 && line[i-1] == '\n')
                                line[i-1] = 0;

                        if ( realloc(dp->items, sizeof(dumbfs_item) * ++dp->entries) == NULL )
                        {
                                --dp->entries;
                                goto bad;
                        }

                        item = &dp->items[dp->entries-1];
                        memset(item, 0, sizeof(*item));
                        c = line[0];

                        if ( c == 0  || c == '#' )
                        {
                                item->full = strjoin(line, "");
                                item->local = strjoin("", "");
                        }
                        else if ( strchr("FDL", c) == NULL || (cp = strchr(line, ' ')) == NULL )
                        {
                                item->full = strjoin("# ", line);
                                item->local = strjoin("", "");
                        }
                        else
                        {
                                cp[0] = 0;
                                cp++;
                                item->full = strjoin(line+1,"");
                                item->local = strjoin(cp, "");
                                switch (c)
                                {
                                case 'F': item->type = S_IFREG; break;
                                case 'D': item->type = S_IFDIR; break;
                                case 'L': item->type = S_IFLNK; break;
                                }

                        }

                        if (item->full == NULL || item->local == NULL)
                                goto bad;
                        item->full_len = strlen(item->full);
                        item->local_len = strlen(item->local);
                }

                read_load_time(dp);

                fclose(fp);
        }

        return 1;


bad:
        if (fp != NULL)
        {
                fclose(fp);
        }
        free_cache_elt(dp);
        return 0;
}


/***************************************************************************/

static ICP lookup_name(char *dirname, char *filename)
{
        char *dbname = strjoin(dirname, DUMBFS_DBNAME);
        dumbfs_db *dp = cache;
        ICP icp;
        int len, i;
        dumbfs_item *item;

        icp.i = 0;
        icp.cp = NULL;

        if (dbname == NULL)
                return icp;

        len = strlen(dbname);

        for (i = 0, ; i < CACHEELTS; i++, dp++)
        {
                if (dp->name_len != 0 && dp->name_len == len && strcmp(dp->name, dbname) == 0)
                {
                        break;
                }
        }

        if (i == CACHEELTS)
        {
                dp = &cache[ rand() % CACHEELTS ];
                free_cache_elt(dp);
                if (! load_db(dp, dbname) )
                        goto done;
        }
        else
        {
                if (out_of_date(dp))
                {
                        free_cache_elt(dp);
                        if ( !load_db(dp, dbname) )
                                goto done;
                }
        }

        len = strlen(filename);

        for (i = 0, item = dp->items; i < dp->entries; i++, item++)
        {
                if ( items->type != 0 && items->full_len == len && strcmp(item->full, filename) == 0 )
                {
                        break;
                }
        }

        if (i != dp->entries)
        {
                icp.i = item->type;
                icp.cp = strjoin(item->local, "");
        }

done:
        nullfree(&dbname);
        return icp;
}


/***************************************************************************/

/*
Create a new entry for the specified filename in the specified directory,
of the specified type. Choose random real names as necessary. It is
assumed that the entry does not currently exist - will go wrong if it does!
*/

static char * newfile_in_dir(char *dirname, char *filename, int type)
{
        char *dbname = strjoin(dirname, DUMBFS_DBNAME);
        dumbfs_db *dp = cache;
        char *cp = NULL;
        int len;
        dumbfs_item *item;
        char rb[10];

        if (dbname == NULL)
                return NULL;

        for (i = 0, len = strlen(dbname); i < CACHEELTS; i++, dp++)
        {
                if (dp->name_len != 0 && dp->name_len == len && strcmp(dp->name, dbname) == 0)
                        break;
        }

        if (i == CACHEELTS)
        {
                dp = &cache[ rand() % CACHEELTS ];
                free_cache_elt(dp);
                if (! load_db(dp, dbname) )
                        goto done;
        }
        else
        {
                if (out_of_date(dp)
                {
                        free_cache_elt(dp);
                        if ( !load_db(dp, dbname) )
                                goto done;
                }
        }

	do
        {
                sprintf(rb, "%x", rand());
                nullfree((void**)&cp);
      		if ( (cp = strjoin(dirname, rb)) == NULL )
      			goto done;
	} while ( local_stat(cp) != 0 ) ;

	item = realloc(dp->items, sizeof(*item) * ++dp->entries);
	if (item == NULL)
	{
	        --dp->entries;
	        nullfree((void**)&cp);
		goto done;
	}

	dp->items = item;
	item = &item[dp->entries - 1];
	memset(item, 0, sizeof(*item));

	item->full = strjoin(filename, "");
	item->local = strjoin(rb, "");

	if (item->full == NULL || item->local == NULL)
	{
	        nullfree((void**)&item->full);
	        nullfree((void**)&item->local);
	        goto done;
	}

	item->full_len = strlen(item->full);
	item->local_len = strlen(item->local);
	item->type = type;

	if (! save_db(dp))
		nullfree((void**)&cp);

done:
        nullfree((void**)&dbname);
        return cp;

}

/***************************************************************************/

/* Given a full filename, return the corresponding local filename.  The    */
/* enquiry can be performed as "lookup" or a "create" actions, the latter  */
/* creating any new links necessary (eg due to long filenames).  A type is */
/* assigned to the return value.  A recursion limit is checked for.  The   */
/* original caller should never be presented with a S_IFLNK type, although */
/* this occurs during evaluation.  Any intermediate data is freed          */
/* appropriately.                                                          */

static ICP dumbfs_lookup(char *fname, int create, int reclim)
{
        ICP icp, type;
        char *dirname, *filename;
        dumbfs_db *dp;

        icp.i = 0;
        icp.cp = NULL;

        if (--reclim < 0 || fname == NULL)
                return icp;

        parse_filename(fname, &dirname, &filename);

        if (dirname == NULL)
                return icp;

        type = lookup_name(dirname, filename);

        switch (type.i)
        {
        case S_IFDIR:
                icp.cp = strjoin(dirname, type.cp);
                icp.i = S_IFDIR;
                break;
        case S_IFREG:
                icp.cp = strjoin(dirname, type.cp);
                icp.i = S_IFREG;
                break;
        case S_IFLNK:
                {
                        char *tp = strjoin(dirname, type.cp);
                        icp = dumbfs_lookup(tp, create, reclim);
                        nullfree((void**)&tp);
                }
                break;
        case 0:
                if (create && strlen(filename) > MAX_FNAME_LEN )
                {
                        nullfree((void**)&type.cp);
                        type.cp = newfile_in_dir(dirname, filename);
                        if (type.cp == NULL)
                        {
                                icp.cp = NULL;
                                icp.i = 0;
                                break;
                        }
                        icp.cp = strjoin(dirname, tcp.cp);
                        icp.i = S_IFREG;
                }
                else
                {
                        icp.cp = strjoin(dirname, filename);
                        icp.i = S_IFREG;
                }
                break;
        default:
                assert(0==1);
                break;
        }

        nullfree((void**)dirname);
        nullfree((void**)filename);
        nullfree((void**)&type.cp);

        return icp;
}

/***************************************************************************/

extern FILE *dumbfs_fopen(char *fname, char *mode)
{
        typedef struct { char *mode; int create; } pair;
        static pair table[] =
        {
                {    "r" ,     0 },
                {    "w" ,     1 },
                {    "a" ,     1 },
                {    "rb" ,    0 },
                {    "rt" ,    0 },
                {    "wb" ,    1 },
                {    "wt" ,    1 },
                {    "ab" ,    1 },
                {    "at" ,    1 },
                {    "r+" ,    0 },
                {    "w+" ,    1 },
                {    "a+" ,    1 },
                {    "r+b",    0 },
                {    "rb+"     0 },
                {    "r+t",    0 },
                {    "rt+" ,   0 },
                {    "w+b",    1 },
                {    "wb+" ,   1 },
                {    "w+t",    1 },
                {    "wt+" ,   1 },
                {    "a+b",    1 },
                {    "ab+" ,   1 },
                {    "a+t",    1 },
                {    "at+" ,   1 },
                {    NULL,     0 }
        };

        FILE *fp;
        pair *p = table;
        ICP icp;

        while (p->mode != NULL)
        {
                if (strcmp(p->mode, mode) == 0)
                        break;
        }

        icp = dumbfs_lookup(fname, p->mode ? p->create : 0, 10);

        if (icp.i == 0)
        {
                return NULL;
        }

        fp = fopen(icp.cp, mode);

        nullfree((void**)&icp.cp);

        return fp;
}


/***************************************************************************/

/* This contains the OS dependent code */

static int do_real_stat(char *fname, struct stat *st)
{
        _kernel_osfile_block bk;

        if ( _kernel_osfile(17, fname, &bk) < 0 )
        {
                return -1;
        }

        st->st_mode = ;
        st->st_size = ;

        return 0;
}

/***************************************************************************/

/* @@@@ the contents of a struct stat get used as pseudo-random */
/* data for initialising various bits of the crypto stuff. As all */
/* we do is fill in one word, this won't be very secure. */

/* The st_size and st_mode fields are the only fields directly examined. */

extern int dumbfs_stat(char *fname, struct stat *st)
{
        ICP icp;
        int rc;

        icp = dumbfs_lookup(fname);

        st->st_mode = icp.i;

        if (icp.i == 0)
        {
                return -1;
        }

        rc = do_real_stat(icp.cp, st);

        nullfree((void**)&icp.cp);

        return rc;
}

/***************************************************************************/


/* eof */



