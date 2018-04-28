/*
 **************************************************************************************
 *       Filename:  restful_api.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-28 14:46:56
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef NGX_REST_HTTP_RESTFUL_API_H_INCLUDED
#define NGX_REST_HTTP_RESTFUL_API_H_INCLUDED

extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_log.h>
}

#include <string>
#include <regex>
#include <list>
#include <map>
#include <vector>
using namespace std;

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

class HttpResponseWriter {
public:
    static HttpResponseWriter* Create(ngx_http_request_t* r);
    HttpResponseWriter(ngx_http_request_t* r) : req(r) {}
    int Send(string &data);
protected:
    ngx_http_request_t* req = nullptr;
};

class HttpHandler {
public:
    virtual ~HttpHandler() {}
    virtual void Serve(HttpRequest& req, HttpResponseWriter& resp) = 0;
};

class HttpRouter {
    enum EMethod {
        POST = 0,
        PUT,
        GET,
        DELETE,
        HEAD
    };
    struct RouterMatcher {
        ~RouterMatcher() { delete handler; }
        string strexpr;
        regex  regexpr;
        HttpHandler* handler;
    };
    typedef list<RouterMatcher*> MatcherList;

public:
    HttpRouter() {}
    ~HttpRouter() {
        Clear();
    }
    HttpRouter& Post(string regexpr, HttpHandler* handler) {
        return Method(POST, regexpr, handler);
    }
    HttpRouter& Put(string regexpr, HttpHandler* handler) {
        return Method(PUT, regexpr, handler);
    }

    HttpRouter& Get(string regexpr, HttpHandler* handler) {
        return Method(GET, regexpr, handler);
    }

    HttpRouter& Delete(string regexpr, HttpHandler* handler) {
        return Method(DELETE, regexpr, handler);
    }

    HttpRouter& Head(string regexpr, HttpHandler* handler) {
        return Method(HEAD, regexpr, handler);
    }

    int Serve(HttpRequest& req, HttpResponseWriter& resp);
    void Clear();

private:
    HttpRouter& Method(EMethod method, string regexpr, HttpHandler* handler);
    EMethod Str2Method(string m) {
        if (m == "POST") {
            return POST;
        } else if (m == "PUT") {
            return PUT;
        } else if (m == "DELETE") {
            return DELETE;
        } else if (m == "HEAD") {
            return HEAD;
        } else {
            return GET;
        }
    }

private:
    map<EMethod, MatcherList> routers;
};

class HttpApp {
public:
    static HttpApp* Instance();
    virtual ~HttpApp() {}
    virtual int Init() = 0;
    virtual HttpRouter& Router() = 0;
};

#endif /*NGX_REST_HTTP_RESTFUL_API_H_INCLUDED*/

/********************************** END **********************************************/

