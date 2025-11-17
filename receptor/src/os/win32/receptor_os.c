#include "receptor_os.h"
#include "../../include/receptor_defs.h"
#include "../../core/receptor_palloc.h"
#include <stdio.h>

#ifdef _WIN32

RECEPTOR_API receptor_int_t
receptor_win32_get_last_error(void)
{
	return (receptor_int_t)GetLastError();
}

RECEPTOR_API void
receptor_win32_set_last_error(receptor_int_t error)
{
	SetLastError((DWORD)error);
}

RECEPTOR_API const char*
receptor_win32_error_string(receptor_int_t error)
{
	static char buffer[1024];
	DWORD len;

	len = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		(DWORD)error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buffer,
		sizeof(buffer),
		NULL
	);

	if (len == 0) {
		_snprintf_s(buffer, sizeof(buffer), _TRUNCATE, "Unknown error %lld", error);
	}

	return buffer;
}

RECEPTOR_API HANDLE
receptor_win32_create_thread(LPTHREAD_START_ROUTINE start, LPVOID param)
{
	return CreateThread(NULL, 0, start, param, 0, NULL);
}

RECEPTOR_API void
receptor_win32_close_handle(HANDLE handle)
{
	if (handle && handle != INVALID_HANDLE_VALUE) {
		CloseHandle(handle);
	}
}

RECEPTOR_API receptor_int_t
receptor_win32_get_system_time(void)
{
	return (receptor_int_t)GetTickCount();
}

RECEPTOR_API receptor_int_t
receptor_win32_sleep(receptor_int_t milliseconds)
{
	Sleep((DWORD)milliseconds);
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_win32_get_process_id(void)
{
	return (receptor_int_t)GetCurrentProcessId();
}

RECEPTOR_API receptor_int_t
receptor_win32_is_debugger_present(void)
{
	return IsDebuggerPresent() ? 1 : 0;
}

#endif /* _WIN32 */