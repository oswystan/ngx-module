/*
 **************************************************************************************
 *       Filename:  router.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 11:00:53
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#include "log.h"
#include "router.h"

HttpRouter& HttpRouter::Method(EMethod method, string regexpr, HttpHandler* handler) {
    if (regexpr.size() == 0 || !handler) {
        return *this;
    }
    RouterMatcher* matcher = new(RouterMatcher);
    if (!matcher) {
        return *this;
    }
    matcher->strexpr = regexpr;
    matcher->regexpr = regexpr;
    matcher->handler = handler;
    routers[method].push_back(matcher);
    return *this;
}
int HttpRouter::Serve(HttpRequest& req, HttpResponseWriter& resp) {
    auto matcher = routers.find(Str2Method(req.Method()));
    if (matcher == routers.end()) {
        return -1;
    }
    auto lst = matcher->second;
    smatch sm;
    int ret = NGX_HTTP_NOT_FOUND;
    for (auto it=lst.begin(); it!=lst.end(); it++) {
        regex_match(req.Uri(), sm, (*it)->regexpr);
        if (sm.size() != 0) {
            req.Param().clear();
            std::for_each(sm.begin(), sm.end(), [&](string e) { req.Param().push_back(e);});
            (*it)->handler->Serve(req, resp);
            ret = NGX_HTTP_OK;
        }
    }

    return ret;
}
void HttpRouter::Clear() {
    auto itmap = routers.begin();
    for (; itmap != routers.end(); itmap++) {
        for (auto it=itmap->second.begin(); it != itmap->second.end(); it++) {
            delete (*it);
        }
        itmap->second.clear();
    }
}

/********************************** END **********************************************/

