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
#ifndef configure_multicaster_h_included
#define configure_multicaster_h_included

extern void configure_init(void);
extern void configure_reread_configuration_file(void);
extern void configure_create_permanent_file_cache(void);

extern struct in_addr configure_read_specific_interface(multicast_file *);
extern struct in_addr configure_read_target_address(const char *, struct in_addr *);
extern struct in_addr configure_read_file_address(multicast_file *, struct in_addr *);
extern void configure_set_target_address(const char * /*file*/, const char * /*ip*/);

extern int configure_read_multiple_multicaster_threshold(void);
extern struct in_addr configure_read_default_interface(void);
extern u_short configure_read_default_port(void);
extern u_char configure_read_default_ttl(void);
extern u_char configure_read_specific_ttl(multicast_file *);
extern int configure_support_unicast_clients(void);
extern int configure_validate_client_allowed(const char *, struct in_addr *);

extern size_t configure_clamp_mtu(size_t /*suggested*/, size_t /*i/f MTU - IP hdrs*/);

extern const char *configure_read_root_directory(void);

extern unsigned long configure_read_minimum_master_rate(void);
extern unsigned long configure_read_specific_master_rate(multicast_file *);
extern int configure_read_specific_enable(const char *);

extern file_list *configure_read_file_list(void);

#endif
