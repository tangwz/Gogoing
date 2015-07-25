/*
 * Gogoing -- A mini HTTP Server
 * Copyright (C) 2015 Tangwz. All rights reserved.
 * 
 * Author:       Tangwz
 * File Name:    http_core.cpp
 * Data:         2015/6/5
 * Blog:         tangwz.com
 */

#include "http_core.h"


/* set timeout.(milliseconds) */
 #define TIMEOUT 1000*60*4
/*
 * @brief: Handle client's thread.
 * @param param: Client's conn_sock.
 * @return: None.
 */
void *going_thread_func(void *param)
{
	going_thread_num_add();

	going_http_header_t *phttphdr = going_alloc_http_header();

	_epollfd_connfd *ptr_fd = (_epollfd_connfd *)param;
	/* get socket fd. */
	int conn_sock = ptr_fd->connfd;

	struct epoll_event ev, events[2];
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = conn_sock;
	//客户连接的新epollfd
	int epollfd = going_epoll_create(2);
	going_epoll_add(epollfd, conn_sock, &ev);
	int nfds = 0;

	pthread_t tid = pthread_self();
	printf("NO.%u thread runs now !!!\n", (unsigned int)tid);

	/* Nginx默认http请求包大小为1M，所以也分配1M缓存来存http请求包 */
	char *buff = (char *)going_malloc(ONEMEGA);
	bzero(buff, ONEMEGA);

	//关闭connfd的Nagle算法
	going_set_off_tcp_nagle(conn_sock);
	//设置接收超时时间为60秒
	going_set_recv_timeo(conn_sock, 60, 0);

begin:
	int32_t nread = 0, n = 0;
	for(;;){
		if((n = read(conn_sock, buff + nread, ONEMEGA - 1)) > 0)
			nread += n;
		else if(0 == n)
			break;
		else if(-1 == n && errno == EINTR)
			continue.
		else if(-1 == n && errno == EAGAIN)
			break;
		else if(-1 == n && errno == EWOULDBLOCK){
			perror("socket read timeout.");
			goto out;
		}else{
			perror("read http request error.");
			going_free(buff);
			break;
		}
	}

	if(0 != nread){
		string str_http_request(buff, buff + nread);

		if(!going_parse_http_request(str_http_request, phttphdr)){
			perror("going_parse_http_request: parse str_http_request failed.");
			goto out;
		}

		cout<<"parsed http request packet:"<<endl;
		going_print_http_header(phttphdr);

		string out;
		int http_code = going_do_http_header(phttphdr, out);


	}
}

/*
 * @brief: going_thread_num add(Atomicity) 1.
 * @param: None.
 * @return: None.
 */
void going_thread_num_add()
{
	pthread_mutex_lock(&going_thread_num_mutex);
	++going_thread_num;
	pthread_mutex_unlock(&going_thread_num_mutex);
}

/*
 * @brief: going_thread_num minus(Atomicity) 1.
 * @param: None.
 * @return: None.
 */
void going_thread_num_minus()
{
	pthread_mutex_lock(&going_thread_num_mutex);
	--going_thread_num;
	pthread_mutex_unlock(&going_thread_num_mutex);
}

/*
 * @brief: Read going_thread_num(Atomicity).
 * @param: None.
 * @return: The value of going_thread_num.
 */
int32_t going_thread_num_read();