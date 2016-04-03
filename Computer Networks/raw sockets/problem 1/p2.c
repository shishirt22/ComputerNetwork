#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
char datagram[4096];
int main()
{
	int raw_fd=socket(AF_INET,SOCK_RAW,198);
	perror("");
	memset(datagram,0,sizeof(datagram));
	int op=1;
	setsockopt(raw_fd,IPPROTO_IP,IP_HDRINCL,&op,sizeof(op));
	perror("");
	struct iphdr * ip_header=(struct iphdr *)datagram;
	ip_header->version=4;
	ip_header->ihl=5;
	ip_header->tos=0;
	ip_header->id=0;
	ip_header->tot_len=htons(sizeof(struct iphdr));
	ip_header->frag_off=sizeof(iphdr);
	ip_header->ttl=htons(64);
	ip_header->protocol=198;
	ip_header->check=0;
	ip_header->saddr=inet_addr("132.0.0.1");
	ip_header->daddr=inet_addr("127.0.0.1");
	struct sockaddr_in sender;
	memset(&sender,0,sizeof(sender));
	sender.sin_family=AF_INET;
	sender.sin_addr.s_addr=inet_addr("127.0.0.1");
	while(1){
		if(sendto(raw_fd,datagram,ip_header->tot_len,0,(struct sockaddr *)&sender,sizeof(sender))<0)
		{
			printf("SENDING FAILED\n");
		}
		else
		{
			printf("SUCCESS\n");
		}
		sleep(5);
	}
}