#include <receptor/def.h>
#include <receptor_event.h>

#ifdef __linux__
#include <sys/epoll.h>
#include <stdlib.h>

/* ==================== Epoll 事件模块实现 ==================== */

#define MAX_EVENTS 64

static int ep = -1;
static struct epoll_event *event_list;

static receptor_int_t
receptor_epoll_init(void)
{
	ep = epoll_create(1024);
	if (ep == -1) {
		return RECEPTOR_ERROR;
	}

	event_list = malloc(sizeof(struct epoll_event) * MAX_EVENTS);
	if (event_list == NULL) {
		close(ep);
		return RECEPTOR_ERROR;
	}

	return RECEPTOR_OK;
}

static receptor_int_t
receptor_epoll_add_event(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags)
{
	struct epoll_event  ee;

	(void)flags;

	ee.events = 0;
	ee.data.ptr = ev;

	if (event == 1) { /* read */
		ee.events = EPOLLIN;
	}
	else if (event == 2) { /* write */
		ee.events = EPOLLOUT;
	}

	if (epoll_ctl(ep, EPOLL_CTL_ADD, *(int *)ev->data, &ee) == -1) {
		return RECEPTOR_ERROR;
	}

	return RECEPTOR_OK;
}

static receptor_int_t
receptor_epoll_process_events(void)
{
	int                 events;
	receptor_event_t   *ev;
	struct epoll_event *ee;

	events = epoll_wait(ep, event_list, MAX_EVENTS, -1);

	if (events == -1) {
		return RECEPTOR_ERROR;
	}

	for (int i = 0; i < events; i++) {
		ee = &event_list[i];
		ev = ee->data.ptr;

		if (ee->events & EPOLLIN) {
			ev->handler(ev);
		}

		if (ee->events & EPOLLOUT) {
			ev->handler(ev);
		}
	}

	return RECEPTOR_OK;
}

static void
receptor_epoll_done(void)
{
	if (event_list) {
		free(event_list);
		event_list = NULL;
	}
	if (ep != -1) {
		close(ep);
		ep = -1;
	}
}

/* ==================== Epoll 事件操作结构 ==================== */

static const receptor_event_actions_t receptor_epoll_actions = {
	receptor_epoll_add_event,
	NULL, /* del */
	NULL, /* enable */
	NULL, /* disable */
	receptor_epoll_process_events,
	receptor_epoll_init,
	receptor_epoll_done
};

/* ==================== Epoll 模块注册函数 ==================== */

RECEPTOR_API void
receptor_event_epoll_register(void)
{
	receptor_event_set_actions(&receptor_epoll_actions);
}

#endif /* __linux__ */