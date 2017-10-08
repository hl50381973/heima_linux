#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "commsocket.h"

#include "myipc_sem.h"
#include "myipc_shm.h"

pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;


int g_key = 0x3333;

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)
        
        
#include <signal.h>

void handle(int signum)
{
	int pid = 0;
	printf("recv signum:%d \n", signum);
	
	//避免僵尸进程
	while ((pid = waitpid(-1, NULL, WNOHANG) ) > 0)
	{
		printf("退出子进程pid%d \n", pid);
		fflush(stdout);
	} 
}

int srvInit()
{
	//共享内存创建
	int		ret = 0;
	int 	shmhdl = 0;
	ret = IPC_CreatShm(".", sizeof(int), &shmhdl);
	if (ret != 0)
	{
		printf("func IPC_CreatShm() err:%d \n", ret);
		return ret;
	}
	
	
	//信号量的创建
 	int  	semid = 0;
	ret = sem_creat(g_key, &semid);
	if (ret != 0)
	{
		printf("func sem_creat() err:%d,重新按照open打开信号量 \n", ret);
		if (ret == SEMERR_EEXIST)
		{
			ret = sem_open(g_key, &semid);
			if (ret != 0)
			{
				printf("按照打开的方式，重新获取sem失败:%d \n", ret);
				return ret;
			}
		}
		else
		{
			return ret;
		}
		
	}
	
	int  val = 0;
	ret = sem_getval(semid, &val);
	if (ret != 0 )
	{
		printf("func sem_getval() err:%d \n", ret);
		return ret;
	}
	printf("sem val:%d\n", val);
	return 0;
	
}


void TestFunc_sem()
{
	int ncount = 0;
	int ret = 0;
	int shmhdl = 0;
	int *addr = NULL;
	
	int semid = 0;
	sem_open(g_key, &semid);


 	sem_p(semid); //临界区开始
 	
	//
		ret = IPC_CreatShm(".", 0, &shmhdl);
		
		ret =IPC_MapShm(shmhdl, (void **)&addr);
		*((int *)addr) =  *((int *)addr)  + 1;
		ncount = *((int *)addr);
		printf("ncount:%d\n", ncount);
		//addr[0] = addr[0] +1;
		ret =IPC_UnMapShm(addr);
		//sleep(2);
		
	sem_v(semid);  //临界区开始
	//
	//printf("进程正常退出:%d\n", getpid());
}	


void TestFunc_mutex()
{
	int ncount = 0;
	int ret = 0;
	int shmhdl = 0;
	int *addr = NULL;
	
	int semid = 0;
	sem_open(g_key, &semid);


 	//sem_p(semid); //临界区开始
 	pthread_mutex_lock(&mymutex);
 	
	//
		ret = IPC_CreatShm(".", 0, &shmhdl);
		
		ret =IPC_MapShm(shmhdl, (void **)&addr);
		*((int *)addr) =  *((int *)addr)  + 1;
		ncount = *((int *)addr);
		printf("ncount:%d\n", ncount);
		//addr[0] = addr[0] +1;
		ret =IPC_UnMapShm(addr);
		//sleep(2);
		
	//sem_v(semid);  //临界区开始
	pthread_mutex_unlock(&mymutex);
	//
	//printf("进程正常退出:%d\n", getpid());
}	


void *thread_routine(void* arg)
{
	int 	connfd = 0;
	int 	ret = 0;
	
	pthread_detach(pthread_self());
	
	if (arg == NULL)
	{
		return NULL;
	}
	connfd = *((int *)arg);
	free(arg);

	unsigned char 	recvbuf[1024];
	int 		recvbuflen = 1024;
	int 		wait_seconds = 5;
		
	
	while(1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		ret = sckServer_rev(connfd, recvbuf, &recvbuflen,  wait_seconds);
		if (ret != 0)
		{
			if (ret == Sck_ErrTimeOut)
			{
				continue;
			}
			printf("func sckServer_rev() err:%d \n", ret);
			break;
		}
		
		TestFunc_mutex();
		printf("recvbuf:%s\n", recvbuf);
		
		//
		ret = sckServer_send(connfd,  recvbuf, recvbuflen, wait_seconds);
		if (ret != 0)
		{
			printf("func sckServer_send() err:%d \n", ret);
			break;
		}
	}
	close(connfd);
	return NULL;	
}

int main(void)
{
	int		ret = 0;
	int 	listenfd;
	
	
	signal(SIGCHLD, handle);
	signal(SIGPIPE, SIG_IGN);
	
	//完成共享内存和信号量的创建
	srvInit();

	ret = sckServer_init(8001, &listenfd);
	if (ret != 0)
	{
		printf("sckServer_init() err:%d \n", ret);
		return ret;
	}
	
	while(1)
	{
		int ret = 0;
		int wait_seconds = 5;
		int connfd = 0;

		ret = sckServer_accept(listenfd, &connfd,  wait_seconds);
		if (ret == Sck_ErrTimeOut)
		{
			//printf("timeout....\n");
			continue;
		}
		int tid = 0;
		
		int *pCon = (int *)malloc(sizeof(int)) ; 
		*pCon = connfd;
		
		//每来一个连接，需要启动I个线程，线程需要收发报文 
		pthread_create(&tid, NULL, thread_routine, (void *)pCon);

	}
	

	return 0;
}
