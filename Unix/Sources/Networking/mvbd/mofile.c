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

enum {
        MPEG2TS_IdealBlock = 188,
        MaxBlocksForEtherMTU = 7
};

#include "multicaster.h"

struct multicast_file {
        multicast_file *next;

        /* Usage counter */
        int usage_count;
        /* A file handle for this file */
        int f;

        /* The details of the file */
        char *filename;
        size_t total_length;

        /* Buffer which holds the file's contents */
        char *buffer;
        size_t bufsize;
};

static multicast_file *mf_list = NULL;

/* Return the length of a file. For a text file, this may return
 * >= the length that can be read from fread().
 */
static bmc_status multicast_file_length(multicast_file *mf)
{
        return platform_file_length(mf->f, &mf->total_length);
}

static multicast_file *multicast_file_create(const char *filename, bmc_status *status)
{
        multicast_file *mf;

        platform_debug((LOG_DEBUG, "Loading new file `%s'\n", filename));

        mf = malloc(sizeof(*mf));

        if (mf != NULL) {
                mf->f = -1;
                mf->buffer = MAP_FAILED;
                mf->usage_count = 1;
                mf->filename = Strdup(filename);
                if (mf->filename != NULL) {
                        mf->f = open(filename, O_RDONLY);
                        if (mf->f >= 0) {
                                bmc_status mfl_status = multicast_file_length(mf);
                                if (mfl_status != bmc_OK) {
                                        *status = mfl_status;
                                }
                                mf->bufsize = mf->total_length;
                                mf->buffer = mmap((caddr_t) 0, mf->total_length,
                                        PROT_READ, MAP_PRIVATE, mf->f, 0);
                                if (mf->buffer == MAP_FAILED) {
                                        close(mf->f);
                                        free(mf->filename);
                                        free(mf);
                                        *status = bmc_SYSCALL;
                                        return NULL;
                                }
                                return mf;
                        }
                        else {
                                platform_log(LOG_ERR, "Unable to load %s\n", filename);
                                *status = bmc_FILE_NOT_FOUND;
                        }
                        free(mf->filename);
                }
                free(mf);
        }

        return NULL;
}

void multicast_file_destroy(multicast_file *mf)
{
        if (--mf->usage_count <= 0) {
                multicast_file **ptr = &mf_list;
                for (; *ptr != NULL; ptr = &((*ptr)->next)) {
                        if (*ptr == mf) {
                                *ptr = mf->next;
                                break;
                        }
                }
                if (mf->filename != NULL) {
                        free(mf->filename);
                }
                if (mf->buffer != MAP_FAILED) {
                        munmap(mf->buffer, mf->total_length);
                }
                if (mf->f >= 0) {
                        close(mf->f);
                }
        }
}

u_long multicast_file_get_size(multicast_file *mf)
{
        return mf->total_length;
}

const char *multicast_file_get_filename(multicast_file *mf)
{
        return mf->filename;
}

/* Strategy independent method of sending a file */
size_t multicast_file_read(void **buffer, size_t from, size_t n, multicast_file *mf)
{
        if (n == 0 || from >= mf->total_length) {
                *buffer = 0;
                return 0;
        }
        else {
                size_t to_copy = n;
                if (from + n > mf->total_length) {
                        to_copy = mf->total_length - from;
                }
                if (to_copy > 0) {
                        *buffer = mf->buffer + from;
                }
                return to_copy;
        }
}

static void multicast_file_atexit(void)
{
        while (mf_list) {
                multicast_file_destroy(mf_list);
        }
}

void multicast_file_initialise(void)
{
        mf_list = NULL;
        atexit(multicast_file_atexit);
        configure_create_permanent_file_cache();
}

static void multicast_file_process(file_list *fl, multicast_file *mf)
{
        multicaster_object *mo;
        struct in_addr ip;

        ip = configure_read_specific_interface(mf);
        mo = multicaster_new(MaxBlocksForEtherMTU * MPEG2TS_IdealBlock);
        if (mo == NULL) {
                platform_log(LOG_ERR, "Unable to create multicaster\n");
                return;
        }

        if (bmc_OK == multicaster_object_ctor(mo, mf, &ip)) {
                multicaster_run(mo);
        }

        multicaster_object_dtor(mo);
}

static bmc_status multicast_file_load_and_run(file_list *fl)
{
        bmc_status s;
        multicast_file *mf;

        mf = multicast_file_create(fl->filename, &s);
        if (mf != NULL) {
                multicast_file_process(fl, mf);
                return bmc_OK;
        }

        return s;
}

bmc_status multicast_file_launch(void)
{
        file_list *fl;
        bmc_status s;
        pid_t pid;
        int pidcount = 0;

        for (fl = configure_read_file_list(); fl; fl = fl->next) {
                if (!configure_read_specific_enable(fl->filename)) {
                        platform_log(LOG_INFO, "%s disabled by configuration",
                                fl->filename);
                        continue;
                }
                pid = fork();
                switch (pid) {
                        case -1:
                                platform_log(LOG_ERR, "Unable to fork()\n");
                                return;
                        case 0:
                                signal(SIGHUP, SIG_IGN);
                                platform_reopen_log();
                                s = multicast_file_load_and_run(fl);
                                if (s != bmc_OK) {
                                        exit(EXIT_FAILURE);
                                }
                                else {
                                        exit(EXIT_SUCCESS);
                                }
                        default:
                                /* I'm the parent - keep going */
                                ++pidcount;
                                usleep(rand() % 1000000);
                                break;
                }
        }

        while (pidcount-- > 0) {
                wait((void *) &s);
        }

        return bmc_OK;
}
