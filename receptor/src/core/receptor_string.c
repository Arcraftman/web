#include "receptor_string.h"
#include <ctype.h>

receptor_int_t
receptor_strcasecmp(u_char *s1, u_char *s2, size_t n)
{
	u_char  c1, c2;

	while (n--) {
		c1 = (u_char)tolower(*s1++);
		c2 = (u_char)tolower(*s2++);

		if (c1 != c2) {
			return c1 - c2;
		}

		if (c1 == 0) {
			return 0;
		}
	}

	return 0;
}