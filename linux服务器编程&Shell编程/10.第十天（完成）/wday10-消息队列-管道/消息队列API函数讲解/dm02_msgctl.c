#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>

//IPC���󡣡���Ϣ������linux�ں˸���־û������ǿ��Դ�linux�ں��л�ȡ ��ȡ��Ϣ���е���Ϣ���޸���Ϣ���У�
/*
struct msqid_ds {
               struct ipc_perm msg_perm;     // Ownership and permissions
               time_t         msg_stime;    // Time of last msgsnd() //
               time_t         msg_rtime;    // Time of last msgrcv() //
               time_t         msg_ctime;    // Time of last change //
               unsigned long  __msg_cbytes; // Current number of bytes in
                                               queue (non-standard) //
               msgqnum_t      msg_qnum;     // Current number of messages
                                               in queue //
               msglen_t       msg_qbytes;   // Maximum number of bytes
                                               allowed in queue //
               pid_t          msg_lspid;    // PID of last msgsnd() //
               pid_t          msg_lrpid;    // PID of last msgrcv() //
           };

       The  ipc_perm  structure  is  defined  in  <sys/ipc.h>  as follows (the highlighted fields are settable using
       IPC_SET):

           struct ipc_perm {
               key_t key;            // Key supplied to msgget() //
               uid_t uid;            // Effective UID of owner //
               gid_t gid;            // Effective GID of owner //
               uid_t cuid;           // Effective UID of creator //
               gid_t cgid;           // Effective GID of creator //
               unsigned short mode;  // Permissions //
               unsigned short seq;   // Sequence number //
           };
*/

 //    int msgctl(int msqid, int cmd, struct msqid_ds *buf);


int main21()
{
	int msgid;
	int ret = 0;
	
	//rwx
	msgid = msgget(0x1234, 0666);
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
	
	
	struct msqid_ds buf;
	memset(&buf, 0, sizeof( struct msqid_ds ));
	
	ret = msgctl(msgid, IPC_STAT, &buf);
	if (ret == -1)
	{
		perror("msgget err");
		return -1;
	}
	printf("Ȩ����Ϣ %o \n", buf.msg_perm.mode);
	printf("��ǰ��Ϣ�������ж����ֽ� %ld \n", buf.__msg_cbytes);
	printf("��ǰ��Ϣ���еĸ��� %d \n", buf.msg_qnum);
	
	
	printf("������Ϣ����...\n");
	
	return 0;
}



int main()
{
	int msgid;
	int ret = 0;
	
	//rwx
	msgid = msgget(0x1234, 0666);
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
	
	
	struct msqid_ds buf;
	memset(&buf, 0, sizeof( struct msqid_ds ));
	
	ret = msgctl(msgid, IPC_STAT, &buf);
	if (ret == -1)
	{
		perror("msgget err");
		return -1;
	}
	/*
	printf("Ȩ����Ϣ %o \n", buf.msg_perm.mode);
	printf("��ǰ��Ϣ�������ж����ֽ� %ld \n", buf.__msg_cbytes);
	printf("��ǰ��Ϣ���еĸ��� %d \n", buf.msg_qnum);
	*/
	
	buf.msg_perm.mode = 0644;
	ret = msgctl(msgid, IPC_SET, &buf);
	if (ret == -1)
	{
		perror("msgget err");
		return -1;
	}
	
	ret = msgctl(msgid, IPC_RMID, NULL);
	if (ret == -1)
	{
		perror("msgget err");
		return -1;
	}
	else
	{
		printf("ɾ���ɹ�\n");
	}
	
	
	
	//printf("������Ϣ����...\n");
	
	return 0;
}

