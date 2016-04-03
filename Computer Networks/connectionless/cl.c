#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
char buf[1001];
int main(int arg,char * argc[])
{
	if(arg<4)
	{
		printf("EROOR");
		return 0;
	}
	int cfd,clilen;
	struct sockaddr_in server,client;
	struct hostent * S;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	int optval = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argc[1]));
	if(bind(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("BIND");
		return 0;
	}
	listen(sfd,5);
	int sfd2=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	 optval = 1;
    setsockopt(sfd2, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(atoi(argc[1]));
	if(bind(sfd2,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("BIND");
		return 0;
	}
	bzero(&server,sizeof(server));
	S=gethostbyname(argc[2]);
	if(S==NULL)
		printf("NO such server");
	server.sin_family=AF_INET;
	bcopy((char *)S->h_addr,(char *)&server.sin_addr.s_addr,S->h_length);
	server.sin_port=htons(atoi(argc[3]));
	if(connect(sfd2,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("FAILED");
	}
	//listen(sfd,5);
	perror("");
	printf("ESTABLISHED THE CONNECTION");
	fflush(stdout);
	while(1){
		clilen=sizeof(client);
		cfd=accept(sfd,(struct sockaddr *)&client,&clilen);
		if(cfd<0){
		perror("HELO");
		scanf("%s",&buf);
		}
		perror("SI+UCCESS");
		write(cfd,"NO",2);
		close(cfd);
	}
}