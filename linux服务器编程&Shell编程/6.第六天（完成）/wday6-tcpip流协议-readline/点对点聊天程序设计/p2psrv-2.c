
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void handle(int num)
{
	printf("recv num:%d \n",num);
	exit(0);
}


int main()
{
	int sockfd = 0;
	
    
 	signal(SIGUSR1,  handle);


	sockfd =  socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("fun socket\n");
		exit(0);
	}
	
	struct sockaddr_in  srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("192.168.6.249"); //127.0.0.1
	//srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //�󶨱���������һ����ַ 
	
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	
	if ( bind( sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		perror("fun bind\n");
		exit(0);
	}
	
	//һ������listen����������׽���sockfd����ɱ����׽���;ֻ�ܽ������ӣ����������ķ�������
	//listen �����������С�����������
	if ( listen(sockfd, SOMAXCONN)<0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	
	//struct sockaddr peeraddr;  //ͨ��ip
	//socklen_t perrlen ;
	
	struct sockaddr_in  peeraddr; //tcpip��ַ�ṹ
	socklen_t peerlen = sizeof(peeraddr);
	
	unsigned int conn = 0;
	//accept ����һ���µ����� ������µ�������һ�������׽���
	conn = accept(sockfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (conn == -1)
	{
		perror("fun listen\n");
		exit(0);
	}
	printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
	
	int pid;
	pid = fork();
	if (pid > 0)
	{
			char revbuf[1024] = {0};
			while (1)
			{
				int ret = read(conn, revbuf, sizeof(revbuf));
				if (ret == 0)
				{
					//����ڶ��Ĺ����У��Է��Ѿ��رգ�tcpipЭ��᷵��һ��0���ݰ�
					printf("�Է��ѹر�\n");
					
					break;
				}
				else if (ret < 0)
				{
					perror("������ʧ��\n"); 
					exit(0);
				}
				//ssize_t write(int fd, const void *buf, size_t count);
				fputs(revbuf, stdout); //���������յ����ݣ���ӡ��Ļ
				
				memset(revbuf, 0, sizeof(revbuf));
			}
			close(conn);
			//kill(pid , SIGUSR1); 
			sleep(1);
	
	}
	else
	{
		//char revbuf[1024] = {0};
		char sendbuf[1024] = {0};
		while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			//��ͻ���д����
			write(conn, sendbuf, strlen(sendbuf)); 
			memset(sendbuf, 0, sizeof(sendbuf));
		}
	}
	   

	close(sockfd);
	
	return 0; 	
}






 