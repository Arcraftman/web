#include <receptor/def.h>
#include "receptor_palloc.h"
#include <stdlib.h>
#include <string.h>

#define RECEPTOR_DEFAULT_POOL_SIZE    (16 * 1024)
#define RECEPTOR_POOL_ALIGNMENT       16
#define RECEPTOR_MAX_ALLOC_FROM_POOL  (RECEPTOR_DEFAULT_POOL_SIZE - 1)

#ifdef _WIN32
#include <malloc.h>
#endif

static void *receptor_memalign(size_t alignment, size_t size);

receptor_pool_t *
receptor_create_pool(size_t size)
{
	receptor_pool_t  *p;

	if (size < RECEPTOR_DEFAULT_POOL_SIZE) {
		size = RECEPTOR_DEFAULT_POOL_SIZE;
	}

	p = receptor_memalign(RECEPTOR_POOL_ALIGNMENT, size);
	if (p == NULL) {
		return NULL;
	}

	p->last = (char *)p + sizeof(receptor_pool_t);
	p->end = (char *)p + size;
	p->next = NULL;
	p->failed = 0;

	return p;
}

void
receptor_destroy_pool(receptor_pool_t *pool)
{
	receptor_pool_t  *p, *n;

	for (p = pool, n = pool->next; /* void */; p = n, n = n->next) {
		free(p);

		if (n == NULL) {
			break;
		}
	}
}

void *
receptor_palloc(receptor_pool_t *pool, size_t size)
{
	receptor_pool_t  *p, *new_p;

	if (size <= RECEPTOR_MAX_ALLOC_FROM_POOL) {
		for (p = pool; p; p = p->next) {
			if ((size_t)(p->end - p->last) >= size) {
				void *ptr = p->last;
				p->last += size;
				return ptr;
			}
		}

		/* 需要创建新的pool块 */
		new_p = receptor_create_pool(RECEPTOR_DEFAULT_POOL_SIZE);
		if (new_p == NULL) {
			return NULL;
		}

		/* 添加到链表 */
		for (p = pool; p->next; p = p->next) { /* void */ }
		p->next = new_p;

		void *ptr = new_p->last;
		new_p->last += size;
		return ptr;
	}

	/* 大内存分配 */
	return receptor_memalign(RECEPTOR_POOL_ALIGNMENT, size);
}

void *
receptor_pcalloc(receptor_pool_t *pool, size_t size)
{
	void *p = receptor_palloc(pool, size);
	if (p) {
		memset(p, 0, size);
	}
	return p;
}

static void *
receptor_memalign(size_t alignment, size_t size)
{
	void *p;

#ifdef _WIN32
	/* Windows 使用 _aligned_malloc */
	p = _aligned_malloc(size, alignment);
#else
	/* Unix 使用 posix_memalign */
	if (posix_memalign(&p, alignment, size) != 0) {
		return NULL;
	}
#endif

	return p;
}