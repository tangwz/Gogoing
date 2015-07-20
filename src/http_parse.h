/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_parse.h
 * Data:         2015/6/3
 * Blog:         tangwz.com
 */

#ifndef _HTTP_PARSE_H_
#define _HTTP_PARSE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <sstream>
#include <iostream>

using namespace std;

typedef map<string, string> going_header;

#define make_going_header(key, value) make_pair((key), (value))

typedef struct _going_http_header_t
{
	string method;
	string url;
	string version;

	going_header header;

	string body;
}going_http_header_t;

/*
 * @brief: Print going_http_header_t's header.
 * @param head: going_header's const reference.
 * @return: None.
 */
void going_print_http_header_header(const going_header& head);

/*
 * @brief: Print going_http_header_t
 * @param: going_http_header_t pointer.
 * @return: None
 */
void going_print_http_header(going_http_header_t *phttphdr);

/*
 * 
 */
going_http_header_t *going_alloc_http_header();

void going_free_http_header(going_http_header_t *phttphdr);

bool going_parse_http_request(const string7 http_request, going_http_header_t* phttphdr);

string going_get_value_from_http_header(const string& key, const going_header& header);

#endif