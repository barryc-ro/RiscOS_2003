/* -*-c-*- */

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "msgs.h"
#include "os.h"
#include "licence.h"
#include "Serial.h"
#include "version.h"
#include "swis.h"
#include "memwatch.h"
#include "config.h"

#ifndef STBWEB_ROM
#define STBWEB_ROM 0
#endif


#ifndef FILEONLY
/*                            0123456789012345678901234567 89012345678901234567 */
#if STBWEB_ROM || (defined(STBWEB) && PRODUCTION)
static char key_string[48] = "Acorn Network Computer ROM\0    YWqg=*g&LNA&Wqcn\0"; /* This is OK */
/* static char key_string[48] = "Acorn Network Computer ROM\0    GgprzA6$XADoyfe7\0"; */ /* This is OK */
#elif DEVELOPMENT
static char key_string[48] = "ANT Ltd. Internal use only.\0   YDh8R$DhHSrPH4SJ\0"; /* This is OK */
/*static char key_string[48] = "ANT Ltd. Internal use only.\0   jcW7Hs7hp#Mbq&pB\0";*/ /* This is OK */
#else /* installable */
/*                            012345678901234567890123456789012345678901234567 */
static char key_string[48] = "MARK twain STNG c   0   4   8   c   0   4   8   ";
#endif
#endif

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char munge;
} timeout_block;

#if TIMEOUT
static timeout_block tob = { (TIMEOUT_DAY ^ TIMEOUT_MUNGE),
			     ((TIMEOUT_MONTH - 1) ^ TIMEOUT_MUNGE),
			     ((TIMEOUT_YEAR - 1900) ^ TIMEOUT_MUNGE),
			     TIMEOUT_MUNGE };
#endif

char *licensee_name;
char *licensee_string;
char *ua_name;

#define BASE_UA_NAME	PROGRAM_TITLE "/" VERSION_NUMBER SPECIAL_SUFFIX

extern os_error *licence_init(void)
{
#ifdef FILEONLY
    licensee_name = "File-only version";
#else
    char buffer[48];

#if TIMEOUT
    time_t tt;
    struct tm *tmp;

    tt = time(NULL);
    tmp = localtime(&tt);

    tob.day ^= tob.munge;
    tob.month ^= tob.munge;
    tob.year ^= tob.munge;

#if 0
    fprintf(stderr, "Time out date is %d-%d-%d, date now is %d-%d-%d\n",
	    tob.day, tob.month, tob.year,
	    tmp->tm_mday, tmp->tm_mon, tmp->tm_year );
#endif

    if ( (tob.year <  tmp->tm_year) ||
	((tob.year == tmp->tm_year) && ( (tob.month <  tmp->tm_mon) ||
					((tob.month == tmp->tm_mon) && (tob.day < tmp->tm_mday)) )) )
    {
	os_error *ep = (os_error *) &key_string;
	strcpy(ep->errmess, "This version has timed out.");
	ep->errnum = 0;
	return ep;
    }
#endif /* TIMEOUT */

/*#ifndef MemCheck_MEMCHECK*/
#if !DEVELOPMENT && !defined(MemCheck_MEMCHECK) && !defined(FILEONLY)
    if (VerifySerial(key_string) == 0)
    {
	os_error *ep = (os_error *) &key_string;
	strcpy(ep->errmess, "Licence error.  Please contact your supplier.");
	ep->errnum = 0;
	return ep;
    }
#endif

    /* Initally we have up to 30 chars, a NULL chars and another NULL.
       We turn this into 16 chars, a colon, up to 30 chars and a NULL. */

    memcpy(buffer, key_string, 48);
    strcpy(key_string, buffer + 31);
    strcat(key_string, ":");
    strcat(key_string, buffer);

    licensee_name = key_string + 17;
    licensee_string = key_string;

    /* Build a user agent name including the OS version */
    {
	os_regset r;
	os_error *ep;
	int len;
	char *s, *fmt, *os_name;

	/* Get the OS string */
	r.r[0] = r.r[1] = 0;
	ep = os_swix(OS_Byte, &r);

	s = strchr(ep->errmess, '(');
	len = s ? s - ep->errmess : strlen(ep->errmess);
	os_name = ep->errmess;
	while (os_name[len-1] == ' ')
	    len--;

	/* decide what format to use */
	/* fmt = msgs_lookup(config_netscape_fake ? "uahdr1" : "uahdr0"); */
	fmt = config_header_useragent[config_netscape_fake ? 1 : 0];

	/* NCFresco has an extra field for the base Fresco version number */
#ifdef STBWEB
	ua_name = mm_calloc(strlen(fmt) + sizeof(BASE_VERSION_NUMBER) + len + sizeof(BASE_UA_NAME), 1);
	sprintf(ua_name, fmt, BASE_UA_NAME, BASE_VERSION_NUMBER, len, os_name);
#else
	ua_name = mm_calloc(strlen(fmt) + len + sizeof(BASE_UA_NAME), 1);
	sprintf(ua_name, fmt, BASE_UA_NAME, len, os_name);
#endif

#if 0
	ua_name = mm_calloc(len + sizeof(BASE_UA_NAME), 1);
	strcpy(ua_name, BASE_UA_NAME);

	s = ua_name + sizeof(BASE_UA_NAME)-1;
	for (i = 0; i < len; i++)
	{
	    int c = ep->errmess[i];
	    if (c != ' ')
		*s++ = c;
	    else if (isdigit(ep->errmess[i+1]))
		*s++ = '/';
	}
	*s = 0;
#endif
    }

#endif /* ndef FILEONLY */
    return NULL;
}

/* eof licence.c */
