/* > main.c

 * Main file for WSClient

 */

#include "windows.h"
#include "fileio.h"

#include <ctype.h>
#include <locale.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "swis.h"

#include "tboxlibs/toolbox.h"
#include "tboxlibs/quit.h"
#include "tboxlibs/event.h"
#include "tboxlibs/wimplib.h"
#include "tboxlibs/proginfo.h"
#include "tboxlibs/gadgets.h"
#include "tboxlibs/menu.h"

#include "tbres.h"
#include "utils.h"
#include "version.h"
#include "session.h"
#include "main.h"
#include "server.h"

#include "icaclient.h"

#include "../inc/clib.h"
#include "../inc/client.h"
#include "../inc/debug.h"
#include "../dll/vd/vdtw31/twh2cinc.h"

#include "vdu.h"

/* --------------------------------------------------------------------------------------------- */

//typedef _kernel_oserror os_error;
//#include "MemLib/memheap.h"
#include "MemLib/memflex.h"

/* --------------------------------------------------------------------------------------------- */

#define POLL_TIME	            100	/* 1 second */
#define MIN_ESC_TIME	            50	/* cs */

#ifdef DEBUG
#define SPLASH_TIME		    100
#else
#define SPLASH_TIME		    200
#endif

#ifdef DEBUG
#define VERSION		Module_MajorVersion " [" __TIME__ ":" __DATE__ "]" Module_MinorVersion
#else
#define VERSION		Module_MajorVersion " (" Module_Date ") " Module_MinorVersion
#endif

/* --------------------------------------------------------------------------------------------- */

/* ANT URL Open protocol */

#define wimp_MOPENURL	0x4AF80

typedef union {
  char *ptr;
  int offset;
} string_value;

typedef union {
    char url[236];
    struct {
        int tag;
        string_value url;
        int flags;
        string_value body_file;
        string_value target;
        string_value content_type;
    } indirect;
} urlopen_data;

/* --------------------------------------------------------------------------------------------- */

/* Acorn URI protocol */

#define MESSAGE_URI_MPROCESS		0x4E383
#define MESSAGE_URI_MPROCESSACK		0x4E384

#define URI_RequestURI			0x4E382

/* --------------------------------------------------------------------------------------------- */

/* Printer protocol */

#define	message_PSPrinterQuery	0x8014C
#define message_PSPrinterAck	0x8014D
#define message_PSPrinterNotPS	0x80151

typedef struct
{
    char *buffer_address;
    int buffer_size;
} printer_message_ps_query;

/* --------------------------------------------------------------------------------------------- */

/* redialling stuff */

#define TaskModule_RegisterService      0x4D302
#define TaskModule_DeRegisterService    0x4D303
#define wimp_MSERVICE                   0x4D300

#define Service_DiallerStatus           0xB4

#define dialler_DISCONNECTED            0
#define dialler_CONNECTED_OUTGOING      (1<<2)

#define NCDialUI_Start			0x4E880

/* --------------------------------------------------------------------------------------------- */

static MessagesFD message_block;    /* declare a message block for use with toolbox initialise */
static IdBlock event_id_block;      /* declare an event block for use with toolbox initialise  */

static int ToolBox_EventList[] =
{
    tbres_event_SERVER_CONNECT,
    tbres_event_MENU_CONNECT,
    tbres_event_CONNECT,
    tbres_event_DISCONNECT,
    tbres_event_QUIT,
    tbres_event_SHOWING_ICON_MENU,
    tbres_event_CONFIRM,
    tbres_event_CANCEL,
    ProgInfo_AboutToBeShown,
    0
};

static int Wimp_MessageList[] =
{
    wimp_MSERVICE,
    message_PSPrinterQuery,
    message_PSPrinterNotPS,
    message_PSPrinterAck,
    message_ICACLIENT_CONTROL,
    wimp_MOPENURL,
    MESSAGE_URI_MPROCESS,
    MESSAGE_URI_MPROCESSACK,
    Wimp_MSaveDesktop,
    Wimp_MDataLoad,
    Wimp_MDataSave,
    Wimp_MDataOpen,
    Wimp_MPreQuit,
    Wimp_MQuit
};

static int event_mask =
    Wimp_Poll_PointerLeavingWindowMask
    | Wimp_Poll_PointerEnteringWindowMask
    | Wimp_Poll_PollWordNonZeroMask;


static icaclient_session current_session = NULL;

static int scrap_ref = -1;

static int task_handle = 0;
static BOOL running = TRUE;

#define quitting_NO		0
#define quitting_SELF		1
#define quitting_DESKTOP	2

static int quitting = quitting_NO;

static ObjectId disconnect_id = NULL_ObjectId;
static ObjectId connect_menu_id = NULL_ObjectId;

/* --------------------------------------------------------------------------------------------- */

static void *splash_ptr = NULL;
static void *splash_coltrans = NULL;
static ObjectId splash_id = NULL_ObjectId;
static int splash_timer = 0;

static int splash_state = initop_UNSTARTED;
static int printinfo_state = initop_UNSTARTED;
static int dial_state = initop_UNSTARTED;
static int connectopen_state = initop_UNSTARTED;

/* --------------------------------------------------------------------------------------------- */

/* Exported to vd/vdspl/spool/rospool.c */
char *printer_name = NULL;
char *printer_type = NULL;

/* --------------------------------------------------------------------------------------------- */

/* cli set values */

static char *cli_filename = NULL;
static char *cli_postfile = NULL;
static char *cli_logfile = NULL;
static int cli_iconbar = FALSE;
static int cli_dopostmortem = FALSE;
static int cli_remote_debug = FALSE;
static int cli_file_debug = FALSE;
       int cli_suspendable = FALSE; /* this is exported to wengine.c */
static int cli_file_is_url = FALSE;
static int cli_loop = FALSE;
static int cli_multitask = FALSE;
static int cli_modem = FALSE;

/* --------------------------------------------------------------------------------------------- */

/* Forward references */

static void splash_close(void);
static void splash_force_top(void);
static BOOL splash_check_close(void);

