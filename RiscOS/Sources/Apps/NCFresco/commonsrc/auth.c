/* -*-c-*- */

/* auth.c */


/* CHANGE LOG
 *
 * 14/3/96: SJM: added auth_supported() call for access_find_realm
 * 12/9/96: SJM: added disposeall routines
 * 25/10/96: SJM: auth_lookup_string() now mallocs a string to return
 */

/* Deal with user authentication.  We will keep this is a separate module
 * so that in the future we can cope with other simple authentication
 * protocols.  Trying to patch in the more sophisticated protocols that use
 * full-scale encryption of the data (not just of the authentication info)
 * will take a bit more work.  Authentication is only used on HTTP.  To
 * match an authentication we need to know the machine, the port and the
 * path.  We use a database that stores a pair of a triple and a pair:
 * ((machine, port, path), (name, passwd)).  We need a function that sets
 * an entry, a function the finds an entry and maybe one that removes it.
 * Setting an entry that already has an different setting overwrites as
 * this can only occur if the use has given an incorrect user,passwd pair.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "memwatch.h"

#include "os.h"
#include "util.h"
#include "url.h"
#include "makeerror.h"
#include "config.h"

#include "auth.h"

#include "debug.h"

#define AUTHDBG(a) DBG(a)
#define AUTHDBGN(a)

/* extern void translate_escaped_text(char *src, char *dest, int len); */

typedef struct auth_realm {
    struct auth_realm *next/* , *prev */;
    char *name;
    auth_type type;
    union {
	struct {
	    char *user;
	    char *passwd;
	} basic;
    } data;
} auth_realm;

typedef struct auth_item {
    struct auth_item *next/* , *prev */;
    char *url;
    auth_realm *realm;
} auth_item;

typedef struct allow_item {
    union {
	unsigned int hash;
	char ch;
    } u;
    struct allow_item *next;
} allow_item;

static auth_realm *realm_first = NULL, *realm_last = NULL;
static auth_item *auth_first = NULL, *auth_last = NULL;

static allow_item *allow_list = NULL, *deny_list = NULL;

/* This needs to line up with the auth_type enum */
static char *auth_type_names[] = {
    "None",
    "Basic",
    "Pubkey",
    0
    };

static int auth_read_allow_file(char *fname, allow_item **head);
static int auth_test_allow(char *site, allow_item *list);

static void auth_realm_dispose(auth_realm *r)
{
    if (r)
    {
	mm_free(r->name);
	switch (r->type)
	{
	case auth_type_BASIC:
	    mm_free(r->data.basic.user);
	    mm_free(r->data.basic.passwd);
	    break;
	}
	mm_free(r);
    }
}

#ifdef STBWEB
static void auth_realms_dispose(void)
{
    auth_realm *r = realm_first;
    while (r)
    {
	auth_realm *next = r->next;
	auth_realm_dispose(r);
	r = next;
    }

    realm_first = realm_last = NULL;
}

static void auth_item_dispose(auth_item *i)
{
    if (i)
    {
	mm_free(i->url);
	mm_free(i);
    }
}

static void auth_items_dispose(void)
{
    auth_item *i = auth_first;
    while (i)
    {
	auth_item *next = i->next;
	auth_item_dispose(i);
	i = next;
    }

    auth_first = auth_last = NULL;
}

static void auth_allow_dispose(allow_item *a)
{
    if (a)
    {
	mm_free(a);
    }
}

static void auth_allows_dispose(allow_item *list)
{
    allow_item *a = list;
    while (a)
    {
	allow_item *next = a->next;
	auth_allow_dispose(a);
	a = next;
    }
}

void auth_dispose(void)
{
    auth_realms_dispose();
    auth_items_dispose();

    auth_allows_dispose(allow_list);
    allow_list = NULL;

    auth_allows_dispose(deny_list);
    deny_list = NULL;
}
#endif

void auth_forget_realm( auth_realm *r )
{
    auth_item **ppItem = &auth_first;
    auth_item *pItem;
    auth_realm **ppRealm = &realm_first;
    auth_realm *pRealm;

    /* Remove any items that refer to this realm */
    while ( ( pItem = *ppItem ) != NULL )
    {
        if ( pItem->realm == r )
        {
            mm_free( pItem->url );
            *ppItem = pItem->next;
            mm_free( pItem );
        }
        else
            ppItem = &(pItem->next);
    }

    /* Remove the realm */
    while ( ( pRealm = *ppRealm ) != NULL )
    {
        if ( pRealm == r )
        {
            *ppRealm = r->next;
            auth_realm_dispose(r);
            break;
        }
        else
            ppRealm = &(pRealm->next);
    }
}

