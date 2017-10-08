#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

#define MSGMAX 8192
struct msgbuf {
	long mtype;       /* message type, must be > 0 */
	char mtext[MSGMAX];    /* message data */
};

//˼·��
//�ͻ��˷�����������Ϣ��������1
//�������˻ظ��ͻ��˵�type�ǶԷ����̺�
//�൱�ڷ������� ����Ϣ����������Ϣ��Ȼ��������˷����ͻظ��ͻ��ˣ�ͨ����Ϣ���У�


//n������ͨ����Ϣ���н��н���
//��û�в��������Ŀ���
//n���ͻ����������������Ϣ�������������ں���Ϣ���з�����Ϣ��������Ϣ�������ˣ����������亭ʱ��������������ɳ�������
//��ʹ����������������Ȼ����������

void echo_cli(int msgid)
{
	int n;
	int pid;
	pid = getpid();
	struct msgbuf msg;
	memset(&msg, 0, sizeof(msg));
	
	//��Ϣ�����ɣ��Լ���pid+��������
	*((int*)msg.mtext) = pid;
	//��Ϣ���� 1
	msg.mtype = 1;
	
	while (fgets(msg.mtext+4, MSGMAX, stdin) != NULL)
	{
		if (msgsnd(msgid, &msg, 4+strlen(msg.mtext+4), 0) < 0)
			ERR_EXIT("msgsnd");

		//ǰ�ĸ��ֽ����Լ���pid���Ͳ�����ˡ�
		memset(msg.mtext+4, 0, MSGMAX-4);
		if ((n = msgrcv(msgid, &msg, MSGMAX, pid, 0)) < 0)
                        ERR_EXIT("msgsnd");

		fputs(msg.mtext+4, stdout);
		memset(msg.mtext+4, 0, MSGMAX-4);
	}
}

int main(int argc, char *argv[])
{
	int msgid;
	msgid = msgget(0x1234, 0);
	if (msgid == -1)
		ERR_EXIT("msgget");

	echo_cli(msgid);
	return 0;
}
