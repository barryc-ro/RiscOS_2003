/* SSLeay/robits/robits.h */
/* Operating system specific bits - RISC OS in this case */

#ifndef included_robits_h
#define included_robits_h

/***************************************************************************/

/* dumbfs filing system name translation layer */

#define DUMBFS_MAXLINE          256
#define DUMBFS_DBNAME           "DumbBase"

extern FILE *dumbfs_fopen(char *fname, char *mode);
extern int dumbfs_stat(char *name, struct stat *st);

extern int riscos_FileExists( char *fname );
extern int riscos_DirExists( char *fname );
extern int riscos_ReadTTY( char *buf, int bufsize );

extern void riscos_ZM_Initialise( void );
extern void *riscos_ZM_realloc( void *ptr, size_t newsize );

/***************************************************************************/

/* General characteristics */

#define DIR_SEP                 "."
#define PATH_SEP                ","
#define EXT_SEP                 "/"
#define CUR_DIR                 "@"
#define PAR_DIR                 "^"

#define LIST_SEPERATOR_CHAR     ','
/* note misspelling as per SSL sources */

#define getch()                 _kernel_osrdch()
#define stat(n,s)               dumbfs_stat(n,s)
#define fileno(fp)              ((fp)->__file)

#ifndef errno
extern int errno;
#endif

/*
 * lseek & access args
 *
 * SEEK_* have to track L_* in sys/file.h & SEEK_* in 5include/stdio.h
 * ?_OK have to track ?_OK in sys/file.h
 */
#ifndef SEEK_SET
#define SEEK_SET        0              /* Set file pointer to "offset"     */
#define SEEK_CUR        1      /* Set file pointer to current plus "offset"*/
#define SEEK_END        2          /* Set file pointer to EOF plus "offset"*/
#endif

#define F_OK            0              /* does file exist                  */
#define X_OK            1              /* is it executable by caller       */
#define W_OK            2              /* is it writable by caller         */
#define R_OK            4              /* is it readable by caller         */

#endif                                 /* included_robits_h                */

/* eof */
