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

/* parse config file and get key-value.e.g. key=docroot value=/home/zxh/desktop/code/webserver */
map<string, int> going_config_keyword_map;

/*
 * @brief: Get system time.
 * @param: None.
 * @return: System time,e.g. Thu, 16 Jul 2015 11:28:35 GMT.
 */
string going_time_get()
{
	time_t now;
	struct tm* time_now;
	string str_time;

	time(&now);
	time_now = localtime(&now);

	switch(time_now->tm_wday)
	{
		case 0:
			str_time += "Sun, ";
			break;
		case 1:
			str_time += "Mon, ";
			break;
		case 2:
			str_time += "Tue, ";
			break;
		case 3:
			str_time += "Wed, ";
			break;
		case 4:
			str_time += "Thu, ";
			break;
		case 5:
			str_time += "Fri, ";
			break;
		case 6:
			str_time += "Sat, ";
			break;
	}
	char buf[16];
	snprintf(buf, sizeof(buf), "%d ", time_now->tm_mday);
	str_time += string(buf);
	switch(time_now->tm_mon)
	{
		case 0:
			str_time += "Jan ";
			break;
		case 1:
			str_time += "Feb ";
			break;
		case 2:
			str_time += "Mar ";
			break;
		case 3:
			str_time += "Apr ";
			break;
		case 4:
			str_time += "May ";
			break;
		case 5:
			str_time += "Jun ";
			break;
		case 6:
			str_time += "Jul ";
			break;
		case 7:
			str_time += "Aug ";
			break;
		case 8:
			str_time += "Sep ";
			break;
		case 9:
			str_time += "Oct ";
			break;
		case 10:
			str_time += "Nov ";
			break;
		case 11:
			str_time += "Dec ";
			break;
	}
	snprintf(buf, sizeof(buf), "%d", time_now->tm_year + 1900);
	str_time += string(buf);
	snprintf(buf, sizeof(buf), "%d:%d:%d ", time_now->tm_hour, time_now->tm_min, time_now->tm_sec);
	str_time += string(buf);

	str_time += "GMT";

	return str_time;
}

/*
 * @brief: 根据http请求包中的url和配置文件中的docroot配置选项构造url
 * @param url: url
 * @return: real url(absolute path ) 
 */
string going_make_url(const string& url)
{
	string real_url, url2;

	int n = 0;

	if((n = url.find(going_domain, 0)) != string::npos)//url中包含域名，要将其删去
		url2 = url.substr(going_domain.size(), url.size() - going_domain.size());
	else
		url2 = url;

	if(going_docroot[going_docroot.size() - 1] == '/')//配置项docroot末尾有'/'
	{
		if(url2[0] == '/')
			real_url = going_docroot + url2.erase(0, 1);
		else
			real_url = going_docroot + url2;
	}else{//配置项末尾没有'/'
		if(url2[0] == '/')
			real_url = going_docroot + url2;
		else
			real_url = going_docroot + '/' + url2;
	}

	return real_url;
}

/*
 * @brief: Get file length
 * @param path: file path + file name
 * @return: file length
 */
int going_get_file_length(const char *path)
{
	struct stat buf;
	/*
	 * stat函数：通过文件名获取文件信息，并保存在buf所指的结构体stat中 
	 * http://linux.die.net/man/2/stat
	 */
	int ret = stat(path, &buf);
	if(ret == -1){
		log_err("going_get_file_length failed.");
		exit(-1);
	}
	return (int)buf.st_size;
}

/*
 * @brief: Get file modified time
 * @param path: file path + file name
 * @return: file modified time
 */
string going_get_file_modified_time(const char *path)
{
	struct stat buf;
	int ret = stat(path, &buf);
	if(ret == -1){
		log_err("going_get_file_modified_time failed.");
		exit(-1);
	}
	char array[32] = {0};
	snprintf(array, sizeof(array), "%s", ctime(&buf.st_mtime));
	return string(array, array + strlen(array));
}

/*
 * @brief: 初始化全局变量going_config_keyword_map，必须在使用going_config_keyword_map前调用此函数
 * @param: None
 * @return: None
 */
void going_init_config_keyword_map()
{
	going_config_keyword_map.insert(make_pair("docroot", GOING_DOCROOT));
	going_config_keyword_map.insert(make_pair("domain", GOING_DOCROOT));
}

/*
 * @brief: Parse config file.
 * @param path: file path + file name
 * @return: parse failed, -1; parse succeed, 0.
 */
int going_parse_config(const char *path)
{
	going_init_config_keyword_map();
	fstream infile(path, fstream::in);
	string line, word;
	if(!infile){
		printf("%s can't open\n", path);
		infile.close();
		return -1;
	}
	while(getline(infile, line))
	{
		stringstream stream(line);
		stream >> word;//keyword
		map<string, int>::const_iterator cit = going_config_keyword_map.find(word);
		if(cit == going_config_keyword_map.end()){
			printf("can't find keyword.\n");
			infile.close();
			return -1;
		}
		switch(cit -> second){
			case GOING_DOCROOT:
				stream >> going_docroot;
				break;
			case GOING_DOMAIN:
				stream >> going_domain;
				break;
			default:
				infile.close();
				return -1;
		}
	}
	infile.close();
	return 0;
}

