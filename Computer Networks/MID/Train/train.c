#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <netdb.h>
char buf[1000];
int main(int arg,char * argc[])
{
	if(arg<3)
	{
		printf("Invalid Argument\n");
		return 0;
	}
	struct sockaddr_in server;
	int cfd;
	cfd=socket(AF_INET,SOCK_STREAM,0);
	struct hostent * S;
	S=gethostbyname(argc[1]);
	if(S==NULL)
	{
		printf("NO such server\n");
		return 0;
	}
	int portno=atoi(argc[2]);
	printf("%d",portno);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	server.sin_port=htons(portno);
	if(connect(cfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("Connection Falied");
		return 0;
	}
	printf("Connection Established\n");
	int a=read(cfd,buf,sizeof(buf));
	if(a>0)
	{
		buf[a]='\0';
		portno=atoi(buf);
		close(cfd);
		cfd=socket(AF_INET,SOCK_STREAM,0);
		printf("Platform at port number %d\n",portno);
		bzero(&server,sizeof(server));
		server.sin_family=AF_INET;
		bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
		server.sin_port=htons(portno);
		if(connect(cfd,(struct sockaddr *)&server,sizeof(server)))
		{
			perror("Connection Failed with platfrom");
		}
		printf("Connection Established\nEnter train information");
		scanf("%s",&buf);
		write(cfd,buf,strlen(buf));
		sleep(10);
	}
	close(cfd);
	printf("Train is leaving\n");
}