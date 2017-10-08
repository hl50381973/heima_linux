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



int main(int argc, char *argv[])
{	
	//��2.txt ׼��д����
	int outfd;
	outfd = open("./2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); //O_TRUNC ���ļ����
	if (outfd == -1)
	{
		printf("open()  2.txt err..\n");
		exit(0);
	}
	
	
	//�򿪹ܵ��ļ���׼������
	int infd;
	infd = open("fifop", O_RDONLY);
	if (infd == -1)
	{
		printf("open err ... \n");
		exit(0);
	}
		 
	char buf[1024];
	int n = 0;
	//���ļ��ж�����д��ܵ��ļ�
	while ( (n = read(infd, buf, 1024)) > 0)
	{
		write(outfd, buf, n);	
	}
	printf("���ܵ��ļ��Ƿ�ʧ�ܡ�������\n");
	close (infd);
	close (outfd);
	unlink("fifop");
	

	printf("fifor ���ܵ��ļ� success\n");
	return 0;
}
