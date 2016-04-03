#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
char buf[100];
int main(int arg,char * argc[])
{
	struct sockaddr_in server,client;
	int sfd,clilen,a;;
	sfd=socket(AF_INET,SOCK_DGRAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argc[1]));
	if(bind(sfd,(struct sockaddr *)&server,sizeof(server)))
		perror("BIND");
	struct hostent * S=gethostbyname(argc[2]);
	perror("");
	bzero(&client,sizeof(client));
	client.sin_family=AF_INET;
	client.sin_port=htons(atoi(argc[3]));
	bcopy((char *)S->h_addr,(char *)&client.sin_addr.s_addr,S->h_length);
	clilen=sizeof(client);
	char * ch="kt1";
	a=sendto(sfd,ch,3,0,(struct sockaddr *)&client,clilen);
	perror("");
	clilen=sizeof(client);
	a=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr *)&client,&clilen);
	buf[a]='\0';
	printf("%s",buf);
	fflush(stdout);
	//close(sfd);
}