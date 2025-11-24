#ifndef _RECEPTOR_CONFIG_H_
#define _RECEPTOR_CONFIG_H_

/* ===========================================================================
 * receptor_config.h - 自动生成的配置头文件
 *
 * 此文件由 CMake 根据系统配置自动生成
 * 不要手动编辑此文件！
 * =========================================================================== */

 /* ==================== 版本信息 ==================== */
#define RECEPTOR_VERSION_MAJOR      1
#define RECEPTOR_VERSION_MINOR      0
#define RECEPTOR_VERSION_PATCH      0
#define RECEPTOR_VERSION            "1.0.0"
#define RECEPTOR_VERSION_STRING     "receptor/1.0.0"

/* ==================== 构建配置 ==================== */
#define RECEPTOR_BUILD_STATIC
/* #undef RECEPTOR_BUILD_SHARED */
/* #undef RECEPTOR_BUILD_DLL */
/* #undef RECEPTOR_BUILD_EXECUTABLE */

/* ==================== 平台检测 ==================== */
/* #undef RECEPTOR_PLATFORM_UNIX */
/* #undef RECEPTOR_PLATFORM_LINUX */
/* #undef RECEPTOR_PLATFORM_MACOS */
/* #undef RECEPTOR_PLATFORM_CYGWIN */
/* #undef RECEPTOR_PLATFORM_MINGW */

/* ==================== 架构检测 ==================== */
/* #undef RECEPTOR_ARCH_X86 */
#define RECEPTOR_ARCH_X64
/* #undef RECEPTOR_ARCH_ARM */
/* #undef RECEPTOR_ARCH_ARM64 */

/* ==================== 编译器检测 ==================== */
#define RECEPTOR_COMPILER_MSVC
/* #undef RECEPTOR_COMPILER_GCC */
/* #undef RECEPTOR_COMPILER_CLANG */
/* #undef RECEPTOR_COMPILER_MINGW */

#define RECEPTOR_COMPILER_VERSION_MAJOR 19
#define RECEPTOR_COMPILER_VERSION_MINOR 0
#define RECEPTOR_COMPILER_VERSION       "19.00.00000"

/* ==================== 事件模型支持 ==================== */
/* #undef RECEPTOR_HAVE_EPOLL */
/* #undef RECEPTOR_HAVE_KQUEUE */
#define RECEPTOR_HAVE_IOCP
#define RECEPTOR_HAVE_SELECT
/* #undef RECEPTOR_HAVE_POLL */
/* #undef RECEPTOR_HAVE_DEVPOLL */

/* ==================== 模块支持 ==================== */
#define RECEPTOR_HTTP_MODULE
#define RECEPTOR_STREAM_MODULE
#define RECEPTOR_EVENT_MODULE
#define RECEPTOR_CORE_MODULE
#define RECEPTOR_OS_MODULE

/* ==================== 特性支持 ==================== */
/* #undef RECEPTOR_HAVE_OPENSSL */
/* #undef RECEPTOR_HAVE_ZLIB */
/* #undef RECEPTOR_HAVE_PCRE */
/* #undef RECEPTOR_HAVE_PCRE2 */
/* #undef RECEPTOR_HAVE_ICONV */
/* #undef RECEPTOR_HAVE_GD */
/* #undef RECEPTOR_HAVE_XML2 */

/* ==================== 系统特性 ==================== */
/* #undef RECEPTOR_HAVE_SENDFILE */
/* #undef RECEPTOR_HAVE_MMAP */
/* #undef RECEPTOR_HAVE_MALLOC_TRIM */
/* #undef RECEPTOR_HAVE_POSIX_MEMALIGN */
/* #undef RECEPTOR_HAVE_MEMALIGN */
/* #undef RECEPTOR_HAVE_VALGRIND */

