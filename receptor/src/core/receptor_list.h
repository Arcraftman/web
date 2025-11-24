#ifndef _RECEPTOR_LIST_H_
#define _RECEPTOR_LIST_H_

#include "receptor/def.h"
#include "receptor_palloc.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* ==================== 链表节点定义 ==================== */

	/**
	 * 链表节点结构
	 */
	typedef struct receptor_list_node_s receptor_list_node_t;

	struct receptor_list_node_s {
		receptor_list_node_t   *prev;      /* 前驱节点 */
		receptor_list_node_t   *next;      /* 后继节点 */
		void                  *data;      /* 节点数据 */
	};

	/* ==================== 链表结构定义 ==================== */

	/**
	 * 双向链表结构
	 */
	typedef struct receptor_list_s receptor_list_t;

	struct receptor_list_s {
		receptor_list_node_t   *head;      /* 头节点 */
		receptor_list_node_t   *tail;      /* 尾节点 */
		receptor_uint_t         size;      /* 链表大小 */
		receptor_pool_t        *pool;      /* 内存池 */
		size_t                  data_size; /* 数据大小（0表示动态大小） */
	};

	/* ==================== 链表迭代器定义 ==================== */

	/**
	 * 链表迭代器
	 */
	typedef struct receptor_list_iter_s receptor_list_iter_t;

	struct receptor_list_iter_s {
		receptor_list_t        *list;      /* 关联的链表 */
		receptor_list_node_t   *current;   /* 当前节点 */
	};

	/* ==================== 链表操作API ==================== */

	/**
	 * @brief 创建链表
	 * @param pool 内存池
	 * @param data_size 数据大小（0表示动态大小）
	 * @return 链表指针
	 */
	RECEPTOR_API receptor_list_t*
		receptor_list_create(receptor_pool_t *pool, size_t data_size);

	/**
	 * @brief 销毁链表
	 * @param list 链表指针
	 */
	RECEPTOR_API void
		receptor_list_destroy(receptor_list_t *list);

	/**
	 * @brief 在链表头部插入元素
	 * @param list 链表指针
	 * @param data 元素数据
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_push_front(receptor_list_t *list, const void *data);

	/**
	 * @brief 在链表尾部插入元素
	 * @param list 链表指针
	 * @param data 元素数据
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_push_back(receptor_list_t *list, const void *data);

	/**
	 * @brief 从链表头部弹出元素
	 * @param list 链表指针
	 * @param data 输出数据缓冲区
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_pop_front(receptor_list_t *list, void *data);

	/**
	 * @brief 从链表尾部弹出元素
	 * @param list 链表指针
	 * @param data 输出数据缓冲区
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_pop_back(receptor_list_t *list, void *data);

	/**
	 * @brief 在指定位置插入元素
	 * @param list 链表指针
	 * @param iter 迭代器位置
	 * @param data 元素数据
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_insert(receptor_list_t *list, receptor_list_iter_t *iter,
			const void *data);

	/**
	 * @brief 删除指定位置的元素
	 * @param list 链表指针
	 * @param iter 迭代器位置
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_erase(receptor_list_t *list, receptor_list_iter_t *iter);

	/**
	 * @brief 查找元素
	 * @param list 链表指针
	 * @param data 查找数据
	 * @param compar 比较函数
	 * @return 迭代器位置
	 */
	RECEPTOR_API receptor_list_iter_t
		receptor_list_find(receptor_list_t *list, const void *data,
			int(*compar)(const void *, const void *));

	/**
	 * @brief 获取链表大小
	 * @param list 链表指针
	 * @return 链表大小
	 */
	RECEPTOR_API receptor_uint_t
		receptor_list_size(receptor_list_t *list);

	/**
	 * @brief 检查链表是否为空
	 * @param list 链表指针
	 * @return 1为空，0为非空
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_empty(receptor_list_t *list);

	/**
	 * @brief 清空链表
	 * @param list 链表指针
	 */
	RECEPTOR_API void
		receptor_list_clear(receptor_list_t *list);

	/**
	 * @brief 链表排序
	 * @param list 链表指针
	 * @param compar 比较函数
	 */
	RECEPTOR_API void
		receptor_list_sort(receptor_list_t *list,
			int(*compar)(const void *, const void *));

	/* ==================== 迭代器操作API ==================== */

	/**
	 * @brief 获取链表头部迭代器
	 * @param list 链表指针
	 * @return 迭代器
	 */
	RECEPTOR_API receptor_list_iter_t
		receptor_list_begin(receptor_list_t *list);

	/**
	 * @brief 获取链表尾部迭代器
	 * @param list 链表指针
	 * @return 迭代器
	 */
	RECEPTOR_API receptor_list_iter_t
		receptor_list_end(receptor_list_t *list);

	/**
	 * @brief 迭代器前进
	 * @param iter 迭代器指针
	 * @return 前进后的迭代器
	 */
	RECEPTOR_API receptor_list_iter_t
		receptor_list_iter_next(receptor_list_iter_t *iter);

	/**
	 * @brief 迭代器后退
	 * @param iter 迭代器指针
	 * @return 后退后的迭代器
	 */
	RECEPTOR_API receptor_list_iter_t
		receptor_list_iter_prev(receptor_list_iter_t *iter);

	/**
	 * @brief 获取迭代器数据
	 * @param iter 迭代器
	 * @return 数据指针
	 */
	RECEPTOR_API void*
		receptor_list_iter_data(receptor_list_iter_t *iter);

	/**
	 * @brief 检查迭代器是否有效
	 * @param iter 迭代器
	 * @return 1有效，0无效
	 */
	RECEPTOR_API receptor_int_t
		receptor_list_iter_valid(receptor_list_iter_t *iter);

	/* ==================== 宏定义 ==================== */

	/**
	 * 链表遍历宏
	 */
#define receptor_list_foreach(list, type, var, code) \
    do { \
        receptor_list_iter_t __iter = receptor_list_begin(list); \
        while (receptor_list_iter_valid(&__iter)) { \
            type var = *(type*)receptor_list_iter_data(&__iter); \
            code \
            __iter = receptor_list_iter_next(&__iter); \
        } \
    } while(0)

	 /**
	  * 安全链表遍历宏（允许在循环中删除元素）
	  */
#define receptor_list_foreach_safe(list, type, var, code) \
    do { \
        receptor_list_iter_t __iter = receptor_list_begin(list); \
        receptor_list_iter_t __next; \
        while (receptor_list_iter_valid(&__iter)) { \
            __next = receptor_list_iter_next(&__iter); \
            type var = *(type*)receptor_list_iter_data(&__iter); \
            code \
            __iter = __next; \
        } \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif /* _RECEPTOR_LIST_H_ */