
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

/* Video multicaster server configuration file parser. */

%{

#include "multicaster.h"
#include "config-parse.h"

int yyerror(const char *s)
{
        fprintf(stderr, "configuration_parser: %s\n", s);
        exit(1);
        return 0;
}

#ifndef alloc
#define alloca malloc
#endif

#define YYMAXDEPTH 0
#ifdef __CC_NORCROFT
#  define __GNUC__ 1
#endif

extern int yylex(void);
extern int yyparser(void);

%}

%union {
        char *          string;
        const char *    string_const;
        struct in_addr  ip;
        cidr_list       cidr;
        cidr_list *     iplist;
        file_list *     filelist;
        file_list *     details;
        int             num_int;
}

%token                  TOK_ADDRESS
%token                  TOK_ALL
%token                  TOK_ALLOW
%token                  TOK_DEFAULT
%token                  TOK_DISABLE
%token                  TOK_DISALLOW
%token                  TOK_ENABLE
%token                  TOK_FILENAME
%token                  TOK_FILES
%token                  TOK_GRACE
%token                  TOK_INTERFACE
%token                  TOK_LIMIT
%token                  TOK_LIVES
%token                  TOK_MAXIMUM
%token                  TOK_MINIMUM
%token                  TOK_MTU
%token                  TOK_MULTICAST
%token                  TOK_NONE
%token                  TOK_PERIOD
%token                  TOK_RATE
%token                  TOK_ROOT
%token                  TOK_SERVERS
%token                  TOK_TIMEOUT
%token                  TOK_TTL
%token                  TOK_UNICAST
%token                  TOK_UNKNOWN

%token  <ip>            TOK_IPADDR
%token  <cidr>          TOK_CIDR
%token  <num_int>       TOK_INT
%token  <string>        TOK_STRING
%token  <num_int>       TOK_CIDRMASK

%type   <num_int>       integer
%type   <num_int>       multicastttl
%type   <num_int>       multicastrate
%type   <filelist>      scope
%type   <iplist>        cidr
%type   <iplist>        cidrlist
%type   <iplist>        emptycidrlist
%type   <iplist>        multicastdestinations
%type   <iplist>        interfacespec
%type   <cidr>          interfacename

%type   <details>       scopedeclaration
%type   <details>       scopestatement

%%

declaration             : /* empty */
                        | toplevelstatement ';' declaration
                        | scope '{' scopedeclaration '}' declaration
                                { config_parse_process_scope($1,$3); }
                        ;

toplevelstatement       : TOK_DEFAULT multicastdestinations
                                { config_parse_default_address($2); }
                        | TOK_DEFAULT multicastttl
                                { config_parse_set_ttl($2); }
                        | TOK_DEFAULT multicastrate
                                { config_parse_set_transfer_rate($2); }
                        | TOK_ROOT TOK_STRING
                                { config_parse_set_home_directory($2); }
                        | TOK_DEFAULT interfacespec
                                { config_parse_set_interface($2); }
                        | TOK_DEFAULT TOK_DISABLE
                                { config_parse_set_enable(0); }
                        | TOK_DEFAULT TOK_ENABLE
                                { config_parse_set_enable(1); }
                        ;

scopedeclaration        : /* empty */
                                { $$ = NULL; }
                        | scopestatement ';' scopedeclaration
                                { $1->next = $3; $$ = $1; }
                        ;

scopestatement          : multicastttl
                                { $$ = filelist_alloc(NULL);
                                  $$->ttl = $1;
                                  $$->flags |= fl_TTL;
                                }
                        | multicastdestinations
                                { $$ = filelist_alloc(NULL);
                                  $$->addresses = $1;
                                  $$->flags |= fl_DESTS;
                                }
                        | multicastrate
                                { $$ = filelist_alloc(NULL);
                                  $$->rate = $1;
                                  $$->flags |= fl_RATE;
                                }
                        | interfacespec
                                { $$ = filelist_alloc(NULL);
                                  $$->interface = $1;
                                  $$->flags |= fl_INTERFACE;
                                }
                        | TOK_DISABLE
                                { $$ = filelist_alloc(NULL);
                                  $$->flags |= fl_DISABLED;
                                }
                        | TOK_ENABLE
                                { $$ = filelist_alloc(NULL);
                                  $$->flags |= fl_ENABLED;
                                }
                        ;

scope                   : TOK_FILENAME TOK_STRING
                                { $$ = filelist_alloc($2); }
                        ;

/* The following options are trivial but may be used either in a
 * top-level declaration or a scoped declaration
 */

multicastttl            : TOK_MULTICAST TOK_TTL TOK_INT
                                { $$ = $3; }
                        ;

multicastdestinations   : TOK_MULTICAST TOK_ADDRESS cidrlist
                                { $$ = $3; }
                        ;

multicastrate           : TOK_RATE integer
                                { $$ = $2; }
                        ;

integer                 : TOK_INT
                        ;

/* IP address list / CIDR parsing */

cidr                    : TOK_CIDR
                                { $$ = cidr_alloc($1.ip,$1.bits); }
                        | TOK_IPADDR
                                { $$ = cidr_alloc($1,32); }
                        ;

emptycidrlist           : /*empty*/
                                { $$ = NULL; }
                        | cidr emptycidrlist
                                { $1->next = $2; $$ = $1; }
                        ;

cidrlist                : TOK_NONE
                                { $$ = NULL; }
                        | TOK_ALL
                                { $$ = cidr_0alloc(); }
                        | cidr emptycidrlist
                                { $1->next = $2; $$ = $1; }
                        ;

/* Interface name / IP address parsing */

interfacespec           : TOK_INTERFACE cidr
                                { $$ = $2; }
                        | TOK_INTERFACE interfacename
                                { $$ = cidr_alloc($2.ip,32); }
                        ;

interfacename           : TOK_STRING
                                { $$.ip = platform_find_interface_address($1); }
                        ;

%%