/*
 * @brief: set file descriptor non-blocking.
 * @param fd: file descriptor.
 * @return: None
 */
void going_set_nonblocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if(flags < 0){
		log_err("set non-blocking failed.");
		exit(-1);
	}

	flags |= O_NONBLOCK;
	int ret = fcntl(fd, F_SETFL, flags);
	if(ret < 0){
		log_err("set non-blocking failed.");
		exit(-1);
	}
}

/*
 * @brief：设置套接字SO_REUSEADDR选项
 * @param sockfd：要设置的套接字
 * @return: None
 */
 void going_set_reuse_addr(int sockfd)
 {
 	int on = 1;
 	int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
 	if(ret < 0){
 		log_err("setsockopt: SO_REUSEADDR failed.");
 		exit(-1);
 	}
 }

/*
 * @brief：开启套接字TCP_NODELAY选项，关闭nagle算法
 * @param sockfd：要设置的套接字
 * @return: None
 */
 void going_set_off_tcp_nagle(int sockfd)
 {
 	int on = 1;
 	int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
 	if(ret < 0){
 		log_err("setsockopt: TCP_NODELAY ON failed.");
 		exit(-1);
 	} 	
 }

/*
 * @brief：关闭套接字TCP_NODELAY选项，开启nagle算法
 * @param sockfd：要设置的套接字
 * @return: None.
 */
 void going_set_on_tcp_nagle(int sockfd)
 {
 	int off = 0;
 	int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &off, sizeof(off));
 	if(ret < 0){
 		log_err("setsockopt: TCP_NODELAY OFF failed.");
 		exit(-1);
 	}
 }

/*
 * @brief：开启套接字TCP_CORK选项
 * @param sockfd：要设置的套接字
 * @return: None.
 */
 void going_set_on_tcp_cork(int sockfd)
 {
 	int on = 1;
 	int ret = setsockopt(sockfd, SOL_TCP, TCP_CORK, &on, sizeof(on));
 	if(ret < 0){
 		log_err("setsockopt: TCP_CORK ON failed.");
 		exit(-1);
 	}
 }

/*
 * @brief：关闭套接字TCP_CORK选项
 * @param sockfd：要设置的套接字
 * @return: None.
 */
 void going_set_off_tcp_cork(int sockfd)
 {
 	int off = 0;
 	int ret = setsockopt(sockfd, SOL_TCP, TCP_CORK, &off, sizeof(off));
 	if(ret < 0){
 		log_err("setsockopt: TCP_CORK OFF failed.");
		exit(-1);
 	}
 }

/*
 * @brief：设置套接字SO_RCVTIMEO选项，接收超时
 * @param：sockfd要设置的套接字, sec秒, usec毫秒
 * @return: None.
 */
 void going_set_recv_timeo(int sockfd, int sec, int usec)
 {
 	struct timeval time = {sec, usec};
 	int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time));
 	if(ret < 0){
 		log_err("setsockopt: SO_RCVTIMEO failed.");
 		exit(-1);
 	}
 }

/*
 * @brief：设置套接字SO_SNDTIMEO选项，发送超时
 * @param：sockfd要设置的套接字, sec秒, usec毫秒
 * @return: None.
 */
 void going_set_snd_timeo(int sockfd, int sec, int usec)
 {
 	struct timeval time = {sec, usec};
 	int ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &time, sizeof(time));
 	if(ret < 0){
 		log_err("setsockopt: SO_SNDTIMEO failed.");
 		exit(-1);
 	}
 }


/*
 * Socket wrapper function 
 */
int going_socket(int domain, int type, int protocol)
{
	int listen_fd;
	if( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		log_err("socket create failed.");
		exit(-1);
	}
	return listen_fd;
}

void going_listen(int sockfd, int backlog)
{
	if(listen(sockfd, backlog) == -1){
		log_err("listen failed.");
		exit(-1);
	}
}

void going_bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
	if(bind(sockfd, addr, addrlen) < 0){
		log_err("bind failed.");
		exit(-1);
	}
}

int going_accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
	int ret_fd = 0;
	for(;;){
		ret_fd = accept(sockfd, addr, addrlen);
		if(ret_fd > 0)
			break;
		else if(ret_fd == -1){
			//由于我们把监听套接字设置为了非阻塞模式
			if(errno != EAGAIN && errno != EPROTO && 
				errno != EINTR && errno != ECONNABORTED){
				log_err("accept failed.");
				exit(-1);
			}
		}else{
			continue;
		}
	}
	return ret_fd;
}

struct servent* going_getservbyname(const char* name, const char* proto)
{
	struct servent* pservent;
	if((pservent = getservbyname(name, proto)) == NULL){
		log_err("getservbyname failed.");
		exit(-1);
	}
	return pservent;
}

/*
 * Memeroy wrapper function
 */
void *going_calloc(size_t nmemb, size_t size)
{
	void *ptr = calloc(nmemb, size);
	if(NULL == ptr){
		log_err("going_calloc failed.");
		exit(-1);
	}
	return ptr;
}

void *going_malloc(size_t size)
{
	void *ptr = malloc(size);
	if(NULL == ptr){
		log_err("going_malloc failed.");
		exit(-1);
	}
	return ptr;
}

void going_free(void *ptr)
{
	free(ptr);
}