static void printinfo_deregister(void);
static void printinfo_request(void);

static void dial_start(void);

/* --------------------------------------------------------------------------------------------- */

/* External references */

extern void KbdClose(void);
extern void reset_char_defs(void);

/* --------------------------------------------------------------------------------------------- */

/*
 * Signal handling.
 * Output postmortem on error if debugging
 */

#define FATAL_ERROR utils_msgs_lookup("Efatal:" APP_NAME " has suffered a fatal internal error (type=%i) and must exit immediately")

static _kernel_oserror	sig_errmsg;
static char		*sig_numspot;

static void escape_handler(int sig)
{
    (void) signal(SIGINT, &escape_handler);
    NOT_USED(sig);
}

static void handler(int signal)
{
    *sig_numspot = signal + '0';

    wimp_report_error(&sig_errmsg, 0, utils_msgs_lookup("_TaskName"));

    if (cli_dopostmortem) switch (signal)
    {
        case SIGFPE:
        case SIGILL:
        case SIGSEGV:
            return;
    }

    exit(EXIT_FAILURE);
}

static void signal_setup(void)
{
    sig_errmsg.errnum = 0;
    sprintf(sig_errmsg.errmess, FATAL_ERROR, utils_msgs_lookup("_TaskName"), 9);
    sig_numspot = strchr(sig_errmsg.errmess, '9');

    signal(SIGABRT, &handler);
    signal(SIGFPE, &handler);
    signal(SIGILL, &handler);
    signal(SIGINT, &escape_handler);
    signal(SIGSEGV, &handler);
    signal(SIGTERM, &handler);
}

/* --------------------------------------------------------------------------------------------- */

void main_close_session(icaclient_session sess)
{
    TRACE((TC_UI, TT_API1, "main_close_session: %p suspenable %d", sess, cli_suspendable));
    if (sess)
    {
	if (current_session == sess)
	{
	    current_session = NULL;

	    if (!cli_suspendable)
		running = FALSE;
	}
	
	session_close(sess);

	/* reset the state markers */
	dial_state = initop_UNSTARTED;
	connectopen_state = initop_UNSTARTED;
	printinfo_state = initop_UNSTARTED;
    }
}

static void kill_current_session(void)
{
    main_close_session(current_session);
}

static void connection_state(int event)
{
    TRACE((TC_UI, TT_API1, "connection_state: event %d current_session %p printinfo_state %d splash_state %d dial_state %d connectopen_state %d",
	   event, current_session, printinfo_state, splash_state, dial_state, connectopen_state));

    /* the new plan is that the connection process is driven from the
     * null handler, so notifications of other changes are ignored.
     * Keep the code in, in case it's useful later */
    if (event != connection_NULL)
    {
	TRACE((TC_UI, TT_API1, "connection_state: event %d current_session %p printinfo_state %d splash_state %d dial_state %d connectopen_state %d",
	   event, current_session, printinfo_state, splash_state, dial_state, connectopen_state));

	return;
    }

    if (current_session)
    {
	if (printinfo_state == initop_UNSTARTED)
	    printinfo_request();

	if (splash_state == initop_COMPLETED)
	{
	    if (dial_state == initop_UNSTARTED)
		dial_start();

	    if (dial_state == initop_COMPLETED)
	    {
		if (connectopen_state == initop_UNSTARTED)
		{
		    connect_open(current_session);
		    connectopen_state = initop_COMPLETED;
		}

		if (printinfo_state == initop_COMPLETED && connectopen_state != initop_UNSTARTED)
		{
		    if (!session_connect(current_session))
		    {
			current_session = NULL;
			
			if (!cli_suspendable)
			    running = FALSE;
			
			dial_state = initop_UNSTARTED;
			connectopen_state = initop_UNSTARTED;
			printinfo_state = initop_UNSTARTED;
		    }
		}
	    }
	}
    }
}

/* --------------------------------------------------------------------------------------------- */

static void control_ack(WimpMessage *message, icaclient_session session_handle, int error)
{
    WimpMessage reply;
    icaclient_message_control_ack *ack = (icaclient_message_control_ack *) &reply.data;

    reply.hdr.size = sizeof(reply.hdr) + sizeof(icaclient_message_control_ack);
    reply.hdr.your_ref = message->hdr.my_ref;
    reply.hdr.action_code = message_ICACLIENT_CONTROL_ACK;
    ack->reason = message->data.words[0];
    ack->flags = error != 0 ? 1 : 0;
    ack->session_handle = session_handle;
    ack->errnum = error;
    ack->errmess[0] = 0;

    LOGERR(wimp_send_message(Wimp_EUserMessage, &reply, message->hdr.sender, 0, NULL));
}

static void status_message(int status, icaclient_session session_handle)
{
    WimpMessage message;
    icaclient_message_status *s = (icaclient_message_status *) &s;

    message.hdr.size = sizeof(message.hdr) + sizeof(icaclient_message_status);
    message.hdr.your_ref = 0;
    message.hdr.action_code = message_ICACLIENT_STATUS;
    s->reason = status;
    s->flags = 0;
    s->session_handle = session_handle;

    LOGERR(wimp_send_message(Wimp_EUserMessage, &message, 0, 0, NULL));
}

/* --------------------------------------------------------------------------------------------- */

static int connect_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    TRACE((TC_UI, TT_API1, "connect_handler:\n"));

    if (session_connected(current_session))
	session_resume(current_session);
    else
	msg_report("Run an ICA file to make a connection");
	    
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

static int proginfo_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    LOGERR(displayfield_set_value(0, id_block->self_id, 0x82b404, VERSION));
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(handle);
}

static int icon_menu_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    BOOL connected = session_connected(current_session);

    TRACE((TC_UI, TT_API1, "icon_menu_handler: id_block %p\n", id_block));
    TRACE((TC_UI, TT_API1, "                   self_id %p\n", id_block->self_id));

    LOGERR(menu_set_fade(0, id_block->self_id, tbres_c_CONNECT, !connected));
    LOGERR(menu_set_fade(0, id_block->self_id, tbres_c_DISCONNECT, !connected));

    if (connect_menu_id == NULL_ObjectId)
	connect_menu_id = serverlist_create_menu( APPSRV_FILE );

    if (connect_menu_id != NULL_ObjectId)
	LOGERR(menu_set_sub_menu_show(0, id_block->self_id, tbres_c_CONNECT_SUBMENU, connect_menu_id));
    
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(handle);
}

