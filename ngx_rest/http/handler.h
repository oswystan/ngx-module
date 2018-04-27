/*
 **************************************************************************************
 *       Filename:  handler.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 11:02:04
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef HANDLER_H_INCLUDED
#define HANDLER_H_INCLUDED

#include "request.h"
#include "response.h"

class HttpHandler {
public:
    virtual ~HttpHandler() {}
    virtual void Serve(HttpRequest& req, HttpResponseWriter& resp) = 0;
};

#endif /*HANDLER_H_INCLUDED*/

/********************************** END **********************************************/

