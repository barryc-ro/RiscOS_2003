/* -*-c-*- */

/* Resolve a net location into an IP address and a port number */
/* The values are returned in NETWORK byte order */

os_error *netloc_resolve(char *location, int def_port, int *status, void *addr);
