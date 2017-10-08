
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
     
/*
       int socket(int domain, int type, int protocol);
       */
       
 /*
int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);

			//man 2 bind
         struct sockaddr {
             sa_family_t sa_family;
             char        sa_data[14];
         }
         
         //man 7 ip
           struct sockaddr_in {
             sa_family_t    sin_family; // address family: AF_INET 
             u_int16_t      sin_port;   // port in network byte order 
             struct in_addr sin_addr;   // internet address 
         };

         //Internet address. 
         struct in_addr {
             u_int32_t      s_addr;     //address in network byte order 
         };

*/

      //int listen(int sockfd, int backlog);
    /*   int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
      	       On success, accept() returns a non-negative integer that is a descriptor for the accepted socket.   On  error,
       -1 is returned, and errno is set appropriately.
     int conn = 0;
     */
int main()
{
	int sockfd = 0;
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
	conn = accept(sockfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (conn == -1)
	{
		perror("fun listen\n");
		exit(0);
	}
	   
	printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
	 
	char revbuf[1024] = {0};
	while (1)
	{
		int ret = read(conn, revbuf, sizeof(revbuf));
		if (ret == 0)
		{
			//����ڶ��Ĺ����У��Է��Ѿ��رգ�tcpipЭ��᷵��һ��0���ݰ�
			printf("�Է��ѹر�\n");
			exit(0);
		}
		else if (ret < 0)
		{
			
			perror("������ʧ��\n"); 
			exit(0);
		}
		//ssize_t write(int fd, const void *buf, size_t count);
		fputs(revbuf, stdout); //���������յ����ݣ���ӡ��Ļ
		write(conn, revbuf, ret); //�������˻ط�������
		
	}
	return 0; 	
}






 