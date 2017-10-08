#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
/*
1��Ϊ����1��pid�ͺ���2��pidȫ���ڸ�������
2 �����̰Ѻ���2��pid����������1

3������1���˺���2��pid������1�Ϳ��Ը�����2�����ź��ˡ�
*/


int pidArray[10];   //дʱ����copy-on-write ���ȫ������ֵ�ı䣬�ӽ������⿽��

void myhandle(int num,siginfo_t *st, void *p)
{
	int val = 0;
	//printf("recve num:%d,data=%d\n",num,st->si_value.sival_int);
	if (num == SIGRTMIN+1)
	{
		//����1���յ����ź�
		printf("����1�յ��źţ�ֵ:%d \n", st->si_value.sival_int);//�����̽��ӽ���2��pid������ݷ����ˣ��ӽ���1
		pidArray[1] = st->si_value.sival_int;// �ӽ���1���Ӹ������յ������ݷ��ڱ�ȫ����pidArray[1]��
	}
	
	if (num == SIGRTMIN+2)
	{
		//����2���յ����ź�
		printf("����2�յ��źţ�ֵ:%d \n", st->si_value.sival_int);//�ӽ���1���Լ���2*pidֵ�����ӽ���2
		pidArray[3] = st->si_value.sival_int;//�ӽ���2���ӽ���1���͵����ݴ浽��ȫ������
	}
	
	if (num == SIGRTMIN+3)
	{
		//�����̽��յ����ź�
		printf("�������յ��źţ�ֵ:%d \n", st->si_value.sival_int);//�ӽ���2���Լ��յ��������ַ���������
		printf("�������е�pid[0]:%d pid[1]:%d \n", pidArray[0], pidArray[1]);//����Լ�ȫ������ԭ���������
		pidArray[3] = st->si_value.sival_int;//�����̽����ӽ���2�յ������ݴ��뱾ȫ������
	}
}


int main()
{
	int i = 0;
	struct sigaction act;
	act.sa_sigaction=myhandle;
	act.sa_flags=SA_SIGINFO;
	  //ע���ź� ������
	if(sigaction(SIGRTMIN+1,&act,NULL)==-1)
	{
	  	perror("func sigaction err:");
	  	return -2;
	 }
		  //ע���ź� ������
	if(sigaction(SIGRTMIN+2,&act,NULL)==-1)
	{
	  	perror("func sigaction err:");
	  	return -2;
	 }
	 
	 	  //ע���ź� ������
	if(sigaction(SIGRTMIN+3,&act,NULL)==-1)
	{
	  	perror("func sigaction err:");
	  	return -2;
	 }
	 
	 int pid = 0;
	 for (i=0; i<2; i++)
	 {
	 	//�����̿��԰����еĺ���ID��������
 		pidArray[i]  = pid = fork();
		if (pid == 0)
		{
			break; //����Ǻ��ӣ�������fork
		}
		else if (pid > 0)
		{
			;
		}
 	}


	if (pid > 0)
	{
		printf("����������\n");
		//�����̸�����1�����ź� �ѽ���2��pid����ȥ
		  //��Ӷ�������
		  union sigval mysigval;
		  mysigval.sival_int=pidArray[1];

		  //������Ϣ
		  int res;
		  res = sigqueue(pidArray[0],SIGRTMIN+1, mysigval);

	}
	//�ӽ���1
	if (pid==0 && i==0)
	{
		printf("�ӽ���1����\n");
		printf("�ӽ���sleep\n");
		sleep(5);
		printf("�ӽ���sleep������Ժ�ȫ�ֱ�����ֵ��\n");
		
		  //��Ӷ�������
		  union sigval mysigval;
		  mysigval.sival_int=getpid()*2;

		  //������Ϣ
		  int res;
		  printf("�ӽ���1������2�����ź�getpid()*2:%d  \n", getpid()*2);
		  res = sigqueue(pidArray[1],SIGRTMIN+2, mysigval);
		
		exit(0);
	}
	
	
	//�ӽ���2
	if (pid==0 && i==1)
	{
		
		sleep(10);
		printf("�ӽ���2˯����������ʼ���ź�\n");
		
		 //��Ӷ�������
		  union sigval mysigval;
		  mysigval.sival_int=pidArray[3];

		  //������Ϣ
		  int res;
		  printf("�ӽ���2�������̷����ź�pidArray[3]%d  \n", pidArray[3]);
		  res = sigqueue(getppid(),SIGRTMIN+3, mysigval);
		
		exit(0);
	}
	
	sleep(2);
	int mypid = 0;
	while ( (mypid = waitpid(-1, NULL, WNOHANG)) > 0)
	{
		printf("�ӽ����˳�pid:%d \n", mypid);
	}
	
	return 0;

}

