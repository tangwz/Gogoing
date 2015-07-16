/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    utility.h
 * Data:         2015/6/3
 * Blog:         tangwz.com
 */

#ifndef _UTILITY_H_
#define _UTILITY_H_

using namespace std;

/*
 * @brief: Get system time.
 * @param: None.
 * @return: System time,e.g. Thu, 16 Jul 2015 11:28:35 GMT.
 */
string going_time_get();

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
 * @brief: Get file length
 * @param path: file path + file name
 * @return: file length
 */
int going_get_file_length(const char *path);

/*
 * Socket wrapper function 
 */
int going_socket(int domain, int type, int protocol);
void going_listen(int sockfd, int backlog);
void going_bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
int going_accept(int sockfd, struct sockaddr* addr, socklen_t addrlen);


#endif