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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/uio.h"

#include "multicaster.h"
#include "platform.h"
#include "mofile.h"


static char id[] = "video_multiblaster[            ]";
static int log_options = 0;

static bmc_status main_run(void)
{
        bmc_status status = bmc_OK;

        multicast_file_launch();

        return status;
}

static void escape_handler(int sig)
{
        platform_log(LOG_CRIT, "received signal (type=%u) and is exiting\n", sig);
        signal(sig, SIG_DFL);
        exit(EXIT_FAILURE);
}

#ifdef SIGHUP
static void sighup_handler(int sig)
{
        signal(sig, SIG_IGN);
        configure_reread_configuration_file();
        signal(sig, sighup_handler);
}
#endif

static bmc_status main_initialise(int argc, char **argv)
{
        bmc_status s;

        /* The order of these is important to ensure atexit fns are called in
         * the right order
         */
        /* Start with the low-level stuff */
        platform_init();
        configure_init();
        platform_init_post_config();

        /* Now the high-level abstract stuff */
        multicast_file_initialise();

        /* Tidy up on close down */
        signal(SIGINT, escape_handler);
#ifdef SIGHUP
        signal(SIGHUP, sighup_handler);
#endif
        while (argc-- > 1) {
                const char *arg = *++argv;
                if (*arg++ != '-') break;
                while (*arg) {
                        switch (*arg++) {
                                case 'c':
                                        raise(SIGUSR1); /* dump config */
                                        break;
                                case 'v':
                                        log_options = LOG_PERROR;
                                        platform_reopen_log();
                                        break;
                                case 'x':
                                        exit(EXIT_SUCCESS);
                                default:
                                        break;
                        }
                }

        }

        return main_run();
}

const char *main_get_application_id(void)
{
        pid_t pid = getpid();
        sprintf(strchr(id, '[') + 1, "%d]", pid);
        return id;
}

int main_get_log_options(void)
{
        return log_options;
}

int main(int argc, char *argv[])
{
        bmc_status status = main_initialise(argc, argv);

        if (status != bmc_OK) {
                status = platform_report_error(id, status);
        }

        return status == bmc_OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
