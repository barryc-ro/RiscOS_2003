/* -*-c-*- */

/* Resolve a net location into an IP address and a port number */
/* The values are returned in NETWORK byte order */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "os.h"
#include "makeerror.h"
#include "util.h"
#include "memwatch.h"

#include "tcplibs.h"
#include "resolve.h"

#if 0
static unsigned int last_hostaddr;
static char* last_hostname = NULL;
#endif

#define Resolver_GetHost	0x46001

extern os_error *netloc_resolve(char *location, int def_port, int *status, void *addr)
{
    struct sockaddr_in *sa = (struct sockaddr_in *) addr;
    struct hostent *hp;
    struct servent *sp;
    char *p;
    os_regset r;
    os_error *ep;

    sa->sin_family = AF_INET;
    sa->sin_port = htons(def_port);

    if ( (p=strchr(location, ':')) != 0)
    {
	char *portstr;
	int i;

	portstr = p+1;
	
	if ( (i=atoi(portstr)) != 0)
	{
	    sa->sin_port = htons(i);
	}
	else if ( ( sp = getservbyname(portstr, "tcp") ) == NULL)
	{
	    return makeerror(ERR_CANT_FIND_SERVICE);
	}
	else
	{
	    sa->sin_port = sp->s_port;
	}
	*p = 0;
    }

    r.r[0] = (int) (long) location;

    if (isdigit(location[0]))
    {
	sa->sin_addr.s_addr = (unsigned int) inet_addr(location);
	*status = 0;
	ep = NULL;
    }
    else if ((ep = os_swix(Resolver_GetHost, &r)) == NULL)
    {
	*status = r.r[0];
	if (r.r[0] == 0)
	{
	    MemCheck_checking checking = MemCheck_SetChecking(0, 0);

	    hp = (struct hostent *) (long) r.r[1];
	    memcpy(&sa->sin_addr, hp->h_addr, hp->h_length);

	    MemCheck_RestoreChecking(checking);
	}
	else if (r.r[0] < 0)
	{
	    ep = makeerror(ERR_NO_SUCH_HOST);
	}
    }
    else
    {
	if ((hp = gethostbyname(location)) == NULL)
	{
	    ep = makeerror(ERR_NO_SUCH_HOST);
	}
	else
	{
	    memcpy(&sa->sin_addr, hp->h_addr, hp->h_length);
	    ep = NULL;
	}

	*status = 0;
    }	

    if (p)
	*p = ':';

    return ep;
}
