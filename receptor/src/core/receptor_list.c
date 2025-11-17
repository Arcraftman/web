#include "receptor_list.h"
#include <string.h>
#include <stdlib.h>

RECEPTOR_API receptor_list_t*
receptor_list_create(receptor_pool_t *pool, size_t data_size)
{
	receptor_list_t *list;

	if (pool == NULL) {
		return NULL;
	}

	list = receptor_pcalloc(pool, sizeof(receptor_list_t));
	if (list == NULL) {
		return NULL;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->pool = pool;
	list->data_size = data_size;

	return list;
}

RECEPTOR_API void
receptor_list_destroy(receptor_list_t *list)
{
	if (list) {
		receptor_list_clear(list);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
}

static receptor_list_node_t*
receptor_list_create_node(receptor_list_t *list, const void *data)
{
	receptor_list_node_t *node;
	void *node_data;

	node = receptor_pcalloc(list->pool, sizeof(receptor_list_node_t));
	if (node == NULL) {
		return NULL;
	}

	if (list->data_size > 0) {
		node_data = receptor_palloc(list->pool, list->data_size);
		if (node_data == NULL) {
			return NULL;
		}
		if (data) {
			memcpy(node_data, data, list->data_size);
		}
	}
	else {
		node_data = (void*)data;  /* 动态大小，直接存储指针 */
	}

	node->data = node_data;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

RECEPTOR_API receptor_int_t
receptor_list_push_front(receptor_list_t *list, const void *data)
{
	receptor_list_node_t *node;

	if (list == NULL) {
		return RECEPTOR_ERROR;
	}

	node = receptor_list_create_node(list, data);
	if (node == NULL) {
		return RECEPTOR_ERROR;
	}

	if (list->head == NULL) {
		/* 空链表 */
		list->head = node;
		list->tail = node;
	}
	else {
		/* 非空链表 */
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
	}

	list->size++;
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_list_push_back(receptor_list_t *list, const void *data)
{
	receptor_list_node_t *node;

	if (list == NULL) {
		return RECEPTOR_ERROR;
	}

	node = receptor_list_create_node(list, data);
	if (node == NULL) {
		return RECEPTOR_ERROR;
	}

	if (list->tail == NULL) {
		/* 空链表 */
		list->head = node;
		list->tail = node;
	}
	else {
		/* 非空链表 */
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}

	list->size++;
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_list_pop_front(receptor_list_t *list, void *data)
{
	receptor_list_node_t *node;

	if (list == NULL || list->head == NULL) {
		return RECEPTOR_ERROR;
	}

	node = list->head;

	if (data && list->data_size > 0) {
		memcpy(data, node->data, list->data_size);
	}

	if (list->head == list->tail) {
		/* 只有一个元素 */
		list->head = NULL;
		list->tail = NULL;
	}
	else {
		list->head = node->next;
		list->head->prev = NULL;
	}

	list->size--;
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_list_pop_back(receptor_list_t *list, void *data)
{
	receptor_list_node_t *node;

	if (list == NULL || list->tail == NULL) {
		return RECEPTOR_ERROR;
	}

	node = list->tail;

	if (data && list->data_size > 0) {
		memcpy(data, node->data, list->data_size);
	}

	if (list->head == list->tail) {
		/* 只有一个元素 */
		list->head = NULL;
		list->tail = NULL;
	}
	else {
		list->tail = node->prev;
		list->tail->next = NULL;
	}

	list->size--;
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_list_insert(receptor_list_t *list, receptor_list_iter_t *iter,
	const void *data)
{
	receptor_list_node_t *node, *current;

	if (list == NULL || iter == NULL || !receptor_list_iter_valid(iter)) {
		return RECEPTOR_ERROR;
	}

	node = receptor_list_create_node(list, data);
	if (node == NULL) {
		return RECEPTOR_ERROR;
	}

	current = iter->current;

	if (current == list->head) {
		/* 在头部插入 */
		return receptor_list_push_front(list, data);
	}

	/* 在中间插入 */
	node->prev = current->prev;
	node->next = current;
	current->prev->next = node;
	current->prev = node;

	list->size++;
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_list_erase(receptor_list_t *list, receptor_list_iter_t *iter)
{
	receptor_list_node_t *node;

	if (list == NULL || iter == NULL || !receptor_list_iter_valid(iter)) {
		return RECEPTOR_ERROR;
	}

	node = iter->current;

	if (node == list->head) {
		return receptor_list_pop_front(list, NULL);
	}
	else if (node == list->tail) {
		return receptor_list_pop_back(list, NULL);
	}
	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
		list->size--;
	}

	return RECEPTOR_OK;
}

RECEPTOR_API receptor_list_iter_t
receptor_list_find(receptor_list_t *list, const void *data,
	int(*compar)(const void *, const void *))
{
	receptor_list_iter_t iter;

	if (list == NULL || compar == NULL) {
		iter.list = NULL;
		iter.current = NULL;
		return iter;
	}

	iter = receptor_list_begin(list);
	while (receptor_list_iter_valid(&iter)) {
		void *current_data = receptor_list_iter_data(&iter);
		if (compar(current_data, data) == 0) {
			return iter;
		}
		iter = receptor_list_iter_next(&iter);
	}

	iter.list = NULL;
	iter.current = NULL;
	return iter;
}

RECEPTOR_API receptor_uint_t
receptor_list_size(receptor_list_t *list)
{
	return list ? list->size : 0;
}

RECEPTOR_API receptor_int_t
receptor_list_empty(receptor_list_t *list)
{
	return (list == NULL || list->size == 0) ? 1 : 0;
}

RECEPTOR_API void
receptor_list_clear(receptor_list_t *list)
{
	receptor_list_node_t *node, *next;

	if (list == NULL) {
		return;
	}

	node = list->head;
	while (node != NULL) {
		next = node->next;
		/* 内存池会自动释放内存 */
		node = next;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

/* 简单的归并排序实现 */
static receptor_list_node_t*
receptor_list_merge_sort(receptor_list_node_t *head,
	int(*compar)(const void *, const void *),
	receptor_list_node_t **tail)
{
	receptor_list_node_t *slow, *fast, *left, *right, *result, *last;

	if (head == NULL || head->next == NULL) {
		if (tail) *tail = head;
		return head;
	}

	/* 找到中间点 */
	slow = head;
	fast = head->next;

	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}

	right = slow->next;
	slow->next = NULL;

	/* 递归排序 */
	left = receptor_list_merge_sort(head, compar, NULL);
	right = receptor_list_merge_sort(right, compar, tail);

	/* 合并 */
	result = NULL;
	last = NULL;

	while (left && right) {
		if (compar(left->data, right->data) <= 0) {
			if (last) {
				last->next = left;
				left->prev = last;
			}
			else {
				result = left;
			}
			last = left;
			left = left->next;
		}
		else {
			if (last) {
				last->next = right;
				right->prev = last;
			}
			else {
				result = right;
			}
			last = right;
			right = right->next;
		}
	}

	if (left) {
		if (last) {
			last->next = left;
			left->prev = last;
		}
		else {
			result = left;
		}
		if (tail) *tail = last;
	}
	else if (right) {
		if (last) {
			last->next = right;
			right->prev = last;
		}
		else {
			result = right;
		}
		if (tail) *tail = last;
	}

	return result;
}

RECEPTOR_API void
receptor_list_sort(receptor_list_t *list,
	int(*compar)(const void *, const void *))
{
	receptor_list_node_t *tail;

	if (list == NULL || list->size < 2 || compar == NULL) {
		return;
	}

	list->head = receptor_list_merge_sort(list->head, compar, &tail);
	list->tail = tail;

	/* 重新设置prev指针 */
	receptor_list_node_t *node = list->head;
	receptor_list_node_t *prev = NULL;

	while (node) {
		node->prev = prev;
		prev = node;
		node = node->next;
	}
}

/* ==================== 迭代器函数实现 ==================== */

RECEPTOR_API receptor_list_iter_t
receptor_list_begin(receptor_list_t *list)
{
	receptor_list_iter_t iter;

	iter.list = list;
	iter.current = list ? list->head : NULL;

	return iter;
}

RECEPTOR_API receptor_list_iter_t
receptor_list_end(receptor_list_t *list)
{
	receptor_list_iter_t iter;

	iter.list = list;
	iter.current = list ? list->tail : NULL;

	return iter;
}

RECEPTOR_API receptor_list_iter_t
receptor_list_iter_next(receptor_list_iter_t *iter)
{
	receptor_list_iter_t next;

	if (iter == NULL || iter->current == NULL) {
		next.list = NULL;
		next.current = NULL;
		return next;
	}

	next.list = iter->list;
	next.current = iter->current->next;

	return next;
}

RECEPTOR_API receptor_list_iter_t
receptor_list_iter_prev(receptor_list_iter_t *iter)
{
	receptor_list_iter_t prev;

	if (iter == NULL || iter->current == NULL) {
		prev.list = NULL;
		prev.current = NULL;
		return prev;
	}

	prev.list = iter->list;
	prev.current = iter->current->prev;

	return prev;
}

RECEPTOR_API void*
receptor_list_iter_data(receptor_list_iter_t *iter)
{
	if (iter == NULL || iter->current == NULL) {
		return NULL;
	}

	return iter->current->data;
}

RECEPTOR_API receptor_int_t
receptor_list_iter_valid(receptor_list_iter_t *iter)
{
	return (iter != NULL && iter->list != NULL && iter->current != NULL) ? 1 : 0;
}