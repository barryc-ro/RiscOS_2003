/* SSLeay/robits/envconf.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char *getconf(char *name)
{
    return getenv(name);
}
