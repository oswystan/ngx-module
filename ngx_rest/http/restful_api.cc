/*
 **************************************************************************************
 *       Filename:  restful_api.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-28 14:47:10
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#include "log.h"
#include "restful_api.h"

//=====================================
// request
//=====================================
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


//=====================================
// responsewriter
//=====================================
HttpResponseWriter* HttpResponseWriter::Create(ngx_http_request_t* r) {
    return new HttpResponseWriter(r);
}

int HttpResponseWriter::Send(string &data) {
    logi("%s", data.c_str());
    ngx_buf_t* buf = ngx_create_temp_buf(req->pool, data.size());
    if (!buf) {
        loge("fail to create memory for temp buffer");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    memcpy(buf->pos, data.c_str(), data.size());
    buf->last = buf->pos + data.size();
    buf->last_buf = 1;
    buf->last_in_chain = 1;

    ngx_chain_t out;
    out.buf = buf;
    out.next = NULL;

    ngx_str_t content_type = ngx_string("application/json; charset=utf-8");
    req->headers_out.status = NGX_HTTP_OK;
    req->headers_out.content_length_n = data.size();
    req->headers_out.content_type = content_type;
    ngx_http_send_header(req);

    return ngx_http_output_filter(req, &out);
}

//=====================================
// router
//=====================================
HttpRouter& HttpRouter::Method(EMethod method, string regexpr, HttpHandler* handler) {
    if (regexpr.size() == 0 || !handler) {
        return *this;
    }
    RouterMatcher* matcher = new(RouterMatcher);
    if (!matcher) {
        return *this;
    }
    matcher->strexpr = regexpr;
    matcher->regexpr = regexpr;
    matcher->handler = handler;
    routers[method].push_back(matcher);
    return *this;
}
int HttpRouter::Serve(HttpRequest& req, HttpResponseWriter& resp) {
    auto matcher = routers.find(Str2Method(req.Method()));
    if (matcher == routers.end()) {
        return -1;
    }
    auto lst = matcher->second;
    smatch sm;
    int ret = NGX_HTTP_NOT_FOUND;
    for (auto it=lst.begin(); it!=lst.end(); it++) {
        regex_match(req.Uri(), sm, (*it)->regexpr);
        if (sm.size() != 0) {
            req.Param().clear();
            std::for_each(sm.begin(), sm.end(), [&](string e) { req.Param().push_back(e);});
            (*it)->handler->Serve(req, resp);
            ret = NGX_HTTP_OK;
        }
    }

    return ret;
}
void HttpRouter::Clear() {
    auto itmap = routers.begin();
    for (; itmap != routers.end(); itmap++) {
        for (auto it=itmap->second.begin(); it != itmap->second.end(); it++) {
            delete (*it);
        }
        itmap->second.clear();
    }
}

//=====================================
// nginx api handler
//=====================================
extern "C" ngx_int_t ngx_rest_api_call(ngx_http_request_t *r) {
    char method[64];
    memset(method, 0x00, sizeof(method));
    memcpy(method, r->method_name.data, r->method_name.len);
    auto app = HttpApp::Instance();
    auto req = HttpRequest::Create(r);
    auto resp = HttpResponseWriter::Create(r);
    int ret = app->Router().Serve(*req, *resp);
    delete req;
    delete resp;
    return ret;
}

extern "C" ngx_int_t ngx_rest_api_init_process(ngx_cycle_t *cycle) {
    auto app = HttpApp::Instance();
    return app->Init();
}

/********************************** END **********************************************/

