#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
char buf[1000];
int main(int arg,char * argc[])
{
	if(arg<3){
		printf("INVALID ARGUMENT");
	}
	struct sockaddr_in server;
	int clilen,sfd;
	int a;
	sfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&server,sizeof(server));
	struct hostent * S=gethostbyname(argc[1]);
	server.sin_port=htons(atoi(argc[2]));
	bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	server.sin_family=AF_INET;
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("CONNECTION FAILED\n");
		return 0;
	}
	printf("CONNECTION ESTABLISHED");
	a=read(sfd,buf,sizeof(buf));
	buf[a]='\0';
	printf("%s",buf);
	return 0;
}