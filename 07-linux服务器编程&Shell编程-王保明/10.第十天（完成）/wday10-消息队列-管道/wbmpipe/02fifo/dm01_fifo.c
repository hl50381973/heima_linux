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
#include <sys/time.h>

/*
���ùܵ����������̼�����ļ����ơ�
fifow
���ļ�1.txt	
	д��ܵ�
fifor	
	���ܵ�
	д��2.txt�ļ�
	*/

int main(int argc, char *argv[])
{
	//�����ܵ��ļ�
	int outfd = 0;
	
	//�򿪹ܵ��ļ���׼��д����
	outfd = open("fifop", O_WRONLY | O_NONBLOCK);
	//outfd = open("pp", O_WRONLY ); //����ģʽ
	if (outfd == -1)
	{
		printf("infd open() err..\n");
		exit(0);
	}
	
	return 0;
}


	/*
	   int fcntl(int fd, int cmd);
	 int fcntl(int fd, int cmd, long arg);
	
	int flags = 0;
	flags = fcntl(infd, F_GETFD);
	flags = flags | O_NONBLOCK;
	
	fcntl(infd, F_SETFD, flags);
	 */