#ifndef _RECEPTOR_ARRAY_H_
#define _RECEPTOR_ARRAY_H_

#include "../include/receptor_defs.h"
#include "receptor_palloc.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* ==================== 数组结构定义 ==================== */

	/**
	 * 动态数组结构
	 * 仿照nginx的数组设计，支持动态扩容
	 */
	typedef struct receptor_array_s receptor_array_t;

	struct receptor_array_s {
		void        *elts;      /* 数组元素指针 */
		receptor_uint_t   nelts;     /* 当前元素数量 */
		size_t       size;      /* 单个元素大小 */
		receptor_uint_t   nalloc;    /* 分配的元素数量 */
		receptor_pool_t  *pool;      /* 内存池 */
	};

	/* ==================== 数组操作API ==================== */

	/**
	 * @brief 创建动态数组
	 * @param pool 内存池
	 * @param n 初始容量
	 * @param size 元素大小
	 * @return 数组指针
	 */
	RECEPTOR_API receptor_array_t*
		receptor_array_create(receptor_pool_t *pool, receptor_uint_t n, size_t size);

	/**
	 * @brief 销毁数组
	 * @param array 数组指针
	 */
	RECEPTOR_API void
		receptor_array_destroy(receptor_array_t *array);

	/**
	 * @brief 向数组添加元素
	 * @param array 数组指针
	 * @return 新元素指针，失败返回NULL
	 */
	RECEPTOR_API void*
		receptor_array_push(receptor_array_t *array);

	/**
	 * @brief 向数组添加多个元素
	 * @param array 数组指针
	 * @param n 元素数量
	 * @return 第一个新元素指针，失败返回NULL
	 */
	RECEPTOR_API void*
		receptor_array_push_n(receptor_array_t *array, receptor_uint_t n);

	/**
	 * @brief 从数组弹出元素
	 * @param array 数组指针
	 * @return 弹出的元素指针
	 */
	RECEPTOR_API void*
		receptor_array_pop(receptor_array_t *array);

	/**
	 * @brief 获取指定索引的元素
	 * @param array 数组指针
	 * @param index 索引位置
	 * @return 元素指针
	 */
	RECEPTOR_API void*
		receptor_array_get(receptor_array_t *array, receptor_uint_t index);

	/**
	 * @brief 设置指定索引的元素
	 * @param array 数组指针
	 * @param index 索引位置
	 * @param value 元素值
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_array_set(receptor_array_t *array, receptor_uint_t index, const void *value);

	/**
	 * @brief 在指定位置插入元素
	 * @param array 数组指针
	 * @param index 插入位置
	 * @param value 元素值
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_array_insert(receptor_array_t *array, receptor_uint_t index, const void *value);

	/**
	 * @brief 删除指定位置的元素
	 * @param array 数组指针
	 * @param index 删除位置
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_array_delete(receptor_array_t *array, receptor_uint_t index);

	/**
	 * @brief 清空数组
	 * @param array 数组指针
	 */
	RECEPTOR_API void
		receptor_array_clear(receptor_array_t *array);

	/**
	 * @brief 获取数组元素数量
	 * @param array 数组指针
	 * @return 元素数量
	 */
	RECEPTOR_API receptor_uint_t
		receptor_array_nelts(receptor_array_t *array);

	/**
	 * @brief 获取数组容量
	 * @param array 数组指针
	 * @return 数组容量
	 */
	RECEPTOR_API receptor_uint_t
		receptor_array_nalloc(receptor_array_t *array);

	/**
	 * @brief 检查数组是否为空
	 * @param array 数组指针
	 * @return 1为空，0为非空
	 */
	RECEPTOR_API receptor_int_t
		receptor_array_empty(receptor_array_t *array);

	/**
	 * @brief 数组排序
	 * @param array 数组指针
	 * @param compar 比较函数
	 */
	RECEPTOR_API void
		receptor_array_sort(receptor_array_t *array,
			int(*compar)(const void *, const void *));

	/**
	 * @brief 数组查找
	 * @param array 数组指针
	 * @param key 查找键
	 * @param compar 比较函数
	 * @return 元素指针，未找到返回NULL
	 */
	RECEPTOR_API void*
		receptor_array_find(receptor_array_t *array, const void *key,
			int(*compar)(const void *, const void *));

	/**
	 * @brief 数组遍历
	 * @param array 数组指针
	 * @param func 遍历函数
	 * @param data 用户数据
	 */
	RECEPTOR_API void
		receptor_array_foreach(receptor_array_t *array,
			void(*func)(void *element, void *data),
			void *data);

	/* ==================== 宏定义 ==================== */

	/**
	 * 快速访问数组元素的宏
	 */


#define receptor_array_elts(array)      ((array)->elts)
#define receptor_array_nelts(array)     ((array)->nelts)
#define receptor_array_size(array)      ((array)->size)

	 /**
	  * 数组遍历宏
	  */
#define receptor_array_foreach_element(array, type, var, code) \
    do { \
        receptor_uint_t __i; \
        type *__elts = (type*)((array)->elts); \
        for (__i = 0; __i < (array)->nelts; __i++) { \
            type var = __elts[__i]; \
            code \
        } \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif /* _RECEPTOR_ARRAY_H_ */