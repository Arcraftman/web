
#ifndef _RECEPTOR_H_
#define _RECEPTOR_H_

#include "../include/receptor_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* ==================== 核心类型定义 ==================== */

	/* 配置上下文 */
	typedef struct receptor_conf_ctx_s receptor_conf_ctx_t;

	/* 模块结构 */
	typedef struct receptor_module_s receptor_module_t;

	/* 核心配置 */
	typedef struct receptor_core_conf_s receptor_core_conf_t;

	/* 周期结构（类似nginx的cycle） */
	typedef struct receptor_cycle_s receptor_cycle_t;

	/* ==================== 模块系统 ==================== */

	struct receptor_module_s {
		const char* name;
		void*       ctx;
		receptor_int_t(*init_module)(receptor_cycle_t *cycle);
		receptor_int_t(*exit_module)(receptor_cycle_t *cycle);
	};

	/* ==================== 核心API ==================== */

	/**
	 * @brief 初始化receptor库
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_init(void);

	/**
	 * @brief 清理receptor库资源
	 */
	RECEPTOR_API void receptor_cleanup(void);

	/**
	 * @brief 获取版本字符串
	 * @return 版本字符串
	 */
	RECEPTOR_API const char* receptor_version(void);

	/**
	 * @brief 获取版本号
	 * @return 版本号 (主版本 << 16 | 次版本 << 8 | 修订版本)
	 */
	RECEPTOR_API receptor_uint_t receptor_version_number(void);

	/* ==================== 配置系统 ==================== */

	/**
	 * @brief 创建配置上下文
	 * @return 配置上下文指针
	 */
	RECEPTOR_API receptor_conf_ctx_t* receptor_create_conf_ctx(void);

	/**
	 * @brief 销毁配置上下文
	 * @param ctx 配置上下文
	 */
	RECEPTOR_API void receptor_destroy_conf_ctx(receptor_conf_ctx_t* ctx);

	/**
	 * @brief 从文件加载配置
	 * @param ctx 配置上下文
	 * @param filename 配置文件路径
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_load_conf(receptor_conf_ctx_t* ctx, const char* filename);

	/**
	 * @brief 解析配置字符串
	 * @param ctx 配置上下文
	 * @param config_str 配置字符串
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_parse_conf(receptor_conf_ctx_t* ctx, const char* config_str);

	/* ==================== 周期管理 ==================== */

	/**
	 * @brief 初始化周期结构
	 * @param conf_ctx 配置上下文
	 * @return 周期结构指针
	 */
	RECEPTOR_API receptor_cycle_t* receptor_init_cycle(receptor_conf_ctx_t* conf_ctx);

	/**
	 * @brief 销毁周期结构
	 * @param cycle 周期结构
	 */
	RECEPTOR_API void receptor_destroy_cycle(receptor_cycle_t* cycle);

	/**
	 * @brief 获取核心配置
	 * @param cycle 周期结构
	 * @return 核心配置指针
	 */
	RECEPTOR_API receptor_core_conf_t* receptor_get_core_conf(receptor_cycle_t* cycle);

	/* ==================== 模块管理 ==================== */

	/**
	 * @brief 注册模块
	 * @param module 模块指针
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_register_module(receptor_module_t* module);

	/**
	 * @brief 按名称查找模块
	 * @param name 模块名称
	 * @return 模块指针
	 */
	RECEPTOR_API receptor_module_t* receptor_find_module(const char* name);

	/**
	 * @brief 初始化所有模块
	 * @param cycle 周期结构
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_init_modules(receptor_cycle_t* cycle);

	/**
	 * @brief 退出所有模块
	 * @param cycle 周期结构
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_exit_modules(receptor_cycle_t* cycle);

	/* ==================== 运行时控制 ==================== */

	/**
	 * @brief 启动receptor服务
	 * @param cycle 周期结构
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_start(receptor_cycle_t* cycle);

	/**
	 * @brief 停止receptor服务
	 * @param cycle 周期结构
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_stop(receptor_cycle_t* cycle);

	/**
	 * @brief 重新加载配置
	 * @param cycle 周期结构
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_reload(receptor_cycle_t* cycle);

	/* ==================== 工具函数 ==================== */

	/**
	 * @brief 获取错误信息
	 * @return 错误信息字符串
	 */
	RECEPTOR_API const char* receptor_get_error(void);

	/**
	 * @brief 设置错误信息
	 * @param format 错误信息格式
	 * @param ... 可变参数
	 */
	RECEPTOR_API void receptor_set_error(const char* format, ...);

	/**
	 * @brief 获取最后错误代码
	 * @return 错误代码
	 */
	RECEPTOR_API receptor_int_t receptor_get_last_error(void);

	/* ==================== 信号处理 ==================== */

	/**
	 * @brief 初始化信号处理
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_init_signals(void);

	/**
	 * @brief 添加信号处理
	 * @param signum 信号编号
	 * @param handler 信号处理函数
	 * @return RECEPTOR_OK 成功, RECEPTOR_ERROR 失败
	 */
	RECEPTOR_API receptor_int_t receptor_add_signal(int signum, void(*handler)(int));

#ifdef __cplusplus
}
#endif

#endif /* _RECEPTOR_H_ */