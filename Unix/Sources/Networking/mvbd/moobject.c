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
#include <stddef.h>
#include <math.h>
#include <signal.h>

#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/uio.h"

#include "multicaster.h"


/* This data structure holds all the information necessary to track the status
 * of the multicaster object.
 */
typedef struct tftp_client {
        struct sockaddr_in      client;
        int                     this_block;
        int                     current_part_number;
        unsigned long           packet_count;
} tftp_client;

/* Multicaster object structure details are private to this source file.
 * Do not change this - add accessor/mutator functions to the header file
 * and implement them here.
 */

struct multicaster_object {
        multicaster_object      *next;

        /* The multicast destination address + parameter for msghdr.
         */
        struct sockaddr_in      target;

        /* The source address for this object */
        struct sockaddr_in      source;

        /* File handle of the object being multicasted */
        multicast_file          *f;

        /* Socket handle */
        multicast_socket        s;

        /* The payload data buffer descriptor */
        int                     iov_size;
        struct iovec            iov[2];

        /* The buffer for holding the file data (appears after this object in memory) */
        size_t                  size_of_buffer;
        char                    *buffer;

        /* Data sent tracking information */
        int                     max_part_number;

        /* Tracking information */
        tftp_client             clients[1];
};


multicast_socket multicaster_get_socket(multicaster_object *mo)
{
        return mo->s;
}

multicaster_object **multicaster_get_next(multicaster_object *mo)
{
        return &mo->next;
}

multicast_file *multicaster_get_file(multicaster_object *mo)
{
        return mo->f;
}

size_t multicaster_get_buffer_size(multicaster_object *mo)
{
        return mo->size_of_buffer;
}

size_t multicaster_get_total_size(multicaster_object *mo)
{
        return (size_t) multicast_file_get_size(mo->f);
}

struct sockaddr_in *multicaster_get_target(multicaster_object *mo)
{
        return &mo->target;
}

int multicaster_set_iovec(multicaster_object *mo, struct iovec *iov)
{
        (void) memcpy(iov, mo->iov, sizeof(mo->iov));
        return mo->iov_size;
}

static void multicaster_write_iovec(multicaster_object *mo,
        void *header, int header_len, void *data, int data_len)
{
        mo->iov[0].iov_base = header;
        mo->iov[0].iov_len = header_len;
        mo->iov[1].iov_base = data;
        mo->iov[1].iov_len = data_len;
        if (data_len == 0) {
                mo->iov_size = 1;
        }
        else {
                mo->iov_size = 2;
        }
}

bmc_status multicaster_object_ctor(multicaster_object *mo, multicast_file *mf,
                struct in_addr *interface)
{
        bmc_status status;

        /* Initialise various members to safe values */
        mo->s = multicaster_INVALID_SOCKET;

        /* The file handle */
        mo->f = mf;

        /* Set up the source address for the packets */
        platform_init_sockaddr_in(&mo->source, *interface, 0);

        /* Calculate and cache the destination address for multicast packets */
        platform_init_sockaddr_in(&mo->target,
                configure_read_file_address(mo->f, interface),
                htons(configure_read_default_port()));

        /* Ensure that the target IP address is a multicast address */
        if (!IN_MULTICAST(htonl(mo->target.sin_addr.s_addr))) {
                return bmc_NOT_MULTICAST;
        }

        mo->max_part_number = 1 + (size_t) multicast_file_get_size(mo->f) / mo->size_of_buffer;

        /* Set up the destination socket address for sending */
        status = platform_create_socket(&mo->s, configure_read_specific_ttl(mo->f));
        if (status == bmc_OK) {
                /* Create a binding to the interface */
                status = platform_bind_socket(mo->s, interface->s_addr, 0);
                if (status == bmc_OK) {
                        /* Find out what the source port is for future reference */
                        status = platform_get_local_port(mo->s, &mo->source.sin_port);
                        if (status == bmc_OK) {
                                /* Now set the default route for packets to be down the right interface */
                                status = platform_set_socket_option(mo->s, IPPROTO_IP,
                                        IP_MULTICAST_IF, interface, sizeof(*interface));
                        }
                }
        }
        if (status != bmc_OK) {
                return status;
        }

        return bmc_OK;
}

static void multicaster_init_client(multicaster_object *mo, tftp_client *c)
{
        memset(c, '\0', sizeof(*c));
        c->this_block = 0;
}