static int server_connect_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
				  void *handle)
{
    const char *name = serverlist_get_name(id_block->self_component);

    TRACE((TC_UI, TT_API1, "server_connect_handler: component %d name '%s'\n", id_block->self_component, strsafe(name)));

    if (name)
    {
	current_session = session_open_appsrv(name);
	
	if (current_session)
	    connection_state(connection_OPENED);
    }
    
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(handle);
}

/* --------------------------------------------------------------------------------------------- */

static void cleanup_task(void)
{
}

static void cleanup(void)
{
    TRACE((TC_UI, TT_API1, "cleanup:\n"));

    kill_current_session();
    
    LOGERR(_swix(Hourglass_Smash, 0));	/* just in case */
    KbdClose();				/* just in case */

    reset_char_defs();
    
#ifdef DEBUG
    {
    time_t tp;
    time(&tp);
    TRACE((TC_UI, TT_API1, "(1) *** " APP_NAME " exiting at %s", ctime(&tp)));
    }

    LogClose();
#endif
}

/* called from disconnect confirm toolbox event */

static int confirm_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    kill_current_session();

    switch (quitting)
    {
    case quitting_NO:
	break;

    case quitting_SELF:
	running = FALSE;
	break;

    case quitting_DESKTOP:
	running = FALSE;
	LOGERR(wimp_process_key(0x1FC));
	break;
    }
    
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

/* Called from Quit_Cancel event */

static int try_disconnect_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    TRACE((TC_UI, TT_API1, "disconnect_handler:\n"));

    if (disconnect_id == NULL_ObjectId ||
	LOGERR(toolbox_show_object(0, disconnect_id, Toolbox_ShowObject_Centre, NULL,
				   id_block ? id_block->self_id : NULL_ObjectId,
				   id_block ? id_block->self_component : NULL_ComponentId)) != NULL)
    {
	kill_current_session();
    }

    quitting = quitting_NO;

    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(handle);
}

/* called from Quit menu entry */

static int try_quit_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    if (!session_connected(current_session))
    {
	running = FALSE;
    }
    else
    {
	if (disconnect_id == NULL_ObjectId ||
	    LOGERR(toolbox_show_object(0, disconnect_id, Toolbox_ShowObject_Centre, NULL,
				       id_block ? id_block->self_id : NULL_ObjectId,
				       id_block ? id_block->self_component : NULL_ComponentId)) != NULL)
	{
	    kill_current_session();
	    running = FALSE;
	}

	quitting = quitting_SELF;
    }

    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

/* called from the PREQUIT message */

static int pre_quit_handler(WimpMessage *message, void *handle)
{
    if (session_connected(current_session))
    {
	WimpMessage reply;

	/* see if it is just us or others also */
	quitting = message->hdr.size == 20 || (message->data.words[0] & 1) == 0 ? quitting_DESKTOP : quitting_SELF;

	/* acknowledge the message to stop the quit */
	reply = *message;
	reply.hdr.your_ref = message->hdr.my_ref;
	err_fatal(wimp_send_message(Wimp_EUserMessageAcknowledge, &reply,
				    message->hdr.sender, 0, NULL));

	/* open the query box */
	if (disconnect_id == NULL_ObjectId ||
	    LOGERR(toolbox_show_object(0, disconnect_id, Toolbox_ShowObject_Centre, NULL, NULL_ObjectId, NULL_ComponentId)) != NULL)
	{
	    kill_current_session();
	    running = FALSE;
	}
    }

    return 1;
    NOT_USED(handle);
}

/* called from the QUIT message */

static int quit_handler(WimpMessage *message, void *handle)
{
    TRACE((TC_UI, TT_API1, "quit_handler: sess %p\n", current_session));
    
    kill_current_session();

    running = FALSE;

    return 1;
    NOT_USED(message);
    NOT_USED(handle);
}

static int key_handler(int event_code, WimpPollBlock *event, IdBlock *id_block, void *handle)
{
    LOGERR(wimp_process_key(event->key_pressed.key_code));
    return 1;
}

