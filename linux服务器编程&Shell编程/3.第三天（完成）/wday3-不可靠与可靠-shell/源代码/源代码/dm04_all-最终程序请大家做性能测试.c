
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

#define ERR_EXIT(m) \
	do \
	{ \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

/*

 int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

DESCRIPTION
       The sigaction() system call is used to change the action taken by a process on receipt of a specific signal.

       signum specifies the signal and can be any valid signal except SIGKILL and SIGSTOP.

       If act is non-null, the new action for signal signum is installed from act.  If oldact is non-null, the previous
       action is saved in oldact.

       The sigaction structure is defined as something like

              struct sigaction {
                  void (*sa_handler)(int);
                  void (*sa_sigaction)(int, siginfo_t *, void *);
                  sigset_t sa_mask;
                  int sa_flags;
                  void (*sa_restorer)(void);
              }
*/

/*

   int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

DESCRIPTION
       sigprocmask()  is  used  to  change the signal mask, the set of currently blocked signals.  The behaviour of the
       call is dependent on the value of how, as follows.

              SIG_BLOCK
                     The set of blocked signals is the union of the current set and the set argument.

              SIG_UNBLOCK
                     The signals in set are removed from the current set of blocked signals.  It is legal to attempt to
                     unblock a signal which is not blocked.

              SIG_SETMASK
                     The set of blocked signals is set to the argument set.

*/

/*
       #include <signal.h>

       int sigemptyset(sigset_t *set);

       int sigfillset(sigset_t *set);

       int sigaddset(sigset_t *set, int signum);

       int sigdelset(sigset_t *set, int signum);

       int sigismember(const sigset_t *set, int signum);
       */
       
       
      /*   
       int sigqueue(pid_t pid, int sig, const union sigval value);

DESCRIPTION
       sigqueue() sends the signal specified in sig to the process whose PID is given in pid.  The permissions required
       to send a signal are the same as for kill(2).  As with kill(2), the null signal (0) can be used to  check  if  a
       process with a given PID exists.

       The  value argument is used to specify an accompanying item of data (either an integer or a pointer value) to be
       sent with the signal, and has the following type:

         union sigval {
             int   sival_int;
             void *sival_ptr;
         };
*/

void myhandle(int num, siginfo_t *info , void * p)
{
		if (num == SIGUSR1)
		{
		//��SIGINT��SIGRTMIN����ӵ������̵�����״̬����
			sigset_t bset;
			sigemptyset(&bset);
			sigaddset(&bset, SIGINT);
			sigaddset(&bset, SIGRTMIN);
	
			sigprocmask(SIG_UNBLOCK, &bset, NULL);
			printf("������� recv sig num:%d \n", num); 
			
		}
		else if (num == SIGINT || num== SIGRTMIN)
		{	
			//printf("recv sig num:%d \n", num);
			printf("�յ��������ǣ�%d\n",info->si_value.sival_int);
		}
		else
		{
			printf("����recv sig num:%d \n",  num);
			printf("�յ��������ǣ�%d\n",info->si_value.sival_int);
		}
}

void  myhandle22(int num)
{
	if (num == SIGUSR1)
	{
		//��SIGINT��SIGRTMIN����ӵ������̵�����״̬����
		sigset_t bset;
		sigemptyset(&bset);
		sigaddset(&bset, SIGINT);
		sigaddset(&bset, SIGRTMIN);
		
		sigprocmask(SIG_UNBLOCK, &bset, NULL);
		printf("������� recv sig num:%d \n", num);
	}
	else if (num == SIGINT || num== SIGRTMIN)
	{	
		printf("recv sig num:%d \n", num);
	}
	else
		{
			printf("����recv sig num:%d \n",  num);
		}
}

void main()
{
	pid_t 	pid;
	
	struct sigaction act;
	act.sa_handler = myhandle;
	act.sa_flags = SA_SIGINFO;  //ǧ���������

	//ע���ʵʱ�ź� ������
	if ( sigaction(SIGINT, &act, NULL) <0 )
	{
		ERR_EXIT("sigaction SIGINT");
	}
	//ע��ʵʱ�źŵĴ�����
	if ( sigaction(SIGRTMIN, &act, NULL) <0 )
	{
		ERR_EXIT("sigaction SIGINT");
	}
	
	//ע����һ���û��Զ����ź�SIGUSR1 ������
	if ( sigaction(SIGUSR1, &act, NULL) <0 )
	{
		ERR_EXIT("sigaction SIGINT");
	}
	
	//��SIGINT��SIGRTMIN����ӵ������̵�����״̬����
	sigset_t bset;
	sigemptyset(&bset);
	sigaddset(&bset, SIGINT);
	sigaddset(&bset, SIGRTMIN);
	
	sigprocmask(SIG_BLOCK, &bset, NULL);
	
	pid = fork();
	if (pid == -1)
	{
		ERR_EXIT("fork err");
	}
	
	if (pid == 0)
	{
		int i = 0;
		int ret = 0;
		 union sigval v;
		 v.sival_int = 201;

		/*
		//�����β��ȶ��ź�
		for (i=0; i<3*1024; i++)
		{
			v.sival_int ++;
			ret = sigqueue(getppid(), SIGINT, v);
			if (ret != 0)
			{
				printf("���Ͳ��ɿ��ź�ʧ�� ret: %d, errno:%d \n", ret, errno);
				exit(0);
			}	
			else
			{
				printf("���Ͳ��ɿ��ź�ok\n");
			}
		}
		*/
	 
 		v.sival_int = 0;
		//�������ȶ��ź� 
		for (i=0; i<9*1024; i++) //8K
		{
			v.sival_int ++;
			ret = sigqueue(getppid(), SIGRTMIN, v);
			if (ret != 0)
			{
				//printf("���Ϳɿ��ź�ʧ�� ret: %d, errno:%d \n", ret, errno);
				//exit(0);
			}	
			//printf("���Ϳɿ��ź�ok i:%d\n", i);
		}
		
		sleep(2);
		kill(getppid(), SIGUSR1);
	}
	
	while(1)
	{
		sleep(1);
	}
	
	printf("main....\n");
}



