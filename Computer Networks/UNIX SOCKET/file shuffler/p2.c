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
	int sfd=socket(AF_UNIX,SOCK_STREAM,0);
	int a,fd,i,a1;
	char ch[100];
	scanf("%d",&a);
	struct sockaddr_un server;
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
		{
			perror("FAILED");
			return 0;
		}
		int b;
	while(1)
	{
		a1=a;
		fd=recv_fd(sfd);
		if(fd<0)
			continue;
		printf("RECEIVIBF FD");
		fflush(stdout);
		while((b=read(fd,ch,a1))>0)
		{
			ch[b]='\0';
			printf("%s ",ch);
			a1=a1-b;
			if(a1<=0)
				break;
		}
		send_fd(sfd,fd);
		close(fd);
		sleep(10);
	}
}