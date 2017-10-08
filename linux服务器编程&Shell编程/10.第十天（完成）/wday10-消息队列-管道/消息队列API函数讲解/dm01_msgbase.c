#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>


////��ʾ����Ϣ���У������Ϣ���в����ڣ���ô����������
int main01()
{
	int msgid;
	
	//��ʾ���ļ�������ļ�����Ҫ����
	msgid = msgget(0x1234, 0666);
	if (msgid == -1)
	{
		if (errno == ENOENT)
		{
			printf("�����Լ���飬��������Ϣ���в�����\n");
		}
		perror("msgget err");
		return -1;
	}
	printf("hello...\n");
	
	return 0;
}


//��ʾ����Ϣ���ˣ������ڣ���ʹ�þ�
//���������򴴽� ��
int main02()
{
	int msgid;
	
	//��ʾ����Ϣ���ˣ������ڣ���ʹ�þ�
	//���������򴴽� 
	msgid = msgget(0x1234, 0666 | IPC_CREAT);
	if (msgid == -1)
	{
		if (errno == ENOENT)
		{
			printf("�����Լ���飬��������Ϣ���в�����\n");
		}
		perror("msgget err");
		return -1;
	}
	printf("���������ڴ�ɹ�...\n");
	
	return 0;
}



//��ʾ����Ϣ���ˣ������ڣ���ʹ�þ�
//���������򴴽� ��
int main03()
{
	int msgid;
	
	//��ʾ����Ϣ���ˣ������ڣ���ʹ�þ�
	//���������򴴽� 
	msgid = msgget(0x1234, 0666 | IPC_CREAT);
	if (msgid == -1)
	{
		if (errno == ENOENT)
		{
			printf("�����Լ���飬��������Ϣ���в�����\n");
		}
		perror("msgget err");
		return -1;
	}
	printf("msgid:%d \n", msgid);
	printf("���������ڴ�ɹ�...\n");
	
	return 0;
}


//��IPC_CREAT and IPC_EXCL  ��һ�𡣡�����
//���û����Ϣ���У��򴴽�
//���������Ϣ���У�����ʾ�Ѿ����ڡ�����
//���������򴴽� ��
//������IPC_EXCLû������
int main04()
{
	int msgid;
	
	//��ʾ����Ϣ���ˣ������ڣ���ʹ�þ�
	//���������򴴽� 
	msgid = msgget(0x1234, 0666 | IPC_CREAT | IPC_EXCL);
	if (msgid == -1)
	{
		if (errno == ENOENT)
		{
			printf("�����Լ���飬��������Ϣ���в�����\n");
		}
		if (errno == EEXIST)
		{
			printf("�����Լ���飬��������Ϣ���Ѿ�����\n");
		}
		
		perror("msgget err");
		return -1;
	}
	printf("msgid:%d \n", msgid);
	printf("���������ڴ�ɹ�...\n");
	
	return 0;
}

//IPC_PRIVATE ��������Ϣ���У�ֻ���Լ�������ʹ�ã�����û��ѪԵ��ϵ�Ľ��̼���
//������ʹ����IPC_PRIVATE��   IPC_CREAT | IPC_EXCL ��������
int main()
{
	int msgid;
	
	//��ʾ����Ϣ���ˣ������ڣ���ʹ�þ�
	//���������򴴽� 
	//IPC_PRIVATE ��ʾÿһ�δ�������Ϣ���в�һ����������������˵��˽�е�
	//����֮����˵�����������ҵ�msgid��ʹ���͸��������̣���������Ҳ���á���������ѪԵ��ϵfork,���⣩
	msgid = msgget(IPC_PRIVATE, 0666);
	if (msgid == -1)
	{
		if (errno == ENOENT)
		{
			printf("�����Լ���飬��������Ϣ���в�����\n");
		}
		if (errno == EEXIST)
		{
			printf("�����Լ���飬��������Ϣ���Ѿ�����\n");
		}
		
		perror("msgget err");
		return -1;
	}
	printf("msgid:%d \n", msgid);
	printf("���������ڴ�ɹ�...\n");
	
	return 0;
}