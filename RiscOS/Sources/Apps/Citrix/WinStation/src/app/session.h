/* > session.h

 *

 */

#ifndef __winframe_h
#include "winframe.h"
#endif

extern winframe_session session_open_url(const char *url);
extern winframe_session session_open(const char *ica_file);
extern winframe_session session_open_server(const char *host);
extern int session_poll(winframe_session sess);
extern void session_close(winframe_session sess);
extern void session_run(const char *file, int file_is_url);
extern void session_resume(winframe_session sess);

/* eof session.h */
