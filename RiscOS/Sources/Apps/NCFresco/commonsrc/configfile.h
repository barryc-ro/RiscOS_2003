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
    config_INT_LIST,
    config_COLOUR_LIST,
    config_STRING_LIST,

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

#define config_filter_phase_START	0
#define config_filter_phase_STOP	1
#define config_filter_phase_ADD		2
#define config_filter_phase_START_WRITE	3
#define config_filter_phase_STOP_WRITE	4
#define config_filter_phase_WRITE	5

/* return TRUE to free any strings allocated to this item */
typedef BOOL (*config_filter_fn)(int phase, const char *name, const void *value);

extern void config_set_filter(config_filter_fn fn);
