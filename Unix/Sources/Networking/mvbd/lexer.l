
%{

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
#include "parser.h"

#define yylex parserlex

#define YY_ALWAYS_INTERACTIVE 0
#define YY_NEVER_INTERACTIVE 1
#define YY_MAIN 0
#define YY_STACK_USED 0

int yywrap(void)
{
        return 1;
}
extern int parserparse(void);
static int lexer_cidr(const char *text, cidr_list *cl);

%}

DIGIT   [0-9]
NUMBER  {DIGIT}+
KNUMBER {DIGIT}+["K"|"k"]
MNUMBER {DIGIT}+["M"|"m"]
IPCOMP  "."{NUMBER}
IP      {NUMBER}{IPCOMP}{IPCOMP}{IPCOMP}
CIDR    {NUMBER}{IPCOMP}*"/"{NUMBER}

COMMENT #.*$

FILENAME [A-Za-z0-9/:\\\-.]+

%%

{COMMENT}               /* swallow single line comments */

address                 return TOK_ADDRESS;
all                     return TOK_ALL;
allow                   return TOK_ALLOW;
default                 return TOK_DEFAULT;
disable                 return TOK_DISABLE;
disallow                return TOK_DISALLOW;
enable                  return TOK_ENABLE;
grace                   return TOK_GRACE;
file                    return TOK_FILENAME;
files                   return TOK_FILES;
interface               return TOK_INTERFACE;
limit                   return TOK_LIMIT;
lives                   return TOK_LIVES;
max|maximum             return TOK_MAXIMUM;
min|minimum             return TOK_MINIMUM;
mtu                     return TOK_MTU;
multicast               return TOK_MULTICAST;
none                    return TOK_NONE;
period                  return TOK_PERIOD;
rate                    return TOK_RATE;
root                    return TOK_ROOT;
servers                 return TOK_SERVERS;
timeout                 return TOK_TIMEOUT;
ttl                     return TOK_TTL;
unicast                 return TOK_UNICAST;
unknown                 return TOK_UNKNOWN;


{CIDR}                  if (lexer_cidr(yytext, &parserlval.cidr)) return TOK_CIDR; REJECT;

{IP}                    if (inet_pton(AF_INET, yytext, &parserlval.ip)) return TOK_IPADDR; REJECT;

{MNUMBER}               parserlval.num_int = atoi(yytext) << 20; return TOK_INT;

{KNUMBER}               parserlval.num_int = atoi(yytext) << 10; return TOK_INT;

{NUMBER}                parserlval.num_int = atoi(yytext); return TOK_INT;

{FILENAME}              parserlval.string = yytext; return TOK_STRING;

[ \t\r\n]+              /* swallow whitespace */

[{};\/]                 return *yytext;


%%

static int lexer_cidr(const char *text, cidr_list *cl)
{
        cl->next = NULL;
        cl->bits = inet_net_pton(AF_INET, text, &cl->ip, sizeof(cl->ip));
        return (cl->bits != -1);
}

void lexer_exec(FILE *source)
{
        (void) input;
        (void) yyunput;
        (void) yy_flex_realloc;

        yyin = source;
        parserparse();
}
