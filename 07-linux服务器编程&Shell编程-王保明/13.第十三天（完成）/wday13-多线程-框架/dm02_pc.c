#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>

int g_Count = 0;

int 	nNum, nLoop;

//������
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//������������ʼ��
pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;

#define CUSTOM_COUNT 2
#define PRODUCT_COUNT 4


  // int pthread_mutex_lock(pthread_mutex_t *mutex);
  //     int pthread_mutex_trylock(pthread_mutex_t *mutex);
   //    int pthread_mutex_unlock(pthread_mutex_t *mutex);

/*
       int pthread_cond_timedwait(pthread_cond_t *restrict cond,
              pthread_mutex_t *restrict mutex,
              const struct timespec *restrict abstime);
       int pthread_cond_wait(pthread_cond_t *restrict cond,
              pthread_mutex_t *restrict mutex);
              */

//posix �߳̿�ĺ��� �߳̿�
void *consume(void* arg)
{
	
	int inum = 0;
	inum = (int)arg;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("consum:%d\n", inum);
		while (g_Count == 0)  //while �����Ժ���Ҫ�����ж� ����g_Count�Ƿ����㣬��������㣬�ٴ�wait
		{
			printf("consum:%d ��ʼ�ȴ�\n", inum);
			pthread_cond_wait(&my_condition, &mutex); //api������������ //pthread_cond_wait����
			printf("consum:%d ����\n", inum);
		}
	
		printf("consum:%d ���Ѳ�Ʒbegin\n", inum);
		g_Count--; //���Ѳ�Ʒ
		printf("consum:%d ���Ѳ�Ʒend\n", inum);
		
		pthread_mutex_unlock(&mutex);
	
		sleep(1);
	}

	pthread_exit(0);

} 

//�������߳�
//
void *produce(void* arg)
{
	int inum = 0;
	inum = (int)arg;
	
	while(1)
	{
	
	/*
		//��Ϊ�Ǻܶ������ߵ���produce��Ҫ����ȫ�ֱ���g_Count�����Լ��� 
		pthread_mutex_lock(&mutex);
		if (g_Count > 20)
		{
			printf("produce:%d ��Ʒ̫�࣬��Ҫ���ƣ�����\n", inum);
			pthread_mutex_unlock(&mutex);
			sleep(1);
			continue;
		}
		else
		{
			pthread_mutex_unlock(&mutex);
		}
		*/
	
		pthread_mutex_lock(&mutex);
		printf("��Ʒ����:%d\n", g_Count);
		printf("produce:%d ������Ʒbegin\n", inum);
		g_Count++;
		//ֻҪ��������һ����Ʒ���͸���������ȥ����
		printf("produce:%d ������Ʒend\n", inum);
		
		printf("produce:%d ������signal begin\n", inum);
		pthread_cond_signal(&my_condition); //֪ͨ���������ϵȴ����߳� 
		printf("produce:%d ������signal end\n", inum);
		
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
	
	pthread_exit(0);

} 

//���ۣ�return arg �� pthread_exit�����Ľ����������pthread_join �ӹ���
int main()
{
	int		i =0;
	pthread_t	tidArray[CUSTOM_COUNT+PRODUCT_COUNT+10];
	
	//�����������߳�
	for (i=0; i<CUSTOM_COUNT; i++)
	{
		pthread_create(&tidArray[i], NULL, consume, (void *)i);
	}
	
	sleep(1);
	//���������߳�
	for (i=0; i<PRODUCT_COUNT; i++)
	{
		pthread_create(&tidArray[i+CUSTOM_COUNT], NULL, produce, (void*)i);
	}
	
	
	
	for (i=0; i<CUSTOM_COUNT+PRODUCT_COUNT; i++)
	{
		pthread_join(tidArray[i], NULL); //�ȴ��߳̽���������
	}
	
	
	printf("����ҲҪ����1233\n");
	
	return 0;
}


