

/* > sessionp.h

 *

 */


#ifndef __sessionp_h
# define __sessionp_h

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

    ObjectId connect_d;		// connection dialogue box
};


extern void connect_open(icaclient_session sess);
extern void connect_status(icaclient_session sess, int state);
extern void connect_close(icaclient_session sess);

#endif

/* eof sessionp.h */
