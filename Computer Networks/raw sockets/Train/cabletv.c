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
#define us unsigned int
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
int main()
{
	char buf[1000];
	int rawfd=socket(AF_INET,SOCK_RAW,196);
	int op=1;
	setsockopt(rawfd,IPPROTO_IP,IP_HDRINCL,&op,sizeof(op));
	if(rawfd<0)
	{
		perror("");
		return 0;
	}
	while(1)
	{
		scanf("%s",buf);
		send_message(rawfd,buf,196,"127.0.0.1");
	}
}
