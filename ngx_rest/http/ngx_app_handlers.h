/*
 **************************************************************************************
 *       Filename:  ngx_app_handlers.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 19:32:50
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef NGX_APP_HANDLERS_H_INCLUDED
#define NGX_APP_HANDLERS_H_INCLUDED

#include "handler.h"


class ListServers : public HttpHandler {
public:
    virtual ~ListServers(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list servers\"}\n";
        resp.Send(data);
    }
};
class ListClients : public HttpHandler {
public:
    virtual ~ListClients(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list clients\"}\n";
        resp.Send(data);
    }
};
class GetClientDetail: public HttpHandler {
public:
    virtual ~GetClientDetail(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"get client detail\"}\n";
        resp.Send(data);
    }
};
class GetServerDetail : public HttpHandler {
public:
    virtual ~GetServerDetail(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"get server detail\"}\n";
        resp.Send(data);
    }
};


#endif /*NGX_APP_HANDLERS_H_INCLUDED*/

/********************************** END **********************************************/

