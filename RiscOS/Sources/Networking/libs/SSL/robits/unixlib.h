/* -*-C-*-
 *
 * $Header$
 * $Source$
 *
 * Copyright (c) 1995 Acorn Computers Ltd., Cambridge, England
 *
 * $Log$
 * Revision 1.2  95/05/02  11:11:46  kwelton
 * Third argument to ioctl() is better described as a void *, rather
 * than a char *.
 *
 * Revision 1.1  95/04/20  09:50:32  kwelton
 * Initial revision
 *
 */
/*
 * variable declarations
 */
extern char *sys_errlist[];
extern int sys_nerr;

/*
 * function prototypes
 */
extern int access(char *path, int mode);

extern int bcmp(char *s1, char *s2, int length);
extern void bcopy(char *src, char *dst, int length);
extern char *bzero(char *s, int length);

extern int chdir(char *dir);
extern int chmod(char *path, int mode);
extern int close(int s);

extern void endpwent(void);

extern int filestat(char *fname, char *type);
extern void flushinput(void);
extern int fstat(int s, char *buf);

extern int getdtablesize(void);
extern int getegid(void);
extern int geteuid(void);
extern int getgroups(int ngrps, int *grparray);
extern int gethostname(char *name, int max_namelen);
extern char *getlogin(void);
extern char *getpass(char *prompt);
extern int getpid(void);
extern struct passwd *getpwent(void);
extern struct passwd *getpwuid(int uid);
extern int gettimeofday(struct timeval *tv, struct timezone *tzp);
extern int getuid(void);
extern char *getvarhostname(void);
extern char *getvarusername(void);
extern char *getwd(char *buf);

extern void herror(char *c);

extern char *index(char *s, char c);
extern int ioctl(int s, int cmd, void *data);

extern void killfile(char *name);

extern long lseek(int d, long offset, int whence);

extern int osreadc(void);

extern int read(int s, char *buf, int nbytes);
extern int readdir(char *path, char *buf, int len, int name, int offset);
extern int readv(int s, struct iovec *iov, int iovcnt);
extern char *rindex(char *s, char c);

extern void setpwent(void);
extern int strcasecmp(char *a, char *b);
extern int strncasecmp(char *a, char *b, int n);

extern int unlink(char *path);

extern int write(int s, char *buf, int nbytes);
extern int writev(int s, struct iovec *iov, int iovcnt);

extern char *xgets(char *buf);
extern void xperror(const char *s);
extern char xputchar(char ch);

/* EOF unixlib.h */
