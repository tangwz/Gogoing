/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_epoll.cpp
 * Data:         2015/6/6
 * Blog:         tangwz.com
 */

#include "http_epoll.h"
#include "Gogoing_dbg.h"
#include "Gogoing_util.h"

struct epoll_event *events;

/*
 *  epoll_create1() returns a file descriptor referring to the new epoll instance.  
 *  This file descriptor is used for all the subsequent calls to the epoll interface. 
 */
int going_epoll_create(int size)
{
	//
	int fd = epoll_create(size);
	check(fd > 0, "going_epoll create failed.");

	// events = (struct epoll_event *)going_malloc(sizeof(struct epoll_event) * MAXEVENTS);
	return fd;
}

/*
 * @about epoll_ctl: http://man7.org/linux/man-pages/man2/epoll_ctl.2.html
 * @Synopsis: int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 */
void going_epoll_add(int epfd, int fd, struct epoll_event* event)
{
	int rc = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, event);
	check(rc == 0, "going_epoll_add failed.");
	return;
}

void going_epoll_mod(int epfd, int fd, struct epoll_event* event)
{
	int rc = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, event);
	check(rc == 0, "going_epoll_mod failed.");
	return;
}

void going_epoll_del(int epfd, int fd, struct epoll_event* event)
{
	int rc = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, event);
	check(rc == 0, "going_epoll_del failed.");
	return;
}

int going_epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout)
{
again:
	int n = epoll_wait(epfd, events, maxevents, timeout);
	check(n > 0, "going_epoll_wait failed.");
	if(errno != EINTR){
		perror("epoll_wait");
		exit(-1);
	}
	else
		goto again;
	
	return n;
}