static int null_handler(int event_code, WimpPollBlock *event, IdBlock *id_block, void *handle)
{
#ifdef DEBUG
    extern void *event__id_block;
    if (event__id_block == NULL)
    {
	TRACE((TC_UI, TT_API1, "**** null_handler: in: id_block gone null ****" ));
    }
#endif
    
    DTRACE((TC_UI, TT_API1, "null_handler: sess %p\n", current_session));

    if (!splash_check_close())
	splash_force_top();
    
    if (session_connected(current_session))
    {
	/* once a full connection has been made then this call won't
         * return until the user logs off or disconnects
	 */

	if (!session_poll(current_session))
	{
	    TRACE((TC_UI, TT_API1, "null_handler: poll returned 0\n"));

	    kill_current_session();
	}

	DTRACE((TC_UI, TT_API1, "null_handler: poll returned 1\n"));

	// clean up if we are not suspendable
//	else if (!cli_suspendable)
//	{
//	    session_close_if_connected(current_session);
//	}
    }
    else if (current_session)
    {
	/* kick off the first stage of the connection sequence */
	connection_state(connection_NULL);
    }
    
#ifdef DEBUG
    if (event__id_block == NULL)
    {
	TRACE((TC_UI, TT_API1, "**** null_handler: out: id_block gone null ****" ));
    }
#endif
    
    return 0;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

static int datasave_handler(WimpMessage *message, void *handle)
{
    WimpDataSaveMessage *msg = &message->data.data_save;
    
    if (msg->file_type == filetype_ICA)
    {
	WimpMessage reply;

	/* report error if we are already connected */
	if (current_session)
	{
	    msg_report(utils_msgs_lookup("conn"));
	    return 0;
	}

	reply = *message;
	reply.hdr.size = sizeof(WimpDataSaveAckMessage);
	reply.hdr.action_code = Wimp_MDataSaveAck;
	reply.hdr.your_ref = message->hdr.my_ref;
	reply.data.data_save_ack.estimated_size = -1;
	strcpy(reply.data.data_save_ack.leaf_name, "<Wimp$Scrap>");

	err_fatal(wimp_send_message(Wimp_EUserMessage, &reply,
				    message->hdr.sender, 0, NULL));

	scrap_ref = reply.hdr.my_ref;
	return 1;
    }

    msg_report(utils_msgs_lookup("badtype"));
    return 0;
    NOT_USED(handle);
}

static int dataload_handler(WimpMessage *message, void *handle)
{
    WimpDataLoadMessage *msg = &message->data.data_load;
    
    if (msg->file_type == filetype_ICA)
    {
	WimpMessage reply;

	/* report error if we are already connected */
	if (current_session)
	{
	    msg_report(utils_msgs_lookup("conn"));
	    return 0;
	}

	reply = *message;
	reply.hdr.size = sizeof(WimpDataLoadAckMessage);
	reply.hdr.action_code = Wimp_MDataLoadAck;
	reply.hdr.your_ref = message->hdr.my_ref;

	err_fatal(wimp_send_message(Wimp_EUserMessage, &reply,
				    message->hdr.sender, 0, NULL));

	current_session = session_open(msg->leaf_name, scrap_ref == message->hdr.your_ref);
	
	if (current_session)
	    connection_state(connection_OPENED);
	
	return 1;
    }

    msg_report(utils_msgs_lookup("badtype"));
    return 0;
    NOT_USED(handle);
}

static int dataopen_handler(WimpMessage *message, void *handle)
{
    WimpDataOpen *msg = &message->data.data_open;

    TRACE((TC_UI, TT_API1, "dataopen_handler: filetype %x\n", msg->file_type));

    if (msg->file_type == filetype_ICA)
    {
	WimpMessage reply;

	reply = *message;
	reply.hdr.size = sizeof(WimpDataLoadAckMessage);
	reply.hdr.action_code = Wimp_MDataLoadAck;
	reply.hdr.your_ref = message->hdr.my_ref;

	err_fatal(wimp_send_message(Wimp_EUserMessage, &reply,
				    message->hdr.sender, 0, NULL));

	/* report error if we are already connected */
	if (current_session)
	{
	    msg_report(utils_msgs_lookup("conn"));
	    return 0;
	}

	current_session = session_open(msg->path_name, FALSE);

	if (current_session)
	    connection_state(connection_OPENED);
	
	return 1;
    }
    return 0;

    NOT_USED(handle);
}

static char *get_ptr(urlopen_data *u, string_value sv)
{
    return sv.offset == 0 ? 0 : sv.offset <= 236 ? (char *)u + sv.offset : sv.ptr;
}

static int openurl_handler(WimpMessage *message, void *handle)
{
    urlopen_data *msg = (urlopen_data *)&message->data;
    char *url, *bfile = NULL;

    if (msg->indirect.tag == 0)
    {
	url = get_ptr(msg, msg->indirect.url);

	if (message->hdr.size > 28)
	    bfile = get_ptr(msg, msg->indirect.body_file);
    }
    else
    {
	url = msg->url;
    }

    TRACE((TC_UI, TT_API1, "openurl_handler: url '%s' tag %d\n", url, msg->indirect.tag));

    if (url && strnicmp(url, "ica:", 4) == 0)
    {
	WimpMessage reply;

	/* claim message with acknowledge */
	reply = *message;
	reply.hdr.your_ref = message->hdr.my_ref;

	err_fatal(wimp_send_message(Wimp_EUserMessageAcknowledge, &reply,
				    message->hdr.sender, 0, NULL));

	/* report error if we are already connected */
	if (current_session)
	{
	    msg_report(utils_msgs_lookup("conn"));
	    return 0;
	}

	current_session = session_open_url(url, bfile);

	if (current_session)
	    connection_state(connection_OPENED);
	
	return 1;
    }
    return 0;

    NOT_USED(handle);
}

static int openuri_handler(WimpMessage *message, void *handle)
{
    int flags = message->data.words[0];
    const char *uri = (const char *)message->data.words[1];
    int uri_handle = message->data.words[2];

    if (strnicmp(uri, "ica:", 4) == 0)
    {
	WimpMessage reply;

	/* report error if we are already connected */
	if (current_session)
	{
	    msg_report(utils_msgs_lookup("conn"));
	    return 0;
	}

	if ((flags & 0x01) == 0)
	{
	    int len;
	    char *url = NULL;
	    _kernel_oserror *e;

	    e = _swix(URI_RequestURI, _INR(0,1) | _IN(3) | _OUT(2), 0, 0, uri_handle, &len);
	    if (!e)
	    {
		url = malloc(len);
		e = _swix(URI_RequestURI, _INR(0,3), 0, url, len, uri_handle);
	    }

	    if (!e)
	    {
		current_session = session_open_url(url, NULL);

		if (current_session)
		    connection_state(connection_OPENED);
	    }
	    
	    free(url);
	}

	reply = *message;
	reply.hdr.your_ref = message->hdr.my_ref;
	reply.hdr.action_code = MESSAGE_URI_MPROCESSACK;
	wimp_send_message(Wimp_EUserMessage, &reply, message->hdr.sender, NULL, 0);
	
	return 1;
    }

    return 0;

    NOT_USED(handle);
}

static int control_handler(WimpMessage *message, void *handle)
{
    icaclient_message_control *control = (icaclient_message_control *)&message->data;

    TRACE((TC_UI, TT_API1, "control_handler: reason %d flags 0x%x\n", control->reason, control->flags));

    switch (control->reason)
    {
    case icaclient_CONTROL_CONNECT:
    {
	icaclient_message_control_connect *control = (icaclient_message_control_connect *)&message->data;
	
	/* report error if we are already connected */
	if (current_session)
	{
	    control_ack(message, NULL, icaclient_ERROR_CONNECTION_OPEN);
	    return 1;
	}

	/* start the open */
	switch (control->flags & icaclient_connect_DATA_MASK)
	{
	case icaclient_connect_SERVER_DESCRIPTION:
	    current_session = session_open_appsrv(control->data.server_description);
	    break;
	case icaclient_connect_SERVER_NAME:
	    current_session = session_open_server(control->data.server_name);
	    break;
	case icaclient_connect_ICA_FILE:
	    current_session = session_open(control->data.ica_file, (control->flags & icaclient_connect_DELETE_ICA) != 0);
	    break;
	}

	if (current_session)
	    connection_state(connection_OPENED);
	
	/* report the status */
	control_ack(message, current_session, current_session == NULL ? icaclient_ERROR_CONNECTION_FAILED : 0);
	break;
    }
    
    case icaclient_CONTROL_RECONNECT:
    {
	icaclient_message_control_reconnect *control = (icaclient_message_control_reconnect *)&message->data;
	
	if (control->session_handle != current_session)
	{
	    control_ack(message, control->session_handle, icaclient_ERROR_HANDLE_UNKNOWN);
	    return 1;
	}

	control_ack(message, current_session, 0);

	session_resume(current_session);
	break;
    }
    
    case icaclient_CONTROL_DISCONNECT:
    {
	icaclient_message_control_disconnect *control = (icaclient_message_control_disconnect *)&message->data;
	
	if (control->session_handle != current_session)
	{
	    control_ack(message, control->session_handle, icaclient_ERROR_HANDLE_UNKNOWN);
	    return 1;
	}

	control_ack(message, current_session, 0);

	main_close_session(current_session);
	break;
    }
    
    case icaclient_CONTROL_QUIT:
    {
	if (current_session != NULL)
	{
	    control_ack(message, NULL, icaclient_ERROR_CONNECTION_OPEN);
	    return 1;
	}

	control_ack(message, NULL, 0);
	running = FALSE;
	break;
    }
    }

    return 1;
    NOT_USED(handle);
}

static int config_handler(WimpMessage *message, void *handle)
{
    icaclient_message_config *config = (icaclient_message_config *)&message->data;
    switch (config->reason)
    {
    case icaclient_CONFIG_FILE_UPDATED:
	if (config->flags & icaclient_config_APPSRV_CHANGED)
	{
	    /* if the AppSrv file has changed then uncache the server details */
	    serverlist_uncache();

	    /* and get rid of the menu object */
	    if (connect_menu_id != NULL_ObjectId)
	    {
		ObjectId menu_id;
		ComponentId menu_cmp;
	    
		if (LOGERR(toolbox_get_parent(0, connect_menu_id, &menu_id, &menu_cmp)) == NULL && menu_id != NULL_ObjectId)
		    LOGERR(menu_set_sub_menu_show(0, menu_id, menu_cmp, NULL_ObjectId));
		
		LOGERR(toolbox_delete_object(0, connect_menu_id));
		connect_menu_id = NULL_ObjectId;
	    }
	}
	break;
    }
    return 1;
    NOT_USED(handle);
}

/* --------------------------------------------------------------------------------------------- */

static char *printinfo_name(void)
{
    char *name;
    if (_swix(PDriver_Info, _OUT(4), &name) == NULL)
	return strdup(name);
    return NULL;
}

static int printer_handler(WimpMessage *message, void *handle)
{
    switch (message->hdr.action_code)
    {
    case message_PSPrinterAck:
    {
	printer_message_ps_query *query = (printer_message_ps_query *)&message->data;
	
	TRACE((TC_UI, TT_API1, "PSPrinterAck: buffer %p size %d", query->buffer_address, query->buffer_size));

	if (query->buffer_address == NULL)
	{
	    query->buffer_address = rma_alloc(query->buffer_size);

	    message->hdr.action_code = message_PSPrinterQuery;
	    message->hdr.your_ref = message->hdr.my_ref;
	    LOGERR(wimp_send_message(Wimp_EUserMessageRecorded, message, 0, NULL, 0));
	}
	else
	{
	    const char *s = query->buffer_address;

	    printer_name = strdup(s);
	    printer_type = strdup(s + strlen(s) + 1);

	    TRACE((TC_UI, TT_API1, "PSPrinterAck: name '%s' type '%s'", printer_name, printer_type));

	    rma_free(query->buffer_address);

	    printinfo_deregister();
	}
	break;
    }
    
    case message_PSPrinterNotPS:
	TRACE((TC_UI, TT_API1, "PSPrinterNotPS:"));

	printer_name = printinfo_name();
	printinfo_deregister();
	break;
    }
    
    return 1;
}

static int printer_ack_handler(int event_code, WimpPollBlock *event, IdBlock *id_block, void *handle)
{
    TRACE((TC_UI, TT_API1, "printer_ack_handler: action %d", event->user_message_acknowledge.hdr.action_code));

    if (event->user_message_acknowledge.hdr.action_code == message_PSPrinterQuery)
    {
	printer_message_ps_query *query = (printer_message_ps_query *)&event->user_message_acknowledge.data;

	printer_name = printinfo_name();

	if (query->buffer_address)
	    rma_free(query->buffer_address);

	printinfo_deregister();
	return 1;
    }
    return 0;
    NOT_USED(event_code);
    NOT_USED(id_block);
    NOT_USED(handle);
}

static void printinfo_deregister(void)
{
    err_fatal(event_deregister_message_handler(message_PSPrinterNotPS, printer_handler, NULL));
    err_fatal(event_deregister_message_handler(message_PSPrinterAck, printer_handler, NULL));
    err_fatal(event_deregister_wimp_handler(-1, Wimp_EUserMessageAcknowledge, printer_ack_handler, NULL));

    printinfo_state = initop_COMPLETED;
    
    connection_state(connection_PRINTINFO_COMPLETED);
}

static void printinfo_register(void)
{
    err_fatal(event_register_message_handler(message_PSPrinterNotPS, printer_handler, NULL));
    err_fatal(event_register_message_handler(message_PSPrinterAck, printer_handler, NULL));
    err_fatal(event_register_wimp_handler(-1, Wimp_EUserMessageAcknowledge, printer_ack_handler, NULL));

    printinfo_state = initop_RUNNING;
}

static void printinfo_request(void)
{
    WimpMessage msg;
    printer_message_ps_query *query;

    TRACE((TC_UI, TT_API1, "printinfo_request:"));

    /* free cached info */
    free(printer_name);
    free(printer_type);
    printer_name = printer_type = NULL;
    
    /* send query message */
    msg.hdr.size = sizeof(msg.hdr) + sizeof(*query);
    msg.hdr.action_code = message_PSPrinterQuery;
    msg.hdr.your_ref = 0;

    query = (printer_message_ps_query *)&msg.data;
    query->buffer_address = 0;
    query->buffer_size = 0;
    LOGERR(wimp_send_message(Wimp_EUserMessageRecorded, &msg, 0, NULL, 0));
    
    /* register handlers */
    printinfo_register();
}

/* --------------------------------------------------------------------------------------------- */

static void dial_end(BOOL success);

static int dial_service_handler(WimpMessage *message, void *handle)
{
    _kernel_swi_regs *r = (_kernel_swi_regs *)&message->data.words[0];

    if (r->r[1] == Service_DiallerStatus)
    {
	int new_state = r->r[2];

	/* IP is up */
	if (new_state == dialler_CONNECTED_OUTGOING)
	{
	    dial_end(TRUE);
	}
	/* IP is down */
	else if (new_state == dialler_DISCONNECTED)
	{
	    dial_end(FALSE);
	}
	/* error has occurred */
	else if ((new_state & 0xf0) == 0x80)
	{
	    dial_end(FALSE);
	}
	return 1;
    }

    return 0;
}

static void dial_end(BOOL success)
{
    LOGERR(event_deregister_message_handler(wimp_MSERVICE, dial_service_handler, NULL));
    LOGERR(_swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle));

    dial_state = initop_COMPLETED;
    connection_state(connection_DIAL_COMPLETED);
}

