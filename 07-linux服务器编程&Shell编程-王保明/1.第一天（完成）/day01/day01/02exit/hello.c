#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <errno.h>
#include <signal.h>

////exit(0)��c�⺯�� =  _exit(0); //ϵͳ����
//\n������ˢ�»�����������
//


void bye1(void)
{
	printf("bye1...do\n");
}


void bye2(void)
{
	printf("bye2...do\n");
}
    
int main01()
{
	
	atexit(bye1);
	
	atexit(bye2);
	printf("hello....  fddddddddd1111");
	fflush(stdout);
	
	//exit(0)��c�⺯�� =======�����������������ڵ���_exit
	_exit(0); //ϵͳ����
}


    
int main()
{
	
	atexit(bye1);
	
	atexit(bye2);
	printf("hello....  fddddddddd1111\n");
	//abort();
	
	sleep(100);

	
	//exit(0); //��c�⺯�� =======�����������������ڵ���_exit
	return 0;
	
}






