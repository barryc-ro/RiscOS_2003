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
#ifndef multicaster_h_included
#define multicaster_h_included

#ifdef DEBUGLIB
#  define platform_debug(args) platform_log args
#else
#  define platform_debug(args) ((void) 0)
#endif

#include "platform-specific.h"

#include "statuscode.h"
#include "mofile.h"
#include "momanager.h"
#include "platform.h"
#include "utilities.h"
#include "moobject.h"
#include "config-parse.h"
#include "configure.h"
#include "i18n.h"

extern const char *main_get_application_id(void);
extern int main_get_log_options(void);

#endif
