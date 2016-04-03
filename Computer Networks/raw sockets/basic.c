#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#define us unsigned int
char buf[65536];
struct in_addr source,dest;
void printdata(char buf[],int size)
{
	int i;
	printf("DATA is ");
	for(i=0;i<size;i++)
		printf("%c",buf[i]);
	printf("\n");
}
void process_tcp_packet(char buf[],int size)
{
	struct tcphdr * tcph;
	tcph=(struct tcphdr *)buf;
	printf("********************TCP HEADER **********************\n");
	printf("Source Port %d\n",ntohs(tcph->source));
	printf("DESTINATION PORT %d\n",ntohs(tcph->dest));
	printf("SEQUENCE %d\n",ntohl(tcph->seq));
	printf(" ACKNOWLEDGEMENT NUMBER %d \n",ntohl(tcph->ack_seq));
	printf(" data Offset in bytes %d \n",tcph->doff*4);
	printf(" ACK %d\n",(us)tcph->ack);
	printf(" SYN %d\n",(us)tcph->syn);
	printf("WINDOW %d \n",ntohs(tcph->window));
	printf("Checksum %d \n",ntohs(tcph->check));
	printf("*********************************************************\n");
	printdata(buf+tcph->doff*4,size-tcph->doff*4);
}
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
	process_tcp_packet(buf+ip_header->ihl*4,len-ip_header->ihl*4);

}
int main()
{
	int raw_fd=socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
	perror("\n");
	int len;int i=0;
	while(1)
	{
		len=read(raw_fd,buf,sizeof(buf));
		if(len<0)
		{
			
		}
		else
		{
			printf("%d \n",i+1);
			Process_packet(buf,len);
			i++;
		}
	}
}