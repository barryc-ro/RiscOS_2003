/* > main.c

 * Main file for WSClient

 */

#include <ctype.h>
#include <locale.h>
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

#include "tbres.h"
#include "utils.h"
#include "version.h"
#include "rdebug.h"
#include "session.h"

#include "../inc/clib.h"

/* --------------------------------------------------------------------------------------------- */

#define POLL_TIME	            100	/* 1 second */
#define MIN_ESC_TIME	            50	/* cs */

/* --------------------------------------------------------------------------------------------- */

static MessagesFD message_block;    /* declare a message block for use with toolbox initialise */
static IdBlock event_id_block;      /* declare an event block for use with toolbox initialise  */

static int ToolBox_EventList[] =
{
    tbres_event_CONNECT,
    tbres_event_DISCONNECT,
    Quit_Quit,
    0
};

static int Wimp_MessageList[] =
{
    Wimp_MDataLoad,
    Wimp_MDataOpen,
    0
};

static int event_mask =
    Wimp_Poll_PointerLeavingWindowMask
    | Wimp_Poll_PointerEnteringWindowMask
    | Wimp_Poll_PollWordNonZeroMask;

/* --------------------------------------------------------------------------------------------- */

/* cli set values */

static char *cli_filename = NULL;
static int cli_iconbar = FALSE;
static int cli_dopostmortem = FALSE;

static Session current_session = NULL;

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

static int connect_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    DBG(("connect_handler:\n"));

    if (current_session)
	session_resume(current_session);
    else
	msg_report("Run an ICA file to make a connection");
	    
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

static int disconnect_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    DBG(("disconnect_handler:\n"));

    if (current_session)
    {
	session_close(current_session);
	current_session = NULL;
    }

    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

/* --------------------------------------------------------------------------------------------- */

static void cleanup_task(void)
{
}

extern void LogClose(void);

static void cleanup(void)
{
    DBG(("cleanup:\n"));

    if (current_session)
    {
	session_close(current_session);
	current_session = NULL;
    }
    
    _swix(Hourglass_Smash, 0);	/* just in case */

#ifdef DEBUG
    {
    time_t tp;
    time(&tp);
    DBG(("(1) *** " APP_NAME " exiting at %s", ctime(&tp)));
    }

    LogClose();
#endif
}

static int quit_handler(WimpMessage *message, void *handle)
{
    exit(EXIT_SUCCESS);
    return 1;
    NOT_USED(message);
    NOT_USED(handle);
}

