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
#ifndef statuscode_multicaster_h_included
#define statuscode_multicaster_h_included

typedef enum {
        bmc_OK,
        bmc_SYSCALL,
        bmc_SYNTAX,
        bmc_MALLOC_FAILED,
        bmc_NOT_MULTICAST,
        bmc_FILENAME_TOO_LONG,
        bmc_FILE_NOT_FOUND,
        bmc_FILE_NOT_OPENED,
        bmc_FILE_NOT_READ,
        bmc_FILE_TOO_LONG,
        bmc_INTERNAL_BAD_PARAM,
        bmc_FATAL_INTERNAL,
        bmc_ALREADY,
        bmc_DATAGRAM_TRUNCATED
} bmc_status;

#endif
