/*
 **************************************************************************************
 *       Filename:  app.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 18:52:01
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_log.h>
}

#include "log.h"
#include "router.h"
#include "app.h"
#include "ngx_app_handlers.h"

#define LOG_FILE NGX_PREFIX"logs/restful.log"
static HttpApp* g_app = nullptr;

class NgxApp : public HttpApp {
public:
    NgxApp(){}
    ~NgxApp() {}

    int Init();
    HttpRouter& Router() {
        return router;
    }
protected:
    HttpRouter router;
};

int NgxApp::Init() {
    log_init(LOG_FILE);

    //TODO init router
    router.Get("^/api/servers/*$", new ListServers);
    router.Get("^/api/servers/([a-f|0-9]+)/status/*$", new GetServerDetail);
    router.Get("^/api/servers/([a-f|0-9]+)/clients/*$", new ListClients);
    router.Get("^/api/servers/([a-f|0-9]+)/clients/([0-9]+)/*$", new GetClientDetail);

    return 0;
}

HttpApp* HttpApp::Instance() {
    if (g_app) {
        return g_app;
    }
    g_app = new NgxApp();
    return g_app;
}

extern "C" ngx_int_t ngx_rest_api_call(ngx_http_request_t *r) {
    char method[64];
    memset(method, 0x00, sizeof(method));
    memcpy(method, r->method_name.data, r->method_name.len);
    auto app = HttpApp::Instance();
    auto req = HttpRequest::Create(r);
    auto resp = HttpResponseWriter::Create(r);
    app->Router().Serve(*req, *resp);
    delete req;
    delete resp;

    return NGX_OK;
}

extern "C" ngx_int_t ngx_rest_api_init_process(ngx_cycle_t *cycle) {
    auto app = HttpApp::Instance();
    return app->Init();
}

/********************************** END **********************************************/
