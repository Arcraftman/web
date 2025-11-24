#include <receptor/def.h>
#include "receptor_array.h"
#include <string.h>
#include <stdlib.h>

/* 默认扩容因子 */
#define RECEPTOR_ARRAY_DEFAULT_CAPACITY  16
#define RECEPTOR_ARRAY_GROWTH_FACTOR     2

RECEPTOR_API receptor_array_t*
receptor_array_create(receptor_pool_t *pool, receptor_uint_t n, size_t size)
{
	receptor_array_t *array;

	if (pool == NULL || size == 0) {
		return NULL;
	}

	/* 分配数组结构 */
	array = receptor_pcalloc(pool, sizeof(receptor_array_t));
	if (array == NULL) {
		return NULL;
	}

	/* 初始化数组 */
	array->nelts = 0;
	array->size = size;
	array->nalloc = (n > 0) ? n : RECEPTOR_ARRAY_DEFAULT_CAPACITY;
	array->pool = pool;

	/* 分配元素内存 */
	array->elts = receptor_palloc(pool, array->nalloc * size);
	if (array->elts == NULL) {
		return NULL;
	}

	return array;
}

RECEPTOR_API void
receptor_array_destroy(receptor_array_t *array)
{
	if (array && array->pool) {
		/* 内存池会自动管理内存，这里只需要重置状态 */
		array->elts = NULL;
		array->nelts = 0;
		array->nalloc = 0;
	}
}

RECEPTOR_API void*
receptor_array_push(receptor_array_t *array)
{
	void *elt;

	if (array == NULL) {
		return NULL;
	}

	/* 检查是否需要扩容 */
	if (array->nelts >= array->nalloc) {
		receptor_uint_t new_nalloc;
		void *new_elts;

		new_nalloc = array->nalloc * RECEPTOR_ARRAY_GROWTH_FACTOR;
		if (new_nalloc < RECEPTOR_ARRAY_DEFAULT_CAPACITY) {
			new_nalloc = RECEPTOR_ARRAY_DEFAULT_CAPACITY;
		}

		new_elts = receptor_palloc(array->pool, new_nalloc * array->size);
		if (new_elts == NULL) {
			return NULL;
		}

		/* 复制原有数据 */
		if (array->nelts > 0) {
			memcpy(new_elts, array->elts, array->nelts * array->size);
		}

		array->elts = new_elts;
		array->nalloc = new_nalloc;
	}

	/* 返回新元素位置 */
	elt = (u_char*)array->elts + array->nelts * array->size;
	array->nelts++;

	return elt;
}

RECEPTOR_API void*
receptor_array_push_n(receptor_array_t *array, receptor_uint_t n)
{
	void *elts;

	if (array == NULL || n == 0) {
		return NULL;
	}

	/* 检查是否需要扩容 */
	if (array->nelts + n > array->nalloc) {
		receptor_uint_t new_nalloc;
		void *new_elts;

		new_nalloc = array->nalloc;
		while (new_nalloc < array->nelts + n) {
			new_nalloc *= RECEPTOR_ARRAY_GROWTH_FACTOR;
		}

		new_elts = receptor_palloc(array->pool, new_nalloc * array->size);
		if (new_elts == NULL) {
			return NULL;
		}

		/* 复制原有数据 */
		if (array->nelts > 0) {
			memcpy(new_elts, array->elts, array->nelts * array->size);
		}

		array->elts = new_elts;
		array->nalloc = new_nalloc;
	}

	/* 返回新元素位置 */
	elts = (u_char*)array->elts + array->nelts * array->size;
	array->nelts += n;

	return elts;
}

RECEPTOR_API void*
receptor_array_pop(receptor_array_t *array)
{
	if (array == NULL || array->nelts == 0) {
		return NULL;
	}

	array->nelts--;
	return (u_char*)array->elts + array->nelts * array->size;
}

RECEPTOR_API void*
receptor_array_get(receptor_array_t *array, receptor_uint_t index)
{
	if (array == NULL || index >= array->nelts) {
		return NULL;
	}

	return (u_char*)array->elts + index * array->size;
}

RECEPTOR_API receptor_int_t
receptor_array_set(receptor_array_t *array, receptor_uint_t index, const void *value)
{
	void *elt;

	if (array == NULL || index >= array->nelts || value == NULL) {
		return RECEPTOR_ERROR;
	}

	elt = (u_char*)array->elts + index * array->size;
	memcpy(elt, value, array->size);

	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_array_insert(receptor_array_t *array, receptor_uint_t index, const void *value)
{
	void *elts;

	if (array == NULL || index > array->nelts || value == NULL) {
		return RECEPTOR_ERROR;
	}

	/* 确保有足够空间 */
	elts = receptor_array_push(array);
	if (elts == NULL) {
		return RECEPTOR_ERROR;
	}

	/* 移动元素 */
	if (index < array->nelts - 1) {
		memmove((u_char*)array->elts + (index + 1) * array->size,
			(u_char*)array->elts + index * array->size,
			(array->nelts - index - 1) * array->size);
	}

	/* 插入新元素 */
	memcpy((u_char*)array->elts + index * array->size, value, array->size);

	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_array_delete(receptor_array_t *array, receptor_uint_t index)
{
	if (array == NULL || index >= array->nelts) {
		return RECEPTOR_ERROR;
	}

	/* 移动元素覆盖删除位置 */
	if (index < array->nelts - 1) {
		memmove((u_char*)array->elts + index * array->size,
			(u_char*)array->elts + (index + 1) * array->size,
			(array->nelts - index - 1) * array->size);
	}

	array->nelts--;
	return RECEPTOR_OK;
}

RECEPTOR_API void
receptor_array_clear(receptor_array_t *array)
{
	if (array) {
		array->nelts = 0;
	}
}


RECEPTOR_API void
receptor_array_sort(receptor_array_t *array,
	int(*compar)(const void *, const void *))
{
	if (array && array->nelts > 1 && compar) {
		qsort(array->elts, array->nelts, array->size, compar);
	}
}

RECEPTOR_API void*
receptor_array_find(receptor_array_t *array, const void *key,
	int(*compar)(const void *, const void *))
{
	receptor_uint_t i;
	u_char *elts;

	if (array == NULL || key == NULL || compar == NULL) {
		return NULL;
	}

	elts = (u_char*)array->elts;
	for (i = 0; i < array->nelts; i++) {
		if (compar(elts + i * array->size, key) == 0) {
			return elts + i * array->size;
		}
	}

	return NULL;
}

RECEPTOR_API void
receptor_array_foreach(receptor_array_t *array,
	void(*func)(void *element, void *data),
	void *data)
{
	receptor_uint_t i;
	u_char *elts;

	if (array == NULL || func == NULL) {
		return;
	}

	elts = (u_char*)array->elts;
	for (i = 0; i < array->nelts; i++) {
		func(elts + i * array->size, data);
	}
}