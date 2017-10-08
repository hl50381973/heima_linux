#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <signal.h>
#include <errno.h>
#include <signal.h>

/*
{
	"aaaa",
	"bbbb"
	null 0 \0
	}
*/


int main01(void )
{
	pid_t pid;
	int ret = 0;
	
	printf("befor fork pid:%d \n", getpid());
	int abc = 10;
	pid = vfork(); //errno
	if (pid == -1)	
	{
		//printf("pid < 0 err.\n");
		perror("tile");
		return -1;
	}
	if (pid > 0)
	{
		
		printf("parent: pid:%d \n", getpid());
		
	
	}
	else if (pid == 0)
	{
		printf("child: %d, parent: %d \n", getpid(), getppid());
		//printf("abc:%d\n", abc);
		
		//���Լ�дӦ�ó�����������
		//
	   	ret =  execve("./hello", NULL, NULL);
	   	if (ret == -1)
		{
			perror("execve:");
		}
	    printf("execve ������û��ִ��\n");
			
			exit(0);
		}

	printf("fork after....\n");
	return 0;
}


int main(void )
{
	pid_t pid;
	int ret = 0;
	
	printf("befor fork pid:%d \n", getpid());
	int abc = 10;
	pid = vfork(); //errno
	if (pid == -1)	
	{
		//printf("pid < 0 err.\n");
		perror("tile");
		return -1;
	}
	if (pid > 0)
	{
		printf("parent: pid:%d \n", getpid());
	
	}
	else if (pid == 0)
	{
		printf("child: %d, parent: %d \n", getpid(), getppid());
		//printf("abc:%d\n", abc);
		
		//���Լ�дӦ�ó�����������
		//ͨ��һ��Ӧ�ó���ִ������
		char *const argv[] = {"ls", "-l", NULL};
	   	ret =  execve("/bin/ls", argv, NULL);
	   	if (ret == -1)
		{
			perror("execve:");
		}
	    printf("execve ������û��ִ��\n");
			
			exit(0);
		}

	//printf("fork after....\n");
	return 0;
}