static void dial_start(void)
{
    if (!cli_modem)
    {
	dial_state = initop_COMPLETED;
	return;
    }
    
    if (LOGERR(_swix(TaskModule_RegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle)) == NULL &&
	LOGERR(event_register_message_handler(wimp_MSERVICE, dial_service_handler, NULL)) == NULL &&
	LOGERR(_swix(NCDialUI_Start, _IN(0), 0)) == NULL)
    {
	dial_state = initop_RUNNING;
    }
    else
    {
	event_deregister_message_handler(wimp_MSERVICE, dial_service_handler, NULL);
	_swix(TaskModule_DeRegisterService, _INR(0,2), 0, Service_DiallerStatus, task_handle);

	dial_state = initop_COMPLETED;
    }
}

/* --------------------------------------------------------------------------------------------- */

static void write_string(int file_handle, const char *s)
{
    LOGERR(_swix(OS_GBPB, _INR(0,3), 2, file_handle, s, strlen(s)));
}

static int save_desktop_handler(WimpMessage *message, void *handle)
{
    WimpSaveDesktopMessage *save = &message->data.save_desktop;

    write_string(save->file_handle, "/");
    write_string(save->file_handle, getenv(APP_DIR_VAR));
    write_string(save->file_handle, "\n");
    
    return 1;
    NOT_USED(handle);
}

