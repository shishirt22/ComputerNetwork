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
#include "sockunix.h"
#include <sys/un.h>
#define us unsigned int
#define SOCK_PATH "backupserver"
int clilen,i,j,is_free[3],pid[3];
char buf[1000];
void handler(int signum,siginfo_t * T,void * F)
{
	for(i=0;i<3;i++)
	{
		if(pid[i]==T->si_pid)
		{
			printf("PLATFROM %d is free\n",i+1);
			fflush(stdout);
			is_free[i]=1;
		}
	}
}
void send_message(int raw_fd,char message[],int protocol,char dest[])
{
	char datagram[4096];
	char * temp=(char *)malloc(strlen(message));
	strcpy(temp,message);
	memset(datagram,0,sizeof(datagram));
	struct iphdr * ip_header=(struct iphdr *)datagram;
	ip_header->version=4;
	ip_header->ihl=5;
	ip_header->tos=0;
	ip_header->id=0;
	ip_header->tot_len=htons(sizeof(struct iphdr)+strlen(message));
	printf("%d %d\n",ip_header->tot_len,sizeof(struct iphdr));
	ip_header->frag_off=0;
	ip_header->ttl=64;
	ip_header->protocol=(us)protocol;
	ip_header->check=0;
	ip_header->saddr=inet_addr("132.0.0.1");
	ip_header->daddr=inet_addr(dest);
	strcat(datagram+sizeof(struct iphdr),temp);
	struct sockaddr_in sender;
	memset(&sender,0,sizeof(sender));
	sender.sin_family=AF_INET;
	sender.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(sendto(raw_fd,datagram,sizeof(struct iphdr)+strlen(message),0,(struct sockaddr *)&sender,sizeof(sender))<0)
		{
			perror("SENDING FAILED\n");
		}
		else
			printf("SUCEESSS\n");
	
}
int main(int argn,char * argc[])
{
	if(argn<2)
	{
		printf("INAVALID ARGUMENT!!");
		return 0;
	}
	int ufd=socket(AF_UNIX,SOCK_STREAM,0),a;
	struct sockaddr_un server;
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	unlink(server.sun_path);
	if(bind(ufd,(struct sockaddr *)&server,sizeof(server)))
		perror("");
	listen(ufd,10);
	for(i=0;i<3;i++)
	{
		scanf("%d",&a);
		pid[i]=a;
		is_free[i]=1;
	}
	int pfd=socket(AF_INET,SOCK_RAW,195);
	int op=1;
	setsockopt(pfd,IPPROTO_IP,IP_HDRINCL,&op,sizeof(op));
	int portno=atoi(argc[1]);
	int tfd[3];
	struct sigaction sa;
	sa.sa_sigaction=handler;
	sa.sa_flags=SA_SIGINFO;
	sigaction(SIGUSR1,&sa,NULL);
	struct sockaddr_in T[3],client;
	for(i=0;i<3;i++)
	{
		tfd[i]=socket(AF_INET,SOCK_STREAM,0);
		bzero(&T[i],sizeof(T[i]));
		T[i].sin_family=AF_INET;
		T[i].sin_addr.s_addr=INADDR_ANY;
		T[i].sin_port=htons(portno);
		if(bind(tfd[i],(struct sockaddr *)&T[i],sizeof(T[i]))<0)
			perror("BIND ERROR");
		listen(tfd[i],5);
		printf("%d\n",portno);
		portno++;
	}
	struct timeval T1;
	fd_set rfds;
	printf("SERVER RUNNING\n");
	int flag=1,ret,station,mfds,cfd;
	while(1)
	{
		T1.tv_sec=0;
		T1.tv_usec=0;
		if(flag==1){
			mfds=-1;FD_ZERO(&rfds);
			for(i=0;i<3;i++)
			{
				FD_SET(tfd[i],&rfds);
				mfds=mfds>tfd[i]?mfds:tfd[i];
			}
		}
		ret=select(mfds+1,&rfds,NULL,NULL,&T1);
		if(ret>0)
		{
			//printf("INCOMING CONNECTIOn\n");
			flag=0;
			for(i=0;i<3;i++)
			{
				if(is_free[i])
				{
					flag=1;
					printf("FRee found\n");
					station=i;
					break;
				}
			}
			fflush(stdout);
			if(flag)
			{
				clilen=sizeof(client);
				for(i=0;i<3;i++)
				{
					if(FD_ISSET(tfd[i],&rfds))
					{
						printf("TFD found\n");
						is_free[station]=0;
						strcpy(buf,"");
						sprintf(buf,"TRAIN FROM terminal %d IS arriving  AT Platfrom %d",i+1,station+1);
						send_message(pfd,buf,195,"127.0.0.1");
						printf("%s\n",buf);
						cfd=accept(tfd[i],(struct sockaddr *)&client,&clilen);
						send_fd(ufd,cfd);
						close(cfd);
						for(j=i+1;j<3;j++)
						{
							if(FD_ISSET(tfd[j],&rfds))
							{
								flag=0;
								break;
							}
						}
						if(j==3)
						{
							flag=1;
						}
						break;
					}
				}
			}
		}
	}
}