multicaster_object *multicaster_new(size_t bufsize)
{
        multicaster_object *mo = calloc(1, sizeof(*mo) + bufsize);

        if (mo != NULL) {
                /* Initialise everything so that dtor can be called safely */
                mo->f = NULL;
                mo->s = multicaster_INVALID_SOCKET;
                mo->size_of_buffer = bufsize;
                mo->buffer = (char *) (mo + 1);
                multicaster_init_client(mo, &mo->clients[0]);
        }

        return mo;
}

void multicaster_object_dtor(multicaster_object *mo)
{
        if (mo->f != NULL) {
                /* If we want the file objects to persist, we can implement that here */
                multicast_file_destroy(mo->f);
        }
        if (mo->s != multicaster_INVALID_SOCKET) {
                platform_close_socket(mo->s);
                mo->s = multicaster_INVALID_SOCKET;
        }
        free(mo);
}

static int multicaster_read_next_part(multicaster_object *mo, int last_acked_part_number,
                tftp_client *client)
{
        size_t offset;
        int data_size;
        void *buffer;

        if (last_acked_part_number >= mo->max_part_number) {
                /* The loop was finished - return to start */
                last_acked_part_number = 0;
        }

        client->this_block = client->current_part_number = last_acked_part_number + 1;

        offset = (client->current_part_number - 1) * mo->size_of_buffer;
        data_size = multicast_file_read(&buffer, offset, mo->size_of_buffer, mo->f),
        multicaster_write_iovec(mo, buffer, data_size, 0, 0);

        /* Next block has been cued up ready for transmission ... indicate by returning 1 */
        return 1;
}


void multicaster_send_part(multicaster_object *mo)
{
        if (multicaster_read_next_part(mo, mo->clients[0].this_block, &mo->clients[0])) {
                bmc_status status;

                status = platform_transmit(mo);

                if (status != bmc_OK) {
                        platform_report_error("Unable to transmit data", status);
                }
        }
}

static void multicaster_get_time(struct timeval *tv)
{
        if (gettimeofday(tv, NULL) < 0) {
                platform_log(LOG_ERR, "Unable to read time of day\n");
                exit(0);
        }
}

