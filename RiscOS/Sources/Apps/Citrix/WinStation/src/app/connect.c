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

#include "utils.h"

#include "session.h"
#include "sessionp.h"

#define tbres_WIN_CONNECT	"connectW"

#define I_MESSAGE		1

void connect_open(Session sess)
{
    if (!sess->connect_d)
	toolbox_create_object(0, tbres_WIN_CONNECT, &sess->connect_d);

    if (sess->connect_d)
    {
	toolbox_show_object(0, sess->connect_d, Toolbox_ShowObject_Default, NULL, NULL_ObjectId, NULL_ComponentId);
	button_set_value(0, sess->connect_d, I_MESSAGE, "");
	window_set_title(0, sess->connect_d, sess->gszServerLabel);
    }
}

void connect_status(Session sess, int state)
{
    char buf[12];
    sprintf(buf, "msg%d", state);
    button_set_value(0, sess->connect_d, I_MESSAGE, utils_msgs_lookup(buf));
}

void connect_close(Session sess)
{
    if (sess->connect_d)
    {
	toolbox_delete_object(0, sess->connect_d);
	sess->connect_d = NULL;
    }
}

/* eof connect.c */
