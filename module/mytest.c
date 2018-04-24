/*
 **************************************************************************************
 *       Filename:  mytest.c
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-24 06:19:22
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_log.h>

static char* ngx_http_mytest2(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf);
static ngx_int_t ngx_http_mytest_handler2(ngx_http_request_t *r);

//自定义的参数结构体
typedef struct {
    ngx_str_t my_config_str;
    ngx_int_t my_config_num;
    ngx_int_t my_cfg_idx;
}  ngx_http_mytest_conf_t;


static ngx_command_t ngx_http_mytest_commands[] = {
    {
        ngx_string("mytest2"),//指令名称，在配置文件中使用
        NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
        ngx_http_mytest2, // 回调函数
        NGX_HTTP_LOC_CONF_OFFSET, //位置
        0, //指令的值保存的位置
        NULL
    },

    ngx_null_command
};



static char* ngx_http_mytest2(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_mytest_conf_t *mycf = conf;
    ngx_str_t *value = cf->args->elts;
    if (cf->args->nelts != 3) {
        return NGX_CONF_ERROR;
    }
    if (!mycf) {
        return NGX_CONF_ERROR;
    }
    if (!value) {
        return NGX_CONF_ERROR;
    }
#if 1
    {
        value[1].data++;
        value[1].len--;
        mycf->my_config_str = value[1];
        mycf->my_cfg_idx = ngx_http_get_variable_index(cf, &value[1]);
        if (mycf->my_cfg_idx == NGX_ERROR) {
            ngx_log_error_core(NGX_LOG_ERR, cf->log, 0, "-----fail to get var idx");
        }
    }

    {
        mycf->my_config_num = ngx_atoi(value[2].data, value[2].len);
        if (mycf->my_config_num == NGX_ERROR) {
            return "invalid number";
        }
    }

#endif

    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_mytest_handler2;
    return NGX_CONF_OK;
}


static ngx_http_module_t ngx_http_mytest_module_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ngx_http_mytest_create_loc_conf,
    NULL
};

ngx_module_t ngx_http_mytest_module2 = {
    NGX_MODULE_V1,
    &ngx_http_mytest_module_ctx,
    ngx_http_mytest_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};


//回调函数 handler
static ngx_int_t ngx_http_mytest_handler2(ngx_http_request_t *r)
{

    ngx_http_mytest_conf_t *mycf =  ngx_http_get_module_loc_conf(r, ngx_http_mytest_module2);
    ngx_log_error_core(NGX_LOG_ERR, r->connection->log, 0, "process count ======%d ", mycf->my_config_num);

    if(!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD)))
        return NGX_HTTP_NOT_ALLOWED;

    ngx_int_t rc = ngx_http_discard_request_body(r);
    if(rc != NGX_OK)
        return rc;

    ngx_str_t type = ngx_string("text/plain");
    ngx_http_variable_value_t* vv;
    vv = ngx_http_get_indexed_variable(r, mycf->my_cfg_idx);
    ngx_str_t str = {len: vv->len, data: vv->data};
    if (vv != NULL && !vv->not_found) {
        ngx_log_error_core(NGX_LOG_ERR, r->connection->log, 0, "variable ======%V", &str);
    }


    char a1[] = "  Hello, -------------his is nginx world!\n";
    char a2[vv->len +1] ;
    size_t i=0;
    while(i < vv->len){
        a2[i]=vv->data[i];
        i++;
    }
     a2[i] = '\0';

    char a3[sizeof(a1)+sizeof(a2) -1];
    memset(a3,0,sizeof(a1)+sizeof(a2) -1);


    strcat(a3,a2);
    strcat(a3,a1);

    ngx_str_t response = ngx_string(a3);
    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = response.len;
    r->headers_out.content_type = type;

    rc = ngx_http_send_header(r);
    if(rc == NGX_ERROR || rc > NGX_OK || r->header_only)
        return rc;

    ngx_buf_t *b;
    b = ngx_create_temp_buf(r->pool, response.len);
    if(b == NULL)
        return NGX_HTTP_INTERNAL_SERVER_ERROR;

    ngx_memcpy(b->pos, response.data, response.len);
    b->last = b->pos + response.len;
    b->last_buf = 1;

    ngx_chain_t out;
    out.buf = b;
    out.next = NULL;

    return ngx_http_output_filter(r, &out);
}

static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_mytest_conf_t *mycf;
    mycf = (ngx_http_mytest_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_mytest_conf_t));
    if (mycf == NULL) {
        return NULL;
    }
    mycf->my_config_num = NGX_CONF_UNSET;
    return mycf;
}

/********************************** END **********************************************/

