/* > icaclient.h

 * External definitions for the WinFrame ICA client.

 */

#ifndef __icaclient_h
# define __icaclient_h

/* --------------------------------------------------------------------------------------------- */

#define filetype_ICA			0xAD0

/* --------------------------------------------------------------------------------------------- */

#define message_ICACLIENT_BASE		0x50F80

#define message_ICACLIENT_STATUS	(message_ICACLIENT_BASE+0)
#define message_ICACLIENT_CONTROL	(message_ICACLIENT_BASE+1)
#define message_ICACLIENT_CONTROL_ACK	(message_ICACLIENT_BASE+2)

/* --------------------------------------------------------------------------------------------- */

#define icaclient_ERROR_CONNECTION_OPEN		0x01 /* A connection is open so can't open new one or quit application */
#define icaclient_ERROR_CONNECTION_FAILED	0x02 /* New connection failed to open */
#define icaclient_ERROR_HANDLE_UNKNOWN		0x03 /* session handle passed in message is unknown */

/* --------------------------------------------------------------------------------------------- */

/* opaque handle representing an open session */
typedef struct icaclient_session_ *icaclient_session;

/* --------------------------------------------------------------------------------------------- */

/* STATUS message, broadcast by !ICAClient */

#define icaclient_STATUS_STARTING	0
#define icaclient_STATUS_QUITTING	1
#define icaclient_STATUS_CONNECTED	2
#define icaclient_STATUS_SUSPENDED	3
#define icaclient_STATUS_DISCONNECTED	4

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* all currently reserved */ 
    icaclient_session session_handle;	/* only used in CONNECTED/SUSPENDED/DISCONNECTED */
    char server_description[40];	/* only used in CONNECTED */
} icaclient_message_status;

/* --------------------------------------------------------------------------------------------- */

/* CONTROL message, sent by a task to !ICAClient */

#define icaclient_CONTROL_CONNECT	0
#define icaclient_CONTROL_RECONNECT	1
#define icaclient_CONTROL_DISCONNECT	2
#define icaclient_CONTROL_QUIT		3

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* all currently reserved */ 
} icaclient_message_control;

#define icaclient_connect_SERVER_DESCRIPTION	0
#define icaclient_connect_SERVER_NAME		1
#define icaclient_connect_ICA_FILE		2
#define icaclient_connect_DATA_MASK		0x00000003
#define icaclient_connect_DELETE_ICA		0x00000004 /* 1 = delete data.ica_file when finished with it */

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* As defined above */ 
    union
    {
	char server_description[40];	/* server description from APPSRV.INI file */
	char server_name[40];		/* server name or IP address */
	char ica_file[228];		/* path name of ica file */
    } data;
} icaclient_message_control_connect;

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* all currently reserved */ 
    icaclient_session session_handle;
} icaclient_message_control_reconnect;

typedef icaclient_message_control_reconnect icaclient_message_control_disconnect;
typedef icaclient_message_control icaclient_message_control_quit;

/* --------------------------------------------------------------------------------------------- */

/* CONTROL ACK message, sent !Icaclient as a reply to a task's CONTROL message */

#define icaclient_control_ack_SUCCESS	0x00000001

typedef struct
{
    int reason;				/* reason code of original control message */
    int flags;				/* see above */ 
    icaclient_session session_handle;	/* for CONNECT, RECONNECT and DISCONNECT */
    int errnum;				/* not a RISC OS error number */
    char errmess[220];			/* textual error message */
} icaclient_message_control_ack;

/* --------------------------------------------------------------------------------------------- */

#endif

/* eof icaclient.h */
