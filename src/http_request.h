/*
 * Gogoing -- A mini HTTP Server
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
#include <sys/types.h>

/* define MIME type */
typedef struct mime_node
{
	const char *type;
	const char *value;
}mime_node;

mime_node going_mime[] = 
{
	{".html", "text/html"},
	{".xml", "text/xml"},
	{".txt", "text/plain"},
	{".xhtml", "application/xhtml+xml"},
	{".pdf", "application/pdf"},
	{".word", "application/msword"},
	{".gz", "application/x-gzip"},
	{".tar", "application/x-tar"},
	{".png", "image/png"},
	{".gif", "image/gif"},
	{".jpg", "image/jpg"},
	{".jpeg", "image/jpeg"},
	{".au", "audio/basic"},
	{".mpeg", "video/mpeg"},
	{".mpg", "video/mpeg"},
	{".avi", "video/x-msvideo"},
	{NULL ,NULL}
};

/*
 * @brief: mime type convert to mime value
 * @param type: mime type
 * @return: if not exist, NULL;other return value pointer. 
 */
inline const char* going_mime_type2value(const char* type)
{
	for(int i = 0; going_mime[i].type != NULL; ++i)
	{
		if(strcmp(type, going_mime[i].type) == 0)
			return going_mime[i].value;
	}
	return NULL;
}

/* MethodSPRequest-URISPHTTP-VersionCRLFMethod */
#define GOING_HTTP_UNKNOWN                 0x0001
#define GOING_HTTP_GET                     0x0002
#define GOING_HTTP_HEAD                    0x0004
#define GOING_HTTP_POST                    0x0008

/* HTTP Keep-Alive ? */
#define GOING_HTTP_CONNECTION_CLOSE        1
#define GOING_HTTP_CONNECTION_KEEP_ALIVE   2   

/* HTTP Status Code */
/* 
 * just a subset of HTTP Status Code
 * you can see the list: https://en.wikipedia.org/wiki/List_of_HTTP_status_codes 
 */ 
#define GOING_HTTP_CONTINUE               100
 
#define GOING_HTTP_OK                     200
#define GOING_HTTP_ACCEPTED               202

#define GOING_HTTP_MOVED_PERMANENTLY      301
#define GOING_HTTP_MOVED_TEMPORARILY      302
#define GOING_HTTP_SEE_OTHER              303
#define GOING_HTTP_NOT_MODIFIED      	  304

#define GOING_HTTP_BAD_REQUEST     		  400
#define GOING_HTTP_FORBIDDEN      		  403
#define GOING_HTTP_NOT_FOUND       		  404

#define GOING_HTTP_INTERNAL_SERVER_ERROR  500
#define GOING_HTTP_NOT_IMPLEMENTED  	  501
#define GOING_HTTP_BAD_GATEWAY     		  502
#define GOING_HTTP_SERVICE_UNAVAILABLE 	  503

char going_ok[]            = "OK";
char going_badrequest[]    = "Bad Request";
char going_forbidden[]     = "Forbidden";
char going_notfound[]      = "Not Found";
char going_noimplemented[] = "No Implemented";
/*
 * @brief: 根据HTTP状态码返回友好语句
 * @param: HTTP status code
 * @return: 相应语句
 */
char *going_get_state_by_codes(int http_codes);

/* HTTP response */


#endif