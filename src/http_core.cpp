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

		cout<<"parsed http request packet: "<<endl;
		going_print_http_header(phttphdr);

		string out;
		int http_code = going_do_http_header(phttphdr, out);

		/* debug */
		cout << "http response packet: " << out << endl;

		char *out_buff = (char *)going_malloc(out.size());
		if(out_buff == NULL)
			goto out;
		for(int i = 0; i != out.size(); ++i)
			out_buff[i] = out[i];
		out_buff[i] =  '\0';

		int nwrite = 0; n = 0;
		if(http_codes == GOING_HTTP_BAD_REQUEST     ||
		   http_codes == GOING_HTTP_NOT_IMPLEMENTED ||
		   http_codes == GOING_HTTP_NOT_FOUND       ||
		   (http_codes == GOING_HTTP_OK && phttphdr->method == "HEAD")){
			while((n = write(conn_sock, out_buff + nwrite, i)) != 0){
				if(n == -1){
					if(errno == EINTR)
						continue;
					else
						goto out;
				}
				nwrite += n;
			}
		}
		if(http_codes == GOING_HTTP_OK){
			if(phttphdr->method == "GET"){
				while((n = write(conn_sock, out_buff + nwrite, i)) != 0){
					cout << n << endl;
					if(n == -1){
						if(errno == EINTR)
							continue;
						else
							goto out;
					}
					nwrite += n;
				}
				string real_url = going_make_url(phttphdr->url);
				int fd = open(real_url.c_str(), O_RDONLY);
				int file_size = going_get_file_length(real_url.c_str());
				cout << "file size: "<< file_size << endl;
				int nwrite = 0;
				cout << "sendfile: " << real_url.c_str() <<endl;
			again:
				if((sendfile(conn_sock, fd, (off_t *)&nwrite, file_size)) < 0)
					perror("sendfile");
				if(nwrite < file_size)
					goto again;
				cout << "sendfile ok: " << nwrite << endl;
			}
		}
		free(out_buff);
		//timeout 4min
		nfds = going_epoll_wait(epollfd, events, 2, TIMEOUT);
		if(0 == nfds)//timeout
			goto out;
		for(int i = 0; i < nfds; ++i){
			if(events[i].data.fd == conn_sock)
				goto begin;
			else
				goto out;
		}
	}

out:
	going_free_http_header(phttphdr);
	close(conn_sock);
	going_thread_num_minus();
	printf("NO.%d thread ends now...\n", (unsigned int)tid);
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

/*
 * @brief: 根据解析下来的going_http_header_t来处理客户的请求.
 * @param phttpdr: 指向要处理的going_http_header_t; out: 保存处理的结果，http响应包.
 * @return: http status code.
 *
 * 目前支持的请求首部：
 * 目前支持的响应首部：
 */
int going_do_http_header(going_http_header_t *phttpdr, string& out)
{
	char status_line[256] = {0};
	string crlf("\r\n");
	string server("Server: Gogoing\r\n");
	string Public("Public: GET, HEAD\r\n");
	string content_base = "Content-base: " + going_domain + crlf;
	string date = "Date: " + going_time_get() + crlf;

	string content_length("Content-Length: ");
	string content_location("Content-Location: ");
	string last_modify("Last-Modify: ");

	if(phttphdr == NULL){
		snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d %s\r\n",
				GOING_HTTP_BAD_REQUEST, going_get_state_by_codes(GOING_HTTP_BAD_REQUEST));
		out = status_line + crlf;
		return GOING_HTTP_BAD_REQUEST;
	}

	string method = phttphdr->method;
	string real_url = going_make_url(phttphdr->url);
	string version = phttphdr->version;
	if(method == "GET" || method == "HEAD"){
		if(going_is_file_existed(real_url.c_str()) == -1){
			snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d %s\r\n",
					GOING_HTTP_NOT_FOUND, going_get_state_by_codes(GOING_HTTP_NOT_FOUND));
			out += status_line;	
			return GOING_HTTP_NOT_FOUND;
		}else{
			int len = going_get_file_length(real_url.c_str());
			snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d %s\r\n",
					GOING_HTTP_OK, going_get_state_by_codes(GOING_HTTP_OK));
			out += status_line;
			snprintf(status_line, sizeof(status_line), "%d\r\n", len);
			out += content_length + status_line;
			out += server + content_base + date;
			out += last_modify + going_get_file_modified_time(real_url.c_str()) + crlf;
		}
	}
	else if(method == "PUT"){
		snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d %s\r\n",
				GOING_HTTP_NOT_IMPLEMENTED, going_get_state_by_codes(GOING_HTTP_NOT_IMPLEMENTED));
		out += status_line + server + Public + date + crlf;
		return GOING_HTTP_NOT_IMPLEMENTED;
	}
	else if(method == "DELETE"){
		snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d %s\r\n",
				GOING_HTTP_NOT_IMPLEMENTED, going_get_state_by_codes(GOING_HTTP_NOT_IMPLEMENTED));
		out += status_line + server + Public + date + crlf;
		return GOING_HTTP_NOT_IMPLEMENTED;
	}
	else{
		snprintf(status_line, sizeof(status_line), "HTTP/1.1 %d %s\r\n",
				GOING_HTTP_BAD_REQUEST, going_get_state_by_codes(GOING_HTTP_BAD_REQUEST));
		out = status_line + crlf;
		return GOING_HTTP_BAD_REQUEST;
	}

	return GOING_HTTP_OK;
}