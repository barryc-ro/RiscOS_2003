/* > makepass.c

 *

 */


#include "windows.h"

#include "../inc/wfengapi.h"
#include "../inc/encrypt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char *input;
    char output[64];
    BOOL decode;

    if (argc == 1 || argc > 3)
    {
	fprintf(stderr, "Syntax: makepass [-e|-d] <password>");
	return EXIT_FAILURE;
    }

    if (argc == 2)
    {
	decode = FALSE;
	input = argv[1];
    }
    else
    {
	decode = strcmp(argv[1], "-d") == 0;
	input = argv[2];
    }
    
    if (decode)
	DecryptFromAscii(input, strlen(input), output, sizeof(output));
    else
	EncryptToAscii(input, strlen(input), output, sizeof(output));

    printf("%s\n", output);
    
    return EXIT_SUCCESS;
}


/* eof makepass.c */
