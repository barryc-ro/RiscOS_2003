/* > winframe.h

 * External definitions for the WinFrame ICA client.

 */

#ifndef __winframe_h
# define __winframe_h

/* --------------------------------------------------------------------------------------------- */

#define filetype_ICA			0xAD0

/* --------------------------------------------------------------------------------------------- */

#define message_WINFRAME_BASE		0x50F80

#define message_WINFRAME_STATUS		(message_WINFRAME_BASE+0)
#define message_WINFRAME_CONTROL	(message_WINFRAME_BASE+1)
#define message_WINFRAME_CONTROL_ACK	(message_WINFRAME_BASE+2)

/* --------------------------------------------------------------------------------------------- */

#define winframe_ERROR_CONNECTION_OPEN		0x01 /* A connection is open so can't open new one or quit application */
#define winframe_ERROR_CONNECTION_FAILED	0x02 /* New connection failed to open */
#define winframe_ERROR_HANDLE_UNKNOWN		0x03 /* session handle passed in message is unknown */

/* --------------------------------------------------------------------------------------------- */

/* opaque handle representing an open session */
typedef struct winframe_session_ *winframe_session;

/* --------------------------------------------------------------------------------------------- */

/* STATUS message, broadcast by !WinFrame */

#define winframe_STATUS_STARTING	0
#define winframe_STATUS_QUITTING	1
#define winframe_STATUS_CONNECTED	2
#define winframe_STATUS_SUSPENDED	3
#define winframe_STATUS_DISCONNECTED	4

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* all currently reserved */ 
    winframe_session session_handle;	/* only used in CONNECTED/SUSPENDED/DISCONNECTED */
    char server_description[40];	/* only used in CONNECTED */
} winframe_message_status;

/* --------------------------------------------------------------------------------------------- */

/* CONTROL message, sent by a task to !WinFrame */

#define winframe_CONTROL_CONNECT	0
#define winframe_CONTROL_RECONNECT	1
#define winframe_CONTROL_DISCONNECT	2
#define winframe_CONTROL_QUIT		3

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* all currently reserved */ 
} winframe_message_control;

#define winframe_connect_ICA_FILE	0x00000001 /* 1 = data.ica_file; 0 = data.server_name */
#define winframe_connect_DELETE_FILE	0x00000002 /* 1 = delete data.ica_file when finished with it */

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* As defined above */ 
    union
    {
	char server_name[40];		/* server name or IP address */
	char ica_file[228];		/* path name of ica file */
    } data;
} winframe_message_control_connect;

typedef struct
{
    int reason;				/* As defined above */
    int flags;				/* all currently reserved */ 
    winframe_session session_handle;
} winframe_message_control_reconnect;

typedef winframe_message_control_reconnect winframe_message_control_disconnect;
typedef winframe_message_control winframe_message_control_quit;

/* --------------------------------------------------------------------------------------------- */

/* CONTROL ACK message, sent !WinFrame as a reply to a task's CONTROL message */

#define winframe_control_ack_SUCCESS	0x00000001

typedef struct
{
    int reason;				/* reason code of original control message */
    int flags;				/* see above */ 
    winframe_session session_handle;	/* for CONNECT, RECONNECT and DISCONNECT */
    int errnum;				/* not a RISC OS error number */
    char errmess[220];			/* textual error message */
} winframe_message_control_ack;

/* --------------------------------------------------------------------------------------------- */

#endif

/* eof winframe.h */
