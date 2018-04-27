/*
 **************************************************************************************
 *       Filename:  request.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 10:59:52
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */


#include "log.h"
#include "request.h"

static void ngx_str2string(ngx_str_t* in, string& out) {
    char str[4096];
    memcpy(str, in->data, in->len);
    str[in->len] = '\0';
    out = str;
}

HttpRequest* HttpRequest::Create(ngx_http_request_t* r) {
    string method;
    string uri;
    ngx_str2string(&r->method_name, method);
    ngx_str2string(&r->uri, uri);
    return new HttpRequest(method, uri);
}

/********************************** END **********************************************/

