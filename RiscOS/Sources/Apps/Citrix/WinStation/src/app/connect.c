/* > connect.c
 *
 *
 */


#include "windows.h"

#include <stdio.h>

#include "../inc/client.h"

#include "tboxlibs/toolbox.h"
#include "tboxlibs/window.h"
#include "tboxlibs/gadgets.h"

#include "tboxlibs/event.h"

#include "utils.h"

#include "session.h"
#include "sessionp.h"

#include "tbres.h"

#define I_MESSAGE		1
#define I_TITLE			3

static int cancel_connect_handler(int event_code, ToolboxEvent *event, IdBlock *id_block, void *handle)
{
    icaclient_session sess = handle;

    main_close_session(sess);

    return 1;
    NOT_USED(event_code);
    NOT_USED(event);
    NOT_USED(id_block);
    NOT_USED(handle);
}

int connect_open(icaclient_session sess)
{
    TRACE((TC_UI, TT_API1, "connect_open: %p d %p", sess, sess->connect_d));

    if (!sess->connect_d)
	LOGERR(toolbox_create_object(0, tbres_WIN_CONNECT, &sess->connect_d));

    if (sess->connect_d)
    {
	LOGERR(toolbox_show_object(0, sess->connect_d, Toolbox_ShowObject_Centre, NULL, NULL_ObjectId, NULL_ComponentId));
	LOGERR(button_set_value(0, sess->connect_d, I_MESSAGE, ""));

	/* purposefully ignore errors on these two as only one will work deepending on the box type */
	button_set_value(0, sess->connect_d, I_TITLE, (char *)sess->gszServerLabel);
	window_set_title(0, sess->connect_d, (char *)sess->gszServerLabel);

	LOGERR(event_register_toolbox_handler(sess->connect_d, tbres_event_CANCEL, cancel_connect_handler, sess));
    }

    return sess->connect_d != NULL;
}

void connect_status(icaclient_session sess, int state)
{
    char buf[12];

    sprintf(buf, "EIDS%d:", state);
    LOGERR(button_set_value(0, sess->connect_d, I_MESSAGE, utils_msgs_lookup(buf)));

    TRACE((TC_UI, TT_API1, "connect_status: %p d %p state %s (%d)", sess, sess->connect_d, utils_msgs_lookup(buf), state));
}

void connect_close(icaclient_session sess)
{
    TRACE((TC_UI, TT_API1, "connect_close: %p d %p", sess, sess->connect_d));

    if (sess->connect_d)
    {
	LOGERR(event_deregister_toolbox_handler(sess->connect_d, tbres_event_CANCEL, cancel_connect_handler, sess));

	LOGERR(toolbox_delete_object(0, sess->connect_d));
	sess->connect_d = NULL;
    }
}

/* eof connect.c */
