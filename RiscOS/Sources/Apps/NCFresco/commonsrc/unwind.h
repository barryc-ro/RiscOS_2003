/* -*-c-*- */

/* Return the name of the caller function or a dummy '<unknown>' is none is found */
extern char *procname(int pc);
/* Return the name of the nth function up the stack, or NULL if the stack does not go that far */
extern char *caller(int n);
