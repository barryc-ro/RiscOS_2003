/* > session.h

 *

 */


typedef struct session_ *Session;

extern Session session_open_url(const char *url);
extern Session session_open(const char *ica_file);
extern Session session_open_server(const char *host);
extern int session_poll(Session sess);
extern void session_close(Session sess);
extern void session_run(const char *file, int file_is_url);
extern void session_resume(Session sess);

/* eof session.h */
