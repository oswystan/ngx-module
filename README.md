## WHAT IS THIS
A tiny nginx restful api gateway framework written in c++11. Have fun with it :-)

## HOWTO

```

// STEP-1: TODO remove this demo and add your app specific handler
class ListDemo : public HttpHandler {
public:
    virtual ~ListDemo(){}
    void Serve(HttpRequest& req, HttpResponseWriter& resp) {
        string data = "{\"error\": 0, \"desc\": \"list Demo\"}\n";
        resp.Send(data);
    }
};

// STEP-2: init router here
class NgxApp : public HttApp {
    int Init() {
        log_init(LOG_FILE);

        //TODO remove this demo and add your app specific handler
        router.Get("^/api/demo/*$", new ListDemo);

        return 0;
    }
}

// STEP-3: make

```
