/* > plugiunfile.h
 *
 */

#define plugin_info_flag_DISABLED	0x01

typedef struct
{
    int flags;
    int file_type;
} plugin_info;

extern plugin_info *plugin_list_get_info(int file_type);
extern void plugin_list_read_file(void);
extern void plugin_list_write_file(void);
extern void plugin_list_add(const plugin_info *info);
extern void plugin_list_toggle_flags(int file_type, int bic_flags, int eor_flags);
extern void plugin_list_dispose(void);

/* eof pluginfile.h */
