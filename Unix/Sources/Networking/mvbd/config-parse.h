/*
 *
 *  Copyright (c) 2000 by Pace Micro Technology plc. All Rights Reserved.
 *
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Pace Micro Technology
 * plc.
 *
 *
 *                PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 */
#ifndef configure_parser_interface_h_included
#define configure_parser_interface_h_included

typedef struct cidr_list {
        struct cidr_list *next;
        struct in_addr ip;
        int bits;
} cidr_list;

typedef struct file_list {
        struct file_list *next;
        int flags;
        char *filename;
        int ttl;
        unsigned long rate;
        cidr_list *addresses;
        cidr_list *interface;
} file_list;

typedef enum {
        fl_MASTER       = 0,
        fl_TTL          = 1,
        fl_DESTS        = 2,
        fl_RATE         = 4,
        fl_INTERFACE    = 8,
        fl_ENABLED      = 16,
        fl_DISABLED     = 32
} file_list_flags;

extern void *parse_alloc(size_t);
extern void parse_free(void *);
extern cidr_list *cidr_0alloc(void);
extern cidr_list *cidr_alloc(struct in_addr, int);
extern void cidr_free(cidr_list *);
extern file_list *filelist_alloc(char *filename);
extern void filelist_free(file_list *);

extern void config_parse_set_ttl(int);
extern void config_parse_default_address(cidr_list *);
extern void config_parse_add_file_details(file_list *);
extern void config_parse_process_scope(file_list *, file_list *);

extern void config_parse_set_transfer_rate(int);
extern void config_parse_set_home_directory(const char *);
extern void config_parse_set_interface(cidr_list *);
extern void config_parse_set_enable(int);

#endif
