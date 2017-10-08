#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

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



void  myhandle(int num)
{
	if (num == SIGINT)
	{
		printf("recv signal SIGINT \n");
	}
	else if (num == SIGUSR1)
	{
		printf("recv signal SIGINT \n");
	}
	else
	{
		printf("recv signal id num : %d \n", num);
	}
}


int main(void)
{

	pid_t 	pid;
	printf("main ....begin\n");
	
	if (signal(SIGINT, myhandle) == SIG_ERR)
	{
		perror("func signal err\n");
		return 0;
	} 
	if (signal(SIGUSR1, myhandle) == SIG_ERR)
	{
		perror("func signal err\n");
		return 0;
	} 
	
	pid = fork();
	if (pid == -1)
	{
		printf("fork err....\n");
		return 0;
	}
	
	//�ӽ����򸸽��̷����ź�
	//�ӽ�����ͬ����̷����ź�
	/*
	if (pid == 0)
	{
		//pid = getpgrp();
		pid = getppid();
		//kill(pid, SIGUSR1); //���ϵ����ź�
		kill(0, SIGUSR1); //������鷢�ź�
		//killpg(pid, SIGUSR1);
		exit(0);
	}
	*/
	
	
	//�ӽ����򸸽��̷����ź�
	//�ӽ�����ͬ����̷����ź�
	if (pid == 0)
	{
		pid = getpgrp();
		killpg(pid, SIGUSR1);
		exit(0);
	}
	
	int n = 3;
	
	do 
	{
		printf("�����̿�ʼ˯��\n");
		n = sleep(n);
		printf("�����̿�ʼ����\n");
	} while (n > 0);
	
	//sleep(n);
	//��˼��sleep���������� man sleep
	printf("sleep ����ִ������Ժ󷵻�...\n");
	return 0;

}

