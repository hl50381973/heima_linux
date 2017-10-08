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
	mkfifo("fifop", 0644);
	int infd;
	
	//��ֻ����ʽ �������ļ�
	infd = open("./1.txt", O_RDONLY);
	if (infd == -1)
	{
		printf("infd open() err..\n");
		exit(0);
	}
	int outfd;
	
	//�򿪹ܵ��ļ���׼��д����
	//outfd = open("fifop", O_WRONLY | O_NONBLOCK);
	outfd = open("fifop", O_WRONLY ); //����ģʽ
	if (outfd == -1)
	{
		printf("infd open() err..\n");
		exit(0);
	}
	
	char buf[1024];
	int n = 0;
	//���ļ��ж�����д��ܵ��ļ�
	while ( (n = read(infd, buf, 1024)) > 0)
	{
		write(outfd, buf, n);	
	}
	close (infd);
	close (outfd);
	

	printf("fifow д�ܵ��ļ� success\n");
	return 0;
}