#ifdef STBWEB
static void auth_realms_optimise(void)
{
    auth_realm *r, *last;
    for (last = NULL, r = realm_first; r; last = r, r = r->next)
    {
	auth_realm *was = r;
	if (optimise_block((void **)&r, sizeof(*r)))
	{
	    auth_item *a;

	    if (last)
		last->next = r;
	    else
		realm_first = r;

	    if (realm_last == was)
		realm_last = r;

	    /* see if any items referenced this realm */
	    for (a = auth_first; a; a = a->next)
	    {
		if (a->realm == was)
		    a->realm = r;
	    }
	}

	r->name = optimise_string(r->name);
	switch (r->type)
	{
	case auth_type_BASIC:
	    r->data.basic.user = optimise_string(r->data.basic.user);
	    r->data.basic.passwd = optimise_string(r->data.basic.passwd);
	    break;
	}
    }
}

static void auth_items_optimise(void)
{
    auth_item *item, *last;
    for (last = NULL, item = auth_first; item; last = item, item = item->next)
    {
	auth_item *was = item;
	if (optimise_block((void **)&item, sizeof(*item)))
	{
	    if (last)
		last->next = item;
	    else
		auth_first = item;

	    if (auth_last == was)
		auth_last = item;
	}

	item->url = optimise_string(item->url);
    }
}

void auth_optimise(void)
{
    auth_items_optimise();
    auth_realms_optimise();
}

#endif

/* ------------------------------------------------------------------------- */

/* This function should read in a realm/method/data file */
void auth_init_passwords(void)
{
#ifdef STBWEB
    auth_realms_dispose();
    auth_items_dispose();
#endif
    if (gstrans_not_null(config_auth_file))
	auth_load_file(config_auth_file);
}

void auth_init_allow(void)
{
#ifdef STBWEB
    auth_allows_dispose(allow_list);
    allow_list = NULL;

    auth_allows_dispose(deny_list);
    deny_list = NULL;
#endif
    if (gstrans_not_null(config_allow_file))
	auth_read_allow_file(config_allow_file, &allow_list);

    if (gstrans_not_null(config_deny_file))
	auth_read_allow_file(config_deny_file, &deny_list);
}

void auth_init(void)
{
    auth_init_passwords();
    auth_init_allow();
}

static int auth_get_type(char *type)
{
    int t;

    for (t=0; auth_type_names[t]; t++)
    {
        if (strcasecomp(auth_type_names[t], type) == 0)
            return t;
     }

    return -1;
}

int auth_supported(char *type)
{
    int t = auth_get_type(type);
    return t == auth_type_BASIC;
}

realm auth_add_realm(char *realm, char *type, char *user, char *passwd)
{
    auth_realm *a;
    int t;

    t = auth_get_type(type);

    if (t != auth_type_BASIC)
        return NULL;

    for(a = realm_first; a; a = a->next)
    {
	if (strcmp(realm, a->name) == 0)
	{
	    break;
	}
    }

    if (a == NULL)
    {
	a = (auth_realm*)mm_calloc(1, sizeof(*a));
	if (a == NULL)
	    return NULL;

	a->name = strdup(realm);

	if (realm_first)
	{
/* 	    a->prev = realm_last; */
	    realm_last->next = a;
	    realm_last = a;
	}
	else
	{
	    realm_first = realm_last = a;
	}
    }

    a->type = (auth_type) t;

    if (t == auth_type_BASIC)
    {
	if (a->data.basic.user)
	    mm_free(a->data.basic.user);
	a->data.basic.user = strdup(user);
	if (a->data.basic.passwd)
	    mm_free(a->data.basic.passwd);
	if (passwd)
	    a->data.basic.passwd = strdup(passwd);
	else
	    a->data.basic.passwd = NULL;
    }

    return a;
}

void auth_add(char *url, realm r)
{
    auth_item *a;

    for(a = auth_first; a; a = a->next)
    {
	if (strcmp(url, a->url) == 0)
	{
	    break;
	}
    }

    if (a == NULL)
    {
	a = (auth_item*)mm_calloc(1, sizeof(*a));
	if (a == NULL)
	    return;

	a->url = strdup(url);

	if (auth_first)
	{
/* 	    a->prev = auth_last; */
	    auth_last->next = a;
	    auth_last = a;
	}
	else
	{
	    auth_first = auth_last = a;
	}
    }

    a->realm = r;

    return;
}

