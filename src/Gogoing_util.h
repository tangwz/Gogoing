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
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

extern string going_docroot;
#define GOING_DOCROOT 1
extern string going_domain;
#define GOING_DOMAIN  2

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
 * @brief: make sure file is existed
 * @param: path is absolute path + file name
 * @return: file isn't existed, -1;existed,others.
 */
inline int going_is_file_existed(const char* path)
{
	int ret = open(path, O_RDONLY | O_EXCL);
	close(ret);
	return ret;
}

/*
 * @brief: Get file length
 * @param path: file path + file name
 * @return: file length
 */
int going_get_file_length(const char *path);

/*
 * @brief: Get file modified time
 * @param path: file path + file name
 * @return: file modified time
 */
string going_get_file_modified_time(const char *path);

/*
 * @brief: 初始化全局变量going_config_keyword_map，必须在使用going_config_keyword_map前调用此函数
 * @param: None
 * @return: None
 */
void going_init_config_keyword_map();

/*
 * @brief: Parse config file.
 * @param path: file path + file name
 * @return: parse failed, -1; parse succeed, 0.
 */
int going_parse_config(const char *path);


/*
 * @brief: set file descriptor non-blocking.
 * @param fd: file descriptor.
 * @return: None
 */
void going_set_nonblocking(int fd);

/*
 * @brief：设置套接字SO_REUSEADDR选项
 * @param sockfd：要设置的套接字
 * @return: None
 */
 void going_set_reuse_addr(int sockfd);

/*
 * @brief：开启套接字TCP_NODELAY选项，关闭nagle算法
 * @param sockfd：要设置的套接字
 * @return: None
 */
 void going_set_off_tcp_nagle(int sockfd);

/*
 * @brief：关闭套接字TCP_NODELAY选项，开启nagle算法
 * @param sockfd：要设置的套接字
 * @return: None.
 */
 void going_set_on_tcp_nagle(int sockfd);

/*
 * @brief：开启套接字TCP_CORK选项
 * @param sockfd：要设置的套接字
 * @return: None.
 */
 void going_set_on_tcp_cork(int sockfd);

/*
 * @brief：关闭套接字TCP_CORK选项
 * @param sockfd：要设置的套接字
 * @return: None.
 */
 void going_set_off_tcp_cork(int sockfd);

/*
 * @brief：设置套接字SO_RCVTIMEO选项，接收超时
 * @param：sockfd要设置的套接字, sec秒, usec毫秒
 * @return: None.
 */
 void going_set_recv_timeo(int sockfd, int sec, int usec);

/*
 * @brief：设置套接字SO_SNDTIMEO选项，发送超时
 * @param：sockfd要设置的套接字, sec秒, usec毫秒
 * @return: None.
 */
 void going_set_snd_timeo(int sockfd, int sec, int usec);

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
struct servent* going_getservbyname(const char* name, const char* proto);

/*
 * Memeroy wrapper function
 */
void *going_calloc(size_t nmemb, size_t size);
void *going_malloc(size_t size);
void going_free(void *ptr);

#endif