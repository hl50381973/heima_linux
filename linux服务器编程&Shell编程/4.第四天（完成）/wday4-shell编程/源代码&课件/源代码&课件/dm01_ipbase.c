
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
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*

       uint32_t htonl(uint32_t hostlong);

       uint16_t htons(uint16_t hostshort);

       uint32_t ntohl(uint32_t netlong);

       uint16_t ntohs(uint16_t netshort);
*/



       
void main11()
{
	unsigned int data = 0x12345678;
	char *p = &data;
	
	printf("%x, %x %x %x \n", p[0], p[1], p[2], p[3]);
	
	if ( p[0] == 0x78)
	{
			printf("��ǰϵͳ��С��ģʽ\n");
	}
	else
	{
		printf("��ǰϵͳ�Ǵ��ģʽ\n");
	}
	//x86ƽ̨��С��ģ��   ibm���� 8��� 
	
	printf("�ѱ����ֽ�ת���������ֽ�\n");
	 uint32_t mynetdat = htonl(data);
	 p = &mynetdat;
	 printf("%x, %x %x %x \n", p[0], p[1], p[2], p[3]);
	if ( p[0] == 0x78)
	{
			printf("�����ֽ�������С��ģʽ\n");
	}
	else
	{
		printf("�����ֽ������Ǵ��ģʽ\n");
	}

}

void main()
{
	/*
	  int inet_aton(const char *cp, struct in_addr *inp); //1

       in_addr_t inet_addr(const char *cp); //2

       in_addr_t inet_network(const char *cp);

       char *inet_ntoa(struct in_addr in); //4

       struct in_addr inet_makeaddr(int net, int host);

       in_addr_t inet_lnaof(struct in_addr in);

       in_addr_t inet_netof(struct in_addr in);
       */
       
       //struct in_addr *inp;
       
         int  myint = inet_addr("192.168.6.222"); //2
         printf("%u\n", myint);
         
         struct in_addr myaddr;
         
       
         inet_aton("192.168.6.222", &myaddr); //1
         printf("%u\n", myaddr.s_addr);
         
         
         
          //char *inet_ntoa(struct in_addr in); //4
          //������̵����Ϊʲô����ط� Ҫ�������Ԫ��
          printf("%s\n",inet_ntoa(myaddr));
         /*
             //Internet address. 
         struct in_addr {
             u_int32_t      s_addr;     // address in network byte order 
         };
         */

      
	
}

 