/* > clientname.c
 *
 *
 */

#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../inc/wfengapi.h"
#include "../inc/debug.h"
#include "../inc/clib.h"

#include "swis.h"

#include "version.h"
#include "clientname.h"
#include "utils.h"

/* user settable client name - overrides all */
#define ENV_CLIENTNAME			APP_NAME "$ClientName"

#define NCMA_CMD_HOME			"ncma:userhome"

#define NCMA_TAG_USER_NAME		"LOGIN_ID"
#define NCMA_TAG_HOME			"URL_INIT"

#ifndef NCAccessManager_Enquiry
#define NCAccessManager_Enquiry		0x4F000
#endif

#define INET_HOSTNAME			"Inet$HostName"

#define BROWSER_HOME_URL		"NCFresco$Home"

#define BROWSER_INTERNAL_PREFIX_1	"ncint:"
#define BROWSER_INTERNAL_PREFIX_2	"ncfrescointernal:"
#define BROWSER_INTERNAL_LOADURL	"loadurl?"

void clientname(char *output, int output_len)
{
    char *s;
    char buf[256];
    int out;
    unsigned uid0, uid1;

    // determine the client name from environment variable first
    if ((s = getenv(ENV_CLIENTNAME)) != NULL)
    {
	strncpy(output, s, output_len);

	TRACE((TC_UI, TT_API1, "clientname: " ENV_CLIENTNAME " returned '%s'", s));
	return;
    }

    /* next thing is to try and see if we have managed access and get a user name out of it */
    if (_swix(NCAccessManager_Enquiry, _INR(0,2) | _OUT(0), NCMA_TAG_USER_NAME, buf, sizeof(buf), &out) == NULL)
    {
	if (out > 0)
	{
	    strncpy(output, buf, output_len);

	    TRACE((TC_UI, TT_API1, "clientname: NCMA returned '%s'", buf));
	    return;
	}
    }

    // then try the internet hostname
    if ((s = getenv(INET_HOSTNAME)) != NULL)
    {
	if (strcmpi(s, "ARM_NoName") != 0)
	{
	    strncpy(output, s, output_len);

	    TRACE((TC_UI, TT_API1, "clientname: " INET_HOSTNAME " returned '%s'", s));
	    return;
	}
    }	

    // try unique machine id
    if (_swix(OS_ReadSysInfo, _IN(0) | _OUTR(3,4), 2, &uid0, &uid1) == NULL)
    {
	if (uid0 != 0 || uid1 != 0)
	{
	    sprintf(output, "UID%08x%08x", ~uid0, ~uid1);

	    TRACE((TC_UI, TT_API1, "clientname: using uid returned '%s'", output));
	    return;
	}
    }

    // use time stamp to create an unique client name
    sprintf(output, "Internet%08u", clock() & 0xffffff);
    TRACE((TC_UI, TT_API1, "clientname: using clock returned '%s'", output));
}

/*
 * This function is passed in the URL with which we were launched and
 * returns whether we were actually the main application launched 
 * ie on an NC, whether we were launched from the smartcard.
 */

int compare_with_browser_home(const char *url)
{
    char *home = strdup(getenv(BROWSER_HOME_URL));
    char *home_url = NULL;
    BOOL matches;

    if (home == NULL)
	return FALSE;

    if (strnicmp(home, "-url ", 5) == 0)
	home += 5;

    /* get the real URL that is being used */
    if (strnicmp(home, "ica:", 4) == 0)
	home_url = strdup(home);
    else 
    {
	char *hurl = NULL;
	
	/* if it is 'ncma:userhome' then pull off the real URL from the smartcard */
	if (strnicmp(home, NCMA_CMD_HOME, sizeof(NCMA_TAG_HOME)-1) == 0)
	{
	    char buf[256];
	    int out;
	    if (_swix(NCAccessManager_Enquiry, _INR(0,2) | _OUT(0), NCMA_TAG_HOME, buf, sizeof(buf), &out) == NULL && out > 0)
		home = strdup(buf);
	}

	/* this lot picks out ncint:loadurl? and ncfrescointernal:loadurl? and then parses the args for the URL */
	if (strnicmp(home, BROWSER_INTERNAL_PREFIX_1, sizeof(BROWSER_INTERNAL_PREFIX_1)-1) == 0)
	    hurl = home + sizeof(BROWSER_INTERNAL_PREFIX_1)-1;
	else if (strnicmp(home, BROWSER_INTERNAL_PREFIX_2, sizeof(BROWSER_INTERNAL_PREFIX_2)-1) == 0)
	    hurl = home + sizeof(BROWSER_INTERNAL_PREFIX_2)-1;
	
	if (hurl && strnicmp(hurl, BROWSER_INTERNAL_LOADURL, sizeof(BROWSER_INTERNAL_LOADURL)-1) == 0)
	{
	    arg_element *list = NULL, *a;

	    hurl += sizeof(BROWSER_INTERNAL_LOADURL)-1;

	    parse_args(&list, hurl);

	    for (a = list; a; a = a->next)
		if (stricmp(a->name, "url") == 0)
		{
		    home_url = a->value;
		    a->value = NULL;
		}

	    free_elements(&list);
	}
    }

    TRACE((TC_UI, TT_API1, "compare_with_browser_home: url '%s' home '%s' (%s)", url, home, strsafe(home_url)));
    
    /* return TRUE if we are using an ica: transport - ignore the reset of the URL for now */
    matches = home_url && strnicmp(home_url, "ica:", 4) == 0;

    free(home_url);
    free(home);

    return matches;
}

/* eof clientname.c */
