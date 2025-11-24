#ifndef _RECEPTOR_HTTP_H_
#define _RECEPTOR_HTTP_H_


typedef struct receptor_http_request_s     receptor_http_request_t;
typedef struct receptor_http_response_s    receptor_http_response_t;

struct receptor_http_request_s {
	void                    *connection;
	receptor_pool_t         *pool;
	receptor_str_t           method;
	receptor_str_t           uri;
	receptor_str_t           args;
	receptor_str_t           proto;

	receptor_http_response_t *response;
};

struct receptor_http_response_s {
	receptor_http_request_t  *request;
	receptor_uint_t          status;
	receptor_str_t           content_type;
	receptor_str_t           body;
};

/* HTTP模块接口 */
typedef struct {
	receptor_int_t(*preconfiguration)(void);
	receptor_int_t(*postconfiguration)(void);
	void           *(*create_main_conf)(void);
	receptor_int_t(*init_main_conf)(void *conf);
} receptor_http_module_t;

#endif /* _RECEPTOR_HTTP_H_ */