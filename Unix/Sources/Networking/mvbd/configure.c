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
#include "config-parse.h"

/* This file manages the configuration of the videoblast server software.  It calls
 * platform_* functions to open and close configuration files.  This may be
 * useful if the platform-specific implementation needs to obtain data from
 * somewhere other than a file, as it can export the data to a file and then
 * return a handle to it to here and then discard the exported data when asked
 * to close the file.  Configuration data is cached in the configuration structure.
 */

typedef struct configuration {
        /* Multicasts are sent to this port number by default */
        u_short         default_port;
        /* Multicasts are sent to these cycled multicast addresses by default */
        cidr_list       *default_address;
        /* The destination interface default - only the first entry is valid */
        cidr_list       *default_interface;
        /* Multicasts are sent with this TTL by default */
        u_char          default_ttl;
        /* An absolute limit on the MTU of outgoing packets */
        int             mtu_ceiling;
        /* Rate in bits/sec that the object is to be delivered at */
        unsigned long   minimum_master_rate;
        /* The root for all served files.  UNIX implementations will attempt
         * to chroot to this directory.  OSes without this capability should
         * always verify files live under this subtree. */
        char *          root_directory;
        /* Whether the default state for a file is enabled or disabled */
        int             enabled;
        /* The details of specific files */
        file_list *     files;
} configuration;

static configuration conf;
static void config_dump(int);

static const char *default_root = "/";

void configure_establish_defaults(void)
{
        struct in_addr default_ia;

        inet_aton("239.192.0.2", &default_ia);
        conf.default_address = cidr_alloc(default_ia, 32);
        inet_aton("0.0.0.0", &default_ia);
        conf.default_interface = cidr_alloc(default_ia, 32);
        conf.default_port = 55555;
        conf.default_ttl = 5;
        conf.mtu_ceiling = 9212;
        conf.minimum_master_rate = 60 * 1024 * 8;
        conf.root_directory = Strdup(default_root);
        conf.enabled = 1;
        conf.files = NULL;
}

static void configure_parse(FILE *f)
{
        extern void lexer_exec(FILE *);
        lexer_exec(f);
}

void configure_reread_configuration_file(void)
{
        FILE *f = platform_configuration_open(NULL);

        if (f != NULL) {
                filelist_free(conf.files);
                conf.files = NULL;
                configure_parse(f);
                fclose(f);
        }
}

void configure_init(void)
{
        configure_establish_defaults();
        signal(SIGUSR1, config_dump);
        configure_reread_configuration_file();
}

void configure_create_permanent_file_cache(void)
{
        /* At this point, it is safe to call multicast_file_create.  This can be done
         * to pre-load various objects and ensure that they are never discarded (since
         * we create them and then forget about them, the reference count will never
         * decrement to zero
         */
}

static void *configure_cycle_list(void *headptr)
{
        typedef struct generic_list {
                struct generic_list *next;
        } generic_list;
        generic_list **head = headptr;
        void *const result = *head;

        if (result != NULL && (*head)->next != NULL) {
                *head = (*head)->next;
                head = &(*head)->next;
                while ((*head)->next) {
                        head = &(*head)->next;
                }
                (*head)->next = result;
        }

        return result;
}

static file_list *configure_find_file_details(const char *filename)
{
        file_list *fl = conf.files;

        if (!filename) return NULL;

        for (;fl; fl=fl->next) {
                if (fl->filename && !strcmp(fl->filename, filename)) return fl;
        }

        return fl;
}

file_list *configure_read_file_list(void)
{
        return conf.files;
}

/* This function increments a stored IP address until it would cause the CIDR block
 * to be exited.  If the range was exhausted, it is reset to the first available
 * address and non-zero returned to indicate wrap.  Zero is returned if the address
 * was updated successfully.
 */
static int configure_cycle_cidr(cidr_list *cl)
{
        struct in_addr ho_mask, ho_next;
        u_long netmask;

        if (cl->bits == 0) return 0; /* use the same address always */
        if (cl->bits >= 32) return 1; /* Constant address, therefore move to next in list */

        netmask = ((1UL << cl->bits) - 1UL) << (32UL - cl->bits);
        ho_mask.s_addr = ntohl(cl->ip.s_addr);
        ho_next.s_addr = ho_mask.s_addr + 1;

        if ((ho_next.s_addr & netmask) != (ho_mask.s_addr & netmask)) {
                /* Wrapped */
                ho_next.s_addr = ho_mask.s_addr & netmask;
                cl->ip.s_addr = htonl(ho_next.s_addr);
                return 0;
        }
        else {
                cl->ip.s_addr = htonl(ho_next.s_addr);
                return 1;
        }
}

struct in_addr configure_read_target_address(const char *filename, struct in_addr *interface)
{
        file_list *f = configure_find_file_details(filename);

