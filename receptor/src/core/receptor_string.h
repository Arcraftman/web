#ifndef _RECEPTOR_STRING_H_
#define _RECEPTOR_STRING_H_

#include "../include/receptor_defs.h"

typedef struct {
	size_t      len;
	u_char     *data;
} receptor_str_t;

#define receptor_string(str)     { sizeof(str) - 1, (u_char *) str }
#define receptor_null_string     { 0, NULL }

#define receptor_str_set(str, text) \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text

receptor_int_t receptor_strcasecmp(u_char *s1, u_char *s2, size_t n);

#endif /* _RECEPTOR_STRING_H_ */