realm auth_lookup_realm(char *realm)
{
    auth_realm *a;

    AUTHDBGN(("Trying to look up realm '%s'... ", realm));

    for(a = realm_first; a; a = a->next)
    {
	if (strcmp(realm, a->name) == 0)
	{
	    AUTHDBGN(("Hit!\n"));

	    return a;
	}
    }

    AUTHDBGN(("Miss.\n"));

    return NULL;
}

/* Nasty lookup function just does a linear search.  One day I might do something better. */
auth_lookup_result auth_lookup(char *url, auth_type *type, char **user, char **passwd)
{
    auth_item *a;

    AUTHDBGN(("Trying to authorise URL='%s' ", url));

    for(a = auth_first; a; a = a->next)
    {
	if (strncmp(url, a->url, strlen(a->url)) == 0)
	{
	    if (a->realm)
	    {
		*type = a->realm->type;
		*user = a->realm->data.basic.user;
		*passwd = a->realm->data.basic.passwd;

		AUTHDBGN(("Hit!\n"));

		return auth_lookup_SUCCESS;
	    }
	    else
	    {
		AUTHDBGN(("Miss.\n"));

		return auth_lookup_NEED_DATA;
	    }
	}
    }

    AUTHDBGN(("Miss.\n"));

    return auth_lookup_FAIL;
}

#define UU_BUFFER_SIZE 256

char *auth_lookup_string(char *url)
{
    auth_type type;
    char *user;
    char *passwd;
    char temp[256];
    char *buffer = NULL;

    if (auth_lookup(url, &type, &user, &passwd) == auth_lookup_FAIL)
	return NULL;

    switch (type)
    {
    case auth_type_BASIC:
	buffer = (char*) mm_malloc(UU_BUFFER_SIZE);
	sprintf(temp, "%s:%s", user, passwd);
	strcpy(buffer, "Basic ");
	uuencode(temp, buffer+6, UU_BUFFER_SIZE-6);
	/* pdh: not
	 *	uuencode(temp, buffer+6, sizeof(buffer)-6);
	 * as sizeof(buffer) isn't 256, it's 4...
	 */

	break;
    default:
	usrtrc( "Unknown auth type\n");
	buffer = strdup("UNKNOWN");
	break;
    }

    return buffer;
}

#ifdef STBWEB
int auth_remove(char *url)
{
    return FALSE;
}
#endif

os_error *auth_write_realms(char *fname, auth_passwd_store pws)
{
    FILE *fh;
    auth_realm *rp;

    AUTHDBG(("auth: write '%s' enc %d\n", fname, pws));

    if (!gstrans_not_null(fname))
	return NULL;

    fh = mmfopen(fname, "w");

    if (fh == NULL)
	return makeerror(ERR_CANT_OPEN_AUTH_FILE);

    fprintf(fh, "# Authorisation entries.\n# Realm name\tType\tuser\tpassword\n");
    fprintf(fh, "Format: 1\n");

    for (rp = realm_first; rp; rp = rp->next)
    {
	char *escr, *escu = NULL, *escp = NULL;
	char uubuf[256];
	char *usep = "";
	char *type = NULL;


	escr = url_escape_chars(rp->name, " ");
	switch (rp->type)
	{
	case auth_type_BASIC:
	    type = auth_type_names[rp->type];
	    escu = url_escape_chars(rp->data.basic.user, " ");
	    escp = url_escape_chars(rp->data.basic.passwd, " ()");
	    break;
	}

	switch (pws)
	{
	case auth_passwd_NONE:
	    usep = "";
	    break;
	case auth_passwd_PLAIN:
	    usep = escp;
	    break;
	case auth_passwd_UUCODE:
	    uubuf[0] = '(';
	    uuencode(escp, uubuf+1, sizeof(uubuf) - 2);
	    strcat(uubuf, ")");
	    usep = uubuf;
	    break;
	}

	if (type)
	{
	    fprintf(fh, "%s\t%s\t%s\t%s\n", escr, type, escu, usep);
	}

	mm_free(escr);
	mm_free(escu);
	if (escp)
	    mm_free(escp);
    }

    mmfclose(fh);

    return NULL;
}

