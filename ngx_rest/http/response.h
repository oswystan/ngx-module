/*
 **************************************************************************************
 *       Filename:  response.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 11:01:07
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef RESPONSE_H_INCLUDED
#define RESPONSE_H_INCLUDED

extern "C" {
#include <ngx_http.h>
}
#include "rest_common.h"

class HttpResponseWriter {
public:
    static HttpResponseWriter* Create(ngx_http_request_t* r);
    HttpResponseWriter(ngx_http_request_t* r) : req(r) {}
    int Send(string data);
protected:
    ngx_http_request_t* req = nullptr;
};

#endif /*RESPONSE_H_INCLUDED*/

/********************************** END **********************************************/

