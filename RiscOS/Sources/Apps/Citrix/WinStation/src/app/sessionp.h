

/* > sessionp.h

 *

 */


#ifndef __sessionp_h
# define __sessionp_h

struct session_
{
    DESCRIPTION gszServerLabel;
    ENCRYPTIONLEVEL szEncryptionDLL;
    char szClientType[15];

    char *gszICAFile;

    HANDLE hWFE;

    int fSdPoll;
    int fSdLoaded;
    int HaveFocus;

    ObjectId connect_d;		// connection dialogue box
};


extern void connect_open(Session sess);
extern void connect_status(Session sess, int state);
extern void connect_close(Session sess);

#endif

/* eof sessionp.h */
