# Gogoing
>"What I cannot create, I do not understand."

>-- Richard Feynman

自己重新造的轮子，一个轻量型Http Server，用于综合考察学习了大半年的Linux服务器编程，理解服务器编程的本质。

## 一次HTTP请求
![Gogoing](https://github.com/tangwz/WebServer/blob/master/docs/WebServer.png)
大致就是：用户访问某个页面，浏览器（IE、Firefox、Opera……）会向Web服务器（端口一般是80）发送一条HTTP请求，端口接受到请求后开始解析请求，服务器会去寻找所期望的对象（HTML页面、JPEG图片、文本文件、MPEG电影、WAV音频文件、Java小程序和其它资源，统称为“文件”），并向客户端返回一个[状态码](https://zh.wikipedia.org/wiki/HTTP%E7%8A%B6%E6%80%81%E7%A0%81)，如果成功就把文件读取后发送给客户端，如果失败就发送错误消息或其它信息，然后关闭连接，请求结束。

## Programming Model

 - Non-blocking I/O
 - Epoll
 - Multithreading

## Support(Completed)

 - Support GET/HEAD/POST method
 - Sendfile
 - Conf file

## To-do

 - Support Keep-Alive
 - Add command line parameter
 - Dynamic content
 - Other HTTP/1.1 features
 - Memory pool
 - Close socket when timeout
 - Support PHP
 - *Lua interpreter*

**可以写得更C++一点！！！**
## 参考资料：
[《HTTP权威指南》](http://book.douban.com/subject/10746113/)

[《UNIX网络编程 卷1：套接字联网API（第3版）》](http://book.douban.com/subject/4859464/)

[《UNIX网络编程 卷2》](http://book.douban.com/subject/4118577/)
