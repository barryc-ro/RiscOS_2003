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
#include "multicaster.h"

static const char *errors[] = {
        "No error",
        "system call failed",
        "syntax: <Multicast IP address> <filename>",
        "malloc failed",
        "destination IP address must be a multicast address",
        "Filename too long",
        "File not found",
        "Unable to open file",
        "Unable to read file",
        "File too long",
        "Internal error - bad parameters",
        "Fatal internal error",
        "Already known",
        "Incoming datagram truncated",
};

const char *i18n_translate_status(bmc_status status)
{
        if (status < bmc_OK || status > bmc_DATAGRAM_TRUNCATED) {
                status = bmc_FATAL_INTERNAL;
        }

        return errors[status];
}
