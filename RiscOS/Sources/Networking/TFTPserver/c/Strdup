#include <string.h>
#include <stdlib.h>
/* Function to duplicate a string (allocate memory and copy bytewise)
 *
 * Originally: sbrodie
 *
 * Parameters: const char *s1 - source string
 * Result: char * - duplicate string (or NULL on failure or s1 NULL)
 *
 *
 * Problems: None known
 *
 */

char *Strdup(const char *s1)
{
  	if (s1 == NULL) {
  	        return NULL;
  	}
  	else {
		const size_t length = strlen(s1) + 1;
		char *const s2 = malloc(length);

		if (s2 == NULL) return NULL;
	        return memcpy(s2, s1, length);
  	}
}

