/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_core.h
 * Data:         2015/6/3
 * Blog:         tangwz.com
 */

#ifndef _HTTP_CORE_H_
#define _HTTP_CORE_H_

#include "Gogoing_util.h"
#include "http_epoll.h"
#include "http_parse.h"
#include "http_request.h"

#define ONEKILO 1024
#define ONEMEGA 1024*ONEKILO
#define ONEGIGA 1024*ONEMEGA

/* 结构体将epoll描述符和socket描述符封装在一起*/
typedef struct _epollfd_connfd
{
	int epollfd;
	int connfd;
}_epollfd_connfd;

/* 监听队列最大数 */
#define MAX_BACKLOG 100

/* 保存配置文件相关值 */
string going_domain("");
string going_docroot("");

/*
 * @brief: Handle client's thread.
 * @param param: Client's conn_sock.
 * @return: None.
 */
void *going_thread_func(void *param);

/*
 * @brief: Number of threads.
 */
int32_t going_thread_num = 0;
pthread_mutex_t going_thread_num_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * @brief: going_thread_num add(Atomicity) 1.
 * @param: None.
 * @return: None.
 */
void going_thread_num_add();

/*
 * @brief: going_thread_num minus(Atomicity) 1.
 * @param: None.
 * @return: None.
 */
void going_thread_num_minus();

/*
 * @brief: Read going_thread_num(Atomicity).
 * @param: None.
 * @return: The value of going_thread_num.
 */
int32_t going_thread_num_read();

/*
 * @brief: 根据解析下来的going_http_header_t来处理客户的请求.
 * @param phttpdr: 指向要处理的going_http_header_t; out: 保存处理的结果，http响应包.
 * @return: http status code.
 *
 * 目前支持的请求首部：
 * 目前支持的响应首部：
 */
int going_do_http_header(going_http_header_t *phttpdr, string& out);

#endif