/* --------------------------------------------------------------------------------------------- */

static int splash_redraw_handler(int event_code, WimpPollBlock *event, IdBlock *id_block, void *handle)
{
    void *area, *sprite;
    WimpRedrawWindowBlock redraw;
    int more;
    
    area = splash_ptr;
    sprite = (char *)splash_ptr + 16;

    redraw.window_handle = event->redraw_window_request.window_handle;
    LOGERR(wimp_redraw_window(&redraw, &more));

    while (more)
    {
	/* plot sprite */
	LOGERR(_swix(OS_SpriteOp, _INR(0,7),
		     52 + 512,
		     area, sprite,
		     redraw.visible_area.xmin,
		     redraw.visible_area.ymin,
		     0,
		     NULL,
		     splash_coltrans));

	LOGERR(wimp_update_window(&redraw, &more));
    }
    
    return 1;
    NOT_USED(event_code);
    NOT_USED(handle);
}

static void splash_open(void)
{
    int fh = open(SPLASH_FILE, O_RDONLY | O_BINARY);
    if (fh)
    {
	int len = _filelength(fh);
	int w, h;
	int screen_w, screen_h;
	int x, y;
	void *area, *sprite;
	int coltrans_size;

	TRACE((TC_UI, TT_API1, "splash_open: fh %d len %d", fh, len));
	
	/* load splash screen */
	if (LOGERR(MemFlex_Alloc(&splash_ptr, len + 4)) == NULL)
	{
	    _kernel_oserror *e;

	    *(int *)splash_ptr = len + 4;
	    read(fh, (char *)splash_ptr + 4, len);

	    /* check for sprite validity */
	    e = LOGERR(_swix(OS_SpriteOp, _INR(0,1), 17 + 512, splash_ptr));
	    if (e && e->errnum == 1822)
		MemFlex_Free(&splash_ptr);
	}
	close(fh);

	if (splash_ptr)
	{
	    /* read sprite and screen info */
	    area = splash_ptr;
	    sprite = (char *)splash_ptr + 16;
	    LOGERR(_swix(OS_SpriteOp, _INR(0,2) | _OUTR(3,4),
			 40 + 512, area, sprite,
			 &w, &h));

	    GetModeSpec(&screen_w, &screen_h);

	    x = (screen_w - w)/2 * 2;
	    y = (screen_h - h)/2 * 2;
	
	    /* open window if we are multitasking */
	    if (cli_multitask)
	    {
		WindowShowObjectBlock show;
		LOGERR(toolbox_create_object(0, tbres_SPLASH_W, &splash_id));

		show.visible_area.xmin = x;
		show.visible_area.ymin = y;
		show.visible_area.xmax = x + w*2;
		show.visible_area.ymax = y + h*2;
		show.xscroll = show.yscroll = 0;
		show.behind = -1;
		LOGERR(toolbox_show_object(0, splash_id, Toolbox_ShowObject_FullSpec, &show, NULL_ObjectId, NULL_ComponentId));
	    
		event_register_wimp_handler(splash_id, Wimp_ERedrawWindow, splash_redraw_handler, NULL);
	    }
	
	    /* generate table */
	    LOGERR(_swix(ColourTrans_GenerateTable, _INR(0,5) | _OUT(4),
			 area, sprite,
			 -1, -1,
			 NULL, 1,
			 &coltrans_size));

	    if ((splash_coltrans = malloc(coltrans_size)) != NULL)
	    {
		/* generate table */
		LOGERR(_swix(ColourTrans_GenerateTable, _INR(0,5),
			     area, sprite,
			     -1, -1,
			     splash_coltrans, 1));

		/* plot sprite */
		LOGERR(_swix(OS_SpriteOp, _INR(0,7),
			     52 + 512,
			     area, sprite,
			     x, y, 0,
			     NULL,
			     splash_coltrans));
	    }

	    splash_timer = clock() + SPLASH_TIME;
	    splash_state = initop_RUNNING;
	}
    }
}

