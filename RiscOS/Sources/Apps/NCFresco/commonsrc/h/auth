/* -*-c-*- */

/* auth.h */

typedef enum {
    auth_type_NONE,		/* Just a dummy */
    auth_type_BASIC,
    auth_type_PUBKEY
    } auth_type;

typedef enum {
    auth_lookup_FAIL,
    auth_lookup_NEED_DATA,
    auth_lookup_SUCCESS
    } auth_lookup_result;

typedef enum {
    auth_passwd_NONE,
    auth_passwd_PLAIN,
    auth_passwd_UUCODE
    } auth_passwd_store;

typedef enum
{
    auth_req_NONE,
    auth_req_WWW,
    auth_req_PROXY
} auth_requester;

typedef struct auth_realm *realm;

extern void auth_init(void);
extern realm auth_add_realm(char *realmp, char *type, char *user, char *passwd);
extern void auth_add(char *url, realm r);
extern realm auth_lookup_realm(char *realmp);
extern auth_lookup_result auth_lookup(char *url, auth_type *type, char **user, char **passwd);
extern char *auth_lookup_string(char *url);
extern int auth_remove(char *url);
os_error *auth_write_realms(char *fname, auth_passwd_store pws);
os_error *auth_load_file(char *fname);

extern int auth_check_allow_deny(char *site);
extern int auth_supported(char *type);

extern void auth_dispose(void);
