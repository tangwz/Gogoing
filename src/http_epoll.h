/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_epoll.h
 * Data:         2015/6/3
 * Blog:         tangwz.com
 */

#ifndef _HTTP_EPOLL_H_
#define _HTTP_EPOLL_H_

#include <sys/epoll.h>

#define MAXEVENTS 1024

/*
 * Epoll wrapper function
 * Descriptions: http://man7.org/linux/man-pages/man7/epoll.7.html
 */
int going_epoll_create(int flags);
void going_epoll_add(int epfd, int fs, struct epoll_event *event);
void going_epoll_mod(int epfd, int fs, struct epoll_event *event);
void going_epoll_del(int epfd, int fs, struct epoll_event *event);
int going_epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout);


#endif