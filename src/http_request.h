/*
 * WebServer -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_request.h
 * Data:         2015/6/3
 * Blog:         tangwz.com
 */

#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include <stdio.h>
#include <stdlib.h>

/* HTTP Status Code */
/* 
 * just a subset of HTTP Status Code
 * you can see the list: https://en.wikipedia.org/wiki/List_of_HTTP_status_codes 
 */ 

#define GOING_CONTINUE       100
 
#define GOING_OK             200
#define GOING_ACCEPTED       202

#define GOING_MOVED          301
#define GOING_FOUND          302
#define GOING_SEEOTHER       303
#define GOING_NOTMODIFIED    304

#define GOING_BADREQUEST     400
#define GOING_FORBIDDEN      403
#define GOING_NOTFOUND       404

#define GOING_ERROR          500
#define GOING_NOIMPLEMENTED  501
#define GOING_BADGATEWAY     502
#define GOING_SRVUNAVAILABLE 503

#endif