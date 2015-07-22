/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_request.cpp
 * Data:         2015/6/5
 * Blog:         tangwz.com
 */

#include "http_request.h"

/*
 * @brief: 根据HTTP状态码返回友好语句
 * @param: HTTP status code
 * @return: 相应语句
 */
char *going_get_state_by_codes(int http_codes)
{
	switch(http_codes){
		case GOING_HTTP_OK:
			return going_ok;
		case GOING_HTTP_BAD_REQUEST:
			return going_badrequest;
		case GOING_HTTP_FORBIDDEN:
			return going_forbidden;
		case GOING_HTTP_NOT_FOUND:
			return going_notfound;
		case GOING_HTTP_NOT_IMPLEMENTED:
			return going_noimplemented;
		default:
			break;
	}

	return NULL;
}