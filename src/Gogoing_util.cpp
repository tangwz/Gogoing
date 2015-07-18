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