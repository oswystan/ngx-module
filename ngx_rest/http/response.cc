/*
 **************************************************************************************
 *       Filename:  response.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 11:01:11
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#include "log.h"
#include "response.h"

HttpResponseWriter* HttpResponseWriter::Create(ngx_http_request_t* r) {
    return new HttpResponseWriter(r);
}

int HttpResponseWriter::Send(string data) {
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

/********************************** END **********************************************/

