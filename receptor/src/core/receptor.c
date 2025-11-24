#include <receptor/def.h>
#include "receptor.h"
#include "receptor_array.h"
#include "receptor_list.h"
#include "receptor_palloc.h"
#include "receptor_string.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

/* ==================== 内部数据结构 ==================== */

struct receptor_conf_ctx_s {
	receptor_pool_t*        pool;
	receptor_array_t*       modules;
	receptor_list_t*        configurations;
	void*                   main_conf;
	void*                   event_conf;
	void*                   http_conf;
	void*                   stream_conf;
};

struct receptor_core_conf_s {
	receptor_str_t          prefix;
	receptor_str_t          conf_file;
	receptor_str_t          error_log;
	receptor_uint_t         daemon;
	receptor_uint_t         master;
	receptor_uint_t         worker_processes;
	receptor_uint_t         debug_points;
};

struct receptor_cycle_s {
	receptor_pool_t*        pool;
	receptor_conf_ctx_t*    conf_ctx;
	receptor_core_conf_t*   conf;
	receptor_uint_t         running;
	receptor_uint_t         terminate;
	receptor_uint_t         quit;
	void*                   event_ctx;
	void*                   http_ctx;
	void*                   stream_ctx;
};

/* ==================== 全局变量 ==================== */

static receptor_uint_t      receptor_initialized = 0;
static receptor_pool_t*     receptor_global_pool = NULL;
static receptor_array_t*    receptor_modules = NULL;
static char                 receptor_error_buf[RECEPTOR_MAX_ERROR_STR];
static receptor_int_t       receptor_last_error = 0;

/* 内置核心模块声明 */
extern receptor_module_t receptor_core_module;
extern receptor_module_t receptor_event_module;
extern receptor_module_t receptor_http_module;
extern receptor_module_t receptor_stream_module;

/* 内置模块列表 - 修复：使用指针数组 */
static receptor_module_t* receptor_builtin_modules[] = {
	&receptor_core_module,
	&receptor_event_module,
	&receptor_http_module,
	&receptor_stream_module,
	NULL  /* 结束标记 */
};

/* ==================== 内部函数声明 ==================== */

static receptor_int_t receptor_internal_init(void);
static void receptor_internal_cleanup(void);
static receptor_int_t receptor_init_builtin_modules(void);
static receptor_int_t receptor_init_global_pool(void);

/* ==================== 内置模块定义 ==================== */

/* 核心模块 */
static receptor_int_t receptor_core_init(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("Core module initialized\n");
	return RECEPTOR_OK;
}

static receptor_int_t receptor_core_exit(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("Core module exited\n");
	return RECEPTOR_OK;
}

receptor_module_t receptor_core_module = {
	"core",
	NULL,
	receptor_core_init,
	receptor_core_exit
};

/* 事件模块 */
static receptor_int_t receptor_event_init(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("Event module initialized\n");
	return RECEPTOR_OK;
}

static receptor_int_t receptor_event_exit(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("Event module exited\n");
	return RECEPTOR_OK;
}

receptor_module_t receptor_event_module = {
	"event",
	NULL,
	receptor_event_init,
	receptor_event_exit
};

/* HTTP模块 */
static receptor_int_t receptor_http_init(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("HTTP module initialized\n");
	return RECEPTOR_OK;
}

static receptor_int_t receptor_http_exit(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("HTTP module exited\n");
	return RECEPTOR_OK;
}

receptor_module_t receptor_http_module = {
	"http",
	NULL,
	receptor_http_init,
	receptor_http_exit
};

/* 流模块 */
static receptor_int_t receptor_stream_init(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("Stream module initialized\n");
	return RECEPTOR_OK;
}

static receptor_int_t receptor_stream_exit(receptor_cycle_t* cycle) {
	(void)cycle;  // 避免未使用参数警告
	printf("Stream module exited\n");
	return RECEPTOR_OK;
}

receptor_module_t receptor_stream_module = {
	"stream",
	NULL,
	receptor_stream_init,
	receptor_stream_exit
};

/* ==================== 核心API实现 ==================== */

RECEPTOR_API receptor_int_t
receptor_init(void)
{
	if (receptor_initialized) {
		return RECEPTOR_OK;
	}

	/* 初始化错误缓冲区 */
	memset(receptor_error_buf, 0, sizeof(receptor_error_buf));
	receptor_last_error = 0;

	/* Windows Winsock 初始化 */
#ifdef _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		receptor_set_error("WSAStartup failed: %d", WSAGetLastError());
		return RECEPTOR_ERROR;
	}
