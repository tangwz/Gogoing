/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    Gogoing_util.cpp
 * Data:         2015/6/14
 * Blog:         tangwz.com
 */

#include "Gogoing_util.h"
#include "Gogoing_dbg.h"

/*
 * @brief: Get system time.
 * @param: None.
 * @return: System time,e.g. Thu, 16 Jul 2015 11:28:35 GMT.
 */
string going_time_get()
{
	time_t now;
	struct tm* time_now;
	string str_time;

	time(&now);
	time_now = localtime(&now);

	switch(time_now->tm_wday)
	{
		case 0:
			str_time += "Sun, ";
			break;
		case 1:
			str_time += "Mon, ";
			break;
		case 2:
			str_time += "Tue, ";
			break;
		case 3:
			str_time += "Wed, ";
			break;
		case 4:
			str_time += "Thu, ";
			break;
		case 5:
			str_time += "Fri, ";
			break;
		case 6:
			str_time += "Sat, ";
			break;
	}
	char buf[16];
	snprintf(buf, sizeof(buf), "%d ", time_now->tm_mday);
	str_time += string(buf);
	switch(time_now->tm_mon)
	{
		case 0:
			str_time += "Jan ";
			break;
		case 1:
			str_time += "Feb ";
			break;
		case 2:
			str_time += "Mar ";
			break;
		case 3:
			str_time += "Apr ";
			break;
		case 4:
			str_time += "May ";
			break;
		case 5:
			str_time += "Jun ";
			break;
		case 6:
			str_time += "Jul ";
			break;
		case 7:
			str_time += "Aug ";
			break;
		case 8:
			str_time += "Sep ";
			break;
		case 9:
			str_time += "Oct ";
			break;
		case 10:
			str_time += "Nov ";
			break;
		case 11:
			str_time += "Dec ";
			break;
	}
	snprintf(buf, sizeof(buf), "%d", time_now->tm_year + 1900);
	str_time += string(buf);
	snprintf(buf, sizeof(buf), "%d:%d:%d ", time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
	str_time += string(buf);

	str_time += "GMT";

	return str_time;
}

/*
 * @brief: 根据http请求包中的url和配置文件中的docroot配置选项构造url
 * @param url: url
 * @return: real url(absolute path ) 
 */
string going_make_url(const string& url)
{
	
}

/*
 * @brief: set file descriptor non-blocking.
 * @param fd: file descriptor.
 * @return: None
 */
void going_set_nonblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0){
		log_err("set non-blocking failed.");
		exit(-1);
	}

	flags |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flags);
	if(ret < 0){
		log_err("set non-blocking failed.");
		exit(-1);
	}
}

/*
 * Socket wrapper function 
 */
int going_socket(int domain, int type, int protocol)
{
	int listen_fd;
	if( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		log_err("socket create failed.");
		exit(-1);
	}
	return listen_fd;
}

void going_listen(int sockfd, int backlog)
{
	if(listen(sockfd, backlog) == -1){
		log_err("listen failed.");
		exit(-1);
	}
}

void going_bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	if(bind(sockfd, addr, addrlen) < 0){
		log_err("bind failed.");
		exit(-1);
	}
}

int going_accept(int sockfd, struct sockaddr* addr, socklen_t addrlen)
{
	
}

/*
 * Memeroy wrapper function
 */
void *going_calloc(size_t nmemb, size_t size)
{
	void *ptr = calloc(nmemb, size);
	if(NULL == ptr){
		log_err("going_calloc failed.");
		exit(-1);
	}
	return ptr;
}

void *going_malloc(size_t size)
{
	void *ptr = malloc(size);
	if(NULL == ptr){
		log_err("going_malloc failed.");
		exit(-1);
	}
	return ptr;
}

void going_free(void *ptr)
{
	free(ptr);
}