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

#endif