static int quit_handler1(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    quit_handler(NULL, NULL);
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

static int null_handler(int event_code, WimpPollBlock *event, IdBlock *id_block, void *handle)
{
    if (current_session)
	if (!session_poll(current_session))
	{
	    DBG(("null_handler: poll returned 0\n"));

	    session_close(current_session);
	    current_session = NULL;
	}    
    
    return 0;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

static int dataload_handler(WimpMessage *message, void *handle)
{
    WimpDataLoadMessage *msg = &message->data.data_load;

    if (current_session)
    {
	msg_report(utils_msgs_lookup("conn"));
	return 0;
    }
    
    if (msg->file_type == filetype_ICA)
    {
	WimpMessage reply;

	reply = *message;
	reply.hdr.action_code = Wimp_MDataLoadAck;
	reply.hdr.your_ref = message->hdr.my_ref;

	err_fatal(wimp_send_message(Wimp_EUserMessage, &reply,
				    message->hdr.sender, 0, NULL));

	current_session = session_open(msg->leaf_name);

	return 1;
    }

    return 0;
}

static int dataopen_handler(WimpMessage *message, void *handle)
{
    WimpDataOpen *msg = &message->data.data_open;

    if (current_session)
    {
	msg_report(utils_msgs_lookup("conn"));
	return 0;
    }

    if (msg->file_type == filetype_ICA)
    {
	WimpMessage reply;

	reply = *message;
	reply.hdr.action_code = Wimp_MDataLoadAck;
	reply.hdr.your_ref = message->hdr.my_ref;

	err_fatal(wimp_send_message(Wimp_EUserMessage, &reply,
				    message->hdr.sender, 0, NULL));

	current_session = session_open(msg->path_name);

	return 1;
    }
    return 0;

    NOT_USED(handle);
}

/* --------------------------------------------------------------------------------------------- */

static void process_args(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; i++)
    {
        char *s = argv[i];

        DBG(("(3) arg(%d)='%s'\n", i, s));

	if (*s == '-')
	{
	    switch (tolower(*(s+1)))
	    {
	    case 'd':	    /* output post mortem on abort? */
		cli_dopostmortem = TRUE;
		break;

	    case 'f':      /* file to play initially */
		if (i+1 < argc)
	            cli_filename = strdup(argv[++i]);
		break;

	    case 'i':      /* create iconbar icon */
		cli_iconbar = TRUE;
		break;
	    } /* switch */
	} /* if */
    } /* for */

    DBG(("(2) process_args result: file=\"%s\", dopostmortem=%d, iconbar=%d\n",
	 strsafe(cli_filename), cli_dopostmortem, cli_iconbar));
}

static void initialise(int argc, char *argv[])
{
    int current_wimp, task;
    void *sprite;

    /* usual library things */
    setlocale(LC_ALL, "");
    setbuf(stderr, NULL);
    atexit(cleanup);

    /* decode args */
    process_args(argc, argv);

    /* initialise toolbox first to get messages file */
#if 0
    /* Wimp 380 needed for embedded windows */
    if (toolbox_initialise(0, 380, &Wimp_MessageList[0], &ToolBox_EventList[0], APP_DIR,
			      &message_block, &event_id_block,
			      &current_wimp, &task, &sprite) != NULL)
#endif
    {
    	/* Wimp 310 needed for other facilities */
 	err_fatal(toolbox_initialise(0, 310, &Wimp_MessageList[0], ToolBox_EventList, APP_DIR,
				     &message_block, &event_id_block,
				     &current_wimp, &task, &sprite));
    }

    /* cleanup that needs task running */
    atexit(cleanup_task);

    /* enabled msgs calls */
    utils_init(&message_block);

    /* after main and utils init */
    signal_setup();

    /* initialise event library */
    err_fatal(event_initialise(&event_id_block));

    /* attach quit handlers */
    err_fatal(event_register_message_handler(Wimp_MQuit, quit_handler, NULL));
    err_fatal(event_register_toolbox_handler(-1, Quit_Quit, quit_handler1, NULL));

    /* other general handlers */
    err_fatal(event_register_wimp_handler(0, Wimp_ENull, null_handler, NULL));
    err_fatal(event_register_message_handler(Wimp_MDataOpen, dataopen_handler, NULL));
    err_fatal(event_register_message_handler(Wimp_MDataLoad, dataload_handler, NULL));

    err_fatal(event_register_toolbox_handler(-1, tbres_event_CONNECT, connect_handler, NULL));
    err_fatal(event_register_toolbox_handler(-1, tbres_event_DISCONNECT, disconnect_handler, NULL));

    /* initialise application components */

    /* create iconbar icon */
    if (cli_iconbar)
    {
	int iconbar_id;
	err_fatal(toolbox_create_object(0, tbres_ICON, &iconbar_id));
	err_fatal(toolbox_show_object(0, iconbar_id, Toolbox_ShowObject_Default, NULL, NULL_ObjectId, NULL_ComponentId));
    }
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
    time_t tp;

    rdebug_open();
    
    time(&tp);
    DBG(("(1) *** New " APP_NAME " " VERSION_STRING " started %s", ctime(&tp)));
#endif

    initialise(argc, argv);

    if (cli_filename && cli_filename[0] != '\0')
    {
	current_session = session_open(cli_filename);

	if (!cli_iconbar)
	    return EXIT_SUCCESS;
    }

    err_fatal(event_set_mask(event_mask));

    DBG(("(1) Entering poll loop\n"));
    while (TRUE)
    {
	int event_code;
	WimpPollBlock poll_block;

        event_poll_idle(&event_code, &poll_block, 100, NULL);
	rdebug_poll();

    	if (event_code != 0)
    	    DBG(("(7) Poll: %d\n", event_code));
    }

    return EXIT_SUCCESS;
}

/* eof main.c */
