/* > session.h

 *

 */

#ifndef __icaclient_h
#include "icaclient.h"
#endif

extern icaclient_session session_open_url(const char *url, const char *bfile);
extern icaclient_session session_open(const char *ica_file, int delete_after);
extern icaclient_session session_open_server(const char *host);
extern icaclient_session session_open_appsrv(const char *description);
extern int session_poll(icaclient_session sess);
extern void session_close(icaclient_session sess);
extern void session_run(const char *file, int file_is_url, const char *bfile);
extern void session_resume(icaclient_session sess);

extern void main_close_session(icaclient_session sess);

/* eof session.h */
