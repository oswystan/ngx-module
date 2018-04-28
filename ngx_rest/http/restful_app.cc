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

    return 0;
}

HttpApp* HttpApp::Instance() {
    if (g_app) {
        return g_app;
    }
    g_app = new NgxApp();
    return g_app;
}

/********************************** END **********************************************/

