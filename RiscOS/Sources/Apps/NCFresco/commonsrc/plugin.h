/************************************************************************/
/*                  Copyright 1997 Acorn Computers Ltd                  */
/*                                                                      */
/*  This material is the confidential trade secret and proprietary      */
/*  information of Acorn Computers. It may not be reproduced, used      */
/*  sold, or transferred to any third party without the prior written   */
/*  consent of Acorn Computers. All rights reserved.                    */
/*                                                                      */
/************************************************************************/

/* 
 * plugin.h 
 *
 * Version 1.02: 24-Feb-97
 */

#ifndef __plugin_h
# define __plugin_h

#ifndef __time_h
# include <time.h>
#endif

/* ----------------------------------------------------------------------------- */

#define MESSAGE_PLUGIN_OPEN		0x4D540
#define MESSAGE_PLUGIN_OPENING		0x4D541
#define MESSAGE_PLUGIN_CLOSE		0x4D542
#define MESSAGE_PLUGIN_CLOSED		0x4D543
#define MESSAGE_PLUGIN_RESHAPE		0x4D544
#define MESSAGE_PLUGIN_RESHAPE_REQUEST	0x4D545
#define MESSAGE_PLUGIN_FOCUS		0x4D546
#define MESSAGE_PLUGIN_UNLOCK		0x4D547
#define MESSAGE_PLUGIN_STREAM_NEW	0x4D548
#define MESSAGE_PLUGIN_STREAM_DESTROY	0x4D549
#define MESSAGE_PLUGIN_STREAM_WRITE	0x4D54A
#define MESSAGE_PLUGIN_STREAM_WRITTEN	0x4D54B
#define MESSAGE_PLUGIN_STREAM_AS_FILE	0x4D54C
#define MESSAGE_PLUGIN_URL_ACCESS	0x4D54D
#define MESSAGE_PLUGIN_URL_NOTIFY	0x4D54E
#define MESSAGE_PLUGIN_STATUS		0x4D54F
#define MESSAGE_PLUGIN_BUSY		0x4D550
#define MESSAGE_PLUGIN_ACTION		0x4D551
#define MESSAGE_PLUGIN_ABORT		0x4D552

/* ----------------------------------------------------------------------------- */

typedef enum
{
    plugin_reason_DONE = 0,
    plugin_reason_NETWORK_ERROR,
    plugin_reason_USER_BREAK
} plugin_reason;

/* ----------------------------------------------------------------------------- */

typedef struct _plugin_w        *plugin_w;
typedef struct
{
    int				x0, y0, x1, y1;
} plugin_box;

/* ----------------------------------------------------------------------------- */

/* parameter record types in initial parameter file */

typedef enum
{
    plugin_parameter_TERMINATOR		= 0,	/* end of file */
    plugin_parameter_PARAM_DATA		= 1,	/* normal data from a PARAM */
    plugin_parameter_PARAM_URL		= 2,	/* a url of some kind from a PARAM */
    plugin_parameter_PARAM_OBJECT	= 3,	/* a reference to an object from a PARAM */
    plugin_parameter_SPECIAL		= 4,	/* special parent data */
    plugin_parameter_DATA		= 5,	/* normal data attribute from OBJECT/APPLET*/
    plugin_parameter_URL		= 6	/* url attribute from OBJECT/APPLET */
} plugin_parameter_type;

typedef struct
{
    int				size;		/* unpadded size of data[] */
    char			data[1];	/* as many bytes as needed, padded to word boundary */
} plugin_parameter_string;

typedef struct
{
    plugin_parameter_type	type;
    int				size;		/* unpadded size of strings[] */
    plugin_parameter_string	strings[1];	/* as many strings as needed */
} plugin_parameter_record;

typedef struct
{
    plugin_parameter_record	records[1];	/* as many record as needed */
} plugin_parameter_file;

#define plugin_parameter_BASE_HREF	"BASEHREF"
#define plugin_parameter_USER_AGENT	"USERAGENT"
#define plugin_parameter_UA_VERSION	"UAVERSION"
#define plugin_parameter_API_VERSION	"APIVERSION"
#define plugin_parameter_BGCOLOR	"BGCOLOR"

#define plugin_API_VERSION_100	"1.00"
#define plugin_API_VERSION_110	"1.10"
#define plugin_API_VERSION	plugin_API_VERSION_110

/* ============================================================================= */

typedef union 
{
    char			*ptr;			/* ptr must be to shared memory, ie RMA or DA */
    int				offset;			/* offset is from the start of the message data area */
} plugin_string_value;

#define plugin_string_ptr(msg, sv) \
	((sv).offset == 0 ? 0 : \
	 (sv).offset <= 236 ? (char *)(msg) + (sv).offset : (sv).ptr)

/* ----------------------------------------------------------------------------- */

typedef struct
{
    void			*plugin;		/* private to the plugin */
    void			*parent;		/* private to the parent */
} plugin_private_data;

/* ----------------------------------------------------------------------------- */

typedef struct
{
    plugin_private_data		instance;
    plugin_string_value		url;
    int				end;
    time_t			last_modified;
    int				notify_data;		/* as passed in to get/post url */
} plugin_stream;

/* This is the base upon which all other messages are built */

typedef struct
{
    int				flags;
    plugin_private_data		instance;
} message_plugin_base;

/* ============================================================================= */

/* Messages sent by the parent to the plugin */

#define plugin_open_HELPER	0x00000001	/* Open as helper, window_handle and box are invalid */

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_w			window_handle;
    plugin_box			box;		/* in window relative OS coordinates */
    int				file_type;	/* RISC OS file type, mime type is in the parameter file */
    plugin_string_value		file_name;	/* filename of parameter file */
} message_plugin_open;

