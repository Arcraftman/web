#include "receptor_event.h"
#include "../include/receptor_defs.h"
#include <stdlib.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/* ==================== Select 事件模块实现 ==================== */

static receptor_int_t
receptor_select_init(void)
{
	return RECEPTOR_OK;
}

static receptor_int_t
receptor_select_add_event(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags)
{
	(void)ev;
	(void)event;
	(void)flags;
	return RECEPTOR_OK;
}

static receptor_int_t
receptor_select_process_events(void)
{
	/* 简单的 select 实现 */
#ifdef _WIN32
	Sleep(100);  /* Windows 休眠 */
#else
	usleep(100000);  /* Unix 休眠 100ms */
#endif
	return RECEPTOR_OK;
}

static void
receptor_select_done(void)
{
	/* 清理资源 */
}

/* ==================== Select 事件操作结构 ==================== */

static const receptor_event_actions_t receptor_select_actions = {
	receptor_select_add_event,
	NULL, /* del */
	NULL, /* enable */
	NULL, /* disable */
	receptor_select_process_events,
	receptor_select_init,
	receptor_select_done
};

/* ==================== Select 模块注册函数 ==================== */

RECEPTOR_API void
receptor_event_select_register(void)
{
	receptor_event_set_actions(&receptor_select_actions);
}