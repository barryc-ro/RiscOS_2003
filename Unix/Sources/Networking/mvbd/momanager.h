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
#ifndef momanager_multicaster_h_included
#define momanager_multicaster_h_included

/* This structure holds details of the multicasting sessions.  It is an opaque
 * structure, defined in moobject.c only.
 */
typedef struct multicaster_object multicaster_object;

extern void mo_manager_initialise(void);

extern void mo_manager_delink(multicaster_object *);
extern void mo_manager_destroy(multicaster_object *);

extern void mo_manager_fd_set(fd_set *, struct timeval *delay);
extern int  mo_manager_fd_isset(fd_set *);

extern multicaster_object *mo_manager_new_client(const char * /*filename*/,
        tftp_mode, size_t /*bufsize*/, struct sockaddr_in *,
        struct in_addr * /* where sender sent packet */,
        int /*unicast*/,
        bmc_status *);

#endif