#define plugin_close_PLEASE_EXIT	0x00000001

typedef message_plugin_base	message_plugin_close;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_w			window_handle;
    plugin_box			box;
} message_plugin_reshape;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_string_value		url;
    plugin_reason		reason;
    int				notify_data;	/* as passed in to get/post url */
} message_plugin_url_notify;

/* ----------------------------------------------------------------------------- */

/* Message sent by the plugin to the parent */

#define plugin_opening_CAN_FOCUS	0x00000001		/* This plugin can accept the input focus */
#define plugin_opening_FETCH_CODE	0x00000002		/* fetch the code resource and send it to plugin */
#define plugin_opening_FETCH_DATA	0x00000004		/* fetch the data resource and send it to plugin */
#define plugin_opening_NEED_FILE	0x00000008		/* plugin will delete the parameter file */
#define plugin_opening_BUSY		0x00000010		/* plugin is still busy, will send BUSY message to clear later */
#define plugin_opening_CAN_ACTION	0x00000020		/* plugin understands ACTION messages */
#define plugin_opening_HELPER		0x00000040		/* plugin instance was actually opened as a helper */

typedef message_plugin_base	message_plugin_opening;

#define plugin_closed_EXITING	0x00000001
#define plugin_closed_NOT_REPLY	0x00000002
#define plugin_closed_ERROR_MSG	0x00000004

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    int				errnum;
    char			errmess[256-20-16]; /* Embedded as plugin may be exiting */
} message_plugin_closed;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    int				width;
    int				height;
} message_plugin_reshape_request;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_string_value		url;
} message_plugin_unlock;

#define plugin_url_access_NOTIFY	0x00000001	/* return notify message on completion */
#define plugin_url_access_POST		0x00000002	/* 1 = POST, 0 = GET */
#define plugin_url_access_POST_FILE	0x00000004	/* if POST, 1 = filename, 0 = buffer */

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_string_value		url;
    plugin_string_value		target;
    int				notify_data;
    int				data_len;
    plugin_string_value		data;		/* data is either a filename or memory pointer */
} message_plugin_url_access;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_string_value		message;
} message_plugin_status;

#define plugin_busy_BUSY	0x00000001	/* Plugin is busy */
#define plugin_busy_STATE_VALID	0x00000002	/* State word is valid*/

#define plugin_state_STOP		0	/* State definitions used in BUSY and ACTION messages */
#define plugin_state_PLAY		1
#define plugin_state_PAUSE		2
#define plugin_state_REWIND		3
#define plugin_state_FAST_FORWARD	4
#define plugin_state_RECORD		5
#define plugin_state_MUTE		6
#define plugin_state_UNMUTE		7

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    int				state;		/* see state definition above */
} message_plugin_busy;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    int				new_state;      /* new state to enter, see definitions above */
} message_plugin_action;

typedef message_plugin_base	message_plugin_abort;

/* ============================================================================= */

/* Message that can be sent in either direction */

typedef message_plugin_base	message_plugin_focus;

/* ============================================================================= */

/* Stream messages 
 * These can be sent in either direction.
 *
 * If the plugin wants to write to the parent then the procedure is
 *  1) plugin fills in mime type and target and calls new()
 *  2) The parent replies with the same message but with the stream fields filled in
 *  3) The plugin calls write() 
 *  4) The parent returns written() to say how much of the data has been handled
 *      repeat 3 and 4 as necessary
 *  5) The plugin calls destroy() with the appropriate reason code
 *
 * If the parent wants to write to the plugin then the procedure is
 *  1) parent fills in flags, mime type, stream and calls new()
 *  2) If the plugin is happy with the stream mode being normal then it need do nothing
 *     otherwise it can reply with the same message but with the type in the flags word updated
 *  3) The parent calls write() 
 *  4) The plugin returns written() to say how much of the data has been handled
 *      repeat 3 and 4 as necessary
 *  5) The parent calls destroy() with the appropriate reason code
 *
 */

#define plugin_stream_TYPE		0x0000000f /* set by parent, updated by plugin */
#define plugin_stream_TYPE_SHIFT	0
#define plugin_stream_TYPE_NORMAL	0
#define plugin_stream_TYPE_SEEK		1
#define plugin_stream_TYPE_ASFILE	2
#define plugin_stream_TYPE_ASFILEONLY	3
#define plugin_stream_SEEKABLE		0x00000010 /* set by parent */

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_stream		stream;		/* only used when coming from parent */
    plugin_string_value		mime_type;
    plugin_string_value		target;		/* only used when plugin originates */
} message_plugin_stream_new;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_stream		stream;
    plugin_reason		reason;
} message_plugin_stream_destroy;

#define plugin_stream_write_DATA	0x0000000f /* how to interpret the data field */
#define plugin_stream_write_DATA_PTR	0x00000000 /* use 'data.ptr' to 'data.ptr + data_len' */
#define plugin_stream_write_DATA_ANCHOR	0x00000001 /* use '*data.ptr + offset' to '*data.ptr + offset + data_len' */
#define plugin_stream_write_DATA_FILE	0x00000002 /* use OS_GBPB, 3, data.offset, your_buf, data_len, offset */

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_stream		stream;
    int				offset;
    int				data_len;
    plugin_string_value		data;
} message_plugin_stream_write;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_stream		stream;
    int				written;	/* n bytes actually written */
} message_plugin_stream_written;

typedef struct
{
    int				flags;
    plugin_private_data		instance;
    plugin_stream		stream;
    plugin_string_value		file_name;
} message_plugin_stream_as_file;

/* ----------------------------------------------------------------------------- */

#endif

/* eof plugin.h */

