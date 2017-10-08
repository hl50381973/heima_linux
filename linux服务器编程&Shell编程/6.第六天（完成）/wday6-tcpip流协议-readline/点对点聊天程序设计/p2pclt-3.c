
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
	int  sockfd = 0;
	
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
	
    if ( connect(sockfd, (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) < 0)
    {
    	printf("errno:%d \n", errno);
    	perror("fun socket\n");
		exit(0);
    }
    
    int pid = 0;
    pid = fork();
    if (pid > 0) //������
    {		
    	
		//char revbuf[1024] = {0};
		char sendbuf[1024] = {0};
		while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			//�����д����
			write(sockfd, sendbuf, strlen(sendbuf)); //�������˻ط���Ϣ
			memset(sendbuf, 0, sizeof(sendbuf));
		}

    }
    else
    {
    	char revbuf[1024] = {0};
    	while(1)
    	{
    		int ret = 0;
    		//�ӷ�����������
			ret = read(sockfd, revbuf, sizeof(revbuf));	
			if (ret < 0)
			{
				printf("read err\n");
				break;
			}
			if (ret == 0)
			{
				printf("read err\n");
				break;
			}
			//
			fputs(revbuf, stdout); //�ӷ������յ����ݣ���ӡ��Ļ
			memset(revbuf, 0, sizeof(revbuf));
    	}
    	close(sockfd);
    	kill(getppid(), SIGUSR1);
    	exit(0);
    	
    }
	   

	close(sockfd);
	
	return 0; 	
}

     




 