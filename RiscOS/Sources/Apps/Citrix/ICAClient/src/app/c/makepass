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
    char *input, *work;
    char output[64];
    BOOL decode;
    int input_len;

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

    input_len = strlen(input);

    /* copy input as the buffer is written back to */
    work = malloc(input_len + 1);
    strcpy(work, input);
    
    if (decode)
	DecryptFromAscii(work, input_len, output, sizeof(output));
    else
	EncryptToAscii(work, input_len, output, sizeof(output));

    printf("%s\n", output);
    
    return EXIT_SUCCESS;
}


/* eof makepass.c */