void multicaster_run(multicaster_object *mo)
{
        int loop;
        struct timeval now, started_block;
        int packet_count, packets_per_loop;
        int rate_bps = configure_read_specific_master_rate(mo->f); /* rate in bits per second */
        unsigned long desired_delay, current_delay;
        static double total_bit_deviation = 0;
        static int last_periodic_deviation_check;

        const int bits_per_packet = (mo->size_of_buffer) * 8.0;
        /* number of packets required per second */
        const double packets_per_second = (double) rate_bps / (double) bits_per_packet;
        /* number of packets per delay period */
        packets_per_loop = 1;

        platform_debug((LOG_DEBUG, "\nMulticaster for %s to %s",
                multicast_file_get_filename(mo->f),
                inet_ntoa(mo->target.sin_addr)));
        platform_log(LOG_INFO, "In order to maintain %dKb/sec with a %d byte buffer, we need %f packets/sec",
                rate_bps >> 10, mo->size_of_buffer, packets_per_second);

        if (packets_per_second < 1) {
                platform_log(LOG_ERR, "Must transmit at least 1 packet per second\n");
                return;
        }

        desired_delay = ((double) packets_per_loop * 1000000.0 / packets_per_second);

        /* Initialise the current variables so everything looks rosy */
        current_delay = desired_delay;
        packet_count = 0;

        multicaster_get_time(&started_block);
        last_periodic_deviation_check = started_block.tv_sec;

        platform_log(LOG_INFO, "Initial delay period: %lu; ppl: %d\n", current_delay, packets_per_loop);

        while (1)
        {
            unsigned int sample_bits_sent = 0;
            unsigned int sample_time_usec = 0;
            signed int   sample_bit_deviation   = 0;
            signed int   packet_correction_up   = 0;
            signed int   packet_correction_down = 0;
            signed int   adjust;
            double sample_bit_ratio, sample_bits_expected, old_delay, new_delay;

            do
            {
                unsigned int packets_this_loop, bits_sent, usec_elapse;
                double bits_expected;

                packets_this_loop = packets_per_loop;
                if (total_bit_deviation > ((double) bits_per_packet * 1.15))
                {
                    packets_this_loop--;
                    packet_correction_down--;
                }
                if (total_bit_deviation < (0.0 - ((double) bits_per_packet) * 1.15))
                {
                    packets_this_loop++;
                    packet_correction_up++;
                }

                for (loop = 0; loop < packets_this_loop; ++loop)
                {
                    multicaster_send_part(mo);
                    ++packet_count;
                    fflush(stdout);
                }
                usleep(current_delay);
                multicaster_get_time(&now);

                /* Calculate the number of microseconds elapsed sending this block */
                usec_elapse = ((now.tv_usec - started_block.tv_usec) +
                                1000000 * (now.tv_sec - started_block.tv_sec));

                /* Get number of bits transmitted - it'll be useful
                 */
                bits_sent = packet_count * bits_per_packet;

                /* The expected number of bits is the number of bits that we *should*
                 * have been able to transmit in the current sample period.
                 */
                bits_expected = (double) rate_bps * (((double) usec_elapse) / 1000000.0);

                total_bit_deviation += (bits_sent - bits_expected);
                sample_bit_deviation += (bits_sent - bits_expected);
                sample_bits_sent += bits_sent;
                sample_time_usec += usec_elapse;

                platform_log(LOG_DEBUG,
                    "beat; usecs %7u (%7u); sent: %6u (%.0f); total_dev: %.0f; packets:%2d ",
                    usec_elapse, current_delay, bits_sent, bits_expected, total_bit_deviation,
                    packets_this_loop);

                packet_count = 0;
                started_block = now;

            } while (now.tv_sec == last_periodic_deviation_check);
            /* *** End of inner transmission loop *** */

            /* We now alter packet delays to adjust the bitrate      */

            sample_bits_expected = ((double) rate_bps) * (double) sample_time_usec / 1000000.0;

            /* Divide bits we actually transmitted by bits we really wanted to, giving
             * the fraction of bits transmitted in this sample period.
             */
            sample_bit_ratio = sample_bits_sent / sample_bits_expected;

            /* Get hold of a double holding the current sleep period */
            old_delay = (double) current_delay;


            /* The division allows us to gradually tend towards the
             * correct speed gradually.
             */
            /*adjust = (old_delay * ratio - old_delay) / 8.0;*/
            /*adjust = (sample_bit_ratio - (1.0 / (sample_bit_ratio)));*/

            adjust    = total_bit_deviation;
            while (fabs(adjust) > old_delay)
            {
                adjust = adjust / 2;
            }
            new_delay = (old_delay + adjust / 10.0);

            /* new_delay = old_delay + (total_bit_deviation / 100.0);*/

            current_delay = (unsigned long) (new_delay + 0.5);

            platform_log(LOG_INFO,
                    "tick; usecs:%8u; bit dev:%7d (%6.4f); total_dev:%7.0f; "
                    "old_delay:%5.0f; new_delay:%5.0f (%6.3f%%); ppl:%2d (%3d) corrected:%4d:%4d",
                     sample_time_usec, sample_bit_deviation, sample_bit_ratio, total_bit_deviation,                    
                     old_delay, new_delay, ((new_delay / old_delay) * 100) - 100,
                     packets_per_loop, sample_bits_sent / bits_per_packet, packet_correction_down,
                     packet_correction_up);

            /*platform_debug((LOG_DEBUG,
                    "tick(%3d/%3d); usec is %lu; ratio is %f; "
                    "old_rate is %f; new_rate is %f; \n",
                    packet_count, (int) ((packet_count / usec_d)), usec_lap,
                    ratio, old_delay, new_delay));
            */
            /* Check to see if we are approaching the target.  If not, then the
             * likelihood is that the machine is not accurate enough to enable the
             * timings to work, so increase the number of packets we send per tick,
             * and reduce the number of ticks required per second accordingly.
             */

            /* Calculate the difference between the desired delay and the actual delay */
            /*delay_diff = new_delay - 1200;  was target_delay;*/
            /* if (delay_diff < 0 ) delay_diff = 0.0 - delay_diff;*/

            /*platform_log(LOG_INFO, "Delay difference = %f", delay_diff);*/

                    if (new_delay < 800 && packets_per_loop < 8) {
                           platform_debug((LOG_DEBUG, "  DIVERGE.  Increment packets/loop"));
                           ++packets_per_loop;
                           desired_delay = packets_per_loop * 1000000 / packets_per_second;
                           current_delay = desired_delay;
                    }
                    if (new_delay > 10000 && packets_per_loop > 1) {
                            --packets_per_loop;
                           desired_delay = packets_per_loop * 1000000 / packets_per_second;
                           current_delay = desired_delay;
                    }

            /* platform_log(LOG_INFO, "Desired delay= %d", desired_delay);*/

            /* For safety! */
            if (current_delay <= 0) {
                    current_delay = desired_delay;
            }

            last_periodic_deviation_check = now.tv_sec;
            sample_bits_sent = 0;
            sample_time_usec = 0;
            sample_bit_deviation = 0;
      }
}
