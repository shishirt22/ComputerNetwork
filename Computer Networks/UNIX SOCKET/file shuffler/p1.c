#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/un.h>
#include "sockunix.h"
#define SOCK_PATH "fileshuffler"
int main()
{
	int a,len,clilen,i,sfd,cfd,fd,a1;
	scanf("%d",&a);
	char ch[100];
	fd=open("lorem.txt",O_RDONLY);
	struct sockaddr_un server,client;
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	unlink(server.sun_path);
	len=sizeof(server.sun_family)+sizeof(server.sun_path);
	if(bind(sfd,(struct sockaddr *)&server,len))
	{
		perror("");
		return 0;
	}
	listen(sfd,5);
	clilen=sizeof(client);
	cfd=accept(sfd,(struct sockaddr *)&client,&clilen);
	if(cfd<0)
	{
		perror("");
		return 1;
	}
	int b;
	while(1)
	{
		a1=a;
		while((b=read(fd,ch,a1))>0)
		{
			ch[b]='\0';
			printf("%s ",ch);
			a1=a1-b;
			if(a1<=0)
				break;
		}
		send_fd(cfd,fd);
	printf("FD SENT");
	fflush(stdout);
		close(fd);
		fd=recv_fd(cfd);
	//	dup2(fd,0);
		sleep(10);
	}
}