static void splash_force_top(void)
{
    if (splash_id)
    {
	WimpGetWindowStateBlock state;
	    
	LOGERR(window_get_wimp_handle(0, splash_id, &state.window_handle));
	LOGERR(wimp_get_window_state(&state));
	
	if ((state.flags & WimpWindow_NotCovered) == 0)
	{
	    WindowShowObjectBlock show;
	    show.visible_area = state.visible_area;
	    show.xscroll = show.yscroll = 0;
	    show.behind = -1;
	    LOGERR(toolbox_show_object(0, splash_id, Toolbox_ShowObject_FullSpec, &show, NULL_ObjectId, NULL_ComponentId));
	}
    }
}

static void splash_close(void)
{
    MemFlex_Free(&splash_ptr);

    free(splash_coltrans);
    splash_coltrans = NULL;

    if (splash_id != NULL_ObjectId)
    {
	event_deregister_wimp_handler(splash_id, Wimp_ERedrawWindow, splash_redraw_handler, NULL);
    
	LOGERR(toolbox_hide_object(0, splash_id));
	LOGERR(toolbox_delete_object(0, splash_id));
	splash_id = NULL_ObjectId;
    }

    splash_state = initop_COMPLETED;
}

static BOOL splash_check_close(void)
{
    if (splash_state != initop_RUNNING)
	return TRUE;

    if (clock() > splash_timer)
    {
	splash_close();
	return TRUE;
    }

    return FALSE;
}

/* --------------------------------------------------------------------------------------------- */

/*
 * EMLogInit
 *
 *
 * Initialize the important stuff for the Log file
 *
 */
static int log_init(void)
{
   LOGOPEN EMLogInfo;
   int rc = CLIENT_STATUS_SUCCESS;

   LogClose();
   
   EMLogInfo.LogFlags   = 0;
   if (cli_remote_debug)
       EMLogInfo.LogFlags |= LOG_REMOTE;
   if (cli_file_debug)
       EMLogInfo.LogFlags |= LOG_FILE;

#if 1
   EMLogInfo.LogClass    = LOG_ASSERT | TC_TW;
   EMLogInfo.LogEnable   = TT_ERROR;
   EMLogInfo.LogTWEnable = TT_TW_DIM;
#else
   EMLogInfo.LogClass   = TC_ALL;
   EMLogInfo.LogEnable  = TT_ERROR;
   EMLogInfo.LogTWEnable = TT_ERROR;
#endif

   strcpy(EMLogInfo.LogFile, cli_logfile ? cli_logfile : "");

   rc = LogOpen(&EMLogInfo);

   return(rc);
}

static void process_args(int argc, char *argv[])
{
    int i;

    for (i = 0; i < argc; i++)
    {
        char *s = argv[i];

        TRACE((TC_UI, TT_API1, "(3) arg(%d)='%s'\n", i, s));

	if (*s == '-')
	{
	    switch (tolower(*(s+1)))
	    {
	    case 'c':	    /* connect via modem on startup */
		cli_modem = TRUE;
		break;

	    case 'd':	    /* output post mortem on abort? */
		cli_dopostmortem = TRUE;
		break;

	    case 'f':      /* file to play initially */
		if (i+1 < argc && argv[i+1][0] != '-')
	            cli_filename = strdup(argv[++i]);
		break;

	    case 'g':	    /* enable log file */
		cli_file_debug = TRUE;
		break;

	    case 'i':      /* create iconbar icon */
		cli_iconbar = TRUE;
		cli_suspendable = TRUE;
		cli_multitask = TRUE;
		break;

	    case 'l':	    /* log file */
		if (i+1 < argc && argv[i+1][0] != '-')
		    cli_logfile = strdup(argv[++i]);
		break;

	    case 'm':      /* multitasking startup */
		cli_multitask = TRUE;
		break;

	    case 'o':	    /* loop */
		cli_loop = TRUE;
		break;

	    case 'r':	    /* enable remote debugging */
		cli_remote_debug = TRUE;
		break;

	    case 's':      /* allow suspend and resume */
		cli_suspendable = TRUE;
		cli_multitask = TRUE;
		break;

	    case 'u':      /* filename is a URL */
		cli_file_is_url = TRUE;

		if (i+1 < argc && argv[i+1][0] != '-')
		{
	            cli_filename = strdup(argv[++i]);

		    if (i+1 < argc && argv[i+1][0] != '-')
			cli_postfile = strdup(argv[++i]);
		}
		break;
	    } /* switch */
	}
    } /* for */
}

#define MAX_OPTION_ARGS	10

