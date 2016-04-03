#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/select.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <sys/un.h>
#include "sockunix.h"
#define SOCK_PATH "backupserver"
char buf[1000];
/*
void Process_packet(char buf[],int len)
{
	struct iphdr * ip_header;
	ip_header=(struct iphdr *)buf;
	printf("*******************RECEIVED IP PACKET***************\n");
	printf("VERSiON %d\n",(us)ip_header->version);
	printf("HEADER LENGTH %d \n",(us)ip_header->ihl);
	printf("TYPE OF SERVICE %d\n",(us)ip_header->tos);
	printf("IDENTIFICATION %d\n",ntohs(ip_header->id));
	printf("Total Length %d\n",ntohs(ip_header->tot_len));
	printf(" FRAGMENT OFFSET %d\n",ip_header->frag_off);
	printf("TIME TO LIVE %d \n",(us)ip_header->ttl);
	printf("PROTOCOL %d \n",(us)ip_header->protocol);
	printf("CHECKSUM %d\n",ntohs(ip_header->check));
	memset(&source,0,sizeof(source));
	memset(&dest,0,sizeof(dest));
	source.s_addr=(unsigned long)(ip_header->saddr);
	dest.s_addr=(unsigned long)(ip_header->daddr);
	char * temp=inet_ntoa(source);
	printf("SOURCE IP %s \n",temp);
	temp=inet_ntoa(dest);
	printf("DESTINATION IP %s \n",temp);
	printf("*****************************************************\n");
	//process_tcp_packet(buf+ip_header->ihl*4,len-ip_header->ihl*4);

}
*/
void PROCESS_PACK(char datagram[],int size)
{
	struct iphdr * iph=(struct iphdr *)datagram;
	int msg=ntohs(iph->tot_len)-iph->ihl*4,i;
	int off=iph->ihl*4;
	printf("%d %d\n",iph->tot_len,ntohs(iph->tot_len));
	for(i=0;i<msg;i++)
	{
		printf("%c",datagram[i+off]);
	}
	fflush(stdout);
}
pthread_mutex_t mutex;
void advertise(void * arg)
{
	char datagram[4096];
	int rawfd=*((int *)arg),a;
	while(1)
	{
		a=read(rawfd,datagram,sizeof(datagram));
		if(a<0)
		{

		}
		else
		{
			pthread_mutex_lock(&mutex);
			printf("***AD**\n");
		//	Process_packet(datagram,a);
			PROCESS_PACK(datagram,a);
			pthread_mutex_unlock(&mutex);
		}
	}
}
void announce_sock(void * arg)
{
	char datagram[4096];
	int rawfd=*((int *)arg),a;
	while(1)
	{
		a=read(rawfd,datagram,sizeof(datagram));
		if(a<0)
		{

		}
		else
		{
			pthread_mutex_lock(&mutex);
			printf("IMPORTANT ANNOUNCMENT TO MAKE\n");
			//Process_packet(datagram,a);
			PROCESS_PACK(datagram,a);
			pthread_mutex_unlock(&mutex);
		}
	}
}
int main(int arg,char * argc[])
{
	char buf[1000];
	int pid,a,tfd;
	pthread_mutex_init(&mutex,NULL);
		scanf("%d",&pid);
		pthread_t t1,t2;
		int rawfd=socket(AF_INET,SOCK_RAW,196);
		int rawfd2=socket(AF_INET,SOCK_RAW,195);
		if(rawfd<0)
		{
			perror("");
			return 0;
		}
		if(rawfd2<0)
		{
			perror("");
			return 0;
		}
		struct sockaddr_un server;
		int sfd=socket(AF_UNIX,SOCK_STREAM,0);
		server.sun_family=AF_UNIX;
		strcpy(server.sun_path,SOCK_PATH);
		while(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
			perror("");
		pthread_create(&t1,NULL,advertise,(void *)&rawfd);
		pthread_create(&t2,NULL,announce_sock,(void *)&rawfd2);
		while(1)
		{
			tfd=recv_fd(sfd);
			while(1)
			{
				a=read(tfd,buf,sizeof(buf));
				if(a<=0)
				{
					printf("TRAIN LEFT\n");
					kill(pid,SIGUSR1);
					break;
				}
				else
				{
					buf[a]='\0';
					printf("%s ",buf[a]);
				}
			}
		}		
}