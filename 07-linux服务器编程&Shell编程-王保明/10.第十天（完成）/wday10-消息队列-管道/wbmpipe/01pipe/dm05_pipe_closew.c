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


//�ܵ���д������ʾ

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)


//��ʾ�ܵ����˵�д�����رգ�������������0�����Ǵ���
int main51(void )
{
	int pipefd[2];
	pid_t pid;
	if (pipe(pipefd) == -1 )	
	{
		printf("pipe() err..\n");	
		return -1;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("fork err..\n");
		return -1;
	}
	
	if (pid == 0)
	{
		close(pipefd[1]);  //�ر��ӽ���д��
		printf("child .....quit\n");
		exit(0);
	} 
	else if (pid > 0 )
	{
		int len = 0; 
		char buf[100] = {0};
		close(pipefd[1]); //�رո�����д��
		
		//������sleep 1s���ٶ�
		sleep(1);
		
		len = read(pipefd[0], buf, 100);
		
		{
			printf("���˵�д���رգ�û�б���len :%d \n", len);
		}
		
	}

	wait(NULL);
	printf("parent ..quit\n");
	return 0;

}

void myhandle(int sig)
{
	printf("recv sig:%d \n", sig);
}

//��ʾ�ܵ����˵Ķ������رգ�д�����ᷢ��һ�������ź� SIGPIPE �ܵ��Ͽ��Ĵ����ź�
int main(void )
{
	int pipefd[2];
	pid_t pid;
	
	//ע��ܵ�������
	signal(SIGPIPE, myhandle);
	
	if (pipe(pipefd) == -1 )	
	{
		printf("pipe() err..\n");	
		return -1;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("fork err..\n");
		return -1;
	}
	
	if (pid == 0)
	{
		close(pipefd[0]);  //�ر��ӽ��̶���
		printf("child .....quit\n");
		exit(0);
	} 
	else if (pid > 0 )
	{
		int len = 0; 
		char buf[100] = {0};
		close(pipefd[0]); //�رո����̶���
		
		//������sleep 1s���ٶ�
		sleep(1);
		
		len = write(pipefd[1], "wangbaomingwangbaoming", 6); 
		//�����źŷ������첽֪ͨ�¼�����������֮�������������ʱ�����
		//�����źţ��Ϳ���֧�����ֻ���
		if (len == -1)
		{
			printf("���˵Ķ����رգ��ᷢ���źţ�Ȼ����ִ����仰 :%d \n", len);
		}
	}

	wait(NULL);
	printf("parent ..quit\n");
	return 0;

}