        if (f && f->addresses) {
                if (!configure_cycle_cidr(f->addresses)) {
                        configure_cycle_list(&f->addresses);
                }
                platform_debug((LOG_DEBUG, "Assigning specific address %s for %s\n",
                        inet_ntoa(f->addresses->ip), filename));
                return f->addresses->ip;
        }

        (void) interface;

        if (!conf.default_address) {
                platform_log(LOG_CRIT, "No default multicast addresses specified");
                exit(EXIT_FAILURE);
        }

        configure_cycle_list(conf.default_address);
        return conf.default_address->ip;
}

struct in_addr configure_read_file_address(multicast_file *mf, struct in_addr *interface)
{
        return configure_read_target_address(multicast_file_get_filename(mf), interface);
}

u_char configure_read_specific_ttl(multicast_file *mf)
{
        file_list *f = configure_find_file_details(multicast_file_get_filename(mf));
        if (f && f->ttl != 0) {
                return (u_char) f->ttl;
        }

        return configure_read_default_ttl();
}

int configure_read_specific_enable(const char *name)
{
        file_list *f = configure_find_file_details(name);
        if (f) {
                if (f->flags & fl_DISABLED) return 0;
                if (f->flags & fl_ENABLED) return 1;
        }

        return conf.enabled != 0;
}

struct in_addr configure_read_specific_interface(multicast_file *mf)
{
        file_list *f = configure_find_file_details(multicast_file_get_filename(mf));
        if (f && f->interface != 0) {
                if (f->interface->ip.s_addr == INADDR_ANY) {
                        platform_log(LOG_ERR, "Address specification for %s failed\n",
                                multicast_file_get_filename(mf));
                        exit(EXIT_FAILURE);
                }
                return f->interface->ip;
        }

        return configure_read_default_interface();
}

unsigned long configure_read_specific_master_rate(multicast_file *mf)
{
        file_list *f = configure_find_file_details(multicast_file_get_filename(mf));
        if (f && f->rate != 0) {
                return f->rate;
        }

        return configure_read_minimum_master_rate();
}


void configure_set_target_address(const char *filename, const char *ip_string)
{
        /* Store away this configuration information one day */
        (void) filename;
        (void) ip_string;
}

size_t configure_clamp_mtu(size_t suggested, size_t if_support)
{
        /* Remember: Special case: 0 => client didn't sent a blksize option */

        /* Enforce the overall ceiling on packet sizes */
        if (if_support > conf.mtu_ceiling) {
                if_support = conf.mtu_ceiling;
        }

        /* Strategy: limit blksize choice to the interface MTU (minus IP, UDP headers) */
        if (suggested > if_support) {
                suggested = if_support;
        }

        return suggested;
}

u_short configure_read_default_port(void)
{
        return conf.default_port;
}

u_char configure_read_default_ttl(void)
{
        return conf.default_ttl;
}

struct in_addr configure_read_default_interface(void)
{
        static struct in_addr zero;
        zero.s_addr = INADDR_ANY;
        return conf.default_interface ? conf.default_interface->ip : zero;
}

unsigned long configure_read_minimum_master_rate(void)
{
        return conf.minimum_master_rate;
}

const char *configure_read_root_directory(void)
{
        return conf.root_directory ? conf.root_directory : default_root;
}

/* The remaining functions are the APIs called by the bison generated parser */

void config_parse_set_enable(int on)
{
        conf.enabled = on;
}

void config_parse_set_ttl(int ttl)
{
        conf.default_ttl = (u_char) ttl;
}

void config_parse_default_address(cidr_list *cl)
{
        cidr_free(conf.default_address);
        conf.default_address = cl;
}

void config_parse_set_interface(cidr_list *cl)
{
        cidr_free(conf.default_interface);
        conf.default_interface = cl;
}

void config_parse_add_file_details(file_list *fl)
{
        fl->next = conf.files;
        conf.files = fl;
}

void config_parse_set_transfer_rate(int rate)
{
        conf.minimum_master_rate = rate;
}

void config_parse_set_mtu_ceiling(int mtu)
{
        conf.mtu_ceiling = mtu;
}

void config_parse_set_home_directory(const char *dir)
{
        free(conf.root_directory);
        conf.root_directory = Strdup(dir);
}

void *parse_malloc(size_t bytes)
{
        void *ptr = malloc(bytes);

        if (!ptr) {
                platform_log(LOG_CRIT, "Memory exhaustion");
                exit(EXIT_FAILURE);
        }

        return ptr;
}

void parse_free(void *ptr)
{
        free(ptr);
}


static char *parse_Strdup(const char *s1)
{
        if (s1 == NULL) {
                return NULL;
        }
        else {
                const size_t length = strlen(s1) + 1;
                return memcpy(parse_malloc(length), s1, length);
        }
}

