#include <sys/types.h>
#include <unistd.h>


#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

/*
       #include <signal.h>

       typedef void (*sighandler_t)(int);

       sighandler_t signal(int signum, sighandler_t handler);
*/


//�ź��첽֪ͨ�¼� �������������ҵ�Ӧ�ó���dm01��������ʱ���źŻ����жϡ�����
//�������д���źŴ�����handler
//��ô���źŷ���ʱ�����Զ�������д�ĺ�����������

void main21()
{
	//ע��һ���ź�
	//SIGINT  ��ctrl+c �����2���źš����� �ж�Ӧ�ó���
	//signal(SIGINT, handler);
	pid_t pid;
	//signal(SIGQUIT, handler);
	signal(SIGCHLD, SIG_IGN); //�����̺����ӽ��̵��˳�
	//signal(SIGKILL, SIG_IGN); //�����̺����ӽ��̵��˳�
	
	pid = fork();
	if (pid == -1)
	{
		perror("fork err");
		exit(0);
	}
	
	if (pid == 0)
	{
		printf("child.....\n");
		exit(0);
	}
	

	
	while(1)
	{
		pause();
	}
	
	printf("main...Begin\n");
		
}

void handler(int num)
{
	printf("recv num:%d \n", num);	
	if (num == SIGQUIT)
	{
		//exit(0);
	}
}

//��ô���źŷ���ʱ�����Զ�������д�ĺ�����������
void main22()
{
	char tmpchar;
	//ע��һ���ź�
	//SIGINT  ��ctrl+c �����2���źš����� �ж�Ӧ�ó���
	signal(SIGINT, handler);
	
	
	printf("��������a�ַ�����ô���ָ� SIGINT ����Ϊ\n");
	while( (tmpchar = getchar()) != 'a' )
	{
		pause();
	}
	//
	
	//������źŻָ�Ĭ����Ϊ SIG_DFL
	signal(SIGINT, SIG_DFL);
	while(1)
	{
		pause();
	}
	
	printf("main...end\n");
		
}


//���Ժ�������ֵ
void main23()
{
	char tmpchar;
	//ע��һ���ź�
	//SIGINT  ��ctrl+c �����2���źš����� �ж�Ӧ�ó���
	__sighandler_t old = signal(SIGINT, handler);
	if (SIG_ERR == old)
	{
		perror("signal err"); //errno
		
	}
	
	
	printf("��������a�ַ�����ô���ָ� SIGINT ����Ϊ\n");
	while( (tmpchar = getchar()) != 'a' )
	{
		pause();
	}
	//
	
	//������źŻָ�Ĭ����Ϊ SIG_DFL
	signal(SIGINT, old);
	while(1)
	{
		pause();
	}
	
	printf("main...end\n");
		
}

//���Ժ�������ֵ
void main()
{
	char tmpchar;
	//ע��һ���ź�
	//SIGINT  ��ctrl+c �����2���źš����� �ж�Ӧ�ó���
 	if (signal(SIGINT, handler) == SIG_ERR)
 	{
 			perror("signal err"); //errno
 			exit(0);
 	} 
	while(1)
	{
		pause();
	}
	
	printf("main...end\n");
		
}



