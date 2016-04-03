#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
char buf[1000];
int main(int arg,char * argc[])
{
	if(arg<3)
	{
		printf("Invalid argument\n");
		return 0;
	}
	int portno,a;
	struct sockaddr_in server;
	struct hostent * S;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	S=gethostbyname(argc[1]);
	portno=atoi(argc[2]);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	server.sin_port=htons(portno);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("Connection Failed: ");
		return 0;
	}
	printf("Connection Established\n");
	while(1)
	{
		a=read(sfd,buf,sizeof(buf));
		if(a>0)
		{
			buf[a]='\0';
			printf("%s\n",buf);fflush(stdout);
		}
	}
}