
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/msg.h>

struct msg_buf
{
    long mtype;
    char data[255];
};
 
/* ע��long �� int ��32bit �� 64bitϵͳ֮���ǲ�һ����
struct msg_buf
{
    long mtype;
    char data[255];
};
*/
 
int main()
{
    key_t key;
    int msgid;
    int ret;
    struct msg_buf msgbuf;
    int msgtype =  getpid();

    key=ftok("./msgfile",'a');
    printf("key =[%x]\n",key);
    
    printf("sizeof(long):%ld, sizeof(int):%d \n", sizeof(long), sizeof(int));
    
    msgid=msgget(key, IPC_CREAT |IPC_EXCL|0666); //ͨ���ļ���Ӧ

    if(msgid==-1)
    {
		if (errno == EEXIST)
		{
			printf("EEXIST:.....\n");
			key=ftok("./msgfile",'a');
			msgid=msgget(key, IPC_CREAT|0666); //ͨ���ļ���Ӧ
		}
		else
		{
		 	printf("create error\n");
			perror("msget: \n");
			return -1;
		}
		
    }
    printf("msgid:%d \n", msgid);

   	msgbuf.mtype = msgtype; //        getpid();

    printf("getpid(): %d \n", getpid());
    strcpy(msgbuf.data,"test haha");
    ret = msgsnd(msgid,&msgbuf, sizeof(msgbuf.data), IPC_NOWAIT);
    if(ret==-1)
    {
            printf("send message err\n");
            perror("senderr");
            return -1;
    }
    sleep(1);

    memset(&msgbuf,0,sizeof(msgbuf));
    
    ret=msgrcv(msgid, &msgbuf, sizeof(msgbuf.data), msgtype, IPC_NOWAIT);
    if(ret==-1)
    {
            printf("recv message err\n");
            perror("dd");
            return -1;
    }
    printf("recv msg =[%s]\n",msgbuf.data);
 
}