cidr_list *cidr_0alloc(void)
{
        cidr_list *list = parse_malloc(sizeof(*list));
        list->next = NULL;
        list->ip.s_addr = 0;
        list->bits = 0;

        return list;
}


cidr_list *cidr_alloc(struct in_addr ia, int bits)
{
        cidr_list *list = cidr_0alloc();
        list->ip = ia;
        list->bits = bits;

        return list;
}

void cidr_free(cidr_list *cl)
{
        if (cl != NULL) {
                cidr_list *next = cl->next;
                free(cl);
                cidr_free(next);
        }
}


file_list *filelist_alloc(char *filename)
{
        file_list *fn = parse_malloc(sizeof(*fn));

        fn->next = NULL;
        fn->filename = parse_Strdup(filename);
        fn->ttl = 0;
        fn->rate = 0;
        fn->addresses = NULL;
        fn->interface = NULL;
        fn->flags = fl_MASTER;

        return fn;
}

void filelist_free(file_list *fl)
{
        if (fl != NULL) {
                file_list *next = fl->next;
                cidr_free(fl->addresses);
                cidr_free(fl->interface);
                free(fl->filename);
                free(fl);
                filelist_free(next);
        }
}

void config_parse_process_scope(file_list *fl, file_list *details)
{
        file_list *det;

        for (det = details; det; det=det->next) {
                switch (det->flags) {
                        case fl_TTL:
                                fl->ttl = det->ttl;
                                break;
                        case fl_DESTS:
                                fl->addresses = det->addresses;
                                det->addresses = NULL;
                                break;
                        case fl_RATE:
                                fl->rate = det->rate;
                                break;
                        case fl_INTERFACE:
                                fl->interface = det->interface;
                                det->interface = NULL;
                                break;
                }
                fl->flags |= (det->flags & (fl_ENABLED | fl_DISABLED));
        }

        config_parse_add_file_details(fl);
}

static void print_cidr_list(cidr_list *cl, int indent)
{
        if (cl) {
                while (cl) {
                        char buf[32];
                        if (cl->bits == 32) {
                                inet_ntop(AF_INET, &cl->ip, buf, sizeof(buf));
                        }
                        else {
                                if (cl->bits == 0 && cl->ip.s_addr == 0L) {
                                        strcpy(buf, "All");
                                }
                                else
                                inet_net_ntop(AF_INET, &cl->ip, cl->bits, buf, sizeof(buf));
                        }
                        printf("%-*s%s\n", indent, "", buf);
                        cl = cl->next;
                }
        }
        else {
                printf("%-*sNone\n", indent, "");
        }

}

static void config_dump_rate(unsigned long rate)
{
        double drate_K = ((double) rate) / (1L << 10);
        double drate_M = ((double) rate) / (1L << 20);
        printf("%lu bits/sec (%.2f Kbit/sec; %.2f Mbit/sec)\n", rate, drate_K, drate_M);
}

static void print_file_list(file_list *fl)
{
        if (!fl) {
                printf("  No specific file information\n");
        }
        else for (;fl; fl=fl->next) {
                printf("  Filename: %s\n", fl->filename);
                if (fl->flags & fl_ENABLED) {
                        printf("    Transmission enabled\n");
                }
                else if ((fl->flags & fl_DISABLED) || !conf.enabled) {
                        printf(" ** Transmission disabled\n");
                }
                if (fl->ttl != 0) {
                        printf("    Multicast TTL: %u\n", fl->ttl);
                }
                if (fl->addresses) {
                        printf("    Multicast target addresses:\n");
                        print_cidr_list(fl->addresses, 6);
                }
                if (fl->rate != 0) {
                        printf("    Target transmission rate: ");
                        config_dump_rate(fl->rate);
                }
                if (fl->interface != 0) {
                        printf("    Local transmission interface: ");
                        print_cidr_list(fl->interface, 0);
                }
        }
}

static void config_dump(int dump_type)
{
        signal(dump_type, SIG_IGN);
        printf("Configuration:\n");

        printf("  Default multicast TTL: %u\n", conf.default_ttl);
        printf("  Default target port: %hu\n", conf.default_port);
        printf("  Default interface for transmission: ");
        print_cidr_list(conf.default_interface, 0);
        printf("  Default multicast target addresses:\n");
        print_cidr_list(conf.default_address, 4);
/*        printf("  MTU ceiling: %d\n", conf.mtu_ceiling); */
        printf("  Target master transfer rate: ");
        config_dump_rate(conf.minimum_master_rate);
        printf("  Root directory: %s\n", conf.root_directory);
        printf("  File transmission enabled by default? %s\n", conf.enabled ? "yes" : "no");
        printf("\n");
        printf("Known files:\n");
        print_file_list(conf.files);
        printf("\nEnd of configuration\n");

        signal(dump_type, config_dump);
}
