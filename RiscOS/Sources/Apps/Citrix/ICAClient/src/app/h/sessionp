/* > sessionp.h

 *

 */


#ifndef __sessionp_h
# define __sessionp_h

#ifndef __utils_h
#include "utils.h"
#endif

struct icaclient_session_
{
    DESCRIPTION gszServerLabel;
    ENCRYPTIONLEVEL szEncryptionDLL;
    char szClientType[15];

    char *gszICAFile;
    int tempICAFile;

    HANDLE hWFE;

    int fSdPoll;
    int fSdLoaded;
    int HaveFocus;
    int Connected;
    int SetUp;			// everything is setup and going

    ObjectId connect_d;		// connection dialogue box

    arg_element *arg_list;
};

#endif

/* eof sessionp.h */
