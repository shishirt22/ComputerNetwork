#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
char buf[1010];
int main(int arg,char * argc[])
{
	int  t,a,clilen,cfd;
	struct sockaddr_in server,client;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	struct hostent * S;
	S=gethostbyname(argc[1]);
		perror("");
	bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	server.sin_port=htons(atoi(argc[2]));
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
		perror("FAILED");
	printf("ESTABLISHED");
	read(sfd,buf,sizeof(buf));
	char * temp=strtok(buf,"|\t\n ");
	strcpy(argc[1],temp);
	temp=strtok(NULL,"|\t\n ");
	strcpy(argc[2],temp);
	printf("%s %s",argc[1],argc[2]);
	close(sfd);
	sfd=socket(AF_INET,SOCK_STREAM,0);
	S=gethostbyname(argc[1]);
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argc[2]));
	bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
		perror("FAILED");
	printf("ESTABLISHED");
	a=read(sfd,buf,sizeof(buf));
	buf[a]='\0';
	printf("%s\n",buf);

}