os_error *auth_load_file(char *fname)
{
    FILE *fh;
    char buffer[1024];

    AUTHDBG(("auth: read '%s'\n", fname));

    if (file_type(fname) == -1)
	return NULL;

    fh = mmfopen(fname, "r");

    if (fh == NULL)
	return makeerror(ERR_CANT_OPEN_AUTH_FILE);

    while (!feof(fh))
    {
	if (fgets(buffer, sizeof(buffer), fh))
	{
	    char *r, *t, *u, *p;

	    r = strtok(buffer, " \t\n\r");

	    AUTHDBG(("Read realm '%s'\n", r ? r : "<none>"));

	    if (r && r[0] != '#' && strcmp(r, "Format:") != 0)
	    {
		t = strtok(NULL, " \t\n\r");
		u = strtok(NULL, " \t\n\r");
		p = strtok(NULL, " \t\n\r");

		AUTHDBG(("Type='%s', user='%s', passwd='%s'\n", t ? t : "<None>", u ? u : "<None>", p ? p : "<None>"));

		if (t && u)
		{
		    unsigned char uucode[256];
/* 		    char unescaped[256]; */
		    char *unescaped;

		    if (p && p[0] == '(')
		    {
			p[strlen(p)-1] = 0;
			uudecode(p+1, uucode, sizeof(uucode));
			p = (char *) uucode;

			AUTHDBG(("Password uudecoded to '%s'\n", p));
		    }

/* 		    translate_escaped_text(r, unescaped, sizeof(unescaped) ); */
		    unescaped = url_unescape(r, FALSE);

		    auth_add_realm(unescaped, t, u, p);

		    mm_free(unescaped);
		}
	    }
	}
    }

    mmfclose(fh);

    return NULL;
}

static int auth_read_allow_file(char *fname, allow_item **head)
{
    FILE *f;
    char buffer[256];


    AUTHDBG(("Reading allow file '%s'\n", fname));


    if (file_type(fname) == -1)
	return 0;

    f = mmfopen(fname, "r");

    if (f == NULL)
	return 0;

    while (!feof(f))
    {
	if (fgets(buffer, sizeof(buffer), f))
	{
	    char *p, *q;
	    allow_item *new_item;

	    p = strtok(buffer, " \t\n\r");
	    if (p == NULL || *p == '#' || *p == 0)
		continue;

	    for(q=p; *q; q++)
		*q = toupper(*q);

	    AUTHDBG(("Addind list item '%s'\n", p));

	    new_item = (allow_item*)mm_malloc(sizeof(*new_item) + strlen(p)+1);

	    if (new_item)
	    {
		strcpy(&(new_item->u.ch) + 8, p);
		new_item->u.hash = string_hash(p);
		new_item->next = *head;
		*head = new_item;
	    }
	}
    }

    mmfclose(f);

    return 1;
}

static int auth_test_allow(char *site, allow_item *list)
{
    char buffer[256];
    unsigned int hash;
    char *p;

    p = strchr(site, '@');

    strcpy(buffer, p ? p+1 : site);

    p = strchr(buffer, ':');
    if (p)
	*p = 0;

    for(p=buffer; *p; p++)
	*p = toupper(*p);


    AUTHDBG(("Checking site '%s'\n", buffer));


    hash = string_hash(buffer);

    while (list)
    {

	AUTHDBG(("Testing against '%s'\n", &(list->u.ch) + 8));

	if ((list->u.hash == hash) && (strcmp(buffer, &(list->u.ch) + 8) == 0))
	    return 1;

	list = list->next;
    }


    AUTHDBG(("No match\n"));


    return 0;
}

/* return 1 to ban the site, 0 to allow it */

int auth_check_allow_deny(char *site)
{
    /* if we have an allow list return 1 if it is in it */
    if (allow_list)
	return auth_test_allow(site, allow_list);

    /* if we have a deny list return 1 if it is not in it */
    if (deny_list)
	return !auth_test_allow(site, deny_list);

    /* if we don't have either list then allow it */
    return 1;
}


        /*========================*
         *   Image blacklisting   *
         *========================*/


BOOL blacklist_match( const char *url )
{
    char *ptr = config_image_blacklist, *ptr2;
    char buffer[256];

    if ( !ptr || !*ptr )
        return FALSE;

    for (;;)
    {
        ptr2 = strchr( ptr, ',' );
        if ( !ptr2 )
        {
            if ( strstr( url, ptr ) )
            {
                STBDBG(("blacklist: %s matches %s\n", url, ptr ));
                return TRUE;
            }

            STBDBG(("blacklist: %s != %s\n", url, ptr ));

            return FALSE;
        }
        strncpy( buffer, ptr, ptr2-ptr );
        buffer[ptr2-ptr] = '\0';

        if ( strstr( url, buffer ) )
        {
            STBDBG(("blacklist: %s matches %s\n", url, buffer ));
            return TRUE;
        }

        ptr = ptr2 + 1;
    }

    return FALSE;
}

/* eof auth.c */
