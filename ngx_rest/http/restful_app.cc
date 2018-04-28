/*
 **************************************************************************************
 *       Filename:  restful_app.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-28 15:02:52
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#include "log.h"
#include "restful_app.h"

#define LOG_FILE NGX_PREFIX"logs/restful.log"
static HttpApp* g_app = nullptr;

//TODO remove this demo and add your app specific handler
class ListDemo : public HttpHandler {
public:
    virtual ~ListDemo(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list Demo\"}\n";
        resp.Send(data);
    }
};


class NgxApp : public HttpApp {
public:
    NgxApp(){}
    ~NgxApp() {}

    int Init() {
        log_init(LOG_FILE);

        //TODO remove this demo and add your app specific handler
        router.Get("^/api/demo/*$", new ListDemo);

        return 0;
    }
    HttpRouter& Router() { return router; }
protected:
    HttpRouter router;
};

HttpApp* HttpApp::Instance() {
    if (g_app) {
        return g_app;
    }

    g_app = new NgxApp();
    return g_app;
}

/********************************** END **********************************************/