#endif

	/* 初始化全局内存池 */
	if (receptor_init_global_pool() != RECEPTOR_OK) {
		receptor_set_error("Failed to initialize global memory pool");
		return RECEPTOR_ERROR;
	}

	/* 初始化模块系统 */
	if (receptor_init_builtin_modules() != RECEPTOR_OK) {
		receptor_set_error("Failed to initialize builtin modules");
		receptor_internal_cleanup();
		return RECEPTOR_ERROR;
	}

	/* 内部初始化 */
	if (receptor_internal_init() != RECEPTOR_OK) {
		receptor_set_error("Internal initialization failed");
		receptor_internal_cleanup();
		return RECEPTOR_ERROR;
	}

	receptor_initialized = 1;
	return RECEPTOR_OK;
}


RECEPTOR_API void
receptor_cleanup(void)
{
	if (!receptor_initialized) {
		return;
	}

	receptor_internal_cleanup();
	receptor_initialized = 0;
}

RECEPTOR_API const char*
receptor_version(void)
{
	return RECEPTOR_VERSION;
}

RECEPTOR_API receptor_uint_t
receptor_version_number(void)
{
	/* 版本号格式: 主版本 << 16 | 次版本 << 8 | 修订版本 */
	return (1 << 16) | (0 << 8) | 0;
}

/* ==================== 配置系统实现 ==================== */

RECEPTOR_API receptor_conf_ctx_t*
receptor_create_conf_ctx(void)
{
	receptor_conf_ctx_t* ctx;
	receptor_pool_t* pool;

	if (!receptor_initialized) {
		receptor_set_error("Receptor not initialized");
		return NULL;
	}

	pool = receptor_create_pool(4096);
	if (pool == NULL) {
		receptor_set_error("Failed to create configuration pool");
		return NULL;
	}

	ctx = receptor_pcalloc(pool, sizeof(receptor_conf_ctx_t));
	if (ctx == NULL) {
		receptor_destroy_pool(pool);
		receptor_set_error("Failed to allocate configuration context");
		return NULL;
	}

	ctx->pool = pool;

	/* 初始化模块数组 */
	ctx->modules = receptor_array_create(pool, 10, sizeof(receptor_module_t*));
	if (ctx->modules == NULL) {
		receptor_destroy_pool(pool);
		receptor_set_error("Failed to create modules array");
		return NULL;
	}

	/* 初始化配置列表 */
	ctx->configurations = receptor_list_create(pool, 10);
	if (ctx->configurations == NULL) {
		receptor_destroy_pool(pool);
		receptor_set_error("Failed to create configurations list");
		return NULL;
	}

	return ctx;
}

RECEPTOR_API void
receptor_destroy_conf_ctx(receptor_conf_ctx_t* ctx)
{
	if (ctx && ctx->pool) {
		receptor_destroy_pool(ctx->pool);
	}
}

RECEPTOR_API receptor_int_t
receptor_load_conf(receptor_conf_ctx_t* ctx, const char* filename)
{
	FILE* file;
	char buffer[1024];
	receptor_str_t line;

	if (!ctx || !filename) {
		receptor_set_error("Invalid arguments");
		return RECEPTOR_ERROR;
	}

	file = fopen(filename, "r");
	if (file == NULL) {
		receptor_set_error("Cannot open configuration file: %s", filename);
		return RECEPTOR_ERROR;
	}

	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		line.data = (u_char*)buffer;
		line.len = strlen(buffer);

		/* 移除换行符 */
		if (line.len > 0 && line.data[line.len - 1] == '\n') {
			line.data[--line.len] = '\0';
		}

		/* 跳过空行和注释 */
		if (line.len == 0 || line.data[0] == '#') {
			continue;
		}

		/* 这里添加配置解析逻辑 */
		/* 暂时只是简单打印 */
		printf("Config: %.*s\n", (int)line.len, line.data);
	}

	fclose(file);
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_parse_conf(receptor_conf_ctx_t* ctx, const char* config_str)
{
	if (!ctx || !config_str) {
		receptor_set_error("Invalid arguments");
		return RECEPTOR_ERROR;
	}

	/* 简单的配置字符串解析 */
	receptor_str_t config;
	config.data = (u_char*)config_str;
	config.len = strlen(config_str);

	printf("Parsing config: %.*s\n", (int)config.len, config.data);

	return RECEPTOR_OK;
}

/* ==================== 周期管理实现 ==================== */

