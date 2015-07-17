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

struct epoll_event *events;

int going_epoll_create(int flags)
{
	int fd = epoll_create1(flags);
	check(fd > 0, "epoll create");

	events = (struct epoll_event *)malloc()
}