#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/un.h>
#include <pthread.h>
#include "sockunix.h"
#include "socket.h"
#define SOCK_PATH "serversock"
int temp[10];
void * READ(void * arg)
{
	int a;
	char buf[100];
	int fd=*((int *)arg);
	while(1)
	{
		while((a=read(fd,buf,sizeof(buf)))>0)
		{
			buf[a]='\0';
			strcat(buf,temp);
			write(fd,buf,strlen(buf));
		}
	}
}
int main()
{
	pthread_t thread[10];
	int n=0,fd[10];
	scanf("%s",&temp);
	struct sockaddr_un server;
int 	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	server.sun_family=AF_UNIX;
	strcpy(server.sun_path,SOCK_PATH);
	if(connect(sfd,(struct sockaddr *)&server,sizeof(server)))
	{
		perror("FAILED");
	}
	perror("");
	while(1)
	{
		fd[n]=recv_fd(sfd);
		printf("RECEIED");
		fflush(stdout);
		nonblock(fd[n]);
		pthread_create(&thread[n],NULL,READ,(void *)&fd[n]);
		n++;
	}
}
