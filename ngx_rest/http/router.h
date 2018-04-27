/*
 **************************************************************************************
 *       Filename:  router.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 11:00:51
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef ROUTER_H_INCLUDED
#define ROUTER_H_INCLUDED


#include "rest_common.h"
#include "handler.h"
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

#endif /*ROUTER_H_INCLUDED*/

/********************************** END **********************************************/

