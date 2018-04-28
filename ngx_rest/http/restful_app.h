/*
 **************************************************************************************
 *       Filename:  restful_app.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-28 15:08:37
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef NGX_REST_HTTP_RESTFUL_APP_H_INCLUDED
#define NGX_REST_HTTP_RESTFUL_APP_H_INCLUDED


#include "restful_api.h"

class ListServers : public HttpHandler {
public:
    virtual ~ListServers(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list servers\"}\n";
        resp.Send(data);
    }
};

#endif /*NGX_REST_HTTP_RESTFUL_APP_H_INCLUDED*/

/********************************** END **********************************************/

