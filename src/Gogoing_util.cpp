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

/*
 * @brief: Get system time.
 * @param: None.
 * @return: System time,e.g. Thu, 16 Jul 2015 11:28:35 GMT.
 */
string going_time_get()
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
		perror("socket create failed.");
		exit(-1);
	}
	return listen_fd;
}

void going_listen(int sockfd, int backlog)
{
	if(listen(sockfd, backlog) == -1){
		perror("listen failed.");
		exit(-1);
	}
}

void going_bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	if(bind(sockfd, addr, addrlen) < 0){
		perror("bind failed.");
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
		debug("going_calloc failed.");
		exit(-1);
	}
	return ptr;
}

void *going_malloc(size_t size)
{
	void *ptr = malloc(size);
	if(NULL == ptr){
		debug("going_malloc failed.");
		exit(-1);
	}
	return ptr;
}

void going_free(void *ptr)
{
	free(ptr);
}