#ifndef _RECEPTOR_EVENT_H_
#define _RECEPTOR_EVENT_H_

#include "../include/receptor_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* ==================== 事件结构定义 ==================== */

	typedef struct receptor_event_s       receptor_event_t;
	typedef void(*receptor_event_handler_pt)(receptor_event_t *ev);

	struct receptor_event_s {
		void                    *data;
		receptor_event_handler_pt   handler;
		receptor_uint_t         write : 1;
		receptor_uint_t         active : 1;
		receptor_uint_t         ready : 1;
	};

	/* ==================== 事件操作接口 ==================== */

	typedef struct {
		receptor_int_t(*add)(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags);
		receptor_int_t(*del)(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags);
		receptor_int_t(*enable)(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags);
		receptor_int_t(*disable)(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags);
		receptor_int_t(*process_events)(void);
		receptor_int_t(*init)(void);
		void(*done)(void);
	} receptor_event_actions_t;

	/* ==================== 全局事件操作声明 ==================== */

	/* 使用 extern 声明，在事件模块中定义 */
	extern RECEPTOR_API receptor_event_actions_t receptor_event_actions;

	/* ==================== 事件API函数 ==================== */

	RECEPTOR_API receptor_int_t receptor_event_init(void);
	RECEPTOR_API receptor_int_t receptor_event_add(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags);
	RECEPTOR_API receptor_int_t receptor_event_del(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags);
	RECEPTOR_API receptor_int_t receptor_event_process(void);
	RECEPTOR_API void receptor_event_done(void);

	/* ==================== 事件操作设置函数 ==================== */

	RECEPTOR_API void receptor_event_set_actions(const receptor_event_actions_t *actions);

	/* ==================== 事件模块注册函数声明 ==================== */

	/* Windows IOCP 模块注册 */
#ifdef _WIN32
	RECEPTOR_API void receptor_event_iocp_register(void);
#endif

	/* Linux epoll 模块注册 */
#ifdef __linux__
	RECEPTOR_API void receptor_event_epoll_register(void);
#endif

	/* 跨平台 select 模块注册 */
	RECEPTOR_API void receptor_event_select_register(void);

	/* ==================== 事件模块自动初始化 ==================== */

	RECEPTOR_API receptor_int_t receptor_event_module_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _RECEPTOR_EVENT_H_ */