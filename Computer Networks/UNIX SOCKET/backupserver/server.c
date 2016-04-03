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
int main()
{
	struct sockaddr_un server,client;
	char buf[100];
	int cfd,a,clilen,fd;
	int sfd=socket(AF_UNIX,SOCK_STREAM,0);
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	unlink(server.sun_path);
	if(bind(sfd,(struct sockaddr *)&server,sizeof(server)))
		perror("");
	listen(sfd,10);
	clilen=sizeof(client);
	cfd=accept(sfd,(struct sockaddr *)&client,&clilen);
	while(1)
	{
		scanf("%s",&buf);
		fd=open(buf,O_RDONLY);
		send_fd(cfd,fd);
		close(fd);
	}
}