#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#define us unsigned int
char datagram[4096];
struct in_addr source,dest;
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
int main()
{
	int raw_fd=socket(AF_INET,SOCK_RAW,198);
	perror("");
	int a;
	memset(datagram,0,sizeof(datagram));
	while(1)
	{
		a=read(raw_fd,datagram,sizeof(datagram));
		if(a<0)
		{

		}
		else
		{
			Process_packet(datagram,a);
		}
	}
}