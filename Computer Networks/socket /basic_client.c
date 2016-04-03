#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
char buf[100];
int main(int arg,char * argc[])
{
	int n;
	int sfd,nsfd,portno;
	struct sockaddr_in serv;
	struct hostent * server;
	if(arg<3)
	{
		printf("INvalied argument\n");
		return 0;
	}
	sfd=socket(AF_INET,SOCK_STREAM,0);
	portno=atoi(argc[2]);
	server=gethostbyname(argc[1]);
	if(server==NULL)
	{
		printf("No such server\n");
	}
	bzero((char *)&serv,sizeof(serv));
	serv.sin_family=AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv.sin_addr.s_addr,server->h_length);
	serv.sin_port=htons(portno);
	if (connect(sfd,(struct sockaddr *)&serv,sizeof(serv)) < 0)
	{
		perror("COnnection failed");
		return 0;
	}
	printf("Connection Established\n"); 
	while(1)
	{
		scanf("%s",&buf);
		write(sfd,buf,strlen(buf));
		n=read(sfd,buf,sizeof(buf));
		if(n>0)
		{
			buf[n]='\0';
			printf("%s\n",buf);
		}
	}
}