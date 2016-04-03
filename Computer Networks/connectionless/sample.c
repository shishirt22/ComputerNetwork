#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
char * buf,ch[100];
int main(int arg,char * argc[])
{
	int clilen,cfd,cfd1;
	buf=(char *)malloc(100*sizeof(char));
	struct sockaddr_in server,client,temp;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argc[1]));
	server.sin_addr.s_addr=INADDR_ANY;
	if(bind(sfd,(struct sockaddr *)&server,sizeof(server)))
			perror("BIND EROOR");
	listen(sfd,6);
	clilen=sizeof(client);
		cfd=accept(sfd,(struct sockaddr *)&client,&clilen);
		printf("CONNECTION");
	fflush(stdout);
	while(1)
	{
		clilen=sizeof(client);
		cfd1=accept(sfd,(struct sockaddr *)&client,&clilen);
		perror("CLIENT");
		clilen=sizeof(temp);
		if(getpeername(cfd,(struct sockaddr *)&temp,&clilen)==-1)
			perror("GET 403");
		buf=inet_ntoa(temp.sin_addr);
		strcat(buf,"|");
		sprintf(ch,"%d",ntohs(temp.sin_port));
		strcat(buf,ch);
		write(cfd1,buf,strlen(buf));
		printf("%s",buf);
		fflush(stdout);
		//close(cfd1);
	}

}