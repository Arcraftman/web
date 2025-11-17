#include "../receptor_event.h"

#ifdef _WIN32

#include <windows.h>

/* ==================== IOCP 事件模块实现 ==================== */

static HANDLE iocp_port = NULL;

static receptor_int_t
receptor_iocp_init(void)
{
	iocp_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (iocp_port == NULL) {
		return RECEPTOR_ERROR;
	}
	return RECEPTOR_OK;
}

static receptor_int_t
receptor_iocp_add_event(receptor_event_t *ev, receptor_int_t event, receptor_uint_t flags)
{
	/* IOCP 实现 */
	(void)ev;
	(void)event;
	(void)flags;
	return RECEPTOR_OK;
}

static receptor_int_t
receptor_iocp_process_events(void)
{
	DWORD bytes_transferred;
	ULONG_PTR completion_key;
	LPOVERLAPPED overlapped;

	if (iocp_port == NULL) {
		return RECEPTOR_ERROR;
	}

	BOOL result = GetQueuedCompletionStatus(
		iocp_port,
		&bytes_transferred,
		&completion_key,
		&overlapped,
		INFINITE
	);

	if (result && completion_key) {
		receptor_event_t *ev = (receptor_event_t *)completion_key;
		if (ev->handler) {
			ev->handler(ev);
		}
	}

	return RECEPTOR_OK;
}

static void
receptor_iocp_done(void)
{
	if (iocp_port) {
		CloseHandle(iocp_port);
		iocp_port = NULL;
	}
}

/* ==================== IOCP 事件操作结构 ==================== */

static const receptor_event_actions_t receptor_iocp_actions = {
	receptor_iocp_add_event,
	NULL, /* del */
	NULL, /* enable */
	NULL, /* disable */
	receptor_iocp_process_events,
	receptor_iocp_init,
	receptor_iocp_done
};

/* ==================== IOCP 模块注册函数 ==================== */

RECEPTOR_API void
receptor_event_iocp_register(void)
{
	receptor_event_set_actions(&receptor_iocp_actions);
}

#endif /* _WIN32 */