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

#include "VersionNum"

#ifdef LOG_PERROR
#  define VERBOSE_LOG_OPTIONS LOG_PERROR
#else
#  ifdef LOG_CONS
#    define VERBOSE_LOG_OPTIONS LOG_CONS
#  else
#    define VERBOSE_LOG_OPTIONS (0)
#  endif
#endif

const char *ident(void)
{
        static char ident[] = "$VersionNum: " Module_FullVersion " $";
        return ident;
}

static char id[] = "video_multiblaster[            ]";
static int log_options = 0;

enum command_line_options {
        cli_EXIT = 1,
        cli_VERBOSE = 2,
        cli_DUMP_CONF = 4
};

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

static void main_arg_syntax(void)
{
        platform_log(LOG_CRIT, "Missing argument to -f parameter\n");
        exit(EXIT_FAILURE);
}

/* This function returns the name of the configuration file if it has been
 * overridden on the command-line with a -f option, otherwise it returns
 * NULL to indicate that the built-in default location should be used.
 */
static const char *main_parse_args(int argc, char **argv, int *options)
{
        const char *filename = NULL;
        *options = 0;

        while (argc-- > 1) {
                const char *arg = *++argv;
                if (*arg++ != '-') break;
                while (*arg) {
                        switch (*arg++) {
                                case 'c':
                                        *options |= cli_DUMP_CONF;
                                        break;
                                case 'v':
                                        *options |= cli_VERBOSE;
                                        break;
                                case 'x':
                                        *options |= cli_EXIT;
                                        break;
                                case 'f':
                                        if (argc-- > 1) {
                                                filename = *++argv;
                                        }
                                        else {
                                                main_arg_syntax();
                                        }
                                        break;
                                default:
                                        break;
                        }
                }

        }

        return filename;
}

static bmc_status main_initialise(int argc, char **argv)
{
        /* The order of these is important to ensure atexit fns are called in
         * the right order
         */
        int options = 0;

        /* Start with the low-level stuff */
        platform_init();
        configure_init(main_parse_args(argc, argv, &options));
        platform_init_post_config();

        /* Now the high-level abstract stuff */
        multicast_file_initialise();

        /* Tidy up on close down */
        signal(SIGINT, escape_handler);
#ifdef SIGHUP
        signal(SIGHUP, sighup_handler);
#endif

        if (options & cli_DUMP_CONF) {
                raise(SIGUSR1);
        }

        if (options & cli_VERBOSE) {
                log_options = VERBOSE_LOG_OPTIONS;
                platform_reopen_log();
        }

        if (options & cli_EXIT) {
                exit(EXIT_SUCCESS);
        }

        return main_run();
}

const char *main_get_application_id(void)
{
        pid_t pid = getpid();
        sprintf(strchr(id, '[') + 1, "%ld]", (long) pid);
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
