/* -*-c-*- */

os_error *awp_print_document(be_doc doc, int scale, int flags, int copies);

#define awp_print_NO_PICS	(1 << 0)
#define awp_print_NO_BACK	(1 << 1)
#define awp_print_NO_COLOUR	(1 << 2)
#define awp_print_UNDERLINE	(1 << 3)
#define awp_print_SIDEWAYS	(1 << 4)
#define awp_print_COLLATED	(1 << 5)
#define awp_print_REVERSED	(1 << 6)

#define PRINT_MAX_COPIES	99
#define PRINT_MAX_SCALE		999
#define PRINT_MIN_SCALE		10

void awp_free_pages(be_doc doc);

#ifdef STBWEB
#define printer_command_CENTRE_HEAD	0
#define printer_command_RETURN_HEAD	1
#define printer_command_CLEAN_HEAD	2
extern os_error *awp_command(int cmd_no);
#endif
