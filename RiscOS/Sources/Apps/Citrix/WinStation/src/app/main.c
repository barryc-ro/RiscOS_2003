/* > main.c

 * Main file for WSClient

 */

#include "windows.h"

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
#include "tboxlibs/menu.h"

#include "tbres.h"
#include "utils.h"
#include "version.h"
#include "session.h"

#include "../inc/clib.h"
#include "../inc/client.h"
#include "../inc/debug.h"
#include "../dll/vd/vdtw31/twh2cinc.h"

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
    tbres_event_SHOWING_ICON_MENU,
    ProgInfo_AboutToBeShown,
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
static char *cli_logfile = NULL;
static int cli_iconbar = FALSE;
static int cli_dopostmortem = FALSE;
static int cli_remote_debug = FALSE;

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
    TRACE((TC_UI, TT_API1, "connect_handler:\n"));

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
    TRACE((TC_UI, TT_API1, "disconnect_handler:\n"));

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

#ifdef DEBUG
#define VERSION		Module_MajorVersion " [" __TIME__ ":" __DATE__ "]" Module_MinorVersion
#else
#define VERSION		Module_MajorVersion " (" Module_Date ") " Module_MinorVersion
#endif

static int proginfo_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    LOGERR(proginfo_set_version(0, id_block->self_id, VERSION));
    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(handle);
}

static int icon_menu_handler(int event_code, ToolboxEvent *event, IdBlock *id_block,
                         void *handle)
{
    LOGERR(menu_set_fade(0, id_block->self_id, tbres_c_CONNECT, current_session == NULL));
    LOGERR(menu_set_fade(0, id_block->self_id, tbres_c_DISCONNECT, current_session == NULL));
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

    if (current_session)
    {
	session_close(current_session);
	current_session = NULL;
    }
    
    LOGERR(_swix(Hourglass_Smash, 0));	/* just in case */

#ifdef DEBUG
    {
    time_t tp;
    time(&tp);
    TRACE((TC_UI, TT_API1, "(1) *** " APP_NAME " exiting at %s", ctime(&tp)));
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
    DTRACE((TC_UI, TT_API1, "null_handler: sess %p\n", current_session));
    
    if (current_session)
	if (!session_poll(current_session))
	{
	    TRACE((TC_UI, TT_API1, "null_handler: poll returned 0\n"));

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

    if (msg->file_type == filetype_ICA)
    {
	WimpMessage reply;

	if (current_session)
	{
	    msg_report(utils_msgs_lookup("conn"));
	    return 0;
	}

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

#ifdef DEBUG
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
   
   EMLogInfo.LogFlags   = (cli_remote_debug ? LOG_REMOTE : 0);
#if 0
   EMLogInfo.LogClass   = TC_UI | TC_TW;
   EMLogInfo.LogEnable  = TT_ERROR;
   EMLogInfo.LogTWEnable = TT_TW_res4;
#else
   EMLogInfo.LogClass   = TC_ALL;
   EMLogInfo.LogEnable  = TT_ERROR;
   EMLogInfo.LogTWEnable = TT_ERROR;
#endif

   strcpy(EMLogInfo.LogFile, cli_logfile ? cli_logfile : "");
   //lstrcpy(EMLogInfo.LogFile, "<Wimp$ScrapDir>." APP_NAME ".Log");

   rc = LogOpen(&EMLogInfo);

   return(rc);
}
#endif //DEBUG

/* --------------------------------------------------------------------------------------------- */

static void process_args(int argc, char *argv[])
{
    int i;

    for (i = 1; i < argc; i++)
    {
        char *s = argv[i];

        TRACE((TC_UI, TT_API1, "(3) arg(%d)='%s'\n", i, s));

	if (*s == '-')
	{
	    switch (tolower(*(s+1)))
	    {
	    case 'd':	    /* output post mortem on abort? */
		cli_dopostmortem = TRUE;
		break;

	    case 'l':	    /* log file */
		cli_logfile = strdup(argv[++i]);
		break;

	    case 'r':	    /* remote debugging */
		cli_remote_debug = TRUE;
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

    TRACE((TC_UI, TT_API1, "(2) process_args result: file=\"%s\", dopostmortem=%d, iconbar=%d\n",
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

#ifdef DEBUG
    log_init();
#endif
    
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

    err_fatal(event_register_toolbox_handler(-1, ProgInfo_AboutToBeShown, proginfo_handler, NULL));
    err_fatal(event_register_toolbox_handler(-1, tbres_event_SHOWING_ICON_MENU, icon_menu_handler, NULL));

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

    time(&tp);
    TRACE((TC_UI, TT_API1, "(1) *** New " APP_NAME " " VERSION_STRING " started %s", ctime(&tp)));
#endif

    initialise(argc, argv);

    if (cli_filename && cli_filename[0] != '\0')
    {
	session_run(cli_filename);

	if (!cli_iconbar)
	    return EXIT_SUCCESS;
    }

    err_fatal(event_set_mask(event_mask));

    TRACE((TC_UI, TT_API1, "(1) Entering poll loop\n"));
    while (TRUE)
    {
	int event_code, t;
	WimpPollBlock poll_block;

	LOGERR(_swix(OS_ReadMonotonicTime, _OUT(0), &t));
//      event_poll_idle(&event_code, &poll_block, t + 100, NULL);
        event_poll(&event_code, &poll_block, NULL);

#ifdef DEBUG
	LogPoll();
#endif

    	if (event_code != 0)
    	    TRACE((TC_UI, TT_API1, "(7) Poll: %d\n", event_code));
    }

    return EXIT_SUCCESS;
}

/* eof main.c */
