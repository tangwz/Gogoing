/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_parse.cpp
 * Data:         2015/6/5
 * Blog:         tangwz.com
 */

#include "http_parse.h"
#include "Gogoing_dbg.h"

/*
 * @brief: Print going_http_header_t's header.
 * @param head: going_header's const reference.
 * @return: None.
 */
void going_print_http_header_header(const going_header& head)
{
	if(!head.empty()){
		going_header::const_iterator cit = head.begin();
		while(cit != head.end()){
			cout << cit->first << ":" << cit->second << endl;
			++cit;
		}
	}
}

/*
 * @brief: Print going_http_header_t.
 * @param: going_http_header_t pointer.
 * @return: None.
 */
void going_print_http_header(going_http_header_t *phttphdr)
{
	if(NULL == phttphdr){
		log_err("going_print_http_header failed.");
		return;
	}

	cout << phttphdr->method << " " << phttphdr->url << " " << phttphdr->version <<endl;
	going_print_http_header_header(phttphdr->header);
	cout << endl << phttphdr->body << endl;
}

/*
 * @brief: alloc memory to going_http_header_t.
 * @param: None.
 * @return: allocate failed, NULL; success, others.
 */
going_http_header_t *going_alloc_http_header()
{
	going_http_header_t* phttphdr = (going_http_header_t *)new going_http_header_t;
	if(phttphdr == NULL){
		log_err("going_alloc_http_header failed.");
		exit(-1);
	}
	return phttphdr;
}

/*
 * @brief: free going_http_header_t's memory.
 * @param phttphdr: going_http_header_t pointer.
 * @return: NULL.
 */
void going_free_http_header(going_http_header_t *phttphdr)
{
	if(phttphdr == NULL)
		return;
	delete phttphdr;
}

/*
 * @brief: parse http_request
 * @param http_request: parsed http_request; phttphdr: save the value.
 * @return: success, true; failed, false.
 */
bool going_parse_http_request(const string& http_request, going_http_header_t* phttphdr)
{
	if(http_request.empty()){
		log_err("going_parse_http_request: http_request is empty.");
		return false;
	}
	if(phttphdr == NULL){
		log_err("going_parse_http_request: phttphdr is NULL.");
		return false;
	}

	string crlf("\r\n"), crlfcrlf("\r\n\r\n");
	size_t prev = 0, next = 0;

	/* parse http request first line. */
	if((next = http_request.find(crlf, prev)) != string::npos){
		string first_line(http_request.substr(prev, next - prev));
		prev = next;
		stringstream sstream(first_line);
		sstream >> (phttphdr->method);
		sstream >> (phttphdr->url);
		sstream >> (phttphdr->version);
	}else{
		log_err("going_parse_http_request: http_request has not a \\r\\n");
		return false;
	}

	/* find "\r\n\r\n" */
	size_t pos_crlfcrlf = http_request.find(crlfcrlf, prev);
	if(pos_crlfcrlf == string::npos){
		log_err("going_parse_http_request: http_request has not  a \"\r\n\r\n\"");
		return false;
	}

	/* 解析首部行 */
	string buff, key, value;
	while(1){
		next = http_request.find(crlf, prev + 2);

		/* 如果找到的next不超过"\r\n\r\n"的位置 */
		if(next <= pos_crlfcrlf){
			/* buff保存了一行 */
			buff = http_request.substr(prev + 2, next - prev -2);
			size_t end = 0;
			/* 跳过前置空白符，到达首部关键字起始位置 */
			for(; isblank(buff[end]); ++end)
				;
			int beg = end;
			/* 到达首部关键字的结束位置 */
			for(; buff[end] != ':' && !isblank(buff[end]); ++end)
				;
			key = buff.substr(beg,end - beg);
			/* 跳至首部值的起始位置 */
			for(; (!isalpha(buff[end]) && !isdigit(buff[end])); ++end)
				;
			beg =end;
			/* 到达首部值的结束位置 */
			for(; next != end; ++end)
				;
			value = buff.substr(beg, end - beg);
			phttphdr->header.insert(make_going_header(key, value));

			prev = next;
		}else{
			break;
		}
	}

	//
	phttphdr->body = http_request.substr(pos_crlfcrlf + 4, http_request.size() - pos_crlfcrlf - 4);

	return true;
}

/*
 * @brief: 根据key的值在phttphdr所指向的going_http_header_t中查找相对应的值
 * @param key: keyword; header.
 * @return: find failed, -1; 
 */
string going_get_value_from_http_header(const string& key, const going_header& header)
{
	if(header.empty()){
		return "";
	}
	going_header::const_iterator cit = header.find(key);
	if(cit == header.end())
		return "";

	return (*cit).second;
}