RECEPTOR_API receptor_cycle_t*
receptor_init_cycle(receptor_conf_ctx_t* conf_ctx)
{
	receptor_cycle_t* cycle;
	receptor_pool_t* pool;

	if (!conf_ctx) {
		receptor_set_error("Invalid configuration context");
		return NULL;
	}

	pool = receptor_create_pool(8192);
	if (pool == NULL) {
		receptor_set_error("Failed to create cycle pool");
		return NULL;
	}

	cycle = receptor_pcalloc(pool, sizeof(receptor_cycle_t));
	if (cycle == NULL) {
		receptor_destroy_pool(pool);
		receptor_set_error("Failed to allocate cycle");
		return NULL;
	}

	cycle->pool = pool;
	cycle->conf_ctx = conf_ctx;
	cycle->running = 0;
	cycle->terminate = 0;
	cycle->quit = 0;

	/* 初始化核心配置 */
	cycle->conf = receptor_pcalloc(pool, sizeof(receptor_core_conf_t));
	if (cycle->conf == NULL) {
		receptor_destroy_pool(pool);
		receptor_set_error("Failed to allocate core configuration");
		return NULL;
	}

	/* 设置默认值 */
	cycle->conf->worker_processes = 1;
	cycle->conf->daemon = 0;
	cycle->conf->master = 0;
	cycle->conf->debug_points = 0;

	return cycle;
}

RECEPTOR_API void
receptor_destroy_cycle(receptor_cycle_t* cycle)
{
	if (cycle && cycle->pool) {
		receptor_destroy_pool(cycle->pool);
	}
}

RECEPTOR_API receptor_core_conf_t*
receptor_get_core_conf(receptor_cycle_t* cycle)
{
	if (!cycle) {
		receptor_set_error("Invalid cycle");
		return NULL;
	}

	return cycle->conf;
}

/* ==================== 模块管理实现 ==================== */

