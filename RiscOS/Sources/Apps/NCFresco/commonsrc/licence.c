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
#include "util.h"

#ifndef STBWEB_ROM
#define STBWEB_ROM 0
#endif


#ifndef FILEONLY
/*                            0123456789012345678901234567 89012345678901234567 */
#if STBWEB_ROM || (defined(STBWEB) && PRODUCTION)

#ifdef ANT_NCFRESCO
static char key_string[48] = "Beta release 06-Feb-98    \0    qE6mZoszSct#c3o9\0";
#else               /* Acorn build */
static char key_string[48] = "Acorn Network Computer ROM\0    YWqg=*g&LNA&Wqcn\0"; /* This is OK */
#endif

/* static char key_string[48] = "Acorn Network Computer ROM\0    GgprzA6$XADoyfe7\0"; */ /* This is OK */
#elif DEVELOPMENT
static char key_string[48] = "ANT Ltd. Internal use only.\0   YDh8R$DhHSrPH4SJ\0"; /* This is OK */
/*static char key_string[48] = "ANT Ltd. Internal use only.\0   jcW7Hs7hp#Mbq&pB\0";*/ /* This is OK */
#else /* installable */
/*                            012345678901234567890123456789012345678901234567 */
static char key_string[48] = "MARK twain STNG c   0   4   8   c   0   4   8   ";
/* pdh: fixlicence.c does strcmp on it, so make sure the next byte is zero!
 */
#endif
#endif

typedef struct {
    unsigned char day;
    unsigned char month;
    unsigned char year;
    unsigned char munge;
} timeout_block;

char *licensee_name;
char *licensee_string;
char *ua_name;

#if TIMEOUT
static timeout_block tob = { (TIMEOUT_DAY ^ TIMEOUT_MUNGE),
			     ((TIMEOUT_MONTH - 1) ^ TIMEOUT_MUNGE),
			     ((TIMEOUT_YEAR - 1900) ^ TIMEOUT_MUNGE),
			     TIMEOUT_MUNGE };

extern void TimeoutError( void );       /* grody.s */
extern BOOL timed_out;                  /* event2.c */
extern unsigned int timeout_time_t;     /* backend.c */
BOOL timeout_message = TRUE;            /* see statusbar.c */

static const char demohtml[] = {
167, 207, 213, 209, 159, 118, 130, 184, 215, 216, 214, 210, 147, 136, 205, 219,
176, 109, 147, 211, 210, 214, 162, 147, 201, 210, 220, 157, 150, 220, 225, 217
};

#endif

#define BASE_UA_NAME	PROGRAM_TITLE "/" VERSION_NUMBER SPECIAL_SUFFIX

extern char *fresco_date;   /* buildtime.c */

#if TIMEOUT

static int before( int y1, int y2, int m1, int m2, int d1, int d2 )
{
    return ( y1 < y2 ) ||
           ( (y1 == y2) && ( ( m1 < m2 ) ||
                             ( (m1 == m2) && (d1 < d2) ) ) );
}
/*     (tob.year <  tmp->tm_year) || */
/* 	((tob.year == tmp->tm_year) && ( (tob.month <  tmp->tm_mon) || */
/* 					((tob.month == tmp->tm_mon) && (tob.day < tmp->tm_mday)) )) ) */

#endif

char timeoutbuf[40];    /* zero-initialised */

extern os_error *licence_init(void)
{
#ifdef FILEONLY
    licensee_name = "File-only version";
#else
    char buffer[48];
    int realserial = 0;
#if TIMEOUT
    time_t tt;
    struct tm *tmp;

    tt = time(NULL);
    tmp = localtime(&tt);
#endif

/*#ifndef MemCheck_MEMCHECK*/
#if !DEVELOPMENT && !defined(MemCheck_MEMCHECK) && !defined(FILEONLY)
    realserial = VerifySerial( key_string );
    if (realserial == 0)
    {
	os_error *ep = (os_error *) &key_string;
	strcpy(ep->errmess, "Licence error.  Please contact your supplier.");
	ep->errnum = 0;
	return ep;
    }
#endif

#if 0
    fprintf(stderr, "Time out date is %d-%d-%d, date now is %d-%d-%d\n",
	    tob.day, tob.month, tob.year,
	    tmp->tm_mday, tmp->tm_mon, tmp->tm_year );
#endif

#if TIMEOUT
    if ( realserial == TIMEOUT_SERIAL_NUMBER )
    {
        if ( before( tob.year  ^ tob.munge, tmp->tm_year,
                     tob.month ^ tob.munge, tmp->tm_mon,
                     tob.day   ^ tob.munge, tmp->tm_mday ) )
        {
            unsigned char c = 'A';
            int i;

            for ( i=0; i < 32; i++ )
            {
                c = (demohtml[i] - c) & 0xFF;
                timeoutbuf[i] = (char)c;
            }
            timeoutbuf[32] = '\0';
        }

        /* Check clock not earlier than compile date
         * Compile date is "Mmm dd yyyy"
         */
        if ( before( tmp->tm_year, atoi( fresco_date+7 ) - 1900,
                     tmp->tm_mon,  make_month( fresco_date ),
                     tmp->tm_mday, atoi( fresco_date+4 )           ) )
            TimeoutError();
    }
    else
    {
        /* We've been sanitised: don't report errors from last-modified dates
         */
        timeout_time_t = 0xFFFFFFFF;
        timeout_message = FALSE;
    }
#endif /* TIMEOUT */

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
	fmt = msgs_lookup(config_netscape_fake ? "uahdr1" : "uahdr0");

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
