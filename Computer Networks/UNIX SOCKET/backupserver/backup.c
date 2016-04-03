#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/un.h>
#include "sockunix.h"
#define SOCK_PATH "backupserver"
int fd[10];
int main()
{
	struct sockaddr_un server;
	int sfd,n=0;
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
		perror("");
	while(1)
	{
		fd[n]=recv_fd(sfd);
		if(fd[n]<0)
			break;
		printf("RECEVIED a fd");
		fflush(stdout);
		n++;
	}
}