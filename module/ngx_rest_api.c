/*
 **************************************************************************************
 *       Filename:  ngx_rest_api.c
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 15:57:26
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_log.h>

extern ngx_int_t ngx_rest_api_call(ngx_http_request_t *r);
extern ngx_int_t ngx_rest_api_init_process(ngx_cycle_t *cycle);

static char *ngx_rest_api_setter(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *cfg;
    cfg = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    cfg->handler = ngx_rest_api_call;
    return NGX_CONF_OK;
}

static ngx_command_t ngx_rest_api_commands[] = {
    {
        ngx_string("ngx_rest_api"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
        ngx_rest_api_setter,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },
    ngx_null_command
};

static ngx_http_module_t ngx_rest_api_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

ngx_module_t ngx_rest_api = {
      NGX_MODULE_V1,
      &ngx_rest_api_ctx,                  /* module context */
      ngx_rest_api_commands,              /* module directives */
      NGX_HTTP_MODULE,                    /* module type */
      NULL,                               /* init master */
      NULL,                               /* init module */
      ngx_rest_api_init_process,          /* init process */
      NULL,                               /* init thread */
      NULL,                               /* exit thread */
      NULL,                               /* exit process */
      NULL,                               /* exit master */
      NGX_MODULE_V1_PADDING
};

/********************************** END **********************************************/

