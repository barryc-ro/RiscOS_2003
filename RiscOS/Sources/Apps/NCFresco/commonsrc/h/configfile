/* -*-C-*- */

/* configfile.h */

/* Used in config.c but not in opther parts that need config information */

typedef enum {
    config_BOOL,
    config_INT,
    config_STRING,
    config_COLOUR,
    config_FILE,
    config_URL,
    config_FONT,

    config_COMMENT,
    config_LAST
    } config_type;

typedef struct {
    config_type type;		/* Type of data to look for */
    char *name;			/* Name of config item */
    void *ptr;			/* Pointer to variable to store value in */
    char *comment;		/* Comment to emit on the line before when config is saved */
    void *def;			/* Default value */
} config_item;

typedef struct {
    int dbnum;			/* dbox number */
    int dbicon;			/* dbox icon */
    int flags;			/* Flags */
} config_item_db;

#define CIDB_FLAG_INVERTED	(1 << 0)	/* The boolean item is inverted */

extern config_item citems[];

extern int config_has_been_changed;