/* ==================== 网络特性 ==================== */
/* #undef RECEPTOR_HAVE_IPV6 */
/* #undef RECEPTOR_HAVE_TCP_FASTOPEN */
/* #undef RECEPTOR_HAVE_TCP_DEFER_ACCEPT */
/* #undef RECEPTOR_HAVE_ACCEPT4 */
/* #undef RECEPTOR_HAVE_SO_REUSEPORT */

/* ==================== 线程支持 ==================== */
/* #undef RECEPTOR_HAVE_PTHREAD */
/* #undef RECEPTOR_HAVE_PTHREAD_AFFINITY */
/* #undef RECEPTOR_HAVE_PTHREAD_BARRIER */
/* #undef RECEPTOR_HAVE_PTHREAD_SPINLOCK */

/* ==================== 原子操作支持 ==================== */
/* #undef RECEPTOR_HAVE_ATOMIC_BUILTINS */
/* #undef RECEPTOR_HAVE_SYNC_BUILTINS */
/* #undef RECEPTOR_HAVE_ATOMIC_OPS */

/* ==================== 编译器特性 ==================== */
/* #undef RECEPTOR_HAVE_VISIBILITY */
/* #undef RECEPTOR_HAVE_ATTRIBUTE_ALIGNED */
/* #undef RECEPTOR_HAVE_ATTRIBUTE_ALWAYS_INLINE */
/* #undef RECEPTOR_HAVE_ATTRIBUTE_NOINLINE */
/* #undef RECEPTOR_HAVE_ATTRIBUTE_NORETURN */
/* #undef RECEPTOR_HAVE_ATTRIBUTE_UNUSED */
/* #undef RECEPTOR_HAVE_ATTRIBUTE_FORMAT */
/* #undef RECEPTOR_HAVE_BUILTIN_EXPECT */
/* #undef RECEPTOR_HAVE_BUILTIN_UNREACHABLE */

/* ==================== 调试支持 ==================== */
/* #undef RECEPTOR_DEBUG */
/* #undef RECEPTOR_DEBUG_ALLOC */
/* #undef RECEPTOR_DEBUG_EVENT */
/* #undef RECEPTOR_DEBUG_HTTP */
/* #undef RECEPTOR_DEBUG_STREAM */

/* ==================== 安装路径 ==================== */
#define RECEPTOR_PREFIX              "C:/Program Files/receptor"
#define RECEPTOR_BINDIR              "bin"
#define RECEPTOR_LIBDIR              "lib"
#define RECEPTOR_INCLUDEDIR          "include"
#define RECEPTOR_SYSCONFDIR          "etc"

/* ==================== 文件路径 ==================== */
#define RECEPTOR_CONF_PATH           "C:/Program Files/receptor/etc/receptor/receptor.conf"
#define RECEPTOR_PID_PATH            "C:/Program Files/receptor/var/run/receptor.pid"
#define RECEPTOR_LOCK_PATH           "C:/Program Files/receptor/var/lock/receptor.lock"
#define RECEPTOR_LOG_PATH            "C:/Program Files/receptor/var/log/receptor.log"

/* ==================== 默认配置 ==================== */
#define RECEPTOR_DEFAULT_PORT        8080
#define RECEPTOR_DEFAULT_BACKLOG     511
#define RECEPTOR_DEFAULT_WORKERS     1
#define RECEPTOR_DEFAULT_TIMEOUT     30000
#define RECEPTOR_MAX_HEADER_SIZE     8192
#define RECEPTOR_MAX_BODY_SIZE       10485760  /* 10MB */

/* ==================== 特性宏定义 ==================== */

/* 内联函数支持 */
#if defined(RECEPTOR_HAVE_ATTRIBUTE_ALWAYS_INLINE)
#define RECEPTOR_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define RECEPTOR_INLINE __forceinline
#else
#define RECEPTOR_INLINE inline
#endif

/* 其余宏定义保持不变... */

#endif /* _RECEPTOR_CONFIG_H_ */