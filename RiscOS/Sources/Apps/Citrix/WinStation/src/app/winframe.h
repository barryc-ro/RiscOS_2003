/* > winframe.h
 *
 */

#ifndef __winframe_h
# define __winframe_h

/* --------------------------------------------------------------------------------------------- */

#define filetype_ICA		0xAD0

/* --------------------------------------------------------------------------------------------- */

#define message_WINFRAME_BASE	0x50F80

#define message_WINFRAME_STATUS		(message_WINFRAME_BASE+0)
#define message_WINFRAME_CONTROL	(message_WINFRAME_BASE+1)
#define message_WINFRAME_CONTROL_ACK	(message_WINFRAME_BASE+2)

#define winframe_STATUS_STARTING	0
#define winframe_STATUS_DYING		1
#define winframe_STATUS_CONNECTED	2
#define winframe_STATUS_SUSPENDED	3
#define winframe_STATUS_DISCONNECTED	4

typedef struct
{
    int reason;			/* As defined above */
    int flags;			/* all currently reserved */ 
} winframe_message_status;

#define winframe_CONTROL_CONNECT	0
#define winframe_CONTROL_RECONNECT	1
#define winframe_CONTROL_DISCONNECT	2
#define winframe_CONTROL_QUIT		3

typedef struct
{
    int reason;			/* As defined above */
    int flags;			/* all currently reserved */ 
    char server[40];		/* server name or IP address. Size is hardwired in WinFrame code anyway */
} winframe_message_control;

#define winframe_CONTROL_ACK_SUCCESS	0x00000001

typedef struct
{
    int reason;			/* As defined above */
    int flags;			/* all currently reserved */ 
} winframe_message_control_ack;

/* --------------------------------------------------------------------------------------------- */

#endif

/* eof winframe.h */
