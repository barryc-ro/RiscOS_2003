

/* > sessionp.h

 *

 */


#ifndef __sessionp_h
# define __sessionp_h

typedef struct arg_element arg_element;

struct arg_element
{
    arg_element *next;

    char *name;
    char *value;
};

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

    arg_element *arg_list;
};

#endif

/* eof sessionp.h */
