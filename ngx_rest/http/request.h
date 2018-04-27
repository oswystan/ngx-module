/*
 **************************************************************************************
 *       Filename:  request.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 10:59:55
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

extern "C" {
#include <ngx_http.h>
}
#include "rest_common.h"

class HttpRequest {
public:
    static HttpRequest* Create(ngx_http_request_t* r);
    HttpRequest(string strMethod, string strUri)
        : method(strMethod)
        , uri(strUri)
    {}

    string& Method() {
        return method;
    }
    string& Uri() {
        return uri;
    }
    vector<string>& Param() {
        return regexParam;
    }

protected:
    string method;
    string uri;
    vector<string> regexParam;
};



#endif /*REQUEST_H_INCLUDED*/

/********************************** END **********************************************/

