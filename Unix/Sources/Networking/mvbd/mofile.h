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
#ifndef mofile_multicaster_h_included
#define mofile_multicaster_h_included

/* This structure holds details of a file which is to be multicasted.
 * It can be shared between multiple multicaster objects and will look
 * after its own cacheing internally.
 */
typedef struct multicast_file multicast_file;

/* File transfer modes */
typedef enum {
        mode_OCTET,
        mode_NETASCII
} tftp_mode;

/* Constructor and destructor */
extern void multicast_file_destroy(multicast_file * /*mf*/);

/* Read data */
extern size_t multicast_file_read(void **, size_t/*from*/, size_t/*n*/, multicast_file *);

/* General functions */
extern u_long multicast_file_get_size(multicast_file *);
extern const char *multicast_file_get_filename(multicast_file *);

/* General module initialisation */
extern void multicast_file_initialise(void);

extern bmc_status multicast_file_launch(void);

#endif
