/*
 **************************************************************************************
 *       Filename:  app.h
 *    Description:   header file
 *
 *        Version:  1.0
 *        Created:  2018-04-27 18:47:03
 *
 *       Revision:  initial draft;
 **************************************************************************************
 */

#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include "router.h"

class HttpApp {
public:
    static HttpApp* Instance();
    virtual ~HttpApp() {}
    virtual int Init() = 0;
    virtual HttpRouter& Router() = 0;
};

#endif /*APP_H_INCLUDED*/

/********************************** END **********************************************/

