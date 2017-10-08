#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char*) buf;

	while (nleft > 0)
	{
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nread == 0)
			return count - nleft;

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwritten;
	char *bufp = (char*) buf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nwritten == 0)
			continue;

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

//maxline 一行最大数
//先提前peek一下缓冲区，如果有数据从缓冲区的读数据，
//1、缓冲区数据中带\n
//2 缓存区中不带\n
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;
	char *bufp = buf;
	int nleft = maxline;
	while (1)
	{
		//看一下缓冲区有没有数据，并不移除内核缓冲区数据
		ret = recv_peek(sockfd, bufp, nleft);
		if (ret < 0) //失败 
			return ret;
		else if (ret == 0) //对方已关闭
			return ret;

		nread = ret;
		int i;
		for (i = 0; i < nread; i++)
		{
			if (bufp[i] == '\n') //若缓冲区有\n
			{
				ret = readn(sockfd, bufp, i + 1); //读走数据
				if (ret != i + 1)
					exit(EXIT_FAILURE);

				return ret; //有\n就返回，并返回读走的数据
			}
		}

		if (nread > nleft) //如果读到的数大于 一行最大数 异常处理
			exit(EXIT_FAILURE);

		nleft -= nread;  ////若缓冲区没有\n, 把剩余的数据读走
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			exit(EXIT_FAILURE);

		bufp += nread; //bufp指针后移后，再接着偷看缓冲区数据recv_peek，直到遇到\n
	}

	return -1;
}

void do_service(int conn)
{
	char recvbuf[1024];
	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));

		int ret = readline(conn, recvbuf, 1024);
		if (ret == -1)
			ERR_EXIT("readline");
		if (ret == 0)
		{
			printf("client close\n");
			break;
		}

		fputs(recvbuf, stdout);
		//将接受的数据再发送
		writen(conn, recvbuf, strlen(recvbuf));
	}
}

int main(void)
{
	int listenfd; //监听socket的文件描述符
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		/*	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)*/
		ERR_EXIT("socket");
	//TCP/IP 结构体
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");*/
	/*inet_aton("127.0.0.1", &servaddr.sin_addr);*/

	//设置地址复用
	int on = 1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");
	//绑定
	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	//监听
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	//定义 socket信息结构体，在新的链接中会返回新的已经链接的socket的信息。
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;

	pid_t pid;
	while (1)
	{
		if ((conn = accept(listenfd, (struct sockaddr*) &peeraddr, &peerlen))
				< 0)
			ERR_EXIT("accept");
		printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr),
				ntohs(peeraddr.sin_port));
		pid = fork();
		if (pid == -1)
			ERR_EXIT("fork");
		if (pid == 0)
		{
			//关闭监听socket
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		} else
			//关闭链接socket
			close(conn);
	}

	return 0;
}
