#include <ctype.h>

/* Function to compare two strings case insensitively
 *
 * Originally: sbrodie
 *
 * Parameters: matches those of strcmp.
 * Result: matches the exit conditions of strcmp.
 *
 *
 * The conversions to unsigned int stop the compiler messing around with
 * shifts all over the place whilst trying to promote the chars to int
 * whilst retaining the sign.
 *
 * Problems: Choice of return value when strings do not match is based
 *           upon character number rather than any alphabetic sorting.
 *
 */
int Strcmp_ci(const char *first, const char *second)
{
	for (;;) {
		unsigned int a = *first++;
		unsigned int b = *second++;

		if (a == 0) return -b;
		if (a != b) {
			unsigned int c = (unsigned int) tolower(a);
			unsigned int d = (unsigned int) tolower(b);
			signed int result = c - d;
			if (result != 0) return result;
		}
	}
}

