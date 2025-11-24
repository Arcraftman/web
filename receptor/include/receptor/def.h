#ifndef _RECEPTOR_DEFS_H_
#define _RECEPTOR_DEFS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

/* 包含自动生成的配置头文件 */
#include <receptor/config.h>

/* 版本信息 */
#ifndef RECEPTOR_VERSION
#define RECEPTOR_VERSION "1.0.0"
#endif

#define RECEPTOR_VER "receptor/" RECEPTOR_VERSION

#define RECEPTOR_OK 0
#define RECEPTOR_ERROR -1
#define RECEPTOR_AGAIN -2
#define RECEPTOR_DONE -3

#define RECEPTOR_MAX_ERROR_STR 2048

/* Windows 兼容性类型定义 */
#ifdef _WIN32
typedef INT_PTR             receptor_int_t;
typedef UINT_PTR            receptor_uint_t;
typedef SOCKET              receptor_socket_t;
#define RECEPTOR_INVALID_SOCKET INVALID_SOCKET
#else
typedef intptr_t            receptor_int_t;
typedef uintptr_t           receptor_uint_t;
typedef int                 receptor_socket_t;
#define RECEPTOR_INVALID_SOCKET (-1)
#endif

/* DLL 导出修饰 - 修复静态库构建问题 */
#if defined(_WIN32) && defined(RECEPTOR_DLL_EXPORT)
	/* 构建 DLL 时导出 */
#define RECEPTOR_API __declspec(dllexport)
#elif defined(_WIN32) && defined(RECEPTOR_USE_DLL)
	/* 使用 DLL 时导入 */
#define RECEPTOR_API __declspec(dllimport)
#else
	/* 静态库或其他平台 */
#define RECEPTOR_API
#endif

/* 时间类型 */
#ifdef _WIN32
typedef LARGE_INTEGER       receptor_time_t;
#else
typedef struct timeval      receptor_time_t;
#endif

typedef receptor_int_t      receptor_msec_t;
typedef receptor_int_t      receptor_sec_t;

#endif /* _RECEPTOR_DEFS_H_ */