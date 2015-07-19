/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    Gogoing_util.h
 * Data:         2015/6/3
 * Blog:         tangwz.com
 */

#ifndef _GOGOING_UTIL_H_
#define _GOGOING_UTIL_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <strings.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

/*
 * @brief: Get system time.
 * @param: None.
 * @return: System time,e.g. Thu, 16 Jul 2015 11:28:35 GMT.
 */
string going_time_get();

/*
 * @brief: 根据http请求包中的url和配置文件中的docroot配置选项构造url
 * @param url: url
 * @return: real url(absolute path ) 
 */
string going_make_url(const string& url);

/*
 * @brief: Parse config file.
 * @param path: file path + file name
 * @return: parse failed, -1; parse succeed, 0.
 */
int going_parse_config(const char *path);

/*
 * @brief: Get file length
 * @param path: file path + file name
 * @return: file length
 */
int going_get_file_length(const char *path);

/*
 * @brief: set file descriptor non-blocking.
 * @param fd: file descriptor.
 * @return: None
 */
void going_set_nonblocking(int fd);

/*
 * @brief: unix-style error
 */
void unix_error(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	/* exit(0); */
}

/*
 * @brief: posix-style error
 */
void posix_error(int code, char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, strerror(code));
	/* exit(0) */
}

/*
 * Socket wrapper function 
 */
int going_socket(int domain, int type, int protocol);
void going_listen(int sockfd, int backlog);
void going_bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int going_accept(int sockfd, struct sockaddr* addr, socklen_t addrlen);

/*
 * Memeroy wrapper function
 */
void *going_calloc(size_t nmemb, size_t size);
void *going_malloc(size_t size);
void going_free(void *ptr);

#endif