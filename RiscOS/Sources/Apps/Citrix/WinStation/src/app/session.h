/* > session.h

 *

 */


typedef struct session_ *Session;

extern Session session_open(const char *ica_file);
extern void session_poll(Session sess);
extern void session_close(Session sess);
extern void session_close_all(void);
extern void session_run(const char *ica_file);


/* eof session.h */