static void process_options(const char *val)
{
    char *s = strdup(val);

    if ((s = strtok(s, " ")) != NULL)
    {
	int argc = 0;
	char *argv[MAX_OPTION_ARGS];
	do
	{
	    argv[argc++] = s;
	}
	while ((s = strtok(NULL, " ")) != NULL && argc < MAX_OPTION_ARGS);

	process_args(argc, argv);
    }
    
    free(s);
}

/* --------------------------------------------------------------------------------------------- */

static void initialise(int argc, char *argv[])
{
    int current_wimp;
    void *sprite;

    /* usual library things */
    setlocale(LC_ALL, "");
    setbuf(stderr, NULL);
    atexit(cleanup);

    /* decode args */
    process_args(argc-1, argv+1);
    process_options(getenv(OPTIONS_VAR));
    process_options(getenv(cli_filename ? FILE_OPTIONS_VAR : NOFILE_OPTIONS_VAR));

    if (cli_iconbar)
	_swix(OS_CLI, _IN(0), ENSURE_TOOLBOX_CMD);
    
#ifdef DEBUG
    log_init();
#endif
    /* initialise toolbox first to get messages file */
    err_fatal(toolbox_initialise(0, 310, &Wimp_MessageList[0], ToolBox_EventList, APP_DIR,
				     &message_block, &event_id_block,
				     &current_wimp, &task_handle, &sprite));

    LOGERR(MemFlex_Initialise2(APP_NAME " flex"));
    LOGERR(MemHeap_Initialise(APP_NAME " heap"));
    
    /* cleanup that needs task running */
    atexit(cleanup_task);

    /* enabled msgs calls */
    utils_init(&message_block);

    /* after main and utils init */
    signal_setup();

    /* initialise event library */
    err_fatal(event_initialise(&event_id_block));

    /* put up splash screen */
    splash_open();
    
    if (cli_multitask)
    {
	/* attach quit handlers */
	err_fatal(event_register_message_handler(Wimp_MQuit, quit_handler, NULL));
	err_fatal(event_register_message_handler(Wimp_MPreQuit, pre_quit_handler, NULL));
	err_fatal(event_register_toolbox_handler(-1, tbres_event_CONFIRM, confirm_handler, NULL));

	/* other general handlers */
	err_fatal(event_register_wimp_handler(-1, Wimp_ENull, null_handler, NULL));
	err_fatal(event_register_wimp_handler(-1, Wimp_EKeyPressed, key_handler, NULL));

	err_fatal(event_register_message_handler(Wimp_MDataOpen, dataopen_handler, NULL));
	err_fatal(event_register_message_handler(Wimp_MDataLoad, dataload_handler, NULL));
	err_fatal(event_register_message_handler(Wimp_MDataSave, datasave_handler, NULL));

	err_fatal(event_register_message_handler(wimp_MOPENURL, openurl_handler, NULL));
	err_fatal(event_register_message_handler(MESSAGE_URI_MPROCESS, openuri_handler, NULL));

	err_fatal(event_register_message_handler(message_ICACLIENT_CONTROL, control_handler, NULL));
	err_fatal(event_register_message_handler(message_ICACLIENT_CONFIG, config_handler, NULL));

	err_fatal(event_register_message_handler(Wimp_MSaveDesktop, save_desktop_handler, NULL));

	err_fatal(event_register_toolbox_handler(-1, tbres_event_CONNECT, connect_handler, NULL));
	err_fatal(event_register_toolbox_handler(-1, tbres_event_DISCONNECT, try_disconnect_handler, NULL));
	err_fatal(event_register_toolbox_handler(-1, tbres_event_QUIT, try_quit_handler, NULL));

	err_fatal(event_register_toolbox_handler(-1, ProgInfo_AboutToBeShown, proginfo_handler, NULL));
	err_fatal(event_register_toolbox_handler(-1, tbres_event_SHOWING_ICON_MENU, icon_menu_handler, NULL));

	err_fatal(event_register_toolbox_handler(-1, tbres_event_SERVER_CONNECT, server_connect_handler, NULL));

	/* initialise application components */

	/* create iconbar icon */
	if (cli_iconbar)
	{
	    int iconbar_id;
	    err_fatal(toolbox_create_object(0, tbres_ICON, &iconbar_id));
	    err_fatal(toolbox_show_object(0, iconbar_id, Toolbox_ShowObject_Default, NULL, NULL_ObjectId, NULL_ComponentId));

	    LOGERR(toolbox_create_object(0, tbres_DISCONNECT_W, &disconnect_id));
	}
    }    
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
    time_t tp;

    time(&tp);
    TRACE((TC_UI, TT_API1, "(1) *** New " APP_NAME " " VERSION_STRING " started %s", ctime(&tp)));
#endif

    initialise(argc, argv);

    if (cli_filename != NULL && cli_filename[0] != '\0')
    {
	if (cli_multitask)
	{
	    if (cli_file_is_url)
		current_session = session_open_url(cli_filename, cli_postfile);
	    else
		current_session = session_open(cli_filename, FALSE);

	    if (current_session)
		connection_state(connection_OPENED);
	}
	else
	{
	    while (!splash_check_close())
		;

	    /* do anything here that we can without relying on messages */
	    printer_name = printinfo_name();
	
	    do
		session_run(cli_filename, cli_file_is_url, cli_postfile);
	    while (cli_loop);

	    return EXIT_SUCCESS;
	}
    }
    else
    {
	if (!cli_iconbar && !cli_suspendable)
	    return EXIT_FAILURE;
    }

    err_fatal(event_set_mask(event_mask));

    TRACE((TC_UI, TT_API1, "(1) Entering poll loop"));
    while (running)
    {
	int event_code;
	WimpPollBlock poll_block;

//	LOGERR(_swix(OS_ReadMonotonicTime, _OUT(0), &t));
//      event_poll_idle(&event_code, &poll_block, t + 100, NULL);
        event_poll(&event_code, &poll_block, NULL);

#ifdef DEBUG
	LogPoll();
#endif

    	if (event_code != 0)
    	    TRACE((TC_UI, TT_API1, "(7) Poll: %d running %d quitting %d", event_code, running, quitting));
    }

    return EXIT_SUCCESS;
}

/* eof main.c */
