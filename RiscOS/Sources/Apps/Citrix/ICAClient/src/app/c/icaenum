/* > icaenum.c

 *

 */


#include "windows.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/wfengapi.h"
#include "../inc/neapi.h"

#include "../inc/logapi.h"

extern int DeviceEnumerate( void *pNe, PNEENUMERATE pNeEnum );

BYTE   G_TcpBrowserAddress[ ADDRESS_LENGTH+1 ];
BYTE   G_IpxBrowserAddress[ ADDRESS_LENGTH+1 ];
BYTE   G_NetBiosBrowserAddress[ ADDRESS_LENGTH+1 ];
USHORT G_RequestRetry;
USHORT G_ReadTimeout;
ADDRESS G_TcpBrowserAddrList[MAX_BROWSERADDRESSLIST];
ADDRESS G_IpxBrowserAddrList[MAX_BROWSERADDRESSLIST];
ADDRESS G_NetBiosBrowserAddrList[MAX_BROWSERADDRESSLIST];

#ifdef DEBUG
static int log_init(void)
{
   LOGOPEN EMLogInfo;
   int rc = CLIENT_STATUS_SUCCESS;

   LogClose();
   
   EMLogInfo.LogFlags   = LOG_FILE;
   EMLogInfo.LogClass   = TC_ALL;
   EMLogInfo.LogEnable  = TT_ERROR;
   EMLogInfo.LogTWEnable = TT_ERROR;
   strcpy(EMLogInfo.LogFile, "<Wimp$ScrapDir>.ICAClient.enum");

   rc = LogOpen(&EMLogInfo);

   return(rc);
}

void dump_objects(void)
{
}

void twreportmem(void)
{
}

char *caller(void)
{
    return "";
}
#endif

int main(int argc, char *argv[])
{
    int data_type, enum_flags;

    if (argc > 3)
    {
	fprintf(stderr, "Syntax: icaenum [<data type> [<enum flags>]]\n\r");
	return EXIT_FAILURE;
    }

    data_type = 2;	// return servers
    enum_flags = 4;	// include application names
    if (argc > 1)
    {
	data_type = (int)strtoul(argv[1], NULL, 10);
	if (argc > 2)
	    enum_flags = (int)strtoul(argv[2], NULL, 16);
    }
    
    memset(G_TcpBrowserAddress, 0, sizeof(G_TcpBrowserAddress));
    memset(G_IpxBrowserAddress, 0, sizeof(G_IpxBrowserAddress));
    memset(G_NetBiosBrowserAddress, 0, sizeof(G_NetBiosBrowserAddress));

    memset(G_TcpBrowserAddrList, 0, sizeof(G_TcpBrowserAddrList));
    memset(G_IpxBrowserAddrList, 0, sizeof(G_IpxBrowserAddrList));
    memset(G_NetBiosBrowserAddrList, 0, sizeof(G_NetBiosBrowserAddrList));

    {
	NEENUMERATE e;
	char buffer[10*1024];
	int rc;
	
	memset(&e, 0, sizeof(e));

	// where the data comes out
	e.pName = buffer;
	e.ByteCount = sizeof(buffer);

	// DeviceEnumerate will copy gfrom these pointers into the G_ equivalents
	// ie copying null over itself
	e.pTcpBrowserAddress = G_TcpBrowserAddress;
	e.pIpxBrowserAddress = G_IpxBrowserAddress;
	e.pNetBiosBrowserAddress = G_NetBiosBrowserAddress;

	e.pTcpBrowserAddrList = (LPBYTE)G_TcpBrowserAddrList;
	e.pIpxBrowserAddrList = (LPBYTE)G_IpxBrowserAddrList;
	e.pNetBiosBrowserAddrList = (LPBYTE)G_NetBiosBrowserAddrList;

	// set defaults for these two
	e.BrowserRetry = DEF_BROWSERRETRY;
	e.BrowserTimeout = DEF_BROWSERTIMEOUT;

	// what information is required
	e.DataType = data_type;
	e.EnumReqFlags = enum_flags;

#ifdef DEBUG
	log_init();
#endif

	rc = DeviceEnumerate( NULL, &e );

	if (rc != CLIENT_STATUS_SUCCESS)
	    printf("*** rc=%d\n", rc);
	else
	{
	    char *s = buffer;
	    while (*s)
	    {
		printf("'%s'\n", s);
		s += strlen(s) + 1;
	    }
	}
    }

#ifdef DEBUG
    LogClose();
#endif
    
    return EXIT_SUCCESS;
}


/* eof icaenum.c */
