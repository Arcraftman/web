#ifndef _RECEPTOR_PALLOC_H_
#define _RECEPTOR_PALLOC_H_

#include <receptor/def.h>

typedef struct receptor_pool_s     receptor_pool_t;

struct receptor_pool_s {
	char                *last;
	char                *end;
	receptor_pool_t     *next;
	receptor_uint_t     failed;
};

receptor_pool_t *receptor_create_pool(size_t size);
void receptor_destroy_pool(receptor_pool_t *pool);
void *receptor_palloc(receptor_pool_t *pool, size_t size);
void *receptor_pcalloc(receptor_pool_t *pool, size_t size);

#endif /* _RECEPTOR_PALLOC_H_ */