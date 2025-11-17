#include "receptor_event.h"
#include "../include/receptor_defs.h"

/* ==================== 全局事件操作定义 ==================== */

/* 在这里定义全局变量，而不是在各个模块中 */
receptor_event_actions_t receptor_event_actions = {
	NULL, /* add */
	NULL, /* del */
	NULL, /* enable */
	NULL, /* disable */
	NULL, /* process_events */
	NULL, /* init */
	NULL  /* done */
};

/* ==================== 事件API实现 ==================== */

RECEPTOR_API receptor_int_t
receptor_event_init(void)
{
	if (receptor_event_actions.init) {
		return receptor_event_actions.init();
	}
	return RECEPTOR_ERROR;
}

RECEPTOR_API receptor_int_t
receptor_event_add(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags)
{
	if (receptor_event_actions.add) {
		return receptor_event_actions.add(ev, event, flags);
	}
	return RECEPTOR_ERROR;
}

RECEPTOR_API receptor_int_t
receptor_event_del(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags)
{
	if (receptor_event_actions.del) {
		return receptor_event_actions.del(ev, event, flags);
	}
	return RECEPTOR_ERROR;
}

RECEPTOR_API receptor_int_t
receptor_event_process(void)
{
	if (receptor_event_actions.process_events) {
		return receptor_event_actions.process_events();
	}
	return RECEPTOR_ERROR;
}

RECEPTOR_API void
receptor_event_done(void)
{
	if (receptor_event_actions.done) {
		receptor_event_actions.done();
	}
}

/* ==================== 事件操作设置函数 ==================== */

RECEPTOR_API void
receptor_event_set_actions(const receptor_event_actions_t *actions)
{
	if (actions) {
		receptor_event_actions = *actions;
	}
}