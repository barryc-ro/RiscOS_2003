/* > stbcodec.c

 * CODEC specified code for NCFresco

 */

#include "swis.h"

#include "debug.h"
#include "interface.h"
#include "plugin.h"

#include "fevents.h"
#include "stbfe.h"
#include "stbtb.h"
#include "stbview.h"

/* ------------------------------------------------------------------------------------------- */

#define RCAIRBlast_BlastToVCR	0x4F182

#define vcr_RECORD	1
#define vcr_PAUSE	2
#define vcr_STOP	3

#define BASE_FLAGS	0x03	/* blast to LEDs and port 0 */

/* ------------------------------------------------------------------------------------------- */

static void recording_start(void)
{
    os_error *e;
    e = (os_error *)_swix(RCAIRBlast_BlastToVCR, _INR(0,1), vcr_RECORD, BASE_FLAGS);
#if DEBUG
    if (e) STBDBG(("recording: start %x %s\n", e->errnum, e->errmess));
#endif

    if (!e)
	tb_status_button(fevent_CODEC_RECORD, tb_status_button_ACTIVE);
}

static void recording_pause(void)
{
    os_error *e;
    e = (os_error *)_swix(RCAIRBlast_BlastToVCR, _INR(0,1), vcr_PAUSE, BASE_FLAGS);
#if DEBUG
    if (e) STBDBG(("recording: pause %x %s\n", e->errnum, e->errmess));
#endif
}

static void recording_stop(void)
{
    os_error *e;
    e = (os_error *)_swix(RCAIRBlast_BlastToVCR, _INR(0,1), vcr_STOP, BASE_FLAGS);
#if DEBUG
    if (e) STBDBG(("recording: stop %x %s\n", e->errnum, e->errmess));
#endif
    tb_status_button(fevent_CODEC_RECORD, tb_status_button_INACTIVE);
}

/* ------------------------------------------------------------------------------------------- */

static int recording = FALSE;

static void send_action(fe_view v, int action)
{
    be_item item = v && v->displaying ? backend_read_highlight(v->displaying, NULL) : NULL;
    if (item == NULL)
	item = be_plugin_action_item_HELPERS;

    STBDBG(("codec_event: v%p item %p action %x\n", v, item, action));

    backend_plugin_action(v ? v->displaying : NULL, item, action);
}

void codec_event_handler(int event, fe_view v)
{
    switch (event)
    {
    case fevent_CODEC_STOP:
	send_action(v, plugin_state_STOP);
	if (recording)
	    recording_stop();
	break;

    case fevent_CODEC_PLAY:
	send_action(v, plugin_state_PLAY);
	break;

    case fevent_CODEC_PAUSE:
	send_action(v, plugin_state_PAUSE);
	if (recording)
	    recording_pause();
	break;

    case fevent_CODEC_REWIND:
	send_action(v, plugin_state_REWIND);
	break;

    case fevent_CODEC_FAST_FORWARD:
	send_action(v, plugin_state_FAST_FORWARD);
	break;

    case fevent_CODEC_RECORD:
	if (recording)
	    recording_stop();
	else
	    recording_start();
	break;

    case fevent_CODEC_MUTE:
	break;

    case fevent_CODEC_CLOSE:
	send_action(v, be_plugin_action_CLOSE);
	if (recording)
	    recording_stop();
	break;
    }
}

/* ------------------------------------------------------------------------------------------- */

/* eof stbcodec.c */

