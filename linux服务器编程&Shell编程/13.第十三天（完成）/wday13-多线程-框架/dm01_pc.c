#include <unistd.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <pthread.h>



int g_num = 0;

int 	nNum, nLoop;

//������
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//������������ʼ��
pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;

#define CUSTOM_COUNT 2
#define PRODUCT_COUNT 2


  // int pthread_mutex_lock(pthread_mutex_t *mutex);
  //     int pthread_mutex_trylock(pthread_mutex_t *mutex);
   //    int pthread_mutex_unlock(pthread_mutex_t *mutex);


//posix �߳̿�ĺ��� �߳̿�
void *consume(void* arg)
{
	int i = 0;
	
	printf("consum\n");
	pthread_exit(0);

} 

//�������߳�
void *produce(void* arg)
{
	int i = 0;
	printf("produce\n");

	pthread_exit(0);

} 

//���ۣ�return arg �� pthread_exit�����Ľ����������pthread_join �ӹ���
int main()
{
	int		i =0;
	int		tidArray[CUSTOM_COUNT+PRODUCT_COUNT];
	
	//�����������߳�
	for (i=0; i<CUSTOM_COUNT; i++)
	{
		pthread_create(&tidArray[i], NULL, consume, NULL);
	}
	
	//���������߳�
	for (i=0; i<PRODUCT_COUNT; i++)
	{
		pthread_create(&tidArray[i+CUSTOM_COUNT], NULL, produce, NULL);
	}
	
	
	
	for (i=0; i<CUSTOM_COUNT+PRODUCT_COUNT; i++)
	{
		pthread_join(tidArray[i], NULL); //�ȴ��߳̽���������
	}
	
	
	printf("����ҲҪ����1233\n");
	
	return 0;
}


