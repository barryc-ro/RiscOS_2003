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
#ifndef moobject_multicaster_h_included
#define moobject_multicaster_h_included

/* Constructors and destructor */
extern multicaster_object *multicaster_new(size_t);
extern bmc_status multicaster_object_ctor(
        multicaster_object *, multicast_file *, struct in_addr * /*local interface*/);
extern void multicaster_object_dtor(multicaster_object *);

/* Accessor functions */
extern struct sockaddr_in *multicaster_get_target(multicaster_object *);
extern multicast_file *multicaster_get_file(multicaster_object *);
extern multicaster_object **multicaster_get_next(multicaster_object *);
extern size_t multicaster_get_buffer_size(multicaster_object *);
extern size_t multicaster_get_total_size(multicaster_object *);
extern multicast_socket multicaster_get_socket(multicaster_object *);

/* Enable tftpserver.c to discover the correct value for the master client flag in its OACKs */
extern int multicaster_is_this_the_master_client(multicaster_object *, struct sockaddr_in *);

/* Called by the momanager to add a new client to an existing multicaster */
extern bmc_status multicaster_add_client(multicaster_object *, struct sockaddr_in *, int/*unicast*/);

/* Called back by the platform dependent code to fill in the I/O scatter/gather array for
 * a writev/sendmsg call.  Returns the number of entries in the copied array, which may
 * be 1..MSG_MAXIOVLEN.
 */
extern int multicaster_set_iovec(multicaster_object *, struct iovec *);

/* Returns non-zero if the multicaster is multicasting on the specified local interface */
extern int multicaster_check_interface(multicaster_object *, struct in_addr *);

/* This function is called when this multicaster's socket was flagged as read pending
 * in the call to select().  It is required to read the pending packet.
 */
extern void multicaster_process_packet(multicaster_object *);

/* This function is called when this multicaster's socket was NOT flagged as read pending
 * in the call to select().  It is required to check any pending timeouts that it may be
 * holding.
 */
extern void multicaster_check_timeout(multicaster_object *, platform_time);
/* Set the timeouts */
extern void multicaster_determine_select_timeout(multicaster_object *, struct timeval *);

/* These functions are called back from the tftpserver's TFTP packet parsing code
 * (which will have been invoked by multicaster_process_packet)
 */
extern void multicaster_investigate_error(multicaster_object *, int /*error*/, struct sockaddr_in *);
extern void multicaster_process_ack(multicaster_object *, int /*block*/);

extern void multicaster_send_part(multicaster_object *);
extern void multicaster_run(multicaster_object *);

#endif
