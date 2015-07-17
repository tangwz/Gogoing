/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    Gogoing_dbg.h
 * Data:         2015/6/25
 * Blog:         tangwz.com
 */

#ifndef _GOGOING_DBG_H_
#define _GOGOING_DBG_H_

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/*
 * @brief: The Debug Macros
 * @from: http://c.learncodethehardway.org/book/ex20.html
 */
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

/* get a safe readable version of errno. */
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

/* The log_err, log_warn, and log_info works like debug but can't be compiled out. */
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); /* exit(-1) */ }

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); /* exit(1); */}

#endif