RECEPTOR_API receptor_int_t
receptor_register_module(receptor_module_t* module)
{
	if (!module || !module->name) {
		receptor_set_error("Invalid module");
		return RECEPTOR_ERROR;
	}

	if (!receptor_modules) {
		receptor_set_error("Module system not initialized");
		return RECEPTOR_ERROR;
	}

	/* 检查是否已注册 */
	receptor_uint_t i;
	receptor_module_t** mods = (receptor_module_t**)receptor_modules->elts;

	for (i = 0; i < receptor_modules->nelts; i++) {
		if (strcmp(mods[i]->name, module->name) == 0) {
			receptor_set_error("Module already registered: %s", module->name);
			return RECEPTOR_ERROR;
		}
	}

	/* 注册模块 */
	receptor_module_t** slot = (receptor_module_t**)receptor_array_push(receptor_modules);
	if (slot == NULL) {
		receptor_set_error("Failed to register module: %s", module->name);
		return RECEPTOR_ERROR;
	}

	*slot = module;
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_module_t*
receptor_find_module(const char* name)
{
	if (!name || !receptor_modules) {
		return NULL;
	}

	receptor_uint_t i;
	receptor_module_t** mods = (receptor_module_t**)receptor_modules->elts;

	for (i = 0; i < receptor_modules->nelts; i++) {
		if (strcmp(mods[i]->name, name) == 0) {
			return mods[i];
		}
	}

	return NULL;
}

RECEPTOR_API receptor_int_t
receptor_init_modules(receptor_cycle_t* cycle)
{
	if (!cycle || !receptor_modules) {
		receptor_set_error("Invalid arguments");
		return RECEPTOR_ERROR;
	}

	receptor_uint_t i;
	receptor_module_t** mods = (receptor_module_t**)receptor_modules->elts;
	receptor_int_t rc;

	for (i = 0; i < receptor_modules->nelts; i++) {
		if (mods[i]->init_module) {
			rc = mods[i]->init_module(cycle);
			if (rc != RECEPTOR_OK) {
				receptor_set_error("Failed to initialize module: %s", mods[i]->name);
				return RECEPTOR_ERROR;
			}
		}
	}

	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_exit_modules(receptor_cycle_t* cycle)
{
	if (!cycle || !receptor_modules) {
		return RECEPTOR_ERROR;
	}

	receptor_uint_t i;
	receptor_module_t** mods = (receptor_module_t**)receptor_modules->elts;
	receptor_int_t rc;

	/* 反向退出模块 */
	for (i = receptor_modules->nelts; i > 0; i--) {
		if (mods[i - 1]->exit_module) {
			rc = mods[i - 1]->exit_module(cycle);
			if (rc != RECEPTOR_OK) {
				receptor_set_error("Failed to exit module: %s", mods[i - 1]->name);
			}
		}
	}

	return RECEPTOR_OK;
}

/* ==================== 运行时控制实现 ==================== */

RECEPTOR_API receptor_int_t
receptor_start(receptor_cycle_t* cycle)
{
	if (!cycle) {
		receptor_set_error("Invalid cycle");
		return RECEPTOR_ERROR;
	}

	if (cycle->running) {
		receptor_set_error("Receptor already running");
		return RECEPTOR_ERROR;
	}

	/* 初始化所有模块 */
	if (receptor_init_modules(cycle) != RECEPTOR_OK) {
		receptor_set_error("Failed to initialize modules");
		return RECEPTOR_ERROR;
	}

	cycle->running = 1;
	cycle->terminate = 0;
	cycle->quit = 0;

	printf("Receptor started successfully\n");
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_stop(receptor_cycle_t* cycle)
{
	if (!cycle) {
		receptor_set_error("Invalid cycle");
		return RECEPTOR_ERROR;
	}

	if (!cycle->running) {
		receptor_set_error("Receptor not running");
		return RECEPTOR_ERROR;
	}

	cycle->running = 0;
	cycle->terminate = 1;

	/* 退出所有模块 */
	receptor_exit_modules(cycle);

	printf("Receptor stopped\n");
	return RECEPTOR_OK;
}

RECEPTOR_API receptor_int_t
receptor_reload(receptor_cycle_t* cycle)
{
	if (!cycle) {
		receptor_set_error("Invalid cycle");
		return RECEPTOR_ERROR;
	}

	printf("Reloading configuration...\n");

	/* 这里实现配置重载逻辑 */

	return RECEPTOR_OK;
}

/* ==================== 工具函数实现 ==================== */

RECEPTOR_API const char*
receptor_get_error(void)
{
	return receptor_error_buf;
}

RECEPTOR_API void
receptor_set_error(const char* format, ...)
{
	va_list args;
	va_start(args, format);

#ifdef _WIN32
	_vsnprintf_s(receptor_error_buf, RECEPTOR_MAX_ERROR_STR, _TRUNCATE, format, args);
#else
	vsnprintf(receptor_error_buf, RECEPTOR_MAX_ERROR_STR, format, args);
#endif

	va_end(args);
}

RECEPTOR_API receptor_int_t
receptor_get_last_error(void)
{
	return receptor_last_error;
}

/* ==================== 信号处理实现 ==================== */

RECEPTOR_API receptor_int_t
receptor_init_signals(void)
{
#ifdef _WIN32
	/* Windows 信号处理 */
	SetConsoleCtrlHandler(NULL, FALSE);
	return RECEPTOR_OK;
#else
	/* Unix 信号处理 */
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	/* 忽略某些信号 */
	sigaction(SIGPIPE, &sa, NULL);
	return RECEPTOR_OK;
#endif
}

RECEPTOR_API receptor_int_t
receptor_add_signal(int signum, void(*handler)(int))
{
	/* 暂时不实现具体信号处理 */
	(void)signum;
	(void)handler;
	return RECEPTOR_OK;
}

/* ==================== 内部函数实现 ==================== */

static receptor_int_t
receptor_internal_init(void)
{
	/* 初始化信号处理 */
	if (receptor_init_signals() != RECEPTOR_OK) {
		return RECEPTOR_ERROR;
	}

	return RECEPTOR_OK;
}

static void
receptor_internal_cleanup(void)
{
	/* 清理全局内存池 */
	if (receptor_global_pool) {
		receptor_destroy_pool(receptor_global_pool);
		receptor_global_pool = NULL;
	}

	/* 清理模块数组 */
	if (receptor_modules) {
		receptor_array_destroy(receptor_modules);
		receptor_modules = NULL;
	}

	/* Windows 清理 */
#ifdef _WIN32
	WSACleanup();
#endif
}

static receptor_int_t
receptor_init_global_pool(void)
{
	receptor_global_pool = receptor_create_pool(16384);
	if (receptor_global_pool == NULL) {
		return RECEPTOR_ERROR;
	}

	/* 初始化全局模块数组 */
	receptor_modules = receptor_array_create(receptor_global_pool, 10, sizeof(receptor_module_t*));
	if (receptor_modules == NULL) {
		receptor_destroy_pool(receptor_global_pool);
		receptor_global_pool = NULL;
		return RECEPTOR_ERROR;
	}

	return RECEPTOR_OK;
}

static receptor_int_t
receptor_init_builtin_modules(void)
{
	receptor_module_t** mod;
	receptor_uint_t i;

	for (i = 0; receptor_builtin_modules[i] != NULL; i++) {
		mod = &receptor_builtin_modules[i];  // 修复：取地址

		if (receptor_register_module(*mod) != RECEPTOR_OK) {  // 修复：解引用
			receptor_set_error("Failed to register builtin module: %s", (*mod)->name);
			return RECEPTOR_ERROR;
		}
	}

	return RECEPTOR_OK;
}
