#ifndef _RECEPTOR_HTTP_REQUEST_H_
#define _RECEPTOR_HTTP_REQUEST_H_


#ifdef __cplusplus
extern "C" {
#endif

	/* ==================== 前向声明 ==================== */

	typedef struct receptor_http_request_s         receptor_http_request_t;
	typedef struct receptor_http_response_s        receptor_http_response_t;
	typedef struct receptor_http_connection_s      receptor_http_connection_t;
	typedef struct receptor_http_header_s          receptor_http_header_t;
	typedef struct receptor_http_chunk_s           receptor_http_chunk_t;

	/* ==================== 常量定义 ==================== */

	/* HTTP 方法 */
#define RECEPTOR_HTTP_UNKNOWN       0x0000
#define RECEPTOR_HTTP_GET           0x0001
#define RECEPTOR_HTTP_HEAD          0x0002
#define RECEPTOR_HTTP_POST          0x0004
#define RECEPTOR_HTTP_PUT           0x0008
#define RECEPTOR_HTTP_DELETE        0x0010
#define RECEPTOR_HTTP_OPTIONS       0x0020
#define RECEPTOR_HTTP_PATCH         0x0040
#define RECEPTOR_HTTP_TRACE         0x0080
#define RECEPTOR_HTTP_CONNECT       0x0100

/* HTTP 版本 */
#define RECEPTOR_HTTP_VERSION_9     9
#define RECEPTOR_HTTP_VERSION_10    1000
#define RECEPTOR_HTTP_VERSION_11    1001
#define RECEPTOR_HTTP_VERSION_20    2000

/* HTTP 状态码 */
#define RECEPTOR_HTTP_CONTINUE                      100
#define RECEPTOR_HTTP_SWITCHING_PROTOCOLS           101
#define RECEPTOR_HTTP_PROCESSING                    102

#define RECEPTOR_HTTP_OK                            200
#define RECEPTOR_HTTP_CREATED                       201
#define RECEPTOR_HTTP_ACCEPTED                      202
#define RECEPTOR_HTTP_NO_CONTENT                    204
#define RECEPTOR_HTTP_PARTIAL_CONTENT               206

#define RECEPTOR_HTTP_MOVED_PERMANENTLY             301
#define RECEPTOR_HTTP_FOUND                         302
#define RECEPTOR_HTTP_SEE_OTHER                     303
#define RECEPTOR_HTTP_NOT_MODIFIED                  304
#define RECEPTOR_HTTP_TEMPORARY_REDIRECT            307
#define RECEPTOR_HTTP_PERMANENT_REDIRECT            308

#define RECEPTOR_HTTP_BAD_REQUEST                   400
#define RECEPTOR_HTTP_UNAUTHORIZED                  401
#define RECEPTOR_HTTP_FORBIDDEN                     403
#define RECEPTOR_HTTP_NOT_FOUND                     404
#define RECEPTOR_HTTP_METHOD_NOT_ALLOWED            405
#define RECEPTOR_HTTP_REQUEST_TIMEOUT               408
#define RECEPTOR_HTTP_PAYLOAD_TOO_LARGE             413
#define RECEPTOR_HTTP_URI_TOO_LONG                  414

#define RECEPTOR_HTTP_INTERNAL_SERVER_ERROR         500
#define RECEPTOR_HTTP_NOT_IMPLEMENTED               501
#define RECEPTOR_HTTP_BAD_GATEWAY                   502
#define RECEPTOR_HTTP_SERVICE_UNAVAILABLE           503
#define RECEPTOR_HTTP_GATEWAY_TIMEOUT               504

/* 缓冲区大小 */
#define RECEPTOR_HTTP_MAX_HEADER_SIZE       8192
#define RECEPTOR_HTTP_MAX_URI_SIZE          4096
#define RECEPTOR_HTTP_MAX_HEADER_FIELD_SIZE 8192
#define RECEPTOR_HTTP_MAX_HEADER_VALUE_SIZE 32768

/* ==================== 数据结构 ==================== */

/**
 * HTTP 头部字段
 */
	struct receptor_http_header_s {
		receptor_str_t          key;            /* 头部字段名 */
		receptor_str_t          value;          /* 头部字段值 */
		receptor_list_t         list;           /* 链表节点 */
	};

	/**
	 * HTTP 块数据
	 */
	struct receptor_http_chunk_s {
		receptor_str_t          data;           /* 块数据 */
		size_t                  size;           /* 块大小 */
		receptor_http_chunk_t  *next;           /* 下一个块 */
	};

	/**
	 * HTTP 连接信息
	 */
	struct receptor_http_connection_s {
		receptor_socket_t       fd;             /* 套接字描述符 */
		receptor_str_t          addr_text;      /* 地址文本 */
		receptor_uint_t         ssl : 1;          /* 是否SSL连接 */
		receptor_uint_t         keepalive : 1;    /* 是否保持连接 */
		receptor_uint_t         reusable : 1;     /* 是否可复用 */
		void                   *ssl_ctx;        /* SSL上下文 */
	};

	/**
	 * HTTP 响应
	 */
	struct receptor_http_response_s {
		receptor_http_request_t *request;       /* 关联的请求 */

		/* 状态行 */
		receptor_uint_t         status;         /* 状态码 */
		receptor_str_t          status_line;    /* 状态行文本 */

		/* 头部 */
		receptor_list_t         headers;        /* 响应头部列表 */
		receptor_str_t          content_type;   /* 内容类型 */
		receptor_str_t          content_encoding; /* 内容编码 */
		receptor_uint_t         content_length; /* 内容长度 */

		/* 主体 */
		receptor_str_t          body;           /* 响应体 */
		receptor_http_chunk_t  *chunks;         /* 块链表 */
		receptor_http_chunk_t  *last_chunk;     /* 最后一个块 */

		/* 缓冲区 */
		receptor_str_t          header_buffer;  /* 头部缓冲区 */
		receptor_str_t          body_buffer;    /* 主体缓冲区 */

		/* 控制标志 */
		receptor_uint_t         headers_sent : 1; /* 头部是否已发送 */
		receptor_uint_t         chunked : 1;      /* 是否分块传输 */
		receptor_uint_t         gzip : 1;         /* 是否gzip压缩 */
	};

	/**
	 * HTTP 请求
	 */
	struct receptor_http_request_s {
		receptor_pool_t        *pool;           /* 内存池 */
		receptor_http_connection_t *connection; /* 连接信息 */

		/* 请求行 */
		receptor_uint_t         method;         /* 请求方法 */
		receptor_str_t          method_name;    /* 方法名称 */
		receptor_str_t          uri;            /* 请求URI */
		receptor_str_t          args;           /* 查询参数 */
		receptor_str_t          exten;          /* 文件扩展名 */
		receptor_str_t          unparsed_uri;   /* 未解析的URI */
		receptor_uint_t         http_version;   /* HTTP版本 */

		/* 头部 */
		receptor_list_t         headers_in;     /* 请求头部 */
		receptor_list_t         headers_out;    /* 输出头部 */
		receptor_table_t       *headers_in_hash; /* 头部哈希表 */

		/* 主体 */
		receptor_str_t          request_body;   /* 请求体 */
		receptor_http_chunk_t  *request_chunks; /* 请求块链表 */
		receptor_uint_t         content_length; /* 内容长度 */
		receptor_uint_t         content_length_n; /* 内容长度数值 */
		receptor_str_t          content_type;   /* 内容类型 */

		/* 响应 */
		receptor_http_response_t *response;     /* 响应对象 */

		/* 状态和控制 */
		receptor_uint_t         state;          /* 请求状态 */
		receptor_uint_t         subrequests;    /* 子请求计数 */
		receptor_uint_t         count : 16;       /* 引用计数 */
		receptor_uint_t         main : 1;         /* 是否主请求 */
	receptor_uint_t         internal:1;     /* 是否内部请求 */
	receptor_uint_t         error : 1;        /* 是否出错 */
	receptor_uint_t         header_sent : 1;  /* 头部是否已发送 */
	receptor_uint_t         header_only : 1;  /* 是否只发送头部 */

	/* 时间和统计 */
	receptor_msec_t         start_sec;      /* 开始时间(秒) */
	receptor_msec_t         start_msec;     /* 开始时间(毫秒) */
	receptor_msec_t         header_time;    /* 头部接收完成时间 */
	receptor_msec_t         body_time;      /* 主体接收完成时间 */

	/* 缓冲区 */
	receptor_str_t          header_name;    /* 当前解析的头部名 */
	receptor_str_t          header_value;   /* 当前解析的头部值 */
	receptor_str_t          request_line;   /* 请求行缓冲区 */

	/* 模块上下文 */
	void                  **ctx;            /* 模块上下文数组 */
	void                   *main_conf;      /* 主配置 */
	void                   *srv_conf;       /* 服务器配置 */
	void                   *loc_conf;       /* 位置配置 */
	};

	/* ==================== 枚举类型 ==================== */

	/**
	 * HTTP 请求解析状态
	 */
	typedef enum {
		RECEPTOR_HTTP_PARSE_REQUEST_LINE = 0,   /* 解析请求行 */
		RECEPTOR_HTTP_PARSE_HEADER,             /* 解析头部 */
		RECEPTOR_HTTP_PARSE_BODY,               /* 解析主体 */
		RECEPTOR_HTTP_PARSE_DONE,               /* 解析完成 */
		RECEPTOR_HTTP_PARSE_ERROR               /* 解析错误 */
	} receptor_http_parse_state_t;

	/**
	 * HTTP 内容编码
	 */
	typedef enum {
		RECEPTOR_HTTP_ENCODING_IDENTITY = 0,    /* 不编码 */
		RECEPTOR_HTTP_ENCODING_GZIP,            /* GZIP压缩 */
		RECEPTOR_HTTP_ENCODING_DEFLATE,         /* DEFLATE压缩 */
		RECEPTOR_HTTP_ENCODING_BR               /* Brotli压缩 */
	} receptor_http_content_encoding_t;

	/* ==================== 函数声明 ==================== */

	/* ==================== 请求创建和销毁 ==================== */

	/**
	 * @brief 创建HTTP请求
	 * @param pool 内存池
	 * @param connection 连接对象
	 * @return 请求对象指针
	 */
	RECEPTOR_API receptor_http_request_t*
		receptor_http_create_request(receptor_pool_t *pool,
			receptor_http_connection_t *connection);

	/**
	 * @brief 销毁HTTP请求
	 * @param request 请求对象
	 */
	RECEPTOR_API void
		receptor_http_destroy_request(receptor_http_request_t *request);

	/**
	 * @brief 复制HTTP请求（用于子请求）
	 * @param pool 内存池
	 * @param src 源请求
	 * @return 新请求对象
	 */
	RECEPTOR_API receptor_http_request_t*
		receptor_http_copy_request(receptor_pool_t *pool,
			receptor_http_request_t *src);

	/* ==================== 请求解析 ==================== */

	/**
	 * @brief 解析HTTP请求
	 * @param request 请求对象
	 * @param data 输入数据
	 * @param size 数据大小
	 * @return 解析状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_parse_request(receptor_http_request_t *request,
			const u_char *data, size_t size);

	/**
	 * @brief 解析请求行
	 * @param request 请求对象
	 * @param data 请求行数据
	 * @param size 数据大小
	 * @return 解析状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_parse_request_line(receptor_http_request_t *request,
			const u_char *data, size_t size);

	/**
	 * @brief 解析请求头部
	 * @param request 请求对象
	 * @param data 头部数据
	 * @param size 数据大小
	 * @return 解析状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_parse_header(receptor_http_request_t *request,
			const u_char *data, size_t size);

	/**
	 * @brief 解析请求主体
	 * @param request 请求对象
	 * @param data 主体数据
	 * @param size 数据大小
	 * @return 解析状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_parse_body(receptor_http_request_t *request,
			const u_char *data, size_t size);

	/* ==================== 请求信息获取 ==================== */

	/**
	 * @brief 获取请求方法名称
	 * @param method 方法枚举值
	 * @return 方法名称字符串
	 */
	RECEPTOR_API const char*
		receptor_http_get_method_name(receptor_uint_t method);

	/**
	 * @brief 根据名称获取请求方法
	 * @param name 方法名称
	 * @param len 名称长度
	 * @return 方法枚举值
	 */
	RECEPTOR_API receptor_uint_t
		receptor_http_get_method(const u_char *name, size_t len);

	/**
	 * @brief 获取请求头部值
	 * @param request 请求对象
	 * @param key 头部字段名
	 * @return 头部字段值
	 */
	RECEPTOR_API receptor_str_t*
		receptor_http_get_header(receptor_http_request_t *request, const char *key);

	/**
	 * @brief 获取查询参数值
	 * @param request 请求对象
	 * @param key 参数名
	 * @return 参数值
	 */
	RECEPTOR_API receptor_str_t*
		receptor_http_get_arg(receptor_http_request_t *request, const char *key);

	/**
	 * @brief 获取客户端IP地址
	 * @param request 请求对象
	 * @return IP地址字符串
	 */
	RECEPTOR_API receptor_str_t
		receptor_http_get_client_ip(receptor_http_request_t *request);

	/* ==================== 请求处理 ==================== */

	/**
	 * @brief 初始化请求处理
	 * @param request 请求对象
	 * @return 处理状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_init_request(receptor_http_request_t *request);

	/**
	 * @brief 处理HTTP请求
	 * @param request 请求对象
	 * @return 处理状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_process_request(receptor_http_request_t *request);

	/**
	 * @brief 结束请求处理
	 * @param request 请求对象
	 * @return 处理状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_finalize_request(receptor_http_request_t *request);

	/**
	 * @brief 终止请求（错误处理）
	 * @param request 请求对象
	 * @param status 状态码
	 * @return 处理状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_terminate_request(receptor_http_request_t *request,
			receptor_uint_t status);

	/* ==================== 头部操作 ==================== */

	/**
	 * @brief 添加请求头部
	 * @param request 请求对象
	 * @param key 头部字段名
	 * @param value 头部字段值
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_add_header(receptor_http_request_t *request,
			const char *key, const char *value);

	/**
	 * @brief 移除请求头部
	 * @param request 请求对象
	 * @param key 头部字段名
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_remove_header(receptor_http_request_t *request, const char *key);

	/**
	 * @brief 设置内容类型
	 * @param request 请求对象
	 * @param content_type 内容类型
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_set_content_type(receptor_http_request_t *request,
			const char *content_type);

	/**
	 * @brief 设置内容长度
	 * @param request 请求对象
	 * @param length 内容长度
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_set_content_length(receptor_http_request_t *request,
			size_t length);

	/* ==================== 主体操作 ==================== */

	/**
	 * @brief 读取请求主体
	 * @param request 请求对象
	 * @param buffer 输出缓冲区
	 * @param size 缓冲区大小
	 * @return 读取的字节数
	 */
	RECEPTOR_API ssize_t
		receptor_http_read_body(receptor_http_request_t *request,
			u_char *buffer, size_t size);

	/**
	 * @brief 获取整个请求主体
	 * @param request 请求对象
	 * @return 请求主体数据
	 */
	RECEPTOR_API receptor_str_t
		receptor_http_get_body(receptor_http_request_t *request);

	/**
	 * @brief 添加请求块数据
	 * @param request 请求对象
	 * @param data 块数据
	 * @param size 数据大小
	 * @return 操作状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_add_chunk(receptor_http_request_t *request,
			const u_char *data, size_t size);

	/* ==================== 工具函数 ==================== */

	/**
	 * @brief 获取HTTP状态描述
	 * @param status 状态码
	 * @return 状态描述字符串
	 */
	RECEPTOR_API const char*
		receptor_http_get_status_text(receptor_uint_t status);

	/**
	 * @brief 解码URL编码字符串
	 * @param dst 输出缓冲区
	 * @param src 源字符串
	 * @param size 源字符串大小
	 * @return 解码后的字符串大小
	 */
	RECEPTOR_API size_t
		receptor_http_unescape_uri(u_char *dst, const u_char *src, size_t size);

	/**
	 * @brief 编码URL字符串
	 * @param dst 输出缓冲区
	 * @param src 源字符串
	 * @param size 源字符串大小
	 * @return 编码后的字符串大小
	 */
	RECEPTOR_API size_t
		receptor_http_escape_uri(u_char *dst, const u_char *src, size_t size);

	/**
	 * @brief 解析查询字符串
	 * @param request 请求对象
	 * @param data 查询字符串
	 * @param size 字符串大小
	 * @return 解析状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_parse_args(receptor_http_request_t *request,
			const u_char *data, size_t size);

	/**
	 * @brief 获取当前时间戳（用于请求计时）
	 * @param request 请求对象
	 */
	RECEPTOR_API void
		receptor_http_update_time(receptor_http_request_t *request);

	/* ==================== 子请求支持 ==================== */

	/**
	 * @brief 创建子请求
	 * @param parent 父请求
	 * @return 子请求对象
	 */
	RECEPTOR_API receptor_http_request_t*
		receptor_http_create_subrequest(receptor_http_request_t *parent);

	/**
	 * @brief 执行子请求
	 * @param subrequest 子请求
	 * @return 执行状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_run_subrequest(receptor_http_request_t *subrequest);

	/**
	 * @brief 等待子请求完成
	 * @param subrequest 子请求
	 * @return 等待状态
	 */
	RECEPTOR_API receptor_int_t
		receptor_http_wait_subrequest(receptor_http_request_t *subrequest);

#ifdef __cplusplus
}
#endif

#endif /* _RECEPTOR_HTTP_REQUEST_H_ */