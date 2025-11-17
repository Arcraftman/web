#ifndef _RECEPTOR_OS_WIN32_H_
#define _RECEPTOR_OS_WIN32_H_

#include "../../include/receptor_defs.h"

#ifdef _WIN32

/* Windows 特定系统调用 */
RECEPTOR_API receptor_int_t receptor_win32_get_last_error(void);
RECEPTOR_API void receptor_win32_set_last_error(receptor_int_t error);
RECEPTOR_API const char* receptor_win32_error_string(receptor_int_t error);

/* 进程和线程 */
RECEPTOR_API HANDLE receptor_win32_create_thread(LPTHREAD_START_ROUTINE start, LPVOID param);
RECEPTOR_API void receptor_win32_close_handle(HANDLE handle);

/* 系统时间 */
RECEPTOR_API receptor_int_t receptor_win32_get_system_time(void);
RECEPTOR_API receptor_int_t receptor_win32_sleep(receptor_int_t milliseconds);

/* 进程信息 */
RECEPTOR_API receptor_int_t receptor_win32_get_process_id(void);
RECEPTOR_API receptor_int_t receptor_win32_is_debugger_present(void);

#endif /* _WIN32 */

#endif /* _RECEPTOR_OS_WIN32_H_ */