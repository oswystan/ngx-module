/*
 **************************************************************************************
 *       Filename:  reg.cc
 *    Description:   source file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 11:03:11
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */
#include <stdio.h>
#include <regex>
#include <iostream>
#include <list>
#include <map>
using namespace std;

class HttpRequest {
public:
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
    int Send(string data) {
        cout <<  data << endl;
        return 0;
    }
};

class HttpHandler {
public:
    virtual ~HttpHandler() {}
    virtual void Serve(HttpRequest& req, HttpResponseWriter& resp) = 0;
};

class ListServers : public HttpHandler {
public:
    virtual ~ListServers(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list servers\"}";
        resp.Send(data);
    }
};
class ListClients : public HttpHandler {
public:
    virtual ~ListClients(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list clients\"}";
        resp.Send(data);
    }
};
class GetClientDetail: public HttpHandler {
public:
    virtual ~GetClientDetail(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"get client detail\"}";
        resp.Send(data);
    }
};
class GetServerDetail : public HttpHandler {
public:
    virtual ~GetServerDetail(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"get server detail\"}";
        resp.Send(data);
    }
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
    for (auto it=lst.begin(); it!=lst.end(); it++) {
        regex_match(req.Uri(), sm, (*it)->regexpr);
        if (sm.size() != 0) {
            req.Param().clear();
            std::for_each(sm.begin(), sm.end(), [&](string e) { req.Param().push_back(e);});
            (*it)->handler->Serve(req, resp);
        }
    }

    return 0;
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

struct Operation {
    string method;
    string uri;
};

Operation operations[] = {
    {
        .method = "GET",
        .uri = "/api/servers"
    },
    {
        .method = "GET",
        .uri = "/api/servers/0987654321abcdef/status"
    },
    {
        .method = "GET",
        .uri = "/api/servers/0987654321abcdef/clients"
    },
    {
        .method = "GET",
        .uri = "/api/servers/0987654321abcdef/clients/0987654321"
    },
    {
        .method = "POST",
        .uri = "/api/servers/0987654321abcdef/clients/0987654321"
    },
    {
        .method = "HEAD",
        .uri = "/api/servers/0987654321abcdef/clients/0987654321"
    },
    {
        .method = "PUT",
        .uri = "/api/servers/0987654321abcdef/clients/0987654321"
    },
    {
        .method = "DELETE",
        .uri = "/api/servers/0987654321abcdef/clients/0987654321"
    },
};

int main()
{
    HttpRouter router;
    router.Get("^/api/servers/*$", new ListServers);
    router.Get("^/api/servers/([a-f|0-9]+)/status/*$", new GetServerDetail);
    router.Get("^/api/servers/([a-f|0-9]+)/clients/*$", new ListClients);
    router.Get("^/api/servers/([a-f|0-9]+)/clients/([0-9]+)/*$", new GetClientDetail);

    unsigned int i=0;
    for (i = 0; i < sizeof(operations)/sizeof(operations[0]); ++i) {
        HttpRequest req(operations[i].method, operations[i].uri);
        HttpResponseWriter resp;
        if (0 != router.Serve(req, resp)) {
            cout << "NO HANDLER FOR: "
                 << operations[i].method << " "
                 << operations[i].uri
                 << endl;
        }
    }
    return 0;
}

/********************************